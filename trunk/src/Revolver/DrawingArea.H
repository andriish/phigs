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

#ifndef _DrawingArea_H
#define _DrawingArea_H

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <GL/glx.h>

class DrawingArea {

private:
  unsigned int width, height;
  Display *display;
  GLXContext glxc;
  Colormap cmap;
  Widget shellW, formdW, glW, scrollW, vsbW;

  Colormap getSharableColormap(XVisualInfo *vi);

public:
  DrawingArea(Widget parent);
  void initStructs();
  void init(GLXContext c);
  void render();
};

#endif

