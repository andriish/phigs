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

#include <phigs/phigs.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/css.h>
#include <phigs/ws.h>

Pint phigs_ws_type_glx_drawable = 0;

static Wst ws_types[] = {
   {
      WST_BASE_TYPE_GLX_DRAWABLE
   }
};

static char default_window_name[] = "Open PHIGS Workstation";
static char default_icon_name[]    = "Open PHIGS";

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

   ERR_SET_CUR_FUNC(erh, Pfn_open_ws);

   if ((ws_id < 0) || (ws_id > MAX_NO_OPEN_WS)) {
      ERR_REPORT(erh, ERR63);
   }
   else if (phg_psl_inq_ws_open(psl, ws_id)) {
      ERR_REPORT(erh, ERR53);
   }
   else if (!phg_psl_ws_free_slot(psl)) {
      ERR_REPORT(erh, ERR63);
   }
   else {
      memset(&args, 0, sizeof(Phg_args_open_ws));
      args.wsid = ws_id;
      args.type = &ws_types[ws_type];
      args.window_name = default_window_name;
      args.icon_name = default_icon_name;

      /* Open workstation */
      ws_list[ws_id] = phg_wsb_open_ws(&args, &ret);
      if (ws_list[ws_id] == NULL) {
         ERR_REPORT(erh, ERR900);
         fprintf(stderr, "Error unable to open workstation\n");
      }
      else {
         /* Add workstation to info list */
         phg_psl_add_ws(psl, ws_id, NULL, &ws_types[ws_type]);

         /* Add workstation to global list of open workstations */
         ws_list[ws_id]->out_ws.model.b.cssh = css;

         /* Store workstation id in workstation structure */
         ws_list[ws_id]->id = ws_id;
      }
   }

   ERR_FLUSH(erh);
}

/*******************************************************************************
 * pset_ws_vp
 *
 * DESCR:	Set workstation viewport
 * RETURNS:	N/A
 */

void pset_ws_vp(Pint ws_id, Plimit *viewport)
{
   Plimit3 vp;
   Ws_handle wsh = ws_list[ws_id];

   vp.x_min = viewport->x_min;
   vp.x_max = viewport->x_max;
   vp.y_min = viewport->y_min;
   vp.y_max = viewport->y_max;

   (*wsh->set_ws_vp)(wsh, 1, &vp);
}

/*******************************************************************************
 * pset_ws_vp3
 *
 * DESCR:	Set workstation viewport
 * RETURNS:	N/A
 */

void pset_ws_vp3(Pint ws_id, Plimit3 *viewport)
{
   Ws_handle wsh = ws_list[ws_id];

   (*wsh->set_ws_vp)(wsh, 0, viewport);
}

/*******************************************************************************
 * pset_ws_win
 *
 * DESCR:	Set workstation window
 * RETURNS:	N/A
 */

void pset_ws_win(Pint ws_id, Plimit *window)
{
   Plimit3 win;
   Ws_handle wsh = ws_list[ws_id];

   win.x_min = window->x_min;
   win.x_max = window->x_max;
   win.y_min = window->y_min;
   win.y_max = window->y_max;

   (*wsh->set_ws_window)(wsh, 1, &win);
}

/*******************************************************************************
 * pset_ws_win3
 *
 * DESCR:	Set workstation window
 * RETURNS:	N/A
 */

void pset_ws_win3(Pint ws_id, Plimit3 *window)
{
   Ws_handle wsh = ws_list[ws_id];

   (*wsh->set_ws_window)(wsh, 0, window);
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

