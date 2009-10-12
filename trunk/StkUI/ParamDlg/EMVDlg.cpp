// EMVDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EMVDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEMVDlg dialog


CEMVDlg::CEMVDlg(CWnd* pParent, CEMV * pEMV )
{
	CParamDlg::CParamDlg(CEMVDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CEMVDlg)
	m_nDays = 0;
	m_nMADays = 0;
	//}}AFX_DATA_INIT

	m_pEMV	=	pEMV;
}


void CEMVDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEMVDlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	DDX_Text(pDX, IDC_MADAYS, m_nMADays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEMVDlg, CParamDlg)
	//{{AFX_MSG_MAP(CEMVDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEMVDlg message handlers

BOOL CEMVDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pEMV );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEMVDlg::OnCancel() 
{
}

void CEMVDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pEMV->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CEMVDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pEMV->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CEMVDlg::OnHelp() 
{
	ParamHelp( STT_EMV );
}

BOOL CEMVDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pEMV->IsValidParameters() )
		m_pEMV->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pEMV->m_nDays		=	m_nDays;
		m_pEMV->m_nMADays	=	m_nMADays;

		m_pEMV->m_itsGoldenFork	=	m_cmbGoldenFork.GetSelect();
		m_pEMV->m_itsDeadFork	=	m_cmbDeadFork.GetSelect();
		return m_pEMV->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_EMV) );

		m_nDays		=	m_pEMV->m_nDays;
		m_nMADays	=	m_pEMV->m_nMADays;

		m_cmbGoldenFork.ResetAndSelect( m_pEMV->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pEMV->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

