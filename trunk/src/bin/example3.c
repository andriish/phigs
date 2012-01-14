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

#define WS_0                   0
#define STRUCT_0               0
#define WIN_X0                 0.0
#define WIN_X1                 1.0
#define WIN_Y0                 0.0
#define WIN_Y1                 1.0

Ppoint quad_pts[4] = {
   {0.0, 0.0},
   {1.0, 0.0},
   {1.0, 1.0},
   {0.0, 1.0}
};
Ppoint_list quad = {4, quad_pts};
Plimit win = {WIN_X0, WIN_X1, WIN_Y0, WIN_Y1};
Pgcolr blue;

void print_event(XEvent *event)
{
    printf("%-2d: ", event->type);
    phg_sin_evt_print(event);
    printf("\tWindow = %x\n", (unsigned) event->xany.window);
}

void sample_locator(Pint ws_id)
{
   Pint view_ind;
   Ppoint3 loc_pos;

   psample_loc3(ws_id, 1, &view_ind, &loc_pos);
   printf("Sample locator #%-2d:\t[%f, %f, %f]\n",
          view_ind,
          loc_pos.x,
          loc_pos.y,
          loc_pos.z);
}

void sample_stroke(Pint ws_id)
{
   int i;
   Pint view_ind;
   Ppoint3 stroke_points[100];
   Ppoint_list3 stroke = {0, stroke_points};

   psample_stroke3(ws_id, 1, &view_ind, &stroke);
   printf("Sample stroke #%-2d:\n", view_ind);
   for (i = 0; i < stroke.num_points; i++) {
      printf("\t[%f, %f, %f]\n",
             stroke.points[i].x,
             stroke.points[i].y,
             stroke.points[i].z);
   }
}

void sample_pick(Pint ws_id)
{
   int i;
   Pin_status status;
   Ppick_path_elem path_list[10];
   Ppick_path pick = {0, path_list};

   psample_pick(ws_id, 1, 10, &status, &pick);
   if (status == PIN_STATUS_OK) {
      printf("Sample pick #%-d:\n", pick.depth);
      for (i = 0; i < pick.depth; i++) {
         printf("\tStruct = %d\tPick id = %d\t Element position = %d\n",
                pick.path_list[i].struct_id,
                pick.path_list[i].pick_id,
                pick.path_list[i].elem_pos);
      }
   }
}

int locator_event(void)
{
   int ret;
   Pin_class class;
   Pint ws_id, in_num, view_ind;
   Ppoint3 loc_pos;

   if (SIN_Q_EMPTY(PHG_INPUT_Q)) {
      ret = FALSE;
   }
   else {
      pawait_event(1.0, &ws_id, &class, &in_num);
      pget_loc3(&view_ind, &loc_pos);
      printf("Locator event #%-2d:\t[%f, %f, %f]\n",
             view_ind,
             loc_pos.x,
             loc_pos.y,
             loc_pos.z);
      ret = TRUE;
   }

   return ret;
}

int stroke_event(void)
{
   int i, ret;
   Pin_class class;
   Pint ws_id, in_num, view_ind;
   Ppoint3 stroke_points[100];
   Ppoint_list3 stroke = {0, stroke_points};

   if (SIN_Q_EMPTY(PHG_INPUT_Q)) {
      ret = FALSE;
   }
   else {
      pawait_event(1.0, &ws_id, &class, &in_num);
      pget_stroke3(&view_ind, &stroke);
      printf("Stroke event #%-2d:\n", view_ind);
      for (i = 0; i < stroke.num_points; i++) {
         printf("\t[%f, %f, %f]\n",
                stroke.points[i].x,
                stroke.points[i].y,
                stroke.points[i].z);
      }
      ret = TRUE;
   }

   return ret;
}

int pick_event(void)
{
   int i, ret;
   Pin_class class;
   Pint ws_id, in_num;
   Pin_status status;
   Ppick_path_elem path_list[10];
   Ppick_path pick = {0, path_list};

   if (SIN_Q_EMPTY(PHG_INPUT_Q)) {
      ret = FALSE;
   }
   else {
      pawait_event(1.0, &ws_id, &class, &in_num);
      pget_pick(10, &status, &pick);
      if (status == PIN_STATUS_OK) {
         printf("Pick event #%-d:\n", pick.depth);
         for (i = 0; i < pick.depth; i++) {
            printf("\tStruct = %d\tPick id = %d\t Element position = %d\n",
                   pick.path_list[i].struct_id,
                   pick.path_list[i].pick_id,
                   pick.path_list[i].elem_pos);
         }
         ret = TRUE;
      }
      else {
         ret = FALSE;
      }
   }

   return ret;
}

int main(void)
{
   Ws *wsh;
   XEvent event;

   popen_phigs(NULL, 0);
   popen_ws(WS_0, NULL, PWST_OUTIN_TRUE_DB);
   pset_ws_win(WS_0, &win);

   wsh = PHG_WSID(WS_0);
   printf("Output window %x\n", (unsigned) wsh->drawable_id);
   printf("Input  window %x\n", (unsigned) wsh->input_overlay_window);

   blue.type = PMODEL_RGB;
   blue.val.general.x = 0.0;
   blue.val.general.y = 0.0;
   blue.val.general.z = 1.0;

   popen_struct(STRUCT_0);
   pset_int_colr(&blue);
   pfill_area(&quad);
   pclose_struct();

   ppost_struct(WS_0, STRUCT_0, 0);

   //pset_loc_mode(WS_0, 1, POP_SAMPLE, PSWITCH_NO_ECHO);
   pset_loc_mode(WS_0, 1, POP_EVENT, PSWITCH_NO_ECHO);
   //pset_stroke_mode(WS_0, 1, POP_SAMPLE, PSWITCH_NO_ECHO);
   //pset_stroke_mode(WS_0, 1, POP_EVENT, PSWITCH_NO_ECHO);
   //pset_pick_mode(WS_0, 1, POP_SAMPLE, PSWITCH_NO_ECHO);
   //pset_pick_mode(WS_0, 1, POP_EVENT, PSWITCH_NO_ECHO);

   if (wsh != NULL) {
      while (1) {
         XNextEvent(wsh->display, &event);
         //print_event(&event);
         phg_sin_evt_dispatch(PHG_EVT_TABLE, wsh->display, &event);
         if (event.type == Expose) {
            while (XCheckTypedEvent(wsh->display, Expose, &event));
            (*wsh->redraw_all)(wsh, PFLAG_ALWAYS);
         }
         //sample_locator(WS_0);
         locator_event();
         //sample_stroke(WS_0);
         //stroke_event();
         //sample_pick(WS_0);
         //pick_event();
      }
   }

   return 1;
}

