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
 * wsgl_polyline
 *
 * DESCR:       Draw lines
 * RETURNS:     N/A
 */

static void wsgl_polyline(
   void *pdata
   )
{
   int i;
   Ppoint_list point_list;
   Pint *data = (Pint *) pdata;

   point_list.num_points = *data;
   point_list.points = (Ppoint *) &data[1];

   glBegin(GL_LINES);
   for (i = 0; i < point_list.num_points; i++) {
      glVertex2f(point_list.points[i].x,
                 point_list.points[i].y);
   }
   glEnd();
}

/*******************************************************************************
 * wsgl_polyline3
 *
 * DESCR:       Draw lines 3D
 * RETURNS:     N/A
 */

static void wsgl_polyline3(
   void *pdata
   )
{
   int i;
   Ppoint_list3 point_list;
   Pint *data = (Pint *) pdata;

   point_list.num_points = *data;
   point_list.points = (Ppoint3 *) &data[1];

   glBegin(GL_LINES);
   for (i = 0; i < point_list.num_points; i++) {
      glVertex3f(point_list.points[i].x,
                 point_list.points[i].y,
                 point_list.points[i].z);
   }
   glEnd();
}

/*******************************************************************************
 * wsgl_render_line
 *
 * DESCR:	Render line element to current workstation rendering window
 * RETURNS:	N/A
 */

void wsgl_render_line(
   Ws_attr_st *ast,
   El_handle el
   )
{
   switch (el->eltype) {
      case PELEM_INDIV_ASF:
         wsgl_setup_line_attr(ast);
         break;

      case PELEM_LINE_IND:
         wsgl_setup_line_attr(ast);
         break;

      case PELEM_LINE_COLR_IND:
         if (phg_nset_name_is_set(&ast->asf_nameset,
                                  (Pint) PASPECT_LINE_COLR_IND)) {
             wsgl_set_gcolr(&ast->indiv_group.line_bundle.colr);
         }
         break;

      case PELEM_LINE_COLR:
         if (phg_nset_name_is_set(&ast->asf_nameset,
                                  (Pint) PASPECT_LINE_COLR_IND)) {
             wsgl_set_gcolr(&ast->indiv_group.line_bundle.colr);
         }
         break;

      case PELEM_LINEWIDTH:
         if (phg_nset_name_is_set(&ast->asf_nameset,
                                  (Pint) PASPECT_LINEWIDTH)) {
            glLineWidth(ast->indiv_group.line_bundle.width);
         }
         break;

      case PELEM_LINETYPE:
         if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_LINETYPE)) {
            wsgl_setup_linetype_attr(ast);
         }
         break;

      case PELEM_POLYLINE:
         wsgl_polyline(ELMT_CONTENT(el));
         break;

      case PELEM_POLYLINE3: 
         wsgl_polyline3(ELMT_CONTENT(el));
         break;

      default:
         break;
   }
}

