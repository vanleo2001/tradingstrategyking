// StkLib.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*
	purpose:		initialize StData.dll
	return:			whether correct
*/

#ifdef	SP_WINDOWS
#ifdef	SP_WIN32
// the interfaces of dll
HINSTANCE	g_hStkLibInstance;

HINSTANCE AfxGetModuleHandle()
{
	return g_hStkLibInstance;
}

extern "C" BOOL APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hStkLibInstance	=	hInstance;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
	}
	return 1;   // ok
}

#else
int CALLBACK LibMain(HINSTANCE hinst, WORD wDataSeg, WORD cbHeapSize, LPSTR lpszCmdLine)
{
	return	TRUE;
}
#endif //SP_WIN32
#endif //SP_WINDOWS


