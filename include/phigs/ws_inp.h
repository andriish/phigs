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
   Pchoice_data3 record;
} Ws_inp_choice;

typedef struct {
   Pint          num;
   Pop_mode      mode;
   Pecho_switch  esw;
   Pint          lebgth;
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
} Ws_inp_handle;

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

#endif /* _ws_inp_h */

