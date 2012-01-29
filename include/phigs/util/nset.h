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

struct _Nset;
typedef struct _Nset *Nameset;

/*******************************************************************************
 * phg_nset_create
 *
 * DESCR:       Create nameset
 * RETURNS:     N/A
 */

Nameset phg_nset_create(
   unsigned size
   );

/*******************************************************************************
 * phg_nset_destroy
 *
 * DESCR:       Destroy nameset
 * RETURNS:     N/A
 */

void phg_nset_destroy(
   Nameset nset
   );

/*******************************************************************************
 * phg_nset_name_set
 *
 * DESCR:       Set name in nameset
 * RETURNS:     TRUE of FALSE
 */

int phg_nset_name_set(
   Nameset nset,
   Pint name
   );

/*******************************************************************************
 * phg_nset_name_clear
 *
 * DESCR:       Clear name in nameset
 * RETURNS:     TRUE or FALSE
 */

int phg_nset_name_clear(
   Nameset nset,
   Pint name
   );

/*******************************************************************************
 * phg_nset_name_is_set
 *
 * DESCR:       Find out if name set is set
 * RETURNS:     TRUE or FALSE
 */

int phg_nset_name_is_set(
   Nameset nset,
   Pint name
   );

#endif /* _nset_h */

