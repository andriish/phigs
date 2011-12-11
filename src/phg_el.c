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

#include <stdlib.h>
#include <string.h>
#include <phigs/phg.h>
#include <phigs/css.h>
#include <phigs/mat_utils.h>
#include <phigs/private/phgP.h>

#define UNDEF_COLR_IND         0

/*******************************************************************************
 * phg_handle_names_set
 *
 * DESCR:	Handle nameset
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_names_set(Css_handle cssh,
                         El_handle elmt,
                         caddr_t argdata,
                         Css_el_op op)
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
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_local_tran
 *
 * DESCR:	Handle local transformation
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_local_tran(Css_handle cssh,
                          El_handle elmt,
                          caddr_t argdata,
                          Css_el_op op)
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
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_local_tran3
 *
 * DESCR:	Handle local transformation 3D
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_local_tran3(Css_handle cssh,
                           El_handle elmt,
                           caddr_t argdata,
                           Css_el_op op)
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
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_point_list
 *
 * DESCR:	Handle point list
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_point_list(Css_handle cssh,
                          El_handle elmt,
                          caddr_t argdata,
                          Css_el_op op)
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
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_point_list3
 *
 * DESCR:	Handle point list 3D
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_point_list3(Css_handle cssh,
                           El_handle elmt,
                           caddr_t argdata,
                           Css_el_op op)
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
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_text
 *
 * DESCR:	Handle text
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_text(Css_handle cssh,
                    El_handle elmt,
                    caddr_t argdata,
                    Css_el_op op)
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
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_int
 *
 * DESCR:	Handle integer data
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_int(Css_handle cssh,
                   El_handle elmt,
                   caddr_t argdata,
                   Css_el_op op)
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
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_float
 *
 * DESCR:	Handle float data
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_float(Css_handle cssh,
                     El_handle elmt,
                     caddr_t argdata,
                     Css_el_op op)
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
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_edge_flag
 *
 * DESCR:	Handle edge flag
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_edge_flag(Css_handle cssh,
                         El_handle elmt,
                         caddr_t argdata,
                         Css_el_op op)
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
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_handle_int_style
 *
 * DESCR:	Handle interiour style
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int phg_handle_int_style(Css_handle cssh,
                         El_handle elmt,
                         caddr_t argdata,
                         Css_el_op op)
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
   }

   return (TRUE);
}

/*******************************************************************************
 * phg_add_el
 *
 * DESCR:	Add an element and update workstations posted to
 * RETURNS:	N/A
 */

static void phg_add_el(Css_handle cssh, Phg_args_add_el *args)
{
   Css_ws_list ws_list;

   ws_list = CSS_GET_WS_ON(CSS_CUR_STRUCTP(cssh));

   if (phg_css_add_elem(cssh, args)) {
      if (ws_list != NULL) {
         for (; ws_list->wsh; ws_list++)
            (*ws_list->wsh->add_el)(ws_list->wsh);
      }
   }
}

/*******************************************************************************
 * padd_names_set
 *
 * DESCR:	Creates a new element - name set Inclusion
 * RETURNS:	N/A
 */

void padd_names_set(Pint_list *names)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_ADD_NAMES_SET;
   ARGS_ELMT_DATA(&args).names.num_ints = names->num_ints;
   ARGS_ELMT_DATA(&args).names.ints = names->ints;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * premove_names_set
 *
 * DESCR:	Creates a new element - name set Remover
 * RETURNS:	N/A
 */

void premove_names_set(Pint_list *names)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_REMOVE_NAMES_SET;
   ARGS_ELMT_DATA(&args).names.num_ints = names->num_ints;
   ARGS_ELMT_DATA(&args).names.ints = names->ints;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pset_local_tran
 *
 * DESCR:	Creates a new element - Set local model space transofmation
 * RETURNS:	N/A
 */

void pset_local_tran(Pmatrix local_tran, Pcompose_type compose_type)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_LOCAL_MODEL_TRAN;
   ARGS_ELMT_DATA(&args).local_tran.compose_type = compose_type;
   phg_mat_copy_3x3(ARGS_ELMT_DATA(&args).local_tran.matrix, local_tran);
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pset_local_tran3
 *
 * DESCR:	Creates a new element - Set local model space transofmation 3D
 * RETURNS:	N/A
 */

void pset_local_tran3(Pmatrix3 local_tran, Pcompose_type compose_type)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_LOCAL_MODEL_TRAN3;
   ARGS_ELMT_DATA(&args).local_tran3.compose_type = compose_type;
   phg_mat_copy(ARGS_ELMT_DATA(&args).local_tran3.matrix, local_tran);
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pset_view_ind3
 *
 * DESCR:	Creates a new element - Set view index
 * RETURNS:	N/A
 */

void pset_view_ind3(Pint index)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_VIEW_IND;
   ARGS_ELMT_DATA(&args).int_data = index;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * ptext
 *
 * DESCR:	Creates a new element - Text
 * RETURNS:	N/A
 */

void ptext(Ppoint *text_pos, char *char_string)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_TEXT;
   memcpy(&ARGS_ELMT_DATA(&args).text.pos, text_pos, sizeof(Ppoint));
   ARGS_ELMT_DATA(&args).text.char_string = char_string;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * ppolyline
 *
 * DESCR:	Creates a new element - Polyline
 * RETURNS:	N/A
 */

void ppolyline(Ppoint_list *point_list)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_POLYLINE;
   ARGS_ELMT_DATA(&args).point_list.num_points = point_list->num_points;
   ARGS_ELMT_DATA(&args).point_list.points = point_list->points;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * ppolyline3
 *
 * DESCR:	Creates a new element - Polyline 3D
 * RETURNS:	N/A
 */

void ppolyline3(Ppoint_list3 *point_list)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_POLYLINE3;
   ARGS_ELMT_DATA(&args).point_list3.num_points =
      point_list->num_points;
   ARGS_ELMT_DATA(&args).point_list3.points = point_list->points;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * ppolymarker
 *
 * DESCR:	Creates a new element - Polymarker
 * RETURNS:	N/A
 */

void ppolymarker(Ppoint_list *point_list)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_POLYMARKER;
   ARGS_ELMT_DATA(&args).point_list.num_points = point_list->num_points;
   ARGS_ELMT_DATA(&args).point_list.points = point_list->points;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * ppolymarker3
 *
 * DESCR:	Creates a new element - Polymarker 3D
 * RETURNS:	N/A
 */

void ppolymarker3(Ppoint_list3 *point_list)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_POLYMARKER3;
   ARGS_ELMT_DATA(&args).point_list3.num_points =
      point_list->num_points;
   ARGS_ELMT_DATA(&args).point_list3.points = point_list->points;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pfill_area
 *
 * DESCR:	Creates a new element - Fill area
 * RETURNS:	N/A
 */

void pfill_area(Ppoint_list *point_list)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_FILL_AREA;
   ARGS_ELMT_DATA(&args).point_list.num_points = point_list->num_points;
   ARGS_ELMT_DATA(&args).point_list.points = point_list->points;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pfill_area3
 *
 * DESCR:	Creates a new element - Fill area 3D
 * RETURNS:	N/A
 */

void pfill_area3(Ppoint_list3 *point_list)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_FILL_AREA3;
   ARGS_ELMT_DATA(&args).point_list3.num_points =
      point_list->num_points;
   ARGS_ELMT_DATA(&args).point_list3.points = point_list->points;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * plabel
 *
 * DESCR:	Creates a new element - Label
 * RETURNS:	N/A
 */

void plabel(Pint label_id)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_LABEL;
   ARGS_ELMT_DATA(&args).int_data = label_id;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pset_pick_id
 *
 * DESCR:	Creates a new element - Pick ID
 * RETURNS:	N/A
 */

void pset_pick_id(Pint pick_id)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_PICK_ID;
   ARGS_ELMT_DATA(&args).int_data = pick_id;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pset_int_colr_ind
 *
 * DESCR:	Creates a new element - Facet Color Attribute
 * RETURNS:	N/A
 */

void pset_int_colr_ind(Pint colr_ind)
{
   Phg_args_add_el args;

   if (colr_ind < 0 || colr_ind > 255)
      colr_ind = UNDEF_COLR_IND;

   ARGS_ELMT_TYPE(&args) = PELEM_INT_COLR_IND;
   ARGS_ELMT_DATA(&args).int_data = colr_ind;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pset_int_style
 *
 * DESCR:	Creates a new element - Face Interiour Style
 * RETURNS:	N/A
 */

void pset_int_style(Pint_style int_style)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_INT_STYLE;
   ARGS_ELMT_DATA(&args).int_style = int_style;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pset_line_colr_ind
 *
 * DESCR:	Creates a new element - Line Color Attribute
 * RETURNS:	N/A
 */

void pset_line_colr_ind(Pint colr_ind)
{
   Phg_args_add_el args;

   if (colr_ind < 0 || colr_ind > 255)
      colr_ind = UNDEF_COLR_IND;

   ARGS_ELMT_TYPE(&args) = PELEM_LINE_COLR_IND;
   ARGS_ELMT_DATA(&args).int_data = colr_ind;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pset_linewidth
 *
 * DESCR:	Creates a new element - Line Width Attribute
 * RETURNS:	N/A
 */

void pset_linewidth(Pfloat linewidth)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_LINEWIDTH;
   ARGS_ELMT_DATA(&args).float_data = linewidth;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pset_linetype
 *
 * DESCR:	Creates a new element - Line Type Attribute
 * RETURNS:	N/A
 */

void pset_linetype(Pint linetype)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_LINETYPE;
   ARGS_ELMT_DATA(&args).int_data = linetype;
   phg_add_el(css, &args);
}

/*******************************************************************************
 *
 * DESCR:	Creates a new element - Marker Color Attribute
 * RETURNS:	N/A
 */

void pset_marker_colr_ind(Pint colr_ind)
{
   Phg_args_add_el args;

   if (colr_ind < 0 || colr_ind > 255)
      colr_ind = UNDEF_COLR_IND;

   ARGS_ELMT_TYPE(&args) = PELEM_MARKER_COLR_IND;
   ARGS_ELMT_DATA(&args).int_data = colr_ind;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pset_marker_size
 *
 * DESCR:	Creates a new element - Marker Size Attribute
 * RETURNS:	N/A
 */

void pset_marker_size(Pfloat marker_size)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_MARKER_SIZE;
   ARGS_ELMT_DATA(&args).float_data = marker_size;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pset_marker_type
 *
 * DESCR:	Creates a new element - Marker Type Attribute
 * RETURNS:	N/A
 */

void pset_marker_type(Pint marker_type)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_MARKER_TYPE;
   ARGS_ELMT_DATA(&args).int_data = marker_type;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pset_edge_colr_ind
 *
 * DESCR:	Creates a new element - Edge Color Attribute
 * RETURNS:	N/A
 */

void pset_edge_colr_ind(Pint colr_ind)
{
   Phg_args_add_el args;

   if (colr_ind < 0 || colr_ind > 255)
      colr_ind = UNDEF_COLR_IND;

   ARGS_ELMT_TYPE(&args) = PELEM_EDGE_COLR_IND;
   ARGS_ELMT_DATA(&args).int_data = colr_ind;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pset_edgetype
 *
 * DESCR:	Creates a new element - Edge Type Attribute
 * RETURNS:	N/A
 */

void pset_edgetype(Pint edgetype)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_EDGETYPE;
   ARGS_ELMT_DATA(&args).int_data = edgetype;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pset_edge_flag
 *
 * DESCR:	Creates a new element - Edge Flag Attribute
 * RETURNS:	N/A
 */

void pset_edge_flag(Pedge_flag edge_flag)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_EDGE_FLAG;
   ARGS_ELMT_DATA(&args).edge_flag = edge_flag;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pset_edgetwidth
 *
 * DESCR:	Creates a new element - Edge Width Attribute
 * RETURNS:	N/A
 */

void pset_edgewidth(Pfloat edgewidth)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_EDGEWIDTH;
   ARGS_ELMT_DATA(&args).float_data = edgewidth;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pset_text_font
 *
 * DESCR:	Creates a new element - Text Font Attribute
 * RETURNS:	N/A
 */

void pset_text_font(Pint font)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_TEXT_FONT;
   ARGS_ELMT_DATA(&args).int_data = font;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pset_text_colr_ind
 *
 * DESCR:	Creates a new element - Text Color Attribute
 * RETURNS:	N/A
 */

void pset_text_colr_ind(Pint colr_ind)
{
   Phg_args_add_el args;

   if (colr_ind < 0 || colr_ind > 255)
      colr_ind = UNDEF_COLR_IND;

   ARGS_ELMT_TYPE(&args) = PELEM_TEXT_COLR_IND;
   ARGS_ELMT_DATA(&args).int_data = colr_ind;
   phg_add_el(css, &args);
}

/*******************************************************************************
 * pexec_struct
 *
 * DESCR:	Creates a new element - Nested Structure
 * RETURNS:	N/A
 */

void pexec_struct(Pint struct_id)
{
   Phg_args_add_el args;

   ARGS_ELMT_TYPE(&args) = PELEM_EXEC_STRUCT;
   ARGS_ELMT_DATA(&args).int_data = struct_id;
   phg_add_el(css, &args);
}

