/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CExpress;
		class	CIndex;
		class	CIndexContainer;
*/

#include "stdafx.h"
#include "../Include/Express.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

SLH_DATA	slh_data_array[SLH_MAX+1]	=	{
	{ SLH_NONE,						"",	100,	"" },
	{ SLH_CODE,						"",	100,	"" },
	{ SLH_NAME,						"",	110,	"" },
	//	技术数据
	{ SLH_DATE,						"",	110,	"" },
	{ SLH_LASTCLOSE,				"",	100,	"lastclose" },
	{ SLH_OPEN,						"",	100,	"open" },
	{ SLH_CLOSE,					"",	100,	"close" },
	{ SLH_HIGH,						"",	100,	"high" },
	{ SLH_LOW,						"",	100,	"low" },
	{ SLH_AVERAGE,					"",	100,	"average" },
	{ SLH_DIFF,						"",	100,	"diff" },
	{ SLH_DIFFPERCENT,				"",	100,	"diffpercent" },
	{ SLH_SCOPE,					"",	100,	"scope" },
	{ SLH_VOLUME,					"",	140,	"volume" },
	{ SLH_AMOUNT,					"",	140,	"amount" },
	{ SLH_VOLUP,					"",	100,	"volup" },
	{ SLH_VOLDOWN,					"",	100,	"voldown" },
	{ SLH_DIFFPERCENT_MIN5,			"",	100,	"diffpercent_min5" },
	{ SLH_SELLBUYRATIO,				"",	100,	"sellbuyratio" },
	{ SLH_SELLBUYDIFF,				"",	100,	"sellbuydiff" },
	{ SLH_BUYPRICE3,				"",	100,	"buyprice3" },
	{ SLH_BUYPRICE2,				"",	100,	"buyprice2" },
	{ SLH_BUYPRICE1,				"",	100,	"buyprice1" },
	{ SLH_SELLPRICE1,				"",	100,	"sellprice1" },
	{ SLH_SELLPRICE2,				"",	100,	"sellprice2" },
	{ SLH_SELLPRICE3,				"",	100,	"sellprice3" },
	{ SLH_BUYVOLUME3,				"",	100,	"buyvolume3" },
	{ SLH_BUYVOLUME2,				"",	100,	"buyvolume2" },
	{ SLH_BUYVOLUME1,				"",	100,	"buyvolume1" },
	{ SLH_SELLVOLUME1,				"",	100,	"sellvolume1" },
	{ SLH_SELLVOLUME2,				"",	100,	"sellvolume2" },
	{ SLH_SELLVOLUME3,				"",	100,	"sellvolume3" },
	
	//	主要基本指标
	{ SLH_REPORTTYPE,				"",	100,	"" },
	{ SLH_PE,						"",	100,	"pe" },
	{ SLH_PNETASSET,				"",	100,	"pnetasset" },
	{ SLH_PMAININCOME,				"",	100,	"pmainincome" },
	{ SLH_RATIO_PCASH,				"",	100,	"ratio_pcash" },
	{ SLH_RATIO_CURRENCY,			"",	100,	"ratio_currency" },
	{ SLH_RATIO_CHANGEHAND,			"",	100,	"ratio_changehand" },
	{ SLH_RATIO_VOLUME,				"",	100,	"ratio_volume" },
	{ SLH_RS,						"",	100,	"rs" },
	{ SLH_MARKETVALUE,				"",	170,	"marketvalue" },
	{ SLH_MARKETVALUEA,				"",	170,	"marketvaluea" },
	{ SLH_MARKETVALUEB,				"",	170,	"marketvalueb" },
	// ★偿债能力
	{ SLH_RATIO_LIQUIDITY,			"",	140,	"ratio_liquidity" },
	{ SLH_RATIO_QUICK,				"",	140,	"ratio_quick" },
	{ SLH_VELOCITY_RECEIVABLES,		"",	170,	"velocity_receivables" },
	// ★经营能力
	{ SLH_VELOCITY_MERCHANDISE,		"",	150,	"velocity_merchandise" },
	{ SLH_MAIN_INCOME,				"",	160,	"main_income" },
	{ SLH_CASH_PS,					"",	170,	"cash_ps" },
	// ★盈利能力
	{ SLH_PROFIT_MARGIN,			"",	170,	"profit_margin" },
	{ SLH_NETASSET_YIELD,			"",	190,	"netasset_yield" },
	// ★资本结构
	{ SLH_DATE_BEGIN,				"",	160,	"" },
	{ SLH_SHARE_COUNT_TOTAL,		"",	160,	"share_count_total" },
	{ SLH_SHARE_COUNT_A,			"",	140,	"share_count_a" },
	{ SLH_SHARE_COUNT_B,			"",	140,	"share_count_b" },
	{ SLH_SHARE_COUNT_H,			"",	140,	"share_count_h" },
	{ SLH_SHARE_COUNT_NATIONAL,		"",	140,	"share_count_national" },
	{ SLH_SHARE_COUNT_CORP,			"",	140,	"share_count_corp" },
	{ SLH_PROFIT_PSUD,				"",	170,	"profit_psud" },
	{ SLH_ASSET,					"",	140,	"asset" },
	{ SLH_RATIO_HOLDERRIGHT,		"",	170,	"ratio_holderright" },
	{ SLH_RATIO_LONGDEBT,			"",	170,	"ratio_longdebt" },
	{ SLH_RATIO_DEBT,				"",	170,	"ration_debt" },
	// ★投资收益能力
	{ SLH_NETASSET_PS,				"",	150,	"netasset_ps" },
	{ SLH_NETASSET_PS_REGULATE,		"",	200,	"netasset_ps_regulate" },
	{ SLH_EPS,						"",	140,	"eps" },
	{ SLH_EPS_DEDUCT,				"",	170,	"eps_deduct" },
	{ SLH_NET_PROFIT,				"",	140,	"net_profit" },
	{ SLH_MAIN_PROFIT,				"",	140,	"main_profit" },
	{ SLH_TOTAL_PROFIT,				"",	140,	"total_profit" },
	{ SLH_PROFIT_INC,				"",	170,	"profit_inc" },
	{ SLH_INCOME_INC,				"",	170,	"income_inc" },
	{ SLH_ASSET_INC,				"",	190,	"asset_inc" },
	{ SLH_ANNGAINS_AVERAGE,			"",	170,	"anngains_average" },
	{ SLH_ANNGAINS_STDDEV,			"",	170,	"anngains_stddev" },
	{ SLH_BETA,						"",	100,	"beta" },
	{ SLH_SHARP,					"",	100,	"sharp" },
	{ SLH_TRADE,					"",	100,	"" },
	{ SLH_PROVINCE,					"",	100,	"" },
};

CSPString AfxGetVariantName( UINT nVariantID, BOOL bWithParameter )
{
	if( nVariantID >= SLH_USERDEFINE_BEGIN )
	{
		CIndexContainer	& aindex	=	AfxGetProfile().GetSListColumnsUser();
		CIndex	index	=	aindex.GetIndex( nVariantID );
		return index.m_strName;
	}
	if( nVariantID < SLH_MIN || nVariantID > SLH_MAX )
		return "";
	
	CSPString	strResult;
	if( strResult.IsEmpty() )
		strResult	=	slh_data_array[nVariantID].string;
	if( strResult.IsEmpty() )
	{
		SP_ASSERT( nVariantID == slh_data_array[nVariantID].slh_id );
		strResult	=	AfxGetSLHTitle( nVariantID );

		int	size	=	sizeof(slh_data_array[nVariantID].string);
		strncpy( slh_data_array[nVariantID].string, strResult, size-1 );
		slh_data_array[nVariantID].string[size-1]	=	0;
	}

	if( bWithParameter )
	{
		if( SLH_DIFF == nVariantID || SLH_DIFFPERCENT == nVariantID || SLH_SCOPE == nVariantID
			|| SLH_RATIO_CHANGEHAND == nVariantID || SLH_RATIO_VOLUME == nVariantID
			|| SLH_RS == nVariantID || SLH_ANNGAINS_AVERAGE == nVariantID )
		{
			CSPString	strSuf, strTemp;
			strSuf	=	slh_hdrday;

			if( SLH_DIFF == nVariantID )
				strTemp.Format( "%s-%d%s", (LPCTSTR)strResult, AfxGetProfile().GetDiffDays(), (LPCTSTR)strSuf );
			else if( SLH_DIFFPERCENT == nVariantID )
				strTemp.Format( "%s-%d%s", (LPCTSTR)strResult, AfxGetProfile().GetDiffPercentDays(), (LPCTSTR)strSuf );
			else if( SLH_SCOPE == nVariantID )
				strTemp.Format( "%s-%d%s", (LPCTSTR)strResult, AfxGetProfile().GetScopeDays(), (LPCTSTR)strSuf );
			else if( SLH_RATIO_CHANGEHAND == nVariantID )
				strTemp.Format( "%s-%d%s", (LPCTSTR)strResult, AfxGetProfile().GetRatioChangeHandDays(), (LPCTSTR)strSuf );
			else if( SLH_RATIO_VOLUME == nVariantID )
				strTemp.Format( "%s-%d%s", (LPCTSTR)strResult, AfxGetProfile().GetRatioVolumeDays(), (LPCTSTR)strSuf );
			else if( SLH_RS == nVariantID )
				strTemp.Format( "%s-%d%s", (LPCTSTR)strResult, AfxGetProfile().GetRSDays(), (LPCTSTR)strSuf );	
			else if( SLH_ANNGAINS_AVERAGE == nVariantID )
				strTemp.Format( "%s-%d%s", (LPCTSTR)strResult, AfxGetProfile().GetYieldAverageDays(), (LPCTSTR)strSuf );
			else
				SP_ASSERT( FALSE );
			strResult	=	strTemp;
		}
	}
	
	return strResult;
}

BOOL AfxGetVariantNameArray( CSPStringArray & astr, BOOL bWithParameter )
{
	astr.SetSize( 0, SLH_MAX+SLH_MIN+1 );
	for( int nVariantID = min(0,SLH_MIN); nVariantID <= SLH_MAX; nVariantID ++ )
	{
		if( strlen(slh_data_array[nVariantID].varname) > 0 )
		{
			CSPString	strTemp	=	AfxGetVariantName( nVariantID, bWithParameter );
			astr.Add( strTemp );
		}
		else
		{
			astr.Add( "" );
		}
	}
	return TRUE;
}

UINT AfxGetVariantID( CSPString strName, BOOL bWithParameter )
{
	for( int nVariantID = SLH_MIN; nVariantID <= SLH_MAX; nVariantID ++ )
	{
		CSPString	strTemp	=	AfxGetVariantName( nVariantID, bWithParameter );
		if( 0 == strTemp.CompareNoCase( strName ) )
			return nVariantID;
	}

	CIndexContainer	& aindex	=	AfxGetProfile().GetSListColumnsUser();
	CIndex	index	=	aindex.GetIndex( strName );
	if( !index.IsInvalidID() )
		return index.m_nID;

	return SLH_INVALID;
}

CSPString AfxGetVariantVarName( int nVariantID )
{
	if( nVariantID >= SLH_MIN && nVariantID <= SLH_MAX )
		return slh_data_array[nVariantID].varname;
	else
		return "";
}

CSPString AfxGetVariantVarName( CSPString strName, BOOL bWithParameter )
{
	UINT nVariantID = AfxGetVariantID( strName, bWithParameter );
	return AfxGetVariantVarName( nVariantID );
}

/* 计算股票列表视图中某一列的值，其数值单位与股票列表视图中相同 */
BOOL AfxGetVariantValue(UINT nVariantID, CStockInfo &info, double * pValue,
						CStockContainer *pContainer )
{
	if( NULL == pValue )
	{
		SP_ASSERT( FALSE );
		return FALSE;
	}

	double	factor	=	1; // CStock::GetReportFactor( info.m_reporttype );

	if( nVariantID >= SLH_MIN && nVariantID <= SLH_MAX )
	{
		switch( nVariantID )
		{
		case	SLH_LASTCLOSE:					//	昨收
			*pValue	=	info.m_fLast;
			return ( info.m_fLast > 1e-4 );
		case	SLH_OPEN:						//	今开
			*pValue	=	info.m_fOpen;
			return ( info.m_fOpen > 1e-4 );
		case	SLH_CLOSE:						//	收盘价
			*pValue	=	info.m_fClose;
			return ( info.m_fClose > 1e-4 );
		case	SLH_HIGH:						//	最高价
			*pValue	=	info.m_fHigh;
			return ( info.m_fHigh > 1e-4 );
		case	SLH_LOW:						//	最低价
			*pValue	=	info.m_fLow;
			return ( info.m_fLow > 1e-4 );
		case	SLH_AVERAGE:					//	均价
			return info.GetAverage( pValue );
		case	SLH_DIFF:						//	涨跌
			return info.GetDiff( pValue, info.m_datetech, AfxGetProfile().GetDiffDays() );
		case	SLH_DIFFPERCENT:				//	涨幅
			return info.GetDiffPercent( pValue, info.m_datetech, AfxGetProfile().GetDiffPercentDays() );
		case	SLH_SCOPE:						//	震幅
			return info.GetScope( pValue, info.m_datetech, AfxGetProfile().GetScopeDays() );
		case	SLH_VOLUME:						//	成交量
			*pValue	=	info.m_fVolume*0.01;
			return ( info.m_fVolume > 1e-4 );
		case	SLH_AMOUNT:						//	成交额
			*pValue	=	info.m_fAmount*0.001;
			return ( info.m_fAmount > 1e-4 );
		case	SLH_VOLUP:						//	外盘
			if( info.m_minute.StatVolumeInfo( NULL, pValue, NULL ) )
			{
				*pValue	=	(*pValue) * 0.01;
				return TRUE;
			}
			break;
		case	SLH_VOLDOWN:					//	内盘
			if( info.m_minute.StatVolumeInfo( NULL, NULL, pValue ) )
			{
				*pValue	=	(*pValue) * 0.01;
				return TRUE;
			}
			break;
		case	SLH_DIFFPERCENT_MIN5:			//	五分钟涨幅%
			return info.GetDiffPercentMin5( pValue );
		case	SLH_SELLBUYRATIO:				//	委比%
			return info.GetSellBuyRatio( pValue, NULL );
		case	SLH_SELLBUYDIFF:				//	委量差
			if( info.GetSellBuyRatio( NULL, pValue ) )
			{
				*pValue	=	(*pValue) * 0.01;
				return TRUE;
			}
			break;
		case	SLH_BUYPRICE3:					//	买价三
			*pValue	=	info.m_fBuyPrice[2];
			return ( info.m_fBuyPrice[2] > 1e-4 );
		case	SLH_BUYPRICE2:					//	买价二
			*pValue	=	info.m_fBuyPrice[1];
			return ( info.m_fBuyPrice[1] > 1e-4 );
		case	SLH_BUYPRICE1:					//	买价一
			*pValue	=	info.m_fBuyPrice[0];
			return ( info.m_fBuyPrice[0] > 1e-4 );
		case	SLH_SELLPRICE1:					//	卖价一
			*pValue	=	info.m_fSellPrice[0];
			return ( info.m_fSellPrice[0] > 1e-4 );
		case	SLH_SELLPRICE2:					//	卖价二
			*pValue	=	info.m_fSellPrice[1];
			return ( info.m_fSellPrice[1] > 1e-4 );
		case	SLH_SELLPRICE3:					//	卖价三
			*pValue	=	info.m_fSellPrice[2];
			return ( info.m_fSellPrice[2] > 1e-4 );
		case	SLH_BUYVOLUME3:					//	买量三
			*pValue	=	info.m_fBuyVolume[2] * 0.01;
			return ( info.m_fBuyVolume[2] > 1e-4 );
		case	SLH_BUYVOLUME2:					//	买量二
			*pValue	=	info.m_fBuyVolume[1] * 0.01;
			return ( info.m_fBuyVolume[1] > 1e-4 );
		case	SLH_BUYVOLUME1:					//	买量一
			*pValue	=	info.m_fBuyVolume[0] * 0.01;
			return ( info.m_fBuyVolume[0] > 1e-4 );
		case	SLH_SELLVOLUME1:				//	卖量一
			*pValue	=	info.m_fSellVolume[0] * 0.01;
			return ( info.m_fSellVolume[0] > 1e-4 );
		case	SLH_SELLVOLUME2:				//	卖量二
			*pValue	=	info.m_fSellVolume[1] * 0.01;
			return ( info.m_fSellVolume[1] > 1e-4 );
		case	SLH_SELLVOLUME3:				//	卖量三
			*pValue	=	info.m_fSellVolume[2] * 0.01;
			return ( info.m_fSellVolume[2] > 1e-4 );

			//	主要基本指标
		case	SLH_PE:							//	市盈率
			return info.GetPE( pValue );
		case	SLH_PNETASSET:					//	市净率
			return info.GetPNetAsset( pValue );
		case	SLH_PMAININCOME:				//	市销率
			return info.GetPMainIncome( pValue );
		case	SLH_RATIO_PCASH:				//	价格净现金比
			return info.GetRatioPCash( pValue );
		case	SLH_RATIO_CURRENCY:				//	流通率
			return info.GetRatioCurrency( pValue );
		case	SLH_RATIO_CHANGEHAND:			//	换手率
			return info.GetRatioChangeHand( pValue, info.m_datetech, AfxGetProfile().GetRatioChangeHandDays() );
		case	SLH_RATIO_VOLUME:				//	量比
			return info.GetRatioVolume( pValue, info.m_datetech, AfxGetProfile().GetRatioVolumeDays() );
		case	SLH_RS:							//	相对强度
			return info.GetRS( pValue, info.m_datetech, AfxGetProfile().GetRSDays() );
		case	SLH_MARKETVALUE:				//	总市值
			if( info.GetMarketValue( pValue ) )
			{
				*pValue	=	(*pValue) * 0.0001;
				return TRUE;
			}
			break;
		case	SLH_MARKETVALUEA:				//	A股市值
			if( info.GetMarketValueA( pValue ) )
			{
				*pValue	=	(*pValue) * 0.0001;
				return TRUE;
			}
			break;
		case	SLH_MARKETVALUEB:				//	B股市值
			if( info.GetMarketValueB( pValue ) )
			{
				*pValue	=	(*pValue) * 0.0001;
				return TRUE;
			}
			break;
			
			// ★偿债能力
		case	SLH_RATIO_LIQUIDITY:			//	流动比率
			*pValue	=	info.m_fRatio_liquidity;
			return ( fabs(info.m_fRatio_liquidity) > 1e-4 );
		case	SLH_RATIO_QUICK:				//	速动比率
			*pValue	=	info.m_fRatio_quick;
			return ( fabs(info.m_fRatio_quick) > 1e-4 );
		case	SLH_VELOCITY_RECEIVABLES:		//	应收帐款周率
			*pValue	=	info.m_fVelocity_receivables;
			return ( fabs(info.m_fVelocity_receivables) > 1e-4 );

			// ★经营能力
		case	SLH_VELOCITY_MERCHANDISE:		// 存货周转率
			*pValue	=	info.m_fVelocity_merchandise;
			return ( fabs(info.m_fVelocity_merchandise) > 1e-4 );
		case	SLH_MAIN_INCOME:				// 主营业务收入
			*pValue	=	factor * info.m_fMain_income * 0.0001;
			return ( fabs(info.m_fMain_income) > 1e-4 );
		case	SLH_CASH_PS:					// 每股净现金流量
			*pValue	=	factor * info.m_fCash_ps;
			return ( fabs(info.m_fCash_ps) > 1e-4 );

			// ★盈利能力
		case	SLH_PROFIT_MARGIN:				// 主营业务利润率
			*pValue	=	info.m_fProfit_margin;
			return ( fabs(info.m_fProfit_margin) > 1e-4 );
		case	SLH_NETASSET_YIELD:				// 净资产收益率
			*pValue	=	factor * info.m_fNetasset_yield;
			return ( fabs(info.m_fNetasset_yield) > 1e-4 );
			
			// ★资本结构
		case	SLH_SHARE_COUNT_TOTAL:			//	总股本
			*pValue	=	info.m_fShare_count_total * 0.0001;
			return ( info.m_fShare_count_total > 1e-4 );
		case	SLH_SHARE_COUNT_A:				//	流通A股
			*pValue	=	info.m_fShare_count_a * 0.0001;
			return TRUE;
		case	SLH_SHARE_COUNT_B:				//	流通B股
			*pValue	=	info.m_fShare_count_b * 0.0001;
			return TRUE;
		case	SLH_SHARE_COUNT_H:				//	流通H股
			*pValue	=	info.m_fShare_count_h * 0.0001;
			return TRUE;
		case	SLH_SHARE_COUNT_NATIONAL:		//	国有股
			*pValue	=	info.m_fShare_count_national * 0.0001;
			return TRUE;
		case	SLH_SHARE_COUNT_CORP:			//	法人股
			*pValue	=	info.m_fShare_count_corp * 0.0001;
			return TRUE;
		case	SLH_PROFIT_PSUD:				// 每股未分配利润
			*pValue	=	info.m_fProfit_psud;
			return TRUE;
		case	SLH_ASSET:						// 总资产
			*pValue	=	info.m_fAsset * 0.0001;
			return ( fabs(info.m_fAsset) > 1e-4 );
		case	SLH_RATIO_HOLDERRIGHT:			// 股东权益比率
			*pValue	=	info.m_fRatio_holderright;
			return ( fabs(info.m_fRatio_holderright) > 1e-4 );
		case	SLH_RATIO_LONGDEBT:				// 长期负债率
			*pValue	=	info.m_fRatio_longdebt;
			return TRUE;
		case	SLH_RATIO_DEBT:					// 资产负债率
			*pValue	=	info.m_fRatio_debt;
			return ( fabs(info.m_fRatio_debt) > 1e-4 );

			// ★投资收益能力
		case	SLH_NETASSET_PS:				// 每股净资产
			*pValue	=	info.m_fNetasset_ps;
			return ( fabs(info.m_fNetasset_ps) > 1e-4 );
		case	SLH_NETASSET_PS_REGULATE:		// 调整每股净资产
			*pValue	=	info.m_fNetasset_ps_regulate;
			return ( fabs(info.m_fNetasset_ps_regulate) > 1e-4 );
		case	SLH_EPS:						// 每股收益
			*pValue	=	factor * info.m_fEps;
			return ( fabs(info.m_fEps) > 1e-4 );
		case	SLH_EPS_DEDUCT:					// 扣除后每股收益
			*pValue	=	factor * info.m_fEps_deduct;
			return ( fabs(info.m_fEps_deduct) > 1e-4 );
		case	SLH_NET_PROFIT:					// 净利润
			*pValue	=	factor * info.m_fNet_profit * 0.0001;
			return ( fabs(info.m_fNet_profit) > 1e-4 );
		case	SLH_MAIN_PROFIT:				// 主营业务利润
			*pValue	=	factor * info.m_fMain_profit * 0.0001;
			return ( fabs(info.m_fMain_profit) > 1e-4 );
		case	SLH_TOTAL_PROFIT:				// 利润总额
			*pValue	=	factor * info.m_fTotal_profit * 0.0001;
			return ( fabs(info.m_fTotal_profit) > 1e-4 );
		case	SLH_PROFIT_INC:					// 主营利润增长率
			*pValue	=	info.m_fProfit_inc;
			return TRUE;
		case	SLH_INCOME_INC:					// 主营收入增长率
			*pValue	=	info.m_fIncome_inc;
			return TRUE;
		case	SLH_ASSET_INC:					// 总资产增长率
			*pValue	=	info.m_fAsset_inc;
			return TRUE;
		case	SLH_ANNGAINS_AVERAGE:			// 年平均收益率
			*pValue	=	info.m_fYield_average;
			return ( fabs(STKLIB_DATA_INVALID - info.m_fYield_average) > 1e-4 );
		case	SLH_ANNGAINS_STDDEV:			// 收益标准差
			*pValue	=	info.m_fYield_stddev;
			return ( fabs(STKLIB_DATA_INVALID - info.m_fYield_stddev) > 1e-4 );
		case	SLH_BETA:						// β值
			*pValue	=	info.m_fBeite;
			return ( fabs(STKLIB_DATA_INVALID - info.m_fBeite) > 1e-4 );
		case	SLH_SHARP:						// 夏普风险指数
			return info.GetXiapu( pValue );
		default:	;
		}
	}
	else if( nVariantID >= SLH_USERDEFINE_BEGIN )
	{
		BOOL	bNoValue	=	FALSE;
		if( pContainer && pContainer->GetVariantSaveValue( pValue, nVariantID, info, &bNoValue ) )
			return !bNoValue;

		CIndexContainer	& container = AfxGetProfile().GetSListColumnsUser();
		CIndex	& index	=	container.GetIndex( nVariantID );
		BOOL bSuccess = index.GetExpressResult( pValue, info, &bNoValue, NULL );
		if( pContainer && pValue )
			pContainer->SetVariantSaveValue( *pValue, nVariantID, info, bNoValue );
		
		return bSuccess;
	}
	return FALSE;
}

int	dwordcmp( DWORD dw1, DWORD dw2 )
{
	if( dw1 > dw2 )
		return 1;
	else if( dw1 < dw2 )
		return -1;
	else
		return 0;
}

int	longcmp( LONG l1, LONG l2 )
{
	if( l1 > l2 )
		return 1;
	else if( l1 < l2 )
		return -1;
	else
		return 0;
}

int doublecmp( double d1, double d2 )
{
	if( d1 > d2 )
		return 1;
	else if( d1 < d2 )
		return -1;
	else
		return 0;
}

int AfxCompareVariantValue( int nVariantID, CStockInfo & info1, CStockInfo &info2,
						    CStockContainer * pContainer )
{
	long	x1 = 0, x2 = 0;
	double	d1 = 0., d2 = 0.;
	BOOL	bGet1 = FALSE, bGet2 = FALSE;

	switch( nVariantID )
	{
	case	SLH_CODE:						//	股票代码
		return strcmp( info1.GetStockCode(), info2.GetStockCode() );
	case	SLH_NAME:						//	股票名称
		return strcmp( info1.GetStockName(), info2.GetStockName() );
		//	技术数据
	case	SLH_DATE:						//	日期
		return dwordcmp( info1.m_datetech, info2.m_datetech );
	case	SLH_DATE_BEGIN:					//	上市日期
		return dwordcmp( info1.m_datebegin, info2.m_datebegin );
		//	主要基本指标
	case	SLH_REPORTTYPE:					// 报表类型
		return dwordcmp( info1.m_reporttype, info2.m_reporttype );
	case	SLH_PE:							//	市盈率
	case	SLH_PNETASSET:					//	市净率
	case	SLH_PMAININCOME:				//	市销率
	case	SLH_RATIO_PCASH:				//	价格净现金比
		bGet2	=	AfxGetVariantValue( nVariantID, info2, &d2, pContainer );
		bGet1	=	AfxGetVariantValue( nVariantID, info1, &d1, pContainer );
		if( !bGet2 && !bGet1 )
			return 0;
		if( !bGet2 )	return 1;
		if( !bGet1 )	return -1;
		if( d1 < 0 && d2 < 0 )
			return doublecmp( d1, d2 );
		if( d1 < 0 )			return 1;
		if( d2 < 0 )			return -1;
		return doublecmp( d1, d2 );
	case	SLH_TRADE:						// 行业
		return strcmp( info1.GetStockDomain(), info2.GetStockDomain() );
	case	SLH_PROVINCE:					// 省份
		return strcmp( info1.GetStockProvince(), info2.GetStockProvince() );
	default:
		bGet2	=	AfxGetVariantValue( nVariantID, info2, &d2, pContainer );
		bGet1	=	AfxGetVariantValue( nVariantID, info1, &d1, pContainer );
		if( !bGet2 && !bGet1 )
			return 0;
		if( !bGet2 )	return 1;
		if( !bGet1 )	return -1;
		return doublecmp( d1, d2 );
	}

	return 0;
}

CSPString AfxGetVariantDispString(UINT nVariantID, CStockInfo &info,
								CStockContainer * pContainer )
{
	CSPString	strTemp;
	int		x	=	0;
	double	dTemp	=	0;

	switch( nVariantID )
	{
	case	SLH_CODE:						//	股票代码
		strTemp	=	info.GetStockCode();
		break;
	case	SLH_NAME:						//	股票名称
		strTemp	=	info.GetStockName();
		break;
	//	技术数据
	case	SLH_DATE:						//	日期
		if( 0 != info.m_datetech )
			strTemp.Format( "%.2d/%.2d/%.2d", (info.m_datetech/10000)%100, (info.m_datetech%10000)/100, info.m_datetech%100 );
		break;
	case	SLH_DATE_BEGIN:					//	上市日期
		if( 0 != info.m_datebegin )
			strTemp.Format( "%.2d/%.2d/%.2d", (info.m_datebegin/10000)%100, (info.m_datebegin%10000)/100, info.m_datebegin%100 );
		break;
	case	SLH_LASTCLOSE:					//	昨收
	case	SLH_OPEN:						//	今开
	case	SLH_CLOSE:						//	收盘价
	case	SLH_HIGH:						//	最高价
	case	SLH_LOW:						//	最低价
	case	SLH_AVERAGE:					//	均价
	case	SLH_DIFF:						//	涨跌
	case	SLH_BUYPRICE3:					//	买价三
	case	SLH_BUYPRICE2:					//	买价二
	case	SLH_BUYPRICE1:					//	买价一
	case	SLH_SELLPRICE1:					//	卖价一
	case	SLH_SELLPRICE2:					//	卖价二
	case	SLH_SELLPRICE3:					//	卖价三
		if( AfxGetVariantValue( nVariantID, info, &dTemp, pContainer ) )
		{
			CSPString strPriceFmt;
			strPriceFmt.Format( "%%.%df", info.DigitBit() );
			strTemp.Format( strPriceFmt, dTemp );
		}
		break;
	case	SLH_VOLUME:						//	成交量
	case	SLH_AMOUNT:						//	成交额
	case	SLH_VOLUP:						//	外盘
	case	SLH_VOLDOWN:					//	内盘
	case	SLH_SELLBUYDIFF:				//	委量差
	case	SLH_BUYVOLUME3:					//	买量三
	case	SLH_BUYVOLUME2:					//	买量二
	case	SLH_BUYVOLUME1:					//	买量一
	case	SLH_SELLVOLUME1:				//	卖量一
	case	SLH_SELLVOLUME2:				//	卖量二
	case	SLH_SELLVOLUME3:				//	卖量三
	case	SLH_MARKETVALUE:				//	总市值
	case	SLH_MARKETVALUEA:				//	A股市值
	case	SLH_MARKETVALUEB:				//	B股市值
	case	SLH_SHARE_COUNT_TOTAL:			//	总股本
	case	SLH_SHARE_COUNT_A:				//	流通A股
	case	SLH_SHARE_COUNT_B:				//	流通B股
	case	SLH_SHARE_COUNT_H:				//	流通H股
	case	SLH_SHARE_COUNT_NATIONAL:		//	国有股
	case	SLH_SHARE_COUNT_CORP:			//	法人股
		if( AfxGetVariantValue( nVariantID, info, &dTemp, pContainer ) )
			strTemp.Format( "%.0f", dTemp );
		break;
	case	SLH_MAIN_INCOME:				// 主营业务收入
	case	SLH_ASSET:						// 总资产
	case	SLH_NET_PROFIT:					// 净利润
	case	SLH_MAIN_PROFIT:				// 主营业务利润
	case	SLH_TOTAL_PROFIT:				// 利润总额
		if( AfxGetVariantValue( nVariantID, info, &dTemp, pContainer ) )
			strTemp.Format( "%.0f", dTemp );
		break;
	case	SLH_REPORTTYPE:					// 报表类型
		strTemp	=	AfxGetReportTypeString( info.m_reporttype );
		break;
	case	SLH_TRADE:						// 行业
		strTemp	=	info.GetStockDomain();
		break;
	case	SLH_PROVINCE:					// 省份
		strTemp	=	info.GetStockProvince();
		break;
	default:
		if( AfxGetVariantValue( nVariantID, info, &dTemp, pContainer ) )
			strTemp.Format( "%.2f", dTemp );
	}

	if( strTemp.IsEmpty() )
		strTemp	=	"-";

	return strTemp;
}

COLORREF AfxGetVariantColor( UINT nVariantID, CStockInfo &info )
{
	int		x		=	0;
	double	dTemp	=	0;
	COLORREF	crText	=	AfxGetProfile().GetColor(CColorClass::clrPlane);

	if( nVariantID < SLH_MIN || nVariantID > SLH_MAX )
		return crText;

	switch( nVariantID )
	{
	case	SLH_CODE:						//	股票代码
		crText	=	AfxGetProfile().GetColor(CColorClass::clrTitle);
		break;
	case	SLH_NAME:						//	股票名称
		crText	=	AfxGetProfile().GetColor(CColorClass::clrTitle);
		break;
	case	SLH_OPEN:						//	今开
		if( info.m_fOpen > info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrRise);
		else if( info.m_fOpen < info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrFall);
		break;
	case	SLH_CLOSE:						//	收盘价
		if( info.m_fClose > info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrRise);
		else if( info.m_fClose < info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrFall);
		break;
	case	SLH_HIGH:						//	最高价
		if( info.m_fHigh > info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrRise);
		else if( info.m_fHigh < info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrFall);
		break;
	case	SLH_LOW:						//	最低价
		if( info.m_fLow > info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrRise);
		else if( info.m_fLow < info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrFall);
		break;
	case	SLH_AVERAGE:					//	均价
		if( info.GetAverage( &dTemp ) )
		{
			if( dTemp - info.m_fLast > 1e-4 )
				crText	=	AfxGetProfile().GetColor(CColorClass::clrRise);
			else if( dTemp - info.m_fLast < -1e-4 )
				crText	=	AfxGetProfile().GetColor(CColorClass::clrFall);
		}
		break;
	case	SLH_DIFF:						//	涨跌
		if( info.GetDiff( &dTemp, info.m_datetech, AfxGetProfile().GetDiffPercentDays() ) )
		{
			if( dTemp > 1e-4 )
				crText	=	AfxGetProfile().GetColor(CColorClass::clrRise);
			else if( dTemp < -1e-4 )
				crText	=	AfxGetProfile().GetColor(CColorClass::clrFall);
		}
		break;
	case	SLH_DIFFPERCENT:				//	涨幅
		if( info.GetDiffPercent( &dTemp, info.m_datetech, AfxGetProfile().GetDiffPercentDays() ) )
		{
			if( dTemp > 1e-4 )
				crText	=	AfxGetProfile().GetColor(CColorClass::clrRise);
			else if( dTemp < -1e-4 )
				crText	=	AfxGetProfile().GetColor(CColorClass::clrFall);
		}
		break;
	case	SLH_VOLUME:						//	成交量
		crText	=	AfxGetProfile().GetColor(CColorClass::clrText);
		break;
	case	SLH_AMOUNT:						//	成交额
		crText	=	AfxGetProfile().GetColor(CColorClass::clrText);
		break;

	case	SLH_VOLUP:						//	外盘
		crText	=	AfxGetProfile().GetColor(CColorClass::clrRise);
		break;
	case	SLH_VOLDOWN:					//	内盘
		crText	=	AfxGetProfile().GetColor(CColorClass::clrFall);
		break;
	case	SLH_DIFFPERCENT_MIN5:			//	五分钟涨幅%
		if( info.GetDiffPercentMin5( &dTemp ) )
		{
			if( dTemp > 1e-4 )
				crText	=	AfxGetProfile().GetColor(CColorClass::clrRise);
			else if( dTemp < -1e-4 )
				crText	=	AfxGetProfile().GetColor(CColorClass::clrFall);
		}
		break;

	case	SLH_SELLBUYRATIO:				//	委比%
	case	SLH_SELLBUYDIFF:				//	委量差
		if( info.GetSellBuyRatio( &dTemp, NULL ) )
		{
			if( dTemp > 1e-4 )
				crText	=	AfxGetProfile().GetColor(CColorClass::clrRise);
			else if( dTemp < -1e-4 )
				crText	=	AfxGetProfile().GetColor(CColorClass::clrFall);
		}
		break;
	
	case	SLH_BUYPRICE3:					//	买价三
		if( info.m_fBuyPrice[2] > info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrRise);
		else if( info.m_fBuyPrice[2] < info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrFall);
		break;
	case	SLH_BUYPRICE2:					//	买价二
		if( info.m_fBuyPrice[1] > info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrRise);
		else if( info.m_fBuyPrice[1] < info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrFall);
		break;
	case	SLH_BUYPRICE1:					//	买价一
		if( info.m_fBuyPrice[0] > info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrRise);
		else if( info.m_fBuyPrice[0] < info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrFall);
		break;
	case	SLH_SELLPRICE1:					//	卖价一
		if( info.m_fSellPrice[0] > info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrRise);
		else if( info.m_fSellPrice[0] < info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrFall);
		break;
	case	SLH_SELLPRICE2:					//	卖价二
		if( info.m_fSellPrice[1] > info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrRise);
		else if( info.m_fSellPrice[1] < info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrFall);
		break;
	case	SLH_SELLPRICE3:					//	卖价三
		if( info.m_fSellPrice[2] > info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrRise);
		else if( info.m_fSellPrice[2] < info.m_fLast )
			crText	=	AfxGetProfile().GetColor(CColorClass::clrFall);
		break;

	case	SLH_BUYVOLUME3:					//	买量三
	case	SLH_BUYVOLUME2:					//	买量二
	case	SLH_BUYVOLUME1:					//	买量一
	case	SLH_SELLVOLUME1:				//	卖量一
	case	SLH_SELLVOLUME2:				//	卖量二
	case	SLH_SELLVOLUME3:				//	卖量三
	case	SLH_SCOPE:
		crText	=	AfxGetProfile().GetColor(CColorClass::clrPlane);
		break;

		//	主要基本指标
	case	SLH_REPORTTYPE:					//	报表类型
		crText	=	AfxGetProfile().GetColor(CColorClass::clrTitle);
		break;

	case	SLH_PE:							//	市盈率
	case	SLH_PNETASSET:					//	市净率
	case	SLH_PMAININCOME:				//	市销率
	case	SLH_RATIO_PCASH:				//	价格净现金比
	case	SLH_RATIO_CURRENCY:				//	流通率
	case	SLH_RATIO_CHANGEHAND:			//	换手率
	case	SLH_RATIO_VOLUME:				//	量比
	case	SLH_RS:							//	相对强度
		crText	=	AfxGetProfile().GetColor(CColorClass::clrPlane);
		break;

	case	SLH_MARKETVALUE:				//	总市值
	case	SLH_MARKETVALUEA:				//	A股市值
	case	SLH_MARKETVALUEB:				//	B股市值
		crText	=	AfxGetProfile().GetColor(CColorClass::clrText);
		break;
		
		// ★偿债能力
	case	SLH_RATIO_LIQUIDITY:			//	流动比率
	case	SLH_RATIO_QUICK:				//	速动比率
	case	SLH_VELOCITY_RECEIVABLES:		//	应收帐款周率
		crText	=	AfxGetProfile().GetColor(CColorClass::clrPlane);
		break;
		
		// ★经营能力
	case	SLH_VELOCITY_MERCHANDISE:		// 存货周转率
		crText	=	AfxGetProfile().GetColor(CColorClass::clrPlane);
		break;
	case	SLH_MAIN_INCOME:				// 主营业务收入
	case	SLH_CASH_PS:					// 每股净现金流量
		crText	=	AfxGetProfile().GetColor(CColorClass::clrText);
		break;
		// ★盈利能力
	case	SLH_PROFIT_MARGIN:				// 主营业务利润率
	case	SLH_NETASSET_YIELD:				// 净资产收益率
		crText	=	AfxGetProfile().GetColor(CColorClass::clrPlane);
		break;
		
		// ★资本结构
	case	SLH_SHARE_COUNT_TOTAL:			//	总股本
	case	SLH_SHARE_COUNT_A:				//	流通A股
	case	SLH_SHARE_COUNT_B:				//	流通B股
	case	SLH_SHARE_COUNT_H:				//	流通H股
	case	SLH_SHARE_COUNT_NATIONAL:		//	国有股
	case	SLH_SHARE_COUNT_CORP:			//	法人股
	case	SLH_PROFIT_PSUD:				// 每股未分配利润
	case	SLH_ASSET:						// 总资产
		crText	=	AfxGetProfile().GetColor(CColorClass::clrText);
		break;
	case	SLH_RATIO_HOLDERRIGHT:			// 股东权益比率
	case	SLH_RATIO_LONGDEBT:				// 长期负债率
	case	SLH_RATIO_DEBT:					// 资产负债率
		crText	=	AfxGetProfile().GetColor(CColorClass::clrPlane);
		break;

		// ★投资收益能力
	case	SLH_NETASSET_PS:				// 每股净资产
	case	SLH_NETASSET_PS_REGULATE:		// 调整每股净资产
	case	SLH_EPS:						// 每股收益
	case	SLH_EPS_DEDUCT:					// 扣除后每股收益
	case	SLH_NET_PROFIT:					// 净利润
	case	SLH_MAIN_PROFIT:				// 主营业务利润
	case	SLH_TOTAL_PROFIT:				// 利润总额
		crText	=	AfxGetProfile().GetColor(CColorClass::clrText);
		break;
	case	SLH_PROFIT_INC:					// 主营利润增长率
	case	SLH_INCOME_INC:					// 主营收入增长率
	case	SLH_ASSET_INC:					// 总资产增长率
	case	SLH_ANNGAINS_AVERAGE:			// 年平均收益率
	case	SLH_ANNGAINS_STDDEV:			// 收益标准差
	case	SLH_BETA:						// β值
	case	SLH_SHARP:						// 夏普风险指数
		crText	=	AfxGetProfile().GetColor(CColorClass::clrPlane);
		break;
	case	SLH_TRADE:						// 行业
	case	SLH_PROVINCE:					// 省份
		crText	=	AfxGetProfile().GetColor(CColorClass::clrTitle);
		break;
	default:	;
	}

	return crText;
}

//////////////////////////////////////////////////////////////////////
// CExpress

CExpress::CExpress( )
{
	m_bIsModified	=	FALSE;
	m_nReserved		=	0;
}

CExpress::CExpress( const CExpress &src )
{
	*this	=	src;
}

CExpress::~CExpress( )
{
}

CExpress &CExpress::operator = ( const CExpress &src )
{
	m_strExpressOrg		=	src.m_strExpressOrg;
	m_strExpress		=	src.m_strExpress;
	m_bIsModified		=	src.m_bIsModified;
	m_nReserved			=	src.m_nReserved;
	return *this;
}

void CExpress::Serialize( CSPArchive &ar )
{
	if( ar.IsStoring() )
	{
		ar	<<	m_strExpressOrg;
		ar	<<	m_nReserved;
	}
	else
	{
		ar	>>	m_strExpressOrg;
		ar	>>	m_nReserved;

		m_bIsModified	=	TRUE;
		m_strExpress	=	m_strExpressOrg;
	}
}

BOOL CExpress::SetExpressString( CSPString strExpressOrg )
{
	m_strExpressOrg	=	strExpressOrg;

	m_bIsModified	=	TRUE;
	m_strExpress	=	m_strExpressOrg;
	return TRUE;
}

CSPString CExpress::GetExpressString( )
{
	return m_strExpressOrg;
}

CSPString CExpress::GetLastErrorMsg( )
{
	if( m_parser.IsError() )
	{
		int nErrorCode = m_parser.GetErrorCode( );
		switch( nErrorCode )
		{
		case MP_ErrParserStack:	return express_errparserstack;	break;
		case MP_ErrBadRange:	return express_errbadrange;		break;
		case MP_ErrExpression:	return express_errexpression;	break;
		case MP_ErrOperator:	return express_erroperator;		break;
		case MP_ErrOpenParen:	return express_erropenparen;	break;
		case MP_ErrOpCloseParen:return express_errcloseparen;	break;
		case MP_ErrInvalidNum:	return express_errinvalidnum;	break;
		case MP_ErrMath:		return express_errmath;			break;
		default:				return express_errunknown;
		}
	}
	return "";
}

BOOL CExpress::GetExpressResult( double * pResult, BOOL * pError )
{
	if( !PreCompile( ) )
	{
		if( pError )	*pError	=	TRUE;
		return FALSE;
	}

	if( !InitParser( ) )
	{
		if( pError )	*pError	=	TRUE;
		return FALSE;
	}

	m_parser.Parse( );
	if( pError )
		*pError	=	m_parser.IsError( );
	if( m_parser.IsError() )
		return FALSE;

	if( pResult )
		*pResult	=	m_parser.GetResult( );
	return TRUE;
}

// 去掉空格、冒号、回车、换行等字符
BOOL CExpress::PreCompile( )
{
	if( m_bIsModified )
	{
		CSPString	sTemp = m_strExpress;
		m_strExpress.Empty();
		for( int i=0; i<sTemp.GetLength(); i++ )
		{
			TCHAR ch = sTemp[i];
			if( ':' != ch && ' ' != ch && '\t' != ch 
				&& '\r' != ch && '\n' != ch )
				m_strExpress	+=	ch;
		}

		m_bIsModified	=	FALSE;
	}
	return m_strExpress.GetLength() > 0;
}

// 初始化m_parser变量
BOOL CExpress::InitParser( )
{
	m_parser.SetParserString( m_strExpress );
	m_parser.InitVarMap( 10 );
/* examples
	for( int i=0; i<nCount; i++ )
	{
		m_parser.AddVar( dValue, strName );
	}
*/
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CIndex

CIndex::CIndex( )
{
	m_nID	=	SLH_INVALID;
}

CIndex::CIndex( const CIndex &src )
{
	*this	=	src;
}

CIndex::~CIndex( )
{
}

CIndex &CIndex::operator = ( const CIndex &src )
{
	*((CExpress *)this)	=	*((CExpress *)&src);
	m_nID			=	src.m_nID;
	m_strName		=	src.m_strName;
	m_strDescript	=	src.m_strDescript;
	m_strReserved	=	src.m_strReserved;

	m_adwVariant.Copy( src.m_adwVariant );
	return *this;
}

void CIndex::Serialize( CSPArchive & ar )
{
	CExpress::Serialize( ar );

	if( ar.IsStoring() )
	{
		ar	<<	m_nID;
		ar	<<	m_strName;
		ar	<<	m_strDescript;
		ar	<<	m_strReserved;
	}
	else
	{
		ar	>>	m_nID;
		ar	>>	m_strName;
		ar	>>	m_strDescript;
		ar	>>	m_strReserved;
	}
}

BOOL CIndex::IsInvalidID( )
{
	return SLH_INVALID == m_nID;
}

BOOL CIndex::SetNextID( )
{
	CIndexContainer &container = AfxGetProfile().GetSListColumnsUser();
	m_nID	=	container.GetNextID( );
	return TRUE;
}

BOOL CIndex::PreCompile( )
{
	if( m_bIsModified )
	{
		// 缓存变量ID
		m_adwVariant.RemoveAll();

		// 将中文变量名替换成英文名
		CSPStringArray astrVarName;
		if( AfxGetVariantNameArray(astrVarName,FALSE) )
		{
			CSPDWordArray adwSortID;
			astrVarName.GetSortIDArray( adwSortID );
			for( int i=adwSortID.GetSize()-1; i>=0; i-- )
			{
				UINT nVariantID = adwSortID.GetAt(i);
				if( strlen(slh_data_array[nVariantID].varname) > 0 )
				{
					if( m_strExpress.Replace( AfxGetVariantName(nVariantID,FALSE), slh_data_array[nVariantID].varname ) > 0 )
						m_adwVariant.Add(nVariantID);
				}
			}
		}

		if( !CExpress::PreCompile( ) )
			return FALSE;

		m_bIsModified	=	FALSE;
	}
	return m_strExpress.GetLength() > 0;
}

// 初始化m_parser变量
BOOL CIndex::InitParser( CStockInfo &info, BOOL *pVariantNoValue )
{
	if( pVariantNoValue )
		*pVariantNoValue	=	FALSE;

	m_parser.SetParserString( m_strExpress );
	m_parser.InitVarMap( SLH_MAX );

	for( int i=0; i<m_adwVariant.GetSize(); i++ )
	{
		UINT nVariantID = m_adwVariant.ElementAt(i);

		// 加入变量及其值
		double	value	=	1.0;
		if( !AfxGetVariantValue( nVariantID, info, &value, NULL ) && pVariantNoValue )
			*pVariantNoValue	=	TRUE;
		m_parser.AddVar( value, slh_data_array[nVariantID].varname );
	}
	return TRUE;
}

BOOL CIndex::CheckExpressValid( )
{
	CStockInfo	info;
	BOOL	bError	=	FALSE;
	double	temp	=	0;
	GetExpressResult( &temp, info, NULL, &bError );
	return !bError;
}

BOOL CIndex::GetExpressResult( double * pResult, CStockInfo &info, BOOL *pVariantNoValue, BOOL * pError )
{
	if( !PreCompile( ) )
	{
		if( pVariantNoValue )	*pVariantNoValue	=	FALSE;
		if( pError )	*pError	=	TRUE;
		return FALSE;
	}

	BOOL	bVariantNoValue	=	FALSE;
	if( !InitParser( info, &bVariantNoValue ) )
	{
		if( pVariantNoValue )	*pVariantNoValue	=	bVariantNoValue;
		if( pError )	*pError	=	TRUE;
		return FALSE;
	}
	if( pVariantNoValue )	*pVariantNoValue	=	bVariantNoValue;

	m_parser.Parse( );
	if( pError )
		*pError	=	m_parser.IsError( );
	if( m_parser.IsError() )
		return FALSE;

	if( bVariantNoValue )
		return FALSE;

	if( pResult )
		*pResult	=	m_parser.GetResult( );
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CIndexContainer

CIndexContainer::CIndexContainer( )
{
}

CIndexContainer::~CIndexContainer( )
{
	RemoveAll( );
}

void CIndexContainer::RemoveIndex( UINT nID )
{
	for( int i=0; i<GetSize(); i++ )
	{
		CIndex & col = ElementAt(i);
		if( nID == col.m_nID )
		{
			RemoveAt( i );
			return;
		}
	}
}

CIndex & CIndexContainer::GetIndex( UINT nID )
{
	static CIndex idx;
	for( int i=0; i<GetSize(); i++ )
	{
		CIndex & col = ElementAt(i);
		if( nID == col.m_nID )
			return col;
	}
	return idx;
}

CIndex & CIndexContainer::GetIndex( CSPString strName )
{
	static CIndex idx;
	for( int i=0; i<GetSize(); i++ )
	{
		CIndex & index = ElementAt(i);
		if( 0 == strName.Compare( index.m_strName ) )
			return index;
	}
	return idx;
}

BOOL CIndexContainer::EditIndex( CIndex &indexEdit )
{
	for( int i=0; i<GetSize(); i++ )
	{
		CIndex & index = ElementAt(i);
		if( indexEdit.m_nID == index.m_nID )
		{
			index	=	indexEdit;
			return TRUE;
		}
	}
	return FALSE;
}

UINT CIndexContainer::GetNextID( )
{
	UINT	nID	=	SLH_USERDEFINE_BEGIN;
	for( int i=0; i<GetSize(); i++ )
	{
		CIndex & index = ElementAt(i);
		if( index.m_nID >= nID )
		{
			nID	=	index.m_nID + 1;
		}
	}
	return nID;
}

void CIndexContainer::FileSerialize( CSPArchive & ar )
{
	if( ar.IsStoring() )
	{
		ar << GetSize();
		for( int i=0; i<GetSize(); i++ )
		{
			CIndex	& col = ElementAt(i);
			col.Serialize( ar );
		}
	}
	else
	{
		int	size;

		ar >> size;
		SetSize( size );
		for( int i=0; i<size; i++ )
		{
			CIndex	col;
			col.Serialize( ar );
			SetAt( i, col );
		}
	}
}
