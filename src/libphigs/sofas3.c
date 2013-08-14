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
 * sofas3_head
 *
 * DESCR:	Get set of fill area set head information 3D
 * RETURNS:	N/A
 */

void sofas3_head(
   Psofas3 *sofas3,
   void *pdata
   )
{
   Pint *data = (Pint *) pdata;

   sofas3->fflag = data[0];
   sofas3->eflag = data[1];
   sofas3->vflag = data[2];
   sofas3->colr_type = data[3];
   sofas3->num_sets = data[4];

   switch (sofas3->fflag) {
      case PFACET_COLOUR:
         sofas3->fdata.colrs = (Pcoval *) &data[5];
         break;

      case PFACET_NORMAL:
         sofas3->fdata.norms = (Pvec3 *) &data[5];
         break;

      case PFACET_COLOUR_NORMAL:
         sofas3->fdata.conorms = (Pconorm3 *) &data[5];
         break;

      default:
         break;
   }
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
   int i;

   printf("Facet flags: %d\n", sofas3->fflag);
   printf("Edge flags: %d\n", sofas3->eflag);
   printf("Vertex flags: %d\n", sofas3->vflag);
   printf("Colour type: %d\n", sofas3->colr_type);
   printf("Num sets: %d\n", sofas3->num_sets);

   for (i = 0; i < sofas3->num_sets; i++) {
      printf("#%d\t", i);
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

