/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CShengLong;
*/

#include	"StdAfx.h"
#include	"../Include/Database.h"
#include	"../Include/SpString.h"
#include	"ShengLong.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////////////
// class	CShenglong

CShenglong::CShenglong( const char * rootpath, BOOL bOK )
{
	m_bIsOK	=	FALSE;
	memset( m_szRootPath, 0, sizeof(m_szRootPath) );
	if( !bOK )
	{
		if( GetAccurateRoot( rootpath, m_szRootPath, sizeof(m_szRootPath)-1 ) )
			m_bIsOK	=	TRUE;
		else
			m_bIsOK	=	FALSE;
	}
	else
	{
		strncpy( m_szRootPath, rootpath, sizeof(m_szRootPath)-1 );
		m_bIsOK	=	TRUE;
	}
}

CShenglong::~CShenglong( )
{
}

int	CShenglong::GetMaxStockNumber( )
{
	SP_ASSERT( FALSE );
	return	2048;
}

int	CShenglong::LoadCodetable( CStockContainer & container )
{
	SP_ASSERT( FALSE );
	return 0;
}

int	CShenglong::StoreCodetable( CStockContainer & container )
{
	SP_ASSERT( FALSE );
	return 0;
}

int	CShenglong::LoadKDataCache( CStockContainer & container, PROGRESS_CALLBACK fnCallback, void *cookie, int nProgStart, int nProgEnd )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::LoadBasetable( CStockContainer & container )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::StoreBasetable( CStockContainer & container )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::LoadBaseText( CStock *pstock )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::LoadKData( CStock *pstock, int nKType )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::LoadDRData( CStock *pstock )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::StoreDRData( CStock *pstock )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::LoadReport( CStock *pstock )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::LoadMinute( CStock *pstock )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::LoadOutline( CStock *pstock )
{
	SP_ASSERT( FALSE );
	return 0;
}

int	CShenglong::StoreReport( REPORT * pReport, int nCount, BOOL bBigTrade )
{
	SP_ASSERT( FALSE );
	return 0;
}

int	CShenglong::StoreMinute( MINUTE * pMinute, int nCount )
{
	SP_ASSERT( FALSE );
	return 0;
}

int	CShenglong::StoreOutline( OUTLINE * pOutline, int nCount )
{
	SP_ASSERT( FALSE );
	return 0;
}

BOOL CShenglong::GetAccurateRoot( const char * rootpath, char *accurateroot, int maxlen )
{
	if( 0 == rootpath || strlen(rootpath)==0 )
		return FALSE;

	// get rootpath
	CSPString	strRoot	=	rootpath;
	int nLen = strRoot.GetLength();
	if( strRoot[nLen-1] != '\\' && strRoot[nLen-1] != '/' )
		strRoot	+=	CHAR_DIRSEP;
	nLen = strRoot.GetLength();

	SP_ASSERT( FALSE );
	return FALSE;
}

int CShenglong::InstallCodetbl( const char * filename, const char *orgname )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::InstallCodetblBlock( const char * filename, const char *orgname )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::InstallCodetblFxjBlock( const char * filename, const char *orgname )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::InstallKData( CKData &kdata, BOOL bOverwrite )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::InstallKDataTy( const char * stkfile, int nKType,
							 PROGRESS_CALLBACK fnCallback, void *cookie )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::InstallKDataFxj( const char * dadfile, int nKType,
							 PROGRESS_CALLBACK fnCallback, void *cookie )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::InstallDRData( CDRData & drdata )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::InstallDRDataClk( const char * filename, const char *orgname )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::InstallDRDataFxj( const char * fxjfilename )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::InstallBasetable( const char * filename, const char *orgname )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::InstallBasetableTdx( const char * filename )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::InstallBasetableFxj( const char * filename )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::InstallBaseText( const char * filename, const char *orgname )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::InstallBaseText( const char * buffer, int nLen, const char *orgname )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::InstallNewsText( const char * filename, const char *orgname )
{
	SP_ASSERT( FALSE );
	return 0;
}

int CShenglong::InstallNewsText( const char * buffer, int nLen, const char *orgname )
{
	SP_ASSERT( FALSE );
	return 0;
}

BOOL CShenglong::GetFileName( CSPString &sFileName, int nDataType,
						CStockInfo * pInfo, int nKType )
{
	SP_ASSERT( FALSE );
	return FALSE;
}
