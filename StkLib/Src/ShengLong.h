/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CShengLong;
*/

#if	!defined( __STKLIB_SHENGLONG_H__ )
#define	__STKLIB_SHENGLONG_H__

#include	"StdAfx.h"
#include	"../Include/Stock.h"
#include	"../Include/Database.h"

#pragma	pack(1)

struct Slon_para{ // in file slonpara.dat
	WORD sh_A_para; // Offset of SH_A stock
	WORD sh_B_para;
	WORD sh_C_para;
	WORD sh_total_para;

	WORD sz_A_para;
	WORD sz_B_para;
	WORD sz_C_para;
	WORD sz_total_para;

	WORD data_tag[3];
};

struct Slon_day{ // in .day files of Slon
	DWORD day_date; // Format is YYYYMMDD
	DWORD open_price; // 0.001
	DWORD close_price; // 0.001
	DWORD high_price; // 0.001
	DWORD low_price; // 0.001

	DWORD day_amount; // 1000
	DWORD day_volume; // 100
	WORD reserved[6]; // Some infomation I don't know
};

struct Slon_HS{ // in lonhs.dat of Slon
	WORD data_tag1; // Some infomation I don't know
	BYTE stock_type; // =0xff is delete, =0x64 is OK
	DWORD stock_code; // number of stock code
	BYTE stock_name[8]; // name of stock
	BYTE data_tag2; // =0x0, Some infomation I don't know
	WORD data_tag3; // =0x64, Some infomation I don't know
	WORD last_close_price;
	WORD open_price;
	DWORD PMA5_volume; // The volume of 5day average

	WORD high_price;
	WORD low_price;
	WORD close_price;
	DWORD data_tag4; // Some infomation I don't know
	DWORD total_volume1; // Total volume
	DWORD total_volume2; // Total volume

	WORD buy_1_price; // The price of buying now
	BYTE buy_2_sub; // The offset of buy 1 and buy 2 price
	BYTE buy_3_sub; // The offset of buy 1 and buy 3 price
	DWORD buy_1_volume;
	DWORD buy_2_volume;
	DWORD buy_3_volume;
	WORD sell_1_price; // The price of selling now
	BYTE sell_2_add; // The offset of sell 1 and sell 2 price
	BYTE sell_3_add; // The offset of sell 1 and sell 3 price

	DWORD sell_1_volume;
	DWORD sell_2_volume;
	DWORD sell_3_volume;

	WORD refx;
	WORD refy;
	WORD hisfptr;

	WORD numppg;
	BYTE pgnum;

	WORD hisptr00;
	WORD hisptr01;
	WORD hisptr02;
	WORD hisptr03;
	WORD hisptr04;
	WORD hisptr05;
	WORD hisptr06;
	WORD hisptr07;
	WORD hisptr08;
	WORD hisptr09;

	WORD data_tag5;

	DWORD cur_volume;
	BYTE reserved1[12];

	WORD minfptr;
	WORD minsize;

	DWORD data_tag6;
	DWORD out_volume;

	BYTE reserved2[133];
	BYTE data_tag7; // always 07

	BYTE reserved3[145];
	BYTE end_hour;
	BYTE end_minute;
	WORD lastmin_vol;

	WORD data_tag8;
	DWORD total_volume3;
	WORD cur_price;
	DWORD average_price;

	BYTE reserved4[82];
};

#pragma	pack()

/* SLON data structure from MR.ZHAO BIAO

Lonhs.dat结构分析（QB）

位置 长度 类型 说明

1 2 INTEGER ****************** 

2 1 STRING 标志，FF->delete 64-> ok 

3 4 LONG 股票代码 

4 8 STRING 股票名称 

5 1 STRING 00-> ? ***************** 

6 2 INTEGER 64-> ? ***************** 

7 2 INTEGER 前收盘价 

8 2 INTEGER 开盘价 

9 4 LONG 五日均量 

10 2 INTEGER 最高价 

11 2 INTEGER 最低价 

12 2 INTEGER 收盘价 

13 4 LONG ***************** 

14 4 LONG 总成交量 

15 4 LONG 总成交量 

16 2 INTEGER 当前买入价，即买1 

17 1 STRING 买2价，为与当前买入价的差值 

18 1 STRING 买3价，为与当前买入价的差值 

19 4 LONG 买1量 

20 4 LONG 买2量 

21 4 LONG 买3量 

22 2 INTEGER 当前卖出价，即卖1 

23 1 STRING 卖2价，为与当前买入价的差值 

24 1 STRING 卖3价，为与当前买入价的差值 

25 4 LONG 卖1量 

26 4 LONG 卖2量 

27 4 LONG 卖3量 

28 2 INTEGER ？与分时线坐标有关 

29 2 INTEGER ？与分时线坐标有关 

30 2 INTEGER 最近成交数据在LONHIS。DAT文件中的指针 

31 2 INTEGER 在LONHIS。DAT中的首页数据个数 

32 1 STRING 在LONHIS。DAT中的数据段数量，页数 

33 2 INTEGER 第一页在LONHIS。DAT中的指针1，FF-》无 

34 2 INTEGER 第二页指针 

35 2 INTEGER 第三页指针 

36 2 INTEGER 第四页指针 

37 2 INTEGER 第五页指针 

38 2 INTEGER 第六页指针 

39 2 INTEGER 第七页指针 

40 2 INTEGER 第八页指针 

41 2 INTEGER 第九页指针 

42 2 INTEGER 第十页指针 

43 2 INTEGER **************** 

44 4 LONG 当前成交手数 

45 12 STRING **************** 

46 2 INTEGER 该股票数据在LONMIN。DAT中的指针 

47 2 INTEGER 分时线在LONMIN文件中的长度，（分钟） 

48 2 INTEGER **************** 

49 2 INTEGER **************** 

50 4 LONG 外盘数据 

51 133 STRING **************** 

52 1 STRING ？ 似乎只能为?/FONT>07” 

53 145 STRING **************** 

54 1 STRING 小时，似为分时线的终止时间 

55 1 STRING 分钟， 

56 2 INTEGER 最近一分钟成交量 

57 2 INTEGER *************** 

58 4 LONG 总成交量 

59 2 INTEGER 最新成交价 

60 4 LONG 均价 

61 82 STRING ************************* 

　

**************** 项作用不详。

　

Lohis.dat结构分析（QB）

位置 长度 类型 说明

1 1 STRING 时 

2 1 STRING 分 

3 4 LONG 成交量（手） 

4 2 INTEGER 成交价 

5 2 INTEGER 买入价 

6 2 INTEGER 卖出价 

　

　

该文件每128个记录为1只股票的某一时间段价格记录，其指针在LONHS。DAT文件中。

按成交情况记录

Lomin.dat结构分析（QB）

位置 长度 类型 说明

1 1 STRING 时 

2 1 STRING 分 

3 4 LONG 现成交量（现手） 

4 4 LONG 总成交量（总手） 

5 2 INTEGER 当前成交价 

6 2 SINGLE 均价 

7 2 INTEGER 开盘至当前的分钟数 

8 6 STRING 估计为买卖力道数据 

该文件每400个记录为1只股票的分时线数据，其指针在LONHS。DAT文件中。

按时间记录
*/

/***
	胜龙数据格式的读取，未实现
*/
class CShenglong : public IStStore
{
public:
	CShenglong( const char * rootpath, BOOL bOK = TRUE );
	virtual	~CShenglong( );

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

	// Attributes
	static	char m_szDataType[256];
	static	int		GetType( ) { return dbtypeShenglong; }
	static	const char * GetName( )	{ return m_szDataType; }
	virtual	int	GetDBType( ) { return GetType(); }
	virtual	const char * GetDBTypeName( ) { return GetName(); }
};


#endif	//__STKLIB_SHENGLONG_H__
