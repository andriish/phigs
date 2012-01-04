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

#ifndef _sin_qP_h
#define _sin_qP_h

#define SIN_Q_SIZE                      500

#define SIN_Q_NO_OVERFLOW               0
#define SIN_Q_OVERFLOW_NOT_INQUIRED     1
#define SIN_Q_OVERFLOW_INQUIRED         2
#define SIN_Q_OVERFLOW_WS_FLUSHED       3

typedef struct {
   Ploc3 evt;
} Sin_locator_event_data;

typedef struct {
   Ppick evt;
} Sin_pick_event_data;

typedef struct {
   Pstroke3 evt;
} Sin_stroke_event_data;

typedef struct {
   Pfloat value;
} Sin_valuator_event_data;

typedef struct {
   Pchoice evt;
} Sin_choice_event_data;

typedef struct {
   Phg_string evt;
} Sin_string_event_data;

typedef union {
   Sin_locator_event_data  locator;
   Sin_pick_event_data     pick;
   Sin_stroke_event_data   stroke;
   Sin_valuator_event_data valuator;
   Sin_choice_event_data   choice;
   Sin_string_event_data   string;
} Sin_event_data;

typedef struct {
   Pint           wsid;
   Pint           dev_num;
   Pin_class      dev_class;
   Sin_event_data data;
} Sin_input_event;

typedef struct {
   Pint            previous;
   Pint            next;
   Sin_input_event event;
} Sin_q_element;

typedef struct {
   Pint          count;
   Pint          size;
   Pint          last;
   Pint          stack_current;
   unsigned      next_simul_id;
   unsigned      cur_simul_id;
   Pint          overflow;
   Pevent        overflow_dev;
   void          (*event_notify_proc)(void);
   int           free_stack[SIN_Q_SIZE];
   Sin_q_element events[SIN_Q_SIZE];
   Err_handle    erh;
} Sin_event_queue;

#define SIN_Q_FULL( queue) \
    ((queue)->count >= (queue)->size)

#define SIN_Q_EMPTY( queue) \
    ((queue)->count <= 0)

#define SIN_Q_ENOUGH_ROOM( queue, count) \
    ((queue)->count + (count) <= (queue)->size)

#define SIN_Q_NUM_FREE_EVENTS( queue) \
    ((queue)->size - (queue)->count)

#define SIN_Q_SET_OVERFLOW( _queue, _dev) \
    ((_queue)->overflow = SIN_Q_OVERFLOW_NOT_INQUIRED); \
    (_queue)->overflow_dev.ws = (_dev)->wsid; \
    (_queue)->overflow_dev.dev = (_dev)->num; \
    (_queue)->overflow_dev.class = SIN_TO_PHIGS_CLASS((_dev)->class)

#define SIN_Q_CLEAR_OVERFLOW( queue) \
    ((queue)->overflow = SIN_Q_NO_OVERFLOW)

#define SIN_Q_OVERFLOWED( queue) \
    ((queue)->overflow)

#define SIN_Q_PUSH_FREE_ELEMENT( queue, index) \
    (queue)->free_stack[++(queue)->stack_current] = (index)

#define SIN_Q_POP_FREE_ELEMENT( queue) \
    (queue)->free_stack[(queue)->stack_current--]

#define SIN_Q_HEAD_EVENT( _queue ) \
    (&(_queue)->events[(_queue)->events[(_queue)->last].next].event)

#define SIN_Q_NEW_SIMUL_ID( _queue ) \
    ((_queue)->next_simul_id++)

#define SIN_Q_SET_CUR_SIMUL_ID( _queue, _event )\
    ((_queue)->cur_simul_id = (_event)->simul_id)

#define SIN_Q_MORE_SIMUL_EVENTS( _queue ) \
    (!SIN_Q_EMPTY(_queue) && SIN_Q_HEAD_EVENT(_queue)->simul_id > 0 \
        && (_queue)->cur_simul_id == SIN_Q_HEAD_EVENT(_queue)->simul_id)

#endif /* _sinqP_h */

