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
 * priv_clear_area3
 *
 * DESCR:	Clear area 3D helper function
 * RETURNS:	N/A
 */

static void priv_clear_area3(
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
 * wsgl_clear_area3
 *
 * DESCR:	Clear area 3D
 * RETURNS:	N/A
 */

void wsgl_clear_area3(
   Ws *ws,
   void *data,
   Ws_attr_st *ast
   )
{
   uint32_t num_points;
   uint32_t *idata;

   idata = (uint32_t *) data;
   num_points = *idata;

   glPolygonOffset(WS_CLEAR_AREA_OFFSET, wsgl_get_edge_width(ast));
   glEnable(GL_POLYGON_OFFSET_FILL);
   wsgl_setup_background(ws);
   priv_clear_area3(num_points, (float32_t *) &idata[1]);
   glDisable(GL_POLYGON_OFFSET_FILL);
}

/*******************************************************************************
 * wsgl_clear_area_set3
 *
 * DESCR:	Clear area set 3D
 * RETURNS:	N/A
 */

void wsgl_clear_area_set3(
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

   glPolygonOffset(WS_CLEAR_AREA_OFFSET, wsgl_get_edge_width(ast));
   glEnable(GL_POLYGON_OFFSET_FILL);
   wsgl_setup_background(ws);

   for (i = 0; i < num_lists; i++) {
      num_points = *idata;
      fdata = (float32_t *) &idata[1];
      priv_clear_area3(num_points, fdata);
      idata = (uint32_t *) &fdata[num_points * 3];
   }

   glDisable(GL_POLYGON_OFFSET_FILL);
}

