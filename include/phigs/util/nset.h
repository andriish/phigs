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

#ifndef _nset_h
#define _nset_h

#include <sys/types.h>

/*******************************************************************************
 * phg_nset_name_set
 *
 * DESCR:       Set name in nameset
 * RETURNS:     N/A
 */

void phg_nset_name_set(
   caddr_t data,
   Pint name
   );

/*******************************************************************************
 * phg_nset_name_clear
 *
 * DESCR:       Clear name in nameset
 * RETURNS:     N/A
 */

void phg_nset_name_clear(
   caddr_t data,
   Pint name
   );

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
   );

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
   );

/*******************************************************************************
 * phg_nset_names_set_all
 *
 * DESCR:       Set all names in nameset
 * RETURNS:     N/A
 */

void phg_nset_names_set_all(
   caddr_t data,
   Pint num_chunks
   );

/*******************************************************************************
 * phg_nset_names_clear_all
 *
 * DESCR:       Clear all names in nameset
 * RETURNS:     N/A
 */

void phg_nset_names_clear_all(
   caddr_t data,
   Pint num_chunks
   );

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
   );

/*******************************************************************************
 * phg_nset_name_is_set
 *
 * DESCR:       Find out if name is set in the nameset
 * RETURNS:     TRUE or FALSE
 */

int phg_nset_name_is_set(
   caddr_t data,
   Pint name
   );

#endif /* _nset_h */

