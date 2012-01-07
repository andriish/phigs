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

#define WS_0 0

void print_event(XEvent *event)
{
    printf("%-2d: ", event->type);
    phg_sin_evt_print(event);
    printf("\tWindow = %x\n", (unsigned) event->xany.window);
}

Ws* phg_open_ws(Pint ws_id)
{
   Ws_handle wsh;
   Input_q_handle input_q;

   wsh = PHG_WSID(ws_id);
   wsh->input_overlay_window = phg_wsx_create_overlay(wsh);

   printf("Creating input queue\n");
   input_q = phg_sin_q_create(PHG_ERH);
   if (input_q == NULL) {
      fprintf(stderr, "Unable to create input queue\n");
      wsh = NULL;
   }
   else
   {
      printf("Opening workstation %d for input\n", ws_id);
      if (!phg_ws_input_init(wsh, input_q)) {
         fprintf(stderr, "Error unable to open workstation for input\n");
         wsh = NULL;
      }
      else {
#if TODO
         event_mask = StructureNotifyMask;
         XSelectInput(wsh->display, wsh->drawable_id, event_mask);
#endif
      }
   }

   return wsh;
}


int main(void)
{
   Ws *wsh;
   XEvent event;

   popen_phigs(NULL, 0);
   popen_ws(WS_0, NULL, PWST_OUTPUT_TRUE);
   wsh = phg_open_ws(WS_0);

   printf("Output window %x\n", (unsigned) wsh->drawable_id);
   printf("Input  window %x\n", (unsigned) wsh->input_overlay_window);

   if (wsh != NULL) {
      printf("Waiting for events...\n");
      while (1) {
         XNextEvent(wsh->display, &event);
         print_event(&event);
         phg_sin_evt_dispatch(PHG_EVT_TABLE, wsh->display, &event);
         if (event.type == Expose) {
            while (XCheckTypedEvent(wsh->display, Expose, &event));
            (*wsh->redraw_all)(wsh, PFLAG_ALWAYS);
         }
      }
   }

   return 1;
}

