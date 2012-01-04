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

#include <stdio.h>
#include <stdlib.h>
#include <phigs/phg.h>
#include <phigs/private/evP.h>

/*******************************************************************************
 * phg_ev_tbl_create
 *
 * DESCR:       Create event table
 * RETURNS:     Pointer to event table or NULL
 */

Phg_ev_tbl* phg_ev_tbl_create(
   int num_events
   )
{
   Phg_ev_tbl *ev_tbl;

   ev_tbl = (Phg_ev_tbl *) malloc(sizeof(Phg_ev_tbl) +
                                  sizeof(Phg_ev_entry) * num_events);
   if (ev_tbl != NULL) {
      ev_tbl->num_events = num_events;
      ev_tbl->events = (List *) &ev_tbl[1];
      if (!phg_ev_tbl_init(ev_tbl)) {
         free(ev_tbl);
         ev_tbl = NULL;
      }
   }

   return ev_tbl;
}

/*******************************************************************************
 * phg_ev_tbl_init
 *
 * DESCR:       Inititalize event table
 * RETURNS:     TRUE or FALSE
 */

int phg_ev_tbl_init(
   Phg_ev_tbl *ev_tbl
   )
{
   int i;

   for (i = 0; i < ev_tbl->num_events; i++) {
      list_init(&ev_tbl->events[i]);
   }

   return TRUE;
}

/*******************************************************************************
 * phg_ev_tbl_destroy
 *
 * DESCR:       Destroy event table
 * RETURNS:     N/A
 */

void phg_ev_tbl_destroy(
   Phg_ev_tbl *ev_tbl
   )
{
   free(ev_tbl);
}

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
   )
{
   int status;
   Phg_ev_entry *ev;

   /* First check if entry exists */
   for (ev = (Phg_ev_entry *) LIST_HEAD(&ev_tbl->events[event_type]);
        ev != NULL;
        ev = (Phg_ev_entry *) NODE_NEXT(&ev->node)) {
      if ((ev->display == display) &&
          (ev->window == window) &&
          (ev->cdata == cdata)) {
         ev->callback = callback;
         break;
      }
   }

   /* Was not found */
   if (ev == NULL) {
      ev = (Phg_ev_entry *) malloc(sizeof(Phg_ev_entry));
      if (ev == NULL) {
         status = FALSE;
      }
      else {
         list_add(&ev_tbl->events[event_type], &ev->node);
      }
   }
   else {
      status = TRUE;
   }

   return status;
}

/*******************************************************************************
 * phg_ev_unregister
 *
 * DESCR:       Unregister event in event table
 * RETURNS:     N/A
 */

void phg_ev_unregister(
   Phg_ev_tbl *ev_tbl,
   Display *display,
   Window window,
   int event_type,
   caddr_t cdata
   )
{
   Phg_ev_entry *ev;

   /* First check if entry exists */
   for (ev = (Phg_ev_entry *) LIST_HEAD(&ev_tbl->events[event_type]);
        ev != NULL;
        ev = (Phg_ev_entry *) NODE_NEXT(&ev->node)) {
      if ((ev->display == display) &&
          (ev->window == window) &&
          (ev->cdata == cdata)) {
         list_remove(&ev_tbl->events[event_type], &ev->node);
         free(ev);
         break;
      }
   }
}

/*******************************************************************************
 * phg_ev_unregister_display
 *
 * DESCR:       Unregister all events for display in event table
 * RETURNS:     N/A
 */

void phg_ev_unregister_display(
   Phg_ev_tbl *ev_tbl,
   Display *display
   )
{
   int i;
   Phg_ev_entry *ev;

   for (i = 0; i < ev_tbl->num_events; i++) {
      for (ev = (Phg_ev_entry *) LIST_HEAD(&ev_tbl->events[i]);
           ev != NULL;
           ev = (Phg_ev_entry *) NODE_NEXT(&ev->node)) {
         if (ev->display == display) {
            list_remove(&ev_tbl->events[i], &ev->node);
            free(ev);
         }
      }
   }
}

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
   )
{
   int i;
   Phg_ev_entry *ev;

   for (i = 0; i < ev_tbl->num_events; i++) {
      for (ev = (Phg_ev_entry *) LIST_HEAD(&ev_tbl->events[i]);
           ev != NULL;
           ev = (Phg_ev_entry *) NODE_NEXT(&ev->node)) {
         if ((ev->display == display) &&
             (ev->window == window)) {
            list_remove(&ev_tbl->events[i], &ev->node);
            free(ev);
         }
      }
   }
}

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
   )
{
   Phg_ev_entry *ev;

   /* First check if entry exists */
   for (ev = (Phg_ev_entry *) LIST_HEAD(&ev_tbl->events[event->type]);
        ev != NULL;
        ev = (Phg_ev_entry *) NODE_NEXT(&ev->node)) {
      if ((ev->display == display) &&
          (ev->window == event->xany.window)) {
         (*ev->callback)(display,
                         event->xany.window,
                         ev->cdata,
                         event);
      }
   }
}

