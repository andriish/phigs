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

#ifndef _wsxP_h
#define _wsxP_h

/*******************************************************************************
 * phg_wsx_create
 *
 * DESCR:       Create workstation
 * RETURNS:     Pointer to workstaion of NULL
 */

Ws* phg_wsx_create(
   Phg_args_open_ws *args
   );

/*******************************************************************************
 * phg_wsx_setup_tool
 *
 * DESCR:       Create toolkit rendering window
 * RETURNS:     TRUE or FALSE
 */

int phg_wsx_setup_tool(
   Ws *ws,
   Phg_args_conn_info *conn_info,
   Wst *wst
   );

#endif /* _wsxP_h */

