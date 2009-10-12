/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		SP_OutputDebugString( LPCSTR );
		SP_MessageBox( UINT, LPCSTR, LPCSTR, UINT );
		SP_Trace( LPCSTR, ...)
*/
#include	"StdAfx.h"
void SP_OutputDebugString( LPCSTR lpszDebugString )
{
#ifdef	SP_WINDOWS
	::OutputDebugString( lpszDebugString );
#else
	fprintf( stderr, "%s", lpszDebugString );
#endif
}

#include	"ctype.h"
int SP_MessageBox( UINT hWnd, LPCSTR szTitle, LPCSTR szMsg, UINT uType )
{
#ifdef	SP_WINDOWS
	return	::MessageBox( (HWND)hWnd, szTitle, szMsg, uType );
#else
	fprintf( stderr, "%s\n%s\n", szTitle, szMsg );
	if( (uType&0x000F) == MB_ABORTRETRYIGNORE )
	{
lableAgain:
		fprintf( stderr, "(A)bort, (R)etry, (I)gnore?" );
		int	chAnswer	=	getchar( );
		switch( toupper( chAnswer ) )
		{
		case	'A':
			return	IDABORT;
		case	'R':
			return	IDRETRY;
		case	'I':
			return	IDIGNORE;
		default:
			goto	lableAgain;
		}
	}
	return	IDIGNORE;
#endif
}

#ifdef	_DEBUG
	void _cdecl	SP_Trace(LPCSTR pszFormat, ...)
	{
		int nBuf;
	#ifndef _WINDLL
		char szBuffer[512];
	#else
		static char szBuffer[512];
	#endif
		char* pszLocalFormat;

	#ifdef _NEARDATA
		char szFormat[128];
		ASSERT(lstrlen(pszFormat) < 128);
		lstrcpy(szFormat, pszFormat);
		pszLocalFormat = szFormat;
	#else
		pszLocalFormat = (LPSTR)pszFormat;
	#endif

		va_list args;
		va_start(args, pszFormat);

		nBuf = vsprintf(szBuffer, pszLocalFormat, args);
		SP_ASSERT(nBuf < sizeof(szBuffer));
		SP_OutputDebugString( szBuffer );
	}

#else

	void _cdecl	SP_Trace(LPCSTR pszFormat, ...)
	{
	}

#endif//_DEBUG