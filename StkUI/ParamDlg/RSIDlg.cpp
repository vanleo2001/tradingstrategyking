// RSIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RSIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRSIDlg dialog


CRSIDlg::CRSIDlg(CWnd* pParent, CRSI * pRSI )
{
	CParamDlg::CParamDlg(CRSIDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CRSIDlg)
	m_nDays1 = 0;
	m_nDays2 = 0;
	//}}AFX_DATA_INIT

	m_pRSI	=	pRSI;
}


void CRSIDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRSIDlg)
	DDX_Control(pDX, IDC_SOLD, m_cmbSold);
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS1, m_nDays1);
	DDX_Text(pDX, IDC_DAYS2, m_nDays2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRSIDlg, CParamDlg)
	//{{AFX_MSG_MAP(CRSIDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRSIDlg message handlers

BOOL CRSIDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pRSI );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRSIDlg::OnCancel() 
{
}

void CRSIDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pRSI->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CRSIDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pRSI->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CRSIDlg::OnHelp() 
{
	ParamHelp( STT_RSI );
}

BOOL CRSIDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pRSI->IsValidParameters() )
		m_pRSI->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pRSI->m_adwDays.RemoveAll();
		if( m_nDays1 > 0 )	m_pRSI->m_adwDays.Add( m_nDays1 );
		if( m_nDays2 > 0 )	m_pRSI->m_adwDays.Add( m_nDays2 );

		m_pRSI->m_itsGoldenFork		=	m_cmbGoldenFork.GetSelect();
		m_pRSI->m_itsDeadFork		=	m_cmbDeadFork.GetSelect();
		m_pRSI->m_itsSold			=	m_cmbSold.GetSelect();
		return m_pRSI->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_RSI) );

		m_nDays1	=	m_nDays2	=	0;
		if( m_pRSI->m_adwDays.GetSize() > 0 )	m_nDays1	=	m_pRSI->m_adwDays.ElementAt(0);
		if( m_pRSI->m_adwDays.GetSize() > 1 )	m_nDays2	=	m_pRSI->m_adwDays.ElementAt(1);

		m_cmbGoldenFork.ResetAndSelect( m_pRSI->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pRSI->m_itsDeadFork );
		m_cmbSold.ResetAndSelect( m_pRSI->m_itsSold );
		UpdateData( FALSE );
	}

	return TRUE;
}

