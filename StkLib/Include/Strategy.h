/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CRateParam;
		class	CSPTimeContainer;
		class	COpParam;
		class	CStockOwnContainer;
		class	COpRecordContainer;
		class	CAssetSerialContainer;
		class	CTechStock;
		class	CTechStockContainer;
		class	CStrategy;
*/

#if	!defined( __STKLIB_STRATEGY_H__ )
#define	__STKLIB_STRATEGY_H__

/////////////////////////////////////////////////////////////////////////////
// CRateParam
/***
	交易税费设置类
*/
class STKLIB_API CRateParam
{
public:
	CRateParam( );

	BOOL	IsValid( );
	void	SetDefault( );
	void	Serialize( CSPArchive &ar );

	double	GetRate( CStockInfo & info );

	double	m_dShaa;
	double	m_dShab;
	double	m_dShafund;
	double	m_dSzna;
	double	m_dSznb;
	double	m_dSznfund;
};

/////////////////////////////////////////////////////////////////////////////
// CSPTimeContainer

//template class __declspec(dllexport)	CSPArray<CSPTime, CSPTime &>;
typedef	CSPArray<CSPTime, CSPTime &>	CSPTimeArray;

class	STKLIB_API	CSPTimeContainer : public CSPTimeArray
{
public:
	CSPTimeContainer() {}
	virtual ~CSPTimeContainer()	{	RemoveAll();	}

	// for access CSPTimeArray
	virtual	int		Add( CSPTime &tm )		{	return CSPTimeArray::Add( tm );	}
	virtual	void	RemoveAll( )			{	CSPTimeArray::RemoveAll();	}
};

/////////////////////////////////////////////////////////////////////////////
// COpParam

STKLIB_API	CSPString	AfxGetLogicString( int nLogic );
STKLIB_API	CSPString	AfxGetStoreDivString( int nStoreDiv );
/***
	策略模拟操作条件类
*/
class STKLIB_API COpParam
{
public:
	COpParam( );

	// Attributes
	BOOL	IsValid( );
	void	SetDefault( );
	void	SetDefaultOfInvalidMember( );
	void	Serialize( CSPArchive &ar );

	// Operation
	BOOL	IsInTimeZones( CSPTime tm );
	CSPTime	GetBeginTime( );
	CSPTime	GetEndTime( );
	BOOL	GetNextTradeTime( CSPTime tmNow, CSPTime &tmNext );
	DWORD	GetProgress( CSPTime tmNow, DWORD dwProgressMax );

	enum storeDivs {
		storedivMin		=	0x01,
		storedivOnce	=	0x01,
		storedivTwice	=	0x02,
		storedivThird	=	0x03,
		storedivForth	=	0x04,
		storedivFifth	=	0x05,
		storedivSixth	=	0x06,
		storedivSeventh	=	0x07,
		storedivEighth	=	0x08,
		storedivNinth	=	0x09,
		storedivTenth	=	0x0A,
		storedivMax		=	0x0A,
	};

	enum Logical {
		logicMin	=	0x01,
		logicAnd	=	0x01,
		logicOr		=	0x02,
		logicMax	=	0x02,
	};

	int		m_nKType;
	int		m_nMaindataType;
	int		m_nBuyLogic;
	int		m_nBuyLimit;
	double	m_dBuyMulti;
	int		m_nSellLogic;
	int		m_nSellLimit;
	double	m_dSellMulti;
	CSPTimeContainer	m_atmBegin;
	CSPTimeContainer	m_atmEnd;
	long	m_nStartAmount;		//	元
	int		m_nStoreDiv;

	BOOL	m_bStopLosing;
	BOOL	m_bStopProfit;
	double	m_dStopLosing;
	double	m_dStopProfit;
	BOOL	m_bLongTrend;
	BOOL	m_bIndexTrend;
	int		m_nKTypeLong;
	int		m_nKTypeIndex;
};

/////////////////////////////////////////////////////////////////////////////
// stockown_t

#pragma pack(1)

typedef	struct stockown_t	{
	char	szCode[STKLIB_MAX_CODE];
	DWORD	dwShare;
	double	dBuyPrice;
	DWORD	dwMarket;
	char	szReserved[20];
} STOCKOWN, * LPSTOCKOWN;

//template class __declspec(dllexport)	CSPArray< STOCKOWN, STOCKOWN & >;
typedef	CSPArray< STOCKOWN, STOCKOWN & >	CStockOwnArray;

/***
	拥有股票数组
*/
class STKLIB_API CStockOwnContainer : public CStockOwnArray
{
public:
	virtual ~CStockOwnContainer()	{	RemoveAll();	}

	// for access CStockOwnArray
	virtual	void	RemoveAll( )			{	CStockOwnArray::RemoveAll();	}

	BOOL	AddStock( CStockInfo & info, DWORD dwShare, double dBuyPrice );
	BOOL	RemoveStock( CStockInfo & info, DWORD dwShare );
	BOOL	HasThisStock( CStockInfo & info, LPSTOCKOWN lpOwn );
};

/////////////////////////////////////////////////////////////////////////////
// oprecord_t

// 操作类型
#define	STRATEGY_OPTYPE_BUY			0x01	// 买入
#define	STRATEGY_OPTYPE_SELL		0x02	// 卖出
#define	STRATEGY_OPTYPE_ADDSTOCK	0x03	// 添加股票
#define	STRATEGY_OPTYPE_REMOVESTOCK	0x04	// 移除股票
#define	STRATEGY_OPTYPE_ADDCASH		0x05	// 添加资金
#define	STRATEGY_OPTYPE_REMOVECASH	0x06	// 取走资金

STKLIB_API	CSPString	AfxGetStrategyOpTypeString( long lOpType );

typedef	struct oprecord_t	{
	long	lOpType;
	time_t	time;
	char	szCode[STKLIB_MAX_CODE];
	DWORD	dwShare;
	double	dSharePrice;
	double	dRateCost;
	long	bViewed;	// for next op use in realop. 
	DWORD	dwMarket;
	char	szReserved[24];
} OPRECORD, * LPOPRECORD;

//template class __declspec(dllexport)	CSPArray< OPRECORD, OPRECORD & >;
typedef	CSPArray< OPRECORD, OPRECORD & >	COpRecordArray;
/***
	操作记录数组
*/
class STKLIB_API COpRecordContainer : public COpRecordArray
{
public:
	virtual ~COpRecordContainer()	{	RemoveAll();	}

	// for access COpRecordArray
	virtual	int		Add( OPRECORD &newElement){	return COpRecordArray::Add(newElement);	}
	virtual	void	RemoveAt(int nIndex, int nCount = 1){	COpRecordArray::RemoveAt(nIndex,nCount);	}
	virtual	void	RemoveAll( )			{	COpRecordArray::RemoveAll();	}
	virtual	void	Copy(const COpRecordArray& src)	{	COpRecordArray::Copy(src);	}

	// operation
	int		AddRecord(	long lOpType, CSPTime tm, const char *szCode,
						DWORD dwShare, double dSharePrice, double dRateCost );
	int		AddRecordUniqueStock(long lOpType, CSPTime tm, const char *szCode,
					   DWORD dwShare, double dSharePrice, double dRateCost );
	int		GetBuyRecordCount();
	BOOL	HasBuyStock( const char * szCode );
	BOOL	StatResults( int *pnTimes, int *pnVictoryTimes, double *pdYieldMax, double *pdYieldMin );
};

typedef	struct assetserial_t	{
	time_t	time;
	double	dAsset;
	double	dCash;
	char	szReserved[32];
} ASSETSERIAL, * LPASSETSERIAL;

//template class __declspec(dllexport)	CSPArray< ASSETSERIAL, ASSETSERIAL & >;
typedef	CSPArray< ASSETSERIAL, ASSETSERIAL & >	CAssetSerialArray;
/***
	资产数组
*/
class STKLIB_API CAssetSerialContainer : public CAssetSerialArray
{
public:
	virtual	~CAssetSerialContainer()	{	RemoveAll();	}

	// for access CAssetSerialArray
	virtual	void	RemoveAt(int nIndex, int nCount = 1){	CAssetSerialArray::RemoveAt(nIndex,nCount);	}
	virtual	void	RemoveAll( )			{	CAssetSerialArray::RemoveAll();	}
	virtual	void	Copy(const CAssetSerialArray& src)	{	CAssetSerialArray::Copy(src);	}

	// operation
	BOOL	SortInsert( ASSETSERIAL serial );
	BOOL	GetStdDev( double *pdStdDev, double * pdXiapu );
};

#pragma pack()

//////////////////////////////////////////////////////////////////////
// CTechStock
/***
	一只待操作的股票，包含股票的信息，K线数据，长周期K线数据，所需技术指标，所需长周期技术指标
*/
class STKLIB_API CTechStock
{
public:
	CTechStock( );
	CTechStock( const CTechStock & src );
	virtual	~CTechStock( );

	CTechStock	& operator = ( const CTechStock &src );
	void	SetAutoDelete( BOOL bAutoDelete = TRUE );
	void	Clear( );

	BOOL	GetPriceOK( long lOpType, CSPTime tmCur, DWORD dwShare, double dSharePrice, double *pdPriceOK );
	BOOL	IsStopTrading( CSPTime tmCur );
	BOOL	GetClosePrice( CSPTime tmCur, double *pdPrice );
	BOOL	GetCloseDiffPercent( CSPTime tmCur, double *pdDiffPercent );

	CStockInfo		m_info;			// 股票信息
	CKData			m_kdata;		// 所需K线数据
	CKData			m_kdataLong;	// 长周期K线数据
	CSPPtrArray		m_techs;		// 所需技术指标
	CSPPtrArray		m_techsLong;	// 所需长周期技术指标

protected:
	BOOL	m_bAutoDelete;
};

//template class __declspec(dllexport)	CSPArray< CTechStock, CTechStock & >;
typedef	CSPArray< CTechStock, CTechStock & >	CTechStockArray;
/***
	CTechStock数组
*/
class STKLIB_API CTechStockContainer : public CTechStockArray
{
public:
	virtual	~CTechStockContainer( );

	// for access CTechStockArray
	virtual	void SetSize(int nNewSize, int nGrowBy = -1)	{	CTechStockArray::SetSize(nNewSize,nGrowBy);	}

	// operation
	void	Clear( );
	
	// Operation
	BOOL	GetShouldBuyShare(	CTechStock & techstock, CSPTime tmCur, double dCash, CRateParam &rate, COpParam &opparam, int nOwnedStockCount,
								CSPTime *ptmOp, DWORD *pdwShare, double *pdSharePrice );
	BOOL	GetShouldSellShare(	CTechStock & techstock, CSPTime tmCur, STOCKOWN &own, COpParam &opparam,
								CSPTime *ptmOp, DWORD *pdwShare, double *pdSharePrice );

	BOOL	GetClosePrice( const char * szCode, CSPTime tmCur, double * pdPrice );
	BOOL	GetSumAsset( CSPTime tmCur, CStockOwnContainer &container, double * pdAsset );
	BOOL	GetNextExistTradeTime( CSPTime tmCur, CSPTime & tmNext );

	CStockInfo		m_infoIndex;
	CKData			m_kdataIndex;
	CSPPtrArray		m_techsIndex;
};

/////////////////////////////////////////////////////////////////////////////
// CStrategy

typedef BOOL (CALLBACK* SIMULATION_CALLBACK)(DWORD dwType, DWORD dwValue, LPCTSTR lpszMsg, VOID * cookie);

#define	STRATEGY_MAX_TITLE			256

#define	SIMULATION_PROGRESS			1
#define	SIMULATION_YIELD			2

#define	STRATEGY_MAX_PROGRESS		10000
#define	STRATEGY_MAXF_PROGRESS	10000.

#define	STRATEGY_BASE_YIELD		10000
#define	STRATEGY_BASEF_YIELD		10000.

/***
	策略
*/
class STKLIB_API CStrategy
{
public:
	CStrategy();
	virtual ~CStrategy();

	static CSPString	GetName( LPCTSTR lpszPath );
// Overrides
public:
	virtual	BOOL	OpenStrategyFile( LPCTSTR lpszPathName, LPSTR lpErr, UINT nMaxSize );
	virtual	BOOL	SaveStrategyFile( LPCTSTR lpszPathName );
	virtual BOOL	Serialize(CSPArchive& ar, LPSTR lpErr, UINT nMaxSize);
	virtual	BOOL	DoFileSave( );
	virtual	BOOL	OnClose( );
	virtual	void	OnRealOpViewed( );

protected:
	// 存盘文件路径和名称，不写到策略文件中
	CSPString	m_strPathName;

// Attributes
public:
	void		SetPathName( LPCTSTR lpszPathName );
	CSPString	GetPathName( );
	void		SetName( LPCTSTR lpszName );
	CSPString	GetName( );
	void		SetDescript( LPCTSTR lpszDescript );
	CSPString	GetDescript( );
	CSPStringArray	&	GetStocks( );
	void				SetStocks( CSPStringArray & astr );
	void				AddStock( LPCTSTR lpszCode );
	void				RemoveStock( LPCTSTR lpszCode );
	CSPString			GetStockTechString( );
	CRateParam		&	GetRateParam( );
	COpParam		&	GetOpParam( );
	CTechParameters	&	GetTechParametersBuy( );
	CTechParameters	&	GetTechParametersSell( );
	CTechParameters	&	GetTechParametersLongTrend( );
	CTechParameters	&	GetTechParametersIndexTrend( );
	CSPDWordArray	&	GetSelectedTechsBuy( );
	CSPDWordArray	&	GetSelectedTechsSell( );
	CSPDWordArray	&	GetSelectedTechsLongTrend( );
	CSPDWordArray	&	GetSelectedTechsIndexTrend( );

// Operations
public:
	// Simulation
	enum	SimuStatus	{
		statusInit		=	0x01,
		statusRunning	=	0x02,
		statusPaused	=	0x03,
		statusFinished	=	0x04,
	};

	// Data Cache
	BOOL	PrepareData( SIMULATION_CALLBACK fnCallback, void * cookie );
	void	ClearLastIntensity( );
	void	ClearCache( );
	CTechStockContainer &	GetTechStockContainer( );
	BOOL	CanModifiedNow()	{	return ( SimuIsStatusInit() || SimuIsStatusFinished() );	}

	// Simulation Status
	int		SimuGetCurStatus( )		{	return m_SimuCurrentStatus;	}
	BOOL	SimuIsStatusInit( )		{	return statusInit == m_SimuCurrentStatus;	}
	BOOL	SimuIsStatusRunning( )	{	return statusRunning == m_SimuCurrentStatus;	}
	BOOL	SimuIsStatusPaused( )	{	return statusPaused == m_SimuCurrentStatus;	}
	BOOL	SimuIsStatusFinished( )	{	return statusFinished == m_SimuCurrentStatus;	}
	void	SimuSetStatusInit( )	{	m_SimuCurrentStatus	=	statusInit;	}
	void	SimuSetStatusRunning( )	{	m_SimuCurrentStatus	=	statusRunning;	}
	void	SimuSetStatusPaused( )	{	m_SimuCurrentStatus	=	statusPaused;	}
	void	SimuSetStatusFinished( ){	m_SimuCurrentStatus	=	statusFinished;	}

	// Simulation
	void	SimuReset( );
	BOOL	SimuGotoNextTime( );
	CSPTime	SimuGetCurrentTime( );
	double	SimuGetCurrentCash( );
	BOOL	SimuOperate( OPRECORD record, BOOL bTimeStrict = TRUE );
	COpRecordContainer &	SimuGetOpRecord( );
	COpRecordContainer &	SimuGetNextOp( );
	CStockOwnContainer &	SimuGetStockOwn( );
	CAssetSerialContainer &	SimuGetAssetSerial( );
	DWORD	SimuGetCurrentProgress( DWORD dwProgressMax = STRATEGY_MAX_PROGRESS );	//	Not Same as Real
	double	SimuGetAsset( CSPTime tmCur );
	double	SimuGetCurrentYield( );
	double	SimuGetCurrentYieldIndexPercent( );
	double	SimuGetCurrentYieldPercent( );
	BOOL	SimuOperateNextop( CSPTime tmCur, COpRecordContainer & nextop, CTechStock & techstock );
	virtual	BOOL	SimuRun( SIMULATION_CALLBACK fnCallback, void * cookie );

	// Real
	void	RealReset( );
	BOOL	RealGotoNextTime( );
	CSPTime	RealGetBeginTime( );	//	Not Same as Simu
	CSPTime	RealGetCurrentTime( );
	double	RealGetCurrentCash( );
	BOOL	RealOperate( OPRECORD record, BOOL bTimeStrict = TRUE );
	COpRecordContainer &	RealGetOpRecord( );
	COpRecordContainer &	RealGetNextOp( );
	CStockOwnContainer &	RealGetStockOwn( );
	CAssetSerialContainer &	RealGetAssetSerial( );
	double	RealGetAsset( CSPTime tmCur );
	double	RealGetCurrentYield( );
	double	RealGetCurrentYieldIndexPercent( );
	double	RealGetCurrentYieldPercent( );
	BOOL	RealOperateNextop( CSPTime tmCur, COpRecordContainer & nextop, CTechStock & techstock );
	virtual	BOOL	RealRun( SIMULATION_CALLBACK fnCallback, void * cookie );
	BOOL	RealAddOpRecordStrict( OPRECORD record );	//	Not Same as Simu
	BOOL	RealDeleteOpRecordStrict( int nRecord );	//	Not Same as Simu

protected:
	// Properties
	DWORD			m_dwFileVersion;
	CSPString		m_strName;		// 策略名称
	CSPString		m_strDescript;	// 策略说明

	// Stocks
	CDomain			m_stocks;		// 策略备选股票

	// Rate
	CRateParam		m_rate;			// 交易费用
	
	// Tech parameters
	CTechParameters	m_paramBuy;			// 买入指标规则
	CTechParameters	m_paramSell;		// 卖出指标规则
	CTechParameters	m_paramLongTrend;	// 长期指标规则
	CTechParameters	m_paramIndexTrend;	// 指数指标规则
	CSPDWordArray	m_anTechsBuy;		// 买入依据的指标
	CSPDWordArray	m_anTechsSell;		// 卖出依据的指标
	CSPDWordArray	m_anTechsLongTrend;	// 长周期趋势依据的指标
	CSPDWordArray	m_anTechsIndexTrend;// 指数趋势依据的指标

	// Operate Parameters
	COpParam		m_opparam;		// 操作规则

	// Simulation
	int				m_SimuCurrentStatus;	// Not Same as Real, 起始，正在进行，暂停，完成
	CSPTime			m_SimuCurrentTime;		// 模拟当前时间
	double			m_SimuCurrentCash;		// 模拟当前资金
	CStockOwnContainer	m_SimuStockOwn;		// 模拟当前拥有股票
	COpRecordContainer	m_SimuOpRecord;		// 模拟记录
	COpRecordContainer	m_SimuNextOp;		// 模拟下一步操作
	CAssetSerialContainer	m_SimuAssetSerial;	// 模拟资产值序列

	// Real Operate
	CSPTime			m_RealBeginTime;		//	Not Same as Simu，实战起始时间
	CSPTime			m_RealCurrentTime;		// 实战当前时间
	double			m_RealCurrentCash;		// 实战当前资金
	CStockOwnContainer	m_RealStockOwn;		// 实战当前拥有股票
	COpRecordContainer	m_RealOpRecord;		// 实战操作记录
	COpRecordContainer	m_RealNextOp;		// 实战下一步操作
	CAssetSerialContainer	m_RealAssetSerial;	// 实战资产值序列

	// Cache Data, Not Saved
	CTechStockContainer	m_techstocks;		// 备选股票，包含信息、数据、技术指标等
};

#endif // __STKLIB_STRATEGY_H__
