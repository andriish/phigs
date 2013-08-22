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

#include <stdlib.h>
#include <string.h>
#include <phigs/phg.h>
#include <phigs/css.h>
#include <phigs/private/phgP.h>

/*******************************************************************************
 * phg_add_el
 *
 * DESCR:	Add an element and update workstations posted to
 * RETURNS:	N/A
 */

void phg_add_el(
   Css_handle cssh,
   Phg_args_add_el *args
   )
{
   Css_ws_list ws_list;

   ws_list = CSS_GET_WS_ON(CSS_CUR_STRUCTP(cssh));

   if (phg_css_add_elem(cssh, args)) {
      if (ws_list != NULL) {
         for (; ws_list->wsh; ws_list++)
            (*ws_list->wsh->add_el)(ws_list->wsh);
      }
   }
}

