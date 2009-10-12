// RealTime.cpp : implementation of the CRealTime class
//

#include "stdafx.h"
#include "RealTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define TOOLBARHIGHT 20

BOOL bShowReport = FALSE;


#define	RT_DETERMINE_COLOR(x,y)	((x)-(y)>1e-4?clrRise:((x)-(y)<1e-4?clrFall:clrPlane))

void DrawPriceAxisNumber(CDC * pDC, COLORREF clr, int nCount,
					int xStart, int yStart, int xEnd, int yEnd,
					double dLastClose, double fMin, double fMax, int nDotCount )
{
	int	nHeight	= yEnd - yStart;
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
		double	dCur	= fMin+k*(fMax-fMin)/nCount;
		if( dCur < 0 && dCur > (0-0.6*pow((double)10,0-nDotCount)) )
			dCur	= 0;
		strText.Format( strFmt, dCur );
		sizeText	= pDC->GetTextExtent( strText );
		pDC->TextOut( xStart-sizeText.cx-1, yStart+(nCount-k)*nHeight/nCount-7, strText );
	}
}

BOOL DrawPriceVolumeAxis(	CDC * pDC, CRect rectPrice, CRect rectVolume,
							int nGapYAxis, double dLastClose, double * pdMin, double * pdMax, double dMaxVolume, int nDotCount )
{
	DECLARE_COLOR_DEFINATION

	ASSERT( pdMin && pdMax );
	if( NULL == pdMin || NULL == pdMax )
		return FALSE;
	if( rectPrice.Height() <= 0 || rectPrice.Width() <= 0 )
		return FALSE;
	if( rectVolume.Height() <= 0 || rectVolume.Width() <= 0 )
		return FALSE;
	ASSERT( rectPrice.left == rectVolume.left && rectPrice.right == rectVolume.right );
	ASSERT( nDotCount >= 0 );
	if( nGapYAxis <= 0 || nDotCount < 0 )
		return FALSE;
	if( dLastClose < 1e-4 )
		dLastClose	= (*pdMin+*pdMax)/2;
	
	// 重新修正 *pdMin, *pdMax
	int	nPriceLines		= 2*((rectPrice.Height()/2)/nGapYAxis);
	int	nVolumeLines	= 2*((rectVolume.Height()/2)/nGapYAxis);
	double dABS = max(fabs(*pdMax-dLastClose),fabs(*pdMin-dLastClose));
	double dPrecision = pow((double)10,0-nDotCount);
	if( dABS < dPrecision*nPriceLines )
		dABS	= dPrecision*nPriceLines;
	else if( dABS < 2*dPrecision*nPriceLines )
		dABS	= 2*dPrecision*nPriceLines;
	else if( dABS < 3*dPrecision*nPriceLines )
		dABS	= 3*dPrecision*nPriceLines;
	*pdMin	= dLastClose-dABS;
	*pdMax	= dLastClose+dABS;
	
	// Draw Border
	pDC->Draw3dRect( &rectPrice, clrBorder, clrBorder );
	pDC->Draw3dRect( &rectVolume, clrBorder, clrBorder );

	// Draw Lines
	for( int k=0; k<nPriceLines; k++ )
		DrawLine( pDC, 1, clrBorder, rectPrice.left, rectPrice.top+k*rectPrice.Height()/nPriceLines, rectPrice.right, rectPrice.top+k*rectPrice.Height()/nPriceLines );
	DrawLine( pDC, 2, clrBorder, rectPrice.left, rectPrice.top+rectPrice.Height()/2, rectPrice.right, rectPrice.top+rectPrice.Height()/2 );
	for( int k=0; k<nVolumeLines; k++ )
		DrawLine( pDC, 1, clrBorder, rectVolume.left, rectVolume.top+k*rectVolume.Height()/nVolumeLines, rectVolume.right, rectVolume.top+k*rectVolume.Height()/nVolumeLines );

	// Draw Columns
	for( int k=1; k<=8; k++ )
	{
		if( k % 2 )
			DrawDashLine( pDC, 1, clrBorder, rectPrice.left+k*rectPrice.Width()/8, rectPrice.top, rectPrice.left+k*rectPrice.Width()/8, rectVolume.bottom );
		else
			DrawLine( pDC, 1, clrBorder, rectPrice.left+k*rectPrice.Width()/8, rectPrice.top, rectPrice.left+k*rectPrice.Width()/8, rectVolume.bottom );
	}

	// Draw Price Axis Number, Will Extent the rect
	CFont	* pOldFont	= AfxSelectDCFont( pDC, 12 );
	pDC->SetBkColor( clrBK );
	CSize	sizeText;
	CString	strFmt, strText;
	strFmt.Format( "%%.%df", nDotCount );
	double	dMinAxis = dLastClose-dABS;
	double	dMaxAxis = dLastClose+dABS;

	for( int k=0; k<=nPriceLines && nPriceLines>0; k++ )
	{
		double	dCur	= dMinAxis+k*(dMaxAxis-dMinAxis)/nPriceLines;
		COLORREF	clr	= clrPlane;
		if( dCur-dLastClose > 1e-4)
			clr	= clrRise;
		if( dCur-dLastClose < -1e-4 )
			clr	= clrFall;

		if( dCur < 0 && dCur > (-0.51*pow((double)10,0-nDotCount)) )
			dCur	= 0;
		strText.Format( strFmt, dCur );
		sizeText	= pDC->GetTextExtent( strText );
		pDC->SetTextColor(  clr );
		pDC->TextOut( rectPrice.left-sizeText.cx-1, rectPrice.top+(nPriceLines-k)*rectPrice.Height()/nPriceLines-7, strText );

		double	dCurPercent	= 0.00;
		if( dLastClose > 1e-4 )
			dCurPercent	= 100*fabs(dCur-dLastClose)/dLastClose;
		strText.Format( "%.2f%%", dCurPercent );
		pDC->TextOut( rectPrice.right+3, rectPrice.top+(nPriceLines-k)*rectPrice.Height()/nPriceLines-7, strText );
	}

	// Draw Volume Axis Number
	DrawAxisNumber(	pDC, clrPlane, nVolumeLines, rectVolume.left, rectVolume.top, rectVolume.right, rectVolume.bottom,
					0, dMaxVolume/100/*成交量手数*/, FALSE, FALSE, 0 );

	pDC->SelectObject( pOldFont );
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CRealTime

#define	RT_RDTITLE_HEIGHT	24
#define	RT_RDELEMENT_HEIGHT	16
#define	RT_RDCOLUMN_WIDTH	220

CRealTime::CRealTime( )
{
	m_pParent			= NULL;
	m_nDrawMode			= CRealTime::modePriceLine;
	m_nReportWhat		= CRealTime::reportQuote;
	m_nTechLine			= CRealTime::techNone;

	m_nIndexCurrent		= -1;

	m_nCurrentStartPos	= -1;
	m_nCurrentPageCount	= -1;
	m_nCurrentSelectPos	= -1;

	m_dMaxPrice			= 0;
	m_dMinPrice			= 0;
	
	ResetMargins( );
	ResetIndexCurrent( );
}

CRealTime::~CRealTime( )
{
	ClearCurStock();
}

void CRealTime::SetParent( CWnd * pParent )
{
	m_pParent	= pParent;
}

void CRealTime::ResetMargins(	int nMarginTop, int nMarginLeft,
								int nMarginCenter, int nMarginBottom,
								int nGapYAxis, int nWidthReport )
{
	ASSERT( nMarginTop >= 0 && nMarginLeft >= 0 );
	ASSERT( nMarginCenter >= 0 && nMarginBottom >= 0 );
	ASSERT( nGapYAxis > 0 );
	ASSERT( nWidthReport >= 0 );

	m_nMarginTop		= nMarginTop;
	m_nMarginLeft		= nMarginLeft;
	m_nMarginCenter		= nMarginCenter;
	m_nMarginBottom		= nMarginBottom;
	m_nGapYAxis		= nGapYAxis;
	m_nWidthReport	= nWidthReport;

	if( m_nGapYAxis <= 0 )
		m_nGapYAxis	= 18;
}

void CRealTime::ClearCurStock( )
{
	m_CurStock.Clear();
}

BOOL CRealTime::SetCurStock( const char * szCode )
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

BOOL CRealTime::SetCurStock( CStockInfo & info )
{
	if( m_CurStock.GetStockInfo().IsEqualTo( info.GetMarket(), info.GetStockCode() ) )
	{
		return TRUE;
	}

	ResetIndexCurrent( );
	ClearCurStock();
	m_CurStock.SetStockInfo( &info );
	
	return TRUE;
}

BOOL CRealTime::PrepareStockData( )
{
	m_CurStock.GetReport().RemoveAll();
	m_CurStock.GetMinute().RemoveAll();
	m_CurStock.GetOutline().RemoveAll();

	// 读取本地文件
	m_CurStock.SetDatabase( &AfxGetDB() );

	// 取得最新行情
	m_CurStock.PrepareData( CStock::dataReport, CKData::ktypeDay, TRUE );
	m_CurStock.PrepareData( CStock::dataMinute, CKData::ktypeDay, TRUE );
	m_CurStock.PrepareData( CStock::dataOutline, CKData::ktypeDay, TRUE );

	AfxGetStkReceiver().GetReport( m_CurStock.GetStockInfo(), m_CurStock.GetReport() );

	int	nLen = m_CurStock.GetReport().GetSize();
	if( nLen > 0 )
		UpdateStockInfoByREPORT( m_CurStock.GetStockInfo(), &(m_CurStock.GetReport().ElementAt(nLen-1)) );
	return TRUE;
}

void CRealTime::SetDrawMode( int nDrawMode, int nTechLine, int nReportWhat )
{
	m_nDrawMode		= nDrawMode;
	m_nTechLine		= nTechLine;
	m_nReportWhat	= nReportWhat;

	m_nCurrentStartPos	= -1;
	m_nCurrentPageCount	= -1;
	m_nCurrentSelectPos	= -1;
}

int CRealTime::GetDrawMode( )
{
	return m_nDrawMode;
}

int CRealTime::GetDrawTechLine( )
{
	return m_nTechLine;
}

int CRealTime::GetReportWhat( )
{
	return m_nReportWhat;
}

LRESULT CRealTime::OnStkReceiverData( WPARAM wParam, LPARAM lParam )
{
	PCOMMPACKET	pCommPacket	= (PCOMMPACKET)lParam;

	switch( wParam )
	{
	case CStock::dataReport:
		if( pCommPacket && CStock::dataReport == pCommPacket->m_dwDataType )
		{
			BOOL	bInv = FALSE;
			for(DWORD i=0; i<pCommPacket->m_dwCount; i++)
			{
				if( m_CurStock.GetStockInfo().IsEqualTo( pCommPacket->m_pReport[i].m_dwMarket, pCommPacket->m_pReport[i].m_szCode ) )
				{
					UpdateStockInfoByREPORT( m_CurStock.GetStockInfo(), &(pCommPacket->m_pReport[i]) );
					if( -1 != m_CurStock.GetReport().InsertReportSort( pCommPacket->m_pReport[i] ) )
					{
						MINUTE	minute;
						if( convert_REPORT_to_MINUTE( &(pCommPacket->m_pReport[i]), &minute ) )
							m_CurStock.GetMinute().InsertMinuteSort( minute );
						m_nCurrentStartPos	= -1;
						bInv = TRUE;
					}
				}
			}
			if( bInv && m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()) )
				m_pParent->Invalidate();
		}
		break;
	case CStock::dataMinute:
		if( pCommPacket && CStock::dataMinute == pCommPacket->m_dwDataType )
		{
			int	nOldSize	= m_CurStock.GetMinute().GetSize();
			for(DWORD i=0; i<pCommPacket->m_dwCount; i++)
			{
				if( m_CurStock.GetStockInfo().IsEqualTo( pCommPacket->m_pMinute[i].m_dwMarket, pCommPacket->m_pMinute[i].m_szCode ) )
					m_CurStock.GetMinute().InsertMinuteSort( pCommPacket->m_pMinute[i] );
			}
			if( nOldSize != m_CurStock.GetMinute().GetSize() && m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()) )
			{
				m_nCurrentStartPos	= -1;
				m_pParent->Invalidate();
			}
		}
		break;
	}
	return 0L;
}

void CRealTime::Redraw( CDC * pDC, CRect rectAll )
{
	ASSERT( m_pParent && ::IsWindow( m_pParent->GetSafeHwnd() ) );
	if( !m_pParent || !::IsWindow(m_pParent->GetSafeHwnd()) )
		return;

	CClientDC	dc(m_pParent);
	if( NULL == pDC )
		pDC		= &dc;

	ResetClient( rectAll );
	pDC->FillSolidRect( &rectAll, AfxGetProfile().GetColor(CColorClass::clrGraphBK) );

	if( CRealTime::modePriceLine == m_nDrawMode )
	{
		DrawPriceVolume( pDC );
		DrawLBDK( pDC );
		DrawMMLD( pDC );
		DrawReportRegion( pDC );
			
		DrawSelectionLine( pDC, TRUE );
		OnIndexCurrentChanged( );
	}
	else if( CRealTime::modeReportDetail == m_nDrawMode )
	{
		DrawReportDetail( pDC );
	}
	else if( CRealTime::modeMinuteDetail == m_nDrawMode )
	{
		DrawMinuteDetail( pDC );
	}
	else if( CRealTime::modeBigTradeDetail == m_nDrawMode )
	{
		DrawBigTradeDetail( pDC );
	}
}

void CRealTime::DrawReportRegion( CDC * pDC )
{
	ASSERT( m_pParent && ::IsWindow( m_pParent->GetSafeHwnd() ) );
	if( !m_pParent || !::IsWindow(m_pParent->GetSafeHwnd()) )
		return;

	CClientDC	dc(m_pParent);
	if( NULL == pDC )
		pDC		= &dc;

	if( CRealTime::modePriceLine == m_nDrawMode )
	{
		switch( m_nReportWhat )
		{
		case CRealTime::reportQuote:		DrawReportQuote( pDC );			break;
		case CRealTime::reportPrice:		DrawReportPrice( pDC );			break;
		case CRealTime::reportMinute:		DrawReportMinute( pDC );		break;
		case CRealTime::reportBuySellEx:	DrawReportBuySellEx( pDC );		break;
		case CRealTime::reportValue:		DrawReportValue( pDC );			break;
		case CRealTime::reportDistribute:	DrawReportDistribute( pDC );	break;
		case CRealTime::reportBigTrade:		DrawReportBigTrade( pDC );		break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// 以下为分时价格线使用 modePriceLine

void CRealTime::MoveLeft( BOOL bShiftPressed )
{
	if( CRealTime::modePriceLine != m_nDrawMode )
		return;
	Move( -1, bShiftPressed, FALSE );
}

void CRealTime::MoveRight( BOOL bShiftPressed )
{
	if( CRealTime::modePriceLine != m_nDrawMode )
		return;
	Move( 1, bShiftPressed, FALSE );
}

void CRealTime::MoveUp( BOOL bShiftPressed )
{
	if( CRealTime::modePriceLine == m_nDrawMode )
	{
		int	nTotalSize = 0;
		if( CRealTime::reportQuote == m_nReportWhat )
			nTotalSize	= m_CurStock.GetReport().GetSize();
//		else if( CRealTime::reportPrice == m_nReportWhat )
//			nTotalSize	= m_CurStock.GetMinute().GetSize();
		else if( CRealTime::reportMinute == m_nReportWhat )
			nTotalSize	= m_CurStock.GetMinute().GetSize();
		else if( CRealTime::reportBuySellEx == m_nReportWhat )
			nTotalSize	= m_CurStock.GetReport().GetSize();
		else if( CRealTime::reportBigTrade == m_nReportWhat )
			nTotalSize	= m_aReportBigTrade.GetSize();
		else
			return;

		if( 0 == m_nCurrentStartPos )
			return;
		if( -1 == m_nCurrentStartPos )
		{
			m_nCurrentStartPos	= 0;
			while( nTotalSize - m_nCurrentStartPos > m_nCurrentPageCount )	m_nCurrentStartPos	+= m_nCurrentPageCount;
			m_nCurrentStartPos	-= m_nCurrentPageCount;
		}
		else
			m_nCurrentStartPos	-= m_nCurrentPageCount;
		
		if( m_nCurrentStartPos < 0 || m_nCurrentStartPos >= nTotalSize )
			m_nCurrentStartPos	= -1;

		DrawReportRegion( NULL );
	}
}

void CRealTime::MoveDown( BOOL bShiftPressed )
{
	if( CRealTime::modePriceLine == m_nDrawMode )
	{
		int	nTotalSize = 0;
		if( CRealTime::reportQuote == m_nReportWhat )
			nTotalSize	= m_CurStock.GetReport().GetSize();
//		else if( CRealTime::reportPrice == m_nReportWhat )
//			nTotalSize	= m_CurStock.GetMinute().GetSize();
		else if( CRealTime::reportMinute == m_nReportWhat )
			nTotalSize	= m_CurStock.GetMinute().GetSize();
		else if( CRealTime::reportBuySellEx == m_nReportWhat )
			nTotalSize	= m_CurStock.GetReport().GetSize();
		else if( CRealTime::reportBigTrade == m_nReportWhat )
			nTotalSize	= m_aReportBigTrade.GetSize();
		else
			return;

		if( -1 != m_nCurrentStartPos )
		{
			m_nCurrentStartPos	+= m_nCurrentPageCount;
		}
		
		if( m_nCurrentStartPos < 0 || m_nCurrentStartPos >= nTotalSize )
			m_nCurrentStartPos	= -1;

		DrawReportRegion( NULL );
	}
}

void CRealTime::MoveHome( BOOL bShiftPressed )
{
	if( CRealTime::modePriceLine != m_nDrawMode )
		return;
	int	nPos	= ( m_CurStock.GetMinute().GetSize() > 0 ? 0 : -1 );
	MoveTo( nPos, bShiftPressed, FALSE );
}

void CRealTime::MoveEnd( BOOL bShiftPressed )
{
	if( CRealTime::modePriceLine != m_nDrawMode )
		return;
	int	nPos	= ( m_CurStock.GetMinute().GetSize() > 0 ? m_CurStock.GetMinute().GetSize()-1 : -1 );
	MoveTo( nPos, bShiftPressed, FALSE );
}

void CRealTime::MoveTo( int nIndex, BOOL bShiftPressed, BOOL bCtrlPressed )
{
	if( CRealTime::modePriceLine != m_nDrawMode )
		return;

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

	if( m_nIndexCurrent != nIndex )
	{
		m_nIndexCurrent	= nIndex;
		OnIndexCurrentChanged( );
	}

	DrawSelectionLine( NULL, TRUE );
}

void CRealTime::Move( int nMove, BOOL bShiftPressed, BOOL bCtrlPressed )
{
	if( CRealTime::modePriceLine != m_nDrawMode )
		return;

	int	nIndexCurrent	= m_nIndexCurrent;
	
	if( nIndexCurrent < 0 )
	{
		if( nMove < 0 )
			nIndexCurrent	= m_CurStock.GetMinute().GetSize()-1;
		else if( nMove > 0 )
			nIndexCurrent	= 0;
	}
	else
	{
		nIndexCurrent	+= nMove;
	}

	if( nIndexCurrent < 0 )
		nIndexCurrent	= 0;
	if( nIndexCurrent >= m_CurStock.GetMinute().GetSize() )
		nIndexCurrent	= m_CurStock.GetMinute().GetSize()-1;

	MoveTo( nIndexCurrent, bShiftPressed, bCtrlPressed );
}

void CRealTime::OnLButtonDown( UINT nFlags, CPoint point, BOOL bShiftPressed, BOOL bCtrlPressed )
{
	if( CRealTime::modePriceLine == m_nDrawMode )
	{
		// Selection
		int	nIndexClick	= -1;
		CRect	rect(m_rectPrice.left,m_rectPrice.top,m_rectPrice.right,m_rectAll.bottom-m_nMarginBottom);
		if( rect.PtInRect(point) )
		{
			DWORD	dwSeconds	= CSPTime::GetTradeSecondsOfOneDay();
			for( int nIndex=0; nIndex<m_CurStock.GetMinute().GetSize(); nIndex ++ )
			{
				CRect	rectLine	= rect;
				DWORD sec = CSPTime(m_CurStock.GetMinute()[nIndex].m_time).ToStockTimeSecOrder();
				DWORD secL = sec-100, secR = sec+100;
				if( nIndex > 0 )
					secL = CSPTime(m_CurStock.GetMinute()[nIndex-1].m_time).ToStockTimeSecOrder();
				if( nIndex < m_CurStock.GetMinute().GetSize()-1 )
					secR = CSPTime(m_CurStock.GetMinute()[nIndex+1].m_time).ToStockTimeSecOrder();
				rectLine.left	= (int)( rect.left+rect.Width()*(secL+sec)*0.5/dwSeconds );
				rectLine.right	= (int)( rect.left+rect.Width()*(sec+secR)*0.5/dwSeconds );
				if( rectLine.PtInRect(point) )
				{
					nIndexClick	= nIndex;
					break;
				}
			}
		}
		MoveTo( nIndexClick, bShiftPressed, bCtrlPressed );

		if( CRealTime::modePriceLine == m_nDrawMode
			&& CRealTime::reportBigTrade == m_nReportWhat )
		{
			// reportBigTrade 大单成交，选中股票
			if( m_rectReport.PtInRect(point) )
			{
				int nShowCount = (m_rectReport.Height()-RT_RDTITLE_HEIGHT) / RT_RDELEMENT_HEIGHT;
				if( nShowCount <= 0 )	nShowCount	= 1;

				nIndexClick	= -1;
				for( int nIndex=0; nIndex<nShowCount; nIndex ++ )
				{
					CRect	rectElement	= m_rectReport;
					rectElement.top		= m_rectReport.top+RT_RDTITLE_HEIGHT+RT_RDELEMENT_HEIGHT*nIndex;
					rectElement.bottom	= m_rectReport.top+RT_RDTITLE_HEIGHT+RT_RDELEMENT_HEIGHT*(nIndex+1);
					if( rectElement.PtInRect(point) )
					{
						nIndexClick	= nIndex;
						break;
					}
				}
				// 画选中标记
				int	nSelectPosNew	= nIndexClick+m_nCurrentStartPos;
				if( -1 != nIndexClick && nSelectPosNew >= 0 && nSelectPosNew < m_aReportBigTrade.GetSize() )
				{
					DECLARE_COLOR_DEFINATION

					CClientDC	dc(m_pParent);
					int	y	= m_rectReport.top+RT_RDTITLE_HEIGHT+RT_RDELEMENT_HEIGHT*(nIndexClick+1) - 1;
					if( y > m_rectReport.top && y < m_rectReport.bottom )
						DrawLine( &dc, 2, clrBorder, m_rectReport.left+5, y, m_rectReport.right-5, y );
					if( -1 != m_nCurrentSelectPos 
						&& nSelectPosNew != m_nCurrentSelectPos
						&& m_nCurrentSelectPos-m_nCurrentStartPos >= 0 )
					{
						y	= m_rectReport.top+RT_RDTITLE_HEIGHT+RT_RDELEMENT_HEIGHT*(m_nCurrentSelectPos-m_nCurrentStartPos+1) - 1;
						if( y > m_rectReport.top && y < m_rectReport.bottom )
							DrawLine( &dc, 2, clrBK, m_rectReport.left+5, y, m_rectReport.right-5, y );
					}
					m_nCurrentSelectPos	= nSelectPosNew;
				}
			}
		}
	}
	else if( CRealTime::modeBigTradeDetail == m_nDrawMode )	// 大单成交明细
	{
		int	nColumnPerPage	= GetColumnPerPage( m_rectAll );
		int	nCountPerColumn	= GetCountPerColumn( m_rectAll );
		int	nCountPerPage	= nCountPerColumn * nColumnPerPage;
		if( nCountPerPage <= 0 )
			return;
		
		int	nOldSelectPos	= m_nCurrentSelectPos;
		int	nStartPos		= m_nCurrentStartPos;

		for( int nColumn=0; nColumn<nColumnPerPage; nColumn ++ )
		{
			if( nStartPos >= m_aReportBigTrade.GetSize() )
				break;

			CRect	rectColumn;
			rectColumn.top		= m_rectAll.top + RT_RDTITLE_HEIGHT;
			rectColumn.bottom	= m_rectAll.bottom;
			rectColumn.left		= m_rectAll.left + nColumn*RT_RDCOLUMN_WIDTH;
			rectColumn.right	= rectColumn.left + RT_RDCOLUMN_WIDTH;
			
			// 清除上次选中标记
			if( -1 != nOldSelectPos 
				&& nOldSelectPos >= nStartPos
				&& nOldSelectPos < nStartPos+nCountPerColumn )
			{
				DECLARE_COLOR_DEFINATION

				CClientDC	dc(m_pParent);
				int y	= rectColumn.top+RT_RDTITLE_HEIGHT+RT_RDELEMENT_HEIGHT*(nOldSelectPos-nStartPos+1) - 1;
				if( y > rectColumn.top && y < rectColumn.bottom )
					DrawLine( &dc, 2, clrBK, rectColumn.left+5, y, rectColumn.right-5, y );
			}
			
			if( rectColumn.PtInRect(point) )
			{
				int nIndexClick	= -1;
				for( int nIndex=0; nIndex<nCountPerColumn; nIndex ++ )
				{
					CRect	rectElement	= rectColumn;
					rectElement.top		= rectColumn.top+RT_RDTITLE_HEIGHT+RT_RDELEMENT_HEIGHT*nIndex;
					rectElement.bottom	= rectColumn.top+RT_RDTITLE_HEIGHT+RT_RDELEMENT_HEIGHT*(nIndex+1);
					if( rectElement.PtInRect(point) )
					{
						nIndexClick	= nIndex;
						break;
					}
				}

				// 画选中标记
				int	nSelectPosNew	= nIndexClick+nStartPos;
				if( -1 != nIndexClick && nSelectPosNew >= 0 && nSelectPosNew < m_aReportBigTrade.GetSize() )
				{
					DECLARE_COLOR_DEFINATION

					CClientDC	dc(m_pParent);
					int	y	= rectColumn.top+RT_RDTITLE_HEIGHT+RT_RDELEMENT_HEIGHT*(nIndexClick+1) - 1;
					if( y > rectColumn.top && y < rectColumn.bottom )
						DrawLine( &dc, 2, clrBorder, rectColumn.left+5, y, rectColumn.right-5, y );
					m_nCurrentSelectPos	= nSelectPosNew;
				}
			}
			
			nStartPos	+= nCountPerColumn;
		}
	}
}


void CRealTime::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	OnLButtonDown( nFlags, point, FALSE, FALSE );

	if( CRealTime::modePriceLine == m_nDrawMode
		&& CRealTime::reportBigTrade == m_nReportWhat )
	{
		// reportBigTrade 大单成交，选中股票
		if( m_rectReport.PtInRect(point) )
		{
			if( m_nCurrentSelectPos >= 0 && m_nCurrentSelectPos < m_aReportBigTrade.GetSize() )
			{
				REPORT & report = m_aReportBigTrade.ElementAt(m_nCurrentSelectPos);
				AfxGetProfile().SetCurrentStock( report.m_szCode, FALSE );
				AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_REALTIMEVIEW, NULL );
			}
		}
	}
	else if( CRealTime::modeBigTradeDetail == m_nDrawMode )
	{
		// 大单成交，选中股票
		if( m_nCurrentSelectPos >= 0 && m_nCurrentSelectPos < m_aReportBigTrade.GetSize() )
		{
			REPORT & report = m_aReportBigTrade.ElementAt(m_nCurrentSelectPos);
			AfxGetProfile().SetCurrentStock( report.m_szCode, FALSE );
			SetDrawMode( CRealTime::modePriceLine, GetDrawTechLine(), GetReportWhat() ); 
			AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_REALTIMEVIEW, NULL );
		}
	}
}

void CRealTime::ResetIndexCurrent( int nIndexCurrent )
{
	m_nIndexCurrent	= nIndexCurrent;
}

BOOL CRealTime::AddIndexCurrent( int nIndex )
{
	if( CRealTime::modePriceLine != m_nDrawMode )
		return FALSE;

	for( int k=0; k<m_anIndexCurrent.GetSize(); k++ )
	{
		if( (UINT)nIndex == m_anIndexCurrent[k] )
			return FALSE;
	}
	m_anIndexCurrent.Add( nIndex );
	return TRUE;
}

void CRealTime::OnIndexCurrentChanged( )
{
	if( CRealTime::modePriceLine != m_nDrawMode )
		return;

	DECLARE_COLOR_DEFINATION

	if( !m_pParent || !::IsWindow(m_pParent->GetSafeHwnd()) )
		return;
	
	CClientDC	dc(m_pParent);
	CDC	*pDC	= &dc;

	CRect	rectTop(m_rectPrice.left+135,m_rectAll.top,m_rectPrice.right,m_rectPrice.top-1 );
	pDC->FillSolidRect( &rectTop, clrBK );
	
	if( m_nIndexCurrent < 0 || m_nIndexCurrent >= m_CurStock.GetMinute().GetSize() )
		return;
	
	MINUTE	minute	= m_CurStock.GetMinute().ElementAt(m_nIndexCurrent);

	// Fill Background
	pDC->SetBkColor( clrBK );

	CString	strTemp;
	CFont	* pOldFont	= AfxSelectDCFont( pDC, 12 );

	int	x	= rectTop.left;
	int	y	= rectTop.top + 4;
	int	right	= rectTop.right-100;
	pDC->SetTextColor( clrText );
	strTemp.LoadString( IDS_REALTIME_TIME );
	if( x < right )		pDC->TextOut( x, y,   strTemp );
	strTemp.LoadString( IDS_REALTIME_PRICE );
	if( x+80 < right )	pDC->TextOut( x+80, y,  strTemp );
	strTemp.LoadString( IDS_REALTIME_VOLUME );
	if( x+160 < right )	pDC->TextOut( x+160, y,  strTemp );
	strTemp.LoadString( IDS_REALTIME_AMOUNT );
	if( x+290 < right )	pDC->TextOut( x+290, y,  strTemp );

	// Time
	CSPTime	time(minute.m_time);
	strTemp	= time.Format( "%H:%M" );
	pDC->SetTextColor( clrPlane );
	if( x+30 < right )	pDC->TextOut( x+30, y, strTemp );

	// Price
	CString strPriceFmt;
	strPriceFmt.Format( "%%.%df", m_CurStock.GetStockInfo().DigitBit() );
	strTemp.Format( strPriceFmt, minute.m_fNew );
	pDC->SetTextColor( RT_DETERMINE_COLOR( minute.m_fNew, m_CurStock.GetStockInfo().m_fLast ) );
	if( x+110 < right )	pDC->TextOut( x+110, y, strTemp );
	
	// Volume
	double	dVolume	= minute.m_fVolume;
	if( m_nIndexCurrent > 0 )
		dVolume	-= m_CurStock.GetMinute()[m_nIndexCurrent-1].m_fVolume;
	strTemp.Format( "%.0f", dVolume*0.01 );
	pDC->SetTextColor( clrTitle );
	if( x+220 < right )	pDC->TextOut( x+220, y, strTemp );
	
	// Amount
	double	dAmount	= minute.m_fAmount;
	if( m_nIndexCurrent > 0 )
		dAmount	-= m_CurStock.GetMinute()[m_nIndexCurrent-1].m_fAmount;
	strTemp.Format( "%.2f", dAmount*0.001 );
	pDC->SetTextColor( clrTitle );
	if( x+350 < right )	pDC->TextOut( x+350, y, strTemp );

	pDC->SelectObject( pOldFont );
}

void CRealTime::DrawSelectionLine( CDC * pDC, BOOL bAddOrRemove )
{
	if( CRealTime::modePriceLine != m_nDrawMode )
		return;

	ASSERT( m_pParent && ::IsWindow( m_pParent->GetSafeHwnd() ) );
	if( !m_pParent || !::IsWindow(m_pParent->GetSafeHwnd()) )
		return;

	CClientDC	dc(m_pParent);
	if( NULL == pDC )
		pDC		= &dc;

	CPen	penVLine( PS_SOLID, 1, AfxGetProfile().GetColor( CColorClass::clrLine1 ) );
	CPen * pOldPen = pDC->SelectObject( &penVLine );
	int nOldDrawMode = pDC->SetROP2( R2_XORPEN );

	DWORD	dwSeconds	= CSPTime::GetTradeSecondsOfOneDay();

	if( m_nIndexCurrent >= 0 && m_nIndexCurrent < m_CurStock.GetMinute().GetSize() )
	{
		DWORD sec = CSPTime(m_CurStock.GetMinute()[m_nIndexCurrent].m_time).ToStockTimeSecOrder();
		long	xMedium = m_rectPrice.left+m_rectPrice.Width()*sec/dwSeconds;
		pDC->MoveTo( xMedium, m_rectPrice.top );
		pDC->LineTo( xMedium, m_rectAll.bottom - m_nMarginBottom );
	}

	for( int k=0; k<m_anIndexCurrent.GetSize(); k++ )
	{
		int	nIndex = m_anIndexCurrent[k];
		if( nIndex == m_nIndexCurrent )
			continue;
		if( nIndex >= 0 && nIndex < m_CurStock.GetMinute().GetSize() )
		{
			DWORD sec = CSPTime(m_CurStock.GetMinute()[nIndex].m_time).ToStockTimeSecOrder();
			long	xMedium = m_rectPrice.left+m_rectPrice.Width()*sec/dwSeconds;
			pDC->MoveTo( xMedium, m_rectPrice.top );
			pDC->LineTo( xMedium, m_rectAll.bottom - m_nMarginBottom );
		}
	}

	pDC->SetROP2( nOldDrawMode );
	pDC->SelectObject( pOldPen );
}

void CRealTime::ResetClient( CRect rectAll )
{
	static long lCount = 0;
	TRACE(_T("CRealTime::ResetClient RECT %d %d %d %d %d"), rectAll.left, rectAll.top, rectAll.right, rectAll.bottom, lCount++);

	if( rectAll.IsRectEmpty() )
		return;
	m_rectAll	= rectAll;

	// 买卖信息Rect
	m_rectReport.top	= m_rectAll.top + m_nMarginTop + 7;
	m_rectReport.left	= m_rectAll.right - m_nWidthReport;
	m_rectReport.bottom	= m_rectAll.bottom - m_nMarginBottom;
	m_rectReport.right	= m_rectAll.right;
	if( CRealTime::modePriceLine != m_nDrawMode )
		m_rectReport.SetRectEmpty();
	if( CRealTime::reportNone == m_nReportWhat )
		m_rectReport.SetRectEmpty();

	// 价格Rect
	m_rectPrice.top		= m_rectAll.top + m_nMarginTop;
	m_rectPrice.left	= m_rectAll.left + m_nMarginLeft;
	m_rectPrice.bottom	= m_rectAll.top + (m_rectAll.Height() - m_nMarginTop - m_nMarginBottom)*7/10;
	m_rectPrice.right	= m_rectAll.right - m_rectReport.Width() - m_nMarginCenter;

	// 成交量Rect
	m_rectVolume.top	= m_rectPrice.bottom + 2;
	m_rectVolume.left	= m_rectPrice.left;
	m_rectVolume.bottom	= m_rectAll.bottom - m_nMarginBottom;
	m_rectVolume.right	= m_rectPrice.right;

	// 量比多空指标
	m_rectLBDK.top		= m_rectAll.bottom - m_nMarginBottom;
	m_rectLBDK.left		= m_rectPrice.left;
	m_rectLBDK.bottom	= m_rectAll.bottom - m_nMarginBottom;
	m_rectLBDK.right	= m_rectPrice.right;

	// 买卖力道
	m_rectMMLD.top		= m_rectAll.bottom - m_nMarginBottom;
	m_rectMMLD.left		= m_rectPrice.left;
	m_rectMMLD.bottom	= m_rectAll.bottom - m_nMarginBottom;
	m_rectMMLD.right	= m_rectPrice.right;

	if( CRealTime::techLBDK == m_nTechLine )// 如果显示量比多空
	{
		m_rectPrice.bottom	= m_rectAll.top + (m_rectAll.Height() - m_nMarginTop - m_nMarginBottom)*5/10;
		m_rectVolume.top	= m_rectPrice.bottom + 2;
		m_rectVolume.bottom	= m_rectVolume.top + m_rectVolume.Height()/2;
		m_rectLBDK.top		= m_rectVolume.bottom + 1;
	}
	else if( CRealTime::techMMLD == m_nTechLine )// 如果显示买卖力道
	{
		m_rectPrice.bottom	= m_rectAll.top + (m_rectAll.Height() - m_nMarginTop - m_nMarginBottom)*5/10;
		m_rectVolume.top	= m_rectPrice.bottom + 2;
		m_rectVolume.bottom	= m_rectVolume.top + m_rectVolume.Height()/2;
		m_rectMMLD.top		= m_rectVolume.bottom + 1;
	}
}

void CRealTime::DrawDateAxis( CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	CRect	rect( m_rectAll.left, m_rectAll.bottom-m_nMarginBottom+1, m_rectPrice.right, m_rectAll.bottom );
	pDC->FillSolidRect( &rect, clrBK );
	pDC->SetBkColor( clrBK );
	pDC->SetTextColor( clrDJ );

	CFont	* pOldFont	= AfxSelectDCFont( pDC, 12 );

	UINT nAlignOld = pDC->SetTextAlign( TA_TOP | TA_CENTER );
	rect.left	= m_rectPrice.left;
	int	nStep = rect.Width()/4;
	pDC->TextOut( rect.left, rect.top, "09:30" );
	pDC->TextOut( rect.left+nStep, rect.top, "10:30" );
	pDC->TextOut( rect.left+nStep*2, rect.top, "11:30" );
	pDC->TextOut( rect.left+nStep*3, rect.top, "14:00" );
	pDC->TextOut( rect.right, rect.top, "15:00" );

	pDC->SetTextAlign( nAlignOld );
	pDC->SelectObject( pOldFont );
}

BOOL CRealTime::GetMinMaxInfo( double *pdLastClose, double *pdMin, double *pdMax, double *pdMaxVolume, BOOL bUptoAxis )
{
	double	dLastClose	= 0;
	if( pdLastClose )
		dLastClose	= *pdLastClose;
	double	dMin = dLastClose, dMax = dLastClose, dMaxVolume = 10;
	BOOL	bPriceOK	= FALSE;
	REPORT	report;
	if( m_CurStock.GetReport().GetSize() > 0 )
	{
		report	= m_CurStock.GetReport().ElementAt( m_CurStock.GetReport().GetSize()-1 );
		if( report.m_fNew > 1e-4 && report.m_fHigh > 1e-4 && report.m_fLow > 1e-4 )
		{
			dMin	= report.m_fLow;
			dMax	= report.m_fHigh;
			bPriceOK	= TRUE;
		}
	}

	for( int i=0; i<m_CurStock.GetMinute().GetSize(); i++ )
	{
		MINUTE & m = m_CurStock.GetMinute().ElementAt(i);
		if( !bPriceOK && dMin < 1e-4 )
			dMin	= m.m_fNew;

		if( !bPriceOK && m.m_fNew < dMin )
			dMin	= m.m_fNew;
		if( !bPriceOK && m.m_fNew > dMax )
			dMax	= m.m_fNew;
		
		double	dVolume = (i > 0 ? m_CurStock.GetMinute()[i].m_fVolume-m_CurStock.GetMinute()[i-1].m_fVolume : m_CurStock.GetMinute()[i].m_fVolume);
		if( dVolume > dMaxVolume )
			dMaxVolume	= dVolume;
	}

	if( dLastClose < 1e-4 )
		dLastClose	= (dMin+dMax)/2;

	if( bUptoAxis )
	{
		double dABS = 1.00 * max(fabs(dMax-dLastClose),fabs(dMin-dLastClose));
		if( dABS < dLastClose * 0.001 )
			dABS	= dLastClose * 0.001;

		dMin = dLastClose-dABS;
		dMax = dLastClose+dABS;
	}
	
	if( pdLastClose )
		*pdLastClose	= dLastClose;
	if( pdMin )
		*pdMin	= dMin;
	if( pdMax )
		*pdMax	= dMax;
	if( pdMaxVolume )
		*pdMaxVolume	= dMaxVolume;
	return TRUE;
}

void CRealTime::DrawPriceVolume( CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	// Draw Time Axis String
	DrawDateAxis( pDC );

	// Draw Title
	CString	strTitle;
	pDC->SetTextColor( clrTitle );
	pDC->SetBkColor( clrBK );
	strTitle.LoadString( IDS_REALTIME_TITLE );
	if( CRealTime::modePriceLine != m_nDrawMode )
		strTitle	= m_CurStock.GetStockCode();
	pDC->TextOut( m_rectPrice.left+1, m_rectAll.top+2, strTitle );

	double	dMin = 0, dMax = 0, dMaxVolume = 0;
	double	dLastClose = m_CurStock.GetStockInfo().m_fLast;
	if( ! GetMinMaxInfo( &dLastClose, &dMin, &dMax, &dMaxVolume, TRUE ) )
		return;

	// Draw Price Axis
	DrawPriceVolumeAxis(	pDC, m_rectPrice, m_rectVolume, m_nGapYAxis,
							dLastClose, &dMin, &dMax, dMaxVolume, m_CurStock.GetStockInfo().DigitBit() );
	m_dMaxPrice	= dMax;
	m_dMinPrice	= dMin;

	// Draw Price and Volume Line
	CStockInfo & info	= m_CurStock.GetStockInfo();
	CMinute	& aMinute	= m_CurStock.GetMinute();
	DWORD	secNow	= CSPTime::GetCurrentTime().ToStockTimeSecOrder();
	DWORD	secStep = 60;
	double	dPriceMA= dLastClose;
	int	ptr = 0;
	int	xPos = m_rectPrice.left;
	int	yPosMid = m_rectPrice.top+m_rectPrice.Height()/2;
	int yPos = yPosMid;
	int	yPosMA = yPos;
	int	yPosLast = yPos;
	COLORREF	clrLine1	= AfxGetProfile().GetColor(CColorClass::clrLine1);
	COLORREF	clrLine2	= AfxGetProfile().GetColor(CColorClass::clrLine3);
	COLORREF	clrWave		= clrRise;
	COLORREF	clrVolume	= clrRise;

	DWORD	dwSeconds	= CSPTime::GetTradeSecondsOfOneDay();
	for( DWORD sec=0; sec<=dwSeconds; sec+=secStep )
	{
		BOOL bDrawed = FALSE;
		for( ; ptr<aMinute.GetSize(); ptr++ )
		{
			DWORD secTemp = CSPTime(aMinute[ptr].m_time).ToStockTimeSecOrder();
			if( secTemp > sec )
				break;
			if( (long)secTemp <= (long)sec-(long)secStep )
				continue;
			if( aMinute[ptr].m_fNew < 1e-4 )
				aMinute[ptr].m_fNew	= m_CurStock.GetStockInfo().m_fLast;

			int	xPosNew	= m_rectPrice.left+m_rectPrice.Width()*secTemp/dwSeconds;
			yPosLast	= yPos;

			// Draw Price Line
			CPen	pen1( PS_SOLID, 1, clrLine1 );
			CPen * pOldPen = pDC->SelectObject( &pen1 );
			pDC->MoveTo( xPos, yPos );
			double	dPriceNew	= aMinute[ptr].m_fNew;
			if( dPriceNew <= dMax && dPriceNew >= dMin && dMax-dMin > 1e-4 )
			{
				yPos	= (int)( m_rectPrice.top+m_rectPrice.Height()*(dMax-dPriceNew)/(dMax-dMin) );
				if( yPos <= m_rectPrice.top )		yPos = m_rectPrice.top+1;
				if( yPos >= m_rectPrice.bottom )	yPos = m_rectPrice.bottom-1;
				pDC->LineTo( xPosNew, yPos );
			}

			// Draw Price MA Lime
			CPen	pen2( PS_SOLID, 1, clrLine2 );
			pDC->SelectObject( &pen2 );
			pDC->MoveTo( xPos, yPosMA );
			dPriceMA	= (dPriceNew + ptr*dPriceMA)/(ptr+1);
			if( dPriceMA <= dMax && dPriceMA >= dMin
				&& dPriceNew <= dMax && dPriceNew >= dMin && dMax-dMin > 1e-4 )
			{
				yPosMA	= (int)( m_rectPrice.top+m_rectPrice.Height()*(dMax-dPriceMA)/(dMax-dMin) );
				if( yPosMA <= m_rectPrice.top )		yPosMA = m_rectPrice.top+1;
				if( yPosMA >= m_rectPrice.bottom )	yPosMA = m_rectPrice.bottom-1;
				pDC->LineTo( xPosNew, yPosMA );
			}

			// Draw Wave Line
			double	dWave = 0;
			if( info.IsIndex() && aMinute.GetIndexWave( &dWave, ptr ) )
			{
				if( dWave > 0 )	clrWave	= clrRise;
				if( dWave < 0 )	clrWave	= clrFall;
				CPen	pen3( PS_SOLID, 1, clrWave );
				pDC->SelectObject( &pen3 );
				pDC->MoveTo( xPosNew, yPosMid );
				if( fabs(dWave) < (dMax-dMin)*0.5
					&& dPriceNew <= dMax && dPriceNew >= dMin && dMax-dMin > 1e-4)
					pDC->LineTo( xPosNew, yPosMid + m_rectPrice.Height() * 0.5 * dWave /(dMax-dMin) );
			}

			// Draw Volume Line
			if( yPos < yPosLast )	clrVolume	= clrRise;
			if( yPos > yPosLast )	clrVolume	= clrFall;
			CPen	pen4( PS_SOLID, 1, clrVolume );
			pDC->SelectObject( &pen4 );
			pDC->MoveTo( xPosNew, m_rectVolume.bottom-1 );
			double	dVolume = 0;
			if( ptr > 0 )
				dVolume	= aMinute[ptr].m_fVolume-aMinute[ptr-1].m_fVolume;
			else if( sec <= secStep )
				dVolume	= aMinute[ptr].m_fVolume;
			if( dVolume >= 0 && dVolume <= dMaxVolume && dMaxVolume > 1e-4
				&& dPriceNew <= dMax && dPriceNew >= dMin && dMax-dMin > 1e-4)
				pDC->LineTo( xPosNew, (int)( m_rectVolume.bottom-m_rectVolume.Height()*dVolume/dMaxVolume ) );

			pDC->SelectObject( pOldPen );
			xPos	= xPosNew;
			bDrawed	= TRUE;
		}

		if( !bDrawed && sec <= secNow )
		{
			int	xPosNew	= m_rectPrice.left+m_rectPrice.Width()*sec/dwSeconds;

			// Draw Price Line
			CPen	pen1( PS_SOLID, 1, clrLine1 );
			CPen * pOldPen = pDC->SelectObject( &pen1 );
			pDC->MoveTo( xPos, yPos );
			pDC->LineTo( xPosNew, yPos );
			
			// Draw Price MA Line
			CPen	pen2( PS_SOLID, 1, clrLine2 );
			pDC->SelectObject( &pen2 );
			pDC->MoveTo( xPos, yPosMA );
			pDC->LineTo( xPosNew, yPosMA );

			pDC->SelectObject( pOldPen );
			xPos	= xPosNew;
		}
	}
}

// 量比（多空）指标
void CRealTime::DrawLBDK( CDC * pDC )
{
	if( CRealTime::techLBDK != m_nTechLine )
		return;

	DECLARE_COLOR_DEFINATION

	// Draw Rect
	pDC->FillSolidRect( &m_rectLBDK, clrBK );
	pDC->Draw3dRect( &m_rectLBDK, clrBorder, clrBorder );

	// Draw Title
	CString	strLBDK;
	strLBDK.LoadString( IDS_REALTIME_LBDK );
	DrawTechTitle( pDC, m_rectLBDK.left+5, m_rectLBDK.top+1, strLBDK, TA_LEFT | TA_TOP, 14, clrBK, clrTitle );

	// Prepare Calculate
	CStockInfo & info	= m_CurStock.GetStockInfo();
	CMinute	& aMinute	= m_CurStock.GetMinute();

	double	dVolRatio = 0;
	if( !info.GetRatioVolume( &dVolRatio, info.m_datetech, 5 ) || dVolRatio < 1e-4 )
		return;
	double	dVolAverage	= (info.m_fVolume / dVolRatio);

	// Draw Axis
	double	dMin = 0, dMax = 0;
	if( !aMinute.GetLBDKMinMaxInfo( dVolAverage, &dMin, &dMax )
		|| !DrawAxis( pDC, m_rectLBDK, 0, 5, dMin, dMax, FALSE, FALSE, 2 ) )
		return;

	// Draw
	DWORD	secNow	= CSPTime::GetCurrentTime().ToStockTimeSecOrder();
	DWORD	secStep = 60;
	int	ptr = 0;
	int	xPos = m_rectLBDK.left;
	int	yPos = m_rectLBDK.top+m_rectLBDK.Height()/2;
	COLORREF	clrLine1	= AfxGetProfile().GetColor(CColorClass::clrLine1);

	DWORD	dwSeconds	= CSPTime::GetTradeSecondsOfOneDay();
	for( DWORD sec=0; sec<=dwSeconds; sec+=secStep )
	{
		BOOL bDrawed = FALSE;
		for( ; ptr<aMinute.GetSize(); ptr++ )
		{
			DWORD secTemp = CSPTime(aMinute[ptr].m_time).ToStockTimeSecOrder();
			if( secTemp > sec )
				break;
			if( (long)secTemp <= (long)sec-(long)secStep )
				continue;

			int	xPosNew	= m_rectLBDK.left+m_rectLBDK.Width()*secTemp/dwSeconds;

			// Draw LBDK Line
			CPen	pen1( PS_SOLID, 1, clrLine1 );
			CPen * pOldPen = pDC->SelectObject( &pen1 );
			pDC->MoveTo( xPos, yPos );
			double	dVolRatioNew	= aMinute[ptr].m_fVolume/dVolAverage;
			if( secTemp > 0 )
				dVolRatioNew	= dVolRatioNew * dwSeconds / secTemp;
			if( dVolRatioNew <= dMax && dVolRatioNew >= dMin && dMax-dMin > 1e-4)
			{
				yPos	= (int)( m_rectLBDK.top+m_rectLBDK.Height()*(dMax-dVolRatioNew)/(dMax-dMin) );
				if( yPos <= m_rectLBDK.top )		yPos = m_rectLBDK.top+1;
				if( yPos >= m_rectLBDK.bottom )		yPos = m_rectLBDK.bottom-1;
				pDC->LineTo( xPosNew, yPos );
			}
			
			pDC->SelectObject( pOldPen );
			xPos	= xPosNew;
			bDrawed	= TRUE;
		}

		if( !bDrawed && sec <= secNow )
		{
			int	xPosNew	= m_rectLBDK.left+m_rectLBDK.Width()*sec/dwSeconds;

			// Draw LBDK Line
			CPen	pen1( PS_SOLID, 1, clrLine1 );
			CPen * pOldPen = pDC->SelectObject( &pen1 );
			pDC->MoveTo( xPos, yPos );
			pDC->LineTo( xPosNew, yPos );
			
			pDC->SelectObject( pOldPen );
			xPos	= xPosNew;
		}
	}
}

// 买卖力道指标
void CRealTime::DrawMMLD( CDC * pDC )
{
	if( CRealTime::techMMLD != m_nTechLine )
		return;

	DECLARE_COLOR_DEFINATION

	// Draw Rect
	pDC->FillSolidRect( &m_rectMMLD, clrBK );
	pDC->Draw3dRect( &m_rectMMLD, clrBorder, clrBorder );

	// Draw Title
	CString	strMMLD;
	strMMLD.LoadString( IDS_REALTIME_MMLD );
	DrawTechTitle( pDC, m_rectMMLD.left+5, m_rectMMLD.top+1, strMMLD, TA_LEFT | TA_TOP, 14, clrBK, clrTitle );

	// Draw
	CStockInfo & info	= m_CurStock.GetStockInfo();
	CReport	& aReport	= m_CurStock.GetReport();
	
	// Draw Axis
	double	dMin = 0, dMax = 0;
	if( !aReport.GetMMLDMinMaxInfo( &dMin, &dMax )
		|| !DrawAxis( pDC, m_rectMMLD, 0, 5, dMin*0.01, dMax*0.01, FALSE, FALSE, 0 ) )
		return;

	// Draw
	DWORD	secNow	= CSPTime::GetCurrentTime().ToStockTimeSecOrder();
	DWORD	secStep = 60;
	int	ptr = 0;
	int	xPos = m_rectMMLD.left;
	int	yPosBuy = m_rectMMLD.top+m_rectMMLD.Height()/2;
	int yPosSell = yPosBuy, yPosDiff = yPosBuy;
	COLORREF	clrLine1	= AfxGetProfile().GetColor(CColorClass::clrLine1);
	COLORREF	clrLine2	= AfxGetProfile().GetColor(CColorClass::clrLine3);

	DWORD	dwSeconds	= CSPTime::GetTradeSecondsOfOneDay();
	for( DWORD sec=0; sec<=dwSeconds; sec+=secStep )
	{
		BOOL bDrawed = FALSE;
		for( ; ptr<aReport.GetSize(); ptr++ )
		{
			DWORD secTemp = CSPTime(aReport[ptr].m_time).ToStockTimeSecOrder();
			if( secTemp > sec )
				break;
			if( (long)secTemp <= (long)sec-(long)secStep )
				continue;

			int	xPosNew	= m_rectMMLD.left+m_rectMMLD.Width()*secTemp/dwSeconds;

			double	dVolBuy = 0, dVolSell = 0, dVolDiff = 0;
			if( !aReport.GetMMLD( ptr, &dVolBuy, &dVolSell, &dVolDiff ) )
				break;
			
			// Draw Buy Line
			CPen	pen1( PS_SOLID, 1, clrLine1 );
			CPen * pOldPen = pDC->SelectObject( &pen1 );
			pDC->MoveTo( xPos, yPosBuy );
			if( dVolBuy <= dMax && dVolBuy >= dMin && dMax-dMin > 1e-4)
			{
				yPosBuy	= (int)( m_rectMMLD.top+m_rectMMLD.Height()*(dMax-dVolBuy)/(dMax-dMin) );
				if( yPosBuy <= m_rectMMLD.top )			yPosBuy	= m_rectMMLD.top+1;
				if( yPosBuy >= m_rectMMLD.bottom )		yPosBuy	= m_rectMMLD.bottom-1;
				pDC->LineTo( xPosNew, yPosBuy );
			}
			
			// Draw Sell Line
			CPen	pen2( PS_SOLID, 1, clrLine2 );
			pDC->SelectObject( &pen2 );
			pDC->MoveTo( xPos, yPosSell );
			if( dVolSell <= dMax && dVolSell >= dMin )
			{
				yPosSell	= (int)( m_rectMMLD.top+m_rectMMLD.Height()*(dMax-dVolSell)/(dMax-dMin) );
				if( yPosSell <= m_rectMMLD.top )		yPosSell	= m_rectMMLD.top+1;
				if( yPosSell >= m_rectMMLD.bottom )		yPosSell	= m_rectMMLD.bottom-1;
				pDC->LineTo( xPosNew, yPosSell );
			}

			// Draw Diff Line
			CPen	pen3( PS_SOLID, 1, dVolDiff > 0 ? clrRise : clrFall );
			pDC->SelectObject( &pen3 );
			pDC->MoveTo( xPosNew, m_rectMMLD.top+m_rectMMLD.Height()/2 );
			if( dVolDiff*0.5 <= dMax && dVolDiff*0.5 >= dMin && dMax-dMin > 1e-4 )
			{
				yPosDiff	= (int)( m_rectMMLD.bottom-m_rectMMLD.Height()*(dVolDiff*0.5+dMax-dMin)*0.5/(dMax-dMin) );
				if( yPosDiff <= m_rectMMLD.top )		yPosDiff	= m_rectMMLD.top+1;
				if( yPosDiff >= m_rectMMLD.bottom )		yPosDiff	= m_rectMMLD.bottom-1;
				pDC->LineTo( xPosNew, yPosDiff );
			}

			pDC->SelectObject( pOldPen );
			xPos	= xPosNew;
			bDrawed	= TRUE;
		}

		if( !bDrawed && sec <= secNow )
		{
			int	xPosNew	= m_rectMMLD.left+m_rectMMLD.Width()*sec/dwSeconds;

			// Draw Buy Line
			CPen	pen1( PS_SOLID, 1, clrLine1 );
			CPen * pOldPen = pDC->SelectObject( &pen1 );
			pDC->MoveTo( xPos, yPosBuy );
			pDC->LineTo( xPosNew, yPosBuy );
			
			// Draw Sell Line
			CPen	pen2( PS_SOLID, 1, clrLine2 );
			pDC->SelectObject( &pen2 );
			pDC->MoveTo( xPos, yPosSell );
			pDC->LineTo( xPosNew, yPosSell );

			// Draw Diff Line
			CPen	pen3( PS_SOLID, 1, yPosDiff > m_rectMMLD.top+m_rectMMLD.Height() ? clrRise : clrFall );
			pDC->SelectObject( &pen3 );
			pDC->MoveTo( xPosNew, m_rectMMLD.top+m_rectMMLD.Height()/2 );
			pDC->LineTo( xPosNew, yPosDiff );

			pDC->SelectObject( pOldPen );
			xPos	= xPosNew;
		}
	}
}

int CRealTime::DrawBuySell( CDC * pDC, int xStart, int yStart, int nWidth )
{
	DECLARE_COLOR_DEFINATION

	CStockInfo & info	= m_CurStock.GetStockInfo();
	CString	strTemp, strTempFmt;

	// Draw Name And Code
	CFont	* pOldFont	= AfxSelectDCFont( pDC, 18 );
	pDC->SetTextColor( clrTitle );
	pDC->TextOut( xStart+1, m_rectAll.top+3, info.GetStockName() );
	pDC->SetTextColor( clrText );
	pDC->TextOut( xStart+96, m_rectAll.top+3, info.GetStockCode() );

	// Select Report Font
	AfxSelectDCFont( pDC, 14 );
	
	// Draw Report Names
	int	x	= xStart+2;
	int	x2	= xStart+2+nWidth/2;
	int	y	= yStart;
	pDC->SetTextColor( clrText );
	
	// 委比和委差
	strTemp.LoadString( IDS_REALTIME_BSRATIO );	// "委比";
	pDC->TextOut( x, y+2,   strTemp );
	strTemp.LoadString( IDS_REALTIME_BSDIFF );	// "委差";
	pDC->TextOut( x2, y+2,   strTemp );

	// 委卖数
	DrawLine( pDC, 1, clrBorder, xStart, y+19, xStart+nWidth, y+19 );
	strTemp.LoadString( IDS_REALTIME_SELL5 );	// "卖⑤";
	pDC->TextOut( x, y+21, strTemp );
	strTemp.LoadString( IDS_REALTIME_SELL4 );	// "卖④";
	pDC->TextOut( x, y+37, strTemp );
	strTemp.LoadString( IDS_REALTIME_SELL3 );	// "卖③";
	pDC->TextOut( x, y+53, strTemp );
	strTemp.LoadString( IDS_REALTIME_SELL2 );	// "卖②";
	pDC->TextOut( x, y+69, strTemp );
	strTemp.LoadString( IDS_REALTIME_SELL1 );	// "卖①";
	pDC->TextOut( x, y+85, strTemp );

	// 委买数
	DrawLine( pDC, 1, clrBorder, xStart, y+102, xStart+nWidth, y+102 );
	strTemp.LoadString( IDS_REALTIME_BUY1 );	// "买①";
	pDC->TextOut( x, y+104, strTemp );
	strTemp.LoadString( IDS_REALTIME_BUY2 );	// "买②";
	pDC->TextOut( x, y+120, strTemp );
	strTemp.LoadString( IDS_REALTIME_BUY3 );	// "买③";
	pDC->TextOut( x, y+136, strTemp );
	strTemp.LoadString( IDS_REALTIME_BUY4 );	// "买④";
	pDC->TextOut( x, y+152, strTemp );
	strTemp.LoadString( IDS_REALTIME_BUY5 );	// "买⑤";
	pDC->TextOut( x, y+168, strTemp );

	// 其他信息报告
	DrawLine( pDC, 1, clrBorder, xStart, y+185, xStart+nWidth, y+185 );
	strTemp.LoadString( IDS_REALTIME_PRICENOW );	// "成交";
	pDC->TextOut( x, y+187, strTemp );
	strTemp.LoadString( IDS_REALTIME_PRICEAVE );	// "均价";
	pDC->TextOut( x2, y+187, strTemp );
	strTemp.LoadString( IDS_REALTIME_PRICEDIFF );	// "涨跌";
	pDC->TextOut( x, y+203, strTemp );
	strTemp.LoadString( IDS_REALTIME_PRICEOPEN );	// "开盘";
	pDC->TextOut( x2, y+203, strTemp );
	strTemp.LoadString( IDS_REALTIME_PRICEDIFFPERCENT );	// "幅度";
	pDC->TextOut( x, y+219, strTemp );
	strTemp.LoadString( IDS_REALTIME_PRICEHIGH );	// "最高";
	pDC->TextOut( x2, y+219, strTemp );
	strTemp.LoadString( IDS_REALTIME_VOLSUM );		// "总手";
	pDC->TextOut( x, y+235, strTemp );
	strTemp.LoadString( IDS_REALTIME_PRICELOW );	// "最低";
	pDC->TextOut( x2, y+235, strTemp );
	strTemp.LoadString( IDS_REALTIME_VOLNOW );		// "现手";
	pDC->TextOut( x, y+251, strTemp );
	strTemp.LoadString( IDS_REALTIME_VOLRATION );	// "量比";
	pDC->TextOut( x2, y+251, strTemp );

	// 内外盘
	DrawLine( pDC, 1, clrBorder, xStart, y+268, xStart+nWidth, y+268 );
	strTemp.LoadString( IDS_REALTIME_VOLOUTER );	// "外盘";
	pDC->TextOut( x, y+270, strTemp );
	strTemp.LoadString( IDS_REALTIME_VOLINNER );	// "内盘";
	pDC->TextOut( x2, y+270, strTemp );

	DrawLine( pDC, 1, clrBorder, xStart, y+287, xStart+nWidth, y+287 );	

	// 涨家数，跌家数
	if( info.IsIndex() )
	{
		strTemp.LoadString( IDS_REALTIME_ADVANCE );	// "涨家数";
		pDC->TextOut( x, y+289, strTemp );
		strTemp.LoadString( IDS_REALTIME_DECLINE );	// "跌家数";
		pDC->TextOut( x2, y+289, strTemp );
		DrawLine( pDC, 1, clrBorder, xStart, y+306, xStart+nWidth, y+306 );
	}
	

	// Draw Report Values
	x	= xStart-1+nWidth/2;
	x2	= xStart+nWidth-1;
	UINT nOldAlign = pDC->SetTextAlign( TA_RIGHT | TA_TOP );
	pDC->SetTextColor( clrPlane );
	

	// 价格小数位数格式串
	CString strPriceFmt;
	strPriceFmt.Format( "%%.%df", info.DigitBit() );

	if( info.IsIndex() )
	{
		// 委比和委差
		double	dRatio = 0, dDiff = 0;
		strTemp.Format( "%.2f%%", dRatio );
		pDC->TextOut( x, y+2,   strTemp );
		strTemp.Format( "%.0f", dDiff*0.01 );
		pDC->TextOut( x2, y+2,   strTemp );

		// 委卖数
		pDC->SetTextColor( clrText );
		strTemp.Format( strPriceFmt, 0.0f );
		pDC->TextOut( x, y+21, strTemp );
		strTemp.Format( "%.0f", 0.0f );
		pDC->TextOut( x2, y+21, strTemp );

		strTemp.Format( strPriceFmt, 0.0f );
		pDC->TextOut( x, y+37, strTemp );
		strTemp.Format( "%.0f", 0.0f );
		pDC->TextOut( x2, y+37, strTemp );

		strTemp.Format( strPriceFmt, 0.0f );
		pDC->TextOut( x, y+53, strTemp );
		strTemp.Format( "%.0f", 0.0f );
		pDC->TextOut( x2, y+53, strTemp );

		strTemp.Format( strPriceFmt, 0.0f );
		pDC->TextOut( x, y+69, strTemp );
		strTemp.Format( "%.0f", 0.0f );
		pDC->TextOut( x2, y+69, strTemp );

		strTemp.Format( strPriceFmt, 0.0f );
		pDC->TextOut( x, y+85, strTemp );
		strTemp.Format( "%.0f", 0.0f );
		pDC->TextOut( x2, y+85, strTemp );

		// 委买数
		strTemp.Format( strPriceFmt, 0.0f );
		pDC->TextOut( x, y+104, strTemp );
		strTemp.Format( "%.0f", 0.0f );
		pDC->TextOut( x2, y+104, strTemp );

		strTemp.Format( strPriceFmt, 0.0f );
		pDC->TextOut( x, y+120, strTemp );
		strTemp.Format( "%.0f", 0.0f );
		pDC->TextOut( x2, y+120, strTemp );
		
		strTemp.Format( strPriceFmt, 0.0f );
		pDC->TextOut( x, y+136, strTemp );
		strTemp.Format( "%.0f", 0.0f );
		pDC->TextOut( x2, y+136, strTemp );

		strTemp.Format( strPriceFmt, 0.0f );
		pDC->TextOut( x, y+152, strTemp );
		strTemp.Format( "%.0f", 0.0f );
		pDC->TextOut( x2, y+152, strTemp );

		strTemp.Format( strPriceFmt, 0.0f );
		pDC->TextOut( x, y+168, strTemp );
		strTemp.Format( "%.0f", 0.0f );
		pDC->TextOut( x2, y+168, strTemp );
	}
	else
	{
		// 委比和委差
		double	dRatio = 0, dDiff = 0;
		if( info.GetSellBuyRatio( &dRatio, &dDiff ) )
		{
			strTemp.Format( "%.2f%%", dRatio );
			pDC->TextOut( x, y+2,   strTemp );
			strTemp.Format( "%.0f", dDiff*0.01 );
			pDC->TextOut( x2, y+2,   strTemp );
		}

		// 委卖数
		pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fSellPrice[4],info.m_fLast) );
		strTemp.Format( strPriceFmt, info.m_fSellPrice[4] );
		pDC->TextOut( x, y+21, strTemp );
		strTemp.Format( "%.0f", info.m_fSellVolume[4]/100 );
		pDC->TextOut( x2, y+21, strTemp );

		pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fSellPrice[3],info.m_fLast) );
		strTemp.Format( strPriceFmt, info.m_fSellPrice[3] );
		pDC->TextOut( x, y+37, strTemp );
		strTemp.Format( "%.0f", info.m_fSellVolume[3]/100 );
		pDC->TextOut( x2, y+37, strTemp );

		pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fSellPrice[2],info.m_fLast) );
		strTemp.Format( strPriceFmt, info.m_fSellPrice[2] );
		pDC->TextOut( x, y+53, strTemp );
		strTemp.Format( "%.0f", info.m_fSellVolume[2]/100 );
		pDC->TextOut( x2, y+53, strTemp );

		pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fSellPrice[1],info.m_fLast) );
		strTemp.Format( strPriceFmt, info.m_fSellPrice[1] );
		pDC->TextOut( x, y+69, strTemp );
		strTemp.Format( "%.0f", info.m_fSellVolume[1]/100 );
		pDC->TextOut( x2, y+69, strTemp );

		pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fSellPrice[0],info.m_fLast) );
		strTemp.Format( strPriceFmt, info.m_fSellPrice[0] );
		pDC->TextOut( x, y+85, strTemp );
		strTemp.Format( "%.0f", info.m_fSellVolume[0]/100 );
		pDC->TextOut( x2, y+85, strTemp );

		// 委买数
		pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fBuyPrice[0],info.m_fLast) );
		strTemp.Format( strPriceFmt, info.m_fBuyPrice[0] );
		pDC->TextOut( x, y+104, strTemp );
		strTemp.Format( "%.0f", info.m_fBuyVolume[0]/100 );
		pDC->TextOut( x2, y+104, strTemp );

		pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fBuyPrice[1],info.m_fLast) );
		strTemp.Format( strPriceFmt, info.m_fBuyPrice[1] );
		pDC->TextOut( x, y+120, strTemp );
		strTemp.Format( "%.0f", info.m_fBuyVolume[1]/100 );
		pDC->TextOut( x2, y+120, strTemp );
		
		pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fBuyPrice[2],info.m_fLast) );
		strTemp.Format( strPriceFmt, info.m_fBuyPrice[2] );
		pDC->TextOut( x, y+136, strTemp );
		strTemp.Format( "%.0f", info.m_fBuyVolume[2]/100 );
		pDC->TextOut( x2, y+136, strTemp );

		pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fBuyPrice[3],info.m_fLast) );
		strTemp.Format( strPriceFmt, info.m_fBuyPrice[3] );
		pDC->TextOut( x, y+152, strTemp );
		strTemp.Format( "%.0f", info.m_fBuyVolume[3]/100 );
		pDC->TextOut( x2, y+152, strTemp );

		pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fBuyPrice[4],info.m_fLast) );
		strTemp.Format( strPriceFmt, info.m_fBuyPrice[4] );
		pDC->TextOut( x, y+168, strTemp );
		strTemp.Format( "%.0f", info.m_fBuyVolume[4]/100 );
		pDC->TextOut( x2, y+168, strTemp );
	}

	// 其他信息报告
	// 成交
	pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fClose,info.m_fLast) );
	strTemp.Format( strPriceFmt, info.m_fClose );
	pDC->TextOut( x, y+187, strTemp );
	
	// 均价
	pDC->SetTextColor( clrPlane );
	strTemp	= "-";
	if( info.m_fVolume > 0 )
	{
		double dAve	= info.m_fAmount/info.m_fVolume;
		pDC->SetTextColor( RT_DETERMINE_COLOR(dAve,info.m_fLast) );
		strTemp.Format( strPriceFmt, dAve );
	}
	pDC->TextOut( x2, y+187, strTemp );
	
	// 涨跌
	pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fClose,info.m_fLast) );
	strTemp.Format( strPriceFmt, info.m_fClose-info.m_fLast );
	pDC->TextOut( x, y+203, strTemp );

	// 开盘
	pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fOpen,info.m_fLast) );
	strTemp.Format( strPriceFmt, info.m_fOpen );
	pDC->TextOut( x2, y+203, strTemp );
	
	// 幅度
	pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fClose,info.m_fLast) );
	strTemp	= "-";
	if( info.m_fLast > 1e-4 )
		strTemp.Format( "%.2f%%", 100*(info.m_fClose-info.m_fLast)/info.m_fLast );
	pDC->TextOut( x, y+219, strTemp );
	
	// 最高
	pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fHigh,info.m_fLast) );
	strTemp.Format( strPriceFmt, info.m_fHigh );
	pDC->TextOut( x2, y+219, strTemp );
	
	// 总手
	pDC->SetTextColor( clrPlane );
	strTemp.Format( "%.0f", info.m_fVolume/100. );
	pDC->TextOut( x, y+235, strTemp );
	
	// 最低
	pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fLow,info.m_fLast) );
	strTemp.Format( strPriceFmt, info.m_fLow );
	pDC->TextOut( x2, y+235, strTemp );
	
	// 现手
	pDC->SetTextColor( clrPlane );
	strTemp	= "-";
	double	dVolNow = 0, dVolOuter = 0, dVolInner = 0;
	BOOL bStatSuc = m_CurStock.GetMinute().StatVolumeInfo( &dVolNow, &dVolOuter, &dVolInner );
	if( bStatSuc )
		strTemp.Format( "%.0f", dVolNow/100. );
	pDC->TextOut( x, y+251, strTemp );
	
	// 量比
	pDC->SetTextColor( clrPlane );
	double	dVolRatio = 0;
	strTemp	= "-";
	if( info.GetRatioVolume( &dVolRatio, info.m_datetech, AfxGetProfile().GetRatioVolumeDays() ) )
	{
		CSPTime	tTrade;
		if( tTrade.FromStockTimeDay( info.m_datetech ) )
			dVolRatio = dVolRatio / CSPTime::GetTimeTradeRatioOfOneDay( tTrade, CSPTime::GetCurrentTime() );
		strTemp.Format( "%.2f", dVolRatio );
	}
	pDC->TextOut( x2, y+251, strTemp );

	// 外盘
	pDC->SetTextColor( clrRise );
	strTemp	= "-";
	if( bStatSuc )
		strTemp.Format( "%.0f", dVolOuter/100 );
	pDC->TextOut( x, y+270, strTemp );
	
	// 内盘
	pDC->SetTextColor( clrFall );
	strTemp	= "-";
	if( bStatSuc )
		strTemp.Format( "%.0f", dVolInner/100 );
	pDC->TextOut( x2, y+270, strTemp );

	// 涨家数，跌家数
	int	nRetValue = 287;
	if( info.IsIndex() )
	{
		pDC->SetTextColor( clrRise );
		strTemp	= "-";
		if( info.m_dwAdvance > 0 )
			strTemp.Format( "%uf", info.m_dwAdvance );
		pDC->TextOut( x, y+289, strTemp );

		pDC->SetTextColor( clrFall );
		strTemp	= "-";
		if( info.m_dwDecline > 0 )
			strTemp.Format( "%uf", info.m_dwDecline );
		pDC->TextOut( x2, y+289, strTemp );
		
		nRetValue = 306;
	}

	pDC->SetTextAlign( nOldAlign );
	pDC->SelectObject( pOldFont );

	return nRetValue;
}

int CRealTime::DrawBuySellEx( CDC * pDC, int xStart, int yStart, int nWidth )
{
	DECLARE_COLOR_DEFINATION

	CStockInfo & info	= m_CurStock.GetStockInfo();
	CString	strTemp, strTempFmt;

	// Draw Name And Code
	CFont	* pOldFont	= AfxSelectDCFont( pDC, 18 );
	pDC->SetTextColor( clrTitle );
	pDC->TextOut( xStart+1, m_rectAll.top+3, info.GetStockName() );
	pDC->SetTextColor( clrText );
	pDC->TextOut( xStart+96, m_rectAll.top+3, info.GetStockCode() );

	// Select Report Font
	AfxSelectDCFont( pDC, 14 );
	
	// Draw Report Names
	int	x	= xStart+2;
	int	x2	= xStart+2+nWidth/2;
	int	y	= yStart;
	pDC->SetTextColor( clrText );
	
	// 委比和委差
	strTemp.LoadString( IDS_REALTIME_BSRATIO );	// "委比";
	pDC->TextOut( x, y+2,   strTemp );
	strTemp.LoadString( IDS_REALTIME_BSDIFF );	// "委差";
	pDC->TextOut( x2, y+2,   strTemp );

	// 委卖数
	DrawLine( pDC, 1, clrBorder, xStart, y+19, xStart+nWidth, y+19 );
	pDC->SetTextColor( clrNewKLine );
	strTemp.LoadString( IDS_REALTIME_SELL8 );	// "卖⑧";
	pDC->TextOut( x, y+21, strTemp );
	strTemp.LoadString( IDS_REALTIME_SELL7 );	// "卖⑦";
	pDC->TextOut( x, y+37, strTemp );
	strTemp.LoadString( IDS_REALTIME_SELL6 );	// "卖⑥";
	pDC->TextOut( x, y+53, strTemp );
	pDC->SetTextColor( clrText );
	strTemp.LoadString( IDS_REALTIME_SELL5 );	// "卖⑤";
	pDC->TextOut( x, y+69, strTemp );
	strTemp.LoadString( IDS_REALTIME_SELL4 );	// "卖④";
	pDC->TextOut( x, y+85, strTemp );
	strTemp.LoadString( IDS_REALTIME_SELL3 );	// "卖③";
	pDC->TextOut( x, y+101, strTemp );
	strTemp.LoadString( IDS_REALTIME_SELL2 );	// "卖②";
	pDC->TextOut( x, y+117, strTemp );
	strTemp.LoadString( IDS_REALTIME_SELL1 );	// "卖①";
	pDC->TextOut( x, y+133, strTemp );

	// 委买数
	DrawLine( pDC, 1, clrBorder, xStart, y+150, xStart+nWidth, y+150 );
	strTemp.LoadString( IDS_REALTIME_BUY1 );	// "买①";
	pDC->TextOut( x, y+152, strTemp );
	strTemp.LoadString( IDS_REALTIME_BUY2 );	// "买②";
	pDC->TextOut( x, y+168, strTemp );
	strTemp.LoadString( IDS_REALTIME_BUY3 );	// "买③";
	pDC->TextOut( x, y+184, strTemp );
	strTemp.LoadString( IDS_REALTIME_BUY4 );	// "买④";
	pDC->TextOut( x, y+200, strTemp );
	strTemp.LoadString( IDS_REALTIME_BUY5 );	// "买⑤";
	pDC->TextOut( x, y+216, strTemp );
	pDC->SetTextColor( clrNewKLine );
	strTemp.LoadString( IDS_REALTIME_BUY6 );	// "买⑥";
	pDC->TextOut( x, y+232, strTemp );
	strTemp.LoadString( IDS_REALTIME_BUY7 );	// "买⑦";
	pDC->TextOut( x, y+248, strTemp );
	strTemp.LoadString( IDS_REALTIME_BUY8 );	// "买⑧";
	pDC->TextOut( x, y+264, strTemp );
	pDC->SetTextColor( clrText );

	// 其他信息报告
	DrawLine( pDC, 1, clrBorder, xStart, y+281, xStart+nWidth, y+281 );
	strTemp.LoadString( IDS_REALTIME_PRICENOW );	// "成交";
	pDC->TextOut( x, y+283, strTemp );
	strTemp.LoadString( IDS_REALTIME_PRICEAVE );	// "均价";
	pDC->TextOut( x2, y+283, strTemp );
	strTemp.LoadString( IDS_REALTIME_PRICEDIFF );	// "涨跌";
	pDC->TextOut( x, y+299, strTemp );
	strTemp.LoadString( IDS_REALTIME_PRICEOPEN );	// "开盘";
	pDC->TextOut( x2, y+299, strTemp );
	strTemp.LoadString( IDS_REALTIME_PRICEDIFFPERCENT );	// "幅度";
	pDC->TextOut( x, y+315, strTemp );
	strTemp.LoadString( IDS_REALTIME_PRICEHIGH );	// "最高";
	pDC->TextOut( x2, y+315, strTemp );
	strTemp.LoadString( IDS_REALTIME_VOLSUM );		// "总手";
	pDC->TextOut( x, y+331, strTemp );
	strTemp.LoadString( IDS_REALTIME_PRICELOW );	// "最低";
	pDC->TextOut( x2, y+331, strTemp );
	strTemp.LoadString( IDS_REALTIME_VOLNOW );		// "现手";
	pDC->TextOut( x, y+347, strTemp );
	strTemp.LoadString( IDS_REALTIME_VOLRATION );	// "量比";
	pDC->TextOut( x2, y+347, strTemp );

	// 内外盘
	DrawLine( pDC, 1, clrBorder, xStart, y+364, xStart+nWidth, y+364 );
	strTemp.LoadString( IDS_REALTIME_VOLOUTER );	// "外盘";
	pDC->TextOut( x, y+366, strTemp );
	strTemp.LoadString( IDS_REALTIME_VOLINNER );	// "内盘";
	pDC->TextOut( x2, y+366, strTemp );

	DrawLine( pDC, 1, clrBorder, xStart, y+383, xStart+nWidth, y+383 );	

	// 涨家数，跌家数
	if( info.IsIndex() )
	{
		strTemp.LoadString( IDS_REALTIME_ADVANCE );	// "涨家数";
		pDC->TextOut( x, y+385, strTemp );
		strTemp.LoadString( IDS_REALTIME_DECLINE );	// "跌家数";
		pDC->TextOut( x2, y+385, strTemp );
		DrawLine( pDC, 1, clrBorder, xStart, y+402, xStart+nWidth, y+402 );
	}
	

	// Draw Report Values
	x	= xStart-1+nWidth/2;
	x2	= xStart+nWidth-1;
	UINT nOldAlign = pDC->SetTextAlign( TA_RIGHT | TA_TOP );
	pDC->SetTextColor( clrPlane );
	
	// 委比和委差
	double	dRatio = 0, dDiff = 0;
	if( info.IsIndex() || info.GetSellBuyRatio( &dRatio, &dDiff ) )
	{
		strTemp.Format( "%.2f%%", dRatio );
		pDC->TextOut( x, y+2,   strTemp );
		strTemp.Format( "%.0f", dDiff*0.01 );
		pDC->TextOut( x2, y+2,   strTemp );
	}

	// 价格小数位数格式串
	CString strPriceFmt;
	strPriceFmt.Format( "%%.%df", info.DigitBit() );

	// 委卖数
	float	fSellPrice[8], fSellVolume[8], fBuyPrice[8], fBuyVolume[8];
	m_CurStock.GetReport().StatBuySellEx( fSellPrice, fSellVolume, fBuyPrice, fBuyVolume, 8 );
	if( info.IsIndex() )
	{
		memset( fSellPrice, 0, sizeof(fSellPrice) );
		memset( fSellVolume, 0, sizeof(fSellVolume) );
		memset( fBuyPrice, 0, sizeof(fBuyPrice) );
		memset( fBuyVolume, 0, sizeof(fBuyVolume) );
	}
	pDC->SetTextColor( RT_DETERMINE_COLOR(fSellPrice[7],info.m_fLast) );
	strTemp.Format( strPriceFmt, fSellPrice[7] );
	pDC->TextOut( x, y+21, strTemp );
	strTemp.Format( "%.0f", fSellVolume[7]/100 );
	pDC->TextOut( x2, y+21, strTemp );

	pDC->SetTextColor( RT_DETERMINE_COLOR(fSellPrice[6],info.m_fLast) );
	strTemp.Format( strPriceFmt, fSellPrice[6] );
	pDC->TextOut( x, y+37, strTemp );
	strTemp.Format( "%.0f", fSellVolume[6]/100 );
	pDC->TextOut( x2, y+37, strTemp );

	pDC->SetTextColor( RT_DETERMINE_COLOR(fSellPrice[5],info.m_fLast) );
	strTemp.Format( strPriceFmt, fSellPrice[5] );
	pDC->TextOut( x, y+53, strTemp );
	strTemp.Format( "%.0f", fSellVolume[5]/100 );
	pDC->TextOut( x2, y+53, strTemp );

	pDC->SetTextColor( RT_DETERMINE_COLOR(fSellPrice[4],info.m_fLast) );
	strTemp.Format( strPriceFmt, fSellPrice[4] );
	pDC->TextOut( x, y+69, strTemp );
	strTemp.Format( "%.0f", fSellVolume[4]/100 );
	pDC->TextOut( x2, y+69, strTemp );

	pDC->SetTextColor( RT_DETERMINE_COLOR(fSellPrice[3],info.m_fLast) );
	strTemp.Format( strPriceFmt, fSellPrice[3] );
	pDC->TextOut( x, y+85, strTemp );
	strTemp.Format( "%.0f", fSellVolume[3]/100 );
	pDC->TextOut( x2, y+85, strTemp );

	pDC->SetTextColor( RT_DETERMINE_COLOR(fSellPrice[2],info.m_fLast) );
	strTemp.Format( strPriceFmt, fSellPrice[2] );
	pDC->TextOut( x, y+101, strTemp );
	strTemp.Format( "%.0f", fSellVolume[2]/100 );
	pDC->TextOut( x2, y+101, strTemp );

	pDC->SetTextColor( RT_DETERMINE_COLOR(fSellPrice[1],info.m_fLast) );
	strTemp.Format( strPriceFmt, fSellPrice[1] );
	pDC->TextOut( x, y+117, strTemp );
	strTemp.Format( "%.0f", fSellVolume[1]/100 );
	pDC->TextOut( x2, y+117, strTemp );

	pDC->SetTextColor( RT_DETERMINE_COLOR(fSellPrice[0],info.m_fLast) );
	strTemp.Format( strPriceFmt, fSellPrice[0] );
	pDC->TextOut( x, y+133, strTemp );
	strTemp.Format( "%.0f", fSellVolume[0]/100 );
	pDC->TextOut( x2, y+133, strTemp );

	// 委买数
	pDC->SetTextColor( RT_DETERMINE_COLOR(fBuyPrice[0],info.m_fLast) );
	strTemp.Format( strPriceFmt, fBuyPrice[0] );
	pDC->TextOut( x, y+152, strTemp );
	strTemp.Format( "%.0f", fBuyVolume[0]/100 );
	pDC->TextOut( x2, y+152, strTemp );

	pDC->SetTextColor( RT_DETERMINE_COLOR(fBuyPrice[1],info.m_fLast) );
	strTemp.Format( strPriceFmt, fBuyPrice[1] );
	pDC->TextOut( x, y+168, strTemp );
	strTemp.Format( "%.0f", fBuyVolume[1]/100 );
	pDC->TextOut( x2, y+168, strTemp );
	
	pDC->SetTextColor( RT_DETERMINE_COLOR(fBuyPrice[2],info.m_fLast) );
	strTemp.Format( strPriceFmt, fBuyPrice[2] );
	pDC->TextOut( x, y+184, strTemp );
	strTemp.Format( "%.0f", fBuyVolume[2]/100 );
	pDC->TextOut( x2, y+184, strTemp );

	pDC->SetTextColor( RT_DETERMINE_COLOR(fBuyPrice[3],info.m_fLast) );
	strTemp.Format( strPriceFmt, fBuyPrice[3] );
	pDC->TextOut( x, y+200, strTemp );
	strTemp.Format( "%.0f", fBuyVolume[3]/100 );
	pDC->TextOut( x2, y+200, strTemp );

	pDC->SetTextColor( RT_DETERMINE_COLOR(fBuyPrice[4],info.m_fLast) );
	strTemp.Format( strPriceFmt, fBuyPrice[4] );
	pDC->TextOut( x, y+216, strTemp );
	strTemp.Format( "%.0f", fBuyVolume[4]/100 );
	pDC->TextOut( x2, y+216, strTemp );

	pDC->SetTextColor( RT_DETERMINE_COLOR(fBuyPrice[5],info.m_fLast) );
	strTemp.Format( strPriceFmt, fBuyPrice[5] );
	pDC->TextOut( x, y+232, strTemp );
	strTemp.Format( "%.0f", fBuyVolume[5]/100 );
	pDC->TextOut( x2, y+232, strTemp );

	pDC->SetTextColor( RT_DETERMINE_COLOR(fBuyPrice[6],info.m_fLast) );
	strTemp.Format( strPriceFmt, fBuyPrice[6] );
	pDC->TextOut( x, y+248, strTemp );
	strTemp.Format( "%.0f", fBuyVolume[6]/100 );
	pDC->TextOut( x2, y+248, strTemp );

	pDC->SetTextColor( RT_DETERMINE_COLOR(fBuyPrice[7],info.m_fLast) );
	strTemp.Format( strPriceFmt, fBuyPrice[7] );
	pDC->TextOut( x, y+264, strTemp );
	strTemp.Format( "%.0f", fBuyVolume[7]/100 );
	pDC->TextOut( x2, y+264, strTemp );

	// 其他信息报告
	// 成交
	pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fClose,info.m_fLast) );
	strTemp.Format( strPriceFmt, info.m_fClose );
	pDC->TextOut( x, y+283, strTemp );
	
	// 均价
	pDC->SetTextColor( clrPlane );
	strTemp	= "-";
	if( info.m_fVolume > 0 )
	{
		double dAve	= info.m_fAmount/info.m_fVolume;
		pDC->SetTextColor( RT_DETERMINE_COLOR(dAve,info.m_fLast) );
		strTemp.Format( strPriceFmt, dAve );
	}
	pDC->TextOut( x2, y+283, strTemp );
	
	// 涨跌
	pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fClose,info.m_fLast) );
	strTemp.Format( strPriceFmt, info.m_fClose-info.m_fLast );
	pDC->TextOut( x, y+299, strTemp );

	// 开盘
	pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fOpen,info.m_fLast) );
	strTemp.Format( strPriceFmt, info.m_fOpen );
	pDC->TextOut( x2, y+299, strTemp );
	
	// 幅度
	pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fClose,info.m_fLast) );
	strTemp	= "-";
	if( info.m_fLast > 1e-4 )
		strTemp.Format( "%.2f%%", 100*(info.m_fClose-info.m_fLast)/info.m_fLast );
	pDC->TextOut( x, y+315, strTemp );
	
	// 最高
	pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fHigh,info.m_fLast) );
	strTemp.Format( strPriceFmt, info.m_fHigh );
	pDC->TextOut( x2, y+315, strTemp );
	
	// 总手
	pDC->SetTextColor( clrPlane );
	strTemp.Format( "%.0f", info.m_fVolume/100. );
	pDC->TextOut( x, y+331, strTemp );
	
	// 最低
	pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fLow,info.m_fLast) );
	strTemp.Format( strPriceFmt, info.m_fLow );
	pDC->TextOut( x2, y+331, strTemp );
	
	// 现手
	pDC->SetTextColor( clrPlane );
	strTemp	= "-";
	double	dVolNow = 0, dVolOuter = 0, dVolInner = 0;
	BOOL bStatSuc = m_CurStock.GetMinute().StatVolumeInfo( &dVolNow, &dVolOuter, &dVolInner );
	if( bStatSuc )
		strTemp.Format( "%.0f", dVolNow/100. );
	pDC->TextOut( x, y+347, strTemp );
	
	// 量比
	pDC->SetTextColor( clrPlane );
	double	dVolRatio = 0;
	strTemp	= "-";
	if( info.GetRatioVolume( &dVolRatio, info.m_datetech, AfxGetProfile().GetRatioVolumeDays() ) )
	{
		CSPTime	tTrade;
		if( tTrade.FromStockTimeDay( info.m_datetech ) )
			dVolRatio = dVolRatio / CSPTime::GetTimeTradeRatioOfOneDay( tTrade, CSPTime::GetCurrentTime() );
		strTemp.Format( "%.2f", dVolRatio );
	}
	pDC->TextOut( x2, y+347, strTemp );

	// 外盘
	pDC->SetTextColor( clrRise );
	strTemp	= "-";
	if( bStatSuc )
		strTemp.Format( "%.0f", dVolOuter/100 );
	pDC->TextOut( x, y+366, strTemp );
	
	// 内盘
	pDC->SetTextColor( clrFall );
	strTemp	= "-";
	if( bStatSuc )
		strTemp.Format( "%.0f", dVolInner/100 );
	pDC->TextOut( x2, y+366, strTemp );

	// 涨家数，跌家数
	int	nRetValue = 383;
	if( info.IsIndex() )
	{
		pDC->SetTextColor( clrRise );
		strTemp	= "-";
		if( info.m_dwAdvance > 0 )
			strTemp.Format( "%uf", info.m_dwAdvance );
		pDC->TextOut( x, y+385, strTemp );

		pDC->SetTextColor( clrFall );
		strTemp	= "-";
		if( info.m_dwDecline > 0 )
			strTemp.Format( "%uf", info.m_dwDecline );
		pDC->TextOut( x2, y+385, strTemp );
		
		nRetValue = 402;
	}

	pDC->SetTextAlign( nOldAlign );
	pDC->SelectObject( pOldFont );

	return nRetValue;
}

void CRealTime::DrawReportQuote( CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	if( m_rectReport.IsRectEmpty() )
		return;

	// Draw Rect
	pDC->FillSolidRect( &m_rectReport, clrBK );
	pDC->Draw3dRect( &m_rectReport, clrBorder, clrBorder );
	pDC->SetBkColor( clrBK );

	int nBuySellHeight = DrawBuySell( pDC, m_rectReport.left, m_rectReport.top, m_rectReport.Width() );

	CStockInfo & info	= m_CurStock.GetStockInfo();
	CReport & aReport	= m_CurStock.GetReport();

	CRect	rect	= m_rectReport;
	rect.top	= m_rectReport.top + nBuySellHeight + 1;
	m_nCurrentPageCount = (rect.Height()-RT_RDTITLE_HEIGHT) / RT_RDELEMENT_HEIGHT;
	if( m_nCurrentPageCount <= 0 )	m_nCurrentPageCount	= 1;
	m_nCurrentStartPos	= CheckValidStartPos( m_nCurrentStartPos, aReport.GetSize(), m_nCurrentPageCount, FALSE );

	DrawReportColumn( pDC, info, aReport, m_nCurrentStartPos, m_nCurrentPageCount, rect );
}

void CRealTime::DrawReportPrice( CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	if( m_rectReport.IsRectEmpty() )
		return;

	// Draw Rect
	pDC->FillSolidRect( &m_rectReport, clrBK );
	pDC->Draw3dRect( &m_rectReport, clrBorder, clrBorder );
	pDC->SetBkColor( clrBK );

	int nBuySellHeight = DrawBuySell( pDC, m_rectReport.left, m_rectReport.top, m_rectReport.Width() );

	UINT nOldAlign = pDC->SetTextAlign( TA_LEFT | TA_TOP );
	CFont	* pOldFont	= AfxSelectDCFont( pDC, 14 );

	CStockInfo & info	= m_CurStock.GetStockInfo();
	CMinute & aMinute	= m_CurStock.GetMinute();

	// 成交价量
	int y	= m_rectReport.top + nBuySellHeight + 2;
	int x	= m_rectReport.left+2;
	int x2	= (int)(x+0.55*m_rectReport.Width());
	int	x3	= m_rectReport.right-1;
	int	nMaxDrawCount	= (m_rectReport.bottom-y-1)/16;
	CSPDWordArray adwPrice, adwVolume;
	double dMaxVolume = 5;

	// 价格小数位数格式串
	CString strPriceFmt;
	strPriceFmt.Format( "%%.%df", info.DigitBit() );
	
	DrawLine( pDC, 1, clrBorder, m_rectReport.left, m_rectReport.top+nBuySellHeight+1, m_rectReport.right, m_rectReport.top+nBuySellHeight+1 );	
	if( aMinute.GetSize() > 0 && nMaxDrawCount > 0
		&& aMinute.StatDealInfo( adwPrice, adwVolume, &dMaxVolume ) 
		&& dMaxVolume >= 1 )
	{
		int	nStartPos	= 0;
		if( adwPrice.GetSize() > nMaxDrawCount )
			nStartPos	= adwPrice.GetSize()-nMaxDrawCount;
		for( int i=nStartPos; i<adwPrice.GetSize(); i++ )
		{
			// Price
			double	dPrice	= double(adwPrice[i]) * 0.001;
			CString	strPrice;
			strPrice.Format( strPriceFmt, dPrice );
			pDC->SetTextColor( RT_DETERMINE_COLOR(dPrice,info.m_fLast) );
			pDC->SetTextAlign( TA_LEFT | TA_TOP );
			pDC->TextOut( x, y+16*(i-nStartPos), strPrice );

			// Volume
			CString	strVolume;
			strVolume.Format( "%.0f", double(adwVolume[i]) * 0.01 );
			pDC->SetTextColor( clrText );
			pDC->SetTextAlign( TA_RIGHT | TA_TOP );
			pDC->TextOut( x2, y+16*(i-nStartPos), strVolume );
			
			// Volume Rect
			CRect	rect( x2+2, y+16*(i-nStartPos)+2, x3-3, y+16*(i-nStartPos+1)-2 );
			rect.right = rect.left + rect.Width() * (double)adwVolume[i] / dMaxVolume;
			pDC->FillSolidRect( &rect, clrRise );
			pDC->SetBkColor( clrBK );
		}
	}

	pDC->SetTextAlign( nOldAlign );
	pDC->SelectObject( pOldFont );
}

void CRealTime::DrawReportMinute( CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	if( m_rectReport.IsRectEmpty() )
		return;

	// Draw Rect
	pDC->FillSolidRect( &m_rectReport, clrBK );
	pDC->Draw3dRect( &m_rectReport, clrBorder, clrBorder );
	pDC->SetBkColor( clrBK );

	int nBuySellHeight = DrawBuySell( pDC, m_rectReport.left, m_rectReport.top, m_rectReport.Width() );

	CStockInfo & info	= m_CurStock.GetStockInfo();
	CMinute & aMinute	= m_CurStock.GetMinute();

	CRect	rect	= m_rectReport;
	rect.top	= m_rectReport.top + nBuySellHeight + 1;
	m_nCurrentPageCount = (rect.Height()-RT_RDTITLE_HEIGHT) / RT_RDELEMENT_HEIGHT;
	if( m_nCurrentPageCount <= 0 )	m_nCurrentPageCount	= 1;
	m_nCurrentStartPos	= CheckValidStartPos( m_nCurrentStartPos, aMinute.GetSize(), m_nCurrentPageCount, FALSE );

	DrawMinuteColumn( pDC, info, aMinute, m_nCurrentStartPos, m_nCurrentPageCount, rect );
}

void CRealTime::DrawReportBuySellEx( CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	if( m_rectReport.IsRectEmpty() )
		return;

	// Draw Rect
	pDC->FillSolidRect( &m_rectReport, clrBK );
	pDC->Draw3dRect( &m_rectReport, clrBorder, clrBorder );
	pDC->SetBkColor( clrBK );

	int nBuySellHeight = DrawBuySellEx( pDC, m_rectReport.left, m_rectReport.top, m_rectReport.Width() );

	CStockInfo & info	= m_CurStock.GetStockInfo();
	CReport & aReport	= m_CurStock.GetReport();

	CRect	rect	= m_rectReport;
	rect.top	= m_rectReport.top + nBuySellHeight + 1;
	m_nCurrentPageCount = (rect.Height()-RT_RDTITLE_HEIGHT) / RT_RDELEMENT_HEIGHT;
	if( m_nCurrentPageCount <= 0 )	m_nCurrentPageCount	= 1;
	m_nCurrentStartPos	= CheckValidStartPos( m_nCurrentStartPos, aReport.GetSize(), m_nCurrentPageCount, FALSE );

	DrawReportColumn( pDC, info, aReport, m_nCurrentStartPos, m_nCurrentPageCount, rect );
}

void CRealTime::DrawReportValue( CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	if( m_rectReport.IsRectEmpty() )
		return;

	// Draw Rect
	pDC->FillSolidRect( &m_rectReport, clrBK );
	pDC->Draw3dRect( &m_rectReport, clrBorder, clrBorder );
	pDC->SetBkColor( clrBK );

	int nBuySellHeight = DrawBuySell( pDC, m_rectReport.left, m_rectReport.top, m_rectReport.Width() );

	CFont	* pOldFont	= AfxSelectDCFont( pDC, 14 );

	CStockInfo & info	= m_CurStock.GetStockInfo();

	CString	strTemp;
	int y	= m_rectReport.top + nBuySellHeight + 2;
	int x	= m_rectReport.left+2;
	int x2	= (int)(x+0.9*m_rectReport.Width());
	pDC->SetTextColor( clrText );

	DrawLine( pDC, 1, clrBorder, m_rectReport.left, m_rectReport.top+nBuySellHeight+1, m_rectReport.right, m_rectReport.top+nBuySellHeight+1 );	

	// Draw Names
	// 涨停价、跌停价
	strTemp.LoadString( IDS_REALTIME_RISINGLIMIT );		// 涨停价
	pDC->TextOut( x, y+2,   strTemp );
	strTemp.LoadString( IDS_REALTIME_FALLINGLIMIT );	// 跌停价
	pDC->TextOut( x, y+18,   strTemp );

	// 逆势操作系统
	DrawLine( pDC, 1, clrBorder, m_rectReport.left, y+35, m_rectReport.right, y+35 );
	strTemp.LoadString( IDS_REALTIME_AH );	// AH
	pDC->TextOut( x, y+37, strTemp );
	strTemp.LoadString( IDS_REALTIME_NH );	// NH
	pDC->TextOut( x, y+53, strTemp );
	strTemp.LoadString( IDS_REALTIME_NL );	// NL
	pDC->TextOut( x, y+69, strTemp );
	strTemp.LoadString( IDS_REALTIME_AL );	// AL
	pDC->TextOut( x, y+85, strTemp );

	// Draw Values
	UINT nOldAlign = pDC->SetTextAlign( TA_RIGHT | TA_TOP );
	pDC->SetTextColor( clrPlane );

	// 价格小数位数格式串
	CString strPriceFmt;
	strPriceFmt.Format( "%%.%df", info.DigitBit() );

	// 涨停价、跌停价
	pDC->SetTextColor( clrRise );
	strTemp.Format( strPriceFmt, info.m_fLast*1.10 );
	pDC->TextOut( x2, y+2, strTemp );
	pDC->SetTextColor( clrFall );
	strTemp.Format( strPriceFmt, info.m_fLast*0.90 );
	pDC->TextOut( x2, y+18, strTemp );
	

	pDC->SetTextColor( RT_DETERMINE_COLOR(info.m_fSellPrice[2],info.m_fLast) );

	// 逆势操作系统
	AfxPrepareStockData( &AfxGetDB(), m_CurStock, CKData::ktypeDay, CKData::formatXDRdown, CKData::mdtypeClose, FALSE, FALSE );
	CKData	& kdata	= m_CurStock.GetKDataDay();
	if( kdata.GetSize() > 0 )
	{
		CCDP	cdp(&kdata);
		cdp.AttachParameters( AfxGetProfile().GetTechParameters().cdp );

		double dAH = 0, dNH = 0, dAL = 0, dNL = 0;
		if( cdp.Calculate( &dAH, &dNH, &dAL, &dNL, kdata.GetSize()-1, FALSE ) )
		{
			pDC->SetTextColor( RT_DETERMINE_COLOR(dAH,info.m_fLast) );
			strTemp.Format( strPriceFmt, dAH );
			pDC->TextOut( x2, y+37, strTemp );

			pDC->SetTextColor( RT_DETERMINE_COLOR(dAH,info.m_fLast) );
			strTemp.Format( strPriceFmt, dNH );
			pDC->TextOut( x2, y+53, strTemp );
		
			pDC->SetTextColor( RT_DETERMINE_COLOR(dAH,info.m_fLast) );
			strTemp.Format( strPriceFmt, dNL );
			pDC->TextOut( x2, y+69, strTemp );

			pDC->SetTextColor( RT_DETERMINE_COLOR(dAH,info.m_fLast) );
			strTemp.Format( strPriceFmt, dAL );
			pDC->TextOut( x2, y+85, strTemp );
		}
	}

	pDC->SetTextAlign( nOldAlign );
	pDC->SelectObject( pOldFont );
}

void CRealTime::DrawReportDistribute( CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	if( m_rectReport.IsRectEmpty() )
		return;

	CStockInfo & info	= m_CurStock.GetStockInfo();
	CMinute & aMinute	= m_CurStock.GetMinute();

	// Draw Rect
	pDC->FillSolidRect( &m_rectReport, clrBK );
	pDC->Draw3dRect( &m_rectReport, clrBorder, clrBorder );
	pDC->SetBkColor( clrBK );

	// Draw Name And Code
	CFont	* pOldFont	= AfxSelectDCFont( pDC, 18 );
	UINT nOldAlign = pDC->SetTextAlign( TA_LEFT | TA_TOP );
	pDC->SetTextColor( clrTitle );
	pDC->TextOut( m_rectReport.left+1, m_rectAll.top+3, info.GetStockName() );
	pDC->SetTextColor( clrText );
	pDC->TextOut( m_rectReport.left+96, m_rectAll.top+3, info.GetStockCode() );

	// Select Report Font
	AfxSelectDCFont( pDC, 14 );

	int y	= m_rectPrice.top;
	int x	= m_rectReport.left+2;
	int	xMaxLen	= m_rectReport.Width() - 8;
	CSPDWordArray adwPrice, adwVolume;
	double dMaxVolume = 5;
	
	if( aMinute.GetSize() > 0
		&& aMinute.StatDealInfo( adwPrice, adwVolume, &dMaxVolume ) 
		&& dMaxVolume >= 1 
		&& m_dMaxPrice - m_dMinPrice > 1e-4 )
	{
		for( int i=0; i<adwPrice.GetSize(); i++ )
		{
			// Price, Volume
			double	dPrice	= double(adwPrice[i]) * 0.001;
			
			int yPos	= (int)( m_rectPrice.top+m_rectPrice.Height()*(m_dMaxPrice-dPrice)/(m_dMaxPrice-m_dMinPrice) );
			int xEnd	= x + xMaxLen * (double)adwVolume[i] / dMaxVolume;
			if( yPos > m_rectReport.top && yPos < m_rectReport.bottom
				&& xEnd > x && xEnd < m_rectReport.right )
			{
				// Volume Rect
				CRect	rect( x, yPos, xEnd, yPos+2 );
				pDC->FillSolidRect( &rect, clrRise );
			}
			pDC->SetBkColor( clrBK );
		}
	}

	pDC->SetTextAlign( nOldAlign );
	pDC->SelectObject( pOldFont );
}

void CRealTime::DrawReportBigTrade( CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	if( m_rectReport.IsRectEmpty() )
		return;

	// Load Report
	LoadReportBigTrade( );

	CReport & aReport	= m_aReportBigTrade;
	CStockInfo & info	= m_CurStock.GetStockInfo();

	// Draw Rect
	pDC->FillSolidRect( &m_rectReport, clrBK );
	pDC->Draw3dRect( &m_rectReport, clrBorder, clrBorder );
	pDC->SetBkColor( clrBK );

	// Draw Name And Code
	CFont	* pOldFont	= AfxSelectDCFont( pDC, 18 );
	UINT nOldAlign = pDC->SetTextAlign( TA_LEFT | TA_TOP );
	pDC->SetTextColor( clrTitle );
	pDC->TextOut( m_rectReport.left+1, m_rectAll.top+3, info.GetStockName() );
	pDC->SetTextColor( clrText );
	pDC->TextOut( m_rectReport.left+96, m_rectAll.top+3, info.GetStockCode() );

	pDC->SetTextAlign( nOldAlign );
	pDC->SelectObject( pOldFont );

	CRect	rect	= m_rectReport;
	m_nCurrentPageCount = (rect.Height()-RT_RDTITLE_HEIGHT) / RT_RDELEMENT_HEIGHT;
	if( m_nCurrentPageCount <= 0 )	m_nCurrentPageCount	= 1;
	m_nCurrentStartPos	= CheckValidStartPos( m_nCurrentStartPos, aReport.GetSize(), m_nCurrentPageCount, FALSE );
	
	DrawBigTradeColumn( pDC, aReport, m_nCurrentStartPos, m_nCurrentPageCount, rect );
}

BOOL CRealTime::LoadReportBigTrade( )
{
	int	nOldSize	= m_aReportBigTrade.GetSize();

	CStock	stock;
	stock.SetStockCode( CStock::marketSHSE, STKLIB_CODE_ZLDD );
	AfxGetDB().LoadReport( &stock );
	m_aReportBigTrade	= stock.GetReport();
	stock.SetStockCode( CStock::marketSZSE, STKLIB_CODE_ZLDD );
	AfxGetDB().LoadReport( &stock );
	m_aReportBigTrade.InsertAt( 0, &(stock.GetReport()) );
	m_aReportBigTrade.Sort( );

	if( nOldSize != m_aReportBigTrade.GetSize() )
		m_nCurrentStartPos	= -1;

	if( m_nCurrentSelectPos < 0 || m_nCurrentSelectPos >= m_aReportBigTrade.GetSize() )
		m_nCurrentSelectPos	= -1;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// 以下为明细使用 modeReportDetail, modeMinuteDetail, modeBigTradeDetail

int CRealTime::GetColumnPerPage( CRect rectAll )
{
	int nColumns = rectAll.Width() / RT_RDCOLUMN_WIDTH;
	if( nColumns <= 0 )
		nColumns = 1;
	return nColumns;
}

int CRealTime::GetCountPerColumn( CRect rectAll )
{
	int nCount = (rectAll.Height() - RT_RDTITLE_HEIGHT*2) / RT_RDELEMENT_HEIGHT;
	if( nCount <= 0 )
		nCount	= 1;
	return nCount;
}

int CRealTime::CheckValidStartPos( int nStartPosOld, int nTotalCount, int nPageCount, BOOL bAlignBegin )
{
	int	nStartPos	= nStartPosOld;
	if( -1 == nStartPos )	// 缺省从后开始
	{
		if( bAlignBegin )
		{
			nStartPos	= 0;
			while( nTotalCount - nStartPos > nPageCount )	nStartPos	+= nPageCount;
		}
		else
		{
			nStartPos	= nTotalCount - nPageCount;
			if( nStartPos < 0 )
				nStartPos	= 0;
		}
	}
	if( nStartPos < 0 || nStartPos >= nTotalCount )
		nStartPos	= 0;
	return nStartPos;
}

void CRealTime::PageUp( )
{
	int	nTotalSize = 0;
	if( CRealTime::modeReportDetail == m_nDrawMode )
		nTotalSize	= m_CurStock.GetReport().GetSize();
	else if( CRealTime::modeMinuteDetail == m_nDrawMode )
		nTotalSize	= m_CurStock.GetMinute().GetSize();
	else if( CRealTime::modeBigTradeDetail == m_nDrawMode )
		nTotalSize	= m_aReportBigTrade.GetSize();
	else
		return;

	int	nColumnPerPage	= GetColumnPerPage( m_rectAll );
	int	nCountPerColumn	= GetCountPerColumn( m_rectAll );
	int	nCountPerPage	= nCountPerColumn * nColumnPerPage;

	if( 0 == m_nCurrentStartPos )
		return;
	if( -1 == m_nCurrentStartPos )
	{
		m_nCurrentStartPos	= 0;
		while( nTotalSize - m_nCurrentStartPos > nCountPerPage )	m_nCurrentStartPos	+= nCountPerPage;
		m_nCurrentStartPos	-= nCountPerPage;
	}
	else
		m_nCurrentStartPos	-= nCountPerPage;
	
	if( m_nCurrentStartPos < 0 || m_nCurrentStartPos >= nTotalSize )
		m_nCurrentStartPos	= -1;

	Redraw( NULL, m_rectAll );
}

void CRealTime::PageDown( )
{
	int	nTotalSize = 0;
	if( CRealTime::modeReportDetail == m_nDrawMode )
		nTotalSize	= m_CurStock.GetReport().GetSize();
	else if( CRealTime::modeMinuteDetail == m_nDrawMode )
		nTotalSize	= m_CurStock.GetMinute().GetSize();
	else if( CRealTime::modeBigTradeDetail == m_nDrawMode )
		nTotalSize	= m_aReportBigTrade.GetSize();
	else
		return;

	int	nColumnPerPage	= GetColumnPerPage( m_rectAll );
	int	nCountPerColumn	= GetCountPerColumn( m_rectAll );
	int	nCountPerPage	= nCountPerColumn * nColumnPerPage;

	if( -1 != m_nCurrentStartPos )
	{
		m_nCurrentStartPos	+= nCountPerPage;
	}
	
	if( m_nCurrentStartPos < 0 || m_nCurrentStartPos >= nTotalSize )
		m_nCurrentStartPos	= -1;

	Redraw( NULL, m_rectAll );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// 以下为每笔成交明细使用 modeReportDetail

void CRealTime::DrawReportDetail( CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	CStockInfo & info	= m_CurStock.GetStockInfo();
	CReport & aReport	= m_CurStock.GetReport();

	// set bkcolor
	pDC->SetBkColor( clrBK );

	// Draw Name And Code
	CFont	* pOldFont	= AfxSelectDCFont( pDC, 18 );
	pDC->SetTextColor( clrTitle );
	pDC->TextOut( m_rectAll.left+1, m_rectAll.top+3, info.GetStockName() );
	pDC->TextOut( m_rectAll.left+96, m_rectAll.top+3, info.GetStockCode() );

	// Draw Title
	CString	strTitle;
	strTitle.LoadString( IDS_REALTIME_REPORTDETAILTITLE );
	pDC->TextOut( m_rectAll.left+180, m_rectAll.top+3, strTitle );

	// Select Report Font
	AfxSelectDCFont( pDC, 14 );
	
	// 成交明细
	int	nColumnPerPage	= GetColumnPerPage( m_rectAll );
	int	nCountPerColumn	= GetCountPerColumn( m_rectAll );
	int	nCountPerPage	= nCountPerColumn * nColumnPerPage;
	if( nCountPerPage <= 0 )
		return;
	
	m_nCurrentStartPos	= CheckValidStartPos( m_nCurrentStartPos, aReport.GetSize(), nCountPerPage, TRUE );
	int	nStartPos	= m_nCurrentStartPos;

	for( int nColumn=0; nColumn<nColumnPerPage; nColumn ++ )
	{
		if( nStartPos >= aReport.GetSize() )
			break;

		CRect	rectColumn;
		rectColumn.top		= m_rectAll.top + RT_RDTITLE_HEIGHT;
		rectColumn.bottom	= m_rectAll.bottom;
		rectColumn.left		= m_rectAll.left + nColumn*RT_RDCOLUMN_WIDTH;
		rectColumn.right	= rectColumn.left + RT_RDCOLUMN_WIDTH;
		DrawReportColumn( pDC, info, aReport, nStartPos, nCountPerColumn, rectColumn );
		nStartPos	+= nCountPerColumn;
	}

	pDC->SelectObject( pOldFont );
}

void CRealTime::DrawReportColumn( CDC * pDC, CStockInfo & info, CReport & aReport, int nStartPos, int nMaxCount, CRect rect )
{
	DECLARE_COLOR_DEFINATION

	int	y	= rect.top + RT_RDTITLE_HEIGHT;
	int	x	= rect.left+2;
	int	x2	= (int)(x+0.55*rect.Width());
	int	x3	= rect.right-1;

	// Draw Border
	pDC->Draw3dRect( &rect, clrBorder, clrBorder );
	DrawLine( pDC, 1, clrBorder, rect.left, rect.top+RT_RDTITLE_HEIGHT-2 , rect.right, rect.top+RT_RDTITLE_HEIGHT-2 );

	// Set Align
	pDC->SetBkColor( clrBK );
	UINT nOldAlign = pDC->SetTextAlign( TA_LEFT | TA_TOP );

	// Draw Title Text
	CString	strTemp;
	CFont	* pOldFont	= AfxSelectDCFont( pDC, 14 );
	pDC->SetTextColor( clrText );
	strTemp.LoadString( IDS_REALTIME_TIME );
	pDC->TextOut( x+1, rect.top+5, strTemp );
	pDC->SetTextAlign( TA_RIGHT | TA_TOP );
	strTemp.LoadString( IDS_REALTIME_PRICE );
	pDC->TextOut( x2-1, rect.top+5, strTemp );
	strTemp.LoadString( IDS_REALTIME_VOLUME );
	pDC->TextOut( x3-1, rect.top+5, strTemp );

	// 价格小数位数格式串
	CString strPriceFmt;
	strPriceFmt.Format( "%%.%df", info.DigitBit() );

	for( int i=nStartPos; i<aReport.GetSize() && i-nStartPos+1<=nMaxCount; i++ )
	{
		REPORT	report	= aReport.ElementAt(i);
		
		// Time
		CSPTime	time(report.m_time);
		CString	strTime	= time.Format("%H:%M");
		pDC->SetTextColor( clrText );
		pDC->SetTextAlign( TA_LEFT | TA_TOP );
		pDC->TextOut( x, y+RT_RDELEMENT_HEIGHT*(i-nStartPos), strTime );

		// Price
		CString	strPrice;
		strPrice.Format( strPriceFmt, report.m_fNew );
		pDC->SetTextColor( RT_DETERMINE_COLOR(report.m_fNew,report.m_fLast) );
		pDC->SetTextAlign( TA_RIGHT | TA_TOP );
		pDC->TextOut( x2, y+RT_RDELEMENT_HEIGHT*(i-nStartPos), strPrice );
		
		// Volume
		double	dVolume = (i > 0 ? aReport[i].m_fVolume-aReport[i-1].m_fVolume : aReport[i].m_fVolume);
		double	dMidPrice = (i > 0 ? (aReport[i-1].m_fBuyPrice[0]+aReport[i-1].m_fSellPrice[0])/2 : (aReport[i].m_fBuyPrice[0]+aReport[i].m_fSellPrice[0])/2 );
		if( dMidPrice < 1e-4 )
			dMidPrice	= report.m_fLast;
		CString	strVolume, strUp, strDown;
		strUp.LoadString( IDS_SLISTVIEW_UP );
		strDown.LoadString( IDS_SLISTVIEW_DOWN );
		strVolume.Format( "%.0f", dVolume/100 );
		if( report.m_fNew-dMidPrice > 1e-4 )
			strVolume	+= strUp;
		else if( report.m_fNew-dMidPrice < 1e-4 )
			strVolume	+= strDown;
		else
			strVolume	+= "-";
		pDC->SetTextColor( RT_DETERMINE_COLOR(report.m_fNew,dMidPrice) );
		pDC->SetTextAlign( TA_RIGHT | TA_TOP );
		pDC->TextOut( x3, y+RT_RDELEMENT_HEIGHT*(i-nStartPos), strVolume );
	}

	pDC->SetTextAlign( nOldAlign );
	pDC->SelectObject( pOldFont );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// 以下为分钟成交明细使用 modeMinuteDetail

void CRealTime::DrawMinuteDetail( CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	CStockInfo & info	= m_CurStock.GetStockInfo();
	CMinute & aMinute	= m_CurStock.GetMinute();

	// set bkcolor
	pDC->SetBkColor( clrBK );

	// Draw Name And Code
	CFont	* pOldFont	= AfxSelectDCFont( pDC, 18 );
	pDC->SetTextColor( clrTitle );
	pDC->TextOut( m_rectAll.left+1, m_rectAll.top+3, info.GetStockName() );
	pDC->TextOut( m_rectAll.left+96, m_rectAll.top+3, info.GetStockCode() );

	// Draw Title
	CString	strTitle;
	strTitle.LoadString( IDS_REALTIME_MINUTEDETAILTITLE );
	pDC->TextOut( m_rectAll.left+180, m_rectAll.top+3, strTitle );

	// Select Report Font
	AfxSelectDCFont( pDC, 14 );
	
	// 成交明细
	int	nColumnPerPage	= GetColumnPerPage( m_rectAll );
	int	nCountPerColumn	= GetCountPerColumn( m_rectAll );
	int	nCountPerPage	= nCountPerColumn * nColumnPerPage;
	if( nCountPerPage <= 0 )
		return;
	
	m_nCurrentStartPos	= CheckValidStartPos( m_nCurrentStartPos, aMinute.GetSize(), nCountPerPage, TRUE );
	int	nStartPos	= m_nCurrentStartPos;

	for( int nColumn=0; nColumn<nColumnPerPage; nColumn ++ )
	{
		if( nStartPos >= aMinute.GetSize() )
			break;

		CRect	rectColumn;
		rectColumn.top		= m_rectAll.top + RT_RDTITLE_HEIGHT;
		rectColumn.bottom	= m_rectAll.bottom;
		rectColumn.left		= m_rectAll.left + nColumn*RT_RDCOLUMN_WIDTH;
		rectColumn.right	= rectColumn.left + RT_RDCOLUMN_WIDTH;
		DrawMinuteColumn( pDC, info, aMinute, nStartPos, nCountPerColumn, rectColumn );
		nStartPos	+= nCountPerColumn;
	}

	pDC->SelectObject( pOldFont );
}

void CRealTime::DrawMinuteColumn( CDC * pDC, CStockInfo & info, CMinute & aMinute, int nStartPos, int nMaxCount, CRect rect )
{
	DECLARE_COLOR_DEFINATION

	int	y	= rect.top + RT_RDTITLE_HEIGHT;
	int	x	= rect.left+2;
	int	x2	= (int)(x+0.55*rect.Width());
	int	x3	= rect.right-1;

	// Draw Border
	pDC->Draw3dRect( &rect, clrBorder, clrBorder );
	DrawLine( pDC, 1, clrBorder, rect.left, rect.top+RT_RDTITLE_HEIGHT-2 , rect.right, rect.top+RT_RDTITLE_HEIGHT-2 );

	// Set Align
	pDC->SetBkColor( clrBK );
	UINT nOldAlign = pDC->SetTextAlign( TA_LEFT | TA_TOP );

	// Draw Title Text
	CString	strTemp;
	CFont	* pOldFont	= AfxSelectDCFont( pDC, 14 );
	pDC->SetTextColor( clrText );
	strTemp.LoadString( IDS_REALTIME_TIME );
	pDC->TextOut( x+1, rect.top+5, strTemp );
	pDC->SetTextAlign( TA_RIGHT | TA_TOP );
	strTemp.LoadString( IDS_REALTIME_PRICE );
	pDC->TextOut( x2-1, rect.top+5, strTemp );
	strTemp.LoadString( IDS_REALTIME_VOLUME );
	pDC->TextOut( x3-1, rect.top+5, strTemp );

	// 价格小数位数格式串
	CString strPriceFmt;
	strPriceFmt.Format( "%%.%df", info.DigitBit() );

	double	dLastClose = info.m_fLast;

	for( int i=nStartPos; i<aMinute.GetSize() && i-nStartPos+1<=nMaxCount; i++ )
	{
		MINUTE	minute	= aMinute.ElementAt(i);
		
		// Time
		CSPTime	time(minute.m_time);
		CString	strTime	= time.Format("%H:%M");
		pDC->SetTextColor( clrText );
		pDC->SetTextAlign( TA_LEFT | TA_TOP );
		pDC->TextOut( x, y+RT_RDELEMENT_HEIGHT*(i-nStartPos), strTime );

		// Price
		CString	strPrice;
		strPrice.Format( strPriceFmt, minute.m_fNew );
		pDC->SetTextColor( RT_DETERMINE_COLOR(minute.m_fNew,dLastClose) );
		pDC->SetTextAlign( TA_RIGHT | TA_TOP );
		pDC->TextOut( x2, y+RT_RDELEMENT_HEIGHT*(i-nStartPos), strPrice );
		
		// Volume
		double	dVolume = (i > 0 ? aMinute[i].m_fVolume-aMinute[i-1].m_fVolume : aMinute[i].m_fVolume);
		CString	strVolume;
		strVolume.Format( "%.0f", dVolume/100 );
		pDC->SetTextColor( RT_DETERMINE_COLOR(minute.m_fNew,dLastClose) );
		pDC->SetTextAlign( TA_RIGHT | TA_TOP );
		pDC->TextOut( x3, y+RT_RDELEMENT_HEIGHT*(i-nStartPos), strVolume );
	}

	pDC->SetTextAlign( nOldAlign );
	pDC->SelectObject( pOldFont );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// 以下为大单成交明细使用 modeBigTradeDetail

void CRealTime::DrawBigTradeDetail( CDC * pDC )
{
	DECLARE_COLOR_DEFINATION

	// Load Report
	LoadReportBigTrade( );

	CReport & aReport	= m_aReportBigTrade;

	// set bkcolor
	pDC->SetBkColor( clrBK );

	// Draw Title
	CFont	* pOldFont	= AfxSelectDCFont( pDC, 18 );
	pDC->SetTextColor( clrTitle );
	CString	strTitle;
	strTitle.LoadString( IDS_REALTIME_BIGTRADEDETAILTITLE );
	pDC->TextOut( m_rectAll.left+180, m_rectAll.top+3, strTitle );

	// Select Report Font
	AfxSelectDCFont( pDC, 14 );
	
	// 
	int	nColumnPerPage	= GetColumnPerPage( m_rectAll );
	int	nCountPerColumn	= GetCountPerColumn( m_rectAll );
	int	nCountPerPage	= nCountPerColumn * nColumnPerPage;
	if( nCountPerPage <= 0 )
		return;
	
	m_nCurrentStartPos	= CheckValidStartPos( m_nCurrentStartPos, aReport.GetSize(), nCountPerPage, TRUE );
	int	nStartPos	= m_nCurrentStartPos;

	for( int nColumn=0; nColumn<nColumnPerPage; nColumn ++ )
	{
		if( nStartPos >= aReport.GetSize() )
			break;

		CRect	rectColumn;
		rectColumn.top		= m_rectAll.top + RT_RDTITLE_HEIGHT;
		rectColumn.bottom	= m_rectAll.bottom;
		rectColumn.left		= m_rectAll.left + nColumn*RT_RDCOLUMN_WIDTH;
		rectColumn.right	= rectColumn.left + RT_RDCOLUMN_WIDTH;
		DrawBigTradeColumn( pDC, aReport, nStartPos, nCountPerColumn, rectColumn );
		nStartPos	+= nCountPerColumn;
	}

	pDC->SelectObject( pOldFont );
}

void CRealTime::DrawBigTradeColumn( CDC * pDC, CReport & aReport, int nStartPos, int nMaxCount, CRect rect )
{
	DECLARE_COLOR_DEFINATION

	int	y	= rect.top + RT_RDTITLE_HEIGHT;
	int	x	= rect.left+2;
	int	x2	= (int)(x+0.48*rect.Width());
	int	x3	= (int)(x+0.68*rect.Width());
	int	x4	= rect.right-1;

	// Draw Border
	pDC->Draw3dRect( &rect, clrBorder, clrBorder );
	DrawLine( pDC, 1, clrBorder, rect.left, rect.top+RT_RDTITLE_HEIGHT-2 , rect.right, rect.top+RT_RDTITLE_HEIGHT-2 );

	// Set Align
	pDC->SetBkColor( clrBK );
	UINT nOldAlign = pDC->SetTextAlign( TA_LEFT | TA_TOP );

	// Draw Title Text
	CString	strTemp;
	CFont	* pOldFont	= AfxSelectDCFont( pDC, 14 );
	pDC->SetTextColor( clrText );
	strTemp.LoadString( IDS_REALTIME_TIME );
	pDC->TextOut( x+1, rect.top+5, strTemp );
	pDC->SetTextAlign( TA_RIGHT | TA_TOP );
	strTemp.LoadString( IDS_REALTIME_STOCKNAME );
	pDC->TextOut( x2-1, rect.top+5, strTemp );
	strTemp.LoadString( IDS_REALTIME_PRICE );
	pDC->TextOut( x3-1, rect.top+5, strTemp );
	strTemp.LoadString( IDS_REALTIME_VOLUME );
	pDC->TextOut( x4-1, rect.top+5, strTemp );

	for( int i=nStartPos; i<aReport.GetSize() && i-nStartPos+1<=nMaxCount; i++ )
	{
		REPORT	report	= aReport.ElementAt(i);
		
		// Time
		CSPTime	time(report.m_time);
		CString	strTime	= time.Format("%H:%M");
		pDC->SetTextColor( clrText );
		pDC->SetTextAlign( TA_LEFT | TA_TOP );
		pDC->TextOut( x, y+RT_RDELEMENT_HEIGHT*(i-nStartPos), strTime );

		// Stock Name
		pDC->SetTextAlign( TA_RIGHT | TA_TOP );
		pDC->TextOut( x2, y+RT_RDELEMENT_HEIGHT*(i-nStartPos), report.m_szName );

		// Price
		CString	strPrice;
		strPrice.Format( "%.2f", report.m_fNew );
		pDC->SetTextColor( RT_DETERMINE_COLOR(report.m_fNew,report.m_fLast) );
		pDC->SetTextAlign( TA_RIGHT | TA_TOP );
		pDC->TextOut( x3, y+RT_RDELEMENT_HEIGHT*(i-nStartPos), strPrice );
		
		// Volume
		// 大单成交的REPORT中的m_fVolume和m_fAmount已转换为成交数量
		double	dVolume = aReport[i].m_fVolume;
		double	dMidPrice = (aReport[i].m_fBuyPrice[0]+aReport[i].m_fSellPrice[0])/2;
		if( dMidPrice < 1e-4 )
			dMidPrice	= report.m_fLast;
		CString	strVolume, strUp, strDown;
		strUp.LoadString( IDS_SLISTVIEW_UP );
		strDown.LoadString( IDS_SLISTVIEW_DOWN );
		strVolume.Format( "%.0f", dVolume/100 );
		if( report.m_fNew-dMidPrice > 1e-4 )
			strVolume	+= strUp;
		else if( report.m_fNew-dMidPrice < 1e-4 )
			strVolume	+= strDown;
		else
			strVolume	+= "-";
		pDC->SetTextColor( RT_DETERMINE_COLOR(report.m_fNew,dMidPrice) );
		pDC->SetTextAlign( TA_RIGHT | TA_TOP );
		pDC->TextOut( x4, y+RT_RDELEMENT_HEIGHT*(i-nStartPos), strVolume );
	}

	if( m_nCurrentSelectPos >= 0 && m_nCurrentSelectPos < m_aReportBigTrade.GetSize()
		&& m_nCurrentSelectPos >= nStartPos && m_nCurrentSelectPos < nStartPos + nMaxCount )
	{
		int	y	= rect.top+RT_RDTITLE_HEIGHT+RT_RDELEMENT_HEIGHT*(m_nCurrentSelectPos-nStartPos+1) - 1;
		if( y > rect.top && y < rect.bottom )
			DrawLine( pDC, 2, clrBorder, rect.left+5, y, rect.right-5, y );
	}

	pDC->SetTextAlign( nOldAlign );
	pDC->SelectObject( pOldFont );
}
