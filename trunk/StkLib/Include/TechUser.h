/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CTechUser
*/

#if	!defined( __STKLIB_TECHUSER_H__ )
#define	__STKLIB_TECHUSER_H__

#ifndef TECHUSER_API
#define	TECHUSER_API	__declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#pragma pack(1)

typedef struct techuser_info_t {
	int		m_nStructSize;		// 以字节为单位表示的本结构大小
	char	m_szName[64];		// 指标名称，以'\0'为结尾，例如：随机指标
	char	m_szShortName[16];	// 指标简称，以'\0'为结尾，例如：KDJ
	
	int		m_nLineCount;		// 指标线个数，调用者根据该变量确定调用返回的计算所得值是否有效
								// 例如：m_nLineCount = 3，则 m_dValue1, m_dValue2, m_dValue3有效
								// m_nLineCount 应介于1至5之间
	char	m_aszLineName[5][8];// 每条线的名称，前m_nLineCount个有效，注意每个名称应以'\0'为结尾，
								// 即：名称不能超过7个字节
	
	int		m_bAutoMA;			// 当m_nLineCount = 1 时，该值有效，m_bAutoMA = true 时，自动加入
								// 平均线
	int		m_nMADays;			// 自动计算平均线时的均线日期

	int		m_itsGoldenFork;	// 如果指标 m_nLineCount = 2 或者　m_nLineCount = 1并自动计算平均
								// 线时，低位金叉发出的买卖信号
	int		m_itsDeadFork;		// 条件同上，高位死叉时的买卖信号
	
	/* 备注：其中 m_szName, m_szShortName, m_nLineCount必须赋值 */
} TECHUSER_INFO, *PTECHUSER_INFO;

typedef struct calculate_info_t {
	int		m_nStructSize;		// 以字节为单位表示的本结构大小
	CStock *	m_pStock;		// CStock指针，包含全部股票数据
	CStockInfo *m_pStockInfo;	// CStockInfo指针，包含股票基本信息数据，该指针实际指向
								// m_pStock->m_stockinfo
	CKData *	m_pKData;		// CKData指针，包含要计算的K 线数据数组，该指针实际指向m_pStock的
								// 相应K 线周期成员变量，K 线周期以及是否复权等已经处理完毕，直接
								// 使用即可
	int			m_nIndex;		// 指向当前要计算指标的K 线位置的序号，即在pKData中的序号
	int		m_bUseLast;			// 当前值是否为上次计算所得值，如果m_bUseLast=TRUE，则m_dValue1,
								// m_dValue2,...为上次调用该指标的计算所得值

	// 以下为计算返回值
	double	m_dValue1;			// 计算所得指标线1值
	double	m_dValue2;			// 计算所得指标线2值
	double	m_dValue3;			// 计算所得指标线3值
	double	m_dValue4;			// 计算所得指标线4值
	double	m_dValue5;			// 计算所得指标线5值
	int		m_nSignal;			// 买卖信号，介于ITS_MIN与ITS_MAX之间
} CALCULATE_INFO, *PCALCULATE_INFO;

#pragma pack()

#ifndef	ITS_MIN
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
#endif

TECHUSER_API UINT WINAPI GetTechUserCount();
TECHUSER_API BOOL WINAPI GetTechUserInfo( UINT nID, PTECHUSER_INFO pInfo );
TECHUSER_API BOOL WINAPI Calculate( UINT nID, PCALCULATE_INFO pInfo );
TECHUSER_API BOOL WINAPI GetSignal( UINT nID, PCALCULATE_INFO pInfo );

#ifdef __cplusplus
}
#endif //__cplusplus

#endif	//__STKLIB_TECHUSER_H__
