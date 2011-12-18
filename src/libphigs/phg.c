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

Err_handle erh;
Psl_handle psl;
Css_handle css;
Ws_handle  *ws_list;

/*******************************************************************************
 * popen_phigs
 *
 * DESCR:       Open phigs
 * RETURNS:     N/A
 */

void popen_phigs(char *error_file, size_t memory)
{
   erh = phg_erh_create(error_file);
   if (erh == NULL) {
      fprintf(stderr, "Error unable to create error handler\n");
      return;
   }

   psl = phg_psl_create();
   if (psl == NULL) {
      fprintf(stderr, "Error unable to create state list\n");
      return;
   }

   ws_list = malloc(sizeof(Ws_handle) * MAX_NO_OPEN_WS);
   if (ws_list == NULL) {
      phg_psl_destroy(psl);
      fprintf(stderr, "Error unable to create workstations storage\n");
      return;
   }
   memset(ws_list, 0, sizeof(Ws_handle) * MAX_NO_OPEN_WS);

   css = phg_css_init(erh, SSH_CSS);
   if (css == NULL) {
      phg_psl_destroy(psl);
      free(ws_list);
      fprintf(stderr, "Error unable to create structure storage\n");
      return;
   }

   phg_init_default_views();
}

