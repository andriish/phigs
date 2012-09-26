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

#ifndef _VkWorkstation_H
#define _VkWorkstation_h

#include <Vk/VkComponent.h>

class VkWorkstation : public VkComponent {
public:

    ///////////////////////////////////////////////////////////////////////////
    // VkWorkstation
    //
    // DESCR:       Initialize workstaion
    // RETURNS:     N/A
    //
    VkWorkstation(
        const char *name,
        Widget parent
        );

    ///////////////////////////////////////////////////////////////////////////
    // className
    //
    // DESCR:       Get name of object class
    // RETURNS:     Class name
    //
    virtual const char* className();

protected:

    ///////////////////////////////////////////////////////////////////////////
    // init
    //
    // DESCR:       Initialize workstation window 
    // RETURNS:     N/A
    //
    void init();

    ///////////////////////////////////////////////////////////////////////////
    // expose
    //
    // DESCR:       Redraw workstation window
    // RETURNS:     N/A
    //
    void expose();

private:
    Display *_dpy;
    Colormap _cmap;

    ///////////////////////////////////////////////////////////////////////////
    // getSharableCmap
    //
    // DESCR:       Get a colourmap that is sharable between windows
    // RETURNS:     Colour map
    //
    Colormap VkWorkstation::getSharableCmap(
        XVisualInfo *vi
        );

    ///////////////////////////////////////////////////////////////////////////
    // initCallback
    //
    // DESCR:       Callback called by toolkit when initialized
    // RETURNS:     N/A
    //
    static void initCallback(
        Widget w,
        XtPointer clientData,
        XtPointer callData
        );

    ///////////////////////////////////////////////////////////////////////////
    // exposeCallback
    //
    // DESCR:       Callback called by toolkit when exposed
    // RETURNS:     N/A
    //
    static void exposeCallback(
        Widget w,
        XtPointer clientData,
        XtPointer callData
        );
};

#endif

