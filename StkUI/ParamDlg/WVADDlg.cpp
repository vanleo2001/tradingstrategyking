// WVADDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WVADDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWVADDlg dialog


CWVADDlg::CWVADDlg(CWnd* pParent, CWVAD * pWVAD )
{
	CParamDlg::CParamDlg(CWVADDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CWVADDlg)
	m_nDays = 0;
	//}}AFX_DATA_INIT

	m_pWVAD	=	pWVAD;
}


void CWVADDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWVADDlg)
	DDX_Control(pDX, IDC_SHORT, m_cmbShort);
	DDX_Control(pDX, IDC_LONG, m_cmbLong);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWVADDlg, CParamDlg)
	//{{AFX_MSG_MAP(CWVADDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWVADDlg message handlers

BOOL CWVADDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pWVAD );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWVADDlg::OnCancel() 
{
}

void CWVADDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pWVAD->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CWVADDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pWVAD->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CWVADDlg::OnHelp() 
{
	ParamHelp( STT_WVAD );
}

BOOL CWVADDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pWVAD->IsValidParameters() )
		m_pWVAD->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pWVAD->m_nDays	=	m_nDays;
		
		m_pWVAD->m_itsLong	=	m_cmbLong.GetSelect();
		m_pWVAD->m_itsShort	=	m_cmbShort.GetSelect();
		return m_pWVAD->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_WVAD) );

		m_nDays		=	m_pWVAD->m_nDays;
		
		m_cmbLong.ResetAndSelect( m_pWVAD->m_itsLong );
		m_cmbShort.ResetAndSelect( m_pWVAD->m_itsShort );
		UpdateData( FALSE );
	}

	return TRUE;
}

