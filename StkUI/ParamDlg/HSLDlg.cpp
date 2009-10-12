// HSLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHSLDlg dialog


CHSLDlg::CHSLDlg(CWnd* pParent, CHSL * pHSL )
{
	CParamDlg::CParamDlg(CHSLDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CHSLDlg)
	//}}AFX_DATA_INIT

	m_pHSL	=	pHSL;
}


void CHSLDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHSLDlg)
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHSLDlg, CParamDlg)
	//{{AFX_MSG_MAP(CHSLDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHSLDlg message handlers

BOOL CHSLDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pHSL );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHSLDlg::OnCancel() 
{
}

void CHSLDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pHSL->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CHSLDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pHSL->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CHSLDlg::OnHelp() 
{
	ParamHelp( STT_HSL );
}

BOOL CHSLDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pHSL->IsValidParameters() )
		m_pHSL->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		return m_pHSL->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_HSL) );

		UpdateData( FALSE );
	}

	return TRUE;
}

