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

#define WIDTH         0.5
#define HEIGHT        0.5
#define STRUCT_OBJECT   0

Ppoint pts_quad1[] = {
   {0.0, 0.0},
   {WIDTH, 0.0 },
   {WIDTH, HEIGHT},
   {0.0, HEIGHT}
};

Ppoint_list plist_quad1 = {
   4, pts_quad1
};

Ppoint pts_quad2[] = {
   {WIDTH, HEIGHT},
   {2.0 * WIDTH, HEIGHT},
   {2.0 * WIDTH, 2.0 * HEIGHT},
   {WIDTH, 2.0 * HEIGHT}
};

Ppoint_list plist_quad2 = {
   4, pts_quad2
};

Ppoint_list_list shape;

Pint errnum;
Pgcolr green, yellow;

int main(int argc, char *argv[])
{
   XEvent event;
   KeySym ks;

   green.type = PMODEL_RGB;
   green.val.general.x = 0.0;
   green.val.general.y = 1.0;
   green.val.general.z = 0.0;

   yellow.type = PMODEL_RGB;
   yellow.val.general.x = 1.0;
   yellow.val.general.y = 1.0;
   yellow.val.general.z = 0.0;

   shape.num_point_lists = 2;
   shape.point_lists = malloc(sizeof(Ppoint_list) * 2);
   memcpy(&shape.point_lists[0], &plist_quad1, sizeof(Ppoint_list));
   memcpy(&shape.point_lists[1], &plist_quad2, sizeof(Ppoint_list));
   popen_phigs(NULL, 0);

   popen_struct(STRUCT_OBJECT);
   pset_int_colr(&green);
   pfill_area_set(&shape);
   pclose_struct();

   popen_ws(0, NULL, PWST_OUTPUT_TRUE_DB);
   ppost_struct(0, STRUCT_OBJECT, 0);

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
            if (ks == XK_Down) {
               punpost_struct(0, 1);
               pupd_ws(0, PFLAG_PERFORM);
            }
            else if (ks == XK_Up) {
               ppost_struct(0, 1, 0);
               pupd_ws(0, PFLAG_PERFORM);
            }
            else if (ks == XK_Left) {
               punpost_all_structs(0);
               pupd_ws(0, PFLAG_PERFORM);
            }
            break;

         default:
            break;
      }
   }

   (*PHG_WSID(0)->close)(PHG_WSID(0));

   return 0;
}

