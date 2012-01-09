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

void sample_locator(Pint ws_id)
{
   int view_ind;
   Ppoint3 loc_pos;

   psample_loc3(ws_id, 1, &view_ind, &loc_pos);
   printf("Locator sample: #%-2d:\t[%f, %f, %f]\n",
          view_ind,
          loc_pos.x,
          loc_pos.y,
          loc_pos.z);
}

int locator_event(void)
{
   int ret;
   Sin_input_event *event;

   event = phg_sin_q_next_event(PHG_INPUT_Q);
   if (event != NULL) {
      SIN_Q_SET_CUR_SIMUL_ID(PHG_INPUT_Q, event);
      printf("Locator event: %f %f %f\n",
             event->data.locator.evt.position.x,
             event->data.locator.evt.position.y,
             event->data.locator.evt.position.z);
      phg_sin_q_deque_event(PHG_INPUT_Q);
      ret = 1;
   }
   else {
      ret = 0;
   }

   return ret;
}

int main(void)
{
   Ws *wsh;
   XEvent event;

   popen_phigs(NULL, 0);
   popen_ws(WS_0, NULL, PWST_OUTIN_TRUE);

   wsh = PHG_WSID(WS_0);
   printf("Output window %x\n", (unsigned) wsh->drawable_id);
   printf("Input  window %x\n", (unsigned) wsh->input_overlay_window);

   pset_loc_mode(WS_0, 1, POP_SAMPLE, PSWITCH_NO_ECHO);
   //pset_loc_mode(WS_0, 1, POP_EVENT, PSWITCH_NO_ECHO);

   if (wsh != NULL) {
      while (1) {
         XNextEvent(wsh->display, &event);
         //print_event(&event);
         phg_sin_evt_dispatch(PHG_EVT_TABLE, wsh->display, &event);
         if (event.type == Expose) {
            while (XCheckTypedEvent(wsh->display, Expose, &event));
            (*wsh->redraw_all)(wsh, PFLAG_ALWAYS);
         }
         sample_locator(WS_0);
         //locator_event();
      }
   }

   return 1;
}

