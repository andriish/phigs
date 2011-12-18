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

#include <stdio.h>
#include <stdlib.h>
#include <phigs/phg.h>
#include <phigs/css.h>
#include <phigs/private/phgP.h>

/*******************************************************************************
 * pinq_elem_ptr
 *
 * DESCR:	Returns the index of the current element.
 * RETURNS:	N/A
 */

void pinq_elem_ptr(
   Pint *err_ind,
   Pint *elem_ptr_value
   )
{
   *elem_ptr_value = CSS_INQ_EL_INDEX(css);
   *err_ind = 0;
}

/*******************************************************************************
 * pinq_open_struct
 *
 * DESCR:	Check if a structure is open for appending or editing.
 * RETURNS:	N/A
 */

void pinq_open_struct(
   Pint *err_ind,
   Popen_struct_status *status,
   Pint *struct_id
   )
{
   if (CSS_CUR_STRUCTP(css)) {
      *struct_id = CSS_CUR_STRUCT_ID(css);
      *status = PSTRUCT_OPEN;
   }
   else {
      *status = PSTRUCT_NONE;
   }

   *err_ind = 0;
}

