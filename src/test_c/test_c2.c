/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2014 Surplus Users Ham Society
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

#include "phg.h"
#include "eldata.h"

#define WIDTH     0.5
#define HEIGHT    0.5
#define ATTR_NO   1
#define HATCH_IND 6

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

Ppoint      text_pos   = {0.1, 0.35};
Ptext_align text_align = {PHOR_NORM, PVERT_NORM};
Pvec        text_up    = {0.5, 0.866};

Pcolr_rep col_rep;
int view_index = 0;

Pline_bundle   lnrep = { PLINE_DASH_DOT, 2.0,  0 };
Pmarker_bundle mkrep = { PMARKER_ASTERISK, 0.1, 0 };
Ptext_bundle   txrep = { 1, PREC_STROKE, 1.0, 0, 1 };
Pedge_bundle   edgerep = { PEDGE_ON, PLINE_SOLID, 2.0, 1 };
Pint_bundle    interrep = { PSTYLE_SOLID, HATCH_IND, 0 };

void lnlist(Pint ws_id)
{
   Pint i;
   Pint len;
   Pint err;
   Pint_list list;
   Pint int_list[10];
   Pline_bundle rep;

   list.num_ints = 0;
   list.ints = int_list;

   pinq_list_line_inds(ws_id, 10, ATTR_NO, &err, &list, &len);
   printf("line\ttype\twidth\tcolr\n");
   for (i = 0; i < list.num_ints; i++) {
      printf("#%d:\t", list.ints[i]);
      pinq_line_rep(ws_id, list.ints[i], 0, &err, &rep);
      if (!err)
         printf("%d\t%g\t%d", rep.type, rep.width, rep.colr_ind);
      else
         printf("ERR%d", err);
      printf("\n");
   }
}

void mklist(Pint ws_id)
{
   Pint i;
   Pint len;
   Pint err;
   Pint_list list;
   Pint int_list[10];
   Pmarker_bundle rep;

   list.num_ints = 0;
   list.ints = int_list;

   pinq_list_marker_inds(ws_id, 10, ATTR_NO, &err, &list, &len);
   printf("marker\ttype\tsize\tcolr\n");
   for (i = 0; i < list.num_ints; i++) {
      printf("#%d:\t", list.ints[i]);
      pinq_marker_rep(ws_id, list.ints[i], 0, &err, &rep);
      if (!err)
         printf("%d\t%g\t%d", rep.type, rep.size, rep.colr_ind);
      else
         printf("ERR%d", err);
      printf("\n");
   }
}

void txlist(Pint ws_id)
{
   Pint i;
   Pint len;
   Pint err;
   Pint_list list;
   Pint int_list[10];
   Ptext_bundle rep;

   list.num_ints = 0;
   list.ints = int_list;

   pinq_list_text_inds(ws_id, 10, ATTR_NO, &err, &list, &len);
   printf("text\tfont\tprec\texpan\tspace\tcolr\n");
   for (i = 0; i < list.num_ints; i++) {
      printf("#%d:\t", list.ints[i]);
      pinq_text_rep(ws_id, list.ints[i], 0, &err, &rep);
      if (!err)
         printf("%d\t%d\t%g\t%g\t%d", rep.font, rep.prec,
                rep.char_expan, rep.char_space, rep.colr_ind);
      else
         printf("ERR%d", err);
      printf("\n");
   }
}

void intlist(Pint ws_id)
{
   Pint i;
   Pint len;
   Pint err;
   Pint_list list;
   Pint int_list[10];
   Pint_bundle rep;

   list.num_ints = 0;
   list.ints = int_list;

   pinq_list_int_inds(ws_id, 10, ATTR_NO, &err, &list, &len);
   printf("int\tstyle\tind\tcolr\n");
   for (i = 0; i < list.num_ints; i++) {
      printf("#%d:\t", list.ints[i]);
      pinq_int_rep(ws_id, list.ints[i], 0, &err, &rep);
      if (!err)
         printf("%d\t%d\t%d", rep.style, rep.style_ind, rep.colr_ind);
      else
         printf("ERR%d", err);
      printf("\n");
   }
}

void edgelist(Pint ws_id)
{
   Pint i;
   Pint len;
   Pint err;
   Pint_list list;
   Pint int_list[10];
   Pedge_bundle rep;

   list.num_ints = 0;
   list.ints = int_list;

   pinq_list_edge_inds(ws_id, 10, ATTR_NO, &err, &list, &len);
   printf("edge\tflag\ttype\twidth\tcolr\n");
   for (i = 0; i < list.num_ints; i++) {
      printf("#%d:\t", list.ints[i]);
      pinq_edge_rep(ws_id, list.ints[i], 0, &err, &rep);
      if (!err)
         printf("%d\t%d\t%g\t%d", rep.flag, rep.type, rep.width, rep.colr_ind);
      else
         printf("ERR%d", err);
      printf("\n");
   }
}

void colrlist(Pint ws_id)
{
   Pint i;
   Pint len;
   Pint err;
   Pint_list list;
   Pint int_list[10];
   Pcolr_rep rep;

   list.num_ints = 0;
   list.ints = int_list;

   pinq_list_colr_inds(ws_id, 10, 0, &err, &list, &len);
   printf("colour\tred\tgreen\tblue\n");
   for (i = 0; i < list.num_ints; i++) {
      printf("#%d:\t", list.ints[i]);
      pinq_colr_rep(ws_id, list.ints[i], 0, &err, &rep);
      if (!err)
         printf("%g\t%g\t%g", rep.rgb.red, rep.rgb.green, rep.rgb.blue);
      else
         printf("ERR%d", err);
      printf("\n");
   }
}

void viewlist(Pint ws_id)
{
   Pint i;
   Pint len;
   Pint err;
   Pint_list list;
   Pint int_list[10];

   list.num_ints = 10;
   list.ints = int_list;

   pinq_list_view_inds(ws_id, 10, 0, &err, &list, &len);
   printf("View(s): ");
   for (i = 0; i < list.num_ints; i++) {
      printf("%d ", list.ints[i]);
   }
   printf("\n");
}

void wslist(void)
{
   Pint i;
   Pint len;
   Pint err;
   Pint_list list;
   Pint int_list[MAX_NO_OPEN_WS];
   Pstore store = 0;
   void *conn_id;
   Pint wstype;
   Pws_cat cat;

   list.num_ints = MAX_NO_OPEN_WS;
   list.ints = int_list;

   printf("Open workstation(s)\n");
   pinq_open_wss(MAX_NO_OPEN_WS, 0, &err, &list, &len);
   for (i = 0; i < list.num_ints; i++) {
      pinq_ws_conn_type(list.ints[i], store, &err, &conn_id, &wstype);
      pinq_ws_cat(wstype, &err, &cat);
      printf("Workstation:\t%d\n", list.ints[i]);
      printf("Ws Type:\t%d\n", wstype);
      printf("Category:\t%d\n", cat);
      printf("\n");
      lnlist(list.ints[i]);
      printf("\n");
      mklist(list.ints[i]);
      printf("\n");
      txlist(list.ints[i]);
      printf("\n");
      intlist(list.ints[i]);
      printf("\n");
      edgelist(list.ints[i]);
      printf("\n");
      colrlist(list.ints[i]);
      printf("\n");
      viewlist(list.ints[i]);
      printf("\n");
   }
}

int main(int argc, char *argv[])
{
   Plimit vp, win;
   XEvent event;
   KeySym ks;

   if (argc > 1) {
      view_index = atoi(argv[1]);
      printf("Use view: %d\n", view_index);
   }

   popen_phigs(NULL, 0);

   popen_struct(0);
   pset_indiv_asf(PASPECT_LINETYPE, PASF_BUNDLED);
   pset_indiv_asf(PASPECT_LINEWIDTH, PASF_BUNDLED);
   pset_indiv_asf(PASPECT_MARKER_COLR_IND, PASF_BUNDLED);
   pset_line_ind(ATTR_NO);
   ppolyline(&plist_line);
   pset_indiv_asf(PASPECT_MARKER_TYPE, PASF_BUNDLED);
   pset_indiv_asf(PASPECT_MARKER_SIZE, PASF_BUNDLED);
   pset_indiv_asf(PASPECT_MARKER_COLR_IND, PASF_BUNDLED);
   pset_marker_ind(ATTR_NO);
   ppolymarker(&plist_mark);
   pclose_struct();

   printf("Struct 0\n");
   print_elem_content(0, 1);
   print_elem_content(0, 2);
   print_elem_content(0, 3);
   print_elem_content(0, 4);
   print_elem_content(0, 5);
   print_elem_content(0, 6);
   print_elem_content(0, 7);
   print_elem_content(0, 8);
   print_elem_content(0, 9);
   print_elem_content(0, 10);

   popen_struct(1);
   pset_indiv_asf(PASPECT_EDGE_FLAG, PASF_BUNDLED);
   pset_indiv_asf(PASPECT_EDGETYPE, PASF_BUNDLED);
   pset_indiv_asf(PASPECT_EDGEWIDTH, PASF_BUNDLED);
   pset_indiv_asf(PASPECT_EDGE_COLR_IND, PASF_BUNDLED);
   pset_edge_ind(ATTR_NO);
   pset_indiv_asf(PASPECT_INT_COLR_IND, PASF_BUNDLED);
   pset_indiv_asf(PASPECT_INT_STYLE, PASF_BUNDLED);
   pset_indiv_asf(PASPECT_INT_STYLE_IND, PASF_BUNDLED);
   pset_int_ind(ATTR_NO);
   pfill_area(&plist_quad);
   pclose_struct();

   printf("\nStruct 1\n");
   print_elem_content(1, 1);
   print_elem_content(1, 2);
   print_elem_content(1, 3);
   print_elem_content(1, 4);
   print_elem_content(1, 5);
   print_elem_content(1, 6);
   print_elem_content(1, 7);
   print_elem_content(1, 8);
   print_elem_content(1, 9);
   print_elem_content(1, 10);

   popen_struct(2);
   pset_indiv_asf(PASPECT_EDGE_FLAG, PASF_INDIV);
   pset_edge_flag(PEDGE_OFF);
   pset_int_ind(ATTR_NO);
   pset_indiv_asf(PASPECT_INT_STYLE, PASF_INDIV);
   pset_indiv_asf(PASPECT_INT_STYLE_IND, PASF_INDIV);
   pset_int_style(PSTYLE_HATCH);
   pset_int_style_ind(HATCH_IND);
   pfill_area(&plist_tri);
   pclose_struct();

   printf("\nStruct 2\n");
   print_elem_content(2, 1);
   print_elem_content(2, 2);
   print_elem_content(2, 3);
   print_elem_content(2, 4);
   print_elem_content(2, 5);
   print_elem_content(2, 6);
   print_elem_content(2, 7);
   print_elem_content(2, 8);

   popen_struct(3);
   pset_view_ind(view_index);
   pset_int_colr_ind(0);
   pexec_struct(0);
   pexec_struct(1);
   pset_text_colr_ind(1);
   pset_text_font(1);
   pset_text_prec(PREC_STROKE);
   pset_char_ht(0.08);
   pset_char_expan(1.0);
   pset_char_space(0);
   pset_text_path(PPATH_RIGHT);
   pset_text_align(&text_align);
   pset_char_up_vec(&text_up);
   ptext(&text_pos, "Hello World!");
   pclose_struct();

   printf("\nStruct 3\n");
   print_elem_content(3, 1);
   print_elem_content(3, 2);
   print_elem_content(3, 3);
   print_elem_content(3, 4);
   print_elem_content(3, 5);
   print_elem_content(3, 6);
   print_elem_content(3, 7);
   print_elem_content(3, 8);
   print_elem_content(3, 9);
   print_elem_content(3, 10);
   print_elem_content(3, 11);
   print_elem_content(3, 12);
   print_elem_content(3, 13);
   print_elem_content(3, 14);

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
   pset_line_rep(0, ATTR_NO, &lnrep);
   pset_marker_rep(0, ATTR_NO, &mkrep);
   pset_text_rep(0, ATTR_NO, &txrep);
   pset_edge_rep(0, ATTR_NO, &edgerep);
   pset_int_rep(0, ATTR_NO, &interrep);

   col_rep.rgb.red = 1.0;
   col_rep.rgb.green = 1.0;
   col_rep.rgb.blue = 1.0;
   pset_colr_rep(0, 0, &col_rep);

   col_rep.rgb.red = 0.0;
   col_rep.rgb.green = 1.0;
   col_rep.rgb.blue = 0.0;
   pset_colr_rep(0, 1, &col_rep);

   popen_ws(1, NULL, PWST_OUTPUT_TRUE_DB);
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
   pset_line_rep(1, ATTR_NO, &lnrep);
   pset_marker_rep(1, ATTR_NO, &mkrep);
   pset_text_rep(1, ATTR_NO, &txrep);
   pset_edge_rep(1, ATTR_NO, &edgerep);
   pset_int_rep(1, ATTR_NO, &interrep);

   col_rep.rgb.red = 0.0;
   col_rep.rgb.green = 1.0;
   col_rep.rgb.blue = 1.0;
   pset_colr_rep(1, 0, &col_rep);

   col_rep.rgb.red = 1.0;
   col_rep.rgb.green = 0.0;
   col_rep.rgb.blue = 1.0;
   pset_colr_rep(1, 1, &col_rep);

   wslist();

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
            predraw_all_structs(0, PFLAG_ALWAYS);
            predraw_all_structs(1, PFLAG_ALWAYS);
         break;

         case KeyPress:
            ks = XLookupKeysym((XKeyEvent *) &event, 0);
            if (ks == XK_s) {
               popen_ar_file(0, "test_c2.ar");
               par_all_structs(0);
               pclose_ar_file(0);
            }
            else if (ks == XK_Escape) {
               goto exit;
            }
            else {
               popen_struct(3);
               pexec_struct(2);
               pclose_struct();
            }
         break;

         default:
         break;
      }
   }

exit:
   pclose_ws(0);
   pclose_ws(1);
   pclose_phigs();

   return 0;
}

