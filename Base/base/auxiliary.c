/*
 * Name:		auxiliary.c
 * Purpose:
 * Author:		King
 * Company:
 * Created:		04/03/2011 13:23:23
 * Copyright:
 */

#include "stdafx.h"

#include "kassert.h"
#include "klog.h"
#include "auxiliary.h"
#include "kerrno.h"


size_t FormatDateTimeW( const SYSTEMTIME *pTime, wchar_t *wcs, size_t size )
{
	size_t cchDate;
	size_t cchTime;

	if ( !wcs || !size )
		return 0;

	cchDate = GetDateFormatEx( NULL, 0, pTime, DATE_FORMATW, wcs, size, NULL );
	if ( cchDate && cchDate < size )
	{
		wcs[cchDate - 1] = L' ';
		wcs[cchDate] = L'\0';
		cchTime = GetTimeFormatEx( NULL, 0, pTime, TIME_FORMATW, wcs + cchDate, size - cchDate );
		if ( cchTime )
			--cchTime;
		return cchDate + cchTime;
	}
	else
		return cchDate - 1;
}


size_t FormatDateTimeA( const SYSTEMTIME *pTime, char *str, size_t size )
{
	wchar_t buf[MAX_DATETIME_BUFFER];
	size_t cch;

	cch = FormatDateTimeW( pTime, buf, ARRAYSIZE( buf ) );

	return WideCharToMultiByte( CP_ACP, 0, buf, cch + 1, str, size, NULL, NULL );
}


KRESULT HRESULT2KRESULT( HRESULT hr )
{
	switch ( hr )
	{
	case S_OK:
		return 0;

	case STRSAFE_E_INSUFFICIENT_BUFFER:
		return ELBUF;

	case STRSAFE_E_INVALID_PARAMETER:
		return EINVAL;

	default:
		KING_ASSERTFAILURE_MESSAGE( "Function HRESULT2KRESULT: Unexpected arguments" );
		return EUNKNOWN;
	}
}


KRESULT WindowsError2KRESULT( DWORD err )
{
	switch ( err )
	{
	case ERROR_SUCCESS:
		return 0;

	case ERROR_BAD_ARGUMENTS:
		return EINVAL;

	default:
		KING_ASSERTFAILURE_MESSAGE( "Function HRESULT2KRESULT: Unexpected arguments" );
		return EUNKNOWN;
	}
}
