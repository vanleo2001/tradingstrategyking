#ifndef _STOCK_DRV_SC_H_
#define _STOCK_DRV_SC_H_

#include "Stockdrv.h"

// 更新日期：20060131
// 更新内容：
// 1. tagSCPankou里面的时间改为long类型。
//    SC_PankouReply里面的个数short m_nCount改为long m_lCount
// 2. SC_KLineAsk和SC_KLineReply有所调整
// 3. 扩充市场定义，三板、外汇、期货、台湾市场，使用SCMarket返回的m_wMarket值即可。
// 4. 大部分数据类型的的#define值有更改，重新编译即可
// 5. SC_Downdload中的m_nCount改为m_lCount，类型改为long. 
//    结构名称从'SC_Downdload'改为'SC_Download'
// 6. SH_MARKET_EX、SZ_MARKET_EX等的定义已删除，直接用SCMarket里面的m_wMarket即可。
// 7. 初始化函数名改为SCInit
// 8. 示范代码更加详细，请参考我们的例子，SCDemo

#define	STKLABEL_LEN				10		// 股号数据长度,国内市场股号编码兼容钱龙
#define	STKNAME_LEN					32		// 股名长度
#define	TITLE_LEN					64		// 信息地雷标题长度

#define SC_INIT						0x2001	// SCInit调用

#define SC_CLEAR_PREVREQ			0x2101	// 撤销前面的所有请求，建议在发请求（或组合请求）之前调用
#define SC_MARKET					0x2131	// 请求指定市场的信息，无需请求，有变化就会自动发送

#define SC_NOWDATA					0x2201

#define SC_PANKOU					0x2301	// 请求盘口数据，对应：long m_time，建议使用这个。

#define SC_IMTYPELIST				0x2401	// 请求信息地雷信息分类表
#define SC_IMTITLE					0x2402	// 请求信息地雷标题列表
#define SC_IMCONTENT				0x2403	// 请求信息地雷数据(f10, 交易所公告、实时和历史的个股信息等)

#define SC_KLINE					0x2501	// 请求K线数据（日线）

#define SC_CQ						0x2601	// 请求除权数据

#define SC_DL_KLINE					0x2701	// K线数据下载
#define SC_DL_CQ					0x2702	// 除权数据下载
#define SC_DL_XX					0x2703	// 信息下载，如F10、交易所公告、新闻等
#define SC_DL_TRACE					0x2704	// 明细数据，旧的，新版已不用
#define SC_DL_5MIN					0x2705	// 5分钟K线下载


#pragma pack(1)

struct SCHead {								// 请求包头
	WORD		m_nType;					// 请求/应答类型，SC_MARKET、SC_IMCONTENT等
	char		m_cStatus;					// 当为应答包时，m_cStatus=1成功，m_cStatus=0失败，m_cStatus=2离线
	long		m_lIndex;					// 包的唯一索引。不要设为0，请求的时候赋值，并在应答包中
											// 判断是否与请求包中的相同，如果不同，则表示不是这个请求包对应的应答包
											// 如果这个值为0，表示不是请求引起的数据变化
};

typedef struct tagStock {					// 证券
	char	m_szLabel[STKLABEL_LEN];		// 股票代码,以'\0'结尾
	char	m_szName[STKNAME_LEN];			// 股票名称,以'\0'结尾
	short	m_nHand;						// 每手股数
} StockInfo;

typedef struct tagMarket {					//市场内容
	union {
		WORD			m_wMarket;			//市场代码, 'HS', 'SZ', 'BS', 'HW', 'KH', 'ZH', 'QS', 'QD', 'QZ', 'WT',
		char			m_EShortName[3];	//英文简写, "SH", "SZ", "SB", "WH", "HK", "HZ", .....
	};
	char			m_Name[16];				//市场名称
	char			m_CShortName[5];		//中文简称
	unsigned long	m_lProperty;			//市场属性（未定义）
	unsigned long	m_lDate;				//数据日期（20030114）
	short			m_PeriodCount;			//交易时段个数
	short			m_OpenTime[5];			//开市时间 1,2,3,4,5
	short			m_CloseTime[5];			//收市时间 1,2,3,4,5
	short			m_nCount;				//该市场的证券个数
	StockInfo       m_Siif[1];   
} SCMarket;

typedef struct tagStockID {					//证券标识
	WORD			m_wMarket;				//市场代码
	short			m_nIndex;				//在该市场SCMarket::m_Siff中的偏移
} StockID;


typedef struct tagSCNow
{
	StockID m_sID;
	long	m_time;							// 成交时间

	float	m_fLastClose;					// 昨收
	float	m_fOpen;						// 今开
	float	m_fHigh;						// 最高
	float	m_fLow;							// 最低
	float	m_fNewPrice;					// 最新
	float	m_fVolume;						// 成交量
	float	m_fAmount;						// 成交额

	long	m_lStroke;						// 本次实际成交笔数
	float	m_fBuyPrice[5];					// 申买价1,2,3,4,5
	float	m_fBuyVolume[5];				// 申买量1,2,3,4,5
	float	m_fSellPrice[5];				// 申卖价1,2,3,4,5
	float	m_fSellVolume[5];				// 申卖量1,2,3,4,5
} RCV_NOW_STRUCTEx;

typedef struct tagSCPankou {				
	long	m_time;							// UCT

	float	m_fHigh;						// 最高
	float	m_fLow;							// 最低
	float	m_fNewPrice;					// 最新
	float	m_fVolume;						// 成交量
	float	m_fAmount;						// 成交额

	long	m_lStroke;						// 本次实际成交笔数
	float	m_fBuyPrice[5];					// 申买价1,2,3
	float	m_fBuyVolume[5];				// 申买量1,2,3，
	float	m_fSellPrice[5];				// 申卖价1,2,3
	float	m_fSellVolume[5];				// 申卖量1,2,3
} RCV_PANKOU_STRUCTEx;

/*
typedef union tagRCV_HISTORY_STRUCTEx
{
	struct
	{
		long	m_time;				//UCT
		float	m_fOpen;			//开盘
		float	m_fHigh;			//最高
		float	m_fLow;				//最低
		float	m_fClose;			//收盘
		float	m_fVolume;			//量
		float	m_fAmount;			//额
		WORD	m_wAdvance;			//涨数,仅大盘有效
		WORD	m_wDecline;			//跌数,仅大盘有效
	};
} RCV_HISTORY_STRUCTEx;

typedef union tagRCV_POWER_STRUCTEx
{
	struct
	{
		long	m_time;				// UCT
		float	m_fGive;			// 每股送
		float	m_fPei;				// 每股配
		float	m_fPeiPrice;		// 配股价,仅当 m_fPei!=0.0f 时有效
		float	m_fProfit;			// 每股红利
	};
}RCV_POWER_STRUCTEx,*PRCV_POWER_STRUCTEx;
*/

///////////////////////////////////////////////////////////////////////////////////
// 引擎初始化包
struct SC_InitAsk {							// 发出初始化请求，调用SCInit时使用，无对应返回包
	SCHead				m_Head;
	DWORD				m_dwSoftware;		// 分析软件标识，由数畅分配
	HWND				m_hWnd;				// 用来接收数据的窗口句柄
	DWORD				m_nMsg;				// 用来接收数据的消息
};


///////////////////////////////////////////////////////////////////////////////////
//请求包和应答包
struct SC_MarketReply {		// 这个包无需请求即可收到，在市场信息变化的时候就可收到
	SCHead				m_Head;
	SCMarket			m_Market;
};

struct SC_NowDataReply {	// 这个包无需请求即可收到。数据有更新，会自动发送
	SCHead				m_Head;
	short				m_nCount;
	RCV_NOW_STRUCTEx	m_Now[1];
};

struct SC_PankouAsk {				// 请求一天盘口数据，现支持当天，历史盘口将通过下载获得
	SCHead				m_Head;
	StockID				m_sID;
	long				m_lDate;	// FORMAT: 20010305
};

struct SC_PankouReply {
	SCHead				m_Head;
	StockID				m_sID;
	long				m_lDate;			// FORMAT: 20010305
	float				m_fLastClose;		// 昨收
	float				m_fOpen;			// 今开
	long				m_lCount;
	RCV_PANKOU_STRUCTEx	m_Data[1];		
};

//define for SC_KLineAsk::m_wDataType
#define KLINE_DAYDATA			0		//日线
#define KLINE_MIN5DATA			1		//5分钟线

// 注意：
// 1、由于服务器的设置，可能只能收到最近某一时段的数据，更多的数据，可以通过下载获得。
// 2、对于K线请求包，当表示日线时，long的格式为yyyymmdd，
// 3、对于K线请求包，当为五分钟线时，long的格式如下面这个TDate结构所示
//	struct TDate {		// 五分钟时间格式
//		unsigned long m_Minute : 6;
//		unsigned long m_Hour : 5;
//		unsigned long m_Day  : 5;
//		unsigned long m_Month : 4; 
//		unsigned long m_Year : 12;
//	};
// 4、关于时间的转行，我们提供例子中有示范代码，可参考。
// 5、对K线应答包，日线和无分钟的时间统一为long格式，已经包含日期和分钟信息。

struct SC_KLineAsk {
	SCHead				m_Head;
	StockID				m_sID;
	WORD				m_wDataType;		// KLINE_DAYDATA or KLINE_MIN5DATA
	long				m_tBegin;			// 开始时间或位置，0表示从第一个数据开始
	long				m_tEnd;				// 结束时间或请求数量，0表示直到结束
};

struct SC_KLineReply {
	SCHead				m_Head;
	StockID				m_sID;
	WORD				m_wDataType;		// KLINE_DAYDATA or KLINE_MIN5DATA
	long				m_lDataCount;		// m_pData数量
	RCV_HISTORY_STRUCTEx m_pData[1];		
};

// 除权数据不校验，总是以服务器的为准
struct SC_CQAsk {
	SCHead				m_Head;
	StockID				m_sID;
};

struct SC_CQReply {
	SCHead				m_Head;
	StockID				m_sID;
	short				m_nCount;
	RCV_POWER_STRUCTEx	m_pData[1];			// m_head不使用
};

//////////////////////////////////////////////////////////////////////////////////
//////////////  信息地雷接口
//信息地雷类型属性的定义
#define CLASS_JYSXX			0		// 交易所公告
#define CLASS_STKINFO       1		// 历史信息地雷
#define CLASS_NEWS          2		// 新闻
#define CLASS_STOCKBASE     3		// F10
#define CLASS_MKTINFO       4		// 综合资讯

struct SCIMType {					// 信息分类，如重大事项、公司公告、个股点评等
	char    m_cFlag[8];
	char    m_cTitle[24];
};

struct SC_IMTypeReply {				// 无需请求，自动返回
	SCHead			m_Head;
	char			m_cVendor;
	char            m_cClass;
	short			m_nSize;
	SCIMType		m_Type[1];
};

struct SCIMTitle {
	unsigned long  m_lID;			// 标题ID
	long		   m_lChkSum;		// 新增加，所对应的内容的Checksum
	unsigned long  m_lDate;			// yyyymmdd
	unsigned long  m_lTime;			// hhmmss
	char           m_cFlag[8];
	char		   m_cMainType[9];
	char           m_cSubtype[17];
	char		   m_cTitle[64];
	char		   m_szLabel[9];	// 股票代码,以'\0'结尾
};

struct SCIMTitleID {
	unsigned long  m_lID;			// 标题ID
	long		   m_lChkSum;		// 新增加，所对应的内容的Checksum
};

struct SC_IMTitleAsk {				// 请求某个时间段内信息地雷标题
	SCHead			m_Head;
	char			m_cClass;
	StockID			m_sID;			//对于个股资讯、基本资料F10使用m_sID，交易所公告只使用
	                                //其中的市场，新闻、综合资讯不使用这个字段
	long			m_lStartDate;	//新闻、综合资讯、交易所公告只使用m_lStartDate，m_lEndDate不使用
	long			m_lEndDate;		//个股资讯使用m_lStartDate，m_lEndDate；
};

struct SC_IMTitleReply { 
	SCHead			m_Head;
	char			m_cVendor;		// 应答包有厂商
	char			m_cClass;
	StockID			m_sID;				//对于个股资讯、基本资料F10、使用m_sID,
	short           m_nSize;			// 标题个数
	SCIMTitle		m_IMTitle[1];
};

struct SC_IMDataAsk {					// 信息地理内容请求，一次可请求多个标题的内容
	SCHead			m_Head;
//	char			m_cVendor;			// 请求包没有厂商代码
	char			m_cClass;
	StockID			m_sID;				//对于个股资讯、基本资料、个股信息地雷使用m_sID,
	short			m_nSize;			// 标题ID个数
	long			m_lDate;			//日期，新闻、交易所公告、综合资讯需要，一次只能设一个日期
	SCIMTitleID		m_ID[1];
};

struct SCIMData {						// 信息地理内容
	SCIMTitle		m_IMTitle;
	unsigned long   m_lDataLength;
	char		    m_cData[1];
};

struct SC_IMDataReply {					// 信息地雷内容应答，一次可返回多个标题和内容
	SCHead			m_Head;
	char			m_cVendor;
	char			m_cClass;
	StockID			m_sID;				//对于个股资讯、基本资料F10使用m_sID,
	long			m_lDate;			//对于新闻、综合资讯、财经新闻、券商信息使用m_lDate
	short			m_nSize;
	SCIMData		m_Data[1];
};

typedef struct tagIMTitle {
	unsigned long	m_lOffset;
	unsigned long	m_lDataLength;
	long			m_time;
	char			m_cCaption[TITLE_LEN];
} IMTitleEx;

// 在引擎里面进行下载操作之后，会自动收到这个包，里面包含了下载下来的指定数据。
struct SC_Download {						// 数据下载接口
	SCHead					m_Head;			// m_head中仅m_nType有效
	StockID					m_sID;
	long					m_lCount;		// // m_KLine,m_power,m_Trace,m_Title的个数
	union {
		RCV_HISTORY_STRUCTEx	m_KLine[1]; // k线、五分钟数据下载
		RCV_POWER_STRUCTEx		m_power[1]; // 除权数据下载
		RCV_PANKOU_STRUCTEx		m_Trace[1];	// 明细数据下载
		struct {							// F10、交易所公告、新闻、综合资讯下载
			char			m_cClass;		// 类别
			IMTitleEx		m_Title[1];
			char			m_cData[1];
		};
	};
};

#pragma pack()

//////////////////////////////////////////////////////////////////////////////////
//新增接口
#ifdef __cplusplus
extern "C"{
#endif

//新接口的DLL请使用HKEY_LOCAL_MACHINE/SOFTWARE/StockDrv下的SCDriver项

// 接口函数1
//****************************************************************************************\\
// 设置处理消息的窗口句柄和自定义的消息
// SCAskData请求的数据在这个消息中返回结果
// 但是，并不是一定要调用SCAskData才有数据返回，在没有调用SCAskData的情况下，有下面三种情况会返回数据
// 1、市场变化（比如初始化，新股上市等），返回SC_MarketReply数据
// 2、开市时段内，实时数据不断变化，返回SC_NowReply数据
// 3、在引擎里面进行了下载操作，返回下载下来的指定数据SC_Downdload
// 其它情况，通过调用SCAskData，返回指定类型的数据。
BOOL WINAPI SCInit(void *pInit, int nSize); // SC_InitAsk, sizeof(SC_InitAsk)
//****************************************************************************************//

// 接口函数2
//****************************************************************************************\\
// 退出的时候调用这个函数，以释放调用SCInit时分配的资源。
void WINAPI SCQuit();
//****************************************************************************************//


//  接口函数3
//****************************************************************************************\\
// 软件向信息平台请求数据，
// nSize为请求包的大小
// pAsk分别为 SC_IMVendorAsk *, SC_TraceAsk *, SC_NowAsk *, SC_IMAsk * 等
// 数据在函数SCInit所设的消息中返回，可参考CSCDemoDlg::OnReply(...)
// 消息中的数据可能在SCAskData之后返回，也可能在SCAskData尚未结束之前返回
// 用户通过调用AskData，信息平台可能从本地返回数据，也可能把用户的请求转交到网上的服务器
// 再返回数据，对用户来说，效果和用法都是一样的，可参考CSCDemoDlg::OnReply(...)。
void WINAPI SCAskData(void *pAsk, int nSize);
//****************************************************************************************//


//  接口函数4
//****************************************************************************************\\
// 取系统信息
enum TSCInfo { 
	INFO_VERSION = 1,
	INFO_USERNAME = 2,
};
UINT WINAPI SCGetInfo(TSCInfo Info, void *pData, int *nSize);
// Info=INFO_VERSION时，pData可设为NULL，函数返回值为引擎版本号
// Info=INFO_USERNAME时，pData指向一个字符串，
//                       返回值为0表示未登录，pData指向的内容未定义
//                       返回值为1表示已成功登录，pData指向的内容为用户名
//****************************************************************************************\\



#ifdef __cplusplus
}
#endif


#endif // _STOCK_DRV_SC_H_