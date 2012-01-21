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
#include <string.h>
#include <phigs/phg.h>
#include <phigs/private/wsxP.h>

#define STRUCT_OBJECT  0
#define STRUCT_SCENE   1
#define STRUCT_MAIN    2

#define WS_MAIN        0

#define WIDTH          0.5
#define HEIGHT         0.5
#define SPACE          0.6
#define DEPTH         -0.2
#define LOW            0.2

#define FILL_STYLE     PSTYLE_SOLID
#define FILL_STYLE_IND 4
#define EDGE_WIDTH     2.0

#define WIN_X0         0.0
#define WIN_X1         1.0
#define WIN_Y0         0.0
#define WIN_Y1         1.0

Ppoint3 pts_quad[] = {
   {0.0, 0.0, 0.0},
   {WIDTH, 0.0, 0.0},
   {WIDTH, HEIGHT, 0.0},
   {0.0, HEIGHT, 0.0}
};

Ppoint_list3 plist_quad = {
   4, pts_quad
};

Ppoint pts_vline[] = {
   {0.0, 0.4},
   {1.0, 0.4}
};

Ppoint_list plist_vline ={
   2, pts_vline
};

Ppoint pts_hline[] = {
   {0.4, 0.0},
   {0.4, 1.0}
};

Ppoint_list plist_hline ={
   2, pts_hline
};

Pint errnum;
Pvec3 tvec3;
Pmatrix3 tran3;
Pmatrix3 rot3;
Pcolr_rep col_rep;
Pgcolr dark, medium, light, white, red;
Plimit win = {WIN_X0, WIN_X1, WIN_Y0, WIN_Y1};
Ppoint3 stroke_points[100];
Ppoint_list3 stroke = {0, stroke_points};
Ppick_path_elem path_list[10];
Ppick_path pick = {0, path_list};

void init_scene(void)
{
   dark.type = PMODEL_RGB;
   dark.val.general.x = 0.0;
   dark.val.general.y = 0.25;
   dark.val.general.z = 0.25;

   medium.type = PMODEL_RGB;
   medium.val.general.x = 0.0;
   medium.val.general.y = 0.5;
   medium.val.general.z = 0.5;

   light.type = PMODEL_RGB;
   light.val.general.x = 0.0;
   light.val.general.y = 0.5;
   light.val.general.z = 0.5;

   white.type = PMODEL_RGB;
   white.val.general.x = 1.0;
   white.val.general.y = 1.0;
   white.val.general.z = 1.0;

   red.type = PMODEL_RGB;
   red.val.general.x = 1.0;
   red.val.general.y = 0.0;
   red.val.general.z = 0.0;

   popen_struct(STRUCT_OBJECT);
   pfill_area3(&plist_quad);
   ppolymarker3(&plist_quad);
   pclose_struct();

   tvec3.delta_x = 0.0;
   tvec3.delta_y = 0.0;
   tvec3.delta_z = DEPTH;
   ptranslate3(&tvec3, &errnum, tran3);
   protate_x(-3.14 / 4.0, &errnum, rot3);

   popen_struct(STRUCT_SCENE);
   pset_edge_flag(PEDGE_ON);
   pset_edge_colr(&white);
   pset_edgewidth(EDGE_WIDTH);
   pset_edgetype(PLINE_SOLID);
   pset_int_colr(&dark);
   pset_int_style(FILL_STYLE);
   pset_int_style_ind(FILL_STYLE_IND);
   pset_marker_type(PMARKER_CROSS);
   pset_marker_size(0.1);
   pset_marker_colr(&red);
   pset_local_tran3(tran3, PTYPE_REPLACE);
   pexec_struct(0);
   tvec3.delta_z += SPACE;
   ptranslate3(&tvec3, &errnum, tran3);
   pset_local_tran3(rot3, PTYPE_REPLACE);
   pset_local_tran3(tran3, PTYPE_POSTCONCAT);
   pset_int_colr(&medium);
   pexec_struct(0);
   tvec3.delta_z += SPACE;
   ptranslate3(&tvec3, &errnum, tran3);
   pset_local_tran3(rot3, PTYPE_REPLACE);
   pset_local_tran3(rot3, PTYPE_POSTCONCAT);
   plabel(10);
   pset_local_tran3(tran3, PTYPE_POSTCONCAT);
   pset_int_colr(&light);
   plabel(20);
   pexec_struct(0);
   plabel(30);
   pclose_struct();

   popen_struct(STRUCT_MAIN);
   ppolyline(&plist_vline);
   ppolyline(&plist_hline);
   pset_hlhsr_id(PHIGS_HLHSR_ID_ON);
   pset_view_ind(1);
   pexec_struct(STRUCT_SCENE);
   pset_view_ind(2);
   pexec_struct(STRUCT_SCENE);
   pset_view_ind(3);
   pexec_struct(STRUCT_SCENE);
   pset_view_ind(4);
   pexec_struct(STRUCT_SCENE);
   pclose_struct();
}

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

   pawait_event(0.1, &ws_id, &class, &in_num);
   if (class != PIN_NONE) {
      pget_loc3(&view_ind, &loc_pos);
      printf("Locator event #%-2d:\t[%f, %f, %f]\n",
             view_ind,
             loc_pos.x,
             loc_pos.y,
             loc_pos.z);
      ret = TRUE;
   }
   else {
      ret = FALSE;
   }

   return ret;
}

int stroke_event(void)
{
   int i, ret;
   Pin_class class;
   Pint ws_id, in_num, view_ind;

   pawait_event(0.1, &ws_id, &class, &in_num);
   if (class != PIN_NONE) {
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
   else {
      ret = FALSE;
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

   pawait_event(0.1, &ws_id, &class, &in_num);
   if (class != PIN_NONE) {
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
   else {
      ret = FALSE;
   }

   return ret;
}

void print_size(Pint ws_type)
{
   Pint err_ind;
   Pdisp_space_size3 size;

   pinq_disp_space_size3(ws_type, &err_ind, &size);
   if (err_ind == 0) {
      printf("Display device size:\t%f x %f x %f\n",
             size.size_dc.size_x,
             size.size_dc.size_y,
             size.size_dc.size_z);
      printf("Display raster size:\t%d x %d x %d\n",
             size.size_raster.size_x,
             size.size_raster.size_y,
             size.size_raster.size_z);
   }
   else {
      ERR_REPORT(PHG_ERH, err_ind);
   }
}

int main(void)
{
   Ws *wsh;
   XEvent event;
   int redraw = 0;

   popen_phigs(NULL, 0);
   init_scene();
   print_size(PWST_OUTIN_TRUE_DB);
   popen_ws(WS_MAIN, NULL, PWST_OUTIN_TRUE_DB);
   pset_ws_win(WS_MAIN, &win);
   pset_hlhsr_mode(WS_MAIN, PHIGS_HLHSR_MODE_ZBUFF);
   pset_view_tran_in_pri(WS_MAIN, 0, 4, PPRI_LOWER);
   ppost_struct(WS_MAIN, STRUCT_MAIN, 0);

   wsh = PHG_WSID(WS_MAIN);
   
   if (wsh != NULL) {
      printf("Output window %x\n", (unsigned) wsh->drawable_id);
      printf("Input  window %x\n", (unsigned) wsh->input_overlay_window);

      XSelectInput(wsh->display, wsh->drawable_id, ExposureMask);

      pset_loc_mode(WS_MAIN, 1, POP_EVENT, PSWITCH_NO_ECHO);

      while (1) {
         if (XCheckWindowEvent(wsh->display,
                               wsh->drawable_id,
                               (unsigned long) 0xffffffffUL,
                               &event) == True) {
#if 0
            print_event(&event);
#endif
            if (event.type == Expose) {
               while (XCheckTypedEvent(wsh->display, Expose, &event));
               redraw = 1;
            }
         }
         locator_event();
         if (redraw) {
            predraw_all_structs(WS_MAIN, PFLAG_ALWAYS);
            redraw = 0;
         }
      }
   }

   return 1;
}
