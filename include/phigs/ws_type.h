/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2011 Surplus Users Ham Society
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

#ifndef _ws_type_h
#define _ws_type_h

typedef enum {
   WST_BASE_TYPE_GLX_DRAWABLE
} Wst_base_type;

typedef struct {
   Pws_cat ws_category;
   Pfloat  dev_coords[3];
   Pint    num_hlhsr_modes;
   Pint    *hlhsr_modes;
} Wst_phigs_dt;

typedef struct {
   Wst_phigs_dt phigs_dt;
} Wst_dt;

struct _Ws;
struct _Phg_args_open_ws;

typedef struct {
   Pint          wsid;
   Wst_base_type base_type;
   Wst_dt        desc_tbl;
   int           (*open_window)(
                    struct _Ws *ws,
                    struct _Phg_args_open_ws *args
                 );
   void          (*resize_window)(
                    struct _Ws *ws,
                    int w,
                    int h
                 );
   void          (*release_window)(
                    struct _Ws *ws
                 );

} Wst;

/*******************************************************************************
 * phg_wst_create
 *
 * DESCR:       Create workstation type structure
 * RETURNS:	Pointer to workstation type or NULL
 */

Wst* phg_wst_create(
   Wst_base_type base_type
   );

/*******************************************************************************
 * phg_wst_init
 *
 * DESCR:       Initialize workstation type structure
 * RETURNS:     TRUE or FALSE
 */

int phg_wst_init(
   Wst *wst,
   Wst_base_type base_type
   );

/*******************************************************************************
 * phg_wst_destroy
 * 
 * DESCR:       Destroy workstation type structure
 * RETURNS:     N/A
 */
 
void phg_wst_destroy(
   Wst *wst
   );

#endif /* _ws_type_h */

