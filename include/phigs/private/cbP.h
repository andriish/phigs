/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2014 Surplus Users Ham Society
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

#ifndef _cbP_h
#define _cbP_h

#ifdef __cplusplus
extern "C" {
#endif

struct _Pstore {
   Pint size;
   union {
      Par_file_list ar_files;
      Pelem_data    elem_data;
   } data;
   void *buf;
   struct _Pstore *next;
};

/*******************************************************************************
 * phg_cb_resize_store
 *
 * DESCR:       Resize storage object
 * RETURNS:     TRUE or FALSE
 */

int phg_cb_resize_store(
   Pstore store,
   Pint size,
   Pint *err_ind
   );

/*******************************************************************************
 * phg_cb_destroy_all_stores
 *
 * DESCR:       Destroy all storage objects
 * RETURNS:     N/A
 */

void phg_cb_destroy_all_stores(
   void
   );

/*******************************************************************************
 * phg_cb_store_el_size
 *
 * DESCR:       Get size for store data buffer used by inquiry function
 * RETURNS:     Element size
 */

int phg_cb_store_el_size(
   Phg_elmt_info *el_info
   );

/*******************************************************************************
 * phg_cb_store_el_data
 *
 * DESCR:       Store element data for inquiry function
 * RETURNS:     N/A
 */

void phg_cb_store_el_data(
   Phg_elmt_info *el_info,
   void *buf,
   Pelem_data *ed
   );

extern struct _Pstore *phg_cb_store_list;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _cbP_h */

