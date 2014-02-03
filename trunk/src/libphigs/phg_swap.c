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
   Pint *p_idata;

   p_idata = (Pint *) phg_swap_head(swp, data);
   (*swp->conv_long)((uint32_t *) p_idata);
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
   Pint *p_idata;

   p_idata = (Pint *) phg_swap_head(swp, data);
   (*swp->conv_long)((uint32_t *) p_idata);
   (*swp->conv_long)((uint32_t *) &p_idata[1]);
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
   Pfloat *p_fdata;

   p_fdata = (Pfloat *) phg_swap_head(swp, data);
   (*swp->conv_float)((float *) p_fdata);
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
   Pfloat *p_fdata;

   p_fdata = (Pfloat *) phg_swap_head(swp, data);
   (*swp->conv_float)((float *) p_fdata);
   (*swp->conv_float)((float *) &p_fdata[1]);
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
   Pint *p_idata;
   Pint_list int_list;
   Pint i;

   p_idata = (Pint *) phg_swap_head(swp, data);

   if (swp->fromFormat & PHG_AR_HOST_BYTE_ORDER) {
      int_list.num_ints = p_idata[0];
      (*swp->conv_long)((uint32_t *) p_idata);
   }
   else if (swp->toFormat & PHG_AR_HOST_BYTE_ORDER) {
      (*swp->conv_long)((uint32_t *) p_idata);
      int_list.num_ints = p_idata[0];
   }
   else {
      int_list.num_ints = p_idata[0];
   }

   int_list.ints = (Pint *) &p_idata[1];
   for (i = 0; i < int_list.num_ints; i++) {
      (*swp->conv_long)((uint32_t *) &int_list.ints[i]);
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
   Pint *p_idata;
   Ppoint_list point_list;
   Pint i;

   p_idata = (Pint *) phg_swap_head(swp, data);

   if (swp->fromFormat & PHG_AR_HOST_BYTE_ORDER) {
      point_list.num_points = p_idata[0];
      (*swp->conv_long)((uint32_t *) p_idata);
   }
   else if (swp->toFormat & PHG_AR_HOST_BYTE_ORDER) {
      (*swp->conv_long)((uint32_t *) p_idata);
      point_list.num_points = p_idata[0];
   }
   else {
      point_list.num_points = p_idata[0];
   }

   point_list.points = (Ppoint *) &p_idata[1];
   for (i = 0; i < point_list.num_points; i++) {
      (*swp->conv_float)((float *) &point_list.points[i].x);
      (*swp->conv_float)((float *) &point_list.points[i].y);
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
   Pint *p_idata;
   Ppoint_list3 point_list;
   Pint i;

   p_idata = (Pint *) phg_swap_head(swp, data);

   if (swp->fromFormat & PHG_AR_HOST_BYTE_ORDER) {
      point_list.num_points = p_idata[0];
      (*swp->conv_long)((uint32_t *) p_idata);
   }
   else if (swp->toFormat & PHG_AR_HOST_BYTE_ORDER) {
      (*swp->conv_long)((uint32_t *) p_idata);
      point_list.num_points = p_idata[0];
   }
   else {
      point_list.num_points = p_idata[0];
   }

   point_list.points = (Ppoint3 *) &p_idata[1];
   for (i = 0; i < point_list.num_points; i++) {
      (*swp->conv_float)((float *) &point_list.points[i].x);
      (*swp->conv_float)((float *) &point_list.points[i].y);
      (*swp->conv_float)((float *) &point_list.points[i].z);
   }
}

/******************************************************************************
 * phg_swap_text
 *
 * DESCR:       Swap text element
 * RETURNS:     N/A
 */

static void phg_swap_text(
   Phg_swap *swp,
   void *data
   )
{
   Ppoint *p_pdata;

   p_pdata = (Ppoint *) phg_swap_head(swp, data);
   (*swp->conv_float)((float *) &p_pdata->x);
   (*swp->conv_float)((float *) &p_pdata->y);
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
   Pfloat *p_fdata;
   Pint i;

   p_fdata = (Pfloat *) phg_swap_head(swp, data);

   for (i = 0; i < 16; i++) {
      (*swp->conv_float)((float *) p_fdata);
   }
}

/******************************************************************************
 * phg_swap_local_tran3
 *
 * DESCR:       Swap local transformation 3D element
 * RETURNS:     N/A
 */

static void phg_swap_local_tran3(
   Phg_swap *swp,
   void *data
   )
{
   Pint *p_idata;
   Pfloat *p_fdata;
   Pint i;

   p_idata = (Pint *) phg_swap_head(swp, data);
   (*swp->conv_long)((uint32_t *) p_idata);
   p_fdata = (Pfloat *) &p_idata[1];

   for (i = 0; i < 16; i++) {
      (*swp->conv_float)((float *) p_fdata);
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
   Pint *p_idata;
   Pfloat *p_fdata;
   Pgcolr gcolr;

   p_idata = (Pint *) phg_swap_head(swp, data);

   if (swp->fromFormat & PHG_AR_HOST_BYTE_ORDER) {
      gcolr.type = p_idata[0];
      (*swp->conv_long)((uint32_t *) p_idata);
   }
   else if (swp->toFormat & PHG_AR_HOST_BYTE_ORDER) {
      (*swp->conv_long)((uint32_t *) p_idata);
      gcolr.type = p_idata[0];
   }
   else {
      gcolr.type = p_idata[0];
   }

   if (gcolr.type == PINDIRECT) {
      (*swp->conv_long)((uint32_t *) &p_idata[1]);
   }
   else if (gcolr.type == PMODEL_RGB) {
      p_fdata = (Pfloat *) &p_idata[1];
      (*swp->conv_float)((float *) &p_fdata[0]);
      (*swp->conv_float)((float *) &p_fdata[1]);
      (*swp->conv_float)((float *) &p_fdata[2]);
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
   phg_swap_text,                  /* PELEM_TEXT */
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
   phg_swap_local_tran3,           /* PELEM_LOCAL_MODEL_TRAN3 */
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

