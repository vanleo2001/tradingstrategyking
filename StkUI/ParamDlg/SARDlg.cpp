// SARDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SARDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSARDlg dialog


CSARDlg::CSARDlg(CWnd* pParent,CSAR * pSAR)
{
	CParamDlg::CParamDlg(CSARDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CSARDlg)
	m_nDays = 0;
	m_strAFStep = _T("");
	m_strAFMax = _T("");
	//}}AFX_DATA_INIT
	m_pSAR	=	pSAR;
}


void CSARDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSARDlg)
	DDX_Control(pDX, IDC_SELL, m_cmbSell);
	DDX_Control(pDX, IDC_BUY, m_cmbBuy);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	DDX_Text(pDX, IDC_AFSTEP, m_strAFStep);
	DDX_Text(pDX, IDC_AFMAX, m_strAFMax);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSARDlg, CParamDlg)
	//{{AFX_MSG_MAP(CSARDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSARDlg message handlers

BOOL CSARDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pSAR );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSARDlg::OnCancel() 
{
}

void CSARDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pSAR->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CSARDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pSAR->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CSARDlg::OnHelp() 
{
	ParamHelp( STT_SAR );
}

BOOL CSARDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pSAR->IsValidParameters() )
		m_pSAR->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );

		if( !IsNumber( m_strAFStep,FALSE ) || !IsNumber(m_strAFMax,FALSE) )
			return FALSE;

		CButton * pbtn = (CButton *)GetDlgItem(IDC_RADIOMIN);
		m_pSAR->m_bFirstUp	=	( pbtn && pbtn->GetCheck() );
		m_pSAR->m_nInitDays	=	m_nDays;
		m_pSAR->m_dAFStep	=	atof(m_strAFStep);
		m_pSAR->m_dAFMax	=	atof(m_strAFMax);

		m_pSAR->m_itsBuy	=	m_cmbBuy.GetSelect();
		m_pSAR->m_itsSell	=	m_cmbSell.GetSelect();
		return m_pSAR->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_SAR) );

		CheckRadioButton( IDC_RADIOMIN, IDC_RADIOMAX, m_pSAR->m_bFirstUp ? IDC_RADIOMIN : IDC_RADIOMAX );
		m_nDays			=	m_pSAR->m_nInitDays;
		m_strAFStep.Format( "%.2f", m_pSAR->m_dAFStep );
		m_strAFMax.Format( "%.2f", m_pSAR->m_dAFMax );

		m_cmbBuy.ResetAndSelect( m_pSAR->m_itsBuy );
		m_cmbSell.ResetAndSelect( m_pSAR->m_itsSell );
		UpdateData( FALSE );
	}

	return TRUE;
}

