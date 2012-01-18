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
#include <sys/types.h>
#include <phigs/phigs.h>
#include <phigs/err.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/css.h>
#include <phigs/ws.h>
#include <phigs/private/wsxP.h>
#include <phigs/private/evtP.h>

Phg_handle phg;

/*******************************************************************************
 * add_ws_type
 *
 * DESCR:       Helper function to initialize workstation type
 * RETURNS:     TRUE or FALSE
 */

static int add_ws_type(
   Pws_cat category,
   int double_buffer
   )
{
   int status;
   Wst *wst;

   wst = phg_wstx_create(PHG_ERH, category, double_buffer);
   if (wst == NULL) {
      status = FALSE;
   }
   else {
      list_add(&PHG_WST_LIST, &wst->node);
      status = TRUE;
   }

   return status;
}

/*******************************************************************************
 * remove_ws_types
 *
 * DESCR:       Helper function to remove workstation types
 * RETURNS:     N/A
 */

static void remove_ws_types(
   void
   )
{
   Wst *wst;

   wst = (Wst *) list_get(&PHG_WST_LIST);
   while (wst != NULL) {
      phg_wst_destroy(wst);
      wst = (Wst *) list_get(&PHG_WST_LIST);
   }
}

/*******************************************************************************
 * popen_phigs
 *
 * DESCR:       Open phigs
 * RETURNS:     N/A
 */

void popen_phigs(
   char *error_file,
   size_t memory
   )
{
   Wst *wst;

   phg = (Phg_handle) malloc(sizeof(Phg_struct));
   if (phg == NULL) {
      goto abort;
   }
   memset(phg, 0, sizeof(Phg_struct));

   PHG_ERH = phg_erh_create(error_file);
   if (PHG_ERH == NULL) {
      goto abort;
   }

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_open_phigs);

   PHG_PSL = phg_psl_create();
   if (PHG_PSL == NULL) {
      ERR_REPORT(PHG_ERH, ERR900);
      goto abort;
   }

   PHG_CSS = phg_css_init(PHG_ERH, SSH_CSS);
   if (PHG_CSS == NULL) {
      ERR_REPORT(PHG_ERH, ERR900);
      phg_psl_destroy(PHG_PSL);
      goto abort;
   }

   PHG_INPUT_SEM = sem_create();
   if (PHG_INPUT_SEM == NULL) {
      ERR_REPORT(PHG_ERH, ERR900);
      phg_css_destroy(PHG_CSS);
      phg_psl_destroy(PHG_PSL);
      goto abort;
   }

   PHG_EVT_TABLE = phg_sin_evt_tbl_create(PHG_NUM_EVENTS);
   if (PHG_EVT_TABLE == NULL) {
      ERR_REPORT(PHG_ERH, ERR900);
      free(PHG_INPUT_SEM);
      phg_css_destroy(PHG_CSS);
      phg_psl_destroy(PHG_PSL);
      goto abort;
   }

   PHG_INPUT_Q = phg_sin_q_create(PHG_ERH);
   if (PHG_INPUT_Q == NULL) {
      ERR_REPORT(PHG_ERH, ERR900);
      free(PHG_INPUT_SEM);
      phg_sin_evt_tbl_destroy(PHG_EVT_TABLE);
      phg_css_destroy(PHG_CSS);
      phg_psl_destroy(PHG_PSL);
      goto abort;
   }

   /* At least one output workstation */
   list_init(&PHG_WST_LIST);
   if (!add_ws_type(PCAT_OUT, 0)) {
      ERR_REPORT(PHG_ERH, ERR900);
      free(PHG_INPUT_SEM);
      free(PHG_INPUT_Q);
      phg_sin_evt_tbl_destroy(PHG_EVT_TABLE);
      phg_css_destroy(PHG_CSS);
      phg_psl_destroy(PHG_PSL);
      goto abort;
   }

   /* Optional workstation types */
   add_ws_type(PCAT_OUT, 1);
   add_ws_type(PCAT_OUTIN, 0);
   add_ws_type(PCAT_OUTIN, 1);

   PHG_WS_LIST = (Ws_handle *) malloc(sizeof(Ws_handle) * MAX_NO_OPEN_WS);
   if (PHG_WS_LIST == NULL) {
      ERR_REPORT(PHG_ERH, ERR900);
      free(PHG_INPUT_SEM);
      free(PHG_INPUT_Q);
      remove_ws_types();
      phg_sin_evt_tbl_destroy(PHG_EVT_TABLE);
      phg_css_destroy(PHG_CSS);
      phg_psl_destroy(PHG_PSL);
      goto abort;
   }
   memset(PHG_WS_LIST, 0, sizeof(Ws_handle) * MAX_NO_OPEN_WS);

   PSL_SYS_STATE(PHG_PSL) = PSYS_ST_PHOP;

abort:
   ERR_FLUSH(PHG_ERH);
}

