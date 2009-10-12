// 36BIASDlg.cpp : implementation file
//

#include "stdafx.h"
#include "36BIASDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C36BIASDlg dialog


C36BIASDlg::C36BIASDlg(CWnd* pParent, C36BIAS * p36BIAS )
{
	CParamDlg::CParamDlg(C36BIASDlg::IDD, pParent);

	//{{AFX_DATA_INIT(C36BIASDlg)
	//}}AFX_DATA_INIT

	m_p36BIAS	=	p36BIAS;
}


void C36BIASDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C36BIASDlg)
	DDX_Control(pDX, IDC_BOUGHT, m_cmbBought);
	DDX_Control(pDX, IDC_SOLD, m_cmbSold);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C36BIASDlg, CParamDlg)
	//{{AFX_MSG_MAP(C36BIASDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C36BIASDlg message handlers

BOOL C36BIASDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_p36BIAS );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C36BIASDlg::OnCancel() 
{
}

void C36BIASDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_p36BIAS->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void C36BIASDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_p36BIAS->SetDefaultParameters( );
	RefreshData( FALSE );
}

void C36BIASDlg::OnHelp() 
{
	ParamHelp( STT_36BIAS );
}

BOOL C36BIASDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_p36BIAS->IsValidParameters() )
		m_p36BIAS->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_p36BIAS->m_itsSold	=	m_cmbSold.GetSelect();
		m_p36BIAS->m_itsBought	=	m_cmbBought.GetSelect();
		return m_p36BIAS->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_36BIAS) );

		m_cmbSold.ResetAndSelect( m_p36BIAS->m_itsSold );
		m_cmbBought.ResetAndSelect( m_p36BIAS->m_itsBought );
		UpdateData( FALSE );
	}

	return TRUE;
}

