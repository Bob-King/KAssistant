/*
 * Name:		assert.c
 * Purpose:
 * Author:		King
 * Company:
 * Created:		03/27/2011 15:47:28
 * Copyright:
 */

#include "stdafx.h"

#define MAX_BUFFER	(512)


int AssertFailure( const char *file, int line )
{
	char buf[MAX_BUFFER];

	StringCchPrintfA( buf, ARRAYSIZE(buf), "!King! : A breakpoint occurs at %s(%d).\n", file, line );

	OutputDebugStringA( buf );

	DebugBreak( );

	return 0;
}


int AssertFailureMessage( const char *file, int line, const char *message )
{
	char buf[MAX_BUFFER];

	StringCchPrintfA( buf, ARRAYSIZE(buf), "!King! : A breakpoint occurs at %s(%d), message : %s.\n", file, line, message );

	OutputDebugStringA( buf );

	DebugBreak( );

	return 0;
}
