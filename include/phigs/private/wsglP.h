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

#include <stdint.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>
#include <phigs/util.h>
#include <GL/gl.h>

#define WS_RENDER_FILL         0x01
#define WS_RENDER_EDGE         0x02
#define WS_RENDER_LINE         0x04
#define WS_RENDER_MARKER       0x08
#define WS_RENDER_TEXT         0x10

typedef struct {
   Pint x, y;
   Pfloat distance;
} Ws_hit_box;

typedef struct {
   Pint sid;
   Pint pickid;
   Pint offset;
} Ws_pick_elmt;

typedef enum {
   WS_RENDER_MODE_DRAW,
   WS_RENDER_MODE_SELECT
} Ws_render_mode;

typedef struct {
   Pattr_group  bundl_group;
   Pattr_group  indiv_group;
   Pfloat       char_ht;
   Ptext_path   text_path;
   Ptext_align  text_align;
   Pvec         char_up_vec;
   Nset         asf_nameset;
   uint32_t     ast_buf[1];
} Ws_attr_st;

typedef struct {
   Pint       id;
   Pint       offset;
   Pint       hlhsr_id;
   Ws_attr_st ast;
   Nset       cur_nameset;
   uint32_t   nameset_buf[WS_MAX_NAMES_IN_NAMESET / 32];
   Pview_rep3 view_rep;
   Pmatrix3   local_tran;
   Pmatrix3   global_tran;
   Pint       pick_id;
   Nset       lightstat;
   uint32_t   lightstat_buf[1];
} Ws_struct;

typedef struct {
   int     used;
   Nameset incl;
   Nameset excl;
} Ws_filter;

typedef struct _Wsgl {
   Plimit3         cur_win;
   int             win_changed;
   Plimit3         cur_vp;
   int             vp_changed;
   int             hlhsr_changed;
   Pint            hlhsr_mode;
   Pgcolr          background;
   Ws_render_mode  render_mode;
   Stack           struct_stack;
   Ws_struct       cur_struct;
   Pmatrix3        composite_tran;
   Pmatrix3        model_tran;
   Pmatrix3        pick_tran;
   Ws_filter       invis_filter;
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
 * wsgl_set_asf
 *
 * DESCR:       Setup asf
 * RETURNS:     N/A
 */

void wsgl_set_asf(
   Ws_attr_st *ast,
   void *asf_info
   );

/*******************************************************************************
 * wsgl_set_colr
 *
 * DESCR:       Set colour value
 * RETURNS:     N/A
 */

void wsgl_set_colr(
   Pint colr_type,
   Pcoval *colr
   );

/*******************************************************************************
 * wsgl_set_gcolr
 *
 * DESCR:       Set colour
 * RETURNS:     N/A
 */

void wsgl_set_gcolr(
   Pgcolr *gcolr
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
 * wsgl_begin_pass
 *
 * DESCR:       Start a rendiering pass for workstation
 * RETURNS:     N/A
 */

void wsgl_begin_pass(
   Ws *ws,
   int flags
   );

/*******************************************************************************
 * wsgl_end_pass
 *
 * DESCR:       End a rendiering pass for workstation
 * RETURNS:     N/A
 */

void wsgl_end_pass(
   Ws *ws,
   int flags
   );

/*******************************************************************************
 * check_draw_primitive
 *
 * DESCR:       Check if the primitive shall be drawn
 * RETURNS:     TRUE or FALSE
 */

int check_draw_primitive(
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
   Ws *ws,
   int flags
   );

/*******************************************************************************
 * wsgl_render_element
 *
 * DESCR:       Render element to current workstation rendering window
 * RETURNS:     N/A
 */

void wsgl_render_element(
   Ws *ws,
   El_handle el,
   int flags
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
 * wsgl_update_projection
 *
 * DESCR:       Update projection matrix
 * RETURNS:     N/A
 */

void wsgl_update_projection(
   Ws *ws
   );

/*******************************************************************************
 * wsgl_update_modelview
 *
 * DESCR:       Update modelview matrix
 * RETURNS:     N/A
 */

void wsgl_update_modelview(
   Ws *ws
   );

/*******************************************************************************
 * wsgl_set_view_ind
 *
 * DESCR:       Setup view
 * RETURNS:     N/A
 */

void wsgl_set_view_ind(
   Ws *ws,
   Pint ind
   );

/*******************************************************************************
 * wsgl_update_hlhsr_id
 *
 * DESCR:       Update depth buffer checking flag
 * RETURNS:     N/A
 */

void wsgl_update_hlhsr_id(
   Ws *ws
   );

/*******************************************************************************
 * wsgl_set_line_ind
 *
 * DESCR:       Setup line index
 * RETURNS:     N/A
 */

void wsgl_set_line_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   );

/*******************************************************************************
 * wsgl_setup_line_attr
 *
 * DESCR:       Setup line attributes
 * RETURNS:     N/A
 */

void wsgl_setup_line_attr(
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_set_int_ind
 *
 * DESCR:       Setup interior index
 * RETURNS:     N/A
 */

void wsgl_set_int_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   );

/*******************************************************************************
 * wsgl_get_int_style
 *
 * DESCR:       Get interior style
 * RETURNS:     Interiour style
 */

Pint_style wsgl_get_int_style(
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_setup_int_nocol
 *
 * DESCR:       Setup interior attributes without color 
 * RETURNS:     N/A
 */

void wsgl_setup_int_attr_nocol(
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_setup_int_attr
 *
 * DESCR:       Setup interior attributes 
 * RETURNS:     N/A
 */

void wsgl_setup_int_attr(
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_set_polygon_offset
 *
 * DESCR:       Setup polygon offset helper function
 * RETURNS:     N/A
 */

void wsgl_set_polygon_offset(
   float w
   );

/*******************************************************************************
 * wsgl_get_facet_colr
 *
 * DESCR:       Get facet colour
 * RETURNS:     N/A
 */

void wsgl_get_facet_colr(
   Pcoval *colr,
   Pint fflag,
   Pfacet_data3 *fdata,
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_get_int_refl_eqn
 *
 * DESCR:       Get interiour reflection equation
 * RETURNS:     N/A
 */

Pint wsgl_get_int_refl_eqn(
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_get_refl_props
 *
 * DESCR:       Get surface reflectance properties
 * RETURNS:     Pointer to surface reflectance properties
 */

Prefl_props* wsgl_get_refl_props(
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_set_edge_ind
 *
 * DESCR:       Setup edge index
 * RETURNS:     N/A
 */

void wsgl_set_edge_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   );

/*******************************************************************************
 * wsgl_get_edge_flag
 *
 * DESCR:       Get edge flag
 * RETURNS:     Edge flag
 */

Pedge_flag wsgl_get_edge_flag(
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_get_edge_width
 *
 * DESCR:       Get edge width
 * RETURNS:     Edge width
 */

Pfloat wsgl_get_edge_width(
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_setup_edge_attr
 *
 * DESCR:       Setup edge attributes 
 * RETURNS:     N/A
 */

void wsgl_setup_edge_attr(
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_set_marker_ind
 *
 * DESCR:       Setup marker index
 * RETURNS:     N/A
 */

void wsgl_set_marker_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   );

/*******************************************************************************
 * wsgl_get_marker_type
 *
 * DESCR:       Get marker type
 * RETURNS:     Marker type
 */

Pint wsgl_get_marker_type(
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_get_marker_size
 *
 * DESCR:       Get marker size
 * RETURNS:     Marker size
 */

Pfloat wsgl_get_marker_size(
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_setup_marker_attr
 *
 * DESCR:       Setup marker attributes 
 * RETURNS:     N/A
 */

void wsgl_setup_marker_attr(
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_setup_background
 *
 * DESCR:       Setup background colour 
 * RETURNS:     N/A
 */

void wsgl_setup_background(
   Ws *ws
   );

/*******************************************************************************
 * wsgl_set_text_ind
 *
 * DESCR:       Setup text index
 * RETURNS:     N/A
 */

void wsgl_set_text_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   );

/*******************************************************************************
 * wsgl_get_text_prec
 *
 * DESCR:       Get text precision
 * RETURNS:     Text precision
 */

Ptext_prec wsgl_get_text_prec(
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_get_text_fnt
 *
 * DESCR:       Get text font
 * RETURNS:     Text font
 */

Phg_font* wsgl_get_text_fnt(
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_get_text_char_expan
 *
 * DESCR:       Get text character expansion
 * RETURNS:     Text font
 */

Pfloat wsgl_get_text_char_expan(
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_get_text_char_space
 *
 * DESCR:       Get text character spacing
 * RETURNS:     Character spacing
 */

Pfloat wsgl_get_text_char_space(
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_setup_text_attr
 *
 * DESCR:       Setup text attributes
 * RETURNS:     N/A
 */

void wsgl_setup_text_attr(
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_add_names_set
 *
 * DESCR:       Add names to nameset
 * RETURNS:     N/A
 */

void wsgl_add_names_set(
   Ws *ws,
   void *names
   );

/*******************************************************************************
 * wsgl_remove_names_set
 *
 * DESCR:       Remove names from nameset
 * RETURNS:     N/A
 */

void wsgl_remove_names_set(
   Ws *ws,
   void *names
   );

/*******************************************************************************
 * phg_draw_fill_area3_data
 *
 * DESCR:       Draw fill area width data 3D
 * RETURNS:     N/A
 */

void phg_draw_fill_area3_data(
   Ws *ws,
   void *pdata,
   Ws_attr_st *ast
   );

/*******************************************************************************
 * wsgl_set_light_src_state
 *
 * DESCR:       Set light source state for workstation
 * RETURNS:     N/A
 */

void wsgl_set_light_src_state(
   Ws *ws,
   void *pdata
   );

/*******************************************************************************
 * wsgl_light_colr
 *
 * DESCR:       Perform lighting calculations and set colour for vertex
 * RETURNS:     N/A
 */

void wsgl_light_colr(
   Ws *ws,
   Pcoval *result,
   Pint refl_eqn,
   Prefl_props *refl_props,
   Pint colr_type,
   Pcoval *coval,
   Pvec3 *normal
   );

/*******************************************************************************
 * wsgl_render_fill
 *
 * DESCR:       Render fill element to current workstation rendering window
 * RETURNS:     N/A
 */

void wsgl_render_fill(
   Ws_attr_st *ast,
   El_handle el
   );

/*******************************************************************************
 * wsgl_begin_edge
 *
 * DESCR:       Start rendering for edge pass
 * RETURNS:     N/A
 */

void wsgl_begin_edge(
    void
    );

/*******************************************************************************
 * wsgl_end_edge
 *
 * DESCR:       Start rendering for edge pass
 * RETURNS:     N/A
 */

void wsgl_end_edge(
    void
    );

/*******************************************************************************
 * wsgl_render_edge
 *
 * DESCR:       Render fill element to current workstation rendering window
 * RETURNS:     N/A
 */

void wsgl_render_edge(
   Ws_attr_st *ast,
   El_handle el
   );

/*******************************************************************************
 * wsgl_render_line
 *
 * DESCR:       Render line element to current workstation rendering window
 * RETURNS:     N/A
 */

void wsgl_render_line(
   Ws_attr_st *ast,
   El_handle el
   );

/*******************************************************************************
 * wsgl_render_marker
 *
 * DESCR:       Render marker element to current workstation rendering window
 * RETURNS:     N/A
 */

void wsgl_render_marker(
   Ws_attr_st *ast,
   El_handle el
   );

/*******************************************************************************
 * wsgl_render_text
 *
 * DESCR:       Render text element to current workstation rendering window
 * RETURNS:     N/A
 */

void wsgl_render_text(
   Ws_attr_st *ast,
   El_handle el
   );

extern unsigned char *wsgl_hatch_tbl[];

#endif /* _wsglP_h */

