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
   Wsgl_handle wsgl = ws->render_context;

#ifdef DEBUG
   printf("Update projection\n");
#endif

   glMatrixMode(GL_PROJECTION);
   if (wsgl->render_mode == WS_RENDER_MODE_SELECT) {
      phg_mat_mul(wsgl->model_tran,
                  wsgl->pick_tran,
                  wsgl->cur_struct.view_rep.map_matrix);
      phg_set_matrix(wsgl->model_tran, FALSE);
   }
   else {
      phg_set_matrix(wsgl->cur_struct.view_rep.map_matrix, FALSE);
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
   phg_mat_mul(wsgl->composite_tran,
               wsgl->cur_struct.global_tran,
               wsgl->cur_struct.local_tran);
   phg_mat_mul(wsgl->model_tran,
               wsgl->cur_struct.view_rep.ori_matrix,
               wsgl->composite_tran);
   phg_set_matrix(wsgl->model_tran, FALSE);
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
      memcpy(&wsgl->cur_struct.view_rep,
             &ret.data.rep.viewrep,
             sizeof(Pview_rep3));
      phg_update_projection(ws);
      phg_update_modelview(ws);
   }
}

/*******************************************************************************
 * phg_update_hlhsr_id
 *
 * DESCR:	Update depth buffer checking flag
 * RETURNS:	N/A
 */

void phg_update_hlhsr_id(
   Ws *ws
   )
{
   Wsgl_handle wsgl = ws->render_context;

   switch(wsgl->cur_struct.hlhsr_id) {
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
 * phg_set_asf
 *
 * DESCR:	Setup asf
 * RETURNS:	N/A
 */

void phg_set_asf(
   Ws_attr_st *ast,
   Pasf_info *asf_info
   )
{
   if (asf_info->source == PASF_INDIV) {
      phg_nset_name_set(&ast->asf_nameset, asf_info->id);
   }
   else {
      phg_nset_name_clear(&ast->asf_nameset, asf_info->id);
   }
}

/*******************************************************************************
 * phg_set_gcolr
 *
 * DESCR:	Setup colour helper function
 * RETURNS:	N/A
 */

static void phg_set_gcolr(
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
 * phg_setup_line_attr
 *
 * DESCR:	Setup line attributes helper function
 * RETURNS:	N/A
 */

static void phg_setup_line_attr(
   Ws_attr_st *ast
   )
{
   Pint type;

   if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_LINE_COLR_IND)) {
      phg_set_gcolr(&ast->indiv_group.line_bundle.colr);
   }
   else {
      phg_set_gcolr(&ast->bundl_group.line_bundle.colr);
   }

   if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_LINETYPE)) {
      type = ast->indiv_group.line_bundle.type;
   }
   else {
      type = ast->bundl_group.line_bundle.type;
   }

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

   if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_LINEWIDTH)) {
      glLineWidth(ast->indiv_group.line_bundle.width);
   }
   else {
      glLineWidth(ast->bundl_group.line_bundle.width);
   }
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
 * phg_get_int_style
 *
 * DESCR:	Get interior style helper function
 * RETURNS:	Interiour style
 */

static Pint_style phg_get_int_style(
   Ws_attr_st *ast
   )
{
   Pint_style style;

   if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_INT_STYLE)) {
      style = ast->indiv_group.int_bundle.style;
   }
   else {
      style = ast->bundl_group.int_bundle.style;
   }

   return style;
}

/*******************************************************************************
 * phg_setup_int_attr
 *
 * DESCR:	Setup interior attributes helper function
 * RETURNS:	N/A
 */

void phg_setup_int_attr(
   Ws_attr_st *ast
   )
{
   Pint_style style;
   Pint style_ind;

   if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_INT_COLR_IND)) {
      phg_set_gcolr(&ast->indiv_group.int_bundle.colr);
   }
   else {
      phg_set_gcolr(&ast->bundl_group.int_bundle.colr);
   }

   style = phg_get_int_style(ast);
   if (style == PSTYLE_HATCH) {
      if (phg_nset_name_is_set(&ast->asf_nameset,
          (Pint) PASPECT_INT_STYLE_IND)) {
         style_ind = ast->indiv_group.int_bundle.style_ind;
      }
      else {
         style_ind = ast->bundl_group.int_bundle.style_ind;
      }
      glEnable(GL_POLYGON_STIPPLE);
      glPolygonStipple(wsgl_hatch_tbl[style_ind - 1]);
   }
   else {
      glDisable(GL_POLYGON_STIPPLE);
   }
}

/*******************************************************************************
 * phg_setup_int_nocol
 *
 * DESCR:	Setup interior attributes without color helper function
 * RETURNS:	N/A
 */

void phg_setup_int_attr_nocol(
   Ws_attr_st *ast
   )
{
   Pint_style style;
   Pint style_ind;

   style = phg_get_int_style(ast);
   if (style == PSTYLE_HATCH) {
      if (phg_nset_name_is_set(&ast->asf_nameset,
          (Pint) PASPECT_INT_STYLE_IND)) {
         style_ind = ast->indiv_group.int_bundle.style_ind;
      }
      else {
         style_ind = ast->bundl_group.int_bundle.style_ind;
      }
      glEnable(GL_POLYGON_STIPPLE);
      glPolygonStipple(wsgl_hatch_tbl[style_ind - 1]);
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
 * phg_get_edge_flag
 *
 * DESCR:	Get edge flag helper function
 * RETURNS:	Edge flag
 */

static Pint_style phg_get_edge_flag(
   Ws_attr_st *ast
   )
{
   Pedge_flag flag;

   if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_EDGE_FLAG)) {
      flag = ast->indiv_group.edge_bundle.flag;
   }
   else {
      flag = ast->bundl_group.edge_bundle.flag;
   }

   return flag;
}

/*******************************************************************************
 * phg_get_edge_width
 *
 * DESCR:	Get edge width helper function
 * RETURNS:	Edge width
 */

static Pfloat phg_get_edge_width(
   Ws_attr_st *ast
   )
{
   Pfloat width;

   if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_EDGEWIDTH)) {
      width = ast->indiv_group.edge_bundle.width;
   }
   else {
      width = ast->bundl_group.edge_bundle.width;
   }

   return width;
}

/*******************************************************************************
 * phg_setup_edge_attr
 *
 * DESCR:	Setup edge attributes helper function
 * RETURNS:	N/A
 */

static void phg_setup_edge_attr(
   Ws_attr_st *ast
   )
{
   Pint type;

   if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_EDGE_COLR_IND)) {
      phg_set_gcolr(&ast->indiv_group.edge_bundle.colr);
   }
   else {
      phg_set_gcolr(&ast->bundl_group.edge_bundle.colr);
   }

   glLineWidth(phg_get_edge_width(ast));

   if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_EDGETYPE)) {
      type = ast->indiv_group.edge_bundle.type;
   }
   else {
      type = ast->bundl_group.edge_bundle.type;
   }

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
 * phg_setup_marker_attr
 *
 * DESCR:	Setup marker attributes helper function
 * RETURNS:	N/A
 */

static void phg_setup_marker_attr(
   Ws_attr_st *ast,
   Pint *type,
   Pfloat *size
   )
{
   if (phg_nset_name_is_set(&ast->asf_nameset,
                            (Pint) PASPECT_MARKER_COLR_IND)) {
      phg_set_gcolr(&ast->indiv_group.marker_bundle.colr);
   }
   else {
      phg_set_gcolr(&ast->bundl_group.marker_bundle.colr);
   }

   if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_MARKER_TYPE)) {
      *type = ast->indiv_group.marker_bundle.type;
   }
   else {
      *type = ast->bundl_group.marker_bundle.type;
   }

   if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_MARKER_SIZE)) {
      *size = ast->indiv_group.marker_bundle.size;
   }
   else {
      *size = ast->bundl_group.marker_bundle.size;
   }
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

   phg_nset_names_set(&wsgl->cur_struct.cur_nameset,
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

   phg_nset_names_clear(&wsgl->cur_struct.cur_nameset,
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
   Ws_attr_st *ast
   )
{
   int i;

   phg_setup_line_attr(ast);
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
   Ws_attr_st *ast
   )
{
   int i;
   Pint_style style;

   style = phg_get_int_style(ast);
   if ((style == PSTYLE_SOLID) || (style == PSTYLE_HATCH)) {
      phg_setup_int_attr(ast);
      glBegin(GL_POLYGON);
      for (i = 0; i < point_list->num_points; i++)
         glVertex2f(point_list->points[i].x,
                    point_list->points[i].y);
      glEnd();
   }

   if (phg_get_edge_flag(ast) == PEDGE_ON) {
      phg_setup_edge_attr(ast);
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
      if ((style == PSTYLE_SOLID) || (style == PSTYLE_HATCH)) {
         phg_setup_int_attr(ast);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++)
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         glEnd();
      }

      if (flag == PEDGE_ON) {
         phg_setup_edge_attr(ast);
         glBegin(GL_LINE_LOOP);
         for (i = 0; i < point_list->num_points; i++)
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         glEnd();
      }
   }
   else {
      if ((style == PSTYLE_EMPTY) &&
          (flag == PEDGE_ON)) {
         phg_set_polygon_offset(phg_get_edge_width(ast));
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

         phg_setup_edge_attr(ast);
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++)
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         glEnd();
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      }
      else if (((style == PSTYLE_SOLID) || (style == PSTYLE_HATCH)) &&
               (flag == PEDGE_ON)) {
         phg_setup_int_attr(ast);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         glEnable(GL_POLYGON_OFFSET_FILL);
         phg_set_polygon_offset(phg_get_edge_width(ast));
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++)
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         glEnd();
         glDisable(GL_POLYGON_OFFSET_FILL);

         phg_setup_edge_attr(ast);
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
      case PVERT_COLOUR:
         if (wsgl->cur_struct.hlhsr_id == PHIGS_HLHSR_ID_OFF) {
            if ((style == PSTYLE_SOLID) || (style == PSTYLE_HATCH)) {
               phg_setup_int_attr_nocol(ast);
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
            if ((style == PSTYLE_EMPTY) &&
                (flag == PEDGE_ON)) {
               phg_set_polygon_offset(phg_get_edge_width(ast));
               glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
               glEnable(GL_POLYGON_OFFSET_FILL);
               glColor3f(wsgl->background.val.general.x,
                         wsgl->background.val.general.y,
                         wsgl->background.val.general.z);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->ptco[i].point.x,
                             fasd3->vdata->ptco[i].point.y,
                             fasd3->vdata->ptco[i].point.z);
               }
               glEnd();
               glDisable(GL_POLYGON_OFFSET_FILL);

               phg_setup_edge_attr(ast);
               glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
               glBegin(GL_POLYGON);
               for (i = 0; i < fasd3->num_vertices; i++) {
                  glVertex3f(fasd3->vdata->ptco[i].point.x,
                             fasd3->vdata->ptco[i].point.y,
                             fasd3->vdata->ptco[i].point.z);
               }
               glEnd();
               glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            else if (((style == PSTYLE_SOLID) || (style == PSTYLE_HATCH)) &&
                      (flag == PEDGE_ON)) {
               phg_setup_int_attr_nocol(ast);
               glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
               glEnable(GL_POLYGON_OFFSET_FILL);
               phg_set_polygon_offset(phg_get_edge_width(ast));
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
               glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

