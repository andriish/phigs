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

#include <string.h>
#include <assert.h>
#include <phigs/phg.h>
#include <phigs/private/phgP.h>
#include <phigs/ws.h>
#include <phigs/private/wsglP.h>
#include <phigs/css.h>
#include <phigs/alloc.h>

#define HASH_SIZE 10

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
    int status;

    *htab = phg_htab_create(hash_size);
    if (*htab == NULL) {
        *err = ERR900;
        status = 0;
    }
    else {
        status = 1;
    }

    return status;
}

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

    if (!phg_create_table(&ows->htab.line, &err, HASH_SIZE)) {
        status = 0;
        goto end;
    }

    if (!phg_create_table(&ows->htab.marker, &err, HASH_SIZE)) {
        status = 0;
        goto end;
    }

    if (!phg_create_table(&ows->htab.text, &err, HASH_SIZE)) {
        status = 0;
        goto end;
    }

    if (!phg_create_table(&ows->htab.interiour, &err, HASH_SIZE)) {
        status = 0;
        goto end;
    }

    if (!phg_create_table(&ows->htab.edge, &err, HASH_SIZE)) {
        status = 0;
        goto end;
    }

    if (!phg_create_table(&ows->htab.colour, &err, 100)) {
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

    if (ows->htab.line)
       phg_htab_destroy(ows->htab.line, (void(*)())NULL);
    if (ows->htab.marker)
       phg_htab_destroy(ows->htab.marker, (void(*)())NULL);
    if (ows->htab.text)
       phg_htab_destroy(ows->htab.text, (void(*)())NULL);
    if (ows->htab.interiour)
       phg_htab_destroy(ows->htab.interiour, (void(*)())NULL);
    if (ows->htab.edge)
       phg_htab_destroy(ows->htab.edge, (void(*)())NULL);
    if (ows->htab.colour)
       phg_htab_destroy(ows->htab.colour, (void(*)())NULL);
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
#ifdef DEBUG
            printf("Set lnrep %d\n", rep->index);
#endif
            data = malloc(sizeof(Pline_bundle_plus));
            if (data != NULL) {
                memset(data, 0, sizeof(Pline_bundle_plus));
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

        case PHG_ARGS_EXTLNREP:
#ifdef DEBUG
            printf("Set extlnrep: %d\n", rep->index);
#endif
            data = malloc(sizeof(Pline_bundle_plus));
            if (data != NULL) {
                memcpy(data, &rep->bundl.extlnrep, sizeof(Pline_bundle_plus));
                if (!phg_htab_add_entry(ows->htab.line, rep->index, data)) {
                    ERR_BUF(ws->erh, ERR900);
                }
            }
            else {
                ERR_BUF(ws->erh, ERR900);
            }
            break;

        case PHG_ARGS_MKREP:
#ifdef DEBUG
            printf("Set mkrep: %d\n", rep->index);
#endif
            data = malloc(sizeof(Pmarker_bundle_plus));
            if (data != NULL) {
                memset(data, 0, sizeof(Pmarker_bundle_plus));
                ((Pmarker_bundle_plus *) data)->type = rep->bundl.mkrep.type;
                ((Pmarker_bundle_plus *) data)->size = rep->bundl.mkrep.size;
                ((Pmarker_bundle_plus *) data)->colr.type = PINDIRECT;
                ((Pmarker_bundle_plus *) data)->colr.val.ind =
                    rep->bundl.mkrep.colr_ind;
                if (!phg_htab_add_entry(ows->htab.marker, rep->index, data)) {
                    ERR_BUF(ws->erh, ERR900);
                }
            }
            else {
                ERR_BUF(ws->erh, ERR900);
            }
            break;

        case PHG_ARGS_EXTMKREP:
#ifdef DEBUG
            printf("Set extmkrep: %d\n", rep->index);
#endif
            data = malloc(sizeof(Pmarker_bundle_plus));
            if (data != NULL) {
                memcpy(data, &rep->bundl.extmkrep, sizeof(Pmarker_bundle_plus));
                if (!phg_htab_add_entry(ows->htab.marker, rep->index, data)) {
                    ERR_BUF(ws->erh, ERR900);
                }
            }
            else {
                ERR_BUF(ws->erh, ERR900);
            }
            break;

        case PHG_ARGS_TXREP:
#ifdef DEBUG
            printf("Set txrep: %d\n", rep->index);
#endif
            data = malloc(sizeof(Ptext_bundle_plus));
            if (data != NULL) {
                memset(data, 0, sizeof(Ptext_bundle_plus));
                ((Ptext_bundle_plus *) data)->font = rep->bundl.txrep.font;
                ((Ptext_bundle_plus *) data)->colr.type = PINDIRECT;
                ((Ptext_bundle_plus *) data)->colr.val.ind =
                    rep->bundl.txrep.colr_ind;
                if (!phg_htab_add_entry(ows->htab.text, rep->index, data)) {
                    ERR_BUF(ws->erh, ERR900);
                }
            }
            else {
                ERR_BUF(ws->erh, ERR900);
            }
            break;

        case PHG_ARGS_EXTTXREP:
#ifdef DEBUG
            printf("Set exttxrep: %d\n", rep->index);
#endif
            data = malloc(sizeof(Ptext_bundle_plus));
            if (data != NULL) {
                memcpy(data, &rep->bundl.exttxrep, sizeof(Ptext_bundle_plus));
                if (!phg_htab_add_entry(ows->htab.text, rep->index, data)) {
                    ERR_BUF(ws->erh, ERR900);
                }
            }
            else {
                ERR_BUF(ws->erh, ERR900);
            }
            break;

        case PHG_ARGS_INTERREP:
#ifdef DEBUG
            printf("Set interrep: %d\n", rep->index);
#endif
            data = malloc(sizeof(Pint_bundle_plus));
            if (data != NULL) {
                memset(data, 0, sizeof(Pint_bundle_plus));
                ((Pint_bundle_plus *) data)->style = rep->bundl.interrep.style;
                ((Pint_bundle_plus *) data)->style_ind =
                    rep->bundl.interrep.style_ind;
                ((Pint_bundle_plus *) data)->colr.type = PINDIRECT;
                ((Pint_bundle_plus *) data)->colr.val.ind =
                    rep->bundl.interrep.colr_ind;
                if (!phg_htab_add_entry(ows->htab.interiour,
                                        rep->index,
                                        data)) {
                    ERR_BUF(ws->erh, ERR900);
                }
            }
            else {
                ERR_BUF(ws->erh, ERR900);
            }
            break;

        case PHG_ARGS_EXTINTERREP:
#ifdef DEBUG
            printf("Set extinterrep: %d\n", rep->index);
#endif
            data = malloc(sizeof(Pint_bundle_plus));
            if (data != NULL) {
                memcpy(data,
                       &rep->bundl.extinterrep,
                       sizeof(Pint_bundle_plus));
                if (!phg_htab_add_entry(ows->htab.interiour,
                                        rep->index,
                                        data)) {
                    ERR_BUF(ws->erh, ERR900);
                }
            }
            else {
                ERR_BUF(ws->erh, ERR900);
            }
            break;

        case PHG_ARGS_EDGEREP:
#ifdef DEBUG
            printf("Set edgerep: %d\n", rep->index);
#endif
            data = malloc(sizeof(Pedge_bundle_plus));
            if (data != NULL) {
                memset(data, 0, sizeof(Pedge_bundle_plus));
                ((Pedge_bundle_plus *) data)->flag = rep->bundl.edgerep.flag;
                ((Pedge_bundle_plus *) data)->type = rep->bundl.edgerep.type;
                ((Pedge_bundle_plus *) data)->width = rep->bundl.edgerep.width;
                ((Pedge_bundle_plus *) data)->colr.type = PINDIRECT;
                ((Pedge_bundle_plus *) data)->colr.val.ind =
                    rep->bundl.edgerep.colr_ind;
                if (!phg_htab_add_entry(ows->htab.edge, rep->index, data)) {
                    ERR_BUF(ws->erh, ERR900);
                }
            }
            else {
                ERR_BUF(ws->erh, ERR900);
            }
            break;

        case PHG_ARGS_EXTEDGEREP:
#ifdef DEBUG
            printf("Set extedgerep: %d\n", rep->index);
#endif
            data = malloc(sizeof(Pedge_bundle_plus));
            if (data != NULL) {
                memcpy(data, &rep->bundl.extedgerep, sizeof(Pedge_bundle_plus));
                if (!phg_htab_add_entry(ows->htab.edge, rep->index, data)) {
                    ERR_BUF(ws->erh, ERR900);
                }
            }
            else {
                ERR_BUF(ws->erh, ERR900);
            }
            break;

        case PHG_ARGS_COREP:
#ifdef DEBUG
            printf("Set corep: %d\n", rep->index);
#endif
            data = malloc(sizeof(Pgcolr));
            if (data != NULL) {
                memcpy(data, gcolr, sizeof(Pgcolr));
                if (!phg_htab_add_entry(ows->htab.colour, rep->index, data)) {
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
#ifdef DEBUG
            printf("Inq line\n");
#endif
            if (!phg_htab_get_entry(ows->htab.line, index, &data)) {
                ret->err = ERR101;
            }
            else {
                memcpy(&rep->extlnrep, data, sizeof(Pline_bundle_plus));
                ret->err = 0;
            }
            break;

        case PHG_ARGS_MKREP:
        case PHG_ARGS_EXTMKREP:
#ifdef DEBUG
            printf("Inq marker\n");
#endif
            if (!phg_htab_get_entry(ows->htab.marker, index, &data)) {
                ret->err = ERR101;
            }
            else {
                memcpy(&rep->extmkrep, data, sizeof(Pmarker_bundle_plus));
                ret->err = 0;
            }
            break;

        case PHG_ARGS_TXREP:
        case PHG_ARGS_EXTTXREP:
#ifdef DEBUG
            printf("Inq text\n");
#endif
            if (!phg_htab_get_entry(ows->htab.text, index, &data)) {
                ret->err = ERR101;
            }
            else {
                memcpy(&rep->exttxrep, data, sizeof(Ptext_bundle_plus));
                ret->err = 0;
            }
            break;

        case PHG_ARGS_INTERREP:
        case PHG_ARGS_EXTINTERREP:
#ifdef DEBUG
            printf("Inq interiour\n");
#endif
            if (!phg_htab_get_entry(ows->htab.interiour, index, &data)) {
                ret->err = ERR101;
            }
            else {
                memcpy(&rep->extinterrep, data, sizeof(Pint_bundle_plus));
                ret->err = 0;
            }
            break;

        case PHG_ARGS_EDGEREP:
        case PHG_ARGS_EXTEDGEREP:
#ifdef DEBUG
            printf("Inq edge\n");
#endif
            if (!phg_htab_get_entry(ows->htab.edge, index, &data)) {
                ret->err = ERR101;
            }
            else {
                memcpy(&rep->extedgerep, data, sizeof(Pedge_bundle_plus));
                ret->err = 0;
            }
            break;

        case PHG_ARGS_COREP:
#ifdef DEBUG
            printf("Inq colour\n");
#endif
            if (!phg_htab_get_entry(ows->htab.colour, index, &data)) {
                ret->err = ERR101;
            }
            else {
                memcpy(gcolr, data, sizeof(Pgcolr));
                ret->err = 0;
            }
            break;

        default:
            break;
    }
}

