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
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/ScrolledW.h>
#include <Xm/ScrollBar.h>
#include <Xm/Label.h>
#include <Xm/Scale.h>
#include <X11/GLw/GLwMDrawA.h>
#include <phigs/phg.h>

#include "DrawingArea.H"

void DrawingAreaInitCB(Widget w, XtPointer client_data, XtPointer call_data)
{
  XVisualInfo *visinfo;
  DrawingArea *area = (DrawingArea *) client_data;

  XtVaGetValues(w, GLwNvisualInfo, &visinfo, NULL);
  GLXContext glxc = glXCreateContext(XtDisplay(w), visinfo, 0, False);
  area->init(glxc);
}

void DrawingAreaExposeCB(Widget w, XtPointer client_data, XtPointer call_data)
{
  DrawingArea *area = (DrawingArea *) client_data;
  area->render();
}

Colormap DrawingArea::getSharableColormap(XVisualInfo *vi)
{
  Status status;
  XStandardColormap *standardCmaps;
  Colormap cmap;
  int i, numCmaps;

  status = XmuLookupStandardColormap(display, vi->screen, vi->visualid,
		 		     vi->depth, XA_RGB_DEFAULT_MAP,
				     False, True);

  if (status == 1) {
    status = XGetRGBColormaps(display, RootWindow(display, vi->screen),
		              &standardCmaps, &numCmaps, XA_RGB_DEFAULT_MAP);
    if (status == 1) {
      for (i = 0; i < numCmaps; i++) {
        if (standardCmaps[i].visualid == vi->visualid) {
          cmap = standardCmaps[i].colormap;
          XFree(standardCmaps);
          return cmap;
        }
      }
    }
  }

  cmap = XCreateColormap(display, RootWindow(display, vi->screen),
			 vi->visual, AllocNone);
  return cmap;
}
 
DrawingArea::DrawingArea(Widget parent)
{
   Arg args[10];
   int n;
   XVisualInfo *visinfo;

   popen_phigs(NULL, 0);
   initStructs();

   int reqVisual[] = {
      GLX_DOUBLEBUFFER,
      GLX_RGBA,
      GLX_RED_SIZE, 1,
      GLX_GREEN_SIZE, 1,
      GLX_BLUE_SIZE, 1,
      None};

   width  = 512;
   height = 512;
   display = XtDisplay(parent);

#if 0
   // Create new shell
   n = 0;
   shellW = XtCreateWidget("window",
      applicationShellWidgetClass,
      parent, args, n);
#endif

   // Find appropriate visual
   visinfo = glXChooseVisual(display, DefaultScreen(display), reqVisual);
   if (visinfo == NULL) {
      std::cerr << "No appropriate visual found" << std::endl;
      exit(1);
   }

#ifdef DEBUG
   std::cerr << visinfo->depth << "Bit visual found" << std::endl;
#endif

   cmap = getSharableColormap(visinfo);

#if 0
   // Create form dialog
   n = 0;
   formdW = XmCreateFormDialog(shellW, "window", args, n); 
#endif

   // Create scroll window
   n = 0;
   XtSetArg(args[n], XmNscrollingPolicy, XmAPPLICATION_DEFINED); n++;
   XtSetArg(args[n], XmNvisualPolicy, XmVARIABLE); n++;
   scrollW = XmCreateScrolledWindow(parent, "scroll", args, n);

   glW = XtVaCreateWidget("drawingarea",
	 glwMDrawingAreaWidgetClass, scrollW,
	 GLwNvisualInfo, visinfo,
	 XtNcolormap, cmap,
         XmNwidth, width,
	 XmNheight, height,
	 NULL);

   // Add callbacks
   XtAddCallback(glW, GLwNginitCallback, DrawingAreaInitCB, this);
   XtAddCallback(glW, GLwNexposeCallback, DrawingAreaExposeCB, this);

   // Create scrollbars
   n = 0;
   XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
   vsbW = XmCreateScrollBar(scrollW, "vscroll", args, n);

   // Add callbacks
   //XtAddCallback(vsbW, XmNvalueChangedCallback, OverlayvScrollCB, this);
   //XtAddCallback(vsbW, XmNdragCallback, OverlayvScrollCB, this);

   // Scrollbar
   n = 0;
   XtSetArg(args[n], XmNmaximum, 1); n++;
   XtSetArg(args[n], XmNsliderSize, 1); n++;
   XtSetValues(vsbW, args, n);

   XtManageChild(vsbW);
   XtManageChild(glW);
   XtManageChild(scrollW);
#if 0
   XtManageChild(formdW);
   //XtManageChild(XtParent(formdW));
   XtManageChild(shellW);
#endif
}

void DrawingArea::initStructs()
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

void DrawingArea::init(GLXContext c)
{
   Phg_args_conn_info conn_info;

   conn_info.display     = XtDisplay(glW);
   conn_info.drawable_id = XtWindow(glW);
   conn_info.glx_context = c;
   conn_info.colormap    = cmap;
   conn_info.background  = WhitePixel(conn_info.display, 0);
   popen_ws(0, &conn_info, PWST_OUTPUT_TRUE_DB);
   ppost_struct(0, 0, 0);
}

void DrawingArea::render()
{
   predraw_all_structs(0, PFLAG_ALWAYS);
}

