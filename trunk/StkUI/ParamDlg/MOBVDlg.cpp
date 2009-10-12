// MOBVDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MOBVDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMOBVDlg dialog


CMOBVDlg::CMOBVDlg(CWnd* pParent, CMOBV * pMOBV )
{
	CParamDlg::CParamDlg(CMOBVDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CMOBVDlg)
	m_nDays1 = 0;
	m_nDays2 = 0;
	//}}AFX_DATA_INIT

	m_pMOBV	=	pMOBV;
}


void CMOBVDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMOBVDlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS1, m_nDays1);
	DDX_Text(pDX, IDC_DAYS2, m_nDays2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMOBVDlg, CParamDlg)
	//{{AFX_MSG_MAP(CMOBVDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMOBVDlg message handlers

BOOL CMOBVDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pMOBV );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMOBVDlg::OnCancel() 
{
}

void CMOBVDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pMOBV->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CMOBVDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pMOBV->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CMOBVDlg::OnHelp() 
{
	ParamHelp( STT_MOBV );
}

BOOL CMOBVDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pMOBV->IsValidParameters() )
		m_pMOBV->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pMOBV->m_nDays1	=	m_nDays1;
		m_pMOBV->m_nDays2	=	m_nDays2;

		m_pMOBV->m_itsGoldenFork	=	m_cmbGoldenFork.GetSelect();
		m_pMOBV->m_itsDeadFork		=	m_cmbDeadFork.GetSelect();
		return m_pMOBV->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_MOBV) );

		m_nDays1	=	m_pMOBV->m_nDays1;
		m_nDays2	=	m_pMOBV->m_nDays2;

		m_cmbGoldenFork.ResetAndSelect( m_pMOBV->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pMOBV->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

