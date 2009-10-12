/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CQianLong;
*/

#if	!defined( __STKLIB_QIANLONG_H__ )
#define	__STKLIB_QIANLONG_H__

#include	"StdAfx.h"
#include	"../Include/Stock.h"
#include	"../Include/Database.h"

//	钱龙网络版动态数据格式（即ml30\lond\dat下的数据格式）
//	Set the default value
#define EXCH_MIN	240
#define EXCH_A		9*60+30
#define EXCH_AE		11*60+30
#define EXCH_B		13*60+00
#define EXCH_BE		15*60+00
#define SEP_TAG		0xffffffff
#define TRA_MAXN	1000
#define INFO_PARA	0x84
#define TYPE_NUM	0x4
#define SLHS_SIZE	0x300
#define SH_ZS		'a'
#define SH_AG		'b'
#define SH_BG		'c'
#define SH_ZQ		'd'
#define SZ_ZS		'A'
#define SZ_AG		'B'
#define SZ_BG		'C'
#define SZ_ZQ		'D'
#define DH_ZS		0x10
#define DH_AG		0x11
#define DH_BG		0x12
#define DH_ZQ		0x13
#define DZ_ZS		0x20
#define DZ_AG		0x21
#define DZ_BG		0x22
#define DZ_ZQ		0x23

#pragma	pack(1)

struct QL_APD_stock_data{	// in 199YMMDD.DAT
	BYTE data_id; // =0x73 with min_data ,
	// =0x09 without min_data ,
	// =0x20 is deleted

	BYTE stock_type; // many be a,b,c,d or A,B,C,D
	BYTE stock_code[6]; // stock code
	BYTE stock_name[8]; // name of the stock

	DWORD last_close_price;
	DWORD open_price;
	DWORD high_price;
	DWORD low_price;
	DWORD close_price;
	DWORD total_volume;
	DWORD total_value;
};

struct QL_APD_stock_min{ // store data for normal shares
	DWORD cur_price; // price of every minutes
	DWORD total_volume; // summary volume by this minute
	WORD average_percent; // swing percent of the average value
};

struct QL_APD_index_min{ // store data for index
	DWORD cur_price; // index of every minutes
	DWORD total_volume; // summary volume by this minute
	WORD average_percent; // swing percent of the average index

	WORD buy_vigour; // the vigour value of buying
	WORD sell_vigour; // the vigour value of selling
};

// Develop data files by appdata structured upwards

struct QL_Data_5min{ // in .nmn files
	DWORD min_off; // Format is MMDDHHMM
	DWORD open_price; // 0.001
	DWORD high_price; // 0.001
	DWORD low_price; // 0.001
	DWORD close_price; // 0.001

	DWORD min_amount; // 100
	DWORD min_volume; // 100

	DWORD time_count; // sum trade time
	BYTE share_value; // share value
	BYTE share_number; // share break number
	WORD share_bonus; // share bonus
	DWORD shares_number; // sum number
};

struct QL_His_data{ // in lonhis.???
	DWORD cur_price;
	DWORD total_volume; // total volume
	DWORD total_value; // total value by this minute
	DWORD buy_volume; // initiactive total buy volume
	DWORD sell_volume; // initiactive total sell volume
	WORD rise_num; // summary rise shares of this minute
	WORD fall_num; // summary fall shares of this minute

	WORD average_percent; // swing percent of the average index
	WORD buy_vigour; // the vigour value of buying
	WORD sell_vigour; // the vigour value of selling
	WORD value_ADL; // the value of current ADL index
	WORD swing_flag; // the flag of rise or fall
};

struct QL_Tra_data{ // in lontra.???
	WORD time_off;
	DWORD cur_price;
	DWORD total_vol;
	DWORD buy_price;
	DWORD sell_price;
};

struct QL_Info_data{ // in loninfo.???
	BYTE stock_id; // This byte is to id stock
	BYTE data_tag; // always be 0 to id
	WORD stock_num;
	WORD start_num;
	WORD max_tra;
	WORD min_exch;

	WORD exch_min;
	WORD A_begin_time;
	WORD A_end_time;
	WORD B_begin_time;
	WORD B_end_time;
};

struct QL_Cdp_data{ // shacdp.dat of HXTW
	BYTE stock_code[6];
	BYTE reserved[20];
	DWORD reserved2;
};

// some dynamic data files structured upwards
struct QL_Data_day{	// in .day files
	DWORD	day_date;		//日期	Format is XXMMDDHHMM for 5min, Format is YYYYMMDD for day
	DWORD	open_price;		// 开盘 0.001
	DWORD	high_price;		// 最高价 0.001
	DWORD	low_price;		// 最低价 0.001
	DWORD	close_price;	// 收盘 0.001
	DWORD	day_amount;		// 成交额(千元) 1000
	DWORD	day_volume;		// 成交量(手) 100
	
	DWORD	time_count;		// sum trade time
	BYTE	share_value;	// share value
	BYTE	share_number;	// share break number
	WORD	share_bonus;	// share bonus
	DWORD	shares_number;	// sum number
};

struct QL_Stock_info_V302{ // in lonnow.??? of QL302S
	BYTE data_id; // =0 is OK , =0xff is deleted
	BYTE stock_name[8];
	BYTE stock_type;
	BYTE stock_code[6];
	DWORD last_close_price;
	DWORD open_price;
	DWORD high_price;
	DWORD low_price;
	DWORD close_price;

	DWORD total_volume;
	DWORD total_value;

	DWORD buy_1_price;
	DWORD buy_1_volume;
	DWORD buy_2_price;
	DWORD buy_2_volume;
	DWORD buy_3_price;
	DWORD buy_3_volume;
	DWORD sell_1_price;
	DWORD sell_1_volume;
	DWORD sell_2_price;
	DWORD sell_2_volume;
	DWORD sell_3_price;
	DWORD sell_3_volume;

	WORD reserved; // Most time is 0x0064
};

struct QL_Stock_info2_V304{ // in lonnow.??? of QL304S
	BYTE data_id; // =0 is OK , =0xff is deleted
	BYTE stock_name[8];
	BYTE stock_type;
	BYTE stock_code[6];
	DWORD last_close_price;
	DWORD open_price;
	DWORD high_price;
	DWORD low_price;
	DWORD close_price;

	DWORD total_volume;
	DWORD total_value;

	DWORD buy_1_price;
	DWORD buy_1_volume;
	DWORD buy_2_price;
	DWORD buy_2_volume;
	DWORD buy_3_price;
	DWORD buy_3_volume;

	DWORD sell_1_price;
	DWORD sell_1_volume;
	DWORD sell_2_price;
	DWORD sell_2_volume;
	DWORD sell_3_price;
	DWORD sell_3_volume;

	WORD reserved; // Most time is 0x0064
	DWORD reserved2; // Maybe the PINYIN
};

struct QL_Sse_data{ // in sse20a.dat,sse21a.dat,sse22a.dat
	BYTE data_id; // 0x30,0x31,0x32
	BYTE stock_code[6];
	BYTE stock_name[8];
	BYTE stock_type1; // 0x30 or 0x31
	BYTE stock_type2; // 00 or 01
	BYTE reserved; // =0
};

struct QL_Name_table{ // in nametbl.*
	BYTE data_id; // 0x10...
	BYTE stock_code[6];
	BYTE stock_name[8];
	BYTE data_tag;
};

#pragma	pack()

/*
void display_version(char*);
void check_config_file(char*);
void display_helpinfo(void);
void prepair_data(void);
void appdata_error(WORD,char*,char*,char*);
void check_anlysis_system(void);
void removetmpfiles(void);
void process_apdfile(char*);
int detect_apdfile(char*,char*,char*);
void clear_slonhsrec(struct Slon_HS);
*/

BOOL convert_QL_Data_5min_to_KDATA( DWORD dwMarket, const char *szCode, struct QL_Data_5min * pqlkd, KDATA *pkd );
BOOL convert_QL_Data_day_to_KDATA( DWORD dwMarket, const char *szCode, struct QL_Data_day * pqlkd, KDATA *pkd );
BOOL convert_KDATA_to_QL_Data_day( KDATA * pkd, struct QL_Data_day * pqlkd );
BOOL convert_KDATA_to_QL_Data_5min( KDATA * pkd, struct QL_Data_5min * pqlkd );
void ConvertQLStockInfo( DWORD dwMarket, QL_Stock_info_V302 & block, CStockInfo *pInfo );
void ConvertQLStockInfo( DWORD dwMarket, QL_Stock_info2_V304 & block, CStockInfo *pInfo );


#define	QL_SHORTNAME_LEN	4

/***
	钱龙数据格式的读取类。
*/
class CQianlong : public IStStore
{
public:
	CQianlong( const char * rootpath, BOOL bOK = TRUE );
	virtual	~CQianlong( );

	enum QL_Version {
		versionUnknown	=	0x01,
		version302	=	0x02,
		version304	=	0x03,
	};

	virtual	int	GetMaxStockNumber( );
	virtual	int	LoadCodetable( CStockContainer & container );
	virtual	int	StoreCodetable( CStockContainer & container );
	virtual	int	LoadKDataCache( CStockContainer & container, PROGRESS_CALLBACK fnCallback, void *cookie, int nProgStart, int nProgEnd );
	virtual	int	LoadBasetable( CStockContainer & container );
	virtual	int StoreBasetable( CStockContainer & container );
	virtual	int	LoadBaseText( CStock *pstock );
	virtual	int	LoadKData( CStock *pstock, int nKType );
	virtual	int	LoadDRData( CStock *pstock );
	virtual	int StoreDRData( CStock *pstock );
	virtual int	LoadReport( CStock *pstock );
	virtual int	LoadMinute( CStock *pstock );
	virtual int	LoadOutline( CStock *pstock );
	virtual int	StoreReport( REPORT * pReport, int nCount, BOOL bBigTrade );
	virtual int	StoreMinute( MINUTE * pMinute, int nCount );
	virtual int	StoreOutline( OUTLINE * pOutline, int nCount );

	virtual	int	InstallCodetbl( const char * filename, const char *orgname );
	virtual	int	InstallCodetblBlock( const char * filename, const char *orgname );
	virtual	int	InstallCodetblFxjBlock( const char * filename, const char *orgname );
	virtual	int	InstallKData( CKData & kdata, BOOL bOverwrite = FALSE );
	virtual	int InstallKDataTy( const char * stkfile, int nKType, PROGRESS_CALLBACK fnCallback, void *cookie );
	virtual	int InstallKDataFxj( const char * dadfile, int nKType, PROGRESS_CALLBACK fnCallback, void *cookie );
	virtual	int InstallDRData( CDRData & drdata );
	virtual	int	InstallDRDataClk( const char * filename, const char *orgname );
	virtual	int	InstallDRDataFxj( const char * fxjfilename );
	virtual	int	InstallBasetable( const char * filename, const char *orgname );
	virtual	int	InstallBasetableTdx( const char * filename );
	virtual	int	InstallBasetableFxj( const char * filename );
	virtual	int InstallBaseText( const char * filename, const char *orgname );
	virtual	int InstallBaseText( const char * buffer, int nLen, const char *orgname );
	virtual	int InstallNewsText( const char * filename, const char *orgname );
	virtual	int InstallNewsText( const char * buffer, int nLen, const char *orgname );

	virtual	BOOL GetFileName( CSPString &sFileName, int nDataType,
				CStockInfo * pInfo = NULL, int nKType = CKData::ktypeDay );

	static	BOOL	GetAccurateRoot( const char * rootpath, char *accurateroot, int maxlen );
	static	int		DetectVersion( const char * szRootPath );

	// Attributes
	static	char m_szDataType[256];
	static	int		GetType( ) { return dbtypeQianlong; }
	static	const char * GetName( )	{ return m_szDataType; }
	virtual	int	GetDBType( ) { return GetType(); }
	virtual	const char * GetDBTypeName( ) { return GetName(); }

protected:
	int		DetectVersion( );
protected:
	int		m_nVersion;
};


#endif	//__STKLIB_QIANLONG_H__
