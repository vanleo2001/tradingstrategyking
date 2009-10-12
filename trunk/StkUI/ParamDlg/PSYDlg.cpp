// PSYDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PSYDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPSYDlg dialog


CPSYDlg::CPSYDlg(CWnd* pParent, CPSY * pPSY )
{
	CParamDlg::CParamDlg(CPSYDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CPSYDlg)
	m_nDays = 0;
	//}}AFX_DATA_INIT

	m_pPSY	=	pPSY;
}


void CPSYDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPSYDlg)
	DDX_Control(pDX, IDC_BOUGHT, m_cmbBought);
	DDX_Control(pDX, IDC_SOLD, m_cmbSold);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPSYDlg, CParamDlg)
	//{{AFX_MSG_MAP(CPSYDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPSYDlg message handlers

BOOL CPSYDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pPSY );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPSYDlg::OnCancel() 
{
}

void CPSYDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pPSY->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CPSYDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pPSY->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CPSYDlg::OnHelp() 
{
	ParamHelp( STT_PSY );
}

BOOL CPSYDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pPSY->IsValidParameters() )
		m_pPSY->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pPSY->m_nDays		=	m_nDays;

		m_pPSY->m_itsSold	=	m_cmbSold.GetSelect();
		m_pPSY->m_itsBought	=	m_cmbBought.GetSelect();
		return m_pPSY->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_PSY) );

		m_nDays		=	m_pPSY->m_nDays;

		m_cmbSold.ResetAndSelect( m_pPSY->m_itsSold );
		m_cmbBought.ResetAndSelect( m_pPSY->m_itsBought );
		UpdateData( FALSE );
	}

	return TRUE;
}

