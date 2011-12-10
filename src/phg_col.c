#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <phigs/phg.h>
#include <phigs/ws.h>
#include <phigs/private/phgP.h>

void pset_colr_rep(Pint ws, Pint index, Pcolr_rep *rep)
{
   Ws *wsh = ws_list[ws];

   memcpy(&wsh->colr_table[index], &rep->rgb, sizeof(Prgb));
}

