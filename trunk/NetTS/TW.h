
#ifndef __NETTS_TW_H__
#define __NETTS_TW_H__

#include "Stockdrv.h"

#pragma pack(1)

typedef	STOCK_STRUCTEx TW_STOCK;
typedef CArray<TW_STOCK, TW_STOCK&> CTWStockArray;

//typedef struct _tw_stock_t
//{
//	BYTE	m_type;					// stock's type
//	char	m_code[6];				// stock code
//} TW_STOCK;

#define TW_MAGIC				0xFDFDFDFD
#define	TW_MAGIC_BYTE			0xFD
#define TW_LENGTH_LEN			8

typedef struct _tw_header_t
{
	DWORD m_magic;					// 0xFDFDFDFD
	char m_length[TW_LENGTH_LEN];	// length
	char m_end;						// always 0
} TW_HEADER;

typedef struct _tw_login_t
{
	TW_HEADER	m_header;			// tw header
	BYTE m_tag;						// 0x0A
	WORD m_name_len;				// name length
	char m_data[256];				// (NAME)(WORD m_passwd_len)(PASSWD)
} TW_LOGIN;

typedef struct _tw_ask_t
{
	TW_HEADER	m_header;			// tw header
	BYTE	m_tag1;					// second data type
	BYTE	m_tag2;					// data type
	BYTE	m_serial;				// request serial, max 0x7F
	BYTE	m_reserved;				// always 0x00
	SHORT	m_size;					// request data's size
	TW_STOCK	m_stocks[32];		// max 32 stocks
} TW_ASK;

typedef struct _tw_ans_t {
	TW_HEADER	m_header;			//
	BYTE	m_tag1;					// second data type
	BYTE	m_tag2;					// data type
	BYTE	m_serial;				// request serial, max 0x7f
	BYTE	m_reserved;				// always 0x00
	SHORT	m_size;					// request data's size
} TW_ANS;

typedef struct _tw_ans_init_t {
	BYTE	m_tag;					// = 0xfd
	CHAR	m_name[8];
	BYTE	m_type;
	CHAR	m_code[6];
	DWORD	m_lastclose;			// 昨收 0.001
	DWORD	m_reserved2;
	CHAR	m_shortname[4];
} TW_ANS_INIT;

typedef struct _tw_ans_report_t {
	WORD	m_number;				// No.
	DWORD	m_volnow;				// 现手（单位为股）
	DWORD	m_open;					// 0.001
	DWORD	m_high;					// 0.001
	DWORD	m_low;					// 0.001
	DWORD	m_new;					// 0.001
	DWORD	m_volume;
	DWORD	m_amount;
	DWORD	m_buy1;					// 0.001
	DWORD	m_buy1vol;
	DWORD	m_buy2;					// 0.001
	DWORD	m_buy2vol;
	DWORD	m_buy3;					// 0.001
	DWORD	m_buy3vol;
	DWORD	m_sell1;				// 0.001
	DWORD	m_sell1vol;
	DWORD	m_sell2;				// 0.001
	DWORD	m_sell2vol;
	DWORD	m_sell3;				// 0.001
	DWORD	m_sell3vol;
	WORD	m_reserved;				// = 0x64 0x00
} TW_ANS_REPORT;

typedef struct _tw_detail_t {
	WORD	m_offset;
	DWORD	m_price;				// 0.001
	DWORD	m_volume;
	DWORD	m_buy;					// 0.001
	DWORD	m_sell;					// 0.001
} TW_DETAIL;

typedef struct _tw_minute_t {
	DWORD	m_data1;				// * 0.001 if price
	DWORD	m_data2;				// * 0.001 if price
} TW_MINUTE;

typedef struct _tw_ans_minute_t {
	TW_DETAIL	m_details[11];
	WORD	m_offset;
	DWORD	m_bargain;
	DWORD	m_outter;
	DWORD	m_innter;
	DWORD	m_open;					// 0.001
	DWORD	m_high;					// 0.001
	DWORD	m_low;					// 0.001
	DWORD	m_new;					// 0.001
	DWORD	m_volume;
	DWORD	m_amount;
	DWORD	m_buy1;					// 0.001
	DWORD	m_buy1vol;
	DWORD	m_buy2;					// 0.001
	DWORD	m_buy2vol;
	DWORD	m_buy3;					// 0.001
	DWORD	m_buy3vol;
	DWORD	m_sell1;				// 0.001
	DWORD	m_sell1vol;
	DWORD	m_sell2;				// 0.001
	DWORD	m_sell2vol;
	DWORD	m_sell3;				// 0.001
	DWORD	m_sell3vol;
	WORD	m_reserved;				// = 0x64 0x00
	TW_MINUTE	m_minutes[246];
} TW_ANS_MINUTE;

typedef struct _tw_ans_history_t {
	DWORD	m_date;					//日期	Format is MMDDHHMM for 5min, Format is YYYYMMDD for day
	DWORD	m_open;					// 0.001
	DWORD	m_high;					// 0.001
	DWORD	m_low;					// 0.001
	DWORD	m_close;				// 0.001
	DWORD	m_volume;				// 100
} TW_ANS_HISTORY;

typedef	RCV_MULTISORT_STRUCTEx	TW_ANS_MULTISORT;

typedef struct _tw_ans_detail_t {
	WORD	m_offset;
	DWORD	m_price;				// 0.001
	DWORD	m_volume;
	DWORD	m_buy1;					// 0.001
	DWORD	m_sell1;				// 0.001
} TW_ANS_DETAIL;

/* INIT
	m_tag1 = 0x01
	m_tag2 = 0x01
	m_serial = 0x00
	m_size = 0x0007
	the next 7 bytes = 0xdc, 0xcc, 0x31, 0x01, 0x00, 0x00, 0x00
	the next is string:
		"E013.45D013.04D022.03D043.20xcs1.00.is2.00"
*/

/* REPORT
	m_tag1 = 0x00
	m_tag2 = 0x03
	m_size = 本次请求股票个数
	重复m_size次TW_STOCK
*/

/* MINUTE
	m_tag1 = 0x01
	m_tag2 = 0x04
	m_size = 0x0001
	m_stocks = 1个stock
*/

/* HISTORY
	更换股票后的第一次请求:
		m_tag1 = 0x20日 0x33五分 0x43十五分 0x53三十分 00x63六十分 0x83周 0x93月
		m_tag2 = 0x09
		m_size = 请求数据个数*-1
		m_stocks = 1个stock

	更换数据类型后的第一次请求:
		m_tag1 = 0x11日 0x31五分 0x41十五分 0x51三十分 00x61六十分 0x81周 0x91月
		m_tag2 = 0x09
		m_size = 请求数据个数*-1
		m_stocks = 1个stock

	连续请求:
		m_tag1 = 0x12日 0x32五分 0x42十五分 0x52三十分 00x62六十分 0x82周 0x92月 
		m_tag2 = 0x09
		m_size = 请求数据个数*-1
		m_stocks = 1个stock
*/

/* MULTISORT
	m_tag1 = 0x0a
	m_tag2 = 0x08
	m_size = 请求数据个数*-1，一般情况下为 ((WORD)-9)
	m_stocks[0].m_type = enum StockType
	m_stocks[0].m_code = 0x06 0x00 0xff 0x01 0x00 0x00
*/

/* DETAIL
	m_tag1 = 0x01
	m_tag2 = 0x02
	m_size = 0x0001
	m_stocks = 1个stock
*/

/* BASE
	m_tag1 = 0x05
	m_tag2 = 0x0b
	m_size = 0x0005
	m_stocks = 5个stock
*/

#pragma pack()

// return the real length of the constructed buffer, -1 means no enough memory
int ConstructLength(TW_HEADER& header, int len);
int ConstructLoginBuffer(BYTE* buffer, size_t maxlen, LPCTSTR lpszUser, LPCTSTR lpszPasswd);
int ConstructAskInitBuffer(TW_ASK& ask);
int ConstructAskReportBuffer(TW_ASK& ask, TW_STOCK* pStocks, size_t size);
int ConstructAskMinuteBuffer(TW_ASK& ask, TW_STOCK* pStock);
int ConstructAskHistoryBuffer(TW_ASK& ask, TW_STOCK* pStock, int nKType, int nDataCount, BOOL bChangeStock, BOOL bFirstRequest);
int ConstructAskMultisortBuffer(TW_ASK& ask, TW_STOCK* pStock);
int ConstructAskDetailBuffer(TW_ASK& ask, TW_STOCK* pStock);
int ConstructAskBaseBuffer(TW_ASK& ask, TW_STOCK* pStock);

size_t TryGetLength(TW_HEADER& header);
size_t TryGetLength(BYTE* buffer, size_t len);
BOOL IsLoginOK(BYTE* buffer, size_t len);
int TryGetInit(BYTE* buffer, size_t len, PRCV_DATA pRCV_DATA);
int TryGetReport(BYTE* buffer, size_t len, PRCV_DATA pRCV_DATA);
int TryGetMinute(BYTE* buffer, size_t len, PRCV_DATA pRCV_MINUTE, PRCV_DATA pRCV_REPORT);
int TryGetHistory(BYTE* buffer, size_t len, PRCV_DATA pRCV_DATA);
int TryGetMultisort(BYTE* buffer, size_t len, PRCV_DATA pRCV_DATA);
int TryGetDetail(BYTE* buffer, size_t len, PRCV_DATA pRCV_DATA);
int TryGetBase(BYTE* buffer, size_t len, PRCV_DATA pRCV_DATA);

#endif
