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
#include <phigs/private/fasd3P.h>

/*******************************************************************************
 * priv_normal3
 *
 * DESCR:	Get facet normal 3D helper function
 * RETURNS:	Pointer to normal or NULL
 */

static Pvec3* priv_normal3(
   Pint fflag,
   Pfacet_data3 *fdata
   )
{
   Pvec3 *normal;

   if (fflag == PFACET_NORMAL) {
      normal = &fdata->norm;
   }
   else if (fflag == PFACET_COLOUR_NORMAL) {
      normal = &fdata->conorm.norm;
   }
   else {
      normal = NULL;
   }

   return normal;
}

/*******************************************************************************
 * priv_shade_facet3
 *
 * DESCR:	Shade facet 3D helper function
 * RETURNS:	N/A
 */

static void priv_shade_facet3(
   Ws *ws,
   Pcoval *result,
   Pint fflag,
   Pint colr_type,
   Pfacet_data3 *fdata,
   Ws_attr_st *ast
   )
{
   Pcoval colr;
   Pvec3 *normal;
   Pint refl_eqn;
   Prefl_props *refl_props;

   normal = priv_normal3(fflag, fdata);
   refl_eqn = wsgl_get_refl_eqn(ast);
   refl_props = wsgl_get_refl_props(ast);

   wsgl_get_facet_colr(&colr, fflag, fdata, ast);

   if (fflag == PFACET_NORMAL || fflag == PFACET_COLOUR_NORMAL) {
      wsgl_light_colr(ws,
                      result,
                      refl_eqn,
                      refl_props,
                      colr_type,
                      &colr,
                      normal);
   }
   else {
      memcpy(result, &colr, sizeof(Pcoval));
   }
}

/*******************************************************************************
 * priv_fill_area3_points
 *
 * DESCR:	Draw fill area with point data 3D helper function
 * RETURNS:	N/A
 */

static void priv_fill_area3_points(
   Pint num_vertices,
   Ppoint3 *points
   )
{
   Pint i;

   glBegin(GL_POLYGON);
   for (i = 0; i < num_vertices; i++) {
      glVertex3f(points[i].x,
                 points[i].y,
                 points[i].z);
   }
   glEnd();
}

/*******************************************************************************
 * priv_fill_area3_ptcolrs
 *
 * DESCR:	Draw fill area with point and colour data 3D helper function
 * RETURNS:	N/A
 */

static void priv_fill_area3_ptcolrs(
   Pint colr_type,
   Pint num_vertices,
   Pptco3 *ptcolrs
   )
{
   Pint i;

   glBegin(GL_POLYGON);
   for (i = 0; i < num_vertices; i++) {
      wsgl_set_colr(colr_type, &ptcolrs[i].colr);
      glVertex3f(ptcolrs[i].point.x,
                 ptcolrs[i].point.y,
                 ptcolrs[i].point.z);
   }
   glEnd();
}

/*******************************************************************************
 * priv_fill_area3_ptcolrs_norm
 *
 * DESCR:	Draw fill area with point and colour data 3D helper function
 * RETURNS:	N/A
 */

static void priv_fill_area3_ptcolrs_norm(
   Ws *ws,
   Pint colr_type,
   Pvec3 *normal,
   Pint refl_eqn,
   Prefl_props *refl_props,
   Pint num_vertices,
   Pptco3 *ptcolrs
   )
{
   Pint i;
   Pcoval result;

   glBegin(GL_POLYGON);
   for (i = 0; i < num_vertices; i++) {
      wsgl_light_colr(ws,
                      &result,
                      refl_eqn,
                      refl_props,
                      colr_type,
                      &ptcolrs[i].colr,
                      normal);
      wsgl_set_colr(colr_type, &result);
      glVertex3f(ptcolrs[i].point.x,
                 ptcolrs[i].point.y,
                 ptcolrs[i].point.z);
   }
   glEnd();
}

/*******************************************************************************
 * priv_fill_area3_ptnorms
 *
 * DESCR:	Draw fill area with point and normal data 3D helper function
 * RETURNS:	N/A
 */

static void priv_fill_area3_ptnorms(
   Ws *ws,
   Pint colr_type,
   Pcoval *colr,
   Pint refl_eqn,
   Prefl_props *refl_props,
   Pint num_vertices,
   Pptnorm3 *ptnorms
   )
{
   Pint i;
   Pcoval result;

   glBegin(GL_POLYGON);
   for (i = 0; i < num_vertices; i++) {
      wsgl_light_colr(ws,
                      &result,
                      refl_eqn,
                      refl_props,
                      colr_type,
                      colr,
                      &ptnorms[i].norm);
      wsgl_set_colr(colr_type, &result);
      glVertex3f(ptnorms[i].point.x,
                 ptnorms[i].point.y,
                 ptnorms[i].point.z);
   }
   glEnd();
}

/*******************************************************************************
 * priv_fill_area3_ptconorms
 *
 * DESCR:	Draw fill area with point, colour and normal data 3D
 *              helper function
 * RETURNS:	N/A
 */

static void priv_fill_area3_ptconorms(
   Ws *ws,
   Pint colr_type,
   Pint refl_eqn,
   Prefl_props *refl_props,
   Pint num_vertices,
   Pptconorm3 *ptconorms
   )
{
   Pint i;
   Pcoval result;

   glBegin(GL_POLYGON);
   for (i = 0; i < num_vertices; i++) {
      wsgl_light_colr(ws,
                      &result,
                      refl_eqn,
                      refl_props,
                      colr_type,
                      &ptconorms[i].colr,
                      &ptconorms[i].norm);
      wsgl_set_colr(colr_type, &result);
      glVertex3f(ptconorms[i].point.x,
                 ptconorms[i].point.y,
                 ptconorms[i].point.z);
   }
   glEnd();
}

/*******************************************************************************
 * wsgl_fill_area_set3_data
 *
 * DESCR:	Draw fill area set with data 3D
 * RETURNS:	N/A
 */

void wsgl_fill_area_set3_data(
   Ws *ws,
   void *pdata,
   Ws_attr_st *ast
   )
{
   Pint i;
   Pfasd3 fasd3;
   Pedge_data_list edata;
   Pfacet_vdata_list3 vdata;

   Pcoval colr;
   Pvec3 *normal;
   Pint refl_eqn;
   Prefl_props *refl_props;

   fasd3.edata = &edata;
   fasd3.vdata = &vdata;
   fasd3_head(&fasd3, pdata);

   glPolygonOffset(WS_FILL_AREA_OFFSET, wsgl_get_edge_width(ast));
   glEnable(GL_POLYGON_OFFSET_FILL);
   glEnable(GL_POLYGON_OFFSET_LINE);
   wsgl_setup_int_attr_nocol(ws, ast);

   switch (fasd3.vflag) {
      case PVERT_COORD:
         priv_shade_facet3(ws,
                           &colr,
                           fasd3.fflag,
                           fasd3.colr_type,
                           &fasd3.fdata,
                           ast);
         wsgl_set_colr(fasd3.colr_type, &colr);
         for (i = 0; i < fasd3.nfa; i++) {
            priv_fill_area3_points(fasd3.vdata->num_vertices,
                                   fasd3.vdata->vertex_data.points);

            /* Advance to next set of data */
            fasd3_next_vdata3(&fasd3);
            if (fasd3.eflag == PEDGE_VISIBILITY) {
               fasd3_next_edata(&fasd3);
            }
         }
         break;

      case PVERT_COORD_COLOUR:
         if (fasd3.fflag == PFACET_NORMAL) {
            normal = priv_normal3(fasd3.fflag, &fasd3.fdata);
            refl_eqn = wsgl_get_refl_eqn(ast);
            refl_props = wsgl_get_refl_props(ast);
            for (i = 0; i < fasd3.nfa; i++) {
               priv_fill_area3_ptcolrs_norm(ws,
                                            fasd3.colr_type,
                                            normal,
                                            refl_eqn,
                                            refl_props,
                                            fasd3.vdata->num_vertices,
                                            fasd3.vdata->vertex_data.ptcolrs);

               /* Advance to next set of data */
               fasd3_next_vdata3(&fasd3);
               if (fasd3.eflag == PEDGE_VISIBILITY) {
                  fasd3_next_edata(&fasd3);
               }
               else {
                  fasd3.edata->num_edges = fasd3.vdata->num_vertices;
               }
            }
         }
         else {
            for (i = 0; i < fasd3.nfa; i++) {
               priv_fill_area3_ptcolrs(fasd3.colr_type,
                                       fasd3.vdata->num_vertices,
                                       fasd3.vdata->vertex_data.ptcolrs);

               /* Advance to next set of data */
               fasd3_next_vdata3(&fasd3);
               if (fasd3.eflag == PEDGE_VISIBILITY) {
                  fasd3_next_edata(&fasd3);
               }
            }
         }
         break;

      case PVERT_COORD_NORMAL:
         wsgl_get_facet_colr(&colr, fasd3.fflag, &fasd3.fdata, ast);
         refl_eqn = wsgl_get_refl_eqn(ast);
         refl_props = wsgl_get_refl_props(ast);
         for (i = 0; i < fasd3.nfa; i++) {
            priv_fill_area3_ptnorms(ws,
                                    fasd3.colr_type,
                                    &colr,
                                    refl_eqn,
                                    refl_props,
                                    fasd3.vdata->num_vertices,
                                    fasd3.vdata->vertex_data.ptnorms);

            /* Advance to next set of data */
            fasd3_next_vdata3(&fasd3);
            if (fasd3.eflag == PEDGE_VISIBILITY) {
               fasd3_next_edata(&fasd3);
            }
         }
         break;

      case PVERT_COORD_COLOUR_NORMAL:
         refl_eqn = wsgl_get_refl_eqn(ast);
         refl_props = wsgl_get_refl_props(ast);
         for (i = 0; i < fasd3.nfa; i++) {
            priv_fill_area3_ptconorms(ws,
                                      fasd3.colr_type,
                                      refl_eqn,
                                      refl_props,
                                      fasd3.vdata->num_vertices,
                                      fasd3.vdata->vertex_data.ptconorms);

            /* Advance to next set of data */
            fasd3_next_vdata3(&fasd3);
            if (fasd3.eflag == PEDGE_VISIBILITY) {
               fasd3_next_edata(&fasd3);
            }
         }
         break;

      default:
         break;
   }

   glDisable(GL_POLYGON_OFFSET_LINE);
   glDisable(GL_POLYGON_OFFSET_FILL);
}

