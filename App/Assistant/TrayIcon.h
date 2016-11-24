/**
 * Name:		TrayIcon.h
 * Purpose:
 * Author:		King
 * Company:
 * Created:		04/07/2011 21:55:46
 * Copyright:
 */

#ifndef _TRAYICON_H_
#define _TRAYICON_H_


// msgId must be greater than or equal to 1024
EXTERN_C HANDLE TrayIcon_Create( HWND hwnd, HICON icon, const GUID *guid, UINT msgId );
EXTERN_C void TrayIcon_Destroy( HANDLE self );

EXTERN_C BOOL TrayIcon_Show ( HANDLE self, HICON icon, LPCTSTR tip );
EXTERN_C BOOL TrayIcon_Hide( HANDLE self );

EXTERN_C WNDPROC TrayIcon_SetProc( HANDLE self, WNDPROC wndProc );
EXTERN_C BOOL TrayIcon_ProcessMessage( HANDLE self, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *pResult );


#endif // _TRAYICON_H_
