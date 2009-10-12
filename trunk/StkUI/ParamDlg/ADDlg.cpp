// ADDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ADDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADDlg dialog


CADDlg::CADDlg(CWnd* pParent, CAD * pAD )
{
	CParamDlg::CParamDlg(CADDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CADDlg)
	m_nDays = 0;
	//}}AFX_DATA_INIT

	m_pAD	=	pAD;
}


void CADDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CADDlg)
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CADDlg, CParamDlg)
	//{{AFX_MSG_MAP(CADDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADDlg message handlers

BOOL CADDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pAD );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CADDlg::OnCancel() 
{
}

void CADDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pAD->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CADDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pAD->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CADDlg::OnHelp() 
{
	ParamHelp( STT_AD );
}

BOOL CADDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pAD->IsValidParameters() )
		m_pAD->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pAD->m_nDays		=	m_nDays;
		return m_pAD->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_AD) );

		m_nDays		=	m_pAD->m_nDays;
		UpdateData( FALSE );
	}

	return TRUE;
}

