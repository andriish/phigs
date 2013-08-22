/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2011 - 2013 Surplus Users Ham Society
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
#include <phigs/phg.h>
#include <phigs/css.h>
#include <phigs/private/phgP.h>
#include <phigs/private/hdlP.h>

/*******************************************************************************
 * hdl_create
 *
 * DESCR:	Create new element handle
 * RETURNS:	Pointer to element head or NULL
 */

Phg_elmt_info* hdl_create(
   void **data,
   caddr_t argdata
   )
{
   Phg_elmt_info *head;

   head = (Phg_elmt_info *) malloc(ARGS_ELMT_SIZE_FULL(argdata));
   if (head != NULL) {
      head->elementType = ARGS_ELMT_TYPE(argdata);
      head->length = ARGS_ELMT_SIZE(argdata);
      *data = &head[1];
   }

   return head;
}

/*******************************************************************************
 * hdl_resize
 *
 * DESCR:	Change size element handle
 * RETURNS:	Pointer to element head or NULL
 */

Phg_elmt_info* hdl_resize(
   void *buf,
   void **data,
   caddr_t argdata
   )
{
   Phg_elmt_info *head;

   head = (Phg_elmt_info *) realloc(buf, ARGS_ELMT_SIZE_FULL(argdata));
   if (head != NULL) {
      head->elementType = ARGS_ELMT_TYPE(argdata);
      head->length = ARGS_ELMT_SIZE(argdata);
      *data = &head[1];
   }

   return head;
}

/*******************************************************************************
 * hdl_dup
 *
 * DESCR:	Duplicate element handle
 * RETURNS:	Pointer to element head or NULL
 */

Phg_elmt_info* hdl_dup(
   caddr_t argdata
   )
{
   Phg_elmt_info *head;

   head = (Phg_elmt_info *) malloc(ARGS_COPY_DATA_LEN(argdata));
   if (head != NULL) {
      memcpy(head,
             ARGS_COPY_DATA(argdata),
             ARGS_COPY_DATA_LEN(argdata));
   }

   return head;
}

/*******************************************************************************
 * hdl_int_list
 *
 * DESCR:	Handle list of integers
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int hdl_int_list(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Pint *data;

   switch (op) {
      case CSS_EL_CREATE:
         ELMT_HEAD(elmt) = hdl_create((void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         *data = ARGS_ELMT_DATA(argdata).int_list.num_ints;
         memcpy(&data[1],
                ARGS_ELMT_DATA(argdata).int_list.ints,
                sizeof(Pint) * ARGS_ELMT_DATA(argdata).int_list.num_ints);
         break;

      case CSS_EL_REPLACE:
         ELMT_HEAD(elmt) = hdl_resize(ELMT_HEAD(elmt),
                                      (void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         *data = ARGS_ELMT_DATA(argdata).int_list.num_ints;
         memcpy(&data[1],
                ARGS_ELMT_DATA(argdata).int_list.ints,
                sizeof(Pint) * ARGS_ELMT_DATA(argdata).int_list.num_ints);
         break;

      case CSS_EL_COPY:
         ELMT_HEAD(elmt) = hdl_dup(argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         break;

      case CSS_EL_INQ_CONTENT:
         ARGS_INQ_HEAD(argdata) = ELMT_HEAD(elmt);
         break;

      case CSS_EL_INQ_TYPE_SIZE:
         ARGS_INQ_SIZE(argdata) = ELMT_INFO_LEN(elmt);
         break;

      case CSS_EL_FREE:
         free(ELMT_HEAD(elmt));
         break;

      default:
         /* Default */
         return (FALSE);
         break;
   }

   return (TRUE);
}

/*******************************************************************************
 * hdl_asf_info
 *
 * DESCR:	Handle asf info
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int hdl_asf_info(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Pasf_info *data;

   switch (op) {
      case CSS_EL_CREATE:
         ELMT_HEAD(elmt) = hdl_create((void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         memcpy(data,
                &ARGS_ELMT_DATA(argdata).asf_info,
                ARGS_ELMT_SIZE(argdata));
         break;

      case CSS_EL_REPLACE:
         memcpy(ELMT_CONTENT(elmt),
                &ARGS_ELMT_DATA(argdata).asf_info,
                ARGS_ELMT_SIZE(argdata));
         break;

      case CSS_EL_COPY:
         ELMT_HEAD(elmt) = hdl_dup(argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         break;

      case CSS_EL_INQ_CONTENT:
         ARGS_INQ_HEAD(argdata) = ELMT_HEAD(elmt);
         break;

      case CSS_EL_INQ_TYPE_SIZE:
         ARGS_INQ_SIZE(argdata) = ELMT_INFO_LEN(elmt);
         break;

      case CSS_EL_FREE:
         free(ELMT_HEAD(elmt));
         break;

      default:
         /* Default */
         return (FALSE);
         break;
   }

   return (TRUE);
}

/*******************************************************************************
 * hdl_vec
 *
 * DESCR:	Handle vector
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int hdl_vec(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Pvec *data;

   switch (op) {
      case CSS_EL_CREATE:
         ELMT_HEAD(elmt) = hdl_create((void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         memcpy(data,
                &ARGS_ELMT_DATA(argdata).vec,
                ARGS_ELMT_SIZE(argdata));
         break;

      case CSS_EL_REPLACE:
         memcpy(ELMT_CONTENT(elmt),
                &ARGS_ELMT_DATA(argdata).vec,
                ARGS_ELMT_SIZE(argdata));
         break;

      case CSS_EL_COPY:
         ELMT_HEAD(elmt) = hdl_dup(argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         break;

      case CSS_EL_INQ_CONTENT:
         ARGS_INQ_HEAD(argdata) = ELMT_HEAD(elmt);
         break;

      case CSS_EL_INQ_TYPE_SIZE:
         ARGS_INQ_SIZE(argdata) = ELMT_INFO_LEN(elmt);
         break;

      case CSS_EL_FREE:
         free(ELMT_HEAD(elmt));
         break;

      default:
         /* Default */
         return (FALSE);
         break;
   }

   return (TRUE);
}

/*******************************************************************************
 * hdl_local_tran
 *
 * DESCR:	Handle local transformation
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int hdl_local_tran(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Plocal_tran *data;

   switch (op) {
      case CSS_EL_CREATE:
         ELMT_HEAD(elmt) = hdl_create((void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         data->compose_type = ARGS_ELMT_DATA(argdata).local_tran.compose_type;
         phg_mat_copy_3x3(data->matrix,
                          ARGS_ELMT_DATA(argdata).local_tran.matrix);
         break;

      case CSS_EL_REPLACE:
         data = (Plocal_tran *) ELMT_CONTENT(elmt);
         data->compose_type = ARGS_ELMT_DATA(argdata).local_tran.compose_type;
         phg_mat_copy_3x3(data->matrix,
                          ARGS_ELMT_DATA(argdata).local_tran.matrix);
         break;

      case CSS_EL_COPY:
         /* TODO */
         return (FALSE);
         break;

      case CSS_EL_INQ_CONTENT:
         ARGS_INQ_HEAD(argdata) = ELMT_HEAD(elmt);
         break;

      case CSS_EL_INQ_TYPE_SIZE:
         ARGS_INQ_SIZE(argdata) = ELMT_INFO_LEN(elmt);
         break;

      case CSS_EL_FREE:
         free(ELMT_HEAD(elmt));
         break;

      default:
         /* Default */
         return (FALSE);
         break;
   }

   return (TRUE);
}

/*******************************************************************************
 * hdl_local_tran3
 *
 * DESCR:	Handle local transformation 3D
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int hdl_local_tran3(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Plocal_tran3 *data;

   switch (op) {
      case CSS_EL_CREATE:
         ELMT_HEAD(elmt) = hdl_create((void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         memcpy(data,
                &ARGS_ELMT_DATA(argdata).local_tran3,
                ARGS_ELMT_SIZE(argdata));
         break;

      case CSS_EL_REPLACE:
         memcpy(ELMT_CONTENT(elmt),
                &ARGS_ELMT_DATA(argdata).local_tran3,
                ARGS_ELMT_SIZE(argdata));
         break;

      case CSS_EL_COPY:
         ELMT_HEAD(elmt) = hdl_dup(argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         break;

      case CSS_EL_INQ_CONTENT:
         ARGS_INQ_HEAD(argdata) = ELMT_HEAD(elmt);
         break;

      case CSS_EL_INQ_TYPE_SIZE:
         ARGS_INQ_SIZE(argdata) = ELMT_INFO_LEN(elmt);
         break;

      case CSS_EL_FREE:
         free(ELMT_HEAD(elmt));
         break;

      default:
         /* Default */
         return (FALSE);
         break;
   }

   return (TRUE);
}

/*******************************************************************************
 * hdl_matrix
 *
 * DESCR:	Handle matrix
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int hdl_matrix(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Pmatrix *data;

   switch (op) {
      case CSS_EL_CREATE:
         ELMT_HEAD(elmt) = hdl_create((void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         phg_mat_copy_3x3(*data, ARGS_ELMT_DATA(argdata).global_tran);
         break;

      case CSS_EL_REPLACE:
         data = (Pmatrix *) ELMT_CONTENT(elmt);
         phg_mat_copy_3x3(*data, ARGS_ELMT_DATA(argdata).global_tran);
         break;

      case CSS_EL_COPY:
         /* TODO */
         return (FALSE);
         break;

      case CSS_EL_INQ_CONTENT:
         ARGS_INQ_HEAD(argdata) = ELMT_HEAD(elmt);
         break;

      case CSS_EL_INQ_TYPE_SIZE:
         ARGS_INQ_SIZE(argdata) = ELMT_INFO_LEN(elmt);
         break;

      case CSS_EL_FREE:
         free(ELMT_HEAD(elmt));
         break;

      default:
         /* Default */
         return (FALSE);
         break;
   }

   return (TRUE);
}

/*******************************************************************************
 * hdl_matrix3
 *
 * DESCR:	Handle matrix 3D
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int hdl_matrix3(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Pmatrix3 *data;

   switch (op) {
      case CSS_EL_CREATE:
         ELMT_HEAD(elmt) = hdl_create((void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         memcpy(data,
                &ARGS_ELMT_DATA(argdata).global_tran3,
                ARGS_ELMT_SIZE(argdata));
         break;

      case CSS_EL_REPLACE:
         memcpy(ELMT_CONTENT(elmt),
                &ARGS_ELMT_DATA(argdata).global_tran3,
                ARGS_ELMT_SIZE(argdata));
         break;

      case CSS_EL_COPY:
         ELMT_HEAD(elmt) = hdl_dup(argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         break;

      case CSS_EL_INQ_CONTENT:
         ARGS_INQ_HEAD(argdata) = ELMT_HEAD(elmt);
         break;

      case CSS_EL_INQ_TYPE_SIZE:
         ARGS_INQ_SIZE(argdata) = ELMT_INFO_LEN(elmt);
         break;

      case CSS_EL_FREE:
         free(ELMT_HEAD(elmt));
         break;

      default:
         /* Default */
         return (FALSE);
         break;
   }

   return (TRUE);
}

/*******************************************************************************
 * hdl_point_list
 *
 * DESCR:	Handle point list
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int hdl_point_list(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Pint *data;

   switch (op) {
      case CSS_EL_CREATE:
         ELMT_HEAD(elmt) = hdl_create((void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         *data = ARGS_ELMT_DATA(argdata).point_list.num_points;
         memcpy(&data[1],
                ARGS_ELMT_DATA(argdata).point_list.points,
                sizeof(Ppoint) *
                   ARGS_ELMT_DATA(argdata).point_list.num_points);
         break;

      case CSS_EL_REPLACE:
         ELMT_HEAD(elmt) = hdl_resize(ELMT_HEAD(elmt),
                                      (void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         *data = ARGS_ELMT_DATA(argdata).point_list.num_points;
         memcpy(&data[1],
                ARGS_ELMT_DATA(argdata).point_list.points,
                sizeof(Ppoint) *
                   ARGS_ELMT_DATA(argdata).point_list.num_points);
         break;

      case CSS_EL_COPY:
         ELMT_HEAD(elmt) = hdl_dup(argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         break;

      case CSS_EL_INQ_CONTENT:
         ARGS_INQ_HEAD(argdata) = ELMT_HEAD(elmt);
         break;

      case CSS_EL_INQ_TYPE_SIZE:
         ARGS_INQ_SIZE(argdata) = ELMT_INFO_LEN(elmt);
         break;

      case CSS_EL_FREE:
         free(ELMT_HEAD(elmt));
         break;

      default:
         /* Default */
         return (FALSE);
         break;
   }

   return (TRUE);
}

/*******************************************************************************
 * hdl_point_list3
 *
 * DESCR:	Handle point list 3D
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int hdl_point_list3(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Pint *data;

   switch (op) {
      case CSS_EL_CREATE:
         ELMT_HEAD(elmt) = hdl_create((void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         *data = ARGS_ELMT_DATA(argdata).point_list3.num_points;
         memcpy(&data[1],
                ARGS_ELMT_DATA(argdata).point_list3.points,
                sizeof(Ppoint3) *
                   ARGS_ELMT_DATA(argdata).point_list3.num_points);
         break;

      case CSS_EL_REPLACE:
         ELMT_HEAD(elmt) = hdl_resize(ELMT_HEAD(elmt),
                                      (void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         *data = ARGS_ELMT_DATA(argdata).point_list3.num_points;
         memcpy(&data[1],
                ARGS_ELMT_DATA(argdata).point_list3.points,
                sizeof(Ppoint3) *
                   ARGS_ELMT_DATA(argdata).point_list3.num_points);
         break;

      case CSS_EL_COPY:
         ELMT_HEAD(elmt) = hdl_dup(argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         break;

      case CSS_EL_INQ_CONTENT:
         ARGS_INQ_HEAD(argdata) = ELMT_HEAD(elmt);
         break;

      case CSS_EL_INQ_TYPE_SIZE:
         ARGS_INQ_SIZE(argdata) = ELMT_INFO_LEN(elmt);
         break;

      case CSS_EL_FREE:
         free(ELMT_HEAD(elmt));
         break;

      default:
         /* Default */
         return (FALSE);
         break;
   }

   return (TRUE);
}

/*******************************************************************************
 * hdl_point_list_list
 *
 * DESCR:	Handle list of point lists
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int hdl_point_list_list(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Pint i, num_lists;
   Ppoint_list point_list;
   Pint *data;

   switch (op) {
      case CSS_EL_CREATE:
         ELMT_HEAD(elmt) = hdl_create((void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         num_lists = ARGS_ELMT_DATA(argdata).point_list_list.num_point_lists;
         *data = num_lists;
         data = &data[1];
         for (i = 0; i < num_lists; i++) {
            point_list.num_points =
             ARGS_ELMT_DATA(argdata).point_list_list.point_lists[i].num_points;
            *data = point_list.num_points;
            point_list.points = (Ppoint *) &data[1];
            memcpy(point_list.points,
                 ARGS_ELMT_DATA(argdata).point_list_list.point_lists[i].points,
                   sizeof(Ppoint) * point_list.num_points);
            data = (Pint *) &point_list.points[point_list.num_points];
         }
         break;

      case CSS_EL_REPLACE:
         ELMT_HEAD(elmt) = hdl_resize(ELMT_HEAD(elmt),
                                      (void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         num_lists = ARGS_ELMT_DATA(argdata).point_list_list.num_point_lists;
         *data = num_lists;
         data = &data[1];
         for (i = 0; i < num_lists; i++) {
            point_list.num_points =
             ARGS_ELMT_DATA(argdata).point_list_list.point_lists[i].num_points;
            *data = point_list.num_points;
            point_list.points = (Ppoint *) &data[1];
            memcpy(point_list.points,
                 ARGS_ELMT_DATA(argdata).point_list_list.point_lists[i].points,
                   sizeof(Ppoint) * point_list.num_points);
            data = (Pint *) &point_list.points[point_list.num_points];
         }
         break;

      case CSS_EL_COPY:
         ELMT_HEAD(elmt) = hdl_dup(argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         break;

      case CSS_EL_INQ_CONTENT:
         ARGS_INQ_HEAD(argdata) = ELMT_HEAD(elmt);
         break;

      case CSS_EL_INQ_TYPE_SIZE:
         ARGS_INQ_SIZE(argdata) = ELMT_INFO_LEN(elmt);
         break;

      case CSS_EL_FREE:
         free(ELMT_HEAD(elmt));
         break;

      default:
         /* Default */
         return (FALSE);
         break;
   }

   return (TRUE);
}

/*******************************************************************************
 * hdl_point_list_list3
 *
 * DESCR:       Handle list of point lists 3D
 * RETURNS:     TRUE on success, otherwise FALSE
 */

int hdl_point_list_list3(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Pint i, num_lists;
   Ppoint_list3 point_list;
   Pint *data;

   switch (op) {
      case CSS_EL_CREATE:
         ELMT_HEAD(elmt) = hdl_create((void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         num_lists = ARGS_ELMT_DATA(argdata).point_list_list3.num_point_lists;
         *data = num_lists;
         data = &data[1];
         for (i = 0; i < num_lists; i++) {
            point_list.num_points =
             ARGS_ELMT_DATA(argdata).point_list_list3.point_lists[i].num_points;
            *data = point_list.num_points;
            point_list.points = (Ppoint3 *) &data[1];
            memcpy(point_list.points,
                 ARGS_ELMT_DATA(argdata).point_list_list3.point_lists[i].points,
                   sizeof(Ppoint3) * point_list.num_points);
            data = (Pint *) &point_list.points[point_list.num_points];
         }
         break;

      case CSS_EL_REPLACE:
         ELMT_HEAD(elmt) = hdl_resize(ELMT_HEAD(elmt),
                                      (void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         num_lists = ARGS_ELMT_DATA(argdata).point_list_list3.num_point_lists;
         *data = num_lists;
         data = &data[1];
         for (i = 0; i < num_lists; i++) {
            point_list.num_points =
             ARGS_ELMT_DATA(argdata).point_list_list3.point_lists[i].num_points;
            *data = point_list.num_points;
            point_list.points = (Ppoint3 *) &data[1];
            memcpy(point_list.points,
                 ARGS_ELMT_DATA(argdata).point_list_list3.point_lists[i].points,
                   sizeof(Ppoint3) * point_list.num_points);
            data = (Pint *) &point_list.points[point_list.num_points];
         }
         break;

      case CSS_EL_COPY:
         ELMT_HEAD(elmt) = hdl_dup(argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         break;

      case CSS_EL_INQ_CONTENT:
         ARGS_INQ_HEAD(argdata) = ELMT_HEAD(elmt);
         break;

      case CSS_EL_INQ_TYPE_SIZE:
         ARGS_INQ_SIZE(argdata) = ELMT_INFO_LEN(elmt);
         break;

      case CSS_EL_FREE:
         free(ELMT_HEAD(elmt));
         break;

      default:
         /* Default */
         return (FALSE);
         break;
   }

   return (TRUE);
}

/*******************************************************************************
 * hdl_text_align
 *
 * DESCR:	Handle text alignment
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int hdl_text_align(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Ptext_align *data;

   switch (op) {
      case CSS_EL_CREATE:
         ELMT_HEAD(elmt) = hdl_create((void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         memcpy(data,
                &ARGS_ELMT_DATA(argdata).text_align,
                ARGS_ELMT_SIZE(argdata));
         elmt->eldata.ptr = data;
         break;

      case CSS_EL_REPLACE:
         memcpy(ELMT_CONTENT(elmt),
                &ARGS_ELMT_DATA(argdata).text_align,
                ARGS_ELMT_SIZE(argdata));
         break;

      case CSS_EL_COPY:
         ELMT_HEAD(elmt) = hdl_dup(argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         break;

      case CSS_EL_INQ_CONTENT:
         ARGS_INQ_HEAD(argdata) = ELMT_HEAD(elmt);
         break;

      case CSS_EL_INQ_TYPE_SIZE:
         ARGS_INQ_SIZE(argdata) = ELMT_INFO_LEN(elmt);
         break;

      case CSS_EL_FREE:
         free(ELMT_HEAD(elmt));
         break;

      default:
         /* Default */
         return (FALSE);
         break;
   }

   return (TRUE);
}

/*******************************************************************************
 * hdl_text
 *
 * DESCR:	Handle text
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int hdl_text(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Ppoint *data;

   switch (op) {
      case CSS_EL_CREATE:
         ELMT_HEAD(elmt) = hdl_create((void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         memcpy(data, &ARGS_ELMT_DATA(argdata).text.pos, sizeof(Ppoint));
         strcpy((char *) &data[1], ARGS_ELMT_DATA(argdata).text.char_string);
         break;

      case CSS_EL_REPLACE:
         ELMT_HEAD(elmt) = hdl_resize(ELMT_HEAD(elmt),
                                      (void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         memcpy(data, &ARGS_ELMT_DATA(argdata).text.pos, sizeof(Ppoint));
         strcpy((char *) &data[1], ARGS_ELMT_DATA(argdata).text.char_string);
         break;

      case CSS_EL_COPY:
         ELMT_HEAD(elmt) = hdl_dup(argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         break;

      case CSS_EL_INQ_CONTENT:
         ARGS_INQ_HEAD(argdata) = ELMT_HEAD(elmt);
         break;

      case CSS_EL_INQ_TYPE_SIZE:
         ARGS_INQ_SIZE(argdata) = ELMT_INFO_LEN(elmt);
         break;

      case CSS_EL_FREE:
         free(ELMT_HEAD(elmt));
         break;

      default:
         /* Default */
         return (FALSE);
         break;
   }

   return (TRUE);
}

/*******************************************************************************
 * hdl_int
 *
 * DESCR:	Handle integer data
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int hdl_int(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Pint *data;

   switch (op) {
      case CSS_EL_CREATE:
         ELMT_HEAD(elmt) = hdl_create((void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         *data = ARGS_ELMT_DATA(argdata).int_data;
         break;

      case CSS_EL_REPLACE:
         data = (Pint *) ELMT_CONTENT(elmt);
         *data = ARGS_ELMT_DATA(argdata).int_data;
         break;

      case CSS_EL_COPY:
         ELMT_HEAD(elmt) = hdl_dup(argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         break;

      case CSS_EL_INQ_CONTENT:
         ARGS_INQ_HEAD(argdata) = ELMT_HEAD(elmt);
         break;

      case CSS_EL_INQ_TYPE_SIZE:
         ARGS_INQ_SIZE(argdata) = ELMT_INFO_LEN(elmt);
         break;

      case CSS_EL_FREE:
         free(ELMT_HEAD(elmt));
         break;

      default:
         /* Default */
         return (FALSE);
         break;
   }

   return (TRUE);
}

/*******************************************************************************
 * hdl_float
 *
 * DESCR:	Handle float data
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int hdl_float(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Pfloat *data;

   switch (op) {
      case CSS_EL_CREATE:
         elmt->eldata.ptr = hdl_create((void *) &data, argdata);
         if (elmt->eldata.ptr == NULL) {
            return (FALSE);
         }
         *data = ARGS_ELMT_DATA(argdata).float_data;
         break;

      case CSS_EL_REPLACE:
         data = (Pfloat *) ELMT_CONTENT(elmt);
         *data = ARGS_ELMT_DATA(argdata).float_data;
         break;

      case CSS_EL_COPY:
         ELMT_HEAD(elmt) = hdl_dup(argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }
         break;

      case CSS_EL_INQ_CONTENT:
         ARGS_INQ_HEAD(argdata) = ELMT_HEAD(elmt);
         break;

      case CSS_EL_INQ_TYPE_SIZE:
         ARGS_INQ_SIZE(argdata) = ELMT_INFO_LEN(elmt);
         break;

      case CSS_EL_FREE:
         free(ELMT_HEAD(elmt));
         break;

      default:
         /* Default */
         return (FALSE);
         break;
   }

   return (TRUE);
}
