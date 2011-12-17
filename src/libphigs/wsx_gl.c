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

static Colormap wsx_gl_get_sharable_colormap(
   XVisualInfo *vi,
   Display *dpy
   );

/*******************************************************************************
 * wsx_gl_open_window
 *
 * DESCR:	Open render window for workstation
 * RETURNS:	Zero on succcess, non zero on error
 */

int wsx_gl_open_window(
   Ws *ws,
   Phg_args_open_ws *args
   )
{
   XVisualInfo *vi;
   Colormap cmap;
   XSetWindowAttributes wattr;
   int screen_num;
   Wsgl *wsgl = (Wsgl *) ws->render_context;

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

   cmap = wsx_gl_get_sharable_colormap(vi, ws->display);

   wsgl->glx_context = glXCreateContext(ws->display, vi, NULL, True);
   if (wsgl->glx_context == NULL) {
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

   XSetStandardProperties(ws->display,
                          ws->drawable_id,
                          args->window_name,
                          args->icon_name,
                          None,
                          arglist,
                          0,
                          NULL);

   return 1;
}

/*******************************************************************************
 * wsx_gl_release_window
 *
 * DESCR:	Close render window for workstation
 * RETURNS:	N/A
 */

void wsx_gl_release_window(
   Ws *ws
   )
{
   XDestroyWindow(ws->display, ws->drawable_id);
}

/*******************************************************************************
 * wsx_gl_flush
 *
 * DESCR:	Flush render window for workstation
 * RETURNS:	N/A
 */
void wsx_gl_flush(
   Ws *ws
   )
{
   XMapWindow(ws->display, ws->drawable_id);
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

