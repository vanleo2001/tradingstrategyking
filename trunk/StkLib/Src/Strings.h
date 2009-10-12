/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
*/
#ifndef	__STKLIB_STRINGS_H__
#define	__STKLIB_STRINGS_H__


#include	"StdAfx.h"


//////////////////////////////////////////////////////////////////////////////////
// 字符串
extern	char	sz_stockname_fund[];
extern	char	sz_stockname_bond[];
extern	char	sz_stockname_index[];
extern	char	sz_stockname_bondindex[];
extern	char	sz_stockname_bondindexcorp[];

//////////////////////////////////////////////////////////////////////////////////
// 指标买卖信号字符串
extern	char	its_sellintense[];
extern	char	its_sell[];
extern	char	its_sellfeeble[];
extern	char	its_nothing[];
extern	char	its_buyfeeble[];
extern	char	its_buy[];
extern	char	its_buyintense[];

extern	char	itsc_nothing[];
extern	char	itsc_deviateonbottom[];
extern	char	itsc_deviateontop[];
extern	char	itsc_goldenfork[];
extern	char	itsc_deadfork[];
extern	char	itsc_long[];
extern	char	itsc_short[];
extern	char	itsc_overbought[];
extern	char	itsc_oversold[];
extern	char	itsc_support[];
extern	char	itsc_resistance[];


//////////////////////////////////////////////////////////////////////////////////
// 指标字符串



//////////////////////////////////////////////////////////////////////////////////
// 股票列表列名称字符串
extern	char	slh_hdrday[];
extern	char	slh_avrcode[];
extern	char	slh_avrname[];
extern	char	slh_wavrcode[];
extern	char	slh_wavrname[];


//////////////////////////////////////////////////////////////////////////////////
// 表达式错误提示字符串
extern	char	express_errparserstack[];
extern	char	express_errbadrange[];
extern	char	express_errexpression[];
extern	char	express_erroperator[];
extern	char	express_erropenparen[];
extern	char	express_errcloseparen[];
extern	char	express_errinvalidnum[];
extern	char	express_errmath[];
extern	char	express_errunknown[];


//////////////////////////////////////////////////////////////////////////////////
// 股票板块字符串
extern	char	domain_self[];


//////////////////////////////////////////////////////////////////////////////////
// 界面配色方案字符串
extern	char	colorclass_standard[];
extern	char	colorclass_byzantinesque[];
extern	char	colorclass_gothic[];
extern	char	colorclass_baroque[];


//////////////////////////////////////////////////////////////////////////////////
// 服务器说明、技术支持
extern	char	sz_mainserverinfo[];
extern	char	sz_mailsubjectsupport[];


//////////////////////////////////////////////////////////////////////////////////
// 周
extern	char	sz_sunday[];
extern	char	sz_monday[];
extern	char	sz_tuesday[];
extern	char	sz_wednesday[];
extern	char	sz_thursday[];
extern	char	sz_friday[];
extern	char	sz_saturday[];

//////////////////////////////////////////////////////////////////////////////////
// 报表类型
extern	char	sz_reportQuarter[];
extern	char	sz_reportMid[];
extern	char	sz_reportQuarter3[];
extern	char	sz_reportAnnals[];

//////////////////////////////////////////////////////////////////////////////////
// 字数据符串
extern	char	db_errcreateselfdb[];
extern	char	db_errrootpath[];
extern	char	db_errqianlongpath[];


//////////////////////////////////////////////////////////////////////////////////
// 策略字符串
extern	char	strategy_logicand[];
extern	char	strategy_logicor[];

extern	char	strategy_sdonce[];
extern	char	strategy_sdtwice[];
extern	char	strategy_sdthird[];
extern	char	strategy_sdforth[];
extern	char	strategy_sdfifth[];
extern	char	strategy_sdsixth[];
extern	char	strategy_sdseventh[];
extern	char	strategy_sdeighth[];
extern	char	strategy_sdninth[];
extern	char	strategy_sdtenth[];

extern	char	strategy_noselectedstock[];
extern	char	strategy_noselectedtech[];

extern	char	strategy_optype_buy[];
extern	char	strategy_optype_sell[];
extern	char	strategy_optype_addstock[];
extern	char	strategy_optype_removestock[];
extern	char	strategy_optype_addcash[];
extern	char	strategy_optype_removecash[];

extern	char	strategy_errfile[];
extern	char	strategy_errfilever[];


#endif	//	__STKLIB_STRINGS_H__
