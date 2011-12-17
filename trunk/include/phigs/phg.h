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

#ifndef _phg_h
#define _phg_h

#include <phigs/phgtype.h>
#include <phigs/util.h>
#include <phigs/phigs.h>
#include <phigs/phgargs.h>
#include <phigs/phgretdata.h>
#include <phigs/mat_utils.h>
#include <phigs/err.h>
#include <phigs/errnum.h>
#include <phigs/css.h>
#include <phigs/ws.h>
#include <phigs/phigsfunc.h>
#include <phigs/ws_type.h>
#include <phigs/psl.h>

#ifndef _ABS
#define _ABS(val) \
   ((val >= 0) ? (val) : (val) * -1)
#endif

extern Psl_handle psl;

#endif

