/*
 * Name:		kstdpath.h
 * Purpose:
 * Author:		King
 * Company:
 * Created:		03/27/2011 16:59:23
 * Copyright:
 */

#ifndef _KSTDPATH_H_
#define _KSTDPATH_H_


#include "ktypes.h"
#include "kobject.h"
#include "kdefines.h"


typedef KRESULT (*GetExecutablePath_Function)( void *self, wchar_t *path, size_t elements );
typedef KRESULT (*GetConfigDir_Function)( void *self, wchar_t *path, size_t elements );
typedef KRESULT (*GetUserConfigDir_Function)( void *self, wchar_t *path, size_t elements );
typedef KRESULT (*GetDataDir_Function)( void *self, wchar_t *path, size_t elements );
typedef KRESULT (*GetLocalDataDir_Function)( void *self, wchar_t *path, size_t elements );
typedef KRESULT (*GetUserDataDir_Function)( void *self, wchar_t *path, size_t elements );
typedef KRESULT (*GetUserLocalDataDir_Function)( void *self, wchar_t *path, size_t elements );
typedef KRESULT (*GetDocumentsDir_Function)( void *self, wchar_t *path, size_t elements );
typedef KRESULT (*GetAppDocumentsDir_Function)( void *self, wchar_t *path, size_t elements );

#define KSTDPATH_FUNCTIONS( )	KOBJECT_FUNCTIONS( )	\
								GetExecutablePath_Function fGetExecutablePath;	\
								GetConfigDir_Function fGetConfigDir;	\
								GetUserConfigDir_Function fGetUserConfigDir;	\
								GetDataDir_Function fGetDataDir;	\
								GetLocalDataDir_Function fGetLocalDataDir;	\
								GetUserDataDir_Function fGetUserDataDir;	\
								GetUserLocalDataDir_Function fGetUserLocalDataDir;	\
								GetDocumentsDir_Function fGetDocumentsDir;	\
								GetAppDocumentsDir_Function fGetAppDocumentsDir;

typedef struct _KStdPath_VTable
{
	KSTDPATH_FUNCTIONS( )
} KSTDPATH_VTABLE, *LPKSTDPATH_VTABLE;

typedef struct _KStdPath
{
	const KSTDPATH_VTABLE *pVTable;
} KSTDPATH, *LPKSTDPATH;

EXTERN_C KRESULT CreateStdPath( const wchar_t *szVendorName, const wchar_t *szAppName, LPKSTDPATH *ppStdPath );


#endif // _KSTDPATH_H_
