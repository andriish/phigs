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
 * wsgl_surface_colr_props
 * 
 * DESCR:       Setup surface reflection and colour properties
 * NOTES:	Make sure to enable GL_COLOR_MATERIAL before use
 * RETURNS:     N/A
 */

void wsgl_surface_colr_props(
   Pint colr_type,
   Pcoval *colr,
   Ws_attr_st *ast
   )
{
   Pint refl_eqn;
   Prefl_props *refl_props;

   if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_INT_REFL_EQN)) {
      refl_eqn = ast->indiv_group.int_bundle.refl_eqn;
   }
   else {
      refl_eqn = ast->bundl_group.int_bundle.refl_eqn;
   }

   if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_REFL_PROPS)) {
      refl_props = &ast->indiv_group.int_bundle.refl_props;
   }
   else {
      refl_props = &ast->bundl_group.int_bundle.refl_props;
   }

   switch (refl_eqn) {
      case PREFL_AMBIENT:
         if (colr_type == PMODEL_RGB) {
            glColorMaterial(GL_FRONT, GL_AMBIENT);
            glColor3f(colr->direct.rgb.red   * refl_props->ambient_coef,
                      colr->direct.rgb.green * refl_props->ambient_coef,
                      colr->direct.rgb.blue  * refl_props->ambient_coef);
         }

         glColorMaterial(GL_FRONT, GL_DIFFUSE);
         glColor3f(0.0, 0.0, 0.0);

         glColorMaterial(GL_FRONT, GL_SPECULAR);
         glColor3f(0.0, 0.0, 0.0);
         break;

      case PREFL_AMB_DIFF:
         if (colr_type == PMODEL_RGB) {
            glColorMaterial(GL_FRONT, GL_AMBIENT);
            glColor3f(colr->direct.rgb.red   * refl_props->ambient_coef,
                      colr->direct.rgb.green * refl_props->ambient_coef,
                      colr->direct.rgb.blue  * refl_props->ambient_coef);

            glColorMaterial(GL_FRONT, GL_DIFFUSE);
            glColor3f(colr->direct.rgb.red   * refl_props->diffuse_coef,
                      colr->direct.rgb.green * refl_props->diffuse_coef,
                      colr->direct.rgb.blue  * refl_props->diffuse_coef);
         }

         glColorMaterial(GL_FRONT, GL_SPECULAR);
         glColor3f(0.0, 0.0, 0.0);
         break;

      case PREFL_AMB_DIFF_SPEC:
         if (colr_type == PMODEL_RGB) {

            glColorMaterial(GL_FRONT, GL_AMBIENT);
            glColor3f(colr->direct.rgb.red   * refl_props->ambient_coef,
                      colr->direct.rgb.green * refl_props->ambient_coef,
                      colr->direct.rgb.blue  * refl_props->ambient_coef);

            glColorMaterial(GL_FRONT, GL_DIFFUSE);
            glColor3f(colr->direct.rgb.red   * refl_props->diffuse_coef,
                      colr->direct.rgb.green * refl_props->diffuse_coef,
                      colr->direct.rgb.blue  * refl_props->diffuse_coef);

            glColorMaterial(GL_FRONT, GL_SPECULAR);
            glColor3f(colr->direct.rgb.red   * refl_props->specular_coef,
                      colr->direct.rgb.green * refl_props->specular_coef,
                      colr->direct.rgb.blue  * refl_props->specular_coef);
         }
         break;

      default:
         break;
   }
}

