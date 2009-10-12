/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		SP_TRACE( LPCSTR, ... );
		SP_TRACE0( LPCSTR );
		SP_TRACE1( LPCSTR, arg1 );
		SP_TRACE2( LPCSTR, arg1, arg2 );
		SP_TRACE3( LPCSTR, arg1, arg2, arg3 );
*/

#ifndef	__SP_DEUBG_H__
#define	__SP_DEBUG_H__

#ifdef	_DEBUG
	void	SP_OutputDebugString( LPCSTR );

	int		SP_MessageBox( UINT, LPCSTR, LPCSTR, UINT );

/*
	Support trace machnism.
*/
	void _cdecl SP_Trace( LPCSTR pszFormat, ... );

	#define	BASED_DEBUG
	#define SP_TRACE	SP_Trace
	#define SP_TRACE0(sz) \
				do { \
					static char BASED_DEBUG _sz[] = sz; \
					SP_Trace(_sz); \
				} while (0)
	#define SP_TRACE1(sz, p1) \
					do { \
						static char BASED_DEBUG _sz[] = sz; \
						SP_Trace(_sz, p1); \
					} while (0)
	#define SP_TRACE2(sz, p1, p2) \
					do { \
						static char BASED_DEBUG _sz[] = sz; \
						SP_Trace(_sz, p1, p2); \
					} while (0)
	#define SP_TRACE3(sz, p1, p2, p3) \
					do { \
						static char BASED_DEBUG _sz[] = sz; \
						SP_Trace(_sz, p1, p2, p3); \
					} while (0)

#else
	void  SP_Trace( LPCSTR /*pszFormat*/, ...);
	#define SP_TRACE       SP_Trace
	#define SP_TRACE0      SP_Trace
	#define SP_TRACE1      SP_Trace
	#define SP_TRACE2      SP_Trace
	#define SP_TRACE3      SP_Trace

#endif //_DEBUG

#endif // __SP_DEBUG_H__