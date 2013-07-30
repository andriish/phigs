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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <GL/gl.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>
#include <phigs/util.h>
#include <phigs/private/wsxP.h>
#include <phigs/private/wsglP.h>

/*******************************************************************************
 * wsgl_fill_area
 *
 * DESCR:       Draw fill area
 * RETURNS:     N/A
 */

static void wsgl_fill_area(
   void *pdata
   )
{
   int i;
   Ppoint_list point_list;
   Pint *data = (Pint *) pdata;

   point_list.num_points = *data;
   point_list.points = (Ppoint *) &data[1];

   glBegin(GL_POLYGON);
   for (i = 0; i < point_list.num_points; i++) {
      glVertex2f(point_list.points[i].x,
                 point_list.points[i].y);
   }
   glEnd();
}

/*******************************************************************************
 * wsgl_fill_area3
 *
 * DESCR:       Draw fill area 3D
 * RETURNS:     N/A
 */

static void wsgl_fill_area3(
   void *pdata
   )
{
   int i;
   Ppoint_list3 point_list;
   Pint *data = (Pint *) pdata;

   point_list.num_points = *data;
   point_list.points = (Ppoint3 *) &data[1];

   glBegin(GL_POLYGON);
   for (i = 0; i < point_list.num_points; i++) {
      glVertex3f(point_list.points[i].x,
                 point_list.points[i].y,
                 point_list.points[i].z);
   }
   glEnd();
}

/*******************************************************************************
 * wsgl_fill_area_set
 *
 * DESCR:	Draw fill area set
 * RETURNS:	N/A
 */

static void wsgl_fill_area_set(
   void *pdata
   )
{
   Pint i, num_lists;
   Ppoint_list point_list;
   Pint *data = (Pint *) pdata;

   num_lists = *data;
   data = &data[1];
   for (i = 0; i < num_lists; i++) {
      point_list.num_points = *data;
      point_list.points = (Ppoint *) &data[1];
      wsgl_fill_area(data);
      data = (Pint *) &point_list.points[point_list.num_points];
   }
}

/*******************************************************************************
 * wsgl_fill_area_set3
 *
 * DESCR:	Draw fill area set 3D
 * RETURNS:	N/A
 */

static void wsgl_fill_area_set3(
   void *pdata
   )
{
   Pint i, num_lists;
   Ppoint_list3 point_list;
   Pint *data = (Pint *) pdata;

   num_lists = *data;
   data = &data[1];
   for (i = 0; i < num_lists; i++) {
      point_list.num_points = *data;
      point_list.points = (Ppoint3 *) &data[1];
      wsgl_fill_area3(data);
      data = (Pint *) &point_list.points[point_list.num_points];
   }
}

/*******************************************************************************
 * wsgl_render_fill
 *
 * DESCR:	Render fill element to current workstation rendering window
 * RETURNS:	N/A
 */

void wsgl_render_fill(
   Ws_attr_st *ast,
   El_handle el
   )
{
   switch (el->eltype) {
      case PELEM_INDIV_ASF:
         wsgl_setup_int_attr(ast);
         break;

      case PELEM_INT_IND:
         wsgl_setup_int_attr(ast);
         break;

      case PELEM_INT_COLR_IND:
         if (phg_nset_name_is_set(&ast->asf_nameset,
                                  (Pint) PASPECT_INT_COLR_IND)) {
             wsgl_set_gcolr(&ast->indiv_group.int_bundle.colr);
         }
         break;

      case PELEM_INT_COLR:
         if (phg_nset_name_is_set(&ast->asf_nameset,
                                  (Pint) PASPECT_INT_COLR_IND)) {
             wsgl_set_gcolr(&ast->indiv_group.int_bundle.colr);
         }
         break;

      case PELEM_INT_STYLE:
         /* TODO: In this case we really done need to setup all attributes */
         wsgl_setup_int_attr(ast);
         break;

      case PELEM_INT_STYLE_IND:
         /* TODO: In this case we really done need to setup all attributes */
         wsgl_setup_int_attr(ast);
         break;

      case PELEM_FILL_AREA:
         wsgl_fill_area(ELMT_CONTENT(el));
         break;

      case PELEM_FILL_AREA_SET:
         wsgl_fill_area_set(ELMT_CONTENT(el));
         break;

      case PELEM_FILL_AREA3:
         wsgl_fill_area3(ELMT_CONTENT(el));
         break;

      case PELEM_FILL_AREA_SET3:
         wsgl_fill_area_set3(ELMT_CONTENT(el));
         break;

      case PELEM_FILL_AREA3_DATA:
         /* TODO */
         break;

      default:
         break;
   }
}

