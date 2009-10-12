// BIASDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BIASDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBIASDlg dialog


CBIASDlg::CBIASDlg(CWnd* pParent, CBIAS * pBIAS )
{
	CParamDlg::CParamDlg(CBIASDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CBIASDlg)
	m_nDays = 0;
	//}}AFX_DATA_INIT

	m_pBIAS	=	pBIAS;
}


void CBIASDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBIASDlg)
	DDX_Control(pDX, IDC_BOUGHT, m_cmbBought);
	DDX_Control(pDX, IDC_SOLD, m_cmbSold);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBIASDlg, CParamDlg)
	//{{AFX_MSG_MAP(CBIASDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBIASDlg message handlers

BOOL CBIASDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pBIAS );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBIASDlg::OnCancel() 
{
}

void CBIASDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pBIAS->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CBIASDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pBIAS->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CBIASDlg::OnHelp() 
{
	ParamHelp( STT_BIAS );
}

BOOL CBIASDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pBIAS->IsValidParameters() )
		m_pBIAS->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pBIAS->m_nDays		=	m_nDays;

		m_pBIAS->m_itsSold		=	m_cmbSold.GetSelect();
		m_pBIAS->m_itsBought	=	m_cmbBought.GetSelect();
		return m_pBIAS->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_BIAS) );

		m_nDays		=	m_pBIAS->m_nDays;

		m_cmbSold.ResetAndSelect( m_pBIAS->m_itsSold );
		m_cmbBought.ResetAndSelect( m_pBIAS->m_itsBought );
		UpdateData( FALSE );
	}

	return TRUE;
}

