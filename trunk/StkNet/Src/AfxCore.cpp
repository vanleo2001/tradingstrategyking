// AfxCore.cpp : implementation of the AfxCore Functions
//

#include	"stdafx.h"
#include	"AfxCore.h"
#include	"ProxySetDlg.h"
#include	"SMailer\SMailer.h"
#include	"Secret.h"

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CNetDatabase & AfxGetNetDB()
{
	static	CNetDatabase	g_netdatabase;
	return g_netdatabase;
}

STKNET_API	BOOL AfxSendMail(LPCTSTR lpszSenderName, LPCTSTR lpszSenderAddress,
							 LPCTSTR lpszReceiverName, LPCTSTR lpszReceiverAddress,
							 LPCTSTR lpszSubject, LPCTSTR lpszContent, CString & strErrorMsg )
{
	CString sContent = lpszContent;
	CString sContentHTML = "<HTML><BODY><PRE>" + sContent + "</PRE></BODY></HTML>";
	
	SMailer::TextPlainContent * content1 = new SMailer::TextPlainContent( LPCTSTR(sContent) );
	SMailer::TextHtmlContent  * content2 = new SMailer::TextHtmlContent( LPCTSTR(sContentHTML) );

	SMailer::MailInfo info;
	info.setSenderName( lpszSenderName );
	info.setSenderAddress( lpszSenderAddress );
	info.addReceiver( lpszReceiverName, lpszReceiverAddress );
	info.setPriority(SMailer::Priority::normal);
	info.setSubject( lpszSubject );
	info.addMimeContent( content1 );
	info.addMimeContent( content2 );

	SMailer::MailSender sender( (LPCTSTR)AfxGetProfile().GetEvaluateSMTPServer(),
								(LPCTSTR)AfxGetProfile().GetEvaluateSMTPUser(),
								(LPCTSTR)AfxGetProfile().GetEvaluateSMTPPasswd() );
	sender.setMail(&SMailer::MailWrapper(&info));
	BOOL success = sender.sendMail();
	if( !success )
		strErrorMsg += sender.getErrMessage();
	if( content1 )	{	delete content1;	content1 = NULL;	}
	if( content2 )	{	delete content2;	content2 = NULL;	}
	return success;
}

BOOL AfxGetEvaluateCode( CString & strEvaCode )
{
	CSPTime	tm = CSPTime::GetCurrentTime();
	tm +=	CSPTimeSpan(AfxGetSView().GetMaxEvaDays()+30);
	CSPString strRegCode0;
	if( AfxGetSView().DumpEva( tm.GetTime(), strRegCode0 ) )
	{
		strEvaCode = strRegCode0;
		return TRUE;
	}
	return FALSE;
}

STKNET_API	BOOL AfxApplyForEvaluation(LPCTSTR lpszUserName, LPCTSTR lpszDistrict,
									   LPCTSTR lpszClass, LPCTSTR lpszEmail,
									   CString & strErrorMsg )
{
	if( NULL == lpszUserName || NULL == lpszDistrict || NULL == lpszClass || NULL == lpszEmail )
		return FALSE;
	if( strlen(lpszUserName) <= 0 || strlen(lpszDistrict) <= 0 || strlen(lpszClass) <= 0 || strlen(lpszEmail) <= 0 )
		return FALSE;

	CString	sSubject = AfxModuleLoadString( IDS_APPLYFOREVA_SUBJECT );

	CString sContent;
#ifdef CLKLAN_ENGLISH_US
	sContent	+=	"\r\nVersion:Ninebulls310-20060325";
#else
	sContent	+=	"\r\nVersion:CLKing310-20060325";
#endif
	sContent	+=	"\r\nUser:";
	sContent	+=	lpszUserName;
	sContent	+=	"\r\n";
	sContent	+=	"District:";
	sContent	+=	lpszDistrict;
	sContent	+=	"\r\n";
	sContent	+=	"Class:";
	sContent	+=	lpszClass;
	sContent	+=	"\r\n";
	sContent	+=	"Email:";
	sContent	+=	lpszEmail;
	sContent	+=	"\r\n";

	CString sEvaCode;
	if( AfxGetEvaluateCode(sEvaCode) )
	{
		sContent	+=	"EvaCode:";
		sContent	+=	sEvaCode;
		sContent	+=	"\r\n";
		if( AfxSendMail( lpszUserName, lpszEmail, "evaluate", (LPCTSTR)AfxGetProfile().GetEvaluateEmail(),
							(LPCTSTR)sSubject, (LPCTSTR)sContent, strErrorMsg ) )
		{
			CString sReplyContent = AfxModuleLoadString( IDS_APPLYFOREVA_REPLYCONTENT );
			sReplyContent += "\r\n\r\n    ";
			sReplyContent += sEvaCode;
			sReplyContent += "\r\n\r\n";
			sReplyContent += AfxModuleLoadString( IDS_APPLYFOREVA_REPLYDESCRIPT );
			return AfxSendMail( "", (LPCTSTR)AfxGetProfile().GetEvaluateEmail(), lpszUserName, lpszEmail,
						(LPCTSTR)("Re:"+sSubject), (LPCTSTR)sReplyContent, strErrorMsg );
		}
	}
	return FALSE;
}

BOOL AfxDoProxySetting( )
{
	CProxySetDlg dlg;
	dlg.DoModal();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// ×¢²á±í¶ÁÐ´º¯Êý

BOOL AfxSetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName)
{
	if (lpszValueName == NULL)
	{
		if (::RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ,
			  lpszValue, lstrlen(lpszValue) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			TRACE1("Warning: registration database update failed for key '%s'.\n",
				lpszKey);
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		HKEY hKey;

		if(::RegCreateKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) == ERROR_SUCCESS)
		{
			LONG lResult = ::RegSetValueEx(hKey, lpszValueName, 0, REG_SZ,
				(CONST BYTE*)lpszValue, (lstrlen(lpszValue) + 1) * sizeof(TCHAR));

			if(::RegCloseKey(hKey) == ERROR_SUCCESS && lResult == ERROR_SUCCESS)
				return TRUE;
		}
		TRACE1("Warning: registration database update failed for key '%s'.\n", lpszKey);
		return FALSE;
	}
}

LONG AfxGetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
	HKEY hkey;
	LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

	if (retval == ERROR_SUCCESS) {
		long datasize = MAX_PATH;
		TCHAR data[MAX_PATH];
		RegQueryValue(hkey, NULL, data, &datasize);
		lstrcpy(retdata,data);
		RegCloseKey(hkey);
	}

	return retval;
}

// recursively remove a registry key if and only if it has no subkeys
BOOL AfxDeleteRegKey(LPCTSTR lpszKey)
{
	// copy the string
	LPTSTR lpszKeyCopy = _tcsdup(lpszKey);
	LPTSTR lpszLast = lpszKeyCopy + lstrlen(lpszKeyCopy);

	// work until the end of the string
	while (lpszLast != NULL)
	{
		*lpszLast = '\0';
		lpszLast = _tcsdec(lpszKeyCopy, lpszLast);

		// try to open that key
		HKEY hKey;
		if (::RegOpenKey(HKEY_CLASSES_ROOT, lpszKeyCopy, &hKey) != ERROR_SUCCESS)
			break;

		// enumerate the keys underneath
		TCHAR szScrap[_MAX_PATH+1];
		DWORD dwLen = sizeof(szScrap);
		BOOL bItExists = FALSE;

		if (::RegEnumKey(hKey, 0, szScrap, dwLen) == ERROR_SUCCESS)
			bItExists = TRUE;
		::RegCloseKey(hKey);

		// found one?  quit looping
		if (bItExists)
			break;

		// otherwise, delete and find the previous backwhack
		::RegDeleteKey(HKEY_CLASSES_ROOT, lpszKeyCopy);
		lpszLast = _tcsrchr(lpszKeyCopy, '\\');
	}

	// release the string and return
	free(lpszKeyCopy);
	return TRUE;
}

HKEY AfxGetSectionKey( HKEY hKey, LPCTSTR szRegKeyGroup, LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp, LPCTSTR lpszSection )
{
	HKEY hGroupKey = NULL;
	HKEY hCompanyKey = NULL;
	HKEY hAppKey = NULL;
	HKEY hSectionKey = NULL;
	if (RegOpenKeyEx( hKey, szRegKeyGroup, 0, KEY_WRITE|KEY_READ,
		&hGroupKey) == ERROR_SUCCESS)
	{
		DWORD dw;
		if (RegCreateKeyEx(hGroupKey, szRegKeyCompany, 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hCompanyKey, &dw) == ERROR_SUCCESS )
		{
			RegCreateKeyEx(hCompanyKey, szRegKeyApp, 0, REG_NONE,
				REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
				&hAppKey, &dw);
		}
	}
	if (hGroupKey != NULL)
		RegCloseKey(hGroupKey);
	if (hCompanyKey != NULL)
		RegCloseKey(hCompanyKey);

	DWORD dw;
	if( NULL == hAppKey )
		return FALSE;
	RegCreateKeyEx(hAppKey, lpszSection, 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
		&hSectionKey, &dw);
	RegCloseKey(hAppKey);
	
	return hSectionKey;
}

BOOL AfxRegSetValue( LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue )
{
	ASSERT( lpszSection && lpszEntry && lpszValue );
	if( !lpszSection || !lpszEntry || !lpszValue )
		return FALSE;
	
	HKEY hSectionKey = AfxGetSectionKey( HKEY_LOCAL_MACHINE, _T("Software"), szRegKeyCompany, szRegKeyApp, lpszSection );

	if (hSectionKey == NULL)
		return FALSE;
	LONG lResult = RegSetValueEx(hSectionKey, lpszEntry, NULL, REG_SZ,
		(LPBYTE)lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR));
	RegCloseKey(hSectionKey);

	return lResult == ERROR_SUCCESS;
}

CString	AfxRegQueryString(HKEY hKey, LPCTSTR szRegKeyGroup, LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault )
// hKey = HKEY_LOCAL_MACHINE, HKEY_CLASSES_ROOT, or so
{
	HKEY hSecKey = AfxGetSectionKey(hKey, szRegKeyGroup, szRegKeyCompany, szRegKeyApp, lpszSection);
	if (hSecKey == NULL)
		return lpszDefault;
	CString strValue;
	DWORD dwType, dwCount;
	LONG lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
		NULL, &dwCount);
	if (lResult == ERROR_SUCCESS)
	{
		ASSERT(dwType == REG_SZ);
		lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
		strValue.ReleaseBuffer();
	}
	RegCloseKey(hSecKey);
	if (lResult == ERROR_SUCCESS)
	{
		ASSERT(dwType == REG_SZ);
		return strValue;
	}
	return lpszDefault;
}

BOOL AfxRegQueryString(HKEY hKey, LPCTSTR szRegKeyGroup, LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp,
					   CStringArray &astrName, CStringArray &astrData )
// hKey = HKEY_LOCAL_MACHINE, HKEY_CLASSES_ROOT, or so
{
	HKEY hSecKey = AfxGetSectionKey(hKey, szRegKeyGroup, szRegKeyCompany, szRegKeyApp, "");
	if (hSecKey == NULL)
		return FALSE;
	
	int	nIndex	=	0;
	while( TRUE )
	{
		CString strName, strData;
		CHAR szName[MAX_PATH];
		BYTE szData[MAX_PATH];
		DWORD dwType, dwNameLen=sizeof(szName), dwDataLen=sizeof(szData);
		if( ERROR_SUCCESS != RegEnumValue(hSecKey, nIndex, szName, &dwNameLen,
				NULL, &dwType, szData, &dwDataLen ) )
			break;
		if( dwType == REG_SZ )
		{
			astrName.Add( (LPCTSTR)szName );
			astrData.Add( (LPCTSTR)szData );
		}
		nIndex	++;
	}

	RegCloseKey(hSecKey);
	return astrName.GetSize() > 0;
}

HINSTANCE AfxGotoURL(LPCTSTR url, int showcmd)
{
	TCHAR key[MAX_PATH + MAX_PATH];

	// First try ShellExecute()
	HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);

	// If it failed, get the .htm regkey and lookup the program
	if ((UINT)result <= HINSTANCE_ERROR) {

		if (AfxGetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) {
			lstrcat(key, _T("\\shell\\open\\command"));

			if (AfxGetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) {
				TCHAR *pos;
				pos = _tcsstr(key, _T("\"%1\""));
				if (pos == NULL) {                     // No quotes found
					pos = strstr(key, _T("%1"));       // Check for %1, without quotes 
					if (pos == NULL)                   // No parameter at all...
						pos = key+lstrlen(key)-1;
					else
						*pos = '\0';                   // Remove the parameter
				}
				else
					*pos = '\0';                       // Remove the parameter

				lstrcat(pos, _T(" "));
				lstrcat(pos, url);
				result = (HINSTANCE) WinExec(key,showcmd);
			}
		}
	}

	return result;
}

BOOL AfxRegisterActiveXCtrls( LPCTSTR lpszOcxFileName )
{
	HINSTANCE hLib = LoadLibrary( lpszOcxFileName );
	if (hLib < (HINSTANCE)HINSTANCE_ERROR)
		return FALSE; 
  
	FARPROC   lpfnRegister;
	(FARPROC&)lpfnRegister = ::GetProcAddress( hLib, _T("DllRegisterServer") );
  
	if (lpfnRegister!= NULL)
	{
		(*lpfnRegister)();//Register control
		return TRUE;
	}
	return FALSE;
}

CString AfxGetVersionString( )
{
	static	CString	strVersion;
	if( strVersion.IsEmpty() )
	{
		if( AfxGetSView().IsProf() )
			strVersion	=	AfxModuleLoadString( IDS_VERSION_PROFESSIONAL );
		else if( AfxGetSView().IsStd() )
			strVersion	=	AfxModuleLoadString( IDS_VERSION_STDANDARD );
		else
			strVersion	=	AfxModuleLoadString( IDS_VERSION_EVALUATE );
		strVersion	=	(LPCTSTR)AfxGetProfile().GetVersion( ) + strVersion;
	}
	return strVersion;
}

CString AfxGetFileTitle( LPCTSTR lpszPathName )
{
	CString	strPathName	=	lpszPathName;
	if( strPathName.IsEmpty() )
		return "";
	UINT nTitleLen = AfxGetFileTitle( strPathName, NULL, 0 );
	if( nTitleLen <= 0 || nTitleLen-1 > (UINT)strPathName.GetLength() )
		return "";

	CString strTitle = strPathName.Right( nTitleLen-1 );
	return strTitle;
}

CString AfxGetFileTitleNoExt( LPCTSTR lpszPathName )
{
	CString	strPathName	=	lpszPathName;
	if( strPathName.IsEmpty() )
		return "";
	UINT nTitleLen = AfxGetFileTitle( strPathName, NULL, 0 );
	if( nTitleLen <= 0 || nTitleLen-1 > (UINT)strPathName.GetLength() )
		return "";

	CString strTitle = strPathName.Right( nTitleLen-1 );
	int nIndex = strTitle.ReverseFind( '.' );
	if( -1 != nIndex )
		strTitle = strTitle.Left( nIndex );
	return strTitle;
}

CString AfxGetFileDirectoryExist( LPCTSTR lpszPathName, LPCTSTR lpszDefault )
{
	CString	strPathName	=	lpszPathName;
	CString	strDefault	=	lpszDefault;
	if( strPathName.IsEmpty() )
		return strDefault;

	CString	strResult	=	strPathName;
	while( !strResult.IsEmpty()
		&& FILE_ATTRIBUTE_DIRECTORY != GetFileAttributes( strResult ) )
	{
		int	nIndex	=	strResult.ReverseFind( '\\' );
		if( -1 == nIndex )
			nIndex	=	strResult.ReverseFind( '/' );
		if( -1 == nIndex )
		{
			strResult = strDefault;
			break;
		}
		strResult	=	strResult.Left( nIndex );
	}

	if( strResult.GetLength() <= 0 )
		return strDefault;

	int	nLen	=	strResult.GetLength();
	if( nLen > 0 && strResult[nLen-1] != '\\' && strResult[nLen-1] != '/' )
		strResult	+=	STRING_DIRSEP;

	return strResult;
}

#ifdef AFX_CORE2_SEG
#pragma code_seg(AFX_CORE2_SEG)
#endif

#ifdef AFX_TERM_SEG
#pragma code_seg(AFX_TERM_SEG)
#endif

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

/////////////////////////////////////////////////////////////////////////////
