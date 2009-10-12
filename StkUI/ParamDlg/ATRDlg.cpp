// ATRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ATRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CATRDlg dialog


CATRDlg::CATRDlg(CWnd* pParent, CATR * pATR )
{
	CParamDlg::CParamDlg(CATRDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CATRDlg)
	m_nDays = 0;
	//}}AFX_DATA_INIT

	m_pATR	=	pATR;
}


void CATRDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CATRDlg)
	DDX_Control(pDX, IDC_BOUGHT, m_cmbBought);
	DDX_Control(pDX, IDC_SOLD, m_cmbSold);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CATRDlg, CParamDlg)
	//{{AFX_MSG_MAP(CATRDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CATRDlg message handlers

BOOL CATRDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pATR );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CATRDlg::OnCancel() 
{
}

void CATRDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pATR->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CATRDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pATR->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CATRDlg::OnHelp() 
{
	ParamHelp( STT_ATR );
}

BOOL CATRDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pATR->IsValidParameters() )
		m_pATR->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_nDays		=	m_pATR->m_nDays;

		m_cmbSold.ResetAndSelect( m_pATR->m_itsSold );
		m_cmbBought.ResetAndSelect( m_pATR->m_itsBought );
		return m_pATR->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_ATR) );

		m_nDays		=	m_pATR->m_nDays;

		m_cmbSold.ResetAndSelect( m_pATR->m_itsSold );
		m_cmbBought.ResetAndSelect( m_pATR->m_itsBought );
		UpdateData( FALSE );
	}

	return TRUE;
}

