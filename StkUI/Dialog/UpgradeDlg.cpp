// UpgradeDlg.cpp : implementation file
//

#include "stdafx.h"

#include "UpgradeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define	ST_UPGRADE_PAUSED			1
#define	ST_UPGRADE_DLOK				2
#define	ST_UPGRADE_NOTFOUND			3
#define	ST_UPGRADE_CANNOTOPENFILE	4


HANDLE CUpgradeDlg::m_hEventKillUpgradeThread	=	NULL;
HANDLE CUpgradeDlg::m_hEventUpgradeThreadKilled	=	NULL;
HFILE  CUpgradeDlg::m_hFileUpgrade				=	NULL;


BOOL CALLBACK UpgradeCallback(DWORD dwCode, DWORD dwProgress,
							LPCTSTR lpszMsg, void *cookie)
{
	HWND	hMainWnd	=	(HWND)cookie;

	if( PROG_PROGRESS == dwCode )
	{
		::SendMessage( hMainWnd, WM_USER_UPGRADE_PROGRESS, dwProgress, (LPARAM)lpszMsg );
	}
	else
	{
		::SendMessage( hMainWnd, WM_USER_UPGRADE_MESSAGE, dwCode, (LPARAM)lpszMsg );
	}

	// User wants to quit program
	if(WAIT_OBJECT_0 == WaitForSingleObject(CUpgradeDlg::m_hEventKillUpgradeThread,0) )
	{
		::PostMessage( hMainWnd, WM_USER_UPGRADE_END, ST_UPGRADE_PAUSED, 0 );
		SetEvent(CUpgradeDlg::m_hEventUpgradeThreadKilled);
		if( NULL != CUpgradeDlg::m_hFileUpgrade )
		{
			::CloseHandle( (HANDLE)CUpgradeDlg::m_hFileUpgrade );
			CUpgradeDlg::m_hFileUpgrade	=	NULL;
		}
		AfxEndThread( 0, TRUE );
		return FALSE;
	}
	return TRUE;
}

char	szUpgradeFileName[]	=	"upgrade.exe";

UINT UpgradeMain(LPVOID pParam)
{	
	LPUPGRADE_INFO pInfo=(LPUPGRADE_INFO)pParam;
	
	PostMessage( pInfo->hMainWnd, WM_USER_UPGRADE_PROGRESS, 0, 0 );

	CHttpClient	http;
	http.SetServer( AfxGetProfile().GetHTTPServerDomain(), AfxGetProfile().GetHTTPServerPort() );
	http.SetAccessType( AfxGetQSProfile().GetAccessType(), AfxGetQSProfile().GetProxyType(),
						AfxGetQSProfile().GetProxyAddress(), AfxGetQSProfile().GetProxyPort(),
						AfxGetQSProfile().GetProxyUser(), AfxGetQSProfile().GetProxyPasswd() );
	
	CFile	file;
	CString	sFileName	=	AfxGetProfile().GetWorkDirectory() + szUpgradeFileName;
	if( file.Open( sFileName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite) )
	{
		CUpgradeDlg::m_hFileUpgrade	=	(HFILE)file.m_hFile;
		DWORD dwHttpCode = http.WGet( AfxGetProfile().GetUpgradeURL(), &file, UpgradeCallback, pInfo->hMainWnd );
		file.Close();
		CUpgradeDlg::m_hFileUpgrade	=	NULL;
		if( HTTP_STATUS_OK == dwHttpCode )
		{
			::SendMessage( pInfo->hMainWnd, WM_USER_UPGRADE_END, ST_UPGRADE_DLOK, 0 );
		}
		else
		{
			::SendMessage( pInfo->hMainWnd, WM_USER_UPGRADE_END, ST_UPGRADE_NOTFOUND, 0 );
		}
	}
	else
	{
		::SendMessage( pInfo->hMainWnd, WM_USER_UPGRADE_END, ST_UPGRADE_CANNOTOPENFILE, 0 );
		if( NULL != CUpgradeDlg::m_hFileUpgrade )
		{
			::CloseHandle( (HANDLE)CUpgradeDlg::m_hFileUpgrade );
			CUpgradeDlg::m_hFileUpgrade	=	NULL;
		}
	}
	
	AfxEndThread( 0, TRUE );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CUpgradeDlg dialog


CUpgradeDlg::CUpgradeDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CUpgradeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUpgradeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pUpgradeInfo	=	NULL;
	m_bAutoRun		=	FALSE;
}

CUpgradeDlg::~CUpgradeDlg( )
{
	if( m_pUpgradeInfo )
	{
		delete	m_pUpgradeInfo;
		m_pUpgradeInfo	=	NULL;
	}
}

void CUpgradeDlg::SetAutoRun( BOOL bAutoRun )
{
	m_bAutoRun	=	bAutoRun;
}

void CUpgradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUpgradeDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_staticStatus);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_staticProgress);
	DDX_Control(pDX, IDC_PROXY_SET, m_btnProxySet);
	DDX_Control(pDX, IDC_EDIT_EXPLAIN, m_editExplain);
	DDX_Control(pDX, IDC_DL_PROGRESS, m_ctrlProgress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUpgradeDlg, CXFDialog)
	//{{AFX_MSG_MAP(CUpgradeDlg)
	ON_BN_CLICKED(IDC_PROXY_SET, OnProxySet)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_UPGRADE_MESSAGE, OnUpgradeMessage)
	ON_MESSAGE(WM_USER_UPGRADE_PROGRESS, OnUpgradeProgress)
	ON_MESSAGE(WM_USER_UPGRADE_END, OnUpgradeEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpgradeDlg message handlers

BOOL CUpgradeDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	HRSRC		hReSrc = NULL;
	HGLOBAL		hRead = NULL;
	LPCTSTR		lpText;

	HINSTANCE	hModule	=	AfxGetResourceHandle();

	hReSrc = ::FindResource( hModule, MAKEINTRESOURCE(IDR_TEXT_UPGRADEEXPLAIN), _T("Text") );
	if( hReSrc )
		hRead	=	::LoadResource( hModule, hReSrc );
	if( hRead )
	{
		lpText	=	(LPCTSTR)::LockResource( hRead );
		if( lpText )
		{
			m_editExplain.SetWindowText( lpText );
		}
		::UnlockResource( hRead );
	}

	m_staticStatus.SetWindowPos( NULL, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW);
	m_ctrlProgress.SetWindowPos( NULL, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW);

	m_ctrlProgress.SetRange( 0, STKLIB_MAX_PROGRESS );
	m_ctrlProgress.SetPos( 0 );

	if( m_bAutoRun )
		OnOK( );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUpgradeDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData( );

	// disable and enble buttons
	m_btnOK.EnableWindow( FALSE );
	m_btnProxySet.EnableWindow( FALSE );

	// show message
	m_staticStatus.SetWindowPos( NULL, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW);
	m_ctrlProgress.SetWindowPos( NULL, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW);
	
	CUpgradeDlg::m_hEventKillUpgradeThread = CreateEvent(NULL,FALSE,FALSE,NULL);
	CUpgradeDlg::m_hEventUpgradeThreadKilled = CreateEvent(NULL,FALSE,FALSE,NULL);

	if( NULL == m_pUpgradeInfo )
		m_pUpgradeInfo	=	new UPGRADE_INFO;
	memset( m_pUpgradeInfo, 0, sizeof(UPGRADE_INFO) );
	m_pUpgradeInfo->hMainWnd	=	GetSafeHwnd();
	AfxBeginThread( UpgradeMain, (LPVOID)m_pUpgradeInfo,THREAD_PRIORITY_NORMAL);
}

void CUpgradeDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	SetEvent( m_hEventKillUpgradeThread );
	// WaitForSingleObject( m_hEventUpgradeThreadKilled, INFINITE );

	CXFDialog::OnCancel();
}

void CUpgradeDlg::OnProxySet() 
{
	// TODO: Add your control notification handler code here
	AfxDoProxySetting();
}

LRESULT CUpgradeDlg::OnUpgradeMessage(WPARAM wParam, LPARAM lParam)
{
	// set message and enable buttons
	CString	strNewMsg;
	CString	strOldMsg;
	m_staticStatus.GetWindowText( strOldMsg );
	
	CString	strMsg	=	CNetDatabase::GetProgressMessage( wParam );
	if( strMsg.GetLength() > 0 )
	{
		strNewMsg	=	strOldMsg;
		int	nIndex	=	strOldMsg.Find( " -- " );
		if( -1 != nIndex )
			strNewMsg	=	strOldMsg.Left( nIndex );
		strNewMsg	+=	" -- " + strMsg;
	}

	if( strNewMsg.GetLength() > 0 )
	{
		if( 0 != strNewMsg.Compare( strOldMsg ) )
			m_staticStatus.SetWindowText( strNewMsg );
	}

	return 1;
}

LRESULT CUpgradeDlg::OnUpgradeProgress(WPARAM wParam, LPARAM lParam)
{
	// set message and enable buttons
	if( wParam < 0 )
		wParam	=	0;
	if( wParam > STKLIB_MAX_PROGRESS-1 )
		wParam = STKLIB_MAX_PROGRESS-1;
	m_ctrlProgress.SetPos( wParam );

	CString	strProgressNew;
	strProgressNew.Format( "%.2f%%", float(wParam)/100. );
	CString	strProgressOld;
	m_staticProgress.GetWindowText( strProgressOld );
	if( 0 != strProgressNew.Compare( strProgressOld ) )
		m_staticProgress.SetWindowText( strProgressNew );
	return 1;
}

LRESULT CUpgradeDlg::OnUpgradeEnd(WPARAM wParam, LPARAM lParam)
{
	m_ctrlProgress.SetPos( STKLIB_MAX_PROGRESS );
	CString	strProgressNew;
	strProgressNew.Format( "%.2f%%", 100. );
	m_staticProgress.SetWindowText( strProgressNew );

	// set message and enable buttons
	CString	strMsg;
	if( ST_UPGRADE_PAUSED == wParam )
		strMsg.LoadString( IDS_UPGRADE_PAUSED );
	else if( ST_UPGRADE_NOTFOUND == wParam )
		strMsg.LoadString( IDS_UPGRADE_NOTFOUND );
	else if( ST_UPGRADE_DLOK == wParam )
		strMsg.LoadString( IDS_UPGRADE_DLOK );
	else if( ST_UPGRADE_CANNOTOPENFILE == wParam )
		strMsg.LoadString( IDS_UPGRADE_CANNOTOPENFILE );
	else
		ASSERT( FALSE );
		
	m_staticStatus.SetWindowText( strMsg );

	if( ST_UPGRADE_DLOK == wParam )
	{
		// Upgrade Now
		AfxGetMainFrame()->PostMessage( WM_CLOSE );
		ShellExecute( NULL, "open", AfxGetProfile().GetWorkDirectory()+szUpgradeFileName,
					  NULL, AfxGetProfile().GetWorkDirectory(), SW_SHOW );
	}

	m_btnOK.EnableWindow( TRUE );
	m_btnCancel.EnableWindow( TRUE );
	m_btnProxySet.EnableWindow( TRUE );

	CloseHandle(CUpgradeDlg::m_hEventKillUpgradeThread);
	CloseHandle(CUpgradeDlg::m_hEventUpgradeThreadKilled);

	return 1;
}

