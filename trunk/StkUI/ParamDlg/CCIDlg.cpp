// CCIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CCIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCCIDlg dialog


CCCIDlg::CCCIDlg(CWnd* pParent, CCCI * pCCI )
{
	CParamDlg::CParamDlg(CCCIDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CCCIDlg)
	m_nDays = 0;
	m_nMADays = 0;
	m_strQuotiety = _T("");
	//}}AFX_DATA_INIT

	m_pCCI	=	pCCI;
}


void CCCIDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCCIDlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	DDX_Text(pDX, IDC_MADAYS, m_nMADays);
	DDX_Text(pDX, IDC_QUOTIETY, m_strQuotiety);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCCIDlg, CParamDlg)
	//{{AFX_MSG_MAP(CCCIDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCCIDlg message handlers

BOOL CCCIDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pCCI );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCCIDlg::OnCancel() 
{
}

void CCCIDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pCCI->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CCCIDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pCCI->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CCCIDlg::OnHelp() 
{
	ParamHelp( STT_CCI );
}

BOOL CCCIDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pCCI->IsValidParameters() )
		m_pCCI->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		if( !IsNumber(m_strQuotiety, FALSE) )
			return FALSE;

		m_pCCI->m_nDays		=	m_nDays;
		m_pCCI->m_nMADays	=	m_nMADays;
		m_pCCI->m_dQuotiety	=	atof(m_strQuotiety);

		m_pCCI->m_itsGoldenFork	=	m_cmbGoldenFork.GetSelect();
		m_pCCI->m_itsDeadFork	=	m_cmbDeadFork.GetSelect();
		return m_pCCI->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_CCI) );

		m_nDays		=	m_pCCI->m_nDays;
		m_nMADays	=	m_pCCI->m_nMADays;
		m_strQuotiety.Format( "%.3f", m_pCCI->m_dQuotiety );

		m_cmbGoldenFork.ResetAndSelect( m_pCCI->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pCCI->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

