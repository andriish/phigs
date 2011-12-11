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

#ifndef _ws_h
#define _ws_h

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <phigs/phg.h>

#define NUM_DEFERRAL            5
#define NUM_MODIFICATION        3
#define WS_MIN_PREDEF_VIEW_REPS 6

typedef enum {
   PHG_TIME_NOW,
   PHG_TIME_BIG,
   PHG_TIME_BIL,
   PHG_TIME_ATI,
   PHG_TIME_NUM
} Ws_update_time;

typedef enum {
   PHG_UPDATE_ACCURATE,
   PHG_UPDATE_UWOR,
   PHG_UPDATE_UQUM,
   PHG_UPDATE_NOTHING,
   PHG_UPDATE_IF_IG,
   PHG_UPDATE_IF_IL,
   PHG_UPDATE_IF_INCORRECT,
   ASSURE_CORRECT,
   PHG_UPDATE_NUM
} Ws_update_action;

typedef enum {
   WS_INV_NOT_CURRENT,
   WS_INV_CURRENT,
   WS_INV_NOT_INVERTIBLE
} Ws_inverse_state;

typedef enum {
   WS_PRE_CSS_DELETE,
   WS_POST_CSS_DELETE
} Ws_delete_flag;

typedef struct {
   Pupd_st          pending;
   Pmatrix3         vom;
   Pmatrix3         vmm;
   Plimit3          clip_limit;
   Pclip_ind        xy_clip;
   Pclip_ind        back_clip;
   Pclip_ind        front_clip;
   Ws_inverse_state npc_to_wc_state;
   Pmatrix3         npc_to_wc;
} Ws_view_entry;

typedef struct {
   Pint higher;
   Pint lower;
} Ws_view_priority;

typedef struct {
   Pint       id;
   Pview_rep3 view;
} Ws_pending_view;

typedef struct _Ws_post_str {
   Pfloat              disp_pri;
   Struct_handle       structh;
   struct _Ws_post_str *lower;
   struct _Ws_post_str *higher;
} Ws_post_str;

typedef struct {
   Ws_post_str lowest;
   Ws_post_str highest;
} Ws_posted_structs;

typedef Ws_update_action
   Ws_action_table[PHG_TIME_NUM][NUM_MODIFICATION][NUM_DEFERRAL];

typedef Ws_action_table *Ws_action_table_ptr;

typedef struct {
   Ppoint3 scale;
   Ppoint3 offset;
} Ws_xform;

typedef struct {
   int unused;
} Wsa_output_ws;

typedef struct {
   Css_handle          cssh;

   Pvisual_st          vis_rep;
   Pdisp_surf_empty    surf_state;

   /* Window */
   Plimit3             ws_window;
   Plimit3             ws_viewport;
   Pupd_st             ws_window_pending;
   Pupd_st             ws_viewport_pending;
   Plimit3             req_ws_window;
   Plimit3             req_ws_viewport;
   Ws_xform            ws_xform;

   /* Views */
   Pint                num_pending_views;
   Ws_pending_view     *pending_views;
   Pint                num_views;
   Ws_view_entry       *views;
   Pint                top_view;
   Ws_view_priority    *view_priorities;

   Ws_posted_structs   posted;

   Ws_update_action    now_action;
   Ws_action_table_ptr update_action_table;

   /* Render modes */
   Pupd_st             hlhsr_mode_pending;
   Pint                req_hlhsr_mode;
   Pint                cur_hlhsr_mode;
} Wsb_output_ws;

typedef struct {
   Pdefer_mode      def_mode;
   Pmod_mode        mod_mode;
   union {
      Wsa_output_ws a;
      Wsb_output_ws b;
   } model;
} Ws_output_ws;

typedef struct _Ws {
   Pint         id;
   Ws_output_ws out_ws;
   Pint         current_colour_model;
   Pint         num_active_input_devs;

   Err_handle   erh;
   Phg_scratch  scratch;

   /* Window system variables */
   Display      *display;
   Drawable     drawable_id;
   GLXContext   glx_context;
   int          has_double_buffer;
   XRectangle   ws_rect;
   Pgcolr       colr_table[256];

   int          black_color;
   int          white_color;

   void         (*close)(
                   struct _Ws *ws
                   );
   void         (*redraw_all)(
                   struct _Ws *ws,
                   Pctrl_flag clear_control
                   );
   void         (*conditional_redraw)(
                   struct _Ws *ws
                   );
   void         (*repaint_all)(
                   struct _Ws *ws,
                   Pctrl_flag clear_control
                   );
   void         (*make_requested_current)(
                   struct _Ws *ws
                   );
   void         (*update)(
                   struct _Ws *ws,
                   Pregen_flag flag
                   );
   void         (*set_disp_update_state)(
                   struct _Ws *ws,
                   Pdefer_mode def_mode,
                   Pmod_mode mod_mode
                   );
   void         (*set_hlhsr_mode)(
                   struct _Ws *ws,
                   Pint mode
                   );
   void         (*set_view_input_priority)(
                   struct _Ws *ws,
                   Pint index,
                   Pint ref_index,
                   Prel_pri priority
                   );
   void         (*set_ws_window)(
                   struct _Ws *ws,
                   Pint two_d,
                   Plimit3 *limits
                   );
   void         (*set_ws_vp)(
                   struct _Ws *ws,
                   Pint two_d,
                   Plimit3 *limits
                   );
   void         (*add_el)(
                   struct _Ws *ws
                   );
   void         (*copy_struct)(
                   struct _Ws *ws,
                   El_handle first_el
                   );
   void         (*close_struct)(
                   struct _Ws *ws,
                   Struct_handle structh
                   );
   void         (*move_ep)(
                   struct _Ws *ws,
                   El_handle ep
                );
   int          (*delete_el)(
                   struct _Ws *ws,
                   Struct_handle structh,
                   El_handle elh1,
                   El_handle elh2,
                   Ws_delete_flag flag
                   );
   int          (*delete_struct)(
                   struct _Ws *ws,
                   Struct_handle structh,
                   Ws_delete_flag flag
                   );
   int          (*delete_struct_net)(
                   struct _Ws *ws,
                   Struct_handle structh,
                   Pref_flag reff,
                   Ws_delete_flag flag
                   );
   void         (*delete_all_structs) (
                   struct _Ws *ws
                   );
   void         (*post)(
                   struct _Ws *ws,
                   Struct_handle structh,
                   Pfloat priority,
                   int first_posting
                   );
   void         (*unpost)(
                    struct _Ws *ws,
                    Struct_handle structh
                    );
   void         (*unpost_all)(
                    struct _Ws *ws
                    );
   void         (*change_posting)(
                   struct _Ws *ws,
                   Struct_handle unpost,
                   Struct_handle post
                   );
   void         (*inq_view_indices)(
                   struct _Ws *ws,
                   Phg_ret *ret
                   );
   void         (*inq_posted)(
                   struct _Ws *ws,
                   Phg_ret *ret
                   );
   void         (*inq_view_rep)(
                   struct _Ws *ws,
                   Pint index,
                   Phg_ret *ret
                   );
   void         (*inq_ws_xform)(
                   struct _Ws *ws,
                   Phg_ret *ret
                   );
   void         (*inq_disp_update_state)(
                   struct _Ws *ws,
                   Phg_ret *ret
                   );
   void         (*inq_hlhsr_mode)(
                   struct _Ws *ws,
                   Phg_ret *ret
                   );
   void         (*set_rep)(
                   struct _Ws *ws,
                   Phg_args_rep_type type,
                   Phg_args_rep_data *rep);
} Ws;

#define WSB_NONE_POSTED(posted_ptr) \
   ((posted_ptr)->lowest.higher == &(posted_ptr)->highest)

#define WSB_SOME_POSTED(posted_ptr) \
   ((posted_ptr)->lowest.higher != &(posted_ptr)->highest)

#define WSB_CHECK_POSTED(posted_ptr) \
        assert(((posted_ptr)->lowest.higher == &(posted_ptr)->highest) \
                == ((posted_ptr)->highest.lower == &(posted_ptr)->lowest))

#define case_PHG_UPDATE_ACCURATE_or_IF_Ix	\
   case PHG_UPDATE_ACCURATE:			\
   case PHG_UPDATE_IF_IG:			\
   case PHG_UPDATE_IF_IL

#define WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, now_action_ptr)	\
{								\
   if ( *(now_action_ptr) == PHG_UPDATE_IF_IG ||		\
        *(now_action_ptr) == PHG_UPDATE_IF_IL)			\
      phg_wsb_resolve_now_action(ws, now_action_ptr);		\
}

#define WS_SET_WS_RECT(_wsh, _wattr)		\
{						\
   (_wsh)->ws_rect.x = (_wattr)->x;		\
   (_wsh)->ws_rect.y = (_wattr)->y;		\
   (_wsh)->ws_rect.width = (_wattr)->width;	\
   (_wsh)->ws_rect.height = (_wattr)->height;	\
}

#define WS_ANY_INP_DEV_ACTIVE(_wsh) \
   ((_wsh)->num_active_input_devs > 0)

extern Ws_handle  *ws_list;

void popen_ws(Pint ws_id, void *conn_id, Pint ws_type);
void ppost_struct(Pint ws_id, Pint struct_id, Pfloat priority);
Ws* phg_wsb_open_ws(Phg_args_open_ws *args, Phg_ret *ret);
void wsb_free_all_posted(Wsb_output_ws *owsb);
void wsb_destroy_ws(Ws *ws);
void phg_wsb_close_ws(Ws *ws);
void phg_wsb_redraw_all(Ws *ws, Pctrl_flag clear_control);
void phg_wsb_make_requested_current(Ws *ws);
void phg_wsb_repaint_all(Ws *ws, Pctrl_flag clear_control);
void phg_wsb_traverse_all_postings(Ws *ws);
void phg_wsb_traverse_net(Ws_handle ws, Struct_handle structp);
void phg_wsb_add_el(Ws *ws);
int phg_wsb_asti_update(Ws *ws, Pctrl_flag clear_control);
void phg_wsb_close_struct(Ws *ws, Struct_handle structh);
void phg_wsb_post(Ws *ws,
                  Struct_handle structh,
                  Pfloat priority,
                  Pint first_posting);
void phg_wsb_change_posting(Ws *ws,
                            Struct_handle unpost,
                            Struct_handle post);
void phg_wsb_unpost(Ws *ws, Struct_handle structh);
void phg_wsb_unpost_all(Ws *ws);
void phg_wsb_delete_all_structs(Ws *ws);
int phg_wsb_delete_struct(Ws *ws,
                          Struct_handle structh,
                          Ws_delete_flag flag);
int phg_wsb_delete_struct_net(Ws *ws,
                              Struct_handle structh,
                              Pref_flag reff,
                              Ws_delete_flag flag);
void phg_wsb_copy_struct(Ws *ws, El_handle first_el);
int phg_wsb_delete_el(Ws *ws,
                      Struct_handle structh,
                      El_handle elh1,
                      El_handle elh2,
                      Ws_delete_flag flag);
void phg_wsb_conditional_redraw(Ws *ws);
void phg_wsb_resolve_now_action(Ws *ws,
                                Ws_update_action *now_action_ptr);
void phg_wsb_update(Ws *ws, Pregen_flag flag);
void phg_wsb_set_disp_update_state(Ws *ws,
                                   Pdefer_mode def_mode,
                                   Pmod_mode mod_mode);
void phg_wsb_set_hlhsr_mode(Ws *ws, Pint mode);
void phg_wsb_set_ws_window(Ws *ws,
                           Pint two_d,
                           Plimit3 *limits);
void phg_wsb_set_ws_vp(Ws *ws,
                       Pint two_d,
                       Plimit3 *limits);
void phg_wsb_set_view_input_priority(Ws *ws,
                                     Pint index,
                                     Pint ref_index,
                                     Prel_pri priority);
void phg_wsb_inq_view_indices(Ws *ws, Phg_ret *ret);
void phg_wsb_inq_posted(Ws *ws, Phg_ret *ret);
void phg_wsb_inq_view_rep(Ws *ws, Pint index, Phg_ret *ret);
void phg_wsb_inq_ws_xform(Ws *ws, Phg_ret *ret);
void phg_wsb_inq_disp_update_state(Ws *ws, Phg_ret *ret);
void phg_wsb_inq_hlhsr_mode(Ws *ws, Phg_ret *ret);
void phg_wsb_set_rep(Ws *ws, Phg_args_rep_type type, Phg_args_rep_data *rep);

#endif

