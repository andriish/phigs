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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <sys/types.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>
#include <phigs/util.h>
#include <phigs/private/wsxP.h>
#include <phigs/private/wsglP.h>

/*******************************************************************************
 * wsgl_init
 *
 * DESCR:	Initialize renderer
 * RETURNS:	Non zero or zero on error
 */

int wsgl_init(
   Ws *ws,
   Pgcolr *background,
   Pint select_size
   )
{
   Wsgl_handle wsgl;

   wsgl = (Wsgl_handle) malloc(sizeof(Wsgl) + 4 * sizeof(GLuint) * select_size);
   if (wsgl == NULL) {
      return FALSE;
   }
   memset(wsgl, 0, sizeof(Wsgl));
   wsgl->struct_stack = stack_create(sizeof(Ws_struct), 10);
   if (wsgl->struct_stack == NULL) {
      free(wsgl);
      return FALSE;
   }

   wsgl->attr.bundl_group = phg_attr_group_create();
   if (wsgl->attr.bundl_group == NULL) {
      stack_destroy(wsgl->struct_stack);
      free(wsgl);
      return FALSE;
   }

   wsgl->attr.indiv_group = phg_attr_group_create();
   if (wsgl->attr.indiv_group == NULL) {
      phg_attr_group_destroy(wsgl->attr.bundl_group);
      stack_destroy(wsgl->struct_stack);
      free(wsgl);
      return FALSE;
   }

   wsgl->attr.asf_nameset = phg_nset_create(WS_MAX_ASF_FLAGS);
   if (wsgl->attr.asf_nameset == NULL) {
      phg_attr_group_destroy(wsgl->attr.indiv_group);
      phg_attr_group_destroy(wsgl->attr.bundl_group);
      stack_destroy(wsgl->struct_stack);
      free(wsgl);
      return FALSE;
   }

   wsgl->cur_nameset = phg_nset_create(WS_MAX_NAMES_IN_NAMESET);
   if (wsgl->cur_nameset == NULL) {
      phg_nset_destroy(wsgl->attr.asf_nameset);
      phg_attr_group_destroy(wsgl->attr.indiv_group);
      phg_attr_group_destroy(wsgl->attr.bundl_group);
      stack_destroy(wsgl->struct_stack);
      free(wsgl);
      return FALSE;
   }

   memcpy(&wsgl->background, background, sizeof(Pgcolr));
   wsgl->render_mode = WS_RENDER_MODE_DRAW;
   wsgl->select_size = select_size;
   wsgl->select_buf  = (unsigned *) &wsgl[1];

   ws->render_context = wsgl;

   return TRUE;
}

/*******************************************************************************
 * wsgl_close
 *
 * DESCR:	Close
 * RETURNS:	N/A
 */

void wsgl_close(
   Ws *ws
   )
{
   Wsgl_handle wsgl = ws->render_context;

   phg_nset_destroy(wsgl->cur_nameset);
   phg_nset_destroy(wsgl->attr.asf_nameset);
   phg_attr_group_destroy(wsgl->attr.indiv_group);
   phg_attr_group_destroy(wsgl->attr.bundl_group);
   free(wsgl->struct_stack);
   free(ws->render_context);
   free(ws);
}

/*******************************************************************************
 * wsgl_set_window
 *
 * DESCR:	Set render window coordinates
 * RETURNS:	N/A
 */

void wsgl_set_window(
   Ws *ws,
   Plimit3 *win
   )
{
   Wsgl_handle wsgl = ws->render_context;

   wsgl->win_changed = 1;
   memcpy(&wsgl->curr_win, win, sizeof(Plimit3));
}

/*******************************************************************************
 * wsgl_set_viewport
 *
 * DESCR:	Set render window viewport
 * RETURNS:	N/A
 */

void wsgl_set_viewport(
   Ws *ws,
   Plimit3 *vp
   )
{
   Wsgl_handle wsgl = ws->render_context;

   wsgl->vp_changed = 1;
   memcpy(&wsgl->curr_vp, vp, sizeof(Plimit3));
}

/*******************************************************************************
 * wsgl_set_hlhsr_mode
 *
 * DESCR:	Set render depth mode
 * RETURNS:	N/A
 */

void wsgl_set_hlhsr_mode(
   Ws *ws,
   Pint hlhsr_mode
   )
{
   Wsgl_handle wsgl = ws->render_context;

   wsgl->hlhsr_mode = hlhsr_mode;
   wsgl->hlhsr_changed = 1;
}

/*******************************************************************************
 * wsgl_clear
 *
 * DESCR:	Clear render window
 * RETURNS:	N/A
 */

void wsgl_clear(
   Ws *ws
   )
{
#ifdef DEBUG
   printf("wsgl_clear\n");
#endif

   glXMakeCurrent(ws->display, ws->drawable_id, ws->glx_context);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   if (ws->has_double_buffer) {
      glXSwapBuffers(ws->display, ws->drawable_id);
   }
   else {
      glFlush();
   }
}

/*******************************************************************************
 * wsgl_flush
 *
 * DESCR:	Flush settings to render window
 * RETURNS:	N/A
 */

void wsgl_flush(
   Ws *ws
   )
{
   Ws_xform ws_xform;
   int clear_flag = 0;
   Wsgl_handle wsgl = ws->render_context;

   glXMakeCurrent(ws->display, ws->drawable_id, ws->glx_context);

   if (wsgl->vp_changed || wsgl->win_changed) {
      phg_wsx_compute_ws_transform(&wsgl->curr_win, &wsgl->curr_vp, &ws_xform);

#ifdef DEBUG
      printf("View: %f %f %f\n"
             "      %f %f %f\n",
             ws_xform.offset.x,
             ws_xform.offset.y,
             ws_xform.offset.z,
             ws_xform.scale.x,
             ws_xform.scale.y,
             ws_xform.scale.z);
#endif

      glViewport((GLint)   (ws_xform.offset.x - ws_xform.scale.x),
                 (GLint)   (ws_xform.offset.y - ws_xform.scale.y),
                 (GLsizei) (ws_xform.scale.x * 2.0),
                 (GLsizei) (ws_xform.scale.y * 2.0));

      glDepthRange(ws_xform.scale.z, ws_xform.offset.z);

      if (wsgl->vp_changed) {
         wsgl->vp_changed = 0;

         ws->ws_rect.x      = (int) wsgl->curr_vp.x_min;
         ws->ws_rect.y      = (int) wsgl->curr_vp.y_min;
         ws->ws_rect.width  = (int) wsgl->curr_vp.x_max - wsgl->curr_vp.x_min;
         ws->ws_rect.height = (int) wsgl->curr_vp.y_max - wsgl->curr_vp.y_min;

#ifdef DEBUG
         printf("Viewport changed: %d x %d\n",
                ws->ws_rect.width,
                ws->ws_rect.height);
#endif

         XResizeWindow(ws->display,
                       ws->drawable_id,
                       wsgl->curr_vp.x_max,
                       wsgl->curr_vp.y_max);
      }

      if (wsgl->win_changed) {
         wsgl->win_changed = 0;
      }

      clear_flag = 1;
   }

   if (wsgl->hlhsr_changed) {
      if (wsgl->hlhsr_mode == PHIGS_HLHSR_MODE_ZBUFF) {
#ifdef DEBUG
         printf("Enable z-buffer\n");
#endif
         glEnable(GL_DEPTH_TEST);
      }
      else if (wsgl->hlhsr_mode == PHIGS_HLHSR_MODE_NONE) {
#ifdef DEBUG
         printf("Disable z-buffer\n");
#endif
         glDisable(GL_DEPTH_TEST);
      }
      wsgl->hlhsr_changed = 0;
   }

   glClearColor(wsgl->background.val.general.x,
                wsgl->background.val.general.y,
                wsgl->background.val.general.z,
                0.0);

   if (clear_flag) {
      wsgl_clear(ws);
   }
}

/*******************************************************************************
 * wsgl_begin_rendering
 *
 * DESCR:	Start a rendiering session for workstation
 * RETURNS:	N/A
 */

void wsgl_begin_rendering(
   Ws *ws
   )
{
   Wsgl_handle wsgl = ws->render_context;

#ifdef DEBUG
   printf("Begin rendering\n");
#endif

   glXMakeCurrent(ws->display, ws->drawable_id, ws->glx_context);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   phg_set_hlhsr_id(PHIGS_HLHSR_ID_OFF);
   phg_mat_identity(wsgl->local_tran);
   phg_set_line_ind(ws, wsgl->attr.bundl_group, 0);
   phg_set_line_ind(ws, wsgl->attr.indiv_group, 0);
   phg_set_marker_ind(ws, wsgl->attr.bundl_group, 0);
   phg_set_marker_ind(ws, wsgl->attr.indiv_group, 0);
   phg_set_text_ind(ws, wsgl->attr.bundl_group, 0);
   phg_set_text_ind(ws, wsgl->attr.indiv_group, 0);
   phg_set_edge_ind(ws, wsgl->attr.bundl_group, 0);
   phg_set_edge_ind(ws, wsgl->attr.indiv_group, 0);
   phg_set_int_ind(ws, wsgl->attr.bundl_group, 0);
   phg_set_int_ind(ws, wsgl->attr.indiv_group, 0);
   phg_nset_names_clear_all(wsgl->attr.asf_nameset);
   phg_set_view_ind(ws, 0);
   phg_nset_names_clear_all(wsgl->cur_nameset);
}

/*******************************************************************************
 * wsgl_end_rendering
 *
 * DESCR:	End a rendiering session
 * RETURNS:	N/A
 */

void wsgl_end_rendering(
   Ws *ws
   )
{
#ifdef DEBUG
   printf("End rendering\n");
#endif

   if (ws->has_double_buffer) {
      glXSwapBuffers(ws->display, ws->drawable_id);
   }
   else {
      glFlush();
   }
}

/*******************************************************************************
 * store_cur_struct
 *
 * DESCR:	Store current structure information helper function
 * RETURNS:	N/A
 */

static void store_cur_struct(
   Ws *ws
   )
{
   uint32_t encode;
   Wsgl_handle wsgl = ws->render_context;

   if (wsgl->render_mode == WS_RENDER_MODE_SELECT) {
#ifdef DEBUG
      printf("\tOffset: %d, Pick ID: %d\n",
             wsgl->cur_struct.offset,
             wsgl->pick_id);
#endif
      encode = (wsgl->cur_struct.offset << 16) | wsgl->pick_id;
      glLoadName(encode);
   }
}

/*******************************************************************************
 * update_cur_struct
 *
 * DESCR:	Update current structure information helper function
 * RETURNS:	N/A
 */

static void update_cur_struct(
   Ws *ws
   )
{
   Wsgl_handle wsgl = ws->render_context;

   store_cur_struct(ws);
   wsgl->cur_struct.offset++;
}

static int check_draw_primitive(
   Ws *ws
   )
{
   int status;
   Wsgl_handle wsgl = ws->render_context;

   switch (wsgl->render_mode) {
      case WS_RENDER_MODE_DRAW:
         if (wsgl->invis_filter.used) {
            if (!phg_nset_names_intersect(wsgl->cur_nameset,
                                         wsgl->invis_filter.incl) ||
                phg_nset_names_intersect(wsgl->cur_nameset,
                                         wsgl->invis_filter.excl)) {
               status = TRUE;
            }
            else {
               status = FALSE;
            }
         }
         else {
            status = TRUE;
         }
         break;

      case WS_RENDER_MODE_SELECT:
         if (wsgl->pick_filter.used) {
            if (phg_nset_names_intersect(wsgl->cur_nameset,
                                         wsgl->pick_filter.incl) &&
                !phg_nset_names_intersect(wsgl->cur_nameset,
                                          wsgl->pick_filter.excl)) {
               status = TRUE;
            }
            else {
               status = FALSE;
            }
         }
         else {
            status = TRUE;
         }
         break;

      default:
         status = TRUE;
         break;
   }

   return status;
}

/*******************************************************************************
 * wsgl_begin_structure
 *
 * DESCR:	Mark the beginning of a new structure element
 * RETURNS:	N/A
 */

void wsgl_begin_structure(
   Ws *ws,
   Pint struct_id
   )
{
   Wsgl_handle wsgl = ws->render_context;

#ifdef DEBUG
   printf("Begin new structure element: %d\n", struct_id);
   printf("Push: offset = %d\n",
          wsgl->cur_struct.offset);
#endif

   stack_push(wsgl->struct_stack, (caddr_t) &wsgl->cur_struct);
   wsgl->cur_struct.offset  = 0;

   if (wsgl->render_mode == WS_RENDER_MODE_SELECT) {
#ifdef DEBUG
      printf("\tPush name: %d\n", struct_id);
#endif
      glPushName(struct_id);
      glPushName(-1);
      store_cur_struct(ws);
   }
}

/*******************************************************************************
 * wsgl_end_structure
 *
 * DESCR:	Mark the ending of a structure element
 * RETURNS:	N/A
 */

void wsgl_end_structure(
   Ws *ws
   )
{
   Wsgl_handle wsgl = ws->render_context;

   stack_pop(wsgl->struct_stack, (caddr_t) &wsgl->cur_struct);
#ifdef DEBUG
   printf("End structure element\n");
   printf("Pop: offset = %d\n",
          wsgl->cur_struct.offset);
#endif

   if (wsgl->render_mode == WS_RENDER_MODE_SELECT) {
#ifdef DEBUG
      printf("\tPop name\n");
#endif
      glPopName();
      glPopName();
   }
}

/*******************************************************************************
 * wsgl_render_element
 *
 * DESCR:	Render element to current workstation rendering window
 * RETURNS:	N/A
 */

void wsgl_render_element(
   Ws *ws,
   El_handle el
   )
{
   Wsgl_handle wsgl = ws->render_context;

   update_cur_struct(ws);
   switch (el->eltype) {
      case PELEM_LABEL:
         break;

      case PELEM_PICK_ID:
         wsgl->pick_id = PHG_INT(el);
         break;

      case PELEM_ADD_NAMES_SET:
         phg_add_names_set(ws, PHG_INT_LIST(el));
         break;

      case PELEM_REMOVE_NAMES_SET:
         phg_remove_names_set(ws, PHG_INT_LIST(el));
         break;

      case PELEM_HLHSR_ID:
         phg_set_hlhsr_id(PHG_INT(el));
         break;

      case PELEM_INDIV_ASF:
         phg_set_asf(&wsgl->attr, PHG_ASF_INFO(el));
         break;

      case PELEM_INT_IND:
         phg_set_int_ind(ws, wsgl->attr.bundl_group, PHG_INT(el));
         break;

      case PELEM_INT_COLR_IND:
         phg_get_colr_ind(ws,
                          &wsgl->attr.indiv_group->int_bundle.colr,
                          PHG_INT(el));
         break;

      case PELEM_INT_COLR:
         memcpy(&wsgl->attr.indiv_group->int_bundle.colr,
                PHG_COLR(el),
                sizeof(Pgcolr));
         break;

      case PELEM_INT_STYLE:
         wsgl->attr.indiv_group->int_bundle.style = PHG_INT_STYLE(el);
         break;

      case PELEM_INT_STYLE_IND:
         wsgl->attr.indiv_group->int_bundle.style_ind = PHG_INT(el);
         break;

      case PELEM_EDGE_IND:
         phg_set_edge_ind(ws, wsgl->attr.bundl_group, PHG_INT(el));
         break;

      case PELEM_EDGE_COLR_IND:
         phg_get_colr_ind(ws,
                          &wsgl->attr.indiv_group->edge_bundle.colr,
                          PHG_INT(el));
         break;

      case PELEM_EDGE_COLR:
         memcpy(&wsgl->attr.indiv_group->edge_bundle.colr,
                PHG_COLR(el),
                sizeof(Pgcolr));
         break;

      case PELEM_EDGEWIDTH:
         wsgl->attr.indiv_group->edge_bundle.width = PHG_FLOAT(el);
         break;

      case PELEM_EDGETYPE:
         wsgl->attr.indiv_group->edge_bundle.type = PHG_INT(el);
         break;

      case PELEM_EDGE_FLAG:
         wsgl->attr.indiv_group->edge_bundle.flag = PHG_EDGE_FLAG(el);
         break;

      case PELEM_MARKER_IND:
         phg_set_marker_ind(ws, wsgl->attr.bundl_group, PHG_INT(el));
         break;

      case PELEM_MARKER_COLR_IND:
         phg_get_colr_ind(ws,
                          &wsgl->attr.indiv_group->marker_bundle.colr,
                          PHG_INT(el));
         break;

      case PELEM_MARKER_COLR:
         memcpy(&wsgl->attr.indiv_group->marker_bundle.colr,
                PHG_COLR(el),
                sizeof(Pgcolr));
         break;

      case PELEM_MARKER_SIZE:
         wsgl->attr.indiv_group->marker_bundle.size = PHG_FLOAT(el);
         break;

      case PELEM_MARKER_TYPE:
         wsgl->attr.indiv_group->marker_bundle.type = PHG_INT(el);
         break;

      case PELEM_TEXT_IND:
         phg_set_text_ind(ws, wsgl->attr.bundl_group, PHG_INT(el));
         break;

      case PELEM_TEXT_COLR_IND:
         phg_get_colr_ind(ws,
                          &wsgl->attr.indiv_group->text_bundle.colr,
                          PHG_INT(el));
         break;

      case PELEM_TEXT_COLR:
         memcpy(&wsgl->attr.indiv_group->text_bundle.colr,
                PHG_COLR(el),
                sizeof(Pgcolr));
         break;

      case PELEM_TEXT_FONT:
         wsgl->attr.indiv_group->text_bundle.font = PHG_INT(el);
         break;

      case PELEM_LINE_IND:
         phg_set_line_ind(ws, wsgl->attr.bundl_group, PHG_INT(el));
         break;

      case PELEM_LINE_COLR_IND:
         phg_get_colr_ind(ws,
                          &wsgl->attr.indiv_group->line_bundle.colr,
                          PHG_INT(el));
         break;

      case PELEM_LINE_COLR:
         memcpy(&wsgl->attr.indiv_group->line_bundle.colr,
                PHG_COLR(el),
                sizeof(Pgcolr));
         break;

      case PELEM_LINEWIDTH:
         wsgl->attr.indiv_group->line_bundle.width = PHG_FLOAT(el);
         break;

      case PELEM_LINETYPE:
         wsgl->attr.indiv_group->line_bundle.type = PHG_INT(el);
         break;

      case PELEM_FILL_AREA:
         if (check_draw_primitive(ws)) {
            phg_draw_fill_area(ws,
                               PHG_POINT_LIST(el),
                               &wsgl->attr);
         }
         break;

      case PELEM_POLYLINE:
         if (check_draw_primitive(ws)) {
            phg_draw_polyline(ws,
                              PHG_POINT_LIST(el),
                              &wsgl->attr
                              );
         }
         break;

      case PELEM_POLYMARKER:
         if (check_draw_primitive(ws)) {
            phg_draw_polymarker(ws,
                                PHG_POINT_LIST(el),
                                &wsgl->attr);
         }
         break;

      case PELEM_FILL_AREA3:
         if (check_draw_primitive(ws)) {
            phg_draw_fill_area3(ws,
                                PHG_POINT_LIST3(el),
                                &wsgl->attr);
         }
         break;

      case PELEM_POLYLINE3:
         if (check_draw_primitive(ws)) {
            phg_draw_polyline3(ws,
                               PHG_POINT_LIST3(el),
                               &wsgl->attr);
         }
         break;

      case PELEM_POLYMARKER3:
         if (check_draw_primitive(ws)) {
            phg_draw_polymarker3(ws,
                                 PHG_POINT_LIST3(el),
                                 &wsgl->attr);
         }
         break;

      case PELEM_LOCAL_MODEL_TRAN3:
         switch (PHG_LOCAL_TRAN3(el)->compose_type) {
            case PTYPE_PRECONCAT:
               phg_mat_mul(wsgl->local_tran,
                           wsgl->local_tran,
                           PHG_LOCAL_TRAN3(el)->matrix);
            break;
            case PTYPE_POSTCONCAT:
               phg_mat_mul(wsgl->local_tran,
                           PHG_LOCAL_TRAN3(el)->matrix,
                           wsgl->local_tran);
            break;
            case PTYPE_REPLACE:
            default:
               phg_mat_copy(wsgl->local_tran, PHG_LOCAL_TRAN3(el)->matrix);
            break;
         }
         phg_update_modelview(ws);
         break;

      case PELEM_VIEW_IND:
         phg_set_view_ind(ws, PHG_INT(el));
         break;

      default:
         css_print_eltype(el->eltype);
         printf(" not processed\n");
         break;
   }
}

/*******************************************************************************
 * wsgl_set_filter
 * 
 * DESCR:       Set filter
 * RETURNS:     N/A
 */

void wsgl_set_filter(
   Ws *ws,
   Phg_args_flt_type type,
   Nameset incl,
   Nameset excl
   )
{
   Wsgl_handle wsgl = ws->render_context;

   switch (type) {
      case PHG_ARGS_FLT_INVIS:
         wsgl->invis_filter.used = TRUE;
         wsgl->invis_filter.incl = incl;
         wsgl->invis_filter.excl = excl;
#ifdef DEBUG
         printf("Include filter:\n");
         phg_nset_print(wsgl->invis_filter.incl);
         printf("Exclude filter:\n");
         phg_nset_print(wsgl->invis_filter.excl);
#endif
         break;

      case PHG_ARGS_FLT_PICK:
         wsgl->pick_filter.used = TRUE;
         wsgl->pick_filter.incl = incl;
         wsgl->pick_filter.excl = excl;
#ifdef DEBUG
         printf("Include filter:\n");
         phg_nset_print(wsgl->pick_filter.incl);
         printf("Exclude filter:\n");
         phg_nset_print(wsgl->pick_filter.excl);
#endif
         break;

      default:
         /* TODO: Other filter types */
         break;
   }
}

/*******************************************************************************
 * wsgl_begin_pick
 * 
 * DESCR:       Begin pick process
 * RETURNS:     N/A
 */
 
void wsgl_begin_pick(
   Ws *ws,
   Ws_hit_box *box
   )
{
   GLint vp[4];
   Pvec3 v;
   Pmatrix3 trans, scale;
   Wsgl_handle wsgl = ws->render_context;
   wsgl->render_mode = WS_RENDER_MODE_SELECT;

#ifdef DEBUG
   printf("Begin pick\n");
#endif
   glXMakeCurrent(ws->display, ws->drawable_id, ws->glx_context);

   glGetIntegerv(GL_VIEWPORT, vp);
   v.delta_x = ((float) vp[2] - 2.0 * ((float) box->x - (float) vp[0])) /
               box->distance;
   v.delta_y = ((float) vp[3] - 2.0 * ((float) box->y - (float) vp[1])) /
               box->distance;
   v.delta_z = 0.0;
   phg_mat_translate(trans, &v);

   v.delta_x = (float) vp[2] / box->distance;
   v.delta_y = (float) vp[3] / box->distance;
   v.delta_z = 1.0;
   phg_mat_scale(scale, &v);

   phg_mat_mul(wsgl->pick_tran, trans, scale);
#ifdef DEBUG
   phg_mat_print(wsgl->pick_tran);
   printf("\n");
#endif

   phg_mat_identity(wsgl->local_tran);
   phg_set_line_ind(ws, wsgl->attr.bundl_group, 0);
   phg_set_line_ind(ws, wsgl->attr.indiv_group, 0);
   phg_set_marker_ind(ws, wsgl->attr.bundl_group, 0);
   phg_set_marker_ind(ws, wsgl->attr.indiv_group, 0);
   phg_set_text_ind(ws, wsgl->attr.bundl_group, 0);
   phg_set_text_ind(ws, wsgl->attr.indiv_group, 0);
   phg_set_edge_ind(ws, wsgl->attr.bundl_group, 0);
   phg_set_edge_ind(ws, wsgl->attr.indiv_group, 0);
   phg_set_int_ind(ws, wsgl->attr.bundl_group, 0);
   phg_set_int_ind(ws, wsgl->attr.indiv_group, 0);
   phg_nset_names_clear_all(wsgl->attr.asf_nameset);
   phg_set_view_ind(ws, 0);
   phg_nset_names_clear_all(wsgl->cur_nameset);
   wsgl->pick_id = 0;

   glSelectBuffer(wsgl->select_size, wsgl->select_buf);
   glRenderMode(GL_SELECT);
   glInitNames();
}

/*******************************************************************************
 * wsgl_end_pick
 * 
 * DESCR:       End pick process
 * RETURNS:     N/A
 */
 
void wsgl_end_pick(
   Ws *ws,
   Pint *err_ind,
   Pint *depth,
   Ws_pick_elmt **elmts
   )
{
   Pint hits;
   Pint i;
   GLuint z, names;
#ifdef DEBUG
   Pint j;
   printf("End pick\n");
#endif

   Wsgl_handle wsgl = ws->render_context;
   GLuint *ptr = wsgl->select_buf;
   GLuint *match = NULL;
   Ws_pick_elmt *data = NULL;
   GLuint zmin = UINT_MAX;
   wsgl->render_mode = WS_RENDER_MODE_DRAW;

   hits = glRenderMode(GL_RENDER);

   for (i = 0; i < hits; i++) {
      names = *ptr / 2;
      ptr++;
      z = *ptr;
      if (z < zmin) {
         zmin = z;
         match = ptr - 1;
      }
#ifdef DEBUG
      ptr += 2;
      printf("Number of name(s) for hit #%d is %d\tAt: %x\n", i, names, z);
      for (j = 0; j < names; j++) {
         printf("\tStruct: %d", *ptr);
         ptr++;
         printf("\tOffset: %d\tPick ID: %d\n",
                (*ptr & 0xffff0000) >> 16, *ptr & 0xffff);
         ptr++;
      }
#else
      ptr += 2 * names + 2;
#endif
   }

   if (match != NULL) {
      names = *match / 2;
#ifdef DEBUG
      printf("\nThe matching hit with %d name(s)\n", names);
#endif
      data = (Ws_pick_elmt *) malloc(sizeof(Ws_pick_elmt) * names);
      if (data == NULL) {
         *depth   = 0;
         *elmts   = NULL;
         *err_ind = ERR900;
      }
      else {
         match += 3;
         for (i = 0; i < names; i++) {
            data[i].sid = *match;
            match++;
            data[i].pickid = *match & 0xffff;
            data[i].offset = (*match & 0xffff0000) >> 16;
#ifdef DEBUG
            printf("\tStruct: %d\tOffset: %d\tPick ID: %d\n",
                   data[i].sid, data[i].offset, data[i].pickid);
#endif
            match++;
         }
         *depth   = names;
         *elmts   = data;
         *err_ind = 0;
      }
   }
   else {
      *depth   = 0;
      *err_ind = 0;
   }
}

