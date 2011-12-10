#include <string.h>
#include <phigs/phg.h>
#include <phigs/css.h>
#include <phigs/private/phgP.h>
#include <phigs/private/cssP.h>
#include <phigs/mat_utils.h>
#include <phigs/ws.h>

/* Default attribute group */
static attribute_group default_attr = {
   /* Line bundle */
   {
      PLINE_SOLID,              /* Line type */
      1.0,                      /* Line width */
      0                         /* Line color index */
   },

   /* Marker bundle */
   {
      PMARKER_DOT,              /* Marker type */
      1.0,                      /* Marker size */
      0                         /* Marker color index */
   },

   /* Interiour bundle */
   {
      PSTYLE_HOLLOW,            /* Interiour style */
      0                         /* Interiour color index */
   },

   /* Edge bundle */
   {
      PEDGE_ON,                 /* Edge flag */
      PLINE_SOLID,              /* Edge type */
      1.0,                      /* Edge width */
      0                         /* Edge color index */
   },

   /* Text bundle */
   {
      0,                        /* Text font */
      0                         /* Text color index */
   }
};

/*******************************************************************************
 * phg_init_default_attr
 *
 * DESCR:	Completes initialization of default attribute colors
 * 		Can't assign colors until after X11 has been init'ed
 * RETURNS:	N/A
 */

void phg_init_default_attr(void)
{
   default_attr.line_bundle.colr_ind     = ws->black_color;
   default_attr.marker_bundle.colr_ind   = ws->black_color;
   default_attr.text_bundle.colr_ind     = ws->black_color;
   default_attr.edge_bundle.colr_ind     = ws->black_color;
   default_attr.int_bundle.colr_ind      = ws->white_color;
}

/*******************************************************************************
 * phg_get_default_attr
 *
 * DESCR:	Get default attribute group
 * 		Can't assign colors until after X11 has been init'ed
 * RETURNS:	N/A
 */

attribute_group* phg_get_default_attr(void)
{
   return &default_attr;
}

