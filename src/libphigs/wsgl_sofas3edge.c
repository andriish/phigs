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
 * priv_edges_points
 *
 * DESCR:	Draw edges for fill area 3D with point data
 * RETURNS:	N/A
 */

static void priv_edges_points(
   Pint eflag,
   Pedge_data_list *edata,
   Pint_list *vlist,
   Ppoint3 *points
   )
{
   Pint i, vert1, vert2;

   if (eflag == PEDGE_VISIBILITY) {
      glBegin(GL_LINES);
      for (i = 0; i < edata->num_edges - 1; i++) {
         if (edata->edgedata.edges[i] == PEDGE_ON) {
            vert1 = vlist->ints[i];
            vert2 = vlist->ints[i + 1];
            glVertex3f(points[vert1].x,
                       points[vert1].y,
                       points[vert1].z);
            glVertex3f(points[vert2].x,
                       points[vert2].y,
                       points[vert2].z);
         }
      }
      if (edata->num_edges < vlist->num_ints) {
         if (edata->edgedata.edges[i] == PEDGE_ON) {
            vert1 = vlist->ints[i];
            vert2 = vlist->ints[i + 1];
            glVertex3f(points[vert1].x,
                       points[vert1].y,
                       points[vert1].z);
            glVertex3f(points[vert2].x,
                       points[vert2].y,
                       points[vert2].z);
         }
      }
      else {
         if (edata->edgedata.edges[i] == PEDGE_ON) {
            vert1 = vlist->ints[i];
            vert2 = vlist->ints[0];
            glVertex3f(points[vert1].x,
                       points[vert1].y,
                       points[vert1].z);
            glVertex3f(points[vert2].x,
                       points[vert2].y,
                       points[vert2].z);
         }
      }
      glEnd();
   }
   else {
      glBegin(GL_LINE_LOOP);
      for (i = 0; i < vlist->num_ints; i++) {
         vert1 = vlist->ints[i];
         glVertex3f(points[vert1].x,
                    points[vert1].y,
                    points[vert1].z);
      }
      glEnd();
   }
}

/*******************************************************************************
 * priv_edges_ptcolrs
 *
 * DESCR:	Draw edges for fill area 3D with point and colour data
 * RETURNS:	N/A
 */

static void priv_edges_ptcolrs(
   Pint eflag,
   Pedge_data_list *edata,
   Pint_list *vlist,
   Pptco3 *ptcolrs
   )
{
   Pint i, vert1, vert2;

   if (eflag == PEDGE_VISIBILITY) {
      glBegin(GL_LINES);
      for (i = 0; i < edata->num_edges - 1; i++) {
         if (edata->edgedata.edges[i] == PEDGE_ON) {
            vert1 = vlist->ints[i];
            vert2 = vlist->ints[i + 1];
            glVertex3f(ptcolrs[vert1].point.x,
                       ptcolrs[vert1].point.y,
                       ptcolrs[vert1].point.z);
            glVertex3f(ptcolrs[vert2].point.x,
                       ptcolrs[vert2].point.y,
                       ptcolrs[vert2].point.z);
         }
      }
      if (edata->num_edges < vlist->num_ints) {
         if (edata->edgedata.edges[i] == PEDGE_ON) {
            vert1 = vlist->ints[i];
            vert2 = vlist->ints[i + 1];
            glVertex3f(ptcolrs[vert1].point.x,
                       ptcolrs[vert1].point.y,
                       ptcolrs[vert1].point.z);
            glVertex3f(ptcolrs[vert2].point.x,
                       ptcolrs[vert2].point.y,
                       ptcolrs[vert2].point.z);
         }
      }
      else {
         if (edata->edgedata.edges[i] == PEDGE_ON) {
            vert1 = vlist->ints[i];
            vert2 = vlist->ints[0];
            glVertex3f(ptcolrs[vert1].point.x,
                       ptcolrs[vert1].point.y,
                       ptcolrs[vert1].point.z);
            glVertex3f(ptcolrs[vert2].point.x,
                       ptcolrs[vert2].point.y,
                       ptcolrs[vert2].point.z);
         }
      }
      glEnd();
   }
   else {
      glBegin(GL_LINE_LOOP);
      for (i = 0; i < vlist->num_ints; i++) {
         vert1 = vlist->ints[i];
         glVertex3f(ptcolrs[vert1].point.x,
                    ptcolrs[vert1].point.y,
                    ptcolrs[vert1].point.z);
      }
      glEnd();
   }
}

/*******************************************************************************
 * priv_edges_ptnorms
 *
 * DESCR:	Draw edges for fill area 3D with point and normal data
 * RETURNS:	N/A
 */

static void priv_edges_ptnorms(
   Pint eflag,
   Pedge_data_list *edata,
   Pint_list *vlist,
   Pptnorm3 *ptnorms
   )
{
   Pint i, vert1, vert2;

   if (eflag == PEDGE_VISIBILITY) {
      glBegin(GL_LINES);
      for (i = 0; i < edata->num_edges - 1; i++) {
         if (edata->edgedata.edges[i] == PEDGE_ON) {
            vert1 = vlist->ints[i];
            vert2 = vlist->ints[i + 1];
            glVertex3f(ptnorms[vert1].point.x,
                       ptnorms[vert1].point.y,
                       ptnorms[vert1].point.z);
            glVertex3f(ptnorms[vert2].point.x,
                       ptnorms[vert2].point.y,
                       ptnorms[vert2].point.z);
         }
      }
      if (edata->num_edges < vlist->num_ints) {
         if (edata->edgedata.edges[i] == PEDGE_ON) {
            vert1 = vlist->ints[i];
            vert2 = vlist->ints[i + 1];
            glVertex3f(ptnorms[vert1].point.x,
                       ptnorms[vert1].point.y,
                       ptnorms[vert1].point.z);
            glVertex3f(ptnorms[vert2].point.x,
                       ptnorms[vert2].point.y,
                       ptnorms[vert2].point.z);
         }
      }
      else {
         if (edata->edgedata.edges[i] == PEDGE_ON) {
            vert1 = vlist->ints[i];
            vert2 = vlist->ints[0];
            glVertex3f(ptnorms[vert1].point.x,
                       ptnorms[vert1].point.y,
                       ptnorms[vert1].point.z);
            glVertex3f(ptnorms[vert2].point.x,
                       ptnorms[vert2].point.y,
                       ptnorms[vert2].point.z);
         }
      }
      glEnd();
   }
   else {
      glBegin(GL_LINE_LOOP);
      for (i = 0; i < vlist->num_ints; i++) {
         vert1 = vlist->ints[i];
         glVertex3f(ptnorms[vert1].point.x,
                    ptnorms[vert1].point.y,
                    ptnorms[vert1].point.z);
      }
      glEnd();
   }
}

/*******************************************************************************
 * priv_edges_ptconorms
 *
 * DESCR:	Draw edges for fill area 3D with point, colour and normal data
 * RETURNS:	N/A
 */

static void priv_edges_ptconorms(
   Pint eflag,
   Pedge_data_list *edata,
   Pint_list *vlist,
   Pptconorm3 *ptconorms
   )
{
   Pint i, vert1, vert2;

   if (eflag == PEDGE_VISIBILITY) {
      glBegin(GL_LINES);
      for (i = 0; i < edata->num_edges - 1; i++) {
         if (edata->edgedata.edges[i] == PEDGE_ON) {
            vert1 = vlist->ints[i];
            vert2 = vlist->ints[i + 1];
            glVertex3f(ptconorms[vert1].point.x,
                       ptconorms[vert1].point.y,
                       ptconorms[vert1].point.z);
            glVertex3f(ptconorms[vert2].point.x,
                       ptconorms[vert2].point.y,
                       ptconorms[vert2].point.z);
         }
      }
      if (edata->num_edges < vlist->num_ints) {
         if (edata->edgedata.edges[i] == PEDGE_ON) {
            vert1 = vlist->ints[i];
            vert2 = vlist->ints[i + 1];
            glVertex3f(ptconorms[vert1].point.x,
                       ptconorms[vert1].point.y,
                       ptconorms[vert1].point.z);
            glVertex3f(ptconorms[vert2].point.x,
                       ptconorms[vert2].point.y,
                       ptconorms[vert2].point.z);
         }
      }
      else {
         if (edata->edgedata.edges[i] == PEDGE_ON) {
            vert1 = vlist->ints[i];
            vert2 = vlist->ints[0];
            glVertex3f(ptconorms[vert1].point.x,
                       ptconorms[vert1].point.y,
                       ptconorms[vert1].point.z);
            glVertex3f(ptconorms[vert2].point.x,
                       ptconorms[vert2].point.y,
                       ptconorms[vert2].point.z);
         }
      }
      glEnd();
   }
   else {
      glBegin(GL_LINE_LOOP);
      for (i = 0; i < vlist->num_ints; i++) {
         vert1 = vlist->ints[i];
         glVertex3f(ptconorms[vert1].point.x,
                    ptconorms[vert1].point.y,
                    ptconorms[vert1].point.z);
      }
      glEnd();
   }
}

/*******************************************************************************
 * wsgl_set_of_edge_area_set3_data
 *
 * DESCR:	Draw set of fill area set edges with data 3D
 * RETURNS:	N/A
 */

void wsgl_set_of_edge_area_set3_data(
   Ws *ws,
   void *pdata,
   Ws_attr_st *ast
   )
{
   Pint i, j;
   Psofas3 sofas3;
   Pint num_lists;
   Pint_list vlist;

   sofas3_head(&sofas3, pdata);

   wsgl_setup_edge_attr(ast);
   switch (sofas3.vflag) {
      case PVERT_COORD:
         for (i = 0; i < sofas3.num_sets; i++) {
            num_lists = *(Pint *) sofas3.vlist;
            for (j = 0; j < num_lists; j++) {
               sofas3_next_vlist(&vlist, &sofas3);
               priv_edges_points(sofas3.eflag,
                                 NULL, /* TODO */
                                 &vlist,
                                 sofas3.vdata.vertex_data.points);
            }
         }
         break;

      case PVERT_COORD_COLOUR:
         for (i = 0; i < sofas3.num_sets; i++) {
            num_lists = *(Pint *) sofas3.vlist;
            for (j = 0; j < num_lists; j++) {
               sofas3_next_vlist(&vlist, &sofas3);
               priv_edges_ptcolrs(sofas3.eflag,
                                  NULL, /* TODO */
                                  &vlist,
                                  sofas3.vdata.vertex_data.ptcolrs);
            }
         }
         break;

      case PVERT_COORD_NORMAL:
         for (i = 0; i < sofas3.num_sets; i++) {
            num_lists = *(Pint *) sofas3.vlist;
            for (j = 0; j < num_lists; j++) {
               sofas3_next_vlist(&vlist, &sofas3);
               priv_edges_ptnorms(sofas3.eflag,
                                  NULL, /* TODO */
                                  &vlist,
                                  sofas3.vdata.vertex_data.ptnorms);
            }
         }
         break;

      case PVERT_COORD_COLOUR_NORMAL:
         for (i = 0; i < sofas3.num_sets; i++) {
            num_lists = *(Pint *) sofas3.vlist;
            for (j = 0; j < num_lists; j++) {
               sofas3_next_vlist(&vlist, &sofas3);
               priv_edges_ptconorms(sofas3.eflag,
                                    NULL, /* TODO */
                                    &vlist,
                                    sofas3.vdata.vertex_data.ptconorms);
            }
         }
         break;

      default:
         break;
   }
}

