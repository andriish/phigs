/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2011 Surplus Users Ham Society
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

#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/css.h>
#include <phigs/ws.h>

/*******************************************************************************
 * popen_ws
 *
 * DESCR:	Open workstation
 * RETURNS:	N/A
 */

void popen_ws(Pint ws_id, void *conn_id, Pint ws_type)
{
   Phg_args_open_ws args;
   Phg_ret ret;

   if (ws_id < 0 || ws_id > MAX_NO_OPEN_WS) {
      fprintf(stderr, "Error invalid workstation: %d\n", ws_id);
      return;
   }

   if (ws_list[ws_id] == NULL) {
      ws_list[ws_id] = phg_wsb_open_ws(&args, &ret);
      if (ws_list[ws_id] == NULL) {
         fprintf(stderr, "Error unable to create workstation: %d\n", ws_id);
         return;
      }
      ws_list[ws_id]->out_ws.model.b.cssh = css;
      ws_list[ws_id]->id = ws_id;
   }
}

/*******************************************************************************
 * ppost_struct
 *
 * DESCR:	Post structure to workstation
 * RETURNS:	N/A
 */

void ppost_struct(Pint ws_id, Pint struct_id, Pfloat priority)
{
   int status;
   Struct_handle structp;
   Css_handle cssh = ws_list[ws_id]->out_ws.model.b.cssh;

   structp = CSS_STRUCT_EXISTS(cssh, struct_id);
   if (structp != NULL) {
      (*ws_list[ws_id]->post) (ws_list[ws_id], structp, priority, 1);
      phg_css_post(cssh, struct_id, ws_list[ws_id], &status);
   }

   phg_init_default_attr();
}

