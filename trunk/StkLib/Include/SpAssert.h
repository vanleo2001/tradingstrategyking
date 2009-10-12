/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		SP_ASSERT( BOOL );
		SP_VERIFY( BOOL );
*/

#if	!defined( __SP_ASSERT_H__ )
#define	__SP_ASSERT_H__

STKLIB_API	BOOL SP_IsValidString(LPCSTR lpsz, int nLength=-1);
STKLIB_API	BOOL SP_IsValidAddress( LPVOID lp, UINT cbSize, BOOL bReadWrite=TRUE );

#ifdef	_DEBUG
	
	class	Object;

	STKLIB_API	void 	SP_AssertFailedLine(LPCSTR lpszFileName, int nLine);
	STKLIB_API	void	SP_AssertValidObject( const Object* pOb, LPCSTR lpszFileName, int nLine );

	#define THIS_FILE			__FILE__
	#define SP_ASSERT(f)		((f) ? (void)0 : \
								::SP_AssertFailedLine(THIS_FILE, __LINE__))
	#define SP_VERIFY(f)		SP_ASSERT(f)
	#define	SP_ASSERT_VALID( pOb )	\
				( ::SP_AssertValidObject( pOb, THIS_FILE, __LINE__ ) )
	
#else

	#define SP_ASSERT(f)			((void)0)
	#define SP_VERIFY(f)			((void)(f))
	#define	SP_ASSERT_VALID( pOb)	((void)0)

#endif // _DEBUG

#endif // __SP_ASSERT_H__
