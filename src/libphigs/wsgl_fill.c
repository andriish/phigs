/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2014 Surplus Users Ham Society
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
#include <GL/gl.h>

#include "phg.h"
#include "private/phgP.h"
#include "ws.h"
#include "private/wsglP.h"

/*******************************************************************************
 * priv_fill_area
 *
 * DESCR:	Draw fill area helper function
 * RETURNS:	N/A
 */

void priv_fill_area(
   uint32_t num_points,
   float32_t *fdata
   )
{
   uint32_t i;

   glBegin(GL_POLYGON);
   for (i = 0; i < num_points; i++) {
      glVertex2f(fdata[0], fdata[1]);
      fdata += 2;
   }
   glEnd();
}

/*******************************************************************************
 * priv_normal3
 *
 * DESCR:	Calculate normal helper function
 * RETURNS:	N/A
 */

void priv_normal3(
   Pvec3 *norm,
   float32_t *fdata
   )
{
   Pvec3 a, b, c;
   Pvec3 v1, v2;

   a.delta_x = *fdata++;
   a.delta_y = *fdata++;
   a.delta_z = *fdata++;

   b.delta_x = *fdata++;
   b.delta_y = *fdata++;
   b.delta_z = *fdata++;

   c.delta_x = *fdata++;
   c.delta_y = *fdata++;
   c.delta_z = *fdata++;

   phg_vector_sub(&v1, &b, &a);
   phg_vector_sub(&v2, &c, &a);
   phg_vector_cross_prod(norm, &v1, &v2);
}

/*******************************************************************************
 * priv_fill_area3
 *
 * DESCR:	Draw fill area 3D helper function
 * RETURNS:	N/A
 */

void priv_fill_area3(
   uint32_t num_points,
   float32_t *fdata
   )
{
   uint32_t i;

   glBegin(GL_POLYGON);
   for (i = 0; i < num_points; i++) {
      glVertex3f(fdata[0], fdata[1], fdata[2]);
      fdata += 3;
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
   void *data,
   Ws_attr_st *ast
   )
{
   uint32_t num_points;
   uint32_t *idata;

   idata = (uint32_t *) data;
   num_points = *idata;

   wsgl_setup_int_attr(ws, ast);
   priv_fill_area(num_points, (float32_t *) &idata[1]);
}

/*******************************************************************************
 * wsgl_fill_area3
 *
 * DESCR:	Draw fill area 3D
 * RETURNS:	N/A
 */

void wsgl_fill_area3(
   Ws *ws,
   void *data,
   Ws_attr_st *ast
   )
{
   Pvec3 norm;
   uint32_t num_points;
   uint32_t *idata;
   float32_t *fdata;

   idata = (uint32_t *) data;
   num_points = *idata;
   fdata = (float32_t *) &idata[1];

   glPolygonOffset(WS_FILL_AREA_OFFSET, wsgl_get_edge_width(ast));
   glEnable(GL_POLYGON_OFFSET_FILL);
   glEnable(GL_POLYGON_OFFSET_LINE);
   if (wsgl_setup_int_attr_plus(ws, ast)) {
      priv_normal3(&norm, fdata);
      glNormal3f(norm.delta_x, norm.delta_y, norm.delta_z);
   }
   priv_fill_area3(num_points, fdata);
   glDisable(GL_POLYGON_OFFSET_LINE);
   glDisable(GL_POLYGON_OFFSET_FILL);
}

/*******************************************************************************
 * wsgl_back_area3
 *
 * DESCR:	Draw backface area 3D
 * RETURNS:	N/A
 */

void wsgl_back_area3(
   Ws *ws,
   void *data,
   Ws_attr_st *ast
   )
{
   Pvec3 norm;
   uint32_t num_points;
   uint32_t *idata;
   float32_t *fdata;

   idata = (uint32_t *) data;
   num_points = *idata;
   fdata = (float32_t *) &idata[1];

   glPolygonOffset(WS_FILL_AREA_OFFSET, wsgl_get_edge_width(ast));
   glEnable(GL_POLYGON_OFFSET_FILL);
   glEnable(GL_POLYGON_OFFSET_LINE);
   if (wsgl_setup_back_int_attr_plus(ws, ast)) {
      priv_normal3(&norm, fdata);
      glNormal3f(norm.delta_x, norm.delta_y, norm.delta_z);
   }
   priv_fill_area3(num_points, fdata);
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
   void *data,
   Ws_attr_st *ast
   )
{
   uint32_t i, num_lists, num_points;
   uint32_t *idata;
   float32_t *fdata;

   idata = (uint32_t *) data;
   num_lists = *idata;
   idata = &idata[1];

   wsgl_setup_int_attr(ws, ast);

   for (i = 0; i < num_lists; i++) {
      num_points = *idata;
      fdata = (float32_t *) &idata[1];
      priv_fill_area(num_points, fdata);
      idata = (uint32_t *) &fdata[num_points * 2];
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
   void *data,
   Ws_attr_st *ast
   )
{
   Pvec3 norm;
   uint32_t i, num_lists, num_points;
   uint32_t *idata;
   float32_t *fdata;

   idata = (uint32_t *) data;
   num_lists = *idata;
   idata = &idata[1];

   glPolygonOffset(WS_FILL_AREA_OFFSET, wsgl_get_edge_width(ast));
   glEnable(GL_POLYGON_OFFSET_FILL);
   glEnable(GL_POLYGON_OFFSET_LINE);
   if (wsgl_setup_int_attr_plus(ws, ast)) {
      priv_normal3(&norm, (float32_t *) &idata[1]);
      glNormal3f(norm.delta_x, norm.delta_y, norm.delta_z);
   }

   for (i = 0; i < num_lists; i++) {
      num_points = *idata;
      fdata = (float32_t *) &idata[1];
      priv_fill_area3(num_points, fdata);
      idata = (uint32_t *) &fdata[num_points * 3];
   }

   glDisable(GL_POLYGON_OFFSET_LINE);
   glDisable(GL_POLYGON_OFFSET_FILL);
}

