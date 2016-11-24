/*
 * Name:		kobject.h
 * Purpose:
 * Author:		King
 * Company:
 * Created:		03/27/2011 15:58:35
 * Copyright:
 */

#ifndef _KOBJECT_H_
#define _KOBJECT_H_


#include "ktypes.h"


typedef KRESULT (*Destructor_Function)( void *self );

#define KOBJECT_FUNCTIONS( )	Destructor_Function fDestructor;

typedef struct _KObject_VTable
{
	KOBJECT_FUNCTIONS( )
} KOBJECT_VTABLE, *LPKOBJECT_VTABLE;

typedef struct _KObject
{
	const KOBJECT_VTABLE *pVTable;
} KOBJECT, *LPKOBJECT;


#define VTableOf( p )	((p)->pVTable)


#endif // _KOBJECT_H_
