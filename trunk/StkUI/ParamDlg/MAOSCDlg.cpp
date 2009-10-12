// MAOSCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MAOSCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMAOSCDlg dialog


CMAOSCDlg::CMAOSCDlg(CWnd* pParent, CMAOSC * pMAOSC )
{
	CParamDlg::CParamDlg(CMAOSCDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CMAOSCDlg)
	m_nDays1 = 0;
	m_nDays2 = 0;
	m_nMADays = 0;
	//}}AFX_DATA_INIT

	m_pMAOSC	=	pMAOSC;
}


void CMAOSCDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMAOSCDlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS1, m_nDays1);
	DDX_Text(pDX, IDC_DAYS2, m_nDays2);
	DDX_Text(pDX, IDC_MADAYS, m_nMADays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMAOSCDlg, CParamDlg)
	//{{AFX_MSG_MAP(CMAOSCDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMAOSCDlg message handlers

BOOL CMAOSCDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pMAOSC );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMAOSCDlg::OnCancel() 
{
}

void CMAOSCDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pMAOSC->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CMAOSCDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pMAOSC->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CMAOSCDlg::OnHelp() 
{
	ParamHelp( STT_MAOSC );
}

BOOL CMAOSCDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pMAOSC->IsValidParameters() )
		m_pMAOSC->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pMAOSC->m_nDays1	=	m_nDays1;
		m_pMAOSC->m_nDays2	=	m_nDays2;
		m_pMAOSC->m_nMADays	=	m_nMADays;

		m_pMAOSC->m_itsGoldenFork		=	m_cmbGoldenFork.GetSelect();
		m_pMAOSC->m_itsDeadFork		=	m_cmbDeadFork.GetSelect();
		return m_pMAOSC->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_MAOSC) );

		m_nDays1	=	m_pMAOSC->m_nDays1;
		m_nDays2	=	m_pMAOSC->m_nDays2;
		m_nMADays	=	m_pMAOSC->m_nMADays;

		m_cmbGoldenFork.ResetAndSelect( m_pMAOSC->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pMAOSC->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

