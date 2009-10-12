// CVDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CVDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCVDlg dialog


CCVDlg::CCVDlg(CWnd* pParent, CCV * pCV )
{
	CParamDlg::CParamDlg(CCVDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CCVDlg)
	m_nMAHLDays = 0;
	m_nCVDays = 0;
	//}}AFX_DATA_INIT

	m_pCV	=	pCV;
}


void CCVDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCVDlg)
	DDX_Control(pDX, IDC_BOUGHT, m_cmbBought);
	DDX_Control(pDX, IDC_SOLD, m_cmbSold);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_MAHLDAYS, m_nMAHLDays);
	DDX_Text(pDX, IDC_CVDAYS, m_nCVDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCVDlg, CParamDlg)
	//{{AFX_MSG_MAP(CCVDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCVDlg message handlers

BOOL CCVDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pCV );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCVDlg::OnCancel() 
{
}

void CCVDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pCV->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CCVDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pCV->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CCVDlg::OnHelp() 
{
	ParamHelp( STT_CV );
}

BOOL CCVDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pCV->IsValidParameters() )
		m_pCV->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pCV->m_nMAHLDays	=	m_nMAHLDays;
		m_pCV->m_nCVDays	=	m_nCVDays;

		m_pCV->m_itsSold	=	m_cmbSold.GetSelect();
		m_pCV->m_itsBought	=	m_cmbBought.GetSelect();
		return m_pCV->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_CV) );

		m_nMAHLDays		=	m_pCV->m_nMAHLDays;
		m_nCVDays		=	m_pCV->m_nCVDays;

		m_cmbSold.ResetAndSelect( m_pCV->m_itsSold );
		m_cmbBought.ResetAndSelect( m_pCV->m_itsBought );
		UpdateData( FALSE );
	}

	return TRUE;
}

