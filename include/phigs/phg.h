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

#ifndef _phg_h
#define _phg_h

#include <phigs/phgtype.h>
#include <phigs/phigs.h>
#include <phigs/util.h>
#include <phigs/phgargs.h>
#include <phigs/phgretdata.h>
#include <phigs/err.h>
#include <phigs/errnum.h>
#include <phigs/css.h>
#include <phigs/attr.h>
#include <phigs/ws.h>
#include <phigs/ws_type.h>
#include <phigs/phigsfunc.h>
#include <phigs/psl.h>
#include <phigs/phg_dt.h>

typedef struct {
   Psl_handle psl;
   Err_handle erh;
   Css_handle css;
   List       wst_list;
   Ws_handle  *ws_list;
} Phg_struct;

typedef Phg_struct *Phg_handle;

extern Phg_handle phg;

#define PHG_ERH (phg->erh)
#define PHG_PSL (phg->psl)
#define PHG_CSS (phg->css)
#define PHG_WST_LIST (phg->wst_list)
#define PHG_WS_LIST (phg->ws_list)
#define PHG_WSID(n) (phg->ws_list[(n)])

#endif

