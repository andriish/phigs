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
#include <phigs/phg.h>

/*******************************************************************************
 * phg_wst_create
 *
 * DESCR:       Create workstation type structure
 * RETURNS:     Pointer to workstation type or NULL
 */

Wst* phg_wst_create(
   Wst_base_type base_type
   )
{
   Wst *wst;

   wst = (Wst *) malloc(sizeof(Wst));
   if (wst == NULL) {
      ERR_REPORT(PHG_ERH, ERR900);
   }
   else if (!phg_wst_init(wst, base_type)) {
      ERR_REPORT(PHG_ERH, ERR900);
   }

   return wst;
}

/*******************************************************************************
 * phg_wst_init
 *
 * DESCR:       Initialize workstation type structure
 * RETURNS:     TRUE or FALSE
 */

int phg_wst_init(
   Wst *wst,
   Wst_base_type base_type
   )
{
   wst->wsid = -1;
   wst->base_type = base_type;

   return TRUE;
}

/*******************************************************************************
 * phg_wst_destroy
 *
 * DESCR:       Destroy workstation type structure
 * RETURNS:     N/A
 */

void phg_wst_destroy(
   Wst *wst
   )
{
   if (wst != NULL) {
      free(wst);
   }
}

