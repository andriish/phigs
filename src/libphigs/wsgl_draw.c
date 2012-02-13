/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2011 - 2012 Surplus Users Ham Society
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
 * phg_draw_marker_dot
 *
 * DESCR:	Draw marker dots helper function
 * RETURNS:	N/A
 */

static void phg_draw_marker_dot(
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
 * phg_draw_marker_plus
 *
 * DESCR:	Draw marker pluses helper function
 * RETURNS:	N/A
 */

static void phg_draw_marker_plus(
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
 * DESCR:	Draw marker asterisks helper function
 * RETURNS:	N/A
 */

static void phg_draw_marker_asterisk(
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
 * phg_draw_marker_cross
 *
 * DESCR:	Draw marker crosses helper function
 * RETURNS:	N/A
 */

static void phg_draw_marker_cross(
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
 * phg_draw_polymarker
 *
 * DESCR:	Draw markers
 * RETURNS:	N/A
 */

void phg_draw_polymarker(
   Ws *ws,
   Ppoint_list *point_list,
   Ws_attr_st *ast
   )
{
   Pint type;
   Pfloat size;

   phg_setup_marker_attr(ast, &type, &size);
   switch (type) {
      case PMARKER_DOT:
         phg_draw_marker_dot(point_list, size);
      break;

      case PMARKER_PLUS:
         phg_draw_marker_plus(point_list, size);
      break;

      case PMARKER_ASTERISK:
         phg_draw_marker_asterisk(point_list, size);
      break;

      case PMARKER_CROSS:
         phg_draw_marker_cross(point_list, size);
      break;
   }
}

/*******************************************************************************
 * phg_draw_polymarker3
 *
 * DESCR:	Draw markers 3D
 * RETURNS:	N/A
 */

void phg_draw_polymarker3(
   Ws *ws,
   Ppoint_list3 *point_list,
   Ws_attr_st *ast
   )
{
   Pint type;
   Pfloat size;
   int i;
   Ppoint_list plist;

   if (PHG_SCRATCH_SPACE(&ws->scratch,
                         point_list->num_points * sizeof(Pposted_struct))) {
      plist.num_points = point_list->num_points;
      plist.points = (Ppoint *) ws->scratch.buf;

      for (i = 0; i < point_list->num_points; i++) {
         plist.points[i].x = point_list->points[i].x;
         plist.points[i].y = point_list->points[i].y;
      }

      phg_setup_marker_attr(ast, &type, &size);
      switch (type) {
         case PMARKER_DOT:
            phg_draw_marker_dot(&plist, size);
         break;

         case PMARKER_PLUS:
            phg_draw_marker_plus(&plist, size);
         break;

         case PMARKER_ASTERISK:
            phg_draw_marker_asterisk(&plist, size);
         break;

         case PMARKER_CROSS:
            phg_draw_marker_cross(&plist, size);
         break;
      }
   }
   else {
      ERR_REPORT(ws->erh, ERR900);
   }
}

/*******************************************************************************
 * phg_draw_polyline
 *
 * DESCR:	Draw lines
 * RETURNS:	N/A
 */

void phg_draw_polyline(
   Ws *ws,
   Ppoint_list *point_list,
   Ws_attr_st *ast
   )
{
   int i;

   phg_setup_line_attr(ast);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++) {
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y);
   }
   glEnd();
}

/*******************************************************************************
 * phg_draw_polyline3
 *
 * DESCR:	Draw lines 3D
 * RETURNS:	N/A
 */

void phg_draw_polyline3(
   Ws *ws,
   Ppoint_list3 *point_list,
   Ws_attr_st *ast
   )
{
   int i;

   phg_setup_line_attr(ast);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++) {
      glVertex3f(point_list->points[i].x,
                 point_list->points[i].y,
                 point_list->points[i].z);
   }
   glEnd();
}

/*******************************************************************************
 * phg_set_polygon_offset
 *
 * DESCR:	Setup polygon offset helper function
 * RETURNS:	N/A
 */

static void phg_set_polygon_offset(
   float w
   )
{
   glPolygonOffset(w, w);
}

/*******************************************************************************
 * phg_draw_fill_area
 *
 * DESCR:	Draw fill area
 * RETURNS:	N/A
 */

void phg_draw_fill_area(
   Ws *ws,
   Ppoint_list *point_list,
   Ws_attr_st *ast
   )
{
   int i;
   Pint_style style;

   style = phg_get_int_style(ast);
   if (style != PSTYLE_EMPTY) {
      phg_setup_int_attr(ast);
      glBegin(GL_POLYGON);
      for (i = 0; i < point_list->num_points; i++) {
         glVertex2f(point_list->points[i].x,
                    point_list->points[i].y);
      }
      glEnd();
   }

   if (phg_get_edge_flag(ast) == PEDGE_ON) {
      phg_setup_edge_attr(ast);
      glBegin(GL_LINE_LOOP);
      for (i = 0; i < point_list->num_points; i++) {
         glVertex2f(point_list->points[i].x,
                    point_list->points[i].y);
      }
      glEnd();
   }
}

/*******************************************************************************
 * phg_draw_fill_area3
 *
 * DESCR:	Draw fill area 3D
 * RETURNS:	N/A
 */

void phg_draw_fill_area3(
   Ws *ws,
   Ppoint_list3 *point_list,
   Ws_attr_st *ast
   )
{
   int i;
   Pint_style style;
   Pedge_flag flag;
   Wsgl_handle wsgl = ws->render_context;

   style = phg_get_int_style(ast);
   flag = phg_get_edge_flag(ast);
   if (wsgl->cur_struct.hlhsr_id == PHIGS_HLHSR_ID_OFF) {
      if (style != PSTYLE_EMPTY) {
         phg_setup_int_attr(ast);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++) {
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         }
         glEnd();
      }

      if (flag == PEDGE_ON) {
         phg_setup_edge_attr(ast);
         glBegin(GL_LINE_LOOP);
         for (i = 0; i < point_list->num_points; i++) {
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         }
         glEnd();
      }
   }
   else {
      if ((flag == PEDGE_ON) &&
          ((style == PSTYLE_EMPTY) || (style == PSTYLE_HOLLOW))) {
         phg_set_polygon_offset(phg_get_edge_width(ast));
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         glEnable(GL_POLYGON_OFFSET_FILL);
         phg_setup_background(ws);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++) {
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         }
         glEnd();
         glDisable(GL_POLYGON_OFFSET_FILL);

         if (flag == PEDGE_ON) {
            phg_setup_edge_attr(ast);
         }
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++) {
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         }
         glEnd();
      }
      else if ((flag == PEDGE_OFF) &&
               (style == PSTYLE_HOLLOW)) {
         phg_set_polygon_offset(phg_get_edge_width(ast));
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         glEnable(GL_POLYGON_OFFSET_FILL);
         phg_setup_background(ws);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++) {
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         }
         glEnd();
         glDisable(GL_POLYGON_OFFSET_FILL);

         phg_setup_int_attr(ast);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++) {
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         }
         glEnd();
      }
      else {
         if (style != PSTYLE_EMPTY) {
            phg_setup_int_attr(ast);
            glEnable(GL_POLYGON_OFFSET_FILL);
            phg_set_polygon_offset(phg_get_edge_width(ast));
            glBegin(GL_POLYGON);
            for (i = 0; i < point_list->num_points; i++) {
               glVertex3f(point_list->points[i].x,
                          point_list->points[i].y,
                          point_list->points[i].z);
            }
            glEnd();
            glDisable(GL_POLYGON_OFFSET_FILL);
         }

         if (flag == PEDGE_ON) {
            phg_setup_edge_attr(ast);
            glDisable(GL_POLYGON_STIPPLE);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBegin(GL_POLYGON);
            for (i = 0; i < point_list->num_points; i++) {
               glVertex3f(point_list->points[i].x,
                          point_list->points[i].y,
                          point_list->points[i].z);
            }
            glEnd();
         }
      }
   }
}

/*******************************************************************************
 * phg_draw_fill_area_set
 *
 * DESCR:	Draw fill area set
 * RETURNS:	N/A
 */

void phg_draw_fill_area_set(
   Ws *ws,
   Ppoint_list_list *point_list_list,
   Ws_attr_st *ast
   )
{
   Pint i, num_lists;

   num_lists = point_list_list->num_point_lists;
   for (i = 0; i < num_lists; i++) {
      phg_draw_fill_area(ws, &point_list_list->point_lists[i], ast);
   }
}

/*******************************************************************************
 * phg_draw_fill_area_set3
 *
 * DESCR:	Draw fill area set 3D
 * RETURNS:	N/A
 */

void phg_draw_fill_area_set3(
   Ws *ws,
   Ppoint_list_list3 *point_list_list,
   Ws_attr_st *ast
   )
{
   Pint i, num_lists;

   num_lists = point_list_list->num_point_lists;
   for (i = 0; i < num_lists; i++) {
      phg_draw_fill_area3(ws, &point_list_list->point_lists[i], ast);
   }
}

/*******************************************************************************
 * phg_draw_fill_area3_data
 *
 * DESCR:	Draw fill area width data 3D
 * RETURNS:	N/A
 */

void phg_draw_fill_area3_data(
   Ws *ws,
   Pfasd3 *fasd3,
   Ws_attr_st *ast
   )
{
   Pint i;
   Pint_style style;
   Pedge_flag flag;
   Wsgl_handle wsgl = ws->render_context;

   style = phg_get_int_style(ast);
   flag = phg_get_edge_flag(ast);
   switch (fasd3->vflag) {
      case PVERT_POINT:
         if (wsgl->cur_struct.hlhsr_id == PHIGS_HLHSR_ID_OFF) {
            if (style != PSTYLE_EMPTY) {
               phg_setup_int_attr_nocol(ast, wsgl->cur_struct.lighting);
               phg_setup_facet_data3(fasd3, ast);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->point[i].x,
                             fasd3->vdata->point[i].y,
                             fasd3->vdata->point[i].z);
               }
               glEnd();
            }

            if (flag == PEDGE_ON) {
               phg_setup_edge_attr(ast);
               glBegin(GL_LINE_LOOP);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->point[i].x,
                             fasd3->vdata->point[i].y,
                             fasd3->vdata->point[i].z);
               }
               glEnd();
            }
         }
         else {
            if ((flag == PEDGE_ON) &&
                ((style == PSTYLE_EMPTY) || (style == PSTYLE_HOLLOW))) {
               phg_set_polygon_offset(phg_get_edge_width(ast));
               glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
               glEnable(GL_POLYGON_OFFSET_FILL);
               phg_setup_background(ws);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->point[i].x,
                             fasd3->vdata->point[i].y,
                             fasd3->vdata->point[i].z);
               }
               glEnd();
               glDisable(GL_POLYGON_OFFSET_FILL);

               if (flag == PEDGE_ON) {
                  phg_setup_edge_attr(ast);
               }
               glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->point[i].x,
                             fasd3->vdata->point[i].y,
                             fasd3->vdata->point[i].z);
               }
               glEnd();
            }
            else if ((flag == PEDGE_OFF) &&
                     (style == PSTYLE_HOLLOW)) {
               phg_set_polygon_offset(phg_get_edge_width(ast));
               glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
               glEnable(GL_POLYGON_OFFSET_FILL);
               phg_setup_background(ws);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->point[i].x,
                             fasd3->vdata->point[i].y,
                             fasd3->vdata->point[i].z);
               }
               glEnd();
               glDisable(GL_POLYGON_OFFSET_FILL);

               phg_setup_int_attr_nocol(ast, wsgl->cur_struct.lighting);
               glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
               phg_setup_facet_data3(fasd3, ast);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->point[i].x,
                             fasd3->vdata->point[i].y,
                             fasd3->vdata->point[i].z);
               }
               glEnd();
            }
            else {
               if (style != PSTYLE_EMPTY) {
                  phg_setup_int_attr_nocol(ast, wsgl->cur_struct.lighting);
                  glEnable(GL_POLYGON_OFFSET_FILL);
                  phg_set_polygon_offset(phg_get_edge_width(ast));
                  phg_setup_facet_data3(fasd3, ast);
                  glBegin(GL_POLYGON);
                  for (i = 0; i < fasd3->num_vertices; i++) {
                     glVertex3f(fasd3->vdata->point[i].x,
                                fasd3->vdata->point[i].y,
                                fasd3->vdata->point[i].z);
                  }
                  glEnd();
                  glDisable(GL_POLYGON_OFFSET_FILL);
               }

               if (flag == PEDGE_ON) {
                  phg_setup_edge_attr(ast);
                  glDisable(GL_POLYGON_STIPPLE);
                  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                  glBegin(GL_POLYGON);
                  for (i = 0; i < fasd3->num_vertices; i++) {
                     glVertex3f(fasd3->vdata->point[i].x,
                                fasd3->vdata->point[i].y,
                                fasd3->vdata->point[i].z);
                  }
                  glEnd();
               }
            }
         }
         break;

      case PVERT_COLOUR:
         if (wsgl->cur_struct.hlhsr_id == PHIGS_HLHSR_ID_OFF) {
            if (style != PSTYLE_EMPTY) {
               phg_setup_int_attr_nocol(ast, wsgl->cur_struct.lighting);
               phg_setup_facet_data3(fasd3, ast);
               if (fasd3->colr_model == PMODEL_RGB) {
                  glBegin(GL_POLYGON);
                  for (i = 0; i < fasd3->num_vertices; i++) {
                     glColor3f(fasd3->vdata->ptco[i].colour.direct.x,
                               fasd3->vdata->ptco[i].colour.direct.y,
                               fasd3->vdata->ptco[i].colour.direct.z);
                     glVertex3f(fasd3->vdata->ptco[i].point.x,
                                fasd3->vdata->ptco[i].point.y,
                                fasd3->vdata->ptco[i].point.z);
                  }
                  glEnd();
               }
               else if (fasd3->colr_model == PINDIRECT) {
                  glBegin(GL_POLYGON);
                  for (i = 0; i < fasd3->num_vertices; i++) {
                     glIndexi(fasd3->vdata->ptco[i].colour.ind);
                     glVertex3f(fasd3->vdata->ptco[i].point.x,
                                fasd3->vdata->ptco[i].point.y,
                                fasd3->vdata->ptco[i].point.z);
                  }
                  glEnd();
               }
            }

            if (flag == PEDGE_ON) {
               phg_setup_edge_attr(ast);
               glBegin(GL_LINE_LOOP);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->ptco[i].point.x,
                             fasd3->vdata->ptco[i].point.y,
                             fasd3->vdata->ptco[i].point.z);
               }
               glEnd();
            }
         }
         else {
            if ((flag == PEDGE_ON) &&
                ((style == PSTYLE_EMPTY) || (style == PSTYLE_HOLLOW))) {
               phg_set_polygon_offset(phg_get_edge_width(ast));
               glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
               glEnable(GL_POLYGON_OFFSET_FILL);
               phg_setup_background(ws);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->ptco[i].point.x,
                             fasd3->vdata->ptco[i].point.y,
                             fasd3->vdata->ptco[i].point.z);
               }
               glEnd();
               glDisable(GL_POLYGON_OFFSET_FILL);

               if (flag == PEDGE_ON) {
                  phg_setup_edge_attr(ast);
               }
               glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->ptco[i].point.x,
                             fasd3->vdata->ptco[i].point.y,
                             fasd3->vdata->ptco[i].point.z);
               }
               glEnd();
            }
            else if ((flag == PEDGE_OFF) &&
                     (style == PSTYLE_HOLLOW)) {
               phg_set_polygon_offset(phg_get_edge_width(ast));
               glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
               glEnable(GL_POLYGON_OFFSET_FILL);
               phg_setup_background(ws);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->ptco[i].point.x,
                             fasd3->vdata->ptco[i].point.y,
                             fasd3->vdata->ptco[i].point.z);
               }
               glEnd();
               glDisable(GL_POLYGON_OFFSET_FILL);

               phg_setup_int_attr_nocol(ast, wsgl->cur_struct.lighting);
               glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
               phg_setup_facet_data3(fasd3, ast);
               if (fasd3->colr_model == PMODEL_RGB) {
                  glBegin(GL_POLYGON);
                  for (i = 0; i < fasd3->num_vertices; i++) {
                     glColor3f(fasd3->vdata->ptco[i].colour.direct.x,
                               fasd3->vdata->ptco[i].colour.direct.y,
                               fasd3->vdata->ptco[i].colour.direct.z);
                     glVertex3f(fasd3->vdata->ptco[i].point.x,
                                fasd3->vdata->ptco[i].point.y,
                                fasd3->vdata->ptco[i].point.z);
                  }
                  glEnd();
               }
               else if (fasd3->colr_model == PINDIRECT) {
                  glBegin(GL_POLYGON);
                  for (i = 0; i < fasd3->num_vertices; i++) {
                     glIndexi(fasd3->vdata->ptco[i].colour.ind);
                     glVertex3f(fasd3->vdata->ptco[i].point.x,
                                fasd3->vdata->ptco[i].point.y,
                                fasd3->vdata->ptco[i].point.z);
                  }
                  glEnd();
               }
            }
            else {
               if (style != PSTYLE_EMPTY) {
                  phg_setup_int_attr_nocol(ast, wsgl->cur_struct.lighting);
                  glEnable(GL_POLYGON_OFFSET_FILL);
                  phg_set_polygon_offset(phg_get_edge_width(ast));
                  phg_setup_facet_data3(fasd3, ast);
                  if (fasd3->colr_model == PMODEL_RGB) {
                     glBegin(GL_POLYGON);
                     for (i = 0; i < fasd3->num_vertices; i++) {
                        glColor3f(fasd3->vdata->ptco[i].colour.direct.x,
                                  fasd3->vdata->ptco[i].colour.direct.y,
                                  fasd3->vdata->ptco[i].colour.direct.z);
                        glVertex3f(fasd3->vdata->ptco[i].point.x,
                                   fasd3->vdata->ptco[i].point.y,
                                   fasd3->vdata->ptco[i].point.z);
                     }
                     glEnd();
                  }
                  else if (fasd3->colr_model == PINDIRECT) {
                     glBegin(GL_POLYGON);
                     for (i = 0; i < fasd3->num_vertices; i++) {
                        glIndexi(fasd3->vdata->ptco[i].colour.ind);
                        glVertex3f(fasd3->vdata->ptco[i].point.x,
                                   fasd3->vdata->ptco[i].point.y,
                                   fasd3->vdata->ptco[i].point.z);
                     }
                     glEnd();
                  }
                  glDisable(GL_POLYGON_OFFSET_FILL);
               }

               if (flag == PEDGE_ON) {
                  phg_setup_edge_attr(ast);
                  glDisable(GL_POLYGON_STIPPLE);
                  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                  glBegin(GL_POLYGON);
                  for (i = 0; i < fasd3->num_vertices; i++) {
                     glVertex3f(fasd3->vdata->ptco[i].point.x,
                                fasd3->vdata->ptco[i].point.y,
                                fasd3->vdata->ptco[i].point.z);
                  }
                  glEnd();
               }
            }
         }
         break;

      case PVERT_NORMAL:
         if (wsgl->cur_struct.hlhsr_id == PHIGS_HLHSR_ID_OFF) {
            if (style != PSTYLE_EMPTY) {
               phg_setup_int_attr_nocol(ast, wsgl->cur_struct.lighting);
               phg_setup_facet_data3(fasd3, ast);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glNormal3f(fasd3->vdata->ptnorm[i].normal.delta_x,
                             fasd3->vdata->ptnorm[i].normal.delta_y,
                             fasd3->vdata->ptnorm[i].normal.delta_z);
                  glVertex3f(fasd3->vdata->ptnorm[i].point.x,
                             fasd3->vdata->ptnorm[i].point.y,
                             fasd3->vdata->ptnorm[i].point.z);
               }
               glEnd();
            }

            if (flag == PEDGE_ON) {
               phg_setup_edge_attr(ast);
               glBegin(GL_LINE_LOOP);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->ptnorm[i].point.x,
                             fasd3->vdata->ptnorm[i].point.y,
                             fasd3->vdata->ptnorm[i].point.z);
               }
               glEnd();
            }
         }
         else {
            if ((flag == PEDGE_ON) &&
                ((style == PSTYLE_EMPTY) || (style == PSTYLE_HOLLOW))) {
               phg_set_polygon_offset(phg_get_edge_width(ast));
               glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
               glEnable(GL_POLYGON_OFFSET_FILL);
               phg_setup_background(ws);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->ptnorm[i].point.x,
                             fasd3->vdata->ptnorm[i].point.y,
                             fasd3->vdata->ptnorm[i].point.z);
               }
               glEnd();
               glDisable(GL_POLYGON_OFFSET_FILL);

               if (flag == PEDGE_ON) {
                  phg_setup_edge_attr(ast);
               }
               glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->ptnorm[i].point.x,
                             fasd3->vdata->ptnorm[i].point.y,
                             fasd3->vdata->ptnorm[i].point.z);
               }
               glEnd();
            }
            else if ((flag == PEDGE_OFF) &&
                     (style == PSTYLE_HOLLOW)) {
               phg_set_polygon_offset(phg_get_edge_width(ast));
               glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
               glEnable(GL_POLYGON_OFFSET_FILL);
               phg_setup_background(ws);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->ptnorm[i].point.x,
                             fasd3->vdata->ptnorm[i].point.y,
                             fasd3->vdata->ptnorm[i].point.z);
               }
               glEnd();
               glDisable(GL_POLYGON_OFFSET_FILL);

               phg_setup_int_attr_nocol(ast, wsgl->cur_struct.lighting);
               glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
               phg_setup_facet_data3(fasd3, ast);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glNormal3f(fasd3->vdata->ptnorm[i].normal.delta_x,
                             fasd3->vdata->ptnorm[i].normal.delta_y,
                             fasd3->vdata->ptnorm[i].normal.delta_z);
                  glVertex3f(fasd3->vdata->ptnorm[i].point.x,
                             fasd3->vdata->ptnorm[i].point.y,
                             fasd3->vdata->ptnorm[i].point.z);
               }
               glEnd();
            }
            else {
               if (style != PSTYLE_EMPTY) {
                  phg_setup_int_attr_nocol(ast, wsgl->cur_struct.lighting);
                  glEnable(GL_POLYGON_OFFSET_FILL);
                  phg_set_polygon_offset(phg_get_edge_width(ast));
                  phg_setup_facet_data3(fasd3, ast);
                  glBegin(GL_POLYGON);
                  for (i = 0; i < fasd3->num_vertices; i++) {
                     glNormal3f(fasd3->vdata->ptnorm[i].normal.delta_x,
                                fasd3->vdata->ptnorm[i].normal.delta_y,
                                fasd3->vdata->ptnorm[i].normal.delta_z);
                     glVertex3f(fasd3->vdata->ptnorm[i].point.x,
                                fasd3->vdata->ptnorm[i].point.y,
                                fasd3->vdata->ptnorm[i].point.z);
                  }
                  glEnd();
                  glDisable(GL_POLYGON_OFFSET_FILL);
               }

               if (flag == PEDGE_ON) {
                  phg_setup_edge_attr(ast);
                  glDisable(GL_POLYGON_STIPPLE);
                  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                  glBegin(GL_POLYGON);
                  for (i = 0; i < fasd3->num_vertices; i++) {
                     glVertex3f(fasd3->vdata->ptnorm[i].point.x,
                                fasd3->vdata->ptnorm[i].point.y,
                                fasd3->vdata->ptnorm[i].point.z);
                  }
                  glEnd();
               }
            }
         }
         break;

      case PVERT_COLOUR_NORMAL:
         if (wsgl->cur_struct.hlhsr_id == PHIGS_HLHSR_ID_OFF) {
            if (style != PSTYLE_EMPTY) {
               phg_setup_int_attr_nocol(ast, wsgl->cur_struct.lighting);
               if (fasd3->colr_model == PMODEL_RGB) {
                  glBegin(GL_POLYGON);
                  for (i = 0; i < fasd3->num_vertices; i++) {
                     glColor3f(fasd3->vdata->ptconorm[i].colour.direct.x,
                               fasd3->vdata->ptconorm[i].colour.direct.y,
                               fasd3->vdata->ptconorm[i].colour.direct.z);
                     glNormal3f(fasd3->vdata->ptconorm[i].normal.delta_x,
                                fasd3->vdata->ptconorm[i].normal.delta_y,
                                fasd3->vdata->ptconorm[i].normal.delta_z);
                     glVertex3f(fasd3->vdata->ptconorm[i].point.x,
                                fasd3->vdata->ptconorm[i].point.y,
                                fasd3->vdata->ptconorm[i].point.z);
                  }
                  glEnd();
               }
               else if (fasd3->colr_model == PINDIRECT) {
                  glBegin(GL_POLYGON);
                  for (i = 0; i < fasd3->num_vertices; i++) {
                     glIndexi(fasd3->vdata->ptconorm[i].colour.ind);
                     glNormal3f(fasd3->vdata->ptconorm[i].normal.delta_x,
                                fasd3->vdata->ptconorm[i].normal.delta_y,
                                fasd3->vdata->ptconorm[i].normal.delta_z);
                     glVertex3f(fasd3->vdata->ptconorm[i].point.x,
                                fasd3->vdata->ptconorm[i].point.y,
                                fasd3->vdata->ptconorm[i].point.z);
                  }
                  glEnd();
               }
            }

            if (flag == PEDGE_ON) {
               phg_setup_edge_attr(ast);
               glBegin(GL_LINE_LOOP);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->ptconorm[i].point.x,
                             fasd3->vdata->ptconorm[i].point.y,
                             fasd3->vdata->ptconorm[i].point.z);
               }
               glEnd();
            }
         }
         else {
            if ((flag == PEDGE_ON) &&
                ((style == PSTYLE_EMPTY) || (style == PSTYLE_HOLLOW))) {
               phg_set_polygon_offset(phg_get_edge_width(ast));
               glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
               glEnable(GL_POLYGON_OFFSET_FILL);
               phg_setup_background(ws);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->ptconorm[i].point.x,
                             fasd3->vdata->ptconorm[i].point.y,
                             fasd3->vdata->ptconorm[i].point.z);
               }
               glEnd();
               glDisable(GL_POLYGON_OFFSET_FILL);

               if (flag == PEDGE_ON) {
                  phg_setup_edge_attr(ast);
               }
               glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->ptconorm[i].point.x,
                             fasd3->vdata->ptconorm[i].point.y,
                             fasd3->vdata->ptconorm[i].point.z);
               }
               glEnd();
            }
            else if ((flag == PEDGE_OFF) &&
                     (style == PSTYLE_HOLLOW)) {
               phg_set_polygon_offset(phg_get_edge_width(ast));
               glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
               glEnable(GL_POLYGON_OFFSET_FILL);
               phg_setup_background(ws);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->ptconorm[i].point.x,
                             fasd3->vdata->ptconorm[i].point.y,
                             fasd3->vdata->ptconorm[i].point.z);
               }
               glEnd();
               glDisable(GL_POLYGON_OFFSET_FILL);

               phg_setup_int_attr_nocol(ast, wsgl->cur_struct.lighting);
               glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
               if (fasd3->colr_model == PMODEL_RGB) {
                  glBegin(GL_POLYGON);
                  for (i = 0; i < fasd3->num_vertices; i++) {
                     glColor3f(fasd3->vdata->ptconorm[i].colour.direct.x,
                               fasd3->vdata->ptconorm[i].colour.direct.y,
                               fasd3->vdata->ptconorm[i].colour.direct.z);
                     glNormal3f(fasd3->vdata->ptconorm[i].normal.delta_x,
                                fasd3->vdata->ptconorm[i].normal.delta_y,
                                fasd3->vdata->ptconorm[i].normal.delta_z);
                     glVertex3f(fasd3->vdata->ptconorm[i].point.x,
                                fasd3->vdata->ptconorm[i].point.y,
                                fasd3->vdata->ptconorm[i].point.z);
                  }
                  glEnd();
               }
               else if (fasd3->colr_model == PINDIRECT) {
                  glBegin(GL_POLYGON);
                  for (i = 0; i < fasd3->num_vertices; i++) {
                     glIndexi(fasd3->vdata->ptconorm[i].colour.ind);
                     glNormal3f(fasd3->vdata->ptconorm[i].normal.delta_x,
                                fasd3->vdata->ptconorm[i].normal.delta_y,
                                fasd3->vdata->ptconorm[i].normal.delta_z);
                     glVertex3f(fasd3->vdata->ptconorm[i].point.x,
                                fasd3->vdata->ptconorm[i].point.y,
                                fasd3->vdata->ptconorm[i].point.z);
                  }
                  glEnd();
               }
            }
            else {
               if (style != PSTYLE_EMPTY) {
                  phg_setup_int_attr_nocol(ast, wsgl->cur_struct.lighting);
                  glEnable(GL_POLYGON_OFFSET_FILL);
                  phg_set_polygon_offset(phg_get_edge_width(ast));
                  if (fasd3->colr_model == PMODEL_RGB) {
                     glBegin(GL_POLYGON);
                     for (i = 0; i < fasd3->num_vertices; i++) {
                        glColor3f(fasd3->vdata->ptconorm[i].colour.direct.x,
                                  fasd3->vdata->ptconorm[i].colour.direct.y,
                                  fasd3->vdata->ptconorm[i].colour.direct.z);
                        glNormal3f(fasd3->vdata->ptconorm[i].normal.delta_x,
                                   fasd3->vdata->ptconorm[i].normal.delta_y,
                                   fasd3->vdata->ptconorm[i].normal.delta_z);
                        glVertex3f(fasd3->vdata->ptconorm[i].point.x,
                                   fasd3->vdata->ptconorm[i].point.y,
                                   fasd3->vdata->ptconorm[i].point.z);
                     }
                     glEnd();
                  }
                  else if (fasd3->colr_model == PINDIRECT) {
                     glBegin(GL_POLYGON);
                     for (i = 0; i < fasd3->num_vertices; i++) {
                        glIndexi(fasd3->vdata->ptconorm[i].colour.ind);
                        glNormal3f(fasd3->vdata->ptconorm[i].normal.delta_x,
                                   fasd3->vdata->ptconorm[i].normal.delta_y,
                                   fasd3->vdata->ptconorm[i].normal.delta_z);
                        glVertex3f(fasd3->vdata->ptconorm[i].point.x,
                                   fasd3->vdata->ptconorm[i].point.y,
                                   fasd3->vdata->ptconorm[i].point.z);
                     }
                     glEnd();
                  }
                  glDisable(GL_POLYGON_OFFSET_FILL);
               }

               if (flag == PEDGE_ON) {
                  phg_setup_edge_attr(ast);
                  glDisable(GL_POLYGON_STIPPLE);
                  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                  glBegin(GL_POLYGON);
                  for (i = 0; i < fasd3->num_vertices; i++) {
                     glVertex3f(fasd3->vdata->ptconorm[i].point.x,
                                fasd3->vdata->ptconorm[i].point.y,
                                fasd3->vdata->ptconorm[i].point.z);
                  }
                  glEnd();
               }
            }
         }
         break;

      default:
         break;
   }
}

/*******************************************************************************
 * phg_draw_text
 *
 * DESCR:	Draw text
 * RETURNS:	N/A
 */

void phg_draw_text(
   Ppoint *pos,
   char *text
   )
{
   printf("Draw text: ");
   printf("%f %f: %s\n", pos->x, pos->y, text);
}

