#if !defined(AFX_STKNETAFXCORE_H__2F96EC6F_0775_45AA_99EE_0FC048AD3208__INCLUDED_)
#define AFX_SKTNETAFXCORE_H__2F96EC6F_0775_45AA_99EE_0FC048AD3208__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AfxCore.h : header file
//

//////////////////////////////////////////////////////////////////////
// The following is implemented by MFC
UINT AFXAPI AfxGetFileTitle(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax);
void AFXAPI AfxGetModuleShortFileName(HINSTANCE hInst, CString& strShortName);

BOOL AFXAPI AfxFullPath(LPTSTR lpszPathOut, LPCTSTR lpszFileIn);
BOOL AFXAPI AfxResolveShortcut(CWnd* pWnd, LPCTSTR pszShortcutFile,
	LPTSTR pszPath, int cchPath);

/////////////////////////////////////////////////////////////////////
// The Following is command functions

STKNET_API	CNetDatabase & AfxGetNetDB();

STKNET_API	BOOL AfxApplyForEvaluation(LPCTSTR lpszUserName, LPCTSTR lpszDistrict,
									   LPCTSTR lpszClass, LPCTSTR lpszEmail,
									   CString & strErrorMsg );

// Proxy setting
STKNET_API	BOOL AfxDoProxySetting( );

// Register Function
STKNET_API	BOOL AfxSetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);
STKNET_API	LONG AfxGetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
STKNET_API	BOOL AfxDeleteRegKey(LPCTSTR lpszKey);
STKNET_API	HKEY AfxGetSectionKey(HKEY hKey, LPCTSTR szRegKeyGroup, 
								  LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp, LPCTSTR lpszSection );
STKNET_API	BOOL AfxRegSetValue( LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue );
STKNET_API	CString	AfxRegQueryString(HKEY hKey, LPCTSTR szRegKeyGroup, 
									  LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault );
STKNET_API	BOOL AfxRegQueryString(HKEY hKey, LPCTSTR szRegKeyGroup, LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp,
					   CStringArray &astrName, CStringArray &astrData );


// Goto URL functions
STKNET_API	HINSTANCE AfxGotoURL(LPCTSTR url, int showcmd);

// ActiveX Functions
STKNET_API	BOOL AfxRegisterActiveXCtrls( LPCTSTR lpszOcxFileName );

STKNET_API	CString	AfxGetVersionString( );
STKNET_API	CString AfxGetFileTitle( LPCTSTR lpszPathName );
STKNET_API	CString AfxGetFileTitleNoExt( LPCTSTR lpszPathName );
STKNET_API	CString	AfxGetFileDirectoryExist( LPCTSTR lpszPathName, LPCTSTR lpszDefault );
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STKNETAFXCORE_H__2F96EC6F_0775_45AA_99EE_0FC048AD3208__INCLUDED_)
