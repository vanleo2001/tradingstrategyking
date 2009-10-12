// MACDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MACDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMACDDlg dialog


CMACDDlg::CMACDDlg(CWnd* pParent, CMACD * pMACD )
{
	CParamDlg::CParamDlg(CMACDDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CMACDDlg)
	m_nEMADays1 = 0;
	m_nEMADays2 = 0;
	m_nDIFDays = 0;
	//}}AFX_DATA_INIT

	m_pMACD	=	pMACD;
}


void CMACDDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMACDDlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_DEVIATEONTOP, m_cmbDeviateOnTop);
	DDX_Control(pDX, IDC_DEVIATEONBOTTOM, m_cmbDeviateOnBottom);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_EMADAYS1, m_nEMADays1);
	DDX_Text(pDX, IDC_EMADAYS2, m_nEMADays2);
	DDX_Text(pDX, IDC_DIFDAYS, m_nDIFDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMACDDlg, CParamDlg)
	//{{AFX_MSG_MAP(CMACDDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMACDDlg message handlers

BOOL CMACDDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pMACD );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMACDDlg::OnCancel() 
{
}

void CMACDDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pMACD->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CMACDDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pMACD->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CMACDDlg::OnHelp() 
{
	ParamHelp( STT_MACD );
}

BOOL CMACDDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pMACD->IsValidParameters() )
		m_pMACD->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );

		m_pMACD->m_nEMA1Days	=	m_nEMADays1;
		m_pMACD->m_nEMA2Days	=	m_nEMADays2;
		m_pMACD->m_nDIFDays		=	m_nDIFDays;

		m_pMACD->m_itsDeviateOnBottom	=	m_cmbDeviateOnBottom.GetSelect();
		m_pMACD->m_itsDeviateOnTop		=	m_cmbDeviateOnTop.GetSelect();
		m_pMACD->m_itsGoldenFork		=	m_cmbGoldenFork.GetSelect();
		m_pMACD->m_itsDeadFork			=	m_cmbDeadFork.GetSelect();
		return m_pMACD->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_MACD) );

		m_nEMADays1		=	m_pMACD->m_nEMA1Days;
		m_nEMADays2		=	m_pMACD->m_nEMA2Days;
		m_nDIFDays		=	m_pMACD->m_nDIFDays;

		m_cmbDeviateOnBottom.ResetAndSelect( m_pMACD->m_itsDeviateOnBottom );
		m_cmbDeviateOnTop.ResetAndSelect( m_pMACD->m_itsDeviateOnTop );
		m_cmbGoldenFork.ResetAndSelect( m_pMACD->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pMACD->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

