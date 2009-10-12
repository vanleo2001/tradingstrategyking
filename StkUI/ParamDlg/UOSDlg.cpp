// UOSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UOSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUOSDlg dialog


CUOSDlg::CUOSDlg(CWnd* pParent, CUOS * pUOS )
{
	CParamDlg::CParamDlg(CUOSDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CUOSDlg)
	m_nDays1 = 0;
	m_nDays2 = 0;
	m_nDays3 = 0;
	m_nMADays = 0;
	//}}AFX_DATA_INIT

	m_pUOS	=	pUOS;
}


void CUOSDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUOSDlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS1, m_nDays1);
	DDX_Text(pDX, IDC_DAYS2, m_nDays2);
	DDX_Text(pDX, IDC_DAYS3, m_nDays3);
	DDX_Text(pDX, IDC_MADAYS, m_nMADays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUOSDlg, CParamDlg)
	//{{AFX_MSG_MAP(CUOSDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUOSDlg message handlers

BOOL CUOSDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pUOS );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUOSDlg::OnCancel() 
{
}

void CUOSDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pUOS->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CUOSDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pUOS->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CUOSDlg::OnHelp() 
{
	ParamHelp( STT_UOS );
}

BOOL CUOSDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pUOS->IsValidParameters() )
		m_pUOS->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pUOS->m_nDays1	=	m_nDays1;
		m_pUOS->m_nDays2	=	m_nDays2;
		m_pUOS->m_nDays3	=	m_nDays3;
		m_pUOS->m_nMADays	=	m_nMADays;

		m_pUOS->m_itsGoldenFork		=	m_cmbGoldenFork.GetSelect();
		m_pUOS->m_itsDeadFork		=	m_cmbDeadFork.GetSelect();
		return m_pUOS->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_UOS) );

		m_nDays1	=	m_pUOS->m_nDays1;
		m_nDays2	=	m_pUOS->m_nDays2;
		m_nDays3	=	m_pUOS->m_nDays3;
		m_nMADays	=	m_pUOS->m_nMADays;

		m_cmbGoldenFork.ResetAndSelect( m_pUOS->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pUOS->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

