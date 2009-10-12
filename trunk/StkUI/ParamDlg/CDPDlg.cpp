// CDPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CDPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCDPDlg dialog


CCDPDlg::CCDPDlg(CWnd* pParent, CCDP * pCDP )
{
	CParamDlg::CParamDlg(CCDPDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CCDPDlg)
	//}}AFX_DATA_INIT

	m_pCDP	=	pCDP;
}


void CCDPDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCDPDlg)
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCDPDlg, CParamDlg)
	//{{AFX_MSG_MAP(CCDPDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCDPDlg message handlers

BOOL CCDPDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pCDP );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCDPDlg::OnCancel() 
{
}

void CCDPDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pCDP->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CCDPDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pCDP->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CCDPDlg::OnHelp() 
{
	ParamHelp( STT_CDP );
}

BOOL CCDPDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pCDP->IsValidParameters() )
		m_pCDP->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		// 
		return m_pCDP->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_CDP) );

		// 
		UpdateData( FALSE );
	}

	return TRUE;
}

