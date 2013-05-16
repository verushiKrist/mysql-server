/*****************************************************************************

Copyright (c) 1995, 2013, Oracle and/or its affiliates. All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Suite 500, Boston, MA 02110-1335 USA

*****************************************************************************/

/**************************************************//**
@file include/sync0arr.h
The wait array used in synchronization primitives

Created 9/5/1995 Heikki Tuuri
*******************************************************/

#ifndef sync0arr_h
#define sync0arr_h

#include "univ.i"
#include "ut0lst.h"
#include "ut0mem.h"
#include "os0thread.h"

/** Synchronization wait array cell */
struct sync_cell_t;
/** Synchronization wait array */
struct sync_array_t;

/******************************************************************//**
Reserves a wait array cell for waiting for an object.
The event of the cell is reset to nonsignalled state. */

void
sync_array_reserve_cell(
/*====================*/
	sync_array_t*	arr,	/*!< in: wait array */
	void*		object, /*!< in: pointer to the object to wait for */
	ulint		type,	/*!< in: lock request type */
	const char*	file,	/*!< in: file where requested */
	ulint		line,	/*!< in: line where requested */
	ulint*		index); /*!< out: index of the reserved cell */
/******************************************************************//**
This function should be called when a thread starts to wait on
a wait array cell. In the debug version this function checks
if the wait for a semaphore will result in a deadlock, in which
case prints info and asserts. */

void
sync_array_wait_event(
/*==================*/
	sync_array_t*	arr,	/*!< in: wait array */
	ulint		index);	 /*!< in: index of the reserved cell */
/******************************************************************//**
Frees the cell. NOTE! sync_array_wait_event frees the cell
automatically! */

void
sync_array_free_cell(
/*=================*/
	sync_array_t*	arr,	/*!< in: wait array */
	ulint		index);	/*!< in: index of the cell in array */
/**********************************************************************//**
Note that one of the wait objects was signalled. */

void
sync_array_object_signalled(void);
/*=============================*/

/**********************************************************************//**
If the wakeup algorithm does not work perfectly at semaphore relases,
this function will do the waking (see the comment in mutex_exit). This
function should be called about every 1 second in the server. */

void
sync_arr_wake_threads_if_sema_free(void);
/*====================================*/
/**********************************************************************//**
Prints warnings of long semaphore waits to stderr.
@return	TRUE if fatal semaphore wait threshold was exceeded */

ibool
sync_array_print_long_waits(
/*========================*/
	os_thread_id_t*	waiter,	/*!< out: longest waiting thread */
	const void**	sema)	/*!< out: longest-waited-for semaphore */
	__attribute__((nonnull));
/********************************************************************//**
Validates the integrity of the wait array. Checks
that the number of reserved cells equals the count variable. */

void
sync_array_validate(
/*================*/
	sync_array_t*	arr);	/*!< in: sync wait array */
/**********************************************************************//**
Prints info of the wait array. */

void
sync_array_print(
/*=============*/
	FILE*		file);	/*!< in: file where to print */

/**********************************************************************//**
Create the primary system wait array(s), they are protected by an OS mutex */

void
sync_array_init(
/*============*/
	ulint		n_threads);	/*!< in: Number of slots to create */
/**********************************************************************//**
Close sync array wait sub-system. */

void
sync_array_close(void);
/*==================*/

/**********************************************************************//**
Get an instance of the sync wait array. */

sync_array_t*
sync_array_get(void);
/*================*/

#ifndef UNIV_NONINL
#include "sync0arr.ic"
#endif

#endif
