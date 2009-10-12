/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CStdatabase;
*/

#if	!defined( __STKLIB_DATABASE_H__ )
#define	__STKLIB_DATABASE_H__

#include	"StdAfx.h"
#include	"Stock.h"

#define	MAX_DATATYPENAME	128

class CDBType
{
public:
	CDBType( );

	int type;
	char name[MAX_DATATYPENAME];
};

#pragma	pack(1)

////////////////////////////////////////////////////////////////////
//	通用数据包的数据结构(*.STK)
typedef	struct tyday_fheader	{
	char	magic;
	char	code[6];
	char	magic2;
	char	name[8];
	DWORD	date;		//日期
	DWORD	datebegin;	//起始日期
	DWORD	dateend;	//终止日期
	DWORD	gendate;	//生成日期
	DWORD	gentime;	//生成时间
	DWORD	from;		//来源
	DWORD	sharecount;	//股票数
	DWORD	recordcount;//记录数
}TYDAY_FHEADER;

typedef	struct tyday_record	{
	char	magic;
	char	code[6];
	char	magic2;
	char	name[8];
	DWORD	date;		//日期
	DWORD	open;		//开盘(元/1000)
	DWORD	high;		//最高价(元/1000)
	DWORD	low;		//最低价(元/1000)
	DWORD	close;		//收盘(元/1000)
	DWORD	amount;		//成交额(千元)
	DWORD	volume;		//成交量(手)
	DWORD	serial;		//记录序号
}TYDAY_RECORD;

////////////////////////////////////////////////////////////////////
//	分析家数据包的数据结构(*.DAD)
#define	FXJDAY_FHEADER_MAGIC	0x8C19FC33
typedef	struct fxjday_fheader	{
	DWORD	m_dwMagic;			// 33 FC 19 8C，老版本的Magic
	DWORD	m_dwUnknown;		// 未知
	DWORD	m_dwStockCount;		// 为本文件的股票数
	DWORD	m_dwReserved;		// = 0
}FXJDAY_FHEADER;

typedef	union fxjday_record	{
	struct
	{
		DWORD	m_dwMagic;		// = -1
		WORD	m_wMarket;		// 市场 SH, SZ
		char	m_szCode[6];
		DWORD	m_dwReserved[5];
	};
	struct
	{
		time_t	m_time;		//日期
		float	open;		//开盘(元/1000)
		float	high;		//最高价(元/1000)
		float	low;		//最低价(元/1000)
		float	close;		//收盘(元/1000)
		float	volume;		//成交量(手)
		float	amount;		//成交额(元)
		float	reserved;	//
	};
}FXJDAY_RECORD;

#pragma	pack()

BOOL convert_TYDAY_RECORD_to_KDATA( TYDAY_RECORD * precord, KDATA * pkd );
BOOL convert_TYDAY_RECORD_MIN_to_KDATA( TYDAY_RECORD * precord, KDATA * pkd );
BOOL convert_KDATA_to_TYDAY_RECORD( DWORD dwSerial, const char * name, KDATA * pkd, TYDAY_RECORD * precord );
BOOL convert_KDATA_to_TYDAY_RECORD_MIN( DWORD dwSerial, const char * name, KDATA * pkd, TYDAY_RECORD * precord );

BOOL convert_FXJDAY_RECORD_to_KDATA( DWORD dwMarket, LPCTSTR lpszCode, int nKType, FXJDAY_RECORD *precord, KDATA *pkd );

typedef BOOL (CALLBACK* PROGRESS_CALLBACK)(DWORD dwCode, DWORD dwProgress, LPCTSTR lpszMsg, VOID * cookie);

#define	PROG_PROGRESS			1
#define	STKLIB_MAX_PROGRESS		10000
#define	STKLIB_MAXF_PROGRESS		10000.

class	CStockContainer;

/***
	读取K线数据的通用接口，各种格式的读取K线数据类必须从此类继承
*/
class IStStore
{
public:
	enum DBTypes {	// 数据格式类型，目前只支持typeSelfDB（自有格式）和typeQianlong（钱龙格式）
		dbtypeUnknown		=	0x01,
		dbtypeSelfDB		=	0x02,
		dbtypeQianlong		=	0x03,
		dbtypeShenglong		=	0x04,
	};

	static	BOOL IsValidDataType( int nType );
	static	int	GetSupportedDataType ( CDBType * pdbtype, int maxsize );
	static	IStStore * CreateStore( const char * rootpath, int nDBType = dbtypeUnknown );	// 给出根目录和类型，新建一个对象

	virtual	int IsOK( ) { return m_bIsOK; }
	virtual	const char * GetRootPath( );	// 得到当前对象的根目录

	virtual	int	GetDBType( )	=	0;		// 得到当前对象的数据类型
	virtual	const char * GetDBTypeName( )	=	0;	// 得到当前对象的数据类型名称
	virtual	int	GetMaxStockNumber( )	=	0;	// 得到股票数量
	virtual	int	LoadCodetable( CStockContainer & container )	=	0;	// 读取所有股票的信息
	virtual	int	StoreCodetable( CStockContainer & container )	=	0;	// 保存代码表
	virtual	int	LoadKDataCache( CStockContainer & container, PROGRESS_CALLBACK fnCallback, void *cookie, int nProgStart, int nProgEnd )	=	0;	// 读取所有股票的最近日线数据缓冲
	virtual	int	LoadBasetable( CStockContainer & container )	=	0;	// 读取某一股票的财务资料表，包括每股收益，每股净资产等，见CBaseData
	virtual	int	StoreBasetable( CStockContainer & container )	=	0;	// 保存某一股票的财务资料表
	virtual	int	LoadBaseText( CStock *pstock )	=	0;					// 读取某一股票的基本资料文本
	virtual	int	LoadKData( CStock *pstock, int nKType )	=	0;			// 读取某一股票的某个周期的K线数据
	virtual	int	LoadDRData( CStock *pstock )	=	0;					// 读取某一股票的除权除息资料
	virtual	int StoreDRData( CStock *pstock )	=	0;					// 保存某一股票的除权除息资料
	virtual int	LoadReport( CStock *pstock )	=	0;					// 读取某一股票的行情刷新数据
	virtual int	LoadMinute( CStock *pstock )	=	0;					// 读取某一股票的行情分时数据
	virtual int	LoadOutline( CStock *pstock )	=	0;					// 读取某一股票的行情额外数据
	virtual int	StoreReport( REPORT * pReport, int nCount, BOOL bBigTrade )	=	0;		// 保存行情刷新数据
	virtual int	StoreMinute( MINUTE * pMinute, int nCount )	=	0;		// 保存行情分时数据
	virtual int	StoreOutline( OUTLINE * pOutline, int nCount )	=	0;	// 保存行情分时数据


	virtual	int	InstallCodetbl( const char * filename, const char *orgname )	=	0;	// 安装下载的代码表
	virtual	int	InstallCodetblBlock( const char * filename, const char *orgname )	=	0;	// 安装下载的板块表
	virtual	int	InstallCodetblFxjBlock( const char * filename, const char *orgname )	=	0;	// 安装下载的分析家板块表
	virtual	int	InstallKData( CKData & kdata, BOOL bOverwrite = FALSE )	=	0;			// 安装K线数据
	virtual	int InstallKDataTy( const char * stkfile, int nKType, PROGRESS_CALLBACK fnCallback, void *cookie )	=	0;	// 安装下载的K线通用格式数据包
	virtual	int InstallKDataFxj( const char * dadfile, int nKType, PROGRESS_CALLBACK fnCallback, void *cookie )	=	0;	// 安装下载的K线分析家格式通用数据包
	virtual	int InstallDRData( CDRData & drdata )	=	0;								// 安装除权除息数据
	virtual	int	InstallDRDataClk( const char * filename, const char *orgname )	=	0;	// 安装下载的除权除息数据，一只股票一个文件
	virtual	int	InstallDRDataFxj( const char * fxjfilename )	=	0;					// 安装分析家除权除息数据
	virtual	int	InstallBasetable( const char * filename, const char *orgname )	=	0;	// 安装财务数据
	virtual	int	InstallBasetableTdx( const char * filename )	=	0;					// 安装通达信财务数据
	virtual	int	InstallBasetableFxj( const char * filename )	=	0;					// 安装分析家财务数据
	virtual	int InstallBaseText( const char * filename, const char *orgname )	=	0;	// 安装下载的基本资料数据，一只股票一个文件
	virtual	int InstallBaseText( const char * buffer, int nLen, const char *orgname )	=	0;	// 安装基本资料数据
	virtual	int InstallNewsText( const char * filename, const char *orgname )	=	0;	// 安装新闻数据文件
	virtual	int InstallNewsText( const char * buffer, int nLen, const char *orgname )	=	0;	// 安装新闻数据

	virtual	BOOL GetFileName( CSPString &sFileName, int nDataType,
				CStockInfo * pInfo = NULL, int nKType = CKData::ktypeDay )	=	0;		// 得到某种数据的文件名称
protected:
	BOOL	m_bIsOK;
	char	m_szRootPath[1024];
};

#define	ERR_DB_MAX	1000

/***
	本地数据类，提供对磁盘数据的操作接口，包括股票信息，各种周期K线数据，基本
	资料，权息资料的读取和安装。
	
	使用方法如下：
	CStDatabase::CreateSelfDB( "./" );

	CStDatabase	db;
	db.SetRootPath( "./", IStStore::dbtypeSelfDB );

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

	注意：使用CStDatabase的子类CNetDatabase也可以实现上述功能，并且提供的功能
	更多，使用方法相同。
*/
class CStDatabase : public Object
{
public:
	// Constructors
	CStDatabase();
	virtual ~CStDatabase();

	// Flag values

	// Attributes

	// Operations
	static	BOOL CreateSelfDB( const char * rootpath );		// 创建所需目录
	static	BOOL IsValidDataType( int nType );				// 类型是否合法
	static	int	GetSupportedDataType ( CDBType * pdbtype, int maxsize );	// 得到支持的数据类型
	BOOL	SetRootPath( const char * rootpath, int nDBType = IStStore::dbtypeUnknown );	// 设定数据目录

	BOOL	IsOK( );	// 数据目录是否符合
	const char * GetRootPath( );	// 得到数据目录
	int	GetDBType( );				// 得到数据类型
	const char * GetDBTypeName( );	// 得到数据类型名称

	virtual	DWORD	GetSelfTempPath( char *szTempPath, int size );	// 得到临时目录

	virtual	BOOL GetLastErrorMessage(LPTSTR lpszError, UINT nMaxError);	// 得到最近错误
	virtual	int	GetMaxStockNumber( );		// 得到股票数量
	virtual	int	LoadCodetable( CStockContainer &container );	// 读取所有股票的信息
	virtual	int	StoreCodetable( CStockContainer & container );	// 保存代码表
	virtual	int	LoadKDataCache( CStockContainer &container, PROGRESS_CALLBACK fnCallback, void *cookie, int nProgStart, int nProgEnd );	// 读取所有股票的最近日线数据缓冲
	virtual	int	LoadBasetable( CStockContainer & container );	// 读取某一股票的财务资料表，包括每股收益，每股净资产等，见CBaseData
	virtual	int	StoreBasetable( CStockContainer & container );	// 保存某一股票的财务资料表
	virtual	int	LoadBaseText( CStock *pstock );					// 读取某一股票的基本资料文本
	virtual	int	LoadKData( CStock *pstock, int nKType );		// 读取某一股票的某个周期的K线数据
	virtual int	LoadDRData( CStock *pstock );					// 读取某一股票的除权除息资料
	virtual	int StoreDRData( CStock *pstock );					// 保存某一股票的除权除息资料
	virtual int	LoadReport( CStock *pstock );					// 读取某一股票的行情刷新数据
	virtual int	LoadMinute( CStock *pstock );					// 读取某一股票的行情分时数据
	virtual int	LoadOutline( CStock *pstock );					// 读取某一股票的行情额外数据
	virtual int	StoreReport( REPORT * pReport, int nCount, BOOL bBigTrade );	// 保存行情刷新数据
	virtual int	StoreMinute( MINUTE * pMinute, int nCount );	// 保存行情分时数据
	virtual int	StoreOutline( OUTLINE * pOutline, int nCount );	// 保存行情分时数据

	virtual	int	InstallCodetbl( const char * filename, const char *orgname );		// 安装下载的代码表
	virtual	int	InstallCodetblBlock( const char * filename, const char *orgname );	// 安装下载的板块表
	virtual	int	InstallCodetblFxjBlock( const char * filename, const char *orgname );	// 安装下载的分析家板块表
	virtual	int	InstallKData( CKData & kdata, BOOL bOverwrite = FALSE );			// 安装K线数据
	virtual	int InstallKDataTy( const char * stkfile, int nKType, PROGRESS_CALLBACK fnCallback, void *cookie );	// 安装下载的K线通用格式数据
	virtual	int InstallKDataFxj( const char * dadfile, int nKType, PROGRESS_CALLBACK fnCallback, void *cookie );	// 安装下载的K线分析家格式数据
	virtual	int InstallDRData( CDRData & drdata );									// 安装除权除息数据
	virtual	int	InstallDRDataClk( const char * filename, const char *orgname );		// 安装下载的除权除息数据，一只股票一个文件
	virtual	int	InstallDRDataFxj( const char * fxjfilename );						// 安装分析家除权除息数据
	virtual	int	InstallBasetable( const char * filename, const char *orgname );		// 安装财务数据
	virtual	int	InstallBasetableTdx( const char * filename );						// 安装通达信财务数据
	virtual	int	InstallBasetableFxj( const char * filename );						// 安装分析家财务数据
	virtual	int InstallBaseText( const char * filename, const char *orgname );		// 安装下载的基本资料数据，一只股票一个文件
	virtual	int InstallBaseText( const char * buffer, int nLen, const char *orgname );		// 安装基本资料数据
	virtual	int InstallNewsText( const char * filename, const char *orgname );		// 安装新闻数据文件
	virtual	int InstallNewsText( const char * buffer, int nLen, const char *orgname );		// 安装新闻数据


	// assistant operation
	BOOL	AddAssistantRootPath( const char * rootpath, int nDBType = IStStore::dbtypeUnknown );	// 指定另外一个附加数据源的目录，自动读取
	void	RemoveAssistant( const char * rootpath );	// 移除另外一个附加数据源目录
	void	RemoveAllAssistant(  );						// 移除所有附加数据源目录

	static CSPTime	GetTimeInitial( );		// 得到初始数据日期
	BOOL	GetTimeLocalRange( CSPTime *ptmLatest, CSPTime * ptmPioneer, CSPTime * ptmInitial );	// 得到本地数据日期区间
	BOOL	GetNeedDownloadRange( CStockInfo &info, CSPTime tmBegin, CSPTime tmEnd, CSPTime &tmDLBegin, CSPTime &tmDLEnd );	// 得到需要下载的数据日期区间

	// 将钱龙格式K线数据 加入 通用数据包
	static	int		AppendToTyData( const char *code, const char *name, int nKType, LPCTSTR lpszKFile, LPCTSTR lpszTyDataFile, time_t tmBegin, time_t tmEnd );

protected:
	IStStore	* m_pStore;
	CSPPtrArray	m_aptrAssistant;
};

#endif	//__STKLIB_DATABASE_H__

