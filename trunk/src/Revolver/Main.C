///////////////////////////////////////////////////////////////////////////////
//  DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
//
//  This file is part of Open PHIGS
//  Copyright (C) 2011 - 2012 Surplus Users Ham Society
//
//  Open PHIGS is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 2.1 of the License, or
//  (at your option) any later version.
//
//  Open PHIGS is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with Open PHIGS. If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <phigs/phg.h>
#include "Workstation.h"

void initStructs()
{
   Ppoint quad_pts[4] = {
      {0.0, 0.0},
      {0.5, 0.0},
      {0.5, 0.5},
      {0.0, 0.5}
   };
   Ppoint_list quad = {4, quad_pts};
   Pgcolr blue;

   blue.type = PMODEL_RGB;
   blue.val.general.x = 0.0;
   blue.val.general.y = 0.0;
   blue.val.general.z = 1.0;

   popen_struct(0);
   pset_int_colr(&blue);
   pfill_area(&quad);
   pclose_struct();
}

int main(int argc, char *argv[])
{
    Widget toplevel;
    XtAppContext app;

    popen_phigs(NULL, 0);
    initStructs();

    toplevel = XtVaAppInitialize(
        &app,                  /* Application context */
        "Revolver",            /* Application name */
        NULL, 0,               /* Command line options list */
        &argc, argv,           /* Command line args */
        NULL,                  /* Fallback resources */
        NULL);

    Workstation *ws = new Workstation("ws", toplevel, 0);

    std::cout << "Class name: " << ws->className() << std::endl;
    std::cout << "Object name: " << ws->name() << std::endl;
    std::cout << "Workstation id: " << ws->id() << std::endl;

    ws->show();

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app);

    return 0;
}

