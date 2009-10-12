// ProxySetDlg.cpp : implementation file
//

#include "stdafx.h"

#include "ProxySetDlg.h"
#include "ProxySocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProxySetDlg dialog

CProxySetDlg::CProxySetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProxySetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProxySetDlg)
	m_strProxyAddress = _T("");
	m_nProxyPort = 0;
	m_strProxyUser = _T("");
	m_strProxyPasswd = _T("");
	//}}AFX_DATA_INIT
}

void CProxySetDlg::EnableWindows( BOOL bEnable )
{
	m_staticProxyAddress.EnableWindow( bEnable );
	m_editProxyAddress.EnableWindow( bEnable );

	m_staticProxyPort.EnableWindow( bEnable );
	m_editProxyPort.EnableWindow( bEnable );

	m_staticProxyUser.EnableWindow( bEnable );
	m_editProxyUser.EnableWindow( bEnable );

	m_staticProxyPasswd.EnableWindow( bEnable );
	m_editProxyPasswd.EnableWindow( bEnable );

	CWnd * pWnd = GetDlgItem( IDC_PROXY_SOCKS4 );
	if( pWnd ) pWnd->EnableWindow( bEnable );

	pWnd = GetDlgItem( IDC_PROXY_SOCKS5 );
	if( pWnd ) pWnd->EnableWindow( bEnable );

	pWnd = GetDlgItem( IDC_PROXY_HTTP );
	if( pWnd ) pWnd->EnableWindow( bEnable );
}

int CProxySetDlg::DoModal()
{
	HINSTANCE hInstanceOld = AfxGetResourceHandle();
	AfxSetResourceHandle( AfxGetModuleHandle() );
	
	int nRet = CDialog::DoModal();
	
	AfxSetResourceHandle( hInstanceOld );
	return nRet;
}

void CProxySetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProxySetDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_EDIT_PROXYPASSWD, m_editProxyPasswd);
	DDX_Control(pDX, IDC_EDIT_PROXYUSER, m_editProxyUser);
	DDX_Control(pDX, IDC_STATIC_PROXYPASSWD, m_staticProxyPasswd);
	DDX_Control(pDX, IDC_STATIC_PROXYUSER, m_staticProxyUser);
	DDX_Control(pDX, IDC_EDIT_PROXYPORT, m_editProxyPort);
	DDX_Control(pDX, IDC_EDIT_PROXYADDRESS, m_editProxyAddress);
	DDX_Control(pDX, IDC_STATIC_PROXYPORT, m_staticProxyPort);
	DDX_Control(pDX, IDC_STATIC_PROXYADDRESS, m_staticProxyAddress);
	DDX_Text(pDX, IDC_EDIT_PROXYADDRESS, m_strProxyAddress);
	DDX_Text(pDX, IDC_EDIT_PROXYPORT, m_nProxyPort);
	DDV_MinMaxUInt(pDX, m_nProxyPort, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_PROXYUSER, m_strProxyUser);
	DDX_Text(pDX, IDC_EDIT_PROXYPASSWD, m_strProxyPasswd);
	DDV_MaxChars(pDX, m_strProxyAddress, CProxySocket::MaxLenAddress);
	DDV_MaxChars(pDX, m_strProxyUser, CProxySocket::MaxLenAddress);
	DDV_MaxChars(pDX, m_strProxyPasswd, CProxySocket::MaxLenAddress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProxySetDlg, CDialog)
	//{{AFX_MSG_MAP(CProxySetDlg)
	ON_BN_CLICKED(IDC_DIRECT, OnDirect)
	ON_BN_CLICKED(IDC_PRECONFIG, OnPreconfig)
	ON_BN_CLICKED(IDC_PROXY, OnProxy)
	ON_BN_CLICKED(IDC_PROXY_SOCKS4, OnProxySocks4)
	ON_BN_CLICKED(IDC_PROXY_SOCKS5, OnProxySocks5)
	ON_BN_CLICKED(IDC_PROXY_HTTP, OnProxyHttp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProxySetDlg message handlers

BOOL CProxySetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int nAccessType		=	AfxGetQSProfile().GetAccessType();
	int nProxyType		=	AfxGetQSProfile().GetProxyType();
	m_strProxyAddress	=	AfxGetQSProfile().GetProxyAddress();
	m_nProxyPort		=	AfxGetQSProfile().GetProxyPort();
	m_strProxyUser		=	AfxGetQSProfile().GetProxyUser();
	m_strProxyPasswd	=	AfxGetQSProfile().GetProxyPasswd();

	UpdateData( FALSE );

	if( CProxySocket::TypeSocks4 == nProxyType )
	{
		CheckRadioButton( IDC_PROXY_SOCKS4, IDC_PROXY_HTTP, IDC_PROXY_SOCKS4 );
		OnProxySocks4();
	}
	else if( CProxySocket::TypeSocks5 == nProxyType )
	{
		CheckRadioButton( IDC_PROXY_SOCKS4, IDC_PROXY_HTTP, IDC_PROXY_SOCKS5 );
		OnProxySocks5();
	}
	else
	{
		CheckRadioButton( IDC_PROXY_SOCKS4, IDC_PROXY_HTTP, IDC_PROXY_HTTP );
		OnProxyHttp();
	}

	if( INTERNET_OPEN_TYPE_DIRECT == nAccessType )
	{
		CheckRadioButton( IDC_DIRECT, IDC_PROXY, IDC_DIRECT );
		OnDirect();
	}
	else if( INTERNET_OPEN_TYPE_PROXY == nAccessType )
	{
		CheckRadioButton( IDC_DIRECT, IDC_PROXY, IDC_PROXY );
		OnProxy();
	}
	else
	{
		CheckRadioButton( IDC_DIRECT, IDC_PROXY, IDC_PRECONFIG );
		OnPreconfig( );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProxySetDlg::OnDirect() 
{
	EnableWindows( FALSE );
}

void CProxySetDlg::OnPreconfig() 
{
	EnableWindows( FALSE );
}

void CProxySetDlg::OnProxy() 
{
	EnableWindows( TRUE );

	CButton * pbtnProxySocks4 = (CButton *)GetDlgItem(IDC_PROXY_SOCKS4);
	if( pbtnProxySocks4 && pbtnProxySocks4->GetCheck() )
	{
		m_staticProxyUser.EnableWindow( FALSE );
		m_editProxyUser.EnableWindow( FALSE );

		m_staticProxyPasswd.EnableWindow( FALSE );
		m_editProxyPasswd.EnableWindow( FALSE );
	}
}

void CProxySetDlg::OnProxySocks4() 
{
	EnableWindows( TRUE );

	m_staticProxyUser.EnableWindow( FALSE );
	m_editProxyUser.EnableWindow( FALSE );

	m_staticProxyPasswd.EnableWindow( FALSE );
	m_editProxyPasswd.EnableWindow( FALSE );
}

void CProxySetDlg::OnProxySocks5() 
{
	EnableWindows( TRUE );
}

void CProxySetDlg::OnProxyHttp() 
{
	EnableWindows( TRUE );
}

void CProxySetDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData( );

	int nAccessType = INTERNET_OPEN_TYPE_PRECONFIG;

	CButton * pbtnDirect	= (CButton *)GetDlgItem( IDC_DIRECT );
	// CButton * pbtnPreconfig	= (CButton *)GetDlgItem( IDC_PRECONFIG );
	CButton * pbtnProxy		= (CButton *)GetDlgItem( IDC_PROXY );

	if( pbtnDirect && pbtnDirect->GetCheck() )
		nAccessType = INTERNET_OPEN_TYPE_DIRECT;
	else if( pbtnProxy && pbtnProxy->GetCheck() )
		nAccessType = INTERNET_OPEN_TYPE_PROXY;

	int nProxyType = CProxySocket::TypeHTTP;

	CButton * pbtnProxySocks4	= (CButton *)GetDlgItem( IDC_PROXY_SOCKS4 );
	CButton * pbtnProxySocks5	= (CButton *)GetDlgItem( IDC_PROXY_SOCKS5 );
	// CButton * pbtnProxyHttp		= (CButton *)GetDlgItem( IDC_PROXY_HTTP );

	if( pbtnProxySocks4 && pbtnProxySocks4->GetCheck() )
		nProxyType = CProxySocket::TypeSocks4;
	else if( pbtnProxySocks5 && pbtnProxySocks5->GetCheck() )
		nProxyType = CProxySocket::TypeSocks5;

	AfxGetQSProfile().SetAccessType( nAccessType );
	AfxGetQSProfile().SetProxyType( nProxyType );
	AfxGetQSProfile().SetProxyAddress( m_strProxyAddress );
	AfxGetQSProfile().SetProxyPort( m_nProxyPort );
	AfxGetQSProfile().SetProxyUser( m_strProxyUser );
	AfxGetQSProfile().SetProxyPasswd( m_strProxyPasswd );

	AfxGetQSProfile().StoreProfile();

	CDialog::OnOK();
}
