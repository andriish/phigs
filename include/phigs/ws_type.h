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

#ifndef _ws_type_h
#define _ws_type_h

#include <phigs/list.h>

/* Workstation types */
#define PWST_OUTPUT_TRUE              0

/* Default tables */
#define WST_MIN_PREDEF_LINE_REPS      1
#define WST_MIN_PREDEF_MARKER_REPS    1
#define WST_MIN_PREDEF_TEXT_REPS      1
#define WST_MIN_PREDEF_INTERIOUR_REPS 1
#define WST_MIN_PREDEF_EDGE_REPS      1
#define WST_MIN_PREDEF_VIEW_REPS      6

typedef struct {

   /* Lines */
   Pline_bundle_plus
      default_polyline_bundle_table[WST_MIN_PREDEF_LINE_REPS];

   /* Markers */
   Pmarker_bundle_plus
      default_polymarker_bundle_table[WST_MIN_PREDEF_MARKER_REPS];

   /* Text */
   Ptext_bundle_plus
      default_text_bundle_table[WST_MIN_PREDEF_TEXT_REPS];

   /* Interiour */
   Pint_bundle_plus
      default_interiour_bundle_table[WST_MIN_PREDEF_INTERIOUR_REPS];

   /* Edge */
   Pedge_bundle_plus
      default_edge_bundle_table[WST_MIN_PREDEF_EDGE_REPS];

} Wst_output_wsdt;

typedef struct {
   Pws_cat ws_category;
   Pfloat  dev_coords[3];
   Pint    num_hlhsr_modes;
   Pint    *hlhsr_modes;


   /* View */
   Pview_rep3
      default_views[WST_MIN_PREDEF_VIEW_REPS];

   Wst_output_wsdt out_dt;
} Wst_phigs_dt;

typedef struct {
   Wst_phigs_dt    phigs_dt;
} Wst_dt;

struct _Ws;
struct _Phg_args_open_ws;

typedef struct {
   Node          node;
   Pint          wsid;
   Pint          ws_type;
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
   Pint ws_type
   );

/*******************************************************************************
 * phg_wst_init
 *
 * DESCR:       Initialize workstation type structure
 * RETURNS:     TRUE or FALSE
 */

int phg_wst_init(
   Wst *wst,
   Pint ws_type
   );

/*******************************************************************************
 * phg_wst_find
 *
 * DESCR:       Find workstation type 
 * RETURNS:     Pointer to workstation type or NULL
 */

Wst* phg_wst_find(
   List *list,
   Pint ws_type
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

/*******************************************************************************
 * phg_wst_check_set_rep
 *
 * DESCR:       Check workstation attribute to set
 * RETURNS:     Pointer to Wst_phigs_dt structure or NULL
 */

Wst_phigs_dt* phg_wst_check_set_rep(
   Pint fn_id,
   Pint ws_id,
   Pint ind,
   Pint colr_ind
   );

#endif /* _ws_type_h */

