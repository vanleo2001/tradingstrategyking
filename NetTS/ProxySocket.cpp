// ProxySocket.cpp: implementation of the CProxySocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <atlconv.h>

#include "ProxySocket.h"
#include "base64.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProxySocket::CProxySocket()
{
	m_ProxyInfo.m_nProxyType = TypeNone;
}

CProxySocket::~CProxySocket()
{
}

CString CProxySocket::GetLastErrorString( )
{
	if( !m_strLastError.IsEmpty() )
		return m_strLastError;
	int nErrorCode = CAsyncSocket::GetLastError();
	CString strFmt = AfxModuleLoadString( IDS_PROXYSOCKET_ERRCODEFMT ); // "服务器连接错误(%d)。";
	CString strResult;
	strResult.Format( strFmt, nErrorCode );
	return strResult;
}

BOOL CProxySocket::SetProxyInfo( int nProxyType, LPCTSTR lpszProxyAddress, UINT nProxyPort,
								LPCTSTR lpszProxyUser, LPCTSTR lpszProxyPasswd )
{
	m_ProxyInfo.m_nProxyType = nProxyType;
	m_ProxyInfo.m_strProxyAddress = lpszProxyAddress;
	m_ProxyInfo.m_nProxyPort = nProxyPort;
	m_ProxyInfo.m_strProxyUser = lpszProxyUser;
	m_ProxyInfo.m_strProxyPasswd = lpszProxyPasswd;
	return TRUE;
}

BOOL CProxySocket::ConnectThroughProxy( LPCTSTR lpszHostAddress, UINT nHostPort )
{
	USES_CONVERSION;

	ASSERT(lpszHostAddress != NULL);

	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

	LPSTR lpszAscii = T2A((LPTSTR)lpszHostAddress);
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(lpszAscii);

	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(lpszAscii);
		if (lphost != NULL)
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
	}

	sockAddr.sin_port = htons((u_short)nHostPort);

	if( TypeSocks4 == m_ProxyInfo.m_nProxyType )
		return ConnectThroughSocks4((SOCKADDR*)&sockAddr, sizeof(sockAddr));
	else if( TypeSocks5 == m_ProxyInfo.m_nProxyType )
		return ConnectThroughSocks5((SOCKADDR*)&sockAddr, sizeof(sockAddr));
	else if( TypeHTTP == m_ProxyInfo.m_nProxyType )
		return ConnectThroughHTTP((SOCKADDR*)&sockAddr, sizeof(sockAddr));
	else
		return Connect((SOCKADDR*)&sockAddr, sizeof(sockAddr));
}

BOOL CProxySocket::ConnectThroughSocks4(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{
	PSOCKADDR_IN paddr_in = (PSOCKADDR_IN)lpSockAddr;

	//通过Socks4方式代理 
	if( !Connect( m_ProxyInfo.m_strProxyAddress,m_ProxyInfo.m_nProxyPort) ) 
	{ 
		m_strLastError = AfxModuleLoadString( IDS_PROXYSOCKET_ERRCONNECTPROXY ); // _T("不能连接到代理服务器！"); 
		Close(); 
		return FALSE; 
	} 
	char buffer[128]; 
	memset(buffer,0,sizeof(buffer)); 
	struct sock4req1 *m_proxyreq; 
	m_proxyreq = (struct sock4req1 *)buffer;
	m_proxyreq->VN = 4;
	m_proxyreq->CD = 1;
	m_proxyreq->Port = paddr_in->sin_port; // htons(GetPort());
	m_proxyreq->IPAddr = paddr_in->sin_addr.s_addr; // inet_addr(GetServerHostName());
	Send(buffer,9);
	struct sock4ans1 *m_proxyans; 
	m_proxyans = (struct sock4ans1 *)buffer;
	memset(buffer,0,sizeof(buffer)); 
	int nReceiveLen = Receive(buffer,sizeof(buffer)); 
	if(m_proxyans->VN != 0 || m_proxyans->CD != 90) 
	{ 
		m_strLastError = AfxModuleLoadString( IDS_PROXYSOCKET_ERRCONNECTTHROUGHPROXY ); // _T("通过代理连接主站不成功！"); 
		Close(); 
		return FALSE; 
	}
	return TRUE;
}

BOOL CProxySocket::ConnectThroughSocks5(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{
	PSOCKADDR_IN paddr_in = (PSOCKADDR_IN)lpSockAddr;

	//通过Socks5方式代理 
	if( !Connect( m_ProxyInfo.m_strProxyAddress,m_ProxyInfo.m_nProxyPort) ) 
	{ 
		m_strLastError = AfxModuleLoadString( IDS_PROXYSOCKET_ERRCONNECTPROXY ); // _T("不能连接到代理服务器！"); 
		Close(); 
		return FALSE; 
	} 
	char buffer[1024]; 
	struct sock5req1 *m_proxyreq1; 
	m_proxyreq1 = (struct sock5req1 *)buffer; 
	m_proxyreq1->Ver = 5; 
	m_proxyreq1->nMethods = 2; 
	m_proxyreq1->Methods[0] = 0; 
	m_proxyreq1->Methods[1] = 2; 
	Send(buffer,4); 
	struct sock5ans1 *m_proxyans1; 
	m_proxyans1 = (struct sock5ans1 *)buffer; 
	memset(buffer,0,sizeof(buffer)); 
	int nReceiveLen = Receive(buffer,sizeof(buffer)); 
	if(m_proxyans1->Ver != 5 || (m_proxyans1->Method!=0 && m_proxyans1->Method!=2)) 
	{ 
		m_strLastError = AfxModuleLoadString( IDS_PROXYSOCKET_ERRCONNECTTHROUGHPROXY ); // _T("通过代理连接主站不成功！"); 
		Close(); 
		return FALSE; 
	} 
	if(m_proxyans1->Method == 2)
	{
		int nUserLen = strlen(m_ProxyInfo.m_strProxyUser);
		int nPassLen = strlen(m_ProxyInfo.m_strProxyPasswd);
		struct authreq *m_authreq;
		m_authreq = (struct authreq *)buffer;
		m_authreq->Ver = 1;
		m_authreq->Ulen = nUserLen;
		strcpy(m_authreq->Name,m_ProxyInfo.m_strProxyUser);
		m_authreq->Name[nUserLen] = nPassLen;
		strcpy(&(m_authreq->Name[nUserLen+1]),m_ProxyInfo.m_strProxyPasswd);

		Send(buffer,3+nUserLen+nPassLen);
		struct authans *m_authans;
		m_authans = (struct authans *)buffer;
		memset(buffer,0,sizeof(buffer));
		nReceiveLen = Receive(buffer,sizeof(buffer));
		if(m_authans->Ver != 1 || m_authans->Status != 0)
		{
			m_strLastError = AfxModuleLoadString( IDS_PROXYSOCKET_ERRAUTH ); // _T("代理服务器用户验证不成功！");
			Close();
			return FALSE;
		}
	}
	struct sock5req2 *m_proxyreq2; 
	m_proxyreq2 = (struct sock5req2 *)buffer;
	m_proxyreq2->Ver = 5; 
	m_proxyreq2->Cmd = 1; 
	m_proxyreq2->Rsv = 0; 
	m_proxyreq2->Atyp = 1; 
	unsigned short port = paddr_in->sin_port; // htons(GetPort());
	unsigned long tmpLong = paddr_in->sin_addr.s_addr; // inet_addr(GetServerHostName());
	memcpy(m_proxyreq2->other,&tmpLong,4); 
	memcpy(m_proxyreq2->other+4,&port,2);
	Send(buffer,sizeof(struct sock5req2)+5); 
	struct sock5ans2 *m_proxyans2; 
	memset(buffer,0,sizeof(buffer)); 
	m_proxyans2 = (struct sock5ans2 *)buffer;
	nReceiveLen = Receive(buffer,sizeof(buffer)); 
	if(m_proxyans2->Ver != 5 || m_proxyans2->Rep != 0) 
	{ 
		m_strLastError = AfxModuleLoadString( IDS_PROXYSOCKET_ERRCONNECTTHROUGHPROXY ); // _T("通过代理连接主站不成功！"); 
		Close(); 
		return FALSE; 
	}
	return TRUE;
}

BOOL CProxySocket::ConnectThroughHTTP(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{
	PSOCKADDR_IN paddr_in = (PSOCKADDR_IN)lpSockAddr;

	//通过HTTP方式代理 
	if( !Connect( m_ProxyInfo.m_strProxyAddress,m_ProxyInfo.m_nProxyPort) ) 
	{ 
		m_strLastError = AfxModuleLoadString( IDS_PROXYSOCKET_ERRCONNECTPROXY ); // _T("不能连接到代理服务器！"); 
		Close(); 
		return FALSE; 
	} 

	char buffer[1024];

	CString strUPSrc = m_ProxyInfo.m_strProxyUser + ":" + m_ProxyInfo.m_strProxyPasswd;
	CString strUP;
	EncodeBase64( strUPSrc, strUP );
	memset(buffer,0,sizeof(buffer));
	sprintf(buffer,"CONNECT %s:%d HTTP/1.1\r\n\
User-Agent: Ninebulls-Stock/3.0\r\n\
Host: %s\r\n\
Pragma: no-cache\r\n\
Cache-Control: no-cache\r\n\
Connection: Keep-Alive\r\n\
Proxy-Connection: Keep-Alive\r\n\
Proxy-Authorization: Basic %s\r\n",
			inet_ntoa(paddr_in->sin_addr), ntohs(paddr_in->sin_port), inet_ntoa(paddr_in->sin_addr), strUP );

	Send(buffer,strlen(buffer));
	memset(buffer,0,sizeof(buffer)); 
	int nReceiveLen = Receive(buffer,sizeof(buffer));

	int httpvn1, httpvn2;
	int codenum;
	if (!sscanf(buffer, "HTTP/%d.%d %d", &httpvn1, &httpvn2, &codenum))
	{
		m_strLastError = AfxModuleLoadString( IDS_PROXYSOCKET_ERRVERSION ); // _T("代理服务器版本不匹配！");
		Close();
		return FALSE;
	}

	if( 407 == codenum )
	{
		m_strLastError = AfxModuleLoadString( IDS_PROXYSOCKET_ERRAUTH ); // _T("代理服务器用户验证不成功！");
		Close();
		return FALSE;
	}
	if( (codenum/100) == 5 )
	{
		m_strLastError = AfxModuleLoadString( IDS_PROXYSOCKET_ERRCONNECTTHROUGHPROXY ); // _T("通过代理连接主站不成功！"); 
		Close();
		return FALSE;
	}

	// Successful 2xx
	// if(strstr(buffer, "HTTP/1.0 200 Connection established") == NULL) //连接不成功
	if( (codenum/100) != 2 )
	{
		m_strLastError = AfxModuleLoadString( IDS_PROXYSOCKET_ERRCONNECTTHROUGHPROXY ); // _T("通过代理连接主站不成功！"); 
		Close();
		return FALSE;
	}
	return TRUE;
}
