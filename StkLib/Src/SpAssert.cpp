/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		SP_ASSERT( BOOL f );
		SP_VERIFY( BOOL f );
*/
#include	"StdAfx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
int NEAR afxIgnoreAssertCount = 0;               // for testing diagnostics
LONG NEAR afxAssertBusy = -1;

#pragma optimize("qgel", off) // assembler cannot be globally optimized

void SP_AssertFailedLine(LPCSTR lpszFileName, int nLine)
{
	if (afxIgnoreAssertCount > 0)
	{
		afxIgnoreAssertCount--;
		return;
	}

	char sz[255];
	static char szTitle[] = "Assertion Failed!";
	static char szMessage[] = "%s: File %s, Line %d";
	static char szUnknown[] = "<unknown application>";

	// In case _AfxGetAppDataFails.
	if (++afxAssertBusy > 0)
	{
		afxAssertBusy--;
		// break into the debugger (or Dr Watson log)
#ifndef _PORTABLE
		_asm { int 3 };
#endif
		return;
	}

	// get app name or NULL if unknown (don't call assert)
	const char* pszAppName = "Spring";
	sprintf(sz, (LPCSTR)szMessage,
		(pszAppName == NULL) ? (LPCSTR)szUnknown : (LPCSTR)pszAppName,
		lpszFileName,
		nLine);

	SP_OutputDebugString(sz);
	SP_OutputDebugString(", ");
	SP_OutputDebugString(szTitle);
	SP_OutputDebugString("\n\r");

	int nCode = SP_MessageBox( NULL, sz, szTitle,
			MB_SYSTEMMODAL | MB_ICONHAND | MB_ABORTRETRYIGNORE);
	afxAssertBusy--;
	if (nCode == IDIGNORE)
	{
		return;     // ignore
	}
	else if (nCode == IDRETRY)
	{
#ifndef _PORTABLE
		_asm { int 3 };
#endif //_PORTABLE
		return;
	}

	abort();
}

#pragma optimize("", on)


void	SP_AssertValidObject( const Object* pOb, 
							 LPCSTR lpszFileName, int nLine )
{
	if (pOb == NULL)
	{
		SP_TRACE0("ASSERT_VALID fails with NULL pointer\n");
		SP_AssertFailedLine(lpszFileName, nLine);
		return;     // quick escape
	}
	if (!SP_IsValidAddress((void*)pOb, sizeof(Object)))
	{
		SP_TRACE0("ASSERT_VALID fails with illegal pointer\n");
		SP_AssertFailedLine(lpszFileName, nLine);
		return;     // quick escape
	}

#ifndef _M_I86SM
	// check to make sure the far VTable pointer is valid
	SP_ASSERT(sizeof(Object) == sizeof(void FAR*) + sizeof(int) );
	if (!SP_IsValidAddress(*(void FAR**)pOb, sizeof(void FAR*), FALSE))
	{
		SP_TRACE0("ASSERT_VALID fails with illegal vtable pointer\n");
		SP_AssertFailedLine(lpszFileName, nLine);
		return;     // quick escape
	}
#endif

	pOb->AssertValid();
}

#endif//_DEBUG