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
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>
#include <phigs/util.h>
#include <phigs/private/wsxP.h>
#include <phigs/private/wsglP.h>

/*******************************************************************************
 * wsgl_fill_area
 *
 * DESCR:       Draw fill area
 * RETURNS:     N/A
 */

static void wsgl_fill_area(
   void *pdata
   )
{
   int i;
   Ppoint_list point_list;
   Pint *data = (Pint *) pdata;

   point_list.num_points = *data;
   point_list.points = (Ppoint *) &data[1];

   glBegin(GL_POLYGON);
   for (i = 0; i < point_list.num_points; i++) {
      glVertex2f(point_list.points[i].x,
                 point_list.points[i].y);
   }
   glEnd();
}

/*******************************************************************************
 * wsgl_render_fill
 *
 * DESCR:	Render fill element to current workstation rendering window
 * RETURNS:	N/A
 */

void wsgl_render_fill(
   Ws *ws,
   Ws_attr_st *ast,
   El_handle el
   )
{
   switch (el->eltype) {
      case PELEM_INDIV_ASF:
         /* Flags in asf_nameset is set in wsgl.c */

         /* TODO:
          * This is not optimal, to flush all the attributes
          * Rather check which to be flushed depending on if asf was changed
          * Maybe flags for this can set in wsgl.c and sent as argument here
          */

         /* TODO: For now only flush colour, all attributes needs to be */
         if (phg_nset_name_is_set(&ast->asf_nameset,
                                  (Pint) PASPECT_INT_COLR_IND)) {
             phg_set_gcolr(&ast->indiv_group.int_bundle.colr);
         }
         else {
             phg_set_gcolr(&ast->bundl_group.int_bundle.colr);
         }
         break;

      case PELEM_INT_IND:
         break;

      case PELEM_INT_COLR_IND:
         /* Individual ast is updated in wsgl.c */
         if (phg_nset_name_is_set(&ast->asf_nameset,
                                  (Pint) PASPECT_INT_COLR_IND)) {
             phg_set_gcolr(&ast->indiv_group.int_bundle.colr);
         }
         break;

      case PELEM_INT_COLR:
         /* Individual ast is updated in wsgl.c */
         if (phg_nset_name_is_set(&ast->asf_nameset,
                                  (Pint) PASPECT_INT_COLR_IND)) {
             phg_set_gcolr(&ast->indiv_group.int_bundle.colr);
         }
         break;

      case PELEM_INT_STYLE:
         /* TODO */
         break;

      case PELEM_INT_STYLE_IND:
         /* TODO */
         break;

      case PELEM_FILL_AREA:
         if (check_draw_primitive(ws)) {
            wsgl_fill_area(ELMT_CONTENT(el));
         }
         break;

      case PELEM_FILL_AREA_SET:
         if (check_draw_primitive(ws)) {
             /* TODO */
         }
         break;

      case PELEM_FILL_AREA3:
         if (check_draw_primitive(ws)) {
             /* TODO */
         }
         break;

      case PELEM_FILL_AREA_SET3:
         if (check_draw_primitive(ws)) {
             /* TODO */
         }
         break;

      case PELEM_FILL_AREA3_DATA:
         if (check_draw_primitive(ws)) {
             /* TODO */
         }
         break;

      default:
         break;
   }
}

