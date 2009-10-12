// NetTS.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "TSWnd.h"
#include "TWSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE NetTSDLL = { NULL, NULL };
HINSTANCE	g_hNetTSInstance;
WinTrace* g_pWinTrace;

HINSTANCE AfxGetModuleHandle()
{
	return g_hNetTSInstance;
}

CString	AfxModuleLoadString( UINT nID )
{
	HINSTANCE hInstanceOld = AfxGetResourceHandle();
	AfxSetResourceHandle( AfxGetModuleHandle() );

	CString	string;
	string.LoadString( nID );
	
	AfxSetResourceHandle( hInstanceOld );
	return string;
}

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("NETTS.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(NetTSDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(NetTSDLL);

		g_hNetTSInstance	=	hInstance;
		AfxSetResourceHandle( hInstance );

		AfxGetQSProfile().LoadProfile();

		g_pWinTrace = new WinTrace("NetTS", "NetTS");

		CTWSocket::Init( );
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		CTWSocket::GetInstance().EndWorking();
		CTWSocket::Release( );

		TRACE0("NETTS.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(NetTSDLL);
	}
	return 1;   // ok
}
