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

#ifdef DEBUG
/*******************************************************************************
 * phg_print_fasd3
 *
 * DESCR:       Print fasd3
 * RETURNS:	N/A
 */

static void phg_print_fasd3(
    Pfasd3 *fasd3
    )
{
   printf("fflag: %d\n", fasd3->fflag);
   printf("eflag: %d\n", fasd3->eflag);
   printf("vflag: %d\n", fasd3->vflag);
   printf("colr_type: %d\n", fasd3->colr_type);
   switch (fasd3->fflag) {
      case PFACET_COLOUR:
         printf("Colour: %f %f %f\n",
                fasd3->fdata.colr.val.general.x,
                fasd3->fdata.colr.val.general.y,
                fasd3->fdata.colr.val.general.z);
         break;

      case PFACET_NORMAL:
         printf("Normal: %f %f %f\n",
                fasd3->fdata.normal.delta_x,
                fasd3->fdata.normal.delta_y,
                fasd3->fdata.normal.delta_z);
         break;

      case PFACET_COLOUR_NORMAL:
         break;

      default:
         break;
   }

   printf("nfa: %d\n", fasd3->nfa);
}
#endif

/*******************************************************************************
 * phg_facet_head3
 *
 * DESCR:	Get facet head information helper function
 * RETURNS:	N/A
 */

static void phg_facet_head3(
   Pfasd3 *fasd3,
   void *pdata
   )
{
   Pint i, num_edges;
   char *tp;
   Pint *data = (Pint *) pdata;

   fasd3->fflag = data[0];
   fasd3->eflag = data[1];
   fasd3->vflag = data[2];
   fasd3->colr_type = data[3];
   tp = (char *) &data[4];

   switch (fasd3->fflag) {
      case PFACET_COLOUR:
         memcpy(&fasd3->fdata.colr, tp, sizeof(Pgcolr));
         tp += sizeof(Pgcolr);
         break;

      case PFACET_NORMAL:
         memcpy(&fasd3->fdata.normal, tp, sizeof(Pvec3));
         tp += sizeof(Pvec3);
         break;

      case PFACET_COLOUR_NORMAL:
         memcpy(&fasd3->fdata.conorm, tp, sizeof(Pptconorm3));
         tp += sizeof(Pptconorm3);
         break;

      default:
         break;
   }

   data = (Pint *) tp;
   fasd3->nfa = data[0];
   tp = (char *) &data[1];

   if (fasd3->eflag == PEDGE_VISIBILITY) {
      data = (Pint *) tp;
      fasd3->edata->num_edges = data[0];
      fasd3->edata->edgedata.edges = (Pedge_flag *) &data[1];

      for (i = 0; i < fasd3->nfa; i++) {
         data = (Pint *) tp;
         num_edges = data[0];
         tp += sizeof(Pedge_flag) * num_edges + sizeof(Pint);
      }
   }

   data = (Pint *) tp;
   fasd3->vdata->num_vertices = data[0];
   switch (fasd3->vflag) {
      case PVERT_COORD:
         fasd3->vdata->vertex_data.points = (Ppoint3 *) &data[1];
         break;

      case PVERT_COORD_COLOUR:
         fasd3->vdata->vertex_data.ptcolrs = (Pptco3 *) &data[1];
         break;

      case PVERT_COORD_NORMAL:
         fasd3->vdata->vertex_data.ptnorms = (Pptnorm3 *) &data[1];
         break;

      case PVERT_COORD_COLOUR_NORMAL:
         fasd3->vdata->vertex_data.ptconorms = (Pptconorm3 *) &data[1];
         break;

      default:
         break;
   }
}

/*******************************************************************************
 * phg_next_facet_edata
 *
 * DESCR:	Get facet edge data helper function
 * NOTE:        Remember to set up Pfasd3->edata to point to a valid
 *              Pedge_data_list structure
 * RETURNS:	N/A
 */

static void phg_next_facet_edata(
   Pfasd3 *fasd3
   )
{
   Pint *data;
   char *tp;
   Pint num_edges;

   num_edges = fasd3->edata->num_edges;
   tp = (char *) fasd3->edata->edgedata.edges;
   tp += sizeof(Pedge_flag) * num_edges;
   data = (Pint *) tp;
   fasd3->edata->num_edges = data[0];
   fasd3->edata->edgedata.edges = (Pedge_flag *) &data[1];
}

/*******************************************************************************
 * phg_next_facet_vdata3
 *
 * DESCR:	Get facet vertex data helper function
 * NOTE:        Remember to set up Pfasd3->vdata to point to a valid
 *              Pfacet_vdata_list3 structure
 * RETURNS:	N/A
 */

static void phg_next_facet_vdata3(
   Pfasd3 *fasd3
   )
{
   Pint *data;
   char *tp;
   Pint num_vertices = fasd3->vdata->num_vertices;

   switch (fasd3->vflag) {
      case PVERT_COORD:
         tp = (char *) fasd3->vdata->vertex_data.points;
         tp += sizeof(Ppoint3) * num_vertices;
         data = (Pint *) tp;
         fasd3->vdata->num_vertices = data[0];
         fasd3->vdata->vertex_data.points = (Ppoint3 *) &data[1];
         break;

      case PVERT_COORD_COLOUR:
         tp = (char *) fasd3->vdata->vertex_data.ptcolrs;
         tp += sizeof(Pptco3) * num_vertices;
         data = (Pint *) tp;
         fasd3->vdata->num_vertices = data[0];
         fasd3->vdata->vertex_data.ptcolrs = (Pptco3 *) &data[1];
         break;

      case PVERT_COORD_NORMAL:
         tp = (char *) fasd3->vdata->vertex_data.ptnorms;
         tp += sizeof(Pptnorm3);
         data = (Pint *) tp;
         fasd3->vdata->num_vertices = data[0];
         fasd3->vdata->vertex_data.ptnorms = (Pptnorm3 *) &data[1];
         break;

      case PVERT_COORD_COLOUR_NORMAL:
         tp = (char *) fasd3->vdata->vertex_data.ptconorms;
         tp += sizeof(Pptconorm3) * num_vertices;
         data = (Pint *) tp;
         fasd3->vdata->num_vertices = data[0];
         fasd3->vdata->vertex_data.ptconorms = (Pptconorm3 *) &data[1];
         break;

      default:
         break;
   }
}

/*******************************************************************************
 * phg_get_facet_normal3
 *
 * DESCR:	Get facet normal 3D helper function
 * RETURNS:	Pointer to normal or NULL
 */

static Pvec3* phg_get_facet_normal3(
   Pint fflag,
   Pfacet_data3 *fdata
   )
{
   Pvec3 *normal;

   if (fflag == PFACET_NORMAL) {
      normal = &fdata->normal;
   }
   else if (fflag == PFACET_COLOUR_NORMAL) {
      normal = &fdata->conorm.normal;
   }
   else {
      normal = NULL;
   }

   return normal;
}

/*******************************************************************************
 * phg_shade_facet3
 *
 * DESCR:	Shade facet 3D helper function
 * RETURNS:	N/A
 */

static void phg_shade_facet3(
   Ws *ws,
   Pgcolr *result,
   Pint fflag,
   Pfacet_data3 *fdata,
   Ws_attr_st *ast
   )
{
   Pvec3 *normal;
   Pint refl_eqn;
   Prefl_props *refl_props;

   normal = phg_get_facet_normal3(fflag, fdata);
   refl_eqn = phg_get_int_refl_eqn(ast);
   refl_props = phg_get_refl_props(ast);

   if (fflag == PFACET_NORMAL) {
      wsgl_light_colr(ws,
                      result,
                      refl_eqn,
                      refl_props,
                      phg_get_facet_colr(fflag, fdata, ast),
                      normal);
   }
   else if (fflag == PFACET_COLOUR_NORMAL) {
      wsgl_light_colr(ws,
                      result,
                      refl_eqn,
                      refl_props,
                      phg_get_facet_colr(fflag, fdata, ast),
                      normal);
   }
   else {
      memcpy(result, phg_get_facet_colr(fflag, fdata, ast), sizeof(Pgcolr));
   }
}

/*******************************************************************************
 * phg_draw_fill_area3_points
 *
 * DESCR:	Draw fill area with point data 3D helper function
 * RETURNS:	N/A
 */

static void phg_draw_fill_area3_points(
   Ws *ws,
   Pint_style style,
   Pedge_flag flag,
   Pgcolr *gcolr,
   Pint num_vertices,
   Ppoint3 *points,
   Ws_attr_st *ast
   )
{
   Pint i;
   Wsgl_handle wsgl = ws->render_context;

   if ((style == PSTYLE_EMPTY) || (style == PSTYLE_HOLLOW)) {

      /* If hidden surface removal, clear interiour to background colour */
      if (wsgl->cur_struct.hlhsr_id == PHIGS_HLHSR_ID_ON) {
         phg_setup_background(ws);
         glBegin(GL_POLYGON);
         for (i = 0; i < num_vertices; i++) {
            glVertex3f(points[i].x,
                       points[i].y,
                       points[i].z);
         }
         glEnd();
      }

      if (style == PSTYLE_HOLLOW) {
         phg_setup_int_attr_nocol(ast);
         phg_set_gcolr(gcolr);
         glBegin(GL_POLYGON);
         for (i = 0; i < num_vertices; i++) {
            glVertex3f(points[i].x,
                       points[i].y,
                       points[i].z);
         }
         glEnd();
      }
   }

   else {
      phg_setup_int_attr_nocol(ast);
      phg_set_gcolr(gcolr);
      glBegin(GL_POLYGON);
      for (i = 0; i < num_vertices; i++) {
         glVertex3f(points[i].x,
                    points[i].y,
                    points[i].z);
      }
      glEnd();
   }
}

/*******************************************************************************
 * phg_draw_fill_area3_ptcolrs
 *
 * DESCR:	Draw fill area with point and colour data 3D helper function
 * RETURNS:	N/A
 */

static void phg_draw_fill_area3_ptcolrs(
   Ws *ws,
   Pint_style style,
   Pedge_flag flag,
   Pint num_vertices,
   Pptco3 *ptcolrs,
   Ws_attr_st *ast
   )
{
   Pint i;
   Wsgl_handle wsgl = ws->render_context;

   if ((style == PSTYLE_EMPTY) || (style == PSTYLE_HOLLOW)) {

      /* If hidden surface removal, clear interiour to background colour */
      if (wsgl->cur_struct.hlhsr_id == PHIGS_HLHSR_ID_ON) {
         phg_setup_background(ws);
         glBegin(GL_POLYGON);
         for (i = 0; i < num_vertices; i++) {
            glVertex3f(ptcolrs[i].point.x,
                       ptcolrs[i].point.y,
                       ptcolrs[i].point.z);
         }
         glEnd();
      }

      if (style == PSTYLE_HOLLOW) {
         phg_setup_int_attr(ast);
         glBegin(GL_POLYGON);
         for (i = 0; i < num_vertices; i++) {
            phg_set_gcolr(&ptcolrs[i].colr);
            glVertex3f(ptcolrs[i].point.x,
                       ptcolrs[i].point.y,
                       ptcolrs[i].point.z);
         }
         glEnd();
      }
   }

   else {
      phg_setup_int_attr_nocol(ast);
      glBegin(GL_POLYGON);
      for (i = 0; i < num_vertices; i++) {
         phg_set_gcolr(&ptcolrs[i].colr);
         glVertex3f(ptcolrs[i].point.x,
                    ptcolrs[i].point.y,
                    ptcolrs[i].point.z);
      }
      glEnd();
   }
}

/*******************************************************************************
 * phg_draw_fill_area3_ptcolrs_norm
 *
 * DESCR:	Draw fill area with point and colour data 3D helper function
 * RETURNS:	N/A
 */

static void phg_draw_fill_area3_ptcolrs_norm(
   Ws *ws,
   Pint_style style,
   Pedge_flag flag,
   Pvec3 *normal,
   Pint refl_eqn,
   Prefl_props *refl_props,
   Pint num_vertices,
   Pptco3 *ptcolrs,
   Ws_attr_st *ast
   )
{
   Pint i;
   Pgcolr result;
   Wsgl_handle wsgl = ws->render_context;

   if ((style == PSTYLE_EMPTY) || (style == PSTYLE_HOLLOW)) {

      /* If hidden surface removal, clear interiour to background colour */
      if (wsgl->cur_struct.hlhsr_id == PHIGS_HLHSR_ID_ON) {
         phg_setup_background(ws);
         glBegin(GL_POLYGON);
         for (i = 0; i < num_vertices; i++) {
            glVertex3f(ptcolrs[i].point.x,
                       ptcolrs[i].point.y,
                       ptcolrs[i].point.z);
         }
         glEnd();
      }

      if (style == PSTYLE_HOLLOW) {
         phg_setup_int_attr(ast);
         glBegin(GL_POLYGON);
         for (i = 0; i < num_vertices; i++) {
            wsgl_light_colr(ws,
                            &result,
                            refl_eqn,
                            refl_props,
                            &ptcolrs[i].colr,
                            normal);
            phg_set_gcolr(&result);
            glVertex3f(ptcolrs[i].point.x,
                       ptcolrs[i].point.y,
                       ptcolrs[i].point.z);
         }
         glEnd();
      }
   }

   else {
      phg_setup_int_attr_nocol(ast);
      glBegin(GL_POLYGON);
      for (i = 0; i < num_vertices; i++) {
         wsgl_light_colr(ws,
                         &result,
                         refl_eqn,
                         refl_props,
                         &ptcolrs[i].colr,
                         normal);
         phg_set_gcolr(&result);
         glVertex3f(ptcolrs[i].point.x,
                    ptcolrs[i].point.y,
                    ptcolrs[i].point.z);
      }
      glEnd();
   }
}

/*******************************************************************************
 * phg_draw_fill_area3_ptnorms
 *
 * DESCR:	Draw fill area with point and normal data 3D helper function
 * RETURNS:	N/A
 */

static void phg_draw_fill_area3_ptnorms(
   Ws *ws,
   Pint_style style,
   Pedge_flag flag,
   Pgcolr *gcolr,
   Pint refl_eqn,
   Prefl_props *refl_props,
   Pint num_vertices,
   Pptnorm3 *ptnorms,
   Ws_attr_st *ast
   )
{
   Pint i;
   Pgcolr result;
   Wsgl_handle wsgl = ws->render_context;

   if ((style == PSTYLE_EMPTY) || (style == PSTYLE_HOLLOW)) {

      /* If hidden surface removal, clear interiour to background colour */
      if (wsgl->cur_struct.hlhsr_id == PHIGS_HLHSR_ID_ON) {
         phg_setup_background(ws);
         glBegin(GL_POLYGON);
         for (i = 0; i < num_vertices; i++) {
            glVertex3f(ptnorms[i].point.x,
                       ptnorms[i].point.y,
                       ptnorms[i].point.z);
         }
         glEnd();
      }

      if (style == PSTYLE_HOLLOW) {
         phg_setup_int_attr_nocol(ast);
         glBegin(GL_POLYGON);
         for (i = 0; i < num_vertices; i++) {
            wsgl_light_colr(ws,
                            &result,
                            refl_eqn,
                            refl_props,
                            gcolr,
                            &ptnorms[i].normal);
            phg_set_gcolr(&result);
            glVertex3f(ptnorms[i].point.x,
                       ptnorms[i].point.y,
                       ptnorms[i].point.z);
         }
         glEnd();
      }
   }

   else {
      phg_setup_int_attr_nocol(ast);
      glBegin(GL_POLYGON);
      for (i = 0; i < num_vertices; i++) {
         wsgl_light_colr(ws,
                         &result,
                         refl_eqn,
                         refl_props,
                         gcolr,
                         &ptnorms[i].normal);
         phg_set_gcolr(&result);
         glVertex3f(ptnorms[i].point.x,
                    ptnorms[i].point.y,
                    ptnorms[i].point.z);
      }
      glEnd();
   }
}

/*******************************************************************************
 * phg_draw_fill_area3_ptconorm_data
 *
 * DESCR:	Draw fill area with colour and normal data 3D helper function
 * RETURNS:	N/A
 */

static void phg_draw_fill_area3_ptconorm_data(
   Ws *ws,
   Pint fflag,
   Pint vflag,
   Pfacet_data3 *fdata,
   Pint num_vertices,
   Pptconorm3 *ptconorm,
   Ws_attr_st *ast
   )
{
   Pint i;
   Pint_style style;
   Pedge_flag flag;
   Pint refl_eqn;
   Prefl_props *refl_props;
   Pgcolr result;
   Wsgl_handle wsgl = ws->render_context;

   style = phg_get_int_style(ast);
   flag = phg_get_edge_flag(ast);
   refl_eqn = phg_get_int_refl_eqn(ast);
   refl_props = phg_get_refl_props(ast);

   if (wsgl->cur_struct.hlhsr_id == PHIGS_HLHSR_ID_OFF) {
      if (style != PSTYLE_EMPTY) {
         phg_setup_int_attr_nocol(ast);
         glBegin(GL_POLYGON);
         for (i = 0; i < num_vertices; i++) {
            wsgl_light_colr(ws,
                            &result,
                            refl_eqn,
                            refl_props,
                            &ptconorm[i].colr,
                            &ptconorm[i].normal);
            phg_set_gcolr(&result);
            glVertex3f(ptconorm[i].point.x,
                       ptconorm[i].point.y,
                       ptconorm[i].point.z);
         }
         glEnd();
      }

      if (flag == PEDGE_ON) {
         phg_setup_edge_attr(ast);
         glBegin(GL_LINE_LOOP);
         for (i = 0; i < num_vertices; i++) {
            glVertex3f(ptconorm[i].point.x,
                       ptconorm[i].point.y,
                       ptconorm[i].point.z);
         }
         glEnd();
      }
   }
   else {
      if ((flag == PEDGE_ON) &&
          ((style == PSTYLE_EMPTY) || (style == PSTYLE_HOLLOW))) {
         phg_set_polygon_offset(phg_get_edge_width(ast));
         glEnable(GL_POLYGON_OFFSET_FILL);
         phg_setup_background(ws);
         glBegin(GL_POLYGON);
         for (i = 0; i < num_vertices; i++) {
            glVertex3f(ptconorm[i].point.x,
                       ptconorm[i].point.y,
                       ptconorm[i].point.z);
         }
         glEnd();
         glDisable(GL_POLYGON_OFFSET_FILL);

         if (flag == PEDGE_ON) {
            phg_setup_edge_attr(ast);
         }
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glBegin(GL_POLYGON);
         for (i = 0; i < num_vertices; i++) {
            glVertex3f(ptconorm[i].point.x,
                       ptconorm[i].point.y,
                       ptconorm[i].point.z);
         }
         glEnd();
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
      else if ((flag == PEDGE_OFF) &&
               (style == PSTYLE_HOLLOW)) {
         phg_set_polygon_offset(phg_get_edge_width(ast));
         glEnable(GL_POLYGON_OFFSET_FILL);
         phg_setup_background(ws);
         glBegin(GL_POLYGON);
         for (i = 0; i < num_vertices; i++) {
            glVertex3f(ptconorm[i].point.x,
                       ptconorm[i].point.y,
                       ptconorm[i].point.z);
         }
         glEnd();
         glDisable(GL_POLYGON_OFFSET_FILL);

         phg_setup_int_attr_nocol(ast);
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glBegin(GL_POLYGON);
         for (i = 0; i < num_vertices; i++) {
            wsgl_light_colr(ws,
                            &result,
                            refl_eqn,
                            refl_props,
                            &ptconorm[i].colr,
                            &ptconorm[i].normal);
            phg_set_gcolr(&result);
            glVertex3f(ptconorm[i].point.x,
                       ptconorm[i].point.y,
                       ptconorm[i].point.z);
         }
         glEnd();
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
      else {
         if (style != PSTYLE_EMPTY) {
            phg_setup_int_attr_nocol(ast);
            glEnable(GL_POLYGON_OFFSET_FILL);
            phg_set_polygon_offset(phg_get_edge_width(ast));
            glBegin(GL_POLYGON);
            for (i = 0; i < num_vertices; i++) {
               wsgl_light_colr(ws,
                               &result,
                               refl_eqn,
                               refl_props,
                               &ptconorm[i].colr,
                               &ptconorm[i].normal);
               phg_set_gcolr(&result);
               glVertex3f(ptconorm[i].point.x,
                          ptconorm[i].point.y,
                          ptconorm[i].point.z);
            }
            glEnd();
            glDisable(GL_POLYGON_OFFSET_FILL);
         }

         if (flag == PEDGE_ON) {
            phg_setup_edge_attr(ast);
            glDisable(GL_POLYGON_STIPPLE);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBegin(GL_POLYGON);
            for (i = 0; i < num_vertices; i++) {
               glVertex3f(ptconorm[i].point.x,
                          ptconorm[i].point.y,
                          ptconorm[i].point.z);
            }
            glEnd();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         }
      }
   }
}

/*******************************************************************************
 * phg_draw_edges3_points
 *
 * DESCR:	Draw edges for fill area 3D with point data
 * RETURNS:	N/A
 */

static void phg_draw_edges3_points(
   Ws *ws,
   Pint eflag,
   Pint num_vertices,
   Ppoint3 *points,
   Pedge_data_list *edata,
   Ws_attr_st *ast
   )
{
   Pint i;

   phg_setup_edge_attr(ast);
   if (eflag == PEDGE_VISIBILITY) {
      glBegin(GL_LINES);
      for (i = 0; i < num_vertices - 1; i++) {
         if (edata->edgedata.edges[i] == PEDGE_ON) {
            glVertex3f(points[i].x,
                       points[i].y,
                       points[i].z);
            glVertex3f(points[i + 1].x,
                       points[i + 1].y,
                       points[i + 1].z);
         }
      }
      if (edata->edgedata.edges[i] == PEDGE_ON) {
         glVertex3f(points[i].x,
                    points[i].y,
                    points[i].z);
         glVertex3f(points[0].x,
                    points[0].y,
                    points[0].z);
      }
      glEnd();
   }
   else {
      glBegin(GL_LINE_LOOP);
      for (i = 0; i < num_vertices; i++) {
         glVertex3f(points[i].x,
                    points[i].y,
                    points[i].z);
      }
      glEnd();
   }
}

/*******************************************************************************
 * phg_draw_edges3_ptcolrs
 *
 * DESCR:	Draw edges for fill area 3D with point and colour data
 * RETURNS:	N/A
 */

static void phg_draw_edges3_ptcolrs(
   Ws *ws,
   Pint eflag,
   Pint num_vertices,
   Pptco3 *ptcolrs,
   Pedge_data_list *edata,
   Ws_attr_st *ast
   )
{
   Pint i;

   phg_setup_edge_attr(ast);
   if (eflag == PEDGE_VISIBILITY) {
      glBegin(GL_LINES);
      for (i = 0; i < num_vertices - 1; i++) {
         if (edata->edgedata.edges[i] == PEDGE_ON) {
            glVertex3f(ptcolrs[i].point.x,
                       ptcolrs[i].point.y,
                       ptcolrs[i].point.z);
            glVertex3f(ptcolrs[i + 1].point.x,
                       ptcolrs[i + 1].point.y,
                       ptcolrs[i + 1].point.z);
         }
      }
      if (edata->edgedata.edges[i] == PEDGE_ON) {
         glVertex3f(ptcolrs[i].point.x,
                    ptcolrs[i].point.y,
                    ptcolrs[i].point.z);
         glVertex3f(ptcolrs[0].point.x,
                    ptcolrs[0].point.y,
                    ptcolrs[0].point.z);
      }
      glEnd();
   }
   else {
      glBegin(GL_LINE_LOOP);
      for (i = 0; i < num_vertices; i++) {
         glVertex3f(ptcolrs[i].point.x,
                    ptcolrs[i].point.y,
                    ptcolrs[i].point.z);
      }
      glEnd();
   }
}

/*******************************************************************************
 * phg_draw_edges3_ptnorms
 *
 * DESCR:	Draw edges for fill area 3D with point and normal data
 * RETURNS:	N/A
 */

static void phg_draw_edges3_ptnorms(
   Ws *ws,
   Pint eflag,
   Pint num_vertices,
   Pptnorm3 *ptnorms,
   Pedge_data_list *edata,
   Ws_attr_st *ast
   )
{
   Pint i;

   phg_setup_edge_attr(ast);
   if (eflag == PEDGE_VISIBILITY) {
      glBegin(GL_LINES);
      for (i = 0; i < num_vertices - 1; i++) {
         if (edata->edgedata.edges[i] == PEDGE_ON) {
            glVertex3f(ptnorms[i].point.x,
                       ptnorms[i].point.y,
                       ptnorms[i].point.z);
            glVertex3f(ptnorms[i + 1].point.x,
                       ptnorms[i + 1].point.y,
                       ptnorms[i + 1].point.z);
         }
      }
      if (edata->edgedata.edges[i] == PEDGE_ON) {
         glVertex3f(ptnorms[i].point.x,
                    ptnorms[i].point.y,
                    ptnorms[i].point.z);
         glVertex3f(ptnorms[0].point.x,
                    ptnorms[0].point.y,
                    ptnorms[0].point.z);
      }
      glEnd();
   }
   else {
      glBegin(GL_LINE_LOOP);
      for (i = 0; i < num_vertices; i++) {
         glVertex3f(ptnorms[i].point.x,
                    ptnorms[i].point.y,
                    ptnorms[i].point.z);
      }
      glEnd();
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
   void *pdata,
   Ws_attr_st *ast
   )
{
   Pint i;
   Pfasd3 fasd3;
   Pedge_data_list edata;
   Pfacet_vdata_list3 vdata;

   Pgcolr result;
   Pgcolr *gcolr;
   Pvec3 *normal;
   Pint refl_eqn;
   Prefl_props *refl_props;

   Pint_style style = phg_get_int_style(ast);
   Pedge_flag flag = phg_get_edge_flag(ast);

   fasd3.edata = &edata;
   fasd3.vdata = &vdata;
   phg_facet_head3(&fasd3, pdata);

   phg_set_polygon_offset(phg_get_edge_width(ast));
   glEnable(GL_POLYGON_OFFSET_FILL);
   glEnable(GL_POLYGON_OFFSET_LINE);

   switch (fasd3.vflag) {
      case PVERT_COORD:
         phg_shade_facet3(ws,
                          &result,
                          fasd3.fflag,
                          &fasd3.fdata,
                          ast);
         for (i = 0; i < fasd3.nfa; i++) {
            phg_draw_fill_area3_points(ws,
                                       style,
                                       flag,
                                       &result,
                                       fasd3.vdata->num_vertices,
                                       fasd3.vdata->vertex_data.points,
                                       ast);
            if (flag == PEDGE_ON) {
               phg_draw_edges3_points(ws,
                                      fasd3.eflag,
                                      fasd3.vdata->num_vertices,
                                      fasd3.vdata->vertex_data.points,
                                      fasd3.edata,
                                      ast);
            }

            /* Advance to next set of data */
            phg_next_facet_vdata3(&fasd3);
            if (fasd3.eflag == PEDGE_VISIBILITY) {
               phg_next_facet_edata(&fasd3);
            }
         }
         break;

      case PVERT_COORD_COLOUR:
         if (fasd3.fflag == PFACET_NORMAL) {
            normal = phg_get_facet_normal3(fasd3.fflag, &fasd3.fdata);
            refl_eqn = phg_get_int_refl_eqn(ast);
            refl_props = phg_get_refl_props(ast);
            for (i = 0; i < fasd3.nfa; i++) {
               phg_draw_fill_area3_ptcolrs_norm(ws,
                                                style,
                                                flag,
                                                normal,
                                                refl_eqn,
                                                refl_props,
                                                fasd3.vdata->num_vertices,
                                                fasd3.vdata->vertex_data.
                                                   ptcolrs,
                                                ast);
               if (flag == PEDGE_ON) {
                  phg_draw_edges3_ptcolrs(ws,
                                          fasd3.eflag,
                                          fasd3.vdata->num_vertices,
                                          fasd3.vdata->vertex_data.ptcolrs,
                                          fasd3.edata,
                                          ast);
               }

               /* Advance to next set of data */
               phg_next_facet_vdata3(&fasd3);
               if (fasd3.eflag == PEDGE_VISIBILITY) {
                  phg_next_facet_edata(&fasd3);
               }
            }
         }
         else {
            for (i = 0; i < fasd3.nfa; i++) {
               phg_draw_fill_area3_ptcolrs(ws,
                                           style,
                                           flag,
                                           fasd3.vdata->num_vertices,
                                           fasd3.vdata->vertex_data.ptcolrs,
                                           ast);
               if (flag == PEDGE_ON) {
                  phg_draw_edges3_ptcolrs(ws,
                                          fasd3.eflag,
                                          fasd3.vdata->num_vertices,
                                          fasd3.vdata->vertex_data.ptcolrs,
                                          fasd3.edata,
                                          ast);
               }

               /* Advance to next set of data */
               phg_next_facet_vdata3(&fasd3);
               if (fasd3.eflag == PEDGE_VISIBILITY) {
                  phg_next_facet_edata(&fasd3);
               }
            }
         }
         break;

      case PVERT_COORD_NORMAL:
         gcolr = phg_get_facet_colr(fasd3.fflag, &fasd3.fdata, ast);
         refl_eqn = phg_get_int_refl_eqn(ast);
         refl_props = phg_get_refl_props(ast);
         for (i = 0; i < fasd3.nfa; i++) {
               phg_draw_fill_area3_ptnorms(ws,
                                           style,
                                           flag,
                                           gcolr,
                                           refl_eqn,
                                           refl_props,
                                           fasd3.vdata->num_vertices,
                                           fasd3.vdata->vertex_data.
                                              ptnorms,
                                           ast);
            if (flag == PEDGE_ON) {
               phg_draw_edges3_ptnorms(ws,
                                       fasd3.eflag,
                                       fasd3.vdata->num_vertices,
                                       fasd3.vdata->vertex_data.ptnorms,
                                       fasd3.edata,
                                       ast);
            }

            /* Advance to next set of data */
            phg_next_facet_vdata3(&fasd3);
            if (fasd3.eflag == PEDGE_VISIBILITY) {
               phg_next_facet_edata(&fasd3);
            }
         }
         break;

      case PVERT_COORD_COLOUR_NORMAL:
         for (i = 0; i < fasd3.nfa; i++) {
            phg_draw_fill_area3_ptconorm_data(ws,
                                              fasd3.fflag,
                                              fasd3.vflag,
                                              &fasd3.fdata,
                                              fasd3.vdata->num_vertices,
                                              fasd3.vdata->vertex_data.
                                                 ptconorms,
                                              ast);

            /* Advance to next set of data */
            phg_next_facet_vdata3(&fasd3);
            if (fasd3.eflag == PEDGE_VISIBILITY) {
               phg_next_facet_edata(&fasd3);
            }
         }
         break;

      default:
         break;
   }

   glDisable(GL_POLYGON_OFFSET_LINE);
   glDisable(GL_POLYGON_OFFSET_FILL);
}

