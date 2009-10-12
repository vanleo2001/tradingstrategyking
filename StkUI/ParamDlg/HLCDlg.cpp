// HLCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HLCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHLCDlg dialog


CHLCDlg::CHLCDlg(CWnd* pParent, CHLC * pHLC )
{
	CParamDlg::CParamDlg(CHLCDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CHLCDlg)
	m_nDays = 0;
	m_nMADays = 0;
	//}}AFX_DATA_INIT

	m_pHLC	=	pHLC;
}


void CHLCDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHLCDlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	DDX_Text(pDX, IDC_MADAYS, m_nMADays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHLCDlg, CParamDlg)
	//{{AFX_MSG_MAP(CHLCDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHLCDlg message handlers

BOOL CHLCDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pHLC );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHLCDlg::OnCancel() 
{
}

void CHLCDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pHLC->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CHLCDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pHLC->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CHLCDlg::OnHelp() 
{
	ParamHelp( STT_HLC );
}

BOOL CHLCDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pHLC->IsValidParameters() )
		m_pHLC->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pHLC->m_nDays		=	m_nDays;
		m_pHLC->m_nMADays	=	m_nMADays;

		m_pHLC->m_itsGoldenFork	=	m_cmbGoldenFork.GetSelect();
		m_pHLC->m_itsDeadFork	=	m_cmbDeadFork.GetSelect();
		return m_pHLC->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_HLC) );

		m_nDays		=	m_pHLC->m_nDays;
		m_nMADays	=	m_pHLC->m_nMADays;

		m_cmbGoldenFork.ResetAndSelect( m_pHLC->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pHLC->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

