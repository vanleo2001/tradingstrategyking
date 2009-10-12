// InstallPacDlg.cpp : implementation file
//

#include "stdafx.h"

#include "InstallPacDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInstallPacDlg dialog

HANDLE CInstallPacDlg::m_hEventKillInstallThread		=	NULL;
HANDLE CInstallPacDlg::m_hEventInstallThreadKilled		=	NULL;


BOOL CALLBACK InstallCallback(DWORD dwCode, DWORD dwProgress,
							LPCTSTR lpszMsg, void *cookie)
{
	HWND	hMainWnd	=	(HWND)cookie;

	if( PROG_PROGRESS == dwCode && ::IsWindow(hMainWnd) )
	{
		::SendMessage( hMainWnd, WM_USER_INSTALL_PROGRESS, dwProgress, (LPARAM)lpszMsg );
	}
	else if( ::IsWindow(hMainWnd) )
	{
		::SendMessage( hMainWnd, WM_USER_INSTALL_MESSAGE, dwCode, (LPARAM)lpszMsg );
	}

	// User wants to quit program
	if(WAIT_OBJECT_0 == WaitForSingleObject(CInstallPacDlg::m_hEventKillInstallThread,0) )
	{
		::PostMessage( hMainWnd, WM_USER_INSTALL_END, FALSE, 0 );
		SetEvent(CInstallPacDlg::m_hEventInstallThreadKilled);
		AfxEndThread( 0, TRUE );
		return FALSE;
	}
	return TRUE;
}

UINT InstallMain(LPVOID pParam)
{	
	LPINSTALL_INFO pInfo=(LPINSTALL_INFO)pParam;
	
	PostMessage( pInfo->hMainWnd, WM_USER_INSTALL_PROGRESS, 0, 0 );

	for( int i=0; i<pInfo->pStringArray->GetSize(); i++ )
	{
		CString	sFileName	=	pInfo->pStringArray->ElementAt(i);
		::SendMessage( pInfo->hMainWnd, WM_USER_INSTALL_MESSAGE, PROG_NEWGROUPMSG, (DWORD)sFileName.GetBuffer(sFileName.GetLength()+1) );
		::SendMessage( pInfo->hMainWnd, WM_USER_INSTALL_PROGRESS, 0, NULL );
		sFileName.ReleaseBuffer();

		CPackage	pac;
		pac.m_nType		=	pInfo->nType;
		pac.m_bIsZipped	=	( sFileName.GetLength() > 4 && 0 == sFileName.Right(4).CompareNoCase(".zip") );
		pac.m_strDescript	=	sFileName;
		pInfo->pNetDB->InstallPackageEx( pac, sFileName, InstallCallback, pInfo->hMainWnd );

		// User wants to quit program
		if(WAIT_OBJECT_0 == WaitForSingleObject(CInstallPacDlg::m_hEventKillInstallThread,0) )
		{
			::PostMessage( pInfo->hMainWnd, WM_USER_INSTALL_END, FALSE, 0 );
			SetEvent(CInstallPacDlg::m_hEventInstallThreadKilled);
			AfxEndThread( 0, TRUE );
			return 0;
		}
	}

	::SendMessage( pInfo->hMainWnd, WM_USER_INSTALL_END, TRUE, 0 );
	AfxEndThread( 0, TRUE );
	return 0;
}

IMPLEMENT_DYNCREATE(CInstallPacDlg, CPropertyPageEx)

CInstallPacDlg::CInstallPacDlg( )
	: CPropertyPageEx(CInstallPacDlg::IDD)
{
	//{{AFX_DATA_INIT(CInstallPacDlg)
	//}}AFX_DATA_INIT
	m_pInstallInfo	=	NULL;
	m_bDBModified	=	FALSE;

	m_bCodetableInstalled	=	FALSE;

	CSPTime	tmLatestOld	=	0;
	if( m_netdb.SetRootPath( AfxGetProfile().GetSelfDBPath(), IStStore::dbtypeSelfDB ) )
		m_netdb.GetTimeLocalRange( &tmLatestOld, NULL, NULL );
	m_tmLatestOld	=	tmLatestOld.GetTime();
}

CInstallPacDlg::~CInstallPacDlg( )
{
	if( m_pInstallInfo )
	{
		delete	m_pInstallInfo;
		m_pInstallInfo	=	NULL;
	}
}

void CInstallPacDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInstallPacDlg)
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_staticProgress);
	DDX_Control(pDX, IDC_START, m_btnStart);
	DDX_Control(pDX, IDC_STOP, m_btnStop);
	DDX_Control(pDX, IDC_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_EXPLORER, m_btnExplorer);
	DDX_Control(pDX, IDC_LIST_FILES, m_listFiles);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_staticStatus);
	DDX_Control(pDX, IDC_GODOWNLOADWEB, m_linkGoWeb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInstallPacDlg, CPropertyPageEx)
	//{{AFX_MSG_MAP(CInstallPacDlg)
	ON_BN_CLICKED(IDC_EXPLORER, OnExplorer)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILES, OnDblclkListFiles)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_START, OnStart)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_INSTALL_MESSAGE, OnInstallMessage)
	ON_MESSAGE(WM_USER_INSTALL_PROGRESS, OnInstallProgress)
	ON_MESSAGE(WM_USER_INSTALL_END, OnInstallEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInstallPacDlg message handlers

BOOL CInstallPacDlg::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString	strURL	=	AfxGetProfile().GetDownloadURL();
	m_linkGoWeb.SetURL( strURL );

	CheckRadioButton( IDC_DAYPACKAGE, IDC_MIN5PACKAGE, IDC_DAYPACKAGE );

	m_staticStatus.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW );
	m_ctrlProgress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW );
	m_btnStop.EnableWindow( FALSE );

	CRect	rect;
	m_listFiles.GetWindowRect( &rect );
	m_listFiles.InsertColumn( 0, "", LVCFMT_LEFT, rect.Width()-5 );

	m_imageList.Create(IDB_PACKAGE, 16, 1, RGB(0, 255, 0));
	m_listFiles.SetImageList( &m_imageList, LVSIL_SMALL );

	m_ctrlProgress.SetRange( 0, STKLIB_MAX_PROGRESS );
	m_ctrlProgress.SetPos( 0 );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CInstallPacDlg::OnSetActive() 
{
	CPropertySheetEx * pSheet = DYNAMIC_DOWNCAST( CPropertySheetEx, GetParent()	);
	if( pSheet )
	{
		CString	strFinish, strClose;
		strFinish.LoadString( IDS_FINISHBUTTON );
		strClose.LoadString( IDS_CLOSEBUTTON );

		CWnd * pWnd = pSheet->GetDlgItem( IDOK );
		if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
		{
			pWnd->SetWindowText( strFinish );
			pWnd->EnableWindow( FALSE );
		}

		pWnd = pSheet->GetDlgItem( IDCANCEL );
		if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
			pWnd->SetWindowText( strClose );
	}
	
	return CPropertyPageEx::OnSetActive();
}

void CInstallPacDlg::OnExplorer() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	CKSFileDialog dlg (TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING | OFN_ALLOWMULTISELECT,
		"Package files (*.zip,*.stk,*.dad)|*.zip;*.stk;*.dad|All files (*.*)|*.*||", NULL);

	int nResult = dlg.DoModal();
	if( IDOK == nResult )
	{
		POSITION pos = dlg.GetStartPosition();
		while (pos)
		{
			CString sFile = dlg.GetNextPathName(pos);
			int i;
			for( i=0; i<m_listFiles.GetItemCount(); i++ )
			{
				CString	strTemp = m_listFiles.GetItemText( i, 0 );
				if( 0 == strTemp.CompareNoCase( sFile ) )
					break;
			}
			if( i == m_listFiles.GetItemCount() )
				m_listFiles.InsertItem( m_listFiles.GetItemCount(), sFile, 0 );
		}
	}
}

void CInstallPacDlg::OnRemove() 
{
	// TODO: Add your control notification handler code here
	for( int i=m_listFiles.GetItemCount()-1; i>=0; i-- )
	{
		if( LVIS_SELECTED == m_listFiles.GetItemState( i, LVIS_SELECTED ) )
			m_listFiles.DeleteItem( i );
	}
	m_listFiles.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED );
	m_listFiles.RedrawItems( 0, m_listFiles.GetItemCount()-1 );
}

void CInstallPacDlg::OnDblclkListFiles(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnRemove( );

	*pResult = 0;
}

void CInstallPacDlg::OnStart() 
{
	// TODO: Add extra validation here
	if( 0 == m_listFiles.GetItemCount() )
	{
		AfxMessageBox( IDS_SELECTPACKAGE_FIRST, MB_OK | MB_ICONINFORMATION );
		return;
	}

	UpdateData( );
	// clear error pac msg
	m_astrErrorPac.RemoveAll();

	m_btnStart.EnableWindow( FALSE );
	m_btnStop.EnableWindow( TRUE );

	BOOL	bOK	=	TRUE;
	if( bOK && !m_netdb.SetRootPath( AfxGetProfile().GetSelfDBPath(), IStStore::dbtypeSelfDB ) )
	{
		AfxMessageBox( IDS_ROOTPATH_ERROR, MB_OK | MB_ICONINFORMATION );
		bOK	=	FALSE;
	}

	if( bOK )
	{
		m_staticStatus.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW );
		m_ctrlProgress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW );

		CInstallPacDlg::m_hEventKillInstallThread = CreateEvent(NULL,FALSE,FALSE,NULL);
		CInstallPacDlg::m_hEventInstallThreadKilled = CreateEvent(NULL,FALSE,FALSE,NULL);

		m_astrPacFiles.RemoveAll();
		for( int i=0; i<m_listFiles.GetItemCount(); i++ )
			m_astrPacFiles.Add( m_listFiles.GetItemText( i, 0 ) );

		if( NULL == m_pInstallInfo )
			m_pInstallInfo	=	new INSTALL_INFO;
		memset( m_pInstallInfo, 0, sizeof(INSTALL_INFO) );
		m_pInstallInfo->hMainWnd	=	GetSafeHwnd();
		m_pInstallInfo->pNetDB		=	&m_netdb;
		m_pInstallInfo->pStringArray=	&m_astrPacFiles;

		CButton * pbtnDay	= (CButton *)GetDlgItem( IDC_DAYPACKAGE );
		CButton * pbtnMin5		= (CButton *)GetDlgItem( IDC_MIN5PACKAGE );

		m_pInstallInfo->nType	=	CPackage::packageUnknown;
		if( pbtnDay && pbtnDay->GetCheck() )
			m_pInstallInfo->nType	=	CPackage::packageDay;
		if( pbtnMin5 && pbtnMin5->GetCheck() )
			m_pInstallInfo->nType	=	CPackage::packageMin5;
		AfxBeginThread( InstallMain, (LPVOID)m_pInstallInfo,THREAD_PRIORITY_NORMAL);
	}
	else
	{
		m_btnStart.EnableWindow( TRUE );
		m_btnStop.EnableWindow( FALSE );
	}
}


void CInstallPacDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	if( m_hEventKillInstallThread )
		SetEvent( m_hEventKillInstallThread );
	// if( m_hEventInstallThreadKilled )
	//		WaitForSingleObject( m_hEventInstallThreadKilled, INFINITE );
}

LRESULT CInstallPacDlg::OnInstallMessage(WPARAM wParam, LPARAM lParam)
{
	// set message and enable buttons
	CString	strNewMsg;
	CString	strOldMsg;
	m_staticStatus.GetWindowText( strOldMsg );
	
	if( PROG_NEWGROUP == wParam )
	{
		strNewMsg.LoadString( lParam );
	}
	else if( PROG_NEWGROUPMSG == wParam )
	{
		strNewMsg	=	(LPCTSTR)lParam;
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

LRESULT CInstallPacDlg::OnInstallProgress(WPARAM wParam, LPARAM lParam)
{
	// set message and enable buttons
	if( wParam < 0 )
		wParam	=	0;
	if( wParam > STKLIB_MAX_PROGRESS )
		wParam = STKLIB_MAX_PROGRESS;
	m_ctrlProgress.SetPos( wParam );

	CString	strProgressNew;
	strProgressNew.Format( "%.2f%%", float(wParam)/100. );
	CString	strProgressOld;
	m_staticProgress.GetWindowText( strProgressOld );
	if( 0 != strProgressNew.Compare( strProgressOld ) )
		m_staticProgress.SetWindowText( strProgressNew );

	return 1;
}

LRESULT CInstallPacDlg::OnInstallEnd(WPARAM wParam, LPARAM lParam)
{
	m_ctrlProgress.SetPos( STKLIB_MAX_PROGRESS );
	CString	strProgressNew;
	strProgressNew.Format( "%.2f%%", 100. );
	m_staticProgress.SetWindowText( strProgressNew );

	// set message and enable buttons
	CString	strMsg;
	if( wParam )
		strMsg.LoadString( IDS_INSTALL_FINISHED );
	else
		strMsg.LoadString( IDS_INSTALL_PAUSED );
	
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

	m_btnStart.EnableWindow( TRUE );
	m_btnStop.EnableWindow( FALSE );

	if( CInstallPacDlg::m_hEventKillInstallThread )
		CloseHandle(CInstallPacDlg::m_hEventKillInstallThread);
	if( CInstallPacDlg::m_hEventInstallThreadKilled )
		CloseHandle(CInstallPacDlg::m_hEventInstallThreadKilled);
	CInstallPacDlg::m_hEventKillInstallThread	=	NULL;
	CInstallPacDlg::m_hEventInstallThreadKilled	=	NULL;

	m_bDBModified	=	TRUE;

	return 1;
}

void CInstallPacDlg::OnCompleted( )
{
	AfxReleaseDB();
	char	szErr[1024];
	if( !AfxInitializeDB( szErr, sizeof(szErr) ) )	AfxMessageBox( szErr, MB_OK|MB_ICONINFORMATION );

	if( m_bDBModified )
	{
		BOOL	bRecalculate	=	FALSE;
		CSPTime	tmLatest	=	0;
		if( m_bCodetableInstalled ||
			( m_netdb.SetRootPath( AfxGetProfile().GetSelfDBPath(), IStStore::dbtypeSelfDB )
			&& m_netdb.GetTimeLocalRange( &tmLatest, NULL, NULL )
			&& tmLatest.GetTime() > m_tmLatestOld.GetTime() ) )
		{
			bRecalculate	=	TRUE;
		}
		AfxGetStkUIApp()->OnDBChanged( FALSE /*bRecalculate*/ );
		m_bDBModified	=	FALSE;
	}
}


void CInstallPacDlg::OnCancel() 
{
	OnCompleted( );

	CPropertyPageEx::OnCancel();
}

void CInstallPacDlg::OnOK() 
{
	OnCompleted( );

	CPropertyPageEx::OnOK();
}

BOOL CInstallPacDlg::OnKillActive() 
{
	OnCompleted( );

	return CPropertyPageEx::OnKillActive();
}
