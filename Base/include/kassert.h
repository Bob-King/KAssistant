/*
 * Name:		kassert.h
 * Purpose:
 * Author:		King
 * Company:
 * Created:		03/27/2011 15:33:40
 * Copyright:
 */

#ifndef _KASSERT_H_
#define _KASSERT_H_


#include "kdefines.h"


EXTERN_C_BEGIN( )

int AssertFailure( const char *file, int line );

int AssertFailureMessage( const char *file, int line, const char *message );

EXTERN_C_END( )


#ifndef NDEBUG

#define KING_ASSERTFAILURE( )						AssertFailure( __FILE__, __LINE__ )

#define KING_ASSERTFAILURE_MESSAGE( m )				AssertFailureMessage( __FILE__, __LINE__, m )

#define KING_ASSERT( f )							((void) ((f) || KING_ASSERTFAILURE( )))

#define KING_ASSERT_MESSAGE( f, m )					((void) ((f) || KING_ASSERTFAILURE_MESSAGE( m )))

#define KING_VERIFY( f )							KING_ASSERT( f )

#define KING_VERIFY_MESSAGE( f, m )					KING_ASSERT_MESSAGE( f, m )

#else

#define KING_ASSERT( f )

#define KING_ASSERT_MESSAGE( f, m )

#define KING_VERIFY( f )							(f)

#define KING_VERIFY_MESSAGE( f, m )					(f)

#endif	// NDEBUG

#define KING_CHECK_OP( f, op )						if (!(f))	\
													{	\
														AssertFailure( __FILE__, __LINE__ );	\
														op;	\
													}

#define KING_CHECK_RETURN( f )						KING_CHECK_OP( f, return )

#define KING_CHECK_RETURN2( f, r )					KING_CHECK_OP( f, return r )

#define KING_CHECK_OP_MESSAGE( f, op, mess )		if (!(f))	\
													{	\
														AssertFailureMessage( __FILE__, __LINE__, mess );	\
														op;	\
													}

#define KING_CHECK_RETURN_MESSAGE( f, mess )		KING_CHECK_OP_MESSAGE( f, return, mess )

#define KING_CHECK_RETURN2_MESSAGE( f, r, mess )	KING_CHECK_OP_MESSAGE( f, return r, mess )


#endif	// _KASSERT_H_
