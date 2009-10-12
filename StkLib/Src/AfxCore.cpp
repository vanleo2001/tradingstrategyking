/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		some afx-global-functions;
*/

#include	"stdafx.h"
#include	"../Include/AfxCore.h"

#include	<ctype.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////////
// Chinese To English Translation

BOOL AfxIsEnglish( const char * sz, int maxlen, float ratio )
{
	UINT nAll = 0, nChCount = 0;
	int len = 0;
	while( *sz && len < maxlen )
	{
		if( IsDBCSLeadByte(BYTE(*sz)) )
			nChCount ++;
		nAll ++;
		sz ++;
		len ++;
	}
	if( ratio && nAll && nChCount )
	{
		return ((float)nChCount/(float)nAll) < ratio;
	}

	return 0 == nChCount;
}

////////////////////////////////////////////////////////////////////////////////////
// global data
CStProfile &AfxGetProfile()
{
	static	CStProfile		g_stprofile;
	return g_stprofile;
}

CStockContainer &AfxGetStockContainer( )
{
	static	CStockContainer		g_stockcontainer;
	return g_stockcontainer;
}

CStockContainer &AfxGetSListStockContainer( )
{
	static	CStockContainer		g_sliststockcontainer;
	return g_sliststockcontainer;
}

CDomainContainer &AfxGetDomainContainer( )
{
	static	CDomainContainer	g_domaincontainer;
	return g_domaincontainer;
}

CDomainContainer &AfxGetGroupContainer( )
{
	static	CDomainContainer	g_groupcontainer;
	if( g_groupcontainer.GetSize() == 0 )
	{
		// Add Default
		CDomain	domain;
		domain.m_strName	=	domain_self;
		g_groupcontainer.AddDomain( domain );
	}
	return g_groupcontainer;
}

static	CStDatabase * g_pdb = NULL;
void AfxSetDB(CStDatabase * pdb)
{
	g_pdb = pdb;
}

CStDatabase & AfxGetDB()
{
	if( g_pdb )	return *g_pdb;

	static	CStDatabase	g_stdatabase;
	return g_stdatabase;
}

CStock & AfxGetStockMain( DWORD dwMarket )
{
	static	CStock	g_stockMain;
	if( !g_stockMain.GetStockInfo().IsValidStock() )
		g_stockMain.SetStockCode( CStock::marketSHSE, STKLIB_CODE_MAIN );

	if( CStock::marketSHSE == dwMarket )
		return g_stockMain;
	else if( CStock::marketSZSE == dwMarket )
	{
		static	CStock	g_stockMain2;
		if( !g_stockMain2.GetStockInfo().IsValidStock() )
			g_stockMain2.SetStockCode( CStock::marketSZSE, STKLIB_CODE_MAINSZN );
		return g_stockMain2;
	}
	else if( CStock::marketCYSE == dwMarket )
		return g_stockMain;
	else if( CStock::marketCHNA == dwMarket )
		return g_stockMain;
	else if( CStock::marketHKEX == dwMarket )
		return g_stockMain;
	else if( CStock::marketTBSE == dwMarket )
		return g_stockMain;
	else if( CStock::marketTKSE == dwMarket )
		return g_stockMain;
	else if( CStock::marketLSE == dwMarket )
		return g_stockMain;
	else if( CStock::marketFLKFSE == dwMarket )
		return g_stockMain;
	else if( CStock::marketNYSE == dwMarket )
		return g_stockMain;
	else if( CStock::marketNASDAQ == dwMarket )
		return g_stockMain;
	else
		return g_stockMain;
}

//////////////////////////////////////////////////////////////////////////////////
// database functions
BOOL AfxInitializeDB( LPSTR lpErr, UINT nMaxSize )
{
	if( lpErr && nMaxSize > 0 )
		memset( lpErr, 0, nMaxSize );

	if( !CStDatabase::CreateSelfDB( AfxGetProfile().GetSelfDBPath() ) )
	{
		if( lpErr )	strncpy( lpErr, db_errcreateselfdb, min(nMaxSize-1,strlen(db_errcreateselfdb)) );
		return FALSE;
	}

	if( !AfxGetDB().SetRootPath( AfxGetProfile().GetSelfDBPath(), IStStore::dbtypeSelfDB ) )
	{
		if( lpErr )	strncpy( lpErr, db_errrootpath, min(nMaxSize-1,strlen(db_errrootpath)) );
		return FALSE;
	}

	CSPString	strQianlongPath	=	AfxGetProfile().GetQianlongPath();
	if( strQianlongPath.GetLength() > 0
		&& !AfxGetDB().AddAssistantRootPath( strQianlongPath, IStStore::dbtypeQianlong ) )
	{
		if( lpErr )	strncpy( lpErr, db_errqianlongpath, min(nMaxSize-1,strlen(db_errqianlongpath)) );
		return FALSE;
	}

	return TRUE;
}

BOOL AfxReleaseDB( )
{
	AfxGetDB().SetRootPath( NULL );
	AfxGetDB().RemoveAllAssistant( );
	return TRUE;
}

BOOL AfxReloadStockMain( )
{
	static CSPMutex	g_mutexStockMain;
	CSPMutex::Scoped	l(g_mutexStockMain);

	AfxReloadStock( AfxGetStockMain(CStock::marketSHSE) );
	AfxReloadStock( AfxGetStockMain(CStock::marketSZSE) );
	return TRUE;
}

BOOL AfxReloadStock( CStock & stock )
{
	CStockInfo info = stock.GetStockInfo();

	// Reload AfxGetStockMain()
	AfxGetStockContainer().GetStockInfo( info.GetStockCode(), &info );

	stock.Clear( );
	stock.SetStockInfo( &info );
	stock.SetDatabase( &AfxGetDB() );
	stock.PrepareData( CStock::dataK, CKData::ktypeDay );
	
	// Merge New
	stock.GetKDataDay().MergeKData( &(info.m_kdata) );
	
	stock.PrepareData( CStock::dataK, CKData::ktypeWeek );
	stock.PrepareData( CStock::dataK, CKData::ktypeMonth );
	stock.PrepareData( CStock::dataK, CKData::ktypeMin5 );
	stock.PrepareData( CStock::dataK, CKData::ktypeMin15 );
	stock.PrepareData( CStock::dataK, CKData::ktypeMin30 );
	stock.PrepareData( CStock::dataK, CKData::ktypeMin60 );
	return TRUE;
}

BOOL AfxPrepareStockData( CStDatabase * pDatabase, CStock &stock, int nKType, int nKFormat, int nMaindataType, BOOL bFullFill, BOOL bReload )
{
	CStockInfo	info;
	if( !AfxGetStockContainer().GetStockInfo( stock.GetStockCode(), &info ) )
		info	=	stock.GetStockInfo( );

	stock.SetDatabase( pDatabase );
	stock.PrepareData( CStock::dataK, CKData::ktypeDay, bReload );
	stock.PrepareData( CStock::dataK, CKData::ktypeMin5, bReload );
	stock.PrepareData( CStock::dataDR, CKData::ktypeDay, bReload );
	if( bReload || stock.GetKDataDay().GetCurFormat() == CKData::formatOriginal )
		stock.GetKDataDay().MergeKData( &(info.m_kdata) );
	
	CKData	& kday	=	stock.GetKData(CKData::ktypeDay);
	CKData	& kdata	=	stock.GetKData( nKType );
	kdata.SetDRData( stock.GetDRData() );
	kday.SetDRData( stock.GetDRData() );
	if( bFullFill )
		kday.FullFillKData( AfxGetStockMain().GetKDataDay(), FALSE );
	kday.ChangeCurFormat( nKFormat, AfxGetProfile().GetAutoResumeDRBegin(), AfxGetProfile().GetAutoResumeDRLimit() );
	if( CKData::ktypeWeek == nKType || CKData::ktypeMonth == nKType )
	{
		// 周线和月线不能ChangeCurFormat(...)
		if( CKData::formatOriginal == nKFormat )
			stock.PrepareData( CStock::dataK, nKType, bReload );
		else
			stock.ExtractKData( nKType, TRUE );
	}
	else if( CKData::ktypeDay != nKType )
	{
		stock.PrepareData( CStock::dataK, nKType, bReload );
		kdata.ChangeCurFormat( nKFormat, AfxGetProfile().GetAutoResumeDRBegin(), AfxGetProfile().GetAutoResumeDRLimit() );
	}
	kdata.SetMaindataType( nMaindataType );
	return TRUE;
}

int AfxRefreshStockContainerMinute( CStockContainer & container, LONG lStockType, int nSecondsToRead )
{
	container.Lock();

	time_t	tmTradeLatest	=	-1;

	int	nCount = 0;
	for( int i=0; i<container.GetSize(); i++ )
	{
		CStockInfo & info = container.ElementAt(i);
		if( info.GetType() != lStockType )
			continue;

		CStock	stock;
		stock.SetStockInfo( &info );
		stock.SetDatabase( &AfxGetDB() );
		AfxGetDB().LoadMinute( &stock );
		info.m_minute.RemoveAll();
		
		CMinute	& minstock = stock.GetMinute();
		if( -1 == tmTradeLatest && stock.GetMinute().GetSize() > 0 )
			tmTradeLatest	=	CSPTime::GetTimeTradeLatest( minstock[0].m_time );
		for( int i=0; i<minstock.GetSize(); i++ )
		{
			if( -1 == nSecondsToRead || tmTradeLatest-minstock[i].m_time<nSecondsToRead )
				info.m_minute.Add( minstock[i] );
		}
		
		nCount	++;
	}
	container.UnLock();
	return nCount;
}

//////////////////////////////////////////////////////////////////////////////////
// usful functions
CSPString AfxGetTimeString( time_t time, LPCTSTR lpszTimeFmt, BOOL bAddWeekDay )
{
	SP_ASSERT( lpszTimeFmt );
	if( !lpszTimeFmt || strlen(lpszTimeFmt) == 0 )
		return "";

	CSPTime	sptime(time);
	CSPString strTime = sptime.Format( lpszTimeFmt );

	if( bAddWeekDay )
	{
		int	nDayOfWeek	=	sptime.GetDayOfWeek();
		switch( nDayOfWeek )
		{
		case 1:	strTime	+=	sz_sunday;		break;
		case 2:	strTime	+=	sz_monday;		break;
		case 3:	strTime	+=	sz_tuesday;		break;
		case 4:	strTime	+=	sz_wednesday;	break;
		case 5:	strTime	+=	sz_thursday;	break;
		case 6:	strTime	+=	sz_friday;		break;
		case 7:	strTime	+=	sz_saturday;	break;
		default:;
		}
	}
	return strTime;
}

CSPString AfxGetReportTypeString( int reporttype )
{
	switch( reporttype )
	{
		case CStock::reportQuarter:
			return sz_reportQuarter;
		case CStock::reportMid:
			return sz_reportMid;
		case CStock::reportQuarter3:
			return sz_reportQuarter3;
		case CStock::reportAnnals:
			return sz_reportAnnals;
		default:
			return "";
	}
}

CSPString AfxGetFileDirectory( CSPString strPathName, CSPString strDefault )
{
	if( strPathName.IsEmpty() )
		return strDefault;

	CSPString	strResult	=	strPathName;
	if( strResult.GetLength() > 0 )
	{
		int	nIndex	=	strResult.ReverseFind( '\\' );
		if( -1 == nIndex )
			nIndex	=	strResult.ReverseFind( '/' );
		if( -1 == nIndex )
			strResult = strDefault;
		else
			strResult	=	strResult.Left( nIndex );
	}

	if( strResult.GetLength() <= 0 )
		return strDefault;

	int	nLen	=	strResult.GetLength();
	if( nLen > 0 && strResult[nLen-1] != '\\' && strResult[nLen-1] != '/' )
		strResult	+=	STRING_DIRSEP;

	return strResult;
}

CSPString	AfxGetFilePath( CSPString strDir, CSPString strFileTitle )
{
	if( strDir.GetLength() > 0 && ( strDir[strDir.GetLength()-1] == '\\' || strDir[strDir.GetLength()-1] == '/' ) )
		strDir	=	strDir.Left( strDir.GetLength()-1 );
	strDir	+=	STRING_DIRSEP;
	return ( strDir + strFileTitle );
}

