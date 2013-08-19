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
 * RETURNS:     N/A
 */

void wsgl_surface_colr_props(
   Pint colr_model,
   Pcoval *colr,
   Ws_attr_st *ast
   )
{
   Pint refl_eqn;
   Prefl_props *refl_props;
   GLfloat ambient[4];
   GLfloat diffuse[4];
   GLfloat specular[4];

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
         if (colr_model == PMODEL_RGB) {
            ambient[0] = colr->direct.rgb.red   * refl_props->ambient_coef;
            ambient[1] = colr->direct.rgb.green * refl_props->ambient_coef;
            ambient[2] = colr->direct.rgb.blue  * refl_props->ambient_coef;
            ambient[3] = 1.0;

            diffuse[0] = 0.0;
            diffuse[1] = 0.0;
            diffuse[2] = 0.0;
            diffuse[3] = 1.0;

            specular[0] = 0.0;
            specular[1] = 0.0;
            specular[2] = 0.0;
            specular[3] = 1.0;
         }
         break;

      case PREFL_AMB_DIFF:
         if (colr_model == PMODEL_RGB) {
            ambient[0] = colr->direct.rgb.red   * refl_props->ambient_coef;
            ambient[1] = colr->direct.rgb.green * refl_props->ambient_coef;
            ambient[2] = colr->direct.rgb.blue  * refl_props->ambient_coef;
            ambient[3] = 1.0;

            diffuse[0] = colr->direct.rgb.red   * refl_props->diffuse_coef;
            diffuse[1] = colr->direct.rgb.green * refl_props->diffuse_coef;
            diffuse[2] = colr->direct.rgb.blue  * refl_props->diffuse_coef;
            diffuse[3] = 1.0;

            specular[0] = 0.0;
            specular[1] = 0.0;
            specular[2] = 0.0;
            specular[3] = 1.0;
         }
         break;

      case PREFL_AMB_DIFF_SPEC:
         if (colr_model == PMODEL_RGB) {
            ambient[0] = colr->direct.rgb.red   * refl_props->ambient_coef;
            ambient[1] = colr->direct.rgb.green * refl_props->ambient_coef;
            ambient[2] = colr->direct.rgb.blue  * refl_props->ambient_coef;
            ambient[3] = 1.0;

            diffuse[0] = colr->direct.rgb.red   * refl_props->diffuse_coef;
            diffuse[1] = colr->direct.rgb.green * refl_props->diffuse_coef;
            diffuse[2] = colr->direct.rgb.blue  * refl_props->diffuse_coef;
            diffuse[3] = 1.0;

            specular[0] = colr->direct.rgb.red   * refl_props->specular_coef;
            specular[1] = colr->direct.rgb.green * refl_props->specular_coef;
            specular[2] = colr->direct.rgb.blue  * refl_props->specular_coef;
            specular[3] = 1.0;
         }
         break;

      default:
         break;
   }

   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
}

