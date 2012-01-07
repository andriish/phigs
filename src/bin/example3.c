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

void prepare_ws(Pint ws_id)
{
   Ppoint3 position = {0.0, 0.0, 0.0};
   Plimit3 e_volume = {0.0, 1.0, 0.0, 1.0, 0.0, 1.0};

   Ws_handle wsh = PHG_WSID(ws_id);
   Wst_input_wsdt *idt = &wsh->type->desc_tbl.phigs_dt.in_dt;

   /* Default locator */
   idt->num_devs.loc = 1;
   memcpy(&idt->locators[0].position, &position, sizeof(Ppoint3));
   memcpy(&idt->locators[0].e_volume, &e_volume, sizeof(Plimit3));
   idt->locators[0].record.pets.pet_r1.unused = 0;

   /* Default stroke */
   idt->num_devs.stroke = 1;
   memcpy(&idt->strokes[0].e_volume, &e_volume, sizeof(Plimit3));
   idt->strokes[0].record.pets.pet_r1.unused = 0;

   /* Default pick */
   idt->num_devs.pick = 1;
   memcpy(&idt->picks[0].e_volume, &e_volume, sizeof(Plimit3));
   idt->picks[0].record.pets.pet_r1.unused = 0;

   /* Default valuator */
   idt->num_devs.val = 1;
   memcpy(&idt->valuators[0].e_volume, &e_volume, sizeof(Plimit3));
   idt->valuators[0].record.pets.pet_r1.unused = 0;

   /* Default choice */
   idt->num_devs.choice = 1;
   memcpy(&idt->choices[0].e_volume, &e_volume, sizeof(Plimit3));
   idt->choices[0].record.pets.pet_r1.unused = 0;

   /* Default string */
   idt->num_devs.string = 1;
   memcpy(&idt->strings[0].e_volume, &e_volume, sizeof(Plimit3));
   idt->strings[0].record.pets.pet_r1.unused = 0;
}

Ws* phg_open_ws(Pint ws_id)
{
   Ws_handle wsh;
   Input_q_handle input_q;

#if 0
   unsigned long event_mask = 
      KeyPressMask |
      KeyReleaseMask |
      ButtonPressMask |
      ButtonReleaseMask |
      EnterWindowMask |
      LeaveWindowMask |
      PointerMotionHintMask |
      ExposureMask;
#endif

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
#if 0
         XSelectInput(wsh->display, wsh->drawable_id, event_mask);
#endif
      }
   }

   return wsh;
}

void init_locator(Ws *wsh)
{
   Phg_args_set_mode_data args;

   args.mode = POP_SAMPLE;
   args.class = PHG_ARGS_INP_LOC;
   args.dev = 1;
   (*wsh->set_device_mode)(wsh, &args);
}

void sample_locator(Ws *wsh)
{
   Phg_ret ret;

   (*wsh->sample_device)(wsh, PHG_ARGS_INP_LOC, 1, &ret);
   if (ret.err == 0) {
      printf("Error sampling locator\n");
   }
   else {
      printf("Sampled locator at: %f ,%f, %f\n",
             ret.data.inp_event.data.loc.position.x,
             ret.data.inp_event.data.loc.position.y,
             ret.data.inp_event.data.loc.position.z);
   }
}

int main(void)
{
   Ws *wsh;
   XEvent event;

   popen_phigs(NULL, 0);
   popen_ws(WS_0, NULL, PWST_OUTPUT_TRUE);
   prepare_ws(WS_0);
   wsh = phg_open_ws(WS_0);

   printf("Output window %x\n", (unsigned) wsh->drawable_id);
   printf("Input  window %x\n", (unsigned) wsh->input_overlay_window);

   init_locator(wsh);

   if (wsh != NULL) {
      printf("Waiting for events...\n");
      while (1) {
         XNextEvent(wsh->display, &event);
         //print_event(&event);
         phg_sin_evt_dispatch(PHG_EVT_TABLE, wsh->display, &event);
         if (event.type == Expose) {
            while (XCheckTypedEvent(wsh->display, Expose, &event));
            (*wsh->redraw_all)(wsh, PFLAG_ALWAYS);
         }
         sample_locator(wsh);
      }
   }

   return 1;
}

