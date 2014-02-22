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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "phg.h"

static void print_int_list(Pelem_data *elem_data)
{
   Pint i;

   printf("Integer list:\n");
   for (i = 0; i < elem_data->int_list.num_ints; i++) {
      printf("\t#%d\t%d\n",
             i, elem_data->int_list.ints[i]);
   }
}

static void print_point_list(Pelem_data *elem_data)
{
   Pint i;

   printf("Point list:\n");
   for (i = 0; i < elem_data->point_list.num_points; i++) {
      printf("\t#%d\t%g\t%g\n",
             i,
             elem_data->point_list.points[i].x,
             elem_data->point_list.points[i].y);
   }
}

static void print_point_list3(Pelem_data *elem_data)
{
   Pint i;

   printf("Point list 3D:\n");
   for (i = 0; i < elem_data->point_list3.num_points; i++) {
      printf("\t#%d\t%g\t%g\t%g\n",
             i,
             elem_data->point_list3.points[i].x,
             elem_data->point_list3.points[i].y,
             elem_data->point_list3.points[i].z);
   }
}

static void print_point_list_list3(Pelem_data *elem_data)
{
   Pint i, j;

   printf("List of point lists 3D:\n");
   for (i = 0; i < elem_data->point_list_list3.num_point_lists; i++) {
      printf("\t#%d\n", i);
      for (j = 0;
           j < elem_data->point_list_list3.point_lists[i].num_points;
           j++) {
         printf("\t\t#%d\t%g\t%g\t%g\n",
                j,
                elem_data->point_list_list3.point_lists[i].points[j].x,
                elem_data->point_list_list3.point_lists[i].points[j].y,
                elem_data->point_list_list3.point_lists[i].points[j].z);
      }
   }
}

static void print_text(Pelem_data *elem_data)
{
   printf("Text at: %g\t%g\n", elem_data->text.pos.x, elem_data->text.pos.y);
   printf("\t%s\n", elem_data->text.char_string);
}

static void print_gcolr(Pelem_data *elem_data)
{
   switch(elem_data->colr.type) {
      case PMODEL_RGB:
         printf("(R G B) = (%g %g %g)\n",
                elem_data->colr.val.general.x,
                elem_data->colr.val.general.y,
                elem_data->colr.val.general.z);
         break;

      case PINDIRECT:
         printf("%d\n", elem_data->colr.val.ind);
         break;

      default:
         printf("Unknown colour model\n");
         break;
   }
}

void print_elem_content(Pint struct_id, Pint elem_num)
{
   Pstore store;
   Pint err;
   Pelem_type el_type;
   size_t el_size;
   Pelem_data *elem_data;

   pcreate_store(&err, &store);
   if (!err) {
      if (elem_num >= 0) {
         pinq_elem_type_size(struct_id, elem_num, &err, &el_type, &el_size);
      }
      else {
         pinq_cur_elem_type_size(&err, &el_type, &el_size);
      }
      if (!err) {
         if (elem_num >= 0) {
            pinq_elem_content(struct_id, elem_num, store, &err, &elem_data);
         }
         else {
            pinq_cur_elem_content(store, &err, &elem_data);
         }
         if (!err) {
            css_print_eltype(el_type);
            printf("\n");
            switch (el_type) {
               case PELEM_ADD_NAMES_SET:
               case PELEM_REMOVE_NAMES_SET:
                  print_int_list(elem_data);
                  break;

               case PELEM_POLYLINE:
               case PELEM_POLYMARKER:
               case PELEM_FILL_AREA:
                  print_point_list(elem_data);
                  break;

               case PELEM_POLYLINE3:
               case PELEM_POLYMARKER3:
               case PELEM_FILL_AREA3:
                  print_point_list3(elem_data);
                  break;

               case PELEM_FILL_AREA_SET3:
                  print_point_list_list3(elem_data);
                  break;

               case PELEM_TEXT:
                  print_text(elem_data);
                  break;

               case PELEM_INDIV_ASF:
                  printf("Asf Source: ID = %d, Source = %d\n",
                         elem_data->asf_info.id,
                         elem_data->asf_info.source);
                  break;

               case PELEM_TEXT_ALIGN:
                  printf("Text align: Horizontal = %d, Vertical = %d\n",
                         elem_data->text_align.hor,
                         elem_data->text_align.vert);
                  break;

               case PELEM_CHAR_UP_VEC:
                  printf("Vector: %g\t%g\n",
                         elem_data->vec.delta_x,
                         elem_data->vec.delta_y);
                  break;

               case PELEM_LINEWIDTH:
               case PELEM_MARKER_SIZE:
               case PELEM_EDGEWIDTH:
               case PELEM_CHAR_HT:
               case PELEM_CHAR_EXPAN:
               case PELEM_CHAR_SPACE:
                  printf("Floating point data: %g\n", elem_data->float_data);
                  break;

               case PELEM_LOCAL_MODEL_TRAN3:
                  printf("Local transformation 3D data: %d\n",
                         elem_data->local_tran3.compose_type);
                  phg_mat_print(elem_data->local_tran3.matrix);
                  printf("\n");
                  break;

               case PELEM_GLOBAL_MODEL_TRAN3:
                  printf("Global transformation 3D data:");
                  phg_mat_print(elem_data->global_tran3);
                  printf("\n");
                  break;

               case PELEM_INT_COLR:
               case PELEM_BACK_INT_COLR:
               case PELEM_LINE_COLR:
               case PELEM_MARKER_COLR:
               case PELEM_EDGE_COLR:
               case PELEM_TEXT_COLR:
                  printf("Colour data: ");
                  print_gcolr(elem_data);
                  break;

               default:
                  printf("Integer data: %d\n", elem_data->int_data);
                  break;
            }
         }
      }
      pdel_store(store);
   }
}

