/**
 * Name:		kmemory.h
 * Purpose:
 * Author:		King
 * Company:
 * Created:		04/16/2011 16:13:30
 * Copyright:
 */

#ifndef _KMEMORY_H_
#define _KMEMORY_H_


#include "kdefines.h"


#define KMAlloc( bytes )	MemoryAlloc( bytes )
#define KMFree( mem )	MemoryFree( mem )


EXTERN_C void *MemoryAlloc( size_t bytes );
EXTERN_C void MemoryFree( void *mem );


#endif
