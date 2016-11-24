/**
 * Name:		MainApp.h
 * Purpose:
 * Author:		King
 * Company:
 * Created:		04/05/2011 21:10:15
 * Copyright:
 */

#pragma once


#define MAX_LOADSTRING	( 128 )

#define AGetInstance( )	( g_mainApp.hInstance )
#define AGetResourceInstance( ) ( (HINSTANCE) g_mainApp.hResource )
#define AGetMainWindow( ) ( g_mainApp.hMainWindow )
#define AGetTrayIcon( ) ( g_mainApp.hTrayIcon )
#define AGetScheduler( ) ( g_mainApp.hScheduler )

struct _MainApp
{
	HINSTANCE	hInstance;
	HMODULE		hResource;
	HWND		hMainWindow;
	HANDLE		hTrayIcon;
	HANDLE		hScheduler;
};
extern struct _MainApp g_mainApp;

#define AGetStdPath( )	( g_lpStdPath )

extern LPKSTDPATH g_lpStdPath;
