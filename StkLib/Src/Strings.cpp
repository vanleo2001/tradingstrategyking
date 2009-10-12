// strings.cpp : implementation file
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1997 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include	"StdAfx.h"
#include	"Qianlong.h"
#include	"Shenglong.h"
#include	"SelfDB.h"
#include	"Strings.h"

#ifdef	CLKLAN_ENGLISH_US
char CQianlong::m_szDataType[]	=	"QianLong";
char CShenglong::m_szDataType[]	=	"Salon";
char CSelfDB::m_szDataType[]	=	"Ninebulls-Stock";
#else
char CQianlong::m_szDataType[]	=	"钱龙";
char CShenglong::m_szDataType[]	=	"胜龙";
char CSelfDB::m_szDataType[]	=	"策略为王";
#endif

//////////////////////////////////////////////////////////////////////////////////
// 字符串
#ifdef	CLKLAN_ENGLISH_US
char	sz_stockname_fund[]		=	"基金";
char	sz_stockname_bond[]		=	"债券";
char	sz_stockname_index[]	=	"指数";
char	sz_stockname_bondindex[]		=	"国债指数";
char	sz_stockname_bondindexcorp[]	=	"企债指数";
#else
char	sz_stockname_fund[]		=	"基金";
char	sz_stockname_bond[]		=	"债券";
char	sz_stockname_index[]	=	"指数";
char	sz_stockname_bondindex[]		=	"国债指数";
char	sz_stockname_bondindexcorp[]	=	"企债指数";
#endif

//////////////////////////////////////////////////////////////////////////////////
// 指标买卖信号字符串
#ifdef	CLKLAN_ENGLISH_US
char	its_sellintense[]		=	"Strong Sell";
char	its_sell[]				=	"Sell";
char	its_sellfeeble[]		=	"Faintly Sell";
char	its_nothing[]			=	"Nothing";
char	its_buyfeeble[]			=	"Faintly Buy";
char	its_buy[]				=	"Buy";
char	its_buyintense[]		=	"Strong Buy";

char	itsc_nothing[]			=	"nothing";
char	itsc_deviateonbottom[]	=	"deviate on bottom";
char	itsc_deviateontop[]		=	"deviate on top";
char	itsc_goldenfork[]		=	"golden fork";
char	itsc_deadfork[]			=	"dead fork";
char	itsc_long[]				=	"long";
char	itsc_short[]			=	"short";
char	itsc_overbought[]		=	"overbought";
char	itsc_oversold[]			=	"oversold";
char	itsc_support[]			=	"support";
char	itsc_resistance[]		=	"resistance";
#else
char	its_sellintense[]		=	"强烈卖出";
char	its_sell[]				=	"卖出";
char	its_sellfeeble[]		=	"微弱卖出";
char	its_nothing[]			=	"无";
char	its_buyfeeble[]			=	"微弱买进";
char	its_buy[]				=	"买进";
char	its_buyintense[]		=	"强烈买进";

char	itsc_nothing[]			=	"无";
char	itsc_deviateonbottom[]	=	"底背离";
char	itsc_deviateontop[]		=	"顶背离";
char	itsc_goldenfork[]		=	"金叉";
char	itsc_deadfork[]			=	"死叉";
char	itsc_long[]				=	"多头强势";
char	itsc_short[]			=	"空头强势";
char	itsc_overbought[]		=	"超买";
char	itsc_oversold[]			=	"超卖";
char	itsc_support[]			=	"支撑";
char	itsc_resistance[]		=	"压力";
#endif

//////////////////////////////////////////////////////////////////////////////////
// 指标字符串

// 给定技术指标唯一编号，返回技术指标中文名称
CSPString	AfxGetSTTName( UINT nTech )
{
	static	CSPString	stt_names[STT_MAX+1];
	SP_ASSERT( STT_MAX >= 0 );
	
	if( stt_names[0].GetLength() <= 0 )
	{
		// stt_names
		stt_names[0]			=	"STTNames";

#ifdef	CLKLAN_ENGLISH_US
		//	K线属性类
		stt_names[STT_MA]		=	("Moving Average");
		stt_names[STT_BBI]		=	("BBI");
		stt_names[STT_BOLL]		=	("Boollinger Bands");
		stt_names[STT_PV]		=	("Price/Volume");
		stt_names[STT_SAR]		=	("Stop And Reverse");
		stt_names[STT_DJ]		=	("Superposition");
		stt_names[STT_CW]		=	("Chips Distribution");
		//	趋势类
		stt_names[STT_MACD]		=	("Moving Average Convergence/Divergence");
		stt_names[STT_MIKE]		=	("Mike Base");
		stt_names[STT_PSY]		=	("Psychological Line");
		//	能量类
		stt_names[STT_VOLUME]	=	("Volume");
		stt_names[STT_NVI]		=	("Negative Volume Indicator");
		stt_names[STT_PVI]		=	("Positive Volume Indicator");
		stt_names[STT_VR]		=	("Volume Ratio");
		stt_names[STT_VROC]		=	("Volume Rate Indicator");
		stt_names[STT_OBV]		=	("On Balance Volume");
		stt_names[STT_MOBV]		=	("Moving On Balance Volume");
		stt_names[STT_MFI]		=	("Money Flow Indicator");
		stt_names[STT_VMACD]	=	("Volume Moving Average Convergence/Divergence");
		stt_names[STT_WVAD]		=	("William's Variable Accumulation Distribution");
		stt_names[STT_EMV]		=	("Ease of Movement Value");
		stt_names[STT_VRSI]		=	("Volume Relative Strength Indicator");
		stt_names[STT_NVRSI]	=	("New Volume Relative Strength Indicator");
		stt_names[STT_AD]		=	("Accumulation/Distribution");
		stt_names[STT_CI]		=	("Cumulation Indicator");
		//	摆动类
		stt_names[STT_KDJ]		=	("Stochastics");
		stt_names[STT_R]		=	("William's Overbought/Oversold Indicator");
		stt_names[STT_RSI]		=	("Relative Strength Indicator");
		stt_names[STT_BIAS]		=	("Bias");
		stt_names[STT_MTM]		=	("Momentum");
		stt_names[STT_DMI]		=	("Directional Movement Indicator");
		stt_names[STT_ROC]		=	("Rate Of Change");
		stt_names[STT_CCI]		=	("Commodity Channel Indicator");
		stt_names[STT_CV]		=	("Chavkin");
		stt_names[STT_ARBR]		=	("AR BR");
		stt_names[STT_CR]		=	("CR");
		stt_names[STT_OSC]		=	("Oscillator");
		stt_names[STT_UOS]		=	("Ultimate Oscillator");
		stt_names[STT_MAOSC]	=	("Moving Average Oscillator");
		stt_names[STT_36BIAS]	=	("3-6 Bias");
		stt_names[STT_DPO]		=	("Detrended Price Oscillator");
		stt_names[STT_KST]		=	("Know Sure Things");
		//	其他类
		stt_names[STT_REI]		=	("Range Expansion Indicator");
		stt_names[STT_DMKI]		=	("Demaker Indicator");
		stt_names[STT_PCNT]		=	("Percent");
		stt_names[STT_HLC]		=	("Highest and Lowest and Close Price");
		stt_names[STT_CDP]		=	("CDP");
		stt_names[STT_ASI]		=	("Accumulation Swing Indicator");
		stt_names[STT_ATR]		=	("Average True Ranger");
		//	策略为王指标类
		stt_names[STT_CYO]		=	("Cycle Oscillator");
		stt_names[STT_DCYO]		=	("Degaussed Cycle Oscillator");
		stt_names[STT_HSL]		=	("Relative Change Hands");
		stt_names[STT_DPER]		=	("Detrended Percent Oscillator");
#else
		//	K线属性类
		stt_names[STT_MA]		=	("移动平均线");
		stt_names[STT_BBI]		=	("多空指标");
		stt_names[STT_BOLL]		=	("布林带");
		stt_names[STT_PV]		=	("汇证均价线");
		stt_names[STT_SAR]		=	("停损点转向指标");
		stt_names[STT_DJ]		=	("叠加图");
		stt_names[STT_CW]		=	("筹码分布图");
		//	趋势类
		stt_names[STT_MACD]		=	("指数平滑异同移动平均线");
		stt_names[STT_MIKE]		=	("麦克指标");
		stt_names[STT_PSY]		=	("心理线");
		//	能量类
		stt_names[STT_VOLUME]	=	("成交量");
		stt_names[STT_NVI]		=	("负成交量指标");
		stt_names[STT_PVI]		=	("正成交量指标");
		stt_names[STT_VR]		=	("容量比率");
		stt_names[STT_VROC]		=	("成交量变动率指标");
		stt_names[STT_OBV]		=	("能量线");
		stt_names[STT_MOBV]		=	("主力进出");
		stt_names[STT_MFI]		=	("资金流动指数");
		stt_names[STT_VMACD]	=	("量指数平滑异同移动平均线");
		stt_names[STT_WVAD]		=	("威廉变异离散量");
		stt_names[STT_EMV]		=	("简易波动指标");
		stt_names[STT_VRSI]		=	("量相对强弱指标");
		stt_names[STT_NVRSI]	=	("新量相对强弱指标");
		stt_names[STT_AD]		=	("聚/散指标");
		stt_names[STT_CI]		=	("累积指数");
		//	摆动类
		stt_names[STT_KDJ]		=	("随机指标");
		stt_names[STT_R]		=	("威廉指标");
		stt_names[STT_RSI]		=	("相对强弱指标");
		stt_names[STT_BIAS]		=	("乖离率");
		stt_names[STT_MTM]		=	("动量指标");
		stt_names[STT_DMI]		=	("动向指标");
		stt_names[STT_ROC]		=	("变动率指标");
		stt_names[STT_CCI]		=	("顺势通道指标");
		stt_names[STT_CV]		=	("佳庆指标");
		stt_names[STT_ARBR]		=	("人气意愿指标");
		stt_names[STT_CR]		=	("中间意愿指标");
		stt_names[STT_OSC]		=	("摆动量");
		stt_names[STT_UOS]		=	("极摆动指标");
		stt_names[STT_MAOSC]	=	("移动平均摆动量");
		stt_names[STT_36BIAS]	=	("3减6日乖离");
		stt_names[STT_DPO]		=	("去势价格摆动指数");
		stt_names[STT_KST]		=	("确然指标");
		//	其他类
		stt_names[STT_REI]		=	("范围膨胀指数");
		stt_names[STT_DMKI]		=	("迪马克尔指标");
		stt_names[STT_PCNT]		=	("幅度比");
		stt_names[STT_HLC]		=	("幅度分析");
		stt_names[STT_CDP]		=	("逆势操作指标");
		stt_names[STT_ASI]		=	("累积摆动指标");
		stt_names[STT_ATR]		=	("均幅指标");
		//	策略为王指标类
		stt_names[STT_CYO]		=	("周期摆动量");
		stt_names[STT_DCYO]		=	("去噪周期摆动量");
		stt_names[STT_HSL]		=	("相对换手率");
		stt_names[STT_DPER]		=	("去势百分比指标");
#endif
	}
	
	if( nTech >= STT_MIN && nTech <= STT_MAX )
	{
		return	stt_names[nTech];
	}
	if( nTech >= STT_USER_MIN )
	{
		return CTechUser::GetTechUserName( nTech );
	}
	return "";
}

// 给定技术指标唯一编号，返回技术指标英文简称
CSPString	AfxGetSTTShortName( UINT nTech )
{
	static	CSPString	stt_shortnames[STT_MAX+1];
	SP_ASSERT( STT_MAX >= 0 );

	if( stt_shortnames[0].GetLength() <= 0 )
	{
		// stt_shortnames
		stt_shortnames[0]			=	"STTShortNames";

		//	K线属性类
		stt_shortnames[STT_MA]		=	("MA");
		stt_shortnames[STT_BBI]		=	("BBI");
		stt_shortnames[STT_BOLL]	=	("BOLL");
		stt_shortnames[STT_PV]		=	("P/V");
		stt_shortnames[STT_SAR]		=	("SAR");
		stt_shortnames[STT_DJ]		=	("DJ");
		stt_shortnames[STT_CW]		=	("CW");
		//	趋势类
		stt_shortnames[STT_MACD]	=	("MACD");
		stt_shortnames[STT_MIKE]	=	("MIKE");
		stt_shortnames[STT_PSY]		=	("PSY");
		//	能量类
		stt_shortnames[STT_VOLUME]	=	("VOLUME");
		stt_shortnames[STT_NVI]		=	("NVI");
		stt_shortnames[STT_PVI]		=	("PVI");
		stt_shortnames[STT_VR]		=	("VR");
		stt_shortnames[STT_VROC]	=	("VROC");
		stt_shortnames[STT_OBV]		=	("OBV");
		stt_shortnames[STT_MOBV]	=	("MOBV");
		stt_shortnames[STT_MFI]		=	("MFI");
		stt_shortnames[STT_VMACD]	=	("VMACD");
		stt_shortnames[STT_WVAD]	=	("WVAD");
		stt_shortnames[STT_EMV]		=	("EMV");
		stt_shortnames[STT_VRSI]	=	("VRSI");
		stt_shortnames[STT_NVRSI]	=	("NVRSI");
		stt_shortnames[STT_AD]		=	("A/D");
		stt_shortnames[STT_CI]		=	("CI");
		//	摆动类
		stt_shortnames[STT_KDJ]		=	("KDJ");
		stt_shortnames[STT_R]		=	("R");
		stt_shortnames[STT_RSI]		=	("RSI");
		stt_shortnames[STT_BIAS]	=	("BIAS");
		stt_shortnames[STT_MTM]		=	("MTM");
		stt_shortnames[STT_DMI]		=	("DMI");
		stt_shortnames[STT_ROC]		=	("ROC");
		stt_shortnames[STT_CCI]		=	("CCI");
		stt_shortnames[STT_CV]		=	("CV");
		stt_shortnames[STT_ARBR]	=	("ARBR");
		stt_shortnames[STT_CR]		=	("CR");
		stt_shortnames[STT_OSC]		=	("OSC");
		stt_shortnames[STT_UOS]		=	("UOS");
		stt_shortnames[STT_MAOSC]	=	("MAOSC");
		stt_shortnames[STT_36BIAS]	=	("3-6BIAS");
		stt_shortnames[STT_DPO]		=	("DPO");
		stt_shortnames[STT_KST]		=	("KST");
		//	其他类
		stt_shortnames[STT_REI]		=	("REI");
		stt_shortnames[STT_DMKI]	=	("DMKI");
		stt_shortnames[STT_PCNT]	=	("PCNT");
		stt_shortnames[STT_HLC]		=	("HLC");
		stt_shortnames[STT_CDP]		=	("CDP");
		stt_shortnames[STT_ASI]		=	("ASI");
		stt_shortnames[STT_ATR]		=	("ATR");
		//	策略为王指标类
		stt_shortnames[STT_CYO]		=	("CYO");
		stt_shortnames[STT_DCYO]	=	("DCYO");
		stt_shortnames[STT_HSL]		=	("HSL");
		stt_shortnames[STT_DPER]	=	("DPER");
	}

	if( nTech >= STT_MIN && nTech <= STT_MAX )
	{
		return	stt_shortnames[nTech];
	}
	if( nTech >= STT_USER_MIN )
	{
		return CTechUser::GetTechUserShortName( nTech );
	}
	return "";
}

// 给定技术指标唯一编号，返回技术指标中文名称+英文简称
CSPString	AfxGetSTTFullName( UINT nTech )
{
	return AfxGetSTTName( nTech ) + "(" + AfxGetSTTShortName( nTech ) + ")";
}

// 给定技术指标唯一编号，返回技术指标类别名称
CSPString	AfxGetSTTClassName( UINT nTech )
{
	static	CSPString	stt_classnames[STT_CLASS_MAX+1];
	SP_ASSERT( STT_MAX >= 0 );

	if( stt_classnames[0].GetLength() <= 0 )
	{
		// stt_classnames
		stt_classnames[0]					=	"STTClassNames";

#ifdef	CLKLAN_ENGLISH_US
		stt_classnames[STT_CLASS_KLINE]		=	"K Line Attribute";
		stt_classnames[STT_CLASS_TREND]		=	"Trend";
		stt_classnames[STT_CLASS_ENERGY]	=	"Energy";
		stt_classnames[STT_CLASS_SWING]		=	"Swing";
		stt_classnames[STT_CLASS_OTHER]		=	"Others";
		stt_classnames[STT_CLASS_CLK]		=	"Ninebulls-Stock";
		stt_classnames[STT_CLASS_USER]		=	"User Define";
#else
		stt_classnames[STT_CLASS_KLINE]		=	"K线属性类";
		stt_classnames[STT_CLASS_TREND]		=	"趋势类";
		stt_classnames[STT_CLASS_ENERGY]	=	"能量类";
		stt_classnames[STT_CLASS_SWING]		=	"摆动类";
		stt_classnames[STT_CLASS_OTHER]		=	"其它类";
		stt_classnames[STT_CLASS_CLK]		=	"策略为王指标类";
		stt_classnames[STT_CLASS_USER]		=	"外挂指标类";
#endif
	}

	if( nTech >= STT_KLINE_MIN && nTech <= STT_KLINE_MAX )
		return stt_classnames[STT_CLASS_KLINE];
	else if( nTech >= STT_TREND_MIN && nTech <= STT_TREND_MAX )
		return stt_classnames[STT_CLASS_TREND];
	else if( nTech >= STT_ENERGY_MIN && nTech <= STT_ENERGY_MAX )
		return stt_classnames[STT_CLASS_ENERGY];
	else if( nTech >= STT_SWING_MIN && nTech <= STT_SWING_MAX )
		return stt_classnames[STT_CLASS_SWING];
	else if( nTech >= STT_OTHER_MIN && nTech <= STT_OTHER_MAX )
		return stt_classnames[STT_CLASS_OTHER];
	else if( nTech >= STT_CLK_MIN && nTech <= STT_CLK_MAX )
		return stt_classnames[STT_CLASS_CLK];
	else if( nTech >= STT_USER_MIN )
		return stt_classnames[STT_CLASS_USER];
	return "";
}

// 给定买卖信号值，得到中文说明
CSPString	AfxGetIntensityString( int nIntensity )
{
	if( nIntensity >= ITS_MIN && nIntensity <= ITSG_SELLINTENSE )
		return its_sellintense;
	else if( nIntensity > ITSG_SELLINTENSE && nIntensity <= ITSG_SELL )
		return its_sell;
	else if( nIntensity > ITSG_SELL && nIntensity <= ITSG_SELLFEEBLE )
		return its_sellfeeble;
	else if( nIntensity > ITSG_SELLFEEBLE && nIntensity < ITSG_BUYFEEBLE  )
		return its_nothing;
	else if( nIntensity >= ITSG_BUYFEEBLE && nIntensity < ITSG_BUY )
		return its_buyfeeble;
	else if( nIntensity >= ITSG_BUY && nIntensity < ITSG_BUYINTENSE )
		return its_buy;
	else if( nIntensity >= ITSG_BUYINTENSE && nIntensity <= ITS_MAX )
		return its_buyintense;
	
	SP_ASSERT( FALSE );
	return "";
}

// 给定买卖信号理由说明值，得到中文说明
CSPString	AfxGetIntensityCodeString( UINT nCode )
{
	switch( nCode )
	{
	case	ITSC_NOTHING:			return	itsc_nothing;
	case	ITSC_DEVIATEONBOTTOM:	return	itsc_deviateonbottom;
	case	ITSC_DEVIATEONTOP:		return	itsc_deviateontop;
	case	ITSC_GOLDENFORK:		return	itsc_goldenfork;
	case	ITSC_DEADFORK:			return	itsc_deadfork;
	case	ITSC_LONG:				return	itsc_long;
	case	ITSC_SHORT:				return	itsc_short;
	case	ITSC_OVERBOUGHT:		return	itsc_overbought;
	case	ITSC_OVERSOLD:			return	itsc_oversold;
	case	ITSC_SUPPORT:			return	itsc_support;
	case	ITSC_RESISTANCE:		return	itsc_resistance;
	default:
		SP_ASSERT( FALSE );
	}
	return "";
}

CSPString AfxGetKTypeString( int ktype )
{
#ifdef	CLKLAN_ENGLISH_US
	if( CKData::ktypeMonth == ktype )
		return "Month Line";
	else if( CKData::ktypeWeek == ktype )
		return "Week Line";
	else if( CKData::ktypeDay == ktype )
		return "Day Line";
	else if( CKData::ktypeMin5 == ktype )
		return "5 Minutes Line";
	else if( CKData::ktypeMin15 == ktype )
		return "15 Minutes Line";
	else if( CKData::ktypeMin30 == ktype )
		return "30 Minutes Line";
	else if( CKData::ktypeMin60 == ktype )
		return "60 Minutes Line";
	else
		return "Unknown";
#else
	if( CKData::ktypeMonth == ktype )
		return "月线";
	else if( CKData::ktypeWeek == ktype )
		return "周线";
	else if( CKData::ktypeDay == ktype )
		return "日线";
	else if( CKData::ktypeMin5 == ktype )
		return "五分钟线";
	else if( CKData::ktypeMin15 == ktype )
		return "十五分钟线";
	else if( CKData::ktypeMin30 == ktype )
		return "三十分钟线";
	else if( CKData::ktypeMin60 == ktype )
		return "六十分钟线";
	else
		return "未知";
#endif
	return "";
}

CSPString AfxGetMaindataTypeString( int type )
{
#ifdef	CLKLAN_ENGLISH_US
	if( CKData::mdtypeClose == type )
		return "Close Price";
	else if( CKData::mdtypeOpen == type )
		return "Open Price";
	else if( CKData::mdtypeAverage == type )
		return "Average Price";
	else
		return "Unknown";
#else
	if( CKData::mdtypeClose == type )
		return "收盘价";
	else if( CKData::mdtypeOpen == type )
		return "开盘价";
	else if( CKData::mdtypeAverage == type )
		return "平均价";
	else
		return "未知";
#endif
	return "";
}

//////////////////////////////////////////////////////////////////////////////////
// 股票列表列名称字符串
#ifdef	CLKLAN_ENGLISH_US
char	slh_hdrday[]	=	"Day";
char	slh_avrcode[]	=	"AVR";
char	slh_avrname[]	=	"Average";
char	slh_wavrcode[]	=	"WAVR";
char	slh_wavrname[]	=	"Weight-Average";
#else
char	slh_hdrday[]	=	"日";
char	slh_avrcode[]	=	"AVR";
char	slh_avrname[]	=	"平均值";
char	slh_wavrcode[]	=	"WAVR";
char	slh_wavrname[]	=	"加权平均";
#endif

CSPString	AfxGetSLHTitle( UINT nSLH )
{
	static	CSPString	slh_titles[SLH_MAX+1];
	SP_ASSERT( SLH_MAX >= 0 );

	if( slh_titles[0].GetLength() <= 0 )
	{
		// slh_titles
		slh_titles[SLH_NONE]				=	"SLHTitles";
		
#ifdef	CLKLAN_ENGLISH_US
		slh_titles[SLH_CODE]				=	"Code";
		slh_titles[SLH_NAME]				=	"Name";
		
		//	技术数据
		slh_titles[SLH_DATE]				=	"Trade Time";
		slh_titles[SLH_LASTCLOSE]			=	"Prev Close";
		slh_titles[SLH_OPEN]				=	"Open";
		slh_titles[SLH_CLOSE]				=	"Last Trade";
		slh_titles[SLH_HIGH]				=	"High";
		slh_titles[SLH_LOW]					=	"Low";
		slh_titles[SLH_AVERAGE]				=	"Average";
		slh_titles[SLH_DIFF]				=	"Change";
		slh_titles[SLH_DIFFPERCENT]			=	"Change(%)";
		slh_titles[SLH_SCOPE]				=	"Range(%)";
		slh_titles[SLH_VOLUME]				=	"Volume(100)";
		slh_titles[SLH_AMOUNT]				=	"Amount(1000)";
		slh_titles[SLH_VOLUP]				=	"Up Vol.";
		slh_titles[SLH_VOLDOWN]				=	"Down Vol.";
		slh_titles[SLH_DIFFPERCENT_MIN5]	=	"5-Minute Change(%)";
		slh_titles[SLH_SELLBUYRATIO]		=	"Consign Ratio(%)";
		slh_titles[SLH_SELLBUYDIFF]			=	"Consign Volume Difference";
		slh_titles[SLH_BUYPRICE3]			=	"Bid 3";
		slh_titles[SLH_BUYPRICE2]			=	"Bid 2";
		slh_titles[SLH_BUYPRICE1]			=	"Bid 1";
		slh_titles[SLH_SELLPRICE1]			=	"Ask 1";
		slh_titles[SLH_SELLPRICE2]			=	"Ask 2";
		slh_titles[SLH_SELLPRICE3]			=	"Ask 3";
		slh_titles[SLH_BUYVOLUME3]			=	"Bid Vol 3(100)";
		slh_titles[SLH_BUYVOLUME2]			=	"Bid Vol 2(100)";
		slh_titles[SLH_BUYVOLUME1]			=	"Bid Vol 1(100)";
		slh_titles[SLH_SELLVOLUME1]			=	"Ask Vol 1(100)";
		slh_titles[SLH_SELLVOLUME2]			=	"Ask Vol 2(100)";
		slh_titles[SLH_SELLVOLUME3]			=	"Ask Vol 3(100)";

		//	主要基本指标
		slh_titles[SLH_REPORTTYPE]			=	"Report Forms";
		slh_titles[SLH_PE]					=	"P/E";
		slh_titles[SLH_PNETASSET]			=	"P/B";
		slh_titles[SLH_PMAININCOME]			=	"P/S"; // "Market Capitalisation/Income";
		slh_titles[SLH_RATIO_PCASH]			=	"Price/Cash";
		slh_titles[SLH_RATIO_CURRENCY]		=	"Currency Percent(%)";
		slh_titles[SLH_RATIO_CHANGEHAND]	=	"Change hands Percent(%)";
		slh_titles[SLH_RATIO_VOLUME]		=	"Volume Ratio";
		slh_titles[SLH_RS]					=	"Relative Strength";
		slh_titles[SLH_MARKETVALUE]			=	"Market Capitalisation(10000)";
		slh_titles[SLH_MARKETVALUEA]		=	"A Market Capitalisation(10000)";
		slh_titles[SLH_MARKETVALUEB]		=	"B Market Capitalisation(10000)";
		
		// ★偿债能力
		slh_titles[SLH_RATIO_LIQUIDITY]		=	"Liquidity Ratio";
		slh_titles[SLH_RATIO_QUICK]			=	"Quickassets Ratio";
		slh_titles[SLH_VELOCITY_RECEIVABLES]=	"Receivables Velocity";
		
		// ★经营能力
		slh_titles[SLH_VELOCITY_MERCHANDISE]=	"Merchandise Velocity";
		slh_titles[SLH_MAIN_INCOME]			=	"Income(10000)";
		slh_titles[SLH_CASH_PS]				=	"Cash per share";
		
		// ★盈利能力
		slh_titles[SLH_PROFIT_MARGIN]		=	"Profit Margin(%)";
		slh_titles[SLH_NETASSET_YIELD]		=	"Return On Equity(%)";
		
		// ★资本结构
		slh_titles[SLH_DATE_BEGIN]			=	"IPO Date";
		slh_titles[SLH_SHARE_COUNT_TOTAL]	=	"Total Shares(10000)";
		slh_titles[SLH_SHARE_COUNT_A]		=	"Issued Shares A(10000)";
		slh_titles[SLH_SHARE_COUNT_B]		=	"Issued Shares B(10000)";
		slh_titles[SLH_SHARE_COUNT_H]		=	"Issued Shares H(10000)";
		slh_titles[SLH_SHARE_COUNT_NATIONAL]=	"National Shares(10000)";
		slh_titles[SLH_SHARE_COUNT_CORP]	=	"Corporation Shares(10000)";
		slh_titles[SLH_PROFIT_PSUD]			=	"Not Assigned Profit per Share";
		slh_titles[SLH_ASSET]				=	"Total Asset(10000)";
		slh_titles[SLH_RATIO_HOLDERRIGHT]	=	"Shareholders Rights Ratio(%)";
		slh_titles[SLH_RATIO_LONGDEBT]		=	"Long Debt Ratio(%)";
		slh_titles[SLH_RATIO_DEBT]			=	"Debt Ratio(%)";

		// ★投资收益能力
		slh_titles[SLH_NETASSET_PS]			=	"Net Asset per Share";
		slh_titles[SLH_NETASSET_PS_REGULATE]=	"Net Asset per Share Regulated";
		slh_titles[SLH_EPS]					=	"EPS"; // "Earnings per Share";
		slh_titles[SLH_EPS_DEDUCT]			=	"EPS Deducted"; // "Earnings per Share Deducted";
		slh_titles[SLH_NET_PROFIT]			=	"Net Profit/Loss(10000)";
		slh_titles[SLH_MAIN_PROFIT]			=	"Main Profit/Loss(10000)";
		slh_titles[SLH_TOTAL_PROFIT]		=	"Total Profit/Loss(10000)";
		slh_titles[SLH_PROFIT_INC]			=	"Profit Increase(%)";
		slh_titles[SLH_INCOME_INC]			=	"Income Increase(%)";
		slh_titles[SLH_ASSET_INC]			=	"Asset Increase(%)";
		slh_titles[SLH_ANNGAINS_AVERAGE]	=	"Average Ann-Gains(%)";
		slh_titles[SLH_ANNGAINS_STDDEV]		=	"Ann-Gains Standard Deviation(%)";
		slh_titles[SLH_BETA]				=	"Beta";
		slh_titles[SLH_SHARP]				=	"Sharp Venture";
		slh_titles[SLH_TRADE]				=	"Business Classification";
		slh_titles[SLH_PROVINCE]			=	"Province";
#else
		slh_titles[SLH_CODE]				=	"代码";
		slh_titles[SLH_NAME]				=	"名称";
		
		//	技术数据
		slh_titles[SLH_DATE]				=	"日期";
		slh_titles[SLH_LASTCLOSE]			=	"昨收";
		slh_titles[SLH_OPEN]				=	"开盘价";
		slh_titles[SLH_CLOSE]				=	"现价";
		slh_titles[SLH_HIGH]				=	"最高价";
		slh_titles[SLH_LOW]					=	"最低价";
		slh_titles[SLH_AVERAGE]				=	"均价";
		slh_titles[SLH_DIFF]				=	"涨跌";
		slh_titles[SLH_DIFFPERCENT]			=	"涨幅%";
		slh_titles[SLH_SCOPE]				=	"震幅%";
		slh_titles[SLH_VOLUME]				=	"成交量(手)";
		slh_titles[SLH_AMOUNT]				=	"成交额(千元)";
		slh_titles[SLH_VOLUP]				=	"外盘";
		slh_titles[SLH_VOLDOWN]				=	"内盘";
		slh_titles[SLH_DIFFPERCENT_MIN5]	=	"五分钟涨幅%";
		slh_titles[SLH_SELLBUYRATIO]		=	"委比%";
		slh_titles[SLH_SELLBUYDIFF]			=	"委量差";
		slh_titles[SLH_BUYPRICE3]			=	"买价三";
		slh_titles[SLH_BUYPRICE2]			=	"买价二";
		slh_titles[SLH_BUYPRICE1]			=	"买价一";
		slh_titles[SLH_SELLPRICE1]			=	"卖价一";
		slh_titles[SLH_SELLPRICE2]			=	"卖价二";
		slh_titles[SLH_SELLPRICE3]			=	"卖价三";
		slh_titles[SLH_BUYVOLUME3]			=	"买量三";
		slh_titles[SLH_BUYVOLUME2]			=	"买量二";
		slh_titles[SLH_BUYVOLUME1]			=	"买量一";
		slh_titles[SLH_SELLVOLUME1]			=	"卖量一";
		slh_titles[SLH_SELLVOLUME2]			=	"卖量二";
		slh_titles[SLH_SELLVOLUME3]			=	"卖量三";

		//	主要基本指标
		slh_titles[SLH_REPORTTYPE]			=	"报表类型";
		slh_titles[SLH_PE]					=	"市盈率";
		slh_titles[SLH_PNETASSET]			=	"市净率";
		slh_titles[SLH_PMAININCOME]			=	"市销率";
		slh_titles[SLH_RATIO_PCASH]			=	"价格净现金比";
		slh_titles[SLH_RATIO_CURRENCY]		=	"流通率%";
		slh_titles[SLH_RATIO_CHANGEHAND]	=	"换手率%";
		slh_titles[SLH_RATIO_VOLUME]		=	"量比";
		slh_titles[SLH_RS]					=	"相对强度";
		slh_titles[SLH_MARKETVALUE]			=	"总市值-万";
		slh_titles[SLH_MARKETVALUEA]		=	"A股市值-万";
		slh_titles[SLH_MARKETVALUEB]		=	"B股市值-万";
		
		// ★偿债能力
		slh_titles[SLH_RATIO_LIQUIDITY]		=	"流动比率";
		slh_titles[SLH_RATIO_QUICK]			=	"速动比率";
		slh_titles[SLH_VELOCITY_RECEIVABLES]=	"应收账款周率";
		
		// ★经营能力
		slh_titles[SLH_VELOCITY_MERCHANDISE]=	"存货周转率";
		slh_titles[SLH_MAIN_INCOME]			=	"主营收入-万";
		slh_titles[SLH_CASH_PS]				=	"每股净现金";
		
		// ★盈利能力
		slh_titles[SLH_PROFIT_MARGIN]		=	"主营利润率%";
		slh_titles[SLH_NETASSET_YIELD]		=	"净资产收益率%";
		
		// ★资本结构
		slh_titles[SLH_DATE_BEGIN]			=	"上市日期";
		slh_titles[SLH_SHARE_COUNT_TOTAL]	=	"总股本-万";
		slh_titles[SLH_SHARE_COUNT_A]		=	"A股-万";
		slh_titles[SLH_SHARE_COUNT_B]		=	"B股-万";
		slh_titles[SLH_SHARE_COUNT_H]		=	"H股-万";
		slh_titles[SLH_SHARE_COUNT_NATIONAL]=	"国有股-万";
		slh_titles[SLH_SHARE_COUNT_CORP]	=	"法人股-万";
		slh_titles[SLH_PROFIT_PSUD]			=	"每股未分利润";
		slh_titles[SLH_ASSET]				=	"总资产-万";
		slh_titles[SLH_RATIO_HOLDERRIGHT]	=	"股东权益比%";
		slh_titles[SLH_RATIO_LONGDEBT]		=	"长期负债率%";
		slh_titles[SLH_RATIO_DEBT]			=	"资产负债率%";

		// ★投资收益能力
		slh_titles[SLH_NETASSET_PS]			=	"每股净资产";
		slh_titles[SLH_NETASSET_PS_REGULATE]=	"调整每股净资产";
		slh_titles[SLH_EPS]					=	"每股收益";
		slh_titles[SLH_EPS_DEDUCT]			=	"扣除每股收益";
		slh_titles[SLH_NET_PROFIT]			=	"净利润-万";
		slh_titles[SLH_MAIN_PROFIT]			=	"主营业务利润-万";
		slh_titles[SLH_TOTAL_PROFIT]		=	"利润总额-万";
		slh_titles[SLH_PROFIT_INC]			=	"主营利润增长率%";
		slh_titles[SLH_INCOME_INC]			=	"收入增长率%";
		slh_titles[SLH_ASSET_INC]			=	"总资产增长率%";
		slh_titles[SLH_ANNGAINS_AVERAGE]	=	"年平均收益率%";
		slh_titles[SLH_ANNGAINS_STDDEV]		=	"收益标准差%";
		slh_titles[SLH_BETA]				=	"β值";
		slh_titles[SLH_SHARP]				=	"夏普风险指数";
		slh_titles[SLH_TRADE]				=	"行业";
		slh_titles[SLH_PROVINCE]			=	"省份";
#endif
	}

	if( nSLH >= SLH_MIN && nSLH <= SLH_MAX )
	{
		return	slh_titles[nSLH];
	}
	return "";
}

CSPString	AfxGetSLHDescript( UINT nSLH )
{
	static	CSPString	slh_descripts[SLH_MAX+1];
	SP_ASSERT( SLH_MAX >= 0 );

	if( slh_descripts[0].GetLength() <= 0 )
	{
		// slh_descripts
#ifdef	CLKLAN_ENGLISH_US
		slh_descripts[SLH_PE]			=	"Ratio of Price and Earnings per Share";

		slh_descripts[SLH_PNETASSET]	=	"Ratio of Price and Net Asset per Share";
		slh_descripts[SLH_PMAININCOME]	=	"Ratio of Market Capitalisation and Income";
		slh_descripts[SLH_RATIO_PCASH]	=	"Ratio of Price and Cash per Share";
		slh_descripts[SLH_RS]			=	"Ratio of Rising and Falling in n Days";
		slh_descripts[SLH_ANNGAINS_AVERAGE]="Average Ann-Gains";
		slh_descripts[SLH_ANNGAINS_STDDEV]=	"Ann-Gains Standard Deviation";
		slh_descripts[SLH_BETA]			=	"Correlation with Indicator";
		slh_descripts[SLH_SHARP]		=	"Ratio of Ann-Gains and Ann-Gains' Standard Deviation";
#else
		slh_descripts[SLH_PE]			=	"每股价格比每股收益";

		slh_descripts[SLH_PNETASSET]	=	"每股价格比每股净资产";
		slh_descripts[SLH_PMAININCOME]	=	"总市值比主营收入";
		slh_descripts[SLH_RATIO_PCASH]	=	"每股价格比每股净现金";
		slh_descripts[SLH_RS]			=	"n日内涨幅与跌幅之比";
		slh_descripts[SLH_ANNGAINS_AVERAGE]="投资该股票年收益率";
		slh_descripts[SLH_ANNGAINS_STDDEV]=	"年收益率的标准差";
		slh_descripts[SLH_BETA]			=	"表示与大盘相关性";
		slh_descripts[SLH_SHARP]		=	"年收益率比标准差";
#endif
	}
	
	if( nSLH >= SLH_MIN && nSLH <= SLH_MAX )
	{
		return	slh_descripts[nSLH];
	}
	return "";
}


//////////////////////////////////////////////////////////////////////////////////
// 表达式错误提示字符串
#ifdef	CLKLAN_ENGLISH_US
char	express_errparserstack[]	=	"Expression: Expression is too long and stack overflow.";
char	express_errbadrange[]		=	"Expression: Value is out of range.";
char	express_errexpression[]		=	"Expression: Strings error.";
char	express_erroperator[]		=	"Expression: Operator Sign error.";
char	express_erropenparen[]		=	"Expression: Open parenthesis error.";
char	express_errcloseparen[]		=	"Expression: Close parenthesis error.";
char	express_errinvalidnum[]		=	"Expression: Invalid number.";
char	express_errmath[]			=	"Expression: Mathematical error.";
char	express_errunknown[]		=	"Expression: Unknown.";
#else
char	express_errparserstack[]	=	"表达式：表达式太长，栈溢出。";
char	express_errbadrange[]		=	"表达式：数值超出范围。";
char	express_errexpression[]		=	"表达式：字符串出现错误。";
char	express_erroperator[]		=	"表达式：操作符出现错误。";
char	express_erropenparen[]		=	"表达式：左括号出现错误。";
char	express_errcloseparen[]		=	"表达式：右括号出现错误。";
char	express_errinvalidnum[]		=	"表达式：非法数字。";
char	express_errmath[]			=	"表达式：数学计算出现错误。";
char	express_errunknown[]		=	"表达式：未知错误。";
#endif


//////////////////////////////////////////////////////////////////////////////////
// 股票板块字符串
#ifdef	CLKLAN_ENGLISH_US
char	domain_self[]	=	"Favorites";
#else
char	domain_self[]	=	"自选股";
#endif


//////////////////////////////////////////////////////////////////////////////////
// 界面配色方案字符串
#ifdef	CLKLAN_ENGLISH_US
char	colorclass_standard[]		=	"Standard color schemes";
char	colorclass_byzantinesque[]	=	"Byzantinesque color schemes";
char	colorclass_gothic[]			=	"Gothic color schemes";
char	colorclass_baroque[]		=	"Barococo color schemes";
#else
char	colorclass_standard[]		=	"传统式配色方案";
char	colorclass_byzantinesque[]	=	"拜占庭式配色方案";
char	colorclass_gothic[]			=	"哥特式配色方案";
char	colorclass_baroque[]		=	"巴洛克式配色方案";
#endif


//////////////////////////////////////////////////////////////////////////////////
// 服务器说明、技术支持
#ifdef	CLKLAN_ENGLISH_US
char	sz_mainserverinfo[]			=	"Main Server";
char	sz_mailsubjectsupport[]		=	"Support";
#else
char	sz_mainserverinfo[]			=	"主服务器";
char	sz_mailsubjectsupport[]		=	"技术支持";
#endif


//////////////////////////////////////////////////////////////////////////////////
// 周
#ifdef	CLKLAN_ENGLISH_US
char	sz_sunday[]		=	"Sunday";
char	sz_monday[]		=	"Monday";
char	sz_tuesday[]	=	"Tuesday";
char	sz_wednesday[]	=	"Wednesday";
char	sz_thursday[]	=	"Thursday";
char	sz_friday[]		=	"Friday";
char	sz_saturday[]	=	"Saturday";
#else
char	sz_sunday[]		=	"星期日";
char	sz_monday[]		=	"星期一";
char	sz_tuesday[]	=	"星期二";
char	sz_wednesday[]	=	"星期三";
char	sz_thursday[]	=	"星期四";
char	sz_friday[]		=	"星期五";
char	sz_saturday[]	=	"星期六";
#endif


//////////////////////////////////////////////////////////////////////////////////
// 报表类型
#ifdef	CLKLAN_ENGLISH_US
char	sz_reportQuarter[]	=	"1st quarter";
char	sz_reportMid[]		=	"midyear";
char	sz_reportQuarter3[]	=	"3rd quarter";
char	sz_reportAnnals[]	=	"annals";
#else
char	sz_reportQuarter[]	=	"第一季度季报";
char	sz_reportMid[]		=	"中报";
char	sz_reportQuarter3[]	=	"第三季度季报";
char	sz_reportAnnals[]	=	"年报";
#endif

//////////////////////////////////////////////////////////////////////////////////
// 数据字符串
#ifdef	CLKLAN_ENGLISH_US
char	db_errcreateselfdb[]	=	"Fail to create file, reinstall this software, please.";
char	db_errrootpath[]		=	"Fail to initialize data, reinstall this software, please.";
char	db_errqianlongpath[]	=	"Fail to read referred QianLong data source.";
#else
char	db_errcreateselfdb[]	=	"创建数据文件失败，请重新安装本软件，或者与软件供应商联系";
char	db_errrootpath[]		=	"初始化数据失败，请重新安装本软件，或者与软件供应商联系";
char	db_errqianlongpath[]	=	"读取指定的钱龙数据源失败。";
#endif


//////////////////////////////////////////////////////////////////////////////////
// 策略字符串

#ifdef	CLKLAN_ENGLISH_US

char	strategy_logicand[]		=	"All";
char	strategy_logicor[]		=	"Any";

char	strategy_sdonce[]		=	"Full";
char	strategy_sdtwice[]		=	"Half";
char	strategy_sdthird[]		=	"One-third";
char	strategy_sdforth[]		=	"Quarter";
char	strategy_sdfifth[]		=	"One-fifth";
char	strategy_sdsixth[]		=	"One-sixth";
char	strategy_sdseventh[]	=	"One-seventh";
char	strategy_sdeighth[]		=	"One-eighth";
char	strategy_sdninth[]		=	"One-ninth";
char	strategy_sdtenth[]		=	"One-tenth";

char	strategy_noselectedstock[]	=	"there is no selected stock.";
char	strategy_noselectedtech[]	=	"there is no selected indicator.";

char	strategy_optype_buy[]		=	"Buy";
char	strategy_optype_sell[]		=	"Sell";
char	strategy_optype_addstock[]	=	"Add Stock";
char	strategy_optype_removestock[]	=	"Remove Stock";
char	strategy_optype_addcash[]		=	"Add Cash";
char	strategy_optype_removecash[]	=	"Reduce Cash";

char	strategy_errfile[]			=	"File type is not coincident, the file is not strategy file.";
char	strategy_errfilever[]		=	"File version is not supported, please upgrade this software.";

#else

char	strategy_logicand[]		=	"全部";
char	strategy_logicor[]		=	"任意一个";

char	strategy_sdonce[]		=	"全仓";
char	strategy_sdtwice[]		=	"半仓";
char	strategy_sdthird[]		=	"三分之一仓";
char	strategy_sdforth[]		=	"四分之一仓";
char	strategy_sdfifth[]		=	"五分之一仓";
char	strategy_sdsixth[]		=	"六分之一仓";
char	strategy_sdseventh[]	=	"七分之一仓";
char	strategy_sdeighth[]		=	"八分之一仓";
char	strategy_sdninth[]		=	"九分之一仓";
char	strategy_sdtenth[]		=	"十分之一仓";

char	strategy_noselectedstock[]	=	"没有选中股票";
char	strategy_noselectedtech[]	=	"没有选中指标";

char	strategy_optype_buy[]		=	"买入";
char	strategy_optype_sell[]		=	"卖出";
char	strategy_optype_addstock[]	=	"增加股票";
char	strategy_optype_removestock[]	=	"减少股票";
char	strategy_optype_addcash[]		=	"增加现金";
char	strategy_optype_removecash[]	=	"减少现金";

char	strategy_errfile[]			=	"文件类型不符，指定文件不是策略文件。";
char	strategy_errfilever[]		=	"错误的文件版本：请升级软件，以便可以读取高版本的策略文件。";

#endif


//////////////////////////////////////////////////////////////////////////////////
// 快捷键
// 给定快捷键唯一编号，返回快捷键及其中文名称
CSPString	AfxGetAccelerator( UINT nAcce, int nCharLeft )
{
	static	CSPString	acce_names[ACCE_MAX+1];
	SP_ASSERT( ACCE_MAX >= 0 );
	
	if( acce_names[0].GetLength() <= 0 )
	{
		// acce_names
		acce_names[0]			=	"ACCENames";

#ifdef	CLKLAN_ENGLISH_US
		// 快捷键
		acce_names[ACCE_01]		=	"01      Hot Key F1";
		acce_names[ACCE_02]		=	"02      Hot Key F2";
		acce_names[ACCE_03]		=	"03      Hot Key F3";
		acce_names[ACCE_04]		=	"04      Hot Key F4";
		acce_names[ACCE_05]		=	"05      Hot Key F5";
		acce_names[ACCE_06]		=	"06      Hot Key F6";
		acce_names[ACCE_07]		=	"07      Hot Key F7";
		acce_names[ACCE_08]		=	"08      Hot Key F8";
		acce_names[ACCE_09]		=	"09      Hot Key F9";
		acce_names[ACCE_10]		=	"10      Hot Key F10";
		acce_names[ACCE_1]		=	"1       Shanghai A Shares List";
		acce_names[ACCE_2]		=	"2       Shanghai B Shares List";
		acce_names[ACCE_3]		=	"3       Shenzhen A Shares List";
		acce_names[ACCE_4]		=	"4       Shenzhen B Shares List";
		acce_names[ACCE_5]		=	"5       Shanghai Bond List";
		acce_names[ACCE_6]		=	"6       Shenzhen Bond List";
		acce_names[ACCE_61]		=	"61      Shanghai A Shares Change";
		acce_names[ACCE_62]		=	"62      Shanghai B Shares Change";
		acce_names[ACCE_63]		=	"63      Shenzhen A Shares Change";
		acce_names[ACCE_64]		=	"64      Shenzhen B Shares Change";
		acce_names[ACCE_65]		=	"65      Shanghai Bond Change";
		acce_names[ACCE_66]		=	"66      Shenzhen Bond Change";
		acce_names[ACCE_71]		=	"71      Shanghai Info.";
		acce_names[ACCE_72]		=	"72      Shenzhen Info.";
		acce_names[ACCE_73]		=	"73      Stockjobber Info.";
		acce_names[ACCE_81]		=	"81      Shanghai A Shares Sort";
		acce_names[ACCE_82]		=	"82      Shanghai B Shares Sort";
		acce_names[ACCE_83]		=	"83      Shenzhen A Shares Sort";
		acce_names[ACCE_84]		=	"84      Shenzhen B Shares Sort";
		acce_names[ACCE_85]		=	"85      Shanghai Bond Sort";
		acce_names[ACCE_86]		=	"86      Shenzhen Bond Sort";
		acce_names[ACCE_LBDK]	=	"LBDK    Volume Ratio";
		acce_names[ACCE_MMLD]	=	"MMLD    Consign Ratio";
#else
		// 快捷键
		acce_names[ACCE_01]		=	"01      热键F1";
		acce_names[ACCE_02]		=	"02      热键F2";
		acce_names[ACCE_03]		=	"03      热键F3";
		acce_names[ACCE_04]		=	"04      热键F4";
		acce_names[ACCE_05]		=	"05      热键F5";
		acce_names[ACCE_06]		=	"06      热键F6";
		acce_names[ACCE_07]		=	"07      热键F7";
		acce_names[ACCE_08]		=	"08      热键F8";
		acce_names[ACCE_09]		=	"09      热键F9";
		acce_names[ACCE_10]		=	"10      热键F10";
		acce_names[ACCE_1]		=	"1       上A报价";
		acce_names[ACCE_2]		=	"2       上B报价";
		acce_names[ACCE_3]		=	"3       深A报价";
		acce_names[ACCE_4]		=	"4       深B报价";
		acce_names[ACCE_5]		=	"5       上债报价";
		acce_names[ACCE_6]		=	"6       深债报价";
		acce_names[ACCE_61]		=	"61      上A涨跌";
		acce_names[ACCE_62]		=	"62      上B涨跌";
		acce_names[ACCE_63]		=	"63      深A涨跌";
		acce_names[ACCE_64]		=	"64      深B涨跌";
		acce_names[ACCE_65]		=	"65      上债涨跌";
		acce_names[ACCE_66]		=	"66      深债涨跌";
		acce_names[ACCE_71]		=	"71      上证信息";
		acce_names[ACCE_72]		=	"72      深证信息";
		acce_names[ACCE_73]		=	"73      券商信息";
		acce_names[ACCE_81]		=	"81      上A综合";
		acce_names[ACCE_82]		=	"82      上B综合";
		acce_names[ACCE_83]		=	"83      深A综合";
		acce_names[ACCE_84]		=	"84      深B综合";
		acce_names[ACCE_85]		=	"85      上债综合";
		acce_names[ACCE_86]		=	"86      深债综合";
		acce_names[ACCE_LBDK]	=	"LBDK    量比（多空）";
		acce_names[ACCE_MMLD]	=	"MMLD    买卖力道（涨跌率）";
#endif
	}
	
	if( nAcce >= ACCE_MIN && nAcce <= ACCE_MAX )
	{
		if( 8 == nCharLeft )
			return	acce_names[nAcce];
		
		CSPString	strResult	=	acce_names[nAcce];
		int nIndex = strResult.Find( ' ' );
		if( -1 != nIndex )
			strResult	=	strResult.Left(nIndex) + strResult.Mid(8);
		return strResult;
	}
	return "";
}

//////////////////////////////////////////////////////////////////////////////////
// 国家或地区

void AfxGetAllDistrict( CSPStringArray & astr )
{
#ifdef	CLKLAN_ENGLISH_US
	astr.SetSize( 0, 30 );
	astr.Add( "Australia & NZ" );
	astr.Add( "Argentina" );
	astr.Add( "Brazil" );
	astr.Add( "Canada" );
	astr.Add( "China" );
	astr.Add( "Denmark" );
	astr.Add( "France" );
	astr.Add( "Germany" );
	astr.Add( "Hong Kong" );
	astr.Add( "India" );
	astr.Add( "Italy" );
	astr.Add( "Japan" );
	astr.Add( "Korea" );
	astr.Add( "Mexico" );
	astr.Add( "Norway" );
	astr.Add( "Singapore" );
	astr.Add( "Spain" );
	astr.Add( "Sweden" );
	astr.Add( "Taiwan" );
	astr.Add( "UK & Ireland" );
	astr.Add( "U.S." );
	astr.Add( "Other" );
#else
	astr.SetSize( 0, 30 );
	astr.Add( "澳大利亚" );
	astr.Add( "阿根廷" );
	astr.Add( "巴西" );
	astr.Add( "加拿大" );
	astr.Add( "中国" );
	astr.Add( "丹麦" );
	astr.Add( "法国" );
	astr.Add( "德国" );
	astr.Add( "中国香港" );
	astr.Add( "印度" );
	astr.Add( "意大利" );
	astr.Add( "日本" );
	astr.Add( "韩国" );
	astr.Add( "墨西哥" );
	astr.Add( "挪威" );
	astr.Add( "新加坡" );
	astr.Add( "西班牙" );
	astr.Add( "瑞典" );
	astr.Add( "中国台湾" );
	astr.Add( "英国" );
	astr.Add( "美国" );
	astr.Add( "其他" );
#endif
}


//////////////////////////////////////////////////////////////////////////////////
// 简单翻译
#define	CHS_ENU_MAX_WORDS	32
typedef	struct	chs_enu_pair_t	{
	char	chs[CHS_ENU_MAX_WORDS];
	char	enu[CHS_ENU_MAX_WORDS];
}CHS_ENU_PAIR;

CSPString	AfxFaintlyChsToEnu( const char * szCh )
{
	static	CHS_ENU_PAIR	chs_enu_pairs[]	=	{
		{ "中国",	" China " },
		{ "指数",	" Index " },
		{ "上证",	" Shanghai " },
		{ "深证",	" Shenzhen " },
		{ "基金",	" Fund " },
		{ "国债",	" Bond " },
		{ "债券",	" Bond " },
		{ "转债",	" Convertible Bond " },
		{ "银行",	" Bank " },
		{ "其他",	" Other " },
		{ "银行业",	" Bank " },
		{ "",	"" },
	};
	
	CSPString	sResult = szCh;
	int i = 0;
	while( * chs_enu_pairs[i].chs )
	{
		sResult.Replace( chs_enu_pairs[i].chs, chs_enu_pairs[i].enu );
		i ++;
	}
	sResult.TrimLeft();
	sResult.TrimRight();

	return sResult;
}

