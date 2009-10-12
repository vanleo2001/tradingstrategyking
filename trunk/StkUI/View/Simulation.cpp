// Simulation.cpp: implementation of the CSimulation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Simulation.h"
#include "../Dialog/DownloadDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CSimulation

HANDLE CSimulation::m_hEventKillSimulationThread	=	NULL;
HANDLE CSimulation::m_hEventSimulationThreadKilled	=	NULL;

/***
	策略模拟进度回调函数
*/
BOOL CALLBACK SimulationCallback(DWORD dwType, DWORD dwValue,
							LPCTSTR lpszMsg, void *cookie)
{
	LPSIMULATION_INFO pInfo=(LPSIMULATION_INFO)cookie;

	if( SIMULATION_PROGRESS == dwType )
	{
		::SendMessage( pInfo->hMainWnd, WM_USER_SIMULATION_PROGRESS, dwValue, (LPARAM)lpszMsg );
	}
	else if( SIMULATION_YIELD == dwType )
	{
		::SendMessage( pInfo->hMainWnd, WM_USER_SIMULATION_YIELD, 0, dwValue );
	}

	// User wants to quit program
	if(WAIT_OBJECT_0 == WaitForSingleObject(CSimulation::m_hEventKillSimulationThread,0) )
	{
		return FALSE;
	}
	return TRUE;
}

/***
	策略模拟线程主函数
*/
UINT SimulationMain(LPVOID pParam)
{	
	LPSIMULATION_INFO pInfo=(LPSIMULATION_INFO)pParam;
	
	::PostMessage( pInfo->hMainWnd, WM_USER_SIMULATION_PROGRESS, 0, 0 );

	ASSERT( pInfo->pStrategy );

	// Run
	pInfo->pStrategy->ClearCache( );
	BOOL bFinished = pInfo->pStrategy->SimuRun( SimulationCallback, pInfo );

	// End OK
	::PostMessage( pInfo->hMainWnd, WM_USER_SIMULATION_END, bFinished, (LPARAM)(pInfo->pStrategy) );
	SetEvent(CSimulation::m_hEventSimulationThreadKilled);
	AfxEndThread( 0, TRUE );
	return 0;
}

CSimulation::CSimulation()
{
	m_pSimulationInfo	=	NULL;
	m_pStrategy			=	NULL;
	m_hMainWnd			=	NULL;
	m_bStopAndReset		=	FALSE;
}

CSimulation::~CSimulation()
{
	if( m_pSimulationInfo )
	{
		delete	m_pSimulationInfo;
		m_pSimulationInfo	=	NULL;
	}
}

/***
	设定模拟策略
*/
void CSimulation::SetStrategy( CStrategy * pStrategy, HWND hMainWnd)
{
	m_pStrategy	=	pStrategy;
	m_hMainWnd	=	hMainWnd;

	if( m_pStrategy )
		m_pStrategy->ClearCache();
}

/***
	如果模拟备选股票的数据不够用，下载之
*/
BOOL CSimulation::DownloadDataIfNeed( )
{
	if( NULL == m_pStrategy )
		return FALSE;

	CSPTime	tmInitial, tmPioneer, tmLatest;
	CSPTime	tmBegin	=	m_pStrategy->GetOpParam().GetBeginTime();
	CSPTime	tmEnd	=	m_pStrategy->GetOpParam().GetEndTime();
	CSPTime	tmDLBegin = tmBegin, tmDLEnd = tmEnd;
	BOOL	bNeedDownload	=	FALSE;
	
	for( int i=0; i<m_pStrategy->GetStocks().GetSize(); i++ )
	{
		CSPTime	tmDLBeginLocal, tmDLEndLocal;
		CStockInfo	info;
		if( AfxGetStockContainer().GetStockInfo( m_pStrategy->GetStocks().ElementAt(i), &info )
			&& AfxGetDB().GetNeedDownloadRange( info, tmBegin, tmEnd, tmDLBeginLocal, tmDLEndLocal ) )
		{
			if( !bNeedDownload )
			{
				tmDLBegin	=	tmDLBeginLocal;
				tmDLEnd		=	tmDLEndLocal;
			}
			bNeedDownload	=	TRUE;
			if( tmDLBeginLocal < tmDLBegin )
				tmDLBegin	=	tmDLBeginLocal;
			if( tmDLEndLocal > tmDLEnd )
				tmDLEnd		=	tmDLEndLocal;
		}
	}
	
	if( bNeedDownload )
	{
		if( IDOK != AfxMessageBox( IDS_SIMULATION_DOWNLOADNOW, MB_YESNO ) )
			return TRUE;
		CDownloadDlg	dlg;
		dlg.SetInitDownloadTime( tmDLBegin.GetTime(), tmDLEnd.GetTime(), TRUE );
		dlg.DoModal();
	}
	return TRUE;
}

/***
	重新开始模拟
*/
void CSimulation::Restart( )
{
	ASSERT( m_pStrategy && m_hMainWnd );
	m_bStopAndReset		=	FALSE;

	CSimulation::m_hEventKillSimulationThread = CreateEvent(NULL,FALSE,FALSE,NULL);
	CSimulation::m_hEventSimulationThreadKilled = CreateEvent(NULL,FALSE,FALSE,NULL);

	if( NULL == m_pSimulationInfo )
		m_pSimulationInfo	=	new SIMULATION_INFO;
	memset( m_pSimulationInfo, 0, sizeof(SIMULATION_INFO) );
	m_pSimulationInfo->hMainWnd	=	m_hMainWnd;
	m_pSimulationInfo->pStrategy	=	m_pStrategy;

	if( NULL == m_pStrategy )
		return;

	if( !DownloadDataIfNeed( ) )
		return;

	m_pStrategy->ClearCache();
	m_pStrategy->SimuReset( );
	m_pStrategy->SimuSetStatusRunning();

	AfxBeginThread( SimulationMain, (LPVOID)m_pSimulationInfo,THREAD_PRIORITY_NORMAL);
}

/***
	暂停模拟
*/
void CSimulation::Pause()
{
	ASSERT( m_pStrategy && m_hMainWnd );
	m_bStopAndReset		=	FALSE;

	// TODO: Add your control notification handler code here
	if( CSimulation::m_hEventKillSimulationThread )
		SetEvent( CSimulation::m_hEventKillSimulationThread );
	// if( CSimulation::m_hEventSimulationThreadKilled )
	//		WaitForSingleObject( CSimulation::m_hEventSimulationThreadKilled, INFINITE );
	if( m_pStrategy )
		m_pStrategy->SimuSetStatusPaused();
}

/***
	继续模拟
*/
void CSimulation::Continue( )
{
	ASSERT( m_pStrategy && m_hMainWnd );
	m_bStopAndReset		=	FALSE;

	CSimulation::m_hEventKillSimulationThread = CreateEvent(NULL,FALSE,FALSE,NULL);
	CSimulation::m_hEventSimulationThreadKilled = CreateEvent(NULL,FALSE,FALSE,NULL);

	if( NULL == m_pSimulationInfo )
		m_pSimulationInfo	=	new SIMULATION_INFO;
	memset( m_pSimulationInfo, 0, sizeof(SIMULATION_INFO) );
	m_pSimulationInfo->hMainWnd	=	m_hMainWnd;
	m_pSimulationInfo->pStrategy	=	m_pStrategy;

	if( m_pStrategy )
	{
		m_pStrategy->SimuSetStatusRunning();
		AfxBeginThread( SimulationMain, (LPVOID)m_pSimulationInfo,THREAD_PRIORITY_NORMAL);
	}
}

/***
	停止模拟并清除结果
*/
void CSimulation::Stop()
{
	ASSERT( m_pStrategy && m_hMainWnd );
	m_bStopAndReset	=	TRUE;

	// TODO: Add your control notification handler code here
	if( CSimulation::m_hEventKillSimulationThread )
		SetEvent( CSimulation::m_hEventKillSimulationThread );
	// if( CSimulation::m_hEventSimulationThreadKilled )
	//	WaitForSingleObject( CSimulation::m_hEventSimulationThreadKilled, INFINITE );
	if( m_pStrategy && m_pStrategy->SimuIsStatusPaused() )
		OnEnd( FALSE );
}

/***
	模拟暂停或者停止或者完成消息响应
*/
void CSimulation::OnEnd( BOOL bFinished )
{
	ASSERT( m_pStrategy && m_hMainWnd );

	if( CSimulation::m_hEventKillSimulationThread )
		CloseHandle(CSimulation::m_hEventKillSimulationThread);
	if( CSimulation::m_hEventSimulationThreadKilled )
		CloseHandle(CSimulation::m_hEventSimulationThreadKilled);
	CSimulation::m_hEventKillSimulationThread	=	NULL;
	CSimulation::m_hEventSimulationThreadKilled	=	NULL;

	if( bFinished && m_pStrategy )
	{
		m_pStrategy->SimuSetStatusFinished();
	}
	if( m_bStopAndReset && m_pStrategy )
	{
		m_pStrategy->SimuReset( );
		m_pStrategy->SimuSetStatusInit();
	}
	if( m_pStrategy )
	{
		m_pStrategy->DoFileSave( );
		m_pStrategy->ClearCache();
	}
}

