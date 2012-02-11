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

#ifdef TODO
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
 * setup_amb_light_src
 *
 * DESCR:	Setup ambient light source helper function
 * RETURNS:	N/A
 */

static void setup_amb_light_src(
   Ws *ws,
   Pint ind
   )
{
   Wsgl_handle wsgl = ws->render_context;
   GLfloat amb[4];
   GLuint id;

   amb[0] = wsgl->cur_light[ind].rec.ambient.colour.direct.x;
   amb[1] = wsgl->cur_light[ind].rec.ambient.colour.direct.y;
   amb[2] = wsgl->cur_light[ind].rec.ambient.colour.direct.z;
   amb[3] = 1.0;

   id = get_light_id(ind);
   glLightfv(id, GL_AMBIENT, amb);
}
#endif

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
   Pint i, ind;

   /* Deactivate light sources */
   for (i = 0; i < lss->deactivation.num_ints; i++) {
      ind = lss->deactivation.ints[i];
#ifdef DEBUG
      printf("Deactivate light source: %d\n", ind);
#endif
   }

   /* Activate light sources */
   for (i = 0; i < lss->activation.num_ints; i++) {
      ind = lss->activation.ints[i];
#ifdef DEBUG
      printf("Activate light source: %d\n", ind);
#endif
   }
}

