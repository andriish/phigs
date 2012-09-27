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
#include <assert.h>
#include <Vk/VkApp.h>
#include <Vk/VkSimpleWindow.h>

///////////////////////////////////////////////////////////////////////////////
// VkSimpleWindow
//
// DESCR:       Create simple window
// RETURNS:     N/A
//
VkSimpleWindow::VkSimpleWindow(
    const char *name,
    ArgList argList,
    Cardinal argCount
    ) :
    VkComponent(name)
{
    // The application must exists first
    assert(theApplication != NULL);

    // Add this window to the application
    theApplication->addWindow(this);
}

///////////////////////////////////////////////////////////////////////////////
// ~VkSimpleWindow
//
// DESCR:       Clear up resources
// RETURNS:     N/A
//
VkSimpleWindow::~VkSimpleWindow()
{
    theApplication->removeWindow(this);
}

///////////////////////////////////////////////////////////////////////////////
// iconify
//
// DESCR:       Iconify window
// RETURNS:     N/A
//
void VkSimpleWindow::iconify()
{
}

///////////////////////////////////////////////////////////////////////////////
// show
//
// DESCR:       Show window
// RETURNS:     N/A
//
void VkSimpleWindow::show()
{
    std::cout << "Show window: " << name() << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
// hide
//
// DESCR:       Hide window
// RETURNS:     N/A
//
void VkSimpleWindow::hide()
{
}

///////////////////////////////////////////////////////////////////////////////
// className
//
// DESCR:       Get class name
// RETURNS:     Class name
//
const char* VkSimpleWindow::className()
{
    return "VkSimpleWindow";
}

///////////////////////////////////////////////////////////////////////////////
// VkSimpleWindowInitialize
//
// DESCR:       The window's real initialization code
// RETURNS:     N/A
//
void VkSimpleWindow::VkSimpleWindowInitialize(
    const char *name,
    ArgList argList,
    Cardinal argCount
    )

{
    // Create a popup shell for window
    _baseWidget = XtCreatePopupShell(name,
                                     applicationShellWidgetClass,
                                     theApplication->baseWidget(),
                                     argList, argCount);

    installDestroyHandler();
}

