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

#include <iostream>
#include "DrawingArea.H"

Widget top_level;
XtAppContext app_con;

int main(int argc, char *argv[])
{
  top_level = XtVaAppInitialize(
        &app_con,              /* Application context */
        "Revolver",            /* Application name */
        NULL, 0,               /* Command line options list */
        &argc, argv,           /* Command line args */
        NULL,                  /* Fallback resources */
        NULL);                 /* Terminate varargs list */

   DrawingArea *area = new DrawingArea(top_level);

   XtRealizeWidget(top_level);
   XtAppMainLoop(app_con);

   return 0;
}

