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

#ifndef _sin_h
#define _sin_h

#include <sys/types.h>
#include <X11/Xlib.h>
#include <phigs/private/sinqP.h>

#define SIN_EVT_ACKNOWLEDGE    0x0001

typedef enum {
   SIN_REQUEST,
   SIN_REQUEST_PENDING,
   SIN_EVENT,
   SIN_SAMPLE
} Sin_input_mode;

typedef enum {
   SIN_LOCATOR  = 0,
   SIN_PICK     = 1,
   SIN_STROKE   = 2,
   SIN_VALUATOR = 3,
   SIN_CHOICE   = 4,
   SIN_STRING   = 5
} Sin_input_class;

typedef XPoint Sin_window_pt;

typedef struct {
   Sin_window_pt ll, ur;
} Sin_window_rect;

struct _Sin_input_device;

typedef struct {
   Wst_loc_type  type;
   Sin_window_pt cur_pos;
   Sin_window_pt init_pos;
   Ppoint3       ws_pt;
   Pint          view;
   int           (*resolve)(
                    struct _Sin_input_device *dev,
                    Pint int_data,
                    Sin_window_pt *pt
                    );
   Pline_bundle  ln_bundl;
} Sin_locator_device_data;

typedef struct {
   Wst_pick_type type;
   Sin_window_pt cur_pos;
   Sin_window_pt init_pos;
   Ppick         cur_pick;
   Ppick         init_pick;
   caddr_t       client_data;
   int           (*resolve)(
                    struct _Sin_input_device *dev,
                    Pint int_data,
                    Sin_window_pt *pt
                    );
} Sin_pick_device_data;

typedef struct {
   Wst_stroke_type type;
   Pint            count;
   Pint            init_count;
   Pint            edit_pos;
   Pint            buf_size;
   Sin_window_pt   *init_pts;
   Ppoint3         *ws_pts;
   Pint            view;
   int             (*resolve)(
                      struct _Sin_input_device *dev,
                      Pint int_data,
                      Sin_window_pt *pt
                      );
   Pline_bundle    ln_bundl;
   Pmarker_bundle  mk_bundl;
} Sin_stroke_device_data;

typedef struct {
   Wst_val_type type;
   Pfloat       value;
   Pfloat       init_value;
   Pfloat       low;
   Pfloat       high;
   char         *label;
   char         *low_label;
   char         *high_label;
} Sin_valuator_device_data;

typedef struct {
   Wst_choice_type type;
   Pint            cur_choice;
   Pint            init_choice;
   Pint            count;
   union {
      char         **strings;
      Ppr_switch   *on_off;
   } choices;
} Sin_choice_device_data;

typedef struct {
   Wst_string_type type;
   Pint            buf_size;
   Pint            edit_pos;
   Pint            last_pos;
   char            *string;
   char            *init_string;
} Sin_string_device_data;

typedef union {
   Sin_locator_device_data  locator;
   Sin_pick_device_data     pick;
   Sin_stroke_device_data   stroke;
   Sin_valuator_device_data valuator;
   Sin_choice_device_data   choice;
   Sin_string_device_data   string;
} Sin_device_data;

typedef struct {
   caddr_t         client_data;
   Pint            pe_type;
   Sin_window_rect echo_area;
   Sin_device_data data;
} Sin_dev_init_data;

typedef struct {
   int unused;
} Sin_valuator_handle;

typedef struct {
   int unused;
} Sin_choice_handle;

typedef struct {
   int unused;
} Sin_string_handle;

typedef union {
   Window              window;
   Sin_valuator_handle valuator;
   Sin_choice_handle   choice;
   Sin_string_handle   string;
} Sin_item_handle;

struct _Sin_input_device;

typedef struct {
   int (*create)(
      struct _Sin_input_device *dev
      );
   int (*init)(
      struct _Sin_input_device *dev,
      Sin_dev_init_data *nd
      );
   void (*destroy)(
      struct _Sin_input_device *dev
      );
   void (*reset)(
      struct _Sin_input_device *dev
      );
   void (*enable)(
      struct _Sin_input_device *dev
      );
   void (*disable)(
      struct _Sin_input_device *dev
      );
   void (*sample)(
      struct _Sin_input_device *dev
      );
   void (*resize)(
      struct _Sin_input_device *dev,
      XRectangle *old_rect,
      XRectangle *new_rect
      );
   void (*repaint)(
      struct _Sin_input_device *dev,
      Pint num_rects,
      XRectangle *rects
      );
} Sin_device_ops;

struct _Sin_input_ws;
typedef struct _Sin_input_ws *Sin_handle;

typedef struct _Sin_input_device {
   Pint            wsid;
   Sin_handle      ws;
   Pint            num;
   Sin_input_class class;
   Sin_input_mode  mode;
   Pint            pe_type;
   Pint            echo_sw;
   Sin_window_rect echo_area;
   Sin_item_handle item_handle;
   Sin_device_data data;
   Sin_device_ops  dev_ops;
   caddr_t         client_data;
   struct {
      unsigned on:          1;
      unsigned buffered:    1;
      unsigned exists:      1;
      unsigned been_up_yet: 1;
   } flags;
} Sin_input_device;

struct _Sin_input_ws;

typedef struct _Sin_notify_data {
   Window                  window;
   caddr_t                 handle;
   void                    (*notify)(
                              struct _Sin_input_ws *ws,
                              caddr_t handle,
                              Window window,
                              XEvent *event
                              );
   struct _Sin_notify_data *next;
} Sin_notify_data;

typedef struct {
   unsigned         flags;
   Pint             count;
   Pint             size;
   Sin_input_device **devs;
} Sin_buf_data;

struct _Sin_window_table;

typedef struct _Sin_input_ws {
   Err_handle               erh;
   Pint                     wsid;
   Ws_handle                wsh;
   Wst_input_wsdt           *idt;
   Sin_event_queue          *queue;
   Sin_input_device         *break_device;
   Display                  *display;
   Window                   input_window;
   Window                   output_window;
   Sin_buf_data             event_buffer;
   Sin_notify_data          *notify_list;
   Pnum_in                  num_devs;
   Sin_input_device         *devices[6];
   struct _Sin_window_table *window_table;
} Sin_input_ws;

typedef struct {
   Sin_event_queue *queue;
   Display         *display;
   Window          output_window;
   Window          input_window;
   Ws_handle       wsh;
   Wst_input_wsdt  *idt;
   void            (*send_request)(void);
   int             (*in_viewport)(void);
} Sin_desc;

#define SIN_CLASS_INDEX( class) \
    ((int)(class))

#define SIN_DEV( ws, class, num) \
    (&(ws)->devices[SIN_CLASS_INDEX(class)][(num)-1])

#define SIN_TO_PHIGS_CLASS( _c ) \
    ((_c) == SIN_LOCATOR ? PIN_LOC \
        : (_c) == SIN_STROKE ? PIN_STROKE \
            : (_c) == SIN_PICK ? PIN_PICK \
                : (_c) == SIN_VALUATOR ? PIN_VAL \
                    : (_c) == SIN_CHOICE ? PIN_CHOICE : PIN_STRING)

#define SIN_PHIGS_TO_SIN_CLASS( _c ) \
    ((_c) == PIN_LOC ? SIN_LOCATOR \
        : (_c) == PIN_STROKE ? SIN_STROKE \
            : (_c) == PIN_PICK ? SIN_PICK \
                : (_c) == PIN_VAL ? SIN_VALUATOR \
                    : (_c) == PIN_CHOICE ? SIN_CHOICE : SIN_STRING)

#define SIN_DEV_EXISTS( _dev ) \
    ((_dev) && (_dev)->flags.exists)

#define SIN_EA_WIDTH( _ea ) ((_ea)->ur.x - (_ea)->ll.x)
#define SIN_EA_HEIGHT( _ea ) ((_ea)->ll.y - (_ea)->ur.y)
#define SIN_EA_X( _ea ) ((_ea)->ll.x)
#define SIN_EA_Y( _ea ) ((_ea)->ur.y)

#define SIN_WS_RESET_EVENT_BUFFER( _ev ) \
    (_ev)->flags = 0, (_ev)->count = 0

#define SIN_WS_SET_ACKNOWLEDGE( _ws ) \
    (_ws)->event_buffer.flags |= SIN_EVT_ACKNOWLEDGE

#endif /* _sin_h */

