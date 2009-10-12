// DPODlg.cpp : implementation file
//

#include "stdafx.h"
#include "DPODlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDPODlg dialog


CDPODlg::CDPODlg(CWnd* pParent, CDPO * pDPO )
{
	CParamDlg::CParamDlg(CDPODlg::IDD, pParent);

	//{{AFX_DATA_INIT(CDPODlg)
	m_nDays = 0;
	m_nMADays = 0;
	//}}AFX_DATA_INIT

	m_pDPO	=	pDPO;
}


void CDPODlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDPODlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	DDX_Text(pDX, IDC_MADAYS, m_nMADays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDPODlg, CParamDlg)
	//{{AFX_MSG_MAP(CDPODlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDPODlg message handlers

BOOL CDPODlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pDPO );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDPODlg::OnCancel() 
{
}

void CDPODlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pDPO->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CDPODlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pDPO->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CDPODlg::OnHelp() 
{
	ParamHelp( STT_DPO );
}

BOOL CDPODlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pDPO->IsValidParameters() )
		m_pDPO->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pDPO->m_nDays		=	m_nDays;
		m_pDPO->m_nMADays	=	m_nMADays;

		m_pDPO->m_itsGoldenFork	=	m_cmbGoldenFork.GetSelect();
		m_pDPO->m_itsDeadFork	=	m_cmbDeadFork.GetSelect();
		return m_pDPO->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_DPO) );

		m_nDays		=	m_pDPO->m_nDays;
		m_nMADays	=	m_pDPO->m_nMADays;

		m_cmbGoldenFork.ResetAndSelect( m_pDPO->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pDPO->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

