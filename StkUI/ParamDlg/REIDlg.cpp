// REIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "REIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CREIDlg dialog


CREIDlg::CREIDlg(CWnd* pParent, CREI * pREI )
{
	CParamDlg::CParamDlg(CREIDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CREIDlg)
	m_nDays = 0;
	//}}AFX_DATA_INIT

	m_pREI	=	pREI;
}


void CREIDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CREIDlg)
	DDX_Control(pDX, IDC_SHORT, m_cmbShort);
	DDX_Control(pDX, IDC_LONG, m_cmbLong);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CREIDlg, CParamDlg)
	//{{AFX_MSG_MAP(CREIDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CREIDlg message handlers

BOOL CREIDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pREI );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CREIDlg::OnCancel() 
{
}

void CREIDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pREI->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CREIDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pREI->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CREIDlg::OnHelp() 
{
	ParamHelp( STT_REI );
}

BOOL CREIDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pREI->IsValidParameters() )
		m_pREI->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pREI->m_nDays		=	m_nDays;

		m_pREI->m_itsLong	=	m_cmbLong.GetSelect();
		m_pREI->m_itsShort	=	m_cmbShort.GetSelect();
		return m_pREI->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_REI) );

		m_nDays		=	m_pREI->m_nDays;

		m_cmbLong.ResetAndSelect( m_pREI->m_itsLong );
		m_cmbShort.ResetAndSelect( m_pREI->m_itsShort );
		UpdateData( FALSE );
	}

	return TRUE;
}

