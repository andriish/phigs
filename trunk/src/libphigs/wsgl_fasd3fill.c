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
#include <math.h>
#include <GL/gl.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>
#include <phigs/private/wsglP.h>
#include <phigs/private/fasd3P.h>

/*******************************************************************************
 * priv_get_facet_colr
 *
 * DESCR:       Get colour helper function
 * RETURNS:     N/A
 */

static void priv_get_colr(
   Pcoval *colr,
   Pint fflag,
   Pfacet_data3 *fdata,
   Ws_attr_st *ast
   )
{
   switch (fflag) {
      case PFACET_COLOUR:
         memcpy(colr, &fdata->colr, sizeof(Pcoval));
         break;

      case PFACET_COLOUR_NORMAL:
         memcpy(colr, &fdata->conorm.colr, sizeof(Pcoval));
         break;

      default:
         wsgl_colr_from_gcolr(colr, wsgl_get_int_colr(ast));
         break;
   }
}

/*******************************************************************************
 * priv_facet_normal3
 *
 * DESCR:	Set facet normal 3D helper function
 * RETURNS:	N/A
 */

static void priv_facet_normal3(
   Pint fflag,
   Pfacet_data3 *fdata
   )
{
   if (fflag == PFACET_NORMAL) {
      glNormal3f(fdata->norm.delta_x,
                 fdata->norm.delta_y,
                 fdata->norm.delta_z);
   }
   else if (fflag == PFACET_COLOUR_NORMAL) {
      glNormal3f(fdata->conorm.norm.delta_x,
                 fdata->conorm.norm.delta_y,
                 fdata->conorm.norm.delta_z);
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
   Pptco3 *ptcolrs,
   Ws_attr_st *ast
   )
{
   Pint i;

   glBegin(GL_POLYGON);
   for (i = 0; i < num_vertices; i++) {
      wsgl_setup_int_refl_props(colr_type, &ptcolrs[i].colr, ast);
      glVertex3f(ptcolrs[i].point.x,
                 ptcolrs[i].point.y,
                 ptcolrs[i].point.z);
   }
   glEnd();
}

/*******************************************************************************
 * priv_fill_area3_ptcolrs_ns
 *
 * DESCR:	Draw fill area with point and colour data 3D, without shading
 *		helper function
 * RETURNS:	N/A
 */

static void priv_fill_area3_ptcolrs_ns(
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
 * priv_fill_area3_ptnorms
 *
 * DESCR:	Draw fill area with point and normal data 3D helper function
 * RETURNS:	N/A
 */

static void priv_fill_area3_ptnorms(
   Pint num_vertices,
   Pptnorm3 *ptnorms
   )
{
   Pint i;

   glBegin(GL_POLYGON);
   for (i = 0; i < num_vertices; i++) {
      glNormal3f(ptnorms[i].norm.delta_x,
                 ptnorms[i].norm.delta_y,
                 ptnorms[i].norm.delta_z);
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
   Pint colr_type,
   Pint num_vertices,
   Pptconorm3 *ptconorms,
   Ws_attr_st *ast
   )
{
   Pint i;

   glBegin(GL_POLYGON);
   for (i = 0; i < num_vertices; i++) {
      glNormal3f(ptconorms[i].norm.delta_x,
                 ptconorms[i].norm.delta_y,
                 ptconorms[i].norm.delta_z);
      wsgl_setup_int_refl_props(colr_type, &ptconorms[i].colr, ast);
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

   Wsgl_handle wsgl = ws->render_context;

   fasd3.edata = &edata;
   fasd3.vdata = &vdata;
   fasd3_head(&fasd3, pdata);

   glPolygonOffset(WS_FILL_AREA_OFFSET, wsgl_get_edge_width(ast));
   glEnable(GL_POLYGON_OFFSET_FILL);
   glEnable(GL_POLYGON_OFFSET_LINE);
   wsgl_setup_int_attr_nocol(ws, ast);

   if (wsgl->cur_struct.lighting) {
      glEnable(GL_LIGHTING);
   }

   switch (fasd3.vflag) {
      case PVERT_COORD:
         priv_facet_normal3(fasd3.fflag, &fasd3.fdata);
         priv_get_colr(&colr, fasd3.fflag, &fasd3.fdata, ast);
         wsgl_setup_int_refl_props(fasd3.colr_type, &colr, ast);
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
            priv_facet_normal3(fasd3.fflag, &fasd3.fdata);
            for (i = 0; i < fasd3.nfa; i++) {
               priv_fill_area3_ptcolrs(fasd3.colr_type,
                                       fasd3.vdata->num_vertices,
                                       fasd3.vdata->vertex_data.ptcolrs,
                                       ast);

               /* Advance to next set of data */
               fasd3_next_vdata3(&fasd3);
               if (fasd3.eflag == PEDGE_VISIBILITY) {
                  fasd3_next_edata(&fasd3);
               }
            }
         }
         else {
            for (i = 0; i < fasd3.nfa; i++) {
               priv_fill_area3_ptcolrs_ns(fasd3.colr_type,
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
         priv_get_colr(&colr, fasd3.fflag, &fasd3.fdata, ast);
         wsgl_setup_int_refl_props(fasd3.colr_type, &colr, ast);
         for (i = 0; i < fasd3.nfa; i++) {
            priv_fill_area3_ptnorms(fasd3.vdata->num_vertices,
                                    fasd3.vdata->vertex_data.ptnorms);

            /* Advance to next set of data */
            fasd3_next_vdata3(&fasd3);
            if (fasd3.eflag == PEDGE_VISIBILITY) {
               fasd3_next_edata(&fasd3);
            }
         }
         break;

      case PVERT_COORD_COLOUR_NORMAL:
         for (i = 0; i < fasd3.nfa; i++) {
            priv_fill_area3_ptconorms(fasd3.colr_type,
                                      fasd3.vdata->num_vertices,
                                      fasd3.vdata->vertex_data.ptconorms,
                                      ast);

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

   if (wsgl->cur_struct.lighting) {
      glDisable(GL_LIGHTING);
   }

   glDisable(GL_POLYGON_OFFSET_LINE);
   glDisable(GL_POLYGON_OFFSET_FILL);
}

