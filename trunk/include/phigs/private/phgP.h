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

#ifndef _phgP_h
#define _phgP_h

#include <stdio.h>
#include <stdlib.h>
#include <phigs/ws.h>
#include <phigs/phg.h>
#include <phigs/phg_el.h>
#include <phigs/private/cssP.h>

#define MAX_NO_OPEN_WS			20

typedef struct {
   Pline_bundle   line_bundle;
   Pmarker_bundle marker_bundle;
   Pint_bundle    int_bundle;
   Pedge_bundle   edge_bundle;
   Ptext_bundle   text_bundle;
} attribute_group;

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

void phg_init_default_attr(void);
attribute_group* phg_get_default_attr(void);
void phg_traverse(Css_handle css, Struct_handle structp);

extern Css_handle css;
extern Pview_rep3 default_views[];

void phg_init_default_views(void);

void phg_draw_fill_area(
   Ws              *ws,
   Ppoint_list     *point_list,
   attribute_group *attrs
   );
void phg_draw_polyline(
   Ws              *ws,
   Ppoint_list     *point_list,
   attribute_group *attrs
   );
void phg_draw_polymarker(
   Ws              *ws,
   Ppoint_list     *point_list,
   attribute_group *attrs
   );
void phg_draw_text(
   Ws              *ws,
   Ppoint          *pos,
   char            *text,
   attribute_group *attrs
   );
void phg_draw_fill_area3(
   Ws              *ws,
   Ppoint_list3    *point_list,
   attribute_group *attrs
   );
void phg_draw_polyline3(
   Ws              *ws,
   Ppoint_list3    *point_list,
   attribute_group *attrs
   );
void phg_draw_polymarker3(
   Ws              *ws,
   Ppoint_list3    *point_list,
   attribute_group *attrs
   );

#endif

