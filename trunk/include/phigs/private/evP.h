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

#endif /* _evP_h */

