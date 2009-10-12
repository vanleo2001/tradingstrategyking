// MFIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFIDlg dialog


CMFIDlg::CMFIDlg(CWnd* pParent, CMFI * pMFI )
{
	CParamDlg::CParamDlg(CMFIDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CMFIDlg)
	m_nDays = 0;
	//}}AFX_DATA_INIT

	m_pMFI	=	pMFI;
}


void CMFIDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMFIDlg)
	DDX_Control(pDX, IDC_SHORT, m_cmbShort);
	DDX_Control(pDX, IDC_LONG, m_cmbLong);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMFIDlg, CParamDlg)
	//{{AFX_MSG_MAP(CMFIDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFIDlg message handlers

BOOL CMFIDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pMFI );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMFIDlg::OnCancel() 
{
}

void CMFIDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pMFI->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CMFIDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pMFI->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CMFIDlg::OnHelp() 
{
	ParamHelp( STT_MFI );
}

BOOL CMFIDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pMFI->IsValidParameters() )
		m_pMFI->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pMFI->m_nDays		=	m_nDays;

		m_pMFI->m_itsLong	=	m_cmbLong.GetSelect();
		m_pMFI->m_itsShort	=	m_cmbShort.GetSelect();
		return m_pMFI->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_MFI) );

		m_nDays		=	m_pMFI->m_nDays;
		m_cmbLong.ResetAndSelect( m_pMFI->m_itsLong );
		m_cmbShort.ResetAndSelect( m_pMFI->m_itsShort );
		UpdateData( FALSE );
	}

	return TRUE;
}

