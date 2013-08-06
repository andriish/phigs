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
   int i;
   Pint_style style;
   Ppoint_list point_list;
   Pint *data = (Pint *) pdata;

   point_list.num_points = *data;
   point_list.points = (Ppoint *) &data[1];

   style = wsgl_get_int_style(ast);
   if (style != PSTYLE_EMPTY) {
      wsgl_setup_int_attr(ast);
      glBegin(GL_POLYGON);
      for (i = 0; i < point_list.num_points; i++) {
         glVertex2f(point_list.points[i].x,
                    point_list.points[i].y);
      }
      glEnd();
   }

   if (wsgl_get_edge_flag(ast) == PEDGE_ON) {
      wsgl_setup_edge_attr(ast);
      glBegin(GL_LINE_LOOP);
      for (i = 0; i < point_list.num_points; i++) {
         glVertex2f(point_list.points[i].x,
                    point_list.points[i].y);
      }
      glEnd();
   }
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
   int i;
   Pint_style style;
   Pedge_flag flag;
   Ppoint_list3 point_list;
   Wsgl_handle wsgl = ws->render_context;
   Pint *data = (Pint *) pdata;

   point_list.num_points = *data;
   point_list.points = (Ppoint3 *) &data[1];

   style = wsgl_get_int_style(ast);
   flag = wsgl_get_edge_flag(ast);

   wsgl_set_polygon_offset(wsgl_get_edge_width(ast));
   glEnable(GL_POLYGON_OFFSET_FILL);
   glEnable(GL_POLYGON_OFFSET_LINE);

   if  ((style == PSTYLE_EMPTY) || (style == PSTYLE_HOLLOW)) {

      /* If hidden surface removal, clear interiour to background colour */
      if (wsgl->cur_struct.hlhsr_id == PHIGS_HLHSR_ID_ON) {
         wsgl_setup_background(ws);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list.num_points; i++) {
            glVertex3f(point_list.points[i].x,
                       point_list.points[i].y,
                       point_list.points[i].z);
         }
         glEnd();
      }

      if (style == PSTYLE_HOLLOW) {
         wsgl_setup_int_attr(ast);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list.num_points; i++) {
            glVertex3f(point_list.points[i].x,
                       point_list.points[i].y,
                       point_list.points[i].z);
         }
         glEnd();
      }
   }
   else {
      wsgl_setup_int_attr(ast);
      glBegin(GL_POLYGON);
      for (i = 0; i < point_list.num_points; i++) {
         glVertex3f(point_list.points[i].x,
                    point_list.points[i].y,
                    point_list.points[i].z);
      }
      glEnd();
   }

   if (flag == PEDGE_ON) {
      wsgl_setup_edge_attr(ast);
      glBegin(GL_LINE_LOOP);
      for (i = 0; i < point_list.num_points; i++) {
         glVertex3f(point_list.points[i].x,
                    point_list.points[i].y,
                    point_list.points[i].z);
      }
      glEnd();
   }

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
   for (i = 0; i < num_lists; i++) {
      point_list.num_points = *data;
      point_list.points = (Ppoint *) &data[1];
      wsgl_fill_area(ws, data, ast);
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
   for (i = 0; i < num_lists; i++) {
      point_list.num_points = *data;
      point_list.points = (Ppoint3 *) &data[1];
      wsgl_fill_area3(ws, data, ast);
      data = (Pint *) &point_list.points[point_list.num_points];
   }
}

