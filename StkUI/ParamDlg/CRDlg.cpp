// CRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCRDlg dialog


CCRDlg::CCRDlg(CWnd* pParent, CCR * pCR )
{
	CParamDlg::CParamDlg(CCRDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CCRDlg)
	m_nDays = 0;
	m_nMADaysA = 0;
	m_nMADaysB = 0;
	m_nMADaysC = 0;
	m_nMADaysD = 0;
	//}}AFX_DATA_INIT

	m_pCR	=	pCR;
}


void CCRDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCRDlg)
	DDX_Control(pDX, IDC_BOUGHT, m_cmbBought);
	DDX_Control(pDX, IDC_SOLD, m_cmbSold);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	DDX_Text(pDX, IDC_MADAYSA, m_nMADaysA);
	DDX_Text(pDX, IDC_MADAYSB, m_nMADaysB);
	DDX_Text(pDX, IDC_MADAYSC, m_nMADaysC);
	DDX_Text(pDX, IDC_MADAYSD, m_nMADaysD);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCRDlg, CParamDlg)
	//{{AFX_MSG_MAP(CCRDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCRDlg message handlers

BOOL CCRDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pCR );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCRDlg::OnCancel() 
{
}

void CCRDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pCR->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CCRDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pCR->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CCRDlg::OnHelp() 
{
	ParamHelp( STT_CR );
}

BOOL CCRDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pCR->IsValidParameters() )
		m_pCR->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pCR->m_nDays		=	m_nDays;
		m_pCR->m_nMADaysA	=	m_nMADaysA;
		m_pCR->m_nMADaysB	=	m_nMADaysB;
		m_pCR->m_nMADaysC	=	m_nMADaysC;
		m_pCR->m_nMADaysD	=	m_nMADaysD;

		m_pCR->m_itsSold	=	m_cmbSold.GetSelect();
		m_pCR->m_itsBought	=	m_cmbBought.GetSelect();
		return m_pCR->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_CR) );

		m_nDays		=	m_pCR->m_nDays;
		m_nMADaysA	=	m_pCR->m_nMADaysA;
		m_nMADaysB	=	m_pCR->m_nMADaysB;
		m_nMADaysC	=	m_pCR->m_nMADaysC;
		m_nMADaysD	=	m_pCR->m_nMADaysD;

		m_cmbSold.ResetAndSelect( m_pCR->m_itsSold );
		m_cmbBought.ResetAndSelect( m_pCR->m_itsBought );
		UpdateData( FALSE );
	}

	return TRUE;
}

