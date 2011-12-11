#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
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
int view_index;

int main(int argc, char *argv[])
{
   XEvent event;

   if (argc > 1) {
      view_index = atoi(argv[1]);
      printf("Use view: %d\n", view_index);
   }

   popen_phigs("", 0);

   popen_struct(0);
   pset_linetype(PLINE_DASH);
   pset_linewidth(4.0);
   ppolyline(&plist_line);
   pset_marker_type(PMARKER_ASTERISK);
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
   pset_view_ind3(view_index);
   pset_int_colr_ind(0);
   pexec_struct(0);
   pexec_struct(1);
   //pexec_struct(2);
   pclose_struct();

   popen_ws(0, NULL, 0);
   ws_list[0]->out_ws.model.b.ws_viewport_pending = PUPD_PEND;
   ws_list[0]->out_ws.model.b.req_ws_viewport.x_min = 0;
   ws_list[0]->out_ws.model.b.req_ws_viewport.x_max = 300;
   ws_list[0]->out_ws.model.b.req_ws_viewport.y_min = 0;
   ws_list[0]->out_ws.model.b.req_ws_viewport.y_max = 300;
   (*ws_list[0]->redraw_all)(ws_list[0], PFLAG_ALWAYS);
   printf("Created workspace: %x\n", (unsigned int) ws_list[0]);

   col_rep.rgb.red = 1.0;
   col_rep.rgb.green = 1.0;
   col_rep.rgb.blue = 1.0;
   pset_colr_rep(0, 0, &col_rep);

   popen_ws(1, NULL, 0);
   ws_list[1]->out_ws.model.b.ws_viewport_pending = PUPD_PEND;
   ws_list[1]->out_ws.model.b.req_ws_viewport.x_min = 0;
   ws_list[1]->out_ws.model.b.req_ws_viewport.x_max = 300;
   ws_list[1]->out_ws.model.b.req_ws_viewport.y_min = 0;
   ws_list[1]->out_ws.model.b.req_ws_viewport.y_max = 300;
   (*ws_list[1]->redraw_all)(ws_list[1], PFLAG_ALWAYS);
   printf("Created workspace: %x\n", (unsigned int) ws_list[1]);

   col_rep.rgb.red = 1.0;
   col_rep.rgb.green = 1.0;
   col_rep.rgb.blue = 1.0;
   pset_colr_rep(1, 0, &col_rep);

   printf("Post struct...");
   ppost_struct(0, 3, 0);
   ppost_struct(1, 3, 0);
   printf("Done.\n");

   XSelectInput(ws_list[0]->display,
                ws_list[0]->drawable_id,
                ExposureMask | KeyPressMask | ButtonPressMask);
   while (1) {
      XNextEvent(ws_list[0]->display, &event);
      switch(event.type) {
         case Expose:
            while (XCheckTypedEvent(ws_list[0]->display, Expose, &event));
            (*ws_list[0]->redraw_all)(ws_list[0], PFLAG_ALWAYS);
            (*ws_list[1]->redraw_all)(ws_list[1], PFLAG_ALWAYS);
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
   (*ws_list[0]->close)(ws_list[0]);
   (*ws_list[1]->close)(ws_list[1]);
   phg_css_destroy(css);
}

