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
#include <sys/types.h>
#include <phigs/phigs.h>
#include <phigs/err.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/css.h>
#include <phigs/ws.h>
#include <phigs/private/wsglP.h>

Phg_handle phg;

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
   phg = (Phg_handle) malloc(sizeof(Phg_struct));
   if (phg == NULL) {
      goto abort;
   }

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

   PHG_WSTID(phigs_ws_type_glx_drawable) = wsx_gl_create();
   if (PHG_WSTID(phigs_ws_type_glx_drawable) == NULL) {
      ERR_REPORT(PHG_ERH, ERR900);
      phg_css_destroy(PHG_CSS);
      phg_psl_destroy(PHG_PSL);
   }

   PHG_WS_LIST = (Ws_handle *) malloc(sizeof(Ws_handle) * MAX_NO_OPEN_WS);
   if (PHG_WS_LIST == NULL) {
      ERR_REPORT(PHG_ERH, ERR900);
      phg_wst_destroy(PHG_WSTID(phigs_ws_type_glx_drawable));
      phg_css_destroy(PHG_CSS);
      phg_psl_destroy(PHG_PSL);
      goto abort;
   }
   memset(PHG_WS_LIST, 0, sizeof(Ws_handle) * MAX_NO_OPEN_WS);

   phg_init_default_views();

   PSL_SYS_STATE(PHG_PSL) = PSYS_ST_PHOP;

abort:
   ERR_FLUSH(PHG_ERH);
}

