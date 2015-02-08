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

#ifndef _VkWidgetList_H
#define _VkWidgetList_H

#include <Xm/Xm.h>
#include "VkBase.h"

class VkComponent;

class VkWidgetList : public VkBase {
public:

    ///////////////////////////////////////////////////////////////////////////
    // VkWidgetList
    //
    // DESCR:       Initialize widget list
    // RETURNS:     N/A
    //
    VkWidgetList();

    ///////////////////////////////////////////////////////////////////////////
    // ~VkWidgetList
    //
    // DESCR:       Destroy widget list
    // RETURNS:     N/A
    //
    ~VkWidgetList();

    ///////////////////////////////////////////////////////////////////////////
    // add
    //
    // DESCR:       Add widget to widget list
    // RETURNS:     N/A
    //
    void add(Widget widget);

    ///////////////////////////////////////////////////////////////////////////
    // add
    //
    // DESCR:       Add component widget to widget list
    // RETURNS:     N/A
    //
    void add(VkComponent *component);

    ///////////////////////////////////////////////////////////////////////////
    // remove
    //
    // DESCR:       Remove widget from widget list
    // RETURNS:     N/A
    //
    void remove(Widget widget);

    ///////////////////////////////////////////////////////////////////////////
    // remove
    //
    // DESCR:       Remove component widget from widget list
    // RETURNS:     N/A
    //
    void remove(VkComponent *component);

    ///////////////////////////////////////////////////////////////////////////
    // removeFirst
    //
    // DESCR:       Remove first widget from widget list
    // RETURNS:     N/A
    //
    void removeFirst();

    ///////////////////////////////////////////////////////////////////////////
    // removeLast
    //
    // DESCR:       Remove last widget from widget list
    // RETURNS:     N/A
    //
    void removeLast();

    ///////////////////////////////////////////////////////////////////////////
    // exists
    //
    // DESCR:       Check if widget exists in list
    // RETURNS:     True if widget exists, otherwise False
    //
    Boolean exists(Widget widget);

    ///////////////////////////////////////////////////////////////////////////
    // size
    //
    // DESCR:       Get number of widgets in widget list
    // RETURNS:     N/A
    //
    int size() { return _numWidgets; }

    ///////////////////////////////////////////////////////////////////////////
    // operator[]
    //
    // DESCR:       Get widget at index in widget list
    // RETURNS:     Widget at index
    //
    Widget operator[](int index) const { return _widgets[index]; }

    ///////////////////////////////////////////////////////////////////////////
    // clear
    //
    // DESCR:       Clear widget list
    // RETURNS:     N/A
    //
    void clear();

protected:

    ///////////////////////////////////////////////////////////////////////////
    // widgetDestroyed
    //
    // DESCR:       Called when a widget is destroyed
    // RETURNS:     N/A
    //
    virtual void widgetDestroyed(Widget widget, XtPointer call_data);

private:

    ///////////////////////////////////////////////////////////////////////////
    // widgetDestroyedCallback
    //
    // DESCR:       Widget destroyed callback
    // RETURNS:     N/A
    //
    static void widgetDestroyedCallback(
        Widget widget,
        XtPointer client_data,
        XtPointer call_data
        );

    // List of widgets
    WidgetList _widgets;

    // Widget count
    int _numWidgets;

    // Maximum number of widgets
    int _maxWidgets;
};

#endif

