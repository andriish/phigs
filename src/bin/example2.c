#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/css.h>
#include <phigs/ws.h>

#define WIDTH  0.5
#define HEIGHT 0.5

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
int view_index = 1;

int main(int argc, char *argv[])
{
   Plimit vp, win;
   XEvent event;

   if (argc > 1) {
      view_index = atoi(argv[1]);
      printf("Use view: %d\n", view_index);
   }

   popen_phigs(NULL, 0);

   popen_struct(0);
   pset_linetype(PLINE_DASH);
   pset_linewidth(4.0);
   pset_line_colr_ind(0);
   ppolyline(&plist_line);
   pset_marker_type(PMARKER_ASTERISK);
   pset_marker_size(0.1);
   pset_marker_colr_ind(0);
   ppolymarker(&plist_mark);
   pclose_struct();

   popen_struct(1);
   pset_edge_flag(PEDGE_ON);
   pset_edge_colr_ind(1);
   pset_edgewidth(2.0);
   pset_int_colr_ind(0);
   pset_int_style(PSTYLE_SOLID);
   pfill_area(&plist_quad);
   pclose_struct();

   popen_struct(2);
   pset_edge_flag(PEDGE_ON);
   pset_edge_colr_ind(1);
   pset_edgewidth(2.0);
   pset_int_colr_ind(0);
   pset_int_style(PSTYLE_SOLID);
   pfill_area(&plist_tri);
   pclose_struct();

   popen_struct(3);
   pset_view_ind(view_index);
   pset_int_colr_ind(0);
   pexec_struct(0);
   pexec_struct(1);
   pclose_struct();

   popen_ws(0, NULL, PWST_OUTPUT_TRUE);
   vp.x_min =   0.0;
   vp.x_max = 200.0;
   vp.y_min =   0.0;
   vp.y_max = 200.0;
   win.x_min =  0.0;
   win.x_max =  0.5;
   win.y_min =  0.0;
   win.y_max =  0.5;
   pset_ws_vp(0, &vp);
   pset_ws_win(0, &win);

   col_rep.rgb.red = 1.0;
   col_rep.rgb.green = 1.0;
   col_rep.rgb.blue = 1.0;
   pset_colr_rep(0, 0, &col_rep);

   col_rep.rgb.red = 0.0;
   col_rep.rgb.green = 1.0;
   col_rep.rgb.blue = 0.0;
   pset_colr_rep(0, 1, &col_rep);

   popen_ws(1, NULL, PWST_OUTPUT_TRUE);
   vp.x_min =   0.0;
   vp.x_max = 300.0;
   vp.y_min =   0.0;
   vp.y_max = 300.0;
   win.x_min =  0.0;
   win.x_max =  1.0;
   win.y_min =  0.0;
   win.y_max =  1.0;
   pset_ws_vp(1, &vp);
   pset_ws_win(1, &win);

   col_rep.rgb.red = 0.0;
   col_rep.rgb.green = 1.0;
   col_rep.rgb.blue = 1.0;
   pset_colr_rep(1, 0, &col_rep);

   col_rep.rgb.red = 1.0;
   col_rep.rgb.green = 0.0;
   col_rep.rgb.blue = 1.0;
   pset_colr_rep(1, 1, &col_rep);

   ppost_struct(0, 3, 0);
   ppost_struct(1, 3, 0);

   XSelectInput(PHG_WSID(0)->display,
                PHG_WSID(0)->drawable_id,
                ExposureMask | KeyPressMask);
   while (1) {
      XNextEvent(PHG_WSID(0)->display, &event);
      switch(event.type) {
         case Expose:
            while (XCheckTypedEvent(PHG_WSID(0)->display, Expose, &event));
            (*PHG_WSID(0)->redraw_all)(PHG_WSID(0), PFLAG_ALWAYS);
            (*PHG_WSID(1)->redraw_all)(PHG_WSID(1), PFLAG_ALWAYS);
         break;

         case KeyPress:
            popen_struct(3);
            pexec_struct(2);
            pclose_struct();
         break;

         default:
         break;
      }
   }

   (*PHG_WSID(0)->close)(PHG_WSID(0));
   (*PHG_WSID(1)->close)(PHG_WSID(1));

   return 0;
}

