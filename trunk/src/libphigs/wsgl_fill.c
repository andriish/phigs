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
#include <math.h>
#include <GL/gl.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>
#include <phigs/private/wsglP.h>

#define FILL_AREA_OFFSET       1.0

/*******************************************************************************
 * priv_fill_area
 *
 * DESCR:	Draw fill area helper function
 * RETURNS:	N/A
 */

void priv_fill_area(
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
 * priv_fill_area3
 *
 * DESCR:	Draw fill area 3D helper function
 * RETURNS:	N/A
 */

void priv_fill_area3(
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
 * wsgl_fill_area
 *
 * DESCR:	Draw fill area
 * RETURNS:	N/A
 */

void wsgl_fill_area(
   Ws *ws,
   void *pdata,
   Ws_attr_st *ast
   )
{
   wsgl_setup_int_attr(ast);
   priv_fill_area(pdata);
}

/*******************************************************************************
 * wsgl_fill_area3
 *
 * DESCR:	Draw fill area 3D
 * RETURNS:	N/A
 */

void wsgl_fill_area3(
   Ws *ws,
   void *pdata,
   Ws_attr_st *ast
   )
{
   glPolygonOffset(FILL_AREA_OFFSET, wsgl_get_edge_width(ast));
   glEnable(GL_POLYGON_OFFSET_FILL);
   glEnable(GL_POLYGON_OFFSET_LINE);
   wsgl_setup_int_attr(ast);
   priv_fill_area3(pdata);
   glDisable(GL_POLYGON_OFFSET_LINE);
   glDisable(GL_POLYGON_OFFSET_FILL);
}

/*******************************************************************************
 * wsgl_fill_area_set
 *
 * DESCR:	Draw fill area set
 * RETURNS:	N/A
 */

void wsgl_fill_area_set(
   Ws *ws,
   void *pdata,
   Ws_attr_st *ast
   )
{
   Pint i, num_lists;
   Ppoint_list point_list;
   Pint *data = (Pint *) pdata;

   num_lists = *data;
   data = &data[1];

   wsgl_setup_int_attr(ast);

   for (i = 0; i < num_lists; i++) {
      point_list.num_points = *data;
      point_list.points = (Ppoint *) &data[1];
      priv_fill_area(data);
      data = (Pint *) &point_list.points[point_list.num_points];
   }
}

/*******************************************************************************
 * wsgl_fill_area_set3
 *
 * DESCR:	Draw fill area set 3D
 * RETURNS:	N/A
 */

void wsgl_fill_area_set3(
   Ws *ws,
   void *pdata,
   Ws_attr_st *ast
   )
{
   Pint i, num_lists;
   Ppoint_list3 point_list;
   Pint *data = (Pint *) pdata;

   num_lists = *data;
   data = &data[1];

   glPolygonOffset(FILL_AREA_OFFSET, wsgl_get_edge_width(ast));
   glEnable(GL_POLYGON_OFFSET_FILL);
   glEnable(GL_POLYGON_OFFSET_LINE);
   wsgl_setup_int_attr(ast);

   for (i = 0; i < num_lists; i++) {
      point_list.num_points = *data;
      point_list.points = (Ppoint3 *) &data[1];
      priv_fill_area3(data);
      data = (Pint *) &point_list.points[point_list.num_points];
   }

   glDisable(GL_POLYGON_OFFSET_LINE);
   glDisable(GL_POLYGON_OFFSET_FILL);
}

