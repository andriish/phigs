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

#ifndef _attr_h
#define _attr_h

#include <phigs/phigs.h>

typedef struct {
   Pline_bundle   line_bundle;
   Pmarker_bundle marker_bundle;
   Pint_bundle    int_bundle;
   Pedge_bundle   edge_bundle;
   Ptext_bundle   text_bundle;
} Pattr_group;

/*******************************************************************************
 * phg_attr_group_create
 *
 * DESCR:       Create attribute group
 * RETURNS:     Pointer to attribute group or NULL
 */

Pattr_group* phg_attr_group_create(
   void
   );

/*******************************************************************************
 * phg_attr_group_init
 *
 * DESCR:       Initialize attribute group
 * RETURNS:     TRUE or FALSE
 */

int phg_attr_group_init(
   Pattr_group *attr_group
   );

/*******************************************************************************
 * phg_attr_group_destroy
 *
 * DESCR:       Destroy attribute group
 * RETURNS:     N/A
 */

void phg_attr_group_destroy(
   Pattr_group *attr_group
   );

#endif /* _attr_h */

