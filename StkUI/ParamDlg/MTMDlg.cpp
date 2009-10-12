// MTMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MTMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMTMDlg dialog


CMTMDlg::CMTMDlg(CWnd* pParent, CMTM * pMTM )
{
	CParamDlg::CParamDlg(CMTMDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CMTMDlg)
	m_nDays = 0;
	m_nMADays = 0;
	//}}AFX_DATA_INIT

	m_pMTM	=	pMTM;
}


void CMTMDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMTMDlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	DDX_Text(pDX, IDC_MADAYS, m_nMADays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMTMDlg, CParamDlg)
	//{{AFX_MSG_MAP(CMTMDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMTMDlg message handlers

BOOL CMTMDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pMTM );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMTMDlg::OnCancel() 
{
}

void CMTMDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pMTM->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CMTMDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pMTM->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CMTMDlg::OnHelp() 
{
	ParamHelp( STT_MTM );
}

BOOL CMTMDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pMTM->IsValidParameters() )
		m_pMTM->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pMTM->m_nDays		=	m_nDays;
		m_pMTM->m_nMADays	=	m_nMADays;

		m_pMTM->m_itsGoldenFork	=	m_cmbGoldenFork.GetSelect();
		m_pMTM->m_itsDeadFork	=	m_cmbDeadFork.GetSelect();
		return m_pMTM->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_MTM) );

		m_nDays		=	m_pMTM->m_nDays;
		m_nMADays	=	m_pMTM->m_nMADays;

		m_cmbGoldenFork.ResetAndSelect( m_pMTM->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pMTM->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

