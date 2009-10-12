// ARBRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ARBRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CARBRDlg dialog


CARBRDlg::CARBRDlg(CWnd* pParent, CARBR * pARBR )
{
	CParamDlg::CParamDlg(CARBRDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CARBRDlg)
	m_nDays = 0;
	//}}AFX_DATA_INIT

	m_pARBR	=	pARBR;
}


void CARBRDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CARBRDlg)
	DDX_Control(pDX, IDC_BOUGHT, m_cmbBought);
	DDX_Control(pDX, IDC_SOLD, m_cmbSold);
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CARBRDlg, CParamDlg)
	//{{AFX_MSG_MAP(CARBRDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CARBRDlg message handlers

BOOL CARBRDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pARBR );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CARBRDlg::OnCancel() 
{
}

void CARBRDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pARBR->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CARBRDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pARBR->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CARBRDlg::OnHelp() 
{
	ParamHelp( STT_ARBR );
}

BOOL CARBRDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pARBR->IsValidParameters() )
		m_pARBR->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pARBR->m_nDays	=	m_nDays;

		m_pARBR->m_itsGoldenFork	=	m_cmbGoldenFork.GetSelect();
		m_pARBR->m_itsDeadFork		=	m_cmbDeadFork.GetSelect();
		m_pARBR->m_itsSold			=	m_cmbSold.GetSelect();
		m_pARBR->m_itsBought		=	m_cmbBought.GetSelect();
		return m_pARBR->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_ARBR) );

		m_nDays		=	m_pARBR->m_nDays;

		m_cmbGoldenFork.ResetAndSelect( m_pARBR->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pARBR->m_itsDeadFork );
		m_cmbSold.ResetAndSelect( m_pARBR->m_itsSold );
		m_cmbBought.ResetAndSelect( m_pARBR->m_itsBought );
		UpdateData( FALSE );
	}

	return TRUE;
}

