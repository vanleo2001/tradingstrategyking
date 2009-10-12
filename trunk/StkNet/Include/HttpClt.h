/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CHttpClient;
*/

#if !defined(AFX_HTTPCLT_H__D5109C29_E3AB_4320_B1EC_2B7A22F3B118__INCLUDED_)
#define AFX_HTTPCLT_H__D5109C29_E3AB_4320_B1EC_2B7A22F3B118__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxinet.h>


#define	ERR_TEAR_CANCEL			101
#define	ERR_TEAR_URLFORMAT		102
#define	ERR_TEAR_REDIRECT		103
#define	ERR_TEAR_INTERRUPTED	104
#define	ERR_TEAR_DATATRANSFER	105
/***
	Http连接异常
*/
class CTearException : public CException
{
	DECLARE_DYNCREATE(CTearException)

public:
	CTearException(int nCode = 0);
	~CTearException() { }

	virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError,
		PUINT pnHelpContext = NULL);

	int m_nErrorCode;
};

/*
 * class CHttpClient provide several function
 */
#define	ERR_HTTP_TEARSESSION	201

/***
	Http客户端，下载Http服务器上的文件
*/
class STKNET_API CHttpClient : public Object
{
public:
	CHttpClient();
	virtual ~CHttpClient();

	// Attributes
	BOOL SetServer( LPCTSTR lpszServer, INTERNET_PORT port = INTERNET_DEFAULT_HTTP_PORT );	//	设置服务器
	BOOL SetAccessType( int nAccessType, int nProxyType, LPCTSTR lpszProxyAddress, UINT nProxyPort,
						LPCTSTR lpszProxyUser, LPCTSTR lpszProxyPasswd );	//	设置代理

	// operation do not need log in
	DWORD	WGet( LPCTSTR lpszURL, CString &strResult, PROGRESS_CALLBACK fnCallback, void *cookie );	// 直接下载文件，返回至strResult
	DWORD	WGet( LPCTSTR lpszURL, CFile *pFile, PROGRESS_CALLBACK fnCallback, void *cookie );			// 直接下载文件，写入文件
	DWORD	WGet(	LPCTSTR lpszRegCode, LPCTSTR lpszUserName,
						LPCTSTR lpszPasswd, LPCTSTR lpszVersion,
						CString &strResult, int nWantLen );

	// Operations
	BOOL	Login(	LPCTSTR lpszRegCode, LPCTSTR lpszUserName,
					LPCTSTR lpszPasswd,	LPCTSTR lpszVersion );	//	登陆网站
	BOOL	Logout( );		// 退出登陆

	BOOL	LoadServerInfo( CString &strResult, PROGRESS_CALLBACK fnCallback, void *cookie );	// 下载服务器信息
	BOOL	LoadPackageInfo( CString &strResult, PROGRESS_CALLBACK fnCallback, void *cookie );	// 下载数据包信息
	BOOL	LoadPackage( LPCTSTR lpszURL, CFile * pFile, PROGRESS_CALLBACK fnCallback, void *cookie );	// 下载数据包

	virtual BOOL GetLastErrorMessage( LPTSTR lpszError, UINT nMaxError );

protected:
	DWORD	RequestGet( LPCTSTR lpszURL, CMapStringToString * pmapParam,
						CFile * pFileSave, PROGRESS_CALLBACK fnCallback, void *cookie );
	DWORD	RequestGet( LPCTSTR lpszURL, CMapStringToString * pmapParam,
						CString*pstrResult, PROGRESS_CALLBACK fnCallback, void *cookie );
	DWORD	RequestPost(LPCTSTR lpszURL, CMapStringToString* pmapParam,
						 CFile * pFileSave, PROGRESS_CALLBACK fnCallback, void *cookie );
	DWORD	RequestPost(LPCTSTR lpszURL, CMapStringToString* pmapParam,
						CString*pstrResult, PROGRESS_CALLBACK fnCallback, void *cookie );
	
	DWORD	Request(	LPCTSTR lpszURL, CString &strPostData,
						CFile *pFileSave, CString*pstrResult,
						PROGRESS_CALLBACK fnCallback, void *cookie );

protected:
	static BOOL EscapeString( LPCTSTR lpszValue, CString& strOut );
	static BOOL MakeHttpHeader( CMapStringToString& map, CString& out );
	static BOOL MakePostData( CMapStringToString &__map, CString &__strOut );
	static BOOL MakeGetData( CMapStringToString &__map, CString &__strOut );
	static BOOL _MakeHttpData( CMapStringToString &__map, CString &__strOut, LPCTSTR __lpszSep );

	static BOOL PostDataEncoding( CString &strPostData );
	static BOOL PostDataDecoding( CString &strPostData );

	void DoOpenURL( LPCTSTR lpszURL, DWORD dwHttpRequestFlags,
					CString &strHeader, CString &strPostData,
					CInternetSession *pSession,
					CHttpConnection **ppServer, CHttpFile **ppFile,
					PROGRESS_CALLBACK fnCallback, void *cookie );
	void DoSecurityRequest( CHttpFile * pFile, CString &strPostDat );
	CString	GetNewLocation( CHttpFile * pFile );
	


protected:
	CString			m_strServer;
	INTERNET_PORT	m_nPort;

	int				m_nAccessType;
	int				m_nProxyType;
	CString			m_strProxyAddress;
	UINT			m_nProxyPort;
	CString			m_strProxyUser;
	CString			m_strProxyPasswd;

	CString			m_strRegCode;
	CString			m_strUserName;
	CString			m_strPasswd;
	CString			m_strCookie;
	BOOL			m_bIsLogin;

	CString			m_strLastErrorMessage;
};

#endif // !defined(AFX_HTTPCLT_H__D5109C29_E3AB_4320_B1EC_2B7A22F3B118__INCLUDED_)
