// SimuReport.cpp : implementation file
//

#include "stdafx.h"

#include "SimuReport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportListCtrl

/***
	获得策略报表ListCtrl的选中项
*/
int CReportListCtrl::GetSelected( )
{
	int	nSelected	=	-1;
	if( GetSelectedCount( ) > 0 )
	{
		for( int i=0; i<GetItemCount(); i++ )
		{
			if( LVIS_SELECTED == GetItemState(i,LVIS_SELECTED) )
			{
				nSelected	=	i;
				break;
			}
		}
	}
	return nSelected;
}

/***
	初始化策略报表ListCtrl成操作记录列表
*/
BOOL CReportListCtrl::InitOpRecordList( )
{
	SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP
						| LVS_EX_FLATSB | LVS_EX_REGIONAL | LVS_EX_INFOTIP | LVS_EX_UNDERLINEHOT );

	while( DeleteColumn(0) );

	CString	strTime, strOp, strStock, strShare, strSharePrice, strAmount, strRateCost;
	strTime.LoadString( IDS_SIMUREPORT_TIME );
	strOp.LoadString( IDS_SIMUREPORT_OP );
	strStock.LoadString( IDS_SIMUREPORT_STOCK );
	strShare.LoadString( IDS_SIMUREPORT_SHARE );
	strSharePrice.LoadString( IDS_SIMUREPORT_SHAREPRICE );
	strAmount.LoadString( IDS_SIMUREPORT_AMOUNT );
	strRateCost.LoadString( IDS_SIMUREPORT_RATECOST );
	InsertColumn( 0, strTime, LVCFMT_CENTER, 70 );
	InsertColumn( 1, strOp, LVCFMT_CENTER, 60 );
	InsertColumn( 2, strStock, LVCFMT_CENTER, 70 );
	InsertColumn( 3, strShare, LVCFMT_CENTER, 60 );
	InsertColumn( 4, strSharePrice, LVCFMT_CENTER, 56 );
	InsertColumn( 5, strAmount, LVCFMT_CENTER, 70 );
	InsertColumn( 6, strRateCost, LVCFMT_CENTER, 60 );

	return TRUE;
}

/***
	初始化操作记录列表的数据
*/
BOOL CReportListCtrl::LoadOpRecordList( COpRecordContainer & container )
{
	DeleteAllItems();

	for( int i=0; i<container.GetSize(); i++ )
	{
		OPRECORD & record = container.ElementAt(i);
		CString	strTime, strOp, strStock, strShare, strSharePrice, strAmount, strRateCost;
		strTime	=	CSPTime(record.time).Format( "%Y/%m/%d" );
		strOp	=	AfxGetStrategyOpTypeString( record.lOpType );
		CStockInfo	info;
		strStock	=	record.szCode;
		if( AfxGetStockContainer().GetStockInfo( record.szCode, &info ) )
			strStock	=	info.GetStockName();
		strShare.Format( "%u", record.dwShare );
		strSharePrice.Format( "%.2f", record.dSharePrice );
		strAmount.Format( "%.2f", record.dwShare*record.dSharePrice );
		strRateCost.Format( "%.2f", record.dRateCost );

		InsertItem( i, strStock );
		SetItemText( i, 0, strTime );
		SetItemText( i, 1, strOp );
		SetItemText( i, 2, strStock );
		SetItemText( i, 3, strShare );
		SetItemText( i, 4, strSharePrice );
		SetItemText( i, 5, strAmount );
		SetItemText( i, 6, strRateCost );
		SetItemData( i, i );
	}

	if( GetItemCount() > 0 )
		SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED );

	return TRUE;
}

/***
	初始化策略报表ListCtrl成下一步操作列表
*/
BOOL CReportListCtrl::InitNextOpList( )
{
	SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP
						| LVS_EX_FLATSB | LVS_EX_REGIONAL | LVS_EX_INFOTIP | LVS_EX_UNDERLINEHOT );

	while( DeleteColumn(0) );

	CString	strOp, strStock, strShare, strSharePrice, strAmount, strRateCost;
	strOp.LoadString( IDS_SIMUREPORT_OP );
	strStock.LoadString( IDS_SIMUREPORT_STOCK );
	strShare.LoadString( IDS_SIMUREALOP_SHARE );
	strSharePrice.LoadString( IDS_SIMUREALOP_SHAREPRICE );
	strAmount.LoadString( IDS_SIMUREPORT_AMOUNT );
	strRateCost.LoadString( IDS_SIMUREPORT_RATECOST );
	InsertColumn( 0, strOp, LVCFMT_CENTER, 70 );
	InsertColumn( 1, strStock, LVCFMT_CENTER, 80 );
	InsertColumn( 2, strShare, LVCFMT_CENTER, 70 );
	InsertColumn( 3, strSharePrice, LVCFMT_CENTER, 70 );
	InsertColumn( 4, strAmount, LVCFMT_CENTER, 80 );
	InsertColumn( 5, strRateCost, LVCFMT_CENTER, 70 );
	
	return TRUE;
}

/***
	初始化下一步操作记录列表数据
*/
BOOL CReportListCtrl::LoadNextOpList( COpRecordContainer & container )
{
	DeleteAllItems();

	for( int i=0; i<container.GetSize(); i++ )
	{
		OPRECORD & record = container.ElementAt(i);
		CString	strOp, strStock, strShare, strSharePrice, strAmount, strRateCost;
		strOp	=	AfxGetStrategyOpTypeString( record.lOpType );
		CStockInfo	info;
		strStock	=	record.szCode;
		if( AfxGetStockContainer().GetStockInfo( record.szCode, &info ) )
			strStock	=	info.GetStockName();
		strShare.Format( "%u", record.dwShare );
		strSharePrice.Format( "%.2f", record.dSharePrice );
		strAmount.Format( "%.2f", record.dwShare*record.dSharePrice );
		strRateCost.Format( "%.2f", record.dRateCost );

		InsertItem( i, strStock );
		SetItemText( i, 0, strOp );
		SetItemText( i, 1, strStock );
		SetItemText( i, 2, strShare );
		SetItemText( i, 3, strSharePrice );
		SetItemText( i, 4, strAmount );
		SetItemText( i, 5, strRateCost );
		SetItemData( i, i );
	}

	if( GetItemCount() > 0 )
		SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED );

	return TRUE;
}

/***
	初始化策略报表ListCtrl成拥有股票列表
*/
BOOL CReportListCtrl::InitStockOwnList( )
{
	SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP
						| LVS_EX_FLATSB | LVS_EX_REGIONAL | LVS_EX_INFOTIP | LVS_EX_UNDERLINEHOT );

	while( DeleteColumn(0) );

	CString	strStock, strShare, strBuyPrice, strNowPrice;
	strStock.LoadString( IDS_SIMUREPORT_STOCK );
	strShare.LoadString( IDS_SIMUREPORT_OWNSHARE );
	strBuyPrice.LoadString( IDS_SIMUREPORT_BUYPRICE );
	strNowPrice.LoadString( IDS_SIMUREPORT_NOWPRICE );
	InsertColumn( 0, strStock, LVCFMT_CENTER, 100 );
	InsertColumn( 1, strShare, LVCFMT_CENTER, 100 );
	InsertColumn( 2, strBuyPrice, LVCFMT_CENTER, 100 );
	InsertColumn( 3, strNowPrice, LVCFMT_CENTER, 100 );
	return TRUE;
}

/***
	初始化拥有股票列表的数据
*/
BOOL CReportListCtrl::LoadStockOwnList( CStockOwnContainer & stockown, double dCash, CSPTime tmCur, double dAsset, CTechStockContainer & techstocks )
{
	DeleteAllItems();

	int i;
	for( i=0; i<stockown.GetSize(); i++ )
	{
		STOCKOWN & own = stockown.ElementAt(i);

		CString	strStock, strShare, strBuyPrice, strNowPrice;
		CStockInfo	info;
		strStock	=	own.szCode;
		if( AfxGetStockContainer().GetStockInfo( own.szCode, &info ) )
			strStock	=	info.GetStockName();
		strShare.Format( "%u", own.dwShare );
		strBuyPrice.Format( "%.2f", own.dBuyPrice );
		double	dNowPrice = 0;
		if( techstocks.GetClosePrice( own.szCode, tmCur.GetTime(), &dNowPrice ) )
			strNowPrice.Format( "%.2f", dNowPrice );

		InsertItem( i, strStock );
		SetItemText( i, 0, strStock );
		SetItemText( i, 1, strShare );
		SetItemText( i, 2, strBuyPrice );
		SetItemText( i, 3, strNowPrice );
		SetItemData( i, i );
	}

	CString	strCash;
	strCash.LoadString( IDS_SIMUREPORT_CASH );
	InsertItem( i, strCash );
	strCash.Format( "%.2f", dCash );
	SetItemText( i, 1, strCash );
	i++;

	CString	strAsset;
	strAsset.LoadString( IDS_SIMUREPORT_ASSET );
	InsertItem( i, strAsset );
	strAsset.Format( "%.2f", dAsset );
	SetItemText( i, 1, strAsset );

	if( GetItemCount() > 0 )
		SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED );

	return TRUE;
}

/***
	拷贝列表到剪贴板
*/
void CReportListCtrl::Copy( )
{
	// Write to shared file (REMEBER: CF_TEXT is ANSI, not UNICODE, so we need to convert)
	CSharedFile sf(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	// Get a tab delimited string to copy to cache
	CString str;

	// Column
	CHeaderCtrl	* pHeader	=	GetHeaderCtrl();
	int	nColCount	=	pHeader ? pHeader->GetItemCount() : 0 ;

	str.Empty();
	char buffer[256];
	LVCOLUMN	column;
	memset( &column, 0, sizeof( column ) );
	column.mask	=	LVCF_TEXT;
	column.pszText	=	buffer;
	column.cchTextMax	=	255;
	int	nColumn	=	0;
	while( GetColumn( nColumn++, &column ) )
	{
		str	+=	buffer;
		str	+=	_T("\t");
	}
	str	+=	_T("\r\n");

	sf.Write(T2A(str.GetBuffer(1)), str.GetLength());
	str.ReleaseBuffer();

	// Data
	for( int nRow = 0; nRow <GetItemCount(); nRow ++ )
	{
		str.Empty();
		for( int nCol = 0; nCol <nColCount; nCol ++ )
		{
			CString	strTemp	=	GetItemText( nRow, nCol );
			str	+=	(strTemp.IsEmpty() ? _T(" ") : strTemp );
			if( nCol != nColCount-1 )
				str	+=	_T("\t");
		}
		if( nRow != GetItemCount()-1 )
			str += _T("\r\n");
		sf.Write(T2A(str.GetBuffer(1)), str.GetLength());
		str.ReleaseBuffer();
	}
	char c = '\0';
	sf.Write(&c, 1);

	DWORD dwLen = (DWORD)sf.GetLength();
	HGLOBAL hMem = sf.Detach();
	if (!hMem) 
		return;

	hMem = ::GlobalReAlloc(hMem, dwLen, GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
	if (!hMem) 
		return;

	// Cache data
	COleDataSource* pSource = new COleDataSource();
	pSource->CacheGlobalData(CF_TEXT, hMem);
	pSource->SetClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// CCurveButton

#define	SRC_LEFTMARGIN		35
#define	SRC_RIGHTMARGIN		10
#define	SRC_TOPMARGIN		10
#define	SRC_BOTTOMMARGIN	35

#define	SRC_SCROLLBARHEIGHT	18

CCurveButton::CCurveButton( )
{
	m_dStartAmount	=	1000000;
	m_nStepWeight	=	2;
	m_nHScrollMax	=	0;
}

CCurveButton::~CCurveButton( )
{
}

/***
	设定曲线显示的资产数据序列
*/
void CCurveButton::SetAssetSerial( CAssetSerialContainer & src, double dStartAmount )
{
	m_AssetSerial.Copy( src );
	
	m_dStartAmount	=	dStartAmount;
	ASSERT( m_dStartAmount > 1e-4 );

	ResetScrollBars( );
}

BOOL CCurveButton::Initialize( )
{
	InitializeFlatSB(GetSafeHwnd());
	ResetScrollBars( );
	return TRUE;
}

BEGIN_MESSAGE_MAP(CCurveButton, CButton)
    //{{AFX_MSG_MAP(CCurveButton)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCurveButton::PreSubclassWindow() 
{
	// Switch to owner-draw
	ModifyStyle(SS_TYPEMASK, BS_OWNERDRAW, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();
}

void CCurveButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC * pDC	=	GetDC();
	if( NULL == pDC )
		return;

	CRect	rectClient;
	GetClientRect( &rectClient );

	pDC->FillSolidRect( rectClient, AfxGetProfile().GetColor(CColorClass::clrGraphBK) );
	
	int scrollpos	=	GetScrollPos32( SB_HORZ );
	DrawCurve( pDC, rectClient, scrollpos );
}

/***
	画曲线
*/
BOOL CCurveButton::DrawCurve( CDC * pDC, CRect rectClient, int nStart )
{
	CRect	rectDraw;
	int nCount;
	double	dMin, dMax;
	if( !PrepareDrawData( rectClient, nStart, m_nStepWeight, &rectDraw, &nStart, &nCount, &dMin, &dMax ) )
		return FALSE;

	DrawAxis( pDC, rectDraw, 15, 5, dMin, dMax, FALSE, TRUE, 2 );

	COLORREF	clrBK		=	AfxGetProfile().GetColor(CColorClass::clrGraphBK);
	COLORREF	clrLine1	=	AfxGetProfile().GetColor(CColorClass::clrLine1);
	COLORREF	clrLine2	=	AfxGetProfile().GetColor(CColorClass::clrLine3);
	COLORREF	clrBorder	=	AfxGetProfile().GetColor(CColorClass::clrBorder);
	COLORREF	clrText		=	AfxGetProfile().GetColor(CColorClass::clrText);
	int	yYieldLast, yYieldNow, yStoreLast, yStoreNow;
	int	xDateLast = 0;
	int	xMedium	= rectDraw.left;
	BOOL	bHasLast = FALSE;
	double	dYield = 1, dStore = 0;

	CFont	* pOldFont	=	AfxSelectDCFont( pDC, 12 );
	pDC->SetBkColor( clrBK );
	pDC->SetTextColor( clrText );
	for( int k=nStart; k<nStart+nCount && k<m_AssetSerial.GetSize(); k++ )
	{
		ASSETSERIAL	&	serial	=	m_AssetSerial.ElementAt(k);
		dYield	=	serial.dAsset / m_dStartAmount;
		if( serial.dAsset > 1e-4 )
			dStore	=	(serial.dAsset - serial.dCash) / serial.dAsset;

		xMedium	+=	m_nStepWeight;

		// Draw
		yYieldNow	=	int( rectDraw.bottom - (rectDraw.Height()-2) * (dYield - dMin) / (dMax-dMin) );
		yStoreNow	=	int( rectDraw.bottom - (rectDraw.Height()-2) * (dStore - dMin) / (dMax-dMin) );
		if( bHasLast )
		{
			if( yYieldLast > rectDraw.bottom || yYieldLast < rectDraw.top
				|| yYieldNow > rectDraw.bottom || yYieldNow < rectDraw.top )
				continue;
			if( yStoreLast > rectDraw.bottom || yStoreLast < rectDraw.top
				|| yStoreNow > rectDraw.bottom || yStoreNow < rectDraw.top )
				continue;
			CPen	penLocal1( PS_SOLID, 1, clrLine1 );
			CPen * pOldPen = pDC->SelectObject( &penLocal1 );
			pDC->MoveTo( xMedium-m_nStepWeight, yYieldLast );
			pDC->LineTo( xMedium, yYieldNow );

			CPen	penLocal2( PS_SOLID, 1, clrLine2 );
			pDC->SelectObject( &penLocal2 );
			pDC->MoveTo( xMedium-m_nStepWeight, yStoreLast );
			pDC->LineTo( xMedium, yStoreNow );
			pDC->SelectObject( pOldPen );
		}
		yYieldLast		=	yYieldNow;
		yStoreLast		=	yStoreNow;
		bHasLast	=	TRUE;

		if( k > 0 )	//	Draw Date
		{
			CSPTime	tmLast(m_AssetSerial.ElementAt(k-1).time);
			CSPTime	tmNow(serial.time);

			CString	strText;
			if( tmLast.GetYear() != tmNow.GetYear()
				|| tmLast.GetMonth() != tmNow.GetMonth() )
				strText	=	tmNow.Format( "%y/%m/%d" );
			if( strText.GetLength() > 0 && xDateLast < xMedium && xMedium+pDC->GetTextExtent(strText).cx < rectDraw.right )
			{
				pDC->TextOut( xMedium+2, rectDraw.bottom+1, strText );
				xDateLast	=	xMedium+2 + pDC->GetTextExtent(strText).cx;

				CPen	penLocal( PS_SOLID, 1, clrBorder );
				CPen * pOldPen = pDC->SelectObject( &penLocal );
				pDC->MoveTo( xMedium, rectDraw.bottom );
				pDC->LineTo( xMedium, rectDraw.bottom+13 );
				pDC->SelectObject( pOldPen );
			}
		}
	}

	// Draw Text
	pDC->SetBkColor( clrBK );
	AfxSelectDCFont( pDC, 14 );
	CString	strText;
	strText.LoadString( IDS_SIMUREPORT_YIELDLINE );
	pDC->SetTextColor( clrLine1 );
	pDC->TextOut( rectDraw.left+rectDraw.Width()/10, rectDraw.bottom+17, strText );

	strText.LoadString( IDS_SIMUREPORT_STORELINE );
	pDC->SetTextColor( clrLine2 );
	pDC->TextOut( rectDraw.left+6*rectDraw.Width()/10, rectDraw.bottom+17, strText );
	pDC->SelectObject( pOldFont );

	return TRUE;
}

/***
	准备数据
*/
BOOL CCurveButton::PrepareDrawData(CRect rectClient, int nStart, int nWeight,
								   LPRECT prectDraw, int *pnStartNew, int *pnCount, double *pdMin, double *pdMax )
{
	// RectDraw
	CRect	rectDraw( rectClient.left+SRC_LEFTMARGIN, rectClient.top+SRC_TOPMARGIN, 
					rectClient.right-SRC_RIGHTMARGIN, rectClient.bottom-SRC_BOTTOMMARGIN );
	int		nStartNew = nStart, nCount = 0;
	double	dMin = 0, dMax = 0;
	
	// Start
	if( nWeight <= 0 || m_AssetSerial.GetSize() <= 0 )
		return FALSE;
	nCount	=	rectDraw.Width() / nWeight;
	if( nStartNew >= m_AssetSerial.GetSize() || nStartNew < 0 )
		nStartNew	=	-1;

	if( -1 == nStartNew )
	{
		nStartNew	=	m_AssetSerial.GetSize()-nCount;
		if( nStartNew < 0 )
			nStartNew	=	0;
	}
	if( nStartNew+nCount-1 >= m_AssetSerial.GetSize() )
		nCount	=	m_AssetSerial.GetSize() - nStartNew;
	if( nStartNew < 0 || nStartNew >= m_AssetSerial.GetSize() )
		return FALSE;

	// MinMax
	for( int i=0; i<m_AssetSerial.GetSize(); i++ )
	{
		if( m_AssetSerial.ElementAt(i).dAsset > dMax )
			dMax	=	m_AssetSerial.ElementAt(i).dAsset;
	}
	if( m_dStartAmount < 1e-4 )
		return FALSE;
	dMax	=	dMax / m_dStartAmount;

	if( prectDraw )	*prectDraw	=	rectDraw;
	if( pnStartNew )*pnStartNew	=	nStartNew;
	if( pnCount )	*pnCount	=	nCount;
	if( pdMin )		*pdMin		=	dMin;
	if( pdMax )		*pdMax		=	dMax;
	return TRUE;
}

void CCurveButton::ResetScrollBars()
{
	if ( !::IsWindow(GetSafeHwnd()) )
		return;

	ASSERT( m_nStepWeight > 0 );
	if( m_nStepWeight <= 0 )
		return;

	CRect	rectClient;
	GetClientRect( &rectClient );
	int	nCountPerPage	=	(rectClient.Width()-SRC_LEFTMARGIN-SRC_RIGHTMARGIN)/m_nStepWeight;

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask  = SIF_PAGE;
	si.nPage  = nCountPerPage;
	FlatSB_SetScrollInfo(GetSafeHwnd(),SB_HORZ, &si, FALSE); 

	m_nHScrollMax = 0;
	if( nCountPerPage < m_AssetSerial.GetSize() )
		m_nHScrollMax = m_AssetSerial.GetSize()-1;

	ASSERT( m_nHScrollMax < INT_MAX );
	ASSERT( m_nHScrollMax < SHRT_MAX );
	FlatSB_SetScrollRange(GetSafeHwnd(),SB_HORZ, 0, m_nHScrollMax, TRUE);
}

int CCurveButton::GetScrollPos32(int nBar, BOOL bGetTrackPos /* = FALSE */)
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);

	if (bGetTrackPos)
	{
		si.fMask	=	SIF_TRACKPOS;
		if (FlatSB_GetScrollInfo(GetSafeHwnd(),nBar, &si))
			return si.nTrackPos;
	}
	else 
	{
		si.fMask	=	SIF_POS;
		if (FlatSB_GetScrollInfo(GetSafeHwnd(),nBar, &si))
			return si.nPos;
	}
	return 0;
}

BOOL CCurveButton::SetScrollPos32(int nBar, int nPos, BOOL bRedraw /* = TRUE */)
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask  = SIF_POS;
	si.nPos   = nPos;
	return FlatSB_SetScrollInfo(GetSafeHwnd(),nBar, &si, bRedraw);
}

int CCurveButton::GetScrollPage( int nBar )
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask  = SIF_PAGE;
	if( FlatSB_GetScrollInfo(GetSafeHwnd(), nBar, &si ) )
		return si.nPage;
	return 10;
}

/***
	响应垂直滚动条消息
*/
void CCurveButton::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int scrollPos = GetScrollPos32(SB_HORZ);

	switch (nSBCode)
	{
		case SB_LINERIGHT:
			if( scrollPos < m_nHScrollMax )
			{
				SetScrollPos32(SB_HORZ, scrollPos + 1);
				if( GetScrollPos32(SB_HORZ) != scrollPos )
					Invalidate( );
			}
		break;

		case SB_LINELEFT:
			if( scrollPos > 0 )
			{
				SetScrollPos32(SB_HORZ, max(0,scrollPos - 1));
				Invalidate( );
			}
			break;

		case SB_PAGERIGHT:
			if( scrollPos < m_nHScrollMax )
			{
				int pos = min(m_nHScrollMax, scrollPos + GetScrollPage(SB_HORZ));
				SetScrollPos32( SB_HORZ, pos);
				Invalidate( );
			}
		break;

		case SB_PAGELEFT:
			if (scrollPos > 0)
			{
				int pos = max(0, scrollPos - GetScrollPage(SB_HORZ));
				SetScrollPos32(SB_HORZ, pos);
				Invalidate( );
			}
		break;

		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			{
				SetScrollPos32(SB_HORZ, GetScrollPos32(SB_HORZ, TRUE));
				Invalidate( );
			}
		break;

		case SB_LEFT:
			if (scrollPos > 0)
			{
				SetScrollPos32(SB_HORZ, 0);
				Invalidate();
			}
		break;

		case SB_RIGHT:
		if (scrollPos < m_nHScrollMax)
			{
				SetScrollPos32(SB_HORZ, m_nHScrollMax);
				Invalidate();
			}
		break;

		default: break;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CSimuReport

CSimuReport::CSimuReport(CWnd* pParent /*=NULL*/)
	: CXFDialog(CSimuReport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimuReport)
	//}}AFX_DATA_INIT
	m_pStrategy	=	NULL;
	SetTransparent( FALSE );
}

CSimuReport::~CSimuReport()
{
}

void CSimuReport::SetStrategy( CStrategy * pStrategy )
{
	m_pStrategy	=	pStrategy;
}

/***
	设定策略模拟报表统计数据
*/
BOOL CSimuReport::SetResults( )
{
	if( NULL == m_pStrategy )
		return FALSE;

	CString	strBuyTimes, strVictoryTimes, strYieldMax, strYieldMin, strYieldFin, strYieldIndex, strYieldStdDev, strXiapu;

	int nTimes = 0, nVictoryTimes = 0;
	double	dYieldMax = 0, dYieldMin = 0;
	COpRecordContainer & container = m_pStrategy->SimuGetOpRecord();
	container.StatResults( &nTimes, &nVictoryTimes, &dYieldMax, &dYieldMin );

	strBuyTimes.Format( "%u", nTimes );
	m_editBuyTimes.SetWindowText( strBuyTimes );

	strVictoryTimes.Format( "%u", nVictoryTimes );
	m_editVictoryTimes.SetWindowText( strVictoryTimes );

	strYieldMax.Format( "%.2f%%", dYieldMax*100 );
	m_editYieldMax.SetWindowText( strYieldMax );

	strYieldMin.Format( "%.2f%%", dYieldMin*100 );
	m_editYieldMin.SetWindowText( strYieldMin );

	strYieldFin.Format( "%.2f%%", m_pStrategy->SimuGetCurrentYieldPercent()*100 );
	m_editYieldFin.SetWindowText( strYieldFin );

	strYieldIndex.Format( "%.2f%%", m_pStrategy->SimuGetCurrentYieldIndexPercent()*100 );
	m_editYieldIndex.SetWindowText( strYieldIndex );

	double	dStdDev = 0, dXiapu = 0;
	CAssetSerialContainer & assetserial = m_pStrategy->SimuGetAssetSerial();
	if( assetserial.GetStdDev( &dStdDev, &dXiapu ) )
	{
		strYieldStdDev.Format( "%.2f%%", dStdDev*100 );
		strXiapu.Format( "%.2f%%", dXiapu*100 );
	}
	m_editYieldStdDev.SetWindowText( strYieldStdDev );
	m_editXiapu.SetWindowText( strXiapu );

	return TRUE;
}

void CSimuReport::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimuReport)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_XIAPU, m_editXiapu);
	DDX_Control(pDX, IDC_YIELDSTDDEV, m_editYieldStdDev);
	DDX_Control(pDX, IDC_STATICEXPLAIN, m_staticExplain);
	DDX_Control(pDX, IDC_CURVE, m_curve);
	DDX_Control(pDX, IDC_COPY, m_btnCopy);
	DDX_Control(pDX, IDC_YIELDINDEX, m_editYieldIndex);
	DDX_Control(pDX, IDC_YIELDFIN, m_editYieldFin);
	DDX_Control(pDX, IDC_YIELDMIN, m_editYieldMin);
	DDX_Control(pDX, IDC_YIELDMAX, m_editYieldMax);
	DDX_Control(pDX, IDC_VICTORYTIMES, m_editVictoryTimes);
	DDX_Control(pDX, IDC_BUYTIMES, m_editBuyTimes);
	DDX_Control(pDX, IDC_OPRECORD, m_listOpRecord);
	DDX_Control(pDX, IDC_STOCKOWN, m_listStockOwn);
	DDX_Control(pDX, IDC_TABCTRL, m_tabctrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSimuReport, CXFDialog)
	//{{AFX_MSG_MAP(CSimuReport)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCTRL, OnSelchangeTabctrl)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimuReport message handlers

BOOL CSimuReport::OnInitDialog()
{
	CXFDialog::OnInitDialog();

	// TODO: Add extra initialization here
	ASSERT( m_pStrategy );

	// Set Attr
	CString	strItem;
	strItem.LoadString( IDS_SIMUREPORT_OPRECORD );
	m_tabctrl.InsertItem( 0, strItem );
	strItem.LoadString( IDS_SIMUREPORT_CURVE );
	m_tabctrl.InsertItem( 1, strItem );
	strItem.LoadString( IDS_SIMUREPORT_STOCKOWN );
	m_tabctrl.InsertItem( 2, strItem );

	// Init
	m_listOpRecord.InitOpRecordList( );
	m_listStockOwn.InitStockOwnList( );
	m_listOpRecord.LoadOpRecordList( m_pStrategy->SimuGetOpRecord() );
	m_listStockOwn.LoadStockOwnList( m_pStrategy->SimuGetStockOwn(), m_pStrategy->SimuGetCurrentCash(), m_pStrategy->SimuGetCurrentTime().GetTime(),
						m_pStrategy->SimuGetAsset(m_pStrategy->SimuGetCurrentTime()), m_pStrategy->GetTechStockContainer() );

	SetResults( );

	// Curve Init
	m_curve.SetAssetSerial( m_pStrategy->SimuGetAssetSerial(), m_pStrategy->GetOpParam().m_nStartAmount );
	m_curve.Initialize( );

	// Tab Show
	m_listOpRecord.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
	m_curve.ShowWindow( SW_HIDE );
	m_listStockOwn.ShowWindow( SW_HIDE );
	m_btnCopy.EnableWindow( TRUE );
	CString	strExplain;
	m_staticExplain.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
	strExplain.LoadString( IDS_SIMUREALOP_EXPLAINOPRECORD );
	m_staticExplain.SetWindowText( strExplain );
	
	UpdateData( FALSE );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSimuReport::OnSelchangeTabctrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int	nCurSel	=	m_tabctrl.GetCurSel();
	CString	strExplain;
	switch( nCurSel )
	{
	case	0:
		m_listOpRecord.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
		m_curve.ShowWindow( SW_HIDE );
		m_listStockOwn.ShowWindow( SW_HIDE );
		m_btnCopy.EnableWindow( TRUE );
		m_staticExplain.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
		strExplain.LoadString( IDS_SIMUREALOP_EXPLAINOPRECORD );
		m_staticExplain.SetWindowText( strExplain );
		break;
	case	1:
		m_listOpRecord.ShowWindow( SW_HIDE );
		m_curve.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
		m_listStockOwn.ShowWindow( SW_HIDE );
		m_btnCopy.EnableWindow( FALSE );
		m_staticExplain.ShowWindow( SW_HIDE );
		break;
	case	2:
		m_listOpRecord.ShowWindow( SW_HIDE );
		m_curve.ShowWindow( SW_HIDE );
		m_listStockOwn.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
		m_btnCopy.EnableWindow( TRUE );
		m_staticExplain.ShowWindow( SW_HIDE );
		break;
	default:
		ASSERT( FALSE );
		break;
	}
	*pResult = 0;
}

void CSimuReport::OnCopy() 
{
	if( m_listOpRecord.IsWindowVisible() )
	{
		m_listOpRecord.Copy( );
	}
	else if( m_listStockOwn.IsWindowVisible() )
	{
		m_listStockOwn.Copy( );
	}
}

