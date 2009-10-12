
#include "QianLong.h"

#pragma	pack(1)

/* 2.0版财务信息数据结构 dat\basetbl.dat

typedef struct clk20_basedata_t {
	LONG	type;								//	stock type
	char	code[8];							// 代码
	char	domain[16+1];						// 板块
	char	province[8+1];						// 省份

	DWORD	date;				 	// 日期	Format is YYYYMMDD for base data
	LONG	reporttype;				// 报告类型：年报、中报、季报
	LONG	erate_dollar;			// 当期美元汇率 0.001
	LONG	erate_hkdollar;			// 当期港币汇率 0.001

	// ★偿债能力
	LONG	ratio_liquidity;		// 流动比率 0.001
	LONG	ratio_quick;			// 速动比率 0.001
	LONG	velocity_receivables;	// 应收帐款周率 0.001
	// ★经营能力
	LONG	velocity_merchandise;	// 存货周转率 0.001
	LONG	main_income;			// 主营业务收入 10000
	LONG	cash_ps;				// 每股净现金流量 0.001
	// ★盈利能力
	LONG	profit_margin;			// 主营业务利润率 % 0.001
	LONG	netasset_yield;			// 净资产收益率 % 0.001
	// ★资本结构
	LONG	date_begin;				// 上市日期 Format is YYYYMMDD
	LONG	share_count_total;		// 总股本 10000
	LONG	share_count_a;			// 流通A股 10000
	LONG	share_count_b;			// 流通B股 10000
	LONG	share_count_h;			// 流通H股 10000
	LONG	share_count_national;	// 国有股 10000
	LONG	share_count_corp;		// 法人股 10000
	LONG	profit_psud;			// 每股未分配利润 0.001
	LONG	asset;					// 总资产 10000
	LONG	ratio_holderright;		// 股东权益比率 0.001
	LONG	ratio_longdebt;			// 长期负债率 0.001
	LONG	ratio_debt;				// 资产负债率 0.001
	// ★投资收益能力
	LONG	netasset_ps;			// 每股净资产 0.001
	LONG	netasset_ps_regulate;	// 调整每股净资产 0.001
	LONG	eps;					// 每股收益 0.001
	LONG	eps_deduct;				// 扣除后每股收益 0.001
	LONG	net_profit;				// 净利润 10000
	LONG	main_profit;			// 主营业务利润 10000
	LONG	total_profit;			// 利润总额 10000
	// ★增长率
	LONG	profit_inc;				// 主营利润增长率% 0.001
	LONG	income_inc;				// 主营收入增长率% 0.001
	LONG	asset_inc;				// 总资产增长率% 0.001
	// ★技术数据统计值
	LONG	yield_average;			// 平均收益率% 0.001
	LONG	yield_stddev;			// 收益标准差% 0.001
	LONG	beite;					// β值 0.001

	DWORD	reserved[4];
}CLK20_BASEDATA, *PCLK20_BASEDATA;
*/

// 除权数据
typedef struct clk_drdata_t {
	DWORD	date;
	DWORD	bonus;			//	10派X元		/1000
	DWORD	bonus_share;	//	10送X股		/100
	DWORD	ration_share;	//	10配X股		/100
	DWORD	ration_value;	//	配股价		/1000
	DWORD	reserved;
}CLK_DRDATA, * PCLK_DRDATA;

// 分析家格式除权数据
typedef union fxj_drdata_t {
	struct
	{
		DWORD	m_dwMagic;		// = -1
		WORD	m_wMarket;		// SH 上海 SZ 深圳
		char	m_szCode[6];
		char	m_szReserved[8];
	};
	struct
	{
		time_t	m_time;
		float	m_fGive;			// 每股送
		float	m_fPei;				// 每股配
		float	m_fPeiPrice;		// 配股价,仅当 m_fPei!=0.0f 时有效
		float	m_fProfit;			// 每股红利
	};
}FXJ_DRDATA, * PFXJ_DRDATA;

// 通达信格式财务数据
#define	TDX_FHEADER_SIZE	0x521
typedef struct tdx_basedata_t {
	char	m_szMagic;
	char	m_szMarket;						// 市场 '0' 深圳 '1' 上海
	char	m_szCode[6];
	char	m_szDate_modified[8];			// 更新日期
	char	m_szShare_count_total[14];		// 总股本 10000
	char	m_szShare_count_national[14];	// 国家股 10000
	char	m_szShare_count_orig[14];		// 发起股 10000
	char	m_szShare_count_corp[14];		// 法人股 10000
	char	m_szShare_count_b[14];			// B股 10000
	char	m_szShare_count_h[14];			// H股 10000
	char	m_szShare_count_currency[14];	// 流通股 10000
	char	m_szShare_count_employ[14];		// 职工股 10000
	char	m_szUnkown1[14];				// 
	char	m_szAsset[14];					// 总资产 1000
	char	m_szAsset_currency[14];			// 流动资产 1000
	char	m_szAsset_capital[14];			// 固定资产 1000
	char	m_szAsset_imma[14];				// 无形资产 1000
	char	m_szInvest_long[14];			// 长期投资 1000
	char	m_szDebt_currency[14];			// 流动负债 1000
	char	m_szDebt_long[14];				// 长期负债 1000
	char	m_szAcc_fund[14];				// 公积金 1000
	char	m_szNet_asset[14];				// 净资产 1000
	char	m_szMain_income[14];			// 主营收入 1000
	char	m_szMain_profit[14];			// 主营利益 1000
	char	m_szOther_profit[14];			// 其他利益 1000
	char	m_szBusiness_profit[14];		// 营业利益 1000
	char	m_szInvest_profit[14];			// 投资收益 1000
	char	m_szAllow_profit[14];			// 补贴收入 1000
	char	m_szOut_profit[14];				// 业外收支 1000
	char	m_szProfit_regulate[14];		// 损益调整 1000
	char	m_szTotal_profit[14];			// 利润总额 1000
	char	m_szProfit_aftertax[14];		// 税后利益 1000
	char	m_szNet_profit[14];				// 净利益 1000
	char	m_szProfit_ud[14];				// 未分利润 1000
	char	m_szNet_asset_ps_regulate[14];	// 每股净资2
	char	m_szUnknown2[3];				// 
	char	m_szUnknown3[4];				// 
	char	m_szUnknown4[2];				//
	char	m_szDate_begin[8];				// 上市日期
	char	m_szUnknown5[3];				// 
	char	m_szUnknown6[11];				// 
}TDX_BASEDATA, *PTDX_BASEDATA;

// 分析家格式财务数据
#define	FXJ_FHEADER_SIZE	0x8
typedef struct fxj_basedata_header_t {
	DWORD	m_dwMagic1;	//	0x223FD90C;
	DWORD	m_dwMagic2;	//	0x000000A6;
}FXJ_BASEDATA_HEADER, *PFXJ_BASEDATA_HEADER;
typedef struct fxj_basedata_t {
	WORD	m_wMarket;						// 市场 'ZS'" 深圳 'HS' 上海
	WORD	m_wUnknown;
	char	m_szCode[6];
	float	m_fData[39];
/*
	m_fData[0]
	m_fData[1]
	m_fData[2]*10000	// 总股本
	m_fData[3]*10000	// 国有股
	m_fData[4]*10000	// 发起人法人股
	m_fData[5]*10000	// 法人股
	m_fData[6]*10000	// 流通B股
	m_fData[7]*10000	// 流通H股
	m_fData[8]*10000	// 流通A股
	m_fData[9]
	m_fData[10]
	m_fData[11]*1000	// 总资产
	m_fData[12]*1000	// 流通资产
	m_fData[13]*1000	// 固定资产
	m_fData[14]
	m_fData[15]*1000	// 长期投资
	m_fData[16]*1000	// 流通负债
	m_fData[17]*1000	// 长期负债
	m_fData[18]*1000	// 资本公积金
	m_fData[19]			// 公积金/股
	m_fData[20]*1000	// 股东权益
	m_fData[21]*1000	// 主营业务收入
	m_fData[22]*1000	// 主营业务利润
	m_fData[23]*1000	// 其它业务利润
	m_fData[24]*1000	// 营业利益
	m_fData[25]*1000	// 投资收益
	m_fData[29]*1000	// 利润总额
	m_fData[30]
	m_fData[31]*1000	// 净利润
	m_fData[32]*1000	// 未分配利润
	m_fData[33]			// 每股未分配利润
	m_fData[34]			// 每股收益
	m_fData[35]			// 每股净资产
	m_fData[36]			// 调整每股净资产
	m_fData[37]			// 股东权益比率
	m_fData[38]			// 净资产收益率 %
*/
}FXJ_BASEDATA, *PFXJ_BASEDATA;

/*	适用各种数据类型存储的文件格式包含三部分

	1. 文件头：CLK_FHEADER

	2. 索引区：大小为 CLK_FHEADER.m_dwIndexRecordSize * CLK_FHADER.m_dwIndexRecordCount
				分为 CLK_FHADER.m_dwIndexRecordCount 个块，
				每块为 CLK_FHEADER.m_dwIndexRecordSize，即 CLK_INDEXRECORD

	3. 数据区：大小为CLK_FHEADER.m_dwBlockSize * CLK_FHEADER.m_dwBlockCount
				分为 CLK_FHADER.m_dwBlockCount 个 Block，
				每一 Block 有一个 CLK_BLOCKHEADER，和紧随其后的 CLK_BLOCKHEADER.m_dwDataRecordCount 个股票数据记录
				如果一个 Block 不能记录该股票全部记录，则 CLK_BLOCKHEAHDRE.m_dwPosNextBlock 指向下一个数据块
*/

#define	CLK_FHEADER_MAGIC		'FKLC'
#define	CLK_FHEADER_VERMAJOR	'2003'
#define	CLK_FHEADER_VERMINOR	'1002'

// 自有文件格式，文件头
typedef	struct clk_fheader
{
	// 文件信息
	DWORD	m_dwMagic;					// = CLK_FHEADER_MAGIC
	DWORD	m_dwVerMajor;				// = CLK_FHEADER_VERMAJOR
	DWORD	m_dwVerMinor;				// = CLK_FHEADER_VERMINOR
	DWORD	m_dwReserved1;				// 保留
	DWORD	m_dwDataType;				// 数据分类说明，see CStock::DataType
	DWORD	m_dwMarket;					// 市场，see CStock::StockMarket，如果文件包含多个市场，则=0无效
	time_t	m_time;						// 文件最后修改日期

	// 索引区信息
	DWORD	m_dwPosFirstIndex;			// 索引区起始位置
	DWORD	m_dwIndexRecordSize;		// 索引区记录单元字节数
	DWORD	m_dwIndexRecordCount;		// 索引区记录单元个数
	DWORD	m_dwStockCount;				// 本文件索引区中的总股票数

	// 数据区信息，包含m_dwBlockCount个Block，每个Block包含m_dwBlockSize/m_dwDataRecordSize个DataRecord
	DWORD	m_dwDataRecordSize;			// 数据区记录字节数
										// 根据m_dwDataType，
										// = sizeof(REPORT)
										// or = sizeof(MINUTE)
										// or = sizeof(KDATA)
	DWORD	m_dwRecordPerBlock;			// 每个Block的记录数
	DWORD	m_dwBlockSize;				// 数据区Block单元字节数，一个Block有一个BlockHeader和若干个DataRecord
	DWORD	m_dwPosFirstBlock;			// 文件中第一个Block位置.
	DWORD	m_dwPosFirstBlankBlock;		// 文件中第一个空Block的位置，所有空Block组成一个链表

	DWORD	m_dwReserved2[4];			// 保留

	char	m_szDescript[128];			// 文件说明
	char	m_szNextFileName[MAX_PATH];	// 有时候，一个文件不能太大，有多个文件，m_szNextFileName指向下
										// 一个同类型文件的相对目录，目前未使用
} CLK_FHEADER;

#define	CLK_INDEXRECORD_MAGIC	'XDNI'

// 自有文件格式，股票索引部分单元
typedef	struct clk_indexrecord
{
	DWORD	m_dwMagic;					// = CLK_INDEXRECORD_MAGIC
	DWORD	m_dwMarket;					// 股票市场 see CStock::StockMarket
	char	m_szCode[STKLIB_MAX_CODE2];	// 股票代码
	DWORD	m_dwPosFirstBlock;			// 本股票第一个Block起始位置，-1或0表示无
	DWORD	m_dwDataRecordCountTotal;	// 本股票数据记录总数
	DWORD	m_dwReserved;				// 保留
} CLK_INDEXRECORD;

#define	CLK_BLOCKHEADER_MAGIC	'KCLB'

// 自有文件格式，数据区部分，数据块头
typedef	struct clk_blockheader
{
	DWORD	m_dwMagic;					// = CLK_BLOCKHEADER_MAGIC
	DWORD	m_bUsed;					// 本Block是否使用
	DWORD	m_dwPosFirstRecord;			// 本Block数据记录起始位置
	DWORD	m_dwDataRecordCount;		// 本Block块数据记录个数
	DWORD	m_dwPosNextBlock;			// 含有本股票数据的下一个Block的起始位置，-1或0表示无
	DWORD	m_dwReserved;				// 保留
} CLK_BLOCKHEADER;

#pragma	pack()

//=============================================================================
// CTSKFile

class CTSKFile
{
public:
	CTSKFile();
	virtual ~CTSKFile();

protected:
	CSPFile	 m_file;
	CLK_FHEADER	m_header;

	// 记下当前正在操作的股票
	CLK_INDEXRECORD m_CurIndexRecord;
	DWORD m_dwPosCurIndex;

public:
	BOOL Open(LPCTSTR lpszFileName);
	void Close();

public:
	static BOOL BuildEmptyFile(LPCTSTR lpszFileName, DWORD dwDataType, DWORD dwMarket, DWORD dwIndexRecordCount = 3000,
				DWORD dwRecordPerBlock = 50, BOOL bRebuildIfExists = FALSE);
	static BOOL RemoveFileIfOutoftime(LPCTSTR lpszFileName, time_t tmNow);

	// Store Function
	BOOL	EmptyAll();
	DWORD	StoreDataRecord(DWORD dwMarket, const char * szCode, void * pData, DWORD dwDataElementSize, DWORD dwDataElementCount, BOOL bOverWrite);	// 返回成功保存记录数

	// Load Function
	DWORD	GetDataRecordCount(DWORD dwMarket, const char * szCode);
	DWORD	LoadDataRecord(DWORD dwMarket, const char * szCode,
							void * pData, DWORD dwDataElementSize, DWORD dwMaxDataElement);// 返回成功读取记录数

protected:
	DWORD	Hash(LPCTSTR key, DWORD dwMax);

	BOOL	GetDataInfo(DWORD dwMarket, const char * szCode, CLK_INDEXRECORD & idxRet, DWORD & dwPosIndexFind, BOOL bAddIfNotExists);
	BOOL	SetDataInfo(DWORD dwPosIndex, CLK_INDEXRECORD idx, BOOL bFlush);

	DWORD	GetFirstBlankBlockPos(BOOL bAddIfNotExists, BOOL bUseIt);
	DWORD	EmptyBlockChain(DWORD dwPosFirstBlock);
	
	DWORD	ReadData(DWORD dwPostBlock, void * pData, DWORD dwDataElementSize, DWORD dwMaxDataElement);
	DWORD	WriteData(DWORD dwPosBlock, void * pData, DWORD dwDataElementSize, DWORD dwDataElementCount, BOOL bFlush);
};

//=============================================================================
// CSelfDB

class CSelfDB : public CQianlong
{
public:
	CSelfDB(const char * rootpath, BOOL bOK = TRUE);
	virtual	~CSelfDB();

public:
	static char m_szDataType[256];

public:
	virtual	BOOL GetFileName(CSPString& sFileName, int nDataType, CStockInfo* pInfo = NULL, int nKType = CKData::ktypeDay);
	static int GetType() { return dbtypeSelfDB; }
	static const char * GetName() { return m_szDataType; }
	virtual	int	GetDBType() { return GetType(); }
	virtual	const char * GetDBTypeName() { return GetName(); }
	static CSPString GetNewsPath(DWORD dwMarket);
	static CSPString GetBasePath(DWORD dwMarket);
	static BOOL CreateSelfDB(const char * rootpath);
	static BOOL GetAccurateRoot(const char * rootpath, char *accurateroot, int maxlen);

public:
	virtual	int LoadCodetable(CStockContainer& container);
	virtual	int	StoreCodetable(CStockContainer& container);
	virtual	int	LoadKData(CStock* pstock, int nKType);
	virtual	int	LoadBasetable(CStockContainer& container);
	virtual	int StoreBasetable(CStockContainer& container);
	virtual	int	LoadDRData(CStock* pstock);
	virtual	int StoreDRData(CStock* pstock);
	virtual int	LoadOutline(CStock* pstock);
	virtual int	StoreOutline(OUTLINE* pOutline, int nCount);

	virtual int	LoadMinute(CStock* pstock);
	virtual int	StoreMinute(MINUTE* pMinute, int nCount);
	virtual int	LoadReport(CStock* pstock);
	virtual int	StoreReport(REPORT* pReport, int nCount, BOOL bBigTrade);

public:
	virtual	int	InstallCodetbl(const char * filename, const char * orgname);
	virtual	int	InstallCodetblBlock(const char * filename, const char * orgname);
	virtual	int	InstallCodetblFxjBlock(const char * filename, const char * orgname);
	virtual	int InstallDRData(CDRData& drdata);
	virtual	int	InstallDRDataClk(const char * filename, const char * orgname);
	virtual	int	InstallDRDataFxj(const char * fxjfilename);
	virtual	int	InstallBasetable(const char * filename, const char * orgname);
	virtual	int	InstallBasetableTdx(const char * filename);
	virtual	int	InstallBasetableFxj(const char * filename);
	virtual	int InstallNewsText(const char * filename, const char * orgname);
	virtual	int InstallNewsText(const char * buffer, int nLen, const char * orgname);
};
