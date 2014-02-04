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

#ifndef _cbP_h
#define _cbP_h

#ifdef __cplusplus
extern "C" {
#endif

struct _Pstore {
   Pint size;
   union {
      Pelem_data elem_data;
   } data;
   void *buf;
   struct _Pstore *next;
};

/*******************************************************************************
 * phg_cb_resize_store
 *
 * DESCR:       Resize storage object
 * RETURNS:     TRUE or FALSE
 */

int phg_cb_resize_store(
   Pstore store,
   Pint size,
   Pint *err_ind
   );

/*******************************************************************************
 * phg_cb_destroy_all_stores
 *
 * DESCR:       Destroy all storage objects
 * RETURNS:     N/A
 */

void phg_cb_destroy_all_stores(
   void
   );

/*******************************************************************************
 * phg_cb_store_int
 *
 * DESCR:       Transfer integer to add element structure
 * RETURNS:     TRUE or FALSE
 */

int phg_cb_store_int(
   Phg_args_add_el *args,
   Pelem_type type,
   Pint int_data
   );

/*******************************************************************************
 * phg_cb_store_int2
 *
 * DESCR:       Transfer 2 integers to add element structure
 * RETURNS:     TRUE or FALSE
 */

int phg_cb_store_int2(
   Phg_args_add_el *args,
   Pelem_type type,
   Pint int_data1,
   Pint int_data2
   );

/*******************************************************************************
 * phg_cb_store_float
 *
 * DESCR:       Transfer floating point number to add element structure
 * RETURNS:     TRUE or FALSE
 */

int phg_cb_store_float(
   Phg_args_add_el *args,
   Pelem_type type,
   Pfloat float_data
   );

/*******************************************************************************
 * phg_cb_store_float2
 *
 * DESCR:       Transfer 2 floating point numbers to add element structure
 * RETURNS:     TRUE or FALSE
 */

int phg_cb_store_float2(
   Phg_args_add_el *args,
   Pelem_type type,
   Pfloat float_data1,
   Pfloat float_data2
   );

/*******************************************************************************
 * phg_cb_store_point_string
 *
 * DESCR:       Transfer a point and a string to add element structure
 * RETURNS:     TRUE or FALSE
 */

int phg_cb_store_point_string(
   Phg_args_add_el *args,
   Pelem_type type,
   Ppoint *point,
   char *str
   );

/*******************************************************************************
 * phg_cb_store_matrix3
 *
 * DESCR:       Transfer matrix 3D to add element structure
 * RETURNS:     TRUE or FALSE
 */

int phg_cb_store_matrix3(
   Phg_args_add_el *args,
   Pelem_type type,
   Pmatrix3 mat
   );

/*******************************************************************************
 * phg_cb_store_int_matrix3
 *
 * DESCR:       Transfer integet and matrix 3D to add element structure
 * RETURNS:     TRUE or FALSE
 */

int phg_cb_store_int_matrix3(
   Phg_args_add_el *args,
   Pelem_type type,
   Pint int_data,
   Pmatrix3 mat
   );

/*******************************************************************************
 * phg_cb_store_int_list
 *
 * DESCR:       Transfer integer list to add element structure
 * RETURNS:     TRUE or FALSE
 */

int phg_cb_store_int_list(
   Phg_args_add_el *args,
   Pelem_type type,
   Pint_list *int_list
   );

/*******************************************************************************
 * phg_cb_store_point_list
 *
 * DESCR:       Transfer point list to add element structure
 * RETURNS:     TRUE or FALSE
 */

int phg_cb_store_point_list(
   Phg_args_add_el *args,
   Pelem_type type,
   Ppoint_list *point_list
   );

/*******************************************************************************
 * phg_cb_store_point_list3
 *
 * DESCR:       Transfer point list 3D to add element structure
 * RETURNS:     TRUE or FALSE
 */

int phg_cb_store_point_list3(
   Phg_args_add_el *args,
   Pelem_type type,
   Ppoint_list3 *point_list
   );

/*******************************************************************************
 * phg_cb_store_point_list_list
 *
 * DESCR:       Transfer list of point lists to add element structure
 * RETURNS:     TRUE or FALSE
 */

int phg_cb_store_point_list_list(
   Phg_args_add_el *args,
   Pelem_type type,
   Ppoint_list_list *point_list_list
   );

/*******************************************************************************
 * phg_cb_store_point_list_list3
 *
 * DESCR:       Transfer list of point lists 3D to add element structure
 * RETURNS:     TRUE or FALSE
 */

int phg_cb_store_point_list_list3(
   Phg_args_add_el *args,
   Pelem_type type,
   Ppoint_list_list3 *point_list_list
   );

extern struct _Pstore *phg_cb_store_list;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _cbP_h */

