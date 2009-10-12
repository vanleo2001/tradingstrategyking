// BBIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BBIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBBIDlg dialog


CBBIDlg::CBBIDlg(CWnd* pParent, CBBI * pBBI )
{
	CParamDlg::CParamDlg(CBBIDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CBBIDlg)
	m_nDays1 = 0;
	m_nDays2 = 0;
	m_nDays3 = 0;
	m_nDays4 = 0;
	//}}AFX_DATA_INIT

	m_pBBI	=	pBBI;
}


void CBBIDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBBIDlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS1, m_nDays1);
	DDX_Text(pDX, IDC_DAYS2, m_nDays2);
	DDX_Text(pDX, IDC_DAYS3, m_nDays3);
	DDX_Text(pDX, IDC_DAYS4, m_nDays4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBBIDlg, CParamDlg)
	//{{AFX_MSG_MAP(CBBIDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBBIDlg message handlers

BOOL CBBIDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pBBI );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBBIDlg::OnCancel() 
{
}

void CBBIDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pBBI->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CBBIDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pBBI->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CBBIDlg::OnHelp() 
{
	ParamHelp( STT_BBI );
}

BOOL CBBIDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pBBI->IsValidParameters() )
		m_pBBI->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pBBI->m_nMA1Days	=	m_nDays1;
		m_pBBI->m_nMA2Days	=	m_nDays2;
		m_pBBI->m_nMA3Days	=	m_nDays3;
		m_pBBI->m_nMA4Days	=	m_nDays4;

		m_pBBI->m_itsGoldenFork	=	m_cmbGoldenFork.GetSelect( );
		m_pBBI->m_itsDeadFork	=	m_cmbDeadFork.GetSelect( );
		return m_pBBI->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_BBI) );

		m_nDays1	=	m_pBBI->m_nMA1Days;
		m_nDays2	=	m_pBBI->m_nMA2Days;
		m_nDays3	=	m_pBBI->m_nMA3Days;
		m_nDays4	=	m_pBBI->m_nMA4Days;

		m_cmbGoldenFork.ResetAndSelect( m_pBBI->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pBBI->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

