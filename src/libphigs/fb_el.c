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
#include <phigs/util/ftn.h>

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
   Pint i;
   unsigned size;
   Phg_args_add_el args;

   Ppoint_list point_list;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_polyline);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      point_list.num_points = FTN_INTEGER_GET(n);
      size = sizeof(Ppoint) * point_list.num_points;
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, size)) {
         ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
         point_list.points = (Ppoint *) PHG_SCRATCH.buf;
         for (i = 0; i < point_list.num_points; i++) {
            point_list.points[i].x = FTN_REAL_ARRAY_GET(pxa, i);
            point_list.points[i].y = FTN_REAL_ARRAY_GET(pya, i);
         }
         ARGS_ELMT_TYPE(&args) = PELEM_POLYLINE;
         ARGS_ELMT_SIZE(&args) = sizeof(Pint) + size;
         ARGS_ELMT_DATA(&args).point_list.num_points = point_list.num_points;
         ARGS_ELMT_DATA(&args).point_list.points = point_list.points;
         phg_add_el(PHG_CSS, &args);
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

   Pint colr_ind = FTN_INTEGER_GET(coli);

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_line_colr_ind);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else if (colr_ind < 0) {
      ERR_REPORT(PHG_ERH, ERR113);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_LINE_COLR_IND;
      ARGS_ELMT_SIZE(&args) = sizeof(Pint);
      ARGS_ELMT_DATA(&args).int_data = colr_ind;
      phg_add_el(PHG_CSS, &args);
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

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_linetype);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_LINETYPE;
      ARGS_ELMT_SIZE(&args) = sizeof(Pint);
      ARGS_ELMT_DATA(&args).int_data = FTN_INTEGER_GET(ltype);
      phg_add_el(PHG_CSS, &args);
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

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_linewidth);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_LINEWIDTH;
      ARGS_ELMT_SIZE(&args) = sizeof(Pfloat);
      ARGS_ELMT_DATA(&args).float_data = FTN_REAL_GET(lwidth);
      phg_add_el(PHG_CSS, &args);
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
   Ppoint text_pos;
   char *char_string;

   Pint len = FTN_CHARACTER_LEN(chars);
   text_pos.x = FTN_REAL_GET(px);
   text_pos.y = FTN_REAL_GET(py);

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_text);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, len + 1)) {
         ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
         char_string = (char *) PHG_SCRATCH.buf;
         ARGS_ELMT_TYPE(&args) = PELEM_TEXT;
         ARGS_ELMT_SIZE(&args) = sizeof(Ppoint) + len + 1;
         memcpy(&ARGS_ELMT_DATA(&args).text.pos, &text_pos, sizeof(Ppoint));
         strncpy(char_string, FTN_CHARACTER_GET(chars), len);
         char_string[len] = '\0';
         ARGS_ELMT_DATA(&args).text.char_string = char_string;
         phg_add_el(PHG_CSS, &args);
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

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_char_ht);

   if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
   }
   else {
      ARGS_ELMT_TYPE(&args) = PELEM_CHAR_HT;
      ARGS_ELMT_SIZE(&args) = sizeof(Pfloat);
      ARGS_ELMT_DATA(&args).float_data = FTN_REAL_GET(chh);
      phg_add_el(PHG_CSS, &args);
   }
}

