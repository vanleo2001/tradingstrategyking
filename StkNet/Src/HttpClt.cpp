/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CHttpClient;
*/

#include "stdafx.h"
#include "Resource.h"
#include "Base64.h"
#include "../Include/HttpClt.h"
#include "Strings.h"
#include "ProxySocket.h"

#include <TCHAR.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CTearException object

IMPLEMENT_DYNCREATE(CTearException, CException)

CTearException::CTearException(int nCode)
	: m_nErrorCode(nCode)
{
}

BOOL CTearException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError,
		PUINT pnHelpContext )
{
	CString	string;
	switch( m_nErrorCode )
	{
	case ERR_TEAR_CANCEL: break;
	case ERR_TEAR_URLFORMAT: string	=	szErrUrlFormat;	break;
	case ERR_TEAR_REDIRECT: string	=	szErrRedirect;	break;
	case ERR_TEAR_INTERRUPTED: string	=	szErrInterrupted;	break;
	case ERR_TEAR_DATATRANSFER: string	=	szErrDataTransfer;	break;
	}

	strncpy( lpszError, string.GetBuffer(string.GetLength()+1), nMaxError );
	string.ReleaseBuffer();
	return TRUE;
}

void ThrowTearException(int nCode)
{
	CTearException* pEx = new CTearException(nCode);
	throw pEx;
}

/////////////////////////////////////////////////////////////////////////////
// CHttpClient object

const TCHAR	*szHeaderFormat	=	"%s: %s\r\n";
const TCHAR	*szUserAgentKey	=	_T( "User-Agent" );
const TCHAR *szUserAgentValue	=	_T( "Internet Explorer" );
const TCHAR *szCookieKey	=	_T( "Cookie" );
const TCHAR *szRange		=	_T( "Range" );
const TCHAR *szCheckSumKey	=	_T( "BLCheckSum" );
const TCHAR *szCheckSumKeySuffix	=	_T( "BLCheckSum: " );

const TCHAR *szRegCodeKey	=	_T( "RegCode" );
const TCHAR *szUserNameKey	=	_T( "User" );
const TCHAR *szPasswdKey	=	_T( "Passwd" );
const TCHAR *szDiskSerialKey=	_T( "SerialCheck" );
const TCHAR *szVersionKey	=	_T( "Version" );

const TCHAR szHttpURLFormat[]	=	"http://%s:%d%s";
#ifdef	CLKLAN_ENGLISH_US
	const TCHAR *szWebpathWGet			=	_T( "/enu/download/ninebulls/wget" );
	const TCHAR *szWebpathLogin			=	_T( "/enu/download/ninebulls/login" );
	const TCHAR *szWebpathLogout		=	_T( "/enu/download/ninebulls/logout" );
	const TCHAR *szWebpathPackageInfo	=	_T( "/enu/download/ninebulls/package3.xml" );
	const TCHAR *szWebpathServerInfo	=	_T( "/enu/download/ninebulls/server3.xml" );
#elif defined CLKVER_OEM
	const TCHAR *szWebpathWGet			=	_T( "/download/stockana/wget" );
	const TCHAR *szWebpathLogin			=	_T( "/download/stockana/login" );
	const TCHAR *szWebpathLogout		=	_T( "/download/stockana/logout" );
	const TCHAR *szWebpathPackageInfo	=	_T( "/download/stockana/package3.xml" );
	const TCHAR *szWebpathServerInfo	=	_T( "/download/stockana/server3.xml" );
#else
	const TCHAR *szWebpathWGet			=	_T( "/download/clking/wget" );
	const TCHAR *szWebpathLogin			=	_T( "/download/clking/login" );
	const TCHAR *szWebpathLogout		=	_T( "/download/clking/logout" );
	const TCHAR *szWebpathPackageInfo	=	_T( "/download/clking/package3.xml" );
	const TCHAR *szWebpathServerInfo	=	_T( "/download/clking/server3.xml" );
#endif

const TCHAR *szWGetResultKey		=	_T( "retvalue=" );
const TCHAR *szResultOKFmt			=	_T( "%sO%s" );
const TCHAR *szResultFailed			=	_T( "-ERR" );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHttpClient::CHttpClient()
{
	m_nAccessType	=	INTERNET_OPEN_TYPE_DIRECT;
	m_bIsLogin		=	FALSE;
}

CHttpClient::~CHttpClient()
{

}

BOOL CHttpClient::SetServer(LPCTSTR lpszServer, INTERNET_PORT nPort)
{
	if( m_bIsLogin )
	{
		if( 0 != m_strServer.CompareNoCase(lpszServer) || m_nPort != nPort )
			m_bIsLogin	=	FALSE;
	}

	if( NULL != lpszServer )
		m_strServer		= lpszServer;
	else
		m_strServer.Empty();
	m_nPort			= nPort;
	return	m_strServer.GetLength()>0;
}

BOOL CHttpClient::SetAccessType( int nAccessType, int nProxyType, LPCTSTR lpszProxyAddress, UINT nProxyPort,
								LPCSTR lpszProxyUser, LPCTSTR lpszProxyPasswd )
{
	switch( nAccessType )
	{
	case INTERNET_OPEN_TYPE_PRECONFIG:
		m_nAccessType	=	nAccessType;
		break;
	case INTERNET_OPEN_TYPE_DIRECT:
		m_nAccessType	=	nAccessType;
		break;
	case INTERNET_OPEN_TYPE_PROXY:
		m_nAccessType	=	nAccessType;
		m_nProxyType	=	nProxyType;
		if( NULL == lpszProxyAddress || strlen(lpszProxyAddress)==0 )
		{
			m_nAccessType	=	INTERNET_OPEN_TYPE_PRECONFIG;
			ASSERT( FALSE );
			return	FALSE;
		}
		m_strProxyAddress	= lpszProxyAddress;
		m_nProxyPort		= nProxyPort;
		m_strProxyUser		=	lpszProxyUser;
		m_strProxyPasswd	=	lpszProxyPasswd;
		break;
	default:
		return FALSE;
	}
	
	return	TRUE;
}

DWORD CHttpClient::WGet( LPCTSTR lpszURL, CString &strResult,
						PROGRESS_CALLBACK fnCallback, void *cookie )
{
	return RequestGet( lpszURL, NULL, &strResult, fnCallback, cookie );
}

DWORD CHttpClient::WGet( LPCTSTR lpszURL, CFile *pFile, 
						PROGRESS_CALLBACK fnCallback, void *cookie )
{
	ASSERT( pFile );
	return RequestGet( lpszURL, NULL, pFile, fnCallback, cookie );
}

long getHardDriveComputerID();

static CString	GetDiskSerial( )
{
	CString	strSerial;
	DWORD id = (DWORD)getHardDriveComputerID ();
	// DWORD	id	=	0;
	// ::GetVolumeInformation( "C:\\", NULL, 0, &id, NULL, NULL, NULL, 0 );
	strSerial.Format( "%u", id );
	return strSerial;
}

DWORD CHttpClient::WGet( LPCTSTR lpszRegCode, LPCTSTR lpszUserName,
						LPCTSTR lpszPasswd, LPCTSTR lpszVersion,
						CString &strResult, int nWantLen )
{
	CMapStringToString	map;
	map.SetAt( szRegCodeKey, lpszRegCode );
	map.SetAt( szUserNameKey, m_strUserName );
	map.SetAt( szPasswdKey, m_strPasswd );
	map.SetAt( szVersionKey, lpszVersion );
	
	CString	strDiskSerial	=	GetDiskSerial( );
	map.SetAt( szDiskSerialKey, strDiskSerial );
	
	CString	strURL;
	strURL.Format( szHttpURLFormat, m_strServer, m_nPort, szWebpathWGet );

	CString	strResultLocal, strValue;
	DWORD dwRet = RequestPost( strURL, &map, &strResultLocal, NULL, NULL );
	if( HTTP_STATUS_OK == dwRet )
	{
		int	nIndex	=	strResultLocal.Find( szWGetResultKey );
		if( -1 != nIndex )
			strValue	=	strResultLocal.Mid( nIndex+strlen(szWGetResultKey), nWantLen );
	}
	if( strValue.GetLength() == nWantLen )
	{
		strResult	=	strValue;
		return dwRet;
	}
	return dwRet;
}

BOOL CHttpClient::Login(LPCTSTR lpszRegCode,
						LPCTSTR lpszUserName, LPCTSTR lpszPasswd,
						LPCTSTR lpszVersion )
{
	if( m_bIsLogin && lpszRegCode && 0 == m_strRegCode.CompareNoCase(lpszRegCode)
		&& lpszUserName && 0 == m_strUserName.CompareNoCase(lpszUserName) )
		return TRUE;

	m_bIsLogin	=	FALSE;
	m_strRegCode.Empty();
	m_strUserName.Empty();
	m_strPasswd.Empty();
	if( NULL != lpszRegCode )
		m_strRegCode	=	lpszRegCode;
	if( NULL != lpszUserName )
		m_strUserName	=	lpszUserName;
	if( NULL != lpszPasswd )
		m_strPasswd		=	lpszPasswd;

	CMapStringToString	map;
	map.SetAt( szRegCodeKey, lpszRegCode );
	map.SetAt( szVersionKey, lpszVersion );
	map.SetAt( szUserNameKey, m_strUserName );
	map.SetAt( szPasswdKey, m_strPasswd );
	
	CString	strDiskSerial	=	GetDiskSerial( );
	map.SetAt( szDiskSerialKey, strDiskSerial );
	
	CString	strURL;
	strURL.Format( szHttpURLFormat, m_strServer, m_nPort, szWebpathLogin );

	CString	strResult;
	if( HTTP_STATUS_OK == RequestPost( strURL, &map, &strResult, NULL, NULL ) )
	{
		CString	strResultOK;
		strResultOK.Format( szResultOKFmt, "+", "K" );
		if( -1 != strResult.Find( strResultOK ) )
		{
			m_bIsLogin	=	TRUE;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CHttpClient::Logout( )
{
	if( !m_bIsLogin )
		return FALSE;

	m_strRegCode.Empty();
	m_strUserName.Empty();
	m_strPasswd.Empty();

	CString	strURL;
	strURL.Format( szHttpURLFormat, m_strServer, m_nPort, szWebpathLogout );
	
	CString	strResult;
	if( HTTP_STATUS_OK == RequestPost( strURL, NULL, &strResult, NULL, NULL ) )
	{
		CString	strResultOK;
		strResultOK.Format( szResultOKFmt, "+", "K" );
		int nIndex = strResult.Find( strResultOK );
	}

	m_bIsLogin	=	FALSE;

	return TRUE;
}


BOOL CHttpClient::LoadServerInfo( CString &strResult,
								 PROGRESS_CALLBACK fnCallback, void *cookie )
{
	if( !m_bIsLogin )
		return FALSE;

	CString	strURL;
	strURL.Format( szHttpURLFormat, m_strServer, m_nPort, szWebpathServerInfo );

	return ( HTTP_STATUS_OK == RequestGet( strURL, NULL, &strResult, fnCallback, cookie ) );
}

BOOL CHttpClient::LoadPackageInfo( CString &strResult,
								  PROGRESS_CALLBACK fnCallback, void *cookie )
{
	if( !m_bIsLogin )
		return FALSE;

	CString	strURL;
	strURL.Format( szHttpURLFormat, m_strServer, m_nPort, szWebpathPackageInfo );

	return ( HTTP_STATUS_OK == RequestGet( strURL, NULL, &strResult, fnCallback, cookie ) );
}

BOOL CHttpClient::LoadPackage( LPCTSTR lpszURL, CFile *pFile,
							  PROGRESS_CALLBACK fnCallback, void *cookie )
{
	if( !m_bIsLogin )
		return FALSE;

	return ( HTTP_STATUS_OK == RequestGet( lpszURL, NULL, pFile, fnCallback, cookie ) );
}

BOOL CHttpClient::GetLastErrorMessage(LPTSTR lpszError, UINT nMaxError )
{
	if( !m_strLastErrorMessage.IsEmpty() )
	{
		strncpy( lpszError, m_strLastErrorMessage.GetBuffer(m_strLastErrorMessage.GetLength()+1), nMaxError );
		m_strLastErrorMessage.ReleaseBuffer();
		return strlen(lpszError)>0;
	}
	if( nMaxError > 0 )
		lpszError[0]	=	'\0';
	return FALSE;
}

//////////////////////////////////////////////////////////////
// static methods
const TCHAR  *szUnEscape	=	_T("^_.-");

BOOL CHttpClient::EscapeString(LPCTSTR lpszValue, CString &strOut)
{
	int	i = 0;
	TCHAR	tszTemp[ 4 ];

	if( NULL == lpszValue )
		return	FALSE;

	strOut.Empty( );
	ZeroMemory( tszTemp, sizeof(tszTemp) );

	for( i=0; i<lstrlen(lpszValue); i++ )
	{
		TCHAR	tch	=	lpszValue[ i ];
		if( !_istalnum( tch ) && (NULL==_tcschr( szUnEscape, tch) ) )
		{
			_stprintf( tszTemp, "%%%02X", (TBYTE)tch );
			strOut	+=	tszTemp;
		}
		else
			strOut	+=	tch;
	}
	return	TRUE;
}

BOOL CHttpClient::_MakeHttpData(CMapStringToString &__map, CString &__strOut, LPCTSTR __lpszSep)
{
	CString	strKey, strValue, strTemp;
	POSITION	pos;

	ASSERT( 0 == lstrcmp(__lpszSep, "\r\n") || 0 == lstrcmp( __lpszSep, "&" ) );

	__strOut	=	_T("");
	pos	=	__map.GetStartPosition( );
	while( pos )
	{
		__map.GetNextAssoc( pos, strKey, strValue );
		EscapeString( strValue, strTemp );
		__strOut	+=	strKey;
		__strOut	+=	_T("=");
		__strOut	+=	strTemp;
		__strOut	+=	__lpszSep;
	}
	return	TRUE;
}

BOOL CHttpClient::MakeHttpHeader(CMapStringToString &__map, CString &__strOut)
{
	CString	strKey, strValue, strTemp;
	POSITION	pos;
	
	pos	=	__map.GetStartPosition( );
	while( pos )
	{
		/* Add other Header */
		__map.GetNextAssoc( pos, strKey, strValue );

		if( 0 != strKey.Compare( szUserAgentKey ) )
		{
			strTemp.Format( szHeaderFormat, strKey, strValue );
			__strOut		+=	strTemp;
		}
	}
	return	TRUE;
}

BOOL CHttpClient::MakePostData(CMapStringToString &__map, CString &__strOut )
{
	return	_MakeHttpData( __map, __strOut, "\r\n" );
}

BOOL CHttpClient::MakeGetData(CMapStringToString &__map, CString &__strOut )
{
	return	_MakeHttpData( __map, __strOut, "&" );
}

BOOL CHttpClient::PostDataEncoding( CString &strPostData )
{
	int len = strPostData.GetLength();
	CString	strDesData;
	CDLocalView	des;
	des.Create( (BYTE *)strPostData.GetBuffer(len), (BYTE *)strDesData.GetBuffer(len+1), len );
	(strDesData.GetBuffer(len+1))[len]	=	'\0';

	size_t srcLen = len;
	const char *srcBuf = strDesData.GetBuffer(srcLen);
	size_t destSize = 4*(srcLen+2)/3;
	destSize += strlen("\r\n")*destSize/72 + 2;
	destSize += 64; /*64;  a little extra room */

	/* Allocate destination buffer */
	char * destBuf = strPostData.GetBuffer(destSize);
	if( 0 == destBuf )
	{
		strDesData.ReleaseBuffer();
		strPostData.ReleaseBuffer();
		return FALSE;
	}
	
	/* Encode source to destination */
	size_t destLen = 0;
	int result = encode_base64(srcBuf, srcLen, destBuf, destSize, &destLen);
	if( destLen < destSize )
		destBuf[destLen]	=	'\0';
	
	strDesData.ReleaseBuffer();
	strPostData.ReleaseBuffer();

	return ( 0 == result && destLen < destSize );
}

BOOL CHttpClient::PostDataDecoding( CString &strPostData )
{
	CString	strDesData;
	int len = strPostData.GetLength();
	
	size_t srcLen = len;
	const char *srcBuf = strPostData.GetBuffer(srcLen);
	size_t destSize = 4*(srcLen+2)/3;
	destSize += strlen("\r\n")*destSize/72 + 2;
	destSize += 64; /*64;  a little extra room */

	/* Allocate destination buffer */
	char * destBuf = strDesData.GetBuffer(destSize);
	if( 0 == destBuf )
	{
		strPostData.ReleaseBuffer();
		strDesData.ReleaseBuffer();
		return FALSE;
	}
	
	/* Encode source to destination */
	size_t destLen = 0;
	int result = decode_base64(srcBuf, srcLen, destBuf, destSize, &destLen);
	if( destLen < destSize )
		destBuf[destLen]	=	'\0';
	strPostData.ReleaseBuffer();
	strDesData.ReleaseBuffer();
	if( 0 != result || destLen >= destSize )
		return FALSE;

	CDLocalView	des;
	len	=	destLen;
	des.Destroy( (BYTE *)strDesData.GetBuffer(destLen), (BYTE *)strPostData.GetBuffer(len+1), len );
	(strPostData.GetBuffer(len+1))[len]	=	'\0';
	
	strPostData.ReleaseBuffer();
	strDesData.ReleaseBuffer();

	return TRUE;
}

//////////////////////////////////////////////////////////////
// common methods
DWORD CHttpClient::RequestGet( LPCTSTR lpszURL, CMapStringToString* pmapParam,
						CFile * pFileSave, PROGRESS_CALLBACK fnCallback, void *cookie )
{
	if( NULL == lpszURL || strlen(lpszURL) == 0 )
		return HTTP_STATUS_BAD_REQUEST;

	if( NULL != pFileSave && CFile::hFileNull == pFileSave->m_hFile )
	{
		ASSERT( FALSE );
		return	HTTP_STATUS_BAD_REQUEST;
	}

	CString	strURL	=	lpszURL;
	if( NULL != pmapParam )
	{
		CString	strParam;
		MakeGetData( *pmapParam, strParam );
		if(!strParam.IsEmpty())
			strURL += "?" + strParam;
	}
	return Request( strURL, CString(""), pFileSave, NULL, fnCallback, cookie );
}

DWORD CHttpClient::RequestGet( LPCTSTR lpszURL, CMapStringToString* pmapParam,
						CString *pstrResult, PROGRESS_CALLBACK fnCallback, void *cookie )
{
	if( NULL == lpszURL || strlen(lpszURL) == 0 )
		return HTTP_STATUS_BAD_REQUEST;

	CString	strURL	=	lpszURL;
	if( NULL != pmapParam )
	{
		CString	strParam;
		MakeGetData( *pmapParam, strParam );
		if(!strParam.IsEmpty())
			strURL += "?" + strParam;
	}
	return Request( strURL, CString(""), NULL, pstrResult, fnCallback, cookie );
}

DWORD CHttpClient::RequestPost( LPCTSTR lpszURL, CMapStringToString* pmapParam,
						CFile * pFileSave, PROGRESS_CALLBACK fnCallback, void *cookie )
{
	if( NULL != pFileSave && CFile::hFileNull == pFileSave->m_hFile )
	{
		ASSERT( FALSE );
		return	HTTP_STATUS_BAD_REQUEST;
	}

	CString	strPostData;
	if( NULL != pmapParam )
		MakePostData( *pmapParam, strPostData );

	// PostDataEncoding( strPostData );

	return Request( lpszURL, strPostData, pFileSave, NULL, fnCallback, cookie );
}

DWORD CHttpClient::RequestPost( LPCTSTR lpszURL, CMapStringToString* pmapParam,
						CString *pstrResult, PROGRESS_CALLBACK fnCallback, void *cookie )
{
	CString	strPostData;
	if( NULL != pmapParam )
		MakePostData( *pmapParam, strPostData );

	// PostDataEncoding( strPostData );

	return Request( lpszURL, strPostData, NULL, pstrResult, fnCallback, cookie );
}

void CHttpClient::DoOpenURL( LPCTSTR lpszURL, DWORD dwHttpRequestFlags,
							CString &strHeader, CString &strPostData,
							CInternetSession *pSession,
							CHttpConnection **ppServer, CHttpFile **ppFile,
							PROGRESS_CALLBACK fnCallback, void * cookie )
{
	CString strServerName;
	CString strObject;
	INTERNET_PORT nPort;
	DWORD dwServiceType;
	
	int nVerb = (strPostData.GetLength()>0 ? CHttpConnection::HTTP_VERB_POST : CHttpConnection::HTTP_VERB_GET);

	if (!AfxParseURL(lpszURL, dwServiceType, strServerName, strObject, nPort) ||
		dwServiceType != INTERNET_SERVICE_HTTP)
	{
		ThrowTearException( ERR_TEAR_URLFORMAT );
	}

	if( fnCallback )
		fnCallback( PROG_HTTPCONNECTTING, 0, NULL, cookie );

	CHttpConnection *pServer = pSession->GetHttpConnection(strServerName, dwHttpRequestFlags, nPort, m_strProxyUser, m_strProxyPasswd );
	CHttpFile	* pFile = pServer->OpenRequest(nVerb, strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
	pFile->AddRequestHeaders(strHeader, HTTP_ADDREQ_FLAG_COALESCE);
	if( strPostData.GetLength() > 0 ) // post
	{
		try
		{
			pFile->SendRequestEx( DWORD(strPostData.GetLength()) );
			pFile->WriteString(strPostData);
			pFile->EndRequest();
		}
		catch( CInternetException *pp )	// HTTP_STATUS_BAD_METHOD
		{
			pp->Delete();

			// close up the redirected site
			pFile->Close();
			delete pFile;
			pFile	=	NULL;
			pServer->Close();
			delete pServer;
			pServer	=	NULL;

			pServer = pSession->GetHttpConnection(strServerName, dwHttpRequestFlags, nPort, m_strProxyUser, m_strProxyPasswd );
			pFile = pServer->OpenRequest(nVerb, strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
			pFile->AddRequestHeaders(strHeader, HTTP_ADDREQ_FLAG_COALESCE);
			pFile->SendRequestEx( DWORD(strPostData.GetLength()) );
			pFile->WriteString(strPostData);
			pFile->EndRequest();
		}
	}
	else
	{
		pFile->SendRequest();
	}

	if( fnCallback )
		fnCallback( PROG_REQUESTSENT, 0, NULL, cookie );

	// Bad Post method
	DWORD dwRet	=	0;
	pFile->QueryInfoStatusCode( dwRet );

	// if access was denied, prompt the user for the password
	if (dwRet == HTTP_STATUS_DENIED
		|| dwRet == HTTP_STATUS_PROXY_AUTH_REQ )
	{
		DWORD dwPrompt;
		dwPrompt = pFile->ErrorDlg(NULL, ERROR_INTERNET_INCORRECT_PASSWORD,
			FLAGS_ERROR_UI_FILTER_FOR_ERRORS | FLAGS_ERROR_UI_FLAGS_GENERATE_DATA
			| FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL);

		// if the user cancelled the dialog, bail out
		if (dwPrompt != ERROR_INTERNET_FORCE_RETRY)
			ThrowTearException( ERR_TEAR_CANCEL );
		if( strPostData.GetLength() > 0 ) // post
		{
			try{
				pFile->SendRequestEx( DWORD(strPostData.GetLength()) );
				pFile->WriteString(strPostData);
				pFile->EndRequest();
			}
			catch( CInternetException *pp ) // HTTP_STATUS_BAD_METHOD
			{
				pp->Delete();

				pFile->SendRequestEx( DWORD(strPostData.GetLength()) );
				pFile->WriteString(strPostData);
				pFile->EndRequest();
			}
		}
		else
		{
			pFile->SendRequest();
		}
		pFile->QueryInfoStatusCode(dwRet);
	}

	if( dwRet == HTTP_STATUS_BAD_METHOD )
	{
		// close up the redirected site
		pFile->Close();
		delete pFile;
		pFile	=	NULL;
		pServer->Close();
		delete pServer;
		pServer	=	NULL;

		pServer = pSession->GetHttpConnection(strServerName, dwHttpRequestFlags, nPort, m_strProxyUser, m_strProxyPasswd );
		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
		pFile->AddRequestHeaders(strHeader, HTTP_ADDREQ_FLAG_COALESCE);
		if( strPostData.GetLength() > 0 ) // post
		{
			pFile->SendRequestEx( DWORD(strPostData.GetLength()) );
			pFile->WriteString(strPostData);
			pFile->EndRequest();
		}
		else
		{
			pFile->SendRequest();
		}
	}

	*ppServer	=	pServer;
	*ppFile	=	pFile;
}

CString CHttpClient::GetNewLocation( CHttpFile * pFile )
{
	CString	strNewLocation;
	pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewLocation);

	int nPlace = strNewLocation.Find(_T("Location: "));
	if (nPlace == -1)
	{
		ThrowTearException( ERR_TEAR_REDIRECT );
	}

	strNewLocation = strNewLocation.Mid(nPlace + 10);
	nPlace = strNewLocation.Find('\n');
	if (nPlace > 0)
		strNewLocation = strNewLocation.Left(nPlace);
	return strNewLocation;
}

void CHttpClient::DoSecurityRequest( CHttpFile * pFile, CString &strPostData )
{
	DWORD dwPrompt;
	dwPrompt = pFile->ErrorDlg(NULL, ERROR_INTERNET_INCORRECT_PASSWORD,
		FLAGS_ERROR_UI_FILTER_FOR_ERRORS | FLAGS_ERROR_UI_FLAGS_GENERATE_DATA
		| FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL);

	// if the user cancelled the dialog, bail out
	if (dwPrompt != ERROR_INTERNET_FORCE_RETRY)
		ThrowTearException( ERR_TEAR_CANCEL );
	if( strPostData.GetLength() > 0 ) // post
	{
		try{
			pFile->SendRequestEx( DWORD(strPostData.GetLength()) );
			pFile->WriteString(strPostData);
			pFile->EndRequest();
		}
		catch( CInternetException *pp )
		{
			pp->Delete();
			// HTTP_STATUS_BAD_METHOD
			pFile->SendRequest();
		}
	}
	else
	{
		pFile->SendRequest();
	}
}

CString FormatProxyString( int nProxyType, LPCTSTR lpszProxyAddress, UINT nProxyPort )
{
	if( NULL == lpszProxyAddress || strlen(lpszProxyAddress) <= 0 )
		return "";
	CString strProxy;
	CString	strProxyType;
	if( CProxySocket::TypeSocks4 == nProxyType || CProxySocket::TypeSocks5 == nProxyType )
		strProxyType = "socks=";
	else if( CProxySocket::TypeHTTP == nProxyType )
		strProxyType = "http=";
	strProxy.Format( "%s%s:%d", strProxyType, lpszProxyAddress, nProxyPort );
	return strProxy;
}

DWORD CHttpClient::Request(LPCTSTR lpszURL, CString &strPostData,
						 CFile *pFileSave, CString *pstrResult,
						 PROGRESS_CALLBACK fnCallback, void *cookie )
{
	DWORD	dwRet	=	HTTP_STATUS_BAD_REQUEST;

	if( NULL == lpszURL || strlen(lpszURL) == 0 )
		return dwRet;

	int	nContentLength = 0;
	int nContentLengthLocal = 0;
	int nContentLengthFinished = 0;
	int nContentLengthTotal = 0;

	// prepare header
	CString	strHeader;
	CMapStringToString	mapHeader;
	if( pFileSave && pFileSave->GetPosition() > 0 )
	{
		nContentLengthFinished = (int)pFileSave->GetPosition();
		CString strRange;
		strRange.Format( "bytes=%u-", nContentLengthFinished );
		mapHeader.SetAt( szRange, strRange );
		
	}
	if( pstrResult && pstrResult->GetLength() > 0 )
	{
		nContentLengthFinished = pstrResult->GetLength();
		CString strRange;
		strRange.Format( "bytes=%u-", nContentLengthFinished );
		mapHeader.SetAt( szRange, strRange );
	}
	if( m_strCookie.GetLength() > 0 )
		mapHeader.SetAt( szCookieKey, m_strCookie );
	MakeHttpHeader( mapHeader, strHeader );

	// default type and flags
	DWORD dwHttpRequestFlags =	INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_RELOAD
								| INTERNET_FLAG_DONT_CACHE
								| INTERNET_FLAG_EXISTING_CONNECT; // | INTERNET_FLAG_KEEP_CONNECTION;
	
	CString	strProxy;
	if( !m_strProxyAddress.IsEmpty() )
		strProxy = FormatProxyString( m_nProxyType, m_strProxyAddress, m_nProxyPort );
	CInternetSession session(	szUserAgentValue, 1, m_nAccessType,
							strProxy, NULL, INTERNET_FLAG_DONT_CACHE );
	// 以下SetOption似乎不起作用
	if( !strProxy.IsEmpty() && !m_strProxyAddress.IsEmpty() )
	{
		session.SetOption( INTERNET_OPTION_PROXY_USERNAME, (LPVOID)(LPCTSTR)m_strProxyUser, m_strProxyUser.GetLength() );
		session.SetOption( INTERNET_OPTION_PROXY_PASSWORD, (LPVOID)(LPCTSTR)m_strProxyPasswd, m_strProxyPasswd.GetLength() );
	}
	session.SetOption( INTERNET_OPTION_RECEIVE_TIMEOUT, 300000 );
	session.SetOption( INTERNET_OPTION_SEND_TIMEOUT, 30000 );
	session.SetOption( INTERNET_OPTION_CONNECT_TIMEOUT, 30000 );

	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	try
	{
		// check to see if this is a reasonable URL
		DoOpenURL( lpszURL, dwHttpRequestFlags, strHeader, strPostData, &session, &pServer, &pFile, fnCallback, cookie );
		if( NULL == pServer || NULL == pFile )
			ThrowTearException( ERR_TEAR_INTERRUPTED );

		pFile->QueryInfoStatusCode(dwRet);

		if (dwRet == HTTP_STATUS_MOVED ||
			dwRet == HTTP_STATUS_REDIRECT ||
			dwRet == HTTP_STATUS_REDIRECT_METHOD)
		{
			CString strNewLocation = GetNewLocation( pFile );

			// close up the redirected site
			pFile->Close();
			delete pFile;
			pFile	=	NULL;
			pServer->Close();
			delete pServer;
			pServer	=	NULL;

			// progress callback
			if( fnCallback )
				fnCallback( PROG_REDIRECTING, 0, NULL, cookie );

			// open new url
			DoOpenURL( strNewLocation, dwHttpRequestFlags,
						strHeader, strPostData,
						&session, &pServer, &pFile, fnCallback, cookie );
			
			pFile->QueryInfoStatusCode(dwRet);
		}

		if (dwRet == HTTP_STATUS_PARTIAL_CONTENT)
			dwRet = HTTP_STATUS_OK;
		if (dwRet != HTTP_STATUS_OK)
			ThrowTearException( ERR_TEAR_INTERRUPTED );

		CString	strInfo;
		pFile->QueryInfo( HTTP_QUERY_SET_COOKIE, strInfo );
		pFile->QueryInfo( HTTP_QUERY_COOKIE, strInfo );
		if( strInfo.GetLength() )
			m_strCookie	=	strInfo;
		pFile->QueryInfo( HTTP_QUERY_CONTENT_LENGTH, strInfo );
		nContentLength	=	atol( strInfo );
		nContentLengthTotal = nContentLength + nContentLengthFinished;
		if( pstrResult && nContentLengthTotal > 0 )
			pstrResult->GetBuffer( nContentLengthTotal+5 );

		DWORD	dwCheckSum	=	0;
		BOOL	bHasCheckSum	=	FALSE;
		CString	strCheckSum;
		pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strCheckSum);
		int nPlace = strCheckSum.Find( szCheckSumKeySuffix );
		if ( -1 != nPlace )
		{
			strCheckSum = strCheckSum.Mid( nPlace+strlen(szCheckSumKeySuffix) );
			nPlace = strCheckSum.Find( '\n' );
			if( nPlace > 0 )
			{
				dwCheckSum = atol( strCheckSum.Left( nPlace ) );
				bHasCheckSum	=	TRUE;
			}
		}

		if( fnCallback )
			fnCallback( PROG_TRANSFERRING, 0, NULL, cookie );

		DWORD	dwCheckSumLocal	=	0;
		TCHAR sz[1028];
		int nRead = pFile->Read(sz+4, 1023);
		while (nRead > 0)
		{
			sz[4+nRead] = '\0';
			if( NULL != pFileSave )
				pFileSave->Write( sz+4, nRead );
			if( NULL != pstrResult )
				*pstrResult	+=	(TCHAR *)(sz+4);
			nContentLengthLocal	+=	nRead;

			if( fnCallback && nContentLengthTotal > 0 )
				fnCallback( PROG_PROGRESS, DWORD(STKLIB_MAXF_PROGRESS*(nContentLengthFinished+nContentLengthLocal)/nContentLengthTotal), NULL, cookie );

			if( bHasCheckSum )
			{
				*((DWORD *)sz)	=	dwCheckSumLocal;
				dwCheckSumLocal = CRC32( sz, nRead );
			}

			nRead = pFile->Read(sz+4, 1023);
		}

		if( pstrResult && nContentLengthTotal > 0 )
			pstrResult->ReleaseBuffer();

		if( (nContentLength > 0 && nContentLengthLocal != nContentLength)
				|| (bHasCheckSum && dwCheckSum != dwCheckSumLocal) )
			ThrowTearException( ERR_TEAR_DATATRANSFER );

		if( fnCallback )
			fnCallback( PROG_PROGRESS, STKLIB_MAX_PROGRESS, NULL, cookie );
	}
	catch (CInternetException* pEx)
	{
		// catch errors from WinINet
		if (HTTP_STATUS_OK == dwRet)
			dwRet	=	HTTP_STATUS_PARTIAL;
		TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);
		m_strLastErrorMessage	=	szErr;
		pEx->Delete();
	}
	catch (CTearException* pEx)
	{
		TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);
		m_strLastErrorMessage	=	szErr;
		pEx->Delete();
	}
	catch( CException * pEx )
	{
		TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);
		m_strLastErrorMessage	=	szErr;
		pEx->Delete();
	}

	if (pFile != NULL)
	{
		pFile->Close();
		delete pFile;
	}
	if (pServer != NULL)
	{
		pServer->Close();
		delete pServer;
	}
	session.Close();

	if(nContentLength > 0 && nContentLengthLocal != nContentLength)
		return Request( lpszURL, strPostData, pFileSave, pstrResult, fnCallback, cookie );

	return dwRet;
}
