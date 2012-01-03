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

#include <stdlib.h>
#include <string.h>
#include <phigs/phg.h>
#include <phigs/css.h>
#include <phigs/private/phgP.h>
#include <phigs/private/hdlP.h>

/*******************************************************************************
 * phg_handle_names_set
 *
 * DESCR:	Handle nameset
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_names_set(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   int n;
   Pint_list *data, *names;

   switch (op) {
      case CSS_EL_CREATE:
         names = &ARGS_ELMT_DATA(argdata).names;
         n = names->num_ints;
         data = malloc(sizeof(Pint_list) + n * sizeof(Pint));
         if (data == NULL)
            return (FALSE);

         data->num_ints = n;
         data->ints = (Pint *) &data[1];
         memcpy(data->ints, names->ints, sizeof(Pint) * n);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_REPLACE:
         names = &ARGS_ELMT_DATA(argdata).names;
         n = names->num_ints;
         data = realloc(ELMT_DATA_PTR(elmt),
                        sizeof(Pint_list) + n * sizeof(Pint));
         if (data == NULL)
            return (FALSE);

         data->num_ints = n;
         data->ints = (Pint *) &data[1];
         memcpy(data->ints, names->ints, sizeof(Pint) * n);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_COPY:
         n = PHG_DATA_INT_LIST(argdata)->num_ints;
         data = malloc(sizeof(Pint_list) + n * sizeof(Pint));
         if (data == NULL)
            return (FALSE);

         data->num_ints = n;
         data->ints = (Pint *) &data[1];
         memcpy(data->ints,
                PHG_DATA_INT_LIST(argdata)->ints,
                sizeof(Pint) * n);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_FREE:
         free(ELMT_DATA_PTR(elmt));
      break;

      default:
         /* Default */
         return (FALSE);
      break;
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_local_tran
 *
 * DESCR:	Handle local transformation
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_local_tran(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Plocal_tran *data;

   switch (op) {
      case CSS_EL_CREATE:
         data = malloc(sizeof(Plocal_tran));
         if (data == NULL)
            return (FALSE);

         data->compose_type = ARGS_ELMT_DATA(argdata).local_tran.compose_type;
         phg_mat_copy_3x3(data->matrix,
                          ARGS_ELMT_DATA(argdata).local_tran.matrix);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_REPLACE:
         data = (Plocal_tran *) elmt->eldata.ptr;
         data->compose_type = ARGS_ELMT_DATA(argdata).local_tran.compose_type;
         phg_mat_copy_3x3(data->matrix,
                          ARGS_ELMT_DATA(argdata).local_tran.matrix);
      break;

      case CSS_EL_COPY:
         data = malloc(sizeof(Plocal_tran));
         if (data == NULL)
            return (FALSE);

         data->compose_type = PHG_DATA_LOCAL_TRAN(argdata)->compose_type;
         phg_mat_copy_3x3(data->matrix, PHG_DATA_LOCAL_TRAN(argdata)->matrix);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_FREE:
         free(ELMT_DATA_PTR(elmt));
      break;

      default:
         /* Default */
         return (FALSE);
      break;
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_local_tran3
 *
 * DESCR:	Handle local transformation 3D
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_local_tran3(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Plocal_tran3 *data;

   switch (op) {
      case CSS_EL_CREATE:
         data = malloc(sizeof(Plocal_tran3));
         if (data == NULL)
            return (FALSE);

         data->compose_type = ARGS_ELMT_DATA(argdata).local_tran3.compose_type;
         phg_mat_copy(data->matrix, ARGS_ELMT_DATA(argdata).local_tran3.matrix);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_REPLACE:
         data = (Plocal_tran3 *) elmt->eldata.ptr;
         data->compose_type = ARGS_ELMT_DATA(argdata).local_tran3.compose_type;
         phg_mat_copy(data->matrix, ARGS_ELMT_DATA(argdata).local_tran3.matrix);
      break;

      case CSS_EL_COPY:
         data = malloc(sizeof(Plocal_tran3));
         if (data == NULL)
            return (FALSE);

         data->compose_type = PHG_DATA_LOCAL_TRAN3(argdata)->compose_type;
         phg_mat_copy(data->matrix, PHG_DATA_LOCAL_TRAN3(argdata)->matrix);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_FREE:
         free(ELMT_DATA_PTR(elmt));
      break;

      default:
         /* Default */
         return (FALSE);
      break;
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_point_list
 *
 * DESCR:	Handle point list
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_point_list(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   int n;
   Ppoint_list *data, *point_list;

   switch (op) {
      case CSS_EL_CREATE:
         point_list = &ARGS_ELMT_DATA(argdata).point_list;
         n = point_list->num_points;
         data = malloc(sizeof(Ppoint_list) + n * sizeof(Ppoint));
         if (data == NULL)
            return (FALSE);

         data->num_points = n;
         data->points = (Ppoint *) &data[1];
         memcpy(data->points, point_list->points, sizeof(Ppoint) * n);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_REPLACE:
         point_list = &ARGS_ELMT_DATA(argdata).point_list;
         n = point_list->num_points;
         data = realloc(ELMT_DATA_PTR(elmt),
                        sizeof(Ppoint_list) + n * sizeof(Ppoint));
         if (data == NULL)
            return (FALSE);

         data->num_points = n;
         data->points = (Ppoint *) &data[1];
         memcpy(data->points, point_list->points, sizeof(Ppoint) * n);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_COPY:
         n = PHG_DATA_POINT_LIST(argdata)->num_points;
         data = malloc(sizeof(Ppoint_list) + n * sizeof(Ppoint));
         if (data == NULL)
            return (FALSE);

         data->num_points = n;
         data->points = (Ppoint *) &data[1];
         memcpy(data->points,
                PHG_DATA_POINT_LIST(argdata)->points,
                sizeof(Ppoint) * n);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_FREE:
         free(ELMT_DATA_PTR(elmt));
      break;

      default:
         /* Default */
         return (FALSE);
      break;
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_point_list3
 *
 * DESCR:	Handle point list 3D
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_point_list3(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   int n;
   Ppoint_list3 *data, *point_list;

   switch (op) {
      case CSS_EL_CREATE:
         point_list = &ARGS_ELMT_DATA(argdata).point_list3;
         n = point_list->num_points;
         data = malloc(sizeof(Ppoint_list3) + n * sizeof(Ppoint3));
         if (data == NULL)
            return (FALSE);

         data->num_points = n;
         data->points = (Ppoint3 *) &data[1];
         memcpy(data->points, point_list->points, sizeof(Ppoint3) * n);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_REPLACE:
         point_list = &ARGS_ELMT_DATA(argdata).point_list3;
         n = point_list->num_points;
         data = realloc(ELMT_DATA_PTR(elmt),
                        sizeof(Ppoint_list3) + n * sizeof(Ppoint3));
         if (data == NULL)
            return (FALSE);

         data->num_points = n;
         data->points = (Ppoint3 *) &data[1];
         memcpy(data->points, point_list->points, sizeof(Ppoint3) * n);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_COPY:
         n = PHG_DATA_POINT_LIST3(argdata)->num_points;
         data = malloc(sizeof(Ppoint_list3) + n * sizeof(Ppoint3));
         if (data == NULL)
            return (FALSE);

         data->num_points = n;
         data->points = (Ppoint3 *) &data[1];
         memcpy(data->points,
                PHG_DATA_POINT_LIST3(argdata)->points,
                sizeof(Ppoint3) * n);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_FREE:
         free(ELMT_DATA_PTR(elmt));
      break;

      default:
         /* Default */
         return (FALSE);
      break;
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_text
 *
 * DESCR:	Handle text
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_text(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   char  *str;
   Ptext *data;

   switch (op) {
      case CSS_EL_CREATE:
         str = ARGS_ELMT_DATA(argdata).text.char_string;
         data = malloc(sizeof(Ptext) + strlen(str) + 1);
         if (data == NULL)
            return (FALSE);

         memcpy(&data->pos, &ARGS_ELMT_DATA(argdata).text.pos, sizeof(Ppoint));
         data->char_string = (char *) &data[1];
         strcpy(data->char_string, str);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_REPLACE:
         str = ARGS_ELMT_DATA(argdata).text.char_string;
         data = realloc(ELMT_DATA_PTR(elmt),
                        sizeof(Ptext) + strlen(str) + 1);
         if (data == NULL)
            return (FALSE);

         memcpy(&data->pos, &ARGS_ELMT_DATA(argdata).text.pos, sizeof(Ppoint));
         data->char_string = (char *) &data[1];
         strcpy(data->char_string, str);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_COPY:
         str = PHG_DATA_TEXT(argdata)->char_string;
         data = malloc(sizeof(Ptext) + strlen(str) + 1);
         if (data == NULL)
            return (FALSE);

         memcpy(&data->pos, &PHG_DATA_TEXT(argdata)->pos, sizeof(Ppoint));
         data->char_string = (char *) &data[1];
         strcpy(data->char_string, str);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_FREE:
         free(ELMT_DATA_PTR(elmt));
      break;

      default:
         /* Default */
         return (FALSE);
      break;
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_int
 *
 * DESCR:	Handle integer data
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_int(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Pint *data;

   switch (op) {
      case CSS_EL_CREATE:
         data = malloc(sizeof(Pint));
         if (data == NULL)
            return (FALSE);

         *data = ARGS_ELMT_DATA(argdata).int_data;
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_REPLACE:
         data = (Pint *) elmt->eldata.ptr;
         *data = ARGS_ELMT_DATA(argdata).int_data;
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_COPY:
         data = malloc(sizeof(Pint));
         if (data == NULL)
            return (FALSE);

         *data = PHG_DATA_INT(argdata);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_FREE:
         free(ELMT_DATA_PTR(elmt));
      break;

      default:
         /* Default */
         return (FALSE);
      break;
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_float
 *
 * DESCR:	Handle float data
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_float(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Pfloat *data;

   switch (op) {
      case CSS_EL_CREATE:
         data = malloc(sizeof(Pfloat));
         if (data == NULL)
            return (FALSE);

         *data = ARGS_ELMT_DATA(argdata).float_data;
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_REPLACE:
         data = (Pfloat *) elmt->eldata.ptr;
         *data = ARGS_ELMT_DATA(argdata).float_data;
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_COPY:
         data = malloc(sizeof(Pfloat));
         if (data == NULL)
            return (FALSE);

         *data = PHG_DATA_FLOAT(argdata);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_FREE:
         free(ELMT_DATA_PTR(elmt));
      break;

      default:
         /* Default */
         return (FALSE);
      break;
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_edge_flag
 *
 * DESCR:	Handle edge flag
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_edge_flag(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Pedge_flag *data;

   switch (op) {
      case CSS_EL_CREATE:
         data = malloc(sizeof(Pedge_flag));
         if (data == NULL)
            return (FALSE);

         *data = ARGS_ELMT_DATA(argdata).edge_flag;
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_REPLACE:
         data = (Pedge_flag *) elmt->eldata.ptr;
         *data = ARGS_ELMT_DATA(argdata).edge_flag;
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_COPY:
         data = malloc(sizeof(Pedge_flag));
         if (data == NULL)
            return (FALSE);

         *data = PHG_DATA_EDGE_FLAG(argdata);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_FREE:
         free(ELMT_DATA_PTR(elmt));
      break;

      default:
         /* Default */
         return (FALSE);
      break;
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_int_style
 *
 * DESCR:	Handle interiour style
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_int_style(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Pint_style *data;

   switch (op) {
      case CSS_EL_CREATE:
         data = malloc(sizeof(Pint_style));
         if (data == NULL)
            return (FALSE);

         *data = ARGS_ELMT_DATA(argdata).int_style;
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_REPLACE:
         data = (Pint_style *) elmt->eldata.ptr;
         *data = ARGS_ELMT_DATA(argdata).int_style;
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_COPY:
         data = malloc(sizeof(Pint_style));
         if (data == NULL)
            return (FALSE);

         *data = PHG_DATA_INT_STYLE(argdata);
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_FREE:
         free(ELMT_DATA_PTR(elmt));
      break;

      default:
         /* Default */
         return (FALSE);
      break;
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_colr
 *
 * DESCR:	Handler colour
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_colr(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Pgcolr *data;

   switch (op) {
      case CSS_EL_CREATE:
         data = malloc(sizeof(Pgcolr));
         if (data == NULL)
            return (FALSE);

         data->type = ARGS_ELMT_DATA(argdata).colr.type;
         if (data->type == PINDIRECT) {
            data->val.ind = ARGS_ELMT_DATA(argdata).colr.val.ind;
         }
         else {
            data->val.general.x = ARGS_ELMT_DATA(argdata).colr.val.general.x;
            data->val.general.y = ARGS_ELMT_DATA(argdata).colr.val.general.y;
            data->val.general.z = ARGS_ELMT_DATA(argdata).colr.val.general.z;
         }
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_REPLACE:
         data = (Pgcolr *) elmt->eldata.ptr;
         data->type = ARGS_ELMT_DATA(argdata).colr.type;
         if (data->type == PINDIRECT) {
            data->val.ind = ARGS_ELMT_DATA(argdata).colr.val.ind;
         }
         else {
            data->val.general.x = ARGS_ELMT_DATA(argdata).colr.val.general.x;
            data->val.general.y = ARGS_ELMT_DATA(argdata).colr.val.general.y;
            data->val.general.z = ARGS_ELMT_DATA(argdata).colr.val.general.z;
         }
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_COPY:
         data = malloc(sizeof(Pgcolr));
         if (data == NULL)
            return (FALSE);

         memcpy(data, PHG_DATA_COLR(argdata), sizeof(Pgcolr));
         elmt->eldata.ptr = data;
      break;

      case CSS_EL_FREE:
         free(ELMT_DATA_PTR(elmt));
      break;

      default:
         /* Default */
         return (FALSE);
      break;
   }

   return (TRUE);
}

