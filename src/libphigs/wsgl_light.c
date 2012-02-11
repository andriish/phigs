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
 * setup_amb_light
 *
 * DESCR:	Setup ambient light source helper function
 * RETURNS:	N/A
 */

static void setup_amb_light(
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
 * setup_dir_light
 *
 * DESCR:	Setup directional light source helper function
 * RETURNS:	N/A
 */

static void setup_dir_light(
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
 * wsgl_setup_light_src_state
 *
 * DESCR:	Setup light source state for workstation
 * RETURNS:	N/A
 */

void wsgl_setup_light_src_state(
   Ws *ws,
   Plss *lss
   )
{
   Pint i, ind, count;
   Phg_ret ret;
   Wsgl *wsgl = ws->render_context;

   count = 0;
   /* Activate light sources */
   for (i = 0; i < lss->activation.num_ints; i++) {
      ind = lss->activation.ints[i];
#ifdef DEBUG
      printf("Activate light source: %d\n", ind);
#endif
      (*ws->inq_representation)(ws,
                                ind,
                                PINQ_REALIZED,
                                PHG_ARGS_LIGHTSRCREP,
                                &ret);
      if (ret.err == 0) {
         switch (ret.data.rep.lightsrcrep.type) {
            case PLIGHT_AMBIENT:
               setup_amb_light(ind, &ret.data.rep.lightsrcrep.rec.ambient);
               count++;
               break;

            case PLIGHT_DIRECTIONAL:
               setup_dir_light(ind, &ret.data.rep.lightsrcrep.rec.directional);
               count++;
               break;

            default:
               break;
         }
      }
   }

   /* Deactivate light sources */
   for (i = 0; i < lss->deactivation.num_ints; i++) {
      ind = lss->deactivation.ints[i];
#ifdef DEBUG
      printf("Deactivate light source: %d\n", ind);
#endif
      glDisable(get_light_id(ind));
      count--;
   }

   if (count) {
      wsgl->cur_struct.lighting = TRUE;
   }
   else {
      wsgl->cur_struct.lighting = FALSE;
   }
}

