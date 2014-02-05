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
   uint32_t *idata;

   idata = (uint32_t *) phg_swap_head(swp, data);
   (*swp->conv_long)(idata);
}

/******************************************************************************
 * phg_swap_int2
 *
 * DESCR:       Swap dual integer element
 * RETURNS:     N/A
 */

static void phg_swap_int2(
   Phg_swap *swp,
   void *data
   )
{
   uint32_t *idata;

   idata = (uint32_t *) phg_swap_head(swp, data);
   (*swp->conv_long)(idata);
   (*swp->conv_long)(&idata[1]);
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
   float32_t *fdata;

   fdata = (float32_t *) phg_swap_head(swp, data);
   (*swp->conv_float)(fdata);
}

/******************************************************************************
 * phg_swap_float2
 *
 * DESCR:       Swap dual float element
 * RETURNS:     N/A
 */

static void phg_swap_float2(
   Phg_swap *swp,
   void *data
   )
{
   float32_t *fdata;

   fdata = (float32_t *) phg_swap_head(swp, data);
   (*swp->conv_float)(fdata);
   (*swp->conv_float)(&fdata[1]);
}

/******************************************************************************
 * phg_swap_int_list
 *
 * DESCR:       Swap integer list element
 * RETURNS:     N/A
 */

static void phg_swap_int_list(
   Phg_swap *swp,
   void *data
   )
{
   uint32_t i, num_ints;
   uint32_t *idata;

   idata = (uint32_t *) phg_swap_head(swp, data);

   if (swp->fromFormat & PHG_AR_HOST_BYTE_ORDER) {
      num_ints = *idata;
      (*swp->conv_long)(idata);
   }
   else if (swp->toFormat & PHG_AR_HOST_BYTE_ORDER) {
      (*swp->conv_long)(idata);
      num_ints = *idata;
   }
   else {
      num_ints = *idata;
   }

   idata = &idata[1];
   for (i = 0; i < num_ints; i++) {
      (*swp->conv_long)(idata++);
   }
}

/******************************************************************************
 * phg_swap_point_list
 *
 * DESCR:       Swap point list element
 * RETURNS:     N/A
 */

static void phg_swap_point_list(
   Phg_swap *swp,
   void *data
   )
{
   uint32_t i, num_points;
   uint32_t *idata;
   float32_t *fdata;

   idata = (uint32_t *) phg_swap_head(swp, data);

   if (swp->fromFormat & PHG_AR_HOST_BYTE_ORDER) {
      num_points = *idata;
      (*swp->conv_long)(idata);
   }
   else if (swp->toFormat & PHG_AR_HOST_BYTE_ORDER) {
      (*swp->conv_long)(idata);
      num_points = *idata;
   }
   else {
      num_points = *idata;
   }

   fdata = (float32_t *) &idata[1];
   for (i = 0; i < num_points; i++) {
      (*swp->conv_float)(fdata++);
      (*swp->conv_float)(fdata++);
   }
}

/******************************************************************************
 * phg_swap_point_list3
 *
 * DESCR:       Swap point list 3D element
 * RETURNS:     N/A
 */

static void phg_swap_point_list3(
   Phg_swap *swp,
   void *data
   )
{
   uint32_t i, num_points;
   uint32_t *idata;
   float32_t *fdata;

   idata = (uint32_t *) phg_swap_head(swp, data);

   if (swp->fromFormat & PHG_AR_HOST_BYTE_ORDER) {
      num_points = *idata;
      (*swp->conv_long)(idata);
   }
   else if (swp->toFormat & PHG_AR_HOST_BYTE_ORDER) {
      (*swp->conv_long)(idata);
      num_points = *idata;
   }
   else {
      num_points = *idata;
   }

   fdata = (float32_t *) &idata[1];
   for (i = 0; i < num_points; i++) {
      (*swp->conv_float)(fdata++);
      (*swp->conv_float)(fdata++);
      (*swp->conv_float)(fdata++);
   }
}

/******************************************************************************
 * phg_swap_point_string
 *
 * DESCR:       Swap point and string
 * RETURNS:     N/A
 */

static void phg_swap_point_string(
   Phg_swap *swp,
   void *data
   )
{
   float32_t *fdata;

   fdata = (float32_t *) phg_swap_head(swp, data);
   (*swp->conv_float)(fdata++);
   (*swp->conv_float)(fdata++);
}

/******************************************************************************
 * phg_swap_matrix3
 *
 * DESCR:       Swap matrix 3D element
 * RETURNS:     N/A
 */

static void phg_swap_matrix3(
   Phg_swap *swp,
   void *data
   )
{
   int i;
   float32_t *fdata;

   fdata = (float32_t *) phg_swap_head(swp, data);

   for (i = 0; i < 16; i++) {
      (*swp->conv_float)(fdata);
   }
}

/******************************************************************************
 * phg_swap_int_matrix3
 *
 * DESCR:       Swap integer and matrix 3D element
 * RETURNS:     N/A
 */

static void phg_swap_int_matrix3(
   Phg_swap *swp,
   void *data
   )
{
   int i;
   uint32_t *idata;
   float32_t *fdata;

   idata = (uint32_t *) phg_swap_head(swp, data);
   (*swp->conv_long)(idata);
   fdata = (float32_t *) &idata[1];

   for (i = 0; i < 16; i++) {
      (*swp->conv_float)(fdata);
   }
}

/******************************************************************************
 * phg_swap_gcolr
 *
 * DESCR:       Swap generic colour element
 * RETURNS:     N/A
 */

static void phg_swap_gcolr(
   Phg_swap *swp,
   void *data
   )
{
   uint32_t type;
   uint32_t *idata;
   float32_t *fdata;

   idata = (uint32_t *) phg_swap_head(swp, data);

   if (swp->fromFormat & PHG_AR_HOST_BYTE_ORDER) {
      type = *idata;
      (*swp->conv_long)(idata);
   }
   else if (swp->toFormat & PHG_AR_HOST_BYTE_ORDER) {
      (*swp->conv_long)(idata);
      type = *idata;
   }
   else {
      type = *idata;
   }

   if (type == PINDIRECT) {
      (*swp->conv_long)(&idata[1]);
   }
   else if (type == PMODEL_RGB) {
      fdata = (float32_t *) &idata[1];
      (*swp->conv_float)(fdata++);
      (*swp->conv_float)(fdata++);
      (*swp->conv_float)(fdata++);
   }
}

Phg_conv phg_swap_tbl[PELEM_NUM_EL_TYPES] = {
   NULL,                           /* PELEM_ALL */
   phg_swap_nil,                   /* PELEM_NIL */
   phg_swap_int_list,              /* PELEM_ADD_NAMES_SET */
   phg_swap_int_list,              /* PELEM_REMOVE_NAMES_SET */
   phg_swap_point_list,            /* PELEM_FILL_AREA */
   phg_swap_point_list3,           /* PELEM_FILL_AREA3 */
   phg_swap_nil,                   /* PELEM_FILL_AREA_SET */
   phg_swap_nil,                   /* PELEM_FILL_AREA_SET3 */
   phg_swap_nil,                   /* PELEM_FILL_AREA_SET3_DATA */
   phg_swap_nil,                   /* PELEM_SET_OF_FILL_AREA_SET3_DATA */
   phg_swap_point_list,            /* PELEM_POLYLINE */
   phg_swap_point_list3,           /* PELEM_POLYLINE3 */
   phg_swap_point_list,            /* PELEM_POLYMARKER */
   phg_swap_point_list3,           /* PELEM_POLYMARKER3 */
   phg_swap_point_string,          /* PELEM_TEXT */
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
   phg_swap_int2,                  /* PELEM_TEXT_ALIGN */
   phg_swap_float,                 /* PELEM_CHAR_HT */
   phg_swap_float,                 /* PELEM_CHAR_EXPAN */
   phg_swap_float,                 /* PELEM_CHAR_SPACE */
   phg_swap_float2,                /* PELEM_CHAR_UP_VEC */
   phg_swap_int,                   /* PELEM_TEXT_COLR_IND */
   phg_swap_int2,                  /* PELEM_INDIV_ASF */
   phg_swap_int_matrix3,           /* PELEM_LOCAL_MODEL_TRAN3 */
   phg_swap_matrix3,               /* PELEM_GLOBAL_MODEL_TRAN3 */
   phg_swap_int,                   /* PELEM_VIEW_IND */
   phg_swap_int,                   /* PELEM_EXEC_STRUCT */
   phg_swap_int,                   /* PELEM_LABEL */
   phg_swap_int,                   /* PELEM_PICK_ID */
   phg_swap_int,                   /* PELEM_HLHSR_ID */
   phg_swap_gcolr,                 /* PELEM_INT_COLR */
   phg_swap_gcolr,                 /* PELEM_BACK_INT_COLR */
   phg_swap_gcolr,                 /* PELEM_LINE_COLR */
   phg_swap_gcolr,                 /* PELEM_MARKER_COLR */
   phg_swap_gcolr,                 /* PELEM_EDGE_COLR */
   phg_swap_gcolr,                 /* PELEM_TEXT_COLR */
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

