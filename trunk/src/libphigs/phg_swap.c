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

#include "phg.h"
#include "phgtype.h"

/******************************************************************************
 * phg_swap_head
 *
 * DESCR:       Swap element heading
 * RETURNS:     N/A
 */

static void* phg_swap_head(
   Phg_swap *swp,
   void *data
   )
{
   Phg_elmt_info *el_head;

   el_head = (Phg_elmt_info *) data;
   (*swp->conv_short)(&el_head->elementType);
   (*swp->conv_short)(&el_head->length);

   return (&el_head[1]);
}

/******************************************************************************
 * phg_swap_nil
 *
 * DESCR:       Swap empty element
 * RETURNS:     N/A
 */

static void phg_swap_nil(
   Phg_swap *swp,
   void *data
   )
{
   phg_swap_head(swp, data);
}

/******************************************************************************
 * phg_swap_int
 *
 * DESCR:       Swap integer element
 * RETURNS:     N/A
 */

static void phg_swap_int(
   Phg_swap *swp,
   void *data
   )
{
   uint32_t *p_idata;

   p_idata = (uint32_t *) phg_swap_head(swp, data);
   (*swp->conv_long)(p_idata);
}

/******************************************************************************
 * phg_swap_float
 *
 * DESCR:       Swap float element
 * RETURNS:     N/A
 */

static void phg_swap_float(
   Phg_swap *swp,
   void *data
   )
{
   float *p_fdata;

   p_fdata = (float *) phg_swap_head(swp, data);
   (*swp->conv_float)(p_fdata);
}

Phg_conv phg_swap_tbl[PELEM_NUM_EL_TYPES] = {
   NULL,                           /* PELEM_ALL */
   phg_swap_nil,                   /* PELEM_NIL */
   phg_swap_nil,                   /* PELEM_ADD_NAMES_SET */
   phg_swap_nil,                   /* PELEM_REMOVE_NAMES_SET */
   phg_swap_nil,                   /* PELEM_FILL_AREA */
   phg_swap_nil,                   /* PELEM_FILL_AREA3 */
   phg_swap_nil,                   /* PELEM_FILL_AREA_SET */
   phg_swap_nil,                   /* PELEM_FILL_AREA_SET3 */
   phg_swap_nil,                   /* PELEM_FILL_AREA_SET3_DATA */
   phg_swap_nil,                   /* PELEM_SET_OF_FILL_AREA_SET3_DATA */
   phg_swap_nil,                   /* PELEM_POLYLINE */
   phg_swap_nil,                   /* PELEM_POLYLINE3 */
   phg_swap_nil,                   /* PELEM_POLYMARKER */
   phg_swap_nil,                   /* PELEM_POLYMARKER3 */
   phg_swap_nil,                   /* PELEM_TEXT */
   phg_swap_int,                   /* PELEM_INT_IND */
   phg_swap_int,                   /* PELEM_INT_COLR_IND */
   phg_swap_int,                   /* PELEM_INT_STYLE */
   phg_swap_int,                   /* PELEM_BACK_INT_STYLE */
   phg_swap_int,                   /* PELEM_INT_STYLE_IND */
   phg_swap_int,                   /* PELEM_BACK_INT_STYLE_IND */
   phg_swap_int,                   /* PELEM_LINE_COLR_IND */
   phg_swap_float,                 /* PELEM_LINEWIDTH */
   phg_swap_int,                   /* PELEM_LINETYPE */
   phg_swap_int,                   /* PELEM_LINE_IND */
   phg_swap_int,                   /* PELEM_MARKER_IND */
   phg_swap_int,                   /* PELEM_MARKER_COLR_IND */
   phg_swap_float,                 /* PELEM_MARKER_SIZE */
   phg_swap_int,                   /* PELEM_MARKER_TYPE */
   phg_swap_int,                   /* PELEM_EDGE_IND */
   phg_swap_int,                   /* PELEM_EDGE_COLR_IND */
   phg_swap_float,                 /* PELEM_EDGEWIDTH */
   phg_swap_int,                   /* PELEM_EDGETYPE */
   phg_swap_int,                   /* PELEM_EDGE_FLAG */
   phg_swap_int,                   /* PELEM_TEXT_IND */
   phg_swap_int,                   /* PELEM_TEXT_FONT */
   phg_swap_int,                   /* PELEM_TEXT_PREC */
   phg_swap_int,                   /* PELEM_TEXT_PATH */
   phg_swap_nil,                   /* PELEM_TEXT_ALIGN */
   phg_swap_float,                 /* PELEM_CHAR_HT */
   phg_swap_float,                 /* PELEM_CHAR_EXPAN */
   phg_swap_float,                 /* PELEM_CHAR_SPACE */
   phg_swap_nil,                   /* PELEM_CHAR_UP_VEC */
   phg_swap_int,                   /* PELEM_TEXT_COLR_IND */
   phg_swap_nil,                   /* PELEM_INDIV_ASF */
   phg_swap_nil,                   /* PELEM_LOCAL_MODEL_TRAN3 */
   phg_swap_nil,                   /* PELEM_GLOBAL_MODEL_TRAN3 */
   phg_swap_int,                   /* PELEM_VIEW_IND */
   phg_swap_int,                   /* PELEM_EXEC_STRUCT */
   phg_swap_int,                   /* PELEM_LABEL */
   phg_swap_int,                   /* PELEM_PICK_ID */
   phg_swap_int,                   /* PELEM_HLHSR_ID */
   phg_swap_nil,                   /* PELEM_INT_COLR */
   phg_swap_nil,                   /* PELEM_BACK_INT_COLR */
   phg_swap_nil,                   /* PELEM_LINE_COLR */
   phg_swap_nil,                   /* PELEM_MARKER_COLR */
   phg_swap_nil,                   /* PELEM_EDGE_COLR */
   phg_swap_nil,                   /* PELEM_TEXT_COLR */
   phg_swap_nil,                   /* PELEM_LIGHT_SRC_STATE */
   phg_swap_int,                   /* PELEM_INT_SHAD_METH */
   phg_swap_int,                   /* PELEM_BACK_INT_SHAD_METH */
   phg_swap_int,                   /* PELEM_INT_REFL_EQN */
   phg_swap_int,                   /* PELEM_BACK_INT_REFL_EQN */
   phg_swap_nil,                   /* PELEM_REFL_PROPS */
   phg_swap_nil,                   /* PELEM_BACK_REFL_PROPS */
   phg_swap_int,                   /* PELEM_FACE_DISTING_MODE */
   phg_swap_int                    /* PELEM_FACE_CULL_MODE */
};

