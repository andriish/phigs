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

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "phg.h"
#include "private/cbP.h"

struct _Pstore *phg_cb_store_list = (struct _Pstore *) NULL;

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
   )
{
   struct _Pstore old_store;

   *err_ind = 0;
   if (store != NULL) {
      old_store.buf = store->buf;
      if (size > 0 && (store->buf = malloc(size)) == NULL) {
         *err_ind = ERR900;
         store->buf = old_store.buf;
      }
      else {
         if (store->size > 0) {
            free(old_store.buf);
         }
         store->size = size;
      }
   }

   return (*err_ind ? FALSE : TRUE);
}

/*******************************************************************************
 * phg_cb_destroy_all_stores
 *
 * DESCR:       Destroy all storage objects
 * RETURNS:     N/A
 */

void phg_cb_destroy_all_stores(
   void
   )
{
   Pstore node;
   Pstore next = NULL;

   for (node = phg_cb_store_list; node != NULL; node = next) {
      node = node->next;
      if (node->size > 0) {
         free(node->buf);
      }
      free(node);
   }
}

/*******************************************************************************
 * phg_cb_store_int
 *
 * DESCR:	Transfer integer to add element structure
 * RETURNS:	TRUE or FALSE
 */

int phg_cb_store_int(
   Phg_args_add_el *args,
   Pelem_type type,
   Pint int_data
   )
{
   args->el_type = type;
   args->el_size = sizeof(uint32_t);
   if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args->el_size)) {
      return (FALSE);
   }
   else {
      args->el_data = PHG_SCRATCH.buf;
      *((uint32_t *) args->el_data) = (uint32_t) int_data;
      return (TRUE);
   }
}

/*******************************************************************************
 * phg_cb_store_int2
 *
 * DESCR:	Transfer 2 integers to add element structure
 * RETURNS:	TRUE or FALSE
 */

int phg_cb_store_int2(
   Phg_args_add_el *args,
   Pelem_type type,
   Pint int_data1,
   Pint int_data2
   )
{
   uint32_t *idata;

   args->el_type = type;
   args->el_size = 2 * sizeof(uint32_t);
   if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args->el_size)) {
      return (FALSE);
   }
   else {
      args->el_data = PHG_SCRATCH.buf;
      idata = (uint32_t *) args->el_data;
      idata[0] = (uint32_t) int_data1;
      idata[1] = (uint32_t) int_data2;
      return (TRUE);
   }
}

/*******************************************************************************
 * phg_cb_store_float
 *
 * DESCR:	Transfer floating point number to add element structure
 * RETURNS:	TRUE or FALSE
 */

int phg_cb_store_float(
   Phg_args_add_el *args,
   Pelem_type type,
   Pfloat float_data
   )
{
   args->el_type = type;
   args->el_size = sizeof(float);
   if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args->el_size)) {
      return (FALSE);
   }
   else {
      args->el_data = PHG_SCRATCH.buf;
      *((float *) args->el_data) = (float) float_data;
      return (TRUE);
   }
}

/*******************************************************************************
 * phg_cb_store_float2
 *
 * DESCR:	Transfer 2 floating point numbers to add element structure
 * RETURNS:	TRUE or FALSE
 */

int phg_cb_store_float2(
   Phg_args_add_el *args,
   Pelem_type type,
   Pfloat float_data1,
   Pfloat float_data2
   )
{
   float *fdata;

   args->el_type = type;
   args->el_size = 2 * sizeof(float);
   if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args->el_size)) {
      return (FALSE);
   }
   else {
      args->el_data = PHG_SCRATCH.buf;
      fdata = (float *) args->el_data;
      fdata[0] = float_data1;
      fdata[1] = float_data2;
      return (TRUE);
   }
}

/*******************************************************************************
 * phg_cb_store_point_string
 *
 * DESCR:	Transfer a point and a string to add element structure
 * RETURNS:	TRUE or FALSE
 */

int phg_cb_store_point_string(
   Phg_args_add_el *args,
   Pelem_type type,
   Ppoint *point,
   char *str
   )
{
   float *fdata;

   args->el_type = type;
   args->el_size = 2 * sizeof(float) + strlen(str) + 1;
   if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args->el_size)) {
      return (FALSE);
   }
   else {
      args->el_data = PHG_SCRATCH.buf;
      fdata = (float *) args->el_data;
      *fdata++ = point->x;
      *fdata++ = point->y;
      strcpy((char *) fdata, str);
      return (TRUE);
   }
}

/*******************************************************************************
 * phg_cb_store_matrix3
 *
 * DESCR:	Transfer matrix 3D to add element structure
 * RETURNS:	TRUE or FALSE
 */

int phg_cb_store_matrix3(
   Phg_args_add_el *args,
   Pelem_type type,
   Pmatrix3 mat
   )
{
   int i, j;
   float *fdata;

   args->el_type = type;
   args->el_size = 16 * sizeof(float);
   if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args->el_size)) {
      return (FALSE);
   }
   else {
      args->el_data = PHG_SCRATCH.buf;
      fdata = (float *) args->el_data;
      for (i = 0; i < 4; i++) {
         for (j = 0; j < 4; j++) {
            *fdata++ = mat[i][j];
         }
      }
      return (TRUE);
   }
}

/*******************************************************************************
 * phg_cb_store_int_matrix3
 *
 * DESCR:	Transfer integet and matrix 3D to add element structure
 * RETURNS:	TRUE or FALSE
 */

int phg_cb_store_int_matrix3(
   Phg_args_add_el *args,
   Pelem_type type,
   Pint int_data,
   Pmatrix3 mat
   )
{
   int i, j;
   uint32_t *idata;
   float *fdata;

   args->el_type = type;
   args->el_size = sizeof(uint32_t) + 16 * sizeof(float);
   if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args->el_size)) {
      return (FALSE);
   }
   else {
      args->el_data = PHG_SCRATCH.buf;
      idata = (uint32_t *) args->el_data;
      *idata = (uint32_t) int_data;
      fdata = (float *) &idata[1];
      for (i = 0; i < 4; i++) {
         for (j = 0; j < 4; j++) {
            *fdata++ = mat[i][j];
         }
      }
      return (TRUE);
   }
}

/*******************************************************************************
 * phg_cb_store_int_list
 *
 * DESCR:	Transfer integer list to add element structure
 * RETURNS:	TRUE or FALSE
 */

int phg_cb_store_int_list(
   Phg_args_add_el *args,
   Pelem_type type,
   Pint_list *int_list
   )
{
   int i;
   uint32_t *idata;

   args->el_type = type;
   args->el_size = sizeof(uint32_t) + sizeof(float) * int_list->num_ints;
   if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args->el_size)) {
      return (FALSE);
   }
   else {
      args->el_data = PHG_SCRATCH.buf;
      idata = (uint32_t *) args->el_data;
      *idata = (uint32_t) int_list->num_ints;
      idata = (uint32_t *) &idata[1];
      for (i = 0; i < int_list->num_ints; i++) {
         *idata++ = (uint32_t) int_list->ints[i];
      }
      return (TRUE);
   }
}

/*******************************************************************************
 * phg_cb_store_point_list
 *
 * DESCR:	Transfer point list to add element structure
 * RETURNS:	TRUE or FALSE
 */

int phg_cb_store_point_list(
   Phg_args_add_el *args,
   Pelem_type type,
   Ppoint_list *point_list
   )
{
   int i;
   uint32_t *idata;
   float *fdata;

   args->el_type = type;
   args->el_size = sizeof(uint32_t) + 2 * sizeof(float) * point_list->num_points;
   if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args->el_size)) {
      return (FALSE);
   }
   else {
      args->el_data = PHG_SCRATCH.buf;
      idata = (uint32_t *) args->el_data;
      *idata = (uint32_t) point_list->num_points;
      fdata = (float *) &idata[1];
      for (i = 0; i < point_list->num_points; i++) {
         *fdata++ = (float) point_list->points[i].x;
         *fdata++ = (float) point_list->points[i].y;
      }
      return (TRUE);
   }
}

/*******************************************************************************
 * phg_cb_store_point_list3
 *
 * DESCR:	Transfer point list 3D to add element structure
 * RETURNS:	TRUE or FALSE
 */

int phg_cb_store_point_list3(
   Phg_args_add_el *args,
   Pelem_type type,
   Ppoint_list3 *point_list
   )
{
   int i;
   uint32_t *idata;
   float *fdata;

   args->el_type = type;
   args->el_size = sizeof(uint32_t) + 3 * sizeof(float) * point_list->num_points;
   if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args->el_size)) {
      return (FALSE);
   }
   else {
      args->el_data = PHG_SCRATCH.buf;
      idata = (uint32_t *) args->el_data;
      *idata = (uint32_t) point_list->num_points;
      fdata = (float *) &idata[1];
      for (i = 0; i < point_list->num_points; i++) {
         *fdata++ = (float) point_list->points[i].x;
         *fdata++ = (float) point_list->points[i].y;
         *fdata++ = (float) point_list->points[i].z;
      }
      return (TRUE);
   }
}

/*******************************************************************************
 * phg_cb_store_point_list_list
 *
 * DESCR:	Transfer list of point lists to add element structure
 * RETURNS:	TRUE or FALSE
 */

int phg_cb_store_point_list_list(
   Phg_args_add_el *args,
   Pelem_type type,
   Ppoint_list_list *point_list_list
   )
{
   int i, j, num_lists, num_points;
   uint32_t *idata;
   float *fdata;

   args->el_type = type;
   num_lists = point_list_list->num_point_lists;
   for (i = 0, num_points = 0; i < num_lists; i++) {
      num_points += point_list_list->point_lists[i].num_points;
   }
   args->el_size = sizeof(uint32_t) +
      sizeof(uint32_t) * num_lists + 2 * sizeof(float) * num_points;
   if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args->el_size)) {
      return (FALSE);
   }
   else {
      args->el_data = PHG_SCRATCH.buf;
      idata = (uint32_t *) args->el_data;
      *idata = (uint32_t) num_lists;
      idata = &idata[1];
      for (i = 0; i < num_lists; i++) {
         *idata = point_list_list->point_lists[i].num_points;
         fdata = (float *) &idata[1];
         for (j = 0; j < point_list_list->point_lists[i].num_points; j++) {
            *fdata++ = (float) point_list_list->point_lists[i].points[j].x;
            *fdata++ = (float) point_list_list->point_lists[i].points[j].y;
         }
         idata = (uint32_t *) fdata;
      }
      return (TRUE);
   }
}

/*******************************************************************************
 * phg_cb_store_point_list_list3
 *
 * DESCR:	Transfer list of point lists 3D to add element structure
 * RETURNS:	TRUE or FALSE
 */

int phg_cb_store_point_list_list3(
   Phg_args_add_el *args,
   Pelem_type type,
   Ppoint_list_list3 *point_list_list
   )
{
   int i, j, num_lists, num_points;
   uint32_t *idata;
   float *fdata;

   args->el_type = type;
   num_lists = point_list_list->num_point_lists;
   for (i = 0, num_points = 0; i < num_lists; i++) {
      num_points += point_list_list->point_lists[i].num_points;
   }
   args->el_size = sizeof(uint32_t) +
      sizeof(uint32_t) * num_lists + 3 * sizeof(float) * num_points;
   if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args->el_size)) {
      return (FALSE);
   }
   else {
      args->el_data = PHG_SCRATCH.buf;
      idata = (uint32_t *) args->el_data;
      *idata = (uint32_t) num_lists;
      idata = &idata[1];
      for (i = 0; i < num_lists; i++) {
         *idata = point_list_list->point_lists[i].num_points;
         fdata = (float *) &idata[1];
         for (j = 0; j < point_list_list->point_lists[i].num_points; j++) {
            *fdata++ = (float) point_list_list->point_lists[i].points[j].x;
            *fdata++ = (float) point_list_list->point_lists[i].points[j].y;
            *fdata++ = (float) point_list_list->point_lists[i].points[j].z;
         }
         idata = (uint32_t *) fdata;
      }
      return (TRUE);
   }
}

