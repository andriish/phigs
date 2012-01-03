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

#ifndef _phgP_h
#define _phgP_h

#include <stdio.h>
#include <stdlib.h>
#include <phigs/ws.h>
#include <phigs/phg.h>
#include <phigs/private/cssP.h>

#define ARGS_ELMT_SIZE(ARG) \
   ((Phg_args_add_el *) (ARG))->el_size

#define ARGS_ELMT_TYPE(ARG) \
   ((Phg_args_add_el *) (ARG))->el_type

#define ARGS_ELMT_DATA(ARG) \
   ((Phg_args_add_el *) (ARG))->el_data

#define ELMT_DATA_PTR(ELMT) \
   ((Pelem_data *) (ELMT)->eldata.ptr)

#define PHG_INT(ELMT) \
   *((int *) ((ELMT)->eldata.ptr))

#define PHG_DATA_INT(ELMT) \
   *((int *) (((Css_eldata *) (ELMT))->ptr))

#define PHG_FLOAT(ELMT) \
   *((float *) ((ELMT)->eldata.ptr))

#define PHG_DATA_FLOAT(ELMT) \
   *((float *) (((Css_eldata *) (ELMT))->ptr))

#define PHG_INT_LIST(ELMT) \
   ((Pint_list *) ((ELMT)->eldata.ptr))

#define PHG_DATA_INT_LIST(ELMT) \
   ((Pint_list *) (((Css_eldata *) (ELMT))->ptr))

#define PHG_POINT_LIST(ELMT) \
   ((Ppoint_list *) ((ELMT)->eldata.ptr))

#define PHG_DATA_POINT_LIST(ELMT) \
   ((Ppoint_list *) (((Css_eldata *) (ELMT))->ptr))

#define PHG_POINT_LIST3(ELMT) \
   ((Ppoint_list3 *) ((ELMT)->eldata.ptr))

#define PHG_DATA_POINT_LIST3(ELMT) \
   ((Ppoint_list3 *) (((Css_eldata *) (ELMT))->ptr))

#define PHG_LOCAL_TRAN(ELMT) \
   ((Plocal_tran *) ((ELMT)->eldata.ptr))

#define PHG_DATA_LOCAL_TRAN(ELMT) \
   ((Plocal_tran *) (((Css_eldata *) (ELMT))->ptr))

#define PHG_LOCAL_TRAN3(ELMT) \
   ((Plocal_tran3 *) ((ELMT)->eldata.ptr))

#define PHG_DATA_LOCAL_TRAN3(ELMT) \
   ((Plocal_tran3 *) (((Css_eldata *) (ELMT))->ptr))

#define PHG_TEXT(ELMT) \
   ((Ptext *) ((ELMT)->eldata.ptr))

#define PHG_DATA_TEXT(ELMT) \
   ((Ptext *) (((Css_eldata *) (ELMT))->ptr))

#define PHG_EDGE_FLAG(ELMT) \
   *((Pedge_flag *) ((ELMT)->eldata.ptr))

#define PHG_DATA_EDGE_FLAG(ELMT) \
   *((Pedge_flag *) (((Css_eldata *) (ELMT))->ptr))

#define PHG_INT_STYLE(ELMT) \
   *((Pint_style *) ((ELMT)->eldata.ptr))

#define PHG_DATA_INT_STYLE(ELMT) \
   *((Pint_style *) (((Css_eldata *) (ELMT))->ptr))

#define PHG_COLR(ELMT) \
   ((Pgcolr *) ((ELMT)->eldata.ptr))

#define PHG_DATA_COLR(ELMT) \
   ((Pgcolr *) (((Css_eldata *) (ELMT))->ptr))

/*******************************************************************************
 * phg_get_colr_ind
 *
 * DESCR:       Get colour from index
 * RETURNS:     N/A
 */

void phg_get_colr_ind(
   Ws *ws,
   Pgcolr *gcolr,
   Pint ind
   );

void phg_traverse(Css_handle css, Struct_handle structp);

#endif

