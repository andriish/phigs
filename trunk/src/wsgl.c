/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2011 Surplus Users Ham Society
*
*   Open PHIGS is free software: you can redistribute it and/or modify
*   it under the terms of the GNU Lesser General Public License as published by
*   the Free Software Foundation, either version 2.1 of the License, or
*   (at your option) any later version.
*
*   Open PHIGS is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU Lesser General Public License for more details.
*
*   You should have received a copy of the GNU Lesser General Public License
*   along with Open PHIGS. If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>
#include <phigs/mat_utils.h>

static int glConfig[] = {
   GLX_DOUBLEBUFFER,
   GLX_RGBA,
   GLX_DEPTH_SIZE, 12,
   GLX_RED_SIZE, 1,
   GLX_GREEN_SIZE, 1,
   GLX_BLUE_SIZE, 1,
   None
};

static attribute_group my_attrs;
static Pmatrix3        total_tran, local_tran;
static Ws_view_entry  *curr_view_entry = 0;
static Pint            curr_view_index = -1;
static Ws             *curr_ws;

static void setMatrix(Pmatrix3 mat)
{
   int i, j;
   GLfloat m[16];
   GLfloat *mp = &m[0];

   for (i = 0; i < 4; i++) {
      for (j = 0; j < 4; j++) {
         *mp = mat[i][j];
         mp++;
      }
   }

   glLoadMatrixf(m);
}

static void setView(Ws *ws, Pint index)
{
   if (curr_view_index != index) {
      curr_view_entry = &ws->out_ws.model.b.views[index];
      curr_view_index = index;
   }

   glMatrixMode(GL_PROJECTION);
   setMatrix(curr_view_entry->vmm);

   glMatrixMode(GL_MODELVIEW);
   phg_mat_copy(total_tran, curr_view_entry->vom);
   setMatrix(total_tran);
}

static Colormap getSharableColormap(XVisualInfo *vi, Display *dpy)
{
  Status status;
  XStandardColormap *standardCmaps;
  Colormap cmap;
  int i, numCmaps;

  /* True color only */
  if (vi->class != TrueColor) {
    fprintf(stderr, "Only true color supported\n");
    exit(1);
  }

  /* If no standard colormap but TrueColor make an unshared one */
  status = XmuLookupStandardColormap(dpy, vi->screen, vi->visualid,
                vi->depth, XA_RGB_DEFAULT_MAP, False, True);
  if (status == 1) {
    status = XGetRGBColormaps(dpy, RootWindow(dpy, vi->screen),
                &standardCmaps, &numCmaps, XA_RGB_DEFAULT_MAP);

    if (status == 1)
      for (i = 0; i < numCmaps; i++)
        if (standardCmaps[i].visualid == vi->visualid) {
          cmap = standardCmaps[i].colormap;
          XFree(standardCmaps);
          return cmap;
        }
  }

  cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen),
                vi->visual, AllocNone);
  return cmap;
}

Ws* phg_wsgl_create(Phg_args_open_ws *args)
{
   Ws_handle wsh;

   wsh = malloc(sizeof(Ws));
   if (wsh == NULL)
      return NULL;

   memset(wsh, 0, sizeof(Ws));

   return wsh;
}

int phg_wsgl_open_window(Ws *ws)
{
   XVisualInfo *vi;
   Colormap cmap;
   XSetWindowAttributes wattr;
   int screen_num;

   ws->display = XOpenDisplay(NULL);
   if (ws->display == NULL) {
      fprintf(stderr, "Error - Unable to open display\n");
      return 0;
   }

  screen_num = DefaultScreen(ws->display);

  ws->ws_rect.x = 0;
  ws->ws_rect.y = 0;
  ws->ws_rect.width = DisplayWidth(ws->display, screen_num) / 2;
  ws->ws_rect.height = ws->ws_rect.width;

   if (!glXQueryExtension(ws->display, NULL, NULL)) {
      fprintf(stderr, "No Open GL support\n");
      return 0;
   }

   ws->has_double_buffer = TRUE;
   vi = glXChooseVisual(ws->display,
                        DefaultScreen(ws->display),
                        glConfig);
   if (vi == NULL) {
      vi = glXChooseVisual(ws->display,
                           DefaultScreen(ws->display),
                           &glConfig[1]);
      fprintf(stderr, "No Open GL with double buffer found\n");
      if (vi == NULL) {
         fprintf(stderr, "No Open GL capable visual found\n");
         return 0;
      }
      ws->has_double_buffer = FALSE;
   }

   cmap = getSharableColormap(vi, ws->display);

   ws->glx_context = glXCreateContext(ws->display, vi, NULL, True);
   if (ws->glx_context == NULL) {
      fprintf(stderr, "Unable to create Open GL context\n");
      return 0;
   }

   wattr.colormap = cmap;
   wattr.border_pixel = 0;
   wattr.event_mask = ExposureMask | StructureNotifyMask | KeyPressMask;

   ws->drawable_id = XCreateWindow(ws->display,
                                   RootWindow(ws->display, vi->screen),
                                   ws->ws_rect.x, ws->ws_rect.y,
                                   ws->ws_rect.width, ws->ws_rect.height,
                                   0, vi->depth,
                                   InputOutput, vi->visual,
                                   CWBorderPixel | CWColormap | CWEventMask,
                                   &wattr);

#ifdef DEBUG
   printf("wsgl: Make context current: %x\n", (unsigned int) ws->glx_context);
#endif

   glXMakeCurrent(ws->display, ws->drawable_id, ws->glx_context);
   glViewport(0,
              0,
              ws->ws_rect.width,
              ws->ws_rect.height);
   glEnable(GL_DEPTH_BUFFER);

   XMapWindow(ws->display, ws->drawable_id);

   return 1;
}

void phg_wsgl_release_window(Ws *ws)
{
   XCloseDisplay(ws->display);
}

void phg_wsgl_destroy(Ws *ws)
{
   free(ws);
}

void phg_wsgl_set_window(Ws *ws, Plimit3 *win)
{
#ifdef DEBUG
   printf("wsgl: set_window: Make context current: %x\n",
          (unsigned int) ws->glx_context);
#endif

   glXMakeCurrent(ws->display, ws->drawable_id, ws->glx_context);
}

void phg_wsgl_set_viewport(Ws *ws, Plimit3 *vp)
{
#ifdef DEBUG
   printf("wsgl: set_viewport: Make context current: %x\n",
          (unsigned int) ws->glx_context);
#endif

   glXMakeCurrent(ws->display, ws->drawable_id, ws->glx_context);
   glViewport(vp->x_min, vp->y_min, vp->x_max, vp->y_max);
}

void phg_wsgl_clear(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void phg_wsgl_flush(Ws *ws)
{
#ifdef DEBUG
   printf("wsgl: Make context current: %x\n", (unsigned int) ws->glx_context);
#endif

   glXMakeCurrent(ws->display, ws->drawable_id, ws->glx_context);
   if (ws->has_double_buffer)
      glXSwapBuffers(ws->display, ws->drawable_id);
   else
      glFlush();
}

void phg_wsgl_compute_ws_transform(Plimit3 *ws_win,
                                   Plimit3 *ws_vp,
                                   Ws_xform *ws_xform)
{
   Pfloat sx, sy, sz, sxy;

   sx = (ws_vp->x_max - ws_vp->x_min) / (ws_win->x_max - ws_win->x_min);
   sy = (ws_vp->y_max - ws_vp->y_min) / (ws_win->y_max - ws_win->y_min);
   sz = (ws_vp->z_max - ws_vp->z_min) / (ws_win->z_max - ws_win->z_min);

   sxy = (sx < sy) ? sx : sy;

   ws_xform->scale.x = ws_xform->scale.y = sxy;
   ws_xform->scale.z = sz;

   ws_xform->offset.x = ws_vp->x_min - (ws_win->x_min * sxy);
   ws_xform->offset.y = ws_vp->y_min - (ws_win->y_min * sxy);
   ws_xform->offset.z = ws_vp->z_min - (ws_win->z_min * sz);
}

void phg_wsgl_begin_rendering(Ws *ws)
{
   curr_ws = ws;
   memcpy(&my_attrs, phg_get_default_attr(), sizeof(attribute_group));
}

void phg_wsgl_end_rendering(void)
{
}

void phg_wsgl_render_element(El_handle el)
{
   switch (el->eltype) {
      case PELEM_LABEL:
      case PELEM_PICK_ID:
      break;

      case PELEM_INT_COLR_IND:
         my_attrs.int_bundle.colr_ind = PHG_INT(el);
      break;
      case PELEM_INT_STYLE:
         my_attrs.int_bundle.style = PHG_INT(el);
      break;

      case PELEM_EDGE_COLR_IND:
         my_attrs.edge_bundle.colr_ind = PHG_INT(el);
      break;
      case PELEM_EDGEWIDTH:
         my_attrs.edge_bundle.width = PHG_FLOAT(el);
      break;
      case PELEM_EDGETYPE:
         my_attrs.edge_bundle.type = PHG_INT(el);
      break;
      case PELEM_EDGE_FLAG:
         my_attrs.edge_bundle.flag = PHG_EDGE_FLAG(el);
      break;

      case PELEM_MARKER_COLR_IND:
         my_attrs.marker_bundle.colr_ind = PHG_INT(el);
      break;
      case PELEM_MARKER_SIZE:
         my_attrs.marker_bundle.size = PHG_FLOAT(el);
      break;
      case PELEM_MARKER_TYPE:
         my_attrs.marker_bundle.type = PHG_INT(el);
      break;

      case PELEM_TEXT_COLR_IND:
         my_attrs.text_bundle.colr_ind = PHG_INT(el);
      break;
      case PELEM_TEXT_FONT:
         my_attrs.text_bundle.font = PHG_INT(el);
      break;

      case PELEM_LINE_COLR_IND:
         my_attrs.line_bundle.colr_ind = PHG_INT(el);
      break;
      case PELEM_LINEWIDTH:
         my_attrs.line_bundle.width = PHG_FLOAT(el);
      break;
      case PELEM_LINETYPE:
         my_attrs.line_bundle.type = PHG_INT(el);
      break;

      case PELEM_FILL_AREA:
         phg_draw_fill_area (curr_ws, PHG_POINT_LIST(el), &my_attrs);
      break;
      case PELEM_POLYLINE:
         phg_draw_polyline (curr_ws, PHG_POINT_LIST(el), &my_attrs);
      break;
      case PELEM_POLYMARKER:
         phg_draw_polymarker (curr_ws, PHG_POINT_LIST(el), &my_attrs);
      break;

      case PELEM_FILL_AREA3:
         phg_draw_fill_area3 (curr_ws, PHG_POINT_LIST3(el), &my_attrs);
      break;
      case PELEM_POLYLINE3:
         phg_draw_polyline3 (curr_ws, PHG_POINT_LIST3(el), &my_attrs);
      break;
      case PELEM_POLYMARKER3:
         phg_draw_polymarker3 (curr_ws, PHG_POINT_LIST3(el), &my_attrs);
      break;

      case PELEM_LOCAL_MODEL_TRAN3:
         switch (PHG_LOCAL_TRAN3(el)->compose_type) {
            case PTYPE_PRECONCAT:
               phg_mat_mul(local_tran,
                           local_tran,
                           PHG_LOCAL_TRAN3(el)->matrix);
            break;
            case PTYPE_POSTCONCAT:
               phg_mat_mul(local_tran,
                           PHG_LOCAL_TRAN3(el)->matrix,
                           local_tran);
            break;
            case PTYPE_REPLACE:
            default:
               phg_mat_copy(local_tran, PHG_LOCAL_TRAN3(el)->matrix);
            break;
         }
         phg_mat_mul(total_tran, curr_view_entry->vom, local_tran);
         setMatrix(total_tran);
      break;

      case PELEM_VIEW_IND:
         setView(curr_ws, PHG_INT(el));
      break;

      default:
         css_print_eltype(el->eltype);
         printf(" not processed\n");
      break;
   }
}

