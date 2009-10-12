/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		some afx-global-functions
*/

#if	!defined( __STKLIB_AFXCORE_H__ )
#define	__STKLIB_AFXCORE_H__

////////////////////////////////////////////////////////////////////////////////////
// Chinese To English Translation
STKLIB_API	BOOL AfxIsEnglish( const char * sz, int maxlen, float ratio = 0.0 );
STKLIB_API	CSPString AfxFaintlyChsToEnu( const char * szCh );
STKLIB_API	CSPString AfxMakeSpellCode( CSPString & strSrc, int nMode = 0x0001 );

////////////////////////////////////////////////////////////////////////////////////
// global data
STKLIB_API	CStProfile &		AfxGetProfile( );
STKLIB_API	CStockContainer &	AfxGetStockContainer( );
STKLIB_API	CStockContainer &	AfxGetSListStockContainer( );
STKLIB_API	CDomainContainer &	AfxGetDomainContainer( );
STKLIB_API	CDomainContainer &	AfxGetGroupContainer( );
STKLIB_API	CStDatabase &		AfxGetDB();
STKLIB_API	void AfxSetDB(CStDatabase * pdb);
STKLIB_API	CStock &			AfxGetStockMain(DWORD dwMarket=CStock::marketSHSE);

////////////////////////////////////////////////////////////////////////////////////
// database functions
STKLIB_API	BOOL	AfxInitializeDB( LPSTR lpBuf, UINT nMaxSize );
STKLIB_API	BOOL	AfxReleaseDB( );
STKLIB_API	BOOL	AfxReloadStockMain( );
STKLIB_API	BOOL	AfxReloadStock( CStock & stock );
STKLIB_API	BOOL	AfxPrepareStockData( CStDatabase * pDatabase, CStock &stock, int nKType, int nKFormat, int nMaindataType, BOOL bFullFill, BOOL bReload );
STKLIB_API	int		AfxRefreshStockContainerMinute( CStockContainer & container, LONG lStockType, int nSecondsToRead = -1 );

////////////////////////////////////////////////////////////////////////////////////
// usful functions
STKLIB_API	CSPString	AfxGetTimeString( time_t time, LPCTSTR lpszTimeFmt, BOOL bAddWeekDay );
STKLIB_API	CSPString	AfxGetReportTypeString( int reporttype );
STKLIB_API	CSPString	AfxGetFileDirectory( CSPString strPathName, CSPString strDefault );
STKLIB_API	CSPString	AfxGetFilePath( CSPString strDir, CSPString strFileTitle );


////////////////////////////////////////////////////////////////////////////////////
// 以下函数实现体在 Strings.cpp

STKLIB_API	CSPString	AfxGetKTypeString( int ktype );
STKLIB_API	CSPString	AfxGetMaindataTypeString( int type );

STKLIB_API	CSPString	AfxGetSTTName( UINT nTech );		// 给定技术指标唯一编号，得到指标中文名称
STKLIB_API	CSPString	AfxGetSTTShortName( UINT nTech );	// 给定技术指标唯一编号，得到指标英文简称
STKLIB_API	CSPString	AfxGetSTTFullName( UINT nTech );	// 给定技术指标唯一编号，得到指标中文名称+英文简称
STKLIB_API	CSPString	AfxGetSTTClassName( UINT nTech );	// 给定技术指标唯一编号，得到指标所属分类，如：趋势类、能量类
STKLIB_API	CSPString	AfxGetIntensityString( int nIntensity );	// 给定买卖信号值，得到中文说明
STKLIB_API	CSPString	AfxGetIntensityCodeString( UINT nCode );	// 给定买卖信号理由说明值，得到中文说明

STKLIB_API	CSPString	AfxGetSLHTitle( UINT nSLH );		// 给定股票列表列号，返回列名称
STKLIB_API	CSPString	AfxGetSLHDescript( UINT nSLH );		// 给定股票列表列号，返回列说明

STKLIB_API	void	AfxGetAllDistrict( CSPStringArray & astr );	// 获得所有国家和地区名称列表

#define		ACCE_MIN	1
#define		ACCE_01		1	// 热键F1
#define		ACCE_02		2	// 热键F2
#define		ACCE_03		3	// 热键F3
#define		ACCE_04		4	// 热键F4
#define		ACCE_05		5	// 热键F5
#define		ACCE_06		6	// 热键F6
#define		ACCE_07		7	// 热键F7
#define		ACCE_08		8	// 热键F8
#define		ACCE_09		9	// 热键F9
#define		ACCE_10		10	// 热键F10
#define		ACCE_1		11	// 上A报价
#define		ACCE_2		12	// 上B报价
#define		ACCE_3		13	// 深A报价
#define		ACCE_4		14	// 深B报价
#define		ACCE_5		15	// 上债报价
#define		ACCE_6		16	// 深债报价
#define		ACCE_61		17	// 上A涨跌
#define		ACCE_62		18	// 上B涨跌
#define		ACCE_63		19	// 深A涨跌
#define		ACCE_64		20	// 深B涨跌
#define		ACCE_65		21	// 上债涨跌
#define		ACCE_66		22	// 深债涨跌
#define		ACCE_71		23	// 上证信息
#define		ACCE_72		24	// 深证信息
#define		ACCE_73		25	// 券商信息
#define		ACCE_81		26	// 上A综合
#define		ACCE_82		27	// 上B综合
#define		ACCE_83		28	// 深A综合
#define		ACCE_84		29	// 深B综合
#define		ACCE_85		30	// 上债综合
#define		ACCE_86		31	// 深债综合
#define		ACCE_LBDK	32	// 显示分时行情量比多空指标
#define		ACCE_MMLD	33	// 显示分时行情买卖力道指标
#define		ACCE_MAX	33
STKLIB_API	CSPString	AfxGetAccelerator( UINT nAcce, int nCharLeft = 8 );	// 给定快捷键编号，返回快捷键及其中文名称


// End
////////////////////////////////////////////////////////////////////////////////////

#endif // __STKLIB_AFXCORE_H__
