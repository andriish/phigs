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
#include <phigs/private/wsxP.h>
#include <phigs/private/wsglP.h>

static void phg_update_projection(
   Ws *ws
   );

static void phg_update_modelview(
   Ws *ws
   );

static void phg_set_view_ind(
   Ws *ws,
   Pint ind
   );

void phg_set_hlhsr_id(
   Pint hlhsr_id
   );

static void phg_set_line_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   );

static void phg_set_line_attr(
   Ws *ws,
   Pline_bundle_plus *attr
   );

static void phg_set_int_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   );

static void phg_set_int_attr(
   Ws *ws,
   Pint_bundle_plus *attr
   );

static void phg_set_edge_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   );

static void phg_set_edge_attr(Ws *ws,
   Pedge_bundle_plus *attr
   );

static void phg_set_marker_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   );

static void phg_set_marker_attr(
   Ws *ws,
   Pmarker_bundle_plus *attr
   );

static void phg_set_text_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   );

static void phg_draw_polymarker(
   Ws *ws,
   Ppoint_list *point_list,
   Pattr_group *attr
   );

static void phg_draw_polymarker3(
   Ws *ws,
   Ppoint_list3 *point_list,
   Pattr_group *attr
   );

static void phg_draw_polyline(
   Ws *ws,
   Ppoint_list *point_list,
   Pattr_group *attr
   );

static void phg_draw_polyline3(
   Ws *ws,
   Ppoint_list3 *point_list,
   Pattr_group *attr
   );

static void phg_draw_fill_area(
   Ws *ws,
   Ppoint_list *point_list,
   Pattr_group *attr
   );

static void phg_draw_fill_area3(
   Ws *ws,
   Ppoint_list3 *point_list,
   Pattr_group *attr
   );

#ifdef NOT_YET
static void phg_draw_text(
   Ppoint *pos,
   char *text
   );
#endif /*NOT_YET*/

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

   wsgl->attr_group = phg_attr_group_create();
   if (wsgl->attr_group == NULL) {
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

   phg_attr_group_destroy(wsgl->attr_group);
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
         printf("Type: %x\n", (unsigned) ws->type);
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
   phg_set_line_ind(ws, wsgl->attr_group, 0);
   phg_set_marker_ind(ws, wsgl->attr_group, 0);
   phg_set_text_ind(ws, wsgl->attr_group, 0);
   phg_set_edge_ind(ws, wsgl->attr_group, 0);
   phg_set_int_ind(ws, wsgl->attr_group, 0);
   phg_set_view_ind(ws, 0);
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
 * store_pick_data
 *
 * DESCR:	Store picking information helper function
 * RETURNS:	N/A
 */

static void store_pick_data(
   Ws *ws
   )
{
   uint32_t encode;
   Wsgl_handle wsgl = ws->render_context;

   if (wsgl->render_mode == WS_RENDER_MODE_SELECT) {
#ifdef DEBUG
      printf("\tOffset: %d, Pick ID: %d\n", wsgl->offset, wsgl->pick_id);
#endif
      encode = (wsgl->offset << 16) | wsgl->pick_id;
      glLoadName(encode);
   }
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
#endif

   wsgl->offset = 0;
   if (wsgl->render_mode == WS_RENDER_MODE_SELECT) {
#ifdef DEBUG
      printf("\tPush name: %d\n", struct_id);
#endif
      glPushName(struct_id);
      glPushName(-1);
      store_pick_data(ws);
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

#ifdef DEBUG
   printf("End structure element\n");
#endif

   wsgl->pick_id = 0;
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

   store_pick_data(ws);
   wsgl->offset++;
   switch (el->eltype) {
      case PELEM_LABEL:
         break;

      case PELEM_PICK_ID:
         wsgl->pick_id = PHG_INT(el);
         break;

      case PELEM_HLHSR_ID:
         phg_set_hlhsr_id(PHG_INT(el));
         break;

      case PELEM_INT_IND:
         phg_set_int_ind(ws, wsgl->attr_group, PHG_INT(el));
         break;

      case PELEM_INT_COLR_IND:
         phg_get_colr_ind(ws,
                          &wsgl->attr_group->int_bundle.colr,
                          PHG_INT(el));
         break;

      case PELEM_INT_COLR:
         memcpy(&wsgl->attr_group->int_bundle.colr,
                PHG_COLR(el),
                sizeof(Pgcolr));
         break;

      case PELEM_INT_STYLE:
         wsgl->attr_group->int_bundle.style = PHG_INT_STYLE(el);
         break;

      case PELEM_INT_STYLE_IND:
         wsgl->attr_group->int_bundle.style_ind = PHG_INT(el);
         break;

      case PELEM_EDGE_IND:
         phg_set_edge_ind(ws, wsgl->attr_group, PHG_INT(el));
         break;

      case PELEM_EDGE_COLR_IND:
         phg_get_colr_ind(ws,
                          &wsgl->attr_group->edge_bundle.colr,
                          PHG_INT(el));
         break;

      case PELEM_EDGE_COLR:
         memcpy(&wsgl->attr_group->edge_bundle.colr,
                PHG_COLR(el),
                sizeof(Pgcolr));
         break;

      case PELEM_EDGEWIDTH:
         wsgl->attr_group->edge_bundle.width = PHG_FLOAT(el);
         break;

      case PELEM_EDGETYPE:
         wsgl->attr_group->edge_bundle.type = PHG_INT(el);
         break;

      case PELEM_EDGE_FLAG:
         wsgl->attr_group->edge_bundle.flag = PHG_EDGE_FLAG(el);
         break;

      case PELEM_MARKER_IND:
         phg_set_marker_ind(ws, wsgl->attr_group, PHG_INT(el));
         break;

      case PELEM_MARKER_COLR_IND:
         phg_get_colr_ind(ws,
                          &wsgl->attr_group->marker_bundle.colr,
                          PHG_INT(el));
         break;

      case PELEM_MARKER_COLR:
         memcpy(&wsgl->attr_group->marker_bundle.colr,
                PHG_COLR(el),
                sizeof(Pgcolr));
         break;

      case PELEM_MARKER_SIZE:
         wsgl->attr_group->marker_bundle.size = PHG_FLOAT(el);
         break;

      case PELEM_MARKER_TYPE:
         wsgl->attr_group->marker_bundle.type = PHG_INT(el);
         break;

      case PELEM_TEXT_IND:
         phg_set_text_ind(ws, wsgl->attr_group, PHG_INT(el));
         break;

      case PELEM_TEXT_COLR_IND:
         phg_get_colr_ind(ws,
                          &wsgl->attr_group->text_bundle.colr,
                          PHG_INT(el));
         break;

      case PELEM_TEXT_COLR:
         memcpy(&wsgl->attr_group->text_bundle.colr,
                PHG_COLR(el),
                sizeof(Pgcolr));
         break;

      case PELEM_TEXT_FONT:
         wsgl->attr_group->text_bundle.font = PHG_INT(el);
         break;

      case PELEM_LINE_IND:
         phg_set_line_ind(ws, wsgl->attr_group, PHG_INT(el));
         break;

      case PELEM_LINE_COLR_IND:
         phg_get_colr_ind(ws,
                          &wsgl->attr_group->line_bundle.colr,
                          PHG_INT(el));
         break;

      case PELEM_LINE_COLR:
         memcpy(&wsgl->attr_group->line_bundle.colr,
                PHG_COLR(el),
                sizeof(Pgcolr));
         break;

      case PELEM_LINEWIDTH:
         wsgl->attr_group->line_bundle.width = PHG_FLOAT(el);
         break;

      case PELEM_LINETYPE:
         wsgl->attr_group->line_bundle.type = PHG_INT(el);
         break;

      case PELEM_FILL_AREA:
         phg_draw_fill_area(ws,
                            PHG_POINT_LIST(el),
                            wsgl->attr_group);
         break;

      case PELEM_POLYLINE:
         phg_draw_polyline(ws,
                           PHG_POINT_LIST(el),
                           wsgl->attr_group
                           );
         break;

      case PELEM_POLYMARKER:
         phg_draw_polymarker(ws,
                             PHG_POINT_LIST(el),
                             wsgl->attr_group);
         break;

      case PELEM_FILL_AREA3:
         phg_draw_fill_area3(ws,
                             PHG_POINT_LIST3(el),
                             wsgl->attr_group);
         break;

      case PELEM_POLYLINE3:
         phg_draw_polyline3(ws,
                            PHG_POINT_LIST3(el),
                            wsgl->attr_group);
         break;

      case PELEM_POLYMARKER3:
         phg_draw_polymarker3(ws,
                              PHG_POINT_LIST3(el),
                              wsgl->attr_group);
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
   phg_set_view_ind(ws, 0);

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

/*******************************************************************************
 * phg_set_matrix
 *
 * DESCR:	Setup matrix
 * RETURNS:	N/A
 */

static void phg_set_matrix(
    Pmatrix3 mat,
    int mult
    )
{
   int i, j;
   GLfloat m[16];
   GLfloat *mp = &m[0];

   for (i = 0; i < 4; i++) {
      for (j = 0; j < 4; j++) {
         *mp = mat[j][i];
         mp++;
      }
   }

   if (mult) {
      glMultMatrixf(m);
   }
   else {
      glLoadMatrixf(m);
   }
}

/*******************************************************************************
 * phg_update_projection
 *
 * DESCR:	Update projection matrix
 * RETURNS:	N/A
 */

static void phg_update_projection(
   Ws *ws
   )
{
   Pmatrix3 mat;
   Wsgl_handle wsgl = ws->render_context;

#ifdef DEBUG
   printf("Update projection\n");
#endif

   glMatrixMode(GL_PROJECTION);
   if (wsgl->render_mode == WS_RENDER_MODE_SELECT) {
      phg_mat_mul(mat, wsgl->pick_tran, wsgl->view_rep.map_matrix);
      phg_set_matrix(mat, FALSE);
   }
   else {
      phg_set_matrix(wsgl->view_rep.map_matrix, FALSE);
   }
}

/*******************************************************************************
 * phg_update_modelview
 *
 * DESCR:	Update modelview matrix
 * RETURNS:	N/A
 */

static void phg_update_modelview(
   Ws *ws
   )
{
   Wsgl_handle wsgl = ws->render_context;

#ifdef DEBUG
   printf("Update modelview\n");
#endif

   glMatrixMode(GL_MODELVIEW);
   phg_mat_mul(wsgl->total_tran, wsgl->view_rep.ori_matrix, wsgl->local_tran);
   phg_set_matrix(wsgl->total_tran, FALSE);
}

/*******************************************************************************
 * phg_set_view_ind
 *
 * DESCR:	Setup view
 * RETURNS:	N/A
 */

static void phg_set_view_ind(
   Ws *ws,
   Pint ind
   )
{
   Phg_ret ret;
   Wsgl_handle wsgl = ws->render_context;

   (*ws->inq_representation)(ws,
                             ind,
                             PINQ_REALIZED,
                             PHG_ARGS_VIEWREP,
                             &ret);
   if (ret.err == 0) {
      wsgl->curr_view_index = ind;
      memcpy(&wsgl->view_rep, &ret.data.rep.viewrep, sizeof(Pview_rep3));
      phg_update_projection(ws);
      phg_update_modelview(ws);
   }
}

/*******************************************************************************
 * phg_set_hlhsr_id
 *
 * DESCR:	Setup depth buffer checking
 * RETURNS:	N/A
 */

void phg_set_hlhsr_id(
   Pint hlhsr_id
   )
{
   switch(hlhsr_id) {
      case PHIGS_HLHSR_ID_OFF:
         glDepthFunc(GL_ALWAYS);
      break;

      case PHIGS_HLHSR_ID_ON:
         glDepthFunc(GL_LESS);
      break;

      default:
      break;
   }
}

/*******************************************************************************
 * phg_set_gcolr
 *
 * DESCR:	Setup colour
 * RETURNS:	N/A
 */

static void phg_set_gcolr(
   Pgcolr *gcolr
   )
{
   switch(gcolr->type) {
      case PINDIRECT:
         glIndexi(gcolr->val.ind);
         break;

      case PMODEL_RGB:
         glColor3f(gcolr->val.general.x,
                   gcolr->val.general.y,
                   gcolr->val.general.z);
         break;

      default:
         break;
   }
}

/*******************************************************************************
 * phg_set_line_ind
 *
 * DESCR:	Setup line index
 * RETURNS:	N/A
 */

static void phg_set_line_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   )
{
   Phg_ret ret;

   (*ws->inq_representation)(ws,
                             ind,
                             PINQ_REALIZED,
                             PHG_ARGS_EXTLNREP,
                             &ret);
   if (ret.err == 0) {
      phg_attr_group_set_line_bundle(ws,
                                     attr_group,
                                     &ret.data.rep.extlnrep);
   }
}

/*******************************************************************************
 * phg_set_line_attr
 *
 * DESCR:	Setup line attributes
 * RETURNS:	N/A
 */

static void phg_set_line_attr(
   Ws *ws,
   Pline_bundle_plus *attr
   )
{
   /* Colour */
   phg_set_gcolr(&attr->colr);

   /* Line style */
   switch (attr->type) {
      case PLINE_DASH:
         glLineStipple(1, 0x00ff);
         glEnable(GL_LINE_STIPPLE);
      break;

      case PLINE_DOT:
         glLineStipple(1, 0x0101);
         glEnable(GL_LINE_STIPPLE);
      break;

      case PLINE_DASH_DOT:
         glLineStipple(1, 0x1c47);
         glEnable(GL_LINE_STIPPLE);
      break;

      default:
         glDisable(GL_LINE_STIPPLE);
      break;
   }

   /* Line width */
   glLineWidth(attr->width);
}

/*******************************************************************************
 * phg_set_int_ind
 *
 * DESCR:	Setup interior index
 * RETURNS:	N/A
 */

static void phg_set_int_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   )
{
   Phg_ret ret;

   (*ws->inq_representation)(ws,
                             ind,
                             PINQ_REALIZED,
                             PHG_ARGS_EXTINTERREP,
                             &ret);
   if (ret.err == 0) {
      phg_attr_group_set_int_bundle(ws,
                                    attr_group,
                                    &ret.data.rep.extinterrep);
   }
}

/*******************************************************************************
 * phg_set_int_attr
 *
 * DESCR:	Setup interior attributes
 * RETURNS:	N/A
 */

static void phg_set_int_attr(
   Ws *ws,
   Pint_bundle_plus *attr
   )
{
   /* Colour */
   phg_set_gcolr(&attr->colr);

   if (attr->style == PSTYLE_HATCH) {
      glEnable(GL_POLYGON_STIPPLE);
      glPolygonStipple(wsgl_hatch_tbl[attr->style_ind - 1]);
   }
   else {
      glDisable(GL_POLYGON_STIPPLE);
   }
}

/*******************************************************************************
 * phg_set_edge_ind
 *
 * DESCR:	Setup edge index
 * RETURNS:	N/A
 */

static void phg_set_edge_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   )
{
   Phg_ret ret;

   (*ws->inq_representation)(ws,
                             ind,
                             PINQ_REALIZED,
                             PHG_ARGS_EXTEDGEREP,
                             &ret);
   if (ret.err == 0) {
      phg_attr_group_set_edge_bundle(ws,
                                     attr_group,
                                     &ret.data.rep.extedgerep);
   }
}

/*******************************************************************************
 * phg_set_edge_attr
 *
 * DESCR:	Setup interior attributes
 * RETURNS:	N/A
 */

static void phg_set_edge_attr(
   Ws *ws,
   Pedge_bundle_plus *attr
   )
{
   /* Colour */
   phg_set_gcolr(&attr->colr);

   /* Edge width */
   glLineWidth(attr->width);

   /* Line style */
   switch (attr->type) {
      case PLINE_DASH:
         glLineStipple(1, 0x00ff);
         glEnable(GL_LINE_STIPPLE);
      break;

      case PLINE_DOT:
         glLineStipple(1, 0x0101);
         glEnable(GL_LINE_STIPPLE);
      break;

      case PLINE_DASH_DOT:
         glLineStipple(1, 0x1c47);
         glEnable(GL_LINE_STIPPLE);
      break;

      default:
         glDisable(GL_LINE_STIPPLE);
      break;
   }
}


/*******************************************************************************
 * phg_set_marker_ind
 *
 * DESCR:	Setup marker index
 * RETURNS:	N/A
 */

static void phg_set_marker_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   )
{
   Phg_ret ret;

   (*ws->inq_representation)(ws,
                             ind,
                             PINQ_REALIZED,
                             PHG_ARGS_EXTMKREP,
                             &ret);
   if (ret.err == 0) {
      phg_attr_group_set_marker_bundle(ws,
                                       attr_group,
                                       &ret.data.rep.extmkrep);
   }
}

/*******************************************************************************
 * phg_set_marker_attr
 *
 * DESCR:	Setup marker attributes
 * RETURNS:	N/A
 */

static void phg_set_marker_attr(
   Ws *ws,
   Pmarker_bundle_plus *attr
   )
{
   /* Colour */
   phg_set_gcolr(&attr->colr);
}

/*******************************************************************************
 * phg_set_text_ind
 *
 * DESCR:	Setup text index
 * RETURNS:	N/A
 */

static void phg_set_text_ind(
   Ws *ws,
   Pattr_group *attr_group,
   Pint ind
   )
{
   Phg_ret ret;

   (*ws->inq_representation)(ws,
                             ind,
                             PINQ_REALIZED,
                             PHG_ARGS_EXTTXREP,
                             &ret);
    if (ret.err == 0) {
      phg_attr_group_set_text_bundle(ws,
                                     attr_group,
                                     &ret.data.rep.exttxrep);
   }
}

/*******************************************************************************
 * phg_draw_marker_dot
 *
 * DESCR:	Draw marker dots
 * RETURNS:	N/A
 */

static void phg_draw_marker_dot(
   Ppoint_list *point_list,
   Pfloat scale
   )
{
   int i;

   glPointSize(scale);
   glBegin(GL_POINTS);
   for (i = 0; i < point_list->num_points; i++)
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y);
   glEnd();
}

/*******************************************************************************
 * phg_draw_marker_plus
 *
 * DESCR:	Draw marker pluses
 * RETURNS:	N/A
 */

static void phg_draw_marker_plus(
   Ppoint_list *point_list,
   Pfloat scale
   )
{
   int i;
   float half_scale;

   half_scale = scale / 2.0;

   glLineWidth(1.0);
   glDisable(GL_LINE_STIPPLE);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++) {
      glVertex2f(point_list->points[i].x - half_scale,
                 point_list->points[i].y);
      glVertex2f(point_list->points[i].x + half_scale,
                 point_list->points[i].y);
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y - half_scale);
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y + half_scale);
   }
   glEnd();
}

/*******************************************************************************
 * phg_draw_marker_asterisk
 *
 * DESCR:	Draw marker asterisks
 * RETURNS:	N/A
 */

static void phg_draw_marker_asterisk(
   Ppoint_list *point_list,
   Pfloat scale
   )
{
   int i;
   float half_scale, small_scale;

   half_scale = scale / 2.0;
   small_scale = half_scale / 1.414;

   glLineWidth(1.0);
   glDisable(GL_LINE_STIPPLE);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++) {
      glVertex2f(point_list->points[i].x - half_scale,
                 point_list->points[i].y);
      glVertex2f(point_list->points[i].x + half_scale,
                 point_list->points[i].y);
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y - half_scale);
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y + half_scale);

      glVertex2f(point_list->points[i].x - small_scale,
                 point_list->points[i].y + small_scale);
      glVertex2f(point_list->points[i].x + small_scale,
                 point_list->points[i].y - small_scale);
      glVertex2f(point_list->points[i].x - small_scale,
                 point_list->points[i].y - small_scale);
      glVertex2f(point_list->points[i].x + small_scale,
                 point_list->points[i].y + small_scale);
   }
   glEnd();
}

/*******************************************************************************
 * phg_draw_marker_cross
 *
 * DESCR:	Draw marker crosses
 * RETURNS:	N/A
 */

static void phg_draw_marker_cross(
   Ppoint_list *point_list,
   Pfloat scale
   )
{
   int i;
   float half_scale;

   half_scale = scale / 2.0;

   glLineWidth(1.0);
   glDisable(GL_LINE_STIPPLE);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++) {
      glVertex2f(point_list->points[i].x - half_scale,
                 point_list->points[i].y + half_scale);
      glVertex2f(point_list->points[i].x + half_scale,
                 point_list->points[i].y - half_scale);
      glVertex2f(point_list->points[i].x - half_scale,
                 point_list->points[i].y - half_scale);
      glVertex2f(point_list->points[i].x + half_scale,
                 point_list->points[i].y + half_scale);
   }
   glEnd();
}

/*******************************************************************************
 * phg_draw_polymarker
 *
 * DESCR:	Draw markers
 * RETURNS:	N/A
 */

static void phg_draw_polymarker(
   Ws *ws,
   Ppoint_list *point_list,
   Pattr_group *attr
   )
{
   phg_set_marker_attr(ws, &attr->marker_bundle);
   switch (attr->marker_bundle.type) {
      case PMARKER_DOT:
         phg_draw_marker_dot(point_list, attr->marker_bundle.size);
      break;

      case PMARKER_PLUS:
         phg_draw_marker_plus(point_list, attr->marker_bundle.size);
      break;

      case PMARKER_ASTERISK:
         phg_draw_marker_asterisk(point_list, attr->marker_bundle.size);
      break;

      case PMARKER_CROSS:
         phg_draw_marker_cross(point_list, attr->marker_bundle.size);
      break;
   }
}

/*******************************************************************************
 * phg_draw_polymarker3
 *
 * DESCR:	Draw markers 3D
 * RETURNS:	N/A
 */

static void phg_draw_polymarker3(
   Ws *ws,
   Ppoint_list3 *point_list,
   Pattr_group *attr
   )
{
   int i;
   Ppoint_list plist;

   if (PHG_SCRATCH_SPACE(&ws->scratch,
                         point_list->num_points * sizeof(Pposted_struct))) {
      plist.num_points = point_list->num_points;
      plist.points = (Ppoint *) ws->scratch.buf;

      for (i = 0; i < point_list->num_points; i++) {
         plist.points[i].x = point_list->points[i].x;
         plist.points[i].y = point_list->points[i].y;
      }

      phg_set_marker_attr(ws, &attr->marker_bundle);
      switch (attr->marker_bundle.type) {
         case PMARKER_DOT:
            phg_draw_marker_dot(&plist, attr->marker_bundle.size);
         break;

         case PMARKER_PLUS:
            phg_draw_marker_plus(&plist, attr->marker_bundle.size);
         break;

         case PMARKER_ASTERISK:
            phg_draw_marker_asterisk(&plist, attr->marker_bundle.size);
         break;

         case PMARKER_CROSS:
            phg_draw_marker_cross(&plist, attr->marker_bundle.size);
         break;
      }
   }
   else {
      ERR_REPORT(ws->erh, ERR900);
   }
}

/*******************************************************************************
 * phg_draw_polyline
 *
 * DESCR:	Draw lines
 * RETURNS:	N/A
 */

static void phg_draw_polyline(
   Ws *ws,
   Ppoint_list *point_list,
   Pattr_group *attr
   )
{
   int i;

   phg_set_line_attr(ws, &attr->line_bundle);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++)
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y);
   glEnd();
}

/*******************************************************************************
 * phg_draw_polyline3
 *
 * DESCR:	Draw lines 3D
 * RETURNS:	N/A
 */

static void phg_draw_polyline3(
   Ws *ws,
   Ppoint_list3 *point_list,
   Pattr_group *attr
   )
{
   int i;

   phg_set_line_attr(ws, &attr->line_bundle);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++)
      glVertex3f(point_list->points[i].x,
                 point_list->points[i].y,
                 point_list->points[i].z);
   glEnd();
}

/*******************************************************************************
 * phg_set_polygon_offset
 *
 * DESCR:	Setup polygon offset
 * RETURNS:	N/A
 */

static void phg_set_polygon_offset(
   float w
   )
{
   glPolygonOffset(w, w);
}

/*******************************************************************************
 * phg_draw_fill_area
 *
 * DESCR:	Draw fill area
 * RETURNS:	N/A
 */

static void phg_draw_fill_area(
   Ws *ws,
   Ppoint_list *point_list,
   Pattr_group *attr
   )
{
   int i;

   if ((attr->int_bundle.style == PSTYLE_SOLID) ||
       (attr->int_bundle.style == PSTYLE_HATCH)) {
      phg_set_int_attr(ws, &attr->int_bundle);
      glBegin(GL_POLYGON);
      for (i = 0; i < point_list->num_points; i++)
         glVertex2f(point_list->points[i].x,
                    point_list->points[i].y);
      glEnd();
   }

   if (attr->edge_bundle.flag == PEDGE_ON) {
      phg_set_edge_attr(ws, &attr->edge_bundle);
      glBegin(GL_LINE_LOOP);
      for (i = 0; i < point_list->num_points; i++)
         glVertex2f(point_list->points[i].x,
                    point_list->points[i].y);
      glEnd();
   }
}

/*******************************************************************************
 * phg_draw_fill_area3
 *
 * DESCR:	Draw fill area 3D
 * RETURNS:	N/A
 */

static void phg_draw_fill_area3(
   Ws *ws,
   Ppoint_list3 *point_list,
   Pattr_group *attr
   )
{
   int i;

   Wsgl_handle wsgl = ws->render_context;

   if (wsgl->hlhsr_mode == PHIGS_HLHSR_MODE_NONE) {
      if ((attr->int_bundle.style == PSTYLE_SOLID) ||
          (attr->int_bundle.style == PSTYLE_HATCH)) {
         phg_set_int_attr(ws, &attr->int_bundle);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++)
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         glEnd();
      }

      if (attr->edge_bundle.flag == PEDGE_ON) {
         phg_set_edge_attr(ws, &attr->edge_bundle);
         glBegin(GL_LINE_LOOP);
         for (i = 0; i < point_list->num_points; i++)
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         glEnd();
      }
   }
   else {
      if ((attr->int_bundle.style == PSTYLE_HOLLOW) &&
          (attr->edge_bundle.flag == PEDGE_ON)) {
         phg_set_polygon_offset(attr->edge_bundle.width);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         glEnable(GL_POLYGON_OFFSET_FILL);
         glColor3f(wsgl->background.val.general.x,
                   wsgl->background.val.general.y,
                   wsgl->background.val.general.z);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++)
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         glEnd();
         glDisable(GL_POLYGON_OFFSET_FILL);

         phg_set_edge_attr(ws, &attr->edge_bundle);
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++)
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         glEnd();
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      }
      else if (((attr->int_bundle.style == PSTYLE_SOLID) ||
                (attr->int_bundle.style == PSTYLE_HATCH)) &&
               (attr->edge_bundle.flag == PEDGE_ON)) {
         phg_set_int_attr(ws, &attr->int_bundle);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         glEnable(GL_POLYGON_OFFSET_FILL);
         phg_set_polygon_offset(attr->edge_bundle.width);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++)
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         glEnd();
         glDisable(GL_POLYGON_OFFSET_FILL);

         phg_set_edge_attr(ws, &attr->edge_bundle);
         glDisable(GL_POLYGON_STIPPLE);
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glBegin(GL_POLYGON);
         for (i = 0; i < point_list->num_points; i++)
            glVertex3f(point_list->points[i].x,
                       point_list->points[i].y,
                       point_list->points[i].z);
         glEnd();
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
   }
}

#ifdef NOT_YET
/*******************************************************************************
 * phg_draw_text
 *
 * DESCR:	Draw text
 * RETURNS:	N/A
 */

static void phg_draw_text(
   Ppoint *pos,
   char *text
   )
{
   printf("Draw text: ");
   printf("%f %f: %s\n", pos->x, pos->y, text);
}
#endif /* NOT_YET */

