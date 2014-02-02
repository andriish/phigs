/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2014 Surplus Users Ham Society
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

#ifndef _wsb_h
#define _wsb_h

#ifdef __cplusplus
extern "C" {
#endif

#define WSB_NONE_POSTED(posted_ptr) \
   ((posted_ptr)->lowest.higher == &(posted_ptr)->highest)

#define WSB_SOME_POSTED(posted_ptr) \
   ((posted_ptr)->lowest.higher != &(posted_ptr)->highest)

#define WSB_CHECK_POSTED(posted_ptr) \
        assert(((posted_ptr)->lowest.higher == &(posted_ptr)->highest) \
                == ((posted_ptr)->highest.lower == &(posted_ptr)->lowest))

#define WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, now_action_ptr)	\
{								\
   if ( *(now_action_ptr) == PHG_UPDATE_IF_IG ||		\
        *(now_action_ptr) == PHG_UPDATE_IF_IL)			\
      phg_wsb_resolve_now_action(ws, now_action_ptr);		\
}

Ws* phg_wsb_open_ws(
    Phg_args_open_ws *args,
    Phg_ret *ret
    );

void wsb_free_all_posted(
    Wsb_output_ws *owsb
    );

void wsb_destroy_ws(
    Ws *ws
    );

void phg_wsb_close_ws(
    Ws *ws
    );

void phg_wsb_redraw_all(
    Ws *ws,
    Pctrl_flag clear_control
    );

void phg_wsb_make_requested_current(
    Ws *ws
    );

void phg_wsb_repaint_all(
    Ws *ws,
    Pctrl_flag clear_control
    );

void phg_wsb_traverse_all_postings(
    Ws *ws
    );

void phg_wsb_traverse_net(
    Ws_handle ws,
    Struct_handle structp
    );

void phg_wsb_add_el(
    Ws *ws
    );

int phg_wsb_asti_update(
    Ws *ws,
    Pctrl_flag clear_control
    );

void phg_wsb_close_struct(
    Ws *ws,
    Struct_handle structh
    );

void phg_wsb_post(
    Ws *ws,
    Struct_handle structh,
    Pfloat priority,
    Pint first_posting
    );

void phg_wsb_change_posting(
    Ws *ws,
    Struct_handle unpost,
    Struct_handle post
    );

void phg_wsb_unpost(
    Ws *ws,
    Struct_handle structh
    );

void phg_wsb_unpost_all(
    Ws *ws
    );

void phg_wsb_delete_all_structs(
    Ws *ws
    );

int phg_wsb_delete_struct(
    Ws *ws,
    Struct_handle structh,
    Ws_delete_flag flag
    );

int phg_wsb_delete_struct_net(
    Ws *ws,
    Struct_handle structh,
    Pref_flag reff,
    Ws_delete_flag flag
    );

void phg_wsb_copy_struct(
    Ws *ws,
    El_handle first_el
    );

int phg_wsb_delete_el(
    Ws *ws,
    Struct_handle structh,
    El_handle elh1,
    El_handle elh2,
    Ws_delete_flag flag
    );

void phg_wsb_conditional_redraw(
    Ws *ws
    );

void phg_wsb_resolve_now_action(
    Ws *ws,
    Ws_update_action *now_action_ptr
    );

void phg_wsb_update(
    Ws *ws,
    Pregen_flag flag
    );

void phg_wsb_set_disp_update_state(
    Ws *ws,
    Pdefer_mode def_mode,
    Pmod_mode mod_mode
    );

void phg_wsb_set_hlhsr_mode(
    Ws *ws,
    Pint mode
    );

void phg_wsb_set_ws_window(
    Ws *ws,
    Pint two_d,
    Plimit3 *limits
    );

void phg_wsb_set_ws_vp(
    Ws *ws,
    Pint two_d,
    Plimit3 *limits
    );

void phg_wsb_set_rep(
    Ws *ws,
    Phg_args_rep_type type,
    Phg_args_rep_data *rep
    );

void phg_wsb_set_filter(
    Ws *ws,
    Phg_args_flt_type type,
    Pint dev_id,
    Pint_list *incl_set,
    Pint_list *excl_set
    );

void phg_wsb_inq_posted(
    Ws *ws,
    Phg_ret *ret
    );

void phg_wsb_inq_ws_xform(
    Ws *ws,
    Phg_ret *ret
    );

void phg_wsb_inq_disp_update_state(
    Ws *ws,
    Phg_ret *ret
    );

void phg_wsb_inq_hlhsr_mode(
    Ws *ws,
    Phg_ret *ret
    );

void phg_wsb_inq_rep(
    Ws *ws,
    Pint index,
    Pinq_type how,
    Phg_args_rep_type rep_type,
    Phg_ret *ret
    );

void phg_wsb_inq_view_indices(
    Ws *ws,
    Phg_ret *ret
    );

/*******************************************************************************
 * phg_wsb_set_view_input_priority
 *
 * DESCR:       Set priority of view relative another view
 * RETURNS:     N/A
 */

void phg_wsb_set_view_input_priority(
   Ws *ws,
   Pint index,
   Pint ref_index,
   Prel_pri priority
   );

/*******************************************************************************
 * phg_wsb_map_initial_points
 *
 * DESCR:       Map initial points
 * RETURNS:     TRUE or FALSE
 */

int phg_wsb_map_initial_points(
    Ws *ws,
    Pint view_index,
    Pint *num_pts,
    Ppoint3 *wc_pts,
    XPoint *dwbl_pts
    );

/*******************************************************************************
 * phg_wsb_resolve_locator
 *
 * DESCR:       Resolve locator device
 * RETURNS:     TRUE or FALSE
 */

int phg_wsb_resolve_locator(
    Ws *ws,
    Ws_point *dc_pt,
    int determine_z,
    Pint *view_index,
    Ppoint3 *wc_pt
    );

/*******************************************************************************
 * phg_wsb_point_in_viewport
 *
 * DESCR:       Test if point is within viewport
 * RETURNS:     TRUE or FALSE
 */

int phg_wsb_point_in_viewport(
    Ws *ws,
    XPoint *pt
    );

/*******************************************************************************
 * phg_wsb_resolve_stroke
 *
 * DESCR:       Resolve stroke device
 * RETURNS:     TRUE or FALSE
 */

int phg_wsb_resolve_stroke(
    Ws *ws,
    Pint num_pts,
    Ws_point *dc_pts,
    int determine_z,
    Pint *view_index,
    Ppoint_list3 *wc_pts
    );

/*******************************************************************************
 * phg_wsb_resolve_pick
 *
 * DESCR:       Resolve pick device
 * RETURNS:     TRUE or FALSE
 */

int phg_wsb_resolve_pick(
    Ws *ws,
    Ws_inp_pick *dev,
    int echo,
    Ws_point *dc_pt,
    Ppick *pick
    );

/*******************************************************************************
 * phg_wsb_create_LUTs
 *  
 * DESCR:       Create workstation lookup tables
 * RETURNS:     Non-zero on success
 */

int phg_wsb_create_LUTs(
    Ws *ws
    );

/*******************************************************************************
 * phg_wsb_destroy_LUTs
 *
 * DESCR:       Destroy workstation lookup tables
 * RETURNS:     N/A
 */

void phg_wsb_destroy_LUTs(
    Ws *ws
    );

/*******************************************************************************
 * phg_wsb_set_LUT_entry
 *
 * DESCR:       Set workstation table entry
 * RETURNS:     N/A
 */

void phg_wsb_set_LUT_entry(
    Ws *ws,
    Phg_args_rep_type type,
    Phg_args_rep_data *rep,
    Pgcolr *gcolr
    );

/*******************************************************************************
 * phg_wsb_inq_LUT_entry
 *
 * DESCR:       Get workstation table entry
 * RETURNS:     N/A
 */

void phg_wsb_inq_LUT_entry(
    Ws *ws,
    Pint index,
    Pinq_type type,
    Phg_args_rep_type rep_type,
    Phg_ret *ret,
    Pgcolr *gcolr,
    Pview_rep3 *vrep
    );

/*******************************************************************************
 * phg_wsb_inq_LUT_indices
 *
 * DESCR:       Get workstation indices in table
 * RETURNS:     N/A
 */

void phg_wsb_inq_LUT_indices(
    Ws *ws,
    Phg_args_rep_type rep_type,
    Phg_ret *ret
    );

/*******************************************************************************
 * phg_wsb_set_name_set
 *
 * DESCR:       Set filter name set
 * RETURNS:     N/A
 */

void phg_wsb_set_name_set(
   Ws *ws,
   Phg_args_flt_type type,
   Pint dev_id,
   Pint_list *incl_set,
   Pint_list *excl_set
   );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _wsb_h */

