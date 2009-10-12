// DJDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DJDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDJDlg dialog


CDJDlg::CDJDlg(CWnd* pParent, CDJ * pDJ)
{
	CParamDlg::CParamDlg(CDJDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CDJDlg)
	//}}AFX_DATA_INIT
	m_pDJ	=	pDJ;
}


void CDJDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDJDlg)
	DDX_Control(pDX, IDC_DJSZN, m_comboDJSzn);
	DDX_Control(pDX, IDC_DJSHA, m_comboDJSha);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDJDlg, CParamDlg)
	//{{AFX_MSG_MAP(CDJDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDJDlg message handlers

BOOL CDJDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();

	ASSERT( m_pDJ );

	// TODO: Add extra initialization here
	m_comboDJSha.InitStocks( FALSE, FALSE, TRUE );
	m_comboDJSha.SetAutoHide( FALSE );

	m_comboDJSzn.InitStocks( FALSE, FALSE, TRUE );
	m_comboDJSzn.SetAutoHide( FALSE );

	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDJDlg::OnCancel() 
{
}

void CDJDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pDJ->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CDJDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pDJ->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CDJDlg::OnHelp() 
{
	ParamHelp( STT_DJ );
}

BOOL CDJDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pDJ->IsValidParameters() )
		m_pDJ->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );

		CString	strStockSha	=	m_comboDJSha.GetSelectedStockCode();
		CString	strStockSzn	=	m_comboDJSzn.GetSelectedStockCode();

		m_pDJ->SetDefaultParameters( );
		if( strStockSha.GetLength() > 0 )
			m_pDJ->m_strCodeSha	=	strStockSha;
		if( strStockSzn.GetLength() > 0 )
			m_pDJ->m_strCodeSzn	=	strStockSzn;

		return m_pDJ->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_DJ) );

		m_comboDJSha.SelectString( 0, m_pDJ->m_strCodeSha );
		m_comboDJSzn.SelectString( 0, m_pDJ->m_strCodeSzn );

		UpdateData( FALSE );
	}

	return TRUE;
}

