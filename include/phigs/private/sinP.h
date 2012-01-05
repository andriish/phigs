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

#ifndef _sinP_h
#define _sinP_h

#define SIN_EVENT_NOT_ENQUED_FLAG       0x1
#define SIN_EVENT_ENQUED_FLAG           0x2
#define SIN_EVENT_NOTIFIED_FLAG         0x4

#define SIN_EVENT_NOT_ENQUED( _s )      ((_s) & SIN_EVENT_NOT_ENQUED_FLAG)
#define SIN_EVENT_ENQUED( _s )          ((_s) & SIN_EVENT_ENQUED_FLAG)
#define SIN_EVENT_NOTIFIED( _s )        ((_s) & SIN_EVENT_NOTIFIED_FLAG)

#define SIN_COPY_LOC_INIT_DATA( _o, _n ) \
    { \
    (_n)->data.locator.cur_pos          = (_o)->data.locator.cur_pos; \
    (_n)->data.locator.init_pos         = (_o)->data.locator.init_pos; \
    (_n)->data.locator.wc_pt            = (_o)->data.locator.wc_pt; \
    (_n)->data.locator.view             = (_o)->data.locator.view; \
    (_n)->data.locator.resolve          = (_o)->data.locator.resolve; \
    (_n)->data.locator.ln_bundl         = (_o)->data.locator.ln_bundl; \
    }

#define SIN_COPY_PICK_INIT_DATA( _o, _n ) \
    { \
    (_n)->data.pick.init_pos            = (_o)->data.pick.init_pos; \
    (_n)->data.pick.cur_pos             = (_o)->data.pick.cur_pos; \
    (_n)->data.pick.init_pick           = (_o)->data.pick.init_pick; \
    (_n)->data.pick.cur_pick            = (_o)->data.pick.cur_pick; \
    (_n)->data.pick.client_data         = (_o)->data.pick.client_data; \
    (_n)->data.pick.resolve             = (_o)->data.pick.resolve; \
    }

#define SIN_COPY_CHOICE_INIT_DATA( _o, _n ) \
    { \
    (_n)->data.choice.init_choice       = (_o)->data.choice.init_choice; \
    (_n)->data.choice.cur_choice        = (_o)->data.choice.cur_choice; \
    }

#define SIN_COPY_VAL_INIT_DATA( _o, _n ) \
    { \
    (_n)->data.valuator.init_value      = (_o)->data.valuator.init_value; \
    (_n)->data.valuator.low             = (_o)->data.valuator.low; \
    (_n)->data.valuator.high            = (_o)->data.valuator.high; \
    (_n)->data.valuator.label           = (_o)->data.valuator.label; \
    (_n)->data.valuator.format          = (_o)->data.valuator.format; \
    (_n)->data.valuator.low_label       = (_o)->data.valuator.low_label; \
    (_n)->data.valuator.high_label      = (_o)->data.valuator.high_label; \
    }

#define SET_DEFAULT_DEV_GENERIC_DATA( _ws, _dev ) \
    (_dev)->ws = (_ws); \
    (_dev)->wsid = (_ws)->wsid; \
    (_dev)->mode = SIN_REQUEST; \
    (_dev)->flags.on = 0; \
    (_dev)->echo_sw = TRUE;

#define SIN_EVENT_IS_WANTED( _dev ) \
    ( (_dev)->sin_dev->mode == SIN_REQUEST_PENDING  || \
      (_dev)->sin_dev->mode == SIN_EVENT )

#define SIN_X_RECT_TO_SIN_RECT( _sw_r, _sin_r) \
    (_sin_r)->ll.x = (_sw_r)->x; \
    (_sin_r)->ll.y = (_sw_r)->y + (_sw_r)->height; \
    (_sin_r)->ur.x = (_sw_r)->x + (_sw_r)->width; \
    (_sin_r)->ur.y = (_sw_r)->y;

#define SIN_POINT_IN_RECT( _p, _r) \
    /* window coords, y increases top to bottom */ \
    (   (_p)->x >= (_r)->ll.x && (_p)->x <= (_r)->ur.x \
     && (_p)->y <= (_r)->ll.y && (_p)->y >= (_r)->ur.y)

#define SIN_POINT_IN_ECHO_AREA( _p, _dev) \
    (SIN_POINT_IN_RECT( (_p), &(_dev)->echo_area))

#define SIN_POINT_IN_WS( _p, _ws) \
    (1) /* not needed yet */

#define SIN_SET_ENABLE_DATA( _dev, _ed) \
    (_dev)->echo_area = (_ed)->echo_area;    \
    switch ((_dev)->class) { \
        case SIN_LOCATOR: \
            (_dev)->data.locator.init_pos = (_ed)->data.locator.init_pos;\
            break; \
        case SIN_STROKE: \
            (_dev)->data.stroke.count = (_ed)->data.stroke.cnt; \
            bcopy( (char *)(_ed)->data.stroke.init_pts, \
                (char *)(_dev)->data.stroke.init_pts, \
                (_dev)->data.stroke.count * sizeof(Sin_window_pt)); \
    }

#define SIN_VALUATOR_SCALE( data) \
    (((data).valuator.high - (data).valuator.low) / (data).valuator.length)

#define SIN_BREAK_DEVICE( ws) \
    ((ws)->break_device)

#define SIN_ENABLE_BREAK( device) \
    SIN_BREAK_DEVICE((device)->ws) = device; \

#define SIN_DISABLE_BREAK( ws) \
    SIN_BREAK_DEVICE((ws)) = NULL; \

/* Control-d (^d) is the break "key." */
#define BREAK_EVENT( _e ) \
    ((_e)->type == KeyPress && ((_e)->xkey.state & ControlMask) \
        && (XLookupKeysym(&(_e)->xkey, 0) == XK_d))

#define SIN_SAME_CLIENT( _ca, _cb ) \
    ((_ca)->window == (_cb)->window && (_ca)->handle == (_cb)->handle)

#endif /* _sinP_h */

