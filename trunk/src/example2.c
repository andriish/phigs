#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <phigs/phg.h>
#include <phigs/css.h>
#include <phigs/ws.h>

#define WIDTH  1.0
#define HEIGHT 1.0

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

int main(void)
{
   XEvent event;

   popen_phigs("", 0);
   popen_ws(0, NULL, 0);
   ws->out_ws.model.b.ws_viewport_pending = PUPD_PEND;
   ws->out_ws.model.b.req_ws_viewport.x_min = 0;
   ws->out_ws.model.b.req_ws_viewport.x_max = 600;
   ws->out_ws.model.b.req_ws_viewport.y_min = 0;
   ws->out_ws.model.b.req_ws_viewport.y_max = 600;
   (*ws->redraw_all)(ws, PFLAG_ALWAYS);

   col_rep.rgb.red = 1.0;
   col_rep.rgb.green = 1.0;
   col_rep.rgb.blue = 1.0;
   pset_colr_rep(0, 0, &col_rep);

   popen_struct(0);
   pset_linetype(PLINE_DASH);
   pset_linewidth(4.0);
   ppolyline(&plist_line);
   pset_marker_type(PMARKER_CROSS);
   pset_marker_size(0.1);
   ppolymarker(&plist_mark);
   pclose_struct();

   popen_struct(1);
   pfill_area(&plist_quad);
   pclose_struct();

   popen_struct(2);
   pfill_area(&plist_tri);
   pclose_struct();

   popen_struct(3);
   pset_view_ind3(0);
   pset_int_colr_ind(0);
   pexec_struct(0);
   pexec_struct(1);
   //pexec_struct(2);
   pclose_struct();

   printf("Post struct...");
   ppost_struct(0, 3, 0);
   printf("Done.\n");

   XSelectInput(ws->display,
                ws->drawable_id,
                ExposureMask | KeyPressMask | ButtonPressMask);
   while (1) {
      XNextEvent(ws->display, &event);
      switch(event.type) {
         case Expose:
            while (XCheckTypedEvent(ws->display, Expose, &event));
            (*ws->redraw_all)(ws, PFLAG_ALWAYS);
         break;

         case KeyPress:
            popen_struct(3);
            printf("Exec struct...");
            pexec_struct(2);
            printf("Done.\n");
            printf("Close struct...");
            pclose_struct();
            printf("Done.\n");
         break;

         case ButtonPress:
            goto end_prog;
         break;

         default:
         break;
      }
   }

end_prog:
   (*ws->close)(ws);
   phg_css_destroy(css);
}

