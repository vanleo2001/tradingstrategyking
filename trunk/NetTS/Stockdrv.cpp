
#include "StdAfx.h"
#include "TSWnd.h"
#include "TSCache.h"
#include "TWSocket.h"
#include "Stockdrv.h"
#include "SelectSrvDlg.h"

int WINAPI	Stock_Init(HWND hWnd,UINT Msg,int nWorkMode)
{
	CTSWnd::GetInstance().AddWnd( hWnd, Msg );
	if( !CTWSocket::GetInstance().IsWorking() )
	{
		AfxGetQSProfile().LoadProfile();

		CSelectSrvDlg	dlg;
		if( IDOK != dlg.DoModal() )
			return 1;
		else
			return -1;
	}
	return 1;
}

int WINAPI	Stock_Init_Nodialog(HWND hWnd,UINT Msg,int nWorkMode,
		const char * szAddress, UINT nPort, const char * szUser, const char * szPasswd )
{
	CTSWnd::GetInstance().AddWnd( hWnd, Msg );
	if( !CTWSocket::GetInstance().IsWorking() )
	{
		AfxGetQSProfile().LoadProfile();

		BOOL bSuccess = CTWSocket::GetInstance().BeginWorking( AfxGetQSProfile().GetAlias(szAddress),
					nPort, szUser, szPasswd );
		if( !bSuccess )
			AfxMessageBox( CTWSocket::GetInstance().GetLastErrorString(), MB_OK );
		return bSuccess ? 1 : -1;
	}
	return 1;
}

int WINAPI 	Stock_Quit(HWND hWnd)
{
	CTSWnd::GetInstance().RemoveWnd( hWnd );
	if( CTSWnd::GetInstance().IsEmpty() && CTWSocket::GetInstance().IsWorking() )
	{
		CTSCache::GetInstance().ClearReceiveBuffer();
		CTWSocket::GetInstance().EndWorking();
	}
	return 1;
}

int WINAPI 	GetTotalNumber()
{
	return CTSCache::GetInstance().GetTotalNumber();
}

int WINAPI 	GetStockByNoEx(int nNo,RCV_REPORT_STRUCTEx * pBuf)
{
	return ( CTSCache::GetInstance().GetStockByNoEx( nNo, pBuf ) ? 1 : -1 );
}

int WINAPI 	GetStockByCodeEx(char * pszStockCode,int nMarket,RCV_REPORT_STRUCTEx * pBuf)
{
	return ( CTSCache::GetInstance().GetStockByCodeEx( pszStockCode, nMarket, pBuf ) ? 1 : -1 );
}

int	WINAPI  SetupReceiver(BOOL bShowWindow)
{
	if( bShowWindow )
	{
		CSelectSrvDlg	dlg;
		if( IDOK != dlg.DoModal() )
			return -1;
		CTWSocket::GetInstance().EndWorking();
		return ( CTWSocket::GetInstance().BeginWorking( dlg.GetServer(), dlg.GetPort(),
					dlg.GetUser(), dlg.GetPasswd() ) ? 1 : -1 );
	}
	if( !CTWSocket::GetInstance().IsWorking() )
		return ( CTWSocket::GetInstance().BeginWorking( ) ? 1 : -1 );
	return TRUE;
}

DWORD WINAPI  GetStockDrvInfo(int nInfo,void * pBuf)
{
	switch( nInfo )
	{
	case RI_IDSTRING:
		strcpy( (char*)pBuf, "Ninebulles_NET_Tongshi_3.00" );
		break;
	case RI_IDCODE:
		strcpy( (char*)pBuf, "88888888" );
		break;
	case RI_VERSION:
		strcpy( (char*)pBuf, "3.00" );
		break;
	case RI_ERRRATE:
		strcpy( (char*)pBuf, "0" );
		break;
	case RI_STKNUM:
		return GetTotalNumber();
	case RI_SUPPORTEXTHQ:
		return -1;
	case RI_ENABLEDZHTRANS:
		return -1;
	case RI_ENABLETS3FILE:
		return -1;
	default:
		return -1;
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////
// The new interfaces
int WINAPI	IsEngineWorking( )
{
	return CTWSocket::GetInstance().IsWorking();
}

int WINAPI	SetAutoReport( int bAutoReport )
{
	return CTWSocket::GetInstance().SetAutoReport( bAutoReport );
}

int WINAPI	RequestStockData( int nDataType, TW_STOCK * pStocks, int nSize, int nKType, int nDataCount )
{
	return CTWSocket::GetInstance().RequestStockData( nDataType, pStocks, nSize, nKType, nDataCount );
}

