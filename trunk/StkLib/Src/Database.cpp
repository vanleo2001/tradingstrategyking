/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		DllMain( HANDLE, DWORD, LPVOID );
		class	CStDatabase;
*/

#include	"StdAfx.h"
#include	"../Include/Database.h"
#include	"../Include/SpString.h"
#include	<direct.h>
#include	"SelfDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////////////////
//	convertor

BOOL convert_TYDAY_RECORD_to_KDATA( TYDAY_RECORD * precord, KDATA * pkd )
{
	SP_ASSERT( precord && pkd );
	if( NULL == precord || NULL == pkd )
		return FALSE;
	memset( pkd, 0, sizeof(KDATA) );

	pkd->m_dwMarket	=	CStock::marketUnknown;
	strncpy( pkd->m_szCode, precord->code, min(sizeof(pkd->m_szCode)-1,sizeof(precord->code)) );

	pkd->m_date		=	precord->date;
	CSPTime	sptime;
	if( sptime.FromStockTimeDay(pkd->m_date) )
		pkd->m_time	=	sptime.GetTime();

	pkd->m_fOpen	=	(float)( precord->open * 0.001 );
	pkd->m_fHigh	=	(float)( precord->high * 0.001 );
	pkd->m_fLow		=	(float)( precord->low * 0.001 );
	pkd->m_fClose	=	(float)( precord->close * 0.001 );
	pkd->m_fAmount	=	(float)( precord->amount * 1000. );
	pkd->m_fVolume	=	(float)( precord->volume * 100. );
	return TRUE;
}

BOOL convert_TYDAY_RECORD_MIN_to_KDATA( TYDAY_RECORD * precord, KDATA * pkd )
{
	SP_ASSERT( precord && pkd );
	if( NULL == precord || NULL == pkd )
		return FALSE;
	memset( pkd, 0, sizeof(KDATA) );

	pkd->m_dwMarket	=	CStock::marketUnknown;
	strncpy( pkd->m_szCode, precord->code, min(sizeof(pkd->m_szCode)-1,sizeof(precord->code)) );

	pkd->m_date		=	precord->date;
	CSPTime	sptime;
	if( sptime.FromStockTimeDay(pkd->m_date) )
		pkd->m_time	=	sptime.GetTime();

	pkd->m_fOpen	=	(float)( precord->open * 0.001 );
	pkd->m_fHigh	=	(float)( precord->high * 0.001 );
	pkd->m_fLow		=	(float)( precord->low * 0.001 );
	pkd->m_fClose	=	(float)( precord->close * 0.001 );
	pkd->m_fAmount	=	(float)( precord->amount * 100. );
	pkd->m_fVolume	=	(float)( precord->volume * 100. );
	return TRUE;
}

BOOL convert_KDATA_to_TYDAY_RECORD( DWORD dwSerial, const char * name, KDATA * pkd, TYDAY_RECORD * precord )
{
	SP_ASSERT( precord && pkd );
	if( NULL == precord || NULL == pkd )
		return FALSE;
	memset( precord, 0, sizeof(TYDAY_RECORD) );

	precord->magic	=	0x06;
	precord->magic2	=	0x08;
	
	strncpy( precord->code, pkd->m_szCode, min(sizeof(precord->code),sizeof(pkd->m_szCode)) );
	if( name )
		strncpy( precord->name, name, min(sizeof(precord->name),strlen(name)) );

	precord->date		=	pkd->m_date;
	
	precord->open	=	(DWORD)( pkd->m_fOpen * 1000 );
	precord->high	=	(DWORD)( pkd->m_fHigh * 1000 );
	precord->low	=	(DWORD)( pkd->m_fLow * 1000 );
	precord->close	=	(DWORD)( pkd->m_fClose * 1000 );
	precord->amount	=	(DWORD)( pkd->m_fAmount * 0.001 );
	precord->volume	=	(DWORD)( pkd->m_fVolume * 0.01 );
	precord->serial	=	dwSerial;
	return TRUE;
}

BOOL convert_KDATA_to_TYDAY_RECORD_MIN( DWORD dwSerial, const char * name, KDATA * pkd, TYDAY_RECORD * precord )
{
	SP_ASSERT( precord && pkd );
	if( NULL == precord || NULL == pkd )
		return FALSE;
	memset( precord, 0, sizeof(TYDAY_RECORD) );

	precord->magic	=	0x06;
	precord->magic2	=	0x08;
	
	strncpy( precord->code, pkd->m_szCode, min(sizeof(precord->code),sizeof(pkd->m_szCode)) );
	if( name )
		strncpy( precord->name, name, min(sizeof(precord->name),strlen(name)) );

	precord->date		=	pkd->m_date;
	
	precord->open	=	(DWORD)( pkd->m_fOpen * 1000 );
	precord->high	=	(DWORD)( pkd->m_fHigh * 1000 );
	precord->low	=	(DWORD)( pkd->m_fLow * 1000 );
	precord->close	=	(DWORD)( pkd->m_fClose * 1000 );
	precord->amount	=	(DWORD)( pkd->m_fAmount * 0.01 );
	precord->volume	=	(DWORD)( pkd->m_fVolume * 0.01 );
	precord->serial	=	dwSerial;
	return TRUE;
}

BOOL convert_FXJDAY_RECORD_to_KDATA( DWORD dwMarket, LPCTSTR lpszCode, int nKType, FXJDAY_RECORD *precord, KDATA *pkd )
{
	SP_ASSERT( precord && pkd && lpszCode );
	if( NULL == precord || NULL == pkd )
		return FALSE;
	memset( pkd, 0, sizeof(KDATA) );

	pkd->m_dwMarket	=	dwMarket;
	strncpy( pkd->m_szCode, lpszCode, min(sizeof(pkd->m_szCode)-1,strlen(lpszCode)) );

	pkd->m_time		=	precord->m_time;
	CSPTime	sptime(pkd->m_time);
	pkd->m_date		=	sptime.ToStockTime( CKData::IsDayOrMin(nKType) );

	pkd->m_fOpen	=	precord->open;
	pkd->m_fHigh	=	precord->high;
	pkd->m_fLow		=	precord->low;
	pkd->m_fClose	=	precord->close;
	pkd->m_fAmount	=	precord->amount;
	pkd->m_fVolume	=	(float)( precord->volume * 100. );
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////
//	class	CStDatabase

CStDatabase::CStDatabase()
{
	m_pStore	=	NULL;
}

CStDatabase::~CStDatabase()
{
	if( m_pStore )
	{
		delete m_pStore;
		m_pStore	=	NULL;
	}

	for( int i=0; i<m_aptrAssistant.GetSize(); i++ )
	{
		IStStore	* pStore	=	(IStStore *)m_aptrAssistant.ElementAt(i);
		if( pStore )
			delete	pStore;
	}
	m_aptrAssistant.RemoveAll();
}

BOOL CStDatabase::CreateSelfDB( const char * rootpath )
{
	return CSelfDB::CreateSelfDB( rootpath );
}

int	CStDatabase::IsValidDataType ( int nType )
{
	return IStStore::IsValidDataType( nType );
}

int	CStDatabase::GetSupportedDataType ( CDBType * pdbtype, int maxsize )
{
	return IStStore::GetSupportedDataType( pdbtype, maxsize );
}

BOOL CStDatabase::SetRootPath( const char * rootpath, int nDBType )
{
	if( m_pStore )
		delete m_pStore;
	m_pStore	=	IStStore::CreateStore( rootpath, nDBType );
	return (NULL != m_pStore);
}

BOOL CStDatabase::IsOK( )
	{ return m_pStore && m_pStore->IsOK(); }
const char *CStDatabase::GetRootPath( )
	{ if(!IsOK()) return NULL; return m_pStore->GetRootPath(); }
int	CStDatabase::GetDBType( )
	{ if(!IsOK()) return IStStore::dbtypeUnknown; return m_pStore->GetDBType(); }
const char *CStDatabase::GetDBTypeName( )
	{ if(!IsOK()) return NULL;	return m_pStore->GetDBTypeName(); }

DWORD CStDatabase::GetSelfTempPath( char *szTempPath, int size )
{
	const char *szRoot = GetRootPath();
	if( 0 == strlen( szRoot ) )
	{
		if( NULL != szTempPath && size > 0 )
			szTempPath[0]	=	'\0';
		return 0;
	}

	char szReturn[MAX_PATH+1];
	memset( szReturn, 0, sizeof(szReturn) );
	strncpy( szReturn, szRoot, MAX_PATH );
	strncat( szReturn, "temp", MAX_PATH-strlen(szReturn) );
	strncat( szReturn, STRING_DIRSEP, MAX_PATH-strlen(szReturn) );
	

	BOOL	bCreated	=	TRUE;
	if( 0 != access(szReturn,0) )
		bCreated = (0==_mkdir(szReturn));
	if( !bCreated )
	{
		if( NULL != szTempPath && size > 0 )
			szTempPath[0]	=	'\0';
		return 0;
	}
	strncpy( szTempPath, szReturn, size );
	return strlen(szReturn);
}

BOOL CStDatabase::GetLastErrorMessage(LPTSTR lpszError, UINT nMaxError)
{
	if( nMaxError > 0 )
		lpszError[0]	=	'\0';
	return FALSE;
}

int	CStDatabase::GetMaxStockNumber( )
{
	if(!IsOK())	return 0;

	int nSize = m_pStore->GetMaxStockNumber();
	if( nSize > 0 )
		return nSize;

	for( int i=0; i<m_aptrAssistant.GetSize(); i++ )
	{
		IStStore	* pStore	=	(IStStore *)m_aptrAssistant.ElementAt(i);
		nSize	=	pStore->GetMaxStockNumber();
		if( nSize > 0 )
			return nSize;
	}
	return nSize;
}

int CStDatabase::LoadCodetable( CStockContainer & container )
{
	if(!IsOK())	return 0;

	int nSize = m_pStore->LoadCodetable( container );
	if( nSize > 0 )
		return nSize;

	for( int i=0; i<m_aptrAssistant.GetSize(); i++ )
	{
		IStStore	* pStore	=	(IStStore *)m_aptrAssistant.ElementAt(i);
		nSize	=	pStore->LoadCodetable( container );
		if( nSize > 0 )
			return nSize;
	}
	return nSize;
}

int CStDatabase::StoreCodetable( CStockContainer & container )
	{ if(!IsOK()) return 0;	return m_pStore->StoreCodetable( container ); }

int CStDatabase::LoadKDataCache( CStockContainer & container, PROGRESS_CALLBACK fnCallback, void *cookie, int nProgStart, int nProgEnd )
{
	if(!IsOK())	return 0;

	int nSize = m_pStore->LoadKDataCache( container, fnCallback, cookie, nProgStart, nProgEnd );
	if( nSize > 0 )
		return nSize;

	for( int i=0; i<m_aptrAssistant.GetSize(); i++ )
	{
		IStStore	* pStore	=	(IStStore *)m_aptrAssistant.ElementAt(i);
		nSize	=	pStore->LoadKDataCache( container, fnCallback, cookie, nProgStart, nProgEnd );
		if( nSize > 0 )
			return nSize;
	}
	return nSize;
}

int CStDatabase::LoadBasetable( CStockContainer & container )
	{ if(!IsOK()) return 0;	return m_pStore->LoadBasetable( container ); }
int CStDatabase::StoreBasetable( CStockContainer & container )
	{ if(!IsOK()) return 0;	return m_pStore->StoreBasetable( container ); }

int CStDatabase::LoadBaseText( CStock *pstock )
{
	if( !pstock || !IsOK() )	return 0;

	int nSize = m_pStore->LoadBaseText( pstock );
	if( nSize > 0 )
		return nSize;

	for( int i=0; i<m_aptrAssistant.GetSize(); i++ )
	{
		IStStore	* pStore	=	(IStStore *)m_aptrAssistant.ElementAt(i);
		CStock	stock;
		stock.SetStockInfo( &(pstock->GetStockInfo()) );
		if( pStore && pStore->LoadBaseText( &stock ) > 0 )
		{
			nSize	=	pstock->MergeBaseText( stock );
		}
	}
	return nSize;
}

int CStDatabase::LoadKData( CStock *pstock, int period )
{
	if( !pstock || !IsOK())	return 0;

	int nSize = m_pStore->LoadKData( pstock, period );

	for( int i=0; i<m_aptrAssistant.GetSize(); i++ )
	{
		IStStore	* pStore	=	(IStStore *)m_aptrAssistant.ElementAt(i);
		CStock	stock;
		stock.SetStockInfo( &(pstock->GetStockInfo()) );
		if( pStore->LoadKData( &stock, period ) > 0 )
		{
			nSize	=	pstock->MergeKData( stock, period );
		}
	}
	return nSize;
}

int CStDatabase::LoadDRData( CStock *pstock )
{
	if( !pstock || !IsOK() )
		return 0;

	int nSize = m_pStore->LoadDRData( pstock );

	for( int i=0; i<m_aptrAssistant.GetSize(); i++ )
	{
		IStStore	* pStore	=	(IStStore *)m_aptrAssistant.ElementAt(i);
		CStock	stock;
		stock.SetStockInfo( &(pstock->GetStockInfo()) );
		if( pStore && pStore->LoadDRData( &stock ) > 0 )
		{
			nSize	=	pstock->MergeDRData( stock );
		}
	}
	return nSize;
}

int CStDatabase::StoreDRData( CStock *pstock )
	{	if(!IsOK()) return 0;
		return m_pStore->StoreDRData( pstock );	}

int CStDatabase::LoadReport( CStock *pstock )
{
	if( !pstock || !IsOK() )	return 0;

	int nSize = m_pStore->LoadReport( pstock );
	if( nSize > 0 )
		return nSize;

	for( int i=0; i<m_aptrAssistant.GetSize(); i++ )
	{
		IStStore	* pStore	=	(IStStore *)m_aptrAssistant.ElementAt(i);
		if( pStore )
			nSize = pStore->LoadReport( pstock );
		if( nSize > 0 )
			return nSize;
	}
	return nSize;
}

int CStDatabase::LoadMinute( CStock *pstock )
{
	if( !pstock || !IsOK() )	return 0;

	int nSize = m_pStore->LoadMinute( pstock );
	if( nSize > 0 )
		return nSize;

	for( int i=0; i<m_aptrAssistant.GetSize(); i++ )
	{
		IStStore	* pStore	=	(IStStore *)m_aptrAssistant.ElementAt(i);
		if( pStore )
			nSize = pStore->LoadMinute( pstock );
		if( nSize > 0 )
			return nSize;
	}
	return nSize;
}

int CStDatabase::LoadOutline( CStock *pstock )
{
	if( !pstock || !IsOK() )	return 0;

	int nSize = m_pStore->LoadOutline( pstock );
	if( nSize > 0 )
		return nSize;

	for( int i=0; i<m_aptrAssistant.GetSize(); i++ )
	{
		IStStore	* pStore	=	(IStStore *)m_aptrAssistant.ElementAt(i);
		if( pStore )
			nSize = pStore->LoadOutline( pstock );
		if( nSize > 0 )
			return nSize;
	}
	return nSize;
}

int CStDatabase::StoreReport( REPORT * pReport, int nCount, BOOL bBigTrade )
	{	if(!IsOK()) return 0;
		return m_pStore->StoreReport( pReport, nCount, bBigTrade );	}
int CStDatabase::StoreMinute( MINUTE * pMinute, int nCount )
	{	if(!IsOK()) return 0;
		return m_pStore->StoreMinute( pMinute, nCount );	}
int CStDatabase::StoreOutline( OUTLINE * pOutline, int nCount )
	{	if(!IsOK()) return 0;
		return m_pStore->StoreOutline( pOutline, nCount );	}

int CStDatabase::InstallCodetbl( const char * filename, const char *orgname )
	{	if(!IsOK()) return 0;
		return m_pStore->InstallCodetbl( filename, orgname );	}
int CStDatabase::InstallCodetblBlock( const char * filename, const char *orgname )
	{	if(!IsOK()) return 0;
		return m_pStore->InstallCodetblBlock( filename, orgname );	}
int CStDatabase::InstallCodetblFxjBlock( const char * filename, const char *orgname )
	{	if(!IsOK()) return 0;
		return m_pStore->InstallCodetblFxjBlock( filename, orgname );	}
int CStDatabase::InstallKData( CKData &kdata, BOOL bOverwrite )
	{	if(!IsOK()) return 0;
		return m_pStore->InstallKData( kdata, bOverwrite );	}
int CStDatabase::InstallKDataTy( const char * stkfile, int ktype, PROGRESS_CALLBACK fnCallback, void *cookie )
	{	if(!IsOK()) return 0;
		return m_pStore->InstallKDataTy( stkfile, ktype, fnCallback, cookie );	}
int CStDatabase::InstallKDataFxj( const char * dadfile, int ktype, PROGRESS_CALLBACK fnCallback, void *cookie )
	{	if(!IsOK()) return 0;
		return m_pStore->InstallKDataFxj( dadfile, ktype, fnCallback, cookie );	}
int CStDatabase::InstallDRData( CDRData &drdata )
	{	if(!IsOK()) return 0;
		return m_pStore->InstallDRData( drdata );	}
int CStDatabase::InstallDRDataClk( const char * filename, const char *orgname )
	{	if(!IsOK()) return 0;
		return m_pStore->InstallDRDataClk( filename, orgname );	}
int CStDatabase::InstallDRDataFxj( const char * fxjfilename )
	{	if(!IsOK()) return 0;
		return m_pStore->InstallDRDataFxj( fxjfilename );	}
int CStDatabase::InstallBasetable( const char * filename, const char *orgname )
	{	if(!IsOK()) return 0;
		return m_pStore->InstallBasetable( filename, orgname );	}
int CStDatabase::InstallBasetableTdx( const char * filename )
	{	if(!IsOK()) return 0;
		return m_pStore->InstallBasetableTdx( filename );	}
int CStDatabase::InstallBasetableFxj( const char * filename )
	{	if(!IsOK()) return 0;
		return m_pStore->InstallBasetableFxj( filename );	}
int CStDatabase::InstallBaseText( const char * filename, const char *orgname )
	{	if(!IsOK()) return 0;
		return m_pStore->InstallBaseText( filename, orgname );	}
int CStDatabase::InstallBaseText( const char * buffer, int nLen, const char *orgname )
	{	if(!IsOK()) return 0;
		return m_pStore->InstallBaseText( buffer, nLen, orgname );	}
int CStDatabase::InstallNewsText( const char * filename, const char *orgname )
	{	if(!IsOK()) return 0;
		return m_pStore->InstallNewsText( filename, orgname );	}
int CStDatabase::InstallNewsText( const char * buffer, int nLen, const char *orgname )
	{	if(!IsOK()) return 0;
		return m_pStore->InstallNewsText( buffer, nLen, orgname );	}


BOOL CStDatabase::AddAssistantRootPath( const char * rootpath, int nDBType )
{
	IStStore	* pStore	=	IStStore::CreateStore( rootpath, nDBType );
	if( NULL == pStore )
		return FALSE;
	m_aptrAssistant.Add( pStore );
	return TRUE;
}

void CStDatabase::RemoveAssistant( const char * rootpath )
{
	if( NULL == rootpath || strlen(rootpath) == 0 )
		return;

	for( int i=m_aptrAssistant.GetSize()-1; i>=0; i-- )
	{
		IStStore	* pStore	=	(IStStore *)m_aptrAssistant.ElementAt(i);
		if( pStore && 0 == strcmp( pStore->GetRootPath(), rootpath ) )
		{
			m_aptrAssistant.RemoveAt(i);
			delete	pStore;
		}
	}
}

void CStDatabase::RemoveAllAssistant( )
{
	for( int i=0; i<m_aptrAssistant.GetSize(); i++ )
	{
		IStStore	* pStore	=	(IStStore *)m_aptrAssistant.ElementAt(i);
		if( pStore )
		{
			delete	pStore;
		}
	}
	m_aptrAssistant.RemoveAll( );
}

CSPTime CStDatabase::GetTimeInitial( )
{
	return CSPTime( 1990, 12, 19, 0, 0, 0 );
}

BOOL CStDatabase::GetTimeLocalRange( CSPTime *ptmLatest, CSPTime * ptmPioneer, CSPTime * ptmInitial )
{
	CStock	stock;
	stock.SetStockCode( CStock::marketSHSE, STKLIB_CODE_MAIN );
	stock.SetDatabase( this );
	if( !stock.PrepareData( CStock::dataK, CKData::ktypeDay ) )
		return FALSE;
	CKData	& kday	=	stock.GetKDataDay();

	int	nYear, nMonth, nDay, nHour, nMinute;
	if( ptmLatest )
	{
		if( !kday.LatestDate( nYear, nMonth, nDay, nHour, nMinute ) )
			return FALSE;
		*ptmLatest	=	CSPTime( nYear, nMonth, nDay, 23, 59, 59 );
	}

	if( ptmPioneer )
	{
		if( kday.GetSize() == 0 || !kday.DateAt(0, nYear, nMonth, nDay, nHour, nMinute ) )
			return FALSE;
		*ptmPioneer	=	CSPTime( nYear, nMonth, nDay, 0, 0, 0 );
	}

	if( ptmInitial )
	{
		*ptmInitial	=	GetTimeInitial( );
	}

	return TRUE;
}

BOOL CStDatabase::GetNeedDownloadRange( CStockInfo &info, CSPTime tmBegin, CSPTime tmEnd, CSPTime &tmDLBegin, CSPTime &tmDLEnd )
{
	tmDLBegin	=	tmBegin;
	tmDLEnd		=	tmEnd;

	// Prepare Data
	CStock	stock;
	stock.SetStockInfo( &info );
	stock.SetDatabase( this );
	if( !stock.PrepareData( CStock::dataK, CKData::ktypeDay ) )
		return TRUE;
	CKData	& kday	=	stock.GetKDataDay();

	// Prepare Time
	CSPTime	tmInitial, tmLatest, tmPioneer;
	CSPTime	sptimeInitial;
	if( 0 != info.m_datebegin && sptimeInitial.FromStockTimeDay( info.m_datebegin ) )
		tmInitial	=	sptimeInitial.GetTime();
	else
		tmInitial	=	GetTimeInitial( );

	int	nYear, nMonth, nDay, nHour, nMinute;
	if( !kday.LatestDate( nYear, nMonth, nDay, nHour, nMinute ) )
		return TRUE;
	tmLatest	=	CSPTime( nYear, nMonth, nDay, 23, 59, 59 );

	if( kday.GetSize() == 0 || !kday.DateAt(0, nYear, nMonth, nDay, nHour, nMinute ) )
		return TRUE;
	tmPioneer	=	CSPTime( nYear, nMonth, nDay, 0, 0, 0 );

	// Deal With
	if( tmBegin < tmInitial )
		tmBegin	=	tmInitial;
	
	if( tmBegin >= tmPioneer && tmEnd <= tmLatest )
	{
		int	nCount	=	0;
		for( int k=0; k<kday.GetSize(); k++ )
		{
			DWORD	date	=	kday.ElementAt(k).m_date;
			CSPTime	sptime;
			if( sptime.FromStockTimeDay(date) )
			{
				if( sptime.GetTime() >= tmBegin.GetTime() && sptime.GetTime() <= tmEnd.GetTime() )
					nCount	++;
			}
		}

		CSPTimeSpan	span	=	tmEnd - tmBegin;
		if( nCount > span.GetDays()*4/7 )
			return FALSE;
	}
	
	if( tmEnd <= tmPioneer )
	{
		tmDLBegin	=	tmBegin;
		tmDLEnd	=	tmPioneer-CSPTimeSpan(1,0,0,0);
	}
	else if( tmBegin >= tmLatest )
	{
		tmDLBegin	=	tmLatest+CSPTimeSpan(1,0,0,0);
		tmDLEnd	=	tmEnd;
	}
	else if( tmBegin < tmPioneer && tmEnd > tmPioneer && tmEnd <= tmLatest )
	{
		tmDLBegin	=	tmBegin;
		tmDLEnd	=	tmPioneer-CSPTimeSpan(1,0,0,0);
	}
	else if( tmBegin >= tmPioneer && tmBegin < tmLatest && tmEnd > tmLatest )
	{
		tmDLBegin	=	tmLatest+CSPTimeSpan(1,0,0,0);
		tmDLEnd	=	tmEnd;
	}
	else
	{
		tmDLBegin	=	tmBegin;
		tmDLEnd		=	tmEnd;
	}
	return TRUE;
}

// 将钱龙格式K线数据 加入 通用数据包
int CStDatabase::AppendToTyData( const char *code, const char *name, int nKType, LPCTSTR lpszKFile, LPCTSTR lpszTyDataFile, time_t tmBegin, time_t tmEnd )
{
	int	nCount	=	0;
	CSPFile	fileK, fileTy;
	if( fileK.Open( lpszKFile, CSPFile::modeRead )
		&& fileTy.Open( lpszTyDataFile, CSPFile::modeCreate|CSPFile::modeNoTruncate|CSPFile::modeReadWrite ) )
	{
		// Init Header
		TYDAY_FHEADER	header;
		memset( &header, 0, sizeof(header) );
		header.magic	=	0x06;
		for( int i=0; i<sizeof(header.code); i++ )
			header.code[i]	=	' ';
		header.magic2	=	0x08;
		for( int i=0; i<sizeof(header.name); i++ )
			header.name[i]	=	' ';

		CSPTime	sptime	=	CSPTime::GetCurrentTime();
		header.date		=	sptime.ToStockTimeDay();
		header.datebegin=	header.date;
		header.dateend	=	header.date;
		header.gendate	=	header.date;
		header.gentime	=	sptime.GetTime();

		if( sizeof(header) != fileTy.Read( &header, sizeof(header) ) )
		{
			fileTy.SeekToBegin();
			fileTy.Write( &header, sizeof(header) );
		}

		header.from	=	'CL';

		fileTy.SeekToEnd();

		// generate
		if( CKData::IsDayOrMin(nKType) )
		{
			DWORD	dateBegin	=	CSPTime(tmBegin).ToStockTimeDay();
			DWORD	dateEnd		=	CSPTime(tmEnd).ToStockTimeDay();

			struct	QL_Data_day		qlkd;
			while( sizeof(qlkd) == fileK.Read( &qlkd, sizeof(qlkd) ) )
			{
				if( qlkd.day_date < dateBegin || qlkd.day_date > dateEnd )
					continue;

				KDATA	kd;
				TYDAY_RECORD	tyrecord;
				convert_QL_Data_day_to_KDATA( CStock::marketUnknown, code, &qlkd, &kd );
				convert_KDATA_to_TYDAY_RECORD( header.recordcount+1, name, &kd, &tyrecord );
				fileTy.Write( &tyrecord, sizeof(tyrecord) );
				
				// Modified header
				if( tyrecord.date < header.datebegin )
					header.datebegin	=	tyrecord.date;
				if( tyrecord.date > header.dateend )
					header.dateend		=	tyrecord.date;
				header.recordcount	+=	1;
				nCount	++;
			}
		}
		else
		{
			DWORD	dateBegin	=	CSPTime(tmBegin).ToStockTimeMin();
			DWORD	dateEnd		=	CSPTime(tmEnd).ToStockTimeMin();

			struct	QL_Data_5min		qlkd;
			while( sizeof(qlkd) == fileK.Read( &qlkd, sizeof(qlkd) ) )
			{
				if( qlkd.min_off < dateBegin || qlkd.min_off > dateEnd )
					continue;

				KDATA	kd;
				TYDAY_RECORD	tyrecord;
				convert_QL_Data_5min_to_KDATA( CStock::marketUnknown, code, &qlkd, &kd );
				convert_KDATA_to_TYDAY_RECORD_MIN( header.recordcount+1, name, &kd, &tyrecord );
				fileTy.Write( &tyrecord, sizeof(tyrecord) );
				
				// Modified header
				if( tyrecord.date < header.datebegin )
					header.datebegin	=	tyrecord.date;
				if( tyrecord.date > header.dateend )
					header.dateend		=	tyrecord.date;
				header.recordcount	+=	1;
				nCount	++;
			}
		}

		if( nCount > 0 )
			header.sharecount	+=	1;
		fileTy.SeekToBegin();
		fileTy.Write( &header, sizeof(header) );

		fileK.Close();
		fileTy.Close();
	}
	return nCount;
}
