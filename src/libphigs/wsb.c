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
*******************************************************************************

Copyright (c) 1989, 1990, 1991  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

Copyright 1989, 1990, 1991 by Sun Microsystems, Inc. 

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of Sun Microsystems,
not be used in advertising or publicity pertaining to distribution of 
the software without specific, written prior permission.  

SUN MICROSYSTEMS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT 
SHALL SUN MICROSYSTEMS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL 
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/* PEX/PHIGS workstation utility functions for the B model (client side
 * workstations and structure storage).
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>
#include <phigs/private/wsglP.h>
#include <phigs/css.h>
#include <phigs/alloc.h>

static void wsb_load_funcs(
    Ws *ws
    )
{
    ws->close = phg_wsb_close_ws;
    ws->redraw_all = phg_wsb_redraw_all;
    ws->conditional_redraw = phg_wsb_conditional_redraw;
    ws->repaint_all = phg_wsb_repaint_all;
    ws->make_requested_current = phg_wsb_make_requested_current;
    ws->update = phg_wsb_update;
    ws->set_disp_update_state = phg_wsb_set_disp_update_state;
    ws->set_rep = phg_wsb_set_rep;
#if TODO
    ws->set_filter = phg_wsb_set_filter;
#endif
    ws->set_hlhsr_mode = phg_wsb_set_hlhsr_mode;
    ws->set_view_input_priority = phg_wsb_set_view_input_priority;
    ws->set_ws_window = phg_wsb_set_ws_window;
    ws->set_ws_vp = phg_wsb_set_ws_vp;
    ws->add_el = phg_wsb_add_el;
    ws->copy_struct = phg_wsb_copy_struct;
    ws->close_struct = phg_wsb_close_struct;
    ws->move_ep = NULL;
    ws->delete_el = phg_wsb_delete_el;
    ws->delete_struct = phg_wsb_delete_struct;
    ws->delete_struct_net = phg_wsb_delete_struct_net;
    ws->delete_all_structs = phg_wsb_delete_all_structs;
    ws->post = phg_wsb_post;
    ws->unpost = phg_wsb_unpost;
    ws->unpost_all = phg_wsb_unpost_all;
    ws->change_posting = phg_wsb_change_posting;
    ws->inq_view_indices = phg_wsb_inq_view_indices;
    ws->inq_posted = phg_wsb_inq_posted;
    ws->inq_representation = phg_wsb_inq_rep;
    ws->inq_view_rep = phg_wsb_inq_view_rep;
    ws->inq_ws_xform = phg_wsb_inq_ws_xform;
    ws->inq_disp_update_state = phg_wsb_inq_disp_update_state;
    ws->inq_hlhsr_mode = phg_wsb_inq_hlhsr_mode;
#if TODO
    ws->inq_bundle_indices = phg_wsx_inq_LUT_indices;
    ws->inq_filter = phg_wsb_inq_filter;
    ws->drawable_pick = phg_wsb_drawable_pick;
    ws->map_points = phg_wsb_map_points;
    ws->redraw_regions = phg_wsb_redraw_regions;
#endif
}

/* 
 * Tables that determine what update action is valid at a give point
 * in time.  The table has 3 axes:
 *	[Time] [Modification Mode] [Deferral Mode].
 */

static Ws_action_table default_action_table =
{
    {   /* PHG_TIME_NOW */  
	{   /* NIVE */   
	    PHG_UPDATE_ACCURATE, 		/* ASAP */
	    PHG_UPDATE_IF_IG,	 		/* BNIG */
	    PHG_UPDATE_IF_IL, 			/* BNIL */
	    PHG_UPDATE_NOTHING, 		/* ASTI */
	    PHG_UPDATE_NOTHING			/* WAIT */
	},
	{   /* UWOR */
	    PHG_UPDATE_ACCURATE, 		/* ASAP */
	    PHG_UPDATE_IF_IG, 			/* BNIG */
	    PHG_UPDATE_IF_IL, 			/* BNIL */
	    PHG_UPDATE_UWOR, 			/* ASTI */
	    PHG_UPDATE_UWOR			/* WAIT */
	},
	{   /* UQUM */
	    PHG_UPDATE_ACCURATE, 		/* ASAP */
	    PHG_UPDATE_IF_IG, 			/* BNIG */
	    PHG_UPDATE_IF_IL, 			/* BNIL */
	    PHG_UPDATE_UQUM, 			/* ASTI */
	    PHG_UPDATE_UQUM			/* WAIT */
	}, 
    }, 

    {   /* PHG_TIME_BIG */  
	{   /* NIVE */   
	    ASSURE_CORRECT, 			/* ASAP */
	    PHG_UPDATE_ACCURATE, 		/* BNIG */
	    PHG_UPDATE_NOTHING, 		/* BNIL */
	    PHG_UPDATE_NOTHING, 		/* ASTI */
	    PHG_UPDATE_NOTHING			/* WAIT */
	}, 
	{   /* UWOR */ 
	    ASSURE_CORRECT, 			/* ASAP */
	    PHG_UPDATE_ACCURATE, 		/* BNIG */
	    PHG_UPDATE_NOTHING, 		/* BNIL */
	    PHG_UPDATE_NOTHING, 		/* ASTI */
	    PHG_UPDATE_NOTHING			/* WAIT */
	}, 
	{   /* UQUM */   
	    ASSURE_CORRECT, 			/* ASAP */
	    PHG_UPDATE_ACCURATE, 		/* BNIG */
	    PHG_UPDATE_NOTHING, 		/* BNIL */
	    PHG_UPDATE_NOTHING, 		/* ASTI */
	    PHG_UPDATE_NOTHING			/* WAIT */
	}, 
    }, 	    
    {   /* PHG_TIME_BIL */  
	{   /* NIVE */   
	    ASSURE_CORRECT, 			/* ASAP */
	    PHG_UPDATE_ACCURATE, 		/* BNIG */
	    PHG_UPDATE_ACCURATE, 		/* BNIL */
	    PHG_UPDATE_NOTHING, 		/* ASTI */
	    PHG_UPDATE_NOTHING			/* WAIT */
	}, 
	{   /* UWOR */ 
	    ASSURE_CORRECT, 			/* ASAP */
	    PHG_UPDATE_ACCURATE, 		/* BNIG */
	    PHG_UPDATE_ACCURATE, 		/* BNIL */
	    PHG_UPDATE_NOTHING, 		/* ASTI */
	    PHG_UPDATE_NOTHING			/* WAIT */
	}, 
	{   /* UQUM */   
	    ASSURE_CORRECT, 			/* ASAP */
	    PHG_UPDATE_ACCURATE, 		/* BNIG */
	    PHG_UPDATE_ACCURATE, 		/* BNIL */
	    PHG_UPDATE_NOTHING, 		/* ASTI */
	    PHG_UPDATE_NOTHING			/* WAIT */
	}, 
    }, 
    {   /* PHG_TIME_ATI */  
	{   /* NIVE */   
	    ASSURE_CORRECT, 			/* ASAP */
	    PHG_UPDATE_IF_INCORRECT, 		/* BNIG */
	    PHG_UPDATE_IF_INCORRECT, 		/* BNIL */
	    PHG_UPDATE_ACCURATE, 		/* ASTI */
	    PHG_UPDATE_NOTHING			/* WAIT */
	}, 
	{   /* UWOR */ 
	    ASSURE_CORRECT, 			/* ASAP */
	    PHG_UPDATE_IF_INCORRECT, 		/* BNIG */
	    PHG_UPDATE_IF_INCORRECT, 		/* BNIL */
	    PHG_UPDATE_ACCURATE, 		/* ASTI */
	    PHG_UPDATE_NOTHING			/* WAIT */
	},
	{   /* UQUM */   
	    ASSURE_CORRECT, 			/* ASAP */
	    PHG_UPDATE_IF_INCORRECT, 		/* BNIG */
	    PHG_UPDATE_IF_INCORRECT, 		/* BNIL */
	    PHG_UPDATE_ACCURATE, 		/* ASTI */
	    PHG_UPDATE_NOTHING			/* WAIT */
	},
    }
};

static void init_update_state(
    Ws *ws
    )
{   
    Ws_output_ws	*ows = &ws->out_ws;
    Wsb_output_ws	*owsb = &ows->model.b;

    ows->def_mode = PDEFER_ASAP;
    ows->mod_mode = PMODE_NIVE;

    owsb->update_action_table = (Ws_action_table_ptr)default_action_table;

    /* cache action for time "NOW" */
    owsb->now_action = (*owsb->update_action_table)
	[(int)PHG_TIME_NOW][(int)ows->mod_mode][(int)ows->def_mode];

    owsb->vis_rep = PVISUAL_ST_CORRECT;
    owsb->surf_state = PSURF_EMPTY;
}

static int init_view_table(
    Ws *ws
    )
{   
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    Ws_view_entry	*view;
    Pview_rep3		*predef_view;
    int			i, max_pd_view;

    ALLOC_DECLARE(5);

    owsb->num_views = WS_MIN_PREDEF_VIEW_REPS;

    if ( !ALLOCATED( owsb->views = (Ws_view_entry *)
	    malloc((unsigned)(owsb->num_views * sizeof(Ws_view_entry)))) ) {
	ERR_BUF( ws->erh, ERR900 );
	return 0;
    }

    owsb->top_view = 0;
    if ( !ALLOCATED( owsb->view_priorities = (Ws_view_priority *)
	    malloc((unsigned)(owsb->num_views * sizeof(Ws_view_priority)))) ) {
	ERR_BUF( ws->erh, ERR900 );
	ALLOC_FREE;
	return 0;
    }

    owsb->num_pending_views = 0;
    if ( !ALLOCATED( owsb->pending_views = (Ws_pending_view *)
	    malloc((unsigned)(owsb->num_views * sizeof(Ws_pending_view)))) ) {
	ERR_BUF( ws->erh, ERR900 );
	ALLOC_FREE;
	return 0;
    }

    max_pd_view = WS_MIN_PREDEF_VIEW_REPS - 1;
    predef_view = default_views;
    view = owsb->views;
    /* Load the predefined views. */
    for ( i = 0; i <= max_pd_view; i++, predef_view++, view++ ) {
#ifdef DEBUG
        printf("wsb: Set default view: %d\n", i);
#endif
        view->pending = PUPD_NOT_PEND;
        memcpy(view->vom,
               predef_view->ori_matrix,
               sizeof(Pmatrix3));
        memcpy(view->vmm,
               predef_view->map_matrix,
               sizeof(Pmatrix3));
        view->clip_limit = predef_view->clip_limit;
        view->xy_clip = predef_view->xy_clip;
        view->back_clip = predef_view->back_clip;
        view->front_clip = predef_view->front_clip;
        /* view->npc_to_wc not computed until needed in input code. */
        view->npc_to_wc_state = WS_INV_NOT_CURRENT;
    }

    /* Load the available but not predefined views. */
    for ( /* use existing index */ ; i < owsb->num_views; i++, view++ )
        *view = owsb->views[0];

    /* Initialize the view transformation priorities.  The list is
     * terminated at top and bottom by -1.
     */
    for ( i = 0; i < owsb->num_views; i++ ) {
	owsb->view_priorities[i].higher = i - 1;
	owsb->view_priorities[i].lower = i + 1;
    }
    owsb->view_priorities[owsb->num_views - 1].lower = -1;
    
    return 1;
}

static int init_output_state(
    Ws *ws
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    if ( ! init_view_table( ws ) )
	return 0;

    /* Initialize the workstation transform. */
    owsb->req_ws_window.x_min = 0.0;
    owsb->req_ws_window.x_max = 1.0;
    owsb->req_ws_window.y_min = 0.0;
    owsb->req_ws_window.y_max = 1.0;
    owsb->req_ws_window.z_min = 0.0;
    owsb->req_ws_window.z_max = 1.0;
    owsb->ws_window = owsb->req_ws_window;
    owsb->ws_window_pending = PUPD_NOT_PEND;
    wsgl_set_window(ws, &owsb->ws_window);

    owsb->req_ws_viewport.x_min = 0.0;
    owsb->req_ws_viewport.x_max = (float) ws->ws_rect.width;
    owsb->req_ws_viewport.y_min = 0.0;
    owsb->req_ws_viewport.y_max = (float) ws->ws_rect.height;
    owsb->req_ws_viewport.z_min = 0.0;
    owsb->req_ws_viewport.z_max = 1.0;
    owsb->ws_viewport = owsb->req_ws_viewport;
    owsb->ws_viewport_pending = PUPD_NOT_PEND;
    wsgl_set_viewport(ws, &owsb->ws_viewport);

    wsgl_compute_ws_transform( &owsb->ws_window, &owsb->ws_viewport,
	&owsb->ws_xform );

    /* Initialize the list of posted structs. */
    owsb->posted.lowest.higher = &owsb->posted.highest;
    owsb->posted.lowest.lower = NULL;
    owsb->posted.highest.higher = NULL;
    owsb->posted.highest.lower = &owsb->posted.lowest;

    /* Initialize other miscellaneous output state. */
    owsb->cur_hlhsr_mode = PHIGS_HLHSR_MODE_NONE;
    owsb->req_hlhsr_mode = PHIGS_HLHSR_MODE_NONE;
    owsb->hlhsr_mode_pending = PUPD_NOT_PEND;

    return 1;
}

Ws* phg_wsb_open_ws(
    Phg_args_open_ws *args,
    Phg_ret *ret
    )
{
    XWindowAttributes	wattr;
    Ws			*ws;

    ret->err = -1;
    if ( !(ws = wsgl_create( args )) )
	return ws;

    if ( !wsgl_open_window( ws, args ) )
        goto abort;

    (void)XGetWindowAttributes( ws->display, ws->drawable_id, &wattr );
    WS_SET_WS_RECT( ws, &wattr )

    /* NOTE:
     * Colour model filled in by wsgl
     * Css filled in by popen_ws
     */

    if ( !init_output_state( ws ) )
	goto abort;
    init_update_state( ws );

    /* NOTE:
     * Setup colourmap if used here
     */

    wsb_load_funcs( ws );

    /* Fill in the return data. */
    ret->err = 0;

    return ws;

abort:
    wsb_destroy_ws( ws );
    return NULL;
}

void wsb_free_all_posted(
    Wsb_output_ws *owsb
    )
{
    Ws_post_str	*cur, *end;

    cur = owsb->posted.lowest.higher;
    end = &owsb->posted.highest;
    while ( cur != end ) {
	cur = cur->higher;
	free( (char *)cur->lower );
    }
    owsb->posted.lowest.higher = end;
    end->lower = &owsb->posted.lowest;
}

void wsb_destroy_ws(
    Ws *ws
    )
{
    if ( ws ) {
	if ( ws->out_ws.model.b.views )
	    free( (char *)ws->out_ws.model.b.views );
	if ( ws->out_ws.model.b.pending_views )
	    free( (char *)ws->out_ws.model.b.pending_views );
	if ( ws->out_ws.model.b.view_priorities )
	    free( (char *)ws->out_ws.model.b.view_priorities );
	if ( ws->display ) {
	    if ( ws->drawable_id )
		wsgl_release_window( ws );

            /* NOTE:
             * Free renderer resource here if needed
             * Destroy colourmap here if needed
             */

	    XFlush( ws->display );
	}
	wsgl_destroy( ws );
    }
}

void phg_wsb_close_ws(
    Ws *ws
    )
{
    if ( ws ) {
	wsb_free_all_posted( &ws->out_ws.model.b );
	wsb_destroy_ws( ws );
    }
}

void phg_wsb_redraw_all(
    Ws *ws,
    Pctrl_flag clear_control
    )
{
#ifdef DEBUG
    printf("wsb: Redraw.\n");
#endif

    (*ws->make_requested_current)( ws );
    (*ws->repaint_all)( ws, clear_control );
    ws->out_ws.model.b.vis_rep = PVISUAL_ST_CORRECT;
}

/* Make all "requested" and pending data current. */
void phg_wsb_make_requested_current(
    Ws *ws
    )
{
    Ws_view_entry	*view;
    Ws_pending_view	*req_view;

    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    /* WS transform */
    if ( owsb->ws_window_pending == PUPD_PEND
	    || owsb->ws_viewport_pending == PUPD_PEND ) {
	if ( owsb->ws_window_pending == PUPD_PEND ) {
#ifdef DEBUG
            printf("wsb: Set window\n");
#endif
	    owsb->ws_window = owsb->req_ws_window;
	    owsb->ws_window_pending = PUPD_NOT_PEND;
            wsgl_set_window(ws, &owsb->ws_window);
	}

	if ( owsb->ws_viewport_pending == PUPD_PEND ) {
#ifdef DEBUG
            printf("wsb: Set viewport\n");
#endif
	    owsb->ws_viewport = owsb->req_ws_viewport;
	    owsb->ws_viewport_pending = PUPD_NOT_PEND;
            wsgl_set_viewport(ws, &owsb->ws_viewport);
	}

#ifdef DEBUG
            printf("wsb: Compute transform\n");
#endif
	wsgl_compute_ws_transform( &owsb->ws_window, &owsb->ws_viewport,
	    &owsb->ws_xform );
    }

    /* View table */
    if ( owsb->num_pending_views > 0 ) {
#ifdef DEBUG
            printf("wsb: Pending views\n");
#endif
	req_view = owsb->pending_views;
	while ( owsb->num_pending_views > 0 ) {
	    view = &owsb->views[req_view->id];
	    /*Set it locally. */
	    view->pending = PUPD_NOT_PEND;
	    memcpy(view->vom,
	           req_view->view.ori_matrix,
		   sizeof(Pmatrix3));
	    memcpy(view->vmm,
	           req_view->view.map_matrix,
		   sizeof(Pmatrix3));
	    view->clip_limit = req_view->view.clip_limit;
	    view->xy_clip = req_view->view.xy_clip;
	    view->back_clip = req_view->view.back_clip;
	    view->front_clip = req_view->view.front_clip;
	    view->npc_to_wc_state = WS_INV_NOT_CURRENT;

	    /* Set it in the server.  Can't set them all as a block with one
	     * request because the pending views may not be contiguous.
	     */
#ifdef DEBUG
	    printf("wsb: Set pending view: %d\n", req_view->id);
#endif
            /* NOTE:
             * Send view to renderer here if needed
             */
	    ++req_view;
	    --owsb->num_pending_views;
	}
    }

    /* Other pending data */
    if ( owsb->hlhsr_mode_pending == PUPD_PEND) {

	 owsb->cur_hlhsr_mode = owsb->req_hlhsr_mode;
	 owsb->hlhsr_mode_pending = PUPD_NOT_PEND;

         /* NOTE:
          * Change renderer hlhsr mode here if needed
          */
    }

#ifdef DEBUG
    printf("wsb: Flush\n");
#endif

    /* Make it all take effect. */
    wsgl_flush(ws);
}

void phg_wsb_repaint_all(
    Ws *ws,
    Pctrl_flag clear_control
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    /* assuming this stuff does clear */
    if (clear_control == PFLAG_ALWAYS
        || owsb->surf_state == PSURF_NOT_EMPTY) {
	/* TODO: Need a way to "clear" the window that uses the zero-th
	 * entry in the WS colour table and runs it through colour mapping.
	 */
#ifdef DEBUG
        printf("wsb: Clear\n");
#endif

	wsgl_clear();
    }
    owsb->surf_state = PSURF_EMPTY;

    phg_wsb_traverse_all_postings(ws);

    /* now swap the buffers and update the drawable indices */
    wsgl_flush(ws);

#if TODO
    /* Redraw input prompts & echos of any active input devices. */
    if ( ws->input_repaint && WS_ANY_INP_DEV_ACTIVE(ws) )
	(ws->input_repaint)( ws, num_rects, exposure_rects );
#endif
}

void phg_wsb_traverse_all_postings(
    Ws *ws
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;
    Ws_post_str		*post_str, *end;

    WSB_CHECK_POSTED(&owsb->posted);
    if( WSB_SOME_POSTED(&owsb->posted) ) {
	/* Set up for complete traversal. */
	post_str = owsb->posted.lowest.higher;
	end = &(owsb->posted.highest);
        wsgl_begin_rendering(ws);
	while ( post_str != end ) {
	    phg_wsb_traverse_net( ws, post_str->structh );
	    post_str = post_str->higher;
	}
        wsgl_end_rendering();
	owsb->surf_state = PSURF_NOT_EMPTY;
    }
}

void phg_wsb_traverse_net(
    Ws_handle ws,
    Struct_handle structp
    )
{
    El_handle	el;

    el = structp->first_el;
    while ( 1 ) {	/* termination test is at the bottom */
	switch ( el->eltype ) {
	    case PELEM_NIL:
		break;
	    case PELEM_EXEC_STRUCT:
		phg_wsb_traverse_net( ws, (Struct_handle)el->eldata.ptr );
		break;
	    default:
                wsgl_render_element(ws, el);
		break;
	}

	if ( el == structp->last_el )
	    break;  /* out of the while over all elements in struct */
	el = el->next;
    }
}

static int wsb_visible_element_type(
    El_handle el
    )
{
    int		status = 1;	/* almost all are */

    switch ( el->eltype ) {
#ifdef TODO
	case PELEM_APPL_DATA:
#endif
	case PELEM_LABEL:
	case PELEM_PICK_ID:
	    status = 0;
	    break;
        default:
            /* Default */
            break;
    }

    return status;
}

void phg_wsb_add_el(
    Ws *ws
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;
    El_handle		cur_el = CSS_CUR_ELP(owsb->cssh);

#ifdef DEBUG
    printf("wsb: Add\n");
#endif

    assert(CSS_CUR_STRUCTP(owsb->cssh)); /* A structure must be open */
    WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, &owsb->now_action);
    switch ( owsb->now_action ) {
	case_PHG_UPDATE_ACCURATE_or_IF_Ix:
	default:
	    if ( wsb_visible_element_type( cur_el ) )
		(*ws->redraw_all)( ws, PFLAG_COND );
	    break;

	case PHG_UPDATE_UWOR:
	case PHG_UPDATE_NOTHING:
	case PHG_UPDATE_UQUM:
	    owsb->vis_rep = PVISUAL_ST_DEFER;
	    break;
    }
}

int phg_wsb_asti_update(
    Ws *ws,
    Pctrl_flag clear_control
    )
{
    /* Returns non-zero if redraw occurred. */

    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

#ifdef DEBUG
    printf("wsb: Asti\n");
#endif
    switch ( (*owsb->update_action_table)
	    [(int)PHG_TIME_ATI]
	    [(int)ws->out_ws.mod_mode]
	    [(int)ws->out_ws.def_mode] ) {
	case PHG_UPDATE_IF_INCORRECT:
	case PHG_UPDATE_IF_IL:
	case PHG_UPDATE_IF_IG:
	    if ( owsb->vis_rep == PVISUAL_ST_CORRECT )
		break;
	    /* else fall through to PHG_UPDATE_ACCURATE case */
	case PHG_UPDATE_ACCURATE:
	    (*ws->redraw_all)( ws, clear_control );
	    return 1;

	case PHG_UPDATE_UQUM:
	case PHG_UPDATE_UWOR:
	case PHG_UPDATE_NOTHING:
	    break;
        default:
            /* Default */
            break;
    }

    return 0;
}

void phg_wsb_close_struct(
    Ws *ws,
    Struct_handle structh
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

#ifdef DEBUG
    printf("wsb: Close\n");
#endif

    WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, &owsb->now_action);
    /* First, do processing that is independent of screen output */
    switch ( owsb->now_action ) {
	case_PHG_UPDATE_ACCURATE_or_IF_Ix:
	case PHG_UPDATE_UWOR:
	case PHG_UPDATE_NOTHING:
	case PHG_UPDATE_UQUM:
	default:
	    break;
    }

    /* Updates are implementation dependent in ASTI mode.  This is one
     * of the cases where we do an ASTI update;  we're hopefully doing the
     * application a favor.
     */
    (void)phg_wsb_asti_update( ws, PFLAG_COND );
}

static void wsb_update_a_posting(
    Ws *ws,
    Ws_post_str *posting
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

#ifdef DEBUG
    printf("wsb: Posting\n");
#endif

    WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, &owsb->now_action);
    switch ( owsb->now_action ) {
	case_PHG_UPDATE_ACCURATE_or_IF_Ix:
	    (*ws->redraw_all)( ws, PFLAG_COND );
	    break;

	case PHG_UPDATE_UWOR:
	case PHG_UPDATE_NOTHING:
	case PHG_UPDATE_UQUM:
	    owsb->vis_rep = PVISUAL_ST_DEFER;
	    break;
        default:
            /* Default */
            break;
    }
}

void phg_wsb_post(
    Ws *ws,
    Struct_handle structh,
    Pfloat priority,
    Pint first_posting
    )
{
    Ws_post_str 	*start;

    Wsb_output_ws	*owsb = &ws->out_ws.model.b;
    Ws_post_str 	*cur, *end;
    Ws_post_str 	*new;

#ifdef DEBUG
    printf("wsb: Post on workspace: %x\n", (unsigned int) ws);
#endif

    if ( !first_posting ) {
	/* Check to see if structure is already posted. */
	cur = owsb->posted.lowest.higher;
	end = &owsb->posted.highest;
	while ( cur != end && cur->structh != structh )
	    cur = cur->higher;
    }

    /* The structure is already_posted if (cur != end). */
    if ( !first_posting && cur != end ) {
	if( cur->higher != end && priority >= cur->higher->disp_pri ) {
	    start = end->lower;
	    assert(start == owsb->posted.highest.lower);
	    end = cur->higher;	/* insert betw. cur->higher & posted.highest */
	} else if ( cur->lower != &owsb->posted.lowest
		&& priority < cur->lower->disp_pri ) {
	    /* Will insert between start and cur->lower. */
	    start = cur->lower;
	    end = &owsb->posted.lowest;
	} else {
	    /* This is a reposting with the same *relative* prio. */
	    cur->disp_pri = priority;
	    return;
	}

	/* Struct is posted.  Remove it, but re-use its Ws_post_str entry */
	cur->lower->higher = cur->higher;
	cur->higher->lower = cur->lower;
	new = cur;

    } else {
	/* Struct is not currently posted, malloc an element. */
	if ( !(new = (Ws_post_str *)malloc(sizeof(Ws_post_str))) ) {
	    ERR_BUF( ws->erh, ERR900);
	    return;
	}
	start = owsb->posted.highest.lower;
	end = &owsb->posted.lowest;
    }

    /* Now figure out where to insert it, working backwards from start
     * to end
     */
    cur = start;
    while ( cur != end && cur->disp_pri > priority )
	cur = cur->lower;	/* if priorities equal, new after cur */
    /* insert new element w/prio >= than cur's, so cur->higher will be new */
    new->lower = cur;
    new->higher = cur->higher;
    cur->higher = new;
    new->higher->lower = new;
    new->structh = structh;
    new->disp_pri = priority;

    if ( structh->num_el != 0 )
	wsb_update_a_posting( ws, new );
}

/* This function only called from the css for change struct ids/refs -
 * it is used to change the structure pointers referencing a given
 * structure, because the way the change struct ids/refs functions work
 * is by changing struct_id fields rather than copying whole structures,
 * so any lists using structure pointers to reference specific structures
 * have to be changed to use the correct pointers.
 */

void phg_wsb_change_posting(
    Ws *ws,
    Struct_handle unpost,
    Struct_handle post
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;
    Ws_post_str 	*cur, *end;

    cur = owsb->posted.lowest.higher;
    end = &owsb->posted.highest;
    while ( cur != end && cur->structh != unpost )
	cur = cur->higher;

    if ( cur != end ) {
	if ( post ) {
	    /* if the structure to be "posted" is already posted, remove it */
	    phg_wsb_change_posting( ws, post, (Struct_handle)NULL );
	    /* Change posted structure from "unpost" to "post", same priority*/
	    cur->structh = post;
	} else {
	    /* Post is NULL - just remove Ws_post_str entry for unpost. */
	    cur->lower->higher = cur->higher;
	    cur->higher->lower = cur->lower;
	    free( (char *)cur );
	}
    }
}

/* Search the list of posted structures for this one.
 * If found, return pointer to next-higher-priority posted structure element.
 * (Remember that that could be the dummy element owsb->posted.highest)
 * If not, return NULL.
 */
static Ws_post_str* wsb_unpost_struct_if_found(
    Wsb_output_ws *owsb,
    Struct_handle structh
    )
{
    Ws_post_str	*cur, *end;

    cur = owsb->posted.lowest.higher;
    end = &owsb->posted.highest;
    while ( cur != end && cur->structh != structh )
	cur = cur->higher;
    if ( cur != end ) {
	/* Found it -- now delete it */
	cur->lower->higher = cur->higher;
	cur->higher->lower = cur->lower;
	end = cur->higher;	/* Save this around the free */
	free( (char *)cur );
	return end;
    } else
	return (Ws_post_str*)NULL;
}

void phg_wsb_unpost(
    Ws *ws,
    Struct_handle structh
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    if ( !wsb_unpost_struct_if_found( owsb, structh ) )
	/* Tried to unpost structure that wasn't there; but that's okay. */
	return;

    if ( structh->num_el != 0 ) {
	WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, &owsb->now_action);
	switch ( owsb->now_action ) {
	    case_PHG_UPDATE_ACCURATE_or_IF_Ix:
		(*ws->redraw_all)( ws, PFLAG_COND );
		break;

	    case PHG_UPDATE_UWOR:
	    case PHG_UPDATE_NOTHING:
	    case PHG_UPDATE_UQUM:
		owsb->vis_rep = PVISUAL_ST_DEFER;
		break;
            default:
                /* Default */
                break;
	}
    }
}

void phg_wsb_unpost_all(
    Ws *ws
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    wsb_free_all_posted( owsb );
    WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, &owsb->now_action);
    switch ( owsb->now_action ) {
	case_PHG_UPDATE_ACCURATE_or_IF_Ix:
	    (*ws->redraw_all)( ws, PFLAG_COND );
	    break;

	case PHG_UPDATE_UWOR:
	case PHG_UPDATE_NOTHING:
	case PHG_UPDATE_UQUM:
	    owsb->vis_rep = PVISUAL_ST_DEFER;
	    break;
        default:
            /* Default */
            break;
    }
}

void phg_wsb_delete_all_structs(
    Ws *ws
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, &owsb->now_action);
    phg_wsb_unpost_all( ws );
}

int phg_wsb_delete_struct(
    Ws *ws,
    Struct_handle structh,
    Ws_delete_flag flag
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;
    int			call_again = 0;

    WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, &owsb->now_action);
    switch ( owsb->now_action ) {
	case_PHG_UPDATE_ACCURATE_or_IF_Ix:
	    if ( flag == WS_PRE_CSS_DELETE ) {
		(void)wsb_unpost_struct_if_found( owsb, structh );
		call_again = 1;
	    } else
		(*ws->redraw_all)( ws, PFLAG_COND );
	    break;

	case PHG_UPDATE_UWOR:
	case PHG_UPDATE_NOTHING:
	case PHG_UPDATE_UQUM:
	    (void)wsb_unpost_struct_if_found( owsb, structh );
	    owsb->vis_rep = PVISUAL_ST_DEFER;
	    break;
        default:
            /* Default */
            break;
    }

    return call_again;
}

int phg_wsb_delete_struct_net(
    Ws *ws,
    Struct_handle structh,
    Pref_flag reff,
    Ws_delete_flag flag
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;
    int			call_again = 0;

    WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, &owsb->now_action);
    switch ( owsb->now_action ) {
	case_PHG_UPDATE_ACCURATE_or_IF_Ix:
	    if ( flag == WS_PRE_CSS_DELETE ) {
		(void)wsb_unpost_struct_if_found( owsb, structh );
		call_again = 1;
	    } else
		(*ws->redraw_all)( ws, PFLAG_COND );
	    break;

	case PHG_UPDATE_UWOR:
	case PHG_UPDATE_NOTHING:
	case PHG_UPDATE_UQUM:
	default:
	    (void)wsb_unpost_struct_if_found( owsb, structh );
	    owsb->vis_rep = PVISUAL_ST_DEFER;
	    break;
    }
    return call_again;
}

void phg_wsb_copy_struct(
    Ws *ws,
    El_handle first_el
    )
{
    (*ws->conditional_redraw)( ws );
}

/* Delete elements elh1 through elh2, inclusive, in structure structh */
int phg_wsb_delete_el(
    Ws *ws,
    Struct_handle structh,
    El_handle elh1,
    El_handle elh2,
    Ws_delete_flag flag
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;
    int			call_again = 0;

    WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, &owsb->now_action);
    switch ( owsb->now_action ) {
	case_PHG_UPDATE_ACCURATE_or_IF_Ix:
	default:
	    if ( flag == WS_PRE_CSS_DELETE ) {
		if ( elh1 == elh2 && !wsb_visible_element_type( elh1 ) )
		    call_again = 0; /* avoid second call. */
		else
		    call_again = 1;
	    } else		/* POST_CSS_DELETE */
		(*ws->redraw_all)(ws, PFLAG_COND);
	    break;

	case PHG_UPDATE_UWOR:
	case PHG_UPDATE_NOTHING:
	case PHG_UPDATE_UQUM:
	    owsb->vis_rep = PVISUAL_ST_DEFER;
	    break;
    }

    return call_again;
}

/* Called by CP after difficult operations like change struct refs/ids.
 * Redraws workstation, if that is permitted, else DEFERs.
 */
void phg_wsb_conditional_redraw(
    Ws *ws
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, &owsb->now_action);
    switch ( owsb->now_action ) {
	case_PHG_UPDATE_ACCURATE_or_IF_Ix:
	    (*ws->redraw_all)( ws, PFLAG_ALWAYS );
	    break;

	case PHG_UPDATE_UQUM:
	case PHG_UPDATE_UWOR:
	case PHG_UPDATE_NOTHING:
	    owsb->vis_rep = PVISUAL_ST_DEFER;
	    break;
        default:
            /* Default */
            break;
    }
}

/* Resolves PHG_UPDATE_IF_Ix to the ASTI now_action, if no input device active.
 *
 * Check this function before every (out_ws->now_action) use.
 * Then use case_PHG_UPDATE_ACCURATE_or_IF_Ix in switch (out_ws->now_action),
 * so that PHG_UPDATE_IF_Ix acts as PHG_UPDATE_ACCURATE (i.e., ASAP)
 * while appicable input devices are (still) active.
 */
void phg_wsb_resolve_now_action(
    Ws *ws,
    Ws_update_action *now_action_ptr
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    switch ( *now_action_ptr ) {
      case PHG_UPDATE_IF_IL:
	/* If none active, treat like ASTI until next bnig_update(ws,ws) */
	if ( !WS_ANY_INP_DEV_ACTIVE(ws) ) {
	    *now_action_ptr = (*owsb->update_action_table)
		[(int)PHG_TIME_NOW][(int)ws->out_ws.mod_mode][(int)PDEFER_ASTI];
	}
	break;

      case PHG_UPDATE_IF_IG:
	/* If none active, treat like ASTI until next bnig_update(ws,*) */
#ifdef TODO
	if ( !phg_cp_any_inp_device_active( ws->cph ) ) {
	    *now_action_ptr = (*owsb->update_action_table)
		[(int)PHG_TIME_NOW][(int)ws->out_ws.mod_mode][(int)PDEFER_ASTI];
	}
#endif
	break;

        default:
          /* Default */
        break;
    }
}

void phg_wsb_update(
    Ws *ws,
    Pregen_flag flag
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    if ( flag != PFLAG_POSTPONE && owsb->vis_rep != PVISUAL_ST_CORRECT ) 
	(*ws->redraw_all)( ws, PFLAG_COND );
    else
	(*ws->make_requested_current)( ws );
}

void phg_wsb_set_disp_update_state(
    Ws *ws,
    Pdefer_mode def_mode,
    Pmod_mode mod_mode
    )
{
    Ws_update_action	previous_now_action;

    Ws_output_ws	*out_ws = &ws->out_ws;
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    out_ws->def_mode = def_mode;
    out_ws->mod_mode = mod_mode;

    previous_now_action = owsb->now_action;
    owsb->now_action = (*owsb->update_action_table)
	[(int)PHG_TIME_NOW][(int)out_ws->mod_mode][(int)out_ws->def_mode];
    assert(owsb->now_action != PHG_UPDATE_IF_INCORRECT);

    if ( owsb->now_action != previous_now_action ) {
	WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, &owsb->now_action);
	switch ( owsb->now_action ) {
	    case_PHG_UPDATE_ACCURATE_or_IF_Ix:
		if( owsb->vis_rep != PVISUAL_ST_CORRECT )
		    (*ws->redraw_all)( ws, PFLAG_COND );
		break;
            default:
                /* Default */
                break;
	}
    }
}

void phg_wsb_set_hlhsr_mode(
    Ws *ws,
    Pint mode
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    owsb->req_hlhsr_mode = mode;
    owsb->hlhsr_mode_pending = PUPD_PEND;

    WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, &owsb->now_action);
    switch ( owsb->now_action ) {
	case_PHG_UPDATE_ACCURATE_or_IF_Ix:
	    (*ws->redraw_all)( ws, PFLAG_COND );
	    break;

	case PHG_UPDATE_UWOR:
	case PHG_UPDATE_UQUM:
	case PHG_UPDATE_NOTHING:
	default:
	    owsb->vis_rep = PVISUAL_ST_DEFER;
	    break;
    }
}

void phg_wsb_set_ws_window(
    Ws *ws,
    Pint two_d,
    Plimit3 *limits
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    owsb->ws_window_pending = PUPD_PEND;
    if ( two_d ) {	/* leave the z values as they are */
	owsb->req_ws_window.x_min = limits->x_min;
	owsb->req_ws_window.x_max = limits->x_max;
	owsb->req_ws_window.y_min = limits->y_min;
	owsb->req_ws_window.y_max = limits->y_max;
    } else
	owsb->req_ws_window = *limits;

    WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, &owsb->now_action);
    switch ( owsb->now_action ) {
	case_PHG_UPDATE_ACCURATE_or_IF_Ix:
	    (*ws->redraw_all)( ws, PFLAG_COND );
	    break;

	case PHG_UPDATE_UQUM:
	case PHG_UPDATE_UWOR:
	case PHG_UPDATE_NOTHING:
	default:
	    owsb->vis_rep = PVISUAL_ST_DEFER;
	    break;
    }
}

void phg_wsb_set_ws_vp(
    Ws *ws,
    Pint two_d,
    Plimit3 *limits
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    owsb->ws_viewport_pending = PUPD_PEND;
    if ( two_d ) {	/* leave the z values as they are */
	owsb->req_ws_viewport.x_min = limits->x_min;
	owsb->req_ws_viewport.x_max = limits->x_max;
	owsb->req_ws_viewport.y_min = limits->y_min;
	owsb->req_ws_viewport.y_max = limits->y_max;
    } else
	owsb->req_ws_viewport = *limits;

    WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, &owsb->now_action);
    switch ( owsb->now_action ) {
	case_PHG_UPDATE_ACCURATE_or_IF_Ix:
	    (*ws->redraw_all)( ws, PFLAG_COND );
	    break;

	case PHG_UPDATE_UQUM:
	case PHG_UPDATE_UWOR:
	case PHG_UPDATE_NOTHING:
	default:
	    owsb->vis_rep = PVISUAL_ST_DEFER;
	    break;
    }
}

void phg_wsb_set_view_input_priority(
    Ws *ws,
    Pint index,
    Pint ref_index,
    Prel_pri priority
    )
{
    Pint		old;

    Ws_view_priority	*ref, *idx;
    Pint		*highest;
    Ws_view_priority	*vp;

    vp = ws->out_ws.model.b.view_priorities;
    highest = &ws->out_ws.model.b.top_view;
    idx = &vp[index];
    ref = &vp[ref_index];

    if ( priority == PPRI_LOWER ) {
	if ( ref->lower != index ) {
	    if ( index == *highest)
		*highest = idx->lower;

	    old = ref->lower;
	    if ( ref->lower != -1 )	/* if ref not lowest priority */
		vp[ref->lower].higher = index;
	    ref->lower = index;

	    if ( idx->higher != -1 )	 /* if idx not highest priority */
		vp[idx->higher].lower = idx->lower;
	    if ( idx->lower != -1 )	 /* if idx not lowest priority */
		vp[idx->lower].higher = idx->higher;
	    idx->higher = ref_index;
	    idx->lower = old;
	}
	/* Don't need to do anything if priority is already as desired. */

    } else if ( ref->higher != index ) {	/* PPRI_HIGHER */
	if ( ref_index == *highest )
	    *highest = index;
	else if ( index == *highest )
	    *highest = idx->lower;

	old = ref->higher;
	if ( ref->higher != -1 )
	    vp[ref->higher].lower = index;
	ref->higher = index;

	if ( idx->higher != -1 )
	    vp[idx->higher].lower = idx->lower;
	if ( idx->lower != -1 )
	    vp[idx->lower].higher = idx->higher;
	idx->lower = ref_index;
	idx->higher = old;
    }

    /* Has no effect on the screen */
}

void phg_wsb_set_rep(
    Ws *ws,
    Phg_args_rep_type type,
    Phg_args_rep_data *rep
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;
    int		i;
    Pgcolr      gcolr;

    switch ( type ) {
	case PHG_ARGS_LNREP:
	case PHG_ARGS_EXTLNREP:
	case PHG_ARGS_MKREP:
	case PHG_ARGS_EXTMKREP:
	case PHG_ARGS_TXREP:
	case PHG_ARGS_EXTTXREP:
	case PHG_ARGS_INTERREP:
	case PHG_ARGS_EXTINTERREP:
        case PHG_ARGS_EFREP:
	case PHG_ARGS_EDGEREP:
	case PHG_ARGS_EXTEDGEREP:
	case PHG_ARGS_PTREP:
	case PHG_ARGS_EXTPTREP:
	case PHG_ARGS_DCUEREP:
	case PHG_ARGS_LIGHTSRCREP:
	case PHG_ARGS_COLRMAPREP:
#if TODO
	    phg_wsx_set_LUT_entry( ws, type, rep, (Pgcolr*)NULL );
#endif
	    break;

	case PHG_ARGS_VIEWREP:
	    /* Add it to the list of pending views. */
	    if ( owsb->views[rep->index].pending == PUPD_NOT_PEND )
		i = owsb->num_pending_views++;
	    else {
		/* Find the existing pending entry so it can be replaced. */
		for ( i = 0; i < owsb->num_pending_views; i++ )
		    if ( owsb->pending_views[i].id == rep->index )
			break;
	    }
	    owsb->pending_views[i].id = rep->index;
	    owsb->pending_views[i].view = rep->bundl.viewrep;
	    owsb->views[rep->index].pending = PUPD_PEND;
	    break;

	case PHG_ARGS_COREP:
	    /* Store in current colour model. */
	    gcolr.type = ws->current_colour_model;
	    gcolr.val.general.x = rep->bundl.corep.rgb.red;
	    gcolr.val.general.y = rep->bundl.corep.rgb.green;
	    gcolr.val.general.z = rep->bundl.corep.rgb.blue;
            memcpy(&ws->colr_table[rep->index], &gcolr, sizeof(Pgcolr));
	    break;
    }

    WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, &owsb->now_action);
    switch ( owsb->now_action ) {
	case_PHG_UPDATE_ACCURATE_or_IF_Ix:
	    (*ws->redraw_all)( ws, PFLAG_COND );
	    break;

	case PHG_UPDATE_NOTHING:
	    /* Defer if rep has PENDING flag, or if screen could be affected. */
	    if ( WSB_SOME_POSTED(&owsb->posted) || (type == PHG_ARGS_VIEWREP) )
		 owsb->vis_rep = PVISUAL_ST_DEFER;
	    break;

	default:
	case PHG_UPDATE_UQUM:
	case PHG_UPDATE_UWOR:
	    owsb->vis_rep = PVISUAL_ST_DEFER;
	    break;
    }
}

#if 0
void
phg_wsb_set_filter( ws, type, devid, inc_set, exc_set )
    Ws			*ws;
    Phg_args_flt_type	type;
    Pint		devid;
    Pint_list		*inc_set;
    Pint_list		*exc_set;
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    phg_wsx_set_name_set( ws, type, devid, inc_set, exc_set );

    if ( (type == PHG_ARGS_FLT_HIGH || type == PHG_ARGS_FLT_INVIS)
	    && WSB_SOME_POSTED(&owsb->posted) ) {
	WSB_CHECK_FOR_INTERACTION_UNDERWAY(ws, &owsb->now_action);
	switch ( owsb->now_action ) {
	    case_PHG_UPDATE_ACCURATE_or_IF_Ix:
		(*ws->redraw_all)( ws, PFLAG_COND );
		break;

	    case PHG_UPDATE_UQUM:
	    case PHG_UPDATE_UWOR:
	    case PHG_UPDATE_NOTHING:
	    default:
		owsb->vis_rep = PVISUAL_ST_DEFER;
		break;
	}
    }
}
#endif

void phg_wsb_inq_view_indices(
    Ws *ws,
    Phg_ret *ret
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    Pint		*list, view;
    int			i;
    Ws_view_priority	*prio = owsb->view_priorities;

    if ( !PHG_SCRATCH_SPACE( &ws->scratch, owsb->num_views * sizeof(Pint))) {
	ret->err = ERR900;

    } else {
	ret->err = 0;
	ret->data.int_list.num_ints = owsb->num_views;
	ret->data.int_list.ints = list = (Pint *)ws->scratch.buf;
	view = owsb->top_view;
	i = 0;
	while ( view != -1 ) {
	    list[i++] = view;
	    view = prio[view].lower;
	}
    }
}

void phg_wsb_inq_posted(
    Ws *ws,
    Phg_ret *ret
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    Ws_post_str	*cur, *end;
    int		cnt;
    Pposted_struct	*list;

    cur = owsb->posted.lowest.higher;
    end = &owsb->posted.highest;

    /* Count them */
    cnt = 0;
    while ( cur != end ) {
	++cnt;
	cur = cur->higher;
    }

    ret->err = 0;
    ret->data.postlist.num_postings = cnt;
    if ( cnt > 0 ) {
	if ( PHG_SCRATCH_SPACE(&ws->scratch, cnt * sizeof(Pposted_struct)) ) {
	    ret->data.postlist.postings = list =
		(Pposted_struct *)ws->scratch.buf;
	    cur = owsb->posted.lowest.higher;
	    while ( cur != end ) {
		list->id = cur->structh->struct_id;
		list++->disp_pri = cur->disp_pri;
		cur = cur->higher;
	    }

	} else {
	    ret->err = ERR900;
	    ret->data.postlist.num_postings = 0;
	}
    }
}

void phg_wsb_inq_view_rep(
    Ws *ws,
    Pint index,
    Phg_ret *ret
    )
{
    int			i;
    Pview_rep3		*cr;
    Ws_view_entry	*cv;
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;
    Phg_ret_view_rep	*vr = &ret->data.view_rep;

    ret->err = 0;
    vr->update_state = owsb->views[index].pending;
    /* Load the "current" view. */
    if ( !PHG_SCRATCH_SPACE( &ws->scratch, sizeof(Pview_rep3) ) ) {
	ret->err = ERR900;
	return;
    } else {
	vr->cur_rep = cr = (Pview_rep3 *)ws->scratch.buf;
	cv = &owsb->views[index];
	cr->clip_limit = cv->clip_limit;
	cr->xy_clip = cv->xy_clip;
	cr->back_clip = cv->back_clip;
	cr->front_clip = cv->front_clip;
	memcpy(cr->ori_matrix, cv->vom, sizeof(Pmatrix3));
	memcpy(cr->map_matrix, cv->vmm, sizeof(Pmatrix3));
    }

    /* Load the "requested" view. */
    if ( vr->update_state == PUPD_NOT_PEND )	/* save some time */
	vr->req_rep = vr->cur_rep;
    else {
	/* Find the requested entry in the pending view list. */
	for ( i = 0; i < owsb->num_pending_views; i++ ) {
	    if ( owsb->pending_views[i].id == index ) {
		cr = &owsb->pending_views[i].view;
		break;	/* we've found it, no need to keep searching */
	    }
	}
	vr->req_rep = cr;
    }
}

void phg_wsb_inq_ws_xform(
    Ws *ws,
    Phg_ret *ret
    )
{
    Phg_ret_ws_tran3	*wsxf = &ret->data.ws_xform;
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    ret->err = 0;
    wsxf->state =
	   owsb->ws_window_pending == PUPD_PEND
	|| owsb->ws_viewport_pending == PUPD_PEND
	    ? PUPD_PEND : PUPD_NOT_PEND;
    wsxf->req_window = owsb->req_ws_window;
    wsxf->req_viewport = owsb->req_ws_viewport;
    wsxf->cur_window = owsb->ws_window;
    wsxf->cur_viewport = owsb->ws_viewport;
}

void phg_wsb_inq_disp_update_state(
    Ws *ws,
    Phg_ret *ret
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    ret->err = 0;
    ret->data.update_state.def_mode = ws->out_ws.def_mode;
    ret->data.update_state.mod_mode = ws->out_ws.mod_mode;
    ret->data.update_state.display_surf = owsb->surf_state;
    ret->data.update_state.state = owsb->vis_rep;
}

void phg_wsb_inq_hlhsr_mode(
    Ws *ws,
    Phg_ret *ret
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

    ret->err = 0;
    ret->data.hlhsr_mode.state =  owsb->hlhsr_mode_pending;
    ret->data.hlhsr_mode.cur_mode = owsb->cur_hlhsr_mode;
    ret->data.hlhsr_mode.req_mode = owsb->req_hlhsr_mode;
}

void phg_wsb_inq_rep(
    Ws *ws,
    Pint index,
    Pinq_type how,
    Phg_args_rep_type rep_type,
    Phg_ret *ret
    )
{
    Pgcolr gcolr;
    Pcolr_rep *cb;

    ret->err = 0;
    switch ( rep_type ) {
	case PHG_ARGS_LNREP:
	case PHG_ARGS_EXTLNREP:
	case PHG_ARGS_MKREP:
	case PHG_ARGS_EXTMKREP:
	case PHG_ARGS_TXREP:
	case PHG_ARGS_EXTTXREP:
	case PHG_ARGS_INTERREP:
	case PHG_ARGS_EXTINTERREP:
	case PHG_ARGS_EDGEREP:
	case PHG_ARGS_EXTEDGEREP:
	case PHG_ARGS_PTREP:
	case PHG_ARGS_EXTPTREP:
	case PHG_ARGS_DCUEREP:
	case PHG_ARGS_LIGHTSRCREP:
	case PHG_ARGS_COLRMAPREP:
	/* View rep is done elsewhere. */
#ifdef TODO
	    phg_wsx_inq_LUT_entry( ws, index, how, rep_type, ret,
		(Pgcolr *)NULL, (Pview_rep3 *)NULL );
#endif
	    break;

	case PHG_ARGS_COREP:
	    cb = &ret->data.rep.corep;

	    /* Need to convert to current colour model. */
            memcpy(&gcolr, &ws->colr_table[index], sizeof(Pgcolr));

            /* NOTE:
             * Convert to correct colour model here if needed
             */

	    cb->rgb.red = gcolr.val.general.x;
	    cb->rgb.green = gcolr.val.general.y;
	    cb->rgb.blue = gcolr.val.general.z;
	    break;

	default:
            /* Default */
            break;
    }
}

int phg_wsb_map_initial_points(
    Ws *ws,
    Pint view_index,
    Pint *num_pts,
    Ppoint3 *wc_pts,
    XPoint *dwbl_pts
    )
{
    Ppoint3       scratch[20];  /* enough for most cases */
    Pmatrix3      wc_to_npc;
    Ppoint3       dc_pt;

    Ppoint3       *npc_pts = (Ppoint3 *)NULL;
    int           i;
    Ws_view_entry *view;
    Ws_xform      *wsxf = &ws->out_ws.model.b.ws_xform;

    /* Transform the initial points to NPC and check that they all fit
     * in the clip limits of the specified view.  Then transform and map
     * them to drawable coordinates.
     */
    if ( *num_pts <= 0 )
	return 0;
    if ( *num_pts <= sizeof(scratch)/sizeof(scratch[0]) )
	npc_pts = scratch;
    else if ( *num_pts > sizeof(scratch)/sizeof(scratch[0]) ) {
	if ( !(npc_pts = (Ppoint3 *)malloc( (unsigned)(*num_pts *
		sizeof(Ppoint3)) )) ) {
	    *num_pts = 0;
	    ERR_BUF( ws->erh, ERR900 );
	    return 0;
	}
    }

    view = &ws->out_ws.model.b.views[view_index];
    phg_mat_mul( wc_to_npc, view->vmm, view->vom );
    if ( !phg_tranpts3( wc_to_npc, *num_pts, wc_pts, npc_pts ) ) {
	*num_pts = 0;
	return 0;
    }

    for ( i = 0; i < *num_pts; i++ ) {
	if ( !WS_PT_IN_LIMIT( &view->clip_limit, &npc_pts[i] ) ) {
	    *num_pts = 0;
	    break;
	} else {
	    WS_NPC_TO_DC( wsxf, &npc_pts[i], &dc_pt )
	    WS_DC_TO_DRWBL2( ws, &dc_pt, &dwbl_pts[i] );
	}
    }

    if ( npc_pts && npc_pts != scratch )
	free( (char *)npc_pts );

    return ( *num_pts > 0 ? 1 : 0 );
}

static void update_inv_view_xform(
    Ws_view_entry *view
    )
{
     /* Calculate the inverse xform, if necessary. */
    if ( view->npc_to_wc_state == WS_INV_NOT_CURRENT ) {
	phg_mat_mul( view->npc_to_wc, view->vmm, view->vom );
	phg_mat_inv( view->npc_to_wc );
	view->npc_to_wc_state = WS_INV_CURRENT;
    }
}

int phg_wsb_resolve_locator(
    Ws *ws,
    Ppoint3 *dc_pt,
    int	determine_z,
    Pint *view_index,
    Ppoint3 *wc_pt
    )
{
    Ppoint3		npc_pt;
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;
    Ws_view_priority	*priorities = owsb->view_priorities;
    Ws_xform		*wsxf = &owsb->ws_xform;
    Plimit3		*ws_win = &owsb->ws_window;
    int			status = 0;

    Ws_view_entry	*view;

    /* Apply the inverse WS transform and see if it's in the ws window.
     * Can't just check against the viewport boundaries because the
     * window may be smaller if the aspect ratios are different.
     */
    WS_DC_TO_NPC2(wsxf, dc_pt, &npc_pt)
    if ( npc_pt.x >= ws_win->x_min && npc_pt.x <= ws_win->x_max
	&& npc_pt.y >= ws_win->y_min && npc_pt.y <= ws_win->y_max ) {

	/* Find the highest priority view that contains the point. */
	for ( *view_index = owsb->top_view; *view_index != -1;
		*view_index = priorities[*view_index].lower ) {
	    view = &owsb->views[*view_index];
	    if ( WS_PT_IN_LIMIT2(&view->clip_limit, &npc_pt) ) {
		/* Assign the clip window minimum to Z. */
		npc_pt.z = view->clip_limit.z_min;

		/* Calculate the inverse xform if necessary. */
		if ( view->npc_to_wc_state == WS_INV_NOT_CURRENT )
		    update_inv_view_xform( view );
  
		/* Map point to WC if xform invertible. */
		if ( view->npc_to_wc_state == WS_INV_CURRENT ) {
		    if ( phg_tranpt3( &npc_pt, view->npc_to_wc, wc_pt ) ) {
			status = 1;
			break;	/* out of the for on view index */
		    }
		}
	    }
	}
    }
    return status;
}

static int wsb_resolve_stroke(
    Ws *ws,
    int	two_d,
    Ppoint3 *dc_ll,
    Ppoint3 *dc_ur,
    Pint *view_index
    )

    /* Returns 1 if stroke resolvable, else 0 */
{
    Ppoint3		npc_ll, npc_ur;
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;
    Ws_view_priority	*priorities = owsb->view_priorities;
    Ws_xform		*wsxf = &owsb->ws_xform;
    Plimit3		*ws_win = &owsb->ws_window;
    int			status = 0, in_limit;

    Ws_view_entry	*view;

    /* Apply the inverse WS transform and see if the bounding box is in
     * the ws window.  Can't just check against the viewport boundaries
     * because the window may be smaller if the aspect ratios are different.
     */
    in_limit = 0;
    if ( two_d ) {
	WS_DC_TO_NPC2(wsxf, dc_ll, &npc_ll)
	WS_DC_TO_NPC2(wsxf, dc_ur, &npc_ur)
	in_limit = WS_PT_IN_LIMIT2(ws_win, &npc_ll)
	    && WS_PT_IN_LIMIT2(ws_win, &npc_ur);
    } else {
	WS_DC_TO_NPC(wsxf, dc_ll, &npc_ll)
	WS_DC_TO_NPC(wsxf, dc_ur, &npc_ur)
	in_limit = WS_PT_IN_LIMIT(ws_win, &npc_ll)
	    && WS_PT_IN_LIMIT(ws_win, &npc_ur);
    }

    if ( in_limit ) {
	/* Find the highest priority view that contains the bounding box. */
	for ( *view_index = owsb->top_view; *view_index != -1;
		*view_index = priorities[*view_index].lower ) {
	    view = &owsb->views[*view_index];
	    in_limit = 0;
	    if ( two_d )
		in_limit = WS_PT_IN_LIMIT2(&view->clip_limit, &npc_ll)
		    && WS_PT_IN_LIMIT2(&view->clip_limit, &npc_ur);
	    else
		in_limit = WS_PT_IN_LIMIT(&view->clip_limit, &npc_ll)
		    && WS_PT_IN_LIMIT(&view->clip_limit, &npc_ur);

	    if ( in_limit ) {
		/* Found the view.  Calculate its inverse transform if
		 * necessary.
		 */
		if ( view->npc_to_wc_state == WS_INV_NOT_CURRENT )
		    update_inv_view_xform( view );

		if ( view->npc_to_wc_state == WS_INV_CURRENT ) {
		    status = 1;
		    break;  /* break out of the view loop when view found */
		}
	    }
	}
    }
    return status;
}

static void wsb_transform_stroke(
    Ws *ws,
    Pint view_index,
    int two_d,
    int	num_pts,
    Ppoint3 *dc_pts,
    Ppoint_list3 *wc_pts
    )
{
    int			i;
    Ppoint3		*npc_pts;
    Ws_xform		*wsxf = &ws->out_ws.model.b.ws_xform;
    Ws_view_entry	*view = &ws->out_ws.model.b.views[view_index];

    /* Shouldn't call this function with num_pts == 0. */
    if ( !(npc_pts = (Ppoint3 *)
	    malloc( (unsigned)(num_pts * sizeof(Ppoint3)) )) ) {
	wc_pts->num_points = 0;
	ERR_BUF( ws->erh, ERR900 );
	return;
    }

    /* Transform the points to npc and add the z value. */
    for ( i = 0; i < num_pts; i++ ) {
	if ( two_d ) {
	    WS_DC_TO_NPC2(wsxf, &dc_pts[i], &npc_pts[i])
	    /* Assign the back clip limit. */
	    npc_pts[i].z = view->clip_limit.z_min;
	} else {
	    WS_DC_TO_NPC(wsxf, &dc_pts[i], &npc_pts[i])
	}
    }

    /* Transform the points to wc. */
    if ( !phg_tranpts3( view->npc_to_wc, num_pts, npc_pts, wc_pts->points ) )
	wc_pts->num_points = 0;

    free( (char *)npc_pts );
}

int phg_wsb_resolve_stroke(
    Ws *ws,
    int num_pts,
    Ppoint3 *dc_pts,
    int determine_z,
    Pint *view_index,
    Ppoint_list3 *wc_pts
    )
{
    Ppoint3	ll, ur;
    int		status = 0, two_d = determine_z;

    Ppoint3	*dp;
    int		i, xmin, xmax, ymin, ymax, zmin, zmax;

    /* Get the bounding box of all the points. */
    xmin = dc_pts->x; xmax = dc_pts->x;
    ymin = dc_pts->y; ymax = dc_pts->y;
    if ( !two_d )
	zmin = dc_pts->z, zmax = dc_pts->z;
    for ( i = 1, dp = &dc_pts[1]; i < num_pts; i++, dp++ ) {
	if ( dp->x < xmin )
	    xmin = dp->x;
	else if ( dp->x > xmax )
	    xmax = dp->x;

	if ( dp->y < ymin )
	    ymin = dp->y;
	else if ( dp->y > ymax )
	    ymax = dp->y;
	
	if ( !two_d ) {
	    /* The incoming points have Z values. */
	    if ( dp->z < zmin )
		zmin = dp->z;
	    else if ( dp->z > zmax )
		zmax = dp->z;
	}
    }
    ll.x = xmin; ll.y = ymax;
    ur.x = xmax; ur.y = ymin;
    if ( !two_d )
	ur.x = xmax, ur.y = ymin;

    /* Resolve and transform the points.  Don't change the current
     * measure if the points can't be resolved.
     */
    if ( wsb_resolve_stroke( ws, two_d, &ll, &ur, view_index ) ) {
	wc_pts->num_points = num_pts;
	wsb_transform_stroke( ws, *view_index, two_d, num_pts, dc_pts, wc_pts );
	status = 1;
    }

    return status;
}

#ifdef NOT_YET
int phg_wsb_resolve_pick(
    Ws *ws,
    Ws_inp_pick *dev,
    int	echo,
    Ppoint3 *dc_pt,
    Ppick *pick
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;
    Ws_post_str		*post_str, *end;
    Ppoint		dc_vol[2];
    Ppoint		npc_pt[2];
    void		*pickPath;
    int			pickDepth;
    int			betterPick, i;

    int			pickDataBytes;

    struct {
	pexEnumTypeIndex pickType;
	CARD16		 unused;
	union {
	    pexPD_DC_HitBox	dcHitBox;
	    pexPD_NPC_HitVolume npcHitVolume;
	} pickRec;
    } pickData;
    
    WSB_CHECK_POSTED (&owsb->posted)

    if (WSB_SOME_POSTED (&owsb->posted)) {

	/*
	 * Use pick rendering to get the pick results.  Call BeginPickOne, do
	 * a complete traversal (starting with highest priority structure,
	 * then call EndPickOne to get the pick results.
	 */

	if (dev->dev_type == PEXPickDeviceDC_HitBox) {

	    pickData.pickType = PEXPickDeviceDC_HitBox;

	    pickData.pickRec.dcHitBox.position.x = dc_pt->x;
	    pickData.pickRec.dcHitBox.position.y = dc_pt->y;
	    pickData.pickRec.dcHitBox.distance = dev->ap_size;

	    pickDataBytes = 4 + sizeof (pexPD_DC_HitBox);

	} else {

	    pickData.pickType = PEXPickDeviceNPC_HitVolume;

	    dc_vol[0].x = dc_pt->x - dev->ap_size;
	    dc_vol[0].y = dc_pt->y - dev->ap_size;
	    dc_vol[1].x = dc_pt->x + dev->ap_size;
	    dc_vol[1].y = dc_pt->y + dev->ap_size;

	    WS_DC_TO_NPC2 (&owsb->ws_xform, &dc_vol[0], &npc_pt[0])
	    WS_DC_TO_NPC2 (&owsb->ws_xform, &dc_vol[1], &npc_pt[1])

	    pickData.pickRec.npcHitVolume.minval.x = npc_pt[0].x;
	    pickData.pickRec.npcHitVolume.minval.y = npc_pt[0].y;
	    pickData.pickRec.npcHitVolume.maxval.x = npc_pt[1].x;
	    pickData.pickRec.npcHitVolume.maxval.y = npc_pt[1].y;
	    pickData.pickRec.npcHitVolume.minval.z = owsb->ws_window.z_min;
	    pickData.pickRec.npcHitVolume.maxval.z = owsb->ws_window.z_max;

	    pickDataBytes = 4 + sizeof (pexPD_NPC_HitVolume);
	}

	PEXChangeRenderer (ws->display, ws->rid,
		(pexBitmask) PEXRDPickInclusion, (CARD32) sizeof (pexNameSet),
		(char *) &(dev->filter.incl));

	PEXChangeRenderer (ws->display, ws->rid,
		(pexBitmask) PEXRDPickExclusion, (CARD32) sizeof (pexNameSet),
		(char *) &(dev->filter.excl));


	PEXBeginPickOne (ws->display, ws->rid, ws->drawable_id, -1,
		PEXLast, pickDataBytes, &pickData);

	post_str = owsb->posted.highest.lower;
	end = &(owsb->posted.lowest);

	while (post_str != end) {
	    phg_wsb_traverse_net (ws, post_str->structh);
	    post_str = post_str->lower;
	}

	PEXEndPickOne (ws->display, ws->rid, &betterPick,
	    &pickPath, &pickDepth);
    }


    if (!pickDepth) {

    	pick->status = PIN_STATUS_NONE;
    	pick->pick_path.depth = 0;
    	pick->pick_path.path_list = (Ppick_path_elem *) NULL;

    } else {

	/*
	 * The protocol pick element ref data structure has its fields
	 * layed out in a different order than the PHIGS data structure.
	 * We must repack the data into PHIGS format.
	 */

	for (i = 1; i < pickDepth; i++) {
	    Ppick_path_elem *dst = (Ppick_path_elem *) &pickPath[i];
	    pexPickElementRef src;

	    src = pickPath[i];
	    dst->struct_id = src.sid;
	    dst->pick_id = src.pickid;
	    dst->elem_pos = src.offset;
	}

	/*
	 * order = bottom first?
	 */

	if (dev->order == PORDER_BOTTOM_FIRST) {
	    int			head, tail;
	    pexPickElementRef 	temp;

	    head = 1;
	    tail = pickDepth - 1;

	    for (i = 0; i < (pickDepth - 1) / 2; i++) {
		temp = pickPath[head];
		pickPath[head] = pickPath[tail];
		pickPath[tail] = temp;
		head++;
		tail--;
	    }
	}

	/*
	 * return status and pick path
	 */

	pick->status = PIN_STATUS_OK;
	pick->pick_path.depth = pickDepth - 1;
	pick->pick_path.path_list = (Ppick_path_elem *) &(pickPath[1]);
    }

    return 1;
}

void phg_wsb_inq_filter(
    Ws *ws,
    Phg_args_flt_type type,
    Phg_ret *ret
    )
{
    phg_wsx_inq_name_set( ws, type, (Pint)0, ret );
}

void phg_wsb_drawable_pick(
    Ws *ws,
    Phg_args_drawable_pick *args,
    Phg_ret *ret
    )
{
    ret->err = ERRN500;
    ERR_BUF( ws->erh, ret->err );
    ret->data.drawable_pick.status = PIN_STATUS_NO_IN;
    ret->data.drawable_pick.pick.depth = 0;
    ret->data.drawable_pick.pick.path_list = (Ppick_path_elem *)NULL;
}

void phg_wsb_map_points(
    Ws *ws,
    Phg_args_map_points *args,
    Phg_ret *ret
    )
{
    Ppoint3 *dc_pts;

    register int	i;

    ret->err = 0;
    ret->data.map_points.view_index = 0;
    ret->data.map_points.points.num_points = 0;
    ret->data.map_points.points.points = (Ppoint3 *)NULL;

    /* Allocate space for both the DC and WC points. */
    if ( !PHG_SCRATCH_SPACE( &ws->scratch,
	    (unsigned)(args->points.num_points
		* (sizeof(Ppoint) + sizeof(Ppoint3))) ) ) {
	ERR_BUF( ws->erh, ERR900 );
	return;
    }
    dc_pts = (pexDeviceCoord *)ws->scratch.buf;
    ret->data.map_points.points.points =
	(Ppoint3 *)(dc_pts + args->points.num_points);

    /* Convert the points to DC. */
    phg_wsx_update_ws_rect( ws );
    for ( i = 0; i < args->points.num_points; i++ ) {
	/* Z coord is already in DC. */
	WS_DRWBL_TO_DC2(ws, &args->points.points[i], &dc_pts[i]);
	dc_pts[i].z = args->points.points[i].z;
    }

    /* Convert the DC points to WC. */
    if ( !phg_wsb_resolve_stroke( ws, args->points.num_points, dc_pts, 0,
	    &ret->data.map_points.view_index,
	    &ret->data.map_points.points ) ) {
	ret->data.map_points.points.num_points = 0;
    }
}

void phg_wsb_redraw_regions(
    Ws *ws,
    Phg_args_redraw_regions *args
    )
{
    pexDeviceRect	*pex_rects;
    pexBitmask		rmask;
    CARD32		*card32_p;

    if ( args->num_regions <= 0 )
	return;

    if ( !(pex_rects = (pexDeviceRect *)PHG_SCRATCH_SPACE( &ws->scratch,
	    sizeof(CARD32) +
	    (unsigned)(args->num_regions * sizeof(*pex_rects)) )) ) {
	ERR_BUF( ws->erh, ERR900 );
	return;
    }
    card32_p = (CARD32 *)ws->scratch.buf;
    pex_rects = (pexDeviceRect *)(card32_p + 1);

    *card32_p = args->num_regions;
    phg_wsx_convert_rects( ws, args->num_regions, args->regions, pex_rects );
    rmask = PEXRDClipList;
    (void)PEXChangeRenderer( ws->display, ws->rid, rmask,
	(CARD32)(sizeof(CARD32) + args->num_regions * sizeof(pexDeviceRect)),
	(char *)card32_p );
    (*ws->repaint_all)( ws, PFLAG_COND, args->num_regions, args->regions );

    /* Reset the renderer's clip list. */ 
    *card32_p = 0;
    (void)PEXChangeRenderer( ws->display, ws->rid, rmask,(CARD32)sizeof(CARD32),
	(char *)card32_p );
}
#endif

