// SelectSrvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SelectSrvDlg.h"
#include "ProxySetDlg.h"
#include "EditSrvDlg.h"
#include "TWSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectSrvDlg dialog


CSelectSrvDlg::CSelectSrvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectSrvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectSrvDlg)
	m_strUser = _T("");
	m_strPasswd = _T("");
	m_bSavePasswd = FALSE;
	//}}AFX_DATA_INIT
}

void CSelectSrvDlg::SetupComboServer( )
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

void CSelectSrvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectSrvDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_COMBO_SERVER, m_comboServer);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	DDV_MaxChars(pDX, m_strUser, 128);
	DDX_Text(pDX, IDC_EDIT_PASSWD, m_strPasswd);
	DDV_MaxChars(pDX, m_strPasswd, 128);
	DDX_Check(pDX, IDC_CHECK_SAVEPASSWD, m_bSavePasswd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectSrvDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectSrvDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_SERVER, OnSelchangeComboServer)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_PROXY_SET, OnProxySet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectSrvDlg message handlers

int CSelectSrvDlg::DoModal() 
{
	HINSTANCE hInstanceOld = AfxGetResourceHandle();
	AfxSetResourceHandle( AfxGetModuleHandle() );
	
	int nRet = CDialog::DoModal();
	
	AfxSetResourceHandle( hInstanceOld );
	return nRet;
}

BOOL CSelectSrvDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	AfxGetQSProfile().LoadProfile();

	SetupComboServer( );
	OnSelchangeComboServer( );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectSrvDlg::OnSelchangeComboServer() 
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

void CSelectSrvDlg::OnAdd() 
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

void CSelectSrvDlg::OnModify() 
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

void CSelectSrvDlg::OnDelete() 
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

void CSelectSrvDlg::OnProxySet() 
{
	CProxySetDlg	dlg;
	dlg.DoModal();
}

void CSelectSrvDlg::OnOK() 
{
	m_btnOK.EnableWindow( FALSE );

	UpdateData( );

	int nSel = m_comboServer.GetCurSel();
	if( CB_ERR == nSel )
	{
		AfxMessageBox( AfxModuleLoadString(IDS_SELECTSRV_NOSELECTED) );
		m_btnOK.EnableWindow( TRUE );
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

	// Connect
	BOOL bSuccess = CTWSocket::GetInstance().BeginWorking( AfxGetQSProfile().GetAlias(GetServer()),
				GetPort(), GetUser(), GetPasswd() );
	if( !bSuccess )
	{
		AfxMessageBox( CTWSocket::GetInstance().GetLastErrorString(), MB_OK );
		return;
	}

	CDialog::OnOK();
}


