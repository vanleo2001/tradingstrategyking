// DownloadDlg.cpp : implementation file
//

#include "stdafx.h"

#include "DownloadDlg.h"
#include "NetInfoDlg.h"
#include "UpgradeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownloadDlg dialog

HANDLE CDownloadDlg::m_hEventKillDownloadThread		=	NULL;
HANDLE CDownloadDlg::m_hEventDownloadThreadKilled	=	NULL;


BOOL CALLBACK DownloadCallback(DWORD dwCode, DWORD dwProgress,
							LPCTSTR lpszMsg, void *cookie)
{
	HWND	hMainWnd	=	(HWND)cookie;

	if( PROG_PROGRESS == dwCode && ::IsWindow(hMainWnd) )
	{
		::SendMessage( hMainWnd, WM_USER_DOWNLOAD_PROGRESS, dwProgress, (LPARAM)lpszMsg );
	}
	else if( ::IsWindow(hMainWnd) )
	{
		::SendMessage( hMainWnd, WM_USER_DOWNLOAD_MESSAGE, dwCode, (LPARAM)lpszMsg );
	}

	// User wants to quit program
	if(WAIT_OBJECT_0 == WaitForSingleObject(CDownloadDlg::m_hEventKillDownloadThread,0) )
	{
		::PostMessage( hMainWnd, WM_USER_DOWNLOAD_END, FALSE, 0 );
		SetEvent(CDownloadDlg::m_hEventDownloadThreadKilled);
		AfxEndThread( 0, TRUE );
		return FALSE;
	}
	return TRUE;
}

UINT DownloadMain(LPVOID pParam)
{	
	LPDOWNLOAD_INFO pInfo=(LPDOWNLOAD_INFO)pParam;
	
	int nCountDay		=	0;
	int nCountBase		=	0;
	int nCountXDR		=	0;
	int nCountMin5		=	0;
	
	if( pInfo->bDay )
	{
		::SendMessage( pInfo->hMainWnd, WM_USER_DOWNLOAD_MESSAGE, PROG_NEWGROUP, IDS_DLMSG_DAY );
		::SendMessage( pInfo->hMainWnd, WM_USER_DOWNLOAD_PROGRESS, 0, NULL );
		CSPTime	tmFrom( pInfo->tmDayFrom );
		CSPTime	tmTo( pInfo->tmDayTo );
		tmFrom	=	CSPTime( tmFrom.GetYear(), tmFrom.GetMonth(), tmFrom.GetDay(), 0, 0, 0 );
		tmTo	=	CSPTime( tmTo.GetYear(), tmTo.GetMonth(), tmTo.GetDay(), 23, 59, 59 );
		nCountDay = pInfo->pNetDB->NetloadDayKLine( NULL, &tmFrom, &tmTo, DownloadCallback, pInfo->hMainWnd );
	}

	// User wants to quit program
	if(WAIT_OBJECT_0 == WaitForSingleObject(CDownloadDlg::m_hEventKillDownloadThread,0) )
	{
		::PostMessage( pInfo->hMainWnd, WM_USER_DOWNLOAD_END, FALSE, 0 );
		SetEvent(CDownloadDlg::m_hEventDownloadThreadKilled);
		AfxEndThread( 0, TRUE );
		return 0;
	}

	if( pInfo->bBase )
	{
		::SendMessage( pInfo->hMainWnd, WM_USER_DOWNLOAD_MESSAGE, PROG_NEWGROUP, IDS_DLMSG_BASE );
		::SendMessage( pInfo->hMainWnd, WM_USER_DOWNLOAD_PROGRESS, 0, NULL );
		nCountBase = pInfo->pNetDB->NetloadBasetable( NULL, NULL, NULL, DownloadCallback, pInfo->hMainWnd );
	}

	// User wants to quit program
	if(WAIT_OBJECT_0 == WaitForSingleObject(CDownloadDlg::m_hEventKillDownloadThread,0) )
	{
		::PostMessage( pInfo->hMainWnd, WM_USER_DOWNLOAD_END, FALSE, 0 );
		::SendMessage( pInfo->hMainWnd, WM_USER_DOWNLOAD_PROGRESS, 0, NULL );
		SetEvent(CDownloadDlg::m_hEventDownloadThreadKilled);
		AfxEndThread( 0, TRUE );
		return 0;
	}

	if( pInfo->bXDR )
	{
		::SendMessage( pInfo->hMainWnd, WM_USER_DOWNLOAD_MESSAGE, PROG_NEWGROUP, IDS_DLMSG_XDR );
		::SendMessage( pInfo->hMainWnd, WM_USER_DOWNLOAD_PROGRESS, 0, NULL );
		nCountXDR = pInfo->pNetDB->NetloadDRData( NULL, NULL, NULL, DownloadCallback, pInfo->hMainWnd );
	}

	// User wants to quit program
	if(WAIT_OBJECT_0 == WaitForSingleObject(CDownloadDlg::m_hEventKillDownloadThread,0) )
	{
		::PostMessage( pInfo->hMainWnd, WM_USER_DOWNLOAD_END, FALSE, 0 );
		SetEvent(CDownloadDlg::m_hEventDownloadThreadKilled);
		AfxEndThread( 0, TRUE );
		return 0;
	}

	if( pInfo->bMin5 )
	{
		::SendMessage( pInfo->hMainWnd, WM_USER_DOWNLOAD_MESSAGE, PROG_NEWGROUP, IDS_DLMSG_MIN5 );
		::SendMessage( pInfo->hMainWnd, WM_USER_DOWNLOAD_PROGRESS, 0, NULL );
		CSPTime	tmFrom( pInfo->tmMin5From );
		CSPTime	tmTo( pInfo->tmMin5To );
		tmFrom	=	CSPTime( tmFrom.GetYear(), tmFrom.GetMonth(), tmFrom.GetDay(), 0, 0, 0 );
		tmTo	=	CSPTime( tmTo.GetYear(), tmTo.GetMonth(), tmTo.GetDay(), 23, 59, 59 );
		nCountMin5 = pInfo->pNetDB->Netload5MinKLine( NULL, &tmFrom, &tmTo, DownloadCallback, pInfo->hMainWnd );
	}

	if( pInfo->bMin5 && nCountMin5 == 0 )
	{
		CString	strNote;
		strNote.LoadString( IDS_DLMIN5_NOTSUPPORT );
		::SendMessage( pInfo->hMainWnd, WM_USER_DOWNLOAD_END, TRUE, (LPARAM)strNote.GetBuffer(strNote.GetLength()+1) );
		strNote.ReleaseBuffer();
	}
	else if( pInfo->bBase && nCountBase == 0 )
	{
		CString	strNote;
		strNote.LoadString( IDS_DLBASE_NOTSUPPORT );
		::SendMessage( pInfo->hMainWnd, WM_USER_DOWNLOAD_END, TRUE, (LPARAM)strNote.GetBuffer(strNote.GetLength()+1) );
		strNote.ReleaseBuffer();
	}
	else
	{
		::SendMessage( pInfo->hMainWnd, WM_USER_DOWNLOAD_END, TRUE, 0 );
	}

	AfxEndThread( 0, TRUE );
	return 0;
}

CDownloadDlg::CDownloadDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CDownloadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDownloadDlg)
	m_tmDayFrom = 0;
	m_tmDayTo = 0;
	m_tmMin5From = 0;
	m_tmMin5To = 0;
	//}}AFX_DATA_INIT

	m_pDownloadInfo		=	NULL;
	m_bDBModified	=	FALSE;
	
	CSPTime	tmLatestOld	=	0;
	if( m_netdb.SetRootPath( AfxGetProfile().GetSelfDBPath(), IStStore::dbtypeSelfDB ) )
		m_netdb.GetTimeLocalRange( &tmLatestOld, NULL, NULL );
	m_tmLatestOld	=	tmLatestOld.GetTime();

	m_bInitDownloadTime	=	FALSE;
	m_bAutoRun			=	FALSE;

	m_bReportFinished = TRUE;
}

CDownloadDlg::~CDownloadDlg()
{
	if( m_pDownloadInfo )
	{
		delete	m_pDownloadInfo;
		m_pDownloadInfo	=	NULL;
	}
}

void CDownloadDlg::SetInitDownloadTime( CSPTime tmDLBegin, CSPTime tmDLEnd, BOOL bAutoRun )
{
	m_bInitDownloadTime	=	TRUE;
	m_tmDLBegin	=	tmDLBegin;
	m_tmDLEnd	=	tmDLEnd;
	m_bAutoRun	=	TRUE;
}

void CDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownloadDlg)
	DDX_Control(pDX, IDC_STATIC_LATEST, m_staticLatest);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_staticProgress);
	DDX_Control(pDX, IDC_CHECK_MIN5, m_btnCheckMin5);
	DDX_Control(pDX, IDC_CHECK_REPORT, m_btnCheckReport);
	DDX_Control(pDX, IDC_PROXY_SET, m_btnProxySet);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_staticStatus);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_STOP_REFRESH, m_btnStopRefresh);
	DDX_Control(pDX, IDC_STOP, m_btnStop);
	DDX_Control(pDX, IDC_REFRESH_SERVERS, m_btnRefreshServers);
	DDX_Control(pDX, IDC_CHECK_XDR, m_btnCheckXDR);
	DDX_Control(pDX, IDC_CHECK_DAY, m_btnCheckDay);
	DDX_Control(pDX, IDC_CHECK_BASE, m_btnCheckBase);
	DDX_Control(pDX, IDC_TIME_DAYTO, m_dtctrlDayTo);
	DDX_Control(pDX, IDC_TIME_DAYFROM, m_dtctrlDayFrom);
	DDX_Control(pDX, IDC_DL_PROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDC_STATIC_REFRESH, m_staticRefreshServer);
	DDX_Control(pDX, IDC_COMBO_SERVERS, m_comboServers);
	DDX_DateTimeCtrl(pDX, IDC_TIME_DAYFROM, m_tmDayFrom);
	DDX_DateTimeCtrl(pDX, IDC_TIME_DAYTO, m_tmDayTo);
	DDX_DateTimeCtrl(pDX, IDC_TIME_MIN5FROM, m_tmMin5From);
	DDX_DateTimeCtrl(pDX, IDC_TIME_MIN5TO, m_tmMin5To);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownloadDlg, CXFDialog)
	//{{AFX_MSG_MAP(CDownloadDlg)
	ON_BN_CLICKED(IDC_REFRESH_SERVERS, OnRefreshServers)
	ON_BN_CLICKED(IDC_STOP_REFRESH, OnStopRefresh)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_NOTIFY(NM_SETFOCUS, IDC_TIME_DAYFROM, OnSetfocusTimeDay)
	ON_BN_CLICKED(IDC_PROXY_SET, OnProxySet)
	ON_NOTIFY(NM_SETFOCUS, IDC_TIME_DAYTO, OnSetfocusTimeDay)
	ON_NOTIFY(NM_SETFOCUS, IDC_TIME_MIN5FROM, OnSetfocusTimeMin5)
	ON_NOTIFY(NM_SETFOCUS, IDC_TIME_MIN5TO, OnSetfocusTimeMin5)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_DOWNLOAD_MESSAGE, OnDownloadMessage)
	ON_MESSAGE(WM_USER_DOWNLOAD_PROGRESS, OnDownloadProgress)
	ON_MESSAGE(WM_USER_DOWNLOAD_END, OnDownloadEnd)
	ON_MESSAGE(WM_APP_STKRECEIVER_DATA, OnStkReceiverData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownloadDlg message handlers

BOOL CDownloadDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_staticRefreshServer.SetWindowPos( NULL, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW);
	m_staticStatus.SetWindowPos( NULL, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW);
	m_ctrlProgress.SetWindowPos( NULL, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW);

	m_btnStopRefresh.EnableWindow( FALSE );
	m_btnStop.EnableWindow( FALSE );

	InitComboServers();

	m_ctrlProgress.SetRange( 0, STKLIB_MAX_PROGRESS );
	m_ctrlProgress.SetPos( 0 );
	m_btnCheckReport.SetCheck( AfxGetStkReceiver().NetEngineIsWorking() );
	m_btnCheckReport.EnableWindow( AfxGetStkReceiver().NetEngineIsWorking() );
	m_btnCheckDay.SetCheck( TRUE );

	// Set Time
	CSPTime	tmCurrent	=	CSPTime::GetCurrentTime();
	if( tmCurrent.GetHour() < 15 )
		tmCurrent	-=	CSPTimeSpan(1,0,0,0);

	if( tmCurrent.GetMonth() == 1 && tmCurrent.GetDay() == 1 )
		tmCurrent	-=	CSPTimeSpan(1,0,0,0);
	if( tmCurrent.GetMonth() == 5 && tmCurrent.GetDay() == 1 )
		tmCurrent	-=	CSPTimeSpan(1,0,0,0);
	if( tmCurrent.GetMonth() == 10 && tmCurrent.GetDay() == 1 )
		tmCurrent	-=	CSPTimeSpan(1,0,0,0);
	if( tmCurrent.GetDayOfWeek() == 1 )
		tmCurrent	-=	CSPTimeSpan(2,0,0,0);
	if( tmCurrent.GetDayOfWeek() == 7 )
		tmCurrent	-=	CSPTimeSpan(1,0,0,0);
	
	m_tmDayFrom		=	m_netdb.GetTimeInitial().GetTime();
	m_tmDayTo		=	tmCurrent;
	m_tmMin5From	=	m_tmMin5To	=	tmCurrent;

	CSPTime	tmLatest;
	if( m_netdb.SetRootPath( AfxGetProfile().GetSelfDBPath(), IStStore::dbtypeSelfDB )
		&& m_netdb.GetTimeLocalRange( &tmLatest, NULL, NULL ) )
	{
		CString	strTimeLatest	=	(LPCTSTR)AfxGetTimeString( tmLatest.GetTime(), "%Y-%m-%d", TRUE ); //tmLatest.Format( "%Y-%m-%d,%A" );
		m_staticLatest.SetWindowText( strTimeLatest );

		m_tmDayFrom	=	CSPTime(tmLatest.GetTime()) + CSPTimeSpan(1,0,0,0 );

		if( m_tmDayFrom.GetMonth() == 1 && m_tmDayFrom.GetDay() == 1 )
			m_tmDayFrom	+=	CSPTimeSpan(1,0,0,0);
		if( m_tmDayFrom.GetMonth() == 5 && m_tmDayFrom.GetDay() == 1 )
			m_tmDayFrom	+=	CSPTimeSpan(1,0,0,0);
		if( m_tmDayFrom.GetMonth() == 10 && m_tmDayFrom.GetDay() == 1 )
			m_tmDayFrom	+=	CSPTimeSpan(1,0,0,0);
		if( m_tmDayFrom.GetDayOfWeek() == 1 )
			m_tmDayFrom	+=	CSPTimeSpan(1,0,0,0);
		if( m_tmDayFrom.GetDayOfWeek() == 7 )
			m_tmDayFrom	+=	CSPTimeSpan(2,0,0,0);
		
		if( m_tmDayFrom > m_tmDayTo )
			m_tmDayTo	=	m_tmDayFrom;
	}

	// Initial Download
	if( m_bInitDownloadTime )
	{
		m_tmDayFrom	=	m_tmDLBegin;
		m_tmDayTo	=	m_tmDLEnd;
	}

	UpdateData( FALSE );

	if( m_bAutoRun )
	{
		UpdateWindow( );
		ShowWindow( SW_SHOW );
		OnOK( );
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDownloadDlg::InitComboServers( )
{
	for( int i=m_comboServers.GetCount()-1; i>=0; i-- )
		m_comboServers.DeleteString(i);
	CString	strMain	=	AfxGetProfile().GetMainServer();
	m_comboServers.AddString( strMain );
	
	CSPStringArray &	astr	=	AfxGetProfile().GetDownloadServers();
	for( int i=0; i<astr.GetSize(); i++ )
	{
		if( 0 != strMain.CompareNoCase((LPCTSTR)astr.ElementAt(i)) )
			m_comboServers.AddString( (LPCTSTR)astr.ElementAt(i) );
	}
	m_comboServers.SelectString( 0, AfxGetProfile().GetDefaultServer() );
}

BOOL CDownloadDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CXFDialog::PreTranslateMessage(pMsg);
}

void CDownloadDlg::OnOK() 
{
	UpdateData( );

	// clear error pac msg
	m_astrErrorPac.RemoveAll();

	// disable and enble buttons
	m_btnOK.EnableWindow( FALSE );
	m_btnCancel.EnableWindow( FALSE );
	m_btnProxySet.EnableWindow( FALSE );
	m_btnRefreshServers.EnableWindow( FALSE );
	m_btnStop.EnableWindow( TRUE );

	BOOL	bOK	=	TRUE;

	// Set Message
	SendMessage( WM_USER_DOWNLOAD_MESSAGE, PROG_NEWGROUP, IDS_DLMSG_CONNECTING );
	m_staticStatus.SetWindowPos( NULL, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW);
	MSG		msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) )
		AfxGetApp()->PumpMessage();
	
	// set server info
	if( bOK && !m_netdb.SetRootPath( AfxGetProfile().GetSelfDBPath(), IStStore::dbtypeSelfDB ) )
	{
		AfxMessageBox( IDS_ROOTPATH_ERROR, MB_OK | MB_ICONINFORMATION );
		bOK	=	FALSE;
	}
	CString	strServer;
	m_comboServers.GetWindowText( strServer );
	CDownloadServer	server;
	if( !server.FromString( strServer ) )
	{
		AfxMessageBox( IDS_NOSERVER_DEFINED, MB_OK | MB_ICONINFORMATION );
		bOK	=	FALSE;
	}
	AfxGetProfile().SetDefaultServer( strServer );
	
	if( bOK && !m_netdb.SetServer( server.m_strAddress, server.m_nPort ) )
	{
		AfxMessageBox( IDS_NOSERVER_DEFINED, MB_OK | MB_ICONINFORMATION );
		bOK	=	FALSE;
	}
	// set access type
	if( bOK && !m_netdb.SetAccessType( AfxGetQSProfile().GetAccessType(), AfxGetQSProfile().GetProxyType(),
			AfxGetQSProfile().GetProxyAddress(), AfxGetQSProfile().GetProxyPort(),
			AfxGetQSProfile().GetProxyUser(), AfxGetQSProfile().GetProxyPasswd() ) )
	{
		AfxMessageBox( IDS_NOSERVER_DEFINED, MB_OK | MB_ICONINFORMATION );
		bOK	=	FALSE;
	}

	// login
	if( bOK && !m_netdb.Login( AfxGetSView().GetS(), AfxGetSView().GetU(),
							   NULL, AfxGetProfile().GetVersion() ) )
	{
		char	szErr[1024];
		if( m_netdb.GetLastErrorMessage(szErr,1023) )
			AfxMessageBox( szErr, MB_OK | MB_ICONINFORMATION );
		else
			AfxMessageBox( IDS_LOGIN_FAILED, MB_OK | MB_ICONINFORMATION );
		bOK	=	FALSE;
	}
	// load package info
	if( bOK && ! m_netdb.NetloadPackageInfo( NULL, NULL ) )
	{
		char	szErr[1024];
		if( m_netdb.GetLastErrorMessage(szErr,1023) )
			AfxMessageBox( szErr, MB_OK | MB_ICONINFORMATION );
		else
			AfxMessageBox( IDS_DOWNLOAD_FAILED, MB_OK | MB_ICONINFORMATION );
		bOK	=	FALSE;
	}

	if( bOK )
	{
		// show message
		m_staticStatus.SetWindowPos( NULL, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW);
		m_ctrlProgress.SetWindowPos( NULL, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW);
		CString	strMsg;
		strMsg.LoadString( IDS_DATA_DOWNLOADING );
		m_staticStatus.SetWindowText( strMsg );

		CDownloadDlg::m_hEventKillDownloadThread = CreateEvent(NULL,FALSE,FALSE,NULL);
		CDownloadDlg::m_hEventDownloadThreadKilled = CreateEvent(NULL,FALSE,FALSE,NULL);

		if( NULL == m_pDownloadInfo )
			m_pDownloadInfo	=	new DOWNLOAD_INFO;
		memset( m_pDownloadInfo, 0, sizeof(DOWNLOAD_INFO) );
		m_pDownloadInfo->hMainWnd	=	GetSafeHwnd();
		m_pDownloadInfo->pNetDB		=	&m_netdb;
		m_pDownloadInfo->bReport	=	m_btnCheckReport.GetCheck();
		m_pDownloadInfo->bDay		=	m_btnCheckDay.GetCheck();
		m_pDownloadInfo->tmDayFrom	=	m_tmDayFrom.GetTime();
		m_pDownloadInfo->tmDayTo	=	m_tmDayTo.GetTime();
		m_pDownloadInfo->bMin5		=	m_btnCheckMin5.GetCheck();
		m_pDownloadInfo->tmMin5From	=	m_tmMin5From.GetTime();
		m_pDownloadInfo->tmMin5To	=	m_tmMin5To.GetTime();
		m_pDownloadInfo->bBase		=	m_btnCheckBase.GetCheck();
		m_pDownloadInfo->bXDR		=	m_btnCheckXDR.GetCheck();

		if( m_pDownloadInfo->bReport )
		{
			::SendMessage( GetSafeHwnd(), WM_USER_DOWNLOAD_MESSAGE, PROG_NEWGROUP, IDS_DLMSG_REPORT );
			::SendMessage( GetSafeHwnd(), WM_USER_DOWNLOAD_PROGRESS, 0, NULL );
			m_nReportCount = 0;
			m_bReportFinished = FALSE;
			AfxGetStkReceiver().AddRcvDataWnd( GetSafeHwnd() );
			CStockContainer & container = AfxGetStockContainer();
			container.Lock();
			m_nReportTotal = container.GetSize();
			m_nReportRequestSent = AfxGetStkReceiver().RequestStockData( CStock::dataReport, container.GetData(), min(160,container.GetSize()), 0, 0 );
			container.UnLock();
		}
		else
		{
			AfxBeginThread( DownloadMain, (LPVOID)m_pDownloadInfo,THREAD_PRIORITY_NORMAL);
		}
	}
	else
	{
		// disable and enble buttons
		m_btnOK.EnableWindow( TRUE );
		m_btnCancel.EnableWindow( TRUE );
		m_btnProxySet.EnableWindow( TRUE );
		m_btnRefreshServers.EnableWindow( TRUE );
		m_btnStop.EnableWindow( FALSE );
	}
}

void CDownloadDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_btnCancel.EnableWindow( FALSE );
	AfxGetStkReceiver().RemoveRcvDataWnd( GetSafeHwnd() );

	AfxReleaseDB();
	char	szErr[1024];
	if( !AfxInitializeDB( szErr, sizeof(szErr) ) )	AfxMessageBox( szErr, MB_OK|MB_ICONINFORMATION );

	CDialog::OnCancel( );

	if( m_bDBModified )
	{
		BOOL	bRecalculate	=	FALSE;
		CSPTime	tmLatest	=	0;
		if( ( m_netdb.SetRootPath( AfxGetProfile().GetSelfDBPath(), IStStore::dbtypeSelfDB )
			&& m_netdb.GetTimeLocalRange( &tmLatest, NULL, NULL )
			&& tmLatest.GetTime() > m_tmLatestOld.GetTime() ) )
		{
			bRecalculate	=	TRUE;
		}
		if( m_bAutoRun )
			bRecalculate	=	FALSE;
		AfxGetStkUIApp()->OnDBChanged( FALSE /*bRecalculate*/ );
	}

	if( !m_bAutoRun )
	{
		CNetInfoDlg	dlg;
		dlg.DoModal( );

		if( AfxGetProfile().HasNewVersion()
			&& IDYES == AfxMessageBox( IDS_DOWNLOAD_UPGRADENOW, MB_YESNO ) )
		{
			CUpgradeDlg	dlg;
			dlg.SetAutoRun( TRUE );
			dlg.DoModal();
		}
	}

	CXFDialog::PostNcDestroy();
}

void CDownloadDlg::OnRefreshServers() 
{
	UpdateData( );

	// disable and enble buttons
	m_btnRefreshServers.EnableWindow( FALSE );
	m_btnOK.EnableWindow( FALSE );
	m_btnStop.EnableWindow( FALSE );
	m_btnCancel.EnableWindow( FALSE );
	m_btnProxySet.EnableWindow( FALSE );
	m_btnStopRefresh.EnableWindow( TRUE );
	
	// show message
	m_staticRefreshServer.SetWindowPos( NULL, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW);
	CString	strMsg;
	strMsg.LoadString( IDS_SERVERS_REFRESHING );
	m_staticRefreshServer.SetWindowText( strMsg );

	m_bStopRefresh	=	FALSE;
	BOOL	bOK	=	TRUE;
	
	// refresh servers
	CString	strServer	=	AfxGetProfile().GetMainServer();
	CDownloadServer	server;
	if( !server.FromString( strServer ) )
	{
		AfxMessageBox( IDS_NOSERVER_DEFINED, MB_OK | MB_ICONINFORMATION );
		bOK	=	FALSE;
	}
	if( bOK && !m_netdb.SetServer( server.m_strAddress, server.m_nPort ) )
	{
		AfxMessageBox( IDS_NOSERVER_DEFINED, MB_OK | MB_ICONINFORMATION );
		bOK	=	FALSE;
	}
	// set access type
	if( bOK && !m_netdb.SetAccessType( AfxGetQSProfile().GetAccessType(), AfxGetQSProfile().GetProxyType(),
			AfxGetQSProfile().GetProxyAddress(), AfxGetQSProfile().GetProxyPort(),
			AfxGetQSProfile().GetProxyUser(), AfxGetQSProfile().GetProxyPasswd() ) )
	{
		AfxMessageBox( IDS_NOSERVER_DEFINED, MB_OK | MB_ICONINFORMATION );
		bOK	=	FALSE;
	}
	// login
	if( bOK && !m_netdb.Login(	AfxGetSView().GetS(), AfxGetSView().GetU(),
								NULL, AfxGetProfile().GetVersion() ) ) 
	{
		char	szErr[1024];
		if( m_netdb.GetLastErrorMessage(szErr,1023) )
			AfxMessageBox( szErr, MB_OK | MB_ICONINFORMATION );
		else
			AfxMessageBox( IDS_LOGIN_FAILED, MB_OK | MB_ICONINFORMATION );
		bOK	=	FALSE;
	}

	// load server info
	if( bOK && ! m_netdb.NetloadServerInfo( NULL, NULL ) )
	{
		char	szErr[1024];
		if( m_netdb.GetLastErrorMessage(szErr,1023) )
			AfxMessageBox( szErr, MB_OK | MB_ICONINFORMATION );
		else
			AfxMessageBox( IDS_REFRESHSERVERS_FAILED, MB_OK | MB_ICONINFORMATION );
	}

	// refresh server combo list
	if( bOK )
	{
		CServers &	servers	=	m_netdb.GetServers();
		servers.SaveToProfile();
		InitComboServers();
	}

	// logout
	m_netdb.Logout();

	// set message and enable buttons
	if( bOK )
		strMsg.LoadString( IDS_SERVERS_REFRESHED );
	else
		strMsg.LoadString( IDS_REFRESHSERVERS_PAUSED );
	m_staticRefreshServer.SetWindowText( strMsg );

	m_btnRefreshServers.EnableWindow( TRUE );
	m_btnOK.EnableWindow( TRUE );
	m_btnCancel.EnableWindow( TRUE );
	m_btnProxySet.EnableWindow( TRUE );
	m_btnStop.EnableWindow( FALSE );
	m_btnStopRefresh.EnableWindow( FALSE );
}

void CDownloadDlg::OnStopRefresh() 
{
	// TODO: Add your control notification handler code here
	// set message and enable buttons
	m_bStopRefresh	=	TRUE;
}

void CDownloadDlg::OnStop()
{
	if( !m_bReportFinished )
	{
		m_bReportFinished = TRUE;
		::PostMessage( GetSafeHwnd(), WM_USER_DOWNLOAD_END, FALSE, 0 );
	}

	if( m_hEventKillDownloadThread )
		SetEvent( m_hEventKillDownloadThread );
	// if( m_hEventDownloadThreadKilled )
	//		WaitForSingleObject( m_hEventDownloadThreadKilled, INFINITE );
}

LRESULT CDownloadDlg::OnStkReceiverData(WPARAM wParam, LPARAM lParam)
{
	PCOMMPACKET	pCommPacket	=	(PCOMMPACKET)lParam;

	switch( wParam )
	{
	case CStock::dataReport:
		if( pCommPacket && CStock::dataReport == pCommPacket->m_dwDataType 
			&& pCommPacket->m_dwCount > 0 )
		{
			m_nReportCount += pCommPacket->m_dwCount;
			for( DWORD i=0; i<pCommPacket->m_dwCount; i++ )
			{
				CKData kdata;
				kdata.SetKType( CKData::ktypeDay );
				KDATA kd;
				if( UpdateKDATAByREPORT( kd, &(pCommPacket->m_pReport[i]) ) )
				{
					kdata.Add( kd );
					AfxGetDB().InstallKData( kdata, FALSE );
				}
			}

			CStockContainer & container = AfxGetStockContainer();
			if( m_nReportRequestSent - m_nReportCount < 64 && m_nReportRequestSent < (UINT)container.GetSize() )
			{
				container.Lock();
				m_nReportRequestSent += AfxGetStkReceiver().RequestStockData( CStock::dataReport, container.GetData()+m_nReportRequestSent, min(160,container.GetSize()-m_nReportRequestSent), 0, 0 );
				container.UnLock();
			}

			if( !m_bReportFinished && m_nReportCount > 0.95 * m_nReportTotal )
			{
				m_bReportFinished = TRUE;
				::SendMessage( GetSafeHwnd(), WM_USER_DOWNLOAD_PROGRESS, STKLIB_MAX_PROGRESS, NULL );
				AfxBeginThread( DownloadMain, (LPVOID)m_pDownloadInfo,THREAD_PRIORITY_NORMAL);
			}
			else if( !m_bReportFinished && m_nReportTotal > 0 )
			{
				DWORD dwProgress = STKLIB_MAX_PROGRESS * m_nReportCount / m_nReportTotal;
				::SendMessage( GetSafeHwnd(), WM_USER_DOWNLOAD_PROGRESS, dwProgress, NULL );
			}
		}
		break;
	}
	return 0L;
}

LRESULT CDownloadDlg::OnDownloadMessage(WPARAM wParam, LPARAM lParam)
{
	// set message and enable buttons
	CString	strNewMsg;
	CString	strOldMsg;
	m_staticStatus.GetWindowText( strOldMsg );
	
	if( PROG_NEWGROUP == wParam )
	{
		strNewMsg.LoadString( lParam );
	}
	else if( PROG_ERRORPAC == wParam )
	{
		int i;
		for( i=0; i<m_astrErrorPac.GetSize(); i++ )
		{
			if( 0 == m_astrErrorPac.ElementAt(i).Compare( (LPCTSTR)lParam ) )
				break;
		}
		if( i == m_astrErrorPac.GetSize() )
			m_astrErrorPac.Add( (LPCTSTR)lParam );

		CString	strMsg	=	CNetDatabase::GetProgressMessage( wParam );
		strMsg	+=	(LPCTSTR)lParam;
		if( strMsg.GetLength() > 0 )
		{
			strNewMsg	=	strOldMsg;
			int	nIndex	=	strOldMsg.Find( " -- " );
			if( -1 != nIndex )
				strNewMsg	=	strOldMsg.Left( nIndex );
			strNewMsg	+=	" -- " + strMsg;
		}
	}
	else
	{
		CString	strMsg	=	CNetDatabase::GetProgressMessage( wParam );
		if( strMsg.GetLength() > 0 )
		{
			strNewMsg	=	strOldMsg;
			int	nIndex	=	strOldMsg.Find( " -- " );
			if( -1 != nIndex )
				strNewMsg	=	strOldMsg.Left( nIndex );
			strNewMsg	+=	" -- " + strMsg;
		}
	}

	if( strNewMsg.GetLength() > 0 )
	{
		if( 0 != strNewMsg.Compare( strOldMsg ) )
			m_staticStatus.SetWindowText( strNewMsg );
	}

	return 1;
}

LRESULT CDownloadDlg::OnDownloadProgress(WPARAM wParam, LPARAM lParam)
{
	// set message and enable buttons
	if( wParam < 0 )
		wParam	=	0;
	if( wParam > STKLIB_MAX_PROGRESS )
		wParam = STKLIB_MAX_PROGRESS;
	m_ctrlProgress.SetPos( wParam );

	CString	strProgressNew;
	strProgressNew.Format( "%.2f%%", 100.*wParam/STKLIB_MAX_PROGRESS );
	CString	strProgressOld;
	m_staticProgress.GetWindowText( strProgressOld );
	if( 0 != strProgressNew.Compare( strProgressOld ) )
		m_staticProgress.SetWindowText( strProgressNew );
	return 1;
}

LRESULT CDownloadDlg::OnDownloadEnd(WPARAM wParam, LPARAM lParam)
{
	// logout
	m_netdb.Logout();

	m_ctrlProgress.SetPos( STKLIB_MAX_PROGRESS );
	CString	strProgressNew;
	strProgressNew.Format( "%.2f%%", 100. );
	m_staticProgress.SetWindowText( strProgressNew );

	// set message and enable buttons
	CString	strMsg;
	if( wParam )
		strMsg.LoadString( IDS_DOWNLOAD_FINISHED );
	else
		strMsg.LoadString( IDS_DOWNLOAD_PAUSED );
	
	CString	strComma;
	strComma.LoadString( IDS_COMMA );
	CString	strFullStop;
	strFullStop.LoadString( IDS_FULLSTOP );
	for( int i=0; i<m_astrErrorPac.GetSize(); i++ )
	{
		if( 0 == i )
			strMsg	+=	CNetDatabase::GetProgressMessage( PROG_ERRORPAC );
		else
			strMsg	+=	strComma;
		strMsg	+=	m_astrErrorPac.ElementAt(i);
	}

	if( m_astrErrorPac.GetSize() > 0 )
		strMsg	+=	strFullStop;
	if( lParam )
		strMsg	+=	(LPCTSTR)lParam;
	
	m_staticStatus.SetWindowText( strMsg );

	m_btnOK.EnableWindow( TRUE );
	m_btnCancel.EnableWindow( TRUE );
	m_btnProxySet.EnableWindow( TRUE );
	m_btnRefreshServers.EnableWindow( TRUE );
	m_btnStop.EnableWindow( FALSE );

	if( CDownloadDlg::m_hEventKillDownloadThread )
		CloseHandle(CDownloadDlg::m_hEventKillDownloadThread);
	if( CDownloadDlg::m_hEventDownloadThreadKilled )
		CloseHandle(CDownloadDlg::m_hEventDownloadThreadKilled);
	CDownloadDlg::m_hEventKillDownloadThread	=	NULL;
	CDownloadDlg::m_hEventDownloadThreadKilled	=	NULL;

	CSPTime	tmLatest;
	if( m_netdb.SetRootPath( AfxGetProfile().GetSelfDBPath(), IStStore::dbtypeSelfDB )
		&& m_netdb.GetTimeLocalRange( &tmLatest, NULL, NULL ) )
	{
		CString	strTimeLatest	=	tmLatest.Format( "%Y-%m-%d,%A" );
		m_staticLatest.SetWindowText( strTimeLatest );
	}

	m_bDBModified	=	TRUE;

	if( m_bAutoRun && wParam )
		OnCancel( );

	return 1;
}

void CDownloadDlg::OnSetfocusTimeDay(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_btnCheckDay.SetCheck( 1 );
	*pResult = 0;
}

void CDownloadDlg::OnSetfocusTimeMin5(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_btnCheckMin5.SetCheck( 1 );	
	*pResult = 0;
}

void CDownloadDlg::OnProxySet() 
{
	AfxDoProxySetting();
}

