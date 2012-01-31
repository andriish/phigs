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
 * phg_set_matrix
 *
 * DESCR:	Setup matrix
 * RETURNS:	N/A
 */

static void phg_set_matrix(
    Pmatrix3 mat,
    int mult
    )
{
   int i, j;
   GLfloat m[16];
   GLfloat *mp = &m[0];

   for (i = 0; i < 4; i++) {
      for (j = 0; j < 4; j++) {
         *mp = mat[j][i];
         mp++;
      }
   }

   if (mult) {
      glMultMatrixf(m);
   }
   else {
      glLoadMatrixf(m);
   }
}

/*******************************************************************************
 * phg_update_projection
 *
 * DESCR:	Update projection matrix
 * RETURNS:	N/A
 */

void phg_update_projection(
   Ws *ws
   )
{
   Pmatrix3 mat;
   Wsgl_handle wsgl = ws->render_context;

#ifdef DEBUG
   printf("Update projection\n");
#endif

   glMatrixMode(GL_PROJECTION);
   if (wsgl->render_mode == WS_RENDER_MODE_SELECT) {
      phg_mat_mul(mat, wsgl->pick_tran, wsgl->view_rep.map_matrix);
      phg_set_matrix(mat, FALSE);
   }
   else {
      phg_set_matrix(wsgl->view_rep.map_matrix, FALSE);
   }
}

/*******************************************************************************
 * phg_update_modelview
 *
 * DESCR:	Update modelview matrix
 * RETURNS:	N/A
 */

void phg_update_modelview(
   Ws *ws
   )
{
   Wsgl_handle wsgl = ws->render_context;

#ifdef DEBUG
   printf("Update modelview\n");
#endif

   glMatrixMode(GL_MODELVIEW);
   phg_mat_mul(wsgl->total_tran, wsgl->view_rep.ori_matrix, wsgl->local_tran);
   phg_set_matrix(wsgl->total_tran, FALSE);
}

/*******************************************************************************
 * phg_set_view_ind
 *
 * DESCR:	Setup view
 * RETURNS:	N/A
 */

void phg_set_view_ind(
   Ws *ws,
   Pint ind
   )
{
   Phg_ret ret;
   Wsgl_handle wsgl = ws->render_context;

   (*ws->inq_representation)(ws,
                             ind,
                             PINQ_REALIZED,
                             PHG_ARGS_VIEWREP,
                             &ret);
   if (ret.err == 0) {
      wsgl->curr_view_index = ind;
      memcpy(&wsgl->view_rep, &ret.data.rep.viewrep, sizeof(Pview_rep3));
      phg_update_projection(ws);
      phg_update_modelview(ws);
   }
}

/*******************************************************************************
 * phg_set_hlhsr_id
 *
 * DESCR:	Setup depth buffer checking
 * RETURNS:	N/A
 */

void phg_set_hlhsr_id(
   Pint hlhsr_id
   )
{
   switch(hlhsr_id) {
      case PHIGS_HLHSR_ID_OFF:
         glDepthFunc(GL_ALWAYS);
      break;

      case PHIGS_HLHSR_ID_ON:
         glDepthFunc(GL_LESS);
      break;

      default:
      break;
   }
}

/*******************************************************************************
 * phg_set_gcolr
 *
 * DESCR:	Setup colour
 * RETURNS:	N/A
 */

void phg_set_gcolr(
   Pgcolr *gcolr
   )
{
   switch(gcolr->type) {
      case PINDIRECT:
         glIndexi(gcolr->val.ind);
         break;

      case PMODEL_RGB:
         glColor3f(gcolr->val.general.x,
                   gcolr->val.general.y,
                   gcolr->val.general.z);
         break;

      default:
         break;
   }
}

/*******************************************************************************
 * phg_set_line_ind
 *
 * DESCR:	Setup line index
 * RETURNS:	N/A
 */

void phg_set_line_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   )
{
   Phg_ret ret;

   (*ws->inq_representation)(ws,
                             ind,
                             PINQ_REALIZED,
                             PHG_ARGS_EXTLNREP,
                             &ret);
   if (ret.err == 0) {
      phg_attr_group_set_line_bundle(ws,
                                     attr_group,
                                     &ret.data.rep.extlnrep);
   }
}

/*******************************************************************************
 * phg_set_line_attr
 *
 * DESCR:	Setup line attributes
 * RETURNS:	N/A
 */

void phg_set_line_attr(
   Ws *ws,
   Pline_bundle_plus *attr
   )
{
   /* Colour */
   phg_set_gcolr(&attr->colr);

   /* Line style */
   switch (attr->type) {
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

   /* Line width */
   glLineWidth(attr->width);
}

/*******************************************************************************
 * phg_set_int_ind
 *
 * DESCR:	Setup interior index
 * RETURNS:	N/A
 */

void phg_set_int_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   )
{
   Phg_ret ret;

   (*ws->inq_representation)(ws,
                             ind,
                             PINQ_REALIZED,
                             PHG_ARGS_EXTINTERREP,
                             &ret);
   if (ret.err == 0) {
      phg_attr_group_set_int_bundle(ws,
                                    attr_group,
                                    &ret.data.rep.extinterrep);
   }
}

/*******************************************************************************
 * phg_set_int_attr
 *
 * DESCR:	Setup interior attributes
 * RETURNS:	N/A
 */

void phg_set_int_attr(
   Ws *ws,
   Pint_bundle_plus *attr
   )
{
   /* Colour */
   phg_set_gcolr(&attr->colr);

   if (attr->style == PSTYLE_HATCH) {
      glEnable(GL_POLYGON_STIPPLE);
      glPolygonStipple(wsgl_hatch_tbl[attr->style_ind - 1]);
   }
   else {
      glDisable(GL_POLYGON_STIPPLE);
   }
}

/*******************************************************************************
 * phg_set_edge_ind
 *
 * DESCR:	Setup edge index
 * RETURNS:	N/A
 */

void phg_set_edge_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   )
{
   Phg_ret ret;

   (*ws->inq_representation)(ws,
                             ind,
                             PINQ_REALIZED,
                             PHG_ARGS_EXTEDGEREP,
                             &ret);
   if (ret.err == 0) {
      phg_attr_group_set_edge_bundle(ws,
                                     attr_group,
                                     &ret.data.rep.extedgerep);
   }
}

/*******************************************************************************
 * phg_set_edge_attr
 *
 * DESCR:	Setup interior attributes
 * RETURNS:	N/A
 */

void phg_set_edge_attr(
   Ws *ws,
   Pedge_bundle_plus *attr
   )
{
   /* Colour */
   phg_set_gcolr(&attr->colr);

   /* Edge width */
   glLineWidth(attr->width);

   /* Line style */
   switch (attr->type) {
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
 * phg_set_marker_ind
 *
 * DESCR:	Setup marker index
 * RETURNS:	N/A
 */

void phg_set_marker_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   )
{
   Phg_ret ret;

   (*ws->inq_representation)(ws,
                             ind,
                             PINQ_REALIZED,
                             PHG_ARGS_EXTMKREP,
                             &ret);
   if (ret.err == 0) {
      phg_attr_group_set_marker_bundle(ws,
                                       attr_group,
                                       &ret.data.rep.extmkrep);
   }
}

/*******************************************************************************
 * phg_set_marker_attr
 *
 * DESCR:	Setup marker attributes
 * RETURNS:	N/A
 */

void phg_set_marker_attr(
   Ws *ws,
   Pmarker_bundle_plus *attr
   )
{
   /* Colour */
   phg_set_gcolr(&attr->colr);
}

/*******************************************************************************
 * phg_set_text_ind
 *
 * DESCR:	Setup text index
 * RETURNS:	N/A
 */

void phg_set_text_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   )
{
   Phg_ret ret;

   (*ws->inq_representation)(ws,
                             ind,
                             PINQ_REALIZED,
                             PHG_ARGS_EXTTXREP,
                             &ret);
    if (ret.err == 0) {
      phg_attr_group_set_text_bundle(ws,
                                     attr_group,
                                     &ret.data.rep.exttxrep);
   }
}

/*******************************************************************************
 * phg_add_names_set
 *
 * DESCR:	Add names to nameset
 * RETURNS:	N/A
 */

void phg_add_names_set(
   Ws *ws,
   Pint_list *names
   )
{
   Wsgl_handle wsgl = ws->render_context;

   phg_nset_names_set(wsgl->cur_nameset,
                      names->num_ints,
                      names->ints);
}

/*******************************************************************************
 * phg_remove_names_set
 *
 * DESCR:	Remove names from nameset
 * RETURNS:	N/A
 */

void phg_remove_names_set(
   Ws *ws,
   Pint_list *names
   )
{
   Wsgl_handle wsgl = ws->render_context;

   phg_nset_names_clear(wsgl->cur_nameset,
                        names->num_ints,
                        names->ints);
}

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
   for (i = 0; i < point_list->num_points; i++)
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y);
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
   Pattr_group *attr
   )
{
   phg_set_marker_attr(ws, &attr->marker_bundle);
   switch (attr->marker_bundle.type) {
      case PMARKER_DOT:
         phg_draw_marker_dot(point_list, attr->marker_bundle.size);
      break;

      case PMARKER_PLUS:
         phg_draw_marker_plus(point_list, attr->marker_bundle.size);
      break;

      case PMARKER_ASTERISK:
         phg_draw_marker_asterisk(point_list, attr->marker_bundle.size);
      break;

      case PMARKER_CROSS:
         phg_draw_marker_cross(point_list, attr->marker_bundle.size);
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
   Pattr_group *attr
   )
{
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

      phg_set_marker_attr(ws, &attr->marker_bundle);
      switch (attr->marker_bundle.type) {
         case PMARKER_DOT:
            phg_draw_marker_dot(&plist, attr->marker_bundle.size);
         break;

         case PMARKER_PLUS:
            phg_draw_marker_plus(&plist, attr->marker_bundle.size);
         break;

         case PMARKER_ASTERISK:
            phg_draw_marker_asterisk(&plist, attr->marker_bundle.size);
         break;

         case PMARKER_CROSS:
            phg_draw_marker_cross(&plist, attr->marker_bundle.size);
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
   Pattr_group *attr
   )
{
   int i;

   phg_set_line_attr(ws, &attr->line_bundle);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++)
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y);
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
   Pattr_group *attr
   )
{
   int i;

   phg_set_line_attr(ws, &attr->line_bundle);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++)
      glVertex3f(point_list->points[i].x,
                 point_list->points[i].y,
                 point_list->points[i].z);
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
   Pattr_group *attr
   )
{
   int i;

   if ((attr->int_bundle.style == PSTYLE_SOLID) ||
       (attr->int_bundle.style == PSTYLE_HATCH)) {
      phg_set_int_attr(ws, &attr->int_bundle);
      glBegin(GL_POLYGON);
      for (i = 0; i < point_list->num_points; i++)
         glVertex2f(point_list->points[i].x,
                    point_list->points[i].y);
      glEnd();
   }

   if (attr->edge_bundle.flag == PEDGE_ON) {
      phg_set_edge_attr(ws, &attr->edge_bundle);
      glBegin(GL_LINE_LOOP);
      for (i = 0; i < point_list->num_points; i++)
         glVertex2f(point_list->points[i].x,
                    point_list->points[i].y);
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
   Pattr_group *attr
   )
{
   int i;

   Wsgl_handle wsgl = ws->render_context;

   if (wsgl->hlhsr_mode == PHIGS_HLHSR_MODE_NONE) {
      if ((attr->int_bundle.style == PSTYLE_SOLID) ||
          (attr->int_bundle.style == PSTYLE_HATCH)) {
         phg_set_int_attr(ws, &attr->int_bundle);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++)
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         glEnd();
      }

      if (attr->edge_bundle.flag == PEDGE_ON) {
         phg_set_edge_attr(ws, &attr->edge_bundle);
         glBegin(GL_LINE_LOOP);
         for (i = 0; i < point_list->num_points; i++)
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         glEnd();
      }
   }
   else {
      if ((attr->int_bundle.style == PSTYLE_HOLLOW) &&
          (attr->edge_bundle.flag == PEDGE_ON)) {
         phg_set_polygon_offset(attr->edge_bundle.width);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         glEnable(GL_POLYGON_OFFSET_FILL);
         glColor3f(wsgl->background.val.general.x,
                   wsgl->background.val.general.y,
                   wsgl->background.val.general.z);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++)
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         glEnd();
         glDisable(GL_POLYGON_OFFSET_FILL);

         phg_set_edge_attr(ws, &attr->edge_bundle);
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++)
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         glEnd();
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      }
      else if (((attr->int_bundle.style == PSTYLE_SOLID) ||
                (attr->int_bundle.style == PSTYLE_HATCH)) &&
               (attr->edge_bundle.flag == PEDGE_ON)) {
         phg_set_int_attr(ws, &attr->int_bundle);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         glEnable(GL_POLYGON_OFFSET_FILL);
         phg_set_polygon_offset(attr->edge_bundle.width);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++)
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         glEnd();
         glDisable(GL_POLYGON_OFFSET_FILL);

         phg_set_edge_attr(ws, &attr->edge_bundle);
         glDisable(GL_POLYGON_STIPPLE);
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++)
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         glEnd();
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
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

