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

#include <string.h>
#include "VkApp.h"
#include "VkSimpleWindow.h"
#include "VkDialogManager.h"

///////////////////////////////////////////////////////////////////////////////
// VkDialogManager
//
// DESCR:       Initialize viewkit base class
// RETURNS:     N/A
//
VkDialogManager::VkDialogManager(const char *name) :
    VkComponent(name)
{
    _nextTitle = 0;
    _nextOKLabel = 0;
    _nextCancelLabel = 0;
    _nextApplyLabel = 0;
}

///////////////////////////////////////////////////////////////////////////////
// ~VkDialogManager
//
// DESCR:       Destroy dialog manager class
// RETURNS:     N/A
//
VkDialogManager::~VkDialogManager()
{
}

///////////////////////////////////////////////////////////////////////////////
// className
//
// DESCR:       Get class name
// RETURNS:     Class name string
//
const char* VkDialogManager::className()
{
    return "VkDialogManager";
}

///////////////////////////////////////////////////////////////////////////
// setTitle
//
// DESCR:       Set dialog title
// RETURNS:     N/A
//
void VkDialogManager::setTitle(const char *nextTitle)
{
    if (nextTitle) {
        _nextTitle = strdup(nextTitle);
    }
}

///////////////////////////////////////////////////////////////////////////////
// setButtonLabels
//
// DESCR:       Set dialog button labels
// RETURNS:     N/A
//
void VkDialogManager::setButtonLabels(
    const char *ok,
    const char *cancel,
    const char *apply
    )
{
    if (ok) {
        _nextOKLabel = strdup(ok);
    }

    if (cancel) {
        _nextCancelLabel = strdup(cancel);
    }

    if (apply) {
        _nextApplyLabel = strdup(apply);
    }
}

///////////////////////////////////////////////////////////////////////////////
// lastPosted
//
// DESCR:       Get last posted widget
// RETURNS:     Last poster widget
//
Widget VkDialogManager::lastPosted()
{
    Widget widget = 0;

    for (int i = 0; i < _widgets.size(); i++) {
        if (XtIsManaged(_widgets[i])) {
            widget = _widgets[i];
            break;
        }
    }

    return widget;
}

///////////////////////////////////////////////////////////////////////////////
// unpost
//
// DESCR:       Unpost last posted dialog
// RETURNS:     N/A
//
void VkDialogManager::unpost()
{
    if (lastPosted()) {
        XtUnmanageChild(lastPosted());
    }
}

///////////////////////////////////////////////////////////////////////////////
// unpost
//
// DESCR:       Unpost dialog widget
// RETURNS:     N/A
//
void VkDialogManager::unpost(Widget w)
{
    if (_widgets.exists(w)) {
        for (int i = 0; i < _widgets.size(); i++) {
            if (_widgets[i] == w) {
                XtUnmanageChild(_widgets[i]);
                break;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// prepost
//
// DESCR:       Create dialog without realizing it
// RETURNS:     Widget of dialog
//
Widget VkDialogManager::prepost(
    const char *message,
    XtCallbackProc okCB,
    XtCallbackProc cancelCB,
    XtCallbackProc applyCB,
    XtPointer clientData,
    const char *helpString,
    VkSimpleWindow *parent
    )
{
    Widget dialogWidget = 0;

    if (parent) {
        if (_widgets.exists(parent->baseWidget())) {
            for (int i = 0; i < _widgets.size(); i++) {
                if (_widgets[i] == parent->baseWidget()) {
                    dialogWidget = _widgets[i];
                    break;
                }
            }
        }
        else {
            dialogWidget = createDialogWidget(parent->baseWidget());
            _widgets.add(dialogWidget);
        }
    }
    else {
        dialogWidget = createDialogWidget(theApplication->baseWidget());
        _widgets.add(dialogWidget);
    }

    if (dialogWidget) {
        if (_nextTitle) {
            XmString xmstr = XmStringCreateSimple(_nextTitle);
            XtVaSetValues(dialogWidget, XmNtitle, xmstr, NULL);
            XmStringFree(xmstr);
            delete _nextTitle;
            _nextTitle = 0;
        }

        if (okCB) {
            XtAddCallback(dialogWidget, XmNokCallback, okCB, clientData);
        }
        XtAddCallback(dialogWidget, XmNokCallback, okXtCallback, this);

        if (_nextOKLabel) {
            XmString xmstr = XmStringCreateSimple(_nextOKLabel);
            XtVaSetValues(dialogWidget, XmNokLabelString, xmstr, NULL);
            XmStringFree(xmstr);
            delete _nextOKLabel;
            _nextOKLabel = 0;
        }
    }

    return dialogWidget;
}

///////////////////////////////////////////////////////////////////////////////
// ok
//
// DESCR:       Select dialog OK
// RETURNS:     N/A
//
void VkDialogManager::ok(Widget w, XtPointer callData)
{
    unpost(w);
}

///////////////////////////////////////////////////////////////////////////////
// createDialogWidget
//
// DESCR:       Create dialog widget using abstract createDialog method
// RETURNS:     Widget of dialog
//
Widget VkDialogManager::createDialogWidget(Widget parent)
{
    Widget dialogWidget = createDialog(parent);

    return dialogWidget;
}

///////////////////////////////////////////////////////////////////////////////
// okXtCallback
//
// DESCR:       Default OK callback
// RETURNS:     N/A
//
void VkDialogManager::okXtCallback(
    Widget w,
    XtPointer clientData,
    XtPointer callData
    )
{
    VkDialogManager *dialogManager = (VkDialogManager *) clientData;

    dialogManager->_action = VkDialogManager::OK;

    dialogManager->ok(w, callData);
}

