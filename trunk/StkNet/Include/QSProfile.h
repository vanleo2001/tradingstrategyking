/*
	All rights reserved.
*/

#if	!defined( __NETTS_QSPROFILE_H__ )
#define	__NETTS_QSPROFILE_H__

#ifndef STKNET_API
#define STKNET_API
#endif

class STKNET_API CQuoteServer
{
public:
	CQuoteServer() { }
	CQuoteServer( const CQuoteServer & src )
	{
		*this = src;
	}
	virtual ~CQuoteServer() { }
	
	CQuoteServer & operator = ( const CQuoteServer & src )
	{
		m_strName		=	src.m_strName;
		m_strAddress	=	src.m_strAddress;
		m_nPort			=	src.m_nPort;
		m_strUser		=	src.m_strUser;
		m_strPasswd		=	src.m_strPasswd;
		m_bSavePasswd	=	src.m_bSavePasswd;
		return *this;
	}

	CString	m_strName;
	CString	m_strAddress;
	UINT	m_nPort;
	CString	m_strUser;
	CString	m_strPasswd;
	BOOL	m_bSavePasswd;
	
};

typedef CArray<CQuoteServer, CQuoteServer &>	CQuoteServerArray;

/***
	Alias a name to another
*/
class STKNET_API CAlias
{
public:
	CAlias( ) { };
	CAlias( const CAlias &src ) { *this = src; }
	virtual ~CAlias( ) { };

	// virtual operation
	CAlias & operator = ( const CAlias & src )
	{
		m_strName		=	src.m_strName;
		m_strValue		=	src.m_strValue;
		return *this;
	}

	CString	m_strName;
	CString	m_strValue;
};

typedef	CArray<CAlias, CAlias &>	CAliasArray;

/***
	关于QuoteServer和Proxy的系统设置
*/
class STKNET_API CQSProfile // Quote Server Profile
{
public:
	CQSProfile();
	virtual ~CQSProfile();
	
	void		StoreProfile( );
	void		LoadProfile( );
	
	BOOL		GetCompoundConfig( int & nProxyType, CString & strProxyAddress, UINT & nProxyPort );

	int			GetAccessType( );
	void		SetAccessType( int nAccessType );
	int			GetProxyType( );
	void		SetProxyType( int nProxyType );
	CString		GetProxyAddress( );
	void		SetProxyAddress( LPCTSTR lpszProxyAddress );
	WORD		GetProxyPort( );
	void		SetProxyPort( WORD nPort );
	CString		GetProxyUser( );
	void		SetProxyUser( LPCTSTR lpszProxyUser );
	CString		GetProxyPasswd( );
	void		SetProxyPasswd( LPCTSTR lpszProxyUser );

	// Work Directory
	CString		GetWorkDirectory( );
	
	// QuoteServer
	int			GetCurrentServerIndex();
	CQuoteServer	GetCurrentServer();
	void		SetCurrentServer( int nServer );
	BOOL		GetCycleConnect();
	void		SetCycleConnect( BOOL bCycleConnect );
	CQuoteServerArray & GetQuoteServers( );
	int			AddQuoteServers( CQuoteServerArray & aNew );

	// alias
	BOOL		StoreAliases( CAliasArray & aNew );
	BOOL		LoadAliases( );
	CString		GetAlias( LPCTSTR lpszName );

protected:
	// Proxy
	int			m_nAccessType;
	int			m_nProxyType;
	CString		m_strProxyAddress;
	WORD		m_nProxyPort;
	CString		m_strProxyUser;
	CString		m_strProxyPasswd;
	
	CString		m_strWorkDirectory;

	// Quote Server
	int			m_nCurrentServer;
	BOOL		m_bCycleConnect;
	CQuoteServerArray	m_aQuoteServers;

	CAliasArray			m_aAliases;
};

STKNET_API CQSProfile & AfxGetQSProfile();

#endif // __NETTS_QSPROFILE_H__

