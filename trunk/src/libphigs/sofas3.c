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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/gl.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>
#include <phigs/private/wsglP.h>
#include <phigs/private/sofas3P.h>

/*******************************************************************************
 * priv_vdata
 *
 * DESCR:	Get set of fill area set vertex data helper function
 * NOTES:	Shall be called after all vertex lists has been processed
 * RETURNS:	N/A
 */

static void priv_vdata(
   Psofas3 *sofas3
   )
{
   Pint *data = (Pint *) sofas3->vlist;

   sofas3->vdata.num_vertices = *data;
   switch (sofas3->vflag) {
      case PVERT_COORD:
         sofas3->vdata.vertex_data.points = (Ppoint3 *) &data[1];
         break;

      case PVERT_COORD_COLOUR:
         sofas3->vdata.vertex_data.ptcolrs = (Pptco3 *) &data[1];
         break;

      case PVERT_COORD_NORMAL:
         sofas3->vdata.vertex_data.ptnorms = (Pptnorm3 *) &data[1];
         break;

      case PVERT_COORD_COLOUR_NORMAL:
         sofas3->vdata.vertex_data.ptconorms = (Pptconorm3 *) &data[1];
         break;

      default:
         break;
   }
}

/*******************************************************************************
 * sofas3_head
 *
 * DESCR:	Get set of fill area set header information 3D
 * RETURNS:	N/A
 */

void sofas3_head(
   Psofas3 *sofas3,
   void *pdata
   )
{
   Pint i, j;
   Pint num_lists;
   Pint_list vlist;
   char *tp;
   Pint *data = (Pint *) pdata;

   sofas3->fflag = data[0];
   sofas3->eflag = data[1];
   sofas3->vflag = data[2];
   sofas3->colr_type = data[3];
   sofas3->num_sets = data[4];
   tp = (char *) &data[5];

   switch (sofas3->fflag) {
      case PFACET_COLOUR:
         sofas3->fdata.colrs = (Pcoval *) tp;
         tp += sizeof(Pcoval) * sofas3->num_sets;
         break;

      case PFACET_NORMAL:
         sofas3->fdata.norms = (Pvec3 *) tp;
         tp += sizeof(Pvec3) * sofas3->num_sets;
         break;

      case PFACET_COLOUR_NORMAL:
         sofas3->fdata.conorms = (Pconorm3 *) tp;
         tp += sizeof(Pconorm3) * sofas3->num_sets;
         break;

      default:
         break;
   }

   /* TODO: Setup and move past edge lists */

   sofas3->vlist = (Pint_list_list *) tp;

   /* Move forward and setup vertext lists */
   for (i = 0; i < sofas3->num_sets; i++) {
      num_lists = sofas3_num_vlists(sofas3);
      for (j = 0; j < num_lists; j++) {
         sofas3_next_vlist(&vlist, sofas3);
      }
   }

   /* Setup pointers to vertex data */
   priv_vdata(sofas3);

   /* Restore pointer to first vertex list */
   sofas3->vlist = (Pint_list_list *) tp;
}

/*******************************************************************************
 * sofas3_num_vlists
 *
 * DESCR:	Get set of fill area set number of vertex lists
 * RETURNS:	Total number of vertext lists
 */

int sofas3_num_vlists(
    Psofas3 *sofas3
    )
{
   Pint *data = (Pint *) sofas3->vlist;
   Pint num_lists = data[0];
   sofas3->vlist = (Pint_list_list *) &data[1];

   return num_lists;
}

/*******************************************************************************
 * sofas3_next_vlist
 *
 * DESCR:	Get set of fill area set next vertex list
 * RETURNS:	N/A
 */

void sofas3_next_vlist(
    Pint_list *vlist,
    Psofas3 *sofas3
    )
{
   Pint *data = (Pint *) sofas3->vlist;

   vlist->num_ints = data[0];
   data = &data[1];
   vlist->ints = data;
   sofas3->vlist = (Pint_list_list *) &data[vlist->num_ints];
}

/*******************************************************************************
 * sofas3_print
 *
 * DESCR:	Print set of fill area set 3D
 * RETURNS:	N/A
 */

void sofas3_print(
   Psofas3 *sofas3
   )
{
   Pint i, j, k;
   Pint num_lists;
   Pint_list vlist;

   printf("Facet flags:\t%d\n", sofas3->fflag);
   printf("Edge flags:\t%d\n", sofas3->eflag);
   printf("Vertex flags:\t%d\n", sofas3->vflag);
   printf("Colour type:\t%d\n", sofas3->colr_type);
   printf("Num sets:\t%d\n", sofas3->num_sets);

   printf("\n");
   for (i = 0; i < sofas3->num_sets; i++) {
      printf("Set #%d\t", i);
      switch (sofas3->fflag) {
         case PFACET_COLOUR:
            printf("Colour: %g, %g, %g\n",
                   sofas3->fdata.colrs[i].direct.rgb.red,
                   sofas3->fdata.colrs[i].direct.rgb.green,
                   sofas3->fdata.colrs[i].direct.rgb.blue);
            break;

         case PFACET_NORMAL:
            printf("Normal: %g, %g, %g\n",
                   sofas3->fdata.norms[i].delta_x,
                   sofas3->fdata.norms[i].delta_y,
                   sofas3->fdata.norms[i].delta_z);
            break;

         case PFACET_COLOUR_NORMAL:
            printf("Colour: %g, %g, %g\n",
                   sofas3->fdata.conorms[i].colr.direct.rgb.red,
                   sofas3->fdata.conorms[i].colr.direct.rgb.green,
                   sofas3->fdata.conorms[i].colr.direct.rgb.blue);
            printf("Normal: %g, %g, %g\n",
                   sofas3->fdata.conorms[i].norm.delta_x,
                   sofas3->fdata.conorms[i].norm.delta_y,
                   sofas3->fdata.conorms[i].norm.delta_z);
            break;

         default:
            break;
      }
   }

   /* TODO: Get edge data */

   printf("\n");
   for (i = 0; i < sofas3->num_sets; i++) {
      num_lists = sofas3_num_vlists(sofas3);
      printf("Set #%d, num lists: %d\n", i, num_lists);
      for (j = 0; j < num_lists; j++) {
         sofas3_next_vlist(&vlist, sofas3);
         printf("\tList #%d, num_ints: %d\t", j, vlist.num_ints);
         for (k = 0; k < vlist.num_ints; k++) {
            printf("%d ", vlist.ints[k]);
         }
         printf("\n");
      }
   }

   priv_vdata(sofas3);
   printf("\nNum vertices: %d\n", sofas3->vdata.num_vertices);
   for (i = 0; i < sofas3->vdata.num_vertices; i++) {
       printf("#%d:\t%g %g %g\n", i,
              sofas3->vdata.vertex_data.points[i].x,
              sofas3->vdata.vertex_data.points[i].y,
              sofas3->vdata.vertex_data.points[i].z);
   }
}

/*******************************************************************************
 * sofas3_list
 *
 * DESCR:	List set of fill area set 3D
 * RETURNS:	N/A
 */

void sofas3_list(
   void *pdata
   )
{
   Psofas3 sofas3;

   sofas3_head(&sofas3, pdata);
   sofas3_print(&sofas3);
}

