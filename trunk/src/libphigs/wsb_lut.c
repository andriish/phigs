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

#include <string.h>
#include <assert.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>
#include <phigs/private/wsglP.h>
#include <phigs/css.h>
#include <phigs/alloc.h>

/*******************************************************************************
 * phg_create_table
 *
 * DESCR:	Helper function to create a hash table
 * RETURNS:	Non-zero on success
 */

static int phg_create_table(
    Hash_table *htab,
    Pint *err,
    int hash_size
    )
{
    int status = 0;

    *htab = phg_htab_create(hash_size);
    if (*htab == NULL) {
        *err = ERR900;
    }
    else {
        status = 1;
    }

    return status;
}

#define HASH_SIZE 10

/*******************************************************************************
 * phg_wsb_create_LUTs
 *
 * DESCR:	Create workstation lookup tables
 * RETURNS:	Non-zero on success
 */

int phg_wsb_create_LUTs(
    Ws *ws
    )
{
    Pint err;
    int status = 1;
    Ws_output_ws *ows = &ws->out_ws;

    if (phg_create_table(&ows->htab.marker, &err, HASH_SIZE) == 0) {
        status = 0;
        goto end;
    }

    if (phg_create_table(&ows->htab.line, &err, HASH_SIZE) == 0) {
        status = 0;
        goto end;
    }

    if (phg_create_table(&ows->htab.text, &err, HASH_SIZE) == 0) {
        status = 0;
        goto end;
    }

    if (phg_create_table(&ows->htab.interiour, &err, HASH_SIZE) == 0) {
        status = 0;
        goto end;
    }

    if (phg_create_table(&ows->htab.edge, &err, HASH_SIZE) == 0) {
        status = 0;
        goto end;
    }

end:

    if (!status) {
       phg_wsb_destroy_LUTs(ws);
    }

    return status;
}

/*******************************************************************************
 * phg_wsb_destroy_LUTs
 *
 * DESCR:	Destroy workstation lookup tables
 * RETURNS:	N/A
 */

void phg_wsb_destroy_LUTs(
    Ws *ws
    )
{
    Ws_output_ws *ows = &ws->out_ws;

    if (ows->htab.marker)
       phg_htab_destroy(ows->htab.marker, (void(*)())NULL);
    if (ows->htab.line)
       phg_htab_destroy(ows->htab.line, (void(*)())NULL);
    if (ows->htab.text)
       phg_htab_destroy(ows->htab.text, (void(*)())NULL);
    if (ows->htab.interiour)
       phg_htab_destroy(ows->htab.interiour, (void(*)())NULL);
    if (ows->htab.edge)
       phg_htab_destroy(ows->htab.edge, (void(*)())NULL);
}

/*******************************************************************************
 * phg_wsb_set_LUT_entry
 *
 * DESCR:	Set workstation table entry
 * RETURNS:	N/A
 */

void phg_wsb_set_LUT_entry(
    Ws *ws,
    Phg_args_rep_type type,
    Phg_args_rep_data *rep,
    Pgcolr *gcolr
    )
{
    caddr_t data;
    Ws_output_ws *ows = &ws->out_ws;

    switch(type) {
        case PHG_ARGS_LNREP:
            data = malloc(sizeof(Pline_bundle_plus));
            if (data != NULL) {
                ((Pline_bundle_plus *) data)->type = rep->bundl.lnrep.type;
                ((Pline_bundle_plus *) data)->width = rep->bundl.lnrep.width;
                ((Pline_bundle_plus *) data)->colr.type = PINDIRECT;
                ((Pline_bundle_plus *) data)->colr.val.ind =
                    rep->bundl.lnrep.colr_ind;
                if (!phg_htab_add_entry(ows->htab.line, rep->index, data)) {
                    ERR_BUF(ws->erh, ERR900);
                }
            }
            else {
                ERR_BUF(ws->erh, ERR900);
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 * phg_wsb_inq_LUT_entry
 *
 * DESCR:	Get workstation table entry
 * RETURNS:	N/A
 */

void phg_wsb_inq_LUT_entry(
    Ws *ws,
    Pint index,
    Pinq_type type,
    Phg_args_rep_type rep_type,
    Phg_ret *ret,
    Pgcolr *gcolr,
    Pview_rep3 *vrep
    )
{
    caddr_t data;
    Phg_ret_rep *rep = &ret->data.rep;
    Ws_output_ws *ows = &ws->out_ws;

    switch(rep_type) {
        case PHG_ARGS_LNREP:
        case PHG_ARGS_EXTLNREP:
            if (!phg_htab_get_entry(ows->htab.line, index, &data)) {
                ret->err = ERR101;
            }
            else {
                memcpy(&rep->extlnrep, data, sizeof(Pline_bundle_plus));
                ret->err = 0;
            }
            break;

        default:
            break;
    }
}

