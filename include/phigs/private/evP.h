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

#ifndef _evP_h
#define _evP_h

#include <sys/types.h>
#include <X11/Xlib.h>
#include <phigs/phg.h>
#include <phigs/util.h>

typedef struct {
   Node    node;
   Display *display;
   Window  window;
   caddr_t cdata;
   void    (*callback)(
              Display *display,
              Window window,
              caddr_t caddr,
              XEvent *event
              );
} Phg_ev_entry;

typedef struct {
   Pint num_events;
   List *events;
} Phg_ev_tbl;

/*******************************************************************************
 * phg_ev_tbl_create
 * 
 * DESCR:       Create event table
 * RETURNS:     Pointer to event table or NULL
 */

Phg_ev_tbl* phg_ev_tbl_create(
   int num_events
   );

/*******************************************************************************
 * phg_ev_tbl_init
 *
 * DESCR:       Inititalize event table
 * RETURNS:     TRUE or FALSE
 */

int phg_ev_tbl_init(
   Phg_ev_tbl *ev_tbl
   );

/*******************************************************************************
 * phg_ev_tbl_destroy
 *
 * DESCR:       Destroy event table
 * RETURNS:     N/A
 */

void phg_ev_tbl_destroy(
   Phg_ev_tbl *ev_tbl
   );

/*******************************************************************************
 * phg_ev_register
 *
 * DESCR:       Register an event in event table
 * RETURNS:     TRUE or FALSE
 */

int phg_ev_register(
   Phg_ev_tbl *ev_tbl,
   Display *display,
   Window window,
   int event_type,
   caddr_t cdata,
   void (*callback)(Display*, Window, caddr_t, XEvent*)
   );

/*******************************************************************************
 * phg_ev_unregister
 * 
 * DESCR:       Unregister an event in event table
 * RETURNS:     N/A
 */

void phg_ev_unregister(
   Phg_ev_tbl *ev_tbl,
   Display *display,
   Window window,
   int event_type,
   caddr_t cdata
   );

/*******************************************************************************
 * phg_ev_unregister_display
 *
 * DESCR:       Unregister all events for display in event table
 * RETURNS:     N/A
 */

void phg_ev_unregister_display(
   Phg_ev_tbl *ev_tbl,
   Display *display
   );

/*******************************************************************************
 * phg_ev_unregister_window
 *
 * DESCR:       Unregister all events for window in event table
 * RETURNS:     N/A
 */

void phg_ev_unregister_window(
   Phg_ev_tbl *ev_tbl,
   Display *display,
   Window window
   );

/*******************************************************************************
 * phg_ev_dispatch
 * 
 * DESCR:       Dipatch events using event table
 * RETURNS:     N/A
 */

void phg_ev_dispatch(
   Phg_ev_tbl *ev_tbl,
   Display *display,
   XEvent *event
   );

#endif /* _evP_h */

