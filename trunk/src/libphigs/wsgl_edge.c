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
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>
#include <phigs/util.h>
#include <phigs/private/wsxP.h>
#include <phigs/private/wsglP.h>

/*******************************************************************************
 * wsgl_edge_type
 *
 * DESCR:       Set edge type
 * RETURNS:     N/A
 */

static void wsgl_edge_type(
    Pint type
    )
{
   /* Line style */
   switch (type) {
      case PLINE_DASH:
         glLineStipple(1, 0x00ff);
         glEnable(GL_LINE_STIPPLE);
      break;

      case PLINE_DOT:
         glLineStipple(1, 0x0101);
         glEnable(GL_LINE_STIPPLE);
      break;

      case PLINE_DASH_DOT:
         glLineStipple(1, 0x1c47);
         glEnable(GL_LINE_STIPPLE);
      break;

      default:
         glDisable(GL_LINE_STIPPLE);
      break;
   }
}

/*******************************************************************************
 * wsgl_edge_area
 *
 * DESCR:       Draw edge for fill area
 * RETURNS:     N/A
 */

static void wsgl_edge_area(
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
 * wsgl_edge_area3
 *
 * DESCR:       Draw edge for fill area 3D
 * RETURNS:     N/A
 */

static void wsgl_edge_area3(
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
 * wsgl_begin_edge
 *
 * DESCR:	Start rendering for edge pass
 * RETURNS:	N/A
 */

void wsgl_begin_edge(
    void
    )
{
   glPolygonOffset(-1, 1);
   glEnable(GL_POLYGON_OFFSET_LINE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

/*******************************************************************************
 * wsgl_end_edge
 *
 * DESCR:	Start rendering for edge pass
 * RETURNS:	N/A
 */

void wsgl_end_edge(
    void
    )
{
   glDisable(GL_POLYGON_OFFSET_LINE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/*******************************************************************************
 * wsgl_render_edge
 *
 * DESCR:	Render fill element to current workstation rendering window
 * RETURNS:	N/A
 */

void wsgl_render_edge(
   Ws_attr_st *ast,
   El_handle el
   )
{
   switch (el->eltype) {
      case PELEM_INDIV_ASF:
         wsgl_setup_edge_attr(ast);
         break;

      case PELEM_EDGE_IND:
         wsgl_setup_edge_attr(ast);
         break;

      case PELEM_EDGE_COLR_IND:
         if (phg_nset_name_is_set(&ast->asf_nameset,
                                  (Pint) PASPECT_EDGE_COLR_IND)) {
             wsgl_set_gcolr(&ast->indiv_group.edge_bundle.colr);
         }
         break;

      case PELEM_EDGE_COLR:
         if (phg_nset_name_is_set(&ast->asf_nameset,
                                  (Pint) PASPECT_EDGE_COLR_IND)) {
             wsgl_set_gcolr(&ast->indiv_group.edge_bundle.colr);
         }
         break;

      case PELEM_EDGEWIDTH:
         if (phg_nset_name_is_set(&ast->asf_nameset,
                                  (Pint) PASPECT_EDGEWIDTH)) {
            glLineWidth(ast->indiv_group.edge_bundle.width);
         }
         break;

      case PELEM_EDGETYPE:
         if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_EDGETYPE)) {
            wsgl_edge_type(ast->indiv_group.edge_bundle.type);
         }
         break;

      case PELEM_FILL_AREA:
         if (wsgl_get_edge_flag(ast) == PEDGE_ON) {
            wsgl_edge_area(ELMT_CONTENT(el));
         }
         break;

      case PELEM_FILL_AREA_SET:
         /* TODO */
         break;

      case PELEM_FILL_AREA3:
         if (wsgl_get_edge_flag(ast) == PEDGE_ON) {
            wsgl_edge_area3(ELMT_CONTENT(el));
         }
         break;

      case PELEM_FILL_AREA_SET3:
         /* TODO */
         break;

      case PELEM_FILL_AREA3_DATA:
         /* TODO */
         break;

      default:
         break;
   }
}

