// MultiSort.cpp : implementation of the CMultiSort class
//

#include "stdafx.h"

#include "MultiSort.h"
#include "../Dialog/EditKDATADlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiSort

CMultiSort::CMultiSort( )
{
	m_pParent		=	NULL;
	m_lMultiSortClass	=	CStock::typeshA;

	m_nSelectArea	=	areaNone;
	m_nSelectPos	=	-1;

	m_nRefreshMode	=	modeSort;
}

CMultiSort::~CMultiSort( )
{
}

void CMultiSort::SetParent( CWnd * pParent )
{
	m_pParent	=	pParent;
}

void CMultiSort::SetMultiSortClass( LONG type )
{
	m_lMultiSortClass	=	type;
	if( modeDirect == m_nRefreshMode )
	{
		CStockInfo info;
		info.SetType( GetMultiSortClass( ) );
		AfxGetStkReceiver().RequestStockData( CStock::dataMultisort, &info, 1, 0, 0 );
	}
	else
	{
		RecalculateSortID( TRUE, TRUE );
	}
}

LONG CMultiSort::GetMultiSortClass( )
{
	return m_lMultiSortClass;
}

BOOL CMultiSort::RecalculateSortID( BOOL bReloadMinute, BOOL bUpdateWindow )
{
	BOOL	bModified	=	FALSE;
	// 备份，以判断是否改变
	CSPDWordArray	aRise, aFall, aDiff, aRiseMin5, aFallMin5, aVolRatio, aBSRatioAsc, aBSRatioDesc, aAmount;
	aRise.Copy( m_adwRise );
	aFall.Copy( m_adwFall );
	aDiff.Copy( m_adwDiff );
	aRiseMin5.Copy( m_adwRiseMin5 );
	aFallMin5.Copy( m_adwFallMin5 );
	aVolRatio.Copy( m_adwVolRatio );
	aBSRatioAsc.Copy( m_adwBSRatioAsc );
	aBSRatioDesc.Copy( m_adwBSRatioDesc );
	aAmount.Copy( m_adwAmount );

	BOOL	bOK	=	TRUE;
	if( !AfxGetStockContainer().GetMultiSortIDArray( m_adwRise, m_lMultiSortClass, SLH_DIFFPERCENT, FALSE, 6 ) )
		bOK	=	FALSE;

	if( !AfxGetStockContainer().GetMultiSortIDArray( m_adwFall, m_lMultiSortClass, SLH_DIFFPERCENT, TRUE, 6 ) )
		bOK	=	FALSE;

	if( !AfxGetStockContainer().GetMultiSortIDArray( m_adwDiff, m_lMultiSortClass, SLH_SCOPE, FALSE, 6 ) )
		bOK	=	FALSE;

	if( bReloadMinute )
	{
		AfxRefreshStockContainerMinute( AfxGetStockContainer(), m_lMultiSortClass, 1000 );
	}
	
	if( !AfxGetStockContainer().GetMultiSortIDArray( m_adwRiseMin5, m_lMultiSortClass, SLH_DIFFPERCENT_MIN5, FALSE, 6 ) )
		bOK	=	FALSE;
	if( !AfxGetStockContainer().GetMultiSortIDArray( m_adwFallMin5, m_lMultiSortClass, SLH_DIFFPERCENT_MIN5, TRUE, 6 ) )
		bOK	=	FALSE;

	if( !AfxGetStockContainer().GetMultiSortIDArray( m_adwVolRatio, m_lMultiSortClass, SLH_RATIO_VOLUME, FALSE, 6 ) )
		bOK	=	FALSE;

	if( !AfxGetStockContainer().GetMultiSortIDArray( m_adwBSRatioAsc, m_lMultiSortClass, SLH_SELLBUYRATIO, FALSE, 6 ) )
		bOK	=	FALSE;

	if( !AfxGetStockContainer().GetMultiSortIDArray( m_adwBSRatioDesc, m_lMultiSortClass, SLH_SELLBUYRATIO, TRUE, 6 ) )
		bOK	=	FALSE;

	if( !AfxGetStockContainer().GetMultiSortIDArray( m_adwAmount, m_lMultiSortClass, SLH_AMOUNT, FALSE, 6 ) )
		bOK	=	FALSE;

	if( !bUpdateWindow )
		return TRUE;

	if( !m_pParent || !::IsWindow(m_pParent->GetSafeHwnd()) )
		return TRUE;

	if( !aRise.IsEqualTo( m_adwRise ) )
		m_pParent->InvalidateRect( m_rectRise );
	if( !aFall.IsEqualTo( m_adwFall ) )
		m_pParent->InvalidateRect( m_rectFall );
	if( !aDiff.IsEqualTo( m_adwDiff ) )
		m_pParent->InvalidateRect( m_rectDiff );
	if( !aRiseMin5.IsEqualTo( m_adwRiseMin5 ) )
		m_pParent->InvalidateRect( m_rectRiseMin5 );
	if( !aFallMin5.IsEqualTo( m_adwFallMin5 ) )
		m_pParent->InvalidateRect( m_rectFallMin5 );
	if( !aVolRatio.IsEqualTo( m_adwVolRatio ) )
		m_pParent->InvalidateRect( m_rectVolRatio );
	if( !aBSRatioAsc.IsEqualTo( m_adwBSRatioAsc ) )
		m_pParent->InvalidateRect( m_rectBSRatioAsc );
	if( !aBSRatioDesc.IsEqualTo( m_adwBSRatioDesc ) )
		m_pParent->InvalidateRect( m_rectBSRatioDesc );
	if( !aAmount.IsEqualTo( m_adwAmount ) )
		m_pParent->InvalidateRect( m_rectAmount );
	
	return bModified;
}

BOOL CMultiSort::RecalculateSortID( PCOMMPACKET pPacket )
{
	// 备份，以判断是否改变
	MULTISORT	msRise, msFall, msDiff, msRiseMin5, msFallMin5, msVolRatio, msBSRatioAsc, msBSRatioDesc, msAmount;
	msRise	=	m_msRise;
	msFall	=	m_msFall;
	msDiff	=	m_msDiff;
	msRiseMin5		=	m_msRiseMin5;
	msFallMin5		=	m_msFallMin5;
	msVolRatio		=	m_msVolRatio;
	msBSRatioAsc	=	m_msBSRatioAsc;
	msBSRatioDesc	=	m_msBSRatioDesc;
	msAmount		=	m_msAmount;

	size_t i;
	for( i=0; i<pPacket->m_dwCount; i++ )
	{
		switch( pPacket->m_pMultisort[i].m_class )
		{
		case CStock::classRise:
			memcpy( &m_msRise, &(pPacket->m_pMultisort[i]), sizeof(MULTISORT) );
			break;
		case CStock::classFall:
			memcpy( &m_msFall, &(pPacket->m_pMultisort[i]), sizeof(MULTISORT) );
			break;
		case CStock::claddRiseMin5:
			memcpy( &m_msRiseMin5, &(pPacket->m_pMultisort[i]), sizeof(MULTISORT) );
			break;
		case CStock::classFallMin5:
			memcpy( &m_msFallMin5, &(pPacket->m_pMultisort[i]), sizeof(MULTISORT) );
			break;
		case CStock::classBSRatioAsc:
			memcpy( &m_msBSRatioAsc, &(pPacket->m_pMultisort[i]), sizeof(MULTISORT) );
			break;
		case CStock::classBSRatioDesc:
			memcpy( &m_msBSRatioDesc, &(pPacket->m_pMultisort[i]), sizeof(MULTISORT) );
			break;
		case CStock::classDiff:
			memcpy( &m_msDiff, &(pPacket->m_pMultisort[i]), sizeof(MULTISORT) );
			break;		
		case CStock::classVolRatio:
			memcpy( &m_msVolRatio, &(pPacket->m_pMultisort[i]), sizeof(MULTISORT) );
			break;
		case CStock::classAmount:
			memcpy( &m_msAmount, &(pPacket->m_pMultisort[i]), sizeof(MULTISORT) );
			break;
		default:;
		}
	}

	if( !m_pParent || !::IsWindow(m_pParent->GetSafeHwnd()) )
		return TRUE;

	if( 0 != memcmp(&msRise, &m_msRise,sizeof(MULTISORT)) )
		m_pParent->InvalidateRect( m_rectRise );
	if( 0 != memcmp(&msFall,&m_msFall,sizeof(MULTISORT)) )
		m_pParent->InvalidateRect( m_rectFall );
	if( 0 != memcmp(&msDiff,&m_msDiff,sizeof(MULTISORT)) )
		m_pParent->InvalidateRect( m_rectDiff );
	if( 0 != memcmp(&msRiseMin5,&m_msRiseMin5,sizeof(MULTISORT)) )
		m_pParent->InvalidateRect( m_rectRiseMin5 );
	if( 0 != memcmp(&msFallMin5,&m_msFallMin5,sizeof(MULTISORT)) )
		m_pParent->InvalidateRect( m_rectFallMin5 );
	if( 0 != memcmp(&msVolRatio,&m_msVolRatio,sizeof(MULTISORT)) )
		m_pParent->InvalidateRect( m_rectVolRatio );
	if( 0 != memcmp(&msBSRatioAsc,&m_msBSRatioAsc,sizeof(MULTISORT)) )
		m_pParent->InvalidateRect( m_rectBSRatioAsc );
	if( 0 != memcmp(&msBSRatioDesc,&m_msBSRatioDesc,sizeof(MULTISORT)) )
		m_pParent->InvalidateRect( m_rectBSRatioDesc );
	if( 0 != memcmp(&msAmount,&m_msAmount,sizeof(MULTISORT)) )
		m_pParent->InvalidateRect( m_rectAmount );
	
	return TRUE;
}

void CMultiSort::Redraw( CDC * pDC, CRect rectAll )
{
	ASSERT( m_pParent && ::IsWindow( m_pParent->GetSafeHwnd() ) );
	if( !m_pParent || !::IsWindow(m_pParent->GetSafeHwnd()) )
		return;
	
	ResetClient( rectAll );

	CClientDC	dc(m_pParent);
	if( NULL == pDC )
		pDC		=	&dc;

	DrawRise( pDC, m_rectRise );
	DrawFall( pDC, m_rectFall );
	DrawDiff( pDC, m_rectDiff );
	DrawRiseMin5( pDC, m_rectRiseMin5 );
	DrawFallMin5( pDC, m_rectFallMin5 );
	DrawVolRatio( pDC, m_rectVolRatio );
	DrawBSRatioAsc( pDC, m_rectBSRatioAsc );
	DrawBSRatioDesc( pDC, m_rectBSRatioDesc );
	DrawAmount( pDC, m_rectAmount );
}

LRESULT CMultiSort::OnStkReceiverData( WPARAM wParam, LPARAM lParam )
{
	PCOMMPACKET	pCommPacket	=	(PCOMMPACKET)lParam;

	switch( wParam )
	{
	case CStock::dataMultisort:
		m_nRefreshMode	=	modeDirect;
		RecalculateSortID( pCommPacket );
		break;
	}
	return 0L;
}

void CMultiSort::OnLButtonDown( UINT nFlags, CPoint point, BOOL bShiftPressed, BOOL bCtrlPressed )
{
	DECLARE_COLOR_DEFINATION

	CClientDC	dc(m_pParent);
	DrawSelection( &dc, m_nSelectArea, m_nSelectPos, clrBK );

	if( -1 != (m_nSelectPos=GetClickPos(m_rectRise,point)) )
	{
		m_nSelectArea	=	areaRise;
	}
	else if( -1 != (m_nSelectPos=GetClickPos(m_rectFall,point)) )
	{
		m_nSelectArea	=	areaFall;
	}
	else if( -1 != (m_nSelectPos=GetClickPos(m_rectDiff,point)) )
	{
		m_nSelectArea	=	areaDiff;
	}
	else if( -1 != (m_nSelectPos=GetClickPos(m_rectRiseMin5,point)) )
	{
		m_nSelectArea	=	areaRiseMin5;
	}
	else if( -1 != (m_nSelectPos=GetClickPos(m_rectFallMin5,point)) )
	{
		m_nSelectArea	=	areaFallMin5;
	}
	else if( -1 != (m_nSelectPos=GetClickPos(m_rectVolRatio,point)) )
	{
		m_nSelectArea	=	areaVolRatio;
	}
	else if( -1 != (m_nSelectPos=GetClickPos(m_rectBSRatioAsc,point)) )
	{
		m_nSelectArea	=	areaBSRatioAsc;
	}
	else if( -1 != (m_nSelectPos=GetClickPos(m_rectBSRatioDesc,point)) )
	{
		m_nSelectArea	=	areaBSRatioDesc;
	}
	else if( -1 != (m_nSelectPos=GetClickPos(m_rectAmount,point)) )
	{
		m_nSelectArea	=	areaAmount;
	}

	DrawSelection( &dc, m_nSelectArea, m_nSelectPos, clrBorder );
}


void CMultiSort::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	OnLButtonDown( nFlags, point, FALSE, FALSE );

	if( m_nSelectPos < 0 )
		return;

	int	nSelectID	=	-1;
	if( modeDirect == m_nRefreshMode )
	{
		int size = sizeof(m_msRise.m_stocks)/sizeof(m_msRise.m_stocks[0]);
		char	buf_code[sizeof(m_msRise.m_stocks[0].m_code)+1];
		memset( buf_code, 0, sizeof(buf_code) );

		if( areaRise == m_nSelectArea && m_nSelectPos < size )
			memcpy( buf_code, m_msRise.m_stocks[m_nSelectPos].m_code, sizeof(m_msRise.m_stocks[0].m_code) );
		else if( areaFall == m_nSelectArea && m_nSelectPos < size )
			memcpy( buf_code, m_msFall.m_stocks[m_nSelectPos].m_code, sizeof(m_msRise.m_stocks[0].m_code) );
		else if( areaDiff == m_nSelectArea && m_nSelectPos < size )
			memcpy( buf_code, m_msDiff.m_stocks[m_nSelectPos].m_code, sizeof(m_msRise.m_stocks[0].m_code) );
		else if( areaRiseMin5 == m_nSelectArea && m_nSelectPos < size )
			memcpy( buf_code, m_msRiseMin5.m_stocks[m_nSelectPos].m_code, sizeof(m_msRise.m_stocks[0].m_code) );
		else if( areaFallMin5 == m_nSelectArea && m_nSelectPos < size )
			memcpy( buf_code, m_msFallMin5.m_stocks[m_nSelectPos].m_code, sizeof(m_msRise.m_stocks[0].m_code) );
		else if( areaVolRatio == m_nSelectArea && m_nSelectPos < size )
			memcpy( buf_code, m_msVolRatio.m_stocks[m_nSelectPos].m_code, sizeof(m_msRise.m_stocks[0].m_code) );
		else if( areaBSRatioAsc == m_nSelectArea && m_nSelectPos < size )
			memcpy( buf_code, m_msBSRatioAsc.m_stocks[m_nSelectPos].m_code, sizeof(m_msRise.m_stocks[0].m_code) );
		else if( areaBSRatioDesc == m_nSelectArea && m_nSelectPos < size )
			memcpy( buf_code, m_msBSRatioDesc.m_stocks[m_nSelectPos].m_code, sizeof(m_msRise.m_stocks[0].m_code) );
		else if( areaAmount == m_nSelectArea && m_nSelectPos < size )
			memcpy( buf_code, m_msAmount.m_stocks[m_nSelectPos].m_code, sizeof(m_msRise.m_stocks[0].m_code) );
		else
			return;
		if( strlen(buf_code) > 0 )
			AfxGetStockContainer().GetStockInfo( buf_code, NULL, &nSelectID );
	}
	else
	{
		if( areaRise == m_nSelectArea && m_nSelectPos < m_adwRise.GetSize() )
			nSelectID		=	m_adwRise[m_nSelectPos];
		else if( areaFall == m_nSelectArea && m_nSelectPos < m_adwFall.GetSize() )
			nSelectID		=	m_adwFall[m_nSelectPos];
		else if( areaDiff == m_nSelectArea && m_nSelectPos < m_adwDiff.GetSize() )
			nSelectID		=	m_adwDiff[m_nSelectPos];
		else if( areaRiseMin5 == m_nSelectArea && m_nSelectPos < m_adwRiseMin5.GetSize() )
			nSelectID		=	m_adwRiseMin5[m_nSelectPos];
		else if( areaFallMin5 == m_nSelectArea && m_nSelectPos < m_adwFallMin5.GetSize() )
			nSelectID		=	m_adwFallMin5[m_nSelectPos];
		else if( areaVolRatio == m_nSelectArea && m_nSelectPos < m_adwVolRatio.GetSize() )
			nSelectID		=	m_adwVolRatio[m_nSelectPos];
		else if( areaBSRatioAsc == m_nSelectArea && m_nSelectPos < m_adwBSRatioAsc.GetSize() )
			nSelectID		=	m_adwBSRatioAsc[m_nSelectPos];
		else if( areaBSRatioDesc == m_nSelectArea && m_nSelectPos < m_adwBSRatioDesc.GetSize() )
			nSelectID		=	m_adwBSRatioDesc[m_nSelectPos];
		else if( areaAmount == m_nSelectArea && m_nSelectPos < m_adwAmount.GetSize() )
			nSelectID		=	m_adwAmount[m_nSelectPos];
		else
			return;
	}

	// 选中股票
	if( nSelectID >= 0 && nSelectID < AfxGetStockContainer().GetSize() )
	{
		AfxShowStockRealTime( nSelectID, FALSE );
	}
}

void CMultiSort::ResetClient( CRect rectAll )
{
	m_rectAll	=	rectAll;

	int	x = m_rectAll.left;
	int	y = m_rectAll.top;
	int width = m_rectAll.Width();
	int height = m_rectAll.Height();

	// 今日涨幅排名区域
	m_rectRise	=	CRect( x, y, x+width/3, y+height/3 );
	
	// 今日跌幅排名区域
	m_rectFall	=	CRect( x, y+height/3, x+width/3, y+2*height/3 );

	// 今日震幅排名区域
	m_rectDiff	=	CRect( x, y+2*height/3, x+width/3, m_rectAll.bottom );
	
	// 5分钟涨幅排名区域
	m_rectRiseMin5	=	CRect( x+width/3, y, x+2*width/3, y+height/3 );

	// 5分钟跌幅排名区域
	m_rectFallMin5	=	CRect( x+width/3, y+height/3, x+2*width/3, y+2*height/3 );

	// 5分钟震幅排名区域
	m_rectVolRatio	=	CRect( x+width/3, y+2*height/3, x+2*width/3, m_rectAll.bottom );
	
	// 今日委比前六名区域
	m_rectBSRatioAsc	=	CRect( x+2*width/3, y, m_rectAll.right, y+height/3 );
	
	// 今日委比后六名区域
	m_rectBSRatioDesc	=	CRect( x+2*width/3, y+height/3, m_rectAll.right, y+2*height/3 );
	
	// 今日总金额排名区域
	m_rectAmount		=	CRect( x+2*width/3, y+2*height/3, m_rectAll.right, m_rectAll.bottom );
}

int CMultiSort::GetClickPos( CRect rect, CPoint point )
{
	if( !rect.PtInRect(point) )
		return -1;

	int	y	=	rect.top+20;
	int	yStep	=	(rect.Height()-23) / 6 ;
	if( yStep <= 0 )
		return -1;

	int nClickPos	=	-1;
	for( int nIndex=0; nIndex<6; nIndex ++ )
	{
		CRect	rectElement	=	rect;
		rectElement.top		=	y + yStep * nIndex;
		rectElement.bottom	=	y + yStep * (nIndex+1);
		if( rectElement.PtInRect(point) )
		{
			nClickPos	=	nIndex;
			break;
		}
	}
	return nClickPos;
}

void CMultiSort::DrawSelection( CDC * pDC, int nSelectArea, int nSelectPos, COLORREF clr )
{
	CRect	rect(0,0,0,0);
	if( -1 == nSelectPos )
		return;

	if( areaRise == nSelectArea )
		rect		=	m_rectRise;
	else if( areaFall == nSelectArea )
		rect		=	m_rectFall;
	else if( areaDiff == nSelectArea )
		rect		=	m_rectDiff;
	else if( areaRiseMin5 == nSelectArea )
		rect		=	m_rectRiseMin5;
	else if( areaFallMin5 == nSelectArea )
		rect		=	m_rectFallMin5;
	else if( areaVolRatio == nSelectArea )
		rect		=	m_rectVolRatio;
	else if( areaBSRatioAsc == nSelectArea )
		rect		=	m_rectBSRatioAsc;
	else if( areaBSRatioDesc == nSelectArea )
		rect		=	m_rectBSRatioDesc;
	else if( areaAmount == nSelectArea )
		rect		=	m_rectAmount;
	else
		return;

	int	y	=	rect.top+20;
	int	yStep	=	(rect.Height()-23) / 6 ;
	if( yStep <= 0 )
		return;

	int ySel = y + yStep*nSelectPos + 16;
	if( ySel > rect.top && ySel < rect.bottom )
		DrawLine( pDC, 2, clr, rect.left+5, ySel, rect.right-5, ySel );
}

void DrawBorderAndTitle( CDC * pDC, CRect rect, LPCTSTR lpszTitle )
{
	DECLARE_COLOR_DEFINATION

	// Draw Border
	CRect	rectTitle	=	CRect( rect.left, rect.top, rect.right, rect.top+18 );
	pDC->Draw3dRect( &rect, clrBorder, clrBorder );
	pDC->Draw3dRect( &rectTitle, clrBorder, clrBorder );

	// Draw Title
	CFont	* pOldFont	=	AfxSelectDCFont( pDC, 14 );
	pDC->SetBkColor( clrBK );
	pDC->SetTextColor( clrText );
	pDC->DrawText( lpszTitle, rectTitle, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	pDC->SelectObject( pOldFont );
}

void CMultiSort::DrawMultiSort( CDC * pDC, CSPDWordArray &adwSortID, CRect rect, UINT nSLH, int nSelectPos )
{
	DECLARE_COLOR_DEFINATION

	int	x	=	rect.left+8;
	int	x2	=	(int)( rect.left+0.56*rect.Width() );
	int	x3	=	rect.right-8;
	int	y	=	rect.top+20;
	int	yStep	=	(rect.Height()-23) / 6 ;
	if( yStep <= 0 )
		return;
	
	CFont	* pOldFont	=	AfxSelectDCFont( pDC, 14 );
	pDC->SetBkColor( clrBK );

	for( int i=0; i<adwSortID.GetSize(); i++ )
	{
		if( (int)adwSortID[i] < 0 || (int)adwSortID[i] >= AfxGetStockContainer().GetSize() )
			continue;

		CStockInfo	& info	=	AfxGetStockContainer().ElementAt(adwSortID[i]);

		pDC->SetTextColor( clrText );
//#ifdef	CLKLAN_ENGLISH_US
//		pDC->TextOut( x, y, info.GetStockCode() );
//#else
		pDC->TextOut( x, y, info.GetStockName() );
//#endif

		UINT nOldAlign = pDC->SetTextAlign( TA_RIGHT | TA_TOP );
		pDC->SetTextColor( AfxGetVariantColor(SLH_CLOSE,info) );
		pDC->TextOut( x2, y, (LPCTSTR)AfxGetVariantDispString(SLH_CLOSE,info,NULL) );

		pDC->SetTextColor( AfxGetVariantColor(nSLH,info) );
		if( SLH_RATIO_VOLUME == nSLH )
		{
			double	dVolRatio	=	1;
			CSPTime	tTrade;
			if( AfxGetVariantValue(nSLH,info,&dVolRatio,NULL)
				&& tTrade.FromStockTimeDay( info.m_datetech ) )
				dVolRatio = dVolRatio / CSPTime::GetTimeTradeRatioOfOneDay( tTrade, CSPTime::GetCurrentTime() );

			CString	strTemp;
			strTemp.Format( "%.2f", dVolRatio );
			pDC->TextOut( x3, y, strTemp );
		}
		else
		{
			pDC->TextOut( x3, y, (LPCTSTR)AfxGetVariantDispString(nSLH,info,NULL) );
		}

		y	+=	yStep;
		pDC->SetTextAlign( nOldAlign );

		// 选中股票
		if( nSelectPos == i )
		{
			int	ySel = y-yStep+16;
			if( ySel > rect.top && ySel < rect.bottom )
				DrawLine( pDC, 2, clrBorder, rect.left+5, ySel, rect.right-5, ySel );
		}
	}

	// 补足股票排名
	for( int i=adwSortID.GetSize(); i<6; i++ )
	{
		pDC->SetTextColor( clrText );
		pDC->TextOut( x, y, "-" );

		UINT nOldAlign = pDC->SetTextAlign( TA_RIGHT | TA_TOP );
		pDC->TextOut( x2, y, "-" );

		pDC->TextOut( x3, y, "-" );

		y	+=	yStep;
		pDC->SetTextAlign( nOldAlign );
	}

	pDC->SelectObject( pOldFont );
}

void CMultiSort::DrawMultiSort( CDC * pDC, MULTISORT &ms, CRect rect, UINT nSLH, int nSelectPos )
{
	DECLARE_COLOR_DEFINATION

	int	x	=	rect.left+8;
	int	x2	=	(int)( rect.left+0.56*rect.Width() );
	int	x3	=	rect.right-8;
	int	y	=	rect.top+20;
	int	yStep	=	(rect.Height()-23) / 6 ;
	if( yStep <= 0 )
		return;
	
	CFont	* pOldFont	=	AfxSelectDCFont( pDC, 14 );
	pDC->SetBkColor( clrBK );

	int size = sizeof(m_msRise.m_stocks)/sizeof(m_msRise.m_stocks[0]);
	for( int i=0; i<size; i++ )
	{
		CStockInfo	info;
		char	buf_code[sizeof(ms.m_stocks[i].m_code)+1];
		memset( buf_code, 0, sizeof(buf_code) );
		memcpy( buf_code, ms.m_stocks[i].m_code, sizeof(ms.m_stocks[i].m_code) );
		if( !AfxGetStockContainer().GetStockInfo(buf_code, &info) )
			continue;
		info.m_fClose	=	float(0.001*ms.m_stocks[i].m_data1);

		pDC->SetTextColor( clrText );
//#ifdef	CLKLAN_ENGLISH_US
//		pDC->TextOut( x, y, info.GetStockCode() );
//#else
		pDC->TextOut( x, y, info.GetStockName() );
//#endif

		UINT nOldAlign = pDC->SetTextAlign( TA_RIGHT | TA_TOP );
		pDC->SetTextColor( AfxGetVariantColor(SLH_CLOSE,info) );
		pDC->TextOut( x2, y, (LPCTSTR)AfxGetVariantDispString(SLH_CLOSE,info,NULL) );

		CString strTemp;
		if( CStock::classVolRatio == ms.m_class )
			strTemp.Format( "%.2f", 0.01*ms.m_stocks[i].m_data2 );
		else if( CStock::classAmount == ms.m_class )
			strTemp.Format( "%.0f", 0.001*ms.m_stocks[i].m_data2 );
		else
			strTemp.Format( "%.2f%%", 0.01*ms.m_stocks[i].m_data2 );
		pDC->TextOut( x3, y, strTemp );

		y	+=	yStep;
		pDC->SetTextAlign( nOldAlign );

		// 选中股票
		if( nSelectPos == i )
		{
			int	ySel = y-yStep+16;
			if( ySel > rect.top && ySel < rect.bottom )
				DrawLine( pDC, 2, clrBorder, rect.left+5, ySel, rect.right-5, ySel );
		}
	}

	// 补足股票排名
	for( int i=size; i<6; i++ )
	{
		pDC->SetTextColor( clrText );
		pDC->TextOut( x, y, "-" );

		UINT nOldAlign = pDC->SetTextAlign( TA_RIGHT | TA_TOP );
		pDC->TextOut( x2, y, "-" );

		pDC->TextOut( x3, y, "-" );

		y	+=	yStep;
		pDC->SetTextAlign( nOldAlign );
	}

	pDC->SelectObject( pOldFont );
}

// 今日涨幅排名
void CMultiSort::DrawRise( CDC * pDC, CRect rect )
{
	CString	strTitle;
	strTitle.LoadString( IDS_MULTISORT_RISE );
	DrawBorderAndTitle( pDC, rect, strTitle );

	if( modeDirect == m_nRefreshMode )
		DrawMultiSort( pDC, m_msRise, rect, SLH_DIFFPERCENT, areaRise == m_nSelectArea ? m_nSelectPos : -1 );
	else
		DrawMultiSort( pDC, m_adwRise, rect, SLH_DIFFPERCENT, areaRise == m_nSelectArea ? m_nSelectPos : -1 );
}

// 今日跌幅排名
void CMultiSort::DrawFall( CDC * pDC, CRect rect )
{
	CString	strTitle;
	strTitle.LoadString( IDS_MULTISORT_FALL );
	DrawBorderAndTitle( pDC, rect, strTitle );

	if( modeDirect == m_nRefreshMode )
		DrawMultiSort( pDC, m_msFall, rect, SLH_DIFFPERCENT, areaFall == m_nSelectArea ? m_nSelectPos : -1 );
	else
		DrawMultiSort( pDC, m_adwFall, rect, SLH_DIFFPERCENT, areaFall == m_nSelectArea ? m_nSelectPos : -1 );
}

// 今日震幅排名
void CMultiSort::DrawDiff( CDC * pDC, CRect rect )
{
	CString	strTitle;
	strTitle.LoadString( IDS_MULTISORT_DIFF );
	DrawBorderAndTitle( pDC, rect, strTitle );

	if( modeDirect == m_nRefreshMode )
		DrawMultiSort( pDC, m_msDiff, rect, SLH_SCOPE, areaDiff == m_nSelectArea ? m_nSelectPos : -1 );
	else
		DrawMultiSort( pDC, m_adwDiff, rect, SLH_SCOPE, areaDiff == m_nSelectArea ? m_nSelectPos : -1 );
}

// 5分钟涨幅排名
void CMultiSort::DrawRiseMin5( CDC * pDC, CRect rect )
{
	CString	strTitle;
	strTitle.LoadString( IDS_MULTISORT_RISEMIN5 );
	DrawBorderAndTitle( pDC, rect, strTitle );

	if( modeDirect == m_nRefreshMode )
		DrawMultiSort( pDC, m_msRiseMin5, rect, SLH_DIFFPERCENT_MIN5, areaRiseMin5 == m_nSelectArea ? m_nSelectPos : -1 );
	else
		DrawMultiSort( pDC, m_adwRiseMin5, rect, SLH_DIFFPERCENT_MIN5, areaRiseMin5 == m_nSelectArea ? m_nSelectPos : -1 );
}

// 5分钟跌幅排名
void CMultiSort::DrawFallMin5( CDC * pDC, CRect rect )
{
	CString	strTitle;
	strTitle.LoadString( IDS_MULTISORT_FALLMIN5 );
	DrawBorderAndTitle( pDC, rect, strTitle );

	if( modeDirect == m_nRefreshMode )
		DrawMultiSort( pDC, m_msFallMin5, rect, SLH_DIFFPERCENT_MIN5, areaFallMin5 == m_nSelectArea ? m_nSelectPos : -1 );
	else
		DrawMultiSort( pDC, m_adwFallMin5, rect, SLH_DIFFPERCENT_MIN5, areaFallMin5 == m_nSelectArea ? m_nSelectPos : -1 );
}

// 今日量比排名
void CMultiSort::DrawVolRatio( CDC * pDC, CRect rect )
{
	CString	strTitle;
	strTitle.LoadString( IDS_MULTISORT_VOLRATIO );
	DrawBorderAndTitle( pDC, rect, strTitle );

	if( modeDirect == m_nRefreshMode )
		DrawMultiSort( pDC, m_msVolRatio, rect, SLH_RATIO_VOLUME, areaVolRatio == m_nSelectArea ? m_nSelectPos : -1 );
	else
		DrawMultiSort( pDC, m_adwVolRatio, rect, SLH_RATIO_VOLUME, areaVolRatio == m_nSelectArea ? m_nSelectPos : -1 );
}

// 今日委比前六名
void CMultiSort::DrawBSRatioAsc( CDC * pDC, CRect rect )
{
	CString	strTitle;
	strTitle.LoadString( IDS_MULTISORT_BSRATIOASC );
	DrawBorderAndTitle( pDC, rect, strTitle );

	if( modeDirect == m_nRefreshMode )
		DrawMultiSort( pDC, m_msBSRatioAsc, rect, SLH_SELLBUYRATIO, areaBSRatioAsc == m_nSelectArea ? m_nSelectPos : -1 );
	else
		DrawMultiSort( pDC, m_adwBSRatioAsc, rect, SLH_SELLBUYRATIO, areaBSRatioAsc == m_nSelectArea ? m_nSelectPos : -1 );
}

// 今日委比后六名
void CMultiSort::DrawBSRatioDesc( CDC * pDC, CRect rect )
{
	CString	strTitle;
	strTitle.LoadString( IDS_MULTISORT_BSRATIODESC );
	DrawBorderAndTitle( pDC, rect, strTitle );

	if( modeDirect == m_nRefreshMode )
		DrawMultiSort( pDC, m_msBSRatioDesc, rect, SLH_SELLBUYRATIO, areaBSRatioDesc == m_nSelectArea ? m_nSelectPos : -1 );
	else
		DrawMultiSort( pDC, m_adwBSRatioDesc, rect, SLH_SELLBUYRATIO, areaBSRatioDesc == m_nSelectArea ? m_nSelectPos : -1 );
}

// 今日总金额排名
void CMultiSort::DrawAmount( CDC * pDC, CRect rect )
{
	CString	strTitle;
	strTitle.LoadString( IDS_MULTISORT_AMOUNT );
	DrawBorderAndTitle( pDC, rect, strTitle );

	if( modeDirect == m_nRefreshMode )
		DrawMultiSort( pDC, m_msAmount, rect, SLH_AMOUNT, areaAmount == m_nSelectArea ? m_nSelectPos : -1 );
	else
		DrawMultiSort( pDC, m_adwAmount, rect, SLH_AMOUNT, areaAmount == m_nSelectArea ? m_nSelectPos : -1 );
}

