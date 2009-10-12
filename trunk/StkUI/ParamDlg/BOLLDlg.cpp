// BOLLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BOLLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBOLLDlg dialog


CBOLLDlg::CBOLLDlg(CWnd* pParent, CBOLL * pBOLL)
{
	CParamDlg::CParamDlg(CBOLLDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CBOLLDlg)
	m_nDays = 0;
	m_strMultiUp = _T("");
	m_strMultiDown = _T("");
	//}}AFX_DATA_INIT
	m_pBOLL	=	pBOLL;
}


void CBOLLDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOLLDlg)
	DDX_Control(pDX, IDC_RESISTANCE, m_cmbResistance);
	DDX_Control(pDX, IDC_SUPPORT, m_cmbSupport);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	DDX_Text(pDX, IDC_MULTIUP, m_strMultiUp);
	DDX_Text(pDX, IDC_MULTIDOWN, m_strMultiDown);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBOLLDlg, CParamDlg)
	//{{AFX_MSG_MAP(CBOLLDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBOLLDlg message handlers

BOOL CBOLLDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();

	ASSERT( m_pBOLL );
	
	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBOLLDlg::OnCancel() 
{
}

void CBOLLDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pBOLL->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CBOLLDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pBOLL->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CBOLLDlg::OnHelp() 
{
	ParamHelp( STT_BOLL );
}

BOOL CBOLLDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pBOLL->IsValidParameters() )
		m_pBOLL->SetDefaultParameters( );

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		if( !IsNumber( m_strMultiUp, FALSE ) || !IsNumber(m_strMultiDown, FALSE) )
			return FALSE;

		m_pBOLL->m_dMultiUp	=	atof(m_strMultiUp);
		m_pBOLL->m_dMultiDown	=	atof(m_strMultiDown);
		m_pBOLL->m_nMADays	=	m_nDays;

		m_pBOLL->m_itsSupport		=	m_cmbSupport.GetSelect( );
		m_pBOLL->m_itsResistance	=	m_cmbResistance.GetSelect( );
		return m_pBOLL->IsValidParameters( );
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_BOLL) );

		m_strMultiUp.Format( "%.2f", m_pBOLL->m_dMultiUp );
		m_strMultiDown.Format( "%.2f", m_pBOLL->m_dMultiDown );
		m_nDays			=	m_pBOLL->m_nMADays;

		m_cmbSupport.ResetAndSelect( m_pBOLL->m_itsSupport );
		m_cmbResistance.ResetAndSelect( m_pBOLL->m_itsResistance );
		UpdateData( FALSE );
	}

	return TRUE;
}

