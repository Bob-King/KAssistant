/**
 * Name:		Configure.c
 * Purpose:
 * Author:		King
 * Company:
 * Created:		04/22/2011 20:57:12
 * Copyright:
 */

#include "stdafx.h"

#include "Configure.h"


#define KEY_AUTORUN	TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run")


BOOL RegisterAutoRun( BOOL autorun )
{
	HKEY hKeyAutoRun;
	TCHAR path[MAX_PATH];
	LONG ret;

	KING_CHECK_RETURN2( !RegOpenKeyEx( HKEY_CURRENT_USER, KEY_AUTORUN, 0, KEY_READ | KEY_SET_VALUE, &hKeyAutoRun ), FALSE );

	if ( autorun )
	{
		GetModuleFileName( NULL, path, ARRAYSIZE( path ) );
		ret = RegSetValueEx( hKeyAutoRun, APP_NAME, 0, REG_SZ, (const BYTE *)path, sizeof( TCHAR ) * ( _tcslen( path ) + 1 ) );
	}
	else
	{
		ret = RegDeleteValue( hKeyAutoRun, APP_NAME );
	}

	RegCloseKey( hKeyAutoRun );

	return !ret;
}


BOOL IsAutoRun( )
{
	HKEY hKeyAutoRun;
	TCHAR path[MAX_PATH];
	TCHAR buf[MAX_PATH];
	DWORD cb;
	BOOL ret;

	KING_VERIFY( !RegOpenKeyEx( HKEY_CURRENT_USER, KEY_AUTORUN, 0, KEY_READ, &hKeyAutoRun ) );

	cb = sizeof( buf );
	ret = !RegQueryValueEx( hKeyAutoRun, APP_NAME, NULL, NULL, (LPBYTE)buf, &cb );
	if ( ret )
	{
		ret = sizeof( buf ) > cb && cb;
		if ( ret )
		{
			buf[cb] = 0;
			GetModuleFileName( NULL, path, ARRAYSIZE( buf ) );
			ret = !_tcscmp( buf, path );
		}
	}

	RegCloseKey( hKeyAutoRun );

	return ret;
}
