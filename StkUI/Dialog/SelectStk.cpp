// SelectStk.cpp : implementation file
//

#include "stdafx.h"

#include "SelectStk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectStk property page

IMPLEMENT_DYNCREATE(CSelectStk, CPropertyPageEx)

CSelectStk::CSelectStk() : CPropertyPageEx(CSelectStk::IDD)
{
	//{{AFX_DATA_INIT(CSelectStk)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dwButtonFlags	=	PSWIZB_BACK | PSWIZB_NEXT;
	m_bModified		=	FALSE;
}

CSelectStk::CSelectStk(UINT nIDCaption, UINT nIDTitle, UINT nIDSubTitle)
			: CPropertyPageEx(CSelectStk::IDD, nIDCaption, nIDTitle, nIDSubTitle)
{
	m_dwButtonFlags	=	PSWIZB_BACK | PSWIZB_NEXT;
	m_bModified		=	FALSE;
}

CSelectStk::~CSelectStk()
{
}

void CSelectStk::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectStk)
	DDX_Control(pDX, IDC_REMOVEALL, m_btnRemoveAll);
	DDX_Control(pDX, IDC_REMOVESELECTED, m_btnRemoveSelected);
	DDX_Control(pDX, IDC_ADDALL, m_btnAddAll);
	DDX_Control(pDX, IDC_ADDSELECTED, m_btnAddSelected);
	DDX_Control(pDX, IDC_LIST_STOCKDEST, m_listStockDest);
	DDX_Control(pDX, IDC_LIST_STOCKSRC, m_listStockSrc);
	DDX_Control(pDX, IDC_COMBO_GROUPSRC, m_comboGroupSrc);
	DDX_Control(pDX, IDC_EDITCODE, m_editCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectStk, CPropertyPageEx)
	//{{AFX_MSG_MAP(CSelectStk)
	ON_BN_CLICKED(IDC_ADDSELECTED, OnAddselected)
	ON_BN_CLICKED(IDC_ADDALL, OnAddall)
	ON_BN_CLICKED(IDC_REMOVESELECTED, OnRemoveselected)
	ON_BN_CLICKED(IDC_REMOVEALL, OnRemoveall)
	ON_EN_CHANGE(IDC_EDITCODE, OnChangeEditcode)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUPSRC, OnSelchangeComboGroupsrc)
	ON_LBN_DBLCLK(IDC_LIST_STOCKSRC, OnDblclkListStocksrc)
	ON_LBN_DBLCLK(IDC_LIST_STOCKDEST, OnDblclkListStockdest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectStk message handlers

BOOL CSelectStk::OnInitDialog()
{
	CPropertyPageEx::OnInitDialog();
	
	CWaitCursor	waitcursor;

	// TODO: Add extra initialization here
	CDomainContainer & groups = AfxGetGroupContainer( );
	CDomainContainer & domains = AfxGetDomainContainer( );
	CStockContainer & container = AfxGetStockContainer();

	// Src
	m_comboGroupSrc.InitStrings( TRUE, TRUE, groups );
	m_comboGroupSrc.SetCurSel( 0 );
	OnSelchangeComboGroupsrc();

	// Dest
	m_listStockDest.InitStorage( m_listStockSrc.GetCount(), 32 );
	CStockContainer cntn;
	if( cntn.RetrieveSpecify( m_domainTemp ) )
	{
		for( int i=0; i<cntn.GetSize(); i++ )
		{
			CStockInfo & info = cntn.ElementAt(i);
			CString	strItem	=	CString(info.GetStockCode()) + "(" + info.GetStockShortName() + ")    ";
			while( strItem.GetLength() < 16 )	strItem	+=	" ";
			strItem	+= info.GetStockName() ;
			int nItem = m_listStockDest.AddString( strItem );
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectStk::OnAddselected() 
{
	// TODO: Add your control notification handler code here
	m_listStockDest.SelectItem( -1, FALSE );

	for( int i=0; i<m_listStockSrc.GetCount(); i++ )
	{
		int bSel = m_listStockSrc.GetSelected(i);
		if( LB_ERR != bSel && bSel )
		{
			CString	rString, strStockCode;
			m_listStockSrc.GetText(i,rString);
			int	nIndex	=	rString.Find( '(' );
			if( -1 != nIndex )
				strStockCode	=	rString.Left( nIndex );
			
			int	nSelDest	=	-1;
			if( m_domainTemp.AddStock( strStockCode ) )
				nSelDest	=	m_listStockDest.AddString( rString );
			else
				nSelDest	=	m_listStockDest.FindString( -1, rString );
			m_listStockDest.SelectAndShowItem( nSelDest );
		}
	}
	m_editCode.SetWindowText( NULL );

	m_bModified		=	TRUE;
}

void CSelectStk::OnAddall() 
{
	// TODO: Add your control notification handler code here
	m_listStockDest.SelectItem( -1, FALSE );

	for( int i=0; i<m_listStockSrc.GetCount(); i++ )
	{
		CString	rString, strStockCode;
		m_listStockSrc.GetText(i,rString);
		int	nIndex	=	rString.Find( '(' );
		if( -1 != nIndex )
			strStockCode	=	rString.Left( nIndex );
			
		int	nSelDest	=	-1;
		if( m_domainTemp.AddStock( strStockCode ) )
			nSelDest	=	m_listStockDest.AddString( rString );
		else
			nSelDest	=	m_listStockDest.FindString( -1, rString );
		m_listStockDest.SelectItem( nSelDest, TRUE );
	}
	m_editCode.SetWindowText( NULL );

	m_bModified		=	TRUE;
}

void CSelectStk::OnRemoveselected() 
{
	// TODO: Add your control notification handler code here
	for( int i=m_listStockDest.GetCount()-1; i>=0; i-- )
	{
		int bSel = m_listStockDest.GetSelected(i);
		if( LB_ERR != bSel && bSel )
		{
			CString	rString, strStockCode;
			m_listStockDest.GetText(i,rString);
			int	nIndex	=	rString.Find( '(' );
			if( -1 != nIndex )
				strStockCode	=	rString.Left( nIndex );

			if( m_domainTemp.RemoveStock( strStockCode ) )
				m_listStockDest.DeleteString( i );
		}
	}

	m_bModified		=	TRUE;
}

void CSelectStk::OnRemoveall() 
{
	// TODO: Add your control notification handler code here
	m_domainTemp.RemoveAll( );
	m_listStockDest.ResetContent( );
	m_bModified		=	TRUE;
}

void CSelectStk::OnDblclkListStocksrc() 
{
	// TODO: Add your control notification handler code here
	m_listStockDest.SelectItem( -1, FALSE );

	int nCaretIndex = m_listStockSrc.GetCaretIndex();
	if( LB_ERR != nCaretIndex )
	{
		CString	rString, strStockCode;
		m_listStockSrc.GetText(nCaretIndex,rString);
		m_listStockSrc.SelectItem( nCaretIndex, FALSE );
		int	nIndex	=	rString.Find( '(' );
		if( -1 != nIndex )
			strStockCode	=	rString.Left( nIndex );
			
		int	nSelDest	=	-1;
		if( m_domainTemp.AddStock( strStockCode ) )
			nSelDest	=	m_listStockDest.AddString( rString );
		else
			nSelDest	=	m_listStockDest.FindString( -1, rString );
		m_listStockDest.SelectAndShowItem( nSelDest );
	}

	m_bModified		=	TRUE;
}

void CSelectStk::OnDblclkListStockdest() 
{
	// TODO: Add your control notification handler code here
	int nCaretIndex	=	m_listStockDest.GetCaretIndex();
	if( LB_ERR != nCaretIndex )
	{
		CString	rString, strStockCode;
		m_listStockDest.GetText(nCaretIndex,rString);
		int	nIndex	=	rString.Find( '(' );
		if( -1 != nIndex )
			strStockCode	=	rString.Left( nIndex );

		if( m_domainTemp.RemoveStock( strStockCode ) )
			m_listStockDest.DeleteString( nCaretIndex );
	}

	m_bModified		=	TRUE;
}

void CSelectStk::OnChangeEditcode() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CNDDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString	strText;
	m_editCode.GetWindowText( strText );
	strText.TrimLeft();
	strText.TrimLeft();
	if( strText.GetLength() > 0 )
	{
		int	nSel	=	m_comboGroupSrc.GetCurSel();
		if( CB_ERR == nSel ||
			CStockContainer::typeAll != (int)(m_comboGroupSrc.GetItemData( nSel )) )
		{
			m_comboGroupSrc.SelectGroupAll( );
			OnSelchangeComboGroupsrc();
		}
	}
	if( !m_listStockSrc.SelectCode( 0, strText ) )
		m_listStockSrc.SelectShortname( 0, strText );
}

void CSelectStk::OnSelchangeComboGroupsrc() 
{
	// TODO: Add your control notification handler code here
	int nType = m_comboGroupSrc.GetCurSelType();
	CString strName = m_comboGroupSrc.GetCurSelName();
	m_listStockSrc.SetCurrentStocks( nType, strName, AfxGetGroupContainer() );
}

BOOL CSelectStk::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( WM_KEYDOWN == pMsg->message )
	{
		if( VK_UP == pMsg->wParam )
		{
			if( ::GetFocus() == m_editCode.GetSafeHwnd() )
			{
				m_listStockSrc.SelectUp( );
				return TRUE;
			}
		}
		else if( VK_DOWN == pMsg->wParam )
		{
			if( ::GetFocus() == m_editCode.GetSafeHwnd() )
			{
				m_listStockSrc.SelectDown( );
				return TRUE;
			}
		}
		else if( VK_PRIOR == pMsg->wParam )
		{
			if( ::GetFocus() == m_editCode.GetSafeHwnd() )
			{
				m_listStockSrc.SelectPrior( );
				return TRUE;
			}
		}
		else if( VK_NEXT == pMsg->wParam )
		{
			if( ::GetFocus() == m_editCode.GetSafeHwnd() )
			{
				m_listStockSrc.SelectNext( );
				return TRUE;
			}
		}
		else if( VK_RETURN == pMsg->wParam )
		{
			OnAddselected( );
			return TRUE;
		}
	}
	
	return CPropertyPageEx::PreTranslateMessage(pMsg);
}

BOOL CSelectStk::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheetEx * pSheet = DYNAMIC_DOWNCAST( CPropertySheetEx, GetParent()	);
	if( pSheet )
	{
		pSheet->SetWizardButtons( m_dwButtonFlags );
	}

	return CPropertyPageEx::OnSetActive();
}

LRESULT CSelectStk::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	if( m_domainTemp.GetSize() == 0 )
	{
		AfxMessageBox( IDS_SELECTSTK_NOSELECTED, MB_OK | MB_ICONINFORMATION );
		return -1;
	}

	return CPropertyPageEx::OnWizardNext();
}
