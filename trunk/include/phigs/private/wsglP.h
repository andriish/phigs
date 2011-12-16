#ifndef _wsglP_h
#define _wsglP_h

#include <GL/glx.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>

typedef struct {
   GLXContext      glx_context;
   attribute_group attrs;
   Plimit3         curr_win;
   Pint            win_changed;
   Plimit3         curr_vp;
   Pint            vp_changed;
   Pmatrix3        win_tran;
   Pmatrix3        local_tran, total_tran;
   Pint            curr_view_index;
} Wsgl;

Ws* phg_wsgl_create(Phg_args_open_ws *args);

/*******************************************************************************
 * phg_wsgl_init
 *
 * DESCR:       Initialize renderer
 * RETURNS:     Non zero or zero on error
 */

int phg_wsgl_init(
   Ws *ws
   );

int phg_wsgl_open_window(Ws *ws);
void phg_wsgl_release_window(Ws *ws);
void phg_wsgl_destroy(Ws *ws);
void phg_wsgl_set_window(Ws *ws, Plimit3 *win);
void phg_wsgl_set_viewport(Ws *ws, Plimit3 *vp);
void phg_wsgl_clear(void);
void phg_wsgl_flush(Ws *ws);
void phg_wsgl_compute_ws_transform(Plimit3 *ws_win,
                                   Plimit3 *ws_vp,
                                   Ws_xform *ws_xform);
void phg_wsgl_begin_rendering(Ws *ws);
void phg_wsgl_end_rendering(void);
void phg_wsgl_render_element(Ws *ws, El_handle el);

#endif /* _wsglP_h */

