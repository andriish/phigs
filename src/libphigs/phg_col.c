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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <phigs/phg.h>
#include <phigs/ws.h>
#include <phigs/private/phgP.h>

/*******************************************************************************
 * phg_get_colr_ind
 *
 * DESCR:       Get colour from index
 * RETURNS:     N/A
 */

void phg_get_colr_ind(
   Ws *ws,
   Pgcolr *gcolr,
   Pint ind
   )
{
   gcolr->type = ws->current_colour_model;
   switch(gcolr->type) {
      case PINDIRECT:
         gcolr->val.ind = ws->colr_table[ind].val.ind;
         break;

      case PMODEL_RGB:
         gcolr->val.general.x = ws->colr_table[ind].val.general.x;
         gcolr->val.general.y = ws->colr_table[ind].val.general.y;
         gcolr->val.general.z = ws->colr_table[ind].val.general.z;
         break;

      default:
         break;
   }
}

/*******************************************************************************
 * pset_colr_rep
 *
 * DESCR:       Set workstation colour representation
 * RETURNS:     N/A
 */

void pset_colr_rep(
   Pint ws_id,
   Pint index,
   Pcolr_rep *rep
   )
{
   Ws *wsh;
   Phg_args_rep_data corep;

   wsh = PHG_WSID(ws_id);
   corep.index = index;
   memcpy(&corep.bundl.corep, rep, sizeof(Pcolr_rep));

   (*wsh->set_rep)(wsh, PHG_ARGS_COREP, &corep);
}

