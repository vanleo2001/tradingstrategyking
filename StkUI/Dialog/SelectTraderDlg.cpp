// SelectTraderDlg.cpp : implementation file
//

#include "stdafx.h"

#include "SelectTraderDlg.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectTraderDlg dialog


CSelectTraderDlg::CSelectTraderDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CSelectTraderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectTraderDlg)
	m_strFile = _T("");
	//}}AFX_DATA_INIT

	m_strFile	=	AfxGetProfile().GetTrader( );
}


void CSelectTraderDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectTraderDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_FILE_EXPLORER, m_btnExplore);
	DDX_Text(pDX, IDC_FILE, m_strFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectTraderDlg, CXFDialog)
	//{{AFX_MSG_MAP(CSelectTraderDlg)
	ON_BN_CLICKED(IDC_FILE_EXPLORER, OnFileExplorer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectTraderDlg message handlers

BOOL CSelectTraderDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectTraderDlg::OnFileExplorer() 
{
	UpdateData( );

	CKSFileDialog dlg (TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING,
		"Stock Trader files (*.exe)|*.exe|All files (*.*)|*.*||", NULL);
	if( IDOK == dlg.DoModal() )
	{
		m_strFile	=	dlg.GetPathName();
		UpdateData( FALSE );
	}
}

void CSelectTraderDlg::OnOK() 
{
	UpdateData( );

	if( 0 != access(m_strFile,0) )
	{
		AfxMessageBox( IDS_SELECTTRADER_ERRFILE, MB_OK|MB_ICONINFORMATION );
		return;
	}

	AfxGetProfile().SetTrader( m_strFile );

	CXFDialog::OnOK();
}
