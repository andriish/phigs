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
 * wsgl_marker_dot
 *
 * DESCR:       Draw marker dots helper function
 * RETURNS:     N/A
 */

static void wsgl_marker_dot(
   Ppoint_list *point_list,
   Pfloat scale
   )
{
   int i;

   glPointSize(scale);
   glBegin(GL_POINTS);
   for (i = 0; i < point_list->num_points; i++) {
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y);
   }
   glEnd();
}

/*******************************************************************************
 * wsgl_marker_plus
 *
 * DESCR:       Draw marker pluses helper function
 * RETURNS:     N/A
 */

static void wsgl_marker_plus(
   Ppoint_list *point_list,
   Pfloat scale
   )
{
   int i;
   float half_scale;

   half_scale = scale / 2.0;

   glLineWidth(1.0);
   glDisable(GL_LINE_STIPPLE);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++) {
      glVertex2f(point_list->points[i].x - half_scale,
                 point_list->points[i].y);
      glVertex2f(point_list->points[i].x + half_scale,
                 point_list->points[i].y);
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y - half_scale);
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y + half_scale);
   }
   glEnd();
}

/*******************************************************************************
 * phg_draw_marker_asterisk
 *
 * DESCR:       Draw marker asterisks helper function
 * RETURNS:     N/A
 */

static void wsgl_marker_asterisk(
   Ppoint_list *point_list,
   Pfloat scale
   )
{
   int i;
   float half_scale, small_scale;

   half_scale = scale / 2.0;
   small_scale = half_scale / 1.414;

   glLineWidth(1.0);
   glDisable(GL_LINE_STIPPLE);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++) {
      glVertex2f(point_list->points[i].x - half_scale,
                 point_list->points[i].y);
      glVertex2f(point_list->points[i].x + half_scale,
                 point_list->points[i].y);
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y - half_scale);
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y + half_scale);

      glVertex2f(point_list->points[i].x - small_scale,
                 point_list->points[i].y + small_scale);
      glVertex2f(point_list->points[i].x + small_scale,
                 point_list->points[i].y - small_scale);
      glVertex2f(point_list->points[i].x - small_scale,
                 point_list->points[i].y - small_scale);
      glVertex2f(point_list->points[i].x + small_scale,
                 point_list->points[i].y + small_scale);
   }
   glEnd();
}

/*******************************************************************************
 * wsgl_marker_cross
 *
 * DESCR:       Draw marker crosses helper function
 * RETURNS:     N/A
 */

static void wsgl_marker_cross(
   Ppoint_list *point_list,
   Pfloat scale
   )
{
   int i;
   float half_scale;

   half_scale = scale / 2.0;

   glLineWidth(1.0);
   glDisable(GL_LINE_STIPPLE);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++) {
      glVertex2f(point_list->points[i].x - half_scale,
                 point_list->points[i].y + half_scale);
      glVertex2f(point_list->points[i].x + half_scale,
                 point_list->points[i].y - half_scale);
      glVertex2f(point_list->points[i].x - half_scale,
                 point_list->points[i].y - half_scale);
      glVertex2f(point_list->points[i].x + half_scale,
                 point_list->points[i].y + half_scale);
   }
   glEnd();
}

/*******************************************************************************
 * wsgl_polymarker
 *
 * DESCR:       Draw markers
 * RETURNS:     N/A
 */

void wsgl_polymarker(
   void *pdata,
   Ws_attr_st *ast
   )
{
   Pint type;
   Pfloat size;
   Ppoint_list point_list;
   Pint *data = (Pint *) pdata;

   point_list.num_points = *data;
   point_list.points = (Ppoint *) &data[1];

   type = wsgl_get_marker_type(ast);
   size = wsgl_get_marker_size(ast);

   switch (type) {
      case PMARKER_DOT:
         wsgl_marker_dot(&point_list, size);
      break;

      case PMARKER_PLUS:
         wsgl_marker_plus(&point_list, size);
      break;

      case PMARKER_ASTERISK:
         wsgl_marker_asterisk(&point_list, size);
      break;

      case PMARKER_CROSS:
         wsgl_marker_cross(&point_list, size);
      break;
   }
}

/*******************************************************************************
 * wsgl_polymarker3
 *
 * DESCR:       Draw markers 3D
 * RETURNS:     N/A
 */

void wsgl_polymarker3(
   void *pdata,
   Ws_attr_st *ast
   )
{
   Pint type;
   Pfloat size;
   int i;
   Ppoint_list plist;
   Ppoint_list3 point_list;
   Pint *data = (Pint *) pdata;

   point_list.num_points = *data;
   point_list.points = (Ppoint3 *) &data[1];

   if (1/*PHG_SCRATCH_SPACE(&ws->scratch,
                         point_list.num_points * sizeof(Ppoint))*/) {
      plist.num_points = point_list.num_points;
      plist.points = (Ppoint *) malloc(plist.num_points * sizeof(Ppoint));

      for (i = 0; i < point_list.num_points; i++) {
         plist.points[i].x = point_list.points[i].x;
         plist.points[i].y = point_list.points[i].y;
      }

      type = wsgl_get_marker_type(ast);
      size = wsgl_get_marker_size(ast);

      switch (type) {
         case PMARKER_DOT:
            wsgl_marker_dot(&plist, size);
         break;

         case PMARKER_PLUS:
            wsgl_marker_plus(&plist, size);
         break;

         case PMARKER_ASTERISK:
            wsgl_marker_asterisk(&plist, size);
         break;

         case PMARKER_CROSS:
            wsgl_marker_cross(&plist, size);
         break;
      }
      // TODO: Remove
      free(plist.points);
   }
   else {
      //ERR_REPORT(ws->erh, ERR900);
   }
}

/*******************************************************************************
 * wsgl_render_marker
 *
 * DESCR:	Render marker element to current workstation rendering window
 * RETURNS:	N/A
 */

void wsgl_render_marker(
   Ws_attr_st *ast,
   El_handle el
   )
{
   switch (el->eltype) {
      case PELEM_INDIV_ASF:
         wsgl_setup_line_attr(ast);
         break;

      case PELEM_MARKER_IND:
         wsgl_setup_marker_attr(ast);
         break;

      case PELEM_MARKER_COLR_IND:
         if (phg_nset_name_is_set(&ast->asf_nameset,
                                  (Pint) PASPECT_MARKER_COLR_IND)) {
             wsgl_set_gcolr(&ast->indiv_group.marker_bundle.colr);
         }
         break;

      case PELEM_MARKER_COLR:
         if (phg_nset_name_is_set(&ast->asf_nameset,
                                  (Pint) PASPECT_MARKER_COLR_IND)) {
             wsgl_set_gcolr(&ast->indiv_group.marker_bundle.colr);
         }
         break;

      case PELEM_MARKER_SIZE:
         if (phg_nset_name_is_set(&ast->asf_nameset,
                                  (Pint) PASPECT_MARKER_SIZE)) {
            glLineWidth(ast->indiv_group.marker_bundle.size);
         }
         break;

      case PELEM_MARKER_TYPE:
         if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_MARKER_TYPE)) {
            /* TODO: In this case we really done need to setup all attributes */
            wsgl_setup_marker_attr(ast);
         }
         break;

      case PELEM_POLYMARKER:
         wsgl_polymarker(ELMT_CONTENT(el), ast);
         break;

      case PELEM_POLYMARKER3: 
         wsgl_polymarker3(ELMT_CONTENT(el), ast);
         break;

      default:
         break;
   }
}

