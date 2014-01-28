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
#include <sys/types.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/css.h>

#define WIDTH   0.8
#define HEIGHT  0.8
#define SPACE   0.9
#define DEPTH  -0.2
#define LOW     0.2

#define FILL_STYLE PSTYLE_SOLID
#define FILL_STYLE_IND 4
#define EDGE_WIDTH 2.0

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

Pint errnum;
Pvec3 tvec3;
Pmatrix3 tran3;
Pmatrix3 rot3;
int view_index = 5;
Pcolr_rep col_rep;

void draw_shape(void)
{
   popen_struct(0);
   pfill_area3(&plist_quad);
   ppolymarker3(&plist_quad);
   pclose_struct();

   tvec3.delta_x = 0.0;
   tvec3.delta_y = 0.0;
   tvec3.delta_z = DEPTH;
   ptranslate3(&tvec3, &errnum, tran3);
   protate_x(-3.14 / 4.0, &errnum, rot3);

   popen_struct(1);
   pset_view_ind(view_index);
   pset_hlhsr_id(PHIGS_HLHSR_ID_ON);
   pset_edge_flag(PEDGE_ON);
   pset_edge_colr_ind(3);
   pset_edgewidth(EDGE_WIDTH);
   pset_edgetype(PLINE_SOLID);
   pset_int_colr_ind(0);
   pset_int_style(FILL_STYLE);
   pset_int_style_ind(FILL_STYLE_IND);
   pset_marker_type(PMARKER_CROSS);
   pset_marker_size(0.1);
   pset_marker_colr_ind(4);
   pset_local_tran3(tran3, PTYPE_REPLACE);
   pexec_struct(0);
   tvec3.delta_z += SPACE;
   ptranslate3(&tvec3, &errnum, tran3);
   pset_local_tran3(rot3, PTYPE_REPLACE);
   pset_local_tran3(tran3, PTYPE_POSTCONCAT);
   pset_int_colr_ind(1);
   pexec_struct(0);
   tvec3.delta_z += SPACE;
   ptranslate3(&tvec3, &errnum, tran3);
   pset_local_tran3(rot3, PTYPE_REPLACE);
   pset_local_tran3(rot3, PTYPE_POSTCONCAT);
   plabel(10);
   pset_local_tran3(tran3, PTYPE_POSTCONCAT);
   pset_int_colr_ind(2);
   plabel(20);
   pexec_struct(0);
   plabel(30);
   pclose_struct();
}

int main(int argc, char *argv[])
{
   XEvent event;
   KeySym ks;
   Plimit3 vp, win;

   if (argc > 1) {
      view_index = atoi(argv[1]);
      printf("Use view: %d\n", view_index);
   }

   popen_phigs(NULL, 0);

   popen_ws(0, NULL, PWST_OUTPUT_TRUE_DB);
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
   pset_hlhsr_mode(0, PHIGS_HLHSR_MODE_ZBUFF);

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
   pset_disp_upd_st(0, PDEFER_BNIL, PMODE_UQUM);

   XSelectInput(PHG_WSID(0)->display,
                PHG_WSID(0)->drawable_id,
                ExposureMask | KeyPressMask);
   while (1) {
      XNextEvent(PHG_WSID(0)->display, &event);
      switch(event.type) {

         case Expose:
            while (XCheckTypedEvent(PHG_WSID(0)->display, Expose, &event));
            predraw_all_structs(0, PFLAG_ALWAYS);
            break;

         case KeyPress:
            ks = XLookupKeysym((XKeyEvent *) &event, 0);
            if (ks == XK_d) {
               draw_shape();
               ppost_struct(0, 1, 0);
               pupd_ws(0, PFLAG_PERFORM);
            }
            else if (ks == XK_w) {
               popen_ar_file(0, "archive.phg");
               par_all_structs(0);
               pclose_ar_file(0);
            }
            else if (ks == XK_Escape) {
               goto exit;
            }
            break;

         default:
            break;
      }
   }

exit:
   pclose_ws(0);
   pclose_phigs();

   return 0;
}

