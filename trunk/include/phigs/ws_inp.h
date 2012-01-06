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

#ifndef _ws_inp_h
#define _ws_inp_h

#include <phigs/sin.h>

typedef struct {
   Pint x, y;
} Ws_point;

typedef struct {
   Pint         num;
   Pop_mode     mode;
   Pecho_switch esw;
   Ploc3        loc;
   Pint         pet;
   Plimit3      e_volume;
   Ploc_data3   record;
} Ws_inp_loc;

typedef struct {
   Pint         num;
   Pop_mode     mode;
   Pecho_switch esw;
   Ppick        pick;
   Pint         pet;
   Plimit3      e_volume;
   Ppick_data3  record;
   Ppath_order  order;
   Pfloat       ap_size;
   Pint         dev_type;
   Pint         measure;
   struct {
      Pint      incl;
      Pint      excl;
   } filter;
   Ppick_path   scratch_path;
} Ws_inp_pick;

typedef struct {
   Pint          num;
   Pop_mode      mode;
   Pecho_switch  esw;
   Pstroke3      stroke;
   Pint          pet;
   Plimit3       e_volume;
   Pstroke_data3 record;
} Ws_inp_stroke;

typedef struct {
   Pint         num;
   Pop_mode     mode;
   Pecho_switch esw;
   Pfloat       val;
   Pint         pet;
   Plimit3      e_volume;
   Pval_data3   record;
   char         *string_buf;
} Ws_inp_val;

typedef struct {
   Pint          num;
   Pop_mode      mode;
   Pecho_switch  esw;
   Pchoice       choice;
   Pint          pet;
   Plimit3       e_volume;
   Pint          strings_length;
   Pchoice_data3 record;
} Ws_inp_choice;

typedef struct {
   Pint          num;
   Pop_mode      mode;
   Pecho_switch  esw;
   Pint          length;
   char          *string;
   Pint          pet;
   Plimit3       e_volume;
   Pstring_data3 record;
} Ws_inp_string;

typedef union {
   Ws_inp_loc    *loc;
   Ws_inp_pick   *pik;
   Ws_inp_stroke *stk;
   Ws_inp_val    *val;
   Ws_inp_choice *cho;
   Ws_inp_string *str;
} Ws_inp_device_handle;

typedef struct _Ws_input_ws {
   Pnum_in          num_devs;
   struct {
      Ws_inp_loc    *locator;
      Ws_inp_pick   *pick;
      Ws_inp_stroke *stroke;
      Ws_inp_val    *valuator;
      Ws_inp_choice *choice;
      Ws_inp_string *string;
   } devs;
   Sin_handle       sin_handle;
   Input_q_handle   input_queue;
   caddr_t          scratch;
   unsigned         scratch_size;
} Ws_input_ws;

#define WS_INP_DEV( _wsh, _class, _num ) \
    (&(_wsh)->in_ws.devs._class[(_num)-1])

#define SET_ECHO_AREA( _ev1, _ev2 ) \
    (_ev2).x_min = (_ev1).x_min; \
    (_ev2).x_max = (_ev1).x_max; \
    (_ev2).y_min = (_ev1).y_min; \
    (_ev2).y_max = (_ev1).y_max

#define MAP_MODE( _m) \
    ((_m) == POP_EVENT ? SIN_EVENT : \
        (_m) == POP_SAMPLE ? SIN_SAMPLE : SIN_REQUEST)

#define WSINP_DC_ECHO_TO_DRWBL_ECHO2( _ws, _ev_vdc, _ea_dc) \
    {   Ppoint  p; \
        p.x = (_ev_vdc)->x_min; \
        p.y = (_ev_vdc)->y_min; \
        WS_DC_TO_DRWBL2((_ws), &p, &(_ea_dc)->ll); \
        p.x = (_ev_vdc)->x_max; \
        p.y = (_ev_vdc)->y_max; \
        WS_DC_TO_DRWBL2((_ws), &p, &(_ea_dc)->ur); \
    }

#define WSINP_INIT_COMMON_FIELDS( stp, def) \
    stp->mode = POP_REQ; \
    stp->esw = PSWITCH_ECHO; \
    stp->pet = 1; \
    stp->e_volume = def->e_volume; \
    stp->record = def->record;

struct _Ws;

/*******************************************************************************
 * phg_ws_inp_init_device
 *
 * DESCR:       Initialize workstation input device
 * RETURNS:     N/A
 */

void phg_ws_inp_init_device(
    struct _Ws *ws,
    Phg_args_inp_init_dev *args
    );

/*******************************************************************************
 * phg_ws_input_init
 *
 * DESCR:       Initialize input workstation
 * RETURNS:     TRUE or FALSE
 */

int phg_ws_input_init(
    struct _Ws *ws,
    Input_q_handle queue
    );

/*******************************************************************************
 * phg_ws_input_close
 *
 * DESCR:       Close input workstation
 * RETURNS:     TRUE or FALSE
 */

void phg_ws_input_close(
    struct _Ws *ws
    );

#endif /* _ws_inp_h */

