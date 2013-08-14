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

#ifndef _sofas3P_h
#define _sofas3P_h

#include <phigs/phigs.h>

/*******************************************************************************
 * sofas3_head
 *
 * DESCR:       Get set of fill area set header information 3D
 * RETURNS:     N/A
 */

void sofas3_head(
   Psofas3 *sofas3,
   void *pdata
   );

/*******************************************************************************
 * sofas3_next_vlist
 *
 * DESCR:       Get set of fill area set next vertex list
 * RETURNS:     N/A
 */

void sofas3_next_vlist(
    Pint_list *vlist,
    Psofas3 *sofas3
    );

/*******************************************************************************
 * sofas3_print
 *
 * DESCR:       Print set of fill area set 3D
 * RETURNS:     N/A
 */

void sofas3_print(
   Psofas3 *sofas3
   );

/*******************************************************************************
 * sofas3_list
 *
 * DESCR:       List set of fill area set 3D
 * RETURNS:     N/A
 */

void sofas3_list(
   void *pdata
   );

#endif /* _sofas3P_h */
