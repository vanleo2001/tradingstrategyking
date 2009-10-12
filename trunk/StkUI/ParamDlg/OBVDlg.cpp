// OBVDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OBVDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COBVDlg dialog


COBVDlg::COBVDlg(CWnd* pParent, COBV * pOBV )
{
	CParamDlg::CParamDlg(COBVDlg::IDD, pParent);

	//{{AFX_DATA_INIT(COBVDlg)
	//}}AFX_DATA_INIT

	m_pOBV	=	pOBV;
}


void COBVDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COBVDlg)
	DDX_Control(pDX, IDC_DEVIATEONTOP, m_cmbDeviateOnTop);
	DDX_Control(pDX, IDC_DEVIATEONBOTTOM, m_cmbDeviateOnBottom);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COBVDlg, CParamDlg)
	//{{AFX_MSG_MAP(COBVDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COBVDlg message handlers

BOOL COBVDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pOBV );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COBVDlg::OnCancel() 
{
}

void COBVDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pOBV->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void COBVDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pOBV->SetDefaultParameters( );
	RefreshData( FALSE );
}

void COBVDlg::OnHelp() 
{
	ParamHelp( STT_OBV );
}

BOOL COBVDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pOBV->IsValidParameters() )
		m_pOBV->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pOBV->m_itsDeviateOnBottom	=	m_cmbDeviateOnBottom.GetSelect();
		m_pOBV->m_itsDeviateOnTop		=	m_cmbDeviateOnBottom.GetSelect();
		return m_pOBV->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_OBV) );

		m_cmbDeviateOnBottom.ResetAndSelect( m_pOBV->m_itsDeviateOnBottom );
		m_cmbDeviateOnTop.ResetAndSelect( m_pOBV->m_itsDeviateOnTop );
		UpdateData( FALSE );
	}

	return TRUE;
}

