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

static char *arglist[] = {""};

static int wsx_gl_open_window(
   Ws *ws,
   Phg_args_open_ws *args
   );

static void wsx_gl_resize_window(
   Ws *ws,
   int w,
   int h
   );

static void wsx_gl_release_window(
   Ws *ws
   );

static Colormap wsx_gl_get_sharable_colormap(
   XVisualInfo *vi,
   Display *dpy
   );

/*******************************************************************************
 * wsx_gl_create
 *
 * DESCR:	Create workstation type
 * RETURNS:	Pointer to workstation type or NULL
 */

Wst* wsx_gl_create(
   void
   )
{
   Wst *wst;

   wst = phg_wst_create(WST_BASE_TYPE_GLX_DRAWABLE);
   if (wst != NULL) {
      if (!wsx_gl_init(wst)) {
         wst = NULL;
      }
   }

   return wst;
}

/*******************************************************************************
 * wsx_gl_init
 *
 * DESCR:	Initialize workstation type
 * RETURNS:	TRUE or FALSE
 */

int wsx_gl_init(
   Wst *wst
   )
{
   Display *display;
   int screen_num;
   Wst_phigs_dt *dt;

   display = XOpenDisplay(NULL);
   if (display == NULL) {
      fprintf(stderr, "Error - Unable to open display\n");
      return FALSE;
   }

   screen_num = DefaultScreen(display);

   dt = &wst->desc_tbl.phigs_dt;

   dt->ws_category = PCAT_OUT;
   dt->dev_coords[0] = (float) DisplayWidth(display, screen_num);
   dt->dev_coords[1] = (float) DisplayHeight(display, screen_num);
   dt->dev_coords[2] = 1.0;

   XCloseDisplay(display);

   dt->num_hlhsr_modes = 2;
   dt->hlhsr_modes = (Pint *) malloc(sizeof(Pint) * dt->num_hlhsr_modes);
   if (dt->hlhsr_modes == NULL) {
      return FALSE;
   }
   dt->hlhsr_modes[0] = PHIGS_HLHSR_MODE_NONE;
   dt->hlhsr_modes[1] = PHIGS_HLHSR_MODE_ZBUFF;

#ifdef DEBUG
   printf("Added wsx_gl with coords: %f %f %f\n",
      wst->desc_tbl.phigs_dt.dev_coords[0],
      wst->desc_tbl.phigs_dt.dev_coords[1],
      wst->desc_tbl.phigs_dt.dev_coords[2]);
#endif

   wst->open_window = wsx_gl_open_window;
   wst->resize_window = wsx_gl_resize_window;
   wst->release_window = wsx_gl_release_window;

   return TRUE;
}

/*******************************************************************************
 * wsx_gl_open_window
 *
 * DESCR:	Open render window for workstation
 * RETURNS:	TRUE or FALSE
 */

static int wsx_gl_open_window(
   Ws *ws,
   Phg_args_open_ws *args
   )
{
   XVisualInfo *vi;
   Colormap cmap;
   XSetWindowAttributes wattr;
   Wst_phigs_dt *dt;
   Wsgl *wsgl;

   wsgl = ws->render_context;
   dt = &args->type->desc_tbl.phigs_dt;

#ifdef DEBUG
   printf("Open workstation window:\n"
          "wsid:                   %d\n"
          "window name:            %s\n"
          "icon name:              %s\n",
          args->wsid,
          args->window_name,
          args->icon_name);
#endif

   ws->display = XOpenDisplay(NULL);
   if (ws->display == NULL) {
      fprintf(stderr, "Error - Unable to open display\n");
      return 0;
   }

  ws->ws_rect.x      = 0;
  ws->ws_rect.y      = 0;
  ws->ws_rect.width  = (int) dt->dev_coords[0] / 2;
  ws->ws_rect.height = ws->ws_rect.width;

#ifdef DEBUG
   printf("Window dimensions: %d %d\n"
          "                   %d %d\n",
          ws->ws_rect.x,
          ws->ws_rect.y,
          ws->ws_rect.width,
          ws->ws_rect.height);
#endif

   if (!glXQueryExtension(ws->display, NULL, NULL)) {
      fprintf(stderr, "No Open GL support\n");
      return FALSE;
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
         return FALSE;
      }
      ws->has_double_buffer = FALSE;
   }

   cmap = wsx_gl_get_sharable_colormap(vi, ws->display);

   wsgl->glx_context = glXCreateContext(ws->display, vi, NULL, True);
   if (wsgl->glx_context == NULL) {
      fprintf(stderr, "Unable to create Open GL context\n");
      return FALSE;
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

   XSetStandardProperties(ws->display,
                          ws->drawable_id,
                          args->window_name,
                          args->icon_name,
                          None,
                          arglist,
                          0,
                          NULL);

   XMapWindow(ws->display, ws->drawable_id);

#ifdef DEBUG
   printf("Opened GLX workstation window: %x\n", (unsigned) ws->drawable_id);
#endif

   return TRUE;
}

/*******************************************************************************
 * wsx_gl_resize_window
 *
 * DESCR:	Resize render window for workstation
 * RETURNS:	N/A
 */

static void wsx_gl_resize_window(
   Ws *ws,
   int w,
   int h
   )
{
   XResizeWindow(ws->display,
                 ws->drawable_id,
                 w,
                 h);
}

/*******************************************************************************
 * wsx_gl_release_window
 *
 * DESCR:	Close render window for workstation
 * RETURNS:	N/A
 */

static void wsx_gl_release_window(
   Ws *ws
   )
{
   XDestroyWindow(ws->display, ws->drawable_id);
}

/*******************************************************************************
 * wsx_gl_get_sharable_colormap
 *
 * DESCR:	Get sharable colormap
 * RETURNS:	Colormap
 */

static Colormap wsx_gl_get_sharable_colormap(
   XVisualInfo *vi,
   Display *dpy
   )
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

