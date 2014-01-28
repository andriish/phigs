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
#include <phigs/css.h>
#include <phigs/ar.h>
#include <phigs/private/phgP.h>
 
/*******************************************************************************
 * valid_ar_fname
 *
 * DESCR:       Check archive filename helper function
 * RETURNS:     Non-zero on success, otherwise zero
 */

static int valid_ar_fname(
   char *fname
   )
{
   int status = 0;

   if (fname != NULL && strlen(fname) <= PHG_MAX_NAMELEN) {
      status = 1;
   }

   return status;
}

/*******************************************************************************
 * popen_ar_file
 *
 * DESCR:       Open archive file
 * RETURNS:     N/A
 */

void popen_ar_file(
   Pint archive_id,
   char *archive_file
   )
{
   Phg_args_ar_open args;
   Phg_ret ret;
   char *fname;

   if (phg_entry_check(ERR2, Pfn_open_ar_file)) {
      if (phg_psl_inq_ar_open(PHG_PSL, archive_id)) {
         ERR_REPORT(PHG_ERH, ERR402);
      }
      else if (!valid_ar_fname(archive_file)) {
         ERR_REPORT(PHG_ERH, ERR400);
      }
      else if (!phg_psl_ar_free_slot(PHG_PSL)) {
         ERR_REPORT(PHG_ERH, ERR401);
      }
      else {
         fname = (char *) malloc(strlen(archive_file) + 1);
         if (fname == NULL) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            args.arid = archive_id;
            args.fname = archive_file;
            args.name_length = strlen(archive_file) + 1;
            phg_ar_open(&args, &ret);
            if (ret.err) {
               free(fname);
            }
            else {
               strcpy(fname, archive_file);
               phg_psl_add_ar(PHG_PSL, archive_id, fname);
               PSL_AR_STATE(PHG_PSL) = PST_AROP;
            }

            ERR_FLUSH(PHG_ERH);
         }
      }
   }
}

/*******************************************************************************
 * pclose_ar_file
 *
 * DESCR:       Close archive file
 * RETURNS:     N/A
 */

void pclose_ar_file(
   Pint archive_id
   )
{
   if (phg_entry_check(ERR7, Pfn_close_ar_file)) {
      if (PSL_AR_STATE(PHG_PSL) != PST_AROP) {
         ERR_REPORT(PHG_ERH, ERR7);
      }
      else if (!phg_psl_inq_ar_open(PHG_PSL, archive_id)) {
         ERR_REPORT(PHG_ERH, ERR404);
      }
      else {
         phg_ar_close(archive_id);
         phg_psl_rem_ar(PHG_PSL, archive_id);
      }
   }
}

/*******************************************************************************
 * par_all_structs
 *
 * DESCR:       Store all structures in archive
 * RETURNS:     N/A
 */

void par_all_structs(
   Pint archive_id
   )
{
   Phg_args_ar_info args;

   if (phg_entry_check(ERR7, Pfn_ar_all_structs)) {
      if (PSL_AR_STATE(PHG_PSL) != PST_AROP) {
         ERR_REPORT(PHG_ERH, ERR7);
      }
      else if (!phg_psl_inq_ar_open(PHG_PSL, archive_id)) {
         ERR_REPORT(PHG_ERH, ERR404);
      }
      else {
         args.arid = archive_id;
         args.op = PHG_ARGS_AR_ALL;
         args.resflag = PSL_ARCHIVE_CONFLICT(PHG_PSL);
         phg_ar_archive(&args);
      }
   }
}

