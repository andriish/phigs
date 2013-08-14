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
#include <phigs/private/sofas3P.h>

/*******************************************************************************
 * priv_facet_colr
 *
 * DESCR:	Get facet normal colour helper function
 * RETURNS:	N/A
 */

static void priv_facet_colr(
   Pcoval *colr,
   Pint fflag,
   Pfacet_data_arr3 *fdata,
   int index,
   Ws_attr_st *ast
   )
{
   switch (fflag) {
      case PFACET_COLOUR:
         memcpy(colr, &fdata->colrs[index], sizeof(Pcoval));
         break;

      case PFACET_COLOUR_NORMAL:
         memcpy(colr, &fdata->conorms[index].colr, sizeof(Pcoval));
         break;

      default:
         wsgl_colr_from_gcolr(colr, wsgl_get_int_colr(ast));
         break;
   }
}

/*******************************************************************************
 * priv_normal3
 *
 * DESCR:	Get facet normal 3D helper function
 * RETURNS:	Pointer to normal or NULL
 */

static Pvec3* priv_normal3(
   Pint fflag,
   Pfacet_data_arr3 *fdata,
   Pint index
   )
{
   Pvec3 *normal;

   if (fflag == PFACET_NORMAL) {
      normal = &fdata->norms[index];
   }
   else if (fflag == PFACET_COLOUR_NORMAL) {
      normal = &fdata->conorms[index].norm;
   }
   else {
      normal = NULL;
   }

   return normal;
}

/*******************************************************************************
 * priv_fill_area3_points
 *
 * DESCR:	Draw fill area with point data 3D helper function
 * RETURNS:	N/A
 */

static void priv_fill_area3_points(
   Pint_list *vlist,
   Ppoint3 *points
   )
{
   Pint i, vert;

   glBegin(GL_POLYGON);
   for (i = 0; i < vlist->num_ints; i++) {
      vert = vlist->ints[i];
      glVertex3f(points[vert].x,
                 points[vert].y,
                 points[vert].z);
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
   Pint_list *vlist,
   Pptco3 *ptcolrs
   )
{
   Pint i, vert;

   glBegin(GL_POLYGON);
   for (i = 0; i < vlist->num_ints; i++) {
      vert = vlist->ints[i];
      wsgl_set_colr(colr_type, &ptcolrs[vert].colr);
      glVertex3f(ptcolrs[vert].point.x,
                 ptcolrs[vert].point.y,
                 ptcolrs[vert].point.z);
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
   Pint_list *vlist,
   Pptco3 *ptcolrs
   )
{
   Pint i, vert;
   Pcoval result;

   glBegin(GL_POLYGON);
   for (i = 0; i < vlist->num_ints; i++) {
      vert = vlist->ints[i];
      wsgl_light_colr(ws,
                      &result,
                      refl_eqn,
                      refl_props,
                      colr_type,
                      &ptcolrs[vert].colr,
                      normal);
      wsgl_set_colr(colr_type, &result);
      glVertex3f(ptcolrs[vert].point.x,
                 ptcolrs[vert].point.y,
                 ptcolrs[vert].point.z);
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
   Pint_list *vlist,
   Pptnorm3 *ptnorms
   )
{
   Pint i, vert;
   Pcoval result;

   glBegin(GL_POLYGON);
   for (i = 0; i < vlist->num_ints; i++) {
      vert = vlist->ints[i];
      wsgl_light_colr(ws,
                      &result,
                      refl_eqn,
                      refl_props,
                      colr_type,
                      colr,
                      &ptnorms[vert].norm);
      wsgl_set_colr(colr_type, &result);
      glVertex3f(ptnorms[vert].point.x,
                 ptnorms[vert].point.y,
                 ptnorms[vert].point.z);
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
   Pint_list *vlist,
   Pptconorm3 *ptconorms
   )
{
   Pint i, vert;
   Pcoval result;

   glBegin(GL_POLYGON);
   for (i = 0; i < vlist->num_ints; i++) {
      vert = vlist->ints[i];
      wsgl_light_colr(ws,
                      &result,
                      refl_eqn,
                      refl_props,
                      colr_type,
                      &ptconorms[vert].colr,
                      &ptconorms[vert].norm);
      wsgl_set_colr(colr_type, &result);
      glVertex3f(ptconorms[vert].point.x,
                 ptconorms[vert].point.y,
                 ptconorms[vert].point.z);
   }
   glEnd();
}

/*******************************************************************************
 * wsgl_set_of_fill_area_set3_data
 *
 * DESCR:	Draw set of fill area set with data 3D
 * RETURNS:	N/A
 */

void wsgl_set_of_fill_area_set3_data(
   Ws *ws,
   void *pdata,
   Ws_attr_st *ast
   )
{
   Pint i, j;
   Psofas3 sofas3;
   Pint num_lists;
   Pint_list vlist;

   Pcoval colr, scolr;
   Pvec3 *normal;
   Pint refl_eqn;
   Prefl_props *refl_props;

   sofas3_head(&sofas3, pdata);

   glPolygonOffset(WS_FILL_AREA_OFFSET, wsgl_get_edge_width(ast));
   glEnable(GL_POLYGON_OFFSET_FILL);
   glEnable(GL_POLYGON_OFFSET_LINE);
   wsgl_setup_int_attr_nocol(ws, ast);

   switch (sofas3.vflag) {
      case PVERT_COORD:
         for (i = 0; i < sofas3.num_sets; i++) {
            refl_eqn = wsgl_get_refl_eqn(ast);
            refl_props = wsgl_get_refl_props(ast);
            priv_facet_colr(&scolr, sofas3.fflag, &sofas3.fdata, i, ast);
            normal = priv_normal3(sofas3.fflag, &sofas3.fdata, i);
            if (normal != NULL) {
               wsgl_light_colr(ws,
                               &colr,
                               refl_eqn,
                               refl_props,
                               sofas3.colr_type,
                               &scolr,
                               normal);
               wsgl_set_colr(sofas3.colr_type, &colr);
            }
            else {
               wsgl_set_colr(sofas3.colr_type, &scolr);
            }
            num_lists = *(Pint *) sofas3.vlist;
            for (j = 0; j < num_lists; j++) {
               sofas3_next_vlist(&vlist, &sofas3);
               priv_fill_area3_points(&vlist, sofas3.vdata.vertex_data.points);
            }
         }
         break;

      case PVERT_COORD_COLOUR:
         if (sofas3.fflag == PFACET_NORMAL) {
            refl_eqn = wsgl_get_refl_eqn(ast);
            refl_props = wsgl_get_refl_props(ast);
            for (i = 0; i < sofas3.num_sets; i++) {
               normal = priv_normal3(sofas3.fflag, &sofas3.fdata, i);
               num_lists = *(Pint *) sofas3.vlist;
               for (j = 0; j < num_lists; j++) {
                  sofas3_next_vlist(&vlist, &sofas3);
                  priv_fill_area3_ptcolrs_norm(ws,
                                               sofas3.colr_type,
                                               normal,
                                               refl_eqn,
                                               refl_props,
                                               &vlist,
                                               sofas3.vdata.vertex_data.ptcolrs);

               }
            }
         }
         else {
            for (i = 0; i < sofas3.num_sets; i++) {
               num_lists = *(Pint *) sofas3.vlist;
               for (j = 0; j < num_lists; j++) {
                  sofas3_next_vlist(&vlist, &sofas3);
                  priv_fill_area3_ptcolrs(sofas3.colr_type,
                                          &vlist,
                                          sofas3.vdata.vertex_data.ptcolrs);
               }
            }
         }
         break;

      case PVERT_COORD_NORMAL:
         refl_eqn = wsgl_get_refl_eqn(ast);
         refl_props = wsgl_get_refl_props(ast);
         wsgl_colr_from_gcolr(&colr, wsgl_get_int_colr(ast));
         for (i = 0; i < sofas3.num_sets; i++) {
            num_lists = *(Pint *) sofas3.vlist;
            for (j = 0; j < num_lists; j++) {
               sofas3_next_vlist(&vlist, &sofas3);
               priv_fill_area3_ptnorms(ws,
                                       sofas3.colr_type,
                                       &colr,
                                       refl_eqn,
                                       refl_props,
                                       &vlist,
                                       sofas3.vdata.vertex_data.ptnorms);
            }
         }
         break;

      case PVERT_COORD_COLOUR_NORMAL:
         refl_eqn = wsgl_get_refl_eqn(ast);
         refl_props = wsgl_get_refl_props(ast);
         for (i = 0; i < sofas3.num_sets; i++) {
            num_lists = *(Pint *) sofas3.vlist;
            for (j = 0; j < num_lists; j++) {
               sofas3_next_vlist(&vlist, &sofas3);
               priv_fill_area3_ptconorms(ws,
                                         sofas3.colr_type,
                                         refl_eqn,
                                         refl_props,
                                         &vlist,
                                         sofas3.vdata.vertex_data.ptconorms);
            }
         }
         break;

      default:
         break;
   }

   glDisable(GL_POLYGON_OFFSET_LINE);
   glDisable(GL_POLYGON_OFFSET_FILL);
}

