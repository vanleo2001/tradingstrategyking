// VRSIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VRSIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVRSIDlg dialog


CVRSIDlg::CVRSIDlg(CWnd* pParent, CVRSI * pVRSI )
{
	CParamDlg::CParamDlg(CVRSIDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CVRSIDlg)
	m_nDays = 0;
	//}}AFX_DATA_INIT

	m_pVRSI	=	pVRSI;
}


void CVRSIDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVRSIDlg)
	DDX_Control(pDX, IDC_BOUGHT, m_cmbBought);
	DDX_Control(pDX, IDC_SOLD, m_cmbSold);
	DDX_Control(pDX, IDC_DEVIATEONTOP, m_cmbDeviateOnTop);
	DDX_Control(pDX, IDC_DEVIATEONBOTTOM, m_cmbDeviateOnBottom);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVRSIDlg, CParamDlg)
	//{{AFX_MSG_MAP(CVRSIDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVRSIDlg message handlers

BOOL CVRSIDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pVRSI );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVRSIDlg::OnCancel() 
{
}

void CVRSIDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pVRSI->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CVRSIDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pVRSI->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CVRSIDlg::OnHelp() 
{
	ParamHelp( STT_VRSI );
}

BOOL CVRSIDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pVRSI->IsValidParameters() )
		m_pVRSI->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pVRSI->m_nDays	=	m_nDays;

		m_pVRSI->m_itsDeviateOnBottom	=	m_cmbDeviateOnBottom.GetSelect();
		m_pVRSI->m_itsDeviateOnTop		=	m_cmbDeviateOnTop.GetSelect();
		m_pVRSI->m_itsSold				=	m_cmbSold.GetSelect();
		m_pVRSI->m_itsBought			=	m_cmbBought.GetSelect();
		return m_pVRSI->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_VRSI) );

		m_nDays		=	m_pVRSI->m_nDays;

		m_cmbDeviateOnBottom.ResetAndSelect( m_pVRSI->m_itsDeviateOnBottom );
		m_cmbDeviateOnTop.ResetAndSelect( m_pVRSI->m_itsDeviateOnTop );
		m_cmbSold.ResetAndSelect( m_pVRSI->m_itsSold );
		m_cmbBought.ResetAndSelect( m_pVRSI->m_itsBought );
		UpdateData( FALSE );
	}

	return TRUE;
}

