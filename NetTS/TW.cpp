// TW.cpp : implementation file
//

#include "stdafx.h"
#include "TW.h"
#include "SPTime.h"
#include "TSCache.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TW Interface

static BYTE g_serial = 0x00;
static CTWStockArray g_serial_stocks[0x80];
static int g_serial_count[0x80];

BYTE GetNextSerial(TW_STOCK & stock, int count = 0)
{
	g_serial ++;
	if(g_serial > 0x7f)	g_serial = 0x01;
	if(g_serial >= 1 && g_serial <= 0x7f)
	{
		g_serial_stocks[g_serial].RemoveAll();
		g_serial_stocks[g_serial].Add(stock);
		g_serial_count[g_serial] = count;
	}
	return g_serial;
}

BYTE GetNextSerial(CTWStockArray & stocks, int count = 0)
{
	g_serial ++;
	if(g_serial > 0x7f)	g_serial = 0x01;
	if(g_serial >= 1 && g_serial <= 0x7f)
	{
		g_serial_stocks[g_serial].RemoveAll();
		g_serial_stocks[g_serial].Copy(stocks);
		g_serial_count[g_serial] = count;
	}
	return g_serial;
}

BOOL GetSerialStock(BYTE serial, TW_STOCK & stock, int * pcount = NULL)
{
	if(serial >= 1 && serial <= 0x7f)
	{
		memset(&stock, 0, sizeof(stock));
		if(g_serial_stocks[serial].GetSize() > 0)
			memcpy(&stock, &(g_serial_stocks[serial].ElementAt(0)), sizeof(stock));
		if(pcount)
			*pcount = g_serial_count[serial];
		return TRUE;
	}
	return FALSE;
}

BOOL GetSerialStock(BYTE serial, CTWStockArray & stocks, int * pcount = NULL)
{
	if(serial >= 1 && serial <= 0x7f)
	{
		stocks.RemoveAll();
		if(g_serial_stocks[serial].GetSize() > 0)
			stocks.Copy(g_serial_stocks[serial]);
		if(pcount)
			*pcount = g_serial_count[serial];
		return TRUE;
	}
	return FALSE;
}

int ConstructLength(TW_HEADER& header, int len)
{
	CString	string = itoa(len,header.m_length,16);
	int nZeros = TW_LENGTH_LEN-string.GetLength();
	int i;
	for(i=0; i<nZeros; i++)
		
	for(i=0; i<TW_LENGTH_LEN; i++)
	{
		if(i< nZeros)
			header.m_length[i] = '0';
		else
			header.m_length[i] = string[i-nZeros];
	}
	return len;
}

int ConstructLoginBuffer(BYTE* buffer, size_t maxlen, LPCTSTR lpszUser, LPCTSTR lpszPasswd)
{
	int nLen = 0;
	int nLenUser = strlen(lpszUser);
	int nLenPasswd = strlen(lpszPasswd);

	TW_LOGIN	packet;
	memset(&packet,0,sizeof(packet));
	packet.m_header.m_magic = TW_MAGIC;
	if(maxlen < (int)(5 + sizeof(TW_HEADER) + nLenUser + nLenPasswd))
	{
		nLen = 5 + sizeof(TW_HEADER);
		return -1;
	}
	packet.m_name_len = (WORD)nLenUser;
	memcpy(packet.m_data, lpszUser, nLenUser);
	*(WORD*)(packet.m_data + nLenUser)	=	(WORD)nLenPasswd;
	memcpy(packet.m_data + nLenUser + sizeof(WORD), lpszPasswd, nLenPasswd);

	nLen = 5 + sizeof(TW_HEADER) + nLenUser + nLenPasswd;
	ConstructLength(packet.m_header, nLen - sizeof(packet.m_header));

	if(buffer)
		memcpy(buffer, &packet, nLen);
	return nLen;
}

int ConstructAskInitBuffer(TW_ASK& ask)
{
	memset(&ask, 0, sizeof(ask));
	ask.m_header.m_magic = TW_MAGIC;

	//const char * str_tag = "E013.45D013.04D022.03D043.20xcs1.00.is2.00";
	const char * str_tag = "E013.66D013.04D022.09D043.41D181.01D063.85";

	ask.m_tag1 = 0x01;
	ask.m_tag2 = 0x01;
	ask.m_serial = 0x00;
	ask.m_size = 0x0007;

	CSPTime sptime = CTSCache::GetInstance().GetLocalLatest();
	DWORD	date = sptime.ToStockTimeDay();
	memcpy(&(ask.m_stocks[0].m_type), &date, sizeof(date));
//	ask.m_stocks[0].m_type = 0xdc;
//	ask.m_stocks[0].m_code[0] = (char)0xcc;
//	ask.m_stocks[0].m_code[1] = 0x31;
//	ask.m_stocks[0].m_code[2] = (char)0x01;	//0xdc cc 31 01 = 20040924
	ask.m_stocks[0].m_code[3] = 0x00;
	ask.m_stocks[0].m_code[4] = 0x00;
	ask.m_stocks[0].m_code[5] = 0x00;
	memcpy(&(ask.m_stocks[1]), str_tag, strlen(str_tag));
	int ret = (6 + sizeof(ask.m_header) + sizeof(TW_STOCK) + strlen(str_tag));
	ConstructLength(ask.m_header, ret - sizeof(ask.m_header));
	return ret;
}

int ConstructAskReportBuffer(TW_ASK& ask, TW_STOCK* pStocks, size_t size)
{
	memset(&ask, 0, sizeof(ask));
	ask.m_header.m_magic = TW_MAGIC;

	SHORT shortSize = (SHORT)size;
	if(shortSize > sizeof(ask.m_stocks)/sizeof(TW_STOCK))
		shortSize = sizeof(ask.m_stocks)/sizeof(TW_STOCK);
	if(shortSize <= 0)
		return -1;

	ask.m_tag1 = 0x00;
	ask.m_tag2 = 0x03;
	CTWStockArray astocks;
	astocks.SetSize(0, size+1);
	for(size_t i=0; i<size; i++)	astocks.Add(pStocks[i]);
	ask.m_serial = GetNextSerial(astocks);
	ask.m_size = shortSize;

	if(size > sizeof(ask.m_stocks)/sizeof(TW_STOCK))
		return -1;

	if(pStocks && shortSize > 0)
		memcpy(ask.m_stocks, pStocks, shortSize*sizeof(TW_STOCK));

	int ret = (6 + sizeof(ask.m_header) + shortSize * sizeof(TW_STOCK));
	ConstructLength(ask.m_header, ret - sizeof(ask.m_header));
	return ret;
}

int ConstructAskMinuteBuffer(TW_ASK& ask, TW_STOCK* pStock)
{
	memset(&ask, 0, sizeof(ask));
	ask.m_header.m_magic = TW_MAGIC;

	ask.m_tag1 = 0x01;
	ask.m_tag2 = 0x04;
	ask.m_size = 0x0001;
	memcpy(&(ask.m_stocks[0]), pStock, sizeof(TW_STOCK));
	ask.m_serial = GetNextSerial(ask.m_stocks[0]);
	
	int ret = (6 + sizeof(ask.m_header) + sizeof(TW_STOCK));
	ConstructLength(ask.m_header, ret - sizeof(ask.m_header));
	return ret;
}

int ConstructAskHistoryBuffer(TW_ASK& ask, TW_STOCK* pStock, int nKType, int nDataCount, BOOL bChangeStock, BOOL bFirstRequest)
{
	memset(&ask, 0, sizeof(ask));
	ask.m_header.m_magic = TW_MAGIC;

	SHORT shortSize = (SHORT)nDataCount;

	ask.m_tag1 = 0x00;
	ask.m_tag2 = 0x09;
	ask.m_size = - shortSize;
	memcpy(&(ask.m_stocks[0]), pStock, sizeof(TW_STOCK));
	ask.m_serial = GetNextSerial(ask.m_stocks[0], nDataCount);

	if (ktypeMin5 == nKType)
		ask.m_tag1 |= 0x30;
	else if(ktypeMin15 == nKType)
		ask.m_tag1 |= 0x40;
	else if(ktypeMin30 == nKType)
		ask.m_tag1 |= 0x50;
	else if(ktypeMin60 == nKType)
		ask.m_tag1 |= 0x60;
	else if(ktypeDay == nKType)
	{
		ask.m_tag1 |= 0x10;
		//ask.m_size = 0xfeac;
	}
	else if(ktypeWeek == nKType)
		ask.m_tag1 |= 0x80;
	else if(ktypeMonth == nKType)
		ask.m_tag1 |= 0x90;

	if (bChangeStock)
		ask.m_tag1 |= 0x03;
	else if(bFirstRequest)
		ask.m_tag1 |= 0x01;
	else
		ask.m_tag1 |= 0x02;

	if (ktypeDay == nKType && bChangeStock)
		ask.m_tag1 = 0x20;

	int ret = (6 + sizeof(ask.m_header) + sizeof(TW_STOCK));
	ConstructLength(ask.m_header, ret - sizeof(ask.m_header));

	return ret;
}

int ConstructAskMultisortBuffer(TW_ASK& ask, TW_STOCK* pStock)
{
	memset(&ask, 0, sizeof(ask));
	ask.m_header.m_magic = TW_MAGIC;

	BYTE	type = typeshA;
	if(pStock)
		type = pStock->m_type;

	TW_STOCK	stock;
	memset(&stock, 0, sizeof(stock));

	SHORT shortSize = 9;

	ask.m_tag1 = 0x0a;
	ask.m_tag2 = 0x08;
	ask.m_size = - shortSize;
	ask.m_stocks[0].m_type = type;
	ask.m_stocks[0].m_code[0] = 0x06;
	ask.m_stocks[0].m_code[1] = 0x00;
	ask.m_stocks[0].m_code[2] = (char)0xff;
	ask.m_stocks[0].m_code[3] = 0x01;
	ask.m_stocks[0].m_code[4] = 0x00;
	ask.m_stocks[0].m_code[5] = 0x00;
	ask.m_serial = GetNextSerial(stock);
	int ret = (6 + sizeof(ask.m_header) + sizeof(TW_STOCK));
	ConstructLength(ask.m_header, ret - sizeof(ask.m_header));
	return ret;
}

int ConstructAskDetailBuffer(TW_ASK& ask, TW_STOCK* pStock)
{
	memset(&ask, 0, sizeof(ask));
	ask.m_header.m_magic = TW_MAGIC;

	ask.m_tag1 = 0x01;
	ask.m_tag2 = 0x02;
	ask.m_size = 0x0001;
	memcpy(&(ask.m_stocks[0]), pStock, sizeof(TW_STOCK));
	ask.m_serial = GetNextSerial(ask.m_stocks[0]);
	int ret = (6 + sizeof(ask.m_header) + sizeof(TW_STOCK));
	ConstructLength(ask.m_header, ret - sizeof(ask.m_header));
	return ret;
}

int ConstructAskBaseBuffer(TW_ASK& ask, TW_STOCK* pStock)
{
	memset(&ask, 0, sizeof(ask));
	ask.m_header.m_magic = TW_MAGIC;

	ask.m_tag1 = 0x05;
	ask.m_tag2 = 0x0b;
	ask.m_size = 0x0005;
	memcpy(&(ask.m_stocks[0]), pStock, sizeof(TW_STOCK));
	ask.m_serial = GetNextSerial(ask.m_stocks[0]);
	int ret = (6 + sizeof(ask.m_header) + 5*sizeof(TW_STOCK));
	ConstructLength(ask.m_header, ret - sizeof(ask.m_header));
	return ret;
}

size_t TryGetLength(TW_HEADER& header)
{
	if(TW_MAGIC != header.m_magic || 0 != header.m_end)
		return 0;
	int len = (int)strtol(header.m_length,NULL,16);
	if(len < 0)
		return 0;
	return (size_t)len;
}

size_t TryGetLength(BYTE* buffer, size_t len)
{
	if(len < sizeof(TW_HEADER))
		return 0;
	TW_HEADER * pheader = (TW_HEADER*)buffer;
	return TryGetLength(*pheader);
}

BOOL IsLoginOK(BYTE* buffer, size_t len)
{
	if(len < sizeof(TW_ANS))
		return FALSE;
	TW_ANS * pans = (TW_ANS *)buffer;
	if(TW_MAGIC != pans->m_header.m_magic)
		return FALSE;
	if(0x00 != pans->m_tag1 || 0x00 != pans->m_tag2)
		return FALSE;
	return TRUE;
}

int TryGetInit(BYTE* buffer, size_t len, PRCV_DATA pRCV_DATA)
{
	if(len < sizeof(TW_ANS))
		return 0;
	TW_ANS * pans = (TW_ANS*)buffer;
	if(TW_MAGIC != pans->m_header.m_magic)
		return 0;
	if(0x02 != pans->m_tag1 || 0x01 != pans->m_tag2)
		return 0;

	// get it, fill pRCV_DATA
	size_t dataoffset = 469;
	int datalen = len - dataoffset;

	CSPTime tLatest(CSPTime::GetLatestTradeTime(time(NULL)));
	if(len > 128)
	{
		DWORD date = (*(DWORD*)(buffer+45));
		CSPTime sptime;
		if(sptime.FromStockTimeDay(date))
			tLatest = CSPTime(sptime.GetYear(),sptime.GetMonth(),sptime.GetDay(),9,15,0).GetTime();
	}
	if(pRCV_DATA && datalen >= (int)sizeof(TW_ANS_INIT))
	{
		// 每组28个字节, 共XXXX组,最后以0xfdfdfdfd为结尾
		TW_ANS_INIT * pinit = (TW_ANS_INIT*)(buffer+dataoffset);
		int packetsize = datalen/(int)sizeof(TW_ANS_INIT);

		memset(pRCV_DATA, 0, sizeof(RCV_DATA));
		pRCV_DATA->m_wDataType	=	RCV_REPORT;	// no use
		pRCV_DATA->m_nPacketNum	=	packetsize;
		pRCV_DATA->m_bDISK		=	FALSE;
		pRCV_DATA->m_pReport	=	new RCV_REPORT_STRUCTEx[packetsize];
		memset(pRCV_DATA->m_pReport, 0, sizeof(RCV_REPORT_STRUCTEx)*packetsize);

		for(int i=0;i <packetsize; i++)
		{
			if(pinit[i].m_tag != TW_MAGIC_BYTE)
				continue;
			pRCV_DATA->m_pReport[i].m_cbSize = sizeof(RCV_REPORT_STRUCTEx);
			pRCV_DATA->m_pReport[i].m_time = tLatest.GetTime();

			pRCV_DATA->m_pReport[i].m_wMarket = (pinit[i].m_type & 0x20) ? SZ_MARKET_EX : SH_MARKET_EX;
			pRCV_DATA->m_pReport[i].m_fLastClose = (float)(pinit[i].m_lastclose * 0.001);
			strncpy(pRCV_DATA->m_pReport[i].m_szLabel, pinit[i].m_code,
					min(sizeof(pRCV_DATA->m_pReport[i].m_szLabel),sizeof(pinit[i].m_code)));
			strncpy(pRCV_DATA->m_pReport[i].m_szName, pinit[i].m_name,
					min(sizeof(pRCV_DATA->m_pReport[i].m_szName),sizeof(pinit[i].m_name)));
			strncpy(pRCV_DATA->m_pReport[i].m_szName+sizeof(pinit[i].m_name)+2, pinit[i].m_shortname,
					min(sizeof(pRCV_DATA->m_pReport[i].m_szName)-sizeof(pinit[i].m_name)-2,sizeof(pinit[i].m_shortname)));
		}

		return len;
	}

	return 0;
}

int TryGetReport(BYTE* buffer, size_t len, PRCV_DATA pRCV_DATA)
{
	if(len < sizeof(TW_ANS))
		return 0;

	TW_ANS * pans = (TW_ANS*)buffer;
	if(TW_MAGIC != pans->m_header.m_magic)
		return 0;
	if(0x00 != pans->m_tag1 || 0x03 != pans->m_tag2)
		return 0;
	CTWStockArray astocks;
	BOOL bStockOK = GetSerialStock(pans->m_serial, astocks);

	// get it, fill pRCV_DATA
	size_t dataoffset = 43;
	int datalen = len - dataoffset;
	if(pRCV_DATA && bStockOK && astocks.GetSize() > 0 && datalen >= (int)sizeof(TW_ANS_REPORT))
	{
		TW_ANS_REPORT * preport = (TW_ANS_REPORT*)(buffer+dataoffset);

		memset(pRCV_DATA, 0, sizeof(RCV_DATA));
		pRCV_DATA->m_wDataType	=	RCV_REPORT;	// no use
		pRCV_DATA->m_nPacketNum	=	astocks.GetSize();
		pRCV_DATA->m_bDISK		=	FALSE;
		pRCV_DATA->m_pReport	=	new RCV_REPORT_STRUCTEx[astocks.GetSize()];
		memset(pRCV_DATA->m_pReport, 0, sizeof(RCV_REPORT_STRUCTEx)*astocks.GetSize());

		CSPTime tLatest = CTSCache::GetInstance().GetLocalLatest();
		for(int i=0; i<astocks.GetSize() && i<datalen/(int)sizeof(TW_ANS_REPORT); i++)
		{
			pRCV_DATA->m_pReport[i].m_cbSize = sizeof(RCV_REPORT_STRUCTEx);
			pRCV_DATA->m_pReport[i].m_time = tLatest.GetTime();

			pRCV_DATA->m_pReport[i].m_wMarket = (astocks[i].m_type & 0x20) ? SZ_MARKET_EX : SH_MARKET_EX;
			strncpy(pRCV_DATA->m_pReport[i].m_szLabel, astocks[i].m_code,
					min(sizeof(pRCV_DATA->m_pReport[i].m_szLabel),sizeof(astocks[i].m_code)));
	
			// unknown pRCV_DATA->m_pReport[i].m_fLastClose	= (float)(0.001*preport->m_open);
			pRCV_DATA->m_pReport[i].m_fOpen		= (float)(0.001*preport[i].m_open);
			pRCV_DATA->m_pReport[i].m_fHigh		= (float)(0.001*preport[i].m_high);
			pRCV_DATA->m_pReport[i].m_fLow		= (float)(0.001*preport[i].m_low);
			pRCV_DATA->m_pReport[i].m_fNewPrice	= (float)(0.001*preport[i].m_new);
			pRCV_DATA->m_pReport[i].m_fVolume	= (float)(0.01*preport[i].m_volume);
			pRCV_DATA->m_pReport[i].m_fAmount	= (float)(0.01*preport[i].m_amount);

			pRCV_DATA->m_pReport[i].m_fBuyPrice[0]= (float)(0.001*preport[i].m_buy1);
			pRCV_DATA->m_pReport[i].m_fBuyPrice[1]= (float)(0.001*preport[i].m_buy2);
			pRCV_DATA->m_pReport[i].m_fBuyPrice[2]= (float)(0.001*preport[i].m_buy3);
			pRCV_DATA->m_pReport[i].m_fBuyVolume[0]= (float)(0.01*preport[i].m_buy1vol);
			pRCV_DATA->m_pReport[i].m_fBuyVolume[1]= (float)(0.01*preport[i].m_buy2vol);
			pRCV_DATA->m_pReport[i].m_fBuyVolume[2]= (float)(0.01*preport[i].m_buy3vol);
			pRCV_DATA->m_pReport[i].m_fSellPrice[0]= (float)(0.001*preport[i].m_sell1);
			pRCV_DATA->m_pReport[i].m_fSellPrice[1]= (float)(0.001*preport[i].m_sell2);
			pRCV_DATA->m_pReport[i].m_fSellPrice[2]= (float)(0.001*preport[i].m_sell3);
			pRCV_DATA->m_pReport[i].m_fSellVolume[0]= (float)(0.01*preport[i].m_sell1vol);
			pRCV_DATA->m_pReport[i].m_fSellVolume[1]= (float)(0.01*preport[i].m_sell2vol);
			pRCV_DATA->m_pReport[i].m_fSellVolume[2]= (float)(0.01*preport[i].m_sell3vol);
		}

		return len;
	}

	return 0;
}

int TryGetMinute(BYTE* buffer, size_t len, PRCV_DATA pRCV_MINUTE, PRCV_DATA pRCV_REPORT)
{
	if(len < sizeof(TW_ANS))
		return 0;

	TW_ANS * pans = (TW_ANS*)buffer;
	if(TW_MAGIC != pans->m_header.m_magic)
		return 0;
	if(0x01 != pans->m_tag1 || 0x04 != pans->m_tag2)
		return 0;
	TW_STOCK	stock;
	memset(&stock, 0, sizeof(stock));
	BOOL bStockOK = GetSerialStock(pans->m_serial, stock);

	// get it, fill pRCV_DATA
	size_t dataoffset = 43;
	int datalen = len - dataoffset;
	if(pRCV_MINUTE && pRCV_REPORT && bStockOK && datalen >= (int)(sizeof(TW_ANS_MINUTE)-240*sizeof(TW_MINUTE)))
	{
		TW_ANS_MINUTE * pminute = (TW_ANS_MINUTE*)(buffer+dataoffset);

		CSPTime tLatest = CTSCache::GetInstance().GetLocalLatest();

		int nCount = min(241,pminute->m_offset+1);

		int count1 = 0, count2 = 0;
		for(int k=0; k<nCount; k++)
		{
			if(pminute->m_minutes[k].m_data1 <= pminute->m_high && pminute->m_minutes[k].m_data1 >= pminute->m_low)
				count1 ++;
			if(pminute->m_minutes[k].m_data2 <= pminute->m_high && pminute->m_minutes[k].m_data2 >= pminute->m_low)
				count2 ++;
		}
		int mode = (count1 > count2 ? 1 : 2);

		// minute
		memset(pRCV_MINUTE, 0, sizeof(RCV_DATA));
		pRCV_MINUTE->m_wDataType	=	FILE_MINUTE_EX;
		pRCV_MINUTE->m_nPacketNum	=	1+nCount;
		pRCV_MINUTE->m_bDISK		=	FALSE;
		pRCV_MINUTE->m_pMinute		=	new RCV_MINUTE_STRUCTEx[1+nCount];
		memset(pRCV_MINUTE->m_pMinute, 0, sizeof(RCV_MINUTE_STRUCTEx)*(1+nCount));
		pRCV_MINUTE->m_pMinute[0].m_head.m_dwHeadTag	=	EKE_HEAD_TAG;
		pRCV_MINUTE->m_pMinute[0].m_head.m_wMarket		=	(stock.m_type & 0x20) ? SZ_MARKET_EX : SH_MARKET_EX;
		strncpy(pRCV_MINUTE->m_pMinute[0].m_head.m_szLabel, stock.m_code,
				min(sizeof(pRCV_MINUTE->m_pMinute[0].m_head.m_szLabel),sizeof(stock.m_code)));
		
		for(int i=0; i<nCount; i++)
		{
			pRCV_MINUTE->m_pMinute[i+1].m_time = CSPTime::GetTradeOffsetToTime(i,tLatest.GetTime());
			if(1 == mode)
			{
				pRCV_MINUTE->m_pMinute[i+1].m_fPrice = (float)(0.001 * pminute->m_minutes[i].m_data1);
				pRCV_MINUTE->m_pMinute[i+1].m_fVolume = (float)(0.01 * pminute->m_minutes[i].m_data2);
			}
			else
			{
				pRCV_MINUTE->m_pMinute[i+1].m_fPrice = (float)(0.001 * pminute->m_minutes[i].m_data2);
				pRCV_MINUTE->m_pMinute[i+1].m_fVolume = (float)(0.01 * pminute->m_minutes[i].m_data1);
			}
			pRCV_MINUTE->m_pMinute[i+1].m_fAmount = (float)(100. * pRCV_MINUTE->m_pMinute[i+1].m_fVolume * pRCV_MINUTE->m_pMinute[i+1].m_fPrice);
		}

		// report
		memset(pRCV_REPORT, 0, sizeof(RCV_DATA));
		pRCV_REPORT->m_wDataType	=	RCV_REPORT;	// no use
		pRCV_REPORT->m_nPacketNum	=	11;
		pRCV_REPORT->m_bDISK		=	FALSE;
		pRCV_REPORT->m_pReport		=	new RCV_REPORT_STRUCTEx[11];
		memset(pRCV_REPORT->m_pReport, 0, sizeof(RCV_REPORT_STRUCTEx)*11);
		for(int i=0;i <11; i++)
		{
			pRCV_REPORT->m_pReport[i].m_cbSize = sizeof(RCV_REPORT_STRUCTEx);
			pRCV_REPORT->m_pReport[i].m_time = CSPTime::GetTradeOffsetToTime(pminute->m_details[i].m_offset,tLatest.GetTime());

			pRCV_REPORT->m_pReport[i].m_wMarket = (stock.m_type & 0x20) ? SZ_MARKET_EX : SH_MARKET_EX;
			strncpy(pRCV_REPORT->m_pReport[i].m_szLabel, stock.m_code,
					min(sizeof(pRCV_REPORT->m_pReport[i].m_szLabel),sizeof(stock.m_code)));
	
			// unknown pRCV_REPORT->m_pReport[i].m_fLastClose	= (float)(0.001*pminute->m_open);
			pRCV_REPORT->m_pReport[i].m_fOpen		= (float)(0.001*pminute->m_open);
			pRCV_REPORT->m_pReport[i].m_fHigh		= (float)(0.001*pminute->m_high);
			pRCV_REPORT->m_pReport[i].m_fLow		= (float)(0.001*pminute->m_low);
			pRCV_REPORT->m_pReport[i].m_fNewPrice	= (float)(0.001*pminute->m_details[i].m_price);
			pRCV_REPORT->m_pReport[i].m_fVolume		= (float)(0.01*pminute->m_details[i].m_volume);
			pRCV_REPORT->m_pReport[i].m_fAmount		= (float)(100.*pRCV_REPORT->m_pReport[i].m_fVolume*pRCV_REPORT->m_pReport[i].m_fNewPrice);

			pRCV_REPORT->m_pReport[i].m_fBuyPrice[0]= (float)(0.001*pminute->m_details[i].m_buy);
			if(10==i)	pRCV_REPORT->m_pReport[i].m_fBuyPrice[1]= (float)(0.001*pminute->m_buy2);
			if(10==i)	pRCV_REPORT->m_pReport[i].m_fBuyPrice[2]= (float)(0.001*pminute->m_buy3);
			if(10==i)	pRCV_REPORT->m_pReport[i].m_fBuyVolume[0]= (float)(0.01*pminute->m_buy1vol);
			if(10==i)	pRCV_REPORT->m_pReport[i].m_fBuyVolume[1]= (float)(0.01*pminute->m_buy2vol);
			if(10==i)	pRCV_REPORT->m_pReport[i].m_fBuyVolume[2]= (float)(0.01*pminute->m_buy3vol);
			pRCV_REPORT->m_pReport[i].m_fSellPrice[0]= (float)(0.001*pminute->m_details[i].m_sell);
			if(10==i)	pRCV_REPORT->m_pReport[i].m_fSellPrice[1]= (float)(0.001*pminute->m_sell2);
			if(10==i)	pRCV_REPORT->m_pReport[i].m_fSellPrice[2]= (float)(0.001*pminute->m_sell3);
			if(10==i)	pRCV_REPORT->m_pReport[i].m_fSellVolume[0]= (float)(0.01*pminute->m_sell1vol);
			if(10==i)	pRCV_REPORT->m_pReport[i].m_fSellVolume[1]= (float)(0.01*pminute->m_sell2vol);
			if(10==i)	pRCV_REPORT->m_pReport[i].m_fSellVolume[2]= (float)(0.01*pminute->m_sell3vol);
		}

		return len;
	}

	return 0;
}

int TryGetHistory(BYTE* buffer, size_t len, PRCV_DATA pRCV_DATA)
{
	if(len < sizeof(TW_ANS))
		return 0;

	TW_ANS * pans = (TW_ANS*)buffer;
	if(TW_MAGIC != pans->m_header.m_magic)
		return 0;
	if(0x09 != pans->m_tag2)
		return 0;
	TW_STOCK	stock;
	memset(&stock, 0, sizeof(stock));
	int	nCount = 0;
	BOOL bStockOK = GetSerialStock(pans->m_serial, stock, &nCount);
	if(1 == nCount)	nCount = 2;

	int ktype = ktypeDay;
	if(0x20 == pans->m_tag1)
		ktype = ktypeDay;
	else if(0x30 == (0xf0 & pans->m_tag1))
		ktype = ktypeMin5;
	else if(0x40 == (0xf0 & pans->m_tag1))
		ktype = ktypeMin15;
	else if(0x50 == (0xf0 & pans->m_tag1))
		ktype = ktypeMin30;
	else if(0x60 == (0xf0 & pans->m_tag1))
		ktype = ktypeMin60;
	else if(0x10 == (0xf0 & pans->m_tag1))
		ktype = ktypeDay;
	else if(0x80 == (0xf0 & pans->m_tag1))
		ktype = ktypeWeek;
	else if(0x90 == (0xf0 & pans->m_tag1))
		ktype = ktypeMonth;

	// get it, fill pRCV_DATA
	size_t dataoffset = 43;
	int datalen = len - dataoffset;
	if(pRCV_DATA && bStockOK && nCount > 0 && datalen >= nCount*(int)(sizeof(TW_ANS_HISTORY)+sizeof(DWORD)))
	{
		int dataprefix = datalen - nCount*(int)(sizeof(TW_ANS_HISTORY)+sizeof(DWORD));
		if(2 == nCount && datalen >= nCount*(int)(sizeof(TW_ANS_HISTORY)+sizeof(DWORD))+36)
		{
			dataprefix = datalen - nCount*(int)(sizeof(TW_ANS_HISTORY)+sizeof(DWORD)) - 36;
		}

		TW_ANS_HISTORY * phistory = (TW_ANS_HISTORY*)(buffer+dataoffset+dataprefix);

		CSPTime tLatest = CTSCache::GetInstance().GetLocalLatest();

		// minute
		memset(pRCV_DATA, 0, sizeof(RCV_DATA));
		pRCV_DATA->m_wDataType	=	FILE_HISTORY_EX;
		pRCV_DATA->m_nPacketNum	=	1 + nCount;
		pRCV_DATA->m_bDISK		=	FALSE;
		pRCV_DATA->m_pDay		=	new RCV_HISTORY_STRUCTEx[pRCV_DATA->m_nPacketNum+1];
		memset(pRCV_DATA->m_pDay, 0, sizeof(RCV_HISTORY_STRUCTEx)*(pRCV_DATA->m_nPacketNum+1));

		pRCV_DATA->m_pDay[0].m_head.m_dwHeadTag	=	EKE_HEAD_TAG;
		pRCV_DATA->m_pDay[0].m_head.m_wMarket	=	(stock.m_type & 0x20) ? SZ_MARKET_EX : SH_MARKET_EX;
		strncpy(pRCV_DATA->m_pDay[0].m_head.m_szLabel, stock.m_code,
				min(sizeof(pRCV_DATA->m_pDay[0].m_head.m_szLabel),sizeof(stock.m_code)));
		
		for(int i=0; i<nCount; i++)
		{
			CSPTime	sptime;
			if(sptime.FromStockTime(phistory[i].m_date, ktype, tLatest.GetYear()))
				pRCV_DATA->m_pDay[i+1].m_time = sptime.GetTime();
			else
				return 0;
			pRCV_DATA->m_pDay[i+1].m_fOpen = (float)(0.001 * phistory[i].m_open);
			pRCV_DATA->m_pDay[i+1].m_fHigh = (float)(0.001 * phistory[i].m_high);
			pRCV_DATA->m_pDay[i+1].m_fLow = (float)(0.001 * phistory[i].m_low);
			pRCV_DATA->m_pDay[i+1].m_fClose = (float)(0.001 * phistory[i].m_close);
			pRCV_DATA->m_pDay[i+1].m_fVolume = (float)(phistory[i].m_volume);
			pRCV_DATA->m_pDay[i+1].m_fAmount = (float)(1000. * ((DWORD*)&(phistory[nCount]))[i]);
		}

		return len;
	}

	return 0;
}

int TryGetMultisort(BYTE* buffer, size_t len, PRCV_DATA pRCV_DATA)
{
	if(len < sizeof(TW_ANS))
		return 0;

	TW_ANS * pans = (TW_ANS*)buffer;
	if(TW_MAGIC != pans->m_header.m_magic)
		return 0;
	if(0x0a != pans->m_tag1 || 0x08 != pans->m_tag2)
		return 0;

	// get it, fill pRCV_DATA
	size_t dataoffset = 41;
	int datalen = len - dataoffset;
	if(pRCV_DATA && datalen >= (int)sizeof(TW_ANS_MULTISORT))
	{
		TW_ANS_MULTISORT * pmultisort = (TW_ANS_MULTISORT*)(buffer+dataoffset);

		if(0x0006 == pmultisort->m_tag1 || datalen == sizeof(TW_ANS_MULTISORT))
		{
			memset(pRCV_DATA, 0, sizeof(RCV_DATA));
			pRCV_DATA->m_wDataType	=	FILE_MULTISORT_EX;	// no use
			pRCV_DATA->m_nPacketNum	=	1;
			pRCV_DATA->m_bDISK		=	FALSE;
			pRCV_DATA->m_pMultisort	=	new RCV_MULTISORT_STRUCTEx[1];
			memset(pRCV_DATA->m_pMultisort, 0, sizeof(RCV_MULTISORT_STRUCTEx)*1);
			memcpy(pRCV_DATA->m_pMultisort, pmultisort, sizeof(TW_ANS_MULTISORT));
			return len;
		}
	}

	return 0;
}

int TryGetDetail(BYTE* buffer, size_t len, PRCV_DATA pRCV_DATA)
{
	if(len < sizeof(TW_ANS))
		return 0;

	TW_ANS * pans = (TW_ANS*)buffer;
	if(TW_MAGIC != pans->m_header.m_magic)
		return 0;
	if(0x01 != pans->m_tag1 || 0x02 != pans->m_tag2)
		return 0;
	TW_STOCK	stock;
	memset(&stock, 0, sizeof(stock));
	BOOL bStockOK = GetSerialStock(pans->m_serial, stock);

	// get it, fill pRCV_DATA
	size_t dataoffset = 43;
	int datalen = len - dataoffset;
	if(pRCV_DATA && bStockOK && datalen >= (int)sizeof(TW_ANS_DETAIL))
	{
		TW_ANS_DETAIL * pdetail = (TW_ANS_DETAIL*)(buffer+dataoffset);
		int packetsize = datalen/(int)sizeof(TW_ANS_DETAIL);

		memset(pRCV_DATA, 0, sizeof(RCV_DATA));
		pRCV_DATA->m_wDataType	=	RCV_REPORT;	// no use
		pRCV_DATA->m_nPacketNum	=	packetsize;
		pRCV_DATA->m_bDISK		=	FALSE;
		pRCV_DATA->m_pReport	=	new RCV_REPORT_STRUCTEx[packetsize];
		memset(pRCV_DATA->m_pReport, 0, sizeof(RCV_REPORT_STRUCTEx)*packetsize);

		CSPTime tLatest = CTSCache::GetInstance().GetLocalLatest();

		for(int i=0;i <packetsize; i++)
		{
			pRCV_DATA->m_pReport[i].m_cbSize = sizeof(RCV_REPORT_STRUCTEx);
			pRCV_DATA->m_pReport[i].m_time = CSPTime::GetTradeOffsetToTime(pdetail[i].m_offset, tLatest.GetTime());

			pRCV_DATA->m_pReport[i].m_wMarket = (stock.m_type & 0x20) ? SZ_MARKET_EX : SH_MARKET_EX;
			strncpy(pRCV_DATA->m_pReport[i].m_szLabel, stock.m_code,
					min(sizeof(pRCV_DATA->m_pReport[i].m_szLabel),sizeof(stock.m_code)));

			pRCV_DATA->m_pReport[i].m_fNewPrice	= (float)(0.001*pdetail[i].m_price);
			pRCV_DATA->m_pReport[i].m_fVolume		= (float)(0.01*pdetail[i].m_volume);
			pRCV_DATA->m_pReport[i].m_fAmount		= (float)(100.*pRCV_DATA->m_pReport[i].m_fVolume*pRCV_DATA->m_pReport[i].m_fNewPrice);

			pRCV_DATA->m_pReport[i].m_fBuyPrice[0]= (float)(0.001*pdetail[i].m_buy1);
			pRCV_DATA->m_pReport[i].m_fSellPrice[0]= (float)(0.001*pdetail[i].m_sell1);
		}

		return len;
	}

	return 0;
}

int TryGetBase(BYTE* buffer, size_t len, PRCV_DATA pRCV_DATA)
{
	if(len < sizeof(TW_ANS))
		return 0;

	TW_ANS * pans = (TW_ANS*)buffer;
	if(TW_MAGIC != pans->m_header.m_magic)
		return 0;
	if(0x05 != pans->m_tag1 || 0x0b != pans->m_tag2)
		return 0;
	TW_STOCK	stock;
	memset(&stock, 0, sizeof(stock));
	BOOL bStockOK = GetSerialStock(pans->m_serial, stock);

	// get it, fill pRCV_DATA
	size_t dataoffset = 45;
	int datalen = len - dataoffset;
	if(pRCV_DATA && bStockOK && datalen > 0)
	{
		memset(pRCV_DATA, 0, sizeof(RCV_DATA));
		pRCV_DATA->m_wDataType	=	FILE_BASE_EX;
		pRCV_DATA->m_nPacketNum	=	1;
		pRCV_DATA->m_bDISK		=	FALSE;

		pRCV_DATA->m_File.m_dwLen	=	datalen;
		strncpy(pRCV_DATA->m_File.m_szFileName, stock.m_code, min(sizeof(stock.m_code),sizeof(pRCV_DATA->m_File.m_szFileName)-1));
		if(strlen(pRCV_DATA->m_File.m_szFileName)+5 < sizeof(pRCV_DATA->m_File.m_szFileName))
			strcat(pRCV_DATA->m_File.m_szFileName, ".txt");
		pRCV_DATA->m_pData		=	new BYTE[datalen+1];
		memcpy(pRCV_DATA->m_pData, buffer + dataoffset, datalen);
		((char*)pRCV_DATA->m_pData)[datalen] = 0;
		return len;
	}

	return 0;
}
