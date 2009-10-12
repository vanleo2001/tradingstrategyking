// CIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCIDlg dialog


CCIDlg::CCIDlg(CWnd* pParent, CCI * pCI )
{
	CParamDlg::CParamDlg(CCIDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CCIDlg)
	//}}AFX_DATA_INIT

	m_pCI	=	pCI;
}


void CCIDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCIDlg)
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCIDlg, CParamDlg)
	//{{AFX_MSG_MAP(CCIDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCIDlg message handlers

BOOL CCIDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pCI );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCIDlg::OnCancel() 
{
}

void CCIDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pCI->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CCIDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pCI->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CCIDlg::OnHelp() 
{
	ParamHelp( STT_CI );
}

BOOL CCIDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pCI->IsValidParameters() )
		m_pCI->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		// 
		return m_pCI->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_CI) );

		// 
		UpdateData( FALSE );
	}

	return TRUE;
}

