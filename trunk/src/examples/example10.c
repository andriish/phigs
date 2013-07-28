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
#include <phigs/private/phgP.h>
#include <phigs/css.h>
#include <phigs/ws.h>

#define WIDTH     0.5
#define HEIGHT    0.5

Ppoint pts_mark[] = {
   {WIDTH / 4, HEIGHT + HEIGHT / 4}
};

Ppoint pts_line[] = {
   {0.0, HEIGHT * 2},
   {WIDTH * 2, 0.0}
};

Ppoint pts_tri[] = {
   {WIDTH, HEIGHT},
   {WIDTH * 2, HEIGHT * 2},
   {WIDTH, HEIGHT * 2}
};

Ppoint pts_quad[] = {
   {0.0, 0.0},
   {WIDTH, 0.0},
   {WIDTH, HEIGHT},
   {0.0, HEIGHT}
};

Ppoint_list plist_mark = {
   1, pts_mark
};

Ppoint_list plist_line = {
   2, pts_line
};

Ppoint_list plist_tri = {
   3, pts_tri
};

Ppoint_list plist_quad = {
   4, pts_quad
};

Pcolr_rep col_rep;
int view_index = 0;
Pgcolr colour1, colour2;

int main(int argc, char *argv[])
{
   Plimit vp, win;
   XEvent event;

   if (argc > 1) {
      view_index = atoi(argv[1]);
      printf("Use view: %d\n", view_index);
   }

   popen_phigs(NULL, 0);

   colour1.type = PMODEL_RGB;
   colour1.val.general.x = 0.0;
   colour1.val.general.y = 1.0;
   colour1.val.general.z = 1.0;

   colour2.type = PMODEL_RGB;
   colour2.val.general.x = 1.0;
   colour2.val.general.y = 1.0;
   colour2.val.general.z = 0.0;

   popen_struct(0);
   ppolyline(&plist_line);
   ppolymarker(&plist_mark);
   pclose_struct();

   popen_struct(1);
   pfill_area(&plist_quad);
   pclose_struct();

   popen_struct(2);
   pfill_area(&plist_tri);
   pclose_struct();

   popen_struct(3);
   pset_view_ind(view_index);
   //pset_int_colr(&colour1);
   pset_int_colr_ind(0);
   pexec_struct(1);
   pset_indiv_asf(PASPECT_INT_COLR_IND, PASF_BUNDLED);
   //pset_int_colr(&colour2);
   pset_int_colr_ind(1);
   pset_indiv_asf(PASPECT_INT_COLR_IND, PASF_INDIV);
   pexec_struct(2);
   pclose_struct();

   popen_ws(0, NULL, PWST_OUTPUT_TRUE_DB);
   vp.x_min =   0.0;
   vp.x_max = 400.0;
   vp.y_min =   0.0;
   vp.y_max = 400.0;
   win.x_min =  0.0;
   win.x_max =  1.0;
   win.y_min =  0.0;
   win.y_max =  1.0;
   pset_ws_vp(0, &vp);
   pset_ws_win(0, &win);

   col_rep.rgb.red = 0.0;
   col_rep.rgb.green = 1.0;
   col_rep.rgb.blue = 1.0;
   pset_colr_rep(0, 0, &col_rep);

   col_rep.rgb.red = 1.0;
   col_rep.rgb.green = 1.0;
   col_rep.rgb.blue = 0.0;
   pset_colr_rep(0, 1, &col_rep);

   ppost_struct(0, 3, 0);

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
         break;

         default:
         break;
      }
   }

   (*PHG_WSID(0)->close)(PHG_WSID(0));

   return 0;
}

