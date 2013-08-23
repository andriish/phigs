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
#include <phigs/phigs.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/css.h>
#include <phigs/ws.h>
#include <phigs/util/ftn.h>

/*******************************************************************************
 * popwk
 *
 * DESCR:	Open workstation
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(popwk)(
   FTN_INTEGER(WKID),
   FTN_INTEGER(CONID),
   FTN_INTEGER(WTYPE)
   )
{
   Wst *wst;
   Phg_args_open_ws args;
   Phg_ret ret;

   Pint ws_id = FTN_INTEGER_GET(WKID);
   /* TODO: Check what to set conn_id to */
   Phg_args_conn_info *conn_id = NULL;
   Pint ws_type = FTN_INTEGER_GET(WTYPE);

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
      wst = phg_wst_find(&PHG_WST_LIST, ws_type);

      if (wst == NULL) {
         ERR_REPORT(PHG_ERH, ERR52);
      }
      else {
         memset(&args, 0, sizeof(Phg_args_open_ws));

         if (conn_id == NULL) {
            args.conn_type = PHG_ARGS_CONN_OPEN;
         }
         else {
            args.conn_type = PHG_ARGS_CONN_DRAWABLE;
            memcpy(&args.conn_info, conn_id, sizeof(Phg_args_conn_info));
         }

         args.wsid = ws_id;
         args.type = wst;
         args.erh = PHG_ERH;
         args.cssh = PHG_CSS;
         args.memory = 8192;
         args.input_q = PHG_INPUT_Q;
         args.window_name = phg_default_window_name;
         args.icon_name = phg_default_icon_name;

         /* Open workstation */
         PHG_WSID(ws_id) = phg_wsb_open_ws(&args, &ret);
         if (PHG_WSID(ws_id) == NULL) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            /* Add workstation to info list */
            phg_psl_add_ws(PHG_PSL, ws_id, NULL, wst);
         }
      }
   }

   ERR_FLUSH(PHG_ERH);
}

/*******************************************************************************
 * ppost
 *
 * DESCR:	Post structure to workstation
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(ppost)(
   FTN_INTEGER(WKID),
   FTN_INTEGER(STRID),
   FTN_REAL(PRIORT)
   )
{
   int status;
   Ws_handle wsh;
   Css_handle cssh;
   Struct_handle structp;

   Pint ws_id = FTN_INTEGER_GET(WKID);
   Pint struct_id = FTN_INTEGER_GET(STRID);
   Pfloat priority = FTN_REAL_GET(PRIORT);

   if (phg_ws_open(ws_id, Pfn_post_struct) != NULL) {
      wsh = PHG_WSID(ws_id);
      cssh = wsh->out_ws.model.b.cssh;
      structp = phg_css_post(cssh, struct_id, wsh, &status);
      if (structp != NULL) {
         (*wsh->post)(wsh, structp, priority, !status);
      }
   }
}

