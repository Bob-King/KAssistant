/**
 * Name:		MainApp.c
 * Purpose:
 * Author:		King
 * Company:
 * Created:		04/05/2011 21:09:02
 * Copyright:
 */

#include "StdAfx.h"

#include "MainApp.h"
#include "Messages.h"
#include "TrayIcon.h"
#include "Scheduler.h"
#include "Configure.h"

#include "resource.h"


U32 g_appLogLevel = 0x7f;
LPKLOG g_pAppLog = NULL;

struct _MainApp g_mainApp;
LPKSTDPATH g_lpStdPath;

static const GUID UUID_TRAYICON = { 0xc67fcf16, 0x5339, 0x4f7d, 0xa9, 0x1a, 0xc1, 0xf5, 0x45, 0xb5, 0x87, 0x1f };
static const WCHAR MAIN_WINDOW_CLASS[] = L"Assistant";

static WCHAR gs_szTitle[MAX_LOADSTRING];


static LRESULT CALLBACK AssistantWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
static ATOM RegisterAssistantWindowClass( HINSTANCE hInstance );
static HMODULE LoadAppResource( );


static LRESULT CALLBACK AssistantTrayIconWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	HMENU hMenu;
	POINT pt;

	UNREFERENCED_PARAMETER( hWnd );
	UNREFERENCED_PARAMETER( message );
	UNREFERENCED_PARAMETER( wParam );

	switch ( LOWORD( lParam ) )
	{
	case WM_RBUTTONUP:
	case WM_CONTEXTMENU:
		hMenu = LoadMenu( AGetResourceInstance( ), MAKEINTRESOURCE( IDR_TRAYICONMENU ) );
		if ( hMenu )
		{
			CheckMenuItem( hMenu, IDM_AUTOSTART, IsAutoRun( ) ? MF_CHECKED : MF_UNCHECKED );
			GetCursorPos( &pt );
			TrackPopupMenu( GetSubMenu( hMenu, 0 ),
				GetSystemMetrics( SM_MENUDROPALIGNMENT ) | TPM_RIGHTBUTTON,
				pt.x,
				pt.y,
				0,
				hWnd,
				NULL );

			DestroyMenu( hMenu );
		}
		break;
	}

	return 0;
}



int APIENTRY wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	MSG msg;
	BOOL fRet;

	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );
	UNREFERENCED_PARAMETER( nCmdShow );

	KING_CHECK_RETURN2_MESSAGE( !CreateKernelLog( &g_pAppLog ), FALSE, "CreateKernelLog failed!\n" );

	KING_CHECK_RETURN2_MESSAGE( !CreateStdPath( NULL, APP_NAME, &g_lpStdPath ), FALSE, "CreateStdPath failed!\n" );

	ZeroMemory( &g_mainApp, sizeof( g_mainApp ) );

	KING_CHECK_RETURN2( RegisterAssistantWindowClass( hInstance ), FALSE );

	KING_CHECK_RETURN2_MESSAGE( NULL != ( AGetResourceInstance( ) = LoadAppResource( ) ), FALSE, "LoadAppResource failed!\n" );

	KING_CHECK_RETURN2( Scheduler_Init( ), FALSE );

	g_mainApp.hScheduler = Scheduler_Create( );

	KING_CHECK_RETURN2( LoadStringW( AGetResourceInstance( ), IDS_APP_TITLE, gs_szTitle, MAX_LOADSTRING ), FALSE );

	AGetMainWindow( ) = CreateWindowExW( 0, MAIN_WINDOW_CLASS, gs_szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL );
	KING_CHECK_RETURN2_MESSAGE( NULL != AGetMainWindow( ), FALSE, "Failed to create main window!\n" );

	AGetInstance( ) = hInstance;

	UpdateWindow( AGetMainWindow( ) );
	PostMessage( AGetMainWindow( ), WM_INITAPP, 0, 0 );

	while ( ( fRet = GetMessage( &msg, AGetMainWindow( ), 0, 0 ) ) != FALSE )
	{
		if ( -1 == fRet )
		{
			KING_LOGA( LOG_FATALERROR, "GetMessage( ... ) return -1 in main thread!\n" );
			break;
		}
		else
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	TrayIcon_Destroy( AGetTrayIcon( ) );

	SAFE_OBJECT_VCALL( g_lpStdPath, fDestructor );
	SAFE_OBJECT_VCALL( g_pAppLog, fDestructor );

	KMFree( g_lpStdPath );
	KMFree( g_pAppLog );

	return msg.wParam;
}


static ATOM RegisterAssistantWindowClass( HINSTANCE hInstance )
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof( WNDCLASSEXW );

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= AssistantWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground	= ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= MAIN_WINDOW_CLASS;
	wcex.hIconSm		= NULL; 

	return RegisterClassExW( &wcex );
}


static LRESULT CALLBACK AssistantWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message )
	{
	case WM_INITAPP:
		AGetTrayIcon( ) = TrayIcon_Create( hWnd,
			(HICON)LoadImage( AGetResourceInstance( ), MAKEINTRESOURCE( IDI_ASSISTANT ), IMAGE_ICON, GetSystemMetrics( SM_CXICON ), GetSystemMetrics( SM_CYICON ), LR_DEFAULTCOLOR ),
			&UUID_TRAYICON,
			WM_TRAYICON );
		TrayIcon_Show( AGetTrayIcon( ), NULL, gs_szTitle );
		TrayIcon_SetProc( AGetTrayIcon( ), AssistantTrayIconWndProc );
		Scheduler_Start( AGetScheduler( ) );
		Scheduler_AddTask( AGetScheduler( ), CreateBreakTask( ), 60 );
		break;

	case WM_COMMAND:
		switch ( LOWORD( wParam ) )
		{
		case IDM_EXIT:
			FORWARD_WM_CLOSE( hWnd, PostMessage );
			break;

		case IDM_AUTOSTART:
			RegisterAutoRun( !IsAutoRun( ) );
			break;
		}
		break;

	case WM_SHOWWINDOW:
		KING_ASSERTFAILURE_MESSAGE ( "WM_SHOWWINDOW message should not be received!\n" );
		break;

	case WM_CLOSE:
		TrayIcon_Hide( AGetTrayIcon( ) );
		FORWARD_WM_DESTROY( hWnd, PostMessage );
		break;

	case WM_DESTROY:
		Scheduler_Destroy( AGetScheduler( ) );
		PostQuitMessage( 0 );
		break;

	case WM_TRAYICON:
		TrayIcon_ProcessMessage( AGetTrayIcon( ), message, wParam, lParam, NULL );
		break;

	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return 0;
}


static HMODULE LoadAppResource( )
{
	wchar_t dir[MAX_PATH];
	wchar_t path[MAX_PATH];
	wchar_t langName[LOCALE_NAME_MAX_LENGTH];

	HMODULE hDll;

	if ( SAFE_OBJECT_VCALL( g_lpStdPath, fGetDataDir, dir, ARRAYSIZE( dir ) ) )
		return NULL;

	GetSystemDefaultLocaleName( langName, ARRAYSIZE( langName ) );

	if ( FAILED( StringCchPrintfW( path, ARRAYSIZE( path ), L"%s\\Lang\\%s\\resource.dll", dir, langName ) ) )
		return NULL;

	hDll = LoadLibraryEx( path, NULL, LOAD_LIBRARY_AS_DATAFILE );
	if ( !hDll )
	{
		if ( SUCCEEDED( StringCchPrintfW( path, ARRAYSIZE( path ), L"%s\\Lang\\%s\\resource.dll", dir, L"en-US" ) ) )
			hDll = LoadLibraryEx ( path, NULL, LOAD_LIBRARY_AS_DATAFILE );
	}

	return hDll;
}
