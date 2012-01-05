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

#define SIN_BREAK_DEVICE( ws) \
    ((ws)->break_device)

#define SIN_ENABLE_BREAK( device) \
    SIN_BREAK_DEVICE((device)->ws) = device; \

#define SIN_DISABLE_BREAK( ws) \
    SIN_BREAK_DEVICE((ws)) = NULL; \

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

#endif /* _sinP_h */

