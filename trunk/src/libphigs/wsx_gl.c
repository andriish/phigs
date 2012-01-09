/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2011 - 2012 Surplus Users Ham Society
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

static int glConfig[] = {
   GLX_DOUBLEBUFFER,
   GLX_RGBA,
   GLX_DEPTH_SIZE, 16,
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

/*******************************************************************************
 * init_output_ws_dt
 *
 * DESCR:	Helper initialization function for output ws dt
 * RETURNS:	N/A
 */

static void init_output_ws_dt(
   Wst_output_wsdt *wsdt
   )
{
   Pgcolr fg;

   wsdt->ws_class             = PCLASS_RASTER;
   wsdt->deferral_mode        = PDEFER_ASAP;
   wsdt->modification_mode    = PMODE_UQUM;
   wsdt->default_colour_model = PMODEL_RGB;

   /* Set foreground colour */
   fg.type = PMODEL_RGB;
   fg.val.general.x = 1.0;
   fg.val.general.y = 1.0;
   fg.val.general.z = 1.0;

   /* Setup default attribute bundles */
   wsdt->num_predefined_polyline_indices = WST_MIN_PREDEF_LINE_REPS;
   wsdt->default_polyline_bundle_table[0].type = PLINE_SOLID;
   wsdt->default_polyline_bundle_table[0].width = 1.0;
   memcpy(&wsdt->default_polyline_bundle_table[0].colr,  &fg, sizeof(Pgcolr));

   wsdt->num_predefined_polymarker_indices = WST_MIN_PREDEF_MARKER_REPS;
   wsdt->default_polymarker_bundle_table[0].type = PLINE_SOLID;
   wsdt->default_polymarker_bundle_table[0].size = 1.0;
   memcpy(&wsdt->default_polymarker_bundle_table[0].colr, &fg, sizeof(Pgcolr));

   wsdt->num_predefined_text_indices = WST_MIN_PREDEF_TEXT_REPS;
   wsdt->default_text_bundle_table[0].font = 0;
   memcpy(&wsdt->default_text_bundle_table[0].colr, &fg, sizeof(Pgcolr));

   wsdt->num_predefined_edge_indices = WST_MIN_PREDEF_EDGE_REPS;
   wsdt->default_edge_bundle_table[0].flag = PEDGE_OFF;
   wsdt->default_edge_bundle_table[0].type = PLINE_SOLID;
   wsdt->default_edge_bundle_table[0].width = 1.0;
   memcpy(&wsdt->default_edge_bundle_table[0].colr, &fg, sizeof(Pgcolr));

   wsdt->num_predefined_interior_indices = WST_MIN_PREDEF_INTERIOR_REPS;
   wsdt->default_interior_bundle_table[0].style = PSTYLE_SOLID;
   wsdt->default_interior_bundle_table[0].style_ind = 1;
   memcpy(&wsdt->default_interior_bundle_table[0].colr, &fg, sizeof(Pgcolr));
}

/*******************************************************************************
 * init_views
 *
 * DESCR:	Helper function to initialize default views
 * RETURNS:	N/A
 */

static void init_views(
    Wst_phigs_dt *ws_dt
    )
{
    int	i;
    Pview_rep3 *view;

    view = ws_dt->default_views;

    ws_dt->num_predefined_views = WST_MIN_PREDEF_VIEW_REPS;

    phg_mat_identity( view[0].ori_matrix );
    phg_mat_identity( view[0].map_matrix );
    view[0].clip_limit.x_min = view[0].clip_limit.y_min =
	view[0].clip_limit.z_min = 0.0;
    view[0].clip_limit.x_max = view[0].clip_limit.y_max =
	view[0].clip_limit.z_max = 1.0;
    view[0].xy_clip = view[0].back_clip = view[0].front_clip = PIND_CLIP;
    for ( i = 1; i < WST_MIN_PREDEF_VIEW_REPS; i++ )
	view[i] = view[0];

    /* Predefined some interesting views. */
    /* View 1: parallel front view in lower left corner of ws window.
     * vrp = (0,0,0), vup = <0,1,0>, vpn = <0,0,1>, prp = (0.5,0.5,5.0)
     * win x limits = [0,1], win y limits = [0,1]
     * view plane = 0.0, front plane = 1.0, back plane = 0.0
     * vp x limits = [.1,.4], vp y limits = [.1,.4], vp z limits = [0,1]
     */
    view[1].map_matrix[0][0] = 0.3;
    view[1].map_matrix[0][3] = 0.1;
    view[1].map_matrix[1][1] = 0.3;
    view[1].map_matrix[1][3] = 0.1;

    /* View 2: top view in upper left corner of ws window.
     * vrp = (0,0,0), vup = <0,0,-1>, vpn = <0,1,0>, prp = (0.5,-0.5,5.0)
     * win x limits = [0,1], win y limits = [-1,0]
     * view plane = 0.0, front plane = 1.0, back plane = 0.0
     * vp x limits = [.1,.4], vp y limits = [.6,.9], vp z limits = [0,1]
     */
    view[2].map_matrix[0][0] = 0.3;
    view[2].map_matrix[0][3] = 0.1;
    view[2].map_matrix[1][1] = 0.3;
    view[2].map_matrix[1][3] = 0.9;
    view[2].ori_matrix[1][1] =  0.0;
    view[2].ori_matrix[1][2] = -1.0;
    view[2].ori_matrix[2][1] =  1.0;
    view[2].ori_matrix[2][2] =  0.0;

    /* View 3: right side view in lower right corner of ws window.
     * vrp = (0,0,0), vup = <0,1,0>, vpn = <1,0,0>, prp = (-0.5,0.5,5.0)
     * win x limits = [-1,0], win y limits = [0,1]
     * view plane = 0.0, front plane = 1.0, back plane = 0.0
     * vp x limits = [.6,.9], vp y limits = [.1,.4], vp z limits = [0,1]
     */
    view[3].map_matrix[0][0] = 0.3;
    view[3].map_matrix[0][3] = 0.9;
    view[3].map_matrix[1][1] = 0.3;
    view[3].map_matrix[1][3] = 0.1;
    view[3].ori_matrix[0][0] =  0.0;
    view[3].ori_matrix[0][2] = -1.0;
    view[3].ori_matrix[2][0] =  1.0;
    view[3].ori_matrix[2][2] =  0.0;

    /* View 4: off-axis view in upper right corner of ws window.
     * vrp = (0,0,0), vup = <0,1,0>, vpn = <1,1,1>, prp = (0,0,5)
     * win x limits = [-a,a], win y limits = [-a,a], a = 1/sqrt(2)
     * view plane = 0.0, front plane = sqrt(3), back plane = 0.0
     * vp x limits = [.6,.9], vp y limits = [.6,.9], vp z limits = [0,1]
     */
    view[4].map_matrix[0][0] = 0.3 / sqrt(2.0);
    view[4].map_matrix[1][1] = 0.3 / sqrt(2.0);
    view[4].map_matrix[2][2] = 1.0 / sqrt(3.0);
    view[4].map_matrix[0][3] = 0.75;
    view[4].map_matrix[1][3] = 0.75;
    view[4].ori_matrix[0][0] =
        -(view[4].ori_matrix[0][2] = -1.0/sqrt(2.0));
    view[4].ori_matrix[2][0] =
        view[4].ori_matrix[2][1] =
        view[4].ori_matrix[2][2] =  1.0/sqrt(3.0);
    view[4].ori_matrix[1][0] =
        view[4].ori_matrix[1][2] = -1.0/(sqrt(3.0) * sqrt(2.0));
    view[4].ori_matrix[1][1] = -2.0 * view[4].ori_matrix[1][0];

    /* View 5: off-axis perspective view in whole of ws window.
     * vrp = (0,0,0), vup = <0,1,0>, vpn = <1,1,1>, prp = (0,0,20)
     * win x limits = [-a,a], win y limits = [-a,a], a = 1/sqrt(2)
     * view plane = 10.0, front plane = sqrt(3), back plane = 0.0
     * vp x limits = [0,1], vp y limits = [0,1], vp z limits = [0,1]
     */
    memcpy(view[5].ori_matrix,
           view[4].ori_matrix,
	   sizeof(Pmatrix3));
    view[5].map_matrix[0][0] =  0.5 / sqrt(2.0);
    view[5].map_matrix[1][1] =  0.5 / sqrt(2.0);
    view[5].map_matrix[0][2] = -0.025;
    view[5].map_matrix[1][2] = -0.025;
    view[5].map_matrix[2][2] = 1.0/sqrt(3.0) - 1.0/20.0;
    view[5].map_matrix[0][3] = 0.5;
    view[5].map_matrix[1][3] = 0.5;
    view[5].map_matrix[3][2] = -1.0/20.0;
}

/*******************************************************************************
 * init_devices
 *
 * DESCR:	Helper function to initialize input devices
 * RETURNS:	N/A
 */

static void init_devices(
    Wst *wst
    )
{
   Ppoint3 position = {0.0, 0.0, 0.0};
   Plimit3 e_volume = {0.0, 1.0, 0.0, 1.0, 0.0, 1.0};
   Wst_input_wsdt *idt = &wst->desc_tbl.phigs_dt.in_dt;

   /* Default locator */
   idt->num_devs.loc = 1;
   memcpy(&idt->locators[0].position, &position, sizeof(Ppoint3));
   memcpy(&idt->locators[0].e_volume, &e_volume, sizeof(Plimit3));
   idt->locators[0].record.pets.pet_r1.unused = 0;

   /* Default stroke */
   idt->num_devs.stroke = 1;
   memcpy(&idt->strokes[0].e_volume, &e_volume, sizeof(Plimit3));
   idt->strokes[0].record.pets.pet_r1.unused = 0;

   /* Default pick */
   idt->num_devs.pick = 1;
   memcpy(&idt->picks[0].e_volume, &e_volume, sizeof(Plimit3));
   idt->picks[0].record.pets.pet_r1.unused = 0;

   /* Default valuator */
   idt->num_devs.val = 1;
   memcpy(&idt->valuators[0].e_volume, &e_volume, sizeof(Plimit3));
   idt->valuators[0].record.pets.pet_r1.unused = 0;

   /* Default choice */
   idt->num_devs.choice = 1;
   memcpy(&idt->choices[0].e_volume, &e_volume, sizeof(Plimit3));
   idt->choices[0].record.pets.pet_r1.unused = 0;

   /* Default string */
   idt->num_devs.string = 1;
   memcpy(&idt->strings[0].e_volume, &e_volume, sizeof(Plimit3));
   idt->strings[0].record.pets.pet_r1.unused = 0;
}

/*******************************************************************************
 * wsx_gl_create
 *
 * DESCR:	Create workstation type
 * RETURNS:	Pointer to workstation type or NULL
 */

Wst* wsx_gl_create(
   Err_handle erh,
   Pws_cat category
   )
{
   Wst *wst;
   Pint ws_type;

   switch (category) {
      case PCAT_OUT:
         ws_type = PWST_OUTPUT_TRUE;
         break;

      case PCAT_OUTIN:
         ws_type = PWST_OUTIN_TRUE;
         break;

      default:
         ws_type = PWST_OUTPUT_TRUE;
         break;
   }

   wst = phg_wst_create(erh, ws_type);
   if (wst != NULL) {
      if (!wsx_gl_init(wst, category)) {
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
   Wst *wst,
   Pws_cat category
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

   dt->ws_category = category;
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

   /* Setup default views */
   init_views(dt);

   /* Initialize workstation output attributes */
   init_output_ws_dt(&dt->out_dt);

   if (category == PCAT_OUTIN) {

      /* Initialize input devices */
      init_devices(wst);
   }

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

   if (dt->ws_category == PCAT_OUTIN) {

      ws->input_overlay_window = phg_wsx_create_overlay(ws);
      if (ws->input_overlay_window == 0) {
         XDestroyWindow(ws->display, ws->drawable_id);
         return FALSE;
      }

      if (!phg_ws_input_init(ws, args->input_q)) {
         XDestroyWindow(ws->display, ws->input_overlay_window);
         XDestroyWindow(ws->display, ws->drawable_id);
         return FALSE;
      }
   }

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
#ifdef DEBUG
   printf("wsgl_gl_resize_window: %d %d\n", w, h);
#endif

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

