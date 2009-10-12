// PVDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PVDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPVDlg dialog


CPVDlg::CPVDlg(CWnd* pParent, CPV * pPV)
{
	CParamDlg::CParamDlg(CPVDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CPVDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pPV	=	pPV;
}


void CPVDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPVDlg)
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPVDlg, CParamDlg)
	//{{AFX_MSG_MAP(CPVDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPVDlg message handlers

BOOL CPVDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();

	ASSERT( m_pPV );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPVDlg::OnCancel() 
{
}

void CPVDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pPV->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CPVDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pPV->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CPVDlg::OnHelp() 
{
	ParamHelp( STT_PV );
}

BOOL CPVDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pPV->IsValidParameters() )
		m_pPV->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_PV) );
		UpdateData( FALSE );
	}

	return TRUE;
}

