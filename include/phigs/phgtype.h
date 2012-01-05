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

#ifndef _phgtype_h
#define _phgtype_h

#ifndef TRUE
#define TRUE     1
#endif

#ifndef FALSE
#define FALSE    0
#endif

#define PHG_ZERO_TOLERANCE      1.0e-30

typedef struct {
   Pint   view_ind;
   Ppoint position;
} Ploc;

typedef struct {
   Pint    view_ind;
   Ppoint3 position;
} Ploc3;

typedef struct {
   Pin_status status;
   Ppick_path pick_path;
} Ppick;

typedef struct {
   Pint   view_ind;
   Pint   num_points;
   Ppoint *points;
} Pstroke;

typedef struct {
   Pint    view_ind;
   Pint    num_points;
   Ppoint3 *points;
} Pstroke3;

typedef struct {
   Pin_status status;
   Pint       choice;
} Pchoice;

typedef struct {
   Pint  length;
   char *string;
} Phg_string;

typedef union {
   Ploc3      loc;
   Ppick      pik;
   Pstroke3   stk;
   Pfloat     val;
   Pchoice    chc;
   Phg_string str;
} Phg_inp_event_data;

typedef struct {
   Pint      ws;
   Pin_class class;
   Pint      dev;
} Pevent;

struct _Sin_event_queue;
typedef struct _Sin_event_queue *Input_q_handle;

typedef struct {
   char     *buf;
   unsigned size;
} Phg_scratch;

typedef struct _Css_structel *El_handle;
typedef struct _Css_ssl      *Struct_handle;
typedef struct _Css_struct   *Css_handle;
typedef struct _Ws           *Ws_handle;

#define PHG_MIN(a, b) \
   (((a) < (b)) ? (a) : (b))

#define PHG_MAX(a, b) \
   (((a) > (b)) ? (a) : (b))

#define PHG_ABS(a) \
   (((a) < 0.0) ? -(a) : (a))

#define PHG_NEAR_ZERO(s) \
   (PHG_ABS(s) < PHG_ZERO_TOLERANCE)

#define PHG_ZERO_MAG(s) \
   ((s) < PHG_ZERO_TOLERANCE)

#define PHG_MAG_V2(v)                  \
   (sqrt((v)->delta_x * (v)->delta_x + \
         (v)->delta_y * (v)->delta_y))

#define PHG_MAG_V3(v)                  \
   (sqrt((v)->delta_x * (v)->delta_x + \
         (v)->delta_y * (v)->delta_y + \
         (v)->delta_z * (v)->delta_z))

#define PHG_DOT_PROD(v1, v2) \
   ((v1)->x * (v2)->x + (v1)->y * (v2)->y + (v1)->z * (v2)->z)

#define PHG_UNIT_VEC(v)                                               \
   { double len;                                                      \
     len = sqrt((v)->x * (v)->x + (v)->y * (v)->y + (v)->z * (v)->z); \
     (v)->x /= len, (v)->y /= len, (v)->z /= len;                     \
   }

#define PHG_IN_RANGE(low, high, val) \
   ((val) >= (low) && (val) <= (high))

#define PHG_SCRATCH_SPACE(_sc, _size) \
    ((_sc)->size >= (_size) ? (_sc)->buf \
        : phg_grow_scratch( (_sc), (unsigned)(_size) ))

#endif

