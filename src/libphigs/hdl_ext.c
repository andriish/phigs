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
 * hdl_sofas3
 *
 * DESCR:	Handle set of facet lists 3D
 * RETURNS:	TRUE on success, otherwise FALSE
 */

int hdl_sofas3(
   Css_handle cssh,
   El_handle elmt,
   caddr_t argdata,
   Css_el_op op
   )
{
   Psofas3 *sofas3;
   Pint *data;
   char *tp;
   Pint i, j;
   Pint num_lists, num_vertices;

   switch (op) {
      case CSS_EL_CREATE:
         ELMT_HEAD(elmt) = hdl_create((void *) &data, argdata);
         if (ELMT_HEAD(elmt) == NULL) {
            return (FALSE);
         }

         sofas3 = &ARGS_ELMT_DATA(argdata).sofas3;
         *data = sofas3->fflag;
         data = &data[1];
         *data = sofas3->eflag;
         data = &data[1];
         *data = sofas3->vflag;
         data = &data[1];
         *data = sofas3->colr_type;
         data = &data[1];
         *data = sofas3->num_sets;
         tp = (char *) &data[1];

         switch(sofas3->fflag) {
            case PFACET_COLOUR:
               memcpy(tp,
                      sofas3->fdata.colrs,
                      sizeof(Pcoval) * sofas3->num_sets);
               tp += sizeof(Pcoval) * sofas3->num_sets;
               break;

            case PFACET_NORMAL:
               memcpy(tp,
                      sofas3->fdata.norms,
                      sizeof(Pvec3) * sofas3->num_sets);
               tp += sizeof(Pvec3) * sofas3->num_sets;
               break;

            case PFACET_COLOUR_NORMAL:
               memcpy(tp,
                      sofas3->fdata.conorms,
                      sizeof(Pconorm3) * sofas3->num_sets);
               tp += sizeof(Pconorm3) * sofas3->num_sets;
               break;

            default:
               break;
         }

         if (sofas3->eflag == PEDGE_VISIBILITY) {

            for (i = 0; i < sofas3->num_sets; i++) {
               data = (Pint *) tp;
               num_lists = sofas3->edata[i].num_lists;
               *data = num_lists;
               tp = (char *) &data[1];
               for (j = 0; j < num_lists; j++) {
                  num_vertices = sofas3->edata[i].edgelist[j].num_edges;
                  data = (Pint *) tp;
                  *data = num_vertices;
                  tp = (char *) &data[1];
                  memcpy(tp,
                         sofas3->edata[i].edgelist[j].edgedata.edges,
                         sizeof(Pedge_flag) * num_vertices);
                  tp += sizeof(Pedge_flag) * num_vertices;
               }
            }
         }

         for (i = 0; i < sofas3->num_sets; i++) {
               data = (Pint *) tp;
               num_lists = sofas3->vlist[i].num_lists;
               *data = num_lists;
               tp = (char *) &data[1];
               for (j = 0; j < num_lists; j++) {
                  num_vertices = sofas3->vlist[i].lists[j].num_ints;
                  data = (Pint *) tp;
                  *data = num_vertices;
                  tp = (char *) &data[1];
                  memcpy(tp,
                         sofas3->vlist[i].lists[j].ints,
                         sizeof(Pint) * num_vertices);
                  tp += sizeof(Pint) * num_vertices;
               }
         }

         num_vertices = sofas3->vdata.num_vertices;

         data = (Pint *) tp;
         *data = num_vertices;
         tp = (char *) &data[1];

         switch (sofas3->vflag) {
            case PVERT_COORD:
               memcpy(tp,
                      sofas3->vdata.vertex_data.points,
                      num_vertices * sizeof(Ppoint3));
               break;

            case PVERT_COORD_COLOUR:
               memcpy(tp,
                      sofas3->vdata.vertex_data.ptcolrs,
                      num_vertices * sizeof(Pptco3));
               break;

            case PVERT_COORD_NORMAL:
               memcpy(tp,
                      sofas3->vdata.vertex_data.ptnorms,
                      num_vertices * sizeof(Pptnorm3));
               break;

            case PVERT_COORD_COLOUR_NORMAL:
               memcpy(tp,
                      sofas3->vdata.vertex_data.ptconorms,
                      num_vertices * sizeof(Pptconorm3));
               break;

            default:
               break;
         }

         break;

      case CSS_EL_REPLACE:
         break;

      case CSS_EL_COPY:
         break;

      case CSS_EL_INQ_CONTENT:
         ARGS_INQ_HEAD(argdata) = ELMT_HEAD(elmt);
         break;

      case CSS_EL_INQ_TYPE_SIZE:
         ARGS_INQ_SIZE(argdata) = ELMT_INFO_LEN(elmt);
         break;

      case CSS_EL_FREE:
         break;

      default:
         /* Default */
         return (FALSE);
         break;
   }

   return (TRUE);
}

