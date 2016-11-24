/*
 * Name:		klog.h
 * Purpose:
 * Author:		King
 * Company:
 * Created:		03/27/2011 22:51:36
 * Copyright:
 */

#ifndef _KLOG_H_
#define _KLOG_H_


#include "ktypes.h"
#include "kobject.h"


#define KING_LOGW( level, format, ... )	( ( void ) ( ( level & g_appLogLevel) && g_pAppLog && g_pAppLog->pVTable->fPrintW( g_pAppLog, format, __VA_ARGS__ ) ) )
#define KING_LOGA( level, format, ... )	( ( void ) ( ( level & g_appLogLevel) && g_pAppLog && g_pAppLog->pVTable->fPrintA( g_pAppLog, format, __VA_ARGS__ ) ) )

#ifdef UNICODE
#define KING_LOG	KING_LOGW
#else
#define KING_LOG	KING_LOGA
#endif


#define MAX_LOG_BUFFER (1024)
#define MAX_DATETIME_BUFFER (32)


#define DATE_FORMATW	(L"yyyy-MM-dd")
#define TIME_FORMATW	(L"HH:mm:ss")


enum
{
	LOG_FATALERROR = 0x1,
	LOG_ERROR = LOG_FATALERROR << 1,
	LOG_WARNING = LOG_ERROR << 1,
	LOG_INFO = LOG_WARNING << 1,
	LOG_VERBOS = LOG_INFO << 1,
	LOG_DEBUG = LOG_VERBOS << 1
};


typedef KRESULT (*PrintW_Function)( void *self, const wchar_t *format, ... );
typedef KRESULT (*PrintA_Function)( void *self, const char *format, ... );


#define KLOG_FUNCTIONS( )	KOBJECT_FUNCTIONS( )	\
							PrintW_Function fPrintW;	\
							PrintA_Function fPrintA;


typedef struct _KLog_VTable
{
	KLOG_FUNCTIONS( )
} KLOG_VTABLE, *LPKLOG_VTABLE;


typedef struct _KLog
{
	const KLOG_VTABLE *pVTable;
} KLOG, *LPKLOG;


extern U32 g_appLogLevel;
extern LPKLOG g_pAppLog;


EXTERN_C KRESULT CreateFileLog( const char *path, LPKLOG *ppLog );
EXTERN_C KRESULT CreateKernelLog( LPKLOG *ppLog );


#endif // _KLOG_H_
