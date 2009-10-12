// StockGraph.cpp : implementation of the CStockGraph class
//

#include "stdafx.h"

#include "StockGraph.h"
#include "../Dialog/EditKDATADlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Some Useful static function

void DrawLine( CDC * pDC, int nWidth, COLORREF clr, int xStart, int yStart, int xEnd, int yEnd )
{
	CPen	pen( PS_SOLID, nWidth, clr );
	CPen *pOldPen = pDC->SelectObject( &pen );
	pDC->MoveTo( xStart, yStart );
	pDC->LineTo( xEnd, yEnd );
	pDC->SelectObject( pOldPen );
}

void DrawDashLine( CDC * pDC, int nWidth, COLORREF clr, int xStart, int yStart, int xEnd, int yEnd )
{
	CPen	pen( PS_SOLID, nWidth, clr );
	CPen *pOldPen = pDC->SelectObject( &pen );
	int	x = xStart, y = yStart;
	while( x < xEnd || y < yEnd )
	{
		if( x > xEnd )	x	=	xEnd;
		if( y > yEnd )	y	=	yEnd;
		pDC->MoveTo( x, y );
		int	xNew = x+3;
		int	yNew = y+3;
		if( xNew > xEnd )	xNew	=	xEnd;
		if( yNew > yEnd )	yNew	=	yEnd;
		pDC->LineTo( xNew, yNew );
		x	=	xNew+3;
		y	=	yNew+3;
	}
	pDC->SelectObject( pOldPen );
}

void DrawDashAxis( CDC * pDC, int nWidth, COLORREF clr, int nCount, int xStart, int yStart, int xEnd, int yEnd )
{
	int	nHeight	=	yEnd - yStart;
	if( nHeight <= 0 )
		return;
	if( nCount <= 0 )
		return;
	for( int k=0; k<nCount; k++ )
		DrawDashLine( pDC, nWidth, clr, xStart, yStart+k*nHeight/nCount, xEnd, yStart+k*nHeight/nCount );
}

void DrawAxisNumber(CDC * pDC, COLORREF clr, int nCount,
					int xStart, int yStart, int xEnd, int yEnd,
					double fMin, double fMax, BOOL bDrawMin, BOOL bDrawMax, int nDotCount )
{
	int	nHeight	=	yEnd - yStart;
	if( nHeight <= 0 )
		return;
	if( nCount <= 0 )
		return;
	ASSERT( nDotCount >= 0 );

	CString	strFmt;
	strFmt.Format( "%%.%df", nDotCount );

	pDC->SetTextColor( clr );
	CString	strText;
	CSize	sizeText;
	
	for( int k=0; k<=nCount; k++ )
	{
		if( !bDrawMin && 0 == k )
			continue;
		if( !bDrawMax && nCount == k )
			continue;
		double	dTemp	=	fMin+k*(fMax-fMin)/nCount;
		if( dTemp < 0 && dTemp > (0-0.6*pow((double)10,0-nDotCount)) )
			dTemp	=	0;
		strText.Format( strFmt, dTemp );
		sizeText	=	pDC->GetTextExtent( strText );
		pDC->TextOut( xStart-sizeText.cx-1, yStart+(nCount-k)*nHeight/nCount-7, strText );
	}
}

BOOL DrawAxis(CDC * pDC, CRect rect, int nMarginTop, int nAxisLines, double dMin, double dMax, BOOL bDrawMin, BOOL bDrawMax, int nDotCount )
{
	DECLARE_COLOR_DEFINATION

	if( rect.Height() <= nMarginTop )
		return FALSE;

	// Draw Axis
	pDC->Draw3dRect( &rect, clrBorder, clrBorder );
	DrawDashAxis(	pDC, 1, clrBorder, nAxisLines,
					rect.left, rect.top+nMarginTop, rect.right, rect.bottom );

	// Draw Axis Number, Will Extent the rect
	CFont	* pOldFont	=	AfxSelectDCFont( pDC, 12 );
	pDC->SetBkColor( clrBK );
	DrawAxisNumber( pDC, clrText, nAxisLines,
					rect.left, rect.top+nMarginTop, rect.right, rect.bottom,
					dMin, dMax, bDrawMin, bDrawMax, nDotCount );
	pDC->SelectObject( pOldFont );
	return TRUE;
}

BOOL DrawTechTitle(CDC * pDC, int x, int y, LPCTSTR lpszText,
				   UINT nTextAlign, int nFontSize, COLORREF clrBK, COLORREF clrText )
{
	CFont	* pOldFont	=	AfxSelectDCFont( pDC, nFontSize );
	pDC->SetBkColor( clrBK );
	pDC->SetTextColor( clrText );
	UINT nOldAlign = pDC->SetTextAlign( nTextAlign );
	pDC->TextOut( x, y, lpszText );
	pDC->SetTextAlign( nOldAlign );
	pDC->SelectObject( pOldFont );
	return TRUE;
}

BOOL FixRect( CRect & rect, LPRECT lpRectMax )
{
	if( lpRectMax )
	{
		if( rect.left < lpRectMax->left )
			rect.left	=	lpRectMax->left;
		if( rect.top < lpRectMax->top )
			rect.top	=	lpRectMax->top;
		if( rect.right > lpRectMax->right )
			rect.right	=	lpRectMax->right;
		if( rect.bottom > lpRectMax->bottom )
			rect.bottom	=	lpRectMax->bottom;
		return TRUE;
	}
	return FALSE;
}

BOOL DrawTextWithRect(CDC *pDC, int left, int top, int right, int bottom, LPCTSTR lpszText,
					  COLORREF clrRect, COLORREF clrText, COLORREF clrBK, LPRECT lpRectMax )
{
	CRect	rectDraw	=	CRect( left, top, right, bottom );
	if( lpRectMax )
		FixRect( rectDraw, lpRectMax );
	pDC->Draw3dRect( &rectDraw, clrRect, clrRect );
	pDC->SetTextColor( clrText );
	pDC->SetBkColor( clrBK );
	pDC->DrawText( lpszText, rectDraw, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	return TRUE;
}

BOOL DrawTextWithRect(CDC *pDC, int left, int top, int right, int bottom, UINT nIDString,
					  COLORREF clrRect, COLORREF clrText, COLORREF clrBK, LPRECT lpRectMax )
{
	CString	string;
	string.LoadString( nIDString );
	return DrawTextWithRect( pDC, left, top, right, bottom, string, clrRect, clrText, clrBK, lpRectMax );
}

/////////////////////////////////////////////////////////////////////////////
// CStockGraph

CStockGraph::CStockGraph( )
{
	ResetMargins( );

	m_pParent		=	NULL;
	m_nThickness	=	m_nDefaultThickness;
	m_dwLatestDate		=	-1;

	m_ptSelectBegin	=	CPoint(0,0);
	m_ptSelectEnd	=	CPoint(0,0);

	m_nReportWhat	=	CStockGraph::reportParam;

	SetKType( );
	SetKFormat( AfxGetProfile().GetGraphKFormat() );
	SetMaindataType( AfxGetProfile().GetGraphMaindataType() );
#ifdef	CLKLAN_ENGLISH_US
	SetKLineMode( CStockGraph::klineAmerica );
#else
	SetKLineMode( CStockGraph::klineCandle );
#endif
	ResetIndex( );

	m_rectAll.SetRectEmpty();

	m_dMaxPrice			=	0;
	m_dMinPrice			=	0;

	m_bCursorBorder		=	FALSE;

	m_nSelectedTechID	=	STT_INVALID;
}

CStockGraph::~CStockGraph( )
{
	ClearCurStock( );
}

void CStockGraph::SetParent( CWnd * pParent )
{
	m_pParent	=	pParent;
}

void CStockGraph::ResetMargins(	int nMarginTop, int nMarginBottom,
								int nHeightSubtitle, int nWidthSubtitle,
								int nWidthParameter, int nDefaultThickness )
{
	ASSERT( nMarginTop >= 0 && nMarginBottom >= 0 );
	ASSERT( nHeightSubtitle >= 0 && nWidthSubtitle >= 0 );
	ASSERT( nWidthParameter >= 0 );
	ASSERT( nDefaultThickness > 0 );

	m_nMarginTop		=	nMarginTop;
	m_nMarginBottom		=	nMarginBottom;
	m_nHeightSubtitle	=	nHeightSubtitle + 2;
	m_nWidthSubtitle	=	nWidthSubtitle;
	m_nWidthParameter	=	nWidthParameter;
	m_nDefaultThickness	=	nDefaultThickness;
}

LRESULT CStockGraph::OnStkReceiverData( WPARAM wParam, LPARAM lParam )
{
	PCOMMPACKET	pCommPacket	=	(PCOMMPACKET)lParam;

	switch( wParam )
	{
	case CStock::dataReport:
		if( pCommPacket && CStock::dataReport == pCommPacket->m_dwDataType )
		{
			DWORD i;
			for( i=0; i<pCommPacket->m_dwCount; i++ )
			{
				if( m_CurStock.GetStockInfo().IsEqualTo( pCommPacket->m_pReport[i].m_dwMarket, pCommPacket->m_pReport[i].m_szCode ) )
					break;
			}
			if( i != pCommPacket->m_dwCount )
			{
				// 加入最新一天的K线数据
				PrepareStockData( TRUE );
				if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()) )
					m_pParent->Invalidate();
			}
		}
		break;
	case CStock::dataK:
		if( pCommPacket && CStock::dataK == pCommPacket->m_dwDataType )
		{
			DWORD i;
			for( i=0; i<pCommPacket->m_dwCount; i++ )
			{
				if( m_CurStock.GetStockInfo().IsEqualTo( pCommPacket->m_pKdata[i].m_dwMarket, pCommPacket->m_pKdata[i].m_szCode ) )
					break;
			}
			if( i != pCommPacket->m_dwCount )
			{
				// 加入最新一天的K线数据
				PrepareStockData( TRUE );
				if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()) )
					m_pParent->Invalidate();
			}
		}
		break;
	case CStock::dataMinute:
		// WILLCHECK
		if( !CKData::IsDayOrMin(m_nCurKType) && pCommPacket && CStock::dataMinute == pCommPacket->m_dwDataType )
		{
			int	nOldSize	=	m_CurStock.GetMinute().GetSize();
			DWORD i;
			for(i=0; i<pCommPacket->m_dwCount; i++)
			{
				if( m_CurStock.GetStockInfo().IsEqualTo( pCommPacket->m_pMinute[i].m_dwMarket, pCommPacket->m_pMinute[i].m_szCode ) )
					break;
			}
			if( i != pCommPacket->m_dwCount )
			{
				// 加入最新的分钟K线数据
				PrepareStockData( TRUE );
				if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()) )
					m_pParent->Invalidate();
			}
		}
		break;
	}

	return 0L;
}

int CStockGraph::GetKType( )
{
	return m_nCurKType;
}

void CStockGraph::SetKType( int ktype )
{
	m_nCurKType	=	ktype;
	PrepareStockData( );
}

int CStockGraph::GetKFormat( )
{
	return m_nCurKFormat;
}

void CStockGraph::SetKFormat( int kformat )
{
	m_nCurKFormat	=	kformat;
	AfxGetProfile().SetGraphKFormat( kformat );
	PrepareStockData( );
}

int CStockGraph::GetMaindataType( )
{
	return m_nCurMaindataType;
}

void CStockGraph::SetMaindataType( int maindatatype )
{
	m_nCurMaindataType	=	maindatatype;
	AfxGetProfile().SetGraphMaindataType( maindatatype );
	PrepareStockData( );
}

int CStockGraph::GetKLineMode( )
{
	return m_nCurKLineMode;
}

void CStockGraph::SetKLineMode( int klineMode )
{
	m_nCurKLineMode	=	klineMode;
}

CRect & CStockGraph::GetKDataRect( )
{
	return m_rectKData;
}

void CStockGraph::ResetIndexCurrent( int nIndexCurrent )
{
	m_nIndexCurrent	=	nIndexCurrent;
	m_anIndexCurrent.RemoveAll();
}

void CStockGraph::ResetIndex( int nIndexStart, int nIndexEnd, int nIndexCurrent )
{
	m_nIndexStart	=	nIndexStart;
	m_nIndexEnd		=	nIndexEnd;
	m_nIndexCurrent	=	nIndexCurrent;
	m_anIndexCurrent.RemoveAll();
}

void CStockGraph::ResetClient( CRect rectAll )
{
	if( rectAll.IsRectEmpty() )
		return;

	m_rectAll	=	rectAll;
	if( m_rectAll.IsRectEmpty() )
		return;

	//	The Left Side m_rectKData
	m_rectKData.top		=	m_rectAll.top + m_nMarginTop+m_nMarginTop;
	m_rectKData.left	=	m_rectAll.left+1;
	m_rectKData.bottom	=	m_rectKData.top + 313;
	m_rectKData.right	=	m_rectKData.left + 82;

	// The Right Side m_rectReport
	m_rectReport.top	=	m_rectAll.top + m_nMarginTop+7;
	m_rectReport.left	=	m_rectAll.right - 174;
	m_rectReport.bottom	=	m_rectAll.bottom - m_nMarginBottom;
	m_rectReport.right	=	m_rectAll.right;

	// The Center m_rectKLine
	CSPDWordArray	& anTech	=	AfxGetProfile().GetGraphTechsShow();
	int	nTechCount	=	anTech.GetSize();
	int	nUnit		=	(int)( (m_rectAll.Height()-m_nMarginTop-m_nMarginBottom) / (nTechCount+1.8) );
	m_rectKLine.top		=	m_rectAll.top + m_nMarginTop;
	m_rectKLine.left	=	m_rectKData.right+1;
	m_rectKLine.bottom	=	m_rectKLine.top + m_rectAll.Height() - m_nMarginTop - m_nMarginBottom - nUnit*nTechCount;
	m_rectKLine.right	=	m_rectAll.right-m_rectReport.Width()-1;

	// The m_rectKLineCenter
	m_rectKLineCenter	=	m_rectKLine;
	m_rectKLineCenter.InflateRect( -2, -m_nHeightSubtitle, -2, -2 );

	// The m_arcTech
	m_arcTech.RemoveAll();
	int	nLastBottom = m_rectKLine.bottom;
	for( int k=0; k<anTech.GetSize(); k++ )
	{
		CRect	rectTech;
		rectTech.top		=	nLastBottom;
		rectTech.left		=	m_rectKLine.left;
		rectTech.bottom		=	rectTech.top + nUnit;
		rectTech.right		=	m_rectKLine.right;
		nLastBottom			=	rectTech.bottom;
		m_arcTech.Add( rectTech );
	}

	// The m_rectCenter
	m_rectCenter	=	m_rectKLine;
	m_rectCenter.bottom	=	m_rectAll.bottom;
}

void CStockGraph::ClearTechniqueCache( )
{
	m_techparam.Clear();
	for( int i=0; i<m_aptrTechUser.GetSize(); i++ )
	{
		CTechUser * pTechUser = (CTechUser *)m_aptrTechUser.GetAt(i);
		if( pTechUser )
			delete	pTechUser;
	}
	m_aptrTechUser.RemoveAll();
}

void CStockGraph::ClearCurStock( )
{
	ClearTechniqueCache( );
	m_CurStock.Clear();
	m_CurStockDown.Clear();
}

BOOL CStockGraph::SetCurStock( const char * szCode )
{
	if( NULL == szCode || strlen(szCode) <= 0 )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	CStockInfo	info;
	if( !AfxGetStockContainer().GetStockInfo( szCode, &info ) )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	return SetCurStock( info );
}

BOOL CStockGraph::SetCurStock( CStockInfo & info )
{
	if( m_CurStock.GetStockInfo().IsEqualTo( info.GetMarket(), info.GetStockCode() ) )
	{
		return TRUE;
	}

	ResetIndex( );
	ClearCurStock( );
	m_CurStock.SetStockInfo( &info );
	m_CurStock.SetDatabase( &AfxGetDB() );
	m_CurStockDown.SetStockInfo( &info );
	m_CurStockDown.SetDatabase( &AfxGetDB() );
	m_dwLatestDate		=	-1;
	
	return TRUE;
}

void CStockGraph::SetReportWhat( int nReportWhat )
{
	m_nReportWhat	=	nReportWhat;
}

BOOL CStockGraph::PrepareStockData( BOOL bReload )
{
	if( !m_CurStock.GetStockInfo().IsValidStock() )
		return FALSE;

	CStockInfo	info;
	if( AfxGetStockContainer().GetStockInfo( m_CurStock.GetStockCode(), &info )
		&& m_CurStock.GetStockInfo().IsEqualTo( info.GetMarket(), info.GetStockCode() ) )
	{
		m_CurStock.SetStockInfo( &info );
		m_CurStockDown.SetStockInfo( &info );
	}

	int nOldSize = m_CurStock.GetKData(m_nCurKType).GetSize();

	// Prepare Data
	AfxPrepareStockData( &AfxGetDB(), m_CurStock, m_nCurKType, m_nCurKFormat, m_nCurMaindataType, FALSE, bReload );
	AfxPrepareStockData( &AfxGetDB(), m_CurStockDown, m_nCurKType, CKData::formatXDRdown, m_nCurMaindataType, FALSE, bReload );

	if( m_CurStock.GetKData(m_nCurKType).GetSize() != nOldSize )
		ResetIndex( );

	AfxGetStkReceiver().RequestKData( &m_CurStock, m_nCurKType );

	m_techparam.Clear( );
	m_techparam.SetKData( &(m_CurStockDown.GetKData(m_nCurKType)) );
	m_techparam.volume.SetKData( &(m_CurStock.GetKData(m_nCurKType)) );
	m_techparam.ma.SetKData( &(m_CurStock.GetKData(m_nCurKType)) );
	m_techparam.bbi.SetKData( &(m_CurStock.GetKData(m_nCurKType)) );
	m_techparam.boll.SetKData( &(m_CurStock.GetKData(m_nCurKType)) );
	m_techparam.pv.SetKData( &(m_CurStock.GetKData(m_nCurKType)) );
	m_techparam.sar.SetKData( &(m_CurStock.GetKData(m_nCurKType)) );
	m_techparam.dj.SetKData( &(m_CurStock.GetKData(m_nCurKType)) );
	m_techparam.mike.SetKData( &(m_CurStock.GetKData(m_nCurKType)) );

	return TRUE;
}

void CStockGraph::Redraw( CDC * pDC, CRect rectAll )
{
	ASSERT( m_pParent && ::IsWindow( m_pParent->GetSafeHwnd() ) );
	if( m_rectAll != rectAll )
		ResetClient( rectAll );
	DrawStock( pDC );
}

void CStockGraph::DrawReportRegion( CDC * pDC )
{
	ASSERT( m_pParent && ::IsWindow( m_pParent->GetSafeHwnd() ) );
	if( !m_pParent || !::IsWindow(m_pParent->GetSafeHwnd()) )
		return;

	CClientDC	dc(m_pParent);
	if( NULL == pDC )
		pDC		=	&dc;

	switch( m_nReportWhat )
	{
	case CStockGraph::reportParam:		DrawReportParam( pDC );		break;
	case CStockGraph::reportCost:		DrawReportCost( pDC );		break;
	case CStockGraph::reportFlame:		DrawReportFlame( pDC );		break;
	case CStockGraph::reportActivity:	DrawReportActivity( pDC );	break;
	}
}

int CStockGraph::GetNextThickness( int nCurrentThickness, BOOL bAsc )
{
	if( bAsc )
	{
		switch( nCurrentThickness )
		{
		case 1:			return 2;
		case 2:			return 4;
		case 4:			return 5;
		case 5:			return 6;
		case 6:			return 7;
		case 7:			return 9;
		case 9:			return 10;
		case 10:		return 13;
		case 13:		return 15;
		case 15:		return 15;
		default:
			ASSERT( FALSE );
			return m_nDefaultThickness;
		}
	}
	else
	{
		switch( nCurrentThickness )
		{
		case 1:			return 1;
		case 2:			return 1;
		case 4:			return 2;
		case 5:			return 4;
		case 6:			return 5;
		case 7:			return 6;
		case 9:			return 7;
		case 10:		return 9;
		case 13:		return 10;
		case 15:		return 13;
		default:
			ASSERT( FALSE );
			return m_nDefaultThickness;
		}
	}
}

void CStockGraph::ExpandUp( BOOL bShiftPressed )
{
	if( m_nIndexCurrent >= 0 )
	{
		CKData	&kdata	=	m_CurStock.GetKData( m_nCurKType );
		if( m_nIndexCurrent < kdata.GetSize() )
		{	
			if( -1 != m_dwLatestDate && kdata.ElementAt(m_nIndexCurrent).m_date > m_dwLatestDate )
			{
				KDATA	&	kd	=	kdata.ElementAt(m_nIndexCurrent);
				if( bShiftPressed )
				{
					float	fIncrease	=	100000;
					fIncrease	=	( kd.m_fVolume/10 > fIncrease ? kd.m_fVolume/10 : fIncrease );
					kd.m_fVolume	+=	fIncrease;
					kd.m_fAmount	=	( kd.m_fVolume*(kd.m_fOpen+kd.m_fClose)/2 );
				}
				else
				{
					float	fIncrease	=	(float)0.001;
					fIncrease	=	(float)( kd.m_fClose/500 > fIncrease ? kd.m_fClose/500 : fIncrease );
					kd.m_fClose		+=	fIncrease;
					kd.m_fOpen		+=	fIncrease;
					kd.m_fHigh		+=	fIncrease;
					kd.m_fLow		+=	fIncrease;
				}
				DrawStock( NULL );
				return;
			}
		}
	}

	int nOld = m_nThickness;
	m_nThickness = GetNextThickness( m_nThickness, TRUE );
	if( nOld != m_nThickness )
		DrawStock( NULL );
}

void CStockGraph::ExpandDown( BOOL bShiftPressed )
{
	if( m_nIndexCurrent >= 0 )
	{
		CKData	&kdata	=	m_CurStock.GetKData( m_nCurKType );
		if( m_nIndexCurrent < kdata.GetSize() )
		{	
			if( -1 != m_dwLatestDate && kdata.ElementAt(m_nIndexCurrent).m_date > m_dwLatestDate )
			{
				KDATA	&	kd	=	kdata.ElementAt(m_nIndexCurrent);
				if( bShiftPressed )
				{
					float	fDecline	=	100000;
					fDecline		=	( kd.m_fVolume/10 > fDecline ? kd.m_fVolume/10 : fDecline );
					if( kd.m_fVolume > fDecline )
						kd.m_fVolume	-=	fDecline;
					kd.m_fAmount	=	(float)( kd.m_fVolume*(kd.m_fOpen+kd.m_fClose)/2 );
				}
				else
				{
					float	fDecline	=	(float)0.001;
					fDecline	=	(float)( kd.m_fClose/500 > fDecline ? kd.m_fClose/500 : fDecline );
					if( kd.m_fLow > fDecline )
					{
						kd.m_fClose		-=	fDecline;
						kd.m_fOpen		-=	fDecline;
						kd.m_fHigh		-=	fDecline;
						kd.m_fLow		-=	fDecline;
					}
				}
				DrawStock( NULL );
				return;
			}
		}
	}

	int nOld = m_nThickness;
	m_nThickness = GetNextThickness( m_nThickness, FALSE );
	if( nOld != m_nThickness )
		DrawStock( NULL );
}

void CStockGraph::MoveLeft( BOOL bShiftPressed )
{
	Move( -1, bShiftPressed, FALSE );
}

void CStockGraph::MoveRight( BOOL bShiftPressed )
{
	if( m_nIndexCurrent >= 0 && bShiftPressed )
	{
		CKData	&kdata	=	m_CurStock.GetKData( m_nCurKType );
		if( m_nIndexCurrent+1 >= kdata.GetSize() )
		{	
			//	Add New KDATA to test predicate
			if( 0 == kdata.GetSize() )
				m_dwLatestDate	=	0;
			else if( -1 == m_dwLatestDate )
				m_dwLatestDate	=	kdata.ElementAt(kdata.GetSize()-1).m_date;
			KDATA	kd;
			memset( &kd, 0, sizeof(kd) );
			if( kdata.GetSize() > 0 )
				kd	=	kdata.ElementAt(kdata.GetSize()-1);
			else
				kd.m_fClose	=	1;
			kd.m_fOpen	=	kd.m_fHigh	=	kd.m_fLow	=	kd.m_fClose;
			kd.m_date	=	CSPTime::GetStockTimeNext( kd.m_date, m_nCurKType );
			kdata.Add( kd );
			m_CurStockDown.GetKData( m_nCurKType ).Add( kd );
			
			bShiftPressed	=	FALSE;
		}
	}

	Move( 1, bShiftPressed, FALSE );
}

void CStockGraph::PageLeft( BOOL bShiftPressed )
{
	int	nMove	=	-(m_nIndexEnd-m_nIndexStart-5);
	if( nMove >= 0 )
		nMove	=	-1;
	Move( nMove, bShiftPressed, FALSE );
}

void CStockGraph::PageRight( BOOL bShiftPressed )
{
	int	nMove	=	(m_nIndexEnd-m_nIndexStart-5);
	if( nMove <= 0 )
		nMove	=	1;
	Move( nMove, bShiftPressed, FALSE );
}

void CStockGraph::MoveHome( BOOL bShiftPressed )
{
	if( m_nIndexCurrent >= 0 )
	{
		// Move Current Selection
		int	nPos	=	m_nIndexStart;
		MoveTo( nPos, bShiftPressed, FALSE );
	}
	else
	{
		// Show Prev Technique Line
		UINT	nTechUserCount	=	CTechUser::GetTechUserCount();
		UINT	nTechMax	=	STT_MAX;
		if( nTechUserCount > 0 )
			nTechMax	=	STT_USER_MIN + nTechUserCount - 1;

		CSPDWordArray	& auShow	=	AfxGetProfile().GetGraphTechsShow();
		int	nOldSize = auShow.GetSize();
		if( auShow.GetSize() > 0 )
		{
			UINT	nNext	=	auShow.ElementAt(auShow.GetSize()-1);
			nNext	--;
			if( nNext < STT_TREND_MIN || nNext > nTechMax )
				nNext	=	nTechMax;
			if( nNext > STT_MAX && nNext < STT_USER_MIN )
				nNext	=	STT_MAX;
			AfxGetProfile().ShowGraphViewTech( nNext );
		}
		else
		{
			auShow.Add( nTechMax );
		}
		if( auShow.GetSize() == nOldSize )
			InvalidateCenter( );
		else if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()) )
		{
			ResetClient( m_rectAll );
			m_pParent->Invalidate();
		}
	}
}

void CStockGraph::MoveEnd( BOOL bShiftPressed )
{
	if( m_nIndexCurrent >= 0 )
	{
		// Move Current Selection
		CKData	& kdata	=	m_CurStock.GetKData( m_nCurKType );
		int	nPos	=	( m_nIndexEnd < 0 ? kdata.GetSize()-1 : m_nIndexEnd );
		MoveTo( nPos, bShiftPressed, FALSE );
	}
	else
	{
		// Show Next Technique Line
		UINT	nTechUserCount	=	CTechUser::GetTechUserCount();
		UINT	nTechMax	=	STT_MAX;
		if( nTechUserCount > 0 )
			nTechMax	=	STT_USER_MIN + nTechUserCount - 1;

		CSPDWordArray	& auShow	=	AfxGetProfile().GetGraphTechsShow();
		int nOldSize = auShow.GetSize();
		if( auShow.GetSize() > 0 )
		{
			UINT	nNext	=	auShow.ElementAt(auShow.GetSize()-1);
			nNext	++;
			if( nNext < STT_TREND_MIN || nNext > nTechMax )
				nNext	=	STT_TREND_MIN;
			if( nNext > STT_MAX && nNext < STT_USER_MIN )
				nNext	=	STT_USER_MIN;
			AfxGetProfile().ShowGraphViewTech( nNext );
		}
		else
		{
			auShow.Add( STT_TREND_MIN );
		}
		if( auShow.GetSize() == nOldSize )
			InvalidateCenter( );
		else if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()) )
		{
			ResetClient( m_rectAll );
			m_pParent->Invalidate();
		}
	}
}

void CStockGraph::OnInsert( )
{
	CKData	&kdata	=	m_CurStock.GetKData( m_nCurKType );

	KDATA	kd;
	memset( &kd, 0, sizeof(kd) );
	if( kdata.GetSize() > 0 )
		kd	=	kdata.ElementAt(kdata.GetSize()-1);
	else
		kd.m_fClose	=	1;
	kd.m_fOpen	=	kd.m_fHigh	=	kd.m_fLow	=	kd.m_fClose;
	kd.m_date	=	CSPTime::GetStockTimeNext( kd.m_date, m_nCurKType );

	CEditKDATADlg	dlg( m_pParent );
	dlg.SetKDATA( kd, m_nCurKType );
	if( IDOK == dlg.DoModal( ) )
	{
		kd	=	dlg.GetKDATA( );
		kdata.Add( kd );
		m_CurStockDown.GetKData( m_nCurKType ).Add( kd );
		m_nIndexCurrent	=	kdata.GetSize()-1;
		if( -1 == m_dwLatestDate )
		{
			if( kdata.GetSize() <= 1 )
				m_dwLatestDate	=	0;
			else
				m_dwLatestDate	=	kdata.ElementAt(kdata.GetSize()-2).m_date;
		}
		MoveTo( m_nIndexCurrent, FALSE, FALSE );
	}

	if( m_pParent && ::IsWindow( m_pParent->GetSafeHwnd() ) )
		m_pParent->SetFocus( );
}

void CStockGraph::OnLButtonDown( UINT nFlags, CPoint point, BOOL bShiftPressed, BOOL bCtrlPressed )
{
	// Border Move
	if( m_bCursorBorder )
	{
		SetCapture(m_pParent->GetSafeHwnd());
		// Draw Border Moving Line
		DrawBorderMovingLine( NULL, point, TRUE );
		m_ptLastMovingLine	=	point;
		return;
	}

	// KLine Index
	int	nIndexClick	=	GetIndexClick( point );
	MoveTo( nIndexClick, bShiftPressed, bCtrlPressed );

	// Selection Rect
	if( -1 != nIndexClick && m_rectKLineCenter.PtInRect(point) )
	{
		SetCapture(m_pParent->GetSafeHwnd());
		m_ptSelectBegin	=	point;
		m_ptSelectEnd	=	point;
		DrawSelectionRect( NULL, m_ptSelectBegin, m_ptSelectEnd, TRUE );
	}

	// Selected TechID
	SelectTechID( point );
}

void CStockGraph::OnLButtonUp( UINT nFlags, CPoint point, BOOL bShiftPressed, BOOL bCtrlPressed )
{
	if( m_bCursorBorder && GetCapture() == m_pParent->GetSafeHwnd() )
	{
		ReleaseCapture( );
		// Remove Border Moving Line
		DrawBorderMovingLine( NULL, point, FALSE );
		// Reset Border
		if( 0 == m_nPosSelectBorder )
		{
			m_rectKLine.bottom	=	point.y;
			m_rectKLineCenter	=	m_rectKLine;
			m_rectKLineCenter.InflateRect( -2, -m_nHeightSubtitle, -2, -2 );
			if( m_arcTech.GetSize() > 0 )
				m_arcTech.ElementAt(0).top	=	point.y;
		}
		else if( m_nPosSelectBorder > 0 )
		{
			if( m_nPosSelectBorder < m_arcTech.GetSize() )
			{
				m_arcTech.ElementAt(m_nPosSelectBorder-1).bottom	=	point.y;
				m_arcTech.ElementAt(m_nPosSelectBorder).top			=	point.y;
			}
		}

		m_bCursorBorder = FALSE;
		DrawStock( NULL );
		return;
	}

	if( GetCapture() == m_pParent->GetSafeHwnd() )
	{
		ReleaseCapture( );
		DrawSelectionRect( NULL, m_ptSelectBegin, m_ptSelectEnd, FALSE );

		int nClickBegin = GetIndexClick( m_ptSelectBegin );
		int nClickEnd = GetIndexClick( m_ptSelectEnd );
		if( -1 == nClickBegin )
		{
			if( m_ptSelectBegin.x < m_rectCenter.left )		nClickBegin	=	m_nIndexStart;
			if( m_ptSelectBegin.x > m_rectCenter.right )	nClickBegin	=	m_nIndexEnd;
		}
		if( -1 == nClickEnd )
		{
			if( m_ptSelectEnd.x < m_rectCenter.left )		nClickEnd	=	m_nIndexStart;
			if( m_ptSelectEnd.x > m_rectCenter.right )		nClickEnd	=	m_nIndexEnd;
		}

		if( -1 != nClickBegin && -1 != nClickEnd )
		{
			if( nClickBegin < nClickEnd ) // 放大
			{
				int nLeft = min(nClickBegin,nClickEnd);
				int nRight = max(nClickBegin,nClickEnd);

				if( 15*(nRight-nLeft+1) < m_rectCenter.Width() )
				{
					nLeft -= (m_rectCenter.Width() - 15*(nRight-nLeft+1))/30 + 1;
					nRight += (m_rectCenter.Width() - 15*(nRight-nLeft+1))/30 + 1;
				}

				int	nOldLen = m_nIndexEnd - m_nIndexStart;
				int	nOldThick = m_nThickness;
				ResetIndex( nLeft, nRight, m_nIndexCurrent );
				int nNewLen = m_nIndexEnd - m_nIndexStart;

				while( nOldLen * nOldThick > nNewLen * m_nThickness )
				{
					int	nTemp = m_nThickness;
					m_nThickness = GetNextThickness( m_nThickness, TRUE );
					if( nTemp == m_nThickness )
						break;
				}
			}
			else if( nClickBegin > nClickEnd ) // 缩小
			{
				m_nThickness = GetNextThickness( m_nThickness, FALSE );
				m_nThickness = GetNextThickness( m_nThickness, FALSE );
				m_nThickness = GetNextThickness( m_nThickness, FALSE );
			}

			DrawStock( NULL );
		}
	}
}

void CStockGraph::OnMouseMove( UINT nFlags, CPoint point )
{
	// Size Border
	if( m_bCursorBorder && GetCapture() == m_pParent->GetSafeHwnd() )
	{
		DrawBorderMovingLine( NULL, m_ptLastMovingLine, TRUE );
		DrawBorderMovingLine( NULL, point, TRUE );
		m_ptLastMovingLine	=	point;
		return;
	}

	int nPosSelectBorder = -1;
	if( m_rectCenter.PtInRect(point) )
	{
		CRect	rect = m_rectKLine;
		for( int i=0; i<m_arcTech.GetSize(); i++ )
		{
			rect.top = rect.bottom-5;
			rect.bottom = rect.bottom+5;
			if( rect.PtInRect(point) )
				nPosSelectBorder = i;
			rect = m_arcTech[i];
		}
	}
	if( -1 != nPosSelectBorder )
	{
		::SetCursor( AfxGetApp()->LoadCursor(AFX_IDC_VSPLITBAR) );
		m_bCursorBorder = TRUE;
		m_nPosSelectBorder = nPosSelectBorder;
	}
	if( -1 == nPosSelectBorder && m_bCursorBorder && GetCapture() != m_pParent->GetSafeHwnd() )
	{
		::SetCursor( AfxGetApp()->LoadCursor(IDC_ARROW) );
		m_bCursorBorder = FALSE;
	}
	if( m_bCursorBorder )
		return;

	// Selection Rect
	if( GetCapture() == m_pParent->GetSafeHwnd() )
	{
		DrawSelectionRect( NULL, m_ptSelectBegin, m_ptSelectEnd, FALSE );

		m_ptSelectEnd	=	point;
		if( m_ptSelectEnd.x <= m_rectKLineCenter.left )
			m_ptSelectEnd.x = m_rectKLineCenter.left+1;
		if( m_ptSelectEnd.x >= m_rectKLineCenter.right )
			m_ptSelectEnd.x = m_rectKLineCenter.right-1;
		if( m_ptSelectEnd.y <= m_rectKLineCenter.top )
			m_ptSelectEnd.y = m_rectKLineCenter.top+1;
		if( m_ptSelectEnd.y >= m_rectKLineCenter.bottom )
			m_ptSelectEnd.y = m_rectKLineCenter.bottom-1;

		DrawSelectionRect( NULL, m_ptSelectBegin, m_ptSelectEnd, TRUE );
	}
}

void CStockGraph::OnLButtonDblClk( UINT nFlags, CPoint point, BOOL bShiftPressed, BOOL bCtrlPressed )
{
	OnLButtonDown( nFlags, point, bShiftPressed, bCtrlPressed );

	AdjustSelectedTech();

	OnReturn( bShiftPressed, bCtrlPressed );
}

void CStockGraph::OnRButtonDown( UINT nFlags, CPoint point, BOOL bShiftPressed, BOOL bCtrlPressed )
{
	SelectTechID( point );
}

void CStockGraph::OnReturn( BOOL bShiftPressed, BOOL bCtrlPressed )
{
	if( m_nIndexCurrent >= 0 )
	{
		CKData	&kdata	=	m_CurStock.GetKData( m_nCurKType );
		if( m_nIndexCurrent < kdata.GetSize() )
		{	
			if( -1 != m_dwLatestDate && kdata.ElementAt(m_nIndexCurrent).m_date > m_dwLatestDate )
			{
				KDATA	&	kd	=	kdata.ElementAt(m_nIndexCurrent);

				CEditKDATADlg	dlg( m_pParent );
				dlg.SetKDATA( kd, m_nCurKType );
				if( IDOK == dlg.DoModal( ) )
				{
					kd	=	dlg.GetKDATA( );
					if( m_nIndexCurrent < m_CurStockDown.GetKData( m_nCurKType ).GetSize() )
						m_CurStockDown.GetKData( m_nCurKType ).SetAt(m_nIndexCurrent,kd);
					DrawStock( NULL );
				}
				if( m_pParent && ::IsWindow( m_pParent->GetSafeHwnd() ) )
					m_pParent->SetFocus( );
			}
		}
	}
}

UINT CStockGraph::GetSelectedTech( )
{
	return m_nSelectedTechID;
}

void CStockGraph::HideSelectedTech()
{
	AfxGetProfile().ToggleGraphViewTech( m_nSelectedTechID );

	if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()) )
	{
		ResetClient( m_rectAll );
		m_pParent->Invalidate();
	}
}

void CStockGraph::AdjustSelectedTech()
{
	if( STT_INVALID != m_nSelectedTechID )
	{
		CSetParamDlg	dlg;
		dlg.SetFirstSelected( m_nSelectedTechID );
		dlg.DoModal();

		CGraphView * pGraphView = AfxGetGraphView();
		if( pGraphView )
			pGraphView->OnTechParametersChange( );
		AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_GRAPHVIEW, NULL );
	}
}

void CStockGraph::MoveTo( int nIndex, BOOL bShiftPressed, BOOL bCtrlPressed )
{
	DrawSelectionLine( NULL, FALSE );

	// To Change m_anIndexCurrent
	if( bShiftPressed && nIndex >= 0 )
	{
		if( m_nIndexCurrent < 0 )
		{
			AddIndexCurrent( nIndex );
		}
		else if( m_nIndexCurrent <= nIndex )
		{
			for( int nSel=m_nIndexCurrent; nSel<=nIndex; nSel++ )
			{
				AddIndexCurrent( nSel );
			}
		}
		else
		{
			for( int nSel=nIndex; nSel<=m_nIndexCurrent; nSel++ )
			{
				AddIndexCurrent( nSel );
			}
		}
	}
	else if( bCtrlPressed && nIndex >= 0 )
	{
		AddIndexCurrent( nIndex );
	}
	else
	{
		m_anIndexCurrent.RemoveAll();
	}

	BOOL	bNeedDrawAll	=	FALSE;
	if( -1 != nIndex && nIndex < m_nIndexStart )
	{
		m_nIndexEnd		-=	(m_nIndexStart-nIndex);
		m_nIndexStart	=	nIndex;
		m_nIndexCurrent	=	nIndex;
		bNeedDrawAll	=	TRUE;
	}
	if( -1 != nIndex && nIndex > m_nIndexEnd )
	{
		m_nIndexStart	+=	(nIndex-m_nIndexEnd);
		m_nIndexEnd		=	nIndex;
		m_nIndexCurrent	=	nIndex;
		bNeedDrawAll	=	TRUE;
	}

	if( m_nIndexCurrent != nIndex )
	{
		int	nOld	=	m_nIndexCurrent;
		m_nIndexCurrent	=	nIndex;
		OnIndexCurrentChanged( );
	}

	if( bNeedDrawAll )
		DrawStock( NULL );
	else
		DrawSelectionLine( NULL, TRUE );
}

void CStockGraph::OnIndexCurrentChanged( )
{
	if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()) )
	{
		CClientDC	dc(m_pParent);

		CRect	rectKLineAttrib = m_rectKLineCenter;
		rectKLineAttrib.top -= m_nHeightSubtitle;
		CSPDWordArray	& anTech	=	AfxGetProfile().GetGraphTechsKLine();
		if( anTech.GetSize() > 0 )
			DrawKLineAttribute( &dc, anTech[0], rectKLineAttrib, TRUE, 0, 0 );
		
		CSPDWordArray	& anShow	=	AfxGetProfile().GetGraphTechsShow( );
		for( int k=0; k<m_arcTech.GetSize(); k++ )
		{
			CRect	rect	=	m_arcTech.ElementAt(k);
			if( k < anShow.GetSize() && anShow[k] == STT_MIKE )
			{
				DrawTechLine( &dc, anShow[k], rect, FALSE );
				DrawTechLine( &dc, anShow[k], rect, TRUE );
			}
			else if( k < anShow.GetSize() )
			{
				DrawTechLine( &dc, anShow[k], rect, TRUE );
			}
		}

		DrawReportRegion( &dc );
		DrawKData( );
	}
}

void CStockGraph::Move( int nMove, BOOL bShiftPressed, BOOL bCtrlPressed )
{
	int	nIndexCurrent	=	m_nIndexCurrent;
	
	if( nIndexCurrent < 0 )
	{
		if( nMove < 0 )
			nIndexCurrent	=	m_nIndexEnd;
		else if( nMove > 0 )
			nIndexCurrent	=	m_nIndexStart;
	}
	else
	{
		nIndexCurrent	+=	nMove;
	}

	CKData	&kdata	=	m_CurStock.GetKData( m_nCurKType );
	if( nIndexCurrent < 0 )
		nIndexCurrent	=	0;
	if( nIndexCurrent >= kdata.GetSize() )
		nIndexCurrent	=	kdata.GetSize()-1;

	MoveTo( nIndexCurrent, bShiftPressed, bCtrlPressed );
}

int CStockGraph::GetIndexClick( CPoint point )
{
	int	nIndexClick	=	-1;
	if( m_rectCenter.PtInRect( point ) )
	{
		for( int nIndex=m_nIndexStart; nIndex<=m_nIndexEnd; nIndex ++ )
		{
			CRect	rectK;
			if( GetOneKLineRect( nIndex, &rectK, NULL, NULL, NULL ) && rectK.PtInRect(point) )
			{
				nIndexClick	=	nIndex;
				break;
			}
		}
	}
	return nIndexClick;
}

void CStockGraph::SelectTechID( CPoint point )
{
	m_nSelectedTechID	=	STT_INVALID;
	int	nTechClick	=	-1;
	for( int k=0; k<m_arcTech.GetSize(); k++ )
	{
		if( m_arcTech.ElementAt(k).PtInRect( point ) )
		{
			nTechClick	=	k;
			break;
		}
	}

	CSPDWordArray	& anTech	=	AfxGetProfile().GetGraphTechsShow();
	if( nTechClick >= 0 && nTechClick < m_arcTech.GetSize() && nTechClick < anTech.GetSize() )
	{
		m_nSelectedTechID	=	anTech.ElementAt(nTechClick);
	}
}

BOOL CStockGraph::AddIndexCurrent( int nIndex )
{
	for( int k=0; k<m_anIndexCurrent.GetSize(); k++ )
	{
		if( (UINT)nIndex == m_anIndexCurrent[k] )
			return FALSE;
	}
	m_anIndexCurrent.Add( nIndex );
	return TRUE;
}

void CStockGraph::InvalidateCenter( )
{
	if( !m_pParent || !::IsWindow(m_pParent->GetSafeHwnd()) )
		return;
	CClientDC	dc(m_pParent);
	DrawDateAxis( &dc );
	DrawTechLine( &dc );
	DrawKLine( &dc );
}

BOOL CStockGraph::GetOneKLineRect( int nIndex, LPRECT lpRect, long *pxEntityLeft, long *pxEntityRight, long *pxMedium )
{
	if( nIndex == -1 || nIndex < m_nIndexStart || nIndex > m_nIndexEnd )
		return FALSE;

	CRect	rectK	=	m_rectCenter;
	rectK.left	=	m_rectKLineCenter.left + (nIndex-m_nIndexStart) * m_nThickness;
	rectK.right	=	rectK.left + m_nThickness;

	if( rectK.Width() <= 0 || rectK.Height() <= 0 )
		return FALSE;
	if( lpRect )
		*lpRect	=	rectK;

	int	xStart	=	rectK.left;

	CRect	rcEntity;
	switch( m_nThickness )
	{
	case 1:
	case 2:
		rcEntity.left	=	xStart;
		rcEntity.right	=	xStart + 1;
		break;
	case 4:
	case 5:
		rcEntity.left	=	xStart;
		rcEntity.right	=	xStart + 3;
		break;
	case 6:
	case 7:
		rcEntity.left	=	xStart;
		rcEntity.right	=	xStart + 5;
		break;
	case 9:
	case 10:
		rcEntity.left	=	xStart;
		rcEntity.right	=	xStart + 7;
		break;
	case 13:
	case 15:
		rcEntity.left	=	xStart;
		rcEntity.right	=	xStart + 11;
		break;
	default:
		ASSERT( FALSE );
		rcEntity.left	=	xStart;
		rcEntity.right	=	xStart + 3;
		return FALSE;
	}
	if( pxEntityLeft )
		*pxEntityLeft	=	rcEntity.left;
	if( pxEntityRight )
		*pxEntityRight	=	rcEntity.right;
	if( pxMedium )
		*pxMedium	=	rcEntity.left + rcEntity.Width() / 2;

	return TRUE;
}

void CStockGraph::DrawStock( CDC * pDC )
{
	ASSERT( m_pParent && ::IsWindow( m_pParent->GetSafeHwnd() ) );
	if( !m_pParent || !::IsWindow(m_pParent->GetSafeHwnd()) )
		return;

	CClientDC	dc(m_pParent);
	if( NULL == pDC )
		pDC		=	&dc;

	if( !m_CurStock.GetStockInfo().IsValidStock() )
		return;

	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	if( kdata.GetSize() == 0 )
	{
		DrawNoDataInfo( pDC );
	}
	else if( PrepareDrawKLine( ) ) // Reset m_nIndexStart, m_nIndexEnd, m_nIndexCurrent
	{
		DrawDateAxis( pDC );
		DrawTechLine( pDC );
		DrawKLine( pDC );
		DrawKData( );
	}

	DrawReportRegion( pDC );
}

BOOL CStockGraph::PrepareDrawKLine(	)
{
	ASSERT( m_nThickness > 0 );
	if( m_nThickness <= 0 || m_rectKLineCenter.Width() <= 0 )
		return FALSE;

	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);

	int	nWidth	=	m_rectKLineCenter.Width();
	int	nCount	=	nWidth / m_nThickness;
	if( nCount <= 0 )
		return FALSE;

	if( kdata.GetSize() == 0 )
	{
		m_nIndexCurrent	=	m_nIndexStart	=	m_nIndexEnd	=	-1;
		return FALSE;
	}

	// Check Old Values
	ASSERT( m_nIndexStart <= m_nIndexEnd );
	ASSERT( m_nIndexCurrent < 0 || (m_nIndexCurrent >= m_nIndexStart && m_nIndexCurrent <= m_nIndexEnd) );
	if( m_nIndexCurrent >= 0 && (m_nIndexCurrent < m_nIndexStart || m_nIndexCurrent > m_nIndexEnd) )
		m_nIndexCurrent	=	-1;

	if( -1 == m_nIndexStart || -1 == m_nIndexEnd )
	{
		m_nIndexEnd	=	kdata.GetSize()-1;
	}
	else if( m_nIndexEnd - m_nIndexStart + 1 < nCount )
	{
		// Reset m_nIndexStart and m_nIndexEnd
		int	nExpand	=	nCount-(m_nIndexEnd-m_nIndexStart+1);
		if( m_nIndexCurrent >= 0 && m_nIndexEnd-m_nIndexStart > 0 )
			m_nIndexEnd		+=	(m_nIndexEnd-m_nIndexCurrent)*nExpand / (m_nIndexEnd-m_nIndexStart);
	}
	else if( m_nIndexEnd - m_nIndexStart + 1 > nCount )
	{
		int	nReduce	=	(m_nIndexEnd-m_nIndexStart+1)-nCount;
		if( m_nIndexCurrent >= 0 && m_nIndexEnd-m_nIndexStart > 0 )
			m_nIndexEnd		-=	(m_nIndexEnd-m_nIndexCurrent)*nReduce / (m_nIndexEnd-m_nIndexStart);
	}
	if( m_nIndexEnd >= kdata.GetSize() )
		m_nIndexEnd	=	kdata.GetSize()-1;
	m_nIndexStart	=	m_nIndexEnd - nCount + 1;
	if( m_nIndexStart < 0 )
	{
		m_nIndexEnd		+=	(-m_nIndexStart);
		m_nIndexStart	=	0;
	}
	if( m_nIndexEnd >= kdata.GetSize() )
		m_nIndexEnd	=	kdata.GetSize()-1;

	ASSERT( m_nIndexStart <= m_nIndexEnd );
	ASSERT( m_nIndexCurrent < 0 || (m_nIndexCurrent >= m_nIndexStart && m_nIndexCurrent <= m_nIndexEnd) );
	if( m_nIndexStart > m_nIndexEnd )
		return FALSE;
	if( m_nIndexCurrent >= 0 && (m_nIndexCurrent < m_nIndexStart || m_nIndexCurrent > m_nIndexEnd) )
		m_nIndexCurrent	=	-1;

	return TRUE;
}

void CStockGraph::DrawNoDataInfo( CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	CKData	&	kdata	=	m_CurStock.GetKData(m_nCurKType);
	if( 0 == kdata.GetSize() )
	{
		CString	strNoKData, strTemp;
		strNoKData.LoadString( IDS_STOCKGRAPH_NOKDATA );
		strTemp		=	CString(m_CurStock.GetStockInfo().GetStockName()) + "(" + m_CurStock.GetStockInfo().GetStockCode() + ")";
		strTemp		+=	(LPCTSTR)AfxGetKTypeString( m_nCurKType );
		strNoKData	=	strTemp + strNoKData;
		CFont	* pOldFont	=	AfxSelectDCFont( pDC, 14 );
		pDC->SetBkColor( clrBK );
		pDC->SetTextColor( clrTitle );
		pDC->TextOut( m_rectAll.left+20, m_rectAll.top+20,  strNoKData );
		pDC->SelectObject( pOldFont );
	}
}

void CStockGraph::DrawDateAxis( CDC * pDC )
{
	CHECK_NODATATODRAW
	DECLARE_COLOR_DEFINATION

	CRect	rect	=	m_rectCenter;
	rect.top	=	m_rectAll.bottom - m_nMarginBottom + 1;
	pDC->FillSolidRect( &rect, clrBK );
	pDC->SetBkColor( clrBK );
	pDC->SetTextColor( clrDJ );

	CKData & kdata	=	m_CurStock.GetKData( m_nCurKType );
	BOOL	bDayOrMin	=	CKData::IsDayOrMin(m_nCurKType);
	int		y	=	m_rectAll.bottom - m_nMarginBottom + 2;
	CFont	* pOldFont	=	AfxSelectDCFont( pDC, 12 );
	int	xLast	=	0;
	for( int k=m_nIndexStart; k<=m_nIndexEnd; k++ )
	{
		if( k > 0 && k < kdata.GetSize() )
		{
			CSPTime	sptimeLast, sptime;
			if( sptimeLast.FromStockTime( kdata.ElementAt(k-1).m_date, bDayOrMin )
				&& sptime.FromStockTime( kdata.ElementAt(k).m_date, bDayOrMin ) )
			{
				CString	strText;
				if( (CKData::ktypeDay == m_nCurKType )
					&& (sptimeLast.GetMonth() != sptime.GetMonth()) )
						strText	=	sptime.Format( "%m" );
				else if( (CKData::ktypeWeek == m_nCurKType || CKData::ktypeMonth == m_nCurKType)
					&& (sptimeLast.GetYear() != sptime.GetYear()) )
						strText	=	sptime.Format( "%y" );
				else if( !CKData::IsDayOrMin(m_nCurKType)
					&& (sptimeLast.GetDay() != sptime.GetDay()) )
						strText	=	sptime.Format( "%d" );
				CRect	rectK;
				if( strText.GetLength() > 0 && GetOneKLineRect( k, &rectK, NULL, NULL, NULL ) && xLast < rectK.left )
				{
					pDC->TextOut( rectK.left, y, strText );
					xLast	=	rectK.left + pDC->GetTextExtent(strText).cx;
				}
			}
		}
	}
	pDC->SelectObject( pOldFont );
}

void CStockGraph::DrawKLine(CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);

	pDC->FillSolidRect( &m_rectKLine, clrBK );
	pDC->Draw3dRect( &m_rectKLine, clrBorder, clrBorder );

	// Draw Title
	CString	strTitle;
	pDC->SetTextColor( clrTitle );
	pDC->SetBkColor( clrBK );
	strTitle	=	AfxGetKTypeString( m_nCurKType );
	pDC->TextOut( m_rectAll.left+8, m_rectAll.top+2, strTitle );
	strTitle.LoadString( IDS_STOCKGRAPH_KLINE );
	pDC->TextOut( m_rectKLine.left+1, m_rectAll.top+2, strTitle );

	double	dMin = 0, dMax = 0;
	if( ! GetKLineMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax ) )
		return;
	m_dMaxPrice			=	dMax;
	m_dMinPrice			=	dMin;

	// Draw Axis
	DrawAxis( pDC, m_rectKLine, m_nHeightSubtitle, 3, dMin, dMax, TRUE, TRUE, 2 );

	// KLine Attribute Rect
	CRect	rectKLineAttrib = m_rectKLineCenter;
	rectKLineAttrib.top -= m_nHeightSubtitle;

	// Draw STT_DJ and STT_CW
	CSPDWordArray	&	anTech	=	AfxGetProfile().GetGraphTechsKLine( );
	for( int k=0; k<anTech.GetSize(); k++ )
		if( STT_DJ == anTech[k] || STT_CW == anTech[k] )
			DrawKLineAttribute( pDC, anTech[k], rectKLineAttrib, FALSE, dMin, dMax );

	// Draw K Line
	for( int k=m_nIndexStart; k<=m_nIndexEnd; k++ )
	{
		DrawOneKLine( pDC, k, k, &kdata, dMin, dMax, FALSE );
	}

	// Draw Tech KLine except STT_DJ and STT_CW
	for( int k=0; k<anTech.GetSize(); k++ )
		if( STT_DJ != anTech[k] && STT_CW != anTech[k] )
			DrawKLineAttribute( pDC, anTech[k], rectKLineAttrib, FALSE, dMin, dMax );
	if( anTech.GetSize() > 0 )
		DrawKLineAttribute( pDC, anTech[0], rectKLineAttrib, TRUE, dMin, dMax );

	// Draw Select Line
	DrawSelectionLine( pDC, TRUE );
}

BOOL CStockGraph::GetKLineMinMaxInfo( int nStart, int nEnd, double * pdMin, double * pdMax )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	CKLine	kline( &kdata );
	double	dMin = 0, dMax = 0, dMinTemp = 0, dMaxTemp = 0;
	if( nStart < 0 || nEnd < 0 || ! kline.GetMinMaxInfo( nStart, nEnd, &dMin, &dMax ) )
		return FALSE;

	CSPDWordArray	&	anTech	=	AfxGetProfile().GetGraphTechsKLine( );
	for( int k=0; k<anTech.GetSize(); k++ )
	{
		UINT	nTech	=	anTech[k];
		switch( nTech )
		{
		case STT_MA:
			{
				CMA	ma( &kdata ); 
				if( ma.GetMinMaxInfo( nStart, nEnd, &dMinTemp, &dMaxTemp ) )
				{
					if( dMinTemp < dMin )	dMin	=	dMinTemp;
					if( dMaxTemp > dMax )	dMax	=	dMaxTemp;
				}
			}
			break;
		case STT_BBI:
			{
				CBBI	bbi( &kdata ); 
				if( bbi.GetMinMaxInfo( nStart, nEnd, &dMinTemp, &dMaxTemp ) )
				{
					if( dMinTemp < dMin )	dMin	=	dMinTemp;
					if( dMaxTemp > dMax )	dMax	=	dMaxTemp;
				}
			}
			break;
		case STT_BOLL:
			{
				CBOLL	boll( &kdata ); 
				if( boll.GetMinMaxInfo( nStart, nEnd, &dMinTemp, &dMaxTemp ) )
				{
					if( dMinTemp < dMin )	dMin	=	dMinTemp;
					if( dMaxTemp > dMax )	dMax	=	dMaxTemp;
				}
			}
			break;
		case STT_PV:
			{
				CPV	pv( &kdata ); 
				if( pv.GetMinMaxInfo( nStart, nEnd, &dMinTemp, &dMaxTemp ) )
				{
					if( dMinTemp < dMin )	dMin	=	dMinTemp;
					if( dMaxTemp > dMax )	dMax	=	dMaxTemp;
				}
			}
			break;
		case STT_SAR:
			{
				CSAR	sar( &kdata ); 
				if( sar.GetMinMaxInfo( nStart, nEnd, &dMinTemp, &dMaxTemp ) )
				{
					if( dMinTemp < dMin )	dMin	=	dMinTemp;
					if( dMaxTemp > dMax )	dMax	=	dMaxTemp;
				}
			}
			break;
		case STT_DJ:
		case STT_CW:
			break;
		default:;
			ASSERT( FALSE );
		}
	}
	
	if( pdMin )
		*pdMin	=	dMin;
	if( pdMax )
		*pdMax	=	dMax;
	return TRUE;
}

void CStockGraph::DrawOneKLine(CDC * pDC, int nIndexPos, int nIndexKD, CKData * pKData, double dMin, double dMax, BOOL bGreyed )
{
	DECLARE_COLOR_DEFINATION

	// Check Valid
	ASSERT( pDC && nIndexPos >= m_nIndexStart && nIndexPos <= m_nIndexEnd && nIndexPos >= 0 );
	if( !(pDC && nIndexPos >= m_nIndexStart && nIndexPos <= m_nIndexEnd && nIndexPos >= 0) )
		return;
	if( !pKData || nIndexKD < 0 || nIndexKD >= pKData->GetSize() )
		return;
	if( dMax-dMin < 1e-4 )
		return;

	// Get Region
	CRect	rectK, rcEntity;
	long	xMedium	=	0;
	if( !GetOneKLineRect( nIndexPos, &rectK, &rcEntity.left, &rcEntity.right, &xMedium ) )
		return;

	int	xStart	=	rectK.left;
	int	xEnd	=	rectK.right;
	ASSERT( xEnd <= m_rectKLineCenter.right );
	if( xEnd > m_rectKLineCenter.right )
		return;

	KDATA	kd	=	pKData->ElementAt(nIndexKD);

	// Set rcEntity's top and bottom, set yLow, yHigh
	int	yLow	=	int( m_rectKLineCenter.bottom - m_rectKLineCenter.Height() * (kd.m_fLow - dMin) / (dMax-dMin) );
	int	yHigh	=	int( m_rectKLineCenter.bottom - m_rectKLineCenter.Height() * (kd.m_fHigh - dMin) / (dMax-dMin) );
	int	yOpen	=	int( m_rectKLineCenter.bottom - m_rectKLineCenter.Height() * (kd.m_fOpen - dMin) / (dMax-dMin) );
	int	yClose	=	int( m_rectKLineCenter.bottom - m_rectKLineCenter.Height() * (kd.m_fClose - dMin) / (dMax-dMin) );
	rcEntity.top	=	min( yOpen, yClose );
	rcEntity.bottom	=	max( yOpen, yClose ) + 1;

	if( CStockGraph::klineCandle == m_nCurKLineMode )
	{
		// Draw Entity
		COLORREF	clr	=	clrRise;
		if( kd.m_fClose < kd.m_fOpen )
			clr	=	clrFallEntity;
		if( kd.m_date > m_dwLatestDate )
			clr	=	clrNewKLine;
		if( bGreyed )
			clr	=	clrDJ;
		pDC->SetBkColor( clrBK );
		if( kd.m_fClose < kd.m_fOpen )
			pDC->FillSolidRect( &rcEntity, clr );
		else
			pDC->Draw3dRect( &rcEntity, clr, clr );
		
		// Draw Line
		CPen	pen( PS_SOLID, 1, clr );
		CPen *pOldPen = pDC->SelectObject( &pen );
		pDC->MoveTo( xMedium, yHigh );
		pDC->LineTo( xMedium, rcEntity.top );
		pDC->MoveTo( xMedium, rcEntity.bottom );
		pDC->LineTo( xMedium, yLow );

		pDC->SelectObject( pOldPen );
	}
	else if( CStockGraph::klineAmerica == m_nCurKLineMode )
	{
		// Draw Entity
		COLORREF	clr	=	clrRise;
		if( kd.m_date > m_dwLatestDate )
			clr	=	clrNewKLine;
		if( bGreyed )
			clr	=	clrDJ;
		pDC->SetBkColor( clrBK );
		
		// Draw Line
		CPen	pen( PS_SOLID, 1, clr );
		CPen *pOldPen = pDC->SelectObject( &pen );
		pDC->MoveTo( xStart, yHigh );
		pDC->LineTo( xStart, yLow );
		pDC->MoveTo( xStart, yClose );
		pDC->LineTo( xEnd, yClose );

		pDC->SelectObject( pOldPen );
	}
	else if( CStockGraph::klineTower == m_nCurKLineMode )
	{
		/*
		在起步股價和當日收盤價之間畫線,股價上漲時,畫紅色空心實體,股價下跌時,畫綠色實心實體。
		若前一交易日是上漲紅線,而次日下跌,股價未跌破前一天紅線低點的那一部分跌幅仍畫紅線,跌破部分畫綠線。
		若前一交易日為下跌綠線,而次日上漲,股價未漲過前一天綠線低點的那一部分漲幅仍畫綠色實體,漲過部分才畫紅線。
		*/
		// Draw Entity
		COLORREF	clr	=	clrRise;
		if( kd.m_fClose < kd.m_fOpen )
			clr	=	clrFallEntity;
		if( kd.m_date > m_dwLatestDate )
			clr	=	clrNewKLine;
		if( bGreyed )
			clr	=	clrDJ;
		pDC->SetBkColor( clrBK );
		if( kd.m_fClose < kd.m_fOpen )
			pDC->FillSolidRect( &rcEntity, clr );
		else
			pDC->Draw3dRect( &rcEntity, clr, clr );

		if( nIndexKD > 0 )
		{
			KDATA	kdLast	=	pKData->ElementAt(nIndexKD-1);
			int	yOpenLast	=	int( m_rectKLineCenter.bottom - m_rectKLineCenter.Height() * (kdLast.m_fOpen - dMin) / (dMax-dMin) );
			int	yCloseLast	=	int( m_rectKLineCenter.bottom - m_rectKLineCenter.Height() * (kdLast.m_fClose - dMin) / (dMax-dMin) );
			if( kdLast.m_fClose > kdLast.m_fOpen && kd.m_fClose < kd.m_fOpen )
			{
				rcEntity.bottom	=	min(yOpenLast,rcEntity.bottom);
				if( rcEntity.bottom > rcEntity.top )
				{
					pDC->FillSolidRect( &rcEntity, clrBK );
					clr	=	clrRise;
					if( kd.m_date > m_dwLatestDate )
						clr	=	clrNewKLine;
					if( bGreyed )
						clr	=	clrDJ;
					pDC->Draw3dRect( &rcEntity, clr, clr );
				}
			}
			else if( kdLast.m_fClose < kdLast.m_fOpen && kd.m_fClose > kd.m_fOpen )
			{
				rcEntity.top	=	max(yOpenLast,rcEntity.top);
				if( rcEntity.bottom > rcEntity.top )
				{
					clr	=	clrFallEntity;
					if( kd.m_date > m_dwLatestDate )
						clr	=	clrNewKLine;
					if( bGreyed )
						clr	=	clrDJ;
					pDC->FillSolidRect( &rcEntity, clr );
				}
			}
		}
	}
}

void CStockGraph::DrawBorderMovingLine( CDC * pDC, CPoint point, BOOL bAddOrRemove )
{
	ASSERT( m_pParent && ::IsWindow( m_pParent->GetSafeHwnd() ) );
	if( !m_pParent || !::IsWindow(m_pParent->GetSafeHwnd()) )
		return;

	CClientDC	dc(m_pParent);
	if( NULL == pDC )
		pDC		=	&dc;

	CPen	penLine( PS_SOLID, 1, AfxGetProfile().GetColor( CColorClass::clrBorder ) );
	CPen * pOldPen = pDC->SelectObject( &penLine );
	int nOldDrawMode = pDC->SetROP2( R2_XORPEN );

	pDC->MoveTo( m_rectKLine.left-5, point.y );
	pDC->LineTo( m_rectKLine.right+5, point.y );

	pDC->SetROP2( nOldDrawMode );
	pDC->SelectObject( pOldPen );
}

void CStockGraph::DrawSelectionLine( CDC * pDC, BOOL bAddOrRemove )
{
	ASSERT( m_pParent && ::IsWindow( m_pParent->GetSafeHwnd() ) );
	if( !m_pParent || !::IsWindow(m_pParent->GetSafeHwnd()) )
		return;

	CClientDC	dc(m_pParent);
	if( NULL == pDC )
		pDC		=	&dc;

	CPen	penVLine( PS_SOLID, 1, AfxGetProfile().GetColor( CColorClass::clrLine1 ) );
	CPen * pOldPen = pDC->SelectObject( &penVLine );
	int nOldDrawMode = pDC->SetROP2( R2_XORPEN );

	CRect	rectK;
	long	xMedium = 0;
	if( GetOneKLineRect( m_nIndexCurrent, &rectK, NULL, NULL, &xMedium ) )
	{
		pDC->MoveTo( xMedium, m_rectCenter.bottom );
		pDC->LineTo( xMedium, m_rectKLineCenter.top );
	}

	for( int k=0; k<m_anIndexCurrent.GetSize(); k++ )
	{
		if( ((int)m_anIndexCurrent[k]) == m_nIndexCurrent )
			continue;
		if( GetOneKLineRect( m_anIndexCurrent[k], &rectK, NULL, NULL, &xMedium ) )
		{
			pDC->MoveTo( xMedium, m_rectKLineCenter.bottom );
			pDC->LineTo( xMedium, m_rectKLineCenter.top );
		}
	}

	pDC->SetROP2( nOldDrawMode );
	pDC->SelectObject( pOldPen );
}

void CStockGraph::DrawSelectionRect( CDC * pDC, CPoint ptBegin, CPoint ptEnd, BOOL bAddOrRemove )
{
	ASSERT( m_pParent && ::IsWindow( m_pParent->GetSafeHwnd() ) );
	if( !m_pParent || !::IsWindow(m_pParent->GetSafeHwnd()) )
		return;

	CClientDC	dc(m_pParent);
	if( NULL == pDC )
		pDC		=	&dc;

	CPen	penVLine( PS_SOLID, 1, AfxGetProfile().GetColor( CColorClass::clrLine2 ) );
	CPen * pOldPen = pDC->SelectObject( &penVLine );
	int nOldDrawMode = pDC->SetROP2( R2_XORPEN );

	CRect	rect;
	rect.left	=	min(ptBegin.x,ptEnd.x);
	rect.right	=	max(ptBegin.x,ptEnd.x);
	rect.top	=	min(ptBegin.y,ptEnd.y);
	rect.bottom	=	max(ptBegin.y,ptEnd.y);

	pDC->MoveTo( rect.left, rect.top );
	pDC->LineTo( rect.right, rect.top );
	pDC->LineTo( rect.right, rect.bottom );
	pDC->LineTo( rect.left, rect.bottom );
	pDC->LineTo( rect.left, rect.top );

	pDC->SetROP2( nOldDrawMode );
	pDC->SelectObject( pOldPen );
}

void CStockGraph::DrawKData( )
{
	if( !::IsWindow( m_wndKData.GetSafeHwnd() ) )
	{
		if( !m_wndKData.Create( NULL, "", WS_CHILD, m_rectKData, m_pParent, 0 ) )
		{
			TRACE( "Failed to create m_wndKData!\n" );
			return;
		}
		m_wndKData.SetWindowPos( NULL, m_rectKData.left, m_rectKData.top, m_rectKData.Width(), m_rectKData.Height(),
								SWP_SHOWWINDOW );
	}

	CClientDC	dc(&m_wndKData);
	CRect		rectClient;
	m_wndKData.GetClientRect( &rectClient );

	CKData	&kdata	=	m_CurStock.GetKData(m_nCurKType);
	if( m_nIndexCurrent >= 0 && m_nIndexCurrent < kdata.GetSize() )
	{
		m_wndKData.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
		m_wndKData.ValidateRect( &rectClient );
		DrawKData( &dc, rectClient );
		m_wndKData.ValidateRect( &rectClient );
	}
	else
		m_wndKData.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW );
}

void CStockGraph::DrawKData( CDC * pDC, CRect rect )
{
	CHECK_NODATATODRAW
	DECLARE_COLOR_DEFINATION

	CStockInfo & info	=	m_CurStock.GetStockInfo();
	CKData &kdata		=	m_CurStock.GetKData( m_nCurKType );
	CKData &kdataDown	=	m_CurStockDown.GetKData( m_nCurKType );

	// Prepare kd and fLastClose
	KDATA	kd;
	memset( &kd, 0, sizeof(kd) );
	float	fLastClose	=	0;
	double	dAmount = 0, dVolume = 0, dPriceAverage = 0, dRatioChangeHand = 0, dVolumeDown = 0;
	if( m_anIndexCurrent.GetSize() > 0 
		&& (int)m_anIndexCurrent[0] < kdata.GetSize() && (int)m_anIndexCurrent[m_anIndexCurrent.GetSize()-1] < kdata.GetSize() )
	{
		int	nStart	=	m_anIndexCurrent[0];
		int	nEnd	=	m_anIndexCurrent[m_anIndexCurrent.GetSize()-1];
		kd.m_date	=	kdata.ElementAt(nStart).m_date;
		kd.m_fOpen	=	kdata.ElementAt(nStart).m_fOpen;
		kd.m_fLow	=	kdata.ElementAt(nStart).m_fLow;
		kd.m_fClose	=	kdata.ElementAt(nEnd).m_fClose;
		for( int k=0; k<m_anIndexCurrent.GetSize() && (int)m_anIndexCurrent[k]<kdata.GetSize(); k++ )
		{
			KDATA	& kdTemp	=	kdata.ElementAt(m_anIndexCurrent[k]);
			if( (int)m_anIndexCurrent[k] < nStart )
			{
				kd.m_fOpen	=	kdTemp.m_fOpen;
				kd.m_date	=	kdTemp.m_date;
				nStart	=	m_anIndexCurrent[k];
			}
			if( (int)m_anIndexCurrent[k] > nEnd )
			{
				kd.m_fClose=	kdTemp.m_fClose;
				nEnd	=	m_anIndexCurrent[k];
			}
			if( kd.m_fHigh < kdTemp.m_fHigh )
				kd.m_fHigh	=	kdTemp.m_fHigh;
			if( kd.m_fLow > kdTemp.m_fLow )
				kd.m_fLow	=	kdTemp.m_fLow;
			dAmount	+=	kdTemp.m_fAmount;
			dVolume	+=	kdTemp.m_fVolume;
			if( (int)m_anIndexCurrent[k] < kdataDown.GetSize() )
				dVolumeDown	+=	kdataDown.ElementAt(m_anIndexCurrent[k]).m_fVolume;
		}
		fLastClose	=	kd.m_fOpen;
		if( nStart - 1 >= 0 )
			fLastClose	=	kdata.ElementAt(nStart-1).m_fClose;
	}
	else if( m_nIndexCurrent >= 0 && m_nIndexCurrent < kdata.GetSize() )
	{
		kd	=	kdata.ElementAt(m_nIndexCurrent);
		dAmount	=	kd.m_fAmount;
		dVolume	=	kd.m_fVolume;
		if( m_nIndexCurrent < kdataDown.GetSize() )
			dVolumeDown	+=	kdataDown.ElementAt(m_nIndexCurrent).m_fVolume;
		fLastClose	=	kd.m_fOpen;
		if( m_nIndexCurrent - 1 >= 0 )
			fLastClose	=	kdata.ElementAt(m_nIndexCurrent-1).m_fClose;
	}
	else
		return;

	if( dVolume >= 1 )
		dPriceAverage	=	dAmount / dVolume;
	m_CurStockDown.GetStockInfo().GetRatioChangeHand( &dRatioChangeHand, dVolumeDown );

	// 相对换手率
	double	dMainChangeHand = CHSL::GetMainChangeHand( m_CurStockDown.GetStockInfo().GetMarket(), kdataDown, m_nIndexCurrent );
	double	dRelativeChangeHand = 1.0;
	if( dMainChangeHand > 1e-6 )
		dRelativeChangeHand	=	dRatioChangeHand / dMainChangeHand;

	// Fill Background
	pDC->FillSolidRect( &rect, clrBK );
	pDC->Draw3dRect( &rect, clrBorder, clrBorder );
	pDC->SetBkColor( clrBK );

	CString	strTemp;
	CFont	* pOldFont	=	AfxSelectDCFont( pDC, 14 );

	int	x	=	rect.left + 1;
	int	y	=	rect.top;
	pDC->SetTextColor( clrText );
	strTemp.LoadString( IDS_STOCKGRAPH_TIME );
	pDC->TextOut( x, y+2,   strTemp );
	strTemp.LoadString( IDS_STOCKGRAPH_OPEN );
	pDC->TextOut( x, y+37,  strTemp );
	strTemp.LoadString( IDS_STOCKGRAPH_HIGH );
	pDC->TextOut( x, y+71,  strTemp );
	strTemp.LoadString( IDS_STOCKGRAPH_LOW );
	pDC->TextOut( x, y+105, strTemp );
	strTemp.LoadString( IDS_STOCKGRAPH_CLOSE );
	pDC->TextOut( x, y+139, strTemp );
	strTemp.LoadString( IDS_STOCKGRAPH_VOLUME );
	pDC->TextOut( x, y+173, strTemp );
	strTemp.LoadString( IDS_STOCKGRAPH_AMOUNT );
	pDC->TextOut( x, y+207, strTemp );
	strTemp.LoadString( IDS_STOCKGRAPH_PRICEAVERAGE );
	pDC->TextOut( x, y+241, strTemp );
	strTemp.LoadString( IDS_STOCKGRAPH_RELATIVECHANGEHAND );
	pDC->TextOut( x, y+275, strTemp );

	x	=	rect.right - 3;
	UINT nOldAlign = pDC->SetTextAlign( TA_RIGHT | TA_TOP );
	CSPTime	sptime;
	if( CKData::IsDayOrMin( m_nCurKType ) )
	{
		sptime.FromStockTimeDay( kd.m_date );
		strTemp	=	sptime.Format( "%y-%m-%d" );
	}
	else
	{
		sptime.FromStockTimeMin( kd.m_date );
		strTemp	=	sptime.Format( "%m%d%H%M" );
	}
	pDC->SetTextColor( clrPlane );
	pDC->TextOut( x, y+18, strTemp );

	// 价格小数位数格式串
	CString strPriceFmt;
	strPriceFmt.Format( "%%.%df", info.DigitBit() );

	strTemp.Format( strPriceFmt, kd.m_fOpen );
	pDC->SetTextColor( kd.m_fOpen > fLastClose ? clrRise : (kd.m_fOpen<fLastClose?clrFall:clrPlane) );
	pDC->TextOut( x, y+53, strTemp );
	
	strTemp.Format( strPriceFmt, kd.m_fHigh );
	pDC->SetTextColor( kd.m_fHigh > fLastClose ? clrRise : (kd.m_fHigh<fLastClose?clrFall:clrPlane) );
	pDC->TextOut( x, y+87, strTemp );
	
	strTemp.Format( strPriceFmt, kd.m_fLow );
	pDC->SetTextColor( kd.m_fLow > fLastClose ? clrRise : (kd.m_fLow<fLastClose?clrFall:clrPlane) );
	pDC->TextOut( x, y+121, strTemp );

	strTemp.Format( strPriceFmt, kd.m_fClose );
	pDC->SetTextColor( kd.m_fClose > fLastClose ? clrRise : (kd.m_fClose<fLastClose?clrFall:clrPlane) );
	pDC->TextOut( x, y+155, strTemp );

	strTemp.Format( "%.0f", dVolume/100 );
	pDC->SetTextColor( clrTitle );
	pDC->TextOut( x, y+189, strTemp );

	strTemp.Format( "%.0f", dAmount/1000 );
	pDC->SetTextColor( clrTitle );
	pDC->TextOut( x, y+223, strTemp );

	strTemp.Format( strPriceFmt, dPriceAverage );
	pDC->SetTextColor( clrTitle );
	pDC->TextOut( x, y+257, strTemp );

	strTemp.Format( "%.2f", dRelativeChangeHand );
	pDC->SetTextColor( clrTitle );
	pDC->TextOut( x, y+291, strTemp );

	pDC->SetTextAlign( nOldAlign );
	pDC->SelectObject( pOldFont );
}

void CStockGraph::DrawTechLine(CDC * pDC )
{
	CSPDWordArray	& anShow	=	AfxGetProfile().GetGraphTechsShow( );
	for( int k=0; k<m_arcTech.GetSize(); k++ )
	{
		CRect	rect	=	m_arcTech.ElementAt(k);
		if( k < anShow.GetSize() )
		{
			DrawTechLine( pDC, anShow[k], rect, FALSE );
			DrawTechLine( pDC, anShow[k], rect, TRUE );
		}
	}
}

void CStockGraph::DrawKLineAttribute(CDC * pDC, UINT nTech, CRect rect, BOOL bDrawTitle, double dMin, double dMax )
{
	switch( nTech )
	{
	case STT_MA:		DrawTechMA( pDC, rect, bDrawTitle, dMin, dMax );		break;
	case STT_BBI:		DrawTechBBI( pDC, rect, bDrawTitle, dMin, dMax );		break;
	case STT_BOLL:		DrawTechBOLL( pDC, rect, bDrawTitle, dMin, dMax );		break;
	case STT_PV:		DrawTechPV( pDC, rect, bDrawTitle, dMin, dMax );		break;
	case STT_SAR:		DrawTechSAR( pDC, rect, bDrawTitle, dMin, dMax );		break;
	case STT_DJ:		DrawTechDJ( pDC, rect, bDrawTitle, dMin, dMax );		break;
	case STT_CW:		DrawTechCW( pDC, rect, bDrawTitle, dMin, dMax );		break;
	default:;
		ASSERT( FALSE );
	}
}

void CStockGraph::DrawTechLine(CDC * pDC, UINT nTech, CRect rect, BOOL bDrawTitle )
{
	DECLARE_COLOR_DEFINATION

	if( !bDrawTitle )
	{
		pDC->FillSolidRect( &rect, clrBK );
		pDC->FillSolidRect( CRect(m_rectAll.left,rect.top,rect.left-1,rect.bottom), clrBK );
		pDC->Draw3dRect( &rect, clrBorder, clrBorder );
	}

	switch( nTech )
	{
	case STT_MACD:		DrawTechMACD( pDC, rect, bDrawTitle );		break;
	case STT_MIKE:		DrawTechMIKE( pDC, rect, bDrawTitle );		break;
	case STT_PSY:		DrawTechPSY( pDC, rect, bDrawTitle );		break;

	case STT_VOLUME:	DrawTechVOLUME( pDC, rect, bDrawTitle );	break;
	case STT_NVI:		DrawTechNVI( pDC, rect, bDrawTitle );		break;
	case STT_PVI:		DrawTechPVI( pDC, rect, bDrawTitle );		break;
	case STT_VR:		DrawTechVR( pDC, rect, bDrawTitle );		break;
	case STT_VROC:		DrawTechVROC( pDC, rect, bDrawTitle );		break;
	case STT_OBV:		DrawTechOBV( pDC, rect, bDrawTitle );		break;
	case STT_MOBV:		DrawTechMOBV( pDC, rect, bDrawTitle );		break;
	case STT_MFI:		DrawTechMFI( pDC, rect, bDrawTitle );		break;
	case STT_VMACD:		DrawTechVMACD( pDC, rect, bDrawTitle );		break;
	case STT_WVAD:		DrawTechWVAD( pDC, rect, bDrawTitle );		break;
	case STT_EMV:		DrawTechEMV( pDC, rect, bDrawTitle );		break;
	case STT_VRSI:		DrawTechVRSI( pDC, rect, bDrawTitle );		break;
	case STT_NVRSI:		DrawTechNVRSI( pDC, rect, bDrawTitle );		break;
	case STT_AD:		DrawTechAD( pDC, rect, bDrawTitle );		break;
	case STT_CI:		DrawTechCI( pDC, rect, bDrawTitle );		break;

	case STT_KDJ:		DrawTechKDJ( pDC, rect, bDrawTitle );		break;
	case STT_R:			DrawTechR( pDC, rect, bDrawTitle );			break;
	case STT_RSI:		DrawTechRSI( pDC, rect, bDrawTitle );		break;
	case STT_BIAS:		DrawTechBIAS( pDC, rect, bDrawTitle );		break;
	case STT_MTM:		DrawTechMTM( pDC, rect, bDrawTitle );		break;
	case STT_DMI:		DrawTechDMI( pDC, rect, bDrawTitle );		break;
	case STT_ROC:		DrawTechROC( pDC, rect, bDrawTitle );		break;
	case STT_CCI:		DrawTechCCI( pDC, rect, bDrawTitle );		break;
	case STT_CV:		DrawTechCV( pDC, rect, bDrawTitle );		break;
	case STT_ARBR:		DrawTechARBR( pDC, rect, bDrawTitle );		break;
	case STT_CR:		DrawTechCR( pDC, rect, bDrawTitle );		break;
	case STT_OSC:		DrawTechOSC( pDC, rect, bDrawTitle );		break;
	case STT_UOS:		DrawTechUOS( pDC, rect, bDrawTitle );		break;
	case STT_MAOSC:		DrawTechMAOSC( pDC, rect, bDrawTitle );		break;
	case STT_36BIAS:	DrawTech3_6BIAS( pDC, rect, bDrawTitle );	break;
	case STT_DPO:		DrawTechDPO( pDC, rect, bDrawTitle );		break;
	case STT_KST:		DrawTechKST( pDC, rect, bDrawTitle );		break;

	case STT_REI:		DrawTechREI( pDC, rect, bDrawTitle );		break;
	case STT_DMKI:		DrawTechDMKI( pDC, rect, bDrawTitle );		break;
	case STT_PCNT:		DrawTechPCNT( pDC, rect, bDrawTitle );		break;
	case STT_HLC:		DrawTechHLC( pDC, rect, bDrawTitle );		break;
	case STT_CDP:		DrawTechCDP( pDC, rect, bDrawTitle );		break;
	case STT_ASI:		DrawTechASI( pDC, rect, bDrawTitle );		break;
	case STT_ATR:		DrawTechATR( pDC, rect, bDrawTitle );		break;

	case STT_CYO:		DrawTechCYO( pDC, rect, bDrawTitle );		break;
	case STT_DCYO:		DrawTechDCYO( pDC, rect, bDrawTitle );		break;
	case STT_HSL:		DrawTechHSL( pDC, rect, bDrawTitle );		break;
	case STT_DPER:		DrawTechDPER( pDC, rect, bDrawTitle );		break;

	default:
		{
			UINT nTechUserCount = CTechUser::GetTechUserCount();
			if( nTech >= STT_USER_MIN && nTech <= STT_USER_MIN+nTechUserCount-1 )
				DrawTechUser( nTech, pDC, rect, bDrawTitle );
			else
				ASSERT( FALSE );
		}
	}
}

void CStockGraph::DrawReportParam( CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	// Prepare Data
/* Old Version
	m_CurStock.PrepareData( CStock::dataK, CKData::ktypeDay );
	m_CurStock.PrepareData( CStock::dataDR );
	m_CurStock.PrepareData( CStock::dataK, m_nCurKType );
	CStockInfo	& info	=	m_CurStock.GetStockInfo();
	CKData	& kday	=	m_CurStock.GetKDataDay();
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	CKData	& kdayMain	=	AfxGetStockMain().GetKDataDay();
	kday.SetDRData( m_CurStock.GetDRData() );
	if( kday.GetCurFormat() < CKData::formatMin
		|| kday.GetCurFormat() > CKData::formatMax )
		return ;
	kday.FullFillKData( kdayMain, FALSE );
	if( CKData::formatXDRup != kday.GetCurFormat() && CKData::formatXDRdown != kday.GetCurFormat() )
		kday.ChangeCurFormat( CKData::formatXDRdown, AfxGetProfile().GetAutoResumeDRBegin(), AfxGetProfile().GetAutoResumeDRLimit() );
*/
	CStockInfo	& info	=	m_CurStock.GetStockInfo();
	CKData	& kday	=	m_CurStockDown.GetKDataDay();
	CKData	& kdata =	m_CurStockDown.GetKData(m_nCurKType);
	CKData	& kdayMain	=	AfxGetStockMain().GetKDataDay();

	double		dValue	=	0.;
	DWORD	dateCurrent	=	kdata.GetDate(m_nIndexCurrent);
	DWORD	dateDayCurrent	=	kdata.GetDateDay( m_nIndexCurrent );
	int		nIndexDay		=	kday.GetIndexByDate( dateDayCurrent );
	int		nIndexDayMain	=	kdayMain.GetIndexByDate( dateDayCurrent );

	// Draw Rect
	pDC->FillSolidRect( &m_rectReport, clrBK );
	pDC->Draw3dRect( &m_rectReport, clrBorder, clrBorder );
	pDC->SetBkColor( clrBK );

	// Select Name And Code Font
	CString	strTemp, strFont, strTempFmt, strDay;
	strDay.LoadString( IDS_STOCKGRAPH_DAY );
	CFont	* pOldFont	=	AfxSelectDCFont( pDC, 18 );

	// Draw Name And Code
	pDC->SetTextColor( clrTitle );
	pDC->TextOut( m_rectReport.left+1, m_rectAll.top+3, m_CurStock.GetStockInfo().GetStockName() );
	pDC->SetTextColor( clrText );
	pDC->TextOut( m_rectReport.left+96, m_rectAll.top+3, m_CurStock.GetStockInfo().GetStockCode() );

	// Select Param Font
	AfxSelectDCFont( pDC, 14 );
	
	// Draw Param Names
	int	x	=	m_rectReport.left + 6;
	int	y	=	m_rectReport.top;
	pDC->SetTextColor( clrText );
	strTemp.LoadString( IDS_STOCKGRAPH_TIME2 );
	pDC->TextOut( x, y+5,   strTemp );
#ifdef CLKLAN_ENGLISH_US
	strTempFmt.Format( "(%d)", AfxGetProfile().GetYieldAverageDays() );
#else
	strTempFmt.Format( "-%d%s-", AfxGetProfile().GetYieldAverageDays(), strDay );
#endif
	strTemp.LoadString( IDS_STOCKGRAPH_YIELDAVERAGE );
	pDC->TextOut( x, y+30,  strTemp+strTempFmt );
	strTemp.LoadString( IDS_STOCKGRAPH_YIELDSTDDEV );
	pDC->TextOut( x, y+55,  strTemp+strTempFmt );
	strTemp.LoadString( IDS_STOCKGRAPH_BEITE );
	pDC->TextOut( x, y+80,  strTemp+strTempFmt );
	strTemp.LoadString( IDS_STOCKGRAPH_XIAPU );
	pDC->TextOut( x, y+105, strTemp+strTempFmt );

	DrawLine( pDC, 1, clrBorder, m_rectReport.left, y+126, m_rectReport.right, y+126 );
	strTempFmt.LoadString( IDS_STOCKGRAPH_CHANGEHAND );
	strTemp.Format( "%s-%d", strTempFmt, 1 );
	pDC->TextOut( x, y+130, strTemp );
	strTemp.Format( "%s-%d", strTempFmt, 5 );
	pDC->TextOut( x, y+155, strTemp );
	strTemp.Format( "%s-%d", strTempFmt, 10 );
	pDC->TextOut( x, y+180, strTemp );
	strTemp.Format( "%s-%d", strTempFmt, 20 );
	pDC->TextOut( x, y+205, strTemp );

	DrawLine( pDC, 1, clrBorder, m_rectReport.left, y+226, m_rectReport.right, y+226 );
	strTempFmt.LoadString( IDS_STOCKGRAPH_RATIOVOLUME );
	strTemp.Format( "%s-%d", strTempFmt, 5 );
	pDC->TextOut( x, y+230, strTemp );
	strTemp.Format( "%s-%d", strTempFmt, 10 );
	pDC->TextOut( x, y+255, strTemp );
	strTemp.Format( "%s-%d", strTempFmt, 20 );
	pDC->TextOut( x, y+280, strTemp );

	DrawLine( pDC, 1, clrBorder, m_rectReport.left, y+301, m_rectReport.right, y+301 );
	strTempFmt.LoadString( IDS_STOCKGRAPH_RS );
	strTemp.Format( "%s-%d", strTempFmt, 5 );
	pDC->TextOut( x, y+305, strTemp );
	strTemp.Format( "%s-%d", strTempFmt, 10 );
	pDC->TextOut( x, y+330, strTemp );
	strTemp.Format( "%s-%d", strTempFmt, 20 );
	pDC->TextOut( x, y+355, strTemp );

	DrawLine( pDC, 1, clrBorder, m_rectReport.left, y+376, m_rectReport.right, y+376 );
	strTemp.LoadString( IDS_STOCKGRAPH_SHARETOTAL );
	pDC->TextOut( x, y+380, strTemp );
	strTemp.LoadString( IDS_STOCKGRAPH_SHARECURRENCY );
	pDC->TextOut( x, y+405, strTemp );
	strTemp.LoadString( IDS_STOCKGRAPH_NETASSETPS );
	pDC->TextOut( x, y+430, strTemp );
	strTemp.LoadString( IDS_STOCKGRAPH_EPS );
	pDC->TextOut( x, y+455, strTemp );
	strTemp.LoadString( IDS_STOCKGRAPH_PE );
	pDC->TextOut( x, y+480, strTemp );

	// Draw Param Values
	// Time
	UINT nOldAlign = pDC->SetTextAlign( TA_RIGHT | TA_TOP );
	pDC->SetTextColor( clrPlane );
	x	=	m_rectReport.right - 5;

	CSPTime	sptime;
	if( CKData::IsDayOrMin( m_nCurKType ) )
	{
		sptime.FromStockTimeDay( dateCurrent );
		strTemp	=	sptime.Format( "%Y-%m-%d" );
	}
	else
	{
		sptime.FromStockTimeMin( dateCurrent );
		strTemp	=	sptime.Format( "%Y-%m-%d %H:%M" );
	}
	pDC->TextOut( x, y+5, strTemp );

	// Change Hand
	strTemp	=	"-   ";
	if( info.GetRatioChangeHand( kdata, &dValue, dateCurrent, 1) )
		strTemp.Format( "%.2f%%", dValue );
	pDC->TextOut( x, y+130, strTemp );
	strTemp	=	"-   ";
	if( info.GetRatioChangeHand( kdata, &dValue, dateCurrent, 5) )
		strTemp.Format( "%.2f%%", dValue );
	pDC->TextOut( x, y+155, strTemp );
	strTemp	=	"-   ";
	if( info.GetRatioChangeHand( kdata, &dValue, dateCurrent, 10) )
		strTemp.Format( "%.2f%%", dValue );
	pDC->TextOut( x, y+180, strTemp );
	strTemp	=	"-   ";
	if( info.GetRatioChangeHand( kdata, &dValue, dateCurrent, 20) )
		strTemp.Format( "%.2f%%", dValue );
	pDC->TextOut( x, y+205, strTemp );

	// Ratio Volume
	strTemp	=	"-   ";
	if( kdata.GetRatioVolume(&dValue, dateCurrent, 5) )
		strTemp.Format( "%.2f", dValue );
	pDC->SetTextColor( dValue >= 1.005 ? clrRise : (dValue<0.995?clrFall:clrPlane) );
	pDC->TextOut( x, y+230, strTemp );
	strTemp	=	"-   ";
	if( kdata.GetRatioVolume(&dValue, dateCurrent, 10) )
		strTemp.Format( "%.2f", dValue );
	pDC->SetTextColor( dValue >= 1.005 ? clrRise : (dValue<0.995?clrFall:clrPlane) );
	pDC->TextOut( x, y+255, strTemp );
	strTemp	=	"-   ";
	if( kdata.GetRatioVolume(&dValue, dateCurrent, 20) )
		strTemp.Format( "%.2f", dValue );
	pDC->SetTextColor( dValue >= 1.005 ? clrRise : (dValue<0.995?clrFall:clrPlane) );
	pDC->TextOut( x, y+280, strTemp );

	// RS
	strTemp	=	"-   ";
	if( kdata.GetRS(&dValue, dateCurrent, 5) )
		strTemp.Format( "%.2f%%", dValue );
	pDC->SetTextColor( dValue >= 1.005 ? clrRise : (dValue<0.995?clrFall:clrPlane) );
	pDC->TextOut( x, y+305, strTemp );
	strTemp	=	"-   ";
	if( kdata.GetRS(&dValue, dateCurrent, 10) )
		strTemp.Format( "%.2f%%", dValue );
	pDC->SetTextColor( dValue >= 1.005 ? clrRise : (dValue<0.995?clrFall:clrPlane) );
	pDC->TextOut( x, y+330, strTemp );
	strTemp	=	"-   ";
	if( kdata.GetRS(&dValue, dateCurrent, 20) )
		strTemp.Format( "%.2f%%", dValue );
	pDC->SetTextColor( dValue >= 1.005 ? clrRise : (dValue<0.995?clrFall:clrPlane) );
	pDC->TextOut( x, y+355, strTemp );

	// Base Info
	pDC->SetTextColor( clrPlane );
	strTemp	=	"-   ";
	if( info.m_fShare_count_total > 1e-4 )
		strTemp.Format( "%.0f", info.m_fShare_count_total*0.0001 );
	pDC->TextOut( x, y+380, strTemp );
	strTemp	=	"-   ";
	if( info.GetShareCurrency( &dValue ) )
		strTemp.Format( "%.0f", dValue*0.0001 );
	pDC->TextOut( x, y+405, strTemp );
	strTemp	=	"-   ";
	if( fabs(info.m_fNetasset_ps) > 1e-4 )
		strTemp.Format( "%.2f", info.m_fNetasset_ps );
	pDC->TextOut( x, y+430, strTemp );
	strTemp	=	"-   ";
	if( fabs(info.m_fEps) > 1e-4 )
		strTemp.Format( "%.2f(%d)", info.m_fEps, info.m_reporttype );
	pDC->TextOut( x, y+455, strTemp );
	strTemp	=	"-   ";
	if( info.GetPE(&dValue) )
		strTemp.Format( "%.1f", dValue );
	pDC->TextOut( x, y+480, strTemp );

	// Yield
	double	market_dayyield_average = 0.0, market_dayyield_d = 0.0;
	BOOL	bmarket_yield_ok	=	TRUE;
	if( !CalculateYieldDay( kdayMain, &market_dayyield_average, &market_dayyield_d, AfxGetProfile().GetYieldAverageDays(), nIndexDayMain ) )
		bmarket_yield_ok	=	FALSE;
	double	yield_average = 0., yield_d = 0., beite = 0., xiapu = 0. ;
	if( CalculateYieldDay( kday, &yield_average, &yield_d, AfxGetProfile().GetYieldAverageDays(), nIndexDay ) )
	{
		yield_average	=	( 100. * (pow(1+yield_average,STKLIB_DAYS_INONEYEAR)-1) );
		yield_d			=	( 100. * sqrt((double)STKLIB_DAYS_INONEYEAR) * yield_d );
		if( fabs(yield_d) > 0.01 )
		{
			xiapu	=	100. * yield_average / yield_d;
			strTemp.Format( "%.2f%%", xiapu );
			pDC->TextOut( x, y+105, strTemp );
		}

		strTemp.Format( "%.2f%%", yield_average );
		pDC->TextOut( x, y+30, strTemp );
		strTemp.Format( "%.2f%%", yield_d );
		pDC->TextOut( x, y+55, strTemp );
	}
	if( bmarket_yield_ok && CalculateBeiteDay( kday, kdayMain, market_dayyield_average, market_dayyield_d, &beite, AfxGetProfile().GetYieldAverageDays(), nIndexDay ) )
	{
		beite		=	beite;
		strTemp.Format( "%.2f", beite );
		pDC->TextOut( x, y+80, strTemp );
	}

	pDC->SetTextAlign( nOldAlign );
	pDC->SelectObject( pOldFont );

//	kday.ChangeCurFormat( m_nCurKFormat, AfxGetProfile().GetAutoResumeDRBegin(), AfxGetProfile().GetAutoResumeDRLimit() );
}

void CStockGraph::DrawReportCost( CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	// Draw Rect
	pDC->FillSolidRect( &m_rectReport, clrBK );
	pDC->Draw3dRect( &m_rectReport, clrBorder, clrBorder );
	pDC->SetBkColor( clrBK );

	// Select Name And Code Font
	CString	strTemp, strFont, strTempFmt, strDay;
	strDay.LoadString( IDS_STOCKGRAPH_DAY );
	CFont	* pOldFont	=	AfxSelectDCFont( pDC, 18 );
	UINT nOldAlign = pDC->SetTextAlign( TA_LEFT | TA_TOP );

	// Draw Name And Code
	pDC->SetTextColor( clrTitle );
	pDC->TextOut( m_rectReport.left+1, m_rectAll.top+3, m_CurStock.GetStockInfo().GetStockName() );
	pDC->SetTextColor( clrText );
	pDC->TextOut( m_rectReport.left+96, m_rectAll.top+3, m_CurStock.GetStockInfo().GetStockCode() );

	// 成本分布图
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	double	dStep	=	(m_dMaxPrice-m_dMinPrice)*0.003;
	CSPDWordArray adwPrice, adwVolume;
	double	dMinVolume = 0, dMaxVolume = 0, dTotalVolume = 0, dVolPercent = 0;

	CCW	cw(&kdata);
	cw.m_dChangeHand	=	1.5;
	int	nEnd = m_nIndexCurrent;
	if( -1 == nEnd )
		nEnd	=	kdata.GetSize()-1;
	if( !cw.CalculatePastCW(nEnd, 0, m_CurStock.GetStockInfo(), dStep, adwPrice, adwVolume, &dMinVolume, &dMaxVolume, &dTotalVolume, &dVolPercent )
		|| dMinVolume < 0 || dMaxVolume < 1e-4 || dTotalVolume < 1e-4 )
		return;

	COLORREF	clrGain = RGB(255,128,0), clrBad = RGB(160,160,255);
	CPen	penGain( PS_SOLID, 1, clrGain );
	CPen	penBad( PS_SOLID, 1, clrBad );
	CPen * pOldPen = pDC->SelectObject( &penGain );

	double	dPriceSel = 0;
	if( nEnd >= 0 && nEnd < kdata.GetSize() )
		dPriceSel = kdata.ElementAt(nEnd).m_fClose;

	double	dVolume = 0, dPrice = 0;
	double	dVolFactor	=	-1;
	for( int k=0; k<adwPrice.GetSize() && k<adwVolume.GetSize(); k++ )
	{
		if( dVolFactor < 0 )
			dVolFactor	=	40.0/dTotalVolume;

		dPrice	=	double(adwPrice[k]) * 0.001;
		dVolume	=	adwVolume[k];
		if( dPrice > dPriceSel )
			pDC->SelectObject( &penBad );
		int	yPrice	=	int( m_rectKLineCenter.bottom - m_rectKLineCenter.Height() * (dPrice - m_dMinPrice) / (m_dMaxPrice-m_dMinPrice) );
		int	xVolume	=	int( m_rectReport.left + (m_rectReport.Width()*dVolume*dVolFactor) );
		if( yPrice > m_rectReport.top+1 && yPrice < m_rectReport.bottom-2 )
		{
			pDC->MoveTo( m_rectReport.left + 1, yPrice );
			pDC->LineTo( xVolume, yPrice );
		}
	}

	// Select Param Font
	AfxSelectDCFont( pDC, 12 );
	pDC->SetBkColor( clrBK );
	pDC->SetTextColor( clrText );
	int	x = m_rectReport.left+2;
	int y = m_rectReport.bottom-100;

	// Date
	CString	strDate;
	if( nEnd >= 0 && nEnd < kdata.GetSize() )
		strDate	=	::AfxGetTimeString( kdata.ElementAt(nEnd).m_time, "%Y-%m-%d", TRUE );
	pDC->TextOut( x+10, y, strDate );

	// 获利比例： 
	double	dGainPercent = 0;
	CCW::StatGainPercent( &dGainPercent, adwPrice, adwVolume, dPriceSel );
	strTemp.LoadString( IDS_STOCKGRAPH_GAINPERCENT );
	pDC->TextOut( x, y+20, strTemp );
	strTemp.Format( "%.1f%%", dGainPercent*100 );
	pDC->TextOut( x+100, y+20, strTemp );
	CRect	rectGain( x+60, y+20, x+160, y+32 );
	pDC->Draw3dRect( rectGain, clrBad, clrBad );
	rectGain.right	=	rectGain.left + rectGain.Width() * dGainPercent;
	pDC->Draw3dRect( rectGain, clrGain, clrGain );
	pDC->SetBkColor( clrBK );

	// 平均成本： %.3f元
	double	dCostAve = 0;
	CCW::StatCostAverage( &dCostAve, adwPrice, adwVolume );
	strTempFmt.LoadString( IDS_STOCKGRAPH_COSTAVE );
	strTemp.Format( strTempFmt, dCostAve );
	pDC->TextOut( x, y+36, strTemp );

	// 90%成本%.2f-%.2f，集中度%.1f%%
	double	dLow = 0, dHigh = 0, dMassPrice = 0;
	CCW::StatMass( &dLow, &dHigh, &dMassPrice, adwPrice, adwVolume, 0.9 );
	strTempFmt.LoadString( IDS_STOCKGRAPH_MASSFMT90 );
	strTemp.Format( strTempFmt, dLow, dHigh, dMassPrice*100 );
	pDC->TextOut( x, y+52, strTemp );

	// 70%成本%.2f-%.2f，集中度%.1f%%
	dLow = 0, dHigh = 0, dMassPrice = 0;
	CCW::StatMass( &dLow, &dHigh, &dMassPrice, adwPrice, adwVolume, 0.7 );
	strTempFmt.LoadString( IDS_STOCKGRAPH_MASSFMT70 );
	strTemp.Format( strTempFmt, dLow, dHigh, dMassPrice*100 );
	pDC->TextOut( x, y+68, strTemp );

	pDC->SelectObject( pOldPen );
	pDC->SetTextAlign( nOldAlign );
	pDC->SelectObject( pOldFont );
}

void CStockGraph::DrawReportFlame( CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	// Draw Rect
	pDC->FillSolidRect( &m_rectReport, clrBK );
	pDC->Draw3dRect( &m_rectReport, clrBorder, clrBorder );
	pDC->SetBkColor( clrBK );

	// Select Name And Code Font
	CString	strTemp, strFont, strTempFmt, strDay;
	strDay.LoadString( IDS_STOCKGRAPH_DAY );
	CFont	* pOldFont	=	AfxSelectDCFont( pDC, 18 );
	UINT nOldAlign = pDC->SetTextAlign( TA_LEFT | TA_TOP );

	// Draw Name And Code
	pDC->SetTextColor( clrTitle );
	pDC->TextOut( m_rectReport.left+1, m_rectAll.top+3, m_CurStock.GetStockInfo().GetStockName() );
	pDC->SetTextColor( clrText );
	pDC->TextOut( m_rectReport.left+96, m_rectAll.top+3, m_CurStock.GetStockInfo().GetStockCode() );

	// 火焰山
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	int	nEnd = m_nIndexCurrent;
	if( -1 == nEnd )
		nEnd	=	kdata.GetSize()-1;

	int	nBot = 5;
	CRect	rectDescript( m_rectReport.left+30, m_rectReport.bottom-nBot-100, m_rectReport.left+170, m_rectReport.bottom-nBot-84);
	DrawReportCW( pDC, FALSE, nEnd, 5, 1.5, RGB(255,0,0), rectDescript );
	
	rectDescript	=	CRect( m_rectReport.left+30, m_rectReport.bottom-nBot-84, m_rectReport.left+170, m_rectReport.bottom-nBot-68);
	DrawReportCW( pDC, FALSE, nEnd, 10, 1.5, RGB(255,0,128), rectDescript );

	rectDescript	=	CRect( m_rectReport.left+30, m_rectReport.bottom-nBot-68, m_rectReport.left+170, m_rectReport.bottom-nBot-52);
	DrawReportCW( pDC, FALSE, nEnd, 20, 1.5, RGB(255,128,0), rectDescript );
	
	rectDescript	=	CRect( m_rectReport.left+30, m_rectReport.bottom-nBot-52, m_rectReport.left+170, m_rectReport.bottom-nBot-36);
	DrawReportCW( pDC, FALSE, nEnd, 30, 1.5, RGB(255,128,128), rectDescript );

	rectDescript	=	CRect( m_rectReport.left+30, m_rectReport.bottom-nBot-36, m_rectReport.left+170, m_rectReport.bottom-nBot-20);
	DrawReportCW( pDC, FALSE, nEnd, 60, 1.5, RGB(255,255,0), rectDescript );

	// Select Param Font
	AfxSelectDCFont( pDC, 12 );

	// Date
	pDC->SetBkColor( clrBK );
	pDC->SetTextColor( clrText );
	CString	strDate;
	if( nEnd >= 0 && nEnd < kdata.GetSize() )
		strDate	=	::AfxGetTimeString( kdata.ElementAt(nEnd).m_time, "%Y-%m-%d", TRUE );
	pDC->TextOut( m_rectReport.left+20, m_rectReport.bottom-20, strDate );

	pDC->SetTextAlign( nOldAlign );
	pDC->SelectObject( pOldFont );
}

void CStockGraph::DrawReportActivity( CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	// Draw Rect
	pDC->FillSolidRect( &m_rectReport, clrBK );
	pDC->Draw3dRect( &m_rectReport, clrBorder, clrBorder );
	pDC->SetBkColor( clrBK );

	// Select Name And Code Font
	CString	strTemp, strFont, strTempFmt, strDay;
	strDay.LoadString( IDS_STOCKGRAPH_DAY );
	CFont	* pOldFont	=	AfxSelectDCFont( pDC, 18 );
	UINT nOldAlign = pDC->SetTextAlign( TA_LEFT | TA_TOP );

	// Draw Name And Code
	pDC->SetTextColor( clrTitle );
	pDC->TextOut( m_rectReport.left+1, m_rectAll.top+3, m_CurStock.GetStockInfo().GetStockName() );
	pDC->SetTextColor( clrText );
	pDC->TextOut( m_rectReport.left+96, m_rectAll.top+3, m_CurStock.GetStockInfo().GetStockCode() );

	// 活跃度
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	int	nEnd = m_nIndexCurrent;
	if( -1 == nEnd )
		nEnd	=	kdata.GetSize()-1;

	int	nBot = 5;
	CRect	rectDescript( m_rectReport.left+30, m_rectReport.bottom-nBot-36, m_rectReport.left+170, m_rectReport.bottom-nBot-20);
	DrawReportCW( pDC, TRUE, nEnd, 60, 1, RGB(0,40,160), rectDescript );

	rectDescript	=	CRect( m_rectReport.left+30, m_rectReport.bottom-nBot-52, m_rectReport.left+170, m_rectReport.bottom-nBot-36);
	DrawReportCW( pDC, TRUE, nEnd, 30, 1, RGB(80,80,255), rectDescript );

	rectDescript	=	CRect( m_rectReport.left+30, m_rectReport.bottom-nBot-68, m_rectReport.left+170, m_rectReport.bottom-nBot-52);
	DrawReportCW( pDC, TRUE, nEnd, 20, 1, RGB(120,80,255), rectDescript );

	rectDescript	=	CRect( m_rectReport.left+30, m_rectReport.bottom-nBot-84, m_rectReport.left+170, m_rectReport.bottom-nBot-68);
	DrawReportCW( pDC, TRUE, nEnd, 10, 1, RGB(120,120,255), rectDescript );

	rectDescript	=	CRect( m_rectReport.left+30, m_rectReport.bottom-nBot-100, m_rectReport.left+170, m_rectReport.bottom-nBot-84);
	DrawReportCW( pDC, TRUE, nEnd, 5, 1, RGB(160,160,255), rectDescript );

	// Select Param Font
	AfxSelectDCFont( pDC, 12 );

	// Date
	pDC->SetBkColor( clrBK );
	pDC->SetTextColor( clrText );
	CString	strDate;
	if( nEnd >= 0 && nEnd < kdata.GetSize() )
		strDate	=	::AfxGetTimeString( kdata.ElementAt(nEnd).m_time, "%Y-%m-%d", TRUE );
	pDC->TextOut( m_rectReport.left+20, m_rectReport.bottom-20, strDate );

	pDC->SetTextAlign( nOldAlign );
	pDC->SelectObject( pOldFont );
}

void CStockGraph::DrawReportCW( CDC * pDC, BOOL bRecent, int nEnd, int nDays, double dChangeHand, COLORREF clr, CRect rectDescript )
{
	double	dVolFactor = -1;

	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	double	dStep	=	(m_dMaxPrice-m_dMinPrice)*0.003;
	CSPDWordArray adwPrice, adwVolume;
	double	dMinVolume = 0, dMaxVolume = 0, dTotalVolume = 0, dVolPercent = 0;

	CCW	cw(&kdata);
	cw.m_dChangeHand	=	dChangeHand;
	int	yPriceLast	=	-1;

	BOOL	bOK	=	FALSE;
	if( bRecent )
		bOK	=	cw.CalculateRecentCW(nEnd, nDays, m_CurStock.GetStockInfo(), dStep, adwPrice, adwVolume, &dMinVolume, &dMaxVolume, &dTotalVolume, &dVolPercent );
	else
		bOK	=	cw.CalculatePastCW(nEnd, nDays, m_CurStock.GetStockInfo(), dStep, adwPrice, adwVolume, &dMinVolume, &dMaxVolume, &dTotalVolume, &dVolPercent );

	CPen	pen( PS_SOLID, 1, clr );
	CPen * pOldPen = pDC->SelectObject( &pen );

	if( bOK && dMinVolume >= 0 && dMaxVolume > 1e-4 && dTotalVolume > 1e-4
		&& m_dMaxPrice-m_dMinPrice > 1e-4 && m_dMinPrice > 1e-4 )
	{
		if( dVolFactor < 0 )
			dVolFactor	=	bRecent ? 15.0/dTotalVolume : 40.0/dTotalVolume;

		CArray <POINT, POINT &>	apt;
		apt.SetSize( 0, m_rectKLineCenter.Height() );
		double	dVolume = 0, dPrice = 0;
		for( int k=0; k<adwPrice.GetSize() && k<adwVolume.GetSize(); k++ )
		{
			dPrice	=	double(adwPrice[k]) * 0.001;
			dVolume	=	adwVolume[k];
			int	yPrice	=	int( m_rectKLineCenter.bottom - m_rectKLineCenter.Height() * (dPrice - m_dMinPrice) / (m_dMaxPrice-m_dMinPrice) );
			int	xVolume	=	int( m_rectReport.left + (m_rectReport.Width()*dVolume*dVolFactor) );
			if( yPrice > m_rectReport.top+1 && yPrice < m_rectReport.bottom-2 )
			{
				if( -1 == yPriceLast )
					yPriceLast	=	yPrice+1;
				for( int y=yPriceLast-1; y>=yPrice; y-- )
				{
					pDC->MoveTo( m_rectReport.left+2, y );
					pDC->LineTo( xVolume, y );
				}
				yPriceLast	=	yPrice;
				apt.Add( CPoint(xVolume, yPrice) );
			}
		}

		if( apt.GetSize() > 0 )
		{
			CPen	penEdge( PS_SOLID, 1, RGB(255,255,255) );
			pDC->SelectObject( &penEdge );
			pDC->MoveTo( apt[0] );
			pDC->PolylineTo( apt.GetData(), apt.GetSize() );
		}
	}

	// rectDescript
	pDC->Draw3dRect( rectDescript, clr, clr );
	pDC->FillSolidRect( rectDescript, clr );
	
	CString	sText, sTextFmt;
	if( bRecent )
		sTextFmt.LoadString( IDS_STOCKGRAPH_RECENTCWDISFMT );
	else
		sTextFmt.LoadString( IDS_STOCKGRAPH_PASTCWDISFMT );
	sText.Format( sTextFmt, nDays, dVolPercent*100 );
	CFont	* pOldFont	=	AfxSelectDCFont( pDC, 12 );
	pDC->SetTextColor( RGB(255,255,255) );
	pDC->SetBkColor( clr );
	pDC->TextOut( rectDescript.left+5, rectDescript.top+2, sText );

	pDC->SelectObject( pOldPen );
	pDC->SelectObject( pOldFont );
}
