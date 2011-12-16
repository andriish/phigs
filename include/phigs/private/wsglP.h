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

#ifndef _wsglP_h
#define _wsglP_h

#include <GL/glx.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>

typedef struct {
   GLXContext      glx_context;
   Wst             *type;
   attribute_group attrs;
   Plimit3         curr_win;
   Pint            win_changed;
   Plimit3         curr_vp;
   Pint            vp_changed;
   Pmatrix3        win_tran;
   Pmatrix3        local_tran, total_tran;
   Pint            curr_view_index;
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
   void
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
 * wsx_gl_open_window
 *
 * DESCR:       Open render window for workstation
 * RETURNS:     Zero on succcess, non zero on error
 */

int wsx_gl_open_window(
   Ws *ws
   );

/*******************************************************************************
 * wsx_gl_release_window
 *
 * DESCR:       Close render window for workstation
 * RETURNS:     N/A
 */

void wsx_gl_release_window(
   Ws *ws
   );

/*******************************************************************************
 * wsx_gl_flush
 *
 * DESCR:       Flush render window for workstation
 * RETURNS:     N/A
 */
void wsx_gl_flush(
   Ws *ws
   );

#endif /* _wsglP_h */

