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
 * popen_struct
 *
 * DESCR:	Opens a structure for appending or editing.
 * RETURNS:	N/A
 */

void popen_struct(
   Pint struct_id
   )
{
   ERR_SET_CUR_FUNC(erh, Pfn_open_struct);

   if (PSL_STRUCT_STATE(psl) == PSTRUCT_ST_STCL) {
      if (phg_css_open_struct(css, struct_id) != NULL) {
         PSL_STRUCT_STATE(psl) = PSTRUCT_ST_STOP;
         psl->open_struct = struct_id;
      }
      ERR_FLUSH(erh);
   }
   else {
      ERR_REPORT(erh, ERR6);
   }
}

/*******************************************************************************
 * pclose_struct
 *
 * DESCR:	Closes a previously opened structure.
 * RETURNS:	N/A
 */

void pclose_struct(
   void
   )
{
   Struct_handle str;
   Css_ws_list   ws_list;

   ERR_SET_CUR_FUNC(erh, Pfn_close_struct);

   if (PSL_STRUCT_STATE(psl) == PSTRUCT_ST_STOP) {
      ws_list = CSS_GET_WS_ON(CSS_CUR_STRUCTP(css));
      str = phg_css_close_struct(css);
      if (ws_list && str) {
         for (; ws_list->wsh; ws_list++)
            if (ws_list->wsh->close_struct)
                (*ws_list->wsh->close_struct)(ws_list->wsh, str);
      }
      PSL_STRUCT_STATE(psl) = PSTRUCT_ST_STCL;
   }
   else {
      ERR_REPORT(erh, ERR5);
   }
}

/*******************************************************************************
 * pset_elem_ptr
 *
 * DESCR:	Make the requested index the "currently active element".
 * 		NOTE: 0 means set element to even before the very first
 * 		element; 1 means set elptr to very first element.
 * RETURNS:	N/A
 */

void pset_elem_ptr(
   Pint elem_ptr_value
   )
{
   El_handle   ep;
   Css_ws_list ws_list;

   ws_list = CSS_GET_WS_ON(CSS_CUR_STRUCTP(css));
   ep = phg_css_set_ep(css, PHG_ARGS_SETEP_ABS, elem_ptr_value);
   if (ws_list && ep) {
      for (; ws_list->wsh; ws_list++) {
         if (ws_list->wsh->move_ep)
             (*ws_list->wsh->move_ep)(ws_list->wsh, ep);
      }
   }
}

/*******************************************************************************
 * poffset_elem_ptr
 *
 * DESCR:	Offsets the element pointer forward by the given number of
 * 		elements.
 * 		Internal call to pset_elem_ptr does the verification work
 * 		on the index.
 * RETURNS:	N/A
 */

void poffset_elem_ptr(
   Pint elem_ptr_offset
   )
{
   El_handle   ep;
   Css_ws_list ws_list;

   ws_list = CSS_GET_WS_ON(CSS_CUR_STRUCTP(css));
   ep = phg_css_set_ep(css, PHG_ARGS_SETEP_REL, elem_ptr_offset);
   if (ws_list && ep) {
      for (; ws_list->wsh; ws_list++) {
         if (ws_list->wsh->move_ep)
             (*ws_list->wsh->move_ep)(ws_list->wsh, ep);
      }
   }
}

/*******************************************************************************
 * pset_elem_ptr_label
 *
 * DESCR:	Searches forward in the structure list for the specified
 * 		label, and makes that the current element.  Search begins
 * 		with the element after (to the right) of the current one.
 * 		Question: should I include the current element in the search?
 * 		Or does the search begin with the first element after the
 * 		current one? 
 * 		This version gives fatal error if label not found.
 * RETURNS:	N/A
 */

void pset_elem_ptr_label(
   Pint label_id
   )
{
   El_handle   ep;
   Css_ws_list ws_list;

   ws_list = CSS_GET_WS_ON(CSS_CUR_STRUCTP(css));
   ep = phg_css_set_ep(css, PHG_ARGS_SETEP_LABEL, label_id);
   if (ws_list && ep) {
      for (; ws_list->wsh; ws_list++) {
         if (ws_list->wsh->move_ep)
             (*ws_list->wsh->move_ep)(ws_list->wsh, ep);
      }
   }
}

/*******************************************************************************
 * pdel_elem
 *
 * DESCR:	Deletes the current element.  The element pointer is left
 * 		pointing to the element just before (to the left) the one to
 * 		be killed, if any.
 * RETURNS:	N/A
 */

void pdel_elem(
   void
   )
{
   El_handle            ep1, ep2;
   Phg_args_del_el_data data;
   Struct_handle        structh;
   Css_ws_list          ws_list;
   Ws_handle            callback_list[MAX_NO_OPEN_WS];
   Ws_handle            *wsp = callback_list;

   structh = CSS_CUR_STRUCTP(css);
   ws_list = CSS_GET_WS_ON(structh);
   data.struct_id = structh->struct_id;
   phg_css_el_delete_list(css, PHG_ARGS_DEL_CURRENT, &data, &ep1, &ep2);
   if (ep1 && ep2) {
      if (ws_list) {
         for (; ws_list->wsh; ws_list++) {
            if ( (*ws_list->wsh->delete_el)(ws_list->wsh, structh,
                    ep1, ep2, WS_PRE_CSS_DELETE ) )
                *wsp++ = ws_list->wsh;
         }
      }

      phg_css_delete_el(css, PHG_ARGS_DEL_CURRENT, &data, ep1, ep2);

      while (wsp-- != callback_list) {
         (void)(*(*wsp)->delete_el)((*wsp), structh, ep1, ep2,
             WS_POST_CSS_DELETE );
      }
   }
}

/*******************************************************************************
 * pdel_elem_range
 *
 * DESCR:	Deletes all elements within and on the bounds of the given
 * 		range.  The element pointer is left pointing to the element
 *		just prior to the first element deleted.
 * RETURNS:	N/A
 */

void pdel_elem_range(
   Pint elem_ptr1_value,
   Pint elem_ptr2_value
   )
{
   El_handle            ep1, ep2;
   Phg_args_del_el_data data;
   Struct_handle        structh;
   Css_ws_list          ws_list;
   Ws_handle            callback_list[MAX_NO_OPEN_WS];
   Ws_handle            *wsp = callback_list;

   structh = CSS_CUR_STRUCTP(css);
   ws_list = CSS_GET_WS_ON(structh);
   data.struct_id = structh->struct_id;
   data.ep_values.ep1 = elem_ptr1_value;
   data.ep_values.ep2 = elem_ptr2_value;
   phg_css_el_delete_list(css, PHG_ARGS_DEL_RANGE, &data, &ep1, &ep2);
   if (ep1 && ep2) {
      if (ws_list) {
         for (; ws_list->wsh; ws_list++) {
            if ( (*ws_list->wsh->delete_el)(ws_list->wsh, structh,
                    ep1, ep2, WS_PRE_CSS_DELETE ) )
                *wsp++ = ws_list->wsh;
         }
      }

      phg_css_delete_el(css, PHG_ARGS_DEL_RANGE, &data, ep1, ep2);

      while (wsp-- != callback_list) {
         (void)(*(*wsp)->delete_el)((*wsp), structh, ep1, ep2,
             WS_POST_CSS_DELETE );
      }
   }
}

/*******************************************************************************
 * pdel_elems_labels
 *
 * DESCR:	Deletes all elements in the structure that lie between the
 * 		given labels, but not the labels themselves.  The element
 * 		pointer is left pointing to the first label.
 * RETURNS:	N/A
 */

void pdel_elems_labels(
   Pint label1_id,
   Pint label2_id
   )
{
   El_handle            ep1, ep2;
   Phg_args_del_el_data data;
   Struct_handle        structh;
   Css_ws_list          ws_list;
   Ws_handle            callback_list[MAX_NO_OPEN_WS];
   Ws_handle            *wsp = callback_list;

   structh = CSS_CUR_STRUCTP(css);
   ws_list = CSS_GET_WS_ON(structh);
   data.struct_id = structh->struct_id;
   data.label_range.label1 = label1_id;
   data.label_range.label2 = label2_id;
   phg_css_el_delete_list(css, PHG_ARGS_DEL_LABEL, &data, &ep1, &ep2);
   if (ep1 && ep2) {
      if (ws_list) {
         for (; ws_list->wsh; ws_list++) {
            if ( (*ws_list->wsh->delete_el)(ws_list->wsh, structh,
                    ep1, ep2, WS_PRE_CSS_DELETE ) )
                *wsp++ = ws_list->wsh;
         }
      }

      phg_css_delete_el(css, PHG_ARGS_DEL_LABEL, &data, ep1, ep2);

      while (wsp-- != callback_list) {
         (void)(*(*wsp)->delete_el)((*wsp), structh, ep1, ep2,
             WS_POST_CSS_DELETE );
      }
   }
}

/*******************************************************************************
 * pcopy_all_elems_struct
 *
 * DESCR:	Copies the elements of another structure into the open
 * 		structure after the current element, which is updated to
 * 		point to the last element inserted.  A structure can be
 * 		copied into itself.
 * RETURNS:	N/A
 */

void pcopy_all_elems_struct(
   Pint struct_id
   )
{
   El_handle     ep;
   Struct_handle str;
   Css_ws_list   ws_list;

   if ((str = CSS_STRUCT_EXISTS(css, struct_id)) != NULL) {
      ws_list = CSS_GET_WS_ON(CSS_CUR_STRUCTP(css));
      /* Get the element pointer before it changes. */
      ep = CSS_CUR_ELP(css);
      if (phg_css_copy_struct(css, str) && ep && ws_list) {
         for (; ws_list->wsh; ws_list++)
              (*ws_list->wsh->copy_struct)(ws_list->wsh, ep);
      }
   }
}

