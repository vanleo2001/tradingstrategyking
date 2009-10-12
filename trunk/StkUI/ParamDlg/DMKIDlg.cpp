// DMKIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DMKIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDMKIDlg dialog


CDMKIDlg::CDMKIDlg(CWnd* pParent, CDMKI * pDMKI )
{
	CParamDlg::CParamDlg(CDMKIDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CDMKIDlg)
	m_nDays = 0;
	//}}AFX_DATA_INIT

	m_pDMKI	=	pDMKI;
}


void CDMKIDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDMKIDlg)
	DDX_Control(pDX, IDC_BOUGHT, m_cmbBought);
	DDX_Control(pDX, IDC_SOLD, m_cmbSold);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDMKIDlg, CParamDlg)
	//{{AFX_MSG_MAP(CDMKIDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMKIDlg message handlers

BOOL CDMKIDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pDMKI );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDMKIDlg::OnCancel() 
{
}

void CDMKIDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pDMKI->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CDMKIDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pDMKI->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CDMKIDlg::OnHelp() 
{
	ParamHelp( STT_DMKI );
}

BOOL CDMKIDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pDMKI->IsValidParameters() )
		m_pDMKI->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pDMKI->m_nDays		=	m_nDays;

		m_pDMKI->m_itsSold	=	m_cmbSold.GetSelect();
		m_pDMKI->m_itsBought	=	m_cmbBought.GetSelect();
		return m_pDMKI->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_DMKI) );

		m_nDays		=	m_pDMKI->m_nDays;

		m_cmbSold.ResetAndSelect( m_pDMKI->m_itsSold );
		m_cmbBought.ResetAndSelect( m_pDMKI->m_itsBought );
		UpdateData( FALSE );
	}

	return TRUE;
}

