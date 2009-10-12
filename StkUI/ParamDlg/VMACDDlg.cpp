// VMACDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VMACDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVMACDDlg dialog


CVMACDDlg::CVMACDDlg(CWnd* pParent, CVMACD * pVMACD )
{
	CParamDlg::CParamDlg(CVMACDDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CVMACDDlg)
	m_nEMADays1 = 0;
	m_nEMADays2 = 0;
	m_nDIFDays = 0;
	//}}AFX_DATA_INIT

	m_pVMACD	=	pVMACD;
}


void CVMACDDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVMACDDlg)
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


BEGIN_MESSAGE_MAP(CVMACDDlg, CParamDlg)
	//{{AFX_MSG_MAP(CVMACDDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVMACDDlg message handlers

BOOL CVMACDDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pVMACD );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVMACDDlg::OnCancel() 
{
}

void CVMACDDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pVMACD->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CVMACDDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pVMACD->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CVMACDDlg::OnHelp() 
{
	ParamHelp( STT_VMACD );
}

BOOL CVMACDDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pVMACD->IsValidParameters() )
		m_pVMACD->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pVMACD->m_nEMA1Days	=	m_nEMADays1;
		m_pVMACD->m_nEMA2Days	=	m_nEMADays2;
		m_pVMACD->m_nDIFDays		=	m_nDIFDays;

		m_pVMACD->m_itsDeviateOnBottom	=	m_cmbDeviateOnBottom.GetSelect();
		m_pVMACD->m_itsDeviateOnTop		=	m_cmbDeviateOnTop.GetSelect();
		m_pVMACD->m_itsGoldenFork		=	m_cmbGoldenFork.GetSelect();
		m_pVMACD->m_itsDeadFork			=	m_cmbDeadFork.GetSelect();
		return m_pVMACD->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_VMACD) );

		m_nEMADays1		=	m_pVMACD->m_nEMA1Days;
		m_nEMADays2		=	m_pVMACD->m_nEMA2Days;
		m_nDIFDays		=	m_pVMACD->m_nDIFDays;

		m_cmbDeviateOnBottom.ResetAndSelect( m_pVMACD->m_itsDeviateOnBottom );
		m_cmbDeviateOnTop.ResetAndSelect( m_pVMACD->m_itsDeviateOnTop );
		m_cmbGoldenFork.ResetAndSelect( m_pVMACD->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pVMACD->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

