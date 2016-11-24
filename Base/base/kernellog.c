/*
 * Name:		kernellog.c
 * Purpose:
 * Author:		King
 * Company:
 * Created:		04/03/2011 13:50:40
 * Copyright:
 */

#include "stdafx.h"

#include "klog.h"
#include "kmemory.h"
#include "auxiliary.h"


static KRESULT KernelLog_PrintW( void *self, const wchar_t *format, ... )
{
	wchar_t buf[MAX_LOG_BUFFER];
	va_list args;

	SYSTEMTIME st;
	wchar_t stbuf[MAX_DATETIME_BUFFER];

	UNREFERENCED_PARAMETER( self );

	GetLocalTime( &st );
	if ( FormatDateTimeW( &st, stbuf, ARRAYSIZE( stbuf ) ) )
		OutputDebugStringW( stbuf );

	OutputDebugStringW( L"\t" );
	
	va_start( args, format );
	if ( STRSAFE_E_INVALID_PARAMETER != StringCchVPrintfW( buf, ARRAYSIZE( buf ), format, args ) )
		OutputDebugStringW( buf );
	va_end( args );

	OutputDebugStringW( L"\r\n" );

	return 0;
}


static KRESULT KernelLog_PrintA( void *self, const char *format, ... )
{
	char buf[MAX_LOG_BUFFER];
	va_list args;

	SYSTEMTIME st;
	char stbuf[MAX_DATETIME_BUFFER];

	UNREFERENCED_PARAMETER( self );

	GetLocalTime( &st );
	if ( FormatDateTimeA( &st, stbuf, ARRAYSIZE( stbuf ) ) )
		OutputDebugStringA( stbuf );

	OutputDebugStringA( "\t" );
	
	va_start( args, format );
	if ( STRSAFE_E_INVALID_PARAMETER != StringCchVPrintfA( buf, ARRAYSIZE( buf ), format, args ) )
		OutputDebugStringA( buf );
	va_end( args );

	OutputDebugStringA( "\r\n" );

	return 0;
}


KRESULT CreateKernelLog( LPKLOG *ppLog )
{
	LPKLOG pKernelLog;

	static const KLOG_VTABLE GKERNELLOG_VTABLE =
	{
		NULL,
		KernelLog_PrintW,
		KernelLog_PrintA
	};

	pKernelLog = (LPKLOG)KMAlloc( sizeof(KLOG) );
	if ( !pKernelLog )
		return GetLastError( );

	VTableOf( pKernelLog ) = &GKERNELLOG_VTABLE;
	*ppLog = pKernelLog;

	return 0;
}
