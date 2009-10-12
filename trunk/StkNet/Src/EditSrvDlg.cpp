// EditSrvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EditSrvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditSrvDlg dialog


CEditSrvDlg::CEditSrvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditSrvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditSrvDlg)
	m_strName = _T("");
	m_strAddress = _T("");
	m_nPort = 0;
	m_strUser = _T("");
	m_strPasswd = _T("");
	m_bSavePasswd = FALSE;
	//}}AFX_DATA_INIT
	m_nPort	=	8001;
	m_bSavePasswd = TRUE;
}

void CEditSrvDlg::SetQuoteServer( CQuoteServer & qs )
{
	m_strName = qs.m_strName;
	m_strAddress = qs.m_strAddress;
	m_nPort = qs.m_nPort;
	m_strUser = qs.m_strUser;
	m_strPasswd = qs.m_strPasswd;
	m_bSavePasswd = qs.m_bSavePasswd;
	if( !m_bSavePasswd )
		m_strPasswd.Empty();
}

void CEditSrvDlg::GetQuoteServer( CQuoteServer & qs )
{
	qs.m_strName = m_strName;
	qs.m_strAddress = m_strAddress;
	qs.m_nPort = m_nPort;
	qs.m_strUser = m_strUser;
	qs.m_strPasswd = m_strPasswd;
	qs.m_bSavePasswd = m_bSavePasswd;
	if( !qs.m_bSavePasswd )
		qs.m_strPasswd.Empty();
}

void CEditSrvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditSrvDlg)
	DDX_Control(pDX, IDC_EDIT_USER, m_editUser);
	DDX_Control(pDX, IDC_EDIT_PORT, m_editPort);
	DDX_Control(pDX, IDC_EDIT_PASSWD, m_editPasswd);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_editAddress);
	DDX_Control(pDX, IDC_CHECK_SAVEPASSWD, m_btnCheckSavePasswd);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 128);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_strAddress);
	DDV_MaxChars(pDX, m_strAddress, 256);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	DDV_MaxChars(pDX, m_strUser, 128);
	DDX_Text(pDX, IDC_EDIT_PASSWD, m_strPasswd);
	DDV_MaxChars(pDX, m_strPasswd, 128);
	DDX_Check(pDX, IDC_CHECK_SAVEPASSWD, m_bSavePasswd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditSrvDlg, CDialog)
	//{{AFX_MSG_MAP(CEditSrvDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditSrvDlg message handlers

int CEditSrvDlg::DoModal() 
{
	HINSTANCE hInstanceOld = AfxGetResourceHandle();
	AfxSetResourceHandle( AfxGetModuleHandle() );
	
	int nRet = CDialog::DoModal();
	
	AfxSetResourceHandle( hInstanceOld );
	return nRet;
}

BOOL CEditSrvDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditSrvDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData( );
	
	if( m_strAddress.IsEmpty() )
	{
		AfxMessageBox( AfxModuleLoadString(IDS_EDITSRV_NOADDRESS) );
		return;
	}

	if( m_strName.IsEmpty() )
	{
		m_strName = m_strAddress;
	}

	CDialog::OnOK();
}

