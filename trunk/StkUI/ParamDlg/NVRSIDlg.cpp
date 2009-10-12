// NVRSIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NVRSIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNVRSIDlg dialog


CNVRSIDlg::CNVRSIDlg(CWnd* pParent, CNVRSI * pNVRSI )
{
	CParamDlg::CParamDlg(CNVRSIDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CNVRSIDlg)
	m_nDays = 0;
	//}}AFX_DATA_INIT

	m_pNVRSI	=	pNVRSI;
}


void CNVRSIDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNVRSIDlg)
	DDX_Control(pDX, IDC_BOUGHT, m_cmbBought);
	DDX_Control(pDX, IDC_SOLD, m_cmbSold);
	DDX_Control(pDX, IDC_DEVIATEONTOP, m_cmbDeviateOnTop);
	DDX_Control(pDX, IDC_DEVIATEONBOTTOM, m_cmbDeviateOnBottom);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNVRSIDlg, CParamDlg)
	//{{AFX_MSG_MAP(CNVRSIDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNVRSIDlg message handlers

BOOL CNVRSIDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pNVRSI );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNVRSIDlg::OnCancel() 
{
}

void CNVRSIDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pNVRSI->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CNVRSIDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pNVRSI->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CNVRSIDlg::OnHelp() 
{
	ParamHelp( STT_NVRSI );
}

BOOL CNVRSIDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pNVRSI->IsValidParameters() )
		m_pNVRSI->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pNVRSI->m_nDays	=	m_nDays;

		m_pNVRSI->m_itsDeviateOnBottom	=	m_cmbDeviateOnBottom.GetSelect();
		m_pNVRSI->m_itsDeviateOnTop		=	m_cmbDeviateOnTop.GetSelect();
		m_pNVRSI->m_itsSold				=	m_cmbSold.GetSelect();
		m_pNVRSI->m_itsBought			=	m_cmbBought.GetSelect();
		return m_pNVRSI->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_NVRSI) );

		m_nDays		=	m_pNVRSI->m_nDays;

		m_cmbDeviateOnBottom.ResetAndSelect( m_pNVRSI->m_itsDeviateOnBottom );
		m_cmbDeviateOnTop.ResetAndSelect( m_pNVRSI->m_itsDeviateOnTop );
		m_cmbSold.ResetAndSelect( m_pNVRSI->m_itsSold );
		m_cmbBought.ResetAndSelect( m_pNVRSI->m_itsBought );
		UpdateData( FALSE );
	}

	return TRUE;
}

