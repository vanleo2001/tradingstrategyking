// StartupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StartupDlg.h"
#include "ProxySetDlg.h"
#include "EditSrvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStartupDlg dialog


CStartupDlg::CStartupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_STARTUP/*CStartupDlg::IDD*/, pParent)
{
	//{{AFX_DATA_INIT(CStartupDlg)
	m_strUser = _T("");
	m_strPasswd = _T("");
	m_bSavePasswd = FALSE;
	//}}AFX_DATA_INIT
	m_pfnLoadProgram	=	NULL;
	m_bDisableOffline	=	FALSE;
}

BOOL CStartupDlg::SetBitmap( HBITMAP hbmp )
{
	return m_bitmap.Attach( hbmp );
}

void CStartupDlg::SetLoadProgramFunc( BOOL (CALLBACK *pfnLoadProgram)(HWND,int,int) )
{
	m_pfnLoadProgram	=	pfnLoadProgram;
}

void CStartupDlg::SetDisableOffline( BOOL bDisableOffline )
{
	m_bDisableOffline = bDisableOffline;
}

void CStartupDlg::SetProgress(int nPos)
{
	if( !::IsWindow(m_ctrlProgress.GetSafeHwnd()) )
		return;

	int	nLower = 0, nUpper = 0;
	m_ctrlProgress.GetRange( nLower, nUpper );

	if( nLower < nUpper && nPos >= nLower && nPos <= nUpper )
	{
		m_ctrlProgress.SetPos(nPos);
	}
	else
		ASSERT( FALSE );
}

void CStartupDlg::SetupComboServer( )
{
	m_comboServer.ResetContent();

	UINT nSelectedItem = 0;
	int nCurrentServer = AfxGetQSProfile().GetCurrentServerIndex();
	CQuoteServerArray & aServers = AfxGetQSProfile().GetQuoteServers();
	for( int i=0; i<aServers.GetSize(); i++ )
	{
		CQuoteServer & qs = aServers.ElementAt(i);
		int nItem = m_comboServer.AddString( qs.m_strName );
		m_comboServer.SetItemData( nItem, i );
		if( i == nCurrentServer )
			nSelectedItem = nItem;
	}

	m_comboServer.SetCurSel( nSelectedItem );
}

void CStartupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStartupDlg)
	DDX_Control(pDX, IDC_EDIT_USER, m_editUser);
	DDX_Control(pDX, IDC_EDIT_PASSWD, m_editPasswd);
	DDX_Control(pDX, IDC_CHECK_SAVEPASSWD, m_btnCheckSavePasswd);
	DDX_Control(pDX, IDC_MODIFY, m_btnModify);
	DDX_Control(pDX, IDC_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_ADD, m_btnAdd);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_PROXY_SET, m_btnProxy);
	DDX_Control(pDX, IDC_STATICINFO, m_staticInfo);
	DDX_Control(pDX, IDC_STARTUPPROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDC_STARTUPPICTURE, m_staticPicture);
	DDX_Control(pDX, IDC_OFFLINE, m_btnOffline);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_COMBO_SERVER, m_comboServer);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	DDV_MaxChars(pDX, m_strUser, 128);
	DDX_Text(pDX, IDC_EDIT_PASSWD, m_strPasswd);
	DDV_MaxChars(pDX, m_strPasswd, 128);
	DDX_Check(pDX, IDC_CHECK_SAVEPASSWD, m_bSavePasswd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStartupDlg, CDialog)
	//{{AFX_MSG_MAP(CStartupDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_SERVER, OnSelchangeComboServer)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_PROXY_SET, OnProxySet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_OFFLINE, OnOffline)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_STARTUP_PROGRESS, OnStartupProgress)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStartupDlg message handlers

LRESULT CStartupDlg::OnStartupProgress(WPARAM wParam, LPARAM lParam)
{
	// set message and enable buttons
	SetProgress( wParam );

	return 1;
}

int CStartupDlg::DoModal() 
{
	HINSTANCE hInstanceOld = AfxGetResourceHandle();
	AfxSetResourceHandle( AfxGetModuleHandle() );
	
	int nRet = CDialog::DoModal();
	
	AfxSetResourceHandle( hInstanceOld );
	return nRet;
}

BOOL CStartupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if( m_bDisableOffline )
		m_btnOffline.EnableWindow(FALSE);

	m_staticPicture.SetBitmap( (HBITMAP)m_bitmap.GetSafeHandle() );

	m_ctrlProgress.SetRange( 0, STKLIB_MAX_PROGRESS );
	m_ctrlProgress.SetPos( 0 );

	AfxGetQSProfile().LoadProfile();

	SetupComboServer( );
	OnSelchangeComboServer( );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStartupDlg::OnSelchangeComboServer() 
{
	CQuoteServer qs;
	int nSel = m_comboServer.GetCurSel();
	if( CB_ERR != nSel )
	{
		int i = m_comboServer.GetItemData(nSel);
		CQuoteServerArray & aServers = AfxGetQSProfile().GetQuoteServers();
		if( i >= 0 && i < aServers.GetSize() )
			qs = aServers.ElementAt(i);
	}

	m_strUser = qs.m_strUser;
	m_strPasswd = qs.m_strPasswd;
	m_bSavePasswd = qs.m_bSavePasswd;
	if( !m_bSavePasswd )
		m_strPasswd.Empty();
	UpdateData( FALSE );
}

void CStartupDlg::OnAdd() 
{
	CEditSrvDlg dlg;
	if( IDOK == dlg.DoModal() )
	{
		CQuoteServer qs;
		dlg.GetQuoteServer( qs );
		int nNew = AfxGetQSProfile().GetQuoteServers().Add( qs );
		AfxGetQSProfile().SetCurrentServer( nNew );

		SetupComboServer( );
		OnSelchangeComboServer( );

		AfxGetQSProfile().StoreProfile();
	}
}

void CStartupDlg::OnModify() 
{
	CQuoteServer qs;
	int nSel = m_comboServer.GetCurSel();
	if( CB_ERR == nSel )
	{
		AfxMessageBox( AfxModuleLoadString(IDS_SELECTSRV_NOSELECTED) );
		return;
	}

	int i = m_comboServer.GetItemData(nSel);
	CQuoteServerArray & aServers = AfxGetQSProfile().GetQuoteServers();
	if( i >= 0 && i < aServers.GetSize() )
		qs = aServers.ElementAt(i);

	CEditSrvDlg dlg;
	dlg.SetQuoteServer( qs );
	if( IDOK == dlg.DoModal() )
	{
		dlg.GetQuoteServer( qs );
		if( i >= 0 && i < aServers.GetSize() )
		{
			aServers.SetAt(i,qs);
			AfxGetQSProfile().SetCurrentServer(i);
		}

		SetupComboServer( );
		OnSelchangeComboServer( );

		AfxGetQSProfile().StoreProfile();
	}
}

void CStartupDlg::OnDelete() 
{
	int nSel = m_comboServer.GetCurSel();
	if( CB_ERR == nSel )
	{
		AfxMessageBox( AfxModuleLoadString(IDS_SELECTSRV_NOSELECTED) );
		return;
	}

	if( IDYES != AfxMessageBox( AfxModuleLoadString(IDS_SELECTSRV_DELCONFIRM), MB_YESNO ) )
		return;

	int i = m_comboServer.GetItemData(nSel);
	CQuoteServerArray & aServers = AfxGetQSProfile().GetQuoteServers();
	if( i >= 0 && i < aServers.GetSize() )
	{
		aServers.RemoveAt(i);
		AfxGetQSProfile().SetCurrentServer(i);
	}

	SetupComboServer( );
	OnSelchangeComboServer( );

	AfxGetQSProfile().StoreProfile();
}

void CStartupDlg::OnProxySet() 
{
	CProxySetDlg	dlg;
	dlg.DoModal();
}

UINT RefreshServers( LPVOID pParam )
{
	CNetDatabase * netdb = new CNetDatabase();
	if( NULL == netdb )	return FALSE;
	if( !netdb->SetRootPath( AfxGetProfile().GetSelfDBPath(), IStStore::dbtypeSelfDB ) )
	{
		delete netdb;
		AfxEndThread( 0, TRUE );
		return FALSE;
	}

	// refresh servers
	CString	strServer	=	AfxGetProfile().GetMainServer();
	CDownloadServer	server;
	if( !server.FromString( strServer ) )
	{
		delete netdb;
		AfxEndThread( 0, TRUE );
		return FALSE;
	}

	if( !netdb->SetServer( server.m_strAddress, server.m_nPort ) )
	{
		delete netdb;
		AfxEndThread( 0, TRUE );
		return FALSE;
	}

	// set access type
	if( !netdb->SetAccessType( AfxGetQSProfile().GetAccessType(), AfxGetQSProfile().GetProxyType(),
			AfxGetQSProfile().GetProxyAddress(), AfxGetQSProfile().GetProxyPort(),
			AfxGetQSProfile().GetProxyUser(), AfxGetQSProfile().GetProxyPasswd() ) )
	{
		delete netdb;
		AfxEndThread( 0, TRUE );
		return FALSE;
	}

	if( !netdb->Login(	AfxGetSView().GetS(), AfxGetSView().GetU(),
							NULL, AfxGetProfile().GetVersion() ) )
	{
		delete netdb;
		AfxEndThread( 0, TRUE );
		return FALSE;
	}

	// load server info
	if( netdb->NetloadServerInfo( NULL, NULL ) )
	{
		CServers &	servers	=	netdb->GetServers();
		servers.SaveToProfile();
	}

	if( netdb->NetloadPackageInfo( NULL, NULL ) && netdb->GetPackages().m_nDataVersion > AfxGetProfile().GetLocalDataVersion() )
	{
		netdb->NetloadCodetbl( NULL, NULL );
		netdb->NetloadBasetable( NULL, NULL, NULL, NULL, NULL );
		netdb->NetloadDRData( NULL, NULL, NULL, NULL, NULL );
		AfxGetProfile().SetLocalDataVersion( netdb->GetPackages().m_nDataVersion );
	}

	netdb->Logout();

	delete netdb;
	AfxEndThread( 0, TRUE );
	return TRUE;
}

void CStartupDlg::OnOK() 
{
	m_btnOK.EnableWindow( FALSE );
	m_btnOffline.EnableWindow( FALSE );

	UpdateData( );

	m_ctrlProgress.ShowWindow( SW_SHOW );

	int nSel = m_comboServer.GetCurSel();
	if( CB_ERR == nSel )
	{
		AfxMessageBox( AfxModuleLoadString(IDS_SELECTSRV_NOSELECTED) );
		m_btnOK.EnableWindow( TRUE );
		if( !m_bDisableOffline )
			m_btnOffline.EnableWindow( TRUE );
		m_ctrlProgress.ShowWindow( SW_HIDE );
		return;
	}

	int i = m_comboServer.GetItemData(nSel);
	CQuoteServerArray & aServers = AfxGetQSProfile().GetQuoteServers();
	if( i >= 0 && i < aServers.GetSize() )
	{
		AfxGetQSProfile().SetCurrentServer(i);
		CQuoteServer & qs = aServers.ElementAt(i);

		// Save
		qs.m_strUser = m_strUser;
		qs.m_strPasswd = m_strPasswd;
		qs.m_bSavePasswd = m_bSavePasswd;
		if( !qs.m_bSavePasswd )
			qs.m_strPasswd.Empty();

		// Load
		m_strAddress = qs.m_strAddress;
		m_nPort = qs.m_nPort;
	}
	AfxGetQSProfile().StoreProfile();

	m_staticInfo.SetWindowText( AfxModuleLoadString(IDS_STARTUP_CONNECTSERVER) );

	AfxBeginThread( RefreshServers, NULL, THREAD_PRIORITY_NORMAL);

	if( !AfxGetStkReceiver().NetEngineBeginWorking( m_strAddress, m_nPort, m_strUser, m_strPasswd ) )
	{
		m_staticInfo.SetWindowText( AfxModuleLoadString(IDS_STARTUP_CONNECTFAILED) );
		m_btnOK.EnableWindow( TRUE );
		if( !m_bDisableOffline )
			m_btnOffline.EnableWindow( TRUE );
		m_ctrlProgress.ShowWindow( SW_HIDE );
		return;
	}

	m_staticInfo.SetWindowText( AfxModuleLoadString(IDS_STARTUP_LOADPROGRAM) );
	ASSERT( m_pfnLoadProgram );
	if( AfxGetStkReceiver().m_bFirst && m_pfnLoadProgram &&
		!m_pfnLoadProgram( GetSafeHwnd(), 0, STKLIB_MAX_PROGRESS ) )
	{
		CDialog::OnCancel();
		return;
	}

	AfxGetStkReceiver().m_bFirst = FALSE;

	CDialog::OnOK();
}

void CStartupDlg::OnOffline() 
{
	m_btnOK.EnableWindow( FALSE );
	m_btnOffline.EnableWindow( FALSE );

	UpdateData( );

	m_ctrlProgress.ShowWindow( SW_SHOW );

	m_staticInfo.SetWindowText( AfxModuleLoadString(IDS_STARTUP_LOADPROGRAM) );
	ASSERT( m_pfnLoadProgram );
	if( AfxGetStkReceiver().m_bFirst && m_pfnLoadProgram &&
		!m_pfnLoadProgram( GetSafeHwnd(), 0, STKLIB_MAX_PROGRESS ) )
	{
		CDialog::OnCancel();
		return;
	}

	AfxGetStkReceiver().m_bFirst = FALSE;

	CDialog::OnOK();
}
