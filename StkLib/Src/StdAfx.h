// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1CAD116B_242A_4541_868B_BDB802746ACA__INCLUDED_)
#define AFX_STDAFX_H__1CAD116B_242A_4541_868B_BDB802746ACA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT 
#define _WIN32_WINNT 0x0501 
#endif

// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include	<stdio.h>
#include	<stdlib.h>
#include	<stdarg.h>
#include	<string.h>
#include	<io.h>
#include	<ctype.h>
#include	<limits.h>
#include	<math.h>

#ifdef		_WINDOWS
	#define	_STRICT
	#include	<windows.h>
#endif

#pragma warning (disable : 4251)	// warning C4251: “CDJ::m_stockSha” : class“CStock”需要有 dll 接口由 class“CDJ”的客户端使用 
#pragma warning (disable : 4275)	// warning C4275: non dll-interface class 'CSPArray<class CStockInfo,class CStockInfo &>' used as base for dll-interface class 'CStockContainer'
#pragma warning (disable : 4996)	// warning C4996: “strncpy”被声明为否决的

#include	<StkLib.h>
#include	"SpDebug.h"
#include	"SpMemory.h"
#include	"Strings.h"

#ifndef		UNUSED
#define		UNUSED(x)	x
#endif

#ifdef	SP_WINDOWS
#ifdef	SP_WIN32
HINSTANCE AfxGetModuleHandle();
#endif
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1CAD116B_242A_4541_868B_BDB802746ACA__INCLUDED_)
