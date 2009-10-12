// CYODlg.cpp : implementation file
//

#include "stdafx.h"
#include "CYODlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCYODlg dialog


CCYODlg::CCYODlg(CWnd* pParent, CCYO * pCYO )
{
	CParamDlg::CParamDlg(CCYODlg::IDD, pParent);

	//{{AFX_DATA_INIT(CCYODlg)
	//}}AFX_DATA_INIT

	m_pCYO	=	pCYO;
}


void CCYODlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCYODlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCYODlg, CParamDlg)
	//{{AFX_MSG_MAP(CCYODlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCYODlg message handlers

BOOL CCYODlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pCYO );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCYODlg::OnCancel() 
{
}

void CCYODlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pCYO->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CCYODlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pCYO->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CCYODlg::OnHelp() 
{
	ParamHelp( STT_CYO );
}

BOOL CCYODlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pCYO->IsValidParameters() )
		m_pCYO->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pCYO->m_itsGoldenFork	=	m_cmbGoldenFork.GetSelect( );
		m_pCYO->m_itsDeadFork	=	m_cmbDeadFork.GetSelect( );
		return m_pCYO->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_CYO) );

		m_cmbGoldenFork.ResetAndSelect( m_pCYO->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pCYO->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

