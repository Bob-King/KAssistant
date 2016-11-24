/*
 * Name:		kdefines.h
 * Purpose:
 * Author:		King
 * Company:
 * Created:		03/27/2011 15:38:52
 * Copyright:
 */

#ifndef _KDEFINES_H_
#define _KDEFINES_H_


#ifdef __cplusplus

#ifndef EXTERN_C
#define EXTERN_C			extern "C"
#endif

#define EXTERN_C_BEGIN( )	extern "C"	\
							{

#define EXTERN_C_END( )		}

#else

#ifndef EXTERN_C
#define EXTERN_C			extern
#endif

#define EXTERN_C_BEGIN( )

#define EXTERN_C_END( )

#endif	// __cplusplus


#define SAFE_OBJECT_VCALL( obj, func, ... )	( ( obj && obj->pVTable->func ) ? ( obj->pVTable->func )( obj, __VA_ARGS__ ) : ENOIMP )


#endif // _KDEFINES_H_
