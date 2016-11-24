/*
 * Name:		mswstdpath.c
 * Purpose:
 * Author:		King
 * Company:
 * Created:		03/27/2011 17:53:46
 * Copyright:
 */

#include "stdafx.h"

#include "kerrno.h"
#include "kassert.h"
#include "kstdpath.h"
#include "kmemory.h"
#include "auxiliary.h"


#define PATH_SEPERATORW	L'\\'
#define PATH_SEPERATOR	'\\'
#define MAX_NAMESTRING	(32)


typedef struct _MSWStdPath
{
	const KSTDPATH_VTABLE *pVTable;
	wchar_t vendorName[MAX_NAMESTRING];
	wchar_t appName[MAX_NAMESTRING];
} MSWSTDPATH, *LPMSWSTDPATH;


static KRESULT MSW_AppendAppInfo( wchar_t *path, size_t elements, const wchar_t *szVendorName, const wchar_t *szAppName )
{
	HRESULT hr;
	size_t cch;
	wchar_t *pEnd;

	hr = StringCchLengthW( path, elements, &cch );
	if ( FAILED( hr ) || !cch )
		return EINVAL;

	if ( szVendorName && szVendorName[0] )
	{
		if ( PATH_SEPERATORW != path[cch - 1] )
		{
			if ( cch >= elements )
				return ELBUF;

			path[cch] = PATH_SEPERATORW;
			path[++cch] = L'\0';
		}

		hr = StringCchCopyExW( path + cch, elements - cch, szVendorName, &pEnd, NULL, STRSAFE_NO_TRUNCATION );

		if ( FAILED( hr ) )
			return HRESULT2KRESULT( hr );

		cch = pEnd - path - 1;
	}

	if ( szAppName && szAppName[0] )
	{
		if ( PATH_SEPERATORW != path[cch - 1] )
		{
			if ( cch + 1 >= elements )
				return ELBUF;

			path[cch] = PATH_SEPERATORW;
			path[++cch] = L'\0';
		}

		hr = StringCchCopyExW( path + cch, elements - cch, szAppName, NULL, NULL, STRSAFE_NO_TRUNCATION );

		if ( FAILED( hr ) )
			return HRESULT2KRESULT( hr );
	}

	return 0;
}


static KRESULT MSW_GetExecutablePath( void* self, wchar_t *path, size_t elements )
{
	UNREFERENCED_PARAMETER( self );

	GetModuleFileNameW( NULL, path, elements );

	return WindowsError2KRESULT( GetLastError( ) );
}


static KRESULT MSW_GetConfigDir ( void* self, wchar_t *path, size_t elements )
{
	HRESULT hr;
	PWSTR szPath;
	LPMSWSTDPATH pThis;

	hr = SHGetKnownFolderPath( &FOLDERID_ProgramData, 0, NULL, &szPath );
	if ( FAILED( hr ) )
		return EINVAL;

	hr = StringCchCopyW( path, elements, szPath );
	CoTaskMemFree( szPath );
	if ( FAILED( hr ) )
		return HRESULT2KRESULT( hr );

	pThis = (LPMSWSTDPATH)self;
	return MSW_AppendAppInfo( path, elements, pThis->vendorName, pThis->appName );
}


static KRESULT MSW_GetUserConfigDir( void *self, wchar_t *path, size_t elements )
{
	HRESULT hr;
	PWSTR szPath;

	UNREFERENCED_PARAMETER( self );

	hr = SHGetKnownFolderPath( &FOLDERID_RoamingAppData, 0, NULL, &szPath );
	if ( FAILED( hr ) )
		return EINVAL;

	hr = StringCchCopyW( path, elements, szPath );
	CoTaskMemFree( szPath );

	return HRESULT2KRESULT( hr );
}


static KRESULT MSW_GetDataDir( void *self, wchar_t *path, size_t elements )
{
	KRESULT kr;

	UNREFERENCED_PARAMETER( self );

	kr = MSW_GetExecutablePath( self, path, elements );
	if ( !kr )
		PathRemoveFileSpecW( path );

	return kr;
}


static KRESULT MSW_GetUserDataDir( void *self, wchar_t *path, size_t elements )
{
	HRESULT hr;
	PWSTR szPath;
	LPMSWSTDPATH pThis;

	hr = SHGetKnownFolderPath( &FOLDERID_RoamingAppData, 0, NULL, &szPath );
	if ( FAILED( hr ) )
		return EINVAL;

	hr = StringCchCopyW( path, elements, szPath );
	CoTaskMemFree( szPath );
	if ( FAILED( hr ) )
		return HRESULT2KRESULT( hr );

	pThis = (LPMSWSTDPATH)self;
	return MSW_AppendAppInfo( path, elements, pThis->vendorName, pThis->appName );
}


static KRESULT MSW_GetUserLocalDataDir( void *self, wchar_t *path, size_t elements )
{
	HRESULT hr;
	PWSTR szPath;
	LPMSWSTDPATH pThis;

	UNREFERENCED_PARAMETER( self );

	hr = SHGetKnownFolderPath( &FOLDERID_LocalAppData, 0, NULL, &szPath );
	if ( FAILED( hr ) )
		return EINVAL;

	hr = StringCchCopyW( path, elements, szPath );
	CoTaskMemFree( szPath );
	if ( FAILED( hr ) )
		return HRESULT2KRESULT( hr );

	pThis = (LPMSWSTDPATH)self;
	return MSW_AppendAppInfo( path, elements, pThis->vendorName, pThis->appName );
}


static KRESULT MSW_GetDocumentsDir( void *self, wchar_t *path, size_t elements )
{
	HRESULT hr;
	PWSTR szPath;

	UNREFERENCED_PARAMETER( self );

	hr = SHGetKnownFolderPath( &FOLDERID_Documents, 0, NULL, &szPath );
	if ( FAILED( hr ) )
		return EINVAL;

	hr = StringCchCopyW( path, elements, szPath );
	CoTaskMemFree( szPath );

	return HRESULT2KRESULT( hr );
}


static KRESULT CreateMSWStdPath( const wchar_t *szVendorName, const wchar_t *szAppName, LPMSWSTDPATH *ppStdPath )
{
	LPMSWSTDPATH pMSWStdPath;
	HRESULT kr;

	static const KSTDPATH_VTABLE GMSWSTDPATH_VTABLE =
	{
		NULL,
		MSW_GetExecutablePath,
		MSW_GetConfigDir,
		MSW_GetUserConfigDir,
		MSW_GetDataDir,
		NULL,
		MSW_GetUserDataDir,
		MSW_GetUserLocalDataDir,
		MSW_GetDocumentsDir,
		NULL
	};

	pMSWStdPath = (LPMSWSTDPATH)KMAlloc( sizeof( MSWSTDPATH ) );
	if ( !pMSWStdPath )
		return ENOMEM;

	kr = 0;

	if ( szVendorName )
	{
		if ( FAILED( StringCchCopyW( pMSWStdPath->vendorName, ARRAYSIZE( pMSWStdPath->vendorName ), szVendorName ) ) )
			kr = ELBUF;
	}
	else
		pMSWStdPath->vendorName[0] = L'\0';

	if ( szAppName )
	{
		if ( FAILED( StringCchCopyW( pMSWStdPath->appName, ARRAYSIZE( pMSWStdPath->appName ), szAppName ) ) )
			kr = ELBUF;
	}
	else
		pMSWStdPath->appName[0] = L'\0';

	VTableOf( pMSWStdPath ) = &GMSWSTDPATH_VTABLE;

	*ppStdPath = pMSWStdPath;

	return kr;
}


KRESULT CreateStdPath( const wchar_t *szVendorName, const wchar_t *szAppName, LPKSTDPATH *ppStdPath )
{
	return CreateMSWStdPath( szVendorName, szAppName, (LPMSWSTDPATH *)ppStdPath );
}
