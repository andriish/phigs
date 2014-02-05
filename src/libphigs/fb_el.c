/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2014 Surplus Users Ham Society
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

#include "phg.h"
#include "css.h"
#include "private/phgP.h"
#include "util/ftn.h"

/*******************************************************************************
 * ppl
 *
 * DESCR:	Creates a new element - Polyline
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(ppl)(
   FTN_INTEGER(n),
   FTN_REAL_ARRAY(pxa),
   FTN_REAL_ARRAY(pya)
   )
{
   Phg_args_add_el args;
   uint32_t i, num_points;
   uint32_t *idata;
   float32_t *fdata;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_polyline)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         num_points = (uint32_t) FTN_INTEGER_GET(n);
         args.el_type = PELEM_POLYLINE;
         args.el_size = sizeof(uint32_t) + 2 * sizeof(float32_t) * num_points;
         if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            args.el_data = PHG_SCRATCH.buf;
            idata = (uint32_t *) args.el_data;
            idata[0] = num_points;
            fdata = (float32_t *) &idata[1];
            for (i = 0; i < num_points; i++) {
               *fdata++ = (float32_t) FTN_REAL_ARRAY_GET(pxa, i);
               *fdata++ = (float32_t) FTN_REAL_ARRAY_GET(pya, i);
            }
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pspcli
 *
 * DESCR:       Creates a new element - Line Color Attribute
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(psplci)(
   FTN_INTEGER(coli)
   )
{
   Phg_args_add_el args;
   uint32_t colr_ind = (uint32_t) FTN_INTEGER_GET(coli);

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_line_colr_ind)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else if (colr_ind < 0) {
         ERR_REPORT(PHG_ERH, ERR113);
      }
      else {
         args.el_type = PELEM_LINE_COLR_IND;
         args.el_size = sizeof(uint32_t);
         if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            args.el_data = PHG_SCRATCH.buf;
            *((uint32_t *) args.el_data) = colr_ind;
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * psln
 *
 * DESCR:       Creates a new element - Line Type Attribute
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(psln)(
   FTN_INTEGER(ltype)
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_linetype)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         args.el_type = PELEM_LINETYPE;
         args.el_size = sizeof(uint32_t);
         if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            args.el_data = PHG_SCRATCH.buf;
            *((uint32_t *) args.el_data) = (uint32_t) FTN_INTEGER_GET(ltype);
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pslwsc
 *
 * DESCR:       Creates a new element - Line Width Attribute
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(pslwsc)(
   FTN_REAL(lwidth)
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_linewidth)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         args.el_type = PELEM_LINEWIDTH;
         args.el_size = sizeof(float32_t);
         if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            args.el_data = PHG_SCRATCH.buf;
            *((float32_t *) args.el_data) = (float32_t) FTN_REAL_GET(lwidth);
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * ptx
 *
 * DESCR:       Creates a new element - Text
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(ptx)(
   FTN_REAL(px),
   FTN_REAL(py),
   FTN_CHARACTER(chars)
   )
{
   Phg_args_add_el args;
   int len;
   float32_t *fdata;
   char *char_string;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_text)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         len = FTN_CHARACTER_LEN(chars);
         args.el_type = PELEM_TEXT;
         args.el_size = sizeof(Ppoint) + len + 1;
         if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            args.el_data = PHG_SCRATCH.buf;
            fdata = (float32_t *) args.el_data;
            fdata[0] = (float32_t) FTN_REAL_GET(px);
            fdata[1] = (float32_t) FTN_REAL_GET(py);
            char_string = (char *) &fdata[2];
            strncpy(char_string, FTN_CHARACTER_GET(chars), len); 
            char_string[len] = '\0';
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

/*******************************************************************************
 * pschh
 *
 * DESCR:       Creates a new element - Character height Attribute
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(pschh)(
   FTN_REAL(chh)
   )
{
   Phg_args_add_el args;

   if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_char_ht)) {
      if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
         ERR_REPORT(PHG_ERH, ERR5);
      }
      else {
         args.el_type = PELEM_CHAR_HT;
         args.el_size = sizeof(float32_t);
         if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            args.el_data = PHG_SCRATCH.buf;
            *((float32_t *) args.el_data) = (float32_t) FTN_REAL_GET(chh);
            phg_add_el(PHG_CSS, &args);
         }
      }
   }
}

