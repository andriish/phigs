#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/css.h>
#include <phigs/ws.h>

Ws_handle ws;

/*******************************************************************************
 * popen_ws
 *
 * DESCR:	Open workstation
 * RETURNS:	N/A
 */

void popen_ws(Pint ws_id, void *conn_id, Pint ws_type)
{
   Err_handle erh;
   Css_ssh_type type;
   Phg_args_open_ws args;
   Phg_ret ret;

   if (ws_id < 0 || ws_id > MAX_NO_OPEN_WS) {
      fprintf(stderr, "Error invalid workstation: %d\n", ws_id);
      return;
   }

   if (ws_list[ws_id] == NULL) {
      ws_list[ws_id] = phg_wsb_open_ws(&args, &ret);
      if (ws_list[ws_id] == NULL) {
         fprintf(stderr, "Error unable to create workstation: %d\n", ws_id);
         return;
      }
      ws_list[ws_id]->out_ws.model.b.cssh = css;
      ws_list[ws_id]->id = ws_id;
   }

   ws = ws_list[ws_id];
}

/*******************************************************************************
 * ppost_struct
 *
 * DESCR:	Post structure to workstation
 * RETURNS:	N/A
 */

void ppost_struct(Pint ws_id, Pint struct_id, Pfloat priority)
{
   int status;
   Struct_handle structp;
   Css_handle cssh = ws_list[ws_id]->out_ws.model.b.cssh;

   structp = CSS_STRUCT_EXISTS(cssh, struct_id);
   if (structp != NULL) {
      (*ws_list[ws_id]->post) (ws_list[ws_id], structp, priority, 1);
      phg_css_post(cssh, struct_id, ws_list[ws_id], &status);
   }

   phg_init_default_attr();
}

