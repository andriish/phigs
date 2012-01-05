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

#include <X11/Xlib.h>
#include <phigs/private/sinqP.h>

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

typedef struct {
   int dummy;
} Sin_dev_init_data;

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
   Sin_device_ops  dev_ops;
} Sin_input_device;

typedef struct _Sin_input_ws {
   Err_handle       erh;
   Pint             wsid;
   Ws_handle        wsh;
   Wst_input_wsdt   *idt;
   Sin_event_queue  *queue;
   Sin_input_device *break_device;
   Display          *display;
   Window           input_window;
   Window           output_window;
   Pnum_in          num_devs;
   Sin_input_device *devices[6];
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

#endif /* _sin_h */

