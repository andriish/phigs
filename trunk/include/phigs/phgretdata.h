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

#ifndef _phgretdata_h
#define _phgretdata_h

typedef struct {
   Psearch_status status;
   Pint           found_el;
} Phg_ret_el_search;

typedef struct {
   Pelem_type type;
   Pint       size;
} Phg_ret_el_type_size;

typedef struct {
   Pelem_type op;
   Pelem_data el_data;
} Phg_ret_q_content;

typedef struct {
   Pint_list counts;
   Pint      num_pairs;
   Pelem_ref *paths;
} Phg_ret_hierarchy;

typedef struct {
   int unused;
} Phg_ret_open_ws;

typedef struct {
   Pupd_st              update_state;
   Pview_rep3           *cur_rep;
   Pview_rep3           *req_rep;
} Phg_ret_view_rep;

typedef struct {
   Pupd_st state;
   Plimit3 req_window;
   Plimit3 cur_window;
   Plimit3 req_viewport;
   Plimit3 cur_viewport;
} Phg_ret_ws_tran3;

typedef struct {
   Pdefer_mode      def_mode;
   Pmod_mode        mod_mode;
   Pdisp_surf_empty display_surf;
   Pvisual_st       state;
} Phg_ret_update_state;

typedef struct {
   Pupd_st state;
   Pint    cur_mode;
   Pint    req_mode;
} Phg_ret_hlhsr_mode;

typedef union {
   Pint                 idata;
   Pfloat               fdata;
   Pint_list            int_list;
   Pposted_struct_list  postlist;
   Phg_ret_open_ws      open_ws;
   Phg_ret_ws_tran3     ws_xform;
   Phg_ret_el_search    el_search;
   Phg_ret_el_type_size el_type_size;
   Phg_ret_q_content    el_info;
   Phg_ret_hierarchy    hierarchy;
   Phg_ret_view_rep     view_rep;
   Phg_ret_update_state update_state;
   Phg_ret_hlhsr_mode   hlhsr_mode;
} Phg_ret_data;

typedef struct {
   Pint         err;
   Phg_ret_data data;
} Phg_ret;

#endif

