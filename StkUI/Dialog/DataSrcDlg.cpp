// DataSrcDlg.cpp : implementation file
//

#include "stdafx.h"

#include "DataSrcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataSrcDlg dialog


CDataSrcDlg::CDataSrcDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CDataSrcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataSrcDlg)
	m_strQLPath = _T("");
	//}}AFX_DATA_INIT
}


void CDataSrcDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataSrcDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_QLTEST, m_btnQLTest);
	DDX_Control(pDX, IDC_QLEXPLORER, m_btnQLExplorer);
	DDX_Control(pDX, IDC_EDIT_QLPATH, m_editQLPath);
	DDX_Control(pDX, IDC_CHECK_QIANLONG, m_btnCheckQianlong);
	DDX_Text(pDX, IDC_EDIT_QLPATH, m_strQLPath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDataSrcDlg, CXFDialog)
	//{{AFX_MSG_MAP(CDataSrcDlg)
	ON_BN_CLICKED(IDC_CHECK_QIANLONG, OnCheckQianlong)
	ON_BN_CLICKED(IDC_QLEXPLORER, OnQlexplorer)
	ON_BN_CLICKED(IDC_QLTEST, OnQltest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataSrcDlg message handlers

BOOL CDataSrcDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString	strOldQLPath	=	AfxGetProfile().GetQianlongPath();
	if( strOldQLPath.GetLength() > 0 )
	{
		m_btnCheckQianlong.SetCheck( 1 );
		m_editQLPath.SetWindowText( strOldQLPath );
	}
	else
	{
		m_editQLPath.EnableWindow( FALSE );
		m_btnQLExplorer.EnableWindow( FALSE );
		m_btnQLTest.EnableWindow( FALSE );
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataSrcDlg::OnCheckQianlong() 
{
	// TODO: Add your control notification handler code here
	int	nCheck	=	m_btnCheckQianlong.GetCheck();
	m_editQLPath.EnableWindow( nCheck );
	m_btnQLExplorer.EnableWindow( nCheck );
	m_btnQLTest.EnableWindow( nCheck );
}

void CDataSrcDlg::OnQlexplorer() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( AfxDoSelectDirectory( m_strQLPath ) )
	{
		UpdateData( FALSE );

		CNetDatabase	testdb;
		if( !testdb.SetRootPath( m_strQLPath, IStStore::dbtypeQianlong ) )
		{
			AfxMessageBox( IDS_ERROR_QIANLONGPATH, MB_OK | MB_ICONINFORMATION );
		}
	}
}

void CDataSrcDlg::OnQltest() 
{
	// TODO: Add your control notification handler code here
	UpdateData( );

	CNetDatabase	testdb;
	if( !testdb.SetRootPath( m_strQLPath, IStStore::dbtypeQianlong ) )
	{
		AfxMessageBox( IDS_ERROR_QIANLONGPATH, MB_OK | MB_ICONINFORMATION );
	}
	else
	{
		AfxMessageBox( IDS_TESTOK_QIANLONGPATH, MB_OK | MB_ICONINFORMATION );
	}
}

void CDataSrcDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData( );

	int	nCheck	=	m_btnCheckQianlong.GetCheck();
	if( !nCheck )
		m_strQLPath = "";

	if( !m_strQLPath.IsEmpty() )
	{
		CNetDatabase	testdb;
		if( !testdb.SetRootPath( m_strQLPath, IStStore::dbtypeQianlong )
			&& IDNO == AfxMessageBox( IDS_CONFIRM_ERRORQIANLONGPATH, MB_YESNO ) )
		{
			m_editQLPath.SetFocus();
			return;
		}
	}

	CString	strOldQLPath	=	AfxGetProfile().GetQianlongPath();
	AfxGetProfile().SetQianlongPath( m_strQLPath );
	if( AfxReleaseDB( )	&& AfxInitializeDB( NULL, 0 ) )
	{
		AfxMessageBox( IDS_SUCCESS_QIANLONGPATH, MB_OK | MB_ICONINFORMATION );
		CXFDialog::OnOK();
	}
	else
	{
		AfxMessageBox( IDS_ERROR_QIANLONGPATH, MB_OK | MB_ICONINFORMATION );
		AfxGetProfile().SetQianlongPath( strOldQLPath );
		AfxReleaseDB( );

		char	szErr[1024];
		if( !AfxInitializeDB( szErr, sizeof(szErr) ) )	AfxMessageBox( szErr, MB_OK|MB_ICONINFORMATION );
		m_editQLPath.SetFocus();
		return;
	}
}

