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

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <phigs/phg.h>
#include <phigs/util/sem.h>

struct _Sem {
   pthread_mutex_t mutex;
};

/*******************************************************************************
 * sem_create
 *
 * RETURNS: Pointer to semaphore or NULL
 */

Sem_handle sem_create(
   void
   )
{
   Sem_handle semh;
   semh = (Sem_handle) malloc(sizeof(struct _Sem));
   if (semh != NULL) {
      if (!sem_init(semh)) {
         free(semh);
         semh = NULL;
       }
    }

   return semh;
}

/*******************************************************************************
 * sem_init
 *
 * RETURNS: TRUE or FALSE
 */

int sem_init(
   Sem_handle semh
   )
{
   int status;

   if (pthread_mutex_init(&semh->mutex, NULL) == 0) {
      status = TRUE;
   }
   else {
      status = FALSE;
   }

   return status;
}

/*******************************************************************************
 * sem_deinit
 *
 * RETURNS: TRUE or FALSE
 */

int sem_deinit(
   Sem_handle semh
   )
{
   int status;

   if (pthread_mutex_destroy(&semh->mutex) == 0) {
      status = TRUE;
   }
   else {
      status = FALSE;
   }

    return status;
}

/*******************************************************************************
 * sem_destroy
 *
 * RETURNS: TRUE or FALSE
 */

int sem_destroy(
   Sem_handle semh
   )
{
   int status;

   if (!sem_deinit(semh)) {
      status = FALSE;
   }
   else {
      free(semh);
      status = TRUE;
   }

   return status;
}

/*******************************************************************************
 * sem_lock
 *
 * RETURNS: TRUE or FALSE
 */

int sem_lock(
   Sem_handle semh
   )
{
   int status;

   if (pthread_mutex_lock(&semh->mutex) == 0) {
      status = TRUE;
   }
   else {
      status = FALSE;
   }

   return status;
}

/*******************************************************************************
 * sem_unlock
 *
 * RETURNS: TRUE or FALSE
 */

int sem_unlock(
   Sem_handle semh
   )
{
   int status;

   if (pthread_mutex_unlock(&semh->mutex) == 0) {
      status = TRUE;
   }
   else {
      status = FALSE;
   }

   return status;
}
