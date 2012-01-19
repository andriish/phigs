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

#ifndef _task_h
#define _task_h

struct _Task;
typedef struct _Task *Task_handle;

/*******************************************************************************
 * task_create
 *
 * RETURNS: Task handle or NULL
 */

Task_handle task_create(
   void* (*func)(void *),
   void *arg
   );

/*******************************************************************************
 * task_init
 *
 * RETURNS: TRUE or FALSE
 */

int task_init(
   Task_handle taskh,
   void* (*func)(void *),
   void *arg
   );

/*******************************************************************************
 * task_start
 *
 * RETURNS: TRUE or FALSE
 */

int task_start(
   Task_handle taskh
   );

/*******************************************************************************
 * task_spawn
 *
 * RETURNS: Task handle or NULL
 */

Task_handle task_spawn(
   void* (*func)(void *),
   void *arg
   );

/*******************************************************************************
 * task_destroy
 *
 * RETURNS: TRUE or FALSE
 */

int task_destroy(
   Task_handle taskh
   );

#endif /* _task_h */

