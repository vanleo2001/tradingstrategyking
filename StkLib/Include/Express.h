/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CExpress;
		class	CIndex;
		class	CIndexContainer;
*/

#if	!defined( __STKLIB_EXPRESS_H__ )
#define	__STKLIB_EXPRESS_H__

/***
	定义股票列表项的唯一标识
*/
	//	股票
#define	SLH_NONE					0
#define	SLH_MIN						1
#define	SLH_CODE					1	//	股票代码
#define	SLH_NAME					2	//	股票名称
	//	技术数据
#define	SLH_DATE					3	//	日期
#define	SLH_LASTCLOSE				4	//	昨收
#define	SLH_OPEN					5	//	今开
#define	SLH_CLOSE					6	//	收盘价
#define	SLH_HIGH					7	//	最高价
#define	SLH_LOW						8	//	最低价
#define	SLH_AVERAGE					9	//	均价
#define	SLH_DIFF					10	//	涨跌
#define	SLH_DIFFPERCENT				11	//	涨幅%
#define	SLH_SCOPE					12	//	震幅%
#define	SLH_VOLUME					13	//	成交量
#define	SLH_AMOUNT					14	//	成交额
#define	SLH_VOLUP					15	//	外盘
#define	SLH_VOLDOWN					16	//	内盘
#define	SLH_DIFFPERCENT_MIN5		17	//	五分钟涨幅%
#define	SLH_SELLBUYRATIO			18	//	委比%
#define	SLH_SELLBUYDIFF				19	//	委量差
#define	SLH_BUYPRICE3				20	//	买价三
#define	SLH_BUYPRICE2				21	//	买价二
#define	SLH_BUYPRICE1				22	//	买价一
#define	SLH_SELLPRICE1				23	//	卖价一
#define	SLH_SELLPRICE2				24	//	卖价二
#define	SLH_SELLPRICE3				25	//	卖价三
#define	SLH_BUYVOLUME3				26	//	买量三
#define	SLH_BUYVOLUME2				27	//	买量二
#define	SLH_BUYVOLUME1				28	//	买量一
#define	SLH_SELLVOLUME1				29	//	卖量一
#define	SLH_SELLVOLUME2				30	//	卖量二
#define	SLH_SELLVOLUME3				31	//	卖量三

	//	主要基本指标
#define	SLH_REPORTTYPE				32	//	报表类型
#define	SLH_PE						33	//	市盈率
#define	SLH_PNETASSET				34	//	市净率
#define	SLH_PMAININCOME				35	//	市销率
#define	SLH_RATIO_PCASH				36	//	价格净现金比
#define	SLH_RATIO_CURRENCY			37	//	流通率
#define	SLH_RATIO_CHANGEHAND		38	//	换手率
#define	SLH_RATIO_VOLUME			39	//	量比
#define	SLH_RS						40	//	相对强度
#define	SLH_MARKETVALUE				41	//	总市值
#define	SLH_MARKETVALUEA			42	//	A股市值
#define	SLH_MARKETVALUEB			43	//	B股市值
	// ★偿债能力
#define	SLH_RATIO_LIQUIDITY			44	//	流动比率
#define	SLH_RATIO_QUICK				45	//	速动比率
#define	SLH_VELOCITY_RECEIVABLES	46	//	应收帐款周率
	// ★经营能力
#define	SLH_VELOCITY_MERCHANDISE	47	// 存货周转率
#define	SLH_MAIN_INCOME				48	// 主营业务收入
#define	SLH_CASH_PS					49	// 每股净现金流量
	// ★盈利能力
#define	SLH_PROFIT_MARGIN			50	// 主营业务利润率
#define	SLH_NETASSET_YIELD			51	// 净资产收益率
	// ★资本结构
#define	SLH_DATE_BEGIN				52	//	上市日期
#define	SLH_SHARE_COUNT_TOTAL		53	//	总股本
#define	SLH_SHARE_COUNT_A			54	//	流通A股
#define	SLH_SHARE_COUNT_B			55	//	流通B股
#define	SLH_SHARE_COUNT_H			56	//	流通H股
#define	SLH_SHARE_COUNT_NATIONAL	57	//	国有股
#define	SLH_SHARE_COUNT_CORP		58	//	法人股
#define	SLH_PROFIT_PSUD				59	// 每股未分配利润
#define	SLH_ASSET					60	// 总资产
#define	SLH_RATIO_HOLDERRIGHT		61	// 股东权益比率
#define	SLH_RATIO_LONGDEBT			62	// 长期负债率
#define	SLH_RATIO_DEBT				63	// 资产负债率
	// ★投资收益能力
#define	SLH_NETASSET_PS				64	// 每股净资产
#define	SLH_NETASSET_PS_REGULATE	65	// 调整每股净资产
#define	SLH_EPS						66	// 每股收益
#define	SLH_EPS_DEDUCT				67	// 扣除后每股收益
#define	SLH_NET_PROFIT				68	// 净利润
#define	SLH_MAIN_PROFIT				69	// 主营业务利润
#define	SLH_TOTAL_PROFIT			70	// 利润总额
#define	SLH_PROFIT_INC				71	// 主营利润增长率
#define	SLH_INCOME_INC				72	// 主营收入增长率
#define	SLH_ASSET_INC				73	// 总资产增长率
#define	SLH_ANNGAINS_AVERAGE		74	// 年平均收益率
#define	SLH_ANNGAINS_STDDEV			75	// 收益标准差
#define	SLH_BETA					76	// β值
#define	SLH_SHARP					77	// 夏普风险指数
#define	SLH_TRADE					78	// 行业
#define	SLH_PROVINCE				79	// 省份
#define	SLH_MAX						79

#define	SLH_INVALID					(UINT(-1))
#define	SLH_USERDEFINE_BEGIN		10000

#define	SLH_WIDTH_MIN				60


#define	SLMAX_SLHTEXTLEN			256

typedef	struct	slh_data_t	{
	UINT	slh_id;
	char	string[SLMAX_SLHTEXTLEN+1];
	UINT	weight;
	char	varname[MP_MaxVarNameLen];
}SLH_DATA;

extern	SLH_DATA	slh_data_array[SLH_MAX+1];

STKLIB_API	CSPString	AfxGetVariantName( UINT nVariantID, BOOL bWithParameter );	// 根据股票列表项ID得到名称，bWithParameter表示名称中是否含有参数
STKLIB_API	BOOL		AfxGetVariantNameArray( CSPStringArray & astr, BOOL bWithParameter );	// 得到所有股票列表项的名称
STKLIB_API	UINT		AfxGetVariantID( CSPString strName, BOOL bWithParameter );	// 根据股票列表项名称得到ID
STKLIB_API	CSPString	AfxGetVariantVarName( int nVariantID );		// 根据股票列表项ID得到使用在表达式中的变量名称
STKLIB_API	CSPString	AfxGetVariantVarName( CSPString strName, BOOL bWithParameter );		// 根据股票列表项名称得到使用在表达式中的变量名称
STKLIB_API	BOOL		AfxGetVariantValue(UINT nVariantID, CStockInfo &info, double * pValue,
							   CStockContainer *pContainer );	// 得到某只股票的相应值，单位与股票列表视图中相同
STKLIB_API	int			AfxCompareVariantValue(int nVariantID, CStockInfo & info1, CStockInfo &info2,
								   CStockContainer *pContainer );	// 比较两只股票的相应值
STKLIB_API	CSPString	AfxGetVariantDispString(UINT nVariantID, CStockInfo &info,
									CStockContainer *pContainer );		// 得到某只股票的相应值的显示字串
STKLIB_API	COLORREF	AfxGetVariantColor( UINT nVariantID, CStockInfo &info );	// 得到某只股票的相应值的显示颜色

/***
	表达式，可以包含变量
*/
class STKLIB_API CExpress  
{
public:
	CExpress();
	CExpress( const CExpress &src );
	virtual ~CExpress();

	CExpress & operator = ( const CExpress &src);
	virtual	void Serialize( CSPArchive &ar );

	BOOL		SetExpressString( CSPString strExpressOrg );
	CSPString	GetExpressString( );

	virtual	CSPString	GetLastErrorMsg( );
	BOOL	GetExpressResult( double * pResult, BOOL * pError );

protected:
	virtual	BOOL	PreCompile( );
	virtual	BOOL	InitParser( );

protected:
	CSPString	m_strExpressOrg;		//	Save to disk
	int			m_nReserved;			//	Save to disk, Not Used

	BOOL		m_bIsModified;
	CSPString	m_strExpress;
	CMathParser	m_parser;
};

/***
	新建指标，包含含有变量的表达式，在股票列表上显示为一列
*/
class STKLIB_API CIndex : public CExpress
{
public:
	CIndex( );
	CIndex( const CIndex &src );
	~CIndex( );

	UINT			m_nID;
	CSPString		m_strName;
	CSPString		m_strDescript;
	CSPString		m_strReserved;

	CIndex	& operator = ( const CIndex &src );
	virtual	void	Serialize( CSPArchive & ar );
	BOOL	IsInvalidID( );
	BOOL	SetNextID( );

	virtual	BOOL	CheckExpressValid( );
	BOOL	GetExpressResult( double * pResult, CStockInfo &info, BOOL *pVariantNoValue, BOOL * pError );

protected:
	virtual	BOOL	PreCompile( );
	virtual	BOOL	InitParser( CStockInfo &info, BOOL *pVariantNoValue );

protected:
	CSPDWordArray	m_adwVariant;	// temp data
};

//template class __declspec(dllexport)	CSPArray<CIndex, CIndex &>;
typedef	CSPArray<CIndex, CIndex &>	CIndexArray;
/***
	新建指标数组
*/
class STKLIB_API CIndexContainer : public CIndexArray
{
public:
	CIndexContainer( );
	virtual ~CIndexContainer( );

	// for access CIndexArray
	virtual	int	Add(CIndex &newElement)	{	return CIndexArray::Add(newElement);	}

	void		RemoveIndex( UINT nID );
	CIndex	&	GetIndex( UINT nID );
	CIndex	&	GetIndex( CSPString strName );
	BOOL		EditIndex( CIndex &index );

	UINT		GetNextID( );

	void		FileSerialize( CSPArchive & ar );
};


#endif // __STKLIB_EXPRESS_H__
