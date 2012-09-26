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

#ifndef _VkApp_H
#define _VkApp_H

#include <X11/Xlib.h>
#include <Vk/VkComponent.h>
#include <Vk/VkComponentList.h>

class VkApp;

extern VkApp *theApplication;

class VkApp : public VkComponent {
public:

    ///////////////////////////////////////////////////////////////////////////
    // VkApp
    //
    // DESCR:       Normal constructor -- most apps should use this one
    // RETURNS:     N/A
    //
    VkApp(
        char *appClassName,
        int *arg_c,
        char **arg_v,
        XrmOptionDescRec *optionList,
        int sizeOfOptionList = 0
        );

    ///////////////////////////////////////////////////////////////////////////
    // ~VkApp
    //
    // DESCR:       Clear up resources
    // RETURNS:     N/A
    //
    virtual ~VkApp();

protected:

    // List of application windows
    VkComponentList _winList;

private:

    ///////////////////////////////////////////////////////////////////////////
    // VkAppInitialize
    //
    // DESCR:       Constructor's real initialization function
    // RETURNS:     N/A
    //
    void VkAppInitialize(
        char *appClassName,
        int *arg_c,
        char **arg_v,
        ArgList argList,
        Cardinal argCount,
        void (*preRealizeFunction)(Widget w),
        XrmOptionDescRec *optionList,
        int sizeOfOptionList
        );

    // Various data fields needed by an application
    char          **_argv;
    int             _argc;
    Display        *_dpy;
    XtAppContext    _appContext;
    char           *_applicationClassName;
};

#endif

