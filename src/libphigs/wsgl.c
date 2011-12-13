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
#include <phigs/private/wsglP.h>
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

static Wsgl wsgl;

static void phg_set_matrix(Pmatrix3 mat);
static void phg_set_view(Ws *ws, Pint index);
static Colormap phg_get_sharable_colormap(XVisualInfo *vi, Display *dpy);
static void phg_set_line_attr(Pline_bundle *attr);
static void phg_set_int_attr(Pint_bundle *attr);
static void phg_set_edge_attr(Pedge_bundle *attr);
static void phg_set_marker_attr(Pmarker_bundle *attr);
static void phg_draw_polymarker(Ppoint_list *point_list);
static void phg_draw_polymarker3(Ppoint_list3 *point_list);
static void phg_draw_polyline(Ppoint_list *point_list);
static void phg_draw_polyline3(Ppoint_list3 *point_list);
static void phg_draw_fill_area(Ppoint_list *point_list);
static void phg_draw_fill_area3(Ppoint_list3 *point_list);
#ifdef NOT_YET
static void phg_draw_text(Ppoint *pos, char *text);
#endif /*NOT_YET*/

/*******************************************************************************
 * phg_wsgl_create
 *
 * DESCR:	Create renderer for workstation
 * RETURNS:	Pointer to workstation or NULL
 */

Ws* phg_wsgl_create(Phg_args_open_ws *args)
{
   Ws_handle wsh;

   wsh = malloc(sizeof(Ws));
   if (wsh == NULL)
      return NULL;

   memset(wsh, 0, sizeof(Ws));

   return wsh;
}

/*******************************************************************************
 * phg_wsgl_open
 *
 * DESCR:	Open render window for workstation
 * RETURNS:	Zero on succcess, non zero on error
 */

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
   ws->current_colour_model = PMODEL_RGB;
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

   cmap = phg_get_sharable_colormap(vi, ws->display);

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

   return 1;
}

/*******************************************************************************
 * phg_wsgl_release_window
 *
 * DESCR:	Close render window for workstation
 * RETURNS:	N/A
 */

void phg_wsgl_release_window(Ws *ws)
{
   XDestroyWindow(ws->display, ws->drawable_id);
}

/*******************************************************************************
 * phg_wsgl_release_window
 *
 * DESCR:	Release render storage for workstation
 * RETURNS:	N/A
 */

void phg_wsgl_destroy(Ws *ws)
{
   free(ws);
}

/*******************************************************************************
 * phg_wsgl_set_window
 *
 * DESCR:	Set render window coordinates
 * RETURNS:	N/A
 */

void phg_wsgl_set_window(Ws *ws, Plimit3 *win)
{
   wsgl.win_changed = 1;
   memcpy(&wsgl.curr_win, win, sizeof(Plimit3));
}

/*******************************************************************************
 * phg_wsgl_set_viewport
 *
 * DESCR:	Set render window viewport
 * RETURNS:	N/A
 */

void phg_wsgl_set_viewport(Ws *ws, Plimit3 *vp)
{
   wsgl.vp_changed = 1;
   memcpy(&wsgl.curr_vp, vp, sizeof(Plimit3));
}

/*******************************************************************************
 * phg_wsgl_set_clear
 *
 * DESCR:	Clear render window
 * RETURNS:	N/A
 */

void phg_wsgl_clear(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*******************************************************************************
 * phg_wsgl_flush
 *
 * DESCR:	Flush settings to render window
 * RETURNS:	N/A
 */

void phg_wsgl_flush(Ws *ws)
{
   float w, h;

   glXMakeCurrent(ws->display, ws->drawable_id, ws->glx_context);
   wsgl.curr_ws = ws;
   wsgl.colr_table = ws->colr_table;

   if (wsgl.vp_changed)
   {
      wsgl.vp_changed = 0;
      XMoveResizeWindow(ws->display, ws->drawable_id,
                    (int)  wsgl.curr_vp.x_min,
                    (int)  wsgl.curr_vp.y_min,
                    (int) (wsgl.curr_vp.x_max - wsgl.curr_vp.x_min),
                    (int) (wsgl.curr_vp.y_max - wsgl.curr_vp.y_min));
   }

   if (wsgl.win_changed)
   {
      wsgl.win_changed = 0;

      w = ws->ws_rect.width;
      h = ws->ws_rect.height;

      glViewport((GLint)   (wsgl.curr_win.x_min * w),
                 (GLint)   (wsgl.curr_win.y_min * h),
                 (GLsizei) (wsgl.curr_win.x_max * w),
                 (GLsizei) (wsgl.curr_win.y_max * h));
   }

   XMapWindow(ws->display, ws->drawable_id);

   glEnable(GL_DEPTH_BUFFER);

   if (wsgl.curr_ws->has_double_buffer)
   {
      glXSwapBuffers(wsgl.curr_ws->display, wsgl.curr_ws->drawable_id);
   }
   else
   {
      glFlush();
   }
}

/*******************************************************************************
 * phg_wsgl_compute_ws_transform
 *
 * DESCR:	Compute workstation transform
 * RETURNS:	N/A
 */

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

/*******************************************************************************
 * phg_wsgl_begin_rendering
 *
 * DESCR:	Start a rendiering session for workstation
 * RETURNS:	N/A
 */

void phg_wsgl_begin_rendering(Ws *ws)
{
   memcpy(&wsgl.attrs, phg_get_default_attr(), sizeof(attribute_group));
}

/*******************************************************************************
 * phg_wsgl_end_rendering
 *
 * DESCR:	End a rendiering session
 * RETURNS:	N/A
 */

void phg_wsgl_end_rendering(void)
{
}

/*******************************************************************************
 * phg_wsgl_render_element
 *
 * DESCR:	Render element to current workstation rendering window
 * RETURNS:	N/A
 */

void phg_wsgl_render_element(El_handle el)
{
   switch (el->eltype) {
      case PELEM_LABEL:
      case PELEM_PICK_ID:
      break;

      case PELEM_INT_COLR_IND:
         wsgl.attrs.int_bundle.colr_ind = PHG_INT(el);
      break;
      case PELEM_INT_STYLE:
         wsgl.attrs.int_bundle.style = PHG_INT(el);
      break;

      case PELEM_EDGE_COLR_IND:
         wsgl.attrs.edge_bundle.colr_ind = PHG_INT(el);
      break;
      case PELEM_EDGEWIDTH:
         wsgl.attrs.edge_bundle.width = PHG_FLOAT(el);
      break;
      case PELEM_EDGETYPE:
         wsgl.attrs.edge_bundle.type = PHG_INT(el);
      break;
      case PELEM_EDGE_FLAG:
         wsgl.attrs.edge_bundle.flag = PHG_EDGE_FLAG(el);
      break;

      case PELEM_MARKER_COLR_IND:
         wsgl.attrs.marker_bundle.colr_ind = PHG_INT(el);
      break;
      case PELEM_MARKER_SIZE:
         wsgl.attrs.marker_bundle.size = PHG_FLOAT(el);
      break;
      case PELEM_MARKER_TYPE:
         wsgl.attrs.marker_bundle.type = PHG_INT(el);
      break;

      case PELEM_TEXT_COLR_IND:
         wsgl.attrs.text_bundle.colr_ind = PHG_INT(el);
      break;
      case PELEM_TEXT_FONT:
         wsgl.attrs.text_bundle.font = PHG_INT(el);
      break;

      case PELEM_LINE_COLR_IND:
         wsgl.attrs.line_bundle.colr_ind = PHG_INT(el);
      break;
      case PELEM_LINEWIDTH:
         wsgl.attrs.line_bundle.width = PHG_FLOAT(el);
      break;
      case PELEM_LINETYPE:
         wsgl.attrs.line_bundle.type = PHG_INT(el);
      break;

      case PELEM_FILL_AREA:
         phg_draw_fill_area(PHG_POINT_LIST(el));
      break;
      case PELEM_POLYLINE:
         phg_draw_polyline(PHG_POINT_LIST(el));
      break;
      case PELEM_POLYMARKER:
         phg_draw_polymarker(PHG_POINT_LIST(el));
      break;

      case PELEM_FILL_AREA3:
         phg_draw_fill_area3(PHG_POINT_LIST3(el));
      break;
      case PELEM_POLYLINE3:
         phg_draw_polyline3(PHG_POINT_LIST3(el));
      break;
      case PELEM_POLYMARKER3:
         phg_draw_polymarker3(PHG_POINT_LIST3(el));
      break;

      case PELEM_LOCAL_MODEL_TRAN3:
         switch (PHG_LOCAL_TRAN3(el)->compose_type) {
            case PTYPE_PRECONCAT:
               phg_mat_mul(wsgl.local_tran,
                           wsgl.local_tran,
                           PHG_LOCAL_TRAN3(el)->matrix);
            break;
            case PTYPE_POSTCONCAT:
               phg_mat_mul(wsgl.local_tran,
                           PHG_LOCAL_TRAN3(el)->matrix,
                           wsgl.local_tran);
            break;
            case PTYPE_REPLACE:
            default:
               phg_mat_copy(wsgl.local_tran, PHG_LOCAL_TRAN3(el)->matrix);
            break;
         }
         phg_mat_mul(wsgl.total_tran,
                     wsgl.curr_view_entry->vom,
                     wsgl.local_tran);
         phg_set_matrix(wsgl.total_tran);
      break;

      case PELEM_VIEW_IND:
         phg_set_view(wsgl.curr_ws, PHG_INT(el));
      break;

      default:
         css_print_eltype(el->eltype);
         printf(" not processed\n");
      break;
   }
}

/*******************************************************************************
 * phg_get_sharable_colormap
 *
 * DESCR:	Get sharable colormap
 * RETURNS:	Colormap
 */

static Colormap phg_get_sharable_colormap(XVisualInfo *vi, Display *dpy)
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

/*******************************************************************************
 * phg_set_matrix
 *
 * DESCR:	Setup matrix
 * RETURNS:	N/A
 */

static void phg_set_matrix(Pmatrix3 mat)
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

/*******************************************************************************
 * phg_set_view
 *
 * DESCR:	Setup view
 * RETURNS:	N/A
 */

static void phg_set_view(Ws *ws, Pint index)
{
   if (wsgl.curr_view_index != index) {
      wsgl.curr_view_entry = &ws->out_ws.model.b.views[index];
      wsgl.curr_view_index = index;
   }

   glMatrixMode(GL_PROJECTION);
   phg_set_matrix(wsgl.curr_view_entry->vmm);

   glMatrixMode(GL_MODELVIEW);
   phg_mat_copy(wsgl.total_tran, wsgl.curr_view_entry->vom);
   phg_set_matrix(wsgl.total_tran);
}

/*******************************************************************************
 * phg_set_line_attr
 *
 * DESCR:	Setup line attributes
 * RETURNS:	N/A
 */

static void phg_set_line_attr(Pline_bundle *attr)
{
   Pint index = attr->colr_ind;

   glColor3f(wsgl.colr_table[index].val.general.x,
             wsgl.colr_table[index].val.general.y,
             wsgl.colr_table[index].val.general.z);

   /* Line style */
   switch (attr->type) {
      case PLINE_DASH:
         glLineStipple(1, 0x00ff);
         glEnable(GL_LINE_STIPPLE);
      break;

      case PLINE_DOT:
         glLineStipple(1, 0x0101);
         glEnable(GL_LINE_STIPPLE);
      break;

      case PLINE_DASH_DOT:
         glLineStipple(1, 0x1c47);
         glEnable(GL_LINE_STIPPLE);
      break;

      default:
         glDisable(GL_LINE_STIPPLE);
      break;
   }

   /* Line width */
   glLineWidth(attr->width);
}

/*******************************************************************************
 * phg_set_int_attr
 *
 * DESCR:	Setup interiour attributes
 * RETURNS:	N/A
 */

static void phg_set_int_attr(Pint_bundle *attr)
{
   Pint index = attr->colr_ind;

   glColor3f(wsgl.colr_table[index].val.general.x,
             wsgl.colr_table[index].val.general.y,
             wsgl.colr_table[index].val.general.z);
}

/*******************************************************************************
 * phg_set_edge_attr
 *
 * DESCR:	Setup interiour attributes
 * RETURNS:	N/A
 */

static void phg_set_edge_attr(Pedge_bundle *attr)
{
   Pint index = attr->colr_ind;

   glColor3f(wsgl.colr_table[index].val.general.x,
             wsgl.colr_table[index].val.general.y,
             wsgl.colr_table[index].val.general.z);
   glLineWidth(attr->width);
}

/*******************************************************************************
 * phg_set_marker_attr
 *
 * DESCR:	Setup marker attributes
 * RETURNS:	N/A
 */

static void phg_set_marker_attr(Pmarker_bundle *attr)
{
   Pint index = attr->colr_ind;

   glColor3f(wsgl.colr_table[index].val.general.x,
             wsgl.colr_table[index].val.general.y,
             wsgl.colr_table[index].val.general.z);
}

/*******************************************************************************
 * phg_draw_marker_dot
 *
 * DESCR:	Draw marker dots
 * RETURNS:	N/A
 */

static void phg_draw_marker_dot(
   Ppoint_list *point_list,
   Pfloat scale
   )
{
   int i;

   glPointSize(scale);
   glBegin(GL_POINTS);
   for (i = 0; i < point_list->num_points; i++)
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y);
   glEnd();
}

/*******************************************************************************
 * phg_draw_marker_plus
 *
 * DESCR:	Draw marker pluses
 * RETURNS:	N/A
 */

static void phg_draw_marker_plus(
   Ppoint_list *point_list,
   Pfloat scale
   )
{
   int i;
   float half_scale;

   half_scale = scale / 2.0;

   glLineWidth(1.0);
   glDisable(GL_LINE_STIPPLE);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++) {
      glVertex2f(point_list->points[i].x - half_scale,
                 point_list->points[i].y);
      glVertex2f(point_list->points[i].x + half_scale,
                 point_list->points[i].y);
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y - half_scale);
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y + half_scale);
   }
   glEnd();
}

/*******************************************************************************
 * phg_draw_marker_asterisk
 *
 * DESCR:	Draw marker asterisks
 * RETURNS:	N/A
 */

static void phg_draw_marker_asterisk(
   Ppoint_list *point_list,
   Pfloat scale
   )
{
   int i;
   float half_scale, small_scale;

   half_scale = scale / 2.0;
   small_scale = half_scale / 1.414;

   glLineWidth(1.0);
   glDisable(GL_LINE_STIPPLE);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++) {
      glVertex2f(point_list->points[i].x - half_scale,
                 point_list->points[i].y);
      glVertex2f(point_list->points[i].x + half_scale,
                 point_list->points[i].y);
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y - half_scale);
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y + half_scale);

      glVertex2f(point_list->points[i].x - small_scale,
                 point_list->points[i].y + small_scale);
      glVertex2f(point_list->points[i].x + small_scale,
                 point_list->points[i].y - small_scale);
      glVertex2f(point_list->points[i].x - small_scale,
                 point_list->points[i].y - small_scale);
      glVertex2f(point_list->points[i].x + small_scale,
                 point_list->points[i].y + small_scale);
   }
   glEnd();
}

/*******************************************************************************
 * phg_draw_marker_cross
 *
 * DESCR:	Draw marker crosses
 * RETURNS:	N/A
 */

static void phg_draw_marker_cross(
   Ppoint_list *point_list,
   Pfloat scale
   )
{
   int i;
   float half_scale;

   half_scale = scale / 2.0;

   glLineWidth(1.0);
   glDisable(GL_LINE_STIPPLE);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++) {
      glVertex2f(point_list->points[i].x - half_scale,
                 point_list->points[i].y + half_scale);
      glVertex2f(point_list->points[i].x + half_scale,
                 point_list->points[i].y - half_scale);
      glVertex2f(point_list->points[i].x - half_scale,
                 point_list->points[i].y - half_scale);
      glVertex2f(point_list->points[i].x + half_scale,
                 point_list->points[i].y + half_scale);
   }
   glEnd();
}

/*******************************************************************************
 * phg_draw_polymarker
 *
 * DESCR:	Draw markers
 * RETURNS:	N/A
 */

static void phg_draw_polymarker(Ppoint_list *point_list)
{
   phg_set_marker_attr(&wsgl.attrs.marker_bundle);
   switch (wsgl.attrs.marker_bundle.type) {
      case PMARKER_DOT:
         phg_draw_marker_dot(point_list, wsgl.attrs.marker_bundle.size);
      break;

      case PMARKER_PLUS:
         phg_draw_marker_plus(point_list, wsgl.attrs.marker_bundle.size);
      break;

      case PMARKER_ASTERISK:
         phg_draw_marker_asterisk(point_list, wsgl.attrs.marker_bundle.size);
      break;

      case PMARKER_CROSS:
         phg_draw_marker_cross(point_list, wsgl.attrs.marker_bundle.size);
      break;
   }
}

/*******************************************************************************
 * phg_draw_polymarker3
 *
 * DESCR:	Draw markers 3D
 * RETURNS:	N/A
 */

static void phg_draw_polymarker3(Ppoint_list3 *point_list)
{
   int i;
   Ppoint_list plist;
   Ppoint pts[10];

   plist.num_points = point_list->num_points;
   plist.points = pts;
   for (i = 0; i < point_list->num_points; i++) {
      pts[i].x = point_list->points[i].x;
      pts[i].y = point_list->points[i].y;
   }

   phg_set_marker_attr(&wsgl.attrs.marker_bundle);
   switch (wsgl.attrs.marker_bundle.type) {
      case PMARKER_DOT:
         phg_draw_marker_dot(&plist, wsgl.attrs.marker_bundle.size);
      break;

      case PMARKER_PLUS:
         phg_draw_marker_plus(&plist, wsgl.attrs.marker_bundle.size);
      break;

      case PMARKER_ASTERISK:
         phg_draw_marker_asterisk(&plist, wsgl.attrs.marker_bundle.size);
      break;

      case PMARKER_CROSS:
         phg_draw_marker_cross(&plist, wsgl.attrs.marker_bundle.size);
      break;
   }
}

/*******************************************************************************
 * phg_draw_polyline
 *
 * DESCR:	Draw lines
 * RETURNS:	N/A
 */

static void phg_draw_polyline(Ppoint_list *point_list)
{
   int i;

   phg_set_line_attr(&wsgl.attrs.line_bundle);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++)
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y);
   glEnd();
}

/*******************************************************************************
 * phg_draw_polyline3
 *
 * DESCR:	Draw lines 3D
 * RETURNS:	N/A
 */

static void phg_draw_polyline3(Ppoint_list3 *point_list)
{
   int i;

   phg_set_line_attr(&wsgl.attrs.line_bundle);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++)
      glVertex3f(point_list->points[i].x,
                 point_list->points[i].y,
                 point_list->points[i].z);
   glEnd();
}

/*******************************************************************************
 * phg_draw_fill_area
 *
 * DESCR:	Draw fill area
 * RETURNS:	N/A
 */

static void phg_draw_fill_area(Ppoint_list *point_list)
{
   int i;

   if (wsgl.attrs.int_bundle.style == PSTYLE_SOLID) {
      phg_set_int_attr(&wsgl.attrs.int_bundle);
      glBegin(GL_POLYGON);
      for (i = 0; i < point_list->num_points; i++)
         glVertex2f(point_list->points[i].x,
                    point_list->points[i].y);
      glEnd();
   }

   if (wsgl.attrs.edge_bundle.flag == PEDGE_ON) {
      phg_set_edge_attr(&wsgl.attrs.edge_bundle);
      glBegin(GL_LINE_LOOP);
      for (i = 0; i < point_list->num_points; i++)
         glVertex2f(point_list->points[i].x,
                    point_list->points[i].y);
      glEnd();
   }
}

/*******************************************************************************
 * phg_draw_fill_area3
 *
 * DESCR:	Draw fill area 3D
 * RETURNS:	N/A
 */

static void phg_draw_fill_area3(Ppoint_list3 *point_list)
{
   int i;

   if (wsgl.attrs.int_bundle.style == PSTYLE_SOLID) {
      phg_set_int_attr(&wsgl.attrs.int_bundle);
      glBegin(GL_POLYGON);
      for (i = 0; i < point_list->num_points; i++)
         glVertex3f(point_list->points[i].x,
                    point_list->points[i].y,
                    point_list->points[i].z);
      glEnd();
   }

   if (wsgl.attrs.edge_bundle.flag == PEDGE_ON) {
      phg_set_edge_attr(&wsgl.attrs.edge_bundle);
      glBegin(GL_LINE_LOOP);
      for (i = 0; i < point_list->num_points; i++)
         glVertex3f(point_list->points[i].x,
                    point_list->points[i].y,
                    point_list->points[i].z);
      glEnd();
   }
}

#ifdef NOT_YET
/*******************************************************************************
 * phg_draw_text
 *
 * DESCR:	Draw text
 * RETURNS:	N/A
 */

static void phg_draw_text(Ppoint *pos, char *text)
{
   printf("Draw text: ");
   printf("%f %f: %s\n", pos->x, pos->y, text);
}
#endif /* NOT_YET */

