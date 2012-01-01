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
    ws->set_hlhsr_mode = phg_wsb_set_hlhsr_mode;
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
    ws->inq_posted = phg_wsb_inq_posted;
    ws->inq_representation = phg_wsb_inq_rep;
    ws->inq_disp_update_state = phg_wsb_inq_disp_update_state;
    ws->inq_hlhsr_mode = phg_wsb_inq_hlhsr_mode;
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

static int init_output_state(
    Ws *ws
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;

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

static int init_attributes(
    Ws *ws
    )
{
    Phg_args_rep_data rep;
    Pgcolr fg;

    Pline_bundle_plus   lnrep    = { PLINE_SOLID, 1.0,  fg };
    Pmarker_bundle_plus mkrep    = { PMARKER_DOT, 1.0, fg };
    Ptext_bundle_plus   txrep    = { 0, fg };
    Pedge_bundle_plus   edgerep  = { PEDGE_OFF, PLINE_SOLID, 1.0, fg };
    Pint_bundle_plus    interrep = { PSTYLE_SOLID, 1, fg };

    if (ws->current_colour_model == PINDIRECT) {
        fg.type = PINDIRECT;
        fg.val.ind = 1;
    }
    else {
        fg.type = PMODEL_RGB;
        fg.val.general.x = 1.0;
        fg.val.general.y = 1.0;
        fg.val.general.z = 1.0;
    }

    memcpy(&lnrep.colr, &fg, sizeof(Pgcolr));
    memcpy(&mkrep.colr, &fg, sizeof(Pgcolr));
    memcpy(&edgerep.colr, &fg, sizeof(Pgcolr));
    memcpy(&interrep.colr, &fg, sizeof(Pgcolr));

    /* Line representation */
    rep.index = 0;
    memcpy(&rep.bundl.lnrep, &lnrep, sizeof(Pline_bundle_plus));
    (*ws->set_rep)(ws, PHG_ARGS_EXTLNREP, &rep);

    /* Marker representation */
    rep.index = 0;
    memcpy(&rep.bundl.mkrep, &mkrep, sizeof(Pmarker_bundle_plus));
    (*ws->set_rep)(ws, PHG_ARGS_EXTMKREP, &rep);

    /* Text representation */
    rep.index = 0;
    memcpy(&rep.bundl.txrep, &txrep, sizeof(Ptext_bundle_plus));
    (*ws->set_rep)(ws, PHG_ARGS_EXTTXREP, &rep);

    /* Edge representation */
    rep.index = 0;
    memcpy(&rep.bundl.edgerep, &edgerep, sizeof(Pedge_bundle_plus));
    (*ws->set_rep)(ws, PHG_ARGS_EXTEDGEREP, &rep);

    /* Interiour representation */
    rep.index = 0;
    memcpy(&rep.bundl.interrep, &interrep, sizeof(Pint_bundle_plus));
    (*ws->set_rep)(ws, PHG_ARGS_EXTINTERREP, &rep);

    return 1;
}

int init_viewrep(
    Ws *ws
    )
{
    Phg_args_rep_data rep;
    int i, max_pd_view;

    max_pd_view = WS_MIN_PREDEF_VIEW_REPS - 1;

    for (i = 0; i <= max_pd_view; i++) {
        rep.index = i;
        memcpy(&rep.bundl.viewrep, &default_views[i], sizeof(Pview_rep3));
        (*ws->set_rep)(ws, PHG_ARGS_VIEWREP, &rep);
    }

    return 1;
}

static void init_view_refs(
    Ws *ws
    )
{
    Ws_output_ws  *ows = &ws->out_ws;
    Wsb_output_ws *owsb = &ows->model.b;

    list_init(&owsb->view_refs);
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
    if (!phg_wsb_create_LUTs(ws))
        goto abort;

    wsb_load_funcs( ws );

    if ( ! init_attributes( ws ) )
        goto abort;

    init_view_refs( ws );

    if ( ! init_viewrep( ws ) )
        goto abort;

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
    }

    /* Other pending data */
    if ( owsb->hlhsr_mode_pending == PUPD_PEND) {
#ifdef DEBUG
         printf("wsb: Set hlhsr mode\n");
#endif
	 owsb->cur_hlhsr_mode = owsb->req_hlhsr_mode;
	 owsb->hlhsr_mode_pending = PUPD_NOT_PEND;
         wsgl_set_hlhsr_mode(ws, owsb->req_hlhsr_mode);
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

static int phg_view_ref_add(
    Ws *ws,
    Pint id,
    Pint priority,
    Pview_rep3 *vrep
    )
{
    Ws_view_ref *vref, *vi;
    Ws_output_ws  *ows = &ws->out_ws;
    Wsb_output_ws *owsb = &ows->model.b;

    vref = (Ws_view_ref *) malloc(sizeof(Ws_view_ref));
    if (vref == NULL)
        return 0;

    vref->id = id;
    vref->priority = priority;
    vref->viewrep = vrep;

    /* Check if view already is in list */
    for (vi = (Ws_view_ref *) LIST_HEAD(&owsb->view_refs);
         (vi != NULL) && (vi->id != id);
         vi = (Ws_view_ref *) NODE_NEXT(&vi->node))
        ;

    /* If node already was in list, delete it */
    if (vi != NULL) {
#ifdef DEBUG
        printf("View reference %d was already in list, deleted.\n", id);
#endif
        list_remove(&owsb->view_refs, &vi->node);
    }

    /* Enqueue new node into list based on priority */
    list_enqueue(&owsb->view_refs, &vref->node, priority);

#ifdef DEBUG
    for (vi = (Ws_view_ref *) LIST_HEAD(&owsb->view_refs);
         vi != NULL;
         vi = (Ws_view_ref *) NODE_NEXT(&vi->node)) {
        printf("View #%d, priority = %d\n", vi->id, vi->priority);
    }
#endif

    return 1;
}

void phg_wsb_set_rep(
    Ws *ws,
    Phg_args_rep_type type,
    Phg_args_rep_data *rep
    )
{
    Wsb_output_ws	*owsb = &ws->out_ws.model.b;
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
            phg_wsb_set_LUT_entry(ws, type, rep, NULL);
	    break;

	case PHG_ARGS_COREP:
	    /* Store in current colour model. */
	    gcolr.type = ws->current_colour_model;
	    gcolr.val.general.x = rep->bundl.corep.rgb.red;
	    gcolr.val.general.y = rep->bundl.corep.rgb.green;
	    gcolr.val.general.z = rep->bundl.corep.rgb.blue;
            phg_wsb_set_LUT_entry(ws, type, rep, &gcolr);
	    break;

	case PHG_ARGS_VIEWREP:
#ifdef DEBUG
            printf("Set view: %d\n", rep->index);
#endif
            phg_wsb_set_LUT_entry(ws, type, rep, NULL);
            if (!phg_view_ref_add(ws,
                                  rep->index,
                                  rep->index,
                                  &rep->bundl.viewrep)) {
	        ERR_BUF(ws->erh, ERR900);
            }
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

void
phg_wsb_inq_filter( ws, type, ret )
    Ws                  *ws;
    Phg_args_flt_type   type;
    Phg_ret             *ret;
{
    phg_wsx_inq_name_set( ws, type, (Pint)0, ret );
}

#endif

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
    Pview_rep3 vrep;

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
	    phg_wsb_inq_LUT_entry(ws, index, how, rep_type, ret, NULL, NULL);
	    break;

	case PHG_ARGS_COREP:
	    cb = &ret->data.rep.corep;

	    /* Need to convert to current colour model. */
            phg_wsb_inq_LUT_entry(ws, index, how, rep_type, ret, &gcolr, NULL);

            /* NOTE:
             * Convert to correct colour model here if needed
             */

	    cb->rgb.red = gcolr.val.general.x;
	    cb->rgb.green = gcolr.val.general.y;
	    cb->rgb.blue = gcolr.val.general.z;
	    break;

        case PHG_ARGS_VIEWREP:
            phg_wsb_inq_LUT_entry(ws, index, how, rep_type, ret, NULL, &vrep);
            memcpy(&ret->data.rep.viewrep, &vrep, sizeof(Pview_rep3));
            break;

	default:
            /* Default */
            break;
    }
}
