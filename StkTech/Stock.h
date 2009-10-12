/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CKData
		class	CDRData
		class	CBaseData
		class	CReport
		class	CMinute
		class	COutline
		class	CStockInfo
		class	CStock
*/

#if	!defined( __STKLIB_STOCK_H__ )
#define	__STKLIB_STOCK_H__

#define	STKLIB_DATA_INVALID	0x7FFFFFFF

#define	STKLIB_DAYS_INONEYEAR	240
#define	STKLIB_DAYS_CACHE		60
#define	STKLIB_DAYS_CALCULATE	30
#define	STKLIB_AUTORESUMEDR_DATEBEGIN	20040519
#define	STKLIB_AUTORESUMEDR_LIMIT		11

#define	STKLIB_CODE_MAIN			"1A0001"
#define	STKLIB_CODE_MAINSZN			"399001"
#define	STKLIB_CODE_SZZS			"1A0001"	// 上证指数
#define	STKLIB_CODE_SZYBL			"1A0009"	// 上证180指数
#define	STKLIB_CODE_SZAG			"1A0002"	// 上证A股
#define	STKLIB_CODE_SZBG			"1A0003"	// 上证B股
#define	STKLIB_CODE_SZNCZ			"399001"	// 深证成指
#define	STKLIB_CODE_SZNZZ			"399106"	// 深证综指
#define	STKLIB_CODE_SZNCFAZ			"399002"	// 深证成份A指
#define	STKLIB_CODE_SZNCFBZ			"399003"	// 深证成份B指
#define	STKLIB_CODE_SZNFIRST		"000001"	// 深证第一支股票
#define	STKLIB_CODE_SHAFIRST		"600000"	// 上证第一支股票
#define	STKLIB_CODE_ZLDD			"000000"	// 主力大单


// some fixed length
#define	STKLIB_MAX_CODE			8
#define	STKLIB_MAX_CODE2		16	// 兼容较长股票代码
#define	STKLIB_MAX_NAME			16
#define	STKLIB_MAX_NAMEENU		80
#define	STKLIB_MAX_SHORTNAME	16
#define	STKLIB_MAX_DOMAIN		32
#define	STKLIB_MAX_PROVINCE		32

// some enum values

#pragma pack(1)

// 本头文件中定义的数值单位：
// 		价格及与金额有关的单位，如成交金额、主营收入、每股收益、总市值等：元
// 		成交量单位：手（=100股）

/////////////////////////////////////////////////////////////////////////////////////////////
// 股票静态数据结构

// K线数据结构	CStock::dataK
typedef struct kdata_t {
	DWORD	m_dwMarket;						// 证券市场类型,see CStock::StockMarket
	char	m_szCode[STKLIB_MAX_CODE];		// 证券代码,以'\0'结尾

	DWORD	m_date;				//日期	Format is XXMMDDHHMM for 5min, Format is YYYYMMDD for day
	time_t	m_time;				//时间　与m_date一致．如果是是某日，则小时分秒均为0

	float	m_fOpen;			// 开盘价（元）
	float	m_fHigh;			// 最高价（元）
	float	m_fLow;				// 最低价（元）
	float	m_fClose;			// 收盘价（元）
	float	m_fVolume;			// 成交量（股）
	float	m_fAmount;			// 成交金额（元）

	DWORD	m_dwAdvance;		// 仅指数有效
	DWORD	m_dwDecline;		// 仅指数有效

	DWORD	m_dwReserved;
} KDATA, *PKDATA;

// 除权数据结构 CStock::dataDR
typedef struct drdata_t
{
	DWORD	m_dwMarket;						// 证券市场类型,see CStock::StockMarket
	char	m_szCode[STKLIB_MAX_CODE];		// 证券代码,以'\0'结尾

	DWORD	m_date;				//日期	Format is XXMMDDHHMM for 5min, Format is YYYYMMDD for day
	time_t	m_time;				//时间　与m_date一致．如果是是某日，则小时分秒均为0

	float	m_fGive;			// 每股送
	float	m_fPei;				// 每股配
	float	m_fPeiPrice;		// 配股价,仅当 m_fPei!=0.0f 时有效
	float	m_fProfit;			// 每股红利

	DWORD	m_dwReserved;
} DRDATA, * PDRDATA;

// 财务信息数据结构 CStock::dataBasetbl
typedef struct basedata_t {
	DWORD	m_dwMarket;						// 证券市场类型,see CStock::StockMarket
	char	m_szCode[STKLIB_MAX_CODE2];		// 证券代码,以'\0'结尾
	
	char	m_szDomain[STKLIB_MAX_DOMAIN];		// 板块
	char	m_szProvince[STKLIB_MAX_PROVINCE];	// 省份

	time_t	m_time;						// 日期，time_t
	DWORD	m_date;				 		// 日期	Format is YYYYMMDD for base data
	LONG	m_reporttype;				// 报告类型：年报、中报、季报
	float	m_fErate_dollar;			// 当期美元汇率
	float	m_fErate_hkdollar;			// 当期港币汇率

	// ★偿债能力
	float	m_fRatio_liquidity;			// 流动比率
	float	m_fRatio_quick;				// 速动比率
	float	m_fVelocity_receivables;	// 应收帐款周率
	// ★经营能力
	float	m_fVelocity_merchandise;	// 存货周转率
	float	m_fMain_income;				// 主营业务收入
	float	m_fCash_ps;					// 每股净现金流量
	// ★盈利能力
	float	m_fProfit_margin;			// 主营业务利润率 %
	float	m_fNetasset_yield;			// 净资产收益率 %
	// ★资本结构
	DWORD	m_datebegin;				// 上市日期 Format is YYYYMMDD
	float	m_fShare_count_total;		// 总股本
	float	m_fShare_count_a;			// 流通A股
	float	m_fShare_count_b;			// 流通B股
	float	m_fShare_count_h;			// 流通H股
	float	m_fShare_count_national;	// 国有股
	float	m_fShare_count_corp;		// 法人股
	float	m_fProfit_psud;				// 每股未分配利润
	float	m_fAsset;					// 总资产
	float	m_fRatio_holderright;		// 股东权益比率
	float	m_fRatio_longdebt;			// 长期负债率
	float	m_fRatio_debt;				// 资产负债率
	// ★投资收益能力
	float	m_fNetasset_ps;				// 每股净资产
	float	m_fNetasset_ps_regulate;	// 调整每股净资产
	float	m_fEps;						// 每股收益
	float	m_fEps_deduct;				// 扣除后每股收益
	float	m_fNet_profit;				// 净利润
	float	m_fMain_profit;				// 主营业务利润
	float	m_fTotal_profit;			// 利润总额
	// ★增长率
	float	m_fProfit_inc;				// 主营利润增长率%
	float	m_fIncome_inc;				// 主营收入增长率%
	float	m_fAsset_inc;				// 总资产增长率%
	// ★技术数据统计值
	float	m_fYield_average;			// 平均收益率%
	float	m_fYield_stddev;			// 收益标准差%
	float	m_fBeite;					// β值

	DWORD	m_dwReserved[4];
}BASEDATA, *PBASEDATA;

// 股票静态数据结构结束
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
// 实时行情使用数据结构

// 行情刷新数据结构 CStock::dataReport
typedef struct report_t {
	DWORD	m_dwMarket;						// 证券市场类型,see CStock::StockMarket
	char	m_szCode[STKLIB_MAX_CODE];		// 证券代码,以'\0'结尾

	char	m_szName[STKLIB_MAX_NAME];		// 证券名称,以'\0'结尾

	time_t	m_time;						// 交易时间

	DWORD	m_dwFlag;					// 停牌标志

	float	m_fLast;					// 昨收（元）
	float	m_fOpen;					// 今开（元）
	float	m_fHigh;					// 最高（元）
	float	m_fLow;						// 最低（元）
	float	m_fNew;						// 最新（元）
	float	m_fVolume;					// 成交量（股）
	float	m_fAmount;					// 成交额（元）

	float	m_fBuyPrice[5];				// 申买价1,2,3,4（元）
	float	m_fBuyVolume[5];			// 申买量1,2,3,4（股）
	float	m_fSellPrice[5];			// 申卖价1,2,3,4（元）
	float	m_fSellVolume[5];			// 申卖量1,2,3,4（股）
} REPORT, *PREPORT;

// 行情分时数据结构 CStock::dataMinute
typedef struct minute_t {
	DWORD	m_dwType;	// 1 min, 5 min, 15 min, 30 min line

	DWORD	m_dwMarket;						// 证券市场类型,see CStock::StockMarket
	char	m_szCode[STKLIB_MAX_CODE];		// 证券代码,以'\0'结尾

	time_t	m_time;				// 交易时间
	float	m_fHigh;			// 最高价（元）
	float	m_fLow;				// 最低价（元）
	float	m_fNew;				// 最新价（元）
	float	m_fVolume;			// 成交量（股）
	float	m_fAmount;			// 成交额（元）
} MINUTE, *PMINUTE;

// 行情额外数据结构 CStock::dataOutline
typedef struct outline_t {
	time_t	m_time;

	DWORD	m_dwShHq;
	DWORD	m_dwSzHq;
	DWORD	m_dwShPriceUp;
	DWORD	m_dwShPriceDown;
	DWORD	m_dwSzPriceUp;
	DWORD	m_dwSzPriceDown;
	DWORD	m_dwShWbUp;
	DWORD	m_dwShWbDown;
	DWORD	m_dwSzWbUp;
	DWORD	m_dwSzWbDown;
	DWORD	m_dwShStockCount;
	DWORD	m_dwSzStockCount;
} OUTLINE, *POUTLINE;

// 综合排名数据结构 CStock::dataMultisort
//综合排名
typedef struct multisort_item_t {
	BYTE	m_type;					// stock's type
	char	m_code[6];				// stock code
	long	m_data1;
	long	m_data2;
} MULTISORT_ITEM;
typedef struct multisort_t {
	DWORD	m_class;				// 1-涨幅 2-跌幅 4-5分钟涨幅 8-5分钟跌幅 16-委比前六 32-委比后六
									// 64-震幅 128-量比 256-总金额
	DWORD	m_tag;					// 0xf0 00 06 00
	MULTISORT_ITEM	m_stocks[6];
} MULTISORT, *PMULTISORT;

// 股票代码 CStock::dataCode
typedef	struct stockcode {
	DWORD	m_dwMarket;						// 证券市场类型,see CStock::StockMarket
	char	m_szCode[STKLIB_MAX_CODE2];		// 证券代码,以'\0'结尾
} STOCKCODE, *PSTOCKCODE;

// 行情通用数据包
#define	STKLIB_COMMPACKET_TAG	'KPMC'
typedef struct commpacket_t {
	DWORD	m_dwTag;			// = STKLIB_COMMPACKET_TAG
	DWORD	m_dwDataType;		// see CStock::DataType
	DWORD	m_dwCount;

	union
	{
		REPORT *	m_pReport;		// 行情刷新数据
		MINUTE *	m_pMinute;		// 分时成交数据
		MULTISORT *	m_pMultisort;	// 综合排名数据
		OUTLINE *	m_pOutline;		// 附加数据
		KDATA *		m_pKdata;		// 补充历史日线数据
		DRDATA *	m_pDrdata;		// 补充权息资料
		STOCKCODE *	m_pStockcode;	// 股票代码
		void *		m_pData;
	};
} COMMPACKET, *PCOMMPACKET;

// 实时行情使用数据结构结束
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack()


#ifndef	STKLIB_API
#define	STKLIB_API	__declspec(dllimport)
#endif

/***
	除权除息数组类
*/
class STKLIB_API CDRData
{
public:
// Construction
	CDRData();
	CDRData( const CDRData &src );

// Attributes
	int		GetSize() const;
	int		GetUpperBound() const;
	void	SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void	FreeExtra();
	void	RemoveAll();

	// Accessing elements
	DRDATA	GetAt(int nIndex) const;
	void	SetAt(int nIndex, DRDATA newElement);
	DRDATA&	ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const DRDATA* GetData() const;
	DRDATA* GetData();

	// Potentially growing the array
	void	SetAtGrow(int nIndex, DRDATA newElement);
	int		Add(DRDATA newElement);

	// overloaded operator helpers
	DRDATA	operator[](int nIndex) const;
	DRDATA&	operator[](int nIndex);

	// Operations that move elements around
	void	InsertAt(int nIndex, DRDATA newElement, int nCount = 1);
	void	RemoveAt(int nIndex, int nCount = 1);
	void	InsertAt(int nStartIndex, CDRData* pNewArray);
	int		InsertDRDataSort( DRDATA newElement );
	void	Sort( );
	CDRData	& operator = ( const CDRData &src );
	void	Copy( const CDRData &src );
	BOOL	IsSameAs( CDRData * psrc );

// Implementation
protected:
	DRDATA* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~CDRData();
};

/***
	K线数据数组类
*/
class STKLIB_API CKData
{
public:
	// Constructors
	CKData();
	CKData( int ktype, int maindatatype = mdtypeClose );
	CKData( const CKData & src );

	CKData	& operator = ( const CKData &src );

	static	BOOL	IsDayOrMin( int nKType );

	// Flag values
	enum KTypes {
		ktypeNone		=	0x00,
		ktypeMin		=	0x01,
		ktypeMin5		=	0x01,
		ktypeMin15		=	0x02,
		ktypeMin30		=	0x03,
		ktypeMin60		=	0x04,
		ktypeDay		=	0x05,
		ktypeWeek		=	0x06,
		ktypeMonth		=	0x07,
		ktypeMax		=	0x07,
	};

	enum Formats {
		formatNone		=	0x00,
		formatMin		=	0x01,
		formatOriginal	=	0x01,
		formatXDRup		=	0x02,
		formatXDRdown	=	0x03,
		formatMax		=	0x03,
	};

	enum MaindataTypes {
		mdtypeNone		=	0x00,
		mdtypeMin		=	0x01,
		mdtypeClose		=	0x01,
		mdtypeOpen		=	0x02,
		mdtypeAverage	=	0x03,
		mdtypeMax		=	0x03,
	};

	// Attributes
	int		GetSize() const;
	int		GetUpperBound() const;
	void	SetSize(int nNewSize, int nGrowBy = -1);

	// Operations
	// Clean up
	void	FreeExtra();
	void	RemoveAll();

	// Accessing elements
	KDATA	GetAt(int nIndex) const;
	void	SetAt(int nIndex, KDATA newElement);
	KDATA&	ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const KDATA* GetData() const;
	KDATA* GetData();
	
	float	MaindataAt(int nIndex) const;	// 得到CKData的nIndex日的主数据，根据主数据类型不同，返回值可能是开盘价、收盘价或者平均价
	BOOL	DateAt(int nIndex,
					int &nYear, int &nMonth, int &nDay, int &nHour, int &nMinute ) const;
	BOOL	LatestDate(
					int &nYear, int &nMonth, int &nDay, int &nHour, int &nMinute ) const;
	DWORD	GetDate(int nIndex );
	DWORD	GetDateDay( int nIndex );
	int		GetIndexByDate( DWORD date );
	int		GetAboutIndexByDate( DWORD date );

	// Potentially growing the array
	void	SetAtGrow(int nIndex, KDATA newElement);
	int		Add(KDATA newElement);

	// overloaded operator helpers
	KDATA	operator[](int nIndex) const;
	KDATA&	operator[](int nIndex);

	// Operations that move elements around
	void	InsertAt(int nIndex, KDATA newElement, int nCount = 1);
	void	RemoveAt(int nIndex, int nCount = 1);
	void	InsertAt(int nStartIndex, CKData* pNewArray);
	int		InsertKDataSort( KDATA newElement );

	// Operation of Analysis
	BOOL	IsNewValue( int nIndex, BOOL bTopOrBottom, int nDays );
	BOOL	GetMinMaxInfo( int nStart, int nEnd, float * pfMin, float *pfMax );

// Implementation
public:
	virtual ~CKData();

#ifdef _DEBUG
	void AssertValid() const;
	void Dump() const;
#endif

public:
	// Extra operations
	void	SetKType( int ktype );
	void	AutoSetKType( );
	int		GetKType( );
	int		GetCurFormat( );
	void	SetDRData( CDRData &drdata );
	CDRData &GetDRData( );
	void	ChangeCurFormat( int format, DWORD dateAutoDRBegin, double dAutoDRLimit );
	void	SetMaindataType( int type );
	int		GetMaindataType( );
	void	Clear( );

	int		CompareLatestDate( CKData &kd );
	static	int Min5ToMin15( CKData &kdm5, CKData &kdm15 );
	static	int Min5ToMin30( CKData &kdm5, CKData &kdm30 );
	static	int Min5ToMin60( CKData &kdm5, CKData &kdm60 );
	static	int DayToMonth( CKData &kdday, CKData &kdmonth );
	static	int DayToWeek( CKData &kdday, CKData &kdweek );

	DWORD	ToDayDate( DWORD date );
	
	int		MergeKData( CKData * pother );
	int		FullFillKData( CKData & kdataMain, BOOL bFillToEnd );

	BOOL	IsAdjacentDays( int nIndex, int nDays );
	BOOL	GetDiff( double * pValue, DWORD dateCur, int nDays );
	BOOL	GetDiffPercent( double * pValue, DWORD dateCur, int nDays );
	BOOL	GetScope( double * pValue, DWORD dateCur, int nDays );
	BOOL	GetVolumeSum( double * pValue, DWORD dateCur, int nDays );
	BOOL	GetRatioVolume( double * pValue, DWORD dateCur, int nDays );
	BOOL	GetRS( double * pValue, DWORD dateCur, int nDays );
	BOOL	GetMA( double * pValue, int nIndex, int nDays );

protected:
	static int		ConvertKData( CKData &kdSrc, CKData &kdDest, int multiple );
	void	CopyData( const CKData &src );
	void	LoadDataOriginal( );
	void	StoreDataOriginal( );
	void	ConvertXDR( BOOL bUP, DWORD dateAutoDRBegin, double dAutoDRLimit );

	static	float	GetRatio( float fLastClose, DRDATA & dr );

protected:
	int		m_nKType;
	int		m_nCurFormat;
	int		m_nCurMaindataType;
	CDRData	m_drdata;

	struct kdata_t * m_pDataOriginal;
	int m_nSizeOriginal;     // # of elements (upperBound - 1)
	int m_nMaxSizeOriginal;  // max allocated
	struct kdata_t * m_pData;
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount
};

/***
	财务资料数组类，可以记录多年的基本资料
*/
class STKLIB_API CBaseData
{
public:
// Construction
	CBaseData();
	CBaseData( const CBaseData &src );

// Attributes
	int		GetSize() const;
	int		GetUpperBound() const;
	void	SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void	FreeExtra();
	void	RemoveAll();

	// Accessing elements
	BASEDATA	GetAt(int nIndex) const;
	void	SetAt(int nIndex, BASEDATA newElement);
	BASEDATA&	ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const BASEDATA* GetData() const;
	BASEDATA* GetData();

	// Potentially growing the array
	void	SetAtGrow(int nIndex, BASEDATA newElement);
	int		Add(BASEDATA newElement);

	// overloaded operator helpers
	BASEDATA	operator[](int nIndex) const;
	BASEDATA&	operator[](int nIndex);

	// Operations that move elements around
	void	InsertAt(int nIndex, BASEDATA newElement, int nCount = 1);
	void	RemoveAt(int nIndex, int nCount = 1);
	void	InsertAt(int nStartIndex, CBaseData* pNewArray);
	int		InsertBaseDataSort( BASEDATA newElement );
	void	Sort( );
	CBaseData	& operator = ( const CBaseData &src );
	void	Copy( const CBaseData &src );

// Implementation
protected:
	BASEDATA* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~CBaseData();
};

/***
	行情信息数据数组类
*/
class STKLIB_API CReport
{
public:
// Construction
	CReport();
	CReport( const CReport &src );

// Attributes
	int		GetSize() const;
	int		GetUpperBound() const;
	void	SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void	FreeExtra();
	void	RemoveAll();

	// Accessing elements
	REPORT	GetAt(int nIndex) const;
	void	SetAt(int nIndex, REPORT newElement);
	REPORT&	ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const REPORT* GetData() const;
	REPORT* GetData();

	// Potentially growing the array
	void	SetAtGrow(int nIndex, REPORT newElement);
	int		Add(REPORT newElement);

	// overloaded operator helpers
	REPORT	operator[](int nIndex) const;
	REPORT&	operator[](int nIndex);

	// Operations that move elements around
	void	InsertAt(int nIndex, REPORT newElement, int nCount = 1);
	void	RemoveAt(int nIndex, int nCount = 1);
	void	InsertAt(int nStartIndex, CReport* pNewArray);
	int		InsertReportSort( REPORT newElement );
	void	Sort( );
	void	RemoveDirty( );
	CReport	& operator = ( const CReport &src );
	void	Copy( const CReport &src );

	BOOL	GetMMLD( int nIndex, double *pdVolBuy, double *pdVolSell, double * pdVolDiff );	// 取得买卖力道指标值
	BOOL	GetMMLDMinMaxInfo( double *pdMin, double *pdMax );			// 取得买卖力道指标最大值最小值
	BOOL	StatBuySellEx( float * fSellPrice, float * fSellVolume, float * fBuyPrice, float * fBuyVolume, int nSize );

// Implementation
protected:
	REPORT* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~CReport();
};

/***
	行情分时数据数组类
*/
class STKLIB_API CMinute
{
public:
// Construction
	CMinute();
	CMinute( const CMinute &src );

// Attributes
	int		GetSize() const;
	int		GetUpperBound() const;
	void	SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void	FreeExtra();
	void	RemoveAll();

	// Accessing elements
	MINUTE	GetAt(int nIndex) const;
	void	SetAt(int nIndex, MINUTE newElement);
	MINUTE&	ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const MINUTE* GetData() const;
	MINUTE* GetData();

	// Potentially growing the array
	void	SetAtGrow(int nIndex, MINUTE newElement);
	int		Add(MINUTE newElement);

	// overloaded operator helpers
	MINUTE	operator[](int nIndex) const;
	MINUTE&	operator[](int nIndex);

	// Operations that move elements around
	void	InsertAt(int nIndex, MINUTE newElement, int nCount = 1);
	void	RemoveAt(int nIndex, int nCount = 1);
	void	InsertAt(int nStartIndex, CMinute* pNewArray);
	int		InsertMinuteSort( MINUTE newElement );
	void	Sort( );
	void	RemoveDirty( );
	CMinute	& operator = ( const CMinute &src );
	void	Copy( const CMinute &src );
	
	BOOL	StatVolumeInfo( double *pdVolNow, double *pdVolOuter, double *pdVolInner );	// 统计内盘外盘
	BOOL	GetDiffPercentMin5( double * pValue );
	BOOL	GetLBDKMinMaxInfo( double dVolAverage, double *pdMin, double *pdMax );	// 取得量比多空指标最大值最小值

	BOOL	GetIndexWave( double *pdWave, int nIndex );

	int		ToKData( CKData & kdata );

// Implementation
protected:
	MINUTE* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~CMinute();
};

/***
	行情额外信息数据数组类
*/
class STKLIB_API COutline
{
public:
// Construction
	COutline();
	COutline( const COutline &src );

// Attributes
	int		GetSize() const;
	int		GetUpperBound() const;
	void	SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void	FreeExtra();
	void	RemoveAll();

	// Accessing elements
	OUTLINE	GetAt(int nIndex) const;
	void	SetAt(int nIndex, OUTLINE newElement);
	OUTLINE&	ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const OUTLINE* GetData() const;
	OUTLINE* GetData();

	// Potentially growing the array
	void	SetAtGrow(int nIndex, OUTLINE newElement);
	int		Add(OUTLINE newElement);

	// overloaded operator helpers
	OUTLINE		operator[](int nIndex) const;
	OUTLINE&	operator[](int nIndex);

	// Operations that move elements around
	void	InsertAt(int nIndex, OUTLINE newElement, int nCount = 1);
	void	RemoveAt(int nIndex, int nCount = 1);
	void	InsertAt(int nStartIndex, COutline* pNewArray);
	int		InsertOutlineSort( OUTLINE newElement );
	void	Sort( );
	COutline	& operator = ( const COutline &src );
	void	Copy( const COutline &src );

// Implementation
protected:
	OUTLINE* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~COutline();
};

class CStDatabase;

/***
	单个股票信息类 CStock::dataInfo
*/
class STKLIB_API	CStockInfo
{
public:
	CStockInfo( );
	CStockInfo( const CStockInfo &src );

	// Operation
	void Clear( );
	CStockInfo & operator = ( const CStockInfo & si );

	BOOL	IsEqualTo( DWORD dwMarket, const char * szCode );
	DWORD	GetMarket( );
	void	SetMarket( DWORD dwMarket );
	const char * GetStockCode( );
	BOOL	SetStockCode( DWORD dwMarket, const char * szCode );
	LONG	GetType( );
	void	SetType( LONG t );
	void	ResolveTypeAndMarket( );
	const char * GetStockName( );
	void	SetStockName( const char * szName );
	const char * GetStockNameEnu( );
	void	SetStockNameEnu( const char * szNameEnu );
	const char * GetStockNameChs( );
	void	SetStockNameChs( const char * szName );
	const char * GetStockShortName( );
	void	SetStockShortName( const char *szShortName );
	const char * GetStockDomain( );
	void	SetStockDomain( const char * szDomain );
	const char * GetStockProvince( );
	void	SetStockProvince( const char * szProvince );

	BOOL	StatTechIndex( DWORD dwDate );	// 设定当前技术资料日期，Format is YYYYMMDD
	BOOL	StatBaseIndex( DWORD dwDate );	// 设定当前基本资料日期，Format is YYYYMMDD
	
	// Attributes
	BOOL	IsValidStock( );	// 股票信息是否正确
	BOOL	IsShangHai( );		// 上证股票
	BOOL	IsShenZhen( );		// 深证股票
	BOOL	IsIndex( );			// 是否指数
	BOOL	IsBond( );			// 是否债券
	BOOL	IsBondIndex( );		// 是否国债指数
	BOOL	IsFund( );			// 是否基金
	BOOL	IsRight( );			// 是否权证
	int		DigitBit( );		// 得到该股价格有几位小数位，基金和上海B股是三位

	// Variant
	BOOL	GetAverage( double *pValue );	// 当日成交均价
	BOOL	GetDiff( double *pValue, DWORD dateCur, int nDays );	// 涨跌
	BOOL	GetDiffPercent( double *pValue, DWORD dateCur, int nDays );	// 涨跌幅度
	BOOL	GetScope( double *pValue, DWORD dateCur, int nDays );	// 涨跌范围
	BOOL	GetDiffPercentMin5( double *pValue );

	BOOL	GetPE( double * pValue );		// 市盈率
	BOOL	GetPNetAsset( double *pValue );	// 市净率
	BOOL	GetPMainIncome( double * pValue );	// 市销率
	BOOL	GetRatioPCash( double * pValue );	// 价格现金流比

	BOOL	GetShareCurrency( double * pValue );	// 流通股数
	BOOL	GetRatioCurrency( double * pValue );	// 流通比率
	BOOL	GetRatioChangeHand( double * pValue, double dVolume );
	BOOL	GetRatioChangeHand( CKData &kdata, double * pValue, DWORD dateCur, int nDays );
	BOOL	GetRatioChangeHand( double * pValue, DWORD dateCur, int nDays );	// n日换手率
	BOOL	GetRatioVolume( double * pValue, DWORD dateCur, int nDays );		// n日量比
	BOOL	GetRS( double * pValue, DWORD dateCur, int nDays );					// n日相对强度
	BOOL	GetSellBuyRatio( double *pdRatio, double *pdDiff );					// 委比和委差

	BOOL	GetMarketValue( double * pValue );	// 市值
	BOOL	GetMarketValueA( double * pValue );	// A股市值
	BOOL	GetMarketValueB( double * pValue );	// B股市值

	BOOL	GetXiapu( double * pValue );		// 夏普风险指数

protected:
	// data from codetable
	DWORD	m_dwMarket;							// 证券市场类型,see CStock::StockMarket
	char	m_szCode[STKLIB_MAX_CODE2];			// 证券代码,以'\0'结尾

	LONG	m_type;								// see CStock::StockType
	char	m_szName[STKLIB_MAX_NAME];			// 名称
	char	m_szNameEnu[STKLIB_MAX_NAMEENU];		// 英文名称
	char	m_szShortname[STKLIB_MAX_SHORTNAME];	// 拼音简称

	char	m_szDomain[STKLIB_MAX_DOMAIN];		// 板块
	char	m_szProvince[STKLIB_MAX_PROVINCE];	// 省份

public:

	DWORD	m_datetech;	//日期	Format is XXMMDDHHMM for 5min, Format is YYYYMMDD for day

	float	m_fLast;			// 昨收（元）
	float	m_fOpen;			// 开盘（元）
	float	m_fHigh;			// 最高价（元）
	float	m_fLow;				// 最低价（元）
	float	m_fClose;			// 收盘（元）

	float	m_fVolume;			// 成交量(股)
	float	m_fAmount;			// 成交额(元)

	DWORD	m_dwAdvance;		// 仅指数有效
	DWORD	m_dwDecline;		// 仅指数有效

	float	m_fBuyPrice[5];
	float	m_fBuyVolume[5];
	float	m_fSellPrice[5];
	float	m_fSellVolume[5];

public:
	CKData		m_kdata;		// 保存最近AfxGetProfile().GetCacheDays()天K线数据
	CBaseData	m_basedata;		// 保存基本资料数据
	CMinute		m_minute;		// 最近5分钟的MINUTE分时数据
	REPORT		m_reportLatest;	// 保存最近的REPORT报价数据
	
public:
	// From Base Data
	DWORD	m_datebase;					//日期	Format is YYYYMMDD for base data
	LONG	m_reporttype;
	float	m_fErate_dollar;			// 当期美元汇率
	float	m_fErate_hkdollar;			// 当期港币汇率

	// ★偿债能力
	float	m_fRatio_liquidity;			// 流动比率
	float	m_fRatio_quick;				// 速动比率
	float	m_fVelocity_receivables;	// 应收帐款周率
	// ★经营能力
	float	m_fVelocity_merchandise;	// 存货周转率
	float	m_fMain_income;				// 主营业务收入
	float	m_fCash_ps;					// 每股净现金流量
	// ★盈利能力
	float	m_fProfit_margin;			// 主营业务利润率 %
	float	m_fNetasset_yield;			// 净资产收益率 %
	// ★资本结构
	DWORD	m_datebegin;				// 上市日期 Format is YYYYMMDD
	float	m_fShare_count_total;		// 总股本
	float	m_fShare_count_a;			// 流通A股
	float	m_fShare_count_b;			// 流通B股
	float	m_fShare_count_h;			// 流通H股
	float	m_fShare_count_national;	// 国有股
	float	m_fShare_count_corp;		// 法人股
	float	m_fProfit_psud;				// 每股未分配利润
	float	m_fAsset;					// 总资产
	float	m_fRatio_holderright;		// 股东权益比率 %
	float	m_fRatio_longdebt;			// 长期负债率 % 
	float	m_fRatio_debt;				// 资产负债率 %
	// ★投资收益能力
	float	m_fNetasset_ps;				// 每股净资产
	float	m_fNetasset_ps_regulate;	// 调整每股净资产
	float	m_fEps;						// 每股收益
	float	m_fEps_deduct;				// 扣除后每股收益
	float	m_fNet_profit;				// 净利润
	float	m_fMain_profit;				// 主营业务利润
	float	m_fTotal_profit;			// 利润总额
	// ★增长率
	float	m_fProfit_inc;				// 主营利润增长率%
	float	m_fIncome_inc;				// 主营收入增长率%
	float	m_fAsset_inc;				// 总资产增长率%
	// ★技术数据统计值
	float	m_fYield_average;			// 平均收益率%
	float	m_fYield_stddev;			// 收益标准差%
	float	m_fBeite;					// β值
};

/***
	单个股票的数据，包括股票信息，K线数据，基本资料，除权除息数据等。
*/
class CStock
{
public:
	// Constructors
	CStock( );
	CStock( CStDatabase * pDatabase );
	CStock( CStDatabase * pDatabase, DWORD dwMarket, const char * code );
	virtual	~CStock();

	// Flag values
	enum StockMarket {
		marketUnknown	=	'UKNW',	// 未知
		marketSHSE		=	'ESHS',	// 上海证券交易所
		marketSZSE		=	'ESZS',	// 深圳证券交易所
		marketCYSE		=	'ESYC',	// 中国大陆创业板市场
		marketCHNA		=	'ANHC',	// 中国大陆上海或深圳或创业板证券交易所
		marketHKEX		=	'XEKH',	// 香港联合交易所
		marketTBSE		=	'ESBT',	// 台北证券交易所
		marketTKSE		=	'EST',	// 东京证券交易所
		marketLSE		=	'ESL',	// 伦敦证券交易所
		marketFLKFSE	=	'FKLF',	// 法兰克福证券交易所
		marketNYSE		=	'ESYN',	// 纽约证券交易所
		marketNASDAQ	=	'QDSN',	// 纳斯达克证券交易所
	};

	enum DataType {
		dataUnknown		=	0x00,		// 未知数据
		dataInfo		=	0x01,		// 股票信息
		dataK			=	0x02,		// K线数据
		dataDR			=	0x03,		// 权息资料
		dataBasetable	=	0x04,		// 财务数据
		dataBasetext	=	0x05,		// F10资料
		dataNews		=	0x06,		// 新闻资讯
		dataReport		=	0x07,		// 行情刷新数据
		dataMinute		=	0x08,		// 行情分时数据
		dataOutline		=	0x09,		// 行情额外数据
		dataCode		=	0x0A,		// 股票代码数组
		dataMultisort	=	0x0B,		// 综合排名
		dataDetail		=	0x0C,		// 成交明细
	};

	enum StockType {
		typeNone		=	0x00,
		typeshIndex		=	0x10,	//	上海指数
		typeshA			=	0x11,	//	上海A股
		typeshB			=	0x12,	//	上海B股
		typeshBond		=	0x13,	//	上海债券
		typeshRight		=	0x14,	//	上海权证
		typeshBonus		=	0x15,	//	上海红利
		typeshAdmeasure	=	0x16,	//	上海配股
		typeshInfo		=	0x1F,	//	上海证交所信息
		typeszIndex		=	0x20,	//	深圳指数 0x20 
		typeszA			=	0x21,	//	深圳A股 0x21 
		typeszB			=	0x22,	//	深圳B股 0x22 
		typeszBond		=	0x23,	//	深圳债券 0x23 
		typeszRight		=	0x24,	//	深圳权证 0x24 
		typeszBonus		=	0x25,	//	深圳红利 0x25 
		typeszAdmeasure	=	0x26,	//	深圳配股 0x26 
		typeszInfo		=	0x2F,	//	深圳证交所信息 0x2f 
		typeGeneralInfo	=	0x05,	//	综合财经信息 0x05 
	};

	enum StockTypeMark {
		markShanghai	=	0x10,	// 上海
		markShenzhen	=	0x20,	// 深圳
		markGeneralInfo	=	0x05,	// 综合财经信息
	};

	enum ReportType	{
		reportUnknown	=	0x00,	//	未知
		reportQuarter	=	0x01,	//	第一季度季报
		reportMid		=	0x02,	//	中报
		reportQuarter3	=	0x03,	//	第三季度季报
		reportAnnals	=	0x04,	//	年报
	};

	enum MultisortType {
		classRise		=	0x01,	// 涨幅
		classFall		=	0x02,	// 跌幅
		claddRiseMin5	=	0x04,	// 5分钟涨幅
		classFallMin5	=	0x08,	// 5分钟跌幅
		classBSRatioAsc	=	0x10,	// 委比前六
		classBSRatioDesc=	0x20,	// 委比后六
		classDiff		=	0x40,	// 震幅
		classVolRatio	=	0x80,	// 量比
		classAmount		=	0x100,	// 总金额
	};

	static	double GetReportFactor( int nReportType );

	// Attributes
	void	SetDatabase( CStDatabase * pDatabase );
	CStDatabase * GetDatabase( );
	void	SetStockCode( DWORD dwMarket, const char * szCode );
	const char * GetStockCode( );
	BOOL	SetStockInfo( CStockInfo * pInfo );
	CStockInfo	&	GetStockInfo( );
	CKData	&		GetKData( int ktype );
	CKData	&		GetKDataMonth( );
	CKData	&		GetKDataWeek( );
	CKData	&		GetKDataDay( );
	CKData	&		GetKDataMin60( );
	CKData	&		GetKDataMin30( );
	CKData	&		GetKDataMin15( );
	CKData	&		GetKDataMin5( );
	CDRData	&		GetDRData( );
	CReport &		GetReport( );
	CMinute &		GetMinute( );
	COutline &		GetOutline( );
	char	*		GetBaseTextPtr( );
	int				GetBaseTextLength( );
	BOOL			AllocBaseTextMem( int nNewLength );

	// Operations
	void	Clear( );
	int		PrepareData( int nDataType = dataInfo, int nKType = CKData::ktypeDay, BOOL bReload = FALSE );
	int		PrepareBaseInfo( BOOL bReload = FALSE );
	int		ExtractKData( int nKType, BOOL bForced );
	int		MergeBaseText( CStock &stock );
	int		MergeKData( CStock &stock, int nKType );
	int		MergeDRData( CStock &stock );
	int		StoreDRData( );

protected:
	void	SetKDataTypes( );

protected:
	CStDatabase	* m_pDatabase;
	CStockInfo	m_stockinfo;
	
	CKData		m_kdMonth;
	CKData		m_kdWeek;
	CKData		m_kdDay;
	CKData		m_kdMin60;
	CKData		m_kdMin30;
	CKData		m_kdMin15;
	CKData		m_kdMin5;
	CDRData		m_drData;
	CReport		m_report;
	CMinute		m_minute;
	COutline	m_outline;
	int			m_nBaseTextLen;
	char *		m_pBaseText;
};


#ifdef	_SP_ENABLE_INLINES
#define	_STOCK_INLINE	inline
#include "Stock.inl"
#undef	_STOCK_INLINE
#endif

#endif	//__STKLIB_STOCK_H__
