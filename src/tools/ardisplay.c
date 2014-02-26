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
#include <sys/types.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "phg.h"

#define NUM_COLR_INDS 5

void list_structs(Pint archive_id)
{
   Pint i, n;
   Pint_list list;
   Pint ints[10];

   list.num_ints = 0;
   list.ints = ints;

   printf("Archive containts the following structure ids:\n");
   pret_struct_ids(archive_id, 10, 0, &list, &n);
   for (i = 0; i < list.num_ints; i++) {
      printf("\t%d\n", list.ints[i]);
   }
}

int main(int argc, char *argv[])
{
   XEvent event;
   KeySym ks;
   int i;
   Pint_list struct_ids;
   Pcolr_rep col_rep;

   if (argc < 2) {
      printf("usage: %s <filename> [view-struct]\n", argv[0]);
      return 1;
   }

   popen_phigs(NULL, 0);

   if (argc < 3) {
      popen_ar_file(0, argv[1]);
      list_structs(0);
      pclose_ar_file(0);
      pclose_phigs();
      printf("list structure ids on command line to display:\n");
      printf("example: %s %s 1, 2, 3, ...\n", argv[0], argv[1]);
      return 0;
   }

   struct_ids.num_ints = argc - 2;
   struct_ids.ints = (Pint *) malloc(struct_ids.num_ints * sizeof(Pint));
   for (i = 0; i < struct_ids.num_ints; i++) {
      struct_ids.ints[i] = atoi(argv[2 + i]);
   }

   popen_ar_file(0, argv[1]);
   pret_struct_nets(0, &struct_ids);
   pclose_ar_file(0);

   popen_ws(0, NULL, PWST_OUTPUT_TRUE_DB);
   pset_hlhsr_mode(0, PHIGS_HLHSR_MODE_ZBUFF);
   pset_disp_upd_st(0, PDEFER_BNIL, PMODE_UQUM);

   for (i = 0; i < NUM_COLR_INDS; i++) {
      col_rep.rgb.red = (Pfloat) i * 1.0 / (Pfloat) NUM_COLR_INDS;
      col_rep.rgb.green = (Pfloat) i * 1.0 / (Pfloat) NUM_COLR_INDS;
      col_rep.rgb.blue = (Pfloat) i * 1.0 / (Pfloat) NUM_COLR_INDS;
      pset_colr_rep(0, i, &col_rep);
   }

   XSelectInput(PHG_WSID(0)->display,
                PHG_WSID(0)->drawable_id,
                ExposureMask | KeyPressMask);
   while (1) {
      XNextEvent(PHG_WSID(0)->display, &event);
      switch(event.type) {

         case Expose:
            while (XCheckTypedEvent(PHG_WSID(0)->display, Expose, &event));
            for (i = 0; i < struct_ids.num_ints; i++) {
               ppost_struct(0, struct_ids.ints[i], 0);
            }
            pupd_ws(0, PFLAG_PERFORM);
            break;

         case KeyPress:
            ks = XLookupKeysym((XKeyEvent *) &event, 0);
            if (ks == XK_Escape) {
               goto exit;
            }
            break;

         default:
            break;
      }
   }

exit:
   free(struct_ids.ints);
   pclose_ws(0);
   pclose_phigs();

   return 0;
}

