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
   Phg_ret ret;

   gcolr->type = ws->current_colour_model;

   if (ws->current_colour_model == PINDIRECT) {
      gcolr->val.ind = ind;
   }
   else {
      (*ws->inq_representation)(ws,
                                ind,
                                PINQ_REALIZED,
                                PHG_ARGS_COREP,
                                &ret);
      if (ret.err == 0) {

            gcolr->val.general.x = ret.data.rep.corep.rgb.red;
            gcolr->val.general.y = ret.data.rep.corep.rgb.green;
            gcolr->val.general.z = ret.data.rep.corep.rgb.blue;
      }
   }
}

