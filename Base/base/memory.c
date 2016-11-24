/**
 * Name:		memory.c
 * Purpose:
 * Author:		King
 * Company:
 * Created:		04/16/2011 16:20:01
 * Copyright:
 */

#include "stdafx.h"

#include "kmemory.h"


void *MemoryAlloc( size_t bytes )
{
	return HeapAlloc( GetProcessHeap( ), HEAP_ZERO_MEMORY, bytes );
}


void MemoryFree( void *mem )
{
	HeapFree( GetProcessHeap( ), 0, mem );
}
