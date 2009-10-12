// SetRate.cpp : implementation file
//

#include "stdafx.h"

#include "SetRate.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetRate property page

IMPLEMENT_DYNCREATE(CSetRate, CPropertyPageEx)

CSetRate::CSetRate() : CPropertyPageEx(CSetRate::IDD)
{
	//{{AFX_DATA_INIT(CSetRate)
	m_strTaxShaa = _T("");
	m_strTaxShab = _T("");
	m_strTaxShafund = _T("");
	m_strTaxSzna = _T("");
	m_strTaxSznb = _T("");
	m_strTaxSznfund = _T("");
	//}}AFX_DATA_INIT
	m_pStrategy	=	NULL;

	m_dwButtonFlags	=	PSWIZB_BACK | PSWIZB_NEXT;
	m_bModified		=	FALSE;
}

CSetRate::CSetRate( UINT nIDCaption, UINT nIDTitle, UINT nIDSubTitle )
		 : CPropertyPageEx(CSetRate::IDD,nIDCaption, nIDTitle, nIDSubTitle)
{
	m_strTaxShaa = _T("");
	m_strTaxShab = _T("");
	m_strTaxShafund = _T("");
	m_strTaxSzna = _T("");
	m_strTaxSznb = _T("");
	m_strTaxSznfund = _T("");

	m_pStrategy	=	NULL;

	m_dwButtonFlags	=	PSWIZB_BACK | PSWIZB_NEXT;
	m_bModified		=	FALSE;
}

CSetRate::~CSetRate()
{
}

void CSetRate::SetStrategy( CStrategy * pStrategy )
{
	m_pStrategy	=	pStrategy;
}

BOOL CSetRate::ValidData( )
{
	if( IsNumber(m_strTaxShaa,FALSE) && IsNumber(m_strTaxShab,FALSE) && IsNumber(m_strTaxShafund,FALSE)
		&& IsNumber(m_strTaxSzna,FALSE) && IsNumber(m_strTaxSznb,FALSE) && IsNumber(m_strTaxSznfund,FALSE) )
	{
		return TRUE;
	}
	AfxMessageBox( IDS_SETRATE_NOTNUMBER, MB_OK | MB_ICONINFORMATION );
	return FALSE;
}

BOOL CSetRate::LoadFromStrategy( )
{
	if( m_pStrategy )
	{
		CRateParam & rate = m_pStrategy->GetRateParam( );

		m_strTaxShaa.Format( "%.4f", rate.m_dShaa );
		m_strTaxShab.Format( "%.4f", rate.m_dShab );
		m_strTaxShafund.Format( "%.4f", rate.m_dShafund );
		m_strTaxSzna.Format( "%.4f", rate.m_dSzna );
		m_strTaxSznb.Format( "%.4f", rate.m_dSznb );
		m_strTaxSznfund.Format( "%.4f", rate.m_dSznfund );

		return TRUE;
	}
	return FALSE;
}

BOOL CSetRate::StoreToStrategy( )
{
	if( m_pStrategy )
	{
		CRateParam & rate = m_pStrategy->GetRateParam( );

		m_bModified		=	(m_bModified || fabs(rate.m_dShaa-atof(m_strTaxShaa)) > 1e-6 );
		m_bModified		=	(m_bModified || fabs(rate.m_dShab-atof(m_strTaxShab)) > 1e-6 );
		m_bModified		=	(m_bModified || fabs(rate.m_dShafund-atof(m_strTaxShafund)) > 1e-6 );
		m_bModified		=	(m_bModified || fabs(rate.m_dSzna-atof(m_strTaxSzna)) > 1e-6 );
		m_bModified		=	(m_bModified || fabs(rate.m_dSznb-atof(m_strTaxSznb)) > 1e-6 );
		m_bModified		=	(m_bModified || fabs(rate.m_dSznfund-atof(m_strTaxSznfund)) > 1e-6 );

		rate.m_dShaa	=	atof(m_strTaxShaa);
		rate.m_dShab	=	atof(m_strTaxShab);
		rate.m_dShafund	=	atof(m_strTaxShafund);
		rate.m_dSzna	=	atof(m_strTaxSzna);
		rate.m_dSznb	=	atof(m_strTaxSznb);
		rate.m_dSznfund	=	atof(m_strTaxSznfund);

		return TRUE;
	}
	return FALSE;
}

void CSetRate::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetRate)
	DDX_Text(pDX, IDC_TAXSHAA, m_strTaxShaa);
	DDX_Text(pDX, IDC_TAXSHAB, m_strTaxShab);
	DDX_Text(pDX, IDC_TAXSHAFUND, m_strTaxShafund);
	DDX_Text(pDX, IDC_TAXSZNA, m_strTaxSzna);
	DDX_Text(pDX, IDC_TAXSZNB, m_strTaxSznb);
	DDX_Text(pDX, IDC_TAXSZNFUND, m_strTaxSznfund);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetRate, CPropertyPageEx)
	//{{AFX_MSG_MAP(CSetRate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetRate message handlers

BOOL CSetRate::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	ASSERT( m_pStrategy );

	LoadFromStrategy( );

	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSetRate::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData( );
	if( !ValidData( ) )
		return FALSE;
	StoreToStrategy( );
	return CPropertyPageEx::OnKillActive();
}

LRESULT CSetRate::OnWizardBack() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData( );
	if( !ValidData( ) )
		return -1L;
	StoreToStrategy( );
	return CPropertyPageEx::OnWizardBack();
}

LRESULT CSetRate::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData( );
	if( !ValidData( ) )
		return -1L;
	StoreToStrategy( );
	return CPropertyPageEx::OnWizardNext();
}

BOOL CSetRate::OnWizardFinish() 
{
	// Add your specialized code here and/or call the base class
	UpdateData( );
	if( !ValidData( ) )
		return FALSE;
	StoreToStrategy( );
	return CPropertyPageEx::OnWizardFinish();
}

void CSetRate::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData( );
	if( !ValidData( ) )
		return;
	StoreToStrategy( );
	CPropertyPageEx::OnOK();
}

BOOL CSetRate::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam )
		return TRUE;
	return CPropertyPageEx::PreTranslateMessage(pMsg);
}

BOOL CSetRate::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheetEx * pSheet = DYNAMIC_DOWNCAST( CPropertySheetEx, GetParent()	);
	if( pSheet )
	{
		pSheet->SetWizardButtons( m_dwButtonFlags );
	}
	
	return CPropertyPageEx::OnSetActive();
}
