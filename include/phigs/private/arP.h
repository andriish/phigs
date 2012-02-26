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

#ifndef _arP_h
#define _arP_h

/******************************************************************************
 * phg_ar_set_conversion
 *
 * DESCR:       Set conversion function
 * RETURNS:     N/A
 */

void phg_ar_set_conversion(
    int from,
    int to
    );

/******************************************************************************
 * phg_ar_convert_afd
 *
 * DESCR:       Convert Archive File Descriptor 
 * RETURNS:     N/A
 */

void phg_ar_convert_afd(
    Phg_ar_descriptor *d
    );

/******************************************************************************
 * phg_ar_convert_bse
 *
 * DESCR:       Convert Archive Begin Structure
 * RETURNS:     N/A
 */

void phg_ar_convert_bse(
    Phg_ar_begin_struct *b
    );

/******************************************************************************
 * phg_ar_convert_afs
 *
 * DESCR:       Convert Archive Free Space
 * RETURNS:     N/A
 */

void phg_ar_convert_afs(
    Phg_ar_free_space *f
    );

/******************************************************************************
 * phg_ar_convert_afie
 *
 * DESCR:       Convert Archive Index Entries
 * RETURNS:     N/A
 */

void phg_ar_convert_afie(
    int n,
    Phg_ar_index_entry *e
    );

/******************************************************************************
 * phg_ar_convert_elements
 *
 * DESCR:       Convert Archive Elements
 * RETURNS:     N/A
 */

void phg_ar_convert_elements(
    int nelts,
    char *buffer,
    Phg_ar_archiving_direction direction
    );

#endif /* _arP_h */

