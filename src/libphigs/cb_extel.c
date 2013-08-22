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

/*******************************************************************************
 * pfill_area_set3_data
 *
 * DESCR:	Creates a new element - Fill area set with data 3D
 * RETURNS:	N/A
 */

void pfill_area_set3_data(
   Pint fflag,
   Pint eflag,
   Pint vflag,
   Pint colr_type,
   Pfacet_data3 *fdata,
   Pint nfa,
   Pedge_data_list *edata,
   Pfacet_vdata_list3 *vdata
   )
{
   Pint i;
   unsigned facet_size, vertex_size;
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_fill_area_set3_data);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      switch (fflag) {
         case PFACET_COLOUR:
            facet_size = sizeof(Pcoval);
            break;

         case PFACET_NORMAL:
            facet_size = sizeof(Pvec3);
            break;

         case PFACET_COLOUR_NORMAL:
            facet_size = sizeof(Pconorm3);
            break;

         default:
            facet_size = 0;
            break;
      }

      switch (vflag) {
         case PVERT_COORD:
            vertex_size = sizeof(Ppoint3);
            break;

         case PVERT_COORD_COLOUR:
            vertex_size = sizeof(Pptco3);
            break;

         case PVERT_COORD_NORMAL:
            vertex_size = sizeof(Pptnorm3);
            break;

         case PVERT_COORD_COLOUR_NORMAL:
            vertex_size = sizeof(Pptconorm3);
            break;

         default: 
            vertex_size = 0;
            break;
      }

      ARGS_ELMT_TYPE(&args) = PELEM_FILL_AREA_SET3_DATA;
      ARGS_ELMT_SIZE(&args) = 5 * sizeof(Pint) + facet_size;

      if (eflag == PEDGE_VISIBILITY) {
         for (i = 0; i < nfa; i++) {
            ARGS_ELMT_SIZE(&args) += sizeof(Pint);   /* Pint num_edges */
            ARGS_ELMT_SIZE(&args) += edata[i].num_edges * sizeof(Pedge_flag);
         }
      }

      for (i = 0; i < nfa; i++) {
         ARGS_ELMT_SIZE(&args) += sizeof(Pint);   /* Pint num_vertices */
         ARGS_ELMT_SIZE(&args) += vdata[i].num_vertices * vertex_size;
      }

      ARGS_ELMT_DATA(&args).fasd3.fflag = fflag;
      ARGS_ELMT_DATA(&args).fasd3.eflag = eflag;
      ARGS_ELMT_DATA(&args).fasd3.vflag = vflag;
      ARGS_ELMT_DATA(&args).fasd3.colr_type = colr_type;
      if (fflag != PFACET_NONE) {
         memcpy(&ARGS_ELMT_DATA(&args).fasd3.fdata,
                fdata,
                sizeof(Pfacet_data3));
      }
      ARGS_ELMT_DATA(&args).fasd3.nfa = nfa;
      ARGS_ELMT_DATA(&args).fasd3.edata = edata;
      ARGS_ELMT_DATA(&args).fasd3.vdata = vdata;
      phg_add_el(PHG_CSS, &args);
   }
}

/*******************************************************************************
 * pset_of_fill_area_set3_data
 *
 * DESCR:	Creates a new element - Set of fill area set with data 3D
 * RETURNS:	N/A
 */

void pset_of_fill_area_set3_data(
   Pint fflag,
   Pint eflag,
   Pint vflag,
   Pint colr_type,
   Pint num_sets,
   Pfacet_data_arr3 *fdata,
   Pedge_data_list_list *edata,
   Pint_list_list *vlist,
   Pfacet_vdata_list3 *vdata
   )
{
   Pint i, j;
   unsigned facet_size, vertex_size;
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_of_fill_area_set3_data);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      switch (fflag) {
         case PFACET_COLOUR:
            facet_size = sizeof(Pcoval);
            break;

         case PFACET_NORMAL:
            facet_size = sizeof(Pvec3);
            break;

         case PFACET_COLOUR_NORMAL:
            facet_size = sizeof(Pconorm3);
            break;

         default:
            facet_size = 0;
            break;
      }

      switch (vflag) {
         case PVERT_COORD:
            vertex_size = sizeof(Ppoint3);
            break;

         case PVERT_COORD_COLOUR:
            vertex_size = sizeof(Pptco3);
            break;

         case PVERT_COORD_NORMAL:
            vertex_size = sizeof(Pptnorm3);
            break;

         case PVERT_COORD_COLOUR_NORMAL:
            vertex_size = sizeof(Pptconorm3);
            break;

         default: 
            vertex_size = 0;
            break;
      }

      ARGS_ELMT_TYPE(&args) = PELEM_SET_OF_FILL_AREA_SET3_DATA;
      ARGS_ELMT_SIZE(&args) = 5 * sizeof(Pint) + num_sets * facet_size;

      if (eflag == PEDGE_VISIBILITY) {
         for (i = 0; i < num_sets; i++) {
            ARGS_ELMT_SIZE(&args) += sizeof(Pint);   /* Pint num_lists */
            for (j = 0; j < edata[i].num_lists; j++) {
               ARGS_ELMT_SIZE(&args) += sizeof(Pint);   /* Pint num_edges */
               ARGS_ELMT_SIZE(&args) += edata[i].edgelist[j].num_edges *
                                        sizeof(Pedge_flag);
            }
         }
      }

      for (i = 0; i < num_sets; i++) {
         ARGS_ELMT_SIZE(&args) += sizeof(Pint);   /* Pint num_lists */
         for (j = 0; j < vlist[i].num_lists; j++) {
            ARGS_ELMT_SIZE(&args) += sizeof(Pint);   /* Pint num_ints */
            ARGS_ELMT_SIZE(&args) += vlist[i].lists[j].num_ints * sizeof(Pint);
         }
      }

      ARGS_ELMT_SIZE(&args) += sizeof(Pint);   /* Pint num_vertices */
      ARGS_ELMT_SIZE(&args) += vdata->num_vertices * vertex_size;

      ARGS_ELMT_DATA(&args).sofas3.fflag = fflag;
      ARGS_ELMT_DATA(&args).sofas3.eflag = eflag;
      ARGS_ELMT_DATA(&args).sofas3.vflag = vflag;
      ARGS_ELMT_DATA(&args).sofas3.colr_type = colr_type;
      ARGS_ELMT_DATA(&args).sofas3.num_sets = num_sets;
      if (fflag != PFACET_NONE) {
         memcpy(&ARGS_ELMT_DATA(&args).sofas3.fdata,
                fdata,
                sizeof(Pfacet_data_arr3));
      }
      ARGS_ELMT_DATA(&args).sofas3.edata = edata;
      ARGS_ELMT_DATA(&args).sofas3.vlist = vlist;
      memcpy(&ARGS_ELMT_DATA(&args).sofas3.vdata,
             vdata,
             sizeof(Pfacet_vdata_list3));
      phg_add_el(PHG_CSS, &args);
   }
}

/*******************************************************************************
 * pset_back_int_style
 *
 * DESCR:	Creates a new element - Backface Interiour Style
 * RETURNS:	N/A
 */

void pset_back_int_style(
   Pint_style int_style
   )
{
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_back_int_style);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_BACK_INT_STYLE;
      ARGS_ELMT_SIZE(&args) = sizeof(Pint_style);
      ARGS_ELMT_DATA(&args).int_style = int_style;
      phg_add_el(PHG_CSS, &args);
   }
}

/*******************************************************************************
 * pset_back_int_style_ind
 *
 * DESCR:	Creates a new element - Backface Interiour Pattern Index
 * RETURNS:	N/A
 */

void pset_back_int_style_ind(
   Pint int_style_ind
   )
{
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_back_int_style);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else if (int_style_ind < 1) {
      ERR_REPORT(PHG_ERH, ERR112);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_BACK_INT_STYLE_IND;
      ARGS_ELMT_SIZE(&args) = sizeof(Pint);
      ARGS_ELMT_DATA(&args).int_data = int_style_ind;
      phg_add_el(PHG_CSS, &args);
   }
}

/*******************************************************************************
 * pset_int_colr
 *
 * DESCR:	Creates a new element - Facet Color Attribute
 * RETURNS:	N/A
 */

void pset_int_colr(
   Pgcolr *colr
   )
{
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_int_colr);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_INT_COLR;
      ARGS_ELMT_SIZE(&args) = sizeof(Pgcolr);
      memcpy(&ARGS_ELMT_DATA(&args).colr, colr, sizeof(Pgcolr));
      phg_add_el(PHG_CSS, &args);
   }
}

/*******************************************************************************
 * pset_back_int_colr
 *
 * DESCR:	Creates a new element - Backface Color Attribute
 * RETURNS:	N/A
 */

void pset_back_int_colr(
   Pgcolr *colr
   )
{
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_back_int_colr);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_BACK_INT_COLR;
      ARGS_ELMT_SIZE(&args) = sizeof(Pgcolr);
      memcpy(&ARGS_ELMT_DATA(&args).colr, colr, sizeof(Pgcolr));
      phg_add_el(PHG_CSS, &args);
   }
}

/*******************************************************************************
 * pset_line_colr
 *
 * DESCR:	Creates a new element - Line Color Attribute
 * RETURNS:	N/A
 */

void pset_line_colr(
   Pgcolr *colr
   )
{
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_line_colr);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_LINE_COLR;
      ARGS_ELMT_SIZE(&args) = sizeof(Pgcolr);
      memcpy(&ARGS_ELMT_DATA(&args).colr, colr, sizeof(Pgcolr));
      phg_add_el(PHG_CSS, &args);
   }
}

/*******************************************************************************
 * pset_marker_colr
 *
 * DESCR:	Creates a new element - Marker Color Attribute
 * RETURNS:	N/A
 */

void pset_marker_colr(
   Pgcolr *colr
   )
{
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_marker_colr);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_MARKER_COLR;
      ARGS_ELMT_SIZE(&args) = sizeof(Pgcolr);
      memcpy(&ARGS_ELMT_DATA(&args).colr, colr, sizeof(Pgcolr));
      phg_add_el(PHG_CSS, &args);
   }
}

/*******************************************************************************
 * pset_edge_colr
 *
 * DESCR:	Creates a new element - Edge Color Attribute
 * RETURNS:	N/A
 */

void pset_edge_colr(
   Pgcolr *colr
   )
{
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_edge_colr);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_EDGE_COLR;
      ARGS_ELMT_SIZE(&args) = sizeof(Pgcolr);
      memcpy(&ARGS_ELMT_DATA(&args).colr, colr, sizeof(Pgcolr));
      phg_add_el(PHG_CSS, &args);
   }
}

/*******************************************************************************
 * pset_light_src_state
 *
 * DESCR:	Creates a new element - Set light source state
 * RETURNS:	N/A
 */

void pset_light_src_state(
   Pint_list *activation,
   Pint_list *deactivation
   )
{
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_light_src_state);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_LIGHT_SRC_STATE;
      ARGS_ELMT_SIZE(&args) = 2 * sizeof(Pint) +
          sizeof(Pint) * activation->num_ints +
          sizeof(Pint) * deactivation->num_ints;
      memcpy(&ARGS_ELMT_DATA(&args).lss.activation,
             activation,
             sizeof(Plss));
      memcpy(&ARGS_ELMT_DATA(&args).lss.deactivation,
             deactivation,
             sizeof(Plss));
      phg_add_el(PHG_CSS, &args);
   }
}

/*******************************************************************************
 * pset_int_shad_meth
 *
 * DESCR:	Creates a new element - Set interiour shading method
 * RETURNS:	N/A
 */

void pset_int_shad_meth(
   Pint shad_meth
   )
{
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_int_shad_meth);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_INT_SHAD_METH;
      ARGS_ELMT_SIZE(&args) = sizeof(Pint);
      ARGS_ELMT_DATA(&args).int_data = shad_meth;
      phg_add_el(PHG_CSS, &args);
   }
}

/*******************************************************************************
 * pset_back_int_shad_meth
 *
 * DESCR:	Creates a new element - Set backface interiour shading method
 * RETURNS:	N/A
 */

void pset_back_int_shad_meth(
   Pint shad_meth
   )
{
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_back_int_shad_meth);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_BACK_INT_SHAD_METH;
      ARGS_ELMT_SIZE(&args) = sizeof(Pint);
      ARGS_ELMT_DATA(&args).int_data = shad_meth;
      phg_add_el(PHG_CSS, &args);
   }
}

/*******************************************************************************
 * pset_refl_eqn
 *
 * DESCR:	Creates a new element - Set surface reflectance equation
 * RETURNS:	N/A
 */

void pset_refl_eqn(
   Pint refl_equ
   )
{
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_refl_eqn);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_INT_REFL_EQN;
      ARGS_ELMT_SIZE(&args) = sizeof(Pint);
      ARGS_ELMT_DATA(&args).int_data = refl_equ;
      phg_add_el(PHG_CSS, &args);
   }
}

/*******************************************************************************
 * pset_back_refl_eqn
 *
 * DESCR:	Creates a new element - Set backsurface reflectance equation
 * RETURNS:	N/A
 */

void pset_back_refl_eqn(
   Pint refl_equ
   )
{
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_back_refl_eqn);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_BACK_INT_REFL_EQN;
      ARGS_ELMT_SIZE(&args) = sizeof(Pint);
      ARGS_ELMT_DATA(&args).int_data = refl_equ;
      phg_add_el(PHG_CSS, &args);
   }
}

/*******************************************************************************
 * pset_refl_props
 *
 * DESCR:	Creates a new element - Set surface reflectance properties
 * RETURNS:	N/A
 */

void pset_refl_props(
   Prefl_props *refl_props
   )
{
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_refl_props);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_REFL_PROPS;
      ARGS_ELMT_SIZE(&args) = sizeof(Prefl_props);
      memcpy(&ARGS_ELMT_DATA(&args).props, refl_props, sizeof(Prefl_props));
      phg_add_el(PHG_CSS, &args);
   }
}

/*******************************************************************************
 * pset_back_refl_props
 *
 * DESCR:	Creates a new element - Set backsurface reflectance properties
 * RETURNS:	N/A
 */

void pset_back_refl_props(
   Prefl_props *refl_props
   )
{
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_back_refl_props);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_BACK_REFL_PROPS;
      ARGS_ELMT_SIZE(&args) = sizeof(Prefl_props);
      memcpy(&ARGS_ELMT_DATA(&args).props, refl_props, sizeof(Prefl_props));
      phg_add_el(PHG_CSS, &args);
   }
}

/*******************************************************************************
 * pset_face_disting_mode
 *
 * DESCR:	Creates a new element - Set facet distinguish mode
 * RETURNS:	N/A
 */

void pset_face_disting_mode(
   Pdisting_mode disting_mode
   )
{
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_face_disting_mode);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_FACE_DISTING_MODE;
      ARGS_ELMT_SIZE(&args) = sizeof(Pdisting_mode);
      ARGS_ELMT_DATA(&args).disting_mode = disting_mode;
      phg_add_el(PHG_CSS, &args);
   }
}

/*******************************************************************************
 * pset_face_cull_mode
 *
 * DESCR:	Creates a new element - Set face culling mode
 * RETURNS:	N/A
 */

void pset_face_cull_mode(
   Pcull_mode cull_mode
   )
{
   Phg_args_add_el args;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_face_cull_mode);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_FACE_CULL_MODE;
      ARGS_ELMT_SIZE(&args) = sizeof(Pcull_mode);
      ARGS_ELMT_DATA(&args).cull_mode = cull_mode;
      phg_add_el(PHG_CSS, &args);
   }
}

