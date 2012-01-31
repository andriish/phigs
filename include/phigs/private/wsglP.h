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

#include <sys/types.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>
#include <phigs/util.h>
#include <GL/gl.h>

typedef enum {
   WS_RENDER_MODE_DRAW,
   WS_RENDER_MODE_SELECT
} Ws_render_mode;

typedef struct {
   Pint x, y;
   Pfloat distance;
} Ws_hit_box;

typedef struct {
   Pint sid;
   Pint pickid;
   Pint offset;
} Ws_pick_elmt;

typedef struct {
   Pint offset;
} Ws_struct;

typedef struct {
   int     used;
   Nameset incl;
   Nameset excl;
} Ws_filter;

typedef struct _Wsgl {
   Pattr_group     *attr_group;
   Plimit3         curr_win;
   Pint            win_changed;
   Plimit3         curr_vp;
   Pint            vp_changed;
   Pint            hlhsr_changed;
   Pint            hlhsr_mode;
   Pgcolr          background;
   Pmatrix3        local_tran, total_tran;
   Pmatrix3        pick_tran;
   Pint            curr_view_index;
   Pview_rep3      view_rep;
   Ws_render_mode  render_mode;
   Stack           struct_stack;
   Ws_struct       cur_struct;
   Pint            pick_id;
   Nameset         cur_nameset;
   Ws_filter       pick_filter;
   Pint            select_size;
   GLuint          *select_buf;
} Wsgl;

/*******************************************************************************
 * wsgl_init
 *
 * DESCR:       Initialize renderer
 * RETURNS:     Non zero or zero on error
 */

int wsgl_init(
   Ws *ws,
   Pgcolr *background,
   Pint select_size
   );

/*******************************************************************************
 * wsgl_close
 *
 * DESCR:       Close 
 * RETURNS:     N/A
 */

void wsgl_close(
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
   Ws *ws
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
 * wsgl_begin_structure
 *
 * DESCR:       Mark the beginning of a new structure element
 * RETURNS:     N/A
 */

void wsgl_begin_structure(
   Ws *ws,
   Pint struct_id
   );

/*******************************************************************************
 * wsgl_end_structure
 *
 * DESCR:       Mark the ending of a structure element
 * RETURNS:     N/A
 */

void wsgl_end_structure(
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
 * wsgl_set_filter
 *
 * DESCR:       Set filter
 * RETURNS:     N/A
 */

void wsgl_set_filter(
   Ws *ws,
   Phg_args_flt_type type,
   Nameset incl,
   Nameset excl
   );

/*******************************************************************************
 * wsgl_begin_pick
 *
 * DESCR:       Begin pick process
 * RETURNS:     N/A
 */

void wsgl_begin_pick(
   Ws *ws,
   Ws_hit_box *box
   );

/*******************************************************************************
 * wsgl_end_pick
 *
 * DESCR:       End pick process
 * RETURNS:     N/A
 */

void wsgl_end_pick(
   Ws *ws,
   Pint *err_ind,
   Pint *depth,
   Ws_pick_elmt **elmts
   );

/*******************************************************************************
 * phg_update_projection
 *
 * DESCR:       Update projection matrix
 * RETURNS:     N/A
 */

void phg_update_projection(
   Ws *ws
   );

/*******************************************************************************
 * phg_update_modelview
 *
 * DESCR:       Update modelview matrix
 * RETURNS:     N/A
 */

void phg_update_modelview(
   Ws *ws
   );

/*******************************************************************************
 * phg_set_view_ind
 *
 * DESCR:       Setup view
 * RETURNS:     N/A
 */

void phg_set_view_ind(
   Ws *ws,
   Pint ind
   );

/*******************************************************************************
 * phg_set_hlhsr_id
 *
 * DESCR:       Setup depth buffer checking
 * RETURNS:     N/A
 */

void phg_set_hlhsr_id(
   Pint hlhsr_id
   );

/*******************************************************************************
 * phg_set_gcolr
 *
 * DESCR:       Setup colour
 * RETURNS:     N/A
 */

void phg_set_gcolr(
   Pgcolr *gcolr
   );

/*******************************************************************************
 * phg_set_line_ind
 *
 * DESCR:       Setup line index
 * RETURNS:     N/A
 */

void phg_set_line_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   );

/*******************************************************************************
 * phg_set_line_attr
 *
 * DESCR:       Setup line attributes
 * RETURNS:     N/A
 */

void phg_set_line_attr(
   Ws *ws,
   Pline_bundle_plus *attr
   );

/*******************************************************************************
 * phg_set_int_ind
 *
 * DESCR:       Setup interior index
 * RETURNS:     N/A
 */

void phg_set_int_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   );

/*******************************************************************************
 * phg_set_int_attr
 *
 * DESCR:       Setup interior attributes
 * RETURNS:     N/A
 */

void phg_set_int_attr(
   Ws *ws,
   Pint_bundle_plus *attr
   );

/*******************************************************************************
 * phg_set_edge_ind
 *
 * DESCR:       Setup edge index
 * RETURNS:     N/A
 */

void phg_set_edge_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   );

/*******************************************************************************
 * phg_set_edge_attr
 *
 * DESCR:       Setup interior attributes
 * RETURNS:     N/A
 */

void phg_set_edge_attr(
   Ws *ws,
   Pedge_bundle_plus *attr
   );

/*******************************************************************************
 * phg_set_marker_ind
 *
 * DESCR:       Setup marker index
 * RETURNS:     N/A
 */

void phg_set_marker_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   );

/*******************************************************************************
 * phg_set_marker_attr
 *
 * DESCR:       Setup marker attributes
 * RETURNS:     N/A
 */

void phg_set_marker_attr(
   Ws *ws,
   Pmarker_bundle_plus *attr
   );

/*******************************************************************************
 * phg_set_text_ind
 *
 * DESCR:       Setup text index
 * RETURNS:     N/A
 */

void phg_set_text_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   );

/*******************************************************************************
 * phg_add_names_set
 *
 * DESCR:       Add names to nameset
 * RETURNS:     N/A
 */

void phg_add_names_set(
   Ws *ws,
   Pint_list *names
   );

/*******************************************************************************
 * phg_remove_names_set
 *
 * DESCR:       Remove names from nameset
 * RETURNS:     N/A
 */

void phg_remove_names_set(
   Ws *ws,
   Pint_list *names
   );

/*******************************************************************************
 * phg_draw_polymarker
 *
 * DESCR:       Draw markers
 * RETURNS:     N/A
 */

void phg_draw_polymarker(
   Ws *ws,
   Ppoint_list *point_list,
   Pattr_group *attr
   );

/*******************************************************************************
 * phg_draw_polymarker3
 *
 * DESCR:       Draw markers 3D
 * RETURNS:     N/A
 */

void phg_draw_polymarker3(
   Ws *ws,
   Ppoint_list3 *point_list,
   Pattr_group *attr
   );

/*******************************************************************************
 * phg_draw_polyline
 *
 * DESCR:       Draw lines
 * RETURNS:     N/A
 */

void phg_draw_polyline(
   Ws *ws,
   Ppoint_list *point_list,
   Pattr_group *attr
   );

/*******************************************************************************
 * phg_draw_polyline3
 *
 * DESCR:       Draw lines 3D
 * RETURNS:     N/A
 */

void phg_draw_polyline3(
   Ws *ws,
   Ppoint_list3 *point_list,
   Pattr_group *attr
   );

/*******************************************************************************
 * phg_draw_fill_area
 *
 * DESCR:       Draw fill area
 * RETURNS:     N/A
 */

void phg_draw_fill_area(
   Ws *ws,
   Ppoint_list *point_list,
   Pattr_group *attr
   );

/*******************************************************************************
 * phg_draw_fill_area3
 *
 * DESCR:       Draw fill area 3D
 * RETURNS:     N/A
 */

void phg_draw_fill_area3(
   Ws *ws,
   Ppoint_list3 *point_list,
   Pattr_group *attr
   );

/*******************************************************************************
 * phg_draw_text
 *
 * DESCR:       Draw text
 * RETURNS:     N/A
 */

void phg_draw_text(
   Ppoint *pos,
   char *text
   );

extern unsigned char *wsgl_hatch_tbl[];

#endif /* _wsglP_h */

