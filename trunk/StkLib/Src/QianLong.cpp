
#include "stdafx.h"
#include "../Include/Database.h"
#include "../Include/SpString.h"
#include "QianLong.h"

//#include	<math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL convert_QL_Data_5min_to_KDATA( DWORD dwMarket, const char *szCode, struct QL_Data_5min * pqlkd, KDATA *pkd )
{
	SP_ASSERT( pqlkd && pkd );
	if( NULL == pqlkd || NULL == pkd )
		return FALSE;
	memset( pkd, 0, sizeof(KDATA) );

	pkd->m_dwMarket	= dwMarket;
	if( szCode )
		strncpy( pkd->m_szCode, szCode, min(sizeof(pkd->m_szCode)-1,strlen(szCode)) );

	pkd->m_date		= pqlkd->min_off;
	CSPTime	sptime;
	if( sptime.FromStockTimeMin( pkd->m_date) )
		pkd->m_time	= sptime.GetTime();

	pkd->m_fOpen	= (float)fabs( pqlkd->open_price * 0.001 );
	pkd->m_fHigh	= (float)fabs( pqlkd->high_price * 0.001 );
	pkd->m_fLow		= (float)fabs( pqlkd->low_price * 0.001 );
	pkd->m_fClose	= (float)fabs( pqlkd->close_price * 0.001 );
	pkd->m_fVolume	= (float)fabs( pqlkd->min_volume * 100. );
	pkd->m_fAmount	= (float)fabs( pqlkd->min_amount * 100. );
	return TRUE;
}

BOOL convert_QL_Data_day_to_KDATA( DWORD dwMarket, const char *szCode, struct QL_Data_day * pqlkd, KDATA *pkd )
{
	SP_ASSERT( pqlkd && pkd );
	if( NULL == pqlkd || NULL == pkd )
		return FALSE;
	memset( pkd, 0, sizeof(KDATA) );

	pkd->m_dwMarket	= dwMarket;
	if( szCode )
		strncpy( pkd->m_szCode, szCode, min(sizeof(pkd->m_szCode)-1,strlen(szCode)) );

	pkd->m_date		= pqlkd->day_date;
	CSPTime	sptime;
	if( sptime.FromStockTimeDay(pkd->m_date) )
		pkd->m_time	= sptime.GetTime();

	pkd->m_fOpen	= (float)fabs( pqlkd->open_price * 0.001 );
	pkd->m_fHigh	= (float)fabs( pqlkd->high_price * 0.001 );
	pkd->m_fLow		= (float)fabs( pqlkd->low_price * 0.001 );
	pkd->m_fClose	= (float)fabs( pqlkd->close_price * 0.001 );
	pkd->m_fVolume	= (float)fabs( pqlkd->day_volume * 100. );
	pkd->m_fAmount	= (float)fabs( pqlkd->day_amount * 1000. );
	return TRUE;
}

BOOL convert_KDATA_to_QL_Data_day( KDATA * pkd, struct QL_Data_day * pqlkd )
{
	SP_ASSERT( pqlkd && pkd );
	if( NULL == pqlkd || NULL == pkd )
		return FALSE;
	memset( pqlkd, 0, sizeof(struct QL_Data_day) );

	pqlkd->day_date		= (DWORD)( pkd->m_date );
	pqlkd->open_price	= (DWORD)( pkd->m_fOpen * 1000 );
	pqlkd->high_price	= (DWORD)( pkd->m_fHigh * 1000 );
	pqlkd->low_price	= (DWORD)( pkd->m_fLow * 1000 );
	pqlkd->close_price	= (DWORD)( pkd->m_fClose * 1000 );
	pqlkd->day_volume	= (DWORD)( pkd->m_fVolume * 0.01 );
	pqlkd->day_amount	= (DWORD)( pkd->m_fAmount * 0.001 );
	return TRUE;
}

BOOL convert_KDATA_to_QL_Data_5min( KDATA * pkd, struct QL_Data_5min * pqlkd )
{
	SP_ASSERT( pqlkd && pkd );
	if( NULL == pqlkd || NULL == pkd )
		return FALSE;
	memset( pqlkd, 0, sizeof(struct QL_Data_day) );

	pqlkd->min_off		= (DWORD)( pkd->m_date );
	pqlkd->open_price	= (DWORD)( pkd->m_fOpen * 1000 );
	pqlkd->high_price	= (DWORD)( pkd->m_fHigh * 1000 );
	pqlkd->low_price	= (DWORD)( pkd->m_fLow * 1000 );
	pqlkd->close_price	= (DWORD)( pkd->m_fClose * 1000 );
	pqlkd->min_volume	= (DWORD)( pkd->m_fVolume * 0.01 );
	pqlkd->min_amount	= (DWORD)( pkd->m_fAmount * 0.01 );
	return TRUE;
}

void ConvertQLStockInfo( DWORD dwMarket, QL_Stock_info_V302 & block, CStockInfo *pInfo )
{
	pInfo->Clear();

	char	code[sizeof(block.stock_code)+2];
	memset(code,0,sizeof(code));
	strncpy(code,(const char *)block.stock_code,sizeof(block.stock_code));

	char	name[sizeof(block.stock_name)+2];
	memset(name,0,sizeof(name));
	strncpy(name,(const char *)block.stock_name,sizeof(block.stock_name));

	pInfo->SetStockCode( dwMarket, code );
	pInfo->SetStockName( name );
	pInfo->SetType( block.stock_type );

/*
	pInfo->m_fLast		= (float)fabs( block.last_close_price * 0.001 );
	pInfo->m_fOpen		= (float)fabs( block.open_price * 0.001 );
	pInfo->m_fHigh		= (float)fabs( block.high_price * 0.001 );
	pInfo->m_fLow		= (float)fabs( block.low_price * 0.001 );
	pInfo->m_fClose		= (float)fabs( block.close_price * 0.001 );
	pInfo->m_fVolume	= (float)fabs( block.total_volume * 100. );
	pInfo->m_fAmount	= (float)fabs( block.total_value * 1000. );
	pInfo->m_fBuyPrice[0]	= (float)fabs( block.buy_1_price * 0.001 );
	pInfo->m_fBuyVolume[0]	= (float)fabs( block.buy_1_volume * 100. );
	pInfo->m_fBuyPrice[1]	= (float)fabs( block.buy_2_price * 0.001 );
	pInfo->m_fBuyVolume[1]	= (float)fabs( block.buy_2_volume * 100. );
	pInfo->m_fBuyPrice[2]	= (float)fabs( block.buy_3_price * 0.001 );
	pInfo->m_fBuyVolume[2]	= (float)fabs( block.buy_3_volume * 100. );
	pInfo->m_fSellPrice[0]	= (float)fabs( block.sell_1_price * 0.001 );
	pInfo->m_fSellVolume[0]	= (float)fabs( block.sell_1_volume * 100. );
	pInfo->m_fSellPrice[1]	= (float)fabs( block.sell_2_price * 0.001 );
	pInfo->m_fSellVolume[1]	= (float)fabs( block.sell_2_volume * 100. );
	pInfo->m_fSellPrice[2]	= (float)fabs( block.sell_3_price * 0.001 );
	pInfo->m_fSellVolume[2]	= (float)fabs( block.sell_3_volume * 100. );
*/
}

void ConvertQLStockInfo( DWORD dwMarket, QL_Stock_info2_V304 & block, CStockInfo *pInfo )
{
	pInfo->Clear();

	char	code[sizeof(block.stock_code)+2];
	memset(code,0,sizeof(code));
	strncpy(code,(const char *)block.stock_code,sizeof(block.stock_code));

	char	name[sizeof(block.stock_name)+2];
	memset(name,0,sizeof(name));
	strncpy(name,(const char *)block.stock_name,sizeof(block.stock_name));

	pInfo->SetStockCode( dwMarket, code );
	pInfo->SetStockName( name );
	pInfo->SetType( block.stock_type );

/*
	pInfo->m_fLast		= (float)fabs( block.last_close_price * 0.001 );
	pInfo->m_fOpen		= (float)fabs( block.open_price * 0.001 );
	pInfo->m_fHigh		= (float)fabs( block.high_price * 0.001 );
	pInfo->m_fLow		= (float)fabs( block.low_price * 0.001 );
	pInfo->m_fClose		= (float)fabs( block.close_price * 0.001 );
	pInfo->m_fVolume	= (float)fabs( block.total_volume * 100. );
	pInfo->m_fAmount	= (float)fabs( block.total_value * 1000. );
	pInfo->m_fBuyPrice[0]	= (float)fabs( block.buy_1_price * 0.001 );
	pInfo->m_fBuyVolume[0]	= (float)fabs( block.buy_1_volume * 100. );
	pInfo->m_fBuyPrice[1]	= (float)fabs( block.buy_2_price * 0.001 );
	pInfo->m_fBuyVolume[1]	= (float)fabs( block.buy_2_volume * 100. );
	pInfo->m_fBuyPrice[2]	= (float)fabs( block.buy_3_price * 0.001 );
	pInfo->m_fBuyVolume[2]	= (float)fabs( block.buy_3_volume * 100. );
	pInfo->m_fSellPrice[0]	= (float)fabs( block.sell_1_price * 0.001 );
	pInfo->m_fSellVolume[0]	= (float)fabs( block.sell_1_volume * 100. );
	pInfo->m_fSellPrice[1]	= (float)fabs( block.sell_2_price * 0.001 );
	pInfo->m_fSellVolume[1]	= (float)fabs( block.sell_2_volume * 100. );
	pInfo->m_fSellPrice[2]	= (float)fabs( block.sell_3_price * 0.001 );
	pInfo->m_fSellVolume[2]	= (float)fabs( block.sell_3_volume * 100. );
*/
}

/////////////////////////////////////////////////////////////////////////////////////
// class	CQianlong
/*

struct QL_Info_data datainfo_sha[TYPE_NUM];
struct QL_Info_data datainfo_szn[TYPE_NUM];

unsigned char exepath[80];
unsigned char appdexe[88];

unsigned char ml_sys[80];
unsigned char ml_dat[80];
unsigned char ml_sh_day[80];
unsigned char ml_sz_day[80];
unsigned char ml_sh_min[80];
unsigned char ml_sz_min[80];
unsigned char ml_sh_wek[80];
unsigned char ml_sz_wek[80];
unsigned char ml_sh_mnt[80];
unsigned char ml_sz_mnt[80];

unsigned char sl_sys[80];
unsigned char sl_dat[80];
unsigned char sl_sh_day[80];
unsigned char sl_sz_day[80];
unsigned char sl_sh_min[80];
unsigned char sl_sz_min[80];
unsigned char sl_sh_wek[80];
unsigned char sl_sz_wek[80];
unsigned char sl_sh_mnt[80];
unsigned char sl_sz_mnt[80];

unsigned char hx_sys[80];
unsigned char hx_dat[80];
unsigned char hx_sh_day[80];
unsigned char hx_sz_day[80];
unsigned char hx_sh_min[80];
unsigned char hx_sz_min[80];
unsigned char hx_sh_wek[80];
unsigned char hx_sz_wek[80];
unsigned char hx_sh_mnt[80];
unsigned char hx_sz_mnt[80];

unsigned char dealpath[80];
unsigned char dealdat[80];
unsigned char tmpdir[80];
unsigned char sysdir[80];
unsigned char infdir[80];
unsigned char datdir[80];
unsigned char basdir[80];
unsigned char flag_mlv304;

unsigned long nowdate;
unsigned char datestr[10];
unsigned char flag_dynamic;
unsigned char ml30_flag;
unsigned char slon_flag;
unsigned char hxtw_flag;
*/

char ml_dat[]			= "data\\";
char ml_sh_info[]		= "data\\shinfo.dat";
char ml_sz_info[]		= "data\\szinfo.dat";
char ml_sh_now[]		= "data\\shnow.dat";
char ml_sz_now[]		= "data\\sznow.dat";
char ml_sh_pyjc[]		= "data\\shpyjc.dat";
char ml_sz_pyjc[]		= "data\\szpyjc.dat";
char ml_sh_trace[]		= "data\\shtrace.dat";
char ml_sz_trace[]		= "data\\sztrace.dat";
char ml_sh_minute[]		= "data\\shminute.dat";
char ml_sz_minute[]		= "data\\szminute.dat";

char ml_data[]			= "data\\";
char ml_sh[]			= "data\\sh\\";
char ml_sz[]			= "data\\sz\\";
char ml_base[]			= "bas\\";
char ml_month[]			= "mon\\";
char ml_week[]			= "wek\\";
char ml_day[]			= "day\\";
char ml_min5[]			= "nmn\\";

char ml_sh_base[]		= "data\\sh\\bas\\";
char ml_sz_base[]		= "data\\sz\\bas\\";
char ml_sh_month[]		= "data\\sh\\mon\\";
char ml_sz_month[]		= "data\\sz\\mon\\";
char ml_sh_week[]		= "data\\sh\\wek\\";
char ml_sz_week[]		= "data\\sz\\wek\\";
char ml_sh_day[]		= "data\\sh\\day\\";
char ml_sz_day[]		= "data\\sz\\day\\";
char ml_sh_min[]		= "data\\sh\\nmn\\";
char ml_sz_min[]		= "data\\sz\\nmn\\";

char ml_ext_base[]		= ".txt";
char ml_ext_month[]		= ".mnt";
char ml_ext_week[]		= ".wek";
char ml_ext_day[]		= ".day";
char ml_ext_min5[]		= ".nmn";

CQianlong::CQianlong( const char * rootpath, BOOL bOK )
{
	m_bIsOK	= FALSE;
	memset( m_szRootPath, 0, sizeof(m_szRootPath) );
	if( !bOK )
	{
		if( GetAccurateRoot( rootpath, m_szRootPath, sizeof(m_szRootPath)-1 ) )
			m_bIsOK	= TRUE;
		else
			m_bIsOK	= FALSE;
	}
	else
	{
		strncpy( m_szRootPath, rootpath, sizeof(m_szRootPath)-1 );
		m_bIsOK	= TRUE;
	}

	m_nVersion	= versionUnknown;
	if( bOK )
		m_nVersion	= DetectVersion( GetRootPath() );
}

CQianlong::~CQianlong( )
{
}

int	CQianlong::GetMaxStockNumber( )
{
	SP_ASSERT( m_bIsOK );
	if( ! m_bIsOK )	return 0;

	// load shinfo.dat szinfo.dat

	int	blocksize	= sizeof(struct QL_Stock_info_V302);
	if( version304 == m_nVersion )
		blocksize	= sizeof(struct QL_Stock_info2_V304);

	DWORD	dwCount	= 0;
	CSPString	sFileName	= GetRootPath();
	sFileName	+= ml_sh_now;
	CSPFile	file;
	if( file.Open( sFileName, CSPFile::modeRead | CSPFile::shareDenyNone ) )
	{
		DWORD dwFileLen = file.GetLength();
		dwCount	+= dwFileLen / blocksize;
		file.Close();
	}
	sFileName	= GetRootPath();
	sFileName	+= ml_sz_now;
	if( file.Open( sFileName, CSPFile::modeRead | CSPFile::shareDenyNone ) )
	{
		DWORD dwFileLen = file.GetLength();
		dwCount	+= dwFileLen / blocksize;
		file.Close();
	}
	return	dwCount;
}

int	CQianlong::LoadCodetable( CStockContainer & container )
{
	SP_ASSERT( m_bIsOK );
	if( !m_bIsOK )	return 0;

	int maxsize = GetMaxStockNumber();
	container.SetSize( maxsize );
	CStockInfo * pdata = container.GetData();

	char	szShortName[QL_SHORTNAME_LEN+1];
	memset( szShortName, 0, sizeof(szShortName) );
	int	 nCount	= 0;
	CSPString	sFileName	= GetRootPath();
	sFileName	+= ml_sh_now;
	CSPFile	file;
	if( file.Open( sFileName, CSPFile::modeRead | CSPFile::shareDenyNone ) )
	{
		// pin yin file
		CSPString	sFileNamePyjc	= GetRootPath();
		sFileNamePyjc	+= ml_sh_pyjc;
		CSPFile	filePyjc;
		filePyjc.Open( sFileNamePyjc, CSPFile::modeRead | CSPFile::shareDenyNone );
		
		if( version302 == m_nVersion )
		{
			struct QL_Stock_info_V302	block;
			while( nCount < maxsize 
					&& sizeof(block) == file.Read( &block, sizeof(block) ) )
			{
				pdata[nCount].Clear();
				ConvertQLStockInfo( CStock::marketSHSE, block, &(pdata[nCount]) );

				// read shortname
				if( CSPFile::hFileNull != filePyjc.m_hFile && QL_SHORTNAME_LEN == filePyjc.Read( szShortName, QL_SHORTNAME_LEN ) )
					pdata[nCount].SetStockShortName( szShortName );

				nCount	++;
			}
		}
		else if( version304 == m_nVersion )
		{
			struct QL_Stock_info2_V304	block;
			while( nCount < maxsize 
					&& sizeof(block) == file.Read( &block, sizeof(block) ) )
			{
				pdata[nCount].Clear();
				ConvertQLStockInfo( CStock::marketSHSE, block, &(pdata[nCount]) );

				// read shortname
				if( CSPFile::hFileNull != filePyjc.m_hFile && QL_SHORTNAME_LEN == filePyjc.Read( szShortName, QL_SHORTNAME_LEN ) )
					pdata[nCount].SetStockShortName( szShortName );

				nCount	++;
			}
		}

		if( CSPFile::hFileNull != filePyjc.m_hFile )
			filePyjc.Close();
		file.Close();
	}

	sFileName	= GetRootPath();
	sFileName	+= ml_sz_now;
	if( file.Open( sFileName, CSPFile::modeRead | CSPFile::shareDenyNone ) )
	{
		// pin yin file
		CSPString	sFileNamePyjc	= GetRootPath();
		sFileNamePyjc	+= ml_sz_pyjc;
		CSPFile	filePyjc;
		filePyjc.Open( sFileNamePyjc, CSPFile::modeRead | CSPFile::shareDenyNone );

		if( version302 == m_nVersion )
		{
			struct QL_Stock_info_V302	block;
			while( nCount < maxsize 
					&& sizeof(block) == file.Read( &block, sizeof(block) ) )
			{
				pdata[nCount].Clear();
				ConvertQLStockInfo( CStock::marketSZSE, block, &(pdata[nCount]) );

				// read shortname
				if( CSPFile::hFileNull != filePyjc.m_hFile && QL_SHORTNAME_LEN == filePyjc.Read( szShortName, QL_SHORTNAME_LEN ) )
					pdata[nCount].SetStockShortName( szShortName );

				nCount	++;
			}
		}
		else if( version304 == m_nVersion )
		{
			struct QL_Stock_info2_V304	block;
			while( nCount < maxsize 
					&& sizeof(block) == file.Read( &block, sizeof(block) ) )
			{
				pdata[nCount].Clear();
				ConvertQLStockInfo( CStock::marketSZSE, block, &(pdata[nCount]) );

				// read shortname
				if( CSPFile::hFileNull != filePyjc.m_hFile && QL_SHORTNAME_LEN == filePyjc.Read( szShortName, QL_SHORTNAME_LEN ) )
					pdata[nCount].SetStockShortName( szShortName );

				nCount	++;
			}
		}
		file.Close();
	}

	container.SetSize( nCount );
	return	nCount;
}

int	CQianlong::StoreCodetable( CStockContainer & container )
{
	SP_ASSERT( FALSE );
	return 0;
}

int	CQianlong::LoadKDataCache( CStockContainer & container, PROGRESS_CALLBACK fnCallback, void *cookie, int nProgStart, int nProgEnd )
{
	SP_ASSERT( m_bIsOK );
	if( !m_bIsOK )	return 0;

	UINT nCacheDays = AfxGetProfile().GetCacheDays();

	// 读取行情缓存
	SP_ASSERT( nProgStart <= nProgEnd );
	int nCount = container.GetSize();
	int	nCacheCount		= 0;
	int	nProgressSegment	= nCount / 25;
	double	dProgressRatio		= ( 0 == nCount ? 1 : (nProgEnd-nProgStart)/nCount );
	for( int i=0; i<nCount; i++ )
	{
		container.Lock();

		CStockInfo	& info = container.ElementAt(i);
		
		DWORD	dwMarket	= info.GetMarket();
		CSPString	sCode	= info.GetStockCode();

		// Get Data From Day K Line
		CSPString	sFileName;
		GetFileName( sFileName, CStock::dataK, &info, CKData::ktypeDay );
		CSPFile	file;
		if( file.Open( sFileName, CSPFile::modeRead | CSPFile::shareDenyNone ) )
		{
			DWORD	dwFileLen	= file.GetLength();
			struct	QL_Data_day	qlkd;
			info.m_kdata.SetSize( 0, nCacheDays+3 );
			if( dwFileLen > sizeof(qlkd)*(nCacheDays+1) )
				file.Seek( dwFileLen-sizeof(qlkd)*(nCacheDays+1), CSPFile::begin );
			
			while( sizeof(qlkd) == file.Read( &qlkd, sizeof(qlkd) ) )
			{
				KDATA	kd;
				convert_QL_Data_day_to_KDATA( dwMarket, sCode, &qlkd, &kd );
				info.m_kdata.Add( kd );
			}
			file.Close();
			nCacheCount	++;

			if( fnCallback && !(nCacheCount % nProgressSegment) )
				fnCallback( PROG_PROGRESS, (int)(nProgStart+nCacheCount*dProgressRatio), NULL, cookie );
		}

		container.UnLock();
	}

	return	nCount;
}

int CQianlong::LoadBasetable( CStockContainer & container )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CQianlong::StoreBasetable( CStockContainer & container )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CQianlong::LoadBaseText( CStock *pstock )
{
	SP_ASSERT( m_bIsOK && pstock && pstock->GetStockInfo().IsValidStock() );
	if( ! m_bIsOK || ! pstock || !pstock->GetStockInfo().IsValidStock() )	return 0;

	CSPString	sFileName;
	GetFileName( sFileName, CStock::dataBasetext, &(pstock->GetStockInfo()) );
	
	int	nCount	= 0;
	CSPFile	file;
	if( file.Open( sFileName, CSPFile::modeRead | CSPFile::shareDenyNone ) )
	{
		DWORD dwLen = file.GetLength();
		if( pstock->AllocBaseTextMem( dwLen ) )
		{
			nCount = file.Read( pstock->GetBaseTextPtr(), dwLen );
		}
		file.Close();
	}

	return nCount;
}

int CQianlong::LoadKData( CStock *pstock, int nKType )
{
	SP_ASSERT( m_bIsOK && pstock && pstock->GetStockInfo().IsValidStock() );
	if( ! m_bIsOK || ! pstock || !pstock->GetStockInfo().IsValidStock() )	return 0;

	CKData	*	pkdata	= NULL;

	DWORD	dwMarket	= pstock->GetStockInfo().GetMarket();
	CSPString	sCode	= pstock->GetStockCode();

	CSPString	sFileName;
	GetFileName( sFileName, CStock::dataK, &(pstock->GetStockInfo()), nKType );
	
	switch( nKType )
	{
	case CKData::ktypeMonth:
		pkdata	= &(pstock->GetKDataMonth());
		break;
	case CKData::ktypeWeek:
		pkdata	= &(pstock->GetKDataWeek());
		break;
	case CKData::ktypeDay:
		pkdata	= &(pstock->GetKDataDay());
		break;
	case CKData::ktypeMin60:
		return	0;
	case CKData::ktypeMin30:
		return	0;
	case CKData::ktypeMin15:
		return	0;
	case CKData::ktypeMin5:
		pkdata	= &(pstock->GetKDataMin5());
		break;
	default:
		return 0;
	}

	if( CKData::IsDayOrMin(nKType) )
	{
		CSPFile	file;
		if( pkdata && file.Open( sFileName, CSPFile::modeRead | CSPFile::shareDenyNone ) )
		{
			DWORD dwLen = file.GetLength();
			struct	QL_Data_day		qlkd;
			int	nSize	= dwLen/sizeof(qlkd);
			
			int	nOldMaindataType = pkdata->GetMaindataType();
			pkdata->Clear();
			pkdata->SetKType( nKType );
			pkdata->SetMaindataType( nOldMaindataType );

			pkdata->SetSize( 0, nSize+1 );
			while( sizeof(qlkd) == file.Read( &qlkd, sizeof(qlkd) ) )
			{
				KDATA	kd;
				convert_QL_Data_day_to_KDATA( dwMarket, sCode, &qlkd, &kd );
				pkdata->Add( kd );
			}

			file.Close();
		}
	}
	else
	{
		CSPFile	file;
		if( pkdata && file.Open( sFileName, CSPFile::modeRead | CSPFile::shareDenyNone ) )
		{
			DWORD dwLen = file.GetLength();
			struct	QL_Data_5min		qlkd;
			int	nSize	= dwLen/sizeof(qlkd);
			
			int	nOldMaindataType = pkdata->GetMaindataType();
			pkdata->Clear();
			pkdata->SetKType( nKType );
			pkdata->SetMaindataType( nOldMaindataType );

			pkdata->SetSize( 0, nSize+1 );
			while( sizeof(qlkd) == file.Read( &qlkd, sizeof(qlkd) ) )
			{
				KDATA	kd;
				convert_QL_Data_5min_to_KDATA( dwMarket, sCode, &qlkd, &kd );
				pkdata->Add( kd );
			}

			file.Close();
		}
	}

	return pkdata->GetSize();
}

int CQianlong::LoadDRData( CStock *pstock )
{
	return 0;
}

int CQianlong::StoreDRData( CStock *pstock )
{
	return 0;
}

int CQianlong::LoadReport( CStock *pstock )
{
	// load shtrace.dat sztrace.dat
	return 0;
}

int CQianlong::LoadMinute( CStock *pstock )
{
	// load shminute.dat szminute.dat
	return 0;
}

int CQianlong::LoadOutline( CStock *pstock )
{
	return 0;
}

int	CQianlong::StoreReport( REPORT * pReport, int nCount, BOOL bBigTrade )
{
	SP_ASSERT( FALSE );
	return 0;
}

int	CQianlong::StoreMinute( MINUTE * pMinute, int nCount )
{
	SP_ASSERT( FALSE );
	return 0;
}

int	CQianlong::StoreOutline( OUTLINE * pOutline, int nCount )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CQianlong::DetectVersion( const char * szRootPath )
{
	int	nRet	= versionUnknown;
	if( ! szRootPath )	return nRet;

	int	 nCount	= 0;
	CSPString	sFileName	= szRootPath;
	sFileName	+= ml_sh_now;
	CSPFile	file;
	if( file.Open( sFileName, CSPFile::modeRead | CSPFile::shareDenyNone ) )
	{
		int		nErrorCount	= 1;
		int		nTotalCount	= 1;

		struct QL_Stock_info_V302	block;
		while( sizeof(block) == file.Read( &block, sizeof(block) ) )
		{
			nTotalCount	++;
			if( block.data_id != 0 && block.data_id != 0xFF && block.data_id != 0x94 )
			{
				nErrorCount	++;
			}
		}

		if( (nErrorCount << 5) < nTotalCount )
		{
			nRet	= version302;
		}
		else
		{
			nErrorCount	= 1;
			nTotalCount	= 1;
			struct QL_Stock_info2_V304	block2;
			file.SeekToBegin();
			while( sizeof(block2) == file.Read( &block2, sizeof(block2) ) )
			{
				nTotalCount	++;
				if( block2.data_id != 0 && block2.data_id != 0xFF )
				{
					nErrorCount	++;
				}
			}
			if( (nErrorCount << 5) < nTotalCount )
				nRet	= version304;
		}
		
		file.Close();
	}
	
	return nRet;
}

BOOL CQianlong::GetAccurateRoot( const char * rootpath, char *accurateroot, int maxlen )
{
	if( 0 == rootpath || strlen(rootpath)==0 )
		return FALSE;

	// get rootpath
	CSPString	strRoot	= rootpath;
	int nLen = strRoot.GetLength();
	if( strRoot[nLen-1] != '\\' && strRoot[nLen-1] != '/' )
		strRoot	+= CHAR_DIRSEP;
	nLen = strRoot.GetLength();

	//if( 0 != access( strRoot + ml_sh_now, 0 ) )
	//	return FALSE;

	if( 0 != access( strRoot + ml_sh, 0 ) )
		return FALSE;
	//if( 0 != access( strRoot + ml_sh_base, 0 ) )
	//	return FALSE;
	//if( 0 != access( strRoot + ml_sh_day, 0 ) )
	//	return FALSE;
	//if( 0 != access( strRoot + ml_sh_min, 0 ) )
	//	return FALSE;

	strncpy( accurateroot, strRoot, maxlen-1);
	accurateroot[maxlen-1]	= '\0';

	return TRUE;
}

int CQianlong::InstallCodetbl( const char * filename, const char *orgname )
{
	if( NULL == filename || strlen(filename) == 0
		|| NULL == orgname || strlen(orgname) == 0 )
		return 0;

	CSPString	sFileName	= GetRootPath();
	sFileName	+= ml_dat;
	sFileName	+= orgname;

	return CSPFile::CopyFile( filename, sFileName, FALSE );
}

int CQianlong::InstallCodetblBlock( const char * filename, const char *orgname )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CQianlong::InstallCodetblFxjBlock( const char * filename, const char *orgname )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CQianlong::InstallKData( CKData &kdata, BOOL bOverwrite )
{
	UINT nOpenFlags = CSPFile::modeCreate | CSPFile::modeReadWrite;
	if( !bOverwrite )	nOpenFlags |= CSPFile::modeNoTruncate;

	int	nCount	= 0;
	CSPFile		fileTo;
	CSPString	sFileName;
	CStockInfo	stockinfo;

	DWORD		dwMarket	= kdata.ElementAt(0).m_dwMarket;
	CSPString	sCode		= kdata.ElementAt(0).m_szCode;

	stockinfo.SetStockCode( dwMarket, sCode );
	if( !GetFileName( sFileName, CStock::dataK, &stockinfo, kdata.GetKType() ) )
		return 0;
	
	if( CKData::IsDayOrMin(kdata.GetKType()) )
	{
		if( fileTo.Open( sFileName, nOpenFlags ) )
		{
			for( int i=0; i<kdata.GetSize(); i++ )
			{
				struct QL_Data_day	qlkdnew;
				convert_KDATA_to_QL_Data_day( &(kdata.ElementAt(i)), &qlkdnew );

				struct QL_Data_day	qlkd;
				memset( &qlkd, 0, sizeof(qlkd) );
				BOOL	bHas	= FALSE;
				BOOL	bInsert	= FALSE;
				while( sizeof(qlkd) == fileTo.Read( &qlkd, sizeof(qlkd) ) )
				{
					if( qlkd.day_date == qlkdnew.day_date )
					{
						bHas	= TRUE;
						fileTo.Seek( fileTo.GetPosition()-sizeof(qlkd), CSPFile::begin );
						break;
					}
					if( qlkd.day_date > qlkdnew.day_date )
					{
						bInsert	= TRUE;
						break;
					}
				}
				if( bHas || !bInsert )
					fileTo.Write( &qlkdnew, sizeof(qlkdnew) );
				else if( bInsert )
				{
					int nCur = fileTo.GetPosition();
					int nLen = fileTo.GetLength();
					BYTE * pbuffer = new BYTE[nLen-nCur+1];
					if( pbuffer )
					{
						if( nLen - nCur > 0 )	fileTo.Read( pbuffer, nLen-nCur );
						fileTo.Seek( nCur-sizeof(qlkd), CSPFile::begin );
						fileTo.Write( &qlkdnew, sizeof(qlkdnew) );
						fileTo.Write( &qlkd, sizeof(qlkd) );
						if( nLen - nCur > 0 )	fileTo.Write( pbuffer, nLen-nCur );
						delete [] pbuffer;
					}
					fileTo.Seek( nCur, CSPFile::begin );
				}
				nCount	++;
			}
		}
	}
	else
	{
		if( fileTo.Open( sFileName, nOpenFlags ) )
		{
			for( int i=0; i<kdata.GetSize(); i++ )
			{
				struct QL_Data_5min	qlkdnew;
				convert_KDATA_to_QL_Data_5min( &(kdata.ElementAt(i)), &qlkdnew );

				struct QL_Data_5min	qlkd;
				memset( &qlkd, 0, sizeof(qlkd) );
				BOOL	bHas	= FALSE;
				BOOL	bInsert	= FALSE;
				while( sizeof(qlkd) == fileTo.Read( &qlkd, sizeof(qlkd) ) )
				{
					if( qlkd.min_off == qlkdnew.min_off )
					{
						bHas	= TRUE;
						fileTo.Seek( fileTo.GetPosition()-sizeof(qlkd), CSPFile::begin );
						break;
					}
					if( qlkd.min_off > qlkdnew.min_off )
					{
						bInsert	= TRUE;
						break;
					}
				}
				if( bHas || !bInsert )
					fileTo.Write( &qlkdnew, sizeof(qlkdnew) );
				else if( bInsert )
				{
					int nCur = fileTo.GetPosition();
					int nLen = fileTo.GetLength();
					BYTE * pbuffer = new BYTE[nLen-nCur+1];
					if( pbuffer )
					{
						if( nLen - nCur > 0 )	fileTo.Read( pbuffer, nLen-nCur );
						fileTo.Seek( nCur-sizeof(qlkd), CSPFile::begin );
						fileTo.Write( &qlkdnew, sizeof(qlkdnew) );
						fileTo.Write( &qlkd, sizeof(qlkd) );
						if( nLen - nCur > 0 )	fileTo.Write( pbuffer, nLen-nCur );
						delete [] pbuffer;
					}
					fileTo.Seek( nCur, CSPFile::begin );
				}
				nCount	++;
			}
		}
	}

	return nCount;
}

int CQianlong::InstallKDataTy( const char * stkfile, int nKType,
							PROGRESS_CALLBACK fnCallback, void *cookie )
{
	if( NULL == stkfile || strlen(stkfile) == 0 )
		return 0;

	int	nTotalRecordCount	= 0;
	int	nProgCount	= 0;
	int	nCount		= 0;

	CSPFile	file;
	if( file.Open( stkfile, CSPFile::modeRead ) )
	{
		TYDAY_FHEADER	header;
		if( sizeof(header) == file.Read(&header,sizeof(header)) )
		{
			nTotalRecordCount	= header.recordcount;
			TYDAY_RECORD	block;
			CKData			kdata( nKType );
			CSPString		sCurCode;
			while( sizeof(block) == file.Read(&block,sizeof(block)) )
			{
				nProgCount	++;
				if( fnCallback && nTotalRecordCount > 0 )
					fnCallback( PROG_PROGRESS, DWORD(STKLIB_MAXF_PROGRESS*nProgCount/nTotalRecordCount), NULL, cookie );

				KDATA	kdnew;
				if( CKData::IsDayOrMin(nKType) )
					convert_TYDAY_RECORD_to_KDATA( &block, &kdnew );
				else
					convert_TYDAY_RECORD_MIN_to_KDATA( &block, &kdnew );

				if( sCurCode.IsEmpty() )
					sCurCode	= kdnew.m_szCode;

				if( 0 == strcmp(kdnew.m_szCode,sCurCode) )
					kdata.Add( kdnew );
				else
				{
					nCount += InstallKData( kdata );
					kdata.RemoveAll();

					kdata.Add( kdnew );
					sCurCode	= kdnew.m_szCode;
				}
			}
			
			if( kdata.GetSize() > 0 )
			{
				nCount += InstallKData( kdata );
				kdata.RemoveAll();
			}
		}
		file.Close();
	}

	if( fnCallback )
		fnCallback( PROG_PROGRESS, STKLIB_MAX_PROGRESS, NULL, cookie );

	return nCount;
}

int CQianlong::InstallKDataFxj( const char * dadfile, int nKType,
							PROGRESS_CALLBACK fnCallback, void *cookie )
{
	if( NULL == dadfile || strlen(dadfile) == 0 )
		return 0;

	int	nTotalStockCount	= 0;
	int	nProgCount	= 0;
	int	nCount		= 0;

	CSPFile	file;
	if( file.Open( dadfile, CSPFile::modeRead ) )
	{
		FXJDAY_FHEADER	header;
		if( sizeof(header) == file.Read(&header,sizeof(header)) )
		{
			nTotalStockCount	= header.m_dwStockCount;
			FXJDAY_RECORD	block;
			CKData			kdata( nKType );
			DWORD			dwMarket = CStock::marketUnknown;
			CSPString		sCurCode;
			while( sizeof(block) == file.Read(&block,sizeof(block)) )
			{
				if( -1 == block.m_dwMagic )
				{
					if( kdata.GetSize() > 0 )
						nCount += InstallKData( kdata );
					kdata.RemoveAll();

					if( 'HS' == block.m_wMarket )
						dwMarket = CStock::marketSHSE;
					else if( 'ZS' == block.m_wMarket )
						dwMarket = CStock::marketSZSE;
					else
						dwMarket = CStock::marketUnknown;

					char	code[sizeof(block.m_szCode)+2];
					memset( code, 0, sizeof(code) );
					strncpy( code, block.m_szCode, min(sizeof(code)-1,sizeof(block.m_szCode)) );
					sCurCode	= code;

					nProgCount	++;
					if( fnCallback && nTotalStockCount > 0 )
						fnCallback( PROG_PROGRESS, DWORD(STKLIB_MAXF_PROGRESS*nProgCount/nTotalStockCount), NULL, cookie );
				}
				else
				{
					KDATA	kdnew;
					if( convert_FXJDAY_RECORD_to_KDATA( dwMarket, sCurCode, nKType, &block, &kdnew ) )
						kdata.Add( kdnew );
				}
			}
			
			if( kdata.GetSize() > 0 )
			{
				nCount += InstallKData( kdata );
				kdata.RemoveAll();
			}
		}
		file.Close();
	}

	if( fnCallback )
		fnCallback( PROG_PROGRESS, STKLIB_MAX_PROGRESS, NULL, cookie );

	return nCount;
}

int CQianlong::InstallDRData( CDRData & drdata )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CQianlong::InstallDRDataClk( const char * filename, const char *orgname )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CQianlong::InstallDRDataFxj( const char * fxjfilename )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CQianlong::InstallBasetable( const char * filename, const char * orgname )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CQianlong::InstallBasetableTdx( const char * filename )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CQianlong::InstallBasetableFxj( const char * filename )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CQianlong::InstallBaseText( const char * filename, const char *orgname )
{
	if( NULL == filename || strlen(filename) == 0
		|| NULL == orgname || strlen(orgname) == 0 )
		return 0;

	CSPString	sCode	= orgname;
	int nIndex = sCode.Find( '.' );
	if( -1 != nIndex )
	{
		sCode	= sCode.Left(nIndex);
		CStockInfo	stockinfo;
		stockinfo.SetStockCode( CStock::marketUnknown, sCode );
		CSPString	sFileName;
		if( GetFileName( sFileName, CStock::dataBasetext, &stockinfo ) )
		{
			return CSPFile::CopyFile( filename, sFileName, FALSE );
		}
	}
	
	return 0;
}

int CQianlong::InstallBaseText( const char * buffer, int nLen, const char *orgname )
{
	if( NULL == buffer || nLen <= 0
		|| NULL == orgname || strlen(orgname) == 0 )
		return 0;

	CSPString	sCode	= orgname;
	int nIndex = sCode.Find( '.' );
	if( -1 != nIndex )
	{
		sCode	= sCode.Left(nIndex);
		CStockInfo	stockinfo;
		stockinfo.SetStockCode( CStock::marketUnknown, sCode );
		CSPString	sFileName;
		CSPFile		file;
		if( GetFileName( sFileName, CStock::dataBasetext, &stockinfo )
			&& file.Open(sFileName,CSPFile::modeWrite|CSPFile::modeCreate) )
		{
			file.Write( buffer, nLen );
			file.Close();
			return 1;
		}
	}
	
	return 0;
}

int CQianlong::InstallNewsText( const char * filename, const char *orgname )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CQianlong::InstallNewsText( const char * buffer, int nLen, const char *orgname )
{
	SP_ASSERT( FALSE );
	return 0;
}

BOOL CQianlong::GetFileName( CSPString &sFileName, int nDataType,
						CStockInfo * pInfo, int nKType )
{
	if( NULL == pInfo || !pInfo->IsValidStock() )
		return FALSE;

	// 确定市场类型
	if( CStock::marketUnknown == pInfo->GetMarket() )
		pInfo->ResolveTypeAndMarket( );

	if( CStock::dataBasetext == nDataType )
	{
		sFileName	= GetRootPath();
		if( pInfo->IsShenZhen() )
			sFileName	+= ml_sz_base;
		else
			sFileName	+= ml_sh_base;
		sFileName	+= CSPString(pInfo->GetStockCode()) + ml_ext_base;
		return TRUE;
	}
	else if( CStock::dataK == nDataType )
	{
		sFileName	= GetRootPath();
		if( pInfo->IsShenZhen() )
			sFileName	+= ml_sz;
		else
			sFileName	+= ml_sh;
			
		switch( nKType )
		{
		case CKData::ktypeMonth:
			sFileName	+= CSPString(ml_month) + pInfo->GetStockCode() + ml_ext_month;
			break;
		case CKData::ktypeWeek:
			sFileName	+= CSPString(ml_week) + pInfo->GetStockCode() + ml_ext_week;
			break;
		case CKData::ktypeDay:
			sFileName	+= CSPString(ml_day) + pInfo->GetStockCode() + ml_ext_day;
			break;
		case CKData::ktypeMin60:
			return	FALSE;
		case CKData::ktypeMin30:
			return	FALSE;
		case CKData::ktypeMin15:
			return	FALSE;
		case CKData::ktypeMin5:
			sFileName	+= CSPString(ml_min5) + pInfo->GetStockCode() + ml_ext_min5;
			break;
		default:
			SP_ASSERT( FALSE );
			return FALSE;
		}
		return TRUE;
	}
	else if( CStock::dataDR == nDataType )
	{
		return FALSE;
	}

	return FALSE;
}
