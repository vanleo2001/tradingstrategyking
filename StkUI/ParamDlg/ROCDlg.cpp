// ROCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ROCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CROCDlg dialog


CROCDlg::CROCDlg(CWnd* pParent, CROC * pROC )
{
	CParamDlg::CParamDlg(CROCDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CROCDlg)
	m_nDays = 0;
	m_nMADays = 0;
	//}}AFX_DATA_INIT

	m_pROC	=	pROC;
}


void CROCDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CROCDlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	DDX_Text(pDX, IDC_MADAYS, m_nMADays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CROCDlg, CParamDlg)
	//{{AFX_MSG_MAP(CROCDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CROCDlg message handlers

BOOL CROCDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pROC );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CROCDlg::OnCancel() 
{
}

void CROCDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pROC->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CROCDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pROC->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CROCDlg::OnHelp() 
{
	ParamHelp( STT_ROC );
}

BOOL CROCDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pROC->IsValidParameters() )
		m_pROC->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pROC->m_nDays		=	m_nDays;
		m_pROC->m_nMADays	=	m_nMADays;

		m_pROC->m_itsGoldenFork	=	m_cmbGoldenFork.GetSelect();
		m_pROC->m_itsDeadFork	=	m_cmbDeadFork.GetSelect();
		return m_pROC->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_ROC) );

		m_nDays		=	m_pROC->m_nDays;
		m_nMADays	=	m_pROC->m_nMADays;

		m_cmbGoldenFork.ResetAndSelect( m_pROC->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pROC->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

