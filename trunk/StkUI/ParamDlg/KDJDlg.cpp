// KDJDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KDJDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKDJDlg dialog


CKDJDlg::CKDJDlg(CWnd* pParent, CKDJ * pKDJ )
{
	CParamDlg::CParamDlg(CKDJDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CKDJDlg)
	m_nRSVDays = 0;
	m_nKDays = 0;
	m_nDDays = 0;
	//}}AFX_DATA_INIT

	m_pKDJ	=	pKDJ;
}


void CKDJDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKDJDlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_RSVDAYS, m_nRSVDays);
	DDX_Text(pDX, IDC_KDAYS, m_nKDays);
	DDX_Text(pDX, IDC_DDAYS, m_nDDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKDJDlg, CParamDlg)
	//{{AFX_MSG_MAP(CKDJDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKDJDlg message handlers

BOOL CKDJDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pKDJ );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKDJDlg::OnCancel() 
{
}

void CKDJDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pKDJ->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CKDJDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pKDJ->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CKDJDlg::OnHelp() 
{
	ParamHelp( STT_KDJ );
}

BOOL CKDJDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pKDJ->IsValidParameters() )
		m_pKDJ->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		CButton * pbtn = (CButton *)GetDlgItem(IDC_3D2K);
		if( pbtn && pbtn->GetCheck() )
			m_pKDJ->m_nJ	=	CKDJ::mode3D2K;
		else
			m_pKDJ->m_nJ	=	CKDJ::mode3K2D;
		
		m_pKDJ->m_nRSVDays	=	m_nRSVDays;
		m_pKDJ->m_nKDays	=	m_nKDays;
		m_pKDJ->m_nDDays	=	m_nDDays;

		m_pKDJ->m_itsGoldenFork	=	m_cmbGoldenFork.GetSelect( );
		m_pKDJ->m_itsDeadFork	=	m_cmbDeadFork.GetSelect( );
		return m_pKDJ->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_KDJ) );

		CheckRadioButton( IDC_3K2D, IDC_3D2K, CKDJ::mode3D2K == m_pKDJ->m_nJ ? IDC_3D2K : IDC_3K2D );

		m_nRSVDays	=	m_pKDJ->m_nRSVDays;
		m_nKDays	=	m_pKDJ->m_nKDays;
		m_nDDays	=	m_pKDJ->m_nDDays;
		
		m_cmbGoldenFork.ResetAndSelect( m_pKDJ->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pKDJ->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

