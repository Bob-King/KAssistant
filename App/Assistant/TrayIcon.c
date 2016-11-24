/**
 * Name:		TrayIcon.c
 * Purpose:
 * Author:		King
 * Company:
 * Created:		04/07/2011 21:55:48
 * Copyright:
 */

#include "stdafx.h"

#include "TrayIcon.h"


struct _TrayIcon
{
	NOTIFYICONDATA nid;
	WNDPROC wproc;
	BOOL shown;
};


HANDLE TrayIcon_Create( HWND hwnd, HICON icon, const GUID *guid, UINT msgId )
{
	struct _TrayIcon *ret;

	KING_CHECK_RETURN2( hwnd && icon && guid && ( 0xfffffc00 & msgId ), NULL );

	ret = (struct _TrayIcon *)KMAlloc( sizeof( struct _TrayIcon ) );
	if ( !ret )
		return NULL;

	ret->nid.cbSize = sizeof( ret->nid );
	ret->nid.hWnd = hwnd;
	ret->nid.hIcon = icon;
	CopyMemory( &ret->nid.guidItem, guid, sizeof( GUID ) );
	ret->nid.uCallbackMessage = msgId;
	ret->nid.uVersion = NOTIFYICON_VERSION_4;

	ret->nid.uFlags = NIF_MESSAGE;

	return (HANDLE)ret;
}


void TrayIcon_Destroy( HANDLE self )
{
	KING_CHECK_RETURN( self );

	DestroyIcon( ( (struct _TrayIcon *)self )->nid.hIcon );

	KMFree( self );
}


BOOL TrayIcon_Show( HANDLE self, HICON icon, LPCTSTR tip )
{
	struct _TrayIcon *pThis;
	BOOL ret;

	KING_CHECK_RETURN2( self, FALSE );

	pThis = (struct _TrayIcon *)self;

	if ( icon )
	{
		if ( pThis->nid.hIcon )
			DestroyIcon( pThis->nid.hIcon );

		pThis->nid.hIcon = icon;
	}

	if ( tip && tip[0] )
		StringCchCopy( pThis->nid.szTip, ARRAYSIZE( pThis->nid.szTip ), tip );

	if ( pThis->nid.hIcon )
		pThis->nid.uFlags |= NIF_ICON;
	else
		pThis->nid.uFlags &= ~NIF_ICON;

	if ( pThis->nid.szTip[0] )
		pThis->nid.uFlags |= NIF_TIP;
	else
		pThis->nid.uFlags &= ~NIF_TIP;

	ret = Shell_NotifyIcon( pThis->shown ? NIM_MODIFY : NIM_ADD, &pThis->nid );
	if ( ret && !pThis->shown )
		pThis->shown = TRUE;

	return ret;
}


BOOL TrayIcon_Hide ( HANDLE self )
{
	struct _TrayIcon *pThis;

	KING_CHECK_RETURN2( self, FALSE );

	pThis = (struct _TrayIcon *)self;

	return Shell_NotifyIcon( NIM_DELETE, &pThis->nid );
}


WNDPROC TrayIcon_SetProc( HANDLE self, WNDPROC wndProc )
{
	struct _TrayIcon *pThis;
	WNDPROC ret;

	KING_CHECK_RETURN2( self, NULL );

	pThis = (struct _TrayIcon *)self;
	ret = pThis->wproc;
	pThis->wproc = wndProc;

	return ret;
}


BOOL TrayIcon_ProcessMessage( HANDLE self, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *pResult )
{
	struct _TrayIcon *pThis;
	LRESULT result;
	BOOL handled;

	if ( !self )
		return FALSE;

	pThis = (struct _TrayIcon *)self;
	handled = pThis->nid.uCallbackMessage == msg;
	if ( handled )
	{
		if ( pThis->wproc )
		{
			result = CallWindowProc( pThis->wproc, pThis->nid.hWnd, msg, wParam, lParam );
			if ( pResult )
				*pResult = result;
		}
	}

	return handled;
}
