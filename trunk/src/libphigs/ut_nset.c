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
 * phg_name_set
 *
 * DESCR:       Set name in nameset
 * RETURNS:     N/A
 */

void phg_name_set(
   caddr_t data,
   Pint name
   )
{
   uint32_t bit;
   uint32_t *nameset = (uint32_t *) data;

   bit = 0x1 << (name & 31);
   nameset[name >> 5] |= bit;
}

/*******************************************************************************
 * phg_name_clear
 *
 * DESCR:       Clear name in nameset
 * RETURNS:     N/A
 */

void phg_name_clear(
   caddr_t data,
   Pint name
   )
{
   uint32_t bit;
   uint32_t *nameset = (uint32_t *) data;

   bit = 0x1 << (name & 31);
   nameset[name >> 5] &= ~bit;
}

/*******************************************************************************
 * phg_name_is_set
 *
 * DESCR:       Find out if name is set in the nameset
 * RETURNS:     TRUE or FALSE
 */

int phg_name_is_set(
   caddr_t data,
   Pint name
   )
{
   int status;
   uint32_t bit;
   uint32_t *nameset = (uint32_t *) data;

   bit = 0x1 << (name & 31);
   if (nameset[name >> 5] & bit) {
      status = TRUE;
   }
   else {
      status = FALSE;
   }

   return status;
}

