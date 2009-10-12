// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E81532B3_EAE0_44B4_B20A_3405CE220A7E__INCLUDED_)
#define AFX_STDAFX_H__E81532B3_EAE0_44B4_B20A_3405CE220A7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions
#include <afxinet.h>

#include <afxtempl.h>		// MFC template

#include <afxmt.h>			// MFC Thread Lock

#pragma warning( disable : 4244 )	// warning C4244: “=” : 从“__time64_t”转换到“time_t”，可能丢失数据

#include "resource.h"

#include "QSProfile.h"

#include "tracetool.h"
extern WinTrace* g_pWinTrace;

HINSTANCE AfxGetModuleHandle();
CString	AfxModuleLoadString( UINT nID );

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E81532B3_EAE0_44B4_B20A_3405CE220A7E__INCLUDED_)
