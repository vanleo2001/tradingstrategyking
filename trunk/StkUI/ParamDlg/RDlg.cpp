// RDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRDlg dialog


CRDlg::CRDlg(CWnd* pParent, CR * pR )
{
	CParamDlg::CParamDlg(CRDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CRDlg)
	m_nDays = 0;
	//}}AFX_DATA_INIT

	m_pR	=	pR;
}


void CRDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRDlg)
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRDlg, CParamDlg)
	//{{AFX_MSG_MAP(CRDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRDlg message handlers

BOOL CRDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pR );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRDlg::OnCancel() 
{
}

void CRDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pR->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CRDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pR->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CRDlg::OnHelp() 
{
	ParamHelp( STT_R );
}

BOOL CRDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pR->IsValidParameters() )
		m_pR->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pR->m_nDays		=	m_nDays;

		return m_pR->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_R) );

		m_nDays		=	m_pR->m_nDays;

		UpdateData( FALSE );
	}

	return TRUE;
}

