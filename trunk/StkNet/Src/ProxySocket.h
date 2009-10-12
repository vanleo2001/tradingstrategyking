// ProxySocket.h: interface for the CProxySocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROXYSOCKET_H__9BA73A9B_B605_4236_8AD0_80BD16B19FE2__INCLUDED_)
#define AFX_PROXYSOCKET_H__9BA73A9B_B605_4236_8AD0_80BD16B19FE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProxySocket : public CSocket
{
private:

#pragma pack(1)

	struct sock4req1 
	{ 
		char VN; 
		char CD; 
		unsigned short Port; 
		unsigned long IPAddr; 
		char other[1]; 
	}; 

	struct sock4ans1 
	{ 
		char VN; 
		char CD; 
	}; 

	struct sock5req1 
	{ 
		char Ver; 
		char nMethods; 
		char Methods[255]; 
	}; 

	struct sock5ans1 
	{ 
		char Ver; 
		char Method; 
	}; 

	struct sock5req2 
	{ 
		char Ver; 
		char Cmd; 
		char Rsv; 
		char Atyp; 
		char other[1]; 
	}; 

	struct sock5ans2 
	{ 
		char Ver; 
		char Rep; 
		char Rsv; 
		char Atyp; 
		char other[1]; 
	}; 

	struct authreq 
	{ 
		char Ver; 
		char Ulen; 
		char Name[255]; 
		char PLen; 
		char Pass[255]; 
	}; 

	struct authans 
	{ 
		char Ver; 
		char Status; 
	}; 
#pragma pack()
	
public:
	CProxySocket();
	virtual ~CProxySocket();

	enum MaxLen {
        MaxLenAddress = 256,
        MaxLenUser = 128,
		MaxLenPasswd = 128,
	};

	enum ProxyType {
		TypeNone = 0,
		TypeSocks4 = 1,
		TypeSocks5 = 2,
		TypeHTTP = 3,
	};

	virtual	CString GetLastErrorString( );

	virtual BOOL SetProxyInfo( int nProxyType = TypeNone, LPCTSTR lpszProxyAddress = NULL, UINT nProxyPort = 1080,
								LPCTSTR lpszProxyUser = NULL, LPCTSTR lpszProxyPasswd = NULL );

	virtual BOOL ConnectThroughProxy( LPCTSTR lpszHostAddress, UINT nHostPort );

	virtual BOOL ConnectThroughSocks4(const SOCKADDR* lpSockAddr, int nSockAddrLen);
	virtual BOOL ConnectThroughSocks5(const SOCKADDR* lpSockAddr, int nSockAddrLen);
	virtual BOOL ConnectThroughHTTP(const SOCKADDR* lpSockAddr, int nSockAddrLen);

protected:
	CString m_strLastError;
	struct ProxyInfo
	{
		int		m_nProxyType;
		CString m_strProxyAddress;
		UINT	m_nProxyPort;
		CString m_strProxyUser;
		CString m_strProxyPasswd;
	} m_ProxyInfo;
};

#endif // !defined(AFX_PROXYSOCKET_H__9BA73A9B_B605_4236_8AD0_80BD16B19FE2__INCLUDED_)
