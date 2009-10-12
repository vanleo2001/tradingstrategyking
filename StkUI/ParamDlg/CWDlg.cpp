// CWDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CWDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCWDlg dialog


CCWDlg::CCWDlg(CWnd* pParent, CCW * pCW)
{
	CParamDlg::CParamDlg(CCWDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CCWDlg)
	m_strChangeHand = _T("");
	//}}AFX_DATA_INIT
	m_pCW	=	pCW;
}


void CCWDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCWDlg)
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_CHANGEHAND, m_strChangeHand);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCWDlg, CParamDlg)
	//{{AFX_MSG_MAP(CCWDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCWDlg message handlers

BOOL CCWDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pCW );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCWDlg::OnCancel() 
{
}

void CCWDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pCW->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CCWDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pCW->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CCWDlg::OnHelp() 
{
	ParamHelp( STT_CW );
}

BOOL CCWDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pCW->IsValidParameters() )
		m_pCW->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );

		if( !IsNumber(m_strChangeHand,FALSE) )
			return FALSE;

		m_pCW->m_dChangeHand	=	atof(m_strChangeHand);
		return m_pCW->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_CW) );

		m_strChangeHand.Format( "%.2f", m_pCW->m_dChangeHand );
		UpdateData( FALSE );
	}

	return TRUE;
}

