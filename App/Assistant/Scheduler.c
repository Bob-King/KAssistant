/**
 * Name:		Scheduler.c
 * Purpose:
 * Author:		King
 * Company:
 * Created:		04/11/2011 22:00:54
 * Copyright:
 */

#include "stdafx.h"


#include "Scheduler.h"
#include "MainApp.h"


#define SCHEDULER_WINDOW_CLASS	L"Scheduler"
#define TIMER_ID				(0x101)
#define ONE_MINUTE				(60 * 1000)
#define TICK_PER_MS( )			(1)


#define USER_WINDOW				0


struct _ScheduleEntry
{
	void *hTask;
	U32 interval;	// by tick
	U32 lastExecuted;	// by tick
};

struct _Scheduler
{
#if USER_WINDOW
	HWND hwnd;
	BOOL running;
#else
	HANDLE thread;
	volatile BOOL running;
	HANDLE exitEvent;
	CRITICAL_SECTION cs;
#endif
	struct _ScheduleEntry tasks[MAX_TASKS];
};


#if USER_WINDOW
static LRESULT CALLBACK SchedulerWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	struct _Scheduler *pScheduler;
	struct _ScheduleEntry *pScheduleEntry;
	struct _Task *pTask;
	int i;
	U32 tick;
	
	switch ( message )
	{
	case WM_TIMER:
		pScheduler = (struct _Scheduler *)GetWindowLongPtr( hWnd, GWLP_USERDATA );
		if ( pScheduler )
		{
			tick = GetTickCount( );
			for ( i = 0; i != MAX_TASKS; ++i )
			{
				pScheduleEntry = &pScheduler->tasks[i];
				if ( pScheduleEntry->hTask )
				{
					if ( tick >= pScheduleEntry->lastExecuted + pScheduleEntry->interval )
					{
						pTask = (struct _Task *)pScheduler->tasks[i].hTask;
						SAFE_OBJECT_VCALL( pTask, fDo );
						pScheduleEntry->lastExecuted = tick;
					}
				}
			}
		}
		break;

	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return 0;
}


static ATOM RegisterSchedulerWindowClass( HINSTANCE hInstance )
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof( WNDCLASSEXW );

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= SchedulerWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground	= ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= SCHEDULER_WINDOW_CLASS;
	wcex.hIconSm		= NULL; 

	return RegisterClassExW( &wcex );
}
#else
DWORD WINAPI Scheduler_ThreadProc( LPVOID lpParameter )
{
	BOOL running = TRUE;
	struct _Scheduler *pScheduler;
	struct _ScheduleEntry *pScheduleEntry;
	struct _Task *pTask;
	DWORD tick;
	int i;
	DWORD exitCode = 0;

	KING_CHECK_RETURN2( lpParameter, 1 );

	pScheduler = (struct _Scheduler *)lpParameter;

	do
	{
		switch ( WaitForSingleObject( pScheduler->exitEvent, ONE_MINUTE ) )
		{
		case WAIT_TIMEOUT:
			if ( !running )
				break;
			tick = GetTickCount( );
			for ( i = 0; i != MAX_TASKS; ++i )
			{
				pScheduleEntry = &pScheduler->tasks[i];
				if ( pScheduleEntry->hTask )
				{
					if ( tick >= pScheduleEntry->lastExecuted + pScheduleEntry->interval )
					{
						pTask = (struct _Task *)pScheduler->tasks[i].hTask;
						SAFE_OBJECT_VCALL( pTask, fDo );
						pScheduleEntry->lastExecuted = tick;
					}
				}
			}
			break;

		default:
			KING_ASSERTFAILURE_MESSAGE( "WaitForSingleObject failed in Scheduler thread!" );
			exitCode = 2;
		case WAIT_OBJECT_0:
			running = FALSE;
			break;
		}
	}
	while ( running );

	return exitCode;
}

#endif

BOOL Scheduler_Init( )
{
#if USER_WINDOW
	return RegisterSchedulerWindowClass( AGetInstance( ) );
#else
	return TRUE;
#endif
}


HANDLE Scheduler_Create( )
{
	struct _Scheduler *ret;

	ret = (struct _Scheduler *)KMAlloc( sizeof( struct _Scheduler ) );
	if ( !ret )
		return NULL;

#if USER_WINDOW

	ret->hwnd = CreateWindowEx( 0, SCHEDULER_WINDOW_CLASS, L"", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, HWND_MESSAGE, NULL, AGetInstance( ), ret );
	if ( !ret->hwnd )
	{
		ASSISTANT_LOG( LOG_ERROR, TEXT("Failed to create scheduler window.") );

		KMFree( ret );
		return NULL;
	}

	// SetWindowLongPtr( ret->hwnd, GWLP_USERDATA, ret );

#else

	ret->exitEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	if ( !ret->exitEvent )
	{
		KMFree( ret );
		return NULL;
	}

	ret->thread = CreateThread( NULL, 0, Scheduler_ThreadProc, ret, CREATE_SUSPENDED, NULL );
	if ( !ret->thread )
	{
		CloseHandle( ret->exitEvent );
		KMFree( ret );
		return NULL;
	}

	InitializeCriticalSection( &ret->cs );
	ret->running = TRUE;

	ResumeThread( ret->thread );

#endif

	return ret;
}


void Scheduler_Destroy( HANDLE self )
{
	struct _Scheduler *pThis;
	struct _ScheduleEntry *pScheduleEntry;
	int i;

	KING_CHECK_RETURN( self );

	Scheduler_Stop( self );

	pThis = (struct _Scheduler *)self;

#if !( USER_WINDOW )
	SetEvent ( pThis->exitEvent );

	WaitForSingleObject( pThis->thread , INFINITE );

	EnterCriticalSection( &pThis->cs );
#endif

	for ( i = 0; i != MAX_TASKS; ++i )
	{
		pScheduleEntry = &pThis->tasks[i];

		if ( pScheduleEntry->hTask )
		{
			SAFE_OBJECT_VCALL( ( (struct _Task *)pScheduleEntry->hTask ), fDestructor );
			KMFree( pScheduleEntry->hTask );
			pScheduleEntry->hTask = NULL;
		}
	}

#if !( USER_WINDOW )
	LeaveCriticalSection( &pThis->cs );

	DeleteCriticalSection( &pThis->cs );

	CloseHandle( pThis->exitEvent );
	CloseHandle( pThis->thread );
#else
	DestroyWindow( ((struct _Scheduler *)self)->hwnd );
#endif

	KMFree( self );
}


BOOL Scheduler_Start( HANDLE self )
{
	struct _Scheduler *pThis;

	KING_CHECK_RETURN2( self, FALSE );

	pThis = (struct _Scheduler *)self;

#if USER_WINDOW
	if ( !pThis->running )
	   pThis->running = SetTimer( pThis->hwnd, TIMER_ID, ONE_MINUTE, NULL );
#else
	pThis->running = TRUE;
#endif

	return pThis->running;
}


BOOL Scheduler_Stop( HANDLE self )
{
	struct _Scheduler *pThis;

	KING_CHECK_RETURN2( self, FALSE );

	pThis = (struct _Scheduler *)self;

#if USER_WINDOW
	if ( pThis->running )
	   pThis->running = !KillTimer( pThis->hwnd, TIMER_ID );
#else
	pThis->running = FALSE;
#endif

	return !pThis->running;
}


int Scheduler_AddTask( HANDLE self, void *hTask, U32 interval )
{
	struct _Scheduler *pThis;
	struct _ScheduleEntry *pScheduleEntry;
	int index;

	KING_CHECK_RETURN2( self, -1 );

	pThis = (struct _Scheduler *)self;
#if !( USER_WINDOW )
	EnterCriticalSection( &pThis->cs );
#endif
	for ( index = 0; MAX_TASKS != index; ++index )
	{
		if ( !pThis->tasks[index].hTask )
			break;
	}
#if !( USER_WINDOW )
	LeaveCriticalSection( &pThis->cs );
#endif

	if ( MAX_TASKS == index )
		return -1;

	pScheduleEntry = &pThis->tasks[index];
	pScheduleEntry->hTask = hTask;
	pScheduleEntry->interval = ( TICK_PER_MS( ) * 1000 * 60 ) * interval;
	pScheduleEntry->lastExecuted = GetTickCount( );

	return index;
}


BOOL Scheduler_RemoveTask( HANDLE self, int index )
{
	struct _Scheduler *pThis;
	struct _ScheduleEntry *pScheduleEntry;

	KING_CHECK_RETURN2( self && 0 <= index && MAX_TASKS > index, FALSE );

	pThis = (struct _Scheduler *)self;

#if !( USER_WINDOW )
	EnterCriticalSection( &pThis->cs );
#endif

	pScheduleEntry = &pThis->tasks[index];
	if ( pScheduleEntry->hTask )
	{
		SAFE_OBJECT_VCALL( ( (struct _Task *)pScheduleEntry->hTask ), fDestructor );
		KMFree( pScheduleEntry->hTask );
		pScheduleEntry->hTask = NULL;
	}

#if !( USER_WINDOW )
	LeaveCriticalSection( &pThis->cs );
#endif

	return TRUE;
}
