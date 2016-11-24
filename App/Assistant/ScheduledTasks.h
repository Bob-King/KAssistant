/**
 * Name:		ScheduledTasks.h
 * Purpose:
 * Author:		King
 * Company:
 * Created:		04/16/2011 17:25:46
 * Copyright:
 */

#ifndef _SCHEDULEDTASKS_H_
#define _SCHEDULEDTASKS_H_


typedef KRESULT (*Do_Function)( void *self );

#define TASK_FUNCTIONS( )	KOBJECT_FUNCTIONS( )	\
							Do_Function fDo;

typedef struct _Task_VTable
{
	TASK_FUNCTIONS( )
} TASK_VTABLE, *LPTASK_VTABLE;

typedef struct _Task
{
	const TASK_VTABLE *pVTable;
} TASK;


EXTERN_C TASK *CreateBreakTask( );


#endif // _SCHEDULEDTASKS_H_
