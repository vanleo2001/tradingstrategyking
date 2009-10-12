// DrawTech.cpp : implementation of the CStockGraph class
//

#include "stdafx.h"
#include "StockGraph.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////
// Draw Technical Utilities

COLORREF	GetLineColor( int nIndex )
{
	int	nColor	=	nIndex % 6;
	if( 0 == nColor )
		return AfxGetProfile().GetColor(CColorClass::clrLine1);
	else if( 1 == nColor )
		return AfxGetProfile().GetColor(CColorClass::clrLine2);
	else if( 2 == nColor )
		return AfxGetProfile().GetColor(CColorClass::clrLine3);
	else if( 3 == nColor )
		return AfxGetProfile().GetColor(CColorClass::clrLine4);
	else if( 4 == nColor )
		return AfxGetProfile().GetColor(CColorClass::clrLine5);
	else
		return AfxGetProfile().GetColor(CColorClass::clrLine6);
}

void CStockGraph::DrawTechUtil( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax,
							   int nTech, CTechnique * pTech, CKData * pKData, CSPDWordArray & adwDays )
{
	CHECK_NODATATODRAW
	DECLARE_COLOR_DEFINATION

	// Draw Sub Title, Param Value
	if( bDrawTitle )
	{
		pDC->FillSolidRect( CRect( rect.left+1, rect.top+1, rect.right-1, rect.top+m_nHeightSubtitle-1 ), clrBK );

		DrawTechTitle( pDC, rect.left+5, rect.top+1, AfxGetSTTShortName(nTech), TA_LEFT | TA_TOP, 14, clrBK, clrTitle );

		CFont	* pOldFont	=	AfxSelectDCFont( pDC, 12 );
		pDC->SetBkColor( clrBK );
		int	nIndexParam	=	( m_nIndexCurrent >= 0 ? m_nIndexCurrent : pKData->GetSize()-1 );
		double	dParam = dMin;
		CString	strText;
		for( int k=0; k<adwDays.GetSize(); k++ )
		{
			if( pTech->Calculate( &dParam, nIndexParam, adwDays[k], FALSE ) )
			{
				strText.Format( "%d%s %.2f", adwDays[k], (LPCTSTR)(AfxGetSTTShortName(nTech)), dParam );
				pDC->SetTextColor( GetLineColor( k ) );
				if( rect.left+m_nWidthSubtitle+(k+1)*m_nWidthParameter > rect.right )
					break;
				pDC->TextOut( rect.left+m_nWidthSubtitle+k*m_nWidthParameter, rect.top+1, strText );
			}
		}
		pDC->SelectObject( pOldFont );
		return;
	}

	if( dMax - dMin < 1e-4 )
		return;

	// Draw
	for( int k=0; k<adwDays.GetSize(); k++ )
	{
		int	yLast, yNow;
		BOOL	bHasLast = FALSE;
		double	dNow = dMin;
		for( int nIndex=m_nIndexStart; nIndex<=m_nIndexEnd; nIndex++ )
		{
			// Prepare Rect 
			CRect	rectK;
			long	xMedium	=	0;
			if( !GetOneKLineRect( nIndex, &rectK, NULL, NULL, &xMedium ) )
				continue;

			// Draw
			if( pTech->Calculate( &dNow, nIndex, adwDays[k], bHasLast ) )
			{
				yNow	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle-2) * (dNow  - dMin) / (dMax-dMin) );
				if( bHasLast )
				{
					if( yLast > rect.bottom || yLast < rect.top+m_nHeightSubtitle
						|| yNow > rect.bottom || yNow < rect.top+m_nHeightSubtitle )
						continue;
					CPen	penLocal( PS_SOLID, 1, GetLineColor(k) );
					CPen * pOldPen = pDC->SelectObject( &penLocal );
					pDC->MoveTo( xMedium-m_nThickness, yLast );
					pDC->LineTo( xMedium, yNow );
					pDC->SelectObject( pOldPen );
				}
				yLast		=	yNow;
				bHasLast	=	TRUE;
			}
		}
	}
}

void CStockGraph::DrawTechUtil1( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax,
							   int nTech, CTechnique * pTech, CKData * pKData,
							   COLORREF clrLine, int nDrawTechType )
{
	CHECK_NODATATODRAW
	DECLARE_COLOR_DEFINATION

	ASSERT( drawtechtype_line == nDrawTechType || drawtechtype_dot == nDrawTechType );

	// Draw Sub Title, Param Value
	if( bDrawTitle )
	{
		pDC->FillSolidRect( CRect( rect.left+1, rect.top+1, rect.right-1, rect.top+m_nHeightSubtitle-1 ), clrBK );

		DrawTechTitle( pDC, rect.left+5, rect.top+1, AfxGetSTTShortName(nTech), TA_LEFT | TA_TOP, 14, clrBK, clrTitle );

		CFont	* pOldFont	=	AfxSelectDCFont( pDC, 12 );
		pDC->SetBkColor( clrBK );
		int	nIndexParam	=	( m_nIndexCurrent >= 0 ? m_nIndexCurrent : pKData->GetSize()-1 );
		double	dParam = dMin;
		CString	strText;
		if( pTech->Calculate( &dParam, nIndexParam, FALSE ) )
		{
			strText.Format( "%s  %.2f", (LPCTSTR)(AfxGetSTTShortName(nTech)), dParam );
			pDC->SetTextColor( clrLine );
			if( rect.left+m_nWidthSubtitle+m_nWidthParameter < rect.right )
				pDC->TextOut( rect.left+m_nWidthSubtitle, rect.top+1, strText );
		}
		pDC->SelectObject( pOldFont );
		return;
	}

	if( dMax - dMin < 1e-4 )
		return;

	// Draw
	int	yLast, yNow;
	BOOL	bHasLast = FALSE;
	double	dNow = dMin;
	for( int nIndex=m_nIndexStart; nIndex<=m_nIndexEnd; nIndex++ )
	{
		// Prepare Rect 
		CRect	rectK;
		long	xMedium	=	0;
		if( !GetOneKLineRect( nIndex, &rectK, NULL, NULL, &xMedium ) )
			continue;

		// Draw
		if( pTech->Calculate( &dNow, nIndex, bHasLast ) )
		{
			yNow	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle-2) * (dNow  - dMin) / (dMax-dMin) );
			if( bHasLast )
			{
				if( yLast > rect.bottom || yLast < rect.top+m_nHeightSubtitle
					|| yNow > rect.bottom || yNow < rect.top+m_nHeightSubtitle )
					continue;
				CPen	penLocal( PS_SOLID, 1, clrLine );
				CPen * pOldPen = pDC->SelectObject( &penLocal );
				if( drawtechtype_line == nDrawTechType )
				{
					pDC->MoveTo( xMedium-m_nThickness, yLast );
					pDC->LineTo( xMedium, yNow );
				}
				else if( drawtechtype_dot == nDrawTechType )
				{
					pDC->Ellipse( xMedium-2, yNow-2, xMedium+2, yNow+2 );
				}
				pDC->SelectObject( pOldPen );
			}
			yLast		=	yNow;
			bHasLast	=	TRUE;
		}
	}
}

void CStockGraph::DrawTechUtil2( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax,
							   int nTech, CTechnique * pTech, CKData * pKData,
							   COLORREF clrLine1, COLORREF clrLine2,
							   CString strTitle1, CString strTitle2 )
{
	CHECK_NODATATODRAW
	DECLARE_COLOR_DEFINATION

	// Draw Sub Title, Param Value
	if( bDrawTitle )
	{
		pDC->FillSolidRect( CRect( rect.left+1, rect.top+1, rect.right-1, rect.top+m_nHeightSubtitle-1 ), clrBK );

		DrawTechTitle( pDC, rect.left+5, rect.top+1, AfxGetSTTShortName(nTech), TA_LEFT | TA_TOP, 14, clrBK, clrTitle );

		CFont	* pOldFont	=	AfxSelectDCFont( pDC, 12 );
		pDC->SetBkColor( clrBK );
		int	nIndexParam	=	( m_nIndexCurrent >= 0 ? m_nIndexCurrent : pKData->GetSize()-1 );
		double	dParam1 = dMin, dParam2 = dMin;
		CString	strText;
		if( pTech->Calculate( &dParam1, &dParam2, nIndexParam, FALSE ) )
		{
			strText.Format( "%s %.2f", strTitle1, dParam1 );
			pDC->SetTextColor( clrLine1 );
			if( rect.left+m_nWidthSubtitle+m_nWidthParameter < rect.right )
				pDC->TextOut( rect.left+m_nWidthSubtitle, rect.top+1, strText );

			strText.Format( "%s %.2f", strTitle2, dParam2 );
			pDC->SetTextColor( clrLine2 );
			if( rect.left+m_nWidthSubtitle+m_nWidthParameter*2 < rect.right )
				pDC->TextOut( rect.left+m_nWidthSubtitle+m_nWidthParameter, rect.top+1, strText );
		}
		pDC->SelectObject( pOldFont );
		return;
	}

	if( dMax - dMin < 1e-4 )
		return;

	// Draw
	int	yLast1, yNow1, yLast2, yNow2;
	BOOL	bHasLast = FALSE;
	double	dNow1 = dMin, dNow2 = dMin;
	for( int nIndex=m_nIndexStart; nIndex<=m_nIndexEnd; nIndex++ )
	{
		// Prepare Rect 
		CRect	rectK;
		long	xMedium	=	0;
		if( !GetOneKLineRect( nIndex, &rectK, NULL, NULL, &xMedium ) )
			continue;

		// Draw
		if( pTech->Calculate( &dNow1, &dNow2, nIndex, bHasLast ) )
		{
			yNow1	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle-2) * (dNow1 - dMin) / (dMax-dMin) );
			yNow2	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle-2) * (dNow2 - dMin) / (dMax-dMin) );
			if( bHasLast )
			{
				if( yLast1 > rect.bottom || yLast1 < rect.top+m_nHeightSubtitle
					|| yNow1 > rect.bottom || yNow1 < rect.top+m_nHeightSubtitle )
					continue;
				if( yLast2 > rect.bottom || yLast2 < rect.top+m_nHeightSubtitle
					|| yNow2 > rect.bottom || yNow2 < rect.top+m_nHeightSubtitle )
					continue;
				CPen	penLocal1( PS_SOLID, 1, clrLine1 );
				CPen * pOldPen = pDC->SelectObject( &penLocal1 );
				pDC->MoveTo( xMedium-m_nThickness, yLast1 );
				pDC->LineTo( xMedium, yNow1 );

				CPen	penLocal2( PS_SOLID, 1, clrLine2 );
				pDC->SelectObject( &penLocal2 );
				pDC->MoveTo( xMedium-m_nThickness, yLast2 );
				pDC->LineTo( xMedium, yNow2 );
				pDC->SelectObject( pOldPen );
			}
			yLast1		=	yNow1;
			yLast2		=	yNow2;
			bHasLast	=	TRUE;
		}
	}
}

void CStockGraph::DrawTechUtil3( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax,
							   int nTech, CTechnique * pTech, CKData * pKData,
							   COLORREF clrLine1, COLORREF clrLine2, COLORREF clrLine3,
							   CString strTitle1, CString strTitle2, CString strTitle3 )
{
	CHECK_NODATATODRAW
	DECLARE_COLOR_DEFINATION

	// Draw Sub Title, Param Value
	if( bDrawTitle )
	{
		pDC->FillSolidRect( CRect( rect.left+1, rect.top+1, rect.right-1, rect.top+m_nHeightSubtitle-1 ), clrBK );
		
		DrawTechTitle( pDC, rect.left+5, rect.top+1, AfxGetSTTShortName(nTech), TA_LEFT | TA_TOP, 14, clrBK, clrTitle );

		CFont	* pOldFont	=	AfxSelectDCFont( pDC, 12 );
		pDC->SetBkColor( clrBK );
		int	nIndexParam	=	( m_nIndexCurrent >= 0 ? m_nIndexCurrent : pKData->GetSize()-1 );
		double	dParam1 = dMin, dParam2 = dMin, dParam3 = dMin;
		CString	strText;
		if( pTech->Calculate( &dParam1, &dParam2, &dParam3, nIndexParam, FALSE ) )
		{
			strText.Format( "%s %.2f", strTitle1, dParam1 );
			pDC->SetTextColor( clrLine1 );
			if( rect.left+m_nWidthSubtitle+m_nWidthParameter < rect.right )
				pDC->TextOut( rect.left+m_nWidthSubtitle, rect.top+1, strText );

			strText.Format( "%s %.2f", strTitle2, dParam2 );
			pDC->SetTextColor( clrLine2 );
			if( rect.left+m_nWidthSubtitle+m_nWidthParameter*2 < rect.right )
				pDC->TextOut( rect.left+m_nWidthSubtitle+m_nWidthParameter, rect.top+1, strText );
			
			strText.Format( "%s %.2f", strTitle3, dParam3 );
			pDC->SetTextColor( clrLine3 );
			if( rect.left+m_nWidthSubtitle+m_nWidthParameter*3 < rect.right )
				pDC->TextOut( rect.left+m_nWidthSubtitle+m_nWidthParameter*2, rect.top+1, strText );
		}
		pDC->SelectObject( pOldFont );
		return;
	}

	if( dMax - dMin < 1e-4 )
		return;

	// Draw
	int	yLast1, yNow1, yLast2, yNow2, yLast3, yNow3;
	BOOL	bHasLast = FALSE;
	double	dNow1 = dMin, dNow2 = dMin, dNow3 = dMin;
	for( int nIndex=m_nIndexStart; nIndex<=m_nIndexEnd; nIndex++ )
	{
		// Prepare Rect 
		CRect	rectK;
		long	xMedium	=	0;
		if( !GetOneKLineRect( nIndex, &rectK, NULL, NULL, &xMedium ) )
			continue;

		// Draw
		if( pTech->Calculate( &dNow1, &dNow2, &dNow3, nIndex, bHasLast ) )
		{
			yNow1	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle-2) * (dNow1 - dMin) / (dMax-dMin) );
			yNow2	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle-2) * (dNow2 - dMin) / (dMax-dMin) );
			yNow3	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle-2) * (dNow3 - dMin) / (dMax-dMin) );
			if( bHasLast )
			{
				if( yLast1 > rect.bottom || yLast1 < rect.top+m_nHeightSubtitle
					|| yNow1 > rect.bottom || yNow1 < rect.top+m_nHeightSubtitle )
					continue;
				if( yLast2 > rect.bottom || yLast2 < rect.top+m_nHeightSubtitle
					|| yNow2 > rect.bottom || yNow2 < rect.top+m_nHeightSubtitle )
					continue;
				if( yLast3 > rect.bottom || yLast3 < rect.top+m_nHeightSubtitle
					|| yNow3 > rect.bottom || yNow3 < rect.top+m_nHeightSubtitle )
					continue;
				CPen	penLocal1( PS_SOLID, 1, clrLine1 );
				CPen * pOldPen = pDC->SelectObject( &penLocal1 );
				pDC->MoveTo( xMedium-m_nThickness, yLast1 );
				pDC->LineTo( xMedium, yNow1 );

				CPen	penLocal2( PS_SOLID, 1, clrLine2 );
				pDC->SelectObject( &penLocal2 );
				pDC->MoveTo( xMedium-m_nThickness, yLast2 );
				pDC->LineTo( xMedium, yNow2 );

				CPen	penLocal3( PS_SOLID, 1, clrLine3 );
				pDC->SelectObject( &penLocal3 );
				pDC->MoveTo( xMedium-m_nThickness, yLast3 );
				pDC->LineTo( xMedium, yNow3 );
				pDC->SelectObject( pOldPen );
			}
			yLast1		=	yNow1;
			yLast2		=	yNow2;
			yLast3		=	yNow3;
			bHasLast	=	TRUE;
		}
	}
}

void CStockGraph::DrawTechUtil4( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax,
							   int nTech, CTechnique * pTech, CKData * pKData,
							   COLORREF clrLine1, COLORREF clrLine2, COLORREF clrLine3, COLORREF clrLine4,
							   CString strTitle1, CString strTitle2, CString strTitle3, CString strTitle4 )
{
	CHECK_NODATATODRAW
	DECLARE_COLOR_DEFINATION

	// Draw Sub Title, Param Value
	if( bDrawTitle )
	{
		pDC->FillSolidRect( CRect( rect.left+1, rect.top+1, rect.right-1, rect.top+m_nHeightSubtitle-1 ), clrBK );

		DrawTechTitle( pDC, rect.left+5, rect.top+1, AfxGetSTTShortName(nTech), TA_LEFT | TA_TOP, 14, clrBK, clrTitle );

		CFont	* pOldFont	=	AfxSelectDCFont( pDC, 12 );
		pDC->SetBkColor( clrBK );
		int	nIndexParam	=	( m_nIndexCurrent >= 0 ? m_nIndexCurrent : pKData->GetSize()-1 );
		double	dParam1 = dMin, dParam2 = dMin, dParam3 = dMin, dParam4 = dMin;
		CString	strText;
		if( pTech->Calculate( &dParam1, &dParam2, &dParam3, &dParam4, nIndexParam, FALSE ) )
		{
			strText.Format( "%s %.2f", strTitle1, dParam1 );
			pDC->SetTextColor( clrLine1 );
			if( rect.left+m_nWidthSubtitle+m_nWidthParameter < rect.right )
				pDC->TextOut( rect.left+m_nWidthSubtitle, rect.top+1, strText );

			strText.Format( "%s %.2f", strTitle2, dParam2 );
			pDC->SetTextColor( clrLine2 );
			if( rect.left+m_nWidthSubtitle+m_nWidthParameter*2 < rect.right )
				pDC->TextOut( rect.left+m_nWidthSubtitle+m_nWidthParameter, rect.top+1, strText );
			
			strText.Format( "%s %.2f", strTitle3, dParam3 );
			pDC->SetTextColor( clrLine3 );
			if( rect.left+m_nWidthSubtitle+m_nWidthParameter*3 < rect.right )
				pDC->TextOut( rect.left+m_nWidthSubtitle+m_nWidthParameter*2, rect.top+1, strText );

			strText.Format( "%s %.2f", strTitle4, dParam4 );
			pDC->SetTextColor( clrLine4 );
			if( rect.left+m_nWidthSubtitle+m_nWidthParameter*4 < rect.right )
				pDC->TextOut( rect.left+m_nWidthSubtitle+m_nWidthParameter*3, rect.top+1, strText );
		}
		pDC->SelectObject( pOldFont );
		return;
	}

	if( dMax - dMin < 1e-4 )
		return;

	// Draw
	int	yLast1, yNow1, yLast2, yNow2, yLast3, yNow3, yLast4, yNow4;
	BOOL	bHasLast = FALSE;
	double	dNow1 = dMin, dNow2 = dMin, dNow3 = dMin, dNow4 = dMin;
	for( int nIndex=m_nIndexStart; nIndex<=m_nIndexEnd; nIndex++ )
	{
		// Prepare Rect 
		CRect	rectK;
		long	xMedium	=	0;
		if( !GetOneKLineRect( nIndex, &rectK, NULL, NULL, &xMedium ) )
			continue;

		// Draw
		if( pTech->Calculate( &dNow1, &dNow2, &dNow3, &dNow4, nIndex, bHasLast ) )
		{
			yNow1	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle-2) * (dNow1 - dMin) / (dMax-dMin) );
			yNow2	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle-2) * (dNow2 - dMin) / (dMax-dMin) );
			yNow3	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle-2) * (dNow3 - dMin) / (dMax-dMin) );
			yNow4	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle-2) * (dNow4 - dMin) / (dMax-dMin) );
			if( bHasLast )
			{
				if( yLast1 > rect.bottom || yLast1 < rect.top+m_nHeightSubtitle
					|| yNow1 > rect.bottom || yNow1 < rect.top+m_nHeightSubtitle )
					continue;
				if( yLast2 > rect.bottom || yLast2 < rect.top+m_nHeightSubtitle
					|| yNow2 > rect.bottom || yNow2 < rect.top+m_nHeightSubtitle )
					continue;
				if( yLast3 > rect.bottom || yLast3 < rect.top+m_nHeightSubtitle
					|| yNow3 > rect.bottom || yNow3 < rect.top+m_nHeightSubtitle )
					continue;
				if( yLast4 > rect.bottom || yLast4 < rect.top+m_nHeightSubtitle
					|| yNow4 > rect.bottom || yNow4 < rect.top+m_nHeightSubtitle )
					continue;
				CPen	penLocal1( PS_SOLID, 1, clrLine1 );
				CPen * pOldPen = pDC->SelectObject( &penLocal1 );
				pDC->MoveTo( xMedium-m_nThickness, yLast1 );
				pDC->LineTo( xMedium, yNow1 );

				CPen	penLocal2( PS_SOLID, 1, clrLine2 );
				pDC->SelectObject( &penLocal2 );
				pDC->MoveTo( xMedium-m_nThickness, yLast2 );
				pDC->LineTo( xMedium, yNow2 );

				CPen	penLocal3( PS_SOLID, 1, clrLine3 );
				pDC->SelectObject( &penLocal3 );
				pDC->MoveTo( xMedium-m_nThickness, yLast3 );
				pDC->LineTo( xMedium, yNow3 );

				CPen	penLocal4( PS_SOLID, 1, clrLine4 );
				pDC->SelectObject( &penLocal4 );
				pDC->MoveTo( xMedium-m_nThickness, yLast4 );
				pDC->LineTo( xMedium, yNow4 );
				pDC->SelectObject( pOldPen );
			}
			yLast1		=	yNow1;
			yLast2		=	yNow2;
			yLast3		=	yNow3;
			yLast4		=	yNow4;
			bHasLast	=	TRUE;
		}
	}
}

void CStockGraph::DrawTechUtil5( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax,
							   int nTech, CTechnique * pTech, CKData * pKData,
							   COLORREF clrLine1, COLORREF clrLine2, COLORREF clrLine3, COLORREF clrLine4, COLORREF clrLine5,
							   CString strTitle1, CString strTitle2, CString strTitle3, CString strTitle4, CString strTitle5 )
{
	CHECK_NODATATODRAW
	DECLARE_COLOR_DEFINATION

	// Draw Sub Title, Param Value
	if( bDrawTitle )
	{
		pDC->FillSolidRect( CRect( rect.left+1, rect.top+1, rect.right-1, rect.top+m_nHeightSubtitle-1 ), clrBK );

		DrawTechTitle( pDC, rect.left+5, rect.top+1, AfxGetSTTShortName(nTech), TA_LEFT | TA_TOP, 14, clrBK, clrTitle );

		CFont	* pOldFont	=	AfxSelectDCFont( pDC, 12 );
		pDC->SetBkColor( clrBK );
		int	nIndexParam	=	( m_nIndexCurrent >= 0 ? m_nIndexCurrent : pKData->GetSize()-1 );
		double	dParam1 = dMin, dParam2 = dMin, dParam3 = dMin, dParam4 = dMin, dParam5 = dMin;
		CString	strText;
		if( pTech->Calculate( &dParam1, &dParam2, &dParam3, &dParam4, &dParam5, nIndexParam, FALSE ) )
		{
			strText.Format( "%s %.2f", strTitle1, dParam1 );
			pDC->SetTextColor( clrLine1 );
			if( rect.left+m_nWidthSubtitle+m_nWidthParameter < rect.right )
				pDC->TextOut( rect.left+m_nWidthSubtitle, rect.top+1, strText );

			strText.Format( "%s %.2f", strTitle2, dParam2 );
			pDC->SetTextColor( clrLine2 );
			if( rect.left+m_nWidthSubtitle+m_nWidthParameter*2 < rect.right )
				pDC->TextOut( rect.left+m_nWidthSubtitle+m_nWidthParameter, rect.top+1, strText );
			
			strText.Format( "%s %.2f", strTitle3, dParam3 );
			pDC->SetTextColor( clrLine3 );
			if( rect.left+m_nWidthSubtitle+m_nWidthParameter*3 < rect.right )
				pDC->TextOut( rect.left+m_nWidthSubtitle+m_nWidthParameter*2, rect.top+1, strText );

			strText.Format( "%s %.2f", strTitle4, dParam4 );
			pDC->SetTextColor( clrLine4 );
			if( rect.left+m_nWidthSubtitle+m_nWidthParameter*4 < rect.right )
				pDC->TextOut( rect.left+m_nWidthSubtitle+m_nWidthParameter*3, rect.top+1, strText );

			strText.Format( "%s %.2f", strTitle5, dParam5 );
			pDC->SetTextColor( clrLine5 );
			if( rect.left+m_nWidthSubtitle+m_nWidthParameter*5 < rect.right )
				pDC->TextOut( rect.left+m_nWidthSubtitle+m_nWidthParameter*4, rect.top+1, strText );
		}
		pDC->SelectObject( pOldFont );
		return;
	}

	if( dMax - dMin < 1e-4 )
		return;

	// Draw
	int	yLast1, yNow1, yLast2, yNow2, yLast3, yNow3, yLast4, yNow4, yLast5, yNow5;
	BOOL	bHasLast = FALSE;
	double	dNow1 = dMin, dNow2 = dMin, dNow3 = dMin, dNow4 = dMin, dNow5 = dMin;
	for( int nIndex=m_nIndexStart; nIndex<=m_nIndexEnd; nIndex++ )
	{
		// Prepare Rect 
		CRect	rectK;
		long	xMedium	=	0;
		if( !GetOneKLineRect( nIndex, &rectK, NULL, NULL, &xMedium ) )
			continue;

		// Draw
		if( pTech->Calculate( &dNow1, &dNow2, &dNow3, &dNow4, &dNow5, nIndex, bHasLast ) )
		{
			yNow1	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle-2) * (dNow1 - dMin) / (dMax-dMin) );
			yNow2	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle-2) * (dNow2 - dMin) / (dMax-dMin) );
			yNow3	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle-2) * (dNow3 - dMin) / (dMax-dMin) );
			yNow4	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle-2) * (dNow4 - dMin) / (dMax-dMin) );
			yNow5	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle-2) * (dNow5 - dMin) / (dMax-dMin) );
			if( bHasLast )
			{
				if( yLast1 > rect.bottom || yLast1 < rect.top+m_nHeightSubtitle
					|| yNow1 > rect.bottom || yNow1 < rect.top+m_nHeightSubtitle )
					continue;
				if( yLast2 > rect.bottom || yLast2 < rect.top+m_nHeightSubtitle
					|| yNow2 > rect.bottom || yNow2 < rect.top+m_nHeightSubtitle )
					continue;
				if( yLast3 > rect.bottom || yLast3 < rect.top+m_nHeightSubtitle
					|| yNow3 > rect.bottom || yNow3 < rect.top+m_nHeightSubtitle )
					continue;
				if( yLast4 > rect.bottom || yLast4 < rect.top+m_nHeightSubtitle
					|| yNow4 > rect.bottom || yNow4 < rect.top+m_nHeightSubtitle )
					continue;
				if( yLast5 > rect.bottom || yLast5 < rect.top+m_nHeightSubtitle
					|| yNow5 > rect.bottom || yNow5 < rect.top+m_nHeightSubtitle )
					continue;
				CPen	penLocal1( PS_SOLID, 1, clrLine1 );
				CPen * pOldPen = pDC->SelectObject( &penLocal1 );
				pDC->MoveTo( xMedium-m_nThickness, yLast1 );
				pDC->LineTo( xMedium, yNow1 );

				CPen	penLocal2( PS_SOLID, 1, clrLine2 );
				pDC->SelectObject( &penLocal2 );
				pDC->MoveTo( xMedium-m_nThickness, yLast2 );
				pDC->LineTo( xMedium, yNow2 );

				CPen	penLocal3( PS_SOLID, 1, clrLine3 );
				pDC->SelectObject( &penLocal3 );
				pDC->MoveTo( xMedium-m_nThickness, yLast3 );
				pDC->LineTo( xMedium, yNow3 );

				CPen	penLocal4( PS_SOLID, 1, clrLine4 );
				pDC->SelectObject( &penLocal4 );
				pDC->MoveTo( xMedium-m_nThickness, yLast4 );
				pDC->LineTo( xMedium, yNow4 );

				CPen	penLocal5( PS_SOLID, 1, clrLine5 );
				pDC->SelectObject( &penLocal5 );
				pDC->MoveTo( xMedium-m_nThickness, yLast5 );
				pDC->LineTo( xMedium, yNow5 );
				pDC->SelectObject( pOldPen );
			}
			yLast1		=	yNow1;
			yLast2		=	yNow2;
			yLast3		=	yNow3;
			yLast4		=	yNow4;
			yLast5		=	yNow5;
			bHasLast	=	TRUE;
		}
	}
}

////////////////////////////////////////////////////////////
// KLine Technical Attributes
void CStockGraph::DrawTechMA( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.ma.AttachParameters( AfxGetProfile().GetTechParameters().ma );
	DrawTechUtil( pDC, rect, bDrawTitle, dMin, dMax, STT_MA, &(m_techparam.ma), &kdata, m_techparam.ma.m_adwMADays );
}

void CStockGraph::DrawTechBBI( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.bbi.AttachParameters( AfxGetProfile().GetTechParameters().bbi );
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_BBI, &(m_techparam.bbi), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechBOLL( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.boll.AttachParameters( AfxGetProfile().GetTechParameters().boll );
	DrawTechUtil3(	pDC, rect, bDrawTitle, dMin, dMax, STT_BOLL, &(m_techparam.boll), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrPlane),
					AfxGetProfile().GetColor(CColorClass::clrRise),
					AfxGetProfile().GetColor(CColorClass::clrFall),
					"MB", "UP", "DOWN");
}

void CStockGraph::DrawTechPV( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.pv.AttachParameters( AfxGetProfile().GetTechParameters().pv );
	
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_PV, &(m_techparam.pv), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechSAR( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.sar.AttachParameters( AfxGetProfile().GetTechParameters().sar );
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_SAR, &(m_techparam.sar), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_dot );
}

void CStockGraph::DrawTechDJ( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax )
{
	CHECK_NODATATODRAW
	DECLARE_COLOR_DEFINATION

	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.dj.AttachParameters( AfxGetProfile().GetTechParameters().dj );

	// Draw Sub Title
	if( bDrawTitle )
	{
		CString	strTitle	=	AfxGetSTTShortName(STT_DJ);
		strTitle	+=	"    ";
		if( m_CurStock.GetStockInfo().IsShangHai() )
			strTitle	+=	m_techparam.dj.m_strCodeSha;
		else if( m_CurStock.GetStockInfo().IsShenZhen() )
			strTitle	+=	m_techparam.dj.m_strCodeSzn;
		DrawTechTitle( pDC, rect.left+5, rect.top+1, strTitle, TA_LEFT | TA_TOP, 14, clrBK, clrTitle );
		return;
	}

	// Prepare Data
	if( !m_techparam.dj.IsValidParameters() )
		m_techparam.dj.SetDefaultParameters( );
	if( !m_techparam.dj.PrepareStockData(	&AfxGetDB(), m_CurStock.GetStockCode(),
							m_nCurKType, m_nCurKFormat, m_nCurMaindataType,
							AfxGetProfile().GetAutoResumeDRBegin(), AfxGetProfile().GetAutoResumeDRLimit() ) )
		return;
	CKData	* pKData = NULL;
	if( m_CurStock.GetStockInfo().IsShangHai() )
		pKData	=	& (	CDJ::m_stockSha.GetKData(m_nCurKType) );
	else if( m_CurStock.GetStockInfo().IsShenZhen() )
		pKData	=	& (	CDJ::m_stockSzn.GetKData(m_nCurKType) );
	else
		return;

	// GetMinMaxInfo
	double	dMinDJ = 0, dMaxDJ = 0;
	CKLine	kline( pKData );
	int	nStartDJ	=	pKData->GetIndexByDate( kdata.ElementAt(m_nIndexStart).m_date );
	int	nEndDJ		=	pKData->GetIndexByDate( kdata.ElementAt(m_nIndexEnd).m_date );
	if( -1 == nStartDJ )	nStartDJ	=	0;
	if( -1 == nEndDJ )		nEndDJ		=	pKData->GetSize()-1;
	if( nStartDJ < 0 || nEndDJ < 0 || !kline.GetMinMaxInfo( nStartDJ, nEndDJ, &dMinDJ, &dMaxDJ ) )
		return;

	// Draw
	for( int nIndex=m_nIndexStart; nIndex<=m_nIndexEnd; nIndex++ )
	{
		DWORD	date	=	kdata.ElementAt(nIndex).m_date;
		int	nIndexDJ	=	pKData->GetIndexByDate( date );
		if( nIndexDJ >= 0 && nIndexDJ < pKData->GetSize() )
		{
			DrawOneKLine( pDC, nIndex, nIndexDJ, pKData, dMinDJ, dMaxDJ, TRUE );
		}
	}
}

void CStockGraph::DrawTechCW( CDC * pDC, CRect rect, BOOL bDrawTitle, double dMin, double dMax )
{
	CHECK_NODATATODRAW
	DECLARE_COLOR_DEFINATION

	if( bDrawTitle )
		return;

	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.cw.AttachParameters( AfxGetProfile().GetTechParameters().cw );

	double	dStep	=	(dMax-dMin)*0.003;
	CSPDWordArray adwPrice, adwVolume;
	double	dMinVolume = 0, dMaxVolume = 0;
	int	nEnd = -1;
	if( !m_techparam.cw.CalculatePastCW(nEnd, 0, m_CurStock.GetStockInfo(), dStep, adwPrice, adwVolume, &dMinVolume, &dMaxVolume, NULL, NULL )
		|| dMinVolume < 0 || dMaxVolume < 1e-4 )
		return;

	CPen	pen( PS_SOLID, 3, clrCW );
	CPen * pOldPen = pDC->SelectObject( &pen );

	double	dVolume = 0, dPrice = 0;
	for( int k=0; k<adwPrice.GetSize() && k<adwVolume.GetSize(); k++ )
	{
		dPrice	=	double(adwPrice[k]) * 0.001;
		dVolume	=	adwVolume[k];
		int	yPrice	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle) * (dPrice - dMin) / (dMax-dMin) );
		int	xVolume	=	int( rect.right - (rect.Width()*dVolume/(3*dMaxVolume)) );
		if( yPrice > rect.top+m_nHeightSubtitle+1 && yPrice < rect.bottom-2 )
		{
			pDC->MoveTo( xVolume, yPrice );
			pDC->LineTo( rect.right-1, yPrice );
		}
	}
	pDC->SelectObject( pOldPen );
}

////////////////////////////////////////////////////////////
// Technical Lines
void CStockGraph::DrawTechMACD( CDC * pDC, CRect rect, BOOL bDrawTitle, int nTech, CMACD * pmacd )
{
	CHECK_NODATATODRAW
	DECLARE_COLOR_DEFINATION
	COLORREF	clrLine1	=	AfxGetProfile().GetColor(CColorClass::clrLine1);
	COLORREF	clrLine2	=	AfxGetProfile().GetColor(CColorClass::clrLine2);

	// Prepare
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.macd.AttachParameters( AfxGetProfile().GetTechParameters().macd );
	if( NULL == pmacd )
	{
		pmacd	=	&(m_techparam.macd);
		nTech	=	STT_MACD;
	}

	double	dMin = 0, dMax = 1;
	if( !pmacd->GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 2 ) )
		return;

	// Draw Title
	if( bDrawTitle )
		DrawTechTitle( pDC, rect.left+5, rect.top+1,AfxGetSTTShortName(nTech), TA_LEFT | TA_TOP, 14, clrBK, clrTitle );

	// Draw
	double	dEMA1 = 0, dEMA2 = 0, dDIF = 0, dDEA = 0;
	int	yDIFLast = (int)dMin, yDIFNow, yDEALast = (int)dMin, yDEANow;
	BOOL	bHasLast	=	FALSE;
	for( int nIndex=m_nIndexStart; nIndex<=m_nIndexEnd; nIndex++ )
	{
		// Prepare Rect
		CRect	rectK;
		long	xMedium	=	0;
		if( !GetOneKLineRect( nIndex, &rectK, NULL, NULL, &xMedium ) )
			continue;
			
		// Draw
		if( pmacd->Calculate( &dEMA1, &dEMA2, &dDIF, &dDEA, nIndex, bHasLast ) )
		{
			yDIFNow		=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle) * (dDIF  - dMin) / (dMax-dMin) );
			yDEANow		=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle) * (dDEA  - dMin) / (dMax-dMin) );
			int	yZero	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle) * (0-dMin) / (dMax-dMin) );
			int	yGap	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle) * (2*dDIF-2*dDEA-dMin) / (dMax-dMin) );

			if( bHasLast )
			{
				CPen	penDiff( PS_SOLID, 1, (dDIF-dDEA)>0 ? clrRise : clrFall );
				CPen * pOldPen = pDC->SelectObject( &penDiff );
				pDC->MoveTo( xMedium, yZero );
				pDC->LineTo( xMedium, yGap );
				pDC->SelectObject( pOldPen );
				if( nIndex > m_nIndexStart )
				{
					if( yDIFLast > rect.bottom || yDIFLast < rect.top+m_nHeightSubtitle || yDIFNow > rect.bottom || yDIFNow < rect.top+m_nHeightSubtitle 
						|| yDEALast > rect.bottom || yDEALast < rect.top+m_nHeightSubtitle || yDEANow > rect.bottom || yDEANow < rect.top+m_nHeightSubtitle )
						continue;
					CPen	penDIF( PS_SOLID, 1, clrLine1 );
					pOldPen = pDC->SelectObject( &penDIF );
					pDC->MoveTo( xMedium-m_nThickness, yDIFLast );
					pDC->LineTo( xMedium, yDIFNow );

					CPen	penDEA( PS_SOLID, 1, clrLine2 );
					pDC->SelectObject( &penDEA );
					pDC->MoveTo( xMedium-m_nThickness, yDEALast );
					pDC->LineTo( xMedium, yDEANow );
					pDC->SelectObject( pOldPen );
				}
			}
			bHasLast	=	TRUE;
			yDIFLast	=	yDIFNow;
			yDEALast	=	yDEANow;

			// Draw Sub Title, Param Value
			int	nIndexParam	=	( m_nIndexCurrent >= 0 ? m_nIndexCurrent : kdata.GetSize()-1 );
			if( bDrawTitle && nIndex == nIndexParam )
			{
				if( rect.left+300 <= rect.right )
				{
					CFont	* pOldFont	=	AfxSelectDCFont( pDC, 12 );
					pDC->SetBkColor( clrBK );
					CString	strText;
					strText.Format( "EMA(%d) EMA(%d) DIF %.2f ", pmacd->m_nEMA1Days, pmacd->m_nEMA2Days, dDIF );
					pDC->SetTextColor( clrLine1 );
					pDC->TextOut( rect.left+m_nWidthSubtitle, rect.top+1, strText );
					strText.Format( "DEA(%d) %.2f",	pmacd->m_nDIFDays, dDEA );
					pDC->SetTextColor( clrLine2 );
					pDC->TextOut( rect.left+m_nWidthSubtitle+m_nWidthParameter*2, rect.top+1, strText );
					pDC->SelectObject( pOldFont );
				}
			}
		}
	}
}

void CStockGraph::DrawTechMIKE( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CHECK_NODATATODRAW
	DECLARE_COLOR_DEFINATION

	// Prepare
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.mike.AttachParameters( AfxGetProfile().GetTechParameters().mike );

	// Draw Title
	if( bDrawTitle )
		DrawTechTitle( pDC, rect.left+5, rect.top+1,AfxGetSTTShortName(STT_MIKE), TA_LEFT | TA_TOP, 14, clrBK, clrTitle );

	// Draw
	double	dWR = 0, dMR = 0, dSR = 0, dWS = 0, dMS = 0, dSS = 0;
	int	nIndexParam	=	m_nIndexCurrent >= 0 ? m_nIndexCurrent : kdata.GetSize()-1;
	if( nIndexParam < 0 || nIndexParam >= kdata.GetSize() )
		return;

	CFont	* pOldFont	=	AfxSelectDCFont( pDC, 14 );
	pDC->SetBkColor( clrBK );

	if( m_techparam.mike.CalculateMIKE(	&dWR, &dMR, &dSR, &dWS, &dMS, &dSS, nIndexParam ) )
	{
		CSize	szUnit		=	CSize( max(35,rect.Width()/6), max(18,rect.Height()/5) );
		int	xStart	=	rect.left+szUnit.cx;
		int	yStart	=	rect.top+szUnit.cy;
		double	dC	=	kdata.ElementAt(nIndexParam).m_fClose;
		CString	strTemp;

		DrawTextWithRect( pDC, xStart, yStart, xStart+szUnit.cx+1, yStart+szUnit.cy+1, IDS_STOCKGRAPH_PROJECT, clrBorder, clrTitle, clrBK, &rect );
		DrawTextWithRect( pDC, xStart+szUnit.cx, yStart, xStart+szUnit.cx*2+1, yStart+szUnit.cy+1, IDS_STOCKGRAPH_PRIMARY, clrBorder, clrTitle, clrBK, &rect );
		DrawTextWithRect( pDC, xStart+szUnit.cx*2, yStart, xStart+szUnit.cx*3+1, yStart+szUnit.cy+1, IDS_STOCKGRAPH_MEDIATE, clrBorder, clrTitle, clrBK, &rect );
		DrawTextWithRect( pDC, xStart+szUnit.cx*3, yStart, xStart+szUnit.cx*4+1, yStart+szUnit.cy+1, IDS_STOCKGRAPH_MIGHT, clrBorder, clrTitle, clrBK, &rect );

		DrawTextWithRect( pDC, xStart, yStart+szUnit.cy, xStart+szUnit.cx+1, yStart+szUnit.cy*2+1, IDS_STOCKGRAPH_PRESSURE, clrBorder, clrTitle, clrBK, &rect );
		strTemp.Format( "%.2f", dWR );
		DrawTextWithRect( pDC, xStart+szUnit.cx, yStart+szUnit.cy, xStart+szUnit.cx*2+1, yStart+szUnit.cy*2+1, strTemp, clrBorder, dWR > dC ? clrRise : clrFall, clrBK, &rect );
		strTemp.Format( "%.2f", dMR );
		DrawTextWithRect( pDC, xStart+szUnit.cx*2, yStart+szUnit.cy, xStart+szUnit.cx*3+1, yStart+szUnit.cy*2+1, strTemp, clrBorder, dMR > dC ? clrRise : clrFall, clrBK, &rect );
		strTemp.Format( "%.2f", dSR );
		DrawTextWithRect( pDC, xStart+szUnit.cx*3, yStart+szUnit.cy, xStart+szUnit.cx*4+1, yStart+szUnit.cy*2+1, strTemp, clrBorder, dSR > dC ? clrRise : clrFall, clrBK, &rect );

		DrawTextWithRect( pDC, xStart, yStart+szUnit.cy*2, xStart+szUnit.cx+1, yStart+szUnit.cy*3+1, IDS_STOCKGRAPH_UPHOLD, clrBorder, clrTitle, clrBK, &rect );
		strTemp.Format( "%.2f", dWS );
		DrawTextWithRect( pDC, xStart+szUnit.cx, yStart+szUnit.cy*2, xStart+szUnit.cx*2+1, yStart+szUnit.cy*3+1, strTemp, clrBorder, dWS > dC ? clrRise : clrFall, clrBK, &rect );
		strTemp.Format( "%.2f", dMS );
		DrawTextWithRect( pDC, xStart+szUnit.cx*2, yStart+szUnit.cy*2, xStart+szUnit.cx*3+1, yStart+szUnit.cy*3+1, strTemp, clrBorder, dMS > dC ? clrRise : clrFall, clrBK, &rect );
		strTemp.Format( "%.2f", dSS );
		DrawTextWithRect( pDC, xStart+szUnit.cx*3, yStart+szUnit.cy*2, xStart+szUnit.cx*4+1, yStart+szUnit.cy*3+1, strTemp, clrBorder, dSS > dC ? clrRise : clrFall, clrBK, &rect );

	}
	pDC->SelectObject( pOldFont );
}

void CStockGraph::DrawTechPSY( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.psy.AttachParameters( AfxGetProfile().GetTechParameters().psy );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.psy.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_PSY, &(m_techparam.psy), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechVOLUME( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CHECK_NODATATODRAW
	DECLARE_COLOR_DEFINATION

	// Prepare
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.volume.AttachParameters( AfxGetProfile().GetTechParameters().volume );
	double	dMin = 0, dMax = 1;
	if( !m_techparam.volume.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 3, dMin*0.01, dMax*0.01, FALSE, TRUE, 0 ) )
		return;

	// Draw Title
	if( bDrawTitle )
	{
		DrawTechTitle( pDC, rect.left+5, rect.top+1, AfxGetSTTShortName(STT_VOLUME), TA_LEFT | TA_TOP, 14, clrBK, clrTitle );

		CFont	* pOldFont	=	AfxSelectDCFont( pDC, 12 );
		pDC->SetBkColor( clrBK );
		int	nIndexParam	=	( m_nIndexCurrent >= 0 ? m_nIndexCurrent : kdata.GetSize()-1 );
		double	dParam;
		CString	strText;
		for( int k=0; k<m_techparam.volume.m_adwMADays.GetSize(); k++ )
		{
			if( m_techparam.volume.Calculate( &dParam, nIndexParam, m_techparam.volume.m_adwMADays[k], FALSE ) )
			{
				strText.Format( "%dMV %.0f", m_techparam.volume.m_adwMADays[k], dParam );
				pDC->SetTextColor( GetLineColor( k ) );
				if( rect.left+m_nWidthSubtitle+(k+1)*m_nWidthParameter > rect.right )
					break;
				pDC->TextOut( rect.left+m_nWidthSubtitle+k*m_nWidthParameter, rect.top+1, strText );
			}
		}
		pDC->SelectObject( pOldFont );
	}

	// Draw
	for( int k=0; k<m_techparam.volume.m_adwMADays.GetSize(); k++ )
	{
		int	yLast, yNow;
		BOOL	bHasLast = FALSE;
		double	dNow;
		for( int nIndex=m_nIndexStart; nIndex<=m_nIndexEnd; nIndex++ )
		{
			// Prepare Rect 
			CRect	rectK, rcEntity;
			long	xMedium	=	0;
			if( !GetOneKLineRect( nIndex, &rectK, &rcEntity.left, &rcEntity.right, &xMedium ) )
				continue;

			// Draw Entity
			if( 0 == k )
			{
				// Calculate rcEntity.top and rcEntity.bottom
				KDATA	kd	=	kdata.ElementAt(nIndex);
				rcEntity.top	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle) * (kd.m_fVolume - dMin) / (dMax-dMin) );
				rcEntity.bottom	=	rect.bottom-1;

				COLORREF	clr	=	clrRise;
				if( kd.m_fClose < kd.m_fOpen )
					clr	=	clrFallEntity;
				if( kd.m_date > m_dwLatestDate )
					clr	=	clrNewKLine;
				pDC->SetBkColor( clrBK );
				if( kd.m_fClose < kd.m_fOpen )
					pDC->FillSolidRect( &rcEntity, clr );
				else
					pDC->Draw3dRect( &rcEntity, clr, clr );
			}

			// Draw MV
			if( m_techparam.volume.Calculate( &dNow, nIndex, m_techparam.volume.m_adwMADays[k], bHasLast ) )
			{
				yNow	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle) * (dNow  - dMin) / (dMax-dMin) );
				if( bHasLast )
				{
					if( yLast > rect.bottom || yLast < rect.top+m_nHeightSubtitle
						|| yNow > rect.bottom || yNow < rect.top+m_nHeightSubtitle )
						continue;
					CPen	penLocal( PS_SOLID, 1, GetLineColor(k) );
					CPen * pOldPen = pDC->SelectObject( &penLocal );
					pDC->MoveTo( xMedium-m_nThickness, yLast );
					pDC->LineTo( xMedium, yNow );
					pDC->SelectObject( pOldPen );
				}
				bHasLast	=	TRUE;
				yLast		=	yNow;
			}
		}
	}
}

void CStockGraph::DrawTechNVI( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.nvi.AttachParameters( AfxGetProfile().GetTechParameters().nvi );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.nvi.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_NVI, &(m_techparam.nvi), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"NVI", "MA");
}

void CStockGraph::DrawTechPVI( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.pvi.AttachParameters( AfxGetProfile().GetTechParameters().pvi );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.pvi.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_PVI, &(m_techparam.pvi), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"PVI", "MA");
}

void CStockGraph::DrawTechVR( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.vr.AttachParameters( AfxGetProfile().GetTechParameters().vr );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.vr.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_VR, &(m_techparam.vr), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechVROC( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.vroc.AttachParameters( AfxGetProfile().GetTechParameters().vroc );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.vroc.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_VROC, &(m_techparam.vroc), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"VROC", "MA" );
}

void CStockGraph::DrawTechOBV( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.obv.AttachParameters( AfxGetProfile().GetTechParameters().obv );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.obv.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 3, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_OBV, &(m_techparam.obv), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechMOBV( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.mobv.AttachParameters( AfxGetProfile().GetTechParameters().mobv );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.mobv.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 3, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil3(	pDC, rect, bDrawTitle, dMin, dMax, STT_MOBV, &(m_techparam.mobv), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					AfxGetProfile().GetColor(CColorClass::clrLine3),
					"SHT", "MED", "LNG" );
}

void CStockGraph::DrawTechMFI( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.mfi.AttachParameters( AfxGetProfile().GetTechParameters().mfi );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.mfi.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_MFI, &(m_techparam.mfi), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechVMACD( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.vmacd.AttachParameters( AfxGetProfile().GetTechParameters().vmacd );

	DrawTechMACD( pDC, rect, bDrawTitle, STT_VMACD, &(m_techparam.vmacd) );
}

void CStockGraph::DrawTechWVAD( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.wvad.AttachParameters( AfxGetProfile().GetTechParameters().wvad );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.wvad.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 3, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_WVAD, &(m_techparam.wvad), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechEMV( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.emv.AttachParameters( AfxGetProfile().GetTechParameters().emv );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.emv.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 2 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_EMV, &(m_techparam.emv), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"EMV", "MA" );
}

void CStockGraph::DrawTechVRSI( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.vrsi.AttachParameters( AfxGetProfile().GetTechParameters().vrsi );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.vrsi.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_VRSI, &(m_techparam.vrsi), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechNVRSI( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.nvrsi.AttachParameters( AfxGetProfile().GetTechParameters().nvrsi );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.nvrsi.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_NVRSI, &(m_techparam.nvrsi), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechAD( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.ad.AttachParameters( AfxGetProfile().GetTechParameters().ad );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.ad.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_AD, &(m_techparam.ad), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechCI( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.ci.AttachParameters( AfxGetProfile().GetTechParameters().ci );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.ci.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_CI, &(m_techparam.ci), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechKDJ( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.kdj.AttachParameters( AfxGetProfile().GetTechParameters().kdj );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.kdj.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil3(	pDC, rect, bDrawTitle, dMin, dMax, STT_KDJ, &(m_techparam.kdj), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					AfxGetProfile().GetColor(CColorClass::clrLine3),
					"K", "D", "J" );
}

void CStockGraph::DrawTechR( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.r.AttachParameters( AfxGetProfile().GetTechParameters().r );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.r.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_R, &(m_techparam.r), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechRSI( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.rsi.AttachParameters( AfxGetProfile().GetTechParameters().rsi );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.rsi.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil(	pDC, rect, bDrawTitle, dMin, dMax, STT_RSI, &(m_techparam.rsi), &kdata, m_techparam.rsi.m_adwDays );
}

void CStockGraph::DrawTechBIAS( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.bias.AttachParameters( AfxGetProfile().GetTechParameters().bias );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.bias.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 2 ) )
		return;
	
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_BIAS, &(m_techparam.bias), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechMTM( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.mtm.AttachParameters( AfxGetProfile().GetTechParameters().mtm );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.mtm.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_MTM, &(m_techparam.mtm), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"MTM", "MA" );
}

void CStockGraph::DrawTechDMI( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.dmi.AttachParameters( AfxGetProfile().GetTechParameters().dmi );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.dmi.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil4(	pDC, rect, bDrawTitle, dMin, dMax, STT_DMI, &(m_techparam.dmi), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					AfxGetProfile().GetColor(CColorClass::clrLine3),
					AfxGetProfile().GetColor(CColorClass::clrLine4),
					"+DI", "-DI", "ADX", "ADXR" );
}

void CStockGraph::DrawTechROC( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.roc.AttachParameters( AfxGetProfile().GetTechParameters().roc );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.roc.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_ROC, &(m_techparam.roc), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"ROC", "MA" );
}

void CStockGraph::DrawTechCCI( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.cci.AttachParameters( AfxGetProfile().GetTechParameters().cci );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.cci.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_CCI, &(m_techparam.cci), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"CCI", "MA" );
}

void CStockGraph::DrawTechCV( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CHECK_NODATATODRAW
	DECLARE_COLOR_DEFINATION
	COLORREF	clrLine1	=	AfxGetProfile().GetColor(CColorClass::clrLine1);

	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.cv.AttachParameters( AfxGetProfile().GetTechParameters().cv );

	// Get Min Max
	double	dMin = 0, dMax = 0;
	if( !m_techparam.cv.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;

	// Draw Title
	if( bDrawTitle )
	{
		DrawTechTitle( pDC, rect.left+5, rect.top+1,AfxGetSTTShortName(STT_CV), TA_LEFT | TA_TOP, 14, clrBK, clrTitle );
		
		CFont	* pOldFont	=	AfxSelectDCFont( pDC, 12 );
		pDC->SetBkColor( clrBK );
		int	nIndexParam	=	( m_nIndexCurrent >= 0 ? m_nIndexCurrent : kdata.GetSize()-1 );
		double	dParam = dMin;
		CString	strText;
		if( m_techparam.cv.Calculate( &dParam, nIndexParam, FALSE ) )
		{
			strText.Format( "%s  %.2f", (LPCTSTR)(AfxGetSTTShortName(STT_CV)), dParam );
			pDC->SetTextColor( clrLine1 );
			if( rect.left+m_nWidthSubtitle+m_nWidthParameter < rect.right )
				pDC->TextOut( rect.left+m_nWidthSubtitle, rect.top+1, strText );
		}
		pDC->SelectObject( pOldFont );
	}

	// Draw
	int	yNow;
	BOOL	bHasLast = FALSE;
	double	dNow = dMin;
	int	yZero	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle) * (0-dMin) / (dMax-dMin) );
	for( int nIndex=m_nIndexStart; nIndex<=m_nIndexEnd; nIndex++ )
	{
		// Prepare Rect 
		CRect	rectK;
		long	xMedium	=	0;
		if( !GetOneKLineRect( nIndex, &rectK, NULL, NULL, &xMedium ) )
			continue;

		// Draw
		if( m_techparam.cv.Calculate( &dNow, nIndex, bHasLast ) )
		{
			yNow	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle) * (dNow  - dMin) / (dMax-dMin) );
			if( bHasLast )
			{
				if( yNow > rect.bottom || yNow < rect.top+m_nHeightSubtitle )
					continue;
				
				CPen	pen( PS_SOLID, 1, (yNow-yZero)<0 ? clrRise : clrFall );
				CPen * pOldPen = pDC->SelectObject( &pen );
				pDC->MoveTo( xMedium, yZero );
				pDC->LineTo( xMedium, yNow );
				pDC->SelectObject( pOldPen );
			}
			bHasLast	=	TRUE;
		}
	}
}

void CStockGraph::DrawTechARBR( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.arbr.AttachParameters( AfxGetProfile().GetTechParameters().arbr );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.arbr.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_ARBR, &(m_techparam.arbr), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"AR", "BR" );
}

void CStockGraph::DrawTechCR( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.cr.AttachParameters( AfxGetProfile().GetTechParameters().cr );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.cr.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil5(	pDC, rect, bDrawTitle, dMin, dMax, STT_CR, &(m_techparam.cr), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					AfxGetProfile().GetColor(CColorClass::clrLine3),
					AfxGetProfile().GetColor(CColorClass::clrLine4),
					AfxGetProfile().GetColor(CColorClass::clrLine5),
					"CR", "A", "B", "C", "D" );
}

void CStockGraph::DrawTechOSC( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.osc.AttachParameters( AfxGetProfile().GetTechParameters().osc );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.osc.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 2 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_OSC, &(m_techparam.osc), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"OSC", "MA" );
}

void CStockGraph::DrawTechUOS( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.uos.AttachParameters( AfxGetProfile().GetTechParameters().uos );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.uos.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 2 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_UOS, &(m_techparam.uos), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"UOS", "MA" );
}

void CStockGraph::DrawTechMAOSC( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.maosc.AttachParameters( AfxGetProfile().GetTechParameters().maosc );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.maosc.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 2 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_MAOSC, &(m_techparam.maosc), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"MAOSC", "MA" );
}

void CStockGraph::DrawTech3_6BIAS( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.bias36.AttachParameters( AfxGetProfile().GetTechParameters().bias36 );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.bias36.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 2 ) )
		return;
	
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_36BIAS, &(m_techparam.bias36), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechDPO( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.dpo.AttachParameters( AfxGetProfile().GetTechParameters().dpo );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.dpo.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 2 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_DPO, &(m_techparam.dpo), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"DPO", "MA" );
}

void CStockGraph::DrawTechKST( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.kst.AttachParameters( AfxGetProfile().GetTechParameters().kst );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.kst.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 2 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_KST, &(m_techparam.kst), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"KST", "MA" );
}

void CStockGraph::DrawTechREI( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.rei.AttachParameters( AfxGetProfile().GetTechParameters().rei );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.rei.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 2 ) )
		return;
	
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_REI, &(m_techparam.rei), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechDMKI( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.dmki.AttachParameters( AfxGetProfile().GetTechParameters().dmki );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.dmki.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 2 ) )
		return;
	
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_DMKI, &(m_techparam.dmki), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechPCNT( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.pcnt.AttachParameters( AfxGetProfile().GetTechParameters().pcnt );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.pcnt.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 2 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_PCNT, &(m_techparam.pcnt), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"PCNT", "MA" );
}

void CStockGraph::DrawTechHLC( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.hlc.AttachParameters( AfxGetProfile().GetTechParameters().hlc );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.hlc.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 2 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_HLC, &(m_techparam.hlc), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"HLC", "MA" );
}

void CStockGraph::DrawTechCDP( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.cdp.AttachParameters( AfxGetProfile().GetTechParameters().cdp );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.cdp.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 2 ) )
		return;
	
	DrawTechUtil4(	pDC, rect, bDrawTitle, dMin, dMax, STT_CDP, &(m_techparam.cdp), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					AfxGetProfile().GetColor(CColorClass::clrLine3),
					AfxGetProfile().GetColor(CColorClass::clrLine4),
					"AH", "NH", "AL", "NL" );
}

void CStockGraph::DrawTechASI( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.asi.AttachParameters( AfxGetProfile().GetTechParameters().asi );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.asi.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_ASI, &(m_techparam.asi), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechATR( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.atr.AttachParameters( AfxGetProfile().GetTechParameters().atr );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.atr.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 2 ) )
		return;
	
	DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, STT_ATR, &(m_techparam.atr), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
}

void CStockGraph::DrawTechCYO( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.cyo.AttachParameters( AfxGetProfile().GetTechParameters().cyo );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.cyo.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_CYO, &(m_techparam.cyo), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"CYO", "MA");
}

void CStockGraph::DrawTechDCYO( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.dcyo.AttachParameters( AfxGetProfile().GetTechParameters().dcyo );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.dcyo.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_DCYO, &(m_techparam.dcyo), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"DCYO", "MA");
}

void CStockGraph::DrawTechHSL( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CHECK_NODATATODRAW
	DECLARE_COLOR_DEFINATION

	// Prepare
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.hsl.AttachParameters( AfxGetProfile().GetTechParameters().hsl );
	m_techparam.hsl.m_stockinfo	=	m_CurStock.GetStockInfo();
	m_techparam.volume.AttachParameters( AfxGetProfile().GetTechParameters().volume );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.hsl.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 2 ) )
		return;
	
	// Draw Title
	if( bDrawTitle )
	{
		DrawTechTitle( pDC, rect.left+5, rect.top+1, AfxGetSTTName(STT_HSL), TA_LEFT | TA_TOP, 14, clrBK, clrTitle );

		CFont	* pOldFont	=	AfxSelectDCFont( pDC, 12 );
		pDC->SetBkColor( clrBK );
		int	nIndexParam	=	( m_nIndexCurrent >= 0 ? m_nIndexCurrent : kdata.GetSize()-1 );
		double	dParam;
		CString	strText;
		for( int k=0; k<m_techparam.volume.m_adwMADays.GetSize(); k++ )
		{
			if( m_techparam.hsl.Calculate( &dParam, nIndexParam, m_techparam.volume.m_adwMADays[k], FALSE ) )
			{
				strText.Format( "%dMV %.2f", m_techparam.volume.m_adwMADays[k], dParam );
				pDC->SetTextColor( GetLineColor( k ) );
				if( rect.left+10+m_nWidthSubtitle+(k+1)*m_nWidthParameter > rect.right )
					break;
				pDC->TextOut( rect.left+12+m_nWidthSubtitle+k*m_nWidthParameter, rect.top+1, strText );
			}
		}
		pDC->SelectObject( pOldFont );
	}

	// Draw
	for( int k=0; k<m_techparam.volume.m_adwMADays.GetSize(); k++ )
	{
		int	yLast, yNow;
		BOOL	bHasLast = FALSE;
		double	dNow;
		for( int nIndex=m_nIndexStart; nIndex<=m_nIndexEnd; nIndex++ )
		{
			// Prepare Rect 
			CRect	rectK, rcEntity;
			long	xMedium	=	0;
			if( !GetOneKLineRect( nIndex, &rectK, &rcEntity.left, &rcEntity.right, &xMedium ) )
				continue;

			// Draw Entity
			if( 0 == k
				&& m_techparam.hsl.Calculate( &dNow, nIndex, FALSE ) )
			{
				// Calculate rcEntity.top and rcEntity.bottom
				KDATA	kd	=	kdata.ElementAt(nIndex);
				rcEntity.top	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle) * (dNow - dMin) / (dMax-dMin) );
				rcEntity.bottom	=	rect.bottom-1;

				COLORREF	clr	=	clrRise;
				if( kd.m_fClose < kd.m_fOpen )
					clr	=	clrFallEntity;
				if( kd.m_date > m_dwLatestDate )
					clr	=	clrNewKLine;
				pDC->SetBkColor( clrBK );
				if( kd.m_fClose < kd.m_fOpen )
					pDC->FillSolidRect( &rcEntity, clr );
				else
					pDC->Draw3dRect( &rcEntity, clr, clr );
			}

			// Draw MV
			if( m_techparam.hsl.Calculate( &dNow, nIndex, m_techparam.volume.m_adwMADays[k], FALSE ) )
			{
				yNow	=	int( rect.bottom - (rect.Height()-m_nHeightSubtitle) * (dNow  - dMin) / (dMax-dMin) );
				if( bHasLast )
				{
					if( yLast > rect.bottom || yLast < rect.top+m_nHeightSubtitle
						|| yNow > rect.bottom || yNow < rect.top+m_nHeightSubtitle )
						continue;
					CPen	penLocal( PS_SOLID, 1, GetLineColor(k) );
					CPen * pOldPen = pDC->SelectObject( &penLocal );
					pDC->MoveTo( xMedium-m_nThickness, yLast );
					pDC->LineTo( xMedium, yNow );
					pDC->SelectObject( pOldPen );
				}
				bHasLast	=	TRUE;
				yLast		=	yNow;
			}
		}
	}
}

void CStockGraph::DrawTechDPER( CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	m_techparam.dper.AttachParameters( AfxGetProfile().GetTechParameters().dper );

	double	dMin = 0, dMax = 0;
	if( !m_techparam.dper.GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, STT_DPER, &(m_techparam.dper), &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					"DPER", "MA");
}

void CStockGraph::DrawTechUser( UINT nTech, CDC * pDC, CRect rect, BOOL bDrawTitle )
{
	CTechUser	* pTechUser = NULL;
	for( int i=0; i<m_aptrTechUser.GetSize(); i++ )
	{
		CTechUser * p = (CTechUser *) m_aptrTechUser.GetAt(i);
		if( p->GetTechUserID() == nTech )
		{
			pTechUser	=	p;
			CKData * pKData = pTechUser->GetKData();
			if( !pKData || pKData->GetKType() != m_nCurKType )
				pTechUser	=	NULL;
		}
	}

	CKData	& kdata	=	m_CurStock.GetKData(m_nCurKType);
	if( NULL == pTechUser )
	{
		pTechUser	=	new CTechUser( &kdata );
		pTechUser->SetTechUserID( nTech );
		pTechUser->SetStock( &m_CurStock );
		m_aptrTechUser.Add( pTechUser );
	}

	double	dMin = 0, dMax = 0;
	if( !pTechUser->GetMinMaxInfo( m_nIndexStart, m_nIndexEnd, &dMin, &dMax )
		|| !DrawAxis( pDC, rect, m_nHeightSubtitle, 5, dMin, dMax, FALSE, TRUE, 0 ) )
		return;
	
	UINT nLineCount = pTechUser->GetLineCount();
	switch( nLineCount )
	{
	case 1:
		DrawTechUtil1(	pDC, rect, bDrawTitle, dMin, dMax, nTech, pTechUser, &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1), drawtechtype_line );
		break;
	case 2:
		DrawTechUtil2(	pDC, rect, bDrawTitle, dMin, dMax, nTech, pTechUser, &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					(LPCTSTR)pTechUser->GetLineName(1), (LPCTSTR)pTechUser->GetLineName(2) );
		break;
	case 3:
		DrawTechUtil3(	pDC, rect, bDrawTitle, dMin, dMax, nTech, pTechUser, &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					AfxGetProfile().GetColor(CColorClass::clrLine3),
					(LPCTSTR)pTechUser->GetLineName(1), (LPCTSTR)pTechUser->GetLineName(2),
					(LPCTSTR)pTechUser->GetLineName(3) );
		break;
	case 4:
		DrawTechUtil4(	pDC, rect, bDrawTitle, dMin, dMax, nTech, pTechUser, &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					AfxGetProfile().GetColor(CColorClass::clrLine3),
					AfxGetProfile().GetColor(CColorClass::clrLine4),
					(LPCTSTR)pTechUser->GetLineName(1), (LPCTSTR)pTechUser->GetLineName(2),
					(LPCTSTR)pTechUser->GetLineName(3), (LPCTSTR)pTechUser->GetLineName(4) );
		break;
	case 5:
		DrawTechUtil5(	pDC, rect, bDrawTitle, dMin, dMax, nTech, pTechUser, &kdata,
					AfxGetProfile().GetColor(CColorClass::clrLine1),
					AfxGetProfile().GetColor(CColorClass::clrLine2),
					AfxGetProfile().GetColor(CColorClass::clrLine3),
					AfxGetProfile().GetColor(CColorClass::clrLine4),
					AfxGetProfile().GetColor(CColorClass::clrLine5),
					(LPCTSTR)pTechUser->GetLineName(1), (LPCTSTR)pTechUser->GetLineName(2),
					(LPCTSTR)pTechUser->GetLineName(3), (LPCTSTR)pTechUser->GetLineName(4),
					(LPCTSTR)pTechUser->GetLineName(5) );
		break;
	default:;
	}
}
