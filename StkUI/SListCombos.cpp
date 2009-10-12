// SListCombos.cpp : implementation file
//

#include "stdafx.h"
#include "SListCombos.h"
#include "Dialog\CacheLoadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDateComboBox
char	szDateComboBoxTimeFormat[]	=	"%Y-%m-%d,";

CDateComboBox::CDateComboBox()
{
}

CDateComboBox::~CDateComboBox()
{
}

BOOL CDateComboBox::InitDates( )
{
	ResetContent( );

	// Add Date ComboBox Members
	CSPDWordArray dwArray;
	AfxGetStockContainer().GetTechDateArray( dwArray );

	InitStorage( dwArray.GetSize()+2, 32 );

	CString strMore;
	strMore.LoadString( IDS_DATECOMBOBOX_MORE );
	int nIndex = AddString( strMore );
	SetItemData( nIndex, -2 );

	/*int	nMaxDaysCalculate	=	AfxGetProfile().GetMaxDaysCalculate( );*/
	// for( int nCount=dwArray.GetSize()-1; nCount >= 0/*nCount > nMaxDaysCalculate*/; nCount -- )
	for( int nCount=0; nCount < dwArray.GetSize(); nCount ++ )
	{
		CSPTime	sptime;
		if( sptime.FromStockTimeDay( dwArray[nCount] ) )
		{
			CSPTime	time( sptime.GetTime() );
			CString	strItem	=	AfxGetTimeString( time.GetTime(), szDateComboBoxTimeFormat, TRUE );
			if( strItem.GetLength() > 0 )
			{
				int nIndex = AddString( strItem );
				SetItemData( nIndex, dwArray[nCount] );
			}
		}
	}

	CString strNow;
	strNow.LoadString( IDS_DATECOMBOBOX_NOW );
	nIndex = AddString( strNow );
	SetItemData( nIndex, -1 );
	SetCurSel( nIndex );

	return TRUE;
}

void CDateComboBox::SetCurrentWindowText( )
{
	if( !::IsWindow(m_hWnd) )
		return;

	DWORD	dwDate = -1;
	AfxGetSListStockContainer().GetCurrentType(NULL,NULL,&dwDate);
	CSPTime	sptime;
	if( DWORD(-2) == dwDate )
	{
		CString	strItem;
		strItem.LoadString( IDS_DATECOMBOBOX_MORE );
		SelectString( 0, strItem );
	}
	else if( DWORD(-1) == dwDate )
	{
		CString	strItem;
		strItem.LoadString( IDS_DATECOMBOBOX_NOW );
		SelectString( 0, strItem );
	}
	else if( sptime.FromStockTimeDay( dwDate ) )
	{
		CString	strItem	=	AfxGetTimeString( sptime.GetTime(), szDateComboBoxTimeFormat, TRUE );
		SelectString( 0, strItem );
	}
	else
	{
		CString	strItem;
		strItem.LoadString( IDS_DATECOMBOBOX_NOW );
		SelectString( 0, strItem );
	}
}

BOOL CDateComboBox::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( CSearchBox::IsWantChar( pMsg->wParam ) )
		{
			CMainFrame	* pFrame = AfxGetMainFrame();
			if( pFrame )
				pFrame->m_SearchBox.OnChangeStatus( pMsg->wParam, pMsg->lParam, FALSE );
			return TRUE;
		}
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CDateComboBox, CComboBox)
	//{{AFX_MSG_MAP(CDateComboBox)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendok)
	ON_CONTROL_REFLECT(CBN_SELENDCANCEL, OnSelendcancel)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, OnKillfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDateComboBox message handlers

int CDateComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// InitDates( );

	return 0;
}

void CDateComboBox::OnSelendok()
{
	// TODO: Add your control notification handler code here
	int nSel = GetCurSel();

	if( CB_ERR != nSel )
	{
		SetCurSel( nSel );
		DWORD dwDate = GetItemData( nSel );
		if( DWORD(-2) == dwDate )
		{
			CCacheLoadDlg dlg;
			dlg.DoModal();
			return;
		}
		else
		{
			int	nType;
			CSPString	strDomain;
			DWORD	date;
			if( AfxGetSListStockContainer().GetCurrentType( &nType, &strDomain, &date )
				&& date != dwDate )
			{
				AfxShowSlist( nType, strDomain, dwDate );
			}
		}
	}
	SetCurrentWindowText( );
}

void CDateComboBox::OnSelendcancel()
{
	// TODO: Add your control notification handler code here
	SetCurrentWindowText( );
}

void CDateComboBox::OnKillfocus( )
{
	SetCurrentWindowText( );
}

/////////////////////////////////////////////////////////////////////////////
// CSearchBox

#define		SEARCHBOX_SIGNBIT_STT	0x80000000		// 技术指标项
#define		SEARCHBOX_SIGNBIT_ACCE	0x40000000		// 快捷键项

CSearchBox::CSearchBox()
{
	m_hwndLastFocus	=	NULL;
	m_bSegmentEnd	=	FALSE;
	m_bShowOnSel	=	TRUE;
	m_bDoNothing	=	FALSE;

	m_bAutoHide		=	FALSE;
}

CSearchBox::~CSearchBox()
{
}

void CSearchBox::SetAutoHide( BOOL bAutoHide )
{
	m_bAutoHide	=	bAutoHide;
}

BOOL CSearchBox::InitStocks( BOOL bHasSTTTech, BOOL bShowOnSel, BOOL bDuplicate )
{
	ResetContent( );

	m_bShowOnSel	=	bShowOnSel;

	CStockContainer & container	=	AfxGetStockContainer();
	InitStorage( bDuplicate ? container.GetSize()*2 : container.GetSize(), 32 );
	for( int i=0; i<container.GetSize(); i++ )
	{
		CStockInfo	& info	=	container.ElementAt(i);
		if( !info.IsValidStock() )
			continue;

		CString	strTemp	=	info.GetStockCode();
		while( strTemp.GetLength() < 8 )
			strTemp	+=	' ';
		strTemp	+=	info.GetStockName();
		int	nItem	=	AddString( strTemp );
		SetItemData( nItem, i );

		if( bDuplicate )
		{
			strTemp	=	info.GetStockShortName();
			while( strTemp.GetLength() < 8 )
				strTemp	+=	' ';
			strTemp	+=	info.GetStockName();
			if( CB_ERR == SelectString( 0, strTemp ) )
			{
				nItem = AddString( strTemp );
				SetItemData( nItem, i );
			}
		}
	}
	if( bHasSTTTech )
	{
		// 技术指标项
		UINT nTechUserCount = CTechUser::GetTechUserCount();
		for( UINT i=STT_MIN; i <= STT_MAX+nTechUserCount; i ++ )
		{
			UINT	nTech	=	i;
			if( nTech > STT_MAX )
				nTech	=	i-STT_MAX-1+STT_USER_MIN;
		
			CString	strTemp	=	AfxGetSTTShortName( nTech );
			while( strTemp.GetLength() < 8 )
				strTemp	+=	' ';
			strTemp	+=	AfxGetSTTName( nTech );
			int	nItem	=	AddString( strTemp );
			SetItemData( nItem, nTech | SEARCHBOX_SIGNBIT_STT );
		}
		// 快捷键项
		for( UINT i = ACCE_MIN; i <= ACCE_MAX; i++ )
		{
			CString	strTemp	=	AfxGetAccelerator( i, 8 );
			SetItemData( AddString(strTemp), i | SEARCHBOX_SIGNBIT_ACCE );
		}
	}
	return TRUE;
}

BOOL CSearchBox::InitStocks( CSPStringArray & astocks, BOOL bHasSTTTech, BOOL bShowOnSel, BOOL bDuplicate )
{
	ResetContent( );

	m_bShowOnSel	=	bShowOnSel;

	CStockContainer & container	=	AfxGetStockContainer();
	InitStorage( bDuplicate ? astocks.GetSize()*2 : astocks.GetSize(), 32 );
	for( int i=0; i<astocks.GetSize(); i++ )
	{
		if( astocks.ElementAt(i).GetLength() <= 0 )
			continue;

		CStockInfo	info;
		int	nAID	=	-1;
		if( !container.GetStockInfo( astocks.ElementAt(i), &info, &nAID ) )
			continue;

		CString	strTemp	=	info.GetStockCode();
		while( strTemp.GetLength() < 8 )
			strTemp	+=	' ';
		strTemp	+=	info.GetStockName();
		int	nItem	=	AddString( strTemp );
		SetItemData( nItem, nAID );

		if( bDuplicate )
		{
			strTemp	=	info.GetStockShortName();
			while( strTemp.GetLength() < 8 )
				strTemp	+=	' ';
			strTemp	+=	info.GetStockName();
			if( CB_ERR == SelectString( 0, strTemp ) )
			{
				nItem = AddString( strTemp );
				SetItemData( nItem, nAID );
			}
		}
	}
	if( bHasSTTTech )
	{
		// 技术指标项
		UINT nTechUserCount = CTechUser::GetTechUserCount();
		for( UINT i=STT_MIN; i <= STT_MAX+nTechUserCount; i ++ )
		{
			UINT	nTech	=	i;
			if( nTech > STT_MAX )
				nTech	=	i-STT_MAX-1+STT_USER_MIN;

			CString	strTemp	=	AfxGetSTTShortName( nTech );
			while( strTemp.GetLength() < 8 )
				strTemp	+=	' ';
			strTemp	+=	AfxGetSTTName( nTech );
			int	nItem	=	AddString( strTemp );
			SetItemData( nItem, nTech | SEARCHBOX_SIGNBIT_STT );
		}
		// 快捷键项
		for( UINT i = ACCE_MIN; i <= ACCE_MAX; i++ )
		{
			CString	strTemp	=	AfxGetAccelerator( i, 8 );
			SetItemData( AddString(strTemp), i | SEARCHBOX_SIGNBIT_ACCE );
		}
	}
	return TRUE;
}

CString	CSearchBox::GetSelectedStockCode( )
{
	int	nSel	=	GetCurSel();
	if( CB_ERR == nSel )
		return "";
	
	CStockContainer & container	=	AfxGetStockContainer();
	long	dwData	=	GetItemData(nSel);
	if( dwData >= 0 && dwData < container.GetSize() )
	{
		return container.ElementAt(dwData).GetStockCode();
	}
	return "";
}

void CSearchBox::SetCurrentWindowText( )
{
	if( !::IsWindow(m_hWnd) )
		return;

	if( !m_bShowOnSel )
		return;

	CStockInfo	info;
	if( AfxGetProfile().GetCurrentStock( &info ) )
	{
		if( CB_ERR == SelectString( 0, info.GetStockCode() ) )
			SetWindowText( info.GetStockCode() );
	}
	else
	{
		SetWindowText( NULL );
	}
}

BOOL CSearchBox::IsWantChar( int nVirtKey )
{
	return ( (nVirtKey >= '0' && nVirtKey <= '9')
		|| (nVirtKey >= 'A' && nVirtKey <= 'Z')
		|| (nVirtKey >= VK_NUMPAD0 && nVirtKey <= VK_NUMPAD9) );
}

char VirtKeyToChar( int nVirtKey )
{
	if( nVirtKey >= VK_NUMPAD0 && nVirtKey <= VK_NUMPAD9 )
	{
		return (char)(nVirtKey - VK_NUMPAD0 + '0');
	}
	return (char)nVirtKey;
}

void CSearchBox::OnChangeStatus( int nVirtKey, long lParam, BOOL bFocused )
{
	CString	strText;
	GetWindowText( strText );
	int	nCurSel	=	GetCurSel();

	/* VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
	/* VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */

	if( IsWantChar( nVirtKey ) && !IsCTRLpressed() )	//	Ctrl Key Not Pressed
	{
		if( !bFocused )
			m_hwndLastFocus	=	::SetFocus( m_hWnd );

		if( m_bSegmentEnd )
		{
			strText	=	VirtKeyToChar( nVirtKey );
			m_bSegmentEnd	=	FALSE;
			
			SetWindowText( strText );
			SetEditSel( strText.GetLength(), strText.GetLength() );
		}
		else
		{
			DWORD dwEditSel = GetEditSel();
			if( CB_ERR != dwEditSel && LOWORD(dwEditSel) <= strText.GetLength() && HIWORD(dwEditSel) <= strText.GetLength() )
			{
				CString	strTemp	=	strText.Left( LOWORD(dwEditSel) );
				strTemp	+=	VirtKeyToChar( nVirtKey );
				strTemp	+=	strText.Mid( HIWORD(dwEditSel) );
				strText	=	strTemp;
				SetWindowText( strText );
				SetEditSel( LOWORD(dwEditSel)+1, LOWORD(dwEditSel)+1 );
			}
			else
			{
				strText	+=	VirtKeyToChar( nVirtKey );
				SetWindowText( strText );
				SetEditSel( strText.GetLength(), strText.GetLength() );
			}
		}
		
		OnEditchange( );
	}
}

BEGIN_MESSAGE_MAP(CSearchBox, CComboBox)
	//{{AFX_MSG_MAP(CSearchBox)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, OnKillfocus)
	ON_CONTROL_REFLECT(CBN_EDITCHANGE, OnEditchange)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendok)
	ON_CONTROL_REFLECT(CBN_SELENDCANCEL, OnSelendcancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchBox message handlers

BOOL CSearchBox::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( IsWantChar( pMsg->wParam ) )
		{
			OnChangeStatus( pMsg->wParam, pMsg->lParam, TRUE );
			return TRUE;
		}
		if( VK_RETURN == pMsg->wParam )
		{
			m_bDoNothing	=	TRUE;
			m_hwndLastFocus	=	NULL;
			m_bSegmentEnd	=	TRUE;

			CString	strCommand;
			GetWindowText( strCommand );
			int nSel = SelectString( 0, strCommand );
			ShowDropDown( FALSE );
			SetCurSel( nSel );
			
			// Show Graph View
			if( CB_ERR != nSel && m_bShowOnSel )
			{
				DWORD	dwData	=	GetItemData( nSel );
				CView * pView = AfxGetStaticDoc()->GetActiveView();
				if( dwData & SEARCHBOX_SIGNBIT_STT )
					AfxShowStockTech( dwData & ~SEARCHBOX_SIGNBIT_STT ); 
				else if( dwData & SEARCHBOX_SIGNBIT_ACCE )
					AfxExecuteAccelerator( dwData & ~SEARCHBOX_SIGNBIT_ACCE );
				else if( IsCTRLpressed() )
					AfxShowStockBase( dwData, FALSE );
				else if( pView && pView->IsKindOf( RUNTIME_CLASS(CRealTimeView) ) )
					AfxShowStockRealTime( dwData, FALSE );
				else
					AfxShowStockGraph( dwData, FALSE );
			}
			else if( CB_ERR == nSel && m_bShowOnSel )
			{
				AfxShowStockGraph( strCommand );
			}
			else
				SetCurrentWindowText( );

			m_bDoNothing	=	FALSE;
			if( m_bAutoHide )
				SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_HIDEWINDOW );
			return TRUE;
		}
		else if( VK_ESCAPE == pMsg->wParam )
		{
			m_bDoNothing	=	TRUE;
			if( m_hwndLastFocus )
				::SetFocus( m_hwndLastFocus );
			m_hwndLastFocus	=	NULL;
			m_bSegmentEnd	=	TRUE;

			CString	strCommand;
			GetWindowText( strCommand );
			int nSel = SelectString( 0, strCommand );
			ShowDropDown( FALSE );
			SetCurSel( nSel );

			SetCurrentWindowText( );
			m_bDoNothing	=	FALSE;

			if( m_bAutoHide )
				SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_HIDEWINDOW );
			return TRUE;
		}
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

int CSearchBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	
	// InitStocks( TRUE, TRUE, TRUE );

	return 0;
}

void CSearchBox::OnKillfocus( )
{
	SetCurrentWindowText( );
}

void CSearchBox::OnEditchange() 
{
	// TODO: Add your control notification handler code here
	DWORD dwEditSel = GetEditSel();

	CString	strText;
	GetWindowText( strText );
	
	int nFind	=	FindString( 0, strText );
	SetCurSel( nFind );
	if( !GetDroppedState() )
		ShowDropDown( );
	SetWindowText( strText );

	SetEditSel( 0, -1 );
	SetEditSel( LOWORD(dwEditSel), HIWORD(dwEditSel) );
}

void CSearchBox::OnSelendok()
{
	// TODO: Add your control notification handler code here
	if( m_bDoNothing )
		return;

	int nSel = GetCurSel();

	m_hwndLastFocus	=	NULL;
	m_bSegmentEnd	=	TRUE;
	
	if( CB_ERR != nSel )
	{
		SetCurSel( nSel );
		if( m_bShowOnSel )
		{
			DWORD	dwData	=	GetItemData( nSel );
			CView * pView = AfxGetStaticDoc()->GetActiveView();
			if( dwData & SEARCHBOX_SIGNBIT_STT )
				AfxShowStockTech( dwData & ~SEARCHBOX_SIGNBIT_STT ); 
			else if( dwData & SEARCHBOX_SIGNBIT_ACCE )
				AfxExecuteAccelerator( dwData & ~SEARCHBOX_SIGNBIT_ACCE );
			else if( pView && pView->IsKindOf( RUNTIME_CLASS(CRealTimeView) ) )
				AfxShowStockRealTime( dwData, FALSE );
			else
				AfxShowStockGraph( dwData, FALSE );
			return;
		}
	}
	SetCurrentWindowText( );

	if( m_bAutoHide )
		SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_HIDEWINDOW );
}

void CSearchBox::OnSelendcancel()
{
	// TODO: Add your control notification handler code here
	if( m_bDoNothing )
		return;

	m_bDoNothing	=	TRUE;
	if( m_hwndLastFocus )
		::SetFocus( m_hwndLastFocus );
	m_hwndLastFocus	=	NULL;
	m_bSegmentEnd	=	TRUE;

	CString	strCommand;
	GetWindowText( strCommand );
	int nSel = SelectString( 0, strCommand );
	ShowDropDown( FALSE );
	SetCurSel( nSel );

	SetCurrentWindowText( );
	m_bDoNothing	=	FALSE;

	if( m_bAutoHide )
		SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_HIDEWINDOW );
}

