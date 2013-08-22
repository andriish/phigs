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
 * ppl
 *
 * DESCR:	Creates a new element - Polyline
 * RETURNS:	N/A
 */

void ppl_(
   Pint *n,
   Pfloat *pxa,
   Pfloat *pya
   )
{
   Pint i;
   unsigned size;
   Phg_args_add_el args;

   Ppoint_list point_list;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_polyline);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      point_list.num_points = *n;
      size = sizeof(Ppoint) * point_list.num_points;
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, size)) {
         ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
         point_list.points = (Ppoint *) PHG_SCRATCH.buf;
         for (i = 0; i < point_list.num_points; i++) {
            point_list.points[i].x = pxa[i];
            point_list.points[i].y = pya[i];
         }
         ARGS_ELMT_TYPE(&args) = PELEM_POLYLINE;
         ARGS_ELMT_SIZE(&args) = sizeof(Pint) + size;
         ARGS_ELMT_DATA(&args).point_list.num_points = point_list.num_points;
         ARGS_ELMT_DATA(&args).point_list.points = point_list.points;
         phg_add_el(PHG_CSS, &args);
      }
   }
}

