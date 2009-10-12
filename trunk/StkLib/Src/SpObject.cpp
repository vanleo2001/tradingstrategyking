/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	Object;
*/
#include	"StdAfx.h"

#define	_SPRING( cause )	static char sz##cause[] = #cause;
#include	"../Include/SpObjErr.inl"
#undef	_SPRING

#ifndef	_SP_ENABLE_INLINES
#define	_SPOBJ_INLINE
#include	"../Include/SpObject.inl"
#undef	_SPOBJ_INLINE
#endif

static	LPCSTR	allObjectErrorNames[] = 
{
	#define	_SPRING( cause )	sz##cause,
	#include	"../Include/SpObjErr.inl"
	#undef	_SPRING
	NULL
};


void	Object::AssertValid( ) const
{
	SP_ASSERT( NULL != this );
}

void	Object::Dump( ) const
{

}

LPCSTR	Object::GetLastErrorString( ) const
{
	if( m_nErrorStatus >= errObject_None && m_nErrorStatus < errorEnd )
		return	allObjectErrorNames[ m_nErrorStatus ];
	return	NULL;
}

#ifdef	_DEBUG
void*	Object::operator new( size_t nSize )
{
	SP_ASSERT( SP_CheckMemory() );
	void	*p	=	SP_AllocMemoryDebug( nSize, TRUE, NULL, 0 );
	if( NULL == p )
	{
		SP_TRACE1("Object::new( size_t ) allocate %d bytes fail.\n", nSize );
	}
	return	p;
}

void*	Object::operator new( size_t nSize, LPCSTR lpszFileName, int nLine )
{
	SP_ASSERT( SP_CheckMemory() );
	void	*p	=	SP_AllocMemoryDebug( nSize, TRUE, lpszFileName, nLine );
	if( NULL == p )
	{
		SP_TRACE1("Object::new( size_t, LPCSTR, int ) allocate %d bytes fail.\n", nSize );
	}
	return	p;
}

void	Object::operator delete( void *p )
{
	SP_ASSERT( SP_CheckMemory() );

	SP_FreeMemoryDebug( p, TRUE );
}

#if	_MSC_VER >= 1200
void	Object::operator delete( void *p, void* )
{
	SP_ASSERT( SP_CheckMemory() );

	SP_FreeMemoryDebug(p,TRUE);
}

void	Object::operator delete( void *p, LPCSTR /* lpszFileName */,
								int	/* nLine */ )
{
	SP_ASSERT( SP_CheckMemory() );

	SP_FreeMemoryDebug(p, TRUE);
}
#endif // _MSC_VER

#endif //_DEBUG