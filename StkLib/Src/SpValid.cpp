/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		SP_IsValidString( LPCSTR, int );
		SP_IsValidAddress( LPVOID, UINT, BOOL );
*/
#include	"StdAfx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif

BOOL SP_IsValidString(LPCSTR lpsz, int nLength)
{
#ifdef	SP_WINDOWS
	if (lpsz == NULL)
		return FALSE;
	return ::IsBadStringPtr( lpsz, nLength) == 0;
#else
	if (lpsz == NULL)
		return FALSE;
	return	TRUE;
#endif
}

BOOL SP_IsValidAddress( LPVOID lp, UINT nBytes, BOOL bReadWrite )
{
#ifdef	SP_WINDOWS
	return (lp != NULL && !IsBadReadPtr(lp, nBytes) &&
		(!bReadWrite || !IsBadWritePtr((LPVOID)lp, nBytes)));
#else
	if( NULL == lp )
		return	FALSE;
	else
		return	TRUE;
#endif
}