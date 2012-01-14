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

#ifndef _wsglP_h
#define _wsglP_h

#include <X11/Xlib.h>
#include <GL/glx.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>

typedef struct {
   GLXContext      glx_context;
   Wst             *type;
   Pattr_group     *attr_group;
   Plimit3         curr_win;
   Pint            win_changed;
   Plimit3         curr_vp;
   Pint            vp_changed;
   Pint            hlhsr_changed;
   Pint            hlhsr_mode;
   Pgcolr          background;
   Pmatrix3        local_tran, total_tran;
   Pint            curr_view_index;
   Pview_rep3      view_rep;
} Wsgl;

/*******************************************************************************
 * wsgl_create
 *
 * DESCR:       Create renderer for workstation
 * RETURNS:     Pointer to workstation or NULL
 */

Ws* wsgl_create(
   Phg_args_open_ws *args
   );

/*******************************************************************************
 * wsgl_init
 *
 * DESCR:       Initialize renderer
 * RETURNS:     Non zero or zero on error
 */

int wsgl_init(
   Ws *ws
   );

/*******************************************************************************
 * wsgl_open_window
 *
 * DESCR:       Open render window for workstation
 * RETURNS:     Zero on succcess, non zero on error
 */

int wsgl_open_window(
   Ws *ws,
   Phg_args_open_ws *args
   );

/*******************************************************************************
 * wsgl_release_window
 *
 * DESCR:       Close render window for workstation
 * RETURNS:     N/A
 */

int wsgl_release_window(
   Ws *ws
   );

/*******************************************************************************
 * wsgl_destroy
 *
 * DESCR:       Release workstation
 * RETURNS:     N/A
 */

void wsgl_destroy(
   Ws *ws
   );

/*******************************************************************************
 * wsgl_set_window
 *
 * DESCR:       Set render window coordinates
 * RETURNS:     N/A
 */

void wsgl_set_window(
   Ws *ws,
   Plimit3 *win
   );

/*******************************************************************************
 * wsgl_set_viewport
 *
 * DESCR:       Set render window viewport
 * RETURNS:     N/A
 */

void wsgl_set_viewport(
   Ws *ws,
   Plimit3 *vp
   );

/*******************************************************************************
 * wsgl_set_hlhsr_mode
 *
 * DESCR:       Set render depth mode
 * RETURNS:     N/A
 */

void wsgl_set_hlhsr_mode(
   Ws *ws,
   Pint hlhsr_mode
   );

/*******************************************************************************
 * wsgl_clear
 *
 * DESCR:       Clear render window
 * RETURNS:     N/A
 */

void wsgl_clear(
   void
   );

/*******************************************************************************
 * wsgl_flush
 *
 * DESCR:       Flush settings to render window
 * RETURNS:     N/A
 */
void wsgl_flush(
   Ws *ws
   );

/*******************************************************************************
 * wsgl_compute_ws_transform
 *
 * DESCR:       Compute workstation transform
 * RETURNS:     N/A
 */

void wsgl_compute_ws_transform(
   Plimit3 *ws_win,
   Plimit3 *ws_vp,
   Ws_xform *ws_xform
   );

/*******************************************************************************
 * wsgl_begin_rendering
 *
 * DESCR:       Start a rendiering session for workstation
 * RETURNS:     N/A
 */

void wsgl_begin_rendering(
   Ws *ws
   );

/*******************************************************************************
 * wsgl_end_rendering
 *
 * DESCR:       End a rendiering session
 * RETURNS:     N/A
 */

void wsgl_end_rendering(
   Ws *ws
   );

/*******************************************************************************
 * wsgl_render_element
 *
 * DESCR:       Render element to current workstation rendering window
 * RETURNS:     N/A
 */

void wsgl_render_element(
   Ws *ws,
   El_handle el
   );

/*******************************************************************************
 * phg_wsgl_find_best_visual
 *
 * DESCR:       Find best matching visual
 * RETURNS:     N/A
 */

void phg_wsgl_find_best_visual(
   Ws *ws,
   Wst *wst,
   XVisualInfo **visual_info,
   Colormap *cmap,
   Pint *err_ind
   );

/*******************************************************************************
 * phg_wsgl_create_context
 *
 * DESCR:       Create rendering context from visual info
 * RETURNS:     N/A
 */

void phg_wsgl_create_context(
   Ws *ws,
   XVisualInfo *visual_info,
   GLXContext *context,
   Pint *err_ind
   );

/*******************************************************************************
 * wsx_gl_create
 *
 * DESCR:       Create workstation type
 * RETURNS:     Pointer to workstation type or NULL
 */

Wst* wsx_gl_create(
   Err_handle erh,
   Pws_cat category
   );

/*******************************************************************************
 * wsx_gl_init
 *
 * DESCR:       Initialize workstation type
 * RETURNS:     TRUE or FALSE
 */

int wsx_gl_init(
   Wst *wst,
   Pws_cat category
   );

extern unsigned char *wsgl_hatch_tbl[];

#endif /* _wsglP_h */

