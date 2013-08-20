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

/*******************************************************************************
 * wsgl_get_back_int_colr
 *
 * DESCR:       Get backface interior colur
 * RETURNS:     Pointer to interiour colour
 */

Pgcolr* wsgl_get_back_int_colr(
   Ws_attr_st *ast
   )
{
   Pgcolr *gcolr;

   if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_BACK_INT_COLR)) {
      gcolr = &ast->indiv_group.int_bundle.back_colr;
   }
   else {
      gcolr = &ast->bundl_group.int_bundle.back_colr;
   }

   return gcolr;
}

/*******************************************************************************
 * wsgl_setup_back_int_attr_nocol
 *
 * DESCR:	Setup backface interior attributes without color
 * RETURNS:	N/A
 */

void wsgl_setup_back_int_attr_nocol(
   Ws *ws,
   Ws_attr_st *ast
   )
{
   Pint_style style;
   Pint style_ind;
   Pint shad_meth;

   Wsgl_handle wsgl = ws->render_context;

   if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_BACK_INT_STYLE)) {
      style = ast->indiv_group.int_bundle.back_style;
   }
   else {
      style = ast->bundl_group.int_bundle.back_style;
   }
   if (style != wsgl->dev_st.int_style) {
      wsgl_setup_int_style(style);
      wsgl->dev_st.int_style = style;
   }

   if (phg_nset_name_is_set(&ast->asf_nameset,
                            (Pint) PASPECT_BACK_INT_STYLE_IND)) {
      style_ind = ast->indiv_group.int_bundle.back_style_ind;
   }
   else {
      style_ind = ast->bundl_group.int_bundle.back_style_ind;
   }

   if (style_ind != wsgl->dev_st.int_style_ind) {
      glPolygonStipple(wsgl_hatch_tbl[style_ind - 1]);
      wsgl->dev_st.int_style_ind = style_ind;
   }

   if (phg_nset_name_is_set(&ast->asf_nameset,
                            (Pint) PASPECT_BACK_INT_SHAD_METH)) {
      shad_meth = ast->indiv_group.int_bundle.back_shad_meth;
   }
   else {
      shad_meth = ast->bundl_group.int_bundle.back_shad_meth;
   }

   if (shad_meth != wsgl->dev_st.int_shad_meth) {
      if (shad_meth == PSD_NONE) {
         glShadeModel(GL_FLAT);
      }
      else {
         glShadeModel(GL_SMOOTH);
      }
      wsgl->dev_st.int_shad_meth = shad_meth;
   }
}

#if 0
/*******************************************************************************
 * wsgl_setup_int_refl_props
 * 
 * DESCR:       Setup surface reflection and colour properties
 * NOTES:	Make sure to enable GL_COLOR_MATERIAL before use
 * RETURNS:     N/A
 */

void wsgl_setup_int_refl_props(
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
            glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
            glColor3f(colr->direct.rgb.red   * refl_props->ambient_coef,
                      colr->direct.rgb.green * refl_props->ambient_coef,
                      colr->direct.rgb.blue  * refl_props->ambient_coef);
         }

         glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
         glColor3f(0.0, 0.0, 0.0);

         glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
         glColor3f(0.0, 0.0, 0.0);
         break;

      case PREFL_AMB_DIFF:
         if (colr_type == PMODEL_RGB) {
            glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
            glColor3f(colr->direct.rgb.red   * refl_props->ambient_coef,
                      colr->direct.rgb.green * refl_props->ambient_coef,
                      colr->direct.rgb.blue  * refl_props->ambient_coef);

            glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
            glColor3f(colr->direct.rgb.red   * refl_props->diffuse_coef,
                      colr->direct.rgb.green * refl_props->diffuse_coef,
                      colr->direct.rgb.blue  * refl_props->diffuse_coef);
         }

         glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
         glColor3f(0.0, 0.0, 0.0);
         break;

      case PREFL_AMB_DIFF_SPEC:
         if (colr_type == PMODEL_RGB) {
            glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
            glColor3f(colr->direct.rgb.red   * refl_props->ambient_coef,
                      colr->direct.rgb.green * refl_props->ambient_coef,
                      colr->direct.rgb.blue  * refl_props->ambient_coef);

            glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
            glColor3f(colr->direct.rgb.red   * refl_props->diffuse_coef,
                      colr->direct.rgb.green * refl_props->diffuse_coef,
                      colr->direct.rgb.blue  * refl_props->diffuse_coef);

            glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
            glColor3f(colr->direct.rgb.red   * refl_props->specular_coef,
                      colr->direct.rgb.green * refl_props->specular_coef,
                      colr->direct.rgb.blue  * refl_props->specular_coef);
         }
         break;

      default:
         break;
   }
}

/*******************************************************************************
 * wsgl_setup_back_int_refl_props
 * 
 * DESCR:       Setup backface surface reflection and colour properties
 * NOTES:	Make sure to enable GL_COLOR_MATERIAL before use
 * RETURNS:     N/A
 */

void wsgl_setup_back_int_refl_props(
   Pint colr_type,
   Pcoval *colr,
   Ws_attr_st *ast
   )
{
   Pint refl_eqn;
   Prefl_props *refl_props;

   if (phg_nset_name_is_set(&ast->asf_nameset,
                            (Pint) PASPECT_BACK_INT_REFL_EQN)) {
      refl_eqn = ast->indiv_group.int_bundle.back_refl_eqn;
   }
   else {
      refl_eqn = ast->bundl_group.int_bundle.back_refl_eqn;
   }

   if (phg_nset_name_is_set(&ast->asf_nameset,
                            (Pint) PASPECT_BACK_REFL_PROPS)) {
      refl_props = &ast->indiv_group.int_bundle.back_refl_props;
   }
   else {
      refl_props = &ast->bundl_group.int_bundle.back_refl_props;
   }

   switch (refl_eqn) {
      case PREFL_AMBIENT:
         if (colr_type == PMODEL_RGB) {
            glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
            glColor3f(colr->direct.rgb.red   * refl_props->ambient_coef,
                      colr->direct.rgb.green * refl_props->ambient_coef,
                      colr->direct.rgb.blue  * refl_props->ambient_coef);
         }

         glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
         glColor3f(0.0, 0.0, 0.0);

         glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
         glColor3f(0.0, 0.0, 0.0);
         break;

      case PREFL_AMB_DIFF:
         if (colr_type == PMODEL_RGB) {
            glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
            glColor3f(colr->direct.rgb.red   * refl_props->ambient_coef,
                      colr->direct.rgb.green * refl_props->ambient_coef,
                      colr->direct.rgb.blue  * refl_props->ambient_coef);

            glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
            glColor3f(colr->direct.rgb.red   * refl_props->diffuse_coef,
                      colr->direct.rgb.green * refl_props->diffuse_coef,
                      colr->direct.rgb.blue  * refl_props->diffuse_coef);
         }

         glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
         glColor3f(0.0, 0.0, 0.0);
         break;

      case PREFL_AMB_DIFF_SPEC:
         if (colr_type == PMODEL_RGB) {
            glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
            glColor3f(colr->direct.rgb.red   * refl_props->ambient_coef,
                      colr->direct.rgb.green * refl_props->ambient_coef,
                      colr->direct.rgb.blue  * refl_props->ambient_coef);

            glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
            glColor3f(colr->direct.rgb.red   * refl_props->diffuse_coef,
                      colr->direct.rgb.green * refl_props->diffuse_coef,
                      colr->direct.rgb.blue  * refl_props->diffuse_coef);

            glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
            glColor3f(colr->direct.rgb.red   * refl_props->specular_coef,
                      colr->direct.rgb.green * refl_props->specular_coef,
                      colr->direct.rgb.blue  * refl_props->specular_coef);
         }
         break;

      default:
         break;
   }
}
#endif
