
#pragma once

#include "STKDRV.h"

class CSTKDRVContainer  
{
public:
	CSTKDRVContainer();
	virtual ~CSTKDRVContainer();

protected:
	CArray<CSTKDRV, CSTKDRV&>	m_aDriver;

public:
	int		LoadDriver(CStringArray& astrDriverPath);
	void	UnloadDriver();

public:
	int		Stock_Init(HWND hWnd,UINT uMsg,int nWorkMode);
	int		Stock_Quit(HWND hWnd);
	int		GetTotalNumber();
	int		GetStockByNoEx(int nNo, RCV_REPORT_STRUCTEx* pBuf);
	int		GetStockByCodeEx(char* pszStockCode, int nMarket, RCV_REPORT_STRUCTEx* pBuf);
	int		SetupReceiver(BOOL bShowWindow);
	DWORD	GetStockDrvInfo(int nInfo, void* pBuf);

public:
	int		GetStockMin(char * StockCode, int nMarket);
	int		GetStockPRP(char * StockCode, int nMarket);

public:
	BOOL	SCInit(void* pInit, int nSize);
	void	SCQuit();
	UINT	SCGetInfo(TSCInfo Info, void* pData, int* nSize);
	void	SCAskData(void* pAsk, int nSize);
	BOOL	SCStockInit(HWND hWnd, UINT Msg, int nWorkMode);
};
