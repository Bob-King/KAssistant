/*
 * Name:		auxiliary.h
 * Purpose:
 * Author:		King
 * Company:
 * Created:		04/03/2011 13:23:27
 * Copyright:
 */

#ifndef _AUXILIARY_H_
#define _AUXILIARY_H_

#include "ktypes.h"


KRESULT HRESULT2KRESULT( HRESULT hr );
KRESULT WindowsError2KRESULT( DWORD code );

size_t FormatDateTimeW( const SYSTEMTIME *pTime, wchar_t *wcs, size_t size );
size_t FormatDateTimeA( const SYSTEMTIME *pTime, char *str, size_t size );


#endif // _AUXILIARY_H_
