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
#include <GL/gl.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>
#include <phigs/util.h>
#include <phigs/private/wsxP.h>
#include <phigs/private/wsglP.h>

/*******************************************************************************
 * add_amb_light
 *
 * DESCR:	Add ambient light helper function
 * RETURNS:	N/A
 */

static void add_amb_light(
   Pcoval *result,
   Pint colr_type,
   Pint refl_eqn,
   Prefl_props *props,
   Pamb_light_src_rec *rec
   )
{
   if (refl_eqn >= PREFL_AMBIENT) {
      result->direct.rgb.red += props->ambient_coef *
                          rec->colr.val.general.x;
      result->direct.rgb.green += props->ambient_coef *
                          rec->colr.val.general.y;
      result->direct.rgb.blue += props->ambient_coef *
                          rec->colr.val.general.z;
   }
}

/*******************************************************************************
 * add_dir_light
 *
 * DESCR:	Add directional light helper function
 * RETURNS:	N/A
 */

static void add_dir_light(
   Pcoval *result,
   Pint colr_type,
   Pvec3 *normal,
   Pint refl_eqn,
   Prefl_props *props,
   Pdir_light_src_rec *rec
   )
{
   Pfloat dp = rec->dir.delta_x * normal->delta_x +
               rec->dir.delta_y * normal->delta_y +
               rec->dir.delta_z * normal->delta_z;

   if (refl_eqn >= PREFL_AMB_DIFF) {
      result->direct.rgb.red += dp * props->diffuse_coef *
                          rec->colr.val.general.x;
      result->direct.rgb.green += dp * props->diffuse_coef *
                          rec->colr.val.general.y;
      result->direct.rgb.blue += dp * props->diffuse_coef *
                          rec->colr.val.general.z;
   }
}

/*******************************************************************************
 * wsgl_light_colr
 *
 * DESCR:	Perform lighting calculations and set colour for vertex
 * RETURNS:	N/A
 */

void wsgl_light_colr(
   Ws *ws,
   Pcoval *result,
   Pint refl_eqn,
   Prefl_props *refl_props,
   Pint colr_type,
   Pcoval *colr,
   Pvec3 *normal
   )
{
   int i;
   Phg_ret ret;
   Pvec3 norm;
   Ppoint3 pt, pn;
   Wsgl *wsgl = ws->render_context;

   pt.x = normal->delta_x;
   pt.y = normal->delta_y;
   pt.z = normal->delta_z;

   phg_tranpt3(&pt, wsgl->model_tran, &pn);

   norm.delta_x = pn.x;
   norm.delta_y = pn.y;
   norm.delta_z = pn.z;

   result->direct.rgb.red = 0.0;
   result->direct.rgb.green = 0.0;
   result->direct.rgb.blue = 0.0;

   /* Process light sources */
   for (i = 0; i < WS_MAX_LIGHT_SRC; i++) {
      if (phg_nset_name_is_set(&wsgl->cur_struct.lightstat, i)) {
         (*ws->inq_representation)(ws,
                                   i,
                                   PINQ_REALIZED,
                                   PHG_ARGS_LIGHTSRCREP,
                                   &ret);
         if (ret.err == 0) {
            switch (ret.data.rep.lightsrcrep.type) {
               case PLIGHT_AMBIENT:
                  add_amb_light(result, colr_type, refl_eqn, refl_props,
                                &ret.data.rep.lightsrcrep.rec.ambient);
                  break;

               case PLIGHT_DIRECTIONAL:
                  add_dir_light(result, colr_type, &norm, refl_eqn, refl_props,
                                &ret.data.rep.lightsrcrep.rec.directional);
                  break;
            }
         }
      }
   }

   result->direct.rgb.red *= colr->direct.rgb.red;
   result->direct.rgb.green *= colr->direct.rgb.green;
   result->direct.rgb.blue *= colr->direct.rgb.blue;

   if (result->direct.rgb.red > 1.0) {
      result->direct.rgb.red = 1.0;
   }
   if (result->direct.rgb.green > 1.0) {
      result->direct.rgb.green = 1.0;
   }
   if (result->direct.rgb.blue > 1.0) {
      result->direct.rgb.blue = 1.0;
   }
}

/*******************************************************************************
 * get_light_id
 *
 * DESCR:	Get light id helper function
 * RETURNS:	Light source idientifier
 */

static GLuint get_light_id(
   Pint ind
   )
{
   GLuint id;

   switch (ind) {
      case 1:  id = GL_LIGHT1; break;
      case 2:  id = GL_LIGHT2; break;
      case 3:  id = GL_LIGHT3; break;
      case 4:  id = GL_LIGHT4; break;
      case 5:  id = GL_LIGHT5; break;
      case 6:  id = GL_LIGHT6; break;
      case 7:  id = GL_LIGHT7; break;
      default: id = GL_LIGHT0; break;
   }

   return id;
}

/*******************************************************************************
 * setup_ambient_light
 *
 * DESCR:	Setup ambient light source helper function
 * RETURNS:	N/A
 */

static void setup_ambient_light(
   Pint ind,
   Pamb_light_src_rec *rec
   )
{
   GLfloat amb[4];
   GLuint id;

   amb[0] = rec->colr.val.general.x;
   amb[1] = rec->colr.val.general.y;
   amb[2] = rec->colr.val.general.z;
   amb[3] = 1.0;

#ifdef DEBUG
   printf("Ambient light: %f %f %f\n", amb[0], amb[1], amb[2]);
#endif

   id = get_light_id(ind);
   glLightfv(id, GL_AMBIENT, amb);
   glEnable(id);
}

/*******************************************************************************
 * setup_directional_light
 *
 * DESCR:	Setup directional light source helper function
 * RETURNS:	N/A
 */

static void setup_directional_light(
   Pint ind,
   Pdir_light_src_rec *rec
   )
{
   GLfloat dif[4];
   GLfloat pos[4];
   GLuint id;

   dif[0] = rec->colr.val.general.x;
   dif[1] = rec->colr.val.general.y;
   dif[2] = rec->colr.val.general.z;
   dif[3] = 1.0;

   pos[0] = rec->dir.delta_x;
   pos[1] = rec->dir.delta_y;
   pos[2] = rec->dir.delta_z;
   pos[3] = 0.0;

#ifdef DEBUG
   printf("Directional light: %f %f %f @(%f, %f %f)\n",
          dif[0], dif[1], dif[2],
          pos[0], pos[1], pos[2]);
#endif

   id = get_light_id(ind);
   glLightfv(id, GL_DIFFUSE, dif);
   glLightfv(id, GL_POSITION, pos);
   glEnable(id);
}

/*******************************************************************************
 * wsgl_update_light_src_state
 *
 * DESCR:	Update light source state for workstation
 * RETURNS:	N/A
 */

void wsgl_update_light_src_state(
   Ws *ws
   )
{
   Pint i;
   Phg_ret ret;
   Wsgl *wsgl = ws->render_context;

   glPushMatrix();
   glLoadIdentity();

   /* Activate light sources */
   for (i = 0; i < WS_MAX_LIGHT_SRC; i++) {
      if (phg_nset_name_is_set(&wsgl->cur_struct.lightstat, i)) {
#ifdef DEBUG
         printf("Setup light source: %d\n", i);
#endif
         (*ws->inq_representation)(ws,
                                   i,
                                   PINQ_REALIZED,
                                   PHG_ARGS_LIGHTSRCREP,
                                   &ret);
         if (ret.err == 0) {
            switch (ret.data.rep.lightsrcrep.type) {
               case PLIGHT_AMBIENT:
                  setup_ambient_light(i, &ret.data.rep.lightsrcrep.rec.ambient);
                  break;

               case PLIGHT_DIRECTIONAL:
                  setup_directional_light(i, &ret.data.rep.lightsrcrep.rec.directional);
                  break;

               default:
                  break;
            }
         }
      }
      else {
         glDisable(get_light_id(i));
      }
   }

   glPopMatrix();
}

/*******************************************************************************
 * wsgl_set_light_src_state
 *
 * DESCR:	Set light source state for workstation
 * RETURNS:	N/A
 */

void wsgl_set_light_src_state(
   Ws *ws,
   void *pdata
   )
{
   Pint i, num_ints;
   Pint *data;
   data = (Pint *) pdata;
   Wsgl *wsgl = ws->render_context;

   num_ints = *data;
   data++;
   for (i = 0; i < num_ints; i++) {
      phg_nset_name_set(&wsgl->cur_struct.lightstat, *data);
      data++;
   }

   num_ints = *data;
   data++;
   for (i = 0; i < num_ints; i++) {
      phg_nset_name_clear(&wsgl->cur_struct.lightstat, *data);
      data++;
   }

   if (phg_nset_names_is_empty_all(&wsgl->cur_struct.lightstat)) {
      wsgl->cur_struct.lighting = FALSE;
   }
   else {
      wsgl->cur_struct.lighting = TRUE;
   }

#ifdef DEBUG
   printf("Lighting nameset: ");
   phg_nset_print(&wsgl->cur_struct.lightstat);
   printf("Lighting is %s\n", (wsgl->cur_struct.lighting) ? "On" : "Off");
#endif

   wsgl_update_light_src_state(ws);
}

