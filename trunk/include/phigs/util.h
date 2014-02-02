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

#ifndef _util_h
#define _util_h

#include <sys/types.h>

#include "util/list.h"
#include "util/stk.h"
#include "util/htab.h"
#include "util/mat.h"
#include "util/nset.h"
#include "util/conv.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * phg_grow_scratch
 *
 * DESCR:       Increase scratch buffer memory size
 * RETURNS:     Pointer to memory or NULL
 */

caddr_t phg_grow_scratch(
   Phg_scratch *sc,
   unsigned size
   );

/*******************************************************************************
 * phg_msleep
 *
 * DESCR:       Sleep in msecs granularity
 * RETURNS:     TRUE or FALSE
 */

int phg_msleep(
   unsigned msecs
   );

/*******************************************************************************
 * phg_mtime
 *
 * DESCR:       Get current time in milleseconds
 * RETURNS:     TRUE or FALSE
 */

int phg_mtime(
   time_t *tm_val
   );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _util_h */

