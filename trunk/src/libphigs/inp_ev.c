#include <stdio.h>
#include <stdlib.h>
#include <phigs/phg.h>
#include <phigs/private/evP.h>

Phg_ev_tbl* phg_ev_tbl_create(
   int num_events
   )
{
   Phg_ev_tbl *ev_tbl;

   ev_tbl = (Phg_ev_tbl *) malloc(sizeof(Phg_ev_tbl) +
                                  sizeof(Phg_ev_entry) * num_events);

   if (ev_tbl != NULL) {
      ev_tbl->num_events = num_events;
      ev_tbl->events = (Phg_ev_entry *) &ev_tbl[1];
      if (!phg_ev_init(ev_tbl)) {
         free(ev_tbl);
         ev_tbl = NULL;
      }
   }

   return ev_tbl;
}

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

void phg_ev_tbl_destroy(
   Phg_ev_tbl *ev_tbl
   )
{
   free(ev_tbl);
}

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

void phg_ev_unregister_display(
   Phg_ev_tbl *ev_tbl,
   Display *display,
   int event_type
   )
{
   Phg_ev_entry *ev;

   /* First check if entry exists */
   for (ev = (Phg_ev_entry *) LIST_HEAD(&ev_tbl->events[event_type]);
        ev != NULL;
        ev = (Phg_ev_entry *) NODE_NEXT(&ev->node)) {
      if (ev->display == display) {
         list_remove(&ev_tbl->events[event_type], &ev->node);
         free(ev);
         break;
      }
   }
}

void phg_ev_unregister_window(
   Phg_ev_tbl *ev_tbl,
   Display *display,
   Window window,
   int event_type
   )
{
   Phg_ev_entry *ev;

   /* First check if entry exists */
   for (ev = (Phg_ev_entry *) LIST_HEAD(&ev_tbl->events[event_type]);
        ev != NULL;
        ev = (Phg_ev_entry *) NODE_NEXT(&ev->node)) {
      if ((ev->display == display) &&
          (ev->window == window)) {
         list_remove(&ev_tbl->events[event_type], &ev->node);
         free(ev);
         break;
      }
   }
}

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

