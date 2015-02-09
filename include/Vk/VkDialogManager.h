///////////////////////////////////////////////////////////////////////////////
//  DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
//
//  This file is part of Open PHIGS
//  Copyright (C) 2011 - 2015 Surplus Users Ham Society
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

#ifndef _VkDialogManager_H
#define _VkDialogManager_H

#include "VkComponent.h"
#include "VkWidgetList.h"

class VkDialog;
class VkSimpleWindow;

class VkDialogManager : public VkComponent {
public:

    ///////////////////////////////////////////////////////////////////////////
    // ~VkDialogManager
    //
    // DESCR:       Destroy dialog manager class
    // RETURNS:     N/A
    //
    virtual ~VkDialogManager();

    ///////////////////////////////////////////////////////////////////////////
    // className
    //
    // DESCR:       Get class name
    // RETURNS:     Class name string
    //
    const char* className();

    ///////////////////////////////////////////////////////////////////////////
    // setTitle
    //
    // DESCR:       Set dialog title
    // RETURNS:     N/A
    //
    void setTitle(const char *nextTitle = 0);

    ///////////////////////////////////////////////////////////////////////////
    // setButtonLabels
    //
    // DESCR:       Set dialog button labels
    // RETURNS:     N/A
    //
    void setButtonLabels(
        const char *ok = 0,
        const char *cancel = 0,
        const char *apply = 0
        );

    ///////////////////////////////////////////////////////////////////////////
    // lastPosted
    //
    // DESCR:       Get last posted widget
    // RETURNS:     Last poster widget
    //
    Widget lastPosted();

    ///////////////////////////////////////////////////////////////////////////
    // unpost
    //
    // DESCR:       Unpost last posted dialog
    // RETURNS:     N/A
    //
    void unpost();

    ///////////////////////////////////////////////////////////////////////////
    // unpost
    //
    // DESCR:       Unpost dialog widget
    // RETURNS:     N/A
    //
    void unpost(Widget w);

    // Dialog action data type
    enum VkDialogReason { CANCEL, OK, APPLY, NONE };

protected:

    ///////////////////////////////////////////////////////////////////////////
    // VkDialogManager
    //
    // DESCR:       Initialize dialog manager class
    // RETURNS:     N/A
    //
    VkDialogManager(const char *name);

    ///////////////////////////////////////////////////////////////////////////
    // prepost
    //
    // DESCR:       Create dialog without realizing it
    // RETURNS:     Widget of dialog
    //
    virtual Widget prepost(
        const char *message,
        XtCallbackProc okCB = 0,
        XtCallbackProc cancelCB = 0,
        XtCallbackProc applyCB = 0,
        XtPointer clientData = 0,
        const char *helpString = 0,
        VkSimpleWindow *parent = 0
        );

    ///////////////////////////////////////////////////////////////////////////
    // ok
    //
    // DESCR:       Select dialog OK
    // RETURNS:     N/A
    //
    virtual void ok(Widget w, XtPointer callData);

    ///////////////////////////////////////////////////////////////////////////
    // createDialog
    //
    // DESCR:       Subclass create dialog method
    // RETURNS:     Widget of dialog
    //
    virtual Widget createDialog(Widget parent) = 0;

    // Dialog action type
    VkDialogReason _action;

private:

    ///////////////////////////////////////////////////////////////////////////
    // createDialogWidget
    //
    // DESCR:       Create dialog widget using abstract createDialog method
    // RETURNS:     Widget of dialog
    //
    Widget createDialogWidget(Widget parent);

    // Widget list
    VkWidgetList _widgets;

    // Labels
    char *_nextTitle;
    char *_nextOKLabel;
    char *_nextCancelLabel;
    char *_nextApplyLabel;

    // Callbacks
    static void okXtCallback(Widget, XtPointer, XtPointer);
};

#endif

