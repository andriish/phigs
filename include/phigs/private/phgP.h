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

#ifndef _phgP_h
#define _phgP_h

#ifdef __cplusplus
extern "C" {
#endif

#define ARGS_ELMT_SIZE(ARG) \
   ((Phg_args_add_el *) (ARG))->el_size

#define ARGS_ELMT_SIZE_FULL(ARG) \
   (((Phg_args_add_el *) (ARG))->el_size + sizeof(Phg_elmt_info))

#define ARGS_ELMT_TYPE(ARG) \
   ((Phg_args_add_el *) (ARG))->el_type

#define ARGS_COPY_DATA(ELMT) \
   (Phg_elmt_info *) (((Css_eldata *) (ELMT))->ptr)

#define ARGS_COPY_DATA_LEN(ELMT) \
   (((Phg_elmt_info *) (((Css_eldata *) (ELMT))->ptr))->length + \
      sizeof(Phg_elmt_info))

#define ARGS_INQ_HEAD(ARG) \
   (((Phg_ret_q_content *) argdata)->el_head)

#define ARGS_INQ_SIZE(ARG) \
   *((Pint *) argdata)

#define ELMT_HEAD(ELMT) \
   ((ELMT)->eldata.ptr)

#define ELMT_INFO(ELMT) \
   ((Phg_elmt_info *) (ELMT)->eldata.ptr)

#define ELMT_INFO_LEN(ELMT) \
   (((Phg_elmt_info *) (ELMT)->eldata.ptr)->length)

#define ELMT_CONTENT(ELMT) \
   (((Phg_elmt_info *) (ELMT)->eldata.ptr) + 1)

#define PHG_LOCAL_TRAN3(ELMT) \
   ((Plocal_tran3 *) ELMT_CONTENT(ELMT))

#define PHG_INT(ELMT) \
   *((Pint *) ELMT_CONTENT(ELMT))

#define PHG_FLOAT(ELMT) \
   *((Pfloat *) ELMT_CONTENT(ELMT))

#define PHG_IN_RANGE( low, high, val) \
   ((val) >= (low) && (val) <= (high))

#define PHG_ECHO_AREA_VALID( _ev ) \
   (   (_ev)->x_min < (_ev)->x_max \
    && (_ev)->y_min < (_ev)->y_max \
   )

#define PHG_ECHO_VOLUME_VALID( _ev ) \
   (   (_ev)->x_min < (_ev)->x_max \
    && (_ev)->y_min < (_ev)->y_max \
    && (_ev)->z_min <= (_ev)->z_max \
   )

#define PHG_ECHO_AREA_IN_RANGE( _ea, _dc ) \
   (   PHG_IN_RANGE( 0.0, (_dc)[0], (_ea)->x_min) \
    && PHG_IN_RANGE( 0.0, (_dc)[0], (_ea)->x_max) \
    && PHG_IN_RANGE( 0.0, (_dc)[1], (_ea)->y_min) \
    && PHG_IN_RANGE( 0.0, (_dc)[1], (_ea)->y_max) \
   )

#define PHG_ECHO_VOLUME_IN_RANGE( _ea, _dc ) \
   (   PHG_IN_RANGE( 0.0, (_dc)[0], (_ea)->x_min) \
    && PHG_IN_RANGE( 0.0, (_dc)[0], (_ea)->x_max) \
    && PHG_IN_RANGE( 0.0, (_dc)[1], (_ea)->y_min) \
    && PHG_IN_RANGE( 0.0, (_dc)[1], (_ea)->y_max) \
    && PHG_IN_RANGE( 0.0, (_dc)[2], (_ea)->z_min) \
    && PHG_IN_RANGE( 0.0, (_dc)[2], (_ea)->z_max) \
   )

/*******************************************************************************
 * phg_int_in_list
 *
 * DESCR:       Find out if integer values is stored in list
 * RETURNS:     TRUE or FALSE
 */

int phg_int_in_list(
   Pint v,
   Pint num,
   Pint *list
   );

/*******************************************************************************
 * phg_add_el
 *
 * DESCR:       Add an element and update workstations posted to
 * RETURNS:     N/A
 */

void phg_add_el(
   Css_handle cssh,
   Phg_args_add_el *args
   );

/*******************************************************************************
 * phg_del_el
 *
 * DESCR:       Delete element and update workstations posted to
 * RETURNS:     N/A
 */

void phg_del_el(
   Css_handle cssh,
   Phg_args_del_el *args
   );

/*******************************************************************************
 * phg_close_struct
 *
 * DESCR:       Close structure and update workstations posted to
 * RETURNS:     N/A
 */

void phg_close_struct(
   Css_handle cssh
   );

/*******************************************************************************
 * phg_change_struct_id
 *
 * DESCR:       Change structure id
 * RETURNS:     N/A
 */

void phg_change_struct_id(
   Css_handle cssh,
   Phg_args_change_struct *args
   );

/*******************************************************************************
 * phg_change_struct_refs
 *
 * DESCR:       Change structure references
 * RETURNS:     N/A
 */

void phg_change_struct_refs(
   Css_handle cssh,
   Phg_args_change_struct *args
   );

/*******************************************************************************
 * phg_change_struct_idrefs
 *
 * DESCR:       Change structure ids and references
 * RETURNS:     N/A
 */

void phg_change_struct_idrefs(
   Css_handle cssh,
   Phg_args_change_struct *args
   );

/*******************************************************************************
 * phg_del_struct
 *
 * DESCR:       Delete structure and update all workstations posted to
 * RETURNS:     N/A
 */

void phg_del_struct(
   Css_handle cssh,
   Pint struct_id
   );

/*******************************************************************************
 * phg_del_struct_net
 *
 * DESCR:       Delete structure network and update all workstations posted to
 * RETURNS:     N/A
 */

void phg_del_struct_net(
   Css_handle cssh,
   Phg_args_del_struct_net *args
   );

/*******************************************************************************
 * phg_del_all_structs
 *
 * DESCR:       Delete all structures and update all workstations posted to
 * RETURNS:     N/A
 */

void phg_del_all_structs(
   Css_handle cssh
   );

/*******************************************************************************
 * phg_set_edit_mode
 *
 * DESCR:       Set structure edit mode
 * RETURNS:     N/A
 */

void phg_set_edit_mode(
   Css_handle cssh,
   Pedit_mode edit_mode
   );

/*******************************************************************************
 * phg_set_el_ptr
 *
 * DESCR:       Set element pointer and update workstations
 * RETURNS:     N/A
 */

void phg_set_el_ptr(
   Css_handle cssh,
   Phg_args_set_el_ptr *args
   );

/*******************************************************************************
 * phg_copy_all_els
 *
 * DESCR:       Copy all elements in structure and update workstations
 * RETURNS:     N/A
 */

void phg_copy_all_els(
   Css_handle cssh,
   Pint struct_id
   );

/*******************************************************************************
 * phg_get_local_tran3
 *
 * DESCR:       Get local transformation 3D
 * RETURNS:     N/A
 */

void phg_get_local_tran3(
   Plocal_tran3 *tran,
   void *elmt
   );

/*******************************************************************************
 * phg_get_colr_ind
 *
 * DESCR:       Get colour from index
 * RETURNS:     N/A
 */

void phg_get_colr_ind(
   Ws *ws,
   Pgcolr *gcolr,
   Pint ind
   );

/*******************************************************************************
 * phg_entry_check
 *
 * DESCR:       Helper function to check entry
 * RETURNS:     Zero on error, otherwise non-zero
 */

int phg_entry_check(
   Err_handle erh,
   int err,
   int fn_id
   );

/*******************************************************************************
 * phg_ws_open
 *
 * DESCR:       Helper function to get workstation information
 * RETURNS:     Pointer to workstation info
 */

Psl_ws_info* phg_ws_open(
   Pint ws_id,
   Pint fn_id
   );

/*******************************************************************************
 * phg_set_ws_filter
 *
 * DESCR:       Set workstation filter
 * RETURNS:     N/A
 */

void phg_set_ws_filter(
   Pint ws_id,
   Pint fn_id,
   Phg_args_flt_type type,
   Pfilter *filter
   );

/*******************************************************************************
 * phg_echo_limits_valid
 *
 * DESCR:       Helper function to check if echo volume limits are valid
 * RETURNS:     TRUE or FALSE
 */

int phg_echo_limits_valid(
   Pint ws_id,
   Pint fn_id,
   Plimit3 *e_volume,
   Wst_phigs_dt *dt
   );

extern struct _Pstore *phg_store_list;
extern char phg_default_window_name[];
extern char phg_default_icon_name[];
extern Phg_conv phg_swap_tbl[];

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _phgP_h */

