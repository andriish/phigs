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

/*******************************************************************************
 * default_err_buf
 *
 * DESCR:       Default error buffer function
 * RETURNS:     N/A
 */

static void default_err_buf(
   Err_handle erh,
   Pint errnum
   )
{
   fprintf(stderr, "ERR%d\n", errnum);
}

/*******************************************************************************
 * default_err_flush
 *
 * DESCR:       Default error flush function
 * RETURNS:     N/A
 */

static void default_err_flush(
   Err_handle erh
   )
{
   fflush(stderr);
}

/*******************************************************************************
 * default_err_report
 *
 * DESCR:       Default error report function
 * RETURNS:     N/A
 */

static void default_err_report(
   Err_handle erh,
   Pint errnum
   )
{
   fprintf(stderr, "ERR%d\n", errnum);
}

/*******************************************************************************
 * default_err_destroy
 *
 * DESCR:       Default error destroy function
 * RETURNS:     N/A
 */

static void default_err_destroy(
   Err_handle erh
   )
{
}

/*******************************************************************************
 * phg_erh_create
 *
 * DESCR:       Create error handler
 * RETURNS:     Pointer to handler or NULL
 */

Err_handle phg_erh_create(
   void
   )
{
   Err_handle erh;

   erh = (Err_handle) malloc(sizeof(Err_struct));
   if (erh == NULL)
      return NULL;

   if (!phg_erh_init(erh)) {
      free(erh);
      return NULL;
   }

   return erh;
}

/*******************************************************************************
 * phg_erh_init
 *
 * DESCR:       Initialize error handler
 * RETURNS:     TRUE or FALSE
 */

int phg_erh_init(
   Err_handle erh
   )
{
   erh->mode          = PERR_ON;
   erh->err_state     = PERR_ON;
   erh->cur_func_num  = 0;
   erh->buf_func      = default_err_buf;
   erh->flush_func    = default_err_flush;
   erh->report_func   = default_err_report;
   erh->destroy_func  = default_err_destroy;

   return TRUE;
}

/*******************************************************************************
 * phg_erh_destroy
 *
 * DESCR:       Initialize error handler
 * RETURNS:     TRUE or FALSE
 */

void phg_erh_destroy(
   Err_handle erh
   )
{
   if (erh != NULL)
      free(erh);
}

