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

/*******************************************************************************
 * phg_nset_name_set
 *
 * DESCR:       Set name in nameset
 * RETURNS:     N/A
 */

void phg_nset_name_set(
   caddr_t data,
   Pint name
   )
{
   uint32_t bit;
   uint32_t *nset = (uint32_t *) data;

   bit = 0x1 << (name & 31);
   nset[name >> 5] |= bit;
}

/*******************************************************************************
 * phg_nset_name_clear
 *
 * DESCR:       Clear name in nameset
 * RETURNS:     N/A
 */

void phg_nset_name_clear(
   caddr_t data,
   Pint name
   )
{
   uint32_t bit;
   uint32_t *nset = (uint32_t *) data;

   bit = 0x1 << (name & 31);
   nset[name >> 5] &= ~bit;
}

/*******************************************************************************
 * phg_nset_names_set
 *
 * DESCR:       Set names in nameset from integer list
 * RETURNS:     N/A
 */

void phg_nset_names_set(
   caddr_t data,
   Pint num_names,
   Pint *name_list
   )
{
   Pint i;

   for (i = 0; i < num_names; i++) {
      phg_nset_name_set(data, name_list[i]);
   }
}

/*******************************************************************************
 * phg_nset_names_clear
 *
 * DESCR:       Clear names in nameset from integer list
 * RETURNS:     N/A
 */

void phg_nset_names_clear(
   caddr_t data,
   Pint num_names,
   Pint *name_list
   )
{
   Pint i;

   for (i = 0; i < num_names; i++) {
      phg_nset_name_clear(data, name_list[i]);
   }
}

/*******************************************************************************
 * phg_nset_names_set_all
 *
 * DESCR:       Set all names in nameset
 * RETURNS:     N/A
 */

void phg_nset_names_set_all(
   caddr_t data,
   Pint num_chunks
   )
{
   Pint i;
   uint32_t *nset = (uint32_t *) data;

   for (i = 0; i < num_chunks; i++) {
      nset[i] = 0xffffffff;
   }
}

/*******************************************************************************
 * phg_nset_names_clear_all
 *
 * DESCR:       Clear all names in nameset
 * RETURNS:     N/A
 */

void phg_nset_names_clear_all(
   caddr_t data,
   Pint num_chunks
   )
{
   Pint i;
   uint32_t *nset = (uint32_t *) data;

   for (i = 0; i < num_chunks; i++) {
      nset[i] = 0x00000000;
   }
}

/*******************************************************************************
 * phg_nset_names_intersect
 *
 * DESCR:       Evaluate if there is an intersection between the two name sets
 * RETURNS:     TRUE or FALSE
 */

int phg_nset_names_intersect(
   caddr_t data1,
   caddr_t data2,
   Pint num_chunks
   )
{
   int status;
   Pint i;
   uint32_t *nset1 = (uint32_t *) data1;
   uint32_t *nset2 = (uint32_t *) data2;

   status = FALSE;
   for (i = 0; i < num_chunks; i++) {
      if (nset1[i] & nset2[i]) {
         status = TRUE;
         break;
      }
   }

   return status;
}

/*******************************************************************************
 * phg_nset_name_is_set
 *
 * DESCR:       Find out if name is set in the nameset
 * RETURNS:     TRUE or FALSE
 */

int phg_nset_name_is_set(
   caddr_t data,
   Pint name
   )
{
   int status;
   uint32_t bit;
   uint32_t *nset = (uint32_t *) data;

   bit = 0x1 << (name & 31);
   if (nset[name >> 5] & bit) {
      status = TRUE;
   }
   else {
      status = FALSE;
   }

   return status;
}

