// PVIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PVIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPVIDlg dialog


CPVIDlg::CPVIDlg(CWnd* pParent, CPVI * pPVI )
{
	CParamDlg::CParamDlg(CPVIDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CPVIDlg)
	m_nMADays = 0;
	//}}AFX_DATA_INIT

	m_pPVI	=	pPVI;
}


void CPVIDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPVIDlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_MADAYS, m_nMADays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPVIDlg, CParamDlg)
	//{{AFX_MSG_MAP(CPVIDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPVIDlg message handlers

BOOL CPVIDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pPVI );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPVIDlg::OnCancel() 
{
}

void CPVIDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pPVI->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CPVIDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pPVI->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CPVIDlg::OnHelp() 
{
	ParamHelp( STT_PVI );
}

BOOL CPVIDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pPVI->IsValidParameters() )
		m_pPVI->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pPVI->m_nMADays	=	m_nMADays;

		m_pPVI->m_itsGoldenFork		=	m_cmbGoldenFork.GetSelect();
		m_pPVI->m_itsDeadFork		=	m_cmbDeadFork.GetSelect();
		return m_pPVI->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_PVI) );

		m_nMADays		=	m_pPVI->m_nMADays;
		
		m_cmbGoldenFork.ResetAndSelect( m_pPVI->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pPVI->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

