/**
 * Name:		ScheduledTasks.c
 * Purpose:
 * Author:		King
 * Company:
 * Created:		04/16/2011 17:25:50
 * Copyright:
 */

#include "stdafx.h"

#include "ScheduledTasks.h"


KRESULT BreakTask_Do( void *self )
{
	UNREFERENCED_PARAMETER( self );

	SendMessage ( GetDesktopWindow (), WM_SYSCOMMAND, SC_SCREENSAVE, -1 );

	return 0;
}


TASK *CreateBreakTask( )
{
	TASK *ret;

	static const struct _Task_VTable G_TASK_VTABLE =
	{
		NULL,
		BreakTask_Do,
	};

	
	ret = (TASK *)KMAlloc( sizeof ( TASK ) );
	if ( ret )
		VTableOf( ret ) = &G_TASK_VTABLE;

	return ret;
}
