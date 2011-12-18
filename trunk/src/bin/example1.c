/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2011 Surplus Users Ham Society
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
#include <phigs/private/phgP.h>
#include <phigs/css.h>
#include <phigs/mat_utils.h>

#define WIDTH  0.9
#define HEIGHT 0.9
#define SPACE  1.0

#define VP_X0    0.0
#define VP_X1  500.0
#define VP_Y0    0.0
#define VP_Y1  500.0
#define WIN_X0   0.0
#define WIN_X1   1.0
#define WIN_Y0   0.0
#define WIN_Y1   1.0

Ppoint3 pts_quad[] = {
   {0.0, 0.0, 0.0},
   {WIDTH, 0.0, 0.0},
   {WIDTH, HEIGHT, 0.0},
   {0.0, HEIGHT, 0.0}
};

Ppoint_list3 plist_quad = {
   4, pts_quad
};

Pmatrix3 tran3;
Pmatrix3 rot3;
int view_index = 5;
Pcolr_rep col_rep;

void struct_stat(void)
{
   Pint index, err;
   Popen_struct_status status;

   pinq_open_struct(&err, &status, &index);
   if (!err && status == PSTRUCT_OPEN)
      printf("Open structure id: %d\n", index);
   else
      printf("No structure open\n");

   pinq_elem_ptr(&err, &index);
   if (!err)
      printf("Label found at: %d\n", index);
   else
      printf("Label not found\n");
}

int main(int argc, char *argv[])
{
   XEvent event;
   Plimit3 vp, win;

   if (argc > 1) {
      view_index = atoi(argv[1]);
      printf("Use view: %d\n", view_index);
   }

   popen_phigs(NULL, 0);

   popen_struct(0);
   pfill_area3(&plist_quad);
   ppolymarker3(&plist_quad);
   pclose_struct();

   phg_mat_trans(tran3, 0.0, 0.0, 0.0);
   phg_mat_rot_x(rot3, 3.14 / 4.0);

   popen_struct(1);
   pset_view_ind3(view_index);
   pset_edge_flag(PEDGE_ON);
   pset_edge_colr_ind(3);
   pset_edgewidth(2.0);
   pset_int_colr_ind(0);
   pset_int_style(PSTYLE_SOLID);
   pset_marker_type(PMARKER_CROSS);
   pset_marker_size(0.1);
   pset_marker_colr_ind(4);
   pset_local_tran3(tran3, PTYPE_REPLACE);
   pexec_struct(0);
   phg_mat_trans(tran3, 0.0, 0.0, SPACE);
   pset_local_tran3(rot3, PTYPE_REPLACE);
   pset_local_tran3(tran3, PTYPE_POSTCONCAT);
   pset_int_colr_ind(1);
   pexec_struct(0);
   phg_mat_trans(tran3, 0.0, 0.0, 2 * SPACE);
   pset_local_tran3(rot3, PTYPE_REPLACE);
   pset_local_tran3(rot3, PTYPE_POSTCONCAT);
   plabel(10);
   pset_local_tran3(tran3, PTYPE_POSTCONCAT);
   pset_int_colr_ind(2);
   plabel(20);
   pexec_struct(0);
   pclose_struct();

   popen_ws(0, NULL, phigs_ws_type_glx_drawable);
   vp.x_min = VP_X0;
   vp.x_max = VP_X1;
   vp.y_min = VP_Y0;
   vp.y_max = VP_Y1;
   vp.z_min = 0.0;
   vp.z_max = 1.0;
   win.x_min = WIN_X0;
   win.x_max = WIN_X1;
   win.y_min = WIN_Y0;
   win.y_max = WIN_Y1;
   win.z_min = 0.0;
   win.z_max = 1.0;
   pset_ws_vp3(0, &vp);
   pset_ws_win3(0, &win);

   col_rep.rgb.red = 0.0;
   col_rep.rgb.green = 0.25;
   col_rep.rgb.blue = 0.25;
   pset_colr_rep(0, 0, &col_rep);

   col_rep.rgb.red = 0.0;
   col_rep.rgb.green = 0.5;
   col_rep.rgb.blue = 0.5;
   pset_colr_rep(0, 1, &col_rep);

   col_rep.rgb.red = 0.0;
   col_rep.rgb.green = 1.0;
   col_rep.rgb.blue = 1.0;
   pset_colr_rep(0, 2, &col_rep);

   col_rep.rgb.red = 1.0;
   col_rep.rgb.green = 1.0;
   col_rep.rgb.blue = 1.0;
   pset_colr_rep(0, 3, &col_rep);

   col_rep.rgb.red = 1.0;
   col_rep.rgb.green = 0.0;
   col_rep.rgb.blue = 0.0;
   pset_colr_rep(0, 4, &col_rep);

   ppost_struct(0, 1, 0);

   XSelectInput(ws_list[0]->display,
                ws_list[0]->drawable_id,
                ExposureMask | KeyPressMask);
   while (1) {
      XNextEvent(ws_list[0]->display, &event);
      switch(event.type) {

         case Expose:
            while (XCheckTypedEvent(ws_list[0]->display, Expose, &event));
            (*ws_list[0]->redraw_all)(ws_list[0], PFLAG_ALWAYS);
         break;

         case KeyPress:
            popen_struct(1);
            phg_mat_trans(tran3, 0.0, -SPACE, 2 * SPACE);
            pset_elem_ptr(0);
            pset_elem_ptr_label(10);
            struct_stat();
            css->edit_mode = PEDIT_REPLACE;
            poffset_elem_ptr(1);
            pset_local_tran3(tran3, PTYPE_POSTCONCAT);
            poffset_elem_ptr(1);
            pset_int_colr_ind(4);

#if 0
            pset_elem_ptr(0);
            //pdel_elem_range(19, 20);
            pdel_elems_labels(10, 20);
            css->edit_mode = PEDIT_INSERT;
            phg_css_print_struct(css->open_struct, 0);
#endif
            pset_elem_ptr(4);
            css->edit_mode = PEDIT_INSERT;
            phg_mat_trans(tran3, -SPACE, 0.0, 0.0);
            pset_local_tran3(tran3, PTYPE_REPLACE);
            pcopy_all_elems_struct(0);
            //phg_css_print_struct(css->open_struct, 0);
            pclose_struct();
         break;

         default:
         break;
      }
   }

   (*ws_list[0]->close)(ws_list[0]);
   phg_css_destroy(css);

   return 0;
}

