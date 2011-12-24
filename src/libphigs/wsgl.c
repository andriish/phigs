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
#include <string.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>
#include <phigs/private/wsglP.h>
#include <phigs/mat_utils.h>

static void phg_update_projection(
   Ws *ws,
   Pint view_index
   );

static void phg_update_modelview(
   Ws *ws,
   Pint view_index
   );

static void phg_set_view(
   Ws *ws,
   Pint view_index
   );

static void phg_set_line_attr(
   Ws *ws,
   Pline_bundle *attr
   );

static void phg_set_int_attr(
   Ws *ws,
   Pint_bundle *attr
   );

static void phg_set_edge_attr(Ws *ws,
   Pedge_bundle *attr
   );

static void phg_set_marker_attr(
   Ws *ws,
   Pmarker_bundle *attr
   );

static void phg_draw_polymarker(
   Ws *ws,
   Ppoint_list *point_list,
   attribute_group *attr
   );

static void phg_draw_polymarker3(
   Ws *ws,
   Ppoint_list3 *point_list,
   attribute_group *attr
   );

static void phg_draw_polyline(
   Ws *ws,
   Ppoint_list *point_list,
   attribute_group *attr
   );

static void phg_draw_polyline3(
   Ws *ws,
   Ppoint_list3 *point_list,
   attribute_group *attr
   );

static void phg_draw_fill_area(
   Ws *ws,
   Ppoint_list *point_list,
   attribute_group *attr
   );

static void phg_draw_fill_area3(
   Ws *ws,
   Ppoint_list3 *point_list,
   attribute_group *attr
   );

#ifdef NOT_YET
static void phg_draw_text(
   Ppoint *pos,
   char *text
   );
#endif /*NOT_YET*/

/*******************************************************************************
 * wsgl_create
 *
 * DESCR:	Create renderer for workstation
 * RETURNS:	Pointer to workstation or NULL
 */

Ws* wsgl_create(
   Phg_args_open_ws *args
   )
{
   Ws_handle wsh;

   wsh = malloc(sizeof(Ws));
   if (wsh == NULL)
      return NULL;

   memset(wsh, 0, sizeof(Ws));
   if (!wsgl_init(wsh))
      return NULL;

   return wsh;
}

/*******************************************************************************
 * wsgl_init
 *
 * DESCR:	Initialize renderer
 * RETURNS:	Non zero or zero on error
 */

int wsgl_init(
   Ws *ws
   )
{
   Wsgl *wsgl;

   wsgl = (Wsgl *) malloc(sizeof(Wsgl));
   if (wsgl == NULL)
      return 0;

   memset(wsgl, 0, sizeof(Wsgl));

   ws->render_context = wsgl;

   return 1;
}

/*******************************************************************************
 * wsgl_open_window
 *
 * DESCR:       Open render window for workstation
 * RETURNS:     Zero on succcess, non zero on error
 */

int wsgl_open_window(
   Ws *ws,
   Phg_args_open_ws *args
   )
{
   int ret;
   Wsgl *wsgl = (Wsgl *) ws->render_context;

   wsgl->type = args->type;

   if (args->type->base_type == WST_BASE_TYPE_GLX_DRAWABLE) {
#ifdef DEBUG
      printf("Open GLX drawable workstation\n");
#endif
      ret = wsx_gl_open_window(ws, args);
   }
   else {
      ret = 0;
   }

   return ret;
}

/*******************************************************************************
 * wsgl_release_window
 *
 * DESCR:       Close render window for workstation
 * RETURNS:     N/A
 */

int wsgl_release_window(
   Ws *ws
   )
{
   int ret;
   Wsgl *wsgl = (Wsgl *) ws->render_context;

   if (wsgl->type->base_type == WST_BASE_TYPE_GLX_DRAWABLE) {
#ifdef DEBUG
      printf("Close GLX drawable workstation\n");
#endif
      wsx_gl_release_window(ws);
      ret = 1;
   }
   else {
      ret = 0;
   }

   return ret;
}

/*******************************************************************************
 * wsgl_destroy
 *
 * DESCR:	Release workstation
 * RETURNS:	N/A
 */

void wsgl_destroy(
   Ws *ws
   )
{
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
   Wsgl *wsgl = (Wsgl *) ws->render_context;

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
   Wsgl *wsgl = (Wsgl *) ws->render_context;

   wsgl->vp_changed = 1;
   memcpy(&wsgl->curr_vp, vp, sizeof(Plimit3));
}

/*******************************************************************************
 * wsgl_clear
 *
 * DESCR:	Clear render window
 * RETURNS:	N/A
 */

void wsgl_clear(
   void
   )
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
   Wsgl *wsgl = (Wsgl *) ws->render_context;

   glXMakeCurrent(ws->display, ws->drawable_id, wsgl->glx_context);

   if (wsgl->vp_changed || wsgl->win_changed) {
      wsgl_compute_ws_transform(&wsgl->curr_win, &wsgl->curr_vp, &ws_xform);
      glViewport((GLint)   ws_xform.offset.x,
                 (GLint)   ws_xform.offset.y,
                 (GLsizei) ws_xform.scale.x,
                 (GLsizei) ws_xform.scale.y);

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

   }

   glEnable(GL_DEPTH_TEST);
   glDepthRange(1, 0);
   glDepthFunc(GL_LESS);

   if (ws->has_double_buffer)
   {
      glXSwapBuffers(ws->display, ws->drawable_id);
   }
   else
   {
      glFlush();
   }
}

/*******************************************************************************
 * wsgl_compute_ws_transform
 *
 * DESCR:	Compute workstation transform
 * RETURNS:	N/A
 */

void wsgl_compute_ws_transform(
   Plimit3 *ws_win,
   Plimit3 *ws_vp,
   Ws_xform *ws_xform
   )
{
   Pfloat sx, sy, sz, sxy;

   sx = (ws_vp->x_max - ws_vp->x_min) / (ws_win->x_max - ws_win->x_min);
   sy = (ws_vp->y_max - ws_vp->y_min) / (ws_win->y_max - ws_win->y_min);
   sz = (ws_vp->z_max - ws_vp->z_min) / (ws_win->z_max - ws_win->z_min);

   sxy = (sx < sy) ? sx : sy;

   ws_xform->scale.x = ws_xform->scale.y = sxy;
   ws_xform->scale.z = sz;

   ws_xform->offset.x = ws_vp->x_min - (ws_win->x_min * sxy);
   ws_xform->offset.y = ws_vp->y_min - (ws_win->y_min * sxy);
   ws_xform->offset.z = ws_vp->z_min - (ws_win->z_min * sz);

#ifdef DEBUG
   printf("Ws transform:\n"
          "%f %f %f\n"
          "%f %f %f\n",
          ws_xform->scale.x,
          ws_xform->scale.y,
          ws_xform->scale.z,
          ws_xform->offset.x,
          ws_xform->offset.y,
          ws_xform->offset.z);
#endif
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
   Wsgl *wsgl = (Wsgl *) ws->render_context;

#ifdef DEBUG
   printf("Begin rendering\n");
#endif

   memcpy(&wsgl->attrs, phg_get_default_attr(), sizeof(attribute_group));
   phg_mat_identity(wsgl->local_tran);
   phg_set_view(ws, 0);
}

/*******************************************************************************
 * wsgl_end_rendering
 *
 * DESCR:	End a rendiering session
 * RETURNS:	N/A
 */

void wsgl_end_rendering(
   void
   )
{
#ifdef DEBUG
   printf("End rendering\n");
#endif

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
   Wsgl *wsgl = (Wsgl *) ws->render_context;

   switch (el->eltype) {
      case PELEM_LABEL:
      case PELEM_PICK_ID:
      break;

      case PELEM_INT_COLR_IND:
         wsgl->attrs.int_bundle.colr_ind = PHG_INT(el);
      break;
      case PELEM_INT_STYLE:
         wsgl->attrs.int_bundle.style = PHG_INT(el);
      break;

      case PELEM_EDGE_COLR_IND:
         wsgl->attrs.edge_bundle.colr_ind = PHG_INT(el);
      break;
      case PELEM_EDGEWIDTH:
         wsgl->attrs.edge_bundle.width = PHG_FLOAT(el);
      break;
      case PELEM_EDGETYPE:
         wsgl->attrs.edge_bundle.type = PHG_INT(el);
      break;
      case PELEM_EDGE_FLAG:
         wsgl->attrs.edge_bundle.flag = PHG_EDGE_FLAG(el);
      break;

      case PELEM_MARKER_COLR_IND:
         wsgl->attrs.marker_bundle.colr_ind = PHG_INT(el);
      break;
      case PELEM_MARKER_SIZE:
         wsgl->attrs.marker_bundle.size = PHG_FLOAT(el);
      break;
      case PELEM_MARKER_TYPE:
         wsgl->attrs.marker_bundle.type = PHG_INT(el);
      break;

      case PELEM_TEXT_COLR_IND:
         wsgl->attrs.text_bundle.colr_ind = PHG_INT(el);
      break;
      case PELEM_TEXT_FONT:
         wsgl->attrs.text_bundle.font = PHG_INT(el);
      break;

      case PELEM_LINE_COLR_IND:
         wsgl->attrs.line_bundle.colr_ind = PHG_INT(el);
      break;
      case PELEM_LINEWIDTH:
         wsgl->attrs.line_bundle.width = PHG_FLOAT(el);
      break;
      case PELEM_LINETYPE:
         wsgl->attrs.line_bundle.type = PHG_INT(el);
      break;

      case PELEM_FILL_AREA:
         phg_draw_fill_area(ws,
                            PHG_POINT_LIST(el),
                            &wsgl->attrs);
      break;
      case PELEM_POLYLINE:
         phg_draw_polyline(ws,
                           PHG_POINT_LIST(el),
                           &wsgl->attrs
                           );
      break;
      case PELEM_POLYMARKER:
         phg_draw_polymarker(ws,
                             PHG_POINT_LIST(el),
                             &wsgl->attrs);
      break;

      case PELEM_FILL_AREA3:
         phg_draw_fill_area3(ws,
                             PHG_POINT_LIST3(el),
                             &wsgl->attrs);
      break;
      case PELEM_POLYLINE3:
         phg_draw_polyline3(ws,
                            PHG_POINT_LIST3(el),
                            &wsgl->attrs);
      break;
      case PELEM_POLYMARKER3:
         phg_draw_polymarker3(ws,
                              PHG_POINT_LIST3(el),
                              &wsgl->attrs);
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
         phg_update_modelview(ws,
                              wsgl->curr_view_index);
      break;

      case PELEM_VIEW_IND:
         phg_set_view(ws, PHG_INT(el));
      break;

      default:
         css_print_eltype(el->eltype);
         printf(" not processed\n");
      break;
   }
}

/*******************************************************************************
 * phg_set_matrix
 *
 * DESCR:	Setup matrix
 * RETURNS:	N/A
 */

static void phg_set_matrix(
    Pmatrix3 mat
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

   glLoadMatrixf(m);
}

/*******************************************************************************
 * phg_update_projection
 *
 * DESCR:	Update projection matrix
 * RETURNS:	N/A
 */

static void phg_update_projection(
   Ws *ws,
   Pint view_index
   )
{
   Ws_view_entry *view_entry = &ws->out_ws.model.b.views[view_index];

#ifdef DEBUG
   printf("Update projection\n");
#endif

   glMatrixMode(GL_PROJECTION);
   phg_set_matrix(view_entry->vmm);
}

/*******************************************************************************
 * phg_update_modelview
 *
 * DESCR:	Update modelview matrix
 * RETURNS:	N/A
 */

static void phg_update_modelview(
   Ws *ws,
   Pint view_index
   )
{
   Wsgl *wsgl = (Wsgl *) ws->render_context;
   Ws_view_entry *view_entry = &ws->out_ws.model.b.views[view_index];

#ifdef DEBUG
   printf("Update modelview\n");
#endif

   glMatrixMode(GL_MODELVIEW);
   phg_mat_mul(wsgl->total_tran, view_entry->vom, wsgl->local_tran);
   phg_set_matrix(wsgl->total_tran);
}

/*******************************************************************************
 * phg_set_view
 *
 * DESCR:	Setup view
 * RETURNS:	N/A
 */

static void phg_set_view(
   Ws *ws,
   Pint view_index
   )
{
   Wsgl *wsgl = (Wsgl *) ws->render_context;

#ifdef DEBUG
   printf("Set view: %d\n", view_index);
#endif

   wsgl->curr_view_index = view_index;
   phg_update_projection(ws, view_index);
   phg_update_modelview(ws, view_index);
}

/*******************************************************************************
 * phg_set_line_attr
 *
 * DESCR:	Setup line attributes
 * RETURNS:	N/A
 */

static void phg_set_line_attr(
   Ws *ws,
   Pline_bundle *attr
   )
{
   Pint index = attr->colr_ind;

   glColor3f(ws->colr_table[index].val.general.x,
             ws->colr_table[index].val.general.y,
             ws->colr_table[index].val.general.z);

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
 * phg_set_int_attr
 *
 * DESCR:	Setup interiour attributes
 * RETURNS:	N/A
 */

static void phg_set_int_attr(
   Ws *ws,
   Pint_bundle *attr
   )
{
   Pint index = attr->colr_ind;

   glColor3f(ws->colr_table[index].val.general.x,
             ws->colr_table[index].val.general.y,
             ws->colr_table[index].val.general.z);
}

/*******************************************************************************
 * phg_set_edge_attr
 *
 * DESCR:	Setup interiour attributes
 * RETURNS:	N/A
 */

static void phg_set_edge_attr(
   Ws *ws,
   Pedge_bundle *attr
   )
{
   Pint index = attr->colr_ind;

   glColor3f(ws->colr_table[index].val.general.x,
             ws->colr_table[index].val.general.y,
             ws->colr_table[index].val.general.z);
   glLineWidth(attr->width);
}

/*******************************************************************************
 * phg_set_marker_attr
 *
 * DESCR:	Setup marker attributes
 * RETURNS:	N/A
 */

static void phg_set_marker_attr(
   Ws *ws,
   Pmarker_bundle *attr
   )
{
   Pint index = attr->colr_ind;

   glColor3f(ws->colr_table[index].val.general.x,
             ws->colr_table[index].val.general.y,
             ws->colr_table[index].val.general.z);
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
   attribute_group *attr
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
   attribute_group *attr
   )
{
   int i;
   Ppoint_list plist;
   Ppoint pts[10];

   plist.num_points = point_list->num_points;
   plist.points = pts;
   for (i = 0; i < point_list->num_points; i++) {
      pts[i].x = point_list->points[i].x;
      pts[i].y = point_list->points[i].y;
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

/*******************************************************************************
 * phg_draw_polyline
 *
 * DESCR:	Draw lines
 * RETURNS:	N/A
 */

static void phg_draw_polyline(
   Ws *ws,
   Ppoint_list *point_list,
   attribute_group *attr
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
   attribute_group *attr
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
 * phg_draw_fill_area
 *
 * DESCR:	Draw fill area
 * RETURNS:	N/A
 */

static void phg_draw_fill_area(
   Ws *ws,
   Ppoint_list *point_list,
   attribute_group *attr
   )
{
   int i;

   if (attr->int_bundle.style == PSTYLE_SOLID) {
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
   attribute_group *attr
   )
{
   int i;

   if (attr->int_bundle.style == PSTYLE_SOLID) {
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

