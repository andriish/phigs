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
#include <sys/types.h>
#include <phigs/phg.h>
#include <phigs/util/nset.h>

typedef struct _Nset {
   unsigned max_names;
   caddr_t  nameset;
} Nset;

/*******************************************************************************
 * phg_nset_create
 *
 * DESCR:       Create nameset
 * RETURNS:     N/A
 */

Nameset phg_nset_create(
   unsigned size
   )
{
   Nameset nset;

   nset = (Nameset) calloc(1, sizeof(Nset) + (size / 32) * sizeof(uint32_t));
   if (nset != NULL) {
      nset->max_names = size;
      nset->nameset   = (caddr_t) &nset[1];
   }

   return nset;
}

/*******************************************************************************
 * phg_nset_destroy
 *
 * DESCR:       Destroy nameset
 * RETURNS:     N/A
 */

void phg_nset_destroy(
   Nameset nset
   )
{
   free(nset);
}

/*******************************************************************************
 * phg_nset_name_set
 *
 * DESCR:       Set name in nameset
 * RETURNS:     TRUE of FALSE
 */

int phg_nset_name_set(
   Nameset nset,
   Pint name
   )
{
   int status;
   uint32_t bit;
   uint32_t *nameset = (uint32_t *) nset->nameset;

   if (name > nset->max_names) {
      status = FALSE;
   }
   else {
      bit = 0x1 << (name & 31);
      nameset[name >> 5] |= bit;
      status = TRUE;
   }

   return status;
}

/*******************************************************************************
 * phg_nset_name_clear
 *
 * DESCR:       Clear name in nameset
 * RETURNS:     TRUE or FALSE
 */

int phg_nset_name_clear(
   Nameset nset,
   Pint name
   )
{
   int status;
   uint32_t bit;
   uint32_t *nameset = (uint32_t *) nset->nameset;

   if (name > nset->max_names) {
      status = FALSE;
   }
   else {
      bit = 0x1 << (name & 31);
      nameset[name >> 5] &= ~bit;
      status = TRUE;
   }

   return status;
}

/*******************************************************************************
 * phg_nset_name_is_set
 *
 * DESCR:       Find out if name set is set
 * RETURNS:     TRUE or FALSE
 */

int phg_nset_name_is_set(
   Nameset nset,
   Pint name
   )
{
   int status;
   uint32_t bit;
   uint32_t *nameset = (uint32_t *) nset->nameset;

   if (name > nset->max_names) {
      status = FALSE;
   }
   else {
      bit = 0x1 << (name & 31);
      if (nameset[name >> 5] & bit) {
         status = TRUE;
      }
      else {
         status = FALSE;
      }
   }

   return status;
}

