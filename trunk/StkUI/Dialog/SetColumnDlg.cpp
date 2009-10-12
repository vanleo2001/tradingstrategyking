// SetColumnDlg.cpp : implementation file
//

#include "stdafx.h"

#include "SetColumnDlg.h"
#include "BaseIndexDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColListCtrl

#define		IDC_COLLISTCTRL_COMBOBOX	100

CColListCtrl::CColListCtrl()
{
	m_nCurItem			=	-1;
	m_nComboBoxColumn	=	-1;
}

CColListCtrl::~CColListCtrl()
{
	for( int i=0; i<m_aptrItemStrings.GetSize(); i++ )
	{
		CStringArray * pastr = (CStringArray *)m_aptrItemStrings.ElementAt(i);
		if( NULL != pastr )
			delete	pastr;
	}
	m_aptrItemStrings.RemoveAll();
}

int CColListCtrl::SetComboBoxColumn( int nCol )
{
	CHeaderCtrl * pHeaderCtrl = GetHeaderCtrl();
	if( pHeaderCtrl )
	{
		ASSERT( nCol < pHeaderCtrl->GetItemCount() );
	}

	m_nComboBoxColumn	=	nCol;
	return m_nComboBoxColumn;
}

int CColListCtrl::SetItemStrings( int nItem, CStringArray &astr )
{
	ASSERT( nItem <= 10000 );
	if( m_aptrItemStrings.GetSize() <= nItem )
	{
		int	nSizeOld	=	m_aptrItemStrings.GetSize();
		m_aptrItemStrings.SetSize( nItem+1 );
		for( int i=nSizeOld; i<m_aptrItemStrings.GetSize(); i++ )
		{
			m_aptrItemStrings.SetAt( i, NULL );
		}
	}
	CStringArray	* pastrNew	=	new CStringArray;
	ASSERT( pastrNew && nItem < m_aptrItemStrings.GetSize() );
	if( pastrNew && nItem < m_aptrItemStrings.GetSize() )
	{
		pastrNew->Copy( astr );
		m_aptrItemStrings.SetAt( nItem, pastrNew );
		return nItem;
	}
	return -1;
}

BEGIN_MESSAGE_MAP(CColListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CColListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(NM_KILLFOCUS, OnKillfocus)
	ON_WM_KILLFOCUS()
	ON_WM_NCLBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColListCtrl message handlers

void CColListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if( -1 == pNMListView->iItem )
		return;
	if( -1 == m_nComboBoxColumn )
		return;

	if( m_nComboBoxColumn == pNMListView->iSubItem )
	{
		ShowItemComboBox( pNMListView->iItem );
	}
	else
	{
		HideItemComboBox( );
	}
	
	*pResult = 0;
}

void CColListCtrl::OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(  ::GetFocus() != m_comboBox.GetSafeHwnd() )
	{
		HideItemComboBox( );
	}
	*pResult = 0;
}

BOOL CColListCtrl::CreateComboBox( int nItem ) 
{
	if( m_nComboBoxColumn < 0 || nItem < 0 )
		return FALSE;

	// Create
	if( !m_comboBox.GetSafeHwnd() )
	{
		CRect	rectItem( 0, 0, 35, 136 );
		GetSubItemRect( nItem, m_nComboBoxColumn, LVIR_BOUNDS, rectItem );
		CRect	rect( 0, 0, rectItem.Width(), rectItem.Height()+120 );
		if( !m_comboBox.Create( WS_CHILD | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_VSCROLL | CBS_HASSTRINGS,
				rect, this, IDC_COLLISTCTRL_COMBOBOX ) )
			return FALSE;

		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		if (hFont == NULL)
			hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);
		m_comboBox.SendMessage(WM_SETFONT, (WPARAM)hFont);
	}

	// Add Strings
	if( nItem >= 0 && nItem < m_aptrItemStrings.GetSize() )
	{
		CStringArray	* pastr	=	(CStringArray *) m_aptrItemStrings.ElementAt(nItem);
		if( NULL != pastr && pastr->GetSize() > 0 )
		{
			m_comboBox.ResetContent();
			for( int k=0; k<pastr->GetSize(); k++ )
				m_comboBox.AddString( pastr->ElementAt(k) );
			m_comboBox.SelectString( 0, GetItemText(nItem,m_nComboBoxColumn) );
			return TRUE;
		}
	}
	return FALSE;
}

int	CColListCtrl::ShowItemComboBox( int nItem )
{
	// Hide and Save Old
	HideItemComboBox( );

	m_nCurItem	=	nItem;
	// Valid
	if( nItem < 0 || m_nComboBoxColumn < 0 )
		return	-1;

	// Create and Show
	if( CreateComboBox( nItem ) )
	{
		CRect rect;
		GetSubItemRect( nItem, m_nComboBoxColumn, LVIR_BOUNDS, rect );
		m_comboBox.SetWindowPos( NULL, rect.left, rect.top+1, rect.Width(), rect.Height()-1, SWP_SHOWWINDOW );
		m_comboBox.SetFocus();
		m_comboBox.SelectString( 0, GetItemText( nItem, m_nComboBoxColumn ) );
		m_comboBox.ShowDropDown( TRUE );
	}
	return	0;
}

int	CColListCtrl::HideItemComboBox( )
{
	// Save Old
	if( m_nCurItem >= 0 && m_nComboBoxColumn >= 0 
		&& m_comboBox.GetSafeHwnd() && m_comboBox.IsWindowVisible() )
	{
		CString		strText;
		m_comboBox.GetWindowText( strText );
		SetItemText( m_nCurItem, m_nComboBoxColumn, strText );
	}

	// Clear m_nCurItem
	m_nCurItem	=	-1;
	
	if( NULL == m_comboBox.GetSafeHwnd() || !m_comboBox.IsWindowVisible() )
		return -1;

	m_comboBox.SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE );
	Invalidate( );
	return	0;
}

void CColListCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CListCtrl::OnKillFocus(pNewWnd);
	if( pNewWnd->GetSafeHwnd() != m_comboBox.GetSafeHwnd() )
	{
		HideItemComboBox( );
	}
}

BOOL CColListCtrl::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if( IDC_COLLISTCTRL_COMBOBOX == nID && EN_KILLFOCUS == nCode )	
	{
		HideItemComboBox( );
	}
	return CListCtrl::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CColListCtrl::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	HideItemComboBox( );
	CListCtrl::OnNcLButtonDown(nHitTest, point);
}

/////////////////////////////////////////////////////////////////////////////
// CSetColumnDlg dialog

CSetColumnDlg::CSetColumnDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CSetColumnDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetColumnDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nYieldAverageDaysOld	=	AfxGetProfile().GetYieldAverageDays();

	m_bUserIndexModified	=	FALSE;
}

void CSetColumnDlg::SetListColumnItems( CSPDWordArray & anShow, BOOL bSetDefault )
{
	m_listColumn.DeleteAllItems( );

	CSPDWordArray	& anOrder	= AfxGetProfile().GetSListColumnsOrder( );
	CIndexContainer	& acol	=	AfxGetProfile().GetSListColumnsUser( );

	CString	strDay;
	strDay.LoadString( IDS_SLHHDR_DAY );

	int	nCount	=	0;
	for( int i=0; i<anOrder.GetSize(); i++ )
	{
		// Set Item Text
		CString	strName, strDes;
		int	nItem	=	-1;
		UINT	nID	=	anOrder[i];
		strName	=	AfxGetVariantName( nID, FALSE );
		if( strName.IsEmpty() )
			continue;
		if( nID >= SLH_USERDEFINE_BEGIN )
			strDes.LoadString( IDS_SETCOLUMN_USERDEFINE );
		else
			strDes	=	(LPCTSTR)AfxGetSLHDescript( nID );

		nItem	=	m_listColumn.InsertItem( nCount, strName, -1 );
		m_listColumn.SetItemText( nItem, 1, strDes );
		m_listColumn.SetItemData( nItem, nID );
		nCount	++;

		// SetCheck
		for( int j=0; j<anShow.GetSize(); j++ )
		{
			if( anShow[j] == nID )
			{
				m_listColumn.SetCheck( nItem );
				break;
			}
		}

		// Set Column Combo Text
		CStringArray	astr;
		if( SLH_DIFF == nID || SLH_DIFFPERCENT == nID || SLH_SCOPE == nID
			|| SLH_RATIO_CHANGEHAND == nID || SLH_RATIO_VOLUME == nID || SLH_RS == nID )
		{
			for( int nDays=1; nDays<=STKLIB_DAYS_CALCULATE; nDays ++ )
			{
				CString	strTemp;
				strTemp.Format( "%d%s", nDays, strDay );
				astr.Add( strTemp );
			}
		}
		else if( SLH_ANNGAINS_AVERAGE == nID )
		{
			CString	strTemp;
			strTemp.Format( "%d%s", 20, strDay );		astr.Add( strTemp );
			strTemp.Format( "%d%s", 40, strDay );		astr.Add( strTemp );
			strTemp.Format( "%d%s", 60, strDay );		astr.Add( strTemp );
			strTemp.Format( "%d%s", 120, strDay );		astr.Add( strTemp );
			strTemp.Format( "%d%s", STKLIB_DAYS_INONEYEAR, strDay );			astr.Add( strTemp );
			strTemp.Format( "%d%s", 2*STKLIB_DAYS_INONEYEAR, strDay );		astr.Add( strTemp );
			strTemp.Format( "%d%s", 3*STKLIB_DAYS_INONEYEAR, strDay );		astr.Add( strTemp );
			strTemp.Format( "%d%s", 6*STKLIB_DAYS_INONEYEAR, strDay );		astr.Add( strTemp );
			strTemp.Format( "%d%s", 10*STKLIB_DAYS_INONEYEAR, strDay );		astr.Add( strTemp );
		}
		if( astr.GetSize() > 0 )
			m_listColumn.SetItemStrings( nItem, astr );

		// Set Current Parameter
		CString	strParam;
		if( SLH_DIFF == nID )
		{
			if( bSetDefault )	AfxGetProfile().SetDiffDays( -1 );
			strParam.Format( "%d%s", AfxGetProfile().GetDiffDays(), strDay );
		}
		else if( SLH_DIFFPERCENT == nID )
		{
			if( bSetDefault )	AfxGetProfile().SetDiffPercentDays( -1 );
			strParam.Format( "%d%s", AfxGetProfile().GetDiffPercentDays(), strDay );
		}
		else if( SLH_SCOPE == nID )
		{
			if( bSetDefault )	AfxGetProfile().SetScopeDays( -1 );
			strParam.Format( "%d%s", AfxGetProfile().GetScopeDays(), strDay );
		}
		else if( SLH_RATIO_CHANGEHAND == nID )
		{
			if( bSetDefault )	AfxGetProfile().SetRatioChangeHandDays( -1 );
			strParam.Format( "%d%s", AfxGetProfile().GetRatioChangeHandDays(), strDay );
		}
		else if( SLH_RATIO_VOLUME == nID )
		{
			if( bSetDefault )	AfxGetProfile().SetRatioVolumeDays( -1 );
			strParam.Format( "%d%s", AfxGetProfile().GetRatioVolumeDays( ), strDay );
		}
		else if( SLH_RS == nID )
		{
			if( bSetDefault )	AfxGetProfile().SetRSDays( -1 );
			strParam.Format( "%d%s", AfxGetProfile().GetRSDays( ), strDay );
		}
		else if( SLH_ANNGAINS_AVERAGE == nID )
		{
			if( bSetDefault )	AfxGetProfile().SetYieldAverageDays( -1 );
			strParam.Format( "%d%s", AfxGetProfile().GetYieldAverageDays( ), strDay );
		}
		else if( SLH_ANNGAINS_STDDEV == nID || SLH_BETA == nID || SLH_SHARP == nID )
		{
			strParam.LoadString( IDS_SETCOLUMN_SAMEASYIELDAVERAGE );
		}
		if( strParam.GetLength() > 0 )
			m_listColumn.SetItemText( nItem, 2, strParam );
	}

	m_listColumn.SetComboBoxColumn( 2 );
	m_listColumn.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED );
}


void CSetColumnDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetColumnDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_SETDEFAULT, m_btnSetDefault);
	DDX_Control(pDX, IDC_NEWINDEX, m_btnNewIndex);
	DDX_Control(pDX, IDC_MOVEUP, m_btnMoveUp);
	DDX_Control(pDX, IDC_MOVEDOWN, m_btnMoveDown);
	DDX_Control(pDX, IDC_SETALL, m_btnSetAll);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_DELINDEX, m_btnDelIndex);
	DDX_Control(pDX, IDC_EDITINDEX, m_btnEditIndex);
	DDX_Control(pDX, IDC_COLUMNLIST, m_listColumn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetColumnDlg, CXFDialog)
	//{{AFX_MSG_MAP(CSetColumnDlg)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
	ON_BN_CLICKED(IDC_MOVEDOWN, OnMovedown)
	ON_BN_CLICKED(IDC_NEWINDEX, OnNewindex)
	ON_BN_CLICKED(IDC_EDITINDEX, OnEditindex)
	ON_BN_CLICKED(IDC_DELINDEX, OnDelindex)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_COLUMNLIST, OnItemchangedColumnlist)
	ON_BN_CLICKED(IDC_SETDEFAULT, OnSetdefault)
	ON_NOTIFY(NM_DBLCLK, IDC_COLUMNLIST, OnDblclkColumnlist)
	ON_BN_CLICKED(IDC_SETALL, OnSetall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetColumnDlg message handlers

BOOL CSetColumnDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_listColumn.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP
						| LVS_EX_FLATSB | LVS_EX_REGIONAL | LVS_EX_INFOTIP | LVS_EX_UNDERLINEHOT );


	CRect	rectList;
	m_listColumn.GetClientRect( &rectList );
	CString	strHeaderName, strHeaderDescript, strHeaderSetting, strUserDefine;
	strHeaderName.LoadString( IDS_SETCOLUMN_NAME );
	strHeaderDescript.LoadString( IDS_SETCOLUMN_DESCRIPT );
	strHeaderSetting.LoadString( IDS_SETCOLUMN_SETTING );
	strUserDefine.LoadString( IDS_SETCOLUMN_USERDEFINE );
	m_listColumn.InsertColumn( 0, strHeaderName, LVCFMT_CENTER, rectList.Width() / 3 - 8 );
	m_listColumn.InsertColumn( 1, strHeaderDescript, LVCFMT_CENTER, rectList.Width() / 3 - 2 );
	m_listColumn.InsertColumn( 2, strHeaderSetting, LVCFMT_CENTER, rectList.Width() / 3 - 8 );

	SetListColumnItems( AfxGetProfile().GetSListColumnsShow(), FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetColumnDlg::OnMoveup() 
{
	// TODO: Add your control notification handler code here
	if( 0 == m_listColumn.GetSelectedCount( ) )
	{
		AfxMessageBox( IDS_SETCOLUMN_NOSEL, MB_OK | MB_ICONINFORMATION );
		return;
	}

	int	nSelected	=	-1;
	for( int i=0; i<m_listColumn.GetItemCount(); i++ )
	{
		if( LVIS_SELECTED == m_listColumn.GetItemState(i,LVIS_SELECTED) )
		{
			nSelected	=	i;
			break;
		}
	}
	
	if( -1 == nSelected )
	{
		AfxMessageBox( IDS_SETCOLUMN_NOSEL, MB_OK | MB_ICONINFORMATION );
		return;
	}

	if( 0 == nSelected )
	{
		AfxMessageBox( IDS_SETCOLUMN_FIRST, MB_OK | MB_ICONINFORMATION );
		return;
	}

	DWORD	dwData	=	m_listColumn.GetItemData( nSelected );
	CString	strName	=	m_listColumn.GetItemText( nSelected, 0 );
	CString	strDis	=	m_listColumn.GetItemText( nSelected, 1 );
	BOOL	bCheck	=	m_listColumn.GetCheck( nSelected );
	if( m_listColumn.DeleteItem( nSelected ) )
	{
		int	nItem	=	m_listColumn.InsertItem( nSelected-1, strName, -1 );
		m_listColumn.SetItemText( nItem, 1, strDis );
		m_listColumn.SetItemData( nItem, dwData );
		m_listColumn.SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
		m_listColumn.SetCheck( nItem, bCheck );
	}
}

void CSetColumnDlg::OnMovedown() 
{
	// TODO: Add your control notification handler code here
	if( 0 == m_listColumn.GetSelectedCount( ) )
	{
		AfxMessageBox( IDS_SETCOLUMN_NOSEL, MB_OK | MB_ICONINFORMATION );
		return;
	}

	int	nSelected	=	-1;
	for( int i=0; i<m_listColumn.GetItemCount(); i++ )
	{
		if( LVIS_SELECTED == m_listColumn.GetItemState(i,LVIS_SELECTED) )
		{
			nSelected	=	i;
			break;
		}
	}
	
	if( -1 == nSelected )
	{
		AfxMessageBox( IDS_SETCOLUMN_NOSEL, MB_OK | MB_ICONINFORMATION );
		return;
	}

	if( m_listColumn.GetItemCount()-1 == nSelected )
	{
		AfxMessageBox( IDS_SETCOLUMN_LAST, MB_OK | MB_ICONINFORMATION );
		return;
	}

	DWORD	dwData	=	m_listColumn.GetItemData( nSelected );
	CString	strName	=	m_listColumn.GetItemText( nSelected, 0 );
	CString	strDis	=	m_listColumn.GetItemText( nSelected, 1 );
	BOOL	bCheck	=	m_listColumn.GetCheck( nSelected );
	if( m_listColumn.DeleteItem( nSelected ) )
	{
		int	nItem	=	m_listColumn.InsertItem( nSelected+1, strName, -1 );
		m_listColumn.SetItemText( nItem, 1, strDis );
		m_listColumn.SetItemData( nItem, dwData );
		m_listColumn.SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
		m_listColumn.SetCheck( nItem, bCheck );
	}
}

void CSetColumnDlg::OnNewindex() 
{
	// TODO: Add your control notification handler code here
	CBaseIndexDlg	dlg;
	if( IDOK == dlg.DoModal() )
	{
		CIndex	index	=	dlg.GetIndexInfo();

		CString	strDis;
		strDis.LoadString( IDS_SETCOLUMN_USERDEFINE );
		int nItem = m_listColumn.InsertItem( m_listColumn.GetItemCount(), index.m_strName, -1 );
		m_listColumn.SetItemText( nItem, 1, strDis );
		m_listColumn.SetItemData( nItem, index.m_nID );
		m_listColumn.SetCheck( nItem, dlg.m_bAddToList );
	}
}

void CSetColumnDlg::OnEditindex() 
{
	// TODO: Add your control notification handler code here
	if( 0 == m_listColumn.GetSelectedCount( ) )
	{
		AfxMessageBox( IDS_SETCOLUMN_NOSEL, MB_OK | MB_ICONINFORMATION );
		return;
	}

	int	nSelected	=	-1;
	for( int i=0; i<m_listColumn.GetItemCount(); i++ )
	{
		if( LVIS_SELECTED == m_listColumn.GetItemState(i,LVIS_SELECTED) )
		{
			nSelected	=	i;
			break;
		}
	}
	
	if( -1 == nSelected )
	{
		AfxMessageBox( IDS_SETCOLUMN_NOSEL, MB_OK | MB_ICONINFORMATION );
		return;
	}

	DWORD	dwData	=	m_listColumn.GetItemData( nSelected );
	if( dwData < SLH_USERDEFINE_BEGIN )
	{
		AfxMessageBox( IDS_SETCOLUMN_NOTUSERDEFINE, MB_OK | MB_ICONINFORMATION );
		return;
	}

	CBaseIndexDlg	dlg;
	dlg.m_bAddToList	=	m_listColumn.GetCheck( nSelected );
	dlg.SetIndexInfo( dwData );
	if( IDOK == dlg.DoModal() )
	{
		CIndex	index	=	dlg.GetIndexInfo();

		m_listColumn.SetItemText( nSelected, 0, index.m_strName );
		m_listColumn.SetCheck( nSelected, dlg.m_bAddToList );

		m_bUserIndexModified	=	TRUE;
	}
}

void CSetColumnDlg::OnDelindex() 
{
	// TODO: Add your control notification handler code here
	if( 0 == m_listColumn.GetSelectedCount( ) )
	{
		AfxMessageBox( IDS_SETCOLUMN_NOSEL, MB_OK | MB_ICONINFORMATION );
		return;
	}

	int	nSelected	=	-1;
	for( int i=0; i<m_listColumn.GetItemCount(); i++ )
	{
		if( LVIS_SELECTED == m_listColumn.GetItemState(i,LVIS_SELECTED) )
		{
			nSelected	=	i;
			break;
		}
	}
	
	if( -1 == nSelected )
	{
		AfxMessageBox( IDS_SETCOLUMN_NOSEL, MB_OK | MB_ICONINFORMATION );
		return;
	}

	DWORD	dwData	=	m_listColumn.GetItemData( nSelected );
	if( dwData < SLH_USERDEFINE_BEGIN )
	{
		AfxMessageBox( IDS_SETCOLUMN_NOTUSERDEFINE, MB_OK | MB_ICONINFORMATION );
		return;
	}

	if( IDYES == AfxMessageBox( IDS_SETCOLUMN_QUERYDEL, MB_YESNO | MB_ICONINFORMATION ) )
	{
		m_listColumn.DeleteItem( nSelected );
		CIndexContainer & acol = AfxGetProfile().GetSListColumnsUser();
		acol.RemoveIndex( dwData );
	}
}

void CSetColumnDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_btnOK.EnableWindow( FALSE );

	// Store Show Items
	CSPDWordArray	& anShow	= AfxGetProfile().GetSListColumnsShow( );
	CSPDWordArray	& anOrder	= AfxGetProfile().GetSListColumnsOrder( );
	anShow.RemoveAll();
	anOrder.RemoveAll();
	for( int i=0; i<m_listColumn.GetItemCount(); i++ )
	{
		UINT	nID	=	m_listColumn.GetItemData( i );
		anOrder.Add( nID );

		if( m_listColumn.GetCheck( i ) )
			anShow.Add( nID );
	}

	if( 0 == anShow.GetSize() && IDNO == AfxMessageBox( IDS_SETCOLUMN_NOSELECT, MB_YESNO ) )
		return;

	// Store Parameters
	m_listColumn.HideItemComboBox( );
	for( int nItem=0; nItem<m_listColumn.GetItemCount(); nItem++ )
	{
		// Set Item Text
		UINT	nID	=	m_listColumn.GetItemData(nItem);

		// Set Column Combo Text
		CString	strParam;
		if( SLH_DIFF == nID )
		{
			strParam	=	m_listColumn.GetItemText( nItem, 2 );
			int	nDays	=	atol( strParam );
			AfxGetProfile().SetDiffDays( nDays );
		}
		else if( SLH_DIFFPERCENT == nID )
		{
			strParam	=	m_listColumn.GetItemText( nItem, 2 );
			int	nDays	=	atol( strParam );
			AfxGetProfile().SetDiffPercentDays( nDays );
		}
		else if( SLH_SCOPE == nID )
		{
			strParam	=	m_listColumn.GetItemText( nItem, 2 );
			int	nDays	=	atol( strParam );
			AfxGetProfile().SetScopeDays( nDays );
		}
		else if( SLH_RATIO_CHANGEHAND == nID )
		{
			strParam	=	m_listColumn.GetItemText( nItem, 2 );
			int	nDays	=	atol( strParam );
			AfxGetProfile().SetRatioChangeHandDays( nDays );
		}
		else if( SLH_RATIO_VOLUME == nID )
		{
			strParam	=	m_listColumn.GetItemText( nItem, 2 );
			int	nDays	=	atol( strParam );
			AfxGetProfile().SetRatioVolumeDays( nDays );
		}
		else if( SLH_RS == nID )
		{
			strParam	=	m_listColumn.GetItemText( nItem, 2 );
			int	nDays	=	atol( strParam );
			AfxGetProfile().SetRSDays( nDays );
		}
		else if( SLH_ANNGAINS_AVERAGE == nID )
		{
			strParam	=	m_listColumn.GetItemText( nItem, 2 );
			int	nDays	=	atol( strParam );
			AfxGetProfile().SetYieldAverageDays( nDays );
		}
	}
	
	AfxGetProfile().StoreProfile( );

	CXFDialog::OnOK();

	//CMainFrame	* pMainFrame	=	AfxGetMainFrame();
	//if( pMainFrame )
	//{
	//	pMainFrame->m_DateBox.InitDates( );
	//	pMainFrame->m_DateBox.SetCurrentWindowText( );
	//}

	// Recalculate Yield Average
	if( AfxGetProfile().GetYieldAverageDays() != m_nYieldAverageDaysOld )
	{
		// Need Recalculate
		if( IDYES == AfxMessageBox( IDS_SETCOLUMN_IFRECALCULATE, MB_YESNO|MB_ICONINFORMATION ) )
		{
			AfxRecalculateYield( AfxGetProfile().GetYieldAverageDays(), TRUE );
			AfxGetStockContainer().ReloadBase( &AfxGetDB() );
			AfxGetSListStockContainer().ReRetrieveFromStatic( AfxGetActiveStrategy() );
		}
	}
	
	//	Clear the User Defined Index Cache
	if( m_bUserIndexModified )
		AfxGetSListStockContainer().ClearVariantSaveValue();
}

void CSetColumnDlg::OnItemchangedColumnlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( LVIS_SELECTED & pNMListView->uNewState )
	{
		DWORD id = m_listColumn.GetItemData( pNMListView->iItem );
		BOOL	b	=	( id >= SLH_USERDEFINE_BEGIN );
		m_btnDelIndex.EnableWindow( b );
		m_btnEditIndex.EnableWindow( b );
	}
	
	*pResult = 0;
}

void CSetColumnDlg::OnSetdefault() 
{
	// TODO: Add your control notification handler code here
	if( IDYES == AfxMessageBox( IDS_SETCOLUMN_SETDEFAULT, MB_YESNO | MB_ICONINFORMATION ) )
	{
		AfxGetProfile().SetSListColumnsDefault( );
		SetListColumnItems( AfxGetProfile().GetSListColumnsShow(), TRUE );
	}
}

void CSetColumnDlg::OnSetall() 
{
	// TODO: Add your control notification handler code here
	int	nCheck	=	m_btnSetAll.GetCheck();
	for( int i=0; i<m_listColumn.GetItemCount(); i++ )
		m_listColumn.SetCheck( i, nCheck );
}

void CSetColumnDlg::OnDblclkColumnlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	DWORD id = m_listColumn.GetItemData( pNMListView->iItem );
	if( id >= SLH_USERDEFINE_BEGIN )
		OnEditindex( );
	
	*pResult = 0;
}

