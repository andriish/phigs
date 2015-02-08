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

#define WIDGET_LIST_BLOCK_SIZE    10

#include <stdlib.h>
#include "VkComponent.h"
#include "VkWidgetList.h"

///////////////////////////////////////////////////////////////////////////////
// VkWidgetList
//
// DESCR:       Initialize widget list
// RETURNS:     N/A
//
VkWidgetList::VkWidgetList()
{
    _numWidgets = 0;
    _maxWidgets = WIDGET_LIST_BLOCK_SIZE;

    _widgets = (WidgetList) malloc(_maxWidgets * sizeof(Widget));
}

///////////////////////////////////////////////////////////////////////////////
// ~VkWidgetList
//
// DESCR:       Destroy widget list
// RETURNS:     N/A
//
VkWidgetList::~VkWidgetList()
{
    _numWidgets = 0;
    _maxWidgets = 0;

    free(_widgets);

    _widgets = 0;
}

///////////////////////////////////////////////////////////////////////////////
// add
//
// DESCR:       Add widget to widget list
// RETURNS:     N/A
//
void VkWidgetList::add(Widget widget)
{
    if (_numWidgets ==  _maxWidgets - 1) {
        _maxWidgets += WIDGET_LIST_BLOCK_SIZE;
        _widgets = (WidgetList) realloc(_widgets, _maxWidgets * sizeof(Widget));
    }

    if (_widgets) {
        XtAddCallback(
            widget,
            XmNdestroyCallback,
            widgetDestroyedCallback,
            this
            );
        _widgets[_numWidgets++] = widget;
    }
}

///////////////////////////////////////////////////////////////////////////////
// add
//
// DESCR:       Add component widget to widget list
// RETURNS:     N/A
//
void VkWidgetList::add(VkComponent *component)
{
    add(component->baseWidget());
}

///////////////////////////////////////////////////////////////////////////////
// remove
//
// DESCR:       Remove widget from widget list
// RETURNS:     N/A
//
void VkWidgetList::remove(Widget widget)
{
    int i, j;

    for (i = 0; i < _numWidgets; i++) {
        if (_widgets[i] == widget) {
            break;
        }
    }

    for (j = i; j < _numWidgets - 1; j++) {
        _widgets[j] = _widgets[j + 1];
    }

    _numWidgets--;
}

///////////////////////////////////////////////////////////////////////////////
// remove
//
// DESCR:       Remove component widget from widget list
// RETURNS:     N/A
//
void VkWidgetList::remove(VkComponent *component)
{
    remove(component->baseWidget());
}

///////////////////////////////////////////////////////////////////////////////
// removeFirst
//
// DESCR:       Remove first widget from widget list
// RETURNS:     N/A
//
void VkWidgetList::removeFirst()
{
    int i;

    for (i = 0; i < _numWidgets - 1; i++) {
        _widgets[i] = _widgets[i + 1];
    }

    _numWidgets--;
}

///////////////////////////////////////////////////////////////////////////////
// removeLast
//
// DESCR:       Remove last widget from widget list
// RETURNS:     N/A
//
void VkWidgetList::removeLast()
{
    _numWidgets--;
}

///////////////////////////////////////////////////////////////////////////////
// exists
//
// DESCR:       Check if widget exists in list
// RETURNS:     True if widget exists, otherwise False
//
Boolean VkWidgetList::exists(Widget widget)
{
    int i;
    Boolean result = False;

    for (i = 0; i < _numWidgets; i++) {
        if (_widgets[i] == widget) {
            result = True;
            break;
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////
// clear
//
// DESCR:       Clear widget list
// RETURNS:     N/A
//
void VkWidgetList::clear()
{
    _numWidgets = 0;
    _maxWidgets = WIDGET_LIST_BLOCK_SIZE;

    _widgets = (WidgetList) realloc(_widgets, _maxWidgets * sizeof(Widget));
}

///////////////////////////////////////////////////////////////////////////////
// widgetDestroyed
//
// DESCR:       Called when a widget is destroyed
// RETURNS:     N/A
//
void VkWidgetList::widgetDestroyed(Widget widget, XtPointer call_data)
{
}

///////////////////////////////////////////////////////////////////////////////
// widgetDestroyedCallback
//
// DESCR:       Widget destroyed callback
// RETURNS:     N/A
//
void VkWidgetList::widgetDestroyedCallback(
    Widget widget,
    XtPointer client_data,
    XtPointer call_data
    )
{
    VkWidgetList *widgetList = (VkWidgetList *) client_data;

    widgetList->widgetDestroyed(widget, call_data);
    widgetList->remove(widget);
}

