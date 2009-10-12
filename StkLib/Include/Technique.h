/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CTechnique
*/

#if	!defined( __STKLIB_TECHNIQUE_H__ )
#define	__STKLIB_TECHNIQUE_H__

/* The Folowing Is Stock Technique Analysis Identify Number */
/* 技术指标唯一编号 */
#define	STT_MIN			1
//	K线属性类
#define	STT_KLINE_MIN	1
#define	STT_MA			1		//	移动平均线MA
#define	STT_BBI			2		//	多空指标BBI
#define	STT_BOLL		3		//	布林带BOLL
#define	STT_PV			4		//	汇证均价线P/V
#define	STT_SAR			5		//	停损点转向指标SAR
#define	STT_DJ			6		//	叠加图DJ
#define	STT_CW			7		//	筹码分布图CW
#define	STT_KLINE_MAX	7

//	趋势类
#define	STT_TREND_MIN	8
#define	STT_MACD		8		//	指数平滑异同移动平均线MACD
#define	STT_MIKE		9		//	麦克指标MIKE
#define	STT_PSY			10		//	心理线PSY
#define	STT_TREND_MAX	10

//	能量类
#define	STT_ENERGY_MIN	11
#define	STT_VOLUME		11		//	成交量
#define	STT_NVI			12		//	负成交量指标NVI
#define	STT_PVI			13		//	正成交量指标PVI
#define	STT_VR			14		//	容量比率VR
#define	STT_VROC		15		//	成交量变动率指标VROC
#define	STT_OBV			16		//	能量线OBV
#define	STT_MOBV		17		//	主力进出MOBV
#define	STT_MFI			18		//	资金流动指数MFI
#define	STT_VMACD		19		//	量指数平滑异同移动平均线VMACD
#define	STT_WVAD		20		//	威廉变异离散量WVAD
#define	STT_EMV			21		//	简易波动指标EMV
#define	STT_VRSI		22		//	量相对强弱指标VRSI
#define	STT_NVRSI		23		//	新量相对强弱指标VRSI
#define	STT_AD			24		//	聚/散指标A/D
#define	STT_CI			25		//	累积指数CI
#define	STT_ENERGY_MAX	25

//	摆动类
#define	STT_SWING_MIN	26
#define	STT_KDJ			26		//	随机指标KDJ
#define	STT_R			27		//	威廉指标R
#define	STT_RSI			28		//	相对强弱指标
#define	STT_BIAS		29		//	乖离率BIAS
#define	STT_MTM			30		//	动量指标MTM
#define	STT_DMI			31		//	动向指标DMI
#define	STT_ROC			32		//	变动率指标ROC
#define	STT_CCI			33		//	顺势通道指标CCI
#define	STT_CV			34		//	佳庆指标CV
#define	STT_ARBR		35		//	人气意愿指标ARBR
#define	STT_CR			36		//	中间意愿指标CR
#define	STT_OSC			37		//	摆动量OSC
#define	STT_UOS			38		//	极摆动指标UOS
#define	STT_MAOSC		39		//	移动平均摆动量MAOSC
#define	STT_36BIAS		40		//	3减6日乖离3-6BIAS
#define	STT_DPO			41		//	去势价格摆动指数DPO
#define	STT_KST			42		//	确然指标KST
#define	STT_SWING_MAX	42

//	其他类
#define	STT_OTHER_MIN	43
#define	STT_REI			43		//	范围膨胀指数REI
#define	STT_DMKI		44		//	迪马克尔指标DMKI
#define	STT_PCNT		45		//	幅度比PCNT
#define	STT_HLC			46		//	幅度分析HLC
#define	STT_CDP			47		//	逆势操作指标CDP
#define	STT_ASI			48		//	累积摆动指标ASI
#define	STT_ATR			49		//	均幅指标ATR
#define	STT_OTHER_MAX	49

#define	STT_CLK_MIN		50
#define	STT_CYO			50		//	周期摆动量CYO
#define	STT_DCYO		51		//	去噪周期摆动量DCYO
#define	STT_HSL			52		//	相对换手率HSL
#define	STT_DPER		53		//	去势百分比指标DPER
#define	STT_CLK_MAX		53

#define	STT_MAX			53

#define	STT_USER_MIN	10000

#define	STT_INVALID		DWORD(-1)

// 指标类别
#define	STT_CLASS_MIN		1
#define	STT_CLASS_KLINE		1	// K线叠加类
#define	STT_CLASS_TREND		2	// 趋势类
#define	STT_CLASS_ENERGY	3	// 能量类
#define	STT_CLASS_SWING		4	// 摆动类
#define	STT_CLASS_OTHER		5	// 其它类
#define	STT_CLASS_CLK		6	// 策略为王类
#define	STT_CLASS_USER		7	// 外挂指标类
#define	STT_CLASS_MAX		7

// 买卖信号
#define	ITS_MIN				-15
#define	ITS_SELLINTENSE		-15	//	强烈卖出
#define	ITSG_SELLINTENSE	-13	//	门限
#define	ITS_SELL			-10	//	卖出
#define	ITSG_SELL			-8	//	门限
#define	ITS_SELLFEEBLE		-5	//	微弱卖出
#define	ITSG_SELLFEEBLE		-3	//	门限
#define	ITS_NOTHING			0	//	无
#define	ITSG_BUYFEEBLE		3	//	门限
#define	ITS_BUYFEEBLE		5	//	微弱买进
#define	ITSG_BUY			8	//	门限
#define	ITS_BUY				10	//	买进
#define	ITSG_BUYINTENSE		13	//	门限
#define	ITS_BUYINTENSE		15	//	强烈买进
#define	ITS_MAX				15

// 买卖信号理由
#define	ITSC_MIN				1
#define	ITSC_NOTHING			1	//	无
#define	ITSC_DEVIATEONBOTTOM	2	//	底背离
#define	ITSC_DEVIATEONTOP		3	//	顶背离
#define	ITSC_GOLDENFORK			4	//	金叉
#define	ITSC_DEADFORK			5	//	死叉
#define	ITSC_LONG				6	//	多头
#define	ITSC_SHORT				7	//	空头
#define	ITSC_OVERBOUGHT			8	//	超买
#define	ITSC_OVERSOLD			9	//	超卖
#define	ITSC_SUPPORT			10	//	支撑位
#define	ITSC_RESISTANCE			11	//	阻力位
#define	ITSC_MAX				11

#define	ITS_ISSELL(x)			(x<0)
#define	ITS_ISBUY(x)			(x>0)
#define	ITS_ISSELLORNOTHING(x)	(x<=0)
#define	ITS_ISBUYORNOTHING(x)	(x>=0)

#define	ITS_GETMINMAXDAYRANGE	120
#define	ITS_DAYS_DEVIATE		115


/***
	一些宏定义，主要是技术指标计算中常用的一些代码
*/
#define	STT_ASSERT_GETMINMAXINFO( m_pKData, nStart, nEnd )	\
	SP_ASSERT( m_pKData && nStart >= 0 && nStart < m_pKData->GetSize() && nEnd >= 0 && nEnd < m_pKData->GetSize() );	\
	if( nStart < 0 || nStart >=m_pKData->GetSize() || nEnd < 0 || nEnd >=m_pKData->GetSize() )							\
		return FALSE;

#define	STT_ASSERT_CALCULATE( m_pKData, nIndex, nDays )		\
	SP_ASSERT( m_pKData && nIndex >= 0 && nIndex < m_pKData->GetSize() && nDays > 0 );	\
	if( !m_pKData || nIndex < 0 || nIndex >=m_pKData->GetSize() || nDays <= 0 )			\
		return FALSE;																	\
	if( !IsValidParameters( ) )															\
		SetDefaultParameters( );

#define	STT_ASSERT_CALCULATE1( m_pKData, nIndex )			\
	SP_ASSERT( m_pKData && nIndex >= 0 && nIndex < m_pKData->GetSize() );	\
	if( !m_pKData || nIndex < 0 || nIndex >=m_pKData->GetSize() )			\
		return FALSE;														\
	if( !IsValidParameters( ) )												\
		SetDefaultParameters( );

#define	VALID_DAYS( x )		(x>=1&&x<=1024)
#define	STT_VALID_DAYSARRAY( a )						\
	if( a.GetSize() <= 0 )								\
		return FALSE;									\
	for( int k=0; k<a.GetSize(); k++ )					\
		if( a[k] < 1 || a[k] > 1024 )					\
			return FALSE;
#define	VALID_ITS( x )		(x>=ITS_MIN&&x<=ITS_MAX)


/***
	一些反复使用的代码，用函数代替，得到技术指标在指定范围内的最大最小值
*/
class	CTechnique;
BOOL AfxGetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech, CSPDWordArray & adwDays );	// 技术指标有不同日期作为参数
BOOL AfxGetMinMaxInfo1( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech );	// 技术指标有1个最终值
BOOL AfxGetMinMaxInfo2( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech );	// 技术指标有2个最终值
BOOL AfxGetMinMaxInfo3( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech );	// 技术指标有3个最终值
BOOL AfxGetMinMaxInfo4( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech );	// 技术指标有4个最终值
BOOL AfxGetMinMaxInfo5( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech );	// 技术指标有5个最终值
DWORD	AfxGetMaxDays( CSPDWordArray & adw );	// 数组adw的最大值

/**
	技术指标基类
*/
class STKLIB_API CTechnique
{
public:
	// Constructors
	CTechnique( );
	CTechnique( CKData * pKData );
	virtual ~CTechnique();

	void SetKData( CKData * pKData );	//	设定K线数据
	CKData * GetKData( );
	virtual	void	Clear( );			//	清除
	virtual	void	ClearLastIntensity( );	//	清除上次买卖信号
	virtual	BOOL	PrepareCache( int nStart, int nEnd, BOOL bForceRecalculate );	//	计算好数据，放入缓冲区

protected:
	BOOL	LoadFromCache( int nIndex, double * pValue1,
				double * pValue2 = NULL, double * pValue3 = NULL, double * pValue4 = NULL, double * pValue5 = NULL );	// 从缓冲区读入结果
	BOOL	StoreToCache( int nIndex, double * pValue1,
				double * pValue2 = NULL, double * pValue3 = NULL, double * pValue4 = NULL, double * pValue5 = NULL );	// 将结果保存至缓冲区

	// Intensity Utility
	BOOL	IntensityPrepare( int nIndex, UINT * pnCode, int nMaxDays, int nMinMaxDays, double *pdLiminalLow, double *pdLiminalHigh, double dGateLow = 0.3, double dGateHigh = 0.7 );		// 准备数据，得到较大值和较小值
	BOOL	IntensityPreparePrice( int nIndex, UINT * pnCode, int nMaxDays, int nMinMaxDays, double *pdLiminalLow, double *pdLiminalHigh, double dGateLow = 0.3, double dGateHigh = 0.7 );	// 准备数据，得到价格较大值和较小值
	BOOL	IsNewValue( int nIndex, BOOL bTopOrBottom, int nDays, double * pdValue1, double * pdValue2 );	// 是否创新高或者创新低
	BOOL	IsDeviateOnBottom( int nIndex, double * pdValue1, double * pdValue2 );	// 是否底背离
	BOOL	IsDeviateOnTop( int nIndex, double * pdValue1, double * pdValue2 );		// 是否顶背离
	BOOL	IsGoldenFork( int nIndex, double * pdValue1, double * pdValue2 );		// 是否金叉
	BOOL	IsDeadFork( int nIndex, double * pdValue1, double * pdValue2 );			// 是否死叉
	int		GetForkSignal( int nIndex, CSPDWordArray & adwDays, UINT itsGoldenFork, UINT itsDeadFork,  UINT * pnCode );	// 得到金叉或者死叉信号
	int		GetForkSignal( int nIndex, UINT itsGoldenFork, UINT itsDeadFork, UINT *pnCode );		// 得到金叉或者死叉信号
	int		GetForkSignal(int nIndex, double * pdValue1, double * pdValue2, UINT itsGoldenFork, UINT itsDeadFork, UINT *pnCode );	// 得到金叉死叉信号
	int		GetTrendIntensity(int nIndex, CSPDWordArray & adwDays, UINT itsLong, UINT itsShort, UINT * pnCode );	// 得到趋势信号
	int		GetTrendIntensity1( int nIndex, UINT itsLong, UINT itsShort, UINT *pnCode );	// 得到趋势信号
	int		GetTrendIntensity2( int nIndex, UINT itsLong, UINT itsShort, UINT *pnCode );	// 得到趋势信号
	int		GetTrendIntensity(int nIndex, double * pdValue1, double * pdValue2, UINT itsLong, UINT itsShort, UINT *pnCode );	// 得到趋势信号

public:
	// Attributes
	virtual	void	SetDefaultParameters( );		// 设定缺省参数
	void	AttachParameters( CTechnique & src );	// 指定参数与src相同
	virtual	BOOL	IsValidParameters( );			// 参数是否合法

	// Creations
	static	CTechnique *	CreateTechnique( UINT nTech, CKData * pKData );	// 创建CTechnique对象
	
	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );	// 得到买卖信号
	virtual	int		GetIntensity( int nIndex, UINT * pnCode = NULL, BOOL bTraceBack = TRUE );	// 得到买卖趋势信号
	virtual	int		GetIntensityTraceback( int nIndex, UINT * pnCode = NULL );					// 得到买卖趋势信号
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );		// 得到最大最小值
	virtual	BOOL	Calculate( double * pValue, int nIndex, int nDays, BOOL bUseLast );			// 计算指标值
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );		// 计算指标值
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, int nIndex, BOOL bUseLast );		// 计算指标值
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, double * pValue3, int nIndex, BOOL bUseLast );		// 计算指标值
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, double * pValue3, double * pValue4, int nIndex, BOOL bUseLast );		// 计算指标值
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, double * pValue3, double * pValue4, double * pValue5, int nIndex, BOOL bUseLast );		// 计算指标值
	virtual	BOOL	CalculateMA( double * pValue, double * pMA, int nIndex, BOOL bUseLast, int nMADays );		// 计算指标值和均值

protected:
	CKData	*	m_pKData;

	double	*	m_pdCache1;
	double	*	m_pdCache2;
	double	*	m_pdCache3;
	double	*	m_pdCache4;
	double	*	m_pdCache5;
	BOOL	*	m_pbHasCache1;
	BOOL	*	m_pbHasCache2;
	BOOL	*	m_pbHasCache3;
	BOOL	*	m_pbHasCache4;
	BOOL	*	m_pbHasCache5;
	int			m_nLength;
	int			m_nIndexStart;

	int			m_nLastIntensity;
	UINT		m_nLastIntensityCode;
};

// K线
class STKLIB_API CKLine : public CTechnique
{
public:
	// Constructors
	CKLine( );
	CKLine( CKData * pKData );
	virtual ~CKLine();

public:
	virtual	void Clear( );

	// Attributes
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CKLine & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
};

/////////////////////////////////////////////////////////////////////////
//	K线属性类
//	移动平均线MA
class STKLIB_API CMA : public CTechnique
{
public:
	// Constructors
	CMA( );
	CMA( CKData * pKData );
	virtual ~CMA();

public:
	virtual	void Clear( );

	// Attributes
	enum MATypes {
		typeMA		=	0x01,
		typeEXPMA	=	0x02,
	};
	int				m_nType;
	CSPDWordArray	m_adwMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	int		m_itsLong;
	int		m_itsShort;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CMA & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, int nDays, BOOL bUseLast );
};

//	多空指标BBI
class STKLIB_API CBBI : public CTechnique
{
public:
	// Constructors
	CBBI( );
	CBBI( CKData * pKData );
	virtual ~CBBI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nMA1Days;
	int		m_nMA2Days;
	int		m_nMA3Days;
	int		m_nMA4Days;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CBBI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	布林带BOLL
class STKLIB_API CBOLL : public CTechnique
{
public:
	// Constructors
	CBOLL( );
	CBOLL( CKData * pKData );
	virtual ~CBOLL();

public:
	virtual	void Clear( );

	// Attributes
	double	m_dMultiUp;
	double	m_dMultiDown;
	int		m_nMADays;
	int		m_itsSupport;
	int		m_itsResistance;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CBOLL & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pdMA, double * pdUp, double * pdDown, int nIndex, BOOL bUseLast );
};

//	汇证均价线P/V
class STKLIB_API CPV : public CTechnique
{
public:
	// Constructors
	CPV( );
	CPV( CKData * pKData );
	virtual ~CPV();

public:
	virtual	void Clear( );

	// Attributes
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CPV & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	停损点转向指标SAR
class STKLIB_API CSAR : public CTechnique
{
public:
	// Constructors
	CSAR( );
	CSAR( CKData * pKData );
	virtual ~CSAR();

protected:
	BOOL	CalculateSAR( double * pValue, int nIndex, BOOL bUseLast );

public:
	virtual	void Clear( );

	// Attributes
	int		m_nInitDays;
	int		m_bFirstUp;
	double	m_dAFStep;
	double	m_dAFMax;
	int		m_itsBuy;
	int		m_itsSell;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CSAR & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );

protected:
	BOOL			m_bCurUp;
	BOOL			m_bTurn;
	double			m_dCurAF;
	double			m_dCurHigh;
	double			m_dCurLow;
};

//	叠加图DJ
class STKLIB_API CDJ : public CTechnique
{
public:
	// Constructors
	CDJ( );
	CDJ( CKData * pKData );
	virtual ~CDJ();

	static	CStock		m_stockSha;
	static	CStock		m_stockSzn;
	static	CSPString	m_strCodeOrg;

public:
	virtual	void Clear( );

	// Attributes
	CSPString		m_strCodeSha;
	CSPString		m_strCodeSzn;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CDJ & src );
	virtual	BOOL	IsValidParameters( );
	BOOL	PrepareStockData(CStDatabase * pDatabase, const char * szCodeOrg,
									int nCurKType, int nCurKFormat, int nCurMaindataType,
									DWORD dwAutoResumeDRBegin, int nAutoResumeDRLimit );

	// Operations
};

//	筹码分布图CW
class STKLIB_API CCW : public CTechnique
{
public:
	// Constructors
	CCW( );
	CCW( CKData * pKData );
	virtual ~CCW();

public:
	virtual	void Clear( );

	// Attributes
	double	m_dChangeHand;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CCW & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	BOOL			GetRange( int & nStart, int & nEnd, CStockInfo & info );
	BOOL			GetMinMaxInfo(int nStart, int nEnd, double dMinPrice, double dMaxPrice, double dStep,
								double *pdMinVolume, double *pdMaxVolume );
	BOOL			CalculateCW( double *pdVolume, int nStart, int nEnd, double dPrice, double dStep );


	BOOL	CalculateCW(int nStart, int nEnd, CStockInfo & info, double dStep,
						CSPDWordArray & adwPrice, CSPDWordArray & adwVolume,
						double * pdMinVolume, double * pdMaxVolume, double * pdTotalVolume, double * pdVolPercent );
	BOOL	CalculateRecentCW(int nEnd, int nDays, CStockInfo & info, double dStep,
						CSPDWordArray & adwPrice, CSPDWordArray & adwVolume,
						double * pdMinVolume, double * pdMaxVolume, double * pdTotalVolume, double * pdVolPercent );
	BOOL	CalculatePastCW(int nEnd, int nDays, CStockInfo & info, double dStep,
						CSPDWordArray & adwPrice, CSPDWordArray & adwVolume,
						double * pdMinVolume, double * pdMaxVolume, double * pdTotalVolume, double * pdVolPercent );

	// Stat
	static	BOOL	StatGainPercent( double *pdGainPercent, CSPDWordArray &adwPrice, CSPDWordArray &adwVolume, double dPriceSel );
	static	BOOL	StatCostAverage( double *pdCostAve, CSPDWordArray &adwPrice, CSPDWordArray &adwVolume );
	static	BOOL	StatMass(double *pdLower, double *pdUpper, double *pdMassPrice, CSPDWordArray &adwPrice, CSPDWordArray &adwVolume, double dMassVol );
};

/////////////////////////////////////////////////////////////////////////
//	趋势类
//	指数平滑异同移动平均线MACD
class STKLIB_API CMACD : public CTechnique
{
public:
	// Constructors
	CMACD( );
	CMACD( CKData * pKData );
	virtual ~CMACD();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nEMA1Days;
	int		m_nEMA2Days;
	int		m_nDIFDays;
	int		m_itsDeviateOnBottom;
	int		m_itsDeviateOnTop;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CMACD & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double *pdEMA1, double *pdEMA2, double *pdDIF, double *pdDEA,
					int nIndex, BOOL bUseLast );
};

//	麦克指标MIKE
class STKLIB_API CMIKE : public CTechnique
{
public:
	// Constructors
	CMIKE( );
	CMIKE( CKData * pKData );
	virtual ~CMIKE();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CMIKE & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	BOOL	CalculateMIKE(	double *pWR, double *pMR, double *pSR,
							double *pWS, double *pMS, double *pSS, int nIndex );
};

//	心理线PSY
class STKLIB_API CPSY : public CTechnique
{
public:
	// Constructors
	CPSY( );
	CPSY( CKData * pKData );
	virtual ~CPSY();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CPSY & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

/////////////////////////////////////////////////////////////////////////
//	能量类
//	成交量
class STKLIB_API CVOLUME : public CTechnique
{
public:
	// Constructors
	CVOLUME( );
	CVOLUME( CKData * pKData );
	virtual ~CVOLUME();

public:
	virtual	void Clear( );

	// Attributes
	CSPDWordArray	m_adwMADays;
	int		m_itsDeviateOnBottom;
	int		m_itsDeviateOnTop;
	int		m_itsLong;
	int		m_itsShort;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CVOLUME & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, int nDays, BOOL bUseLast );
};

//	负成交量指标NVI
class STKLIB_API CNVI : public CTechnique
{
public:
	// Constructors
	CNVI( );
	CNVI( CKData * pKData );
	virtual ~CNVI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CNVI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, double *pMA, int nIndex, BOOL bUseLast );
};

//	正成交量指标PVI
class STKLIB_API CPVI : public CTechnique
{
public:
	// Constructors
	CPVI( );
	CPVI( CKData * pKData );
	virtual ~CPVI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CPVI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, double *pMA, int nIndex, BOOL bUseLast );
};

//	容量比率VR
class STKLIB_API CVR : public CTechnique
{
public:
	// Constructors
	CVR( );
	CVR( CKData * pKData );
	virtual ~CVR();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsLong;
	int		m_itsShort;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CVR & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	成交量变动率指标VROC
class STKLIB_API CVROC : public CTechnique
{
public:
	// Constructors
	CVROC( );
	CVROC( CKData * pKData );
	virtual ~CVROC();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADays;
	int		m_itsDeviateOnBottom;
	int		m_itsDeviateOnTop;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CVROC & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	能量线OBV
class STKLIB_API COBV : public CTechnique
{
public:
	// Constructors
	COBV( );
	COBV( CKData * pKData );
	virtual ~COBV();

public:
	virtual	void Clear( );

	// Attributes
	int		m_itsDeviateOnBottom;
	int		m_itsDeviateOnTop;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( COBV & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pdOBV, int nIndex, BOOL bUseLast );
};

//	主力进出MOBV
class STKLIB_API CMOBV : public COBV
{
public:
	// Constructors
	CMOBV( );
	CMOBV( CKData * pKData );
	virtual ~CMOBV();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays1;
	int		m_nDays2;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CMOBV & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, double * pValue3, int nIndex, BOOL bUseLast );
};

//	资金流动指数MFI
class STKLIB_API CMFI : public CTechnique
{
public:
	// Constructors
	CMFI( );
	CMFI( CKData * pKData );
	virtual ~CMFI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsLong;
	int		m_itsShort;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CMFI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	量指数平滑异同移动平均线VMACD
class STKLIB_API CVMACD : public CMACD
{
public:
	// Constructors
	CVMACD( );
	CVMACD( CKData * pKData );
	virtual ~CVMACD();

protected:

public:
	virtual	BOOL	Calculate( double *pdEMA1, double *pdEMA2, double *pdDIF, double *pdDEA,
					int nIndex, BOOL bUseLast );
};

//	威廉变异离散量WVAD
class STKLIB_API CWVAD : public CTechnique
{
public:
	// Constructors
	CWVAD( );
	CWVAD( CKData * pKData );
	virtual ~CWVAD();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsLong;
	int		m_itsShort;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CWVAD & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	简易波动指标EMV
class STKLIB_API CEMV : public CTechnique
{
public:
	// Constructors
	CEMV( );
	CEMV( CKData * pKData );
	virtual ~CEMV();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CEMV & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	量相对强弱指标VRSI
class STKLIB_API CVRSI : public CTechnique
{
public:
	// Constructors
	CVRSI( );
	CVRSI( CKData * pKData );
	virtual ~CVRSI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsDeviateOnBottom;
	int		m_itsDeviateOnTop;
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CVRSI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	新量相对强弱指标VRSI
class STKLIB_API CNVRSI : public CVRSI
{
public:
	// Constructors
	CNVRSI( );
	CNVRSI( CKData * pKData );
	virtual ~CNVRSI();

public:
	// Operations
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	聚/散指标A/D
class STKLIB_API CAD : public CTechnique
{
public:
	// Constructors
	CAD( );
	CAD( CKData * pKData );
	virtual ~CAD();

public:
	virtual	void Clear( );

	// Attributes
	int				m_nDays;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CAD & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	累积指数CI
class STKLIB_API CCI : public CTechnique
{
public:
	// Constructors
	CCI( );
	CCI( CKData * pKData );
	virtual ~CCI();

public:
	virtual	void Clear( );

	// Attributes
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CCI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

/////////////////////////////////////////////////////////////////////////
//	摆动类
//	随机指标KDJ
class STKLIB_API CKDJ : public CTechnique
{
public:
	// Constructors
	CKDJ( );
	CKDJ( CKData * pKData );
	virtual ~CKDJ();

protected:
	BOOL	CalculateRSV( double * pValue, int nIndex );

public:
	virtual	void Clear( );

	// Attributes
	enum modeJ	{
		mode3K2D	=	0x01,
		mode3D2K	=	0x02,
	};
	int		m_nRSVDays;
	int		m_nKDays;
	int		m_nDDays;
	int		m_nJ;		//	1 For 3K-2D, 2 For 3D-2K
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CKDJ & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double *pValue1, double *pValue2, double *pValue3, int nIndex, BOOL bUseLast );
};

//	威廉指标R
class STKLIB_API CR : public CTechnique
{
public:
	// Constructors
	CR( );
	CR( CKData * pKData );
	virtual ~CR();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CR & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	相对强弱指标
class STKLIB_API CRSI : public CTechnique
{
public:
	// Constructors
	CRSI( );
	CRSI( CKData * pKData );
	virtual ~CRSI();

public:
	virtual	void Clear( );

	// Attributes
	CSPDWordArray	m_adwDays;
	int		m_itsSold;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CRSI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, int nDays, BOOL bUseLast );
};

//	乖离率BIAS
class STKLIB_API CBIAS : public CTechnique
{
public:
	// Constructors
	CBIAS( );
	CBIAS( CKData * pKData );
	virtual ~CBIAS();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CBIAS & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	动量指标MTM
class STKLIB_API CMTM : public CTechnique
{
public:
	// Constructors
	CMTM( );
	CMTM( CKData * pKData );
	virtual ~CMTM();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CMTM & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double *pMA, int nIndex, BOOL bUseLast );
};

//	动向指标DMI
class STKLIB_API CDMI : public CTechnique
{
public:
	// Constructors
	CDMI( );
	CDMI( CKData * pKData );
	virtual ~CDMI();

protected:
	double	*	m_pdDMICache;
	int		*	m_pnDMICacheIndex;
	int			m_nDMICacheCurrent;
	BOOL	CalculateDM( double *pDMPlus, double *pDMMinus, double *pTR, int nIndex );
	BOOL	CalculateDIDX( double *pDIPlus, double *pDIMinus, double *pDX, int nIndex, int nDays );
public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CDMI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pDIPlus, double * pDIMinus, double *pADX, double *pADXR, int nIndex, BOOL bUseLast );
};

//	变动率指标ROC
class STKLIB_API CROC : public CTechnique
{
public:
	// Constructors
	CROC( );
	CROC( CKData * pKData );
	virtual ~CROC();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CROC & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	顺势通道指标CCI
class STKLIB_API CCCI : public CTechnique
{
public:
	// Constructors
	CCCI( );
	CCCI( CKData * pKData );
	virtual ~CCCI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	double	m_dQuotiety;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CCCI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	佳庆指标CV
class STKLIB_API CCV : public CTechnique
{
public:
	// Constructors
	CCV( );
	CCV( CKData * pKData );
	virtual ~CCV();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nMAHLDays;
	int		m_nCVDays;
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CCV & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	人气意愿指标ARBR
class STKLIB_API CARBR : public CTechnique
{
public:
	// Constructors
	CARBR( );
	CARBR( CKData * pKData );
	virtual ~CARBR();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CARBR & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pAR, double *pBR, int nIndex, BOOL bUseLast );
};

//	中间意愿指标CR
class STKLIB_API CCR : public CTechnique
{
public:
	// Constructors
	CCR( );
	CCR( CKData * pKData );
	virtual ~CCR();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADaysA;
	int		m_nMADaysB;
	int		m_nMADaysC;
	int		m_nMADaysD;
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CCR & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pA, double * pB, double * pC, double * pD, int nIndex, BOOL bUseLast );
};

//	摆动量OSC
class STKLIB_API COSC : public CTechnique
{
public:
	// Constructors
	COSC( );
	COSC( CKData * pKData );
	virtual ~COSC();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( COSC & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	极摆动指标UOS
class STKLIB_API CUOS : public CTechnique
{
public:
	// Constructors
	CUOS( );
	CUOS( CKData * pKData );
	virtual ~CUOS();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays1;
	int		m_nDays2;
	int		m_nDays3;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CUOS & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	移动平均摆动量MAOSC
class STKLIB_API CMAOSC : public CTechnique
{
public:
	// Constructors
	CMAOSC( );
	CMAOSC( CKData * pKData );
	virtual ~CMAOSC();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays1;
	int		m_nDays2;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CMAOSC & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	3减6日乖离3-6BIAS
class STKLIB_API C36BIAS : public CTechnique
{
public:
	// Constructors
	C36BIAS( );
	C36BIAS( CKData * pKData );
	virtual ~C36BIAS();

public:
	virtual	void Clear( );

	// Attributes
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( C36BIAS & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	去势价格摆动指数DPO
class STKLIB_API CDPO : public CTechnique
{
public:
	// Constructors
	CDPO( );
	CDPO( CKData * pKData );
	virtual ~CDPO();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CDPO & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	确然指标KST
class STKLIB_API CKST : public CTechnique
{
public:
	// Constructors
	CKST( );
	CKST( CKData * pKData );
	virtual ~CKST();

public:
	virtual	void Clear( );

	// Attributes
	CSPDWordArray	m_adwROCDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CKST & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

/////////////////////////////////////////////////////////////////////////
//	其他类
//	范围膨胀指数REI
class STKLIB_API CREI : public CTechnique
{
public:
	// Constructors
	CREI( );
	CREI( CKData * pKData );
	virtual ~CREI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsLong;
	int		m_itsShort;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CREI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	迪马克尔指标DMKI
class STKLIB_API CDMKI : public CTechnique
{
public:
	// Constructors
	CDMKI( );
	CDMKI( CKData * pKData );
	virtual ~CDMKI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CDMKI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	幅度比PCNT
class STKLIB_API CPCNT : public CTechnique
{
public:
	// Constructors
	CPCNT( );
	CPCNT( CKData * pKData );
	virtual ~CPCNT();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CPCNT & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	幅度分析HLC
class STKLIB_API CHLC : public CTechnique
{
public:
	// Constructors
	CHLC( );
	CHLC( CKData * pKData );
	virtual ~CHLC();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CHLC & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	逆势操作指标CDP
class STKLIB_API CCDP : public CTechnique
{
public:
	// Constructors
	CCDP( );
	CCDP( CKData * pKData );
	virtual ~CCDP();

public:
	virtual	void Clear( );

	// Attributes
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CCDP & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pAH, double * pNH, double * pAL, double * pNL, int nIndex, BOOL bUseLast );
};

//	累积摆动指标ASI
class STKLIB_API CASI : public CTechnique
{
public:
	// Constructors
	CASI( );
	CASI( CKData * pKData );
	virtual ~CASI();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsDeviateOnBottom;
	int		m_itsDeviateOnTop;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CASI & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

//	均幅指标ATR
class STKLIB_API CATR : public CTechnique
{
public:
	// Constructors
	CATR( );
	CATR( CKData * pKData );
	virtual ~CATR();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_itsSold;
	int		m_itsBought;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CATR & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
};

/////////////////////////////////////////////////////////////////////////
//	策略为王类
//	周期摆动量CYO
class STKLIB_API CCYO : public CTechnique
{
public:
	// Constructors
	CCYO( );
	CCYO( CKData * pKData );
	virtual ~CCYO();

public:
	virtual	void Clear( );

	// Attributes
	CSPDWordArray	m_adwMTMDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CCYO & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	去噪周期摆动量DCYO
class STKLIB_API CDCYO : public CTechnique
{
public:
	// Constructors
	CDCYO( );
	CDCYO( CKData * pKData );
	virtual ~CDCYO();

public:
	virtual	void Clear( );

	// Attributes
	CSPDWordArray	m_adwMTMDays;
	int		m_nMADays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CDCYO & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

//	相对换手率HSL
class STKLIB_API CHSL : public CTechnique
{
public:
	// Constructors
	CHSL( );
	CHSL( CKData * pKData );
	virtual ~CHSL();

	static	double GetMainChangeHand( DWORD dwMarket, CKData & kdata, int nIndexKD );

public:
	virtual	void Clear( );

	CStockInfo	m_stockinfo;

	// Attributes
	int		m_nDays;	// Not Used
	int		m_nMADays;	// Not Used
	int		m_itsGoldenFork;	// Not Used
	int		m_itsDeadFork;		// Not used
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CHSL & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, int nIndex, int nDays, BOOL bUseLast );
};

//	去势百分比指标DPER
class STKLIB_API CDPER : public CTechnique
{
public:
	// Constructors
	CDPER( );
	CDPER( CKData * pKData );
	virtual ~CDPER();

public:
	virtual	void Clear( );

	// Attributes
	int		m_nDays;
	int		m_nMADays;
	int		m_nDetrendDays;
	int		m_itsGoldenFork;
	int		m_itsDeadFork;
	virtual	void	SetDefaultParameters( );
	void	AttachParameters( CDPER & src );
	virtual	BOOL	IsValidParameters( );

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );
	virtual	BOOL	GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax );
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );
	virtual	BOOL	Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast );
};

#define	TECHUSER_API
#include "TechUser.h"

// 用户外挂指标
class STKLIB_API CTechUser : public CTechnique
{
public:
	// Constructors
	CTechUser( );
	CTechUser( CKData * pKData );
	virtual ~CTechUser();

	// static functions
	static	UINT		GetTechUserCount();
	static	CSPString	GetTechUserName( int nTech );
	static	CSPString	GetTechUserShortName( int nTech );

	// techuser attributes
	BOOL		SetTechUserID( int nTechUserID );
	UINT		GetTechUserID( );
	void		SetStock( CStock * pStock );
	UINT		GetLineCount( );
	CSPString	GetLineName( int nLine );

public:
	virtual	void	Clear( );			//	清除

public:
	// Attributes
	virtual	void	SetDefaultParameters( );		// 设定缺省参数
	void	AttachParameters( CTechUser & src );	// 指定参数与src相同
	virtual	BOOL	IsValidParameters( );			// 参数是否合法

	// Operations
	virtual	int		GetSignal( int nIndex, UINT * pnCode = NULL );	// 得到买卖信号
	virtual	int		GetIntensity( int nIndex, UINT * pnCode = NULL, BOOL bTraceBack = TRUE );	// 得到买卖趋势信号
	virtual	int		GetIntensityTraceback( int nIndex, UINT * pnCode = NULL );					// 得到买卖趋势信号
	virtual	BOOL	GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax );		// 得到最大最小值
	virtual	BOOL	Calculate( double * pValue, int nIndex, BOOL bUseLast );					// 计算指标值
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, int nIndex, BOOL bUseLast );		// 计算指标值
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, double * pValue3, int nIndex, BOOL bUseLast );		// 计算指标值
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, double * pValue3, double * pValue4, int nIndex, BOOL bUseLast );		// 计算指标值
	virtual	BOOL	Calculate( double * pValue1, double * pValue2, double * pValue3, double * pValue4, double * pValue5, int nIndex, BOOL bUseLast );		// 计算指标值

protected:
	UINT			m_nTechUserID;
	TECHUSER_INFO	m_techuserinfo;
	CStock *		m_pStock;
};

#endif	//__STKLIB_TECHNIQUE_H__
