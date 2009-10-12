// NVIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NVIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNVIDlg dialog


CNVIDlg::CNVIDlg(CWnd* pParent, CNVI * pNVI )
{
	CParamDlg::CParamDlg(CNVIDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CNVIDlg)
	m_nMADays = 0;
	//}}AFX_DATA_INIT

	m_pNVI	=	pNVI;
}


void CNVIDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNVIDlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_MADAYS, m_nMADays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNVIDlg, CParamDlg)
	//{{AFX_MSG_MAP(CNVIDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNVIDlg message handlers

BOOL CNVIDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pNVI );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNVIDlg::OnCancel() 
{
}

void CNVIDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pNVI->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CNVIDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pNVI->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CNVIDlg::OnHelp() 
{
	ParamHelp( STT_NVI );
}

BOOL CNVIDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pNVI->IsValidParameters() )
		m_pNVI->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pNVI->m_nMADays	=	m_nMADays;

		m_pNVI->m_itsGoldenFork		=	m_cmbGoldenFork.GetSelect();
		m_pNVI->m_itsDeadFork		=	m_cmbDeadFork.GetSelect();
		return m_pNVI->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_NVI) );

		m_nMADays		=	m_pNVI->m_nMADays;
		
		m_cmbGoldenFork.ResetAndSelect( m_pNVI->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pNVI->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

