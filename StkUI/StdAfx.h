// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A20D8132_8317_4482_B56B_7EDBCF089D4F__INCLUDED_)
#define AFX_STDAFX_H__A20D8132_8317_4482_B56B_7EDBCF089D4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用 Windows 95 和 Windows NT 4 或更高版本的特定功能。
#define WINVER 0x0400		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif

#define _WIN32_IE 0x0601

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常被安全忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#pragma warning( disable : 4244 )	// warning C4244: “=” : 从“__time64_t”转换到“time_t”，可能丢失数据
#pragma warning( disable : 4995 )	// warning C4995: “CDaoRecordset”: 名称被标记为 #pragma deprecated
#pragma warning (disable : 4996)	// warning C4996: “strncpy”被声明为否决的

#include <math.h>

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxpriv.h>
#include <afxtempl.h>
#include <afxcview.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdao.h>			// MFC DaoDatabase
#include <afxdb.h>			// MFC ODBC database classes
#include "htmlhelp/htmlhelp.h"

#include <../src/mfc/afximpl.h>

// GuiLib
//#include	"../GuiLib/Header/ExtLib.h"

// XPFace
// #define	SPFACE_STATIC
#include	"../XPFace/Include/XPFace.h"

// StkLib
// #define	STKLIB_STATIC
#include	"../StkLib/Include/StkLib.h"

// StkNet
// #define	STKNET_STATIC
#include	"../StkNet/Include/StkNet.h"

#include	"Resource.h"
#include	"AfxCore.h"

// 使用单文档
#ifndef _TSK_MAINFRAME
#define _TSK_MAINFRAME
#endif

#endif // !defined(AFX_STDAFX_H__A20D8132_8317_4482_B56B_7EDBCF089D4F__INCLUDED_)
