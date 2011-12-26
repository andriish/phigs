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

static char default_window_name[] = "Open PHIGS Workstation";
static char default_icon_name[]    = "Open PHIGS";

/*******************************************************************************
 * popen_ws
 *
 * DESCR:	Open workstation
 * RETURNS:	N/A
 */

void popen_ws(
   Pint ws_id,
   void *conn_id,
   Pint ws_type
   )
{
   Phg_args_open_ws args;
   Phg_ret ret;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_open_ws);

   if ((ws_id < 0) || (ws_id > MAX_NO_OPEN_WS)) {
      ERR_REPORT(PHG_ERH, ERR65);
   }
   else if (phg_psl_inq_ws_open(PHG_PSL, ws_id)) {
      ERR_REPORT(PHG_ERH, ERR53);
   }
   else if (!phg_psl_ws_free_slot(PHG_PSL)) {
      ERR_REPORT(PHG_ERH, ERR63);
   }
   else {
      memset(&args, 0, sizeof(Phg_args_open_ws));
      args.wsid = ws_id;
      args.type = PHG_WSTID(ws_type);
      args.window_name = default_window_name;
      args.icon_name = default_icon_name;

      /* Open workstation */
      PHG_WSID(ws_id) = phg_wsb_open_ws(&args, &ret);
      if (PHG_WSID(ws_id) == NULL) {
         ERR_REPORT(PHG_ERH, ERR900);
         fprintf(stderr, "Error unable to open workstation\n");
      }
      else {
         /* Add workstation to info list */
         phg_psl_add_ws(PHG_PSL, ws_id, NULL, PHG_WSTID(ws_type));

         /* Add workstation to global list of open workstations */
         PHG_WSID(ws_id)->out_ws.model.b.cssh = PHG_CSS;

         /* Store workstation id in workstation structure */
         PHG_WSID(ws_id)->id = ws_id;
      }
   }

   ERR_FLUSH(PHG_ERH);
}

/*******************************************************************************
 * pset_ws_vp
 *
 * DESCR:	Set workstation viewport
 * RETURNS:	N/A
 */

void pset_ws_vp(
   Pint ws_id,
   Plimit *viewport
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;
   Plimit3 vp;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_ws_vp);

   if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      ERR_REPORT(PHG_ERH, ERR3);
   }
   else if ((wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id)) == NULL) {
      ERR_REPORT(PHG_ERH, ERR54);
   }
   else {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (dt->ws_category == PCAT_MI) {
         ERR_REPORT(PHG_ERH, ERR57);
      }
      else if (!PHG_IN_RANGE(0.0, dt->dev_coords[0], viewport->x_min) ||
               !PHG_IN_RANGE(0.0, dt->dev_coords[0], viewport->x_max) ||
               !PHG_IN_RANGE(0.0, dt->dev_coords[1], viewport->y_min) ||
               !PHG_IN_RANGE(0.0, dt->dev_coords[1], viewport->y_max)) {
         ERR_REPORT(PHG_ERH, ERR157);
      }
      else if (!(viewport->x_min < viewport->x_max) ||
               !(viewport->y_min < viewport->y_max)) {
         ERR_REPORT(PHG_ERH, ERR152);
      }
      else {
         wsh = PHG_WSID(ws_id);
         vp.x_min = viewport->x_min;
         vp.x_max = viewport->x_max;
         vp.y_min = viewport->y_min;
         vp.y_max = viewport->y_max;
         (*wsh->set_ws_vp)(wsh, 1, &vp);
      }
   }
}

/*******************************************************************************
 * pset_ws_vp3
 *
 * DESCR:	Set workstation viewport
 * RETURNS:	N/A
 */

void pset_ws_vp3(
   Pint ws_id,
   Plimit3 *viewport
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_ws_vp3);

   if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      ERR_REPORT(PHG_ERH, ERR3);
   }
   else if ((wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id)) == NULL) {
      ERR_REPORT(PHG_ERH, ERR54);
   }
   else {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (dt->ws_category == PCAT_MI) {
         ERR_REPORT(PHG_ERH, ERR57);
      }
      else if (!PHG_IN_RANGE(0.0, dt->dev_coords[0], viewport->x_min) ||
               !PHG_IN_RANGE(0.0, dt->dev_coords[0], viewport->x_max) ||
               !PHG_IN_RANGE(0.0, dt->dev_coords[1], viewport->y_min) ||
               !PHG_IN_RANGE(0.0, dt->dev_coords[1], viewport->y_max) ||
               !PHG_IN_RANGE(0.0, dt->dev_coords[2], viewport->z_min) ||
               !PHG_IN_RANGE(0.0, dt->dev_coords[2], viewport->z_max)) {
         ERR_REPORT(PHG_ERH, ERR157);
      }
      else if (!(viewport->x_min < viewport->x_max) ||
               !(viewport->y_min < viewport->y_max) ||
               !(viewport->z_min <= viewport->z_max)) {
         ERR_REPORT(PHG_ERH, ERR152);
      }
      else {
         wsh = PHG_WSID(ws_id);
         (*wsh->set_ws_vp)(wsh, 0, viewport);
      }
   }
}

/*******************************************************************************
 * pset_ws_win
 *
 * DESCR:	Set workstation window
 * RETURNS:	N/A
 */

void pset_ws_win(
   Pint ws_id,
   Plimit *window
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;
   Plimit3 win;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_ws_win);

   if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      ERR_REPORT(PHG_ERH, ERR3);
   }
   else if ((wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id)) == NULL) {
      ERR_REPORT(PHG_ERH, ERR54);
   }
   else {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (dt->ws_category == PCAT_MI) {
         ERR_REPORT(PHG_ERH, ERR57);
      }
      else if (!PHG_IN_RANGE(PDT_NPC_XMIN, PDT_NPC_XMAX, window->x_min) ||
               !PHG_IN_RANGE(PDT_NPC_XMIN, PDT_NPC_XMAX, window->x_max) ||
               !PHG_IN_RANGE(PDT_NPC_YMIN, PDT_NPC_YMAX, window->y_min) ||
               !PHG_IN_RANGE(PDT_NPC_YMIN, PDT_NPC_YMAX, window->y_max)) {
         ERR_REPORT(PHG_ERH, ERR156);
      }
      else if (!(window->x_min < window->x_max) ||
               !(window->y_min < window->y_max)) {
         ERR_REPORT(PHG_ERH, ERR151);
      }
      else {
         wsh = PHG_WSID(ws_id);
         win.x_min = window->x_min;
         win.x_max = window->x_max;
         win.y_min = window->y_min;
         win.y_max = window->y_max;
         (*wsh->set_ws_window)(wsh, 1, &win);
      }
   }
}

/*******************************************************************************
 * pset_ws_win3
 *
 * DESCR:	Set workstation window
 * RETURNS:	N/A
 */

void pset_ws_win3(
   Pint ws_id,
   Plimit3 *window
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_ws_win3);

   if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      ERR_REPORT(PHG_ERH, ERR3);
   }
   else if ((wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id)) == NULL) {
      ERR_REPORT(PHG_ERH, ERR54);
   }
   else {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (dt->ws_category == PCAT_MI) {
         ERR_REPORT(PHG_ERH, ERR57);
      }
      else if (!PHG_IN_RANGE(PDT_NPC_XMIN, PDT_NPC_XMAX, window->x_min) ||
               !PHG_IN_RANGE(PDT_NPC_XMIN, PDT_NPC_XMAX, window->x_max) ||
               !PHG_IN_RANGE(PDT_NPC_YMIN, PDT_NPC_YMAX, window->y_min) ||
               !PHG_IN_RANGE(PDT_NPC_YMIN, PDT_NPC_YMAX, window->y_max) ||
               !PHG_IN_RANGE(PDT_NPC_ZMIN, PDT_NPC_ZMAX, window->z_min) ||
               !PHG_IN_RANGE(PDT_NPC_ZMIN, PDT_NPC_ZMAX, window->z_max)) {
         ERR_REPORT(PHG_ERH, ERR156);
      }
      else if (!(window->x_min < window->x_max) ||
               !(window->y_min < window->y_max) ||
               !(window->z_min <= window->z_max)) {
         ERR_REPORT(PHG_ERH, ERR151);
      }
      else {
         wsh = PHG_WSID(ws_id);
         (*wsh->set_ws_window)(wsh, 0, window);
      }
   }
}

/*******************************************************************************
 * ppost_struct
 *
 * DESCR:	Post structure to workstation
 * RETURNS:	N/A
 */

void ppost_struct(
   Pint ws_id,
   Pint struct_id,
   Pfloat priority
   )
{
   int status;
   Struct_handle structp;
   Css_handle cssh;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_post_struct);

   if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      ERR_REPORT(PHG_ERH, ERR3);
   }
   else if (!phg_psl_inq_ws_open(PHG_PSL, ws_id)) {
      ERR_REPORT(PHG_ERH, ERR54);
   }
   else {
      cssh = PHG_WSID(ws_id)->out_ws.model.b.cssh;
      structp = CSS_STRUCT_EXISTS(cssh, struct_id);
      if (structp == NULL) {
         ERR_REPORT(PHG_ERH, ERR201);
      }
      else {
         (*PHG_WSID(ws_id)->post) (PHG_WSID(ws_id), structp, priority, 1);
         phg_css_post(cssh, struct_id, PHG_WSID(ws_id), &status);
      }
   }
}

