/**
 * Name:		defines.h
 * Purpose:
 * Author:		King
 * Company:
 * Created:		04/06/2011 21:37:02
 * Copyright:
 */

#ifndef _DEFINES_H_
#define _DEFINES_H_


#define A_CAT( s1, s2 )	( s1 ## s2 )

#define ASSISTANT_LOGW( level, format, ... ) KING_LOGW( level, A_CAT( L"[Assistant] ", format ), __VA_ARGS__ )
#define ASSISTANT_LOGA( level, format, ... ) KING_LOGA( level, A_CAT( "[Assistant] ", format ), __VA_ARGS__ )

#ifdef UNICODE
#define ASSISTANT_LOG	ASSISTANT_LOGW
#else
#define ASSISTANT_LOG	ASSISTANT_LOGA
#endif


#endif
