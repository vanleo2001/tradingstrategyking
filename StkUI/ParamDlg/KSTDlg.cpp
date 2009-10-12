// KSTDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KSTDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKSTDlg dialog


CKSTDlg::CKSTDlg(CWnd* pParent, CKST * pKST )
{
	CParamDlg::CParamDlg(CKSTDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CKSTDlg)
	m_nROCDays1 = 0;
	m_nROCDays2 = 0;
	m_nROCDays3 = 0;
	m_nROCDays4 = 0;
	m_nMADays = 0;
	//}}AFX_DATA_INIT

	m_pKST	=	pKST;
}


void CKSTDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKSTDlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS1, m_nROCDays1);
	DDX_Text(pDX, IDC_DAYS2, m_nROCDays2);
	DDX_Text(pDX, IDC_DAYS3, m_nROCDays3);
	DDX_Text(pDX, IDC_DAYS4, m_nROCDays4);
	DDX_Text(pDX, IDC_MADAYS, m_nMADays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKSTDlg, CParamDlg)
	//{{AFX_MSG_MAP(CKSTDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKSTDlg message handlers

BOOL CKSTDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pKST );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKSTDlg::OnCancel() 
{
}

void CKSTDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pKST->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CKSTDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pKST->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CKSTDlg::OnHelp() 
{
	ParamHelp( STT_KST );
}

BOOL CKSTDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pKST->IsValidParameters() )
		m_pKST->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pKST->m_adwROCDays.RemoveAll();
		if( m_nROCDays1 > 0 )	m_pKST->m_adwROCDays.Add( m_nROCDays1 );
		if( m_nROCDays2 > 0 )	m_pKST->m_adwROCDays.Add( m_nROCDays2 );
		if( m_nROCDays3 > 0 )	m_pKST->m_adwROCDays.Add( m_nROCDays3 );
		if( m_nROCDays4 > 0 )	m_pKST->m_adwROCDays.Add( m_nROCDays4 );
		
		m_pKST->m_nMADays	=	m_nMADays;

		m_pKST->m_itsGoldenFork	=	m_cmbGoldenFork.GetSelect();
		m_pKST->m_itsDeadFork	=	m_cmbDeadFork.GetSelect();
		return m_pKST->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_KST) );

		m_nROCDays1	=	m_nROCDays2	=	m_nROCDays3	=	m_nROCDays4	=	0;
		if( m_pKST->m_adwROCDays.GetSize() > 0 )	m_nROCDays1		=	m_pKST->m_adwROCDays.ElementAt(0);
		if( m_pKST->m_adwROCDays.GetSize() > 1 )	m_nROCDays2		=	m_pKST->m_adwROCDays.ElementAt(1);
		if( m_pKST->m_adwROCDays.GetSize() > 2 )	m_nROCDays3		=	m_pKST->m_adwROCDays.ElementAt(2);
		if( m_pKST->m_adwROCDays.GetSize() > 3 )	m_nROCDays4		=	m_pKST->m_adwROCDays.ElementAt(3);
		
		m_nMADays	=	m_pKST->m_nMADays;

		m_cmbGoldenFork.ResetAndSelect( m_pKST->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pKST->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

