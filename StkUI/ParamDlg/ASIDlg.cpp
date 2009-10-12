// ASIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ASIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CASIDlg dialog


CASIDlg::CASIDlg(CWnd* pParent, CASI * pASI )
{
	CParamDlg::CParamDlg(CASIDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CASIDlg)
	m_nDays = 0;
	//}}AFX_DATA_INIT

	m_pASI	=	pASI;
}


void CASIDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CASIDlg)
	DDX_Control(pDX, IDC_DEVIATEONTOP, m_cmbDeviateOnTop);
	DDX_Control(pDX, IDC_DEVIATEONBOTTOM, m_cmbDeviateOnBottom);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CASIDlg, CParamDlg)
	//{{AFX_MSG_MAP(CASIDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CASIDlg message handlers

BOOL CASIDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pASI );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CASIDlg::OnCancel() 
{
}

void CASIDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pASI->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CASIDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pASI->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CASIDlg::OnHelp() 
{
	ParamHelp( STT_ASI );
}

BOOL CASIDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pASI->IsValidParameters() )
		m_pASI->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pASI->m_nDays		=	m_nDays;

		m_pASI->m_itsDeviateOnBottom	=	m_cmbDeviateOnBottom.GetSelect();
		m_pASI->m_itsDeviateOnTop		=	m_cmbDeviateOnTop.GetSelect();
		return m_pASI->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_ASI) );

		m_nDays		=	m_pASI->m_nDays;

		m_cmbDeviateOnBottom.ResetAndSelect( m_pASI->m_itsDeviateOnBottom );
		m_cmbDeviateOnTop.ResetAndSelect( m_pASI->m_itsDeviateOnTop );
		UpdateData( FALSE );
	}

	return TRUE;
}

