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
#include <phigs/phg.h>

/*******************************************************************************
 * input_ws_open
 *
 * DESCR:       Open input workstation desciption helper function
 * RETURNS:     Pointer to input description or NULL
 */

static Wst_input_wsdt* input_ws_open(
   Pint ws_id,
   Pint fn_id,
   Wst_phigs_dt **dtp,
   Pint *err_ind
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Wst_input_wsdt *idt = NULL;

   if (err_ind == NULL) {
      ERR_SET_CUR_FUNC(PHG_ERH, fn_id);
   }

   if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
       if (err_ind == NULL) {
          ERR_REPORT(PHG_ERH, ERR3);
       }
       else {
          *err_ind = ERR3;
       }
   }
   else {
      wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
      if (wsinfo == NULL) {
         if (err_ind == NULL) {
            ERR_REPORT(PHG_ERH, ERR54);
         }
         else {
            *err_ind = ERR54;
         }
      }
      else {
         dt = &wsinfo->wstype->desc_tbl.phigs_dt;
         if (!((dt->ws_category == PCAT_OUTIN) ||
               (dt->ws_category == PCAT_IN))) {
            if (err_ind == NULL) {
               ERR_REPORT(PHG_ERH, ERR61);
            }
            else {
               *err_ind = ERR61;
            }
         }
         else {
            idt = &wsinfo->wstype->desc_tbl.phigs_dt.in_dt;
            if (dtp != NULL) {
               *dtp = dt;
            }
         }
      }
   }

   return idt;
}

/*******************************************************************************
 * set_mode
 *
 * DESCR:       Set mode helper function
 * RETURNS:     N/A
 */

static void set_mode(
   Pint ws_id,
   Phg_args_idev_class dev_class,
   Pint dev_num,
   Pop_mode op_mode,
   Pecho_switch echo_switch
   )
{
   Ws_handle wsh;
   Phg_args_set_mode_data args;

   args.class = dev_class;
   args.dev = dev_num;
   args.mode = op_mode;
   args.echo = echo_switch;

   /* The calling function shall always check the requested workstation first */
   wsh = PHG_WSID(ws_id);
   (*wsh->set_device_mode)(wsh, &args);
}

/*******************************************************************************
 * pset_loc_mode
 *
 * DESCR:       Set locator input device mode
 * RETURNS:     N/A
 */

void pset_loc_mode(
   Pint ws_id,
   Pint loc_num,
   Pop_mode op_mode,
   Pecho_switch echo_switch
   )
{
   Wst_input_wsdt *idt;

   idt = input_ws_open(ws_id, Pfn_set_loc_mode, NULL, NULL);
   if (idt != NULL) {
      if ((loc_num > 0) &&  (loc_num <= idt->num_devs.loc)) {
         set_mode(ws_id, PHG_ARGS_INP_LOC, loc_num, op_mode, echo_switch);
      }
      else {
         ERR_REPORT(PHG_ERH, ERR250);
      }
   }
}

/*******************************************************************************
 * sample_device
 *
 * DESCR:       Sample device helper function
 * RETURNS:     N/A
 */

static void sample_device(
   Pint ws_id,
   Pint dev_num,
   Phg_args_idev_class dev_class,
   Phg_ret *ret
   )
{
   Ws_handle wsh;

   /* The calling function shall always check the requested workstation first */
   wsh = PHG_WSID(ws_id);
   (*wsh->sample_device)(wsh, dev_class, dev_num, ret);
}

/*******************************************************************************
 * psample_loc
 *
 * DESCR:       Sample locator device
 * RETURNS:     N/A
 */

void psample_loc(
   Pint ws_id,
   Pint loc_num,
   Pint *view_ind,
   Ppoint *loc_pos
   )
{
   Phg_ret ret;
   Wst_input_wsdt *idt;

   idt = input_ws_open(ws_id, Pfn_sample_loc, NULL, NULL);
   if (idt != NULL) {
      if ((loc_num > 0) &&  (loc_num <= idt->num_devs.loc)) {
         sample_device(ws_id, loc_num, PHG_ARGS_INP_LOC, &ret);
         if (ret.err == 0) {
            *view_ind = ret.data.inp_event.data.loc.view_ind;
            loc_pos->x = ret.data.inp_event.data.loc.position.x;
            loc_pos->y = ret.data.inp_event.data.loc.position.y;
         }
      }
      else {
         ERR_REPORT(PHG_ERH, ERR250);
      }
   }
}

/*******************************************************************************
 * psample_loc3
 *
 * DESCR:       Sample locator device 3D
 * RETURNS:     N/A
 */

void psample_loc3(
   Pint ws_id,
   Pint loc_num,
   Pint *view_ind,
   Ppoint3 *loc_pos
   )
{
   Phg_ret ret;
   Wst_input_wsdt *idt;

   idt = input_ws_open(ws_id, Pfn_sample_loc3, NULL, NULL);
   if (idt != NULL) {
      if ((loc_num > 0) &&  (loc_num <= idt->num_devs.loc)) {
         sample_device(ws_id, loc_num, PHG_ARGS_INP_LOC3, &ret);
         if (ret.err == 0) {
            *view_ind = ret.data.inp_event.data.loc.view_ind;
            memcpy(loc_pos,
                   &ret.data.inp_event.data.loc.position,
                   sizeof(Ppoint3));
         }
      }
      else {
         ERR_REPORT(PHG_ERH, ERR250);
      }
   }
}

