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

void pinq_elem_ptr(Pint *err_ind, Pint *elem_ptr_value)
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
   Pint                *err_ind,
   Popen_struct_status *status,
   Pint                *struct_id)
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

