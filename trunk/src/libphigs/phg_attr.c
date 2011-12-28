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

#include <string.h>
#include <phigs/phg.h>
#include <phigs/css.h>
#include <phigs/private/phgP.h>
#include <phigs/private/cssP.h>
#include <phigs/mat_utils.h>
#include <phigs/ws.h>

/*******************************************************************************
 * phg_attr_group_create
 *
 * DESCR:	Create attribute group
 * RETURNS:	Pointer to attribute group or NULL
 */

Pattr_group* phg_attr_group_create(
   void
   )
{
   Pattr_group *attr_group;

   attr_group = (Pattr_group *) malloc(sizeof(Pattr_group));
   if (attr_group != NULL) {
      if (!phg_attr_group_init(attr_group)) {
         ERR_REPORT(PHG_ERH, ERR900);
         attr_group = NULL;
      }
   }

   return attr_group;
}

/*******************************************************************************
 * phg_attr_group_init
 *
 * DESCR:	Initialize attribute group
 * RETURNS:	TRUE or FALSE
 */

int phg_attr_group_init(
   Pattr_group *attr_group
   )
{
   attr_group->line_bundle.type       = PLINE_SOLID;
   attr_group->line_bundle.width      = 1.0;
   //attr_group->line_bundle.colr_ind   = 0;

   attr_group->marker_bundle.type     = PMARKER_DOT;
   attr_group->marker_bundle.size     = 1.0;
   //attr_group->marker_bundle.colr_ind = 0;

   attr_group->int_bundle.style       = PSTYLE_SOLID;
   //attr_group->int_bundle.colr_ind    = 0;

   attr_group->edge_bundle.flag       = PEDGE_ON;
   attr_group->edge_bundle.type       = PLINE_SOLID;
   attr_group->edge_bundle.width      = 1.0;
   //attr_group->edge_bundle.colr_ind   = 0;

   return TRUE;
}

/*******************************************************************************
 * phg_attr_group_destroy
 *
 * DESCR:	Destroy attribute group
 * RETURNS:	N/A
 */

void phg_attr_group_destroy(
   Pattr_group *attr_group
   )
{
   if (attr_group != NULL) {
      free(attr_group);
   }
}

