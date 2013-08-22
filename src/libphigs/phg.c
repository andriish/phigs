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

#include <stdlib.h>
#include <string.h>
#include <phigs/phg.h>
#include <phigs/css.h>
#include <phigs/private/phgP.h>

char phg_default_window_name[] = "Open PHIGS Workstation";
char phg_default_icon_name[]   = "Open PHIGS";
Phg_handle phg = NULL;

/*******************************************************************************
 * phg_int_in_list
 *
 * DESCR:       Find out if integer values is stored in list
 * RETURNS:     TRUE or FALSE
 */

int phg_int_in_list(
   Pint v,
   Pint num,
   Pint *list
   )
{
   Pint i;
   int status = FALSE;

   for (i = 0; i < num; i++) {
      if (list[i] == v) {
         status = TRUE;
         break;
      }
   }

   return status;
}

/*******************************************************************************
 * phg_add_el
 *
 * DESCR:	Add an element and update workstations posted to
 * RETURNS:	N/A
 */

void phg_add_el(
   Css_handle cssh,
   Phg_args_add_el *args
   )
{
   Css_ws_list ws_list;

   ws_list = CSS_GET_WS_ON(CSS_CUR_STRUCTP(cssh));

   if (phg_css_add_elem(cssh, args)) {
      if (ws_list != NULL) {
         for (; ws_list->wsh; ws_list++)
            (*ws_list->wsh->add_el)(ws_list->wsh);
      }
   }
}

/*******************************************************************************
 * phg_get_colr_ind
 *
 * DESCR:       Get colour from index
 * RETURNS:     N/A
 */

void phg_get_colr_ind(
   Ws *ws,
   Pgcolr *gcolr,
   Pint ind
   )
{
   Phg_ret ret;

   gcolr->type = ws->current_colour_model;

   if (ws->current_colour_model == PINDIRECT) {
      gcolr->val.ind = ind;
   }
   else {
      (*ws->inq_representation)(ws,
                                ind,
                                PINQ_REALIZED,
                                PHG_ARGS_COREP,
                                &ret);
      if (ret.err == 0) {

            gcolr->val.general.x = ret.data.rep.corep.rgb.red;
            gcolr->val.general.y = ret.data.rep.corep.rgb.green;
            gcolr->val.general.z = ret.data.rep.corep.rgb.blue;
      }
   }
}

/*******************************************************************************
 * phg_ws_open
 *
 * DESCR:	Helper function to get workstation information
 * RETURNS:	N/A
 */

Psl_ws_info* phg_ws_open(
   Pint ws_id,
   Pint fn_id
   )
{
   Psl_ws_info *wsinfo = NULL;

   ERR_SET_CUR_FUNC(PHG_ERH, fn_id);

   if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      ERR_REPORT(PHG_ERH, ERR3);
   }
   else {
      wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
      if (wsinfo == NULL) {
         ERR_REPORT(PHG_ERH, ERR54);
      }
   }

   return wsinfo;
}

/*******************************************************************************
 * phg_set_ws_filter
 *
 * DESCR:       Set workstation filter
 * RETURNS:     N/A
 */

void phg_set_ws_filter(
   Pint ws_id,
   Pint fn_id,
   Phg_args_flt_type type,
   Pfilter *filter
   )
{
   Psl_ws_info *wsinfo;
   Ws_handle wsh;

   wsinfo = phg_ws_open(ws_id, fn_id);
   if (wsinfo != NULL) {
      switch (wsinfo->wstype->desc_tbl.phigs_dt.ws_category) {
         case PCAT_OUTIN:
         case PCAT_OUT:
         case PCAT_MO:
            wsh = PHG_WSID(ws_id);
            (*wsh->set_filter)(wsh,
                               type,
                               1,
                               &filter->incl_set,
                               &filter->excl_set);
            break;

         default:
            ERR_REPORT(PHG_ERH, ERR59);
            break;
      }
   }
}

/*******************************************************************************
 * phg_echo_limits_valid
 *
 * DESCR:       Helper function to check if echo volume limits are valid
 * RETURNS:     TRUE or FALSE
 */

int phg_echo_limits_valid(
   Pint ws_id,
   Pint fn_id,
   Plimit3 *e_volume,
   Wst_phigs_dt *dt
   )
{
   int status = FALSE;

   phg_psl_get_ws_info(PHG_PSL, ws_id);
   switch(fn_id) {
      case Pfn_init_loc3:
      case Pfn_init_stroke3:
      case Pfn_init_pick3:
      case Pfn_init_val3:
      case Pfn_init_choice3:
      case Pfn_init_string3:
      case Pfn_escape:
         if (!PHG_ECHO_VOLUME_VALID(e_volume)) {
            ERR_REPORT(PHG_ERH, ERR254);
         }
         else if (!PHG_ECHO_VOLUME_IN_RANGE(e_volume, dt->dev_coords)) {
            ERR_REPORT(PHG_ERH, ERR255);
         }
         else {
            status = TRUE;
         }
         break;


      case Pfn_init_loc:
      case Pfn_init_stroke:
      case Pfn_init_pick:
      case Pfn_init_val:
      case Pfn_init_choice:
      case Pfn_init_string:
         if (!PHG_ECHO_AREA_VALID(e_volume)) {
            ERR_REPORT(PHG_ERH, ERR254);
         }
         else if (!PHG_ECHO_AREA_IN_RANGE(e_volume, dt->dev_coords)) {
            ERR_REPORT(PHG_ERH, ERR255);
         }
         else {
            status = TRUE;
         }
         break;
   }

   return status;
}

