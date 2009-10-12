/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CCodeTable;
		class	CNetDatabase;
*/

#ifndef	__ND_STKNET_NETDB_H__
#define __ND_STKNET_NETDB_H__

#include "XMLDoc.h"
#include "QSProfile.h"

class	CPackages;

/***
	数据包类，表示一个数据包
*/
class STKNET_API CPackage : public CXMLNode
{
public:
	CPackage( );
	CPackage( const CPackage &src );
	virtual ~CPackage( );

	enum PackageTypes {
		packageUnknown	=	0x00,
		packageCode		=	0x01,
		packageDay		=	0x02,
		packageMin5		=	0x03,
		packageDR		=	0x04,
		packageBasetext	=	0x05,
		packageBasetable=	0x06,
	};

	// virtual operation
	virtual	BOOL	SetAttribute( CMarkup & markup );

	CPackage & operator = ( const CPackage &src );
	BOOL operator<(CPackage& p) const;

	CString GetURL( CString strBasePath );	// 该数据包的URL
	BOOL	HasAlternateURL( ) const;		// 该数据包是否有替代URL
	CString	GetAlternateURL( CString strBasePath );		// 该数据包的替代URL

	BOOL	HasExpandPackage( ) const;		// 该数据包是否有扩展的数据包
	BOOL	ExpandYearPackage( CString strMonthfmt, CString strDayfmt, int nYear );	// 将年数据包扩展成12个月份分数据包
	BOOL	ExpandMonthPackage( CString strDayfmt, int nYear, int nMonth );			// 将月数据包扩展成日分数据包
	CPackages * GetExpandPackages( );		// 得到扩展数据包
	void	DeleteExpandPackages( );		// 释放扩展数据包
	
	CString		m_strURL;
	CString		m_strAlternateURL;
	int			m_nType;
	CSPTime		m_tmFrom;
	CSPTime		m_tmTo;
	int			m_nLength;
	CString		m_strDescript;
	BOOL		m_bIsZipped;
	BOOL		m_bIsTimefmt;

	// temp data
	BOOL		m_bDownloadOK;
	CString		m_strLocalFilename;
	CPackages * m_pExpandPackages;
};

typedef CArray<CPackage, CPackage &>	CPackageArray;

/***
	数据包数组
*/
class STKNET_API CPackages : public CPackageArray
{
public:
	CPackages( );
	virtual ~CPackages( );

	void	Clear( );
	BOOL	IsOK( );
	BOOL	SetRawXMLDocument( CString &strDoc );		// 设定原始XML文档，包含各种数据包信息
	virtual	BOOL	SetAttribute( CMarkup & markup );	// 设定数据包数组的综合信息
	BOOL	AddSmartAdditional( );		// 为没一个数据包自动加入扩展数据包
	BOOL	ExtractFromXMLDoc( );		// 获取数据包信息
	void	Sort( );					// 数据包按时间排序

	void	AppendPackages( CPackages * psrc );

	BOOL	GetLatestTime( CSPTime &tm, int packagetype );
	virtual	BOOL	GetLastErrorMessage(LPTSTR lpszError, UINT nMaxError);

public:
	BOOL			m_bIsOK;
	CXMLDocument	m_xmlDocument;

	CString			m_strVersion;
	int				m_nDataVersion;
	CString			m_strBasePath;
	CString			m_strYearfmt;
	CString			m_strMonthfmt;
	CString			m_strDayfmt;
	CString			m_strInfo;
	CString			m_strInfourl;
	CString			m_strLatestver;
};

/***
	产品服务器信息类，表示一个产品服务器
*/
class STKNET_API CDownloadServer : public CXMLNode
{
public:
	CDownloadServer( ) { };
	CDownloadServer( const CDownloadServer &src );
	virtual ~CDownloadServer( ) { };

	// virtual operation
	CDownloadServer & operator = ( const CDownloadServer & src );

	virtual	BOOL	SetAttribute( CMarkup & markup );
	BOOL	FromString( CString	string );
	CString	AsString( );

	CString	m_strAddress;
	UINT	m_nPort;
	CString	m_strDescript;
};

typedef	CArray<CDownloadServer, CDownloadServer &>	CDownloadServerArray;

BOOL	AliasSetAttribute( CMarkup & markup, CAlias & alias );
BOOL	QuoteServerSetAttribute( CMarkup & markup, CQuoteServer & qs );

/***
	产品服务器数组
*/
class STKNET_API CServers
{
public:
	CServers( );
	virtual ~CServers( );

	void	Clear( );
	BOOL	IsOK( );
	BOOL	SetRawXMLDocument( CString &strDoc );
	virtual	BOOL	SetAttribute( CMarkup & markup );
	BOOL	ExtractFromXMLDoc( );

	virtual	BOOL	GetLastErrorMessage(LPTSTR lpszError, UINT nMaxError);
	
	BOOL	SaveToProfile( );

public:
	BOOL			m_bIsOK;
	CXMLDocument	m_xmlDocument;

	CString			m_strVersion;
	
	CDownloadServerArray	m_aDownloadServers;
	CQuoteServerArray		m_aQuoteServers;
	CAliasArray				m_aAliases;
};

#define	ERR_NETDB_HTTPCLIENT	ERR_DB_MAX+1
#define	ERR_NETDB_PACKAGES		ERR_DB_MAX+2
#define	ERR_NETDB_SERVERS		ERR_DB_MAX+3
#define	ERR_NETDB_ZIPDLL		ERR_DB_MAX+4
#define	ERR_NETDB_ZIP			ERR_DB_MAX+5

#define	PROG_HTTPCONNECTTING	PROG_PROGRESS+1
#define	PROG_REQUESTSENT		PROG_PROGRESS+2
#define	PROG_REDIRECTING		PROG_PROGRESS+3
#define	PROG_TRANSFERRING		PROG_PROGRESS+4
#define	PROG_INSTALLPACKAGE		PROG_PROGRESS+5
#define	PROG_EXTRACTZIPFILES	PROG_PROGRESS+6
#define	PROG_ERRORPAC			PROG_PROGRESS+7

#define	PROG_NEWGROUP			PROG_PROGRESS+8
#define	PROG_NEWGROUPMSG		PROG_PROGRESS+9

/***
	网络数据类，提供对磁盘数据的操作接口，包括股票信息，各种周期K线数据，基本
	资料，权息资料的读取和安装。
	
	使用方法如下：
	CStDatabase::CreateSelfDB( "./" );

	CNetDatabase	db;
	db.SetRootPath( "./", IStStore::typeSelfDB );


	CStockContainer container;
	nLen	=	db.LoadCodetable( container );
	db.LoadBasetable( container );
	db.LoadKDataCache( container, NULL, NULL, 0, 0 );

	CStock	stock;
	stock.SetDatabase( &db );
	stock.PrepareData( CStock::dataK, CKData::ktypeDay );
	stock.PrepareData( CStock::dataDR );
	stock.PrepareData( CStock::dataK, m_nCurKType );
	CStockInfo	& info	=	stock.GetStockInfo();
	CKData	& kday	=	stock.GetKDataDay();
	CKData	& kdata	=	stock.GetKData(m_nCurKType);
	kday.SetDRData( stock.GetDRData() );
	kday.ChangeCurFormat( CKData::formatXDRdown, AfxGetProfile().GetAutoResumeDRBegin(), AfxGetProfile().GetAutoResumeDRLimit() );

	注意：与父类CStDatabase的用法相同
*/
class STKNET_API CNetDatabase : public CStDatabase
{
public:
	CNetDatabase();
	virtual ~CNetDatabase();

	BOOL SetServer( LPCTSTR lpszServer, INTERNET_PORT port = INTERNET_DEFAULT_HTTP_PORT );	// 设定服务器
	BOOL SetAccessType( int nAccessType, int nProxyType, LPCTSTR lpszProxyAddress, UINT nProxyPort,
								LPCSTR lpszProxyUser, LPCTSTR lpszProxyPasswd );	// 设定代理选项
	BOOL Login( LPCTSTR lpszRegCode, LPCTSTR lpszUserName, LPCTSTR lpszPasswd, LPCTSTR lpszVersion );	// 登陆服务器
	BOOL Logout( );		// 退出登陆

	BOOL	NetloadPackageInfo( PROGRESS_CALLBACK fnCallback, void *cookie );	// 从网络下载数据包信息文件(.xml)
	BOOL	NetloadServerInfo( PROGRESS_CALLBACK fnCallback, void *cookie );	// 从网络下载服务器信息文件(.xml)
	int		NetloadCodetbl( PROGRESS_CALLBACK fnCallback, void *cookie );		// 从网络下载股票代码表
	int		NetloadDayKLine( LPCTSTR lpszStockCode, CSPTime *ptmFrom, CSPTime *ptmTo, PROGRESS_CALLBACK fnCallback, void *cookie );		// 从网络下载日K线数据
	int		Netload5MinKLine( LPCTSTR lpszStockCode, CSPTime *ptmFrom, CSPTime *ptmTo, PROGRESS_CALLBACK fnCallback, void *cookie );	// 从网络下载5分钟K线数据
	int		NetloadDRData( LPCTSTR lpszStockCode, CSPTime *ptmFrom, CSPTime *ptmTo, PROGRESS_CALLBACK fnCallback, void *cookie );		// 从网络下载除权除息数据
	int		NetloadBasetext( LPCTSTR lpszStockCode, CSPTime *ptmFrom, CSPTime *ptmTo, PROGRESS_CALLBACK fnCallback, void *cookie );		// 从网络下载F10基本资料文本
	int		NetloadBasetable( LPCTSTR lpszStockCode, CSPTime *ptmFrom, CSPTime *ptmTo, PROGRESS_CALLBACK fnCallback, void *cookie );		// 从网络下载财务资料表

	int		NetloadPackage( int nPackageType, CSPTime &tmFrom, CSPTime &tmTo, PROGRESS_CALLBACK fnCallback, void *cookie );	// 根据类型和起止日期从网络下载数据包

	BOOL	GetLatestTimeNet( CSPTime &tm, int packagetype );		// 得到服务器上最近的数据日期

	CServers	& GetServers( );	// 得到所有服务器列表
	CPackages	& GetPackages( );	// 得到所有数据包列表

	// CStDatabase virtual functions
	virtual	BOOL	GetLastErrorMessage(LPTSTR lpszError, UINT nMaxError);	// 得到最近错误信息

	static	CString	GetProgressMessage(UINT nProgressCode);	// 得到进度说明
	
	virtual	int	GetMaxStockNumber( );		// 得到股票数量
	virtual	int	LoadCodetable( CStockContainer & container );	// 读取所有股票的信息
	virtual	int	LoadBaseText( CStock *pstock );					// 读取某一股票的基本资料文本
	virtual	int	LoadKData( CStock *pstock, int period );		// 读取某一股票的某个周期的K线数据
	virtual	int	LoadDRData( CStock *pstock );					// 读取某一股票的除权除息资料

	BOOL	InstallPackageEx( CPackage &pac, const char *zipfilename,
								PROGRESS_CALLBACK fnCallback, void *cookie );	// 安装数据包
	BOOL	InstallPackagePath( CPackage &pac, const char *path,
								PROGRESS_CALLBACK fnCallback, void *cookie,
								int nTotalCount, int nFinishCount, BOOL bDeleteAfterInstall = TRUE );
	BOOL	InstallPackage( CPackage &pac, const char *filename,
								PROGRESS_CALLBACK fnCallback, void *cookie );

	BOOL	EmptyTempDirectory( LPCTSTR lpszPath = NULL );						// 清空某个目录
	static	int		GetFileCount( const char * path, BOOL bRecurse = TRUE );	// 得到某个目录下的文件个数

protected:
	BOOL	GetTempFile( CString &rString );
	BOOL	RemoveTempFile( CString sFileName );
	BOOL	OpenTempFile( CFile &file );
	BOOL	CloseAndRemoveTempFile( CFile &file );
	BOOL	GetTempNewDirectory( CString &sTempNewDir );
	BOOL	RemoveTempNewDirectory( CString sTempNewDir );

protected:
	CPackages		m_packages;
	CServers		m_servers;
	CHttpClient		m_httpClient;
};

#endif //__ND_STKNET_NETDB_H__
