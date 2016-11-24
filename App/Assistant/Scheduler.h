/**
 * Name:		Scheduler.h
 * Purpose:
 * Author:		King
 * Company:
 * Created:		04/11/2011 22:01:01
 * Copyright:
 */

#pragma once


#include "ScheduledTasks.h"


#define MAX_TASKS	(10)


EXTERN_C BOOL Scheduler_Init( );


EXTERN_C HANDLE Scheduler_Create( );
EXTERN_C void Scheduler_Destroy( HANDLE self );

EXTERN_C BOOL Scheduler_Start( HANDLE self );
EXTERN_C BOOL Scheduler_Stop( HANDLE self );

/**
 * Add a task for scheduling
 *
 * @param	self
 * @param	hTask		task handle
 * @param	interval	schedule interval, by minute
 * @return				if succeeds, it returns the index of the task which can be used to delete the task.
 * 						otherwise, it returns -1
 */
EXTERN_C int Scheduler_AddTask( HANDLE self, void *hTask, U32 interval );

EXTERN_C BOOL Scheduler_RemoveTask( HANDLE self, int index );
