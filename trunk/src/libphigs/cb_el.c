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

#include "phg.h"
#include "css.h"
#include "private/phgP.h"
#include "private/cbP.h"

/*******************************************************************************
 * padd_names_set
 *
 * DESCR:	Creates a new element - name set Inclusion
 * RETURNS:	N/A
 */

void padd_names_set(
   Pint_list *names
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_add_names_set)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_int_list(&args, PELEM_ADD_NAMES_SET, names)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * premove_names_set
 *
 * DESCR:	Creates a new element - name set Remover
 * RETURNS:	N/A
 */

void premove_names_set(
   Pint_list *names
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_remove_names_set)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_int_list(&args, PELEM_REMOVE_NAMES_SET, names)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_indiv_asf
 *
 * DESCR:	Creates a new element - Set attribute source flag
 * RETURNS:	N/A
 */

void pset_indiv_asf(
   Paspect asf_id,
   Pasf asf_source
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_indiv_asf)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_int2(&args, PELEM_INDIV_ASF,
                                (Pint) asf_id, (Pint) asf_source)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_local_tran3
 *
 * DESCR:	Creates a new element - Set local model space transformation 3D
 * RETURNS:	N/A
 */

void pset_local_tran3(
   Pmatrix3 local_tran,
   Pcompose_type compose_type
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_local_tran3)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_int_matrix3(&args, PELEM_LOCAL_MODEL_TRAN3,
                                       compose_type, local_tran)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_global_tran3
 *
 * DESCR:	Creates a new element - Set global model space transformation 3D
 * RETURNS:	N/A
 */

void pset_global_tran3(
   Pmatrix3 global_tran
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_global_tran3)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_matrix3(&args, PELEM_GLOBAL_MODEL_TRAN3,
                                   global_tran)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_view_ind
 *
 * DESCR:	Creates a new element - Set view index
 * RETURNS:	N/A
 */

void pset_view_ind(
   Pint view_ind
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_view_ind)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else if (view_ind < 0) {
         ERR_REPORT(PHG_ERH, ERR114);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_VIEW_IND, view_ind)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * ptext
 *
 * DESCR:	Creates a new element - Text
 * RETURNS:	N/A
 */

void ptext(
   Ppoint *text_pos,
   char *char_string
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_text)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_point_string(&args, PELEM_TEXT,
                                        text_pos, char_string)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * ppolyline
 *
 * DESCR:	Creates a new element - Polyline
 * RETURNS:	N/A
 */

void ppolyline(
   Ppoint_list *point_list
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_polyline)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_point_list(&args, PELEM_POLYLINE, point_list)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * ppolyline3
 *
 * DESCR:	Creates a new element - Polyline 3D
 * RETURNS:	N/A
 */

void ppolyline3(
   Ppoint_list3 *point_list
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_polyline3)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_point_list3(&args, PELEM_POLYLINE3, point_list)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * ppolymarker
 *
 * DESCR:	Creates a new element - Polymarker
 * RETURNS:	N/A
 */

void ppolymarker(
   Ppoint_list *point_list
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_polymarker)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_point_list(&args, PELEM_POLYMARKER, point_list)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * ppolymarker3
 *
 * DESCR:	Creates a new element - Polymarker 3D
 * RETURNS:	N/A
 */

void ppolymarker3(
   Ppoint_list3 *point_list
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_polymarker3)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_point_list3(&args, PELEM_POLYMARKER3, point_list)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pfill_area
 *
 * DESCR:	Creates a new element - Fill area
 * RETURNS:	N/A
 */

void pfill_area(
   Ppoint_list *point_list
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_fill_area)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_point_list(&args, PELEM_FILL_AREA, point_list)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pfill_area3
 *
 * DESCR:	Creates a new element - Fill area 3D
 * RETURNS:	N/A
 */

void pfill_area3(
   Ppoint_list3 *point_list
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_fill_area3)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_point_list3(&args, PELEM_FILL_AREA3, point_list)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pfill_area_set
 *
 * DESCR:	Creates a new element - Fill area set
 * RETURNS:	N/A
 */

void pfill_area_set(
   Ppoint_list_list *point_list_list
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_fill_area_set)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_point_list_list(&args, PELEM_FILL_AREA_SET,
                                           point_list_list)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pfill_area_set3
 *
 * DESCR:	Creates a new element - Fill area set 3D
 * RETURNS:	N/A
 */

void pfill_area_set3(
   Ppoint_list_list3 *point_list_list
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_fill_area_set3)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_point_list_list3(&args, PELEM_FILL_AREA_SET3,
                                            point_list_list)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * plabel
 *
 * DESCR:	Creates a new element - Label
 * RETURNS:	N/A
 */

void plabel(
   Pint label_id
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_label)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_LABEL, label_id)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_pick_id
 *
 * DESCR:	Creates a new element - Pick ID
 * RETURNS:	N/A
 */

void pset_pick_id(
   Pint pick_id
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_pick_id)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_PICK_ID, pick_id)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_hlhsr_id
 *
 * DESCR:	Create hidden lines, surface removal flag element
 * RETURNS:	N/A
 */

void pset_hlhsr_id(
   Pint hlhsr_id
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_hlhsr_id)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_HLHSR_ID, hlhsr_id)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_int_ind
 *
 * DESCR:	Creates a new element - Facet Interiour Attribute Index
 * RETURNS:	N/A
 */

void pset_int_ind(
   Pint int_ind
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_int_ind)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else if (int_ind < 1) {
         ERR_REPORT(PHG_ERH, ERR100);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_INT_IND, int_ind)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_int_colr_ind
 *
 * DESCR:	Creates a new element - Facet Color Attribute
 * RETURNS:	N/A
 */

void pset_int_colr_ind(
   Pint colr_ind
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_int_colr_ind)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else if (colr_ind < 0) {
         ERR_REPORT(PHG_ERH, ERR113);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_INT_COLR_IND, colr_ind)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_int_style
 *
 * DESCR:	Creates a new element - Face Interiour Style
 * RETURNS:	N/A
 */

void pset_int_style(
   Pint_style int_style
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_int_style)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_INT_STYLE, (Pint) int_style)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_int_style_ind
 *
 * DESCR:	Creates a new element - Face Interiour Pattern Index
 * RETURNS:	N/A
 */

void pset_int_style_ind(
   Pint int_style_ind
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_int_style)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else if (int_style_ind < 1) {
         ERR_REPORT(PHG_ERH, ERR112);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_INT_STYLE_IND, int_style_ind)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_line_ind
 *
 * DESCR:	Creates a new element - Line Attribute Index
 * RETURNS:	N/A
 */

void pset_line_ind(
   Pint line_ind
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_line_ind)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else if (line_ind < 1) {
         ERR_REPORT(PHG_ERH, ERR100);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_LINE_IND, line_ind)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_line_colr_ind
 *
 * DESCR:	Creates a new element - Line Color Attribute
 * RETURNS:	N/A
 */

void pset_line_colr_ind(
   Pint colr_ind
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_line_colr_ind)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else if (colr_ind < 0) {
         ERR_REPORT(PHG_ERH, ERR113);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_LINE_COLR_IND, colr_ind)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_linewidth
 *
 * DESCR:	Creates a new element - Line Width Attribute
 * RETURNS:	N/A
 */

void pset_linewidth(
   Pfloat linewidth
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_linewidth)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_float(&args, PELEM_LINEWIDTH, linewidth)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_linetype
 *
 * DESCR:	Creates a new element - Line Type Attribute
 * RETURNS:	N/A
 */

void pset_linetype(
   Pint linetype
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_linetype)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_LINETYPE, linetype)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_marker_ind
 *
 * DESCR:	Creates a new element - Marker Attribute Index
 * RETURNS:	N/A
 */

void pset_marker_ind(
   Pint marker_ind
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_marker_ind)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else if (marker_ind < 1) {
         ERR_REPORT(PHG_ERH, ERR100);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_MARKER_IND, marker_ind)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_marker_colr_ind
 *
 * DESCR:	Creates a new element - Marker Color Attribute
 * RETURNS:	N/A
 */

void pset_marker_colr_ind(
   Pint colr_ind
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_marker_colr_ind)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else if (colr_ind < 0) {
         ERR_REPORT(PHG_ERH, ERR113);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_MARKER_COLR_IND, colr_ind)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_marker_size
 *
 * DESCR:	Creates a new element - Marker Size Attribute
 * RETURNS:	N/A
 */

void pset_marker_size(
   Pfloat marker_size
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_marker_size)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_float(&args, PELEM_MARKER_SIZE, marker_size)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_marker_type
 *
 * DESCR:	Creates a new element - Marker Type Attribute
 * RETURNS:	N/A
 */

void pset_marker_type(
   Pint marker_type
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_marker_type)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_MARKER_TYPE, marker_type)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_edge_ind
 *
 * DESCR:	Creates a new element - Edge Attribute Index
 * RETURNS:	N/A
 */

void pset_edge_ind(
   Pint edge_ind
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_edge_ind)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else if (edge_ind < 1) {
         ERR_REPORT(PHG_ERH, ERR100);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_EDGE_IND, edge_ind)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_edge_colr_ind
 *
 * DESCR:	Creates a new element - Edge Color Attribute
 * RETURNS:	N/A
 */

void pset_edge_colr_ind(
   Pint colr_ind
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_edge_colr_ind)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else if (colr_ind < 0) {
         ERR_REPORT(PHG_ERH, ERR113);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_EDGE_COLR_IND, colr_ind)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_edgetype
 *
 * DESCR:	Creates a new element - Edge Type Attribute
 * RETURNS:	N/A
 */

void pset_edgetype(
   Pint edgetype
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_edgetype)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_EDGETYPE, edgetype)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_edge_flag
 *
 * DESCR:	Creates a new element - Edge Flag Attribute
 * RETURNS:	N/A
 */

void pset_edge_flag(
   Pedge_flag edge_flag
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_edge_flag)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_EDGE_FLAG, (Pint) edge_flag)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_edgewidth
 *
 * DESCR:	Creates a new element - Edge Width Attribute
 * RETURNS:	N/A
 */

void pset_edgewidth(
   Pfloat edgewidth
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_edgewidth)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_float(&args, PELEM_EDGEWIDTH, edgewidth)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_text_font
 *
 * DESCR:	Creates a new element - Text Font Attribute
 * RETURNS:	N/A
 */

void pset_text_font(
   Pint font
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_text_font)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_TEXT_FONT, font)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_text_prec
 *
 * DESCR:	Creates a new element - Text Precision Attribute
 * RETURNS:	N/A
 */

void pset_text_prec(
   Ptext_prec prec
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_text_prec)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_TEXT_PREC, (Pint) prec)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_text_path
 *
 * DESCR:	Creates a new element - Text Path Attribute
 * RETURNS:	N/A
 */

void pset_text_path(
   Ptext_path text_path
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_text_path)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_TEXT_PATH, (Pint) text_path)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_text_align
 *
 * DESCR:	Creates a new element - Text Alignment Attribute
 * RETURNS:	N/A
 */

void pset_text_align(
   Ptext_align *text_align
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_text_align)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_int2(&args, PELEM_TEXT_ALIGN,
                                text_align->hor, text_align->vert)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_char_ht
 *
 * DESCR:	Creates a new element - Character height Attribute
 * RETURNS:	N/A
 */

void pset_char_ht(
   Pfloat char_ht
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_char_ht)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_float(&args, PELEM_CHAR_HT, char_ht)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_char_expan
 *
 * DESCR:	Creates a new element - Character expansion factor Attribute
 * RETURNS:	N/A
 */

void pset_char_expan(
   Pfloat char_expan
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_char_expan)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_float(&args, PELEM_CHAR_EXPAN, char_expan)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_char_space
 *
 * DESCR:	Creates a new element - Character spaceing Attribute
 * RETURNS:	N/A
 */

void pset_char_space(
   Pfloat char_space
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_char_space)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_float(&args, PELEM_CHAR_SPACE, char_space)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_char_up_vec
 *
 * DESCR:	Creates a new element - Character up vector Attribute
 * RETURNS:	N/A
 */

void pset_char_up_vec(
   Pvec *char_up_vec
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_char_up_vec)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_float2(&args, PELEM_CHAR_UP_VEC,
                                  char_up_vec->delta_x, char_up_vec->delta_y)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_text_ind
 *
 * DESCR:	Creates a new element - Text Attribute Index
 * RETURNS:	N/A
 */

void pset_text_ind(
   Pint text_ind
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_text_ind)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else if (text_ind < 1) {
         ERR_REPORT(PHG_ERH, ERR100);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_TEXT_IND, text_ind)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pset_text_colr_ind
 *
 * DESCR:	Creates a new element - Text Color Attribute
 * RETURNS:	N/A
 */

void pset_text_colr_ind(
   Pint colr_ind
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_text_colr_ind)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else if (colr_ind < 0) {
         ERR_REPORT(PHG_ERH, ERR113);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_TEXT_COLR_IND, colr_ind)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pexec_struct
 *
 * DESCR:	Creates a new element - Nested Structure
 * RETURNS:	N/A
 */

void pexec_struct(
   Pint struct_id
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_exec_struct)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         if (!phg_cb_store_int(&args, PELEM_EXEC_STRUCT, struct_id)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

