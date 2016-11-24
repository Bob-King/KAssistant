/*
 * Name:		filelog.c
 * Purpose:
 * Author:		King
 * Company:
 * Created:		03/30/2011 22:01:12
 * Copyright:
 */

#include "stdafx.h"

#include "klog.h"
#include "kmemory.h"
#include "auxiliary.h"


typedef struct _FileLog
{
	const KLOG_VTABLE *pVTable;
	HANDLE hFile;
} FILELOG, *LPFILELOG;


static KRESULT FileLog_PrintW( void *self, const wchar_t *format, ... )
{
	wchar_t buf[MAX_LOG_BUFFER];
	va_list args;
	HANDLE hFile;
	HRESULT hr;

	SYSTEMTIME st;
	char stbuf[MAX_DATETIME_BUFFER];
	size_t len;

	hFile = ((LPFILELOG)self)->hFile;
	if ( INVALID_HANDLE_VALUE == hFile )
		return ERROR_BAD_ARGUMENTS;

	va_start( args, format );
	hr = StringCchVPrintfW( buf, ARRAYSIZE( buf ), format, args );
	va_end( args );

	if ( STRSAFE_E_INVALID_PARAMETER == hr )
		return ERROR_BAD_ARGUMENTS;

	GetLocalTime( &st );
	len = FormatDateTimeA( &st, stbuf, ARRAYSIZE( stbuf ) );

	if ( len )
	{
		WriteFile( hFile, (LPCVOID)stbuf, len, NULL, NULL );
		WriteFile( hFile, (LPCVOID)" ", 1, NULL, NULL );
	}

	hr = StringCchLengthW( buf, ARRAYSIZE( buf ), &len );
	if ( SUCCEEDED( hr ) )
		WriteFile( hFile, (LPCVOID)buf, len, NULL, NULL );
	WriteFile( hFile, (LPCVOID)"\r\n", 2, NULL, NULL );

	return 0;
}


static KRESULT FileLog_PrintA( void *self, const char *format, ... )
{
	char buf[MAX_LOG_BUFFER];
	va_list args;
	size_t len;
	HRESULT hr;
	SYSTEMTIME st;
	char stbuf[MAX_DATETIME_BUFFER];
	HANDLE hFile;

	hFile = ((LPFILELOG)self)->hFile;
	if ( INVALID_HANDLE_VALUE == hFile )
		return ERROR_BAD_ARGUMENTS;

	va_start( args, format );
	hr = StringCchVPrintfA( buf, ARRAYSIZE( buf ), format, args );
	va_end( args );

	if ( STRSAFE_E_INVALID_PARAMETER == hr )
		return ERROR_BAD_ARGUMENTS;

	GetLocalTime( &st );
	len = FormatDateTimeA( &st, stbuf, ARRAYSIZE( stbuf ) );

	if ( len )
	{
		WriteFile( hFile, (LPCVOID)stbuf, len, NULL, NULL );
		WriteFile( hFile, (LPCVOID)" ", 1, NULL, NULL );
	}

	hr = StringCchLengthA( buf, ARRAYSIZE( buf ), &len );
	if ( SUCCEEDED( hr ) )
		WriteFile( hFile, (LPCVOID)buf, len, NULL, NULL );
	WriteFile( hFile, (LPCVOID)"\r\n", 2, NULL, NULL );

	return 0;
}


static KRESULT FileLog_Destroy( void *self )
{
	CloseHandle( ((LPFILELOG)self)->hFile );

	return 0;
}


KRESULT CreateFileLog( const char *path, LPKLOG *ppLog )
{
	HANDLE hFile;
	LPFILELOG pFileLog;
	KRESULT kr;

	static const KLOG_VTABLE GFILELOG_VTABLE =
	{
		FileLog_Destroy,
		FileLog_PrintW,
		FileLog_PrintA
	};

	if ( !path || !ppLog )
		return ERROR_BAD_ARGUMENTS;

	hFile = CreateFileA( path, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL );
	if ( INVALID_HANDLE_VALUE == hFile )
		return GetLastError( );

	pFileLog = (LPFILELOG)KMAlloc( sizeof(FILELOG) );
	if ( !pFileLog )
	{
		kr = GetLastError( );
		CloseHandle( hFile );
		return kr;
	}

	VTableOf( pFileLog ) = &GFILELOG_VTABLE;
	pFileLog->hFile = hFile;
	*ppLog = (LPKLOG)pFileLog;

	return 0;
}
