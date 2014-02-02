/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2014 Surplus Users Ham Society
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

#ifndef _phg_h
#define _phg_h

#include "phigs.h"
#include "phgtype.h"
#include "err.h"
#include "phgargs.h"
#include "phgretdata.h"
#include "util.h"
#include "errnum.h"
#include "css.h"
#include "attr.h"
#include "ws_inp.h"
#include "ws.h"
#include "ws_type.h"
#include "phigsfunc.h"
#include "psl.h"
#include "phg_dt.h"
#include "sin.h"
#include "private/evtP.h"
#include "ar.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PHG_NUM_EVENTS         35

typedef struct {
   Psl_handle      psl;
   Err_handle      erh;
   Css_handle      css;
   Phg_sin_evt_tbl *evt_table;
   Input_q_handle  input_q;
   List            wst_list;
   Ws_handle       *ws_list;
   Ar_handle       ar_list;
   Phg_scratch     scratch;
} Phg_struct;

typedef Phg_struct *Phg_handle;

extern Phg_handle phg;

#define PHG_ERH (phg->erh)
#define PHG_PSL (phg->psl)
#define PHG_CSS (phg->css)
#define PHG_EVT_TABLE (phg->evt_table)
#define PHG_INPUT_Q (phg->input_q)
#define PHG_WST_LIST (phg->wst_list)
#define PHG_WS_LIST (phg->ws_list)
#define PHG_AR_LIST (phg->ar_list)
#define PHG_WSID(n) (phg->ws_list[(n)])
#define PHG_SCRATCH (phg->scratch)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _phg_h */

