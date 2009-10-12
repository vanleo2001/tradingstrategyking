/*
	All rights reserved.
*/

#include "stdafx.h"
#include "QSProfile.h"
#include "ProxySocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// CQSProfile

CQSProfile & AfxGetQSProfile()
{
	static CQSProfile s_qsprofile;
	return s_qsprofile;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQSProfile::CQSProfile()
{
	m_nAccessType		=	0;		// INTERNET_OPEN_TYPE_PRECONFIG
	m_nProxyType		=	CProxySocket::TypeHTTP;
	m_nProxyPort		=	80;		// INTERNET_DEFAULT_HTTP_PORT
	
	m_nCurrentServer	=	-1;
	m_bCycleConnect		=	TRUE;
}

CQSProfile::~CQSProfile()
{
}

char szQuoteServerProfileName[] = "QuoteServer.ini";

char szProfile_Proxy[] = "Proxy";
char szProfile_AccessType[] = "AccessType";
char szProfile_ProxyType[] = "ProxyType";
char szProfile_ProxyAddress[] = "ProxyAddress";
char szProfile_ProxyPort[] = "ProxyPort";
char szProfile_ProxyUser[] = "ProxyUser";
char szProfile_ProxyPasswd[] = "ProxyPasswd";

char szProfile_Server[] = "Server";
char szProfile_ServerNumber[] = "ServerNumber";
char szProfile_CurrentServer[] = "CurrentServer";
char szProfile_CycleConnect[] = "CycleConnect";

char szProfile_ServerFmt[] = "-Server%d";
char szProfile_Name[] = "Name";
char szProfile_Address[] = "Address";
char szProfile_Port[] = "Port";
char szProfile_User[] = "User";
char szProfile_Passwd[] = "Passwd";
char szProfile_SavePasswd[] = "SavePasswd";

void CQSProfile::LoadProfile()
{
	CString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	m_nCurrentServer = ::GetPrivateProfileInt( szProfile_Server, szProfile_CurrentServer, 0, sFileName );
	m_bCycleConnect = ::GetPrivateProfileInt( szProfile_Server, szProfile_CycleConnect, TRUE, sFileName );
	int nServerNumber = ::GetPrivateProfileInt( szProfile_Server, szProfile_ServerNumber, 0, sFileName );
	
	m_aQuoteServers.SetSize( 0, nServerNumber + 3 );
	char buffer[1024];
	for( int k=0; k<nServerNumber; k++ )
	{
		CString	sSession;
		sSession.Format( szProfile_ServerFmt, k );
		
		CQuoteServer	qs;

		memset( buffer, 0, sizeof(buffer) );
		::GetPrivateProfileString( sSession, szProfile_Name, "", buffer, sizeof(buffer)-2, sFileName );
		qs.m_strName = buffer;

		memset( buffer, 0, sizeof(buffer) );
		::GetPrivateProfileString( sSession, szProfile_Address, "", buffer, sizeof(buffer)-2, sFileName );
		qs.m_strAddress = buffer;
		
		qs.m_nPort = ::GetPrivateProfileInt( sSession, szProfile_Port, 8001, sFileName );

		memset( buffer, 0, sizeof(buffer) );
		::GetPrivateProfileString( sSession, szProfile_User, "", buffer, sizeof(buffer)-2, sFileName );
		qs.m_strUser = buffer;

		memset( buffer, 0, sizeof(buffer) );
		::GetPrivateProfileString( sSession, szProfile_Passwd, "", buffer, sizeof(buffer)-2, sFileName );
		qs.m_strPasswd = buffer;
		
		qs.m_bSavePasswd = ::GetPrivateProfileInt( sSession, szProfile_SavePasswd, TRUE, sFileName );
		
		if( qs.m_strName.IsEmpty() )
			qs.m_strName = qs.m_strAddress;
		if( !qs.m_strName.IsEmpty() )
			m_aQuoteServers.Add( qs );
	}

	LoadAliases();
}

void CQSProfile::StoreProfile()
{
	CString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	CString sTemp;
	sTemp.Format( "%d", m_aQuoteServers.GetSize() );
	::WritePrivateProfileString( szProfile_Server, szProfile_ServerNumber, sTemp, sFileName );
	sTemp.Format( "%d", m_nCurrentServer );
	::WritePrivateProfileString( szProfile_Server, szProfile_CurrentServer, sTemp, sFileName );
	sTemp.Format( "%d", m_bCycleConnect );
	::WritePrivateProfileString( szProfile_Server, szProfile_CycleConnect, sTemp, sFileName );
	::GetPrivateProfileInt( szProfile_Server, szProfile_ServerNumber, m_aQuoteServers.GetSize(), sFileName );

	for( int k=0; k<m_aQuoteServers.GetSize(); k++ )
	{
		CQuoteServer & qs = m_aQuoteServers.ElementAt(k);

		CString	sSession;
		sSession.Format( szProfile_ServerFmt, k );

		::WritePrivateProfileString( sSession, szProfile_Name, qs.m_strName, sFileName );
		::WritePrivateProfileString( sSession, szProfile_Address, qs.m_strAddress, sFileName );
		sTemp.Format( "%d", qs.m_nPort );
		::WritePrivateProfileString( sSession, szProfile_Port, sTemp, sFileName );
		::WritePrivateProfileString( sSession, szProfile_User, qs.m_strUser, sFileName );
		::WritePrivateProfileString( sSession, szProfile_Passwd, qs.m_strPasswd, sFileName );
		sTemp.Format( "%d", qs.m_bSavePasswd );
		::WritePrivateProfileString( sSession, szProfile_SavePasswd, sTemp, sFileName );
	}
}

static HKEY QSP_GetSectionKey( HKEY hKey, LPCTSTR szRegKeyGroup, LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp, LPCTSTR lpszVersion, LPCTSTR lpszSection )
{
	HKEY hGroupKey = NULL;
	HKEY hCompanyKey = NULL;
	HKEY hAppKey = NULL;
	HKEY hVersionKey = NULL;
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
	RegCreateKeyEx(hAppKey, lpszVersion, 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
		&hVersionKey, &dw);
	RegCloseKey(hAppKey);

	if( NULL == hVersionKey )
		return FALSE;
	RegCreateKeyEx(hVersionKey, lpszSection, 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
		&hSectionKey, &dw);
	RegCloseKey(hVersionKey);
	
	return hSectionKey;
}

static DWORD	QSP_RegQueryDWORD(HKEY hKey, LPCTSTR szRegKeyGroup, LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp, LPCTSTR lpszVersion, LPCTSTR lpszSection, LPCTSTR lpszEntry, DWORD dwDefault )
// hKey = HKEY_LOCAL_MACHINE, HKEY_CLASSES_ROOT, or so
{
	HKEY hSecKey = QSP_GetSectionKey(hKey, szRegKeyGroup, szRegKeyCompany, szRegKeyApp, lpszVersion, lpszSection);
	if (hSecKey == NULL)
		return dwDefault;
	DWORD dwValue;
	DWORD dwType, dwCount = 4;
	LONG lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
		(LPBYTE)&dwValue, &dwCount);
	RegCloseKey(hSecKey);
	if (lResult == ERROR_SUCCESS)
	{
		ASSERT(dwType == REG_DWORD);
		return dwValue;
	}
	return dwDefault;
}

static CString	QSP_RegQueryString(HKEY hKey, LPCTSTR szRegKeyGroup, LPCTSTR szRegKeyCompany, LPCTSTR szRegKeyApp, LPCTSTR lpszVersion, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault )
// hKey = HKEY_LOCAL_MACHINE, HKEY_CLASSES_ROOT, or so
{
	HKEY hSecKey = QSP_GetSectionKey(hKey, szRegKeyGroup, szRegKeyCompany, szRegKeyApp, lpszVersion, lpszSection);
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

static BOOL QSP_ParseProxyString( LPCTSTR lpszProxy, int & nProxyType, CString & rProxyAddress, UINT & nProxyPort )
{
	CString strProxy = lpszProxy;
	int nIndex1, nIndex2, nIndex3;

	char szSocks[] = "socks=";
	nIndex1 = strProxy.Find( szSocks );
	if( -1 != nIndex1 )
	{
		nIndex1 = nIndex1 + strlen(szSocks);
		nIndex2 = strProxy.Find( ":", nIndex1 );
		if( -1 != nIndex2 )
		{
			nIndex3 = strProxy.Find( ";", nIndex2 );
			if( -1 == nIndex3 ) nIndex3 = strProxy.GetLength();
			nProxyType = CProxySocket::TypeSocks5;
			rProxyAddress = strProxy.Mid( nIndex1, nIndex2-nIndex1 );
			nProxyPort = atol(strProxy.Mid( nIndex2+1, nIndex3-nIndex2-1) );
			return TRUE;
		}
	}

	char szHttp[] = "http=";
	nIndex1 = strProxy.Find( szHttp );
	if( -1 != nIndex1 )
	{
		nIndex1 = nIndex1 + strlen(szHttp);
		nIndex2 = strProxy.Find( ":", nIndex1 );
		if( -1 != nIndex2 )
		{
			nIndex3 = strProxy.Find( ";", nIndex2 );
			if( -1 == nIndex3 ) nIndex3 = strProxy.GetLength();
			nProxyType = CProxySocket::TypeHTTP;
			rProxyAddress = strProxy.Mid( nIndex1, nIndex2-nIndex1 );
			nProxyPort = atol(strProxy.Mid( nIndex2+1, nIndex3-nIndex2-1) );
			return TRUE;
		}
	}

	// All is the same	
	nIndex1 = 0;
	nIndex2 = strProxy.Find( ":", nIndex1 );
	if( -1 != nIndex2 )
	{
		nIndex3 = strProxy.Find( ";", nIndex2 );
		if( -1 == nIndex3 ) nIndex3 = strProxy.GetLength();
		nProxyType = CProxySocket::TypeSocks5;
		rProxyAddress = strProxy.Mid( nIndex1, nIndex2-nIndex1 );
		nProxyPort = atol(strProxy.Mid( nIndex2+1, nIndex3-nIndex2-1) );
		return TRUE;
	}

	return FALSE;
}

BOOL CQSProfile::GetCompoundConfig( int & nProxyType, CString & strProxyAddress, UINT & nProxyPort )
{
	int nAccessType = GetAccessType();
	nProxyType = GetProxyType();
	strProxyAddress = GetProxyAddress();
	nProxyPort = GetProxyPort();
	if( INTERNET_OPEN_TYPE_PRECONFIG == nAccessType )
	{
		DWORD dwProxyEnable = QSP_RegQueryDWORD( HKEY_CURRENT_USER, _T("Software"), _T("Microsoft"), _T("Windows"),
							_T("CurrentVersion"), _T("Internet Settings"), _T("ProxyEnable"), 0 );
		if( dwProxyEnable )
		{
			CString strProxy = QSP_RegQueryString( HKEY_CURRENT_USER, _T("Software"), _T("Microsoft"), _T("Windows"),
								_T("CurrentVersion"), _T("Internet Settings"), _T("ProxyServer"), "" );
			QSP_ParseProxyString( strProxy, nProxyType, strProxyAddress, nProxyPort );
		}
		else
			nProxyType = CProxySocket::TypeNone;
	}
	else if( INTERNET_OPEN_TYPE_DIRECT == nAccessType )
		nProxyType = CProxySocket::TypeNone;
	return TRUE;
}

int CQSProfile::GetAccessType( )
{
/*	if( INTERNET_OPEN_TYPE_DIRECT != m_nAccessType
		&& INTERNET_OPEN_TYPE_PRECONFIG != m_nAccessType
		&& INTERNET_OPEN_TYPE_PROXY != m_nAccessType )
		return INTERNET_OPEN_TYPE_DIRECT;
*/
	CString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	m_nAccessType = ::GetPrivateProfileInt( szProfile_Proxy, szProfile_AccessType, 0, sFileName );
	return m_nAccessType;
}

void CQSProfile::SetAccessType( int nAccessType )
{
	m_nAccessType	=	nAccessType;

	CString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	CString sTemp;
	sTemp.Format( "%d", m_nAccessType );
	::WritePrivateProfileString( szProfile_Proxy, szProfile_AccessType, sTemp, sFileName );
}

int CQSProfile::GetProxyType( )
{
	CString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	m_nProxyType = ::GetPrivateProfileInt( szProfile_Proxy, szProfile_ProxyType, 3, sFileName );
	return m_nProxyType;
}

void CQSProfile::SetProxyType( int nProxyType )
{
	m_nProxyType	=	nProxyType;
	
	CString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	CString sTemp;
	sTemp.Format( "%d", m_nProxyType );
	::WritePrivateProfileString( szProfile_Proxy, szProfile_ProxyType, sTemp, sFileName );
}

CString	CQSProfile::GetProxyAddress( )
{
	CString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	char szAddress[CProxySocket::MaxLenAddress+2];
	memset( szAddress, 0, sizeof(szAddress) );
	::GetPrivateProfileString( szProfile_Proxy, szProfile_ProxyAddress, "", szAddress, sizeof(szAddress)-2, sFileName );
	m_strProxyAddress	=	szAddress;
	return m_strProxyAddress;
}

void CQSProfile::SetProxyAddress( LPCTSTR lpszProxyAddress )
{
	m_strProxyAddress	=	lpszProxyAddress;
	
	CString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	::WritePrivateProfileString( szProfile_Proxy, szProfile_ProxyAddress, m_strProxyAddress, sFileName );
}

WORD CQSProfile::GetProxyPort( )
{
	CString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	m_nProxyPort = ::GetPrivateProfileInt( szProfile_Proxy, szProfile_ProxyPort, 80, sFileName );
	return m_nProxyPort;
}

void CQSProfile::SetProxyPort( WORD nPort )
{
	m_nProxyPort	=	nPort;
	
	CString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	CString sTemp;
	sTemp.Format( "%d", m_nProxyPort );
	::WritePrivateProfileString( szProfile_Proxy, szProfile_ProxyPort, sTemp, sFileName );
}

CString	CQSProfile::GetProxyUser( )
{
	CString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	char szUser[CProxySocket::MaxLenUser+2];
	memset( szUser, 0, sizeof(szUser) );
	::GetPrivateProfileString( szProfile_Proxy, szProfile_ProxyUser, "", szUser, sizeof(szUser)-2, sFileName );
	m_strProxyUser		=	szUser;
	return m_strProxyUser;
}

void CQSProfile::SetProxyUser( LPCTSTR lpszProxyUser )
{
	m_strProxyUser	=	lpszProxyUser;

	CString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	::WritePrivateProfileString( szProfile_Proxy, szProfile_ProxyUser, m_strProxyUser, sFileName );
}

CString	CQSProfile::GetProxyPasswd( )
{
	CString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	char szPasswd[CProxySocket::MaxLenPasswd+2];
	memset( szPasswd, 0, sizeof(szPasswd) );
	::GetPrivateProfileString( szProfile_Proxy, szProfile_ProxyPasswd, "", szPasswd, sizeof(szPasswd)-2, sFileName );
	m_strProxyPasswd	=	szPasswd;
	return m_strProxyPasswd;
}

void CQSProfile::SetProxyPasswd( LPCTSTR lpszProxyPasswd )
{
	m_strProxyPasswd	=	lpszProxyPasswd;
	
	CString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	::WritePrivateProfileString( szProfile_Proxy, szProfile_ProxyPasswd, m_strProxyPasswd, sFileName );
}

static CString QSP_AfxGetFileDirectory( CString strPathName, CString strDefault )
{
	if( strPathName.IsEmpty() )
		return strDefault;

	CString	strResult	=	strPathName;
	if( strResult.GetLength() > 0 )
	{
		int	nIndex	=	strResult.ReverseFind( '\\' );
		if( -1 == nIndex )
			nIndex	=	strResult.ReverseFind( '/' );
		if( -1 == nIndex )
			strResult = strDefault;
		else
			strResult	=	strResult.Left( nIndex );
	}

	if( strResult.GetLength() <= 0 )
		return strDefault;

	int	nLen	=	strResult.GetLength();
	if( nLen > 0 && strResult[nLen-1] != '\\' && strResult[nLen-1] != '/' )
		strResult	+=	"\\";

	return strResult;
}

CString CQSProfile::GetWorkDirectory( )
{
	if( m_strWorkDirectory.IsEmpty() )
	{
		TCHAR szLongPathName[_MAX_PATH];
		::GetModuleFileName( AfxGetModuleHandle(), szLongPathName, _MAX_PATH);
		m_strWorkDirectory	=	QSP_AfxGetFileDirectory( szLongPathName, "" );
	}
	return m_strWorkDirectory;
}

int CQSProfile::GetCurrentServerIndex()
{
	if( m_nCurrentServer < 0 || m_nCurrentServer >= m_aQuoteServers.GetSize() )
		m_nCurrentServer = 0;
	if( 0 == m_aQuoteServers.GetSize() )
		m_nCurrentServer = -1;
	return m_nCurrentServer;
}

CQuoteServer CQSProfile::GetCurrentServer()
{
	CQuoteServer qs;
	int nIndex = GetCurrentServerIndex();
	if( nIndex >= 0 && nIndex < m_aQuoteServers.GetSize() )
		qs = m_aQuoteServers.ElementAt(nIndex);
	return qs;
}

void CQSProfile::SetCurrentServer( int nServer )
{
	m_nCurrentServer = nServer;
}

BOOL CQSProfile::GetCycleConnect()
{
	return m_bCycleConnect;
}

void CQSProfile::SetCycleConnect( BOOL bCycleConnect )
{
	m_bCycleConnect = bCycleConnect;
}

CQuoteServerArray & CQSProfile::GetQuoteServers( )
{
	return m_aQuoteServers;
}

int CQSProfile::AddQuoteServers( CQuoteServerArray & aNew )
{
	int nCount = 0;
	for( int i=0; i<aNew.GetSize(); i++ )
	{
		CQuoteServer & qs = aNew.ElementAt(i);
		int j;
		for( j=0; j<m_aQuoteServers.GetSize(); j++ )
		{
			if( 0 == qs.m_strName.CompareNoCase(m_aQuoteServers.ElementAt(j).m_strName)
				|| 0 == qs.m_strAddress.CompareNoCase(m_aQuoteServers.ElementAt(j).m_strAddress) )
			{
				m_aQuoteServers.SetAt( j, qs );
				nCount ++;
				break;
			}
		}
		if( j == m_aQuoteServers.GetSize() )
		{
			m_aQuoteServers.Add( qs );
			nCount ++;
		}
	}
	return nCount;
}

void SerializeAlias( CArchive & ar, CAliasArray & aAliases )
{
	char szFileMagic[]		=	"NETTSALIAS";
	DWORD	dwFileVersion	=	0x20040825;

	if( ar.IsStoring() )
	{
		ar.WriteString( szFileMagic );
		ar << dwFileVersion;

		ar << (int)aAliases.GetSize();
		for( int i=0; i<aAliases.GetSize(); i++ )
		{
			CAlias & a = aAliases.ElementAt(i);
			ar << a.m_strName;
			ar << a.m_strValue;
		}
	}
	else
	{
		TCHAR buffer[64];
		memset( buffer, 0, sizeof(buffer) );
		ar.ReadString( buffer, strlen(szFileMagic) );
		if( 0 != strncmp( szFileMagic, buffer, strlen(szFileMagic) ) )
			return;

		DWORD	dwVer;				
		ar >> dwVer;
		if( dwVer > dwFileVersion )
			return;

		int size;
		ar >> size;
		if( size < 0 || size > 10000 )	// too big
			return;

		aAliases.SetSize( 0, size+2 );
		for( int i=0; i<size; i++ )
		{
			CAlias a;
			ar >> a.m_strName;
			ar >> a.m_strValue;
			aAliases.Add( a );
		}
	}
}

CString GetNetTSAliasFileName( )
{
	TCHAR buffer[MAX_PATH];
	memset( buffer, 0, sizeof(buffer) );
	GetWindowsDirectory(buffer, sizeof(buffer));
	buffer[sizeof(buffer)-1] = 0;
	CString string = buffer;
	int nLen = string.GetLength();
	if( nLen > 0 && '\\' != string[nLen-1] )
		string	+=	'\\';
	return string + "NetTSAlias";
}

BOOL CQSProfile::StoreAliases( CAliasArray & aNew )
{
	m_aAliases.SetSize( 0, aNew.GetSize() + 2 );
	for( int i=0; i<aNew.GetSize(); i++ )
	{
		CAlias & alias = aNew.ElementAt(i);
		m_aAliases.Add( alias );
	}

	CFile file;
	if( file.Open( GetNetTSAliasFileName(), CFile::modeWrite | CFile::modeCreate ) )
	{
		CArchive ar( &file, CArchive::store );
		SerializeAlias( ar, m_aAliases );
		ar.Close();
		file.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CQSProfile::LoadAliases( )
{
	CFile file;
	if( file.Open( GetNetTSAliasFileName(), CFile::modeRead ) )
	{
		DWORD dwLen = (DWORD)file.GetLength();
		if( 0 == dwLen || dwLen > 102400 )	// too big
		{
			file.Close();
			return FALSE;
		}
		
		CArchive ar( &file, CArchive::load );
		SerializeAlias( ar, m_aAliases );
		ar.Close();
		file.Close();
		return TRUE;
	}
	return FALSE;
}

CString CQSProfile::GetAlias( LPCTSTR lpszName )
{
	for( int i=0; i<m_aAliases.GetSize(); i++ )
	{
		CAlias & alias = m_aAliases.ElementAt(i);
		if( 0 == alias.m_strName.CompareNoCase(lpszName) )
			return alias.m_strValue;
	}
	return lpszName;
}

