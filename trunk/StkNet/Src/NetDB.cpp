/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CPackages;
		class	CNetDatabase;
*/

#include "stdafx.h"
#include "../Include/XMLDoc.h"
#include "../Include/HttpClt.h"
#include "../Include/NetDB.h"
#include "../Include/InfoZip.h"
#include "Strings.h"
#include <direct.h>
#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const char *szMarkupStockana	=	"stockana";				//	For CMarkup Use
const char *szMarkupSetting		=	"setting";				//	For CMarkup Use
const char *szMarkupServer		=	"server";				//	For CMarkup Use
const char *szMarkupQuoteServer	=	"quoteserver";			//	For CMarkup Use
const char *szMarkupAlias		=	"alias";				//	For CMarkup Use
const char *szMarkupPackage		=	"package";				//	For CMarkup Use
const char *szMarkupElement		=	"element";				//	For CMarkup Use

const char *szSettingElement	=	"/stockana/setting/element";
	const char *szSettingType		=	"type";
	const char *szSettingValue		=	"value";
	const char *szSettingVersion	=	"version";
	const char *szSettingDataVersion=	"dataversion";
	const char *szSettingBasepath	=	"basepath";
	const char *szSettingYearfmt	=	"yearfmt";
	const char *szSettingMonthfmt	=	"monthfmt";
	const char *szSettingDayfmt		=	"dayfmt";
	const char *szSettingInfo		=	"info";
	const char *szSettingInfourl	=	"infourl";
	const char *szSettingLatestver	=	"latestver";
const char *szServerElement		=	"/stockana/server/element";
	const char *szServerAddress		=	"address";
	const char *szServerPort		=	"port";
	const char *szServerDescript	=	"discript";

const char *szQuoteServerElement	=	"/stockana/quoteserver/element";
	const char *szQuoteServerName	=	"name";
	const char *szQuoteServerAddress=	"address";
	const char *szQuoteServerPort	=	"port";
	const char *szQuoteServerUser	=	"user";
	const char *szQuoteServerPasswd	=	"passwd";
const char *szAliasElement		=	"/stockana/alias/element";
	const char *szAliasName			=	"name";
	const char *szAliasValue		=	"value";
const char *szPackageElement	=	"/stockana/package/element";
	const char *szPackageURL		=	"url";
	const char *szPackageType		=	"type";
	const char *szPackageTypeCode		=	"code";	
	const char *szPackageTypeDay		=	"day";
	const char *szPackageTypeMin5		=	"min5";
	const char *szPackageTypeDR			=	"dr";
	const char *szPackageTypeBasetext	=	"basetext";
	const char *szPackageTypeBasetable	=	"basetable";
	const char *szPackageLength		=	"length";
	const char *szPackageDescript	=	"discript";
	const char *szPackageFrom		=	"from";
	const char *szPackageTo			=	"to";
	const char *szPackageZipStatus	=	"zipstatus";
	const char *szPackageTimefmt	=	"timefmt";
	const char *szPackageAlternateURL	=	"alter";

//////////////////////////////////////////////////////////////////////////////////
//	class CPackage

CPackage::CPackage()
{
	m_nType		=	packageUnknown;
	m_nLength	=	0;
	m_tmFrom	=	CSPTime(NULL);
	m_tmTo		=	CSPTime(NULL);
	m_bIsZipped	=	FALSE;
	m_bIsTimefmt=	FALSE;

	m_bDownloadOK	=	FALSE;
	m_pExpandPackages	=	NULL;
}

CPackage::CPackage( const CPackage &src )
{
	m_pExpandPackages	=	NULL;
	*this	=	src;
}

CPackage::~CPackage( )
{
	if( m_pExpandPackages )
	{
		delete	m_pExpandPackages;
		m_pExpandPackages	=	NULL;
	}
}

CPackage & CPackage::operator = ( const CPackage &src )
{
	if( m_pExpandPackages )
	{
		delete	m_pExpandPackages;
		m_pExpandPackages	=	NULL;
	}

	m_strURL			=	src.m_strURL;
	m_strAlternateURL	=	src.m_strAlternateURL;
	m_nType				=	src.m_nType;
	m_tmFrom			=	src.m_tmFrom;
	m_tmTo				=	src.m_tmTo;
	m_nLength			=	src.m_nLength;
	m_strDescript		=	src.m_strDescript;
	m_bIsZipped			=	src.m_bIsZipped;
	m_bIsTimefmt		=	src.m_bIsTimefmt;

	// temp data
	m_bDownloadOK		=	src.m_bDownloadOK;
	m_strLocalFilename	=	src.m_strLocalFilename;
	if( src.HasExpandPackage() )
	{
		m_pExpandPackages	=	new CPackages;
		m_pExpandPackages->AppendPackages( src.m_pExpandPackages );
	}

	return *this;
}

BOOL CPackage::SetAttribute( CMarkup & markup )
{
	m_nType	=	packageUnknown;
	CString	strType	=	CXMLNode::getAttrValue( markup, szPackageType );
	if( 0 == strType.CompareNoCase( szPackageTypeCode ) )
		m_nType	=	packageCode;
	else if( 0 == strType.CompareNoCase( szPackageTypeDay ) )
		m_nType	=	packageDay;
	else if( 0 == strType.CompareNoCase( szPackageTypeMin5 ) )
		m_nType	=	packageMin5;
	else if( 0 == strType.CompareNoCase( szPackageTypeDR ) )
		m_nType	=	packageDR;
	else if( 0 == strType.CompareNoCase( szPackageTypeBasetext ) )
		m_nType	=	packageBasetext;
	else if( 0 == strType.CompareNoCase( szPackageTypeBasetable ) )
		m_nType	=	packageBasetable;

	m_strURL	=	CXMLNode::getAttrValue( markup, szPackageURL );
	m_strAlternateURL	=	CXMLNode::getAttrValue( markup, szPackageAlternateURL );
	m_tmFrom	=	CXMLNode::getAttrValue_T( markup, szPackageFrom );
	m_tmTo		=	CXMLNode::getAttrValue_T( markup, szPackageTo );
	m_nLength	=	CXMLNode::getAttrValue_I( markup, szPackageLength );
	m_strDescript	=	CXMLNode::getAttrValue( markup, szPackageDescript );
	m_bIsZipped	=	CXMLNode::getAttrValue_I( markup, szPackageZipStatus );
	m_bIsTimefmt=	CXMLNode::getAttrValue_I( markup, szPackageTimefmt );

	if( m_tmFrom > m_tmTo )
		m_tmFrom	=	m_tmTo;
	
	if( m_tmTo.GetHour() == 0 )
		m_tmTo += CSPTimeSpan( 0, 23, 59, 59 );
	return TRUE;
}

BOOL CPackage::operator<(CPackage& p) const
{
	return (m_tmFrom < p.m_tmFrom);
}

CString CPackage::GetURL( CString strBasePath )
{
	if( !m_bIsTimefmt )
		return strBasePath + m_strURL;
	else
	{
		CSPTime	time	=	CSPTime::GetCurrentTime();
		int nWeekDay = time.GetDayOfWeek( );
		if( nWeekDay == 1 )
		{
			time	-=	CSPTimeSpan( 2, 0, 0, 0 );
		}
		else if( nWeekDay == 7 )
		{
			time	-=	CSPTimeSpan( 1, 0, 0, 0 );
		}
		return strBasePath + (LPCTSTR)time.Format( m_strURL );
	}
}

BOOL CPackage::HasAlternateURL( ) const
{
	return !m_strAlternateURL.IsEmpty();
}

CString	CPackage::GetAlternateURL( CString strBasePath )
{
	return strBasePath + m_strAlternateURL;
}

BOOL CPackage::HasExpandPackage( ) const
{
	return ( m_pExpandPackages && m_pExpandPackages->GetSize() > 0 );
}

BOOL CPackage::ExpandYearPackage( CString strMonthfmt, CString strDayfmt, int nYear )
{
	if( NULL != m_pExpandPackages )
	{
		delete	m_pExpandPackages;
		m_pExpandPackages	=	NULL;
	}

	m_pExpandPackages	=	new CPackages( );

	if( m_pExpandPackages && !strMonthfmt.IsEmpty() )
	{
		int nMonthStart	=	1;
		int	nMonthEnd	=	12;
		for( int nMonth=nMonthStart; nMonth<=nMonthEnd; nMonth ++ )
		{
			CPackage	pac;
			pac.m_bIsTimefmt	=	FALSE;
			pac.m_bIsZipped		=	TRUE;
			pac.m_nLength		=	400000;
			pac.m_nType			=	CPackage::packageDay;
			pac.m_strDescript.Format( szFmtMonthDescript, nYear, nMonth );
			
			CSPTime	tmMonth( nYear, nMonth, 1, 0, 0, 0 );
			pac.m_strURL		=	tmMonth.Format( strMonthfmt );
			pac.m_tmFrom		=	CSPTime( nYear, nMonth, 1, 0, 0, 0 );
			pac.m_tmTo			=	CSPTime( nYear, nMonth, 31, 23, 59, 59 );
			if( nMonth == nMonthEnd )
				pac.ExpandMonthPackage( strDayfmt, nYear, nMonth );

			m_pExpandPackages->Add( pac );
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CPackage::ExpandMonthPackage( CString strDayfmt, int nYear, int nMonth )
{
	if( NULL != m_pExpandPackages )
	{
		delete	m_pExpandPackages;
		m_pExpandPackages	=	NULL;
	}

	m_pExpandPackages	=	new CPackages( );

	if( m_pExpandPackages && !strDayfmt.IsEmpty() )
	{
		int	nDayStart	=	1;
		CSPTime	tmDay( nYear, nMonth, nDayStart, 23, 0, 0 );
		for( ; tmDay.GetMonth() == nMonth; tmDay += CSPTimeSpan(1,0,0,0) )
		{
			CPackage	pac;
			pac.m_bIsTimefmt	=	FALSE;
			pac.m_bIsZipped		=	FALSE;
			pac.m_nLength		=	20000;
			pac.m_nType			=	CPackage::packageDay;
			pac.m_strDescript.Format( szFmtDayDescript, nYear, nMonth, tmDay.GetDay() );
			
			pac.m_strURL		=	tmDay.Format( strDayfmt );
			pac.m_tmFrom		=	CSPTime( nYear, nMonth, tmDay.GetDay(), 0, 0, 0 );
			pac.m_tmTo			=	CSPTime( nYear, nMonth, tmDay.GetDay(), 23, 59, 59 );
			
			if( tmDay.GetDayOfWeek() != 1 && tmDay.GetDayOfWeek() != 7 )
				m_pExpandPackages->Add( pac );
		}
		return TRUE;
	}
	return FALSE;
}

CPackages * CPackage::GetExpandPackages( )
{
	ASSERT( HasExpandPackage() );
	return	m_pExpandPackages;
}

void CPackage::DeleteExpandPackages( )
{
	if( m_pExpandPackages )
	{
		delete	m_pExpandPackages;
		m_pExpandPackages	=	NULL;
	}
}


//////////////////////////////////////////////////////////////////////////////////
//	class CPackages

CPackages::CPackages()
{
	Clear( );
}

CPackages::~CPackages()
{
	Clear();
}

void CPackages::Clear( )
{
	m_bIsOK	=	FALSE;
	m_xmlDocument.Release();

	m_strVersion.Empty();
	m_nDataVersion = 0;
	m_strBasePath.Empty();
	m_strYearfmt.Empty();
	m_strMonthfmt.Empty();
	m_strDayfmt.Empty();
	m_strInfo.Empty();
	m_strInfourl.Empty();
	m_strLatestver.Empty();
	SetSize( 0 );
}

BOOL CPackages::IsOK( )
{
	return m_bIsOK;
}

BOOL CPackages::AddSmartAdditional( )
{
	CSPTime	tmLatest;
	GetLatestTime( tmLatest, CPackage::packageDay );
	CSPTime	tmNow	=	CSPTime::GetCurrentTime();

	CSPTime	tmBegin	=	tmLatest + CSPTimeSpan( 1, 0, 0, 0 );

	if( !m_strYearfmt.IsEmpty() )
	{
		int nYearStart	=	tmBegin.GetYear();
		int	nYearEnd	=	tmNow.GetYear()-1;
		for( int nYear=nYearStart; nYear<=nYearEnd; nYear ++ )
		{
			CPackage	pac;
			pac.m_bIsTimefmt	=	FALSE;
			pac.m_bIsZipped		=	TRUE;
			pac.m_nLength		=	5000000;
			pac.m_nType			=	CPackage::packageDay;
			pac.m_strDescript.Format( szFmtYearDescript, nYear );
			
			CSPTime	tmYear( nYear, tmBegin.GetMonth(), 1, 0, 0, 0 );
			pac.m_strURL		=	tmYear.Format( m_strYearfmt );
			pac.m_tmFrom		=	CSPTime( nYear, 1, 1, 0, 0, 0 );
			pac.m_tmTo			=	CSPTime( nYear, 12, 31, 23, 59, 59 );
			if( nYear == nYearEnd )
				pac.ExpandYearPackage( m_strMonthfmt, m_strDayfmt, nYear );

			Add( pac );
		}
	}
	
	if( !m_strMonthfmt.IsEmpty() )
	{
		int nMonthStart	=	1;
		if( tmBegin.GetYear() == tmNow.GetYear() )
			nMonthStart	=	tmBegin.GetMonth();
		int	nMonthEnd	=	tmNow.GetMonth()-1;
		for( int nMonth=nMonthStart; nMonth<=nMonthEnd; nMonth ++ )
		{
			CPackage	pac;
			pac.m_bIsTimefmt	=	FALSE;
			pac.m_bIsZipped		=	TRUE;
			pac.m_nLength		=	400000;
			pac.m_nType			=	CPackage::packageDay;
			pac.m_strDescript.Format( szFmtMonthDescript, tmNow.GetYear(), nMonth );
			
			CSPTime	tmMonth( tmNow.GetYear(), nMonth, 1, 0, 0, 0 );
			pac.m_strURL		=	tmMonth.Format( m_strMonthfmt );
			pac.m_tmFrom		=	CSPTime( tmNow.GetYear(), nMonth, 1, 0, 0, 0 );
			pac.m_tmTo			=	CSPTime( tmNow.GetYear(), nMonth, 31, 23, 59, 59 );
			if( nMonth == nMonthEnd )
				pac.ExpandMonthPackage( m_strDayfmt, tmNow.GetYear(), nMonth );

			Add( pac );
		}
	}

	if( !m_strDayfmt.IsEmpty() )
	{
		int	nDayStart	=	1;
		if( tmBegin.GetYear() == tmNow.GetYear() && tmBegin.GetMonth() == tmNow.GetMonth() )
			nDayStart	=	tmBegin.GetDay();
		int nDayEnd	=	tmNow.GetDay();
		for( int nDay=nDayStart; nDay<=nDayEnd; nDay ++ )
		{
			CPackage	pac;
			pac.m_bIsTimefmt	=	FALSE;
			pac.m_bIsZipped		=	FALSE;
			pac.m_nLength		=	20000;
			pac.m_nType			=	CPackage::packageDay;
			pac.m_strDescript.Format( szFmtDayDescript, tmNow.GetYear(), tmNow.GetMonth(), nDay );
			
			CSPTime	tmDay( tmNow.GetYear(), tmNow.GetMonth(), nDay, 23, 0, 0 );
			pac.m_strURL		=	tmDay.Format( m_strDayfmt );
			pac.m_tmFrom		=	CSPTime( tmNow.GetYear(), tmNow.GetMonth(), nDay, 0, 0, 0 );
			pac.m_tmTo			=	CSPTime( tmNow.GetYear(), tmNow.GetMonth(), nDay, 23, 59, 59 );
			
			if( tmDay.GetDayOfWeek() != 1 && tmDay.GetDayOfWeek() != 7 )
				Add( pac );
		}
	}

	return TRUE;
}

BOOL CPackages::SetRawXMLDocument( CString &strDoc )
{
	Clear( );

	m_bIsOK	=	FALSE;

	if( m_xmlDocument.Initialize() )
		m_bIsOK	=	m_xmlDocument.SetRawDocument( strDoc );

	if( m_bIsOK )
	{
		m_bIsOK	=	ExtractFromXMLDoc( );
	}

	if( m_bIsOK )
	{
		m_bIsOK	=	AddSmartAdditional( );
	}

	return m_bIsOK;
}

BOOL CPackages::SetAttribute( CMarkup & markup )
{
	CString	strType		=	CXMLNode::getAttrValue( markup, szSettingType );

	if( 0 == strType.CompareNoCase(szSettingVersion) )
		m_strVersion	=	CXMLNode::getAttrValue( markup, szSettingValue );
	else if( 0 == strType.CompareNoCase(szSettingDataVersion) )
		m_nDataVersion	=	CXMLNode::getAttrValue_I( markup, szSettingValue );
	else if( 0 == strType.CompareNoCase(szSettingBasepath) )
		m_strBasePath	=	CXMLNode::getAttrValue( markup, szSettingValue );
	else if( 0 == strType.CompareNoCase(szSettingYearfmt) )
		m_strYearfmt	=	CXMLNode::getAttrValue( markup, szSettingValue );
	else if( 0 == strType.CompareNoCase(szSettingMonthfmt) )
		m_strMonthfmt	=	CXMLNode::getAttrValue( markup, szSettingValue );
	else if( 0 == strType.CompareNoCase(szSettingDayfmt) )
		m_strDayfmt		=	CXMLNode::getAttrValue( markup, szSettingValue );
	else if( 0 == strType.CompareNoCase(szSettingInfo) )
		m_strInfo		=	CXMLNode::getAttrValue( markup, szSettingValue );
	else if( 0 == strType.CompareNoCase(szSettingInfourl) )
		m_strInfourl	=	CXMLNode::getAttrValue( markup, szSettingValue );
	else if( 0 == strType.CompareNoCase(szSettingLatestver) )
		m_strLatestver	=	CXMLNode::getAttrValue( markup, szSettingValue );
	
	return TRUE;
}

BOOL CPackages::ExtractFromXMLDoc( )
{
	CMarkup	&	markup	=	m_xmlDocument.Markup();

	// extract setting
	markup.ResetPos();
	if( !markup.FindElem(szMarkupStockana) )
		return FALSE;
	ASSERT( 0 == markup.GetTagName().Compare( szMarkupStockana ) );
	if( markup.FindChildElem(szMarkupSetting) )
	{
		markup.IntoElem();
		ASSERT( 0 == markup.GetTagName().Compare( szMarkupSetting ) );
		if( markup.FindChildElem( szMarkupElement ) )
		{
			do	{
				markup.IntoElem();
				ASSERT( 0 == markup.GetTagName().Compare( szMarkupElement ) );
				if( !SetAttribute( markup ) )
				{
					m_xmlDocument.SetLastErrorMessage( szErrXMLServerSetting );
					return FALSE;
				}
			}while( markup.FindElem( szMarkupElement ) );
		}
	}

	// extract package
	markup.ResetPos();
	if( !markup.FindElem(szMarkupStockana) )
		return FALSE;
	ASSERT( 0 == markup.GetTagName().Compare( szMarkupStockana ) );
	if( markup.FindChildElem(szMarkupPackage) )
	{
		markup.IntoElem();
		ASSERT( 0 == markup.GetTagName().Compare( szMarkupPackage ) );
		if( markup.FindChildElem( szMarkupElement ) )
		{
			do	{
				markup.IntoElem();
				ASSERT( 0 == markup.GetTagName().Compare( szMarkupElement ) );
				CPackage	package;
				if( package.SetAttribute( markup ) )
					Add( package );
			} while( markup.FindElem( szMarkupElement ) );
		}
	}

	markup.ResetPos();
	return TRUE;
/*	OLD VERSION
	m_strVersion.Empty();
	m_nDataVersion = 0;
	m_strBasePath.Empty();
	SetSize( 0 );

	MSXML::IXMLDOMNodePtr		pNodeRoot = m_xmlDocument.XMLDoc();

	try
	{
		// extract setting
		MSXML::IXMLDOMNodeListPtr	pNodeList = pNodeRoot->selectNodes( szSettingElement );
		if( NULL != pNodeList )
		{
			for( int i=0; i<pNodeList->length; i++ )
			{
				MSXML::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);
				if( !SetAttribute( pNode ) )
				{
					m_xmlDocument.SetLastErrorMessage( szErrXMLPackageSetting );
					return FALSE;
				}
			}
		}

		// extract package
		pNodeList = pNodeRoot->selectNodes( szPackageElement );
		if( NULL == pNodeList || 0 == pNodeList->length )
		{
			m_xmlDocument.SetLastErrorMessage( szErrXMLPackageData );
			return FALSE;
		}

		for( int i=0; i<pNodeList->length; i++ )
		{
			MSXML::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);
			CPackage	package;
			if( package.SetAttribute( pNode ) )
				Add( package );
		}
	}
	catch( _com_error e )
	{
		m_xmlDocument.SetLastErrorMessage( e.ErrorMessage() );
		return FALSE;
	}
	return TRUE;
*/
}

int SortPackage(const void *p1,const void *p2)
{
	CPackage *pPackage1 = (CPackage *)p1;
	CPackage *pPackage2 = (CPackage *)p2;

	if( pPackage1 && pPackage2 && pPackage1->m_tmFrom < pPackage2->m_tmFrom )
		return -1;
	else if( pPackage1 && pPackage2 && pPackage1->m_tmFrom > pPackage2->m_tmFrom )
		return 1;
	return 0;
}

void CPackages::Sort( )
{
	if( NULL != GetData() )
		qsort( GetData(), GetSize(), sizeof(CPackage), SortPackage );
}

void CPackages::AppendPackages( CPackages * psrc )
{
	if( !psrc || psrc->GetSize() == 0 )
		return;

	int	nOldSize	=	GetSize();

	SetSize( nOldSize + psrc->GetSize() );
	for( int i=0; i<psrc->GetSize(); i++ )
	{
		SetAt( nOldSize+i, psrc->GetAt(i) );
	}
}

BOOL CPackages::GetLatestTime( CSPTime &tm, int packagetype )
{
	tm	=	CSPTime(NULL);
	BOOL	bHas	=	FALSE;
	for( int i=0; i<GetSize(); i++ )
	{
		CPackage &	pac	=	ElementAt(i);
		if( pac.m_nType == packagetype
			&& tm < pac.m_tmTo )
		{
			tm	=	pac.m_tmTo;
			bHas	=	TRUE;
		}
	}
	if( !bHas )
		tm	=	CSPTime::GetCurrentTime();
	return TRUE;
}

BOOL CPackages::GetLastErrorMessage(LPTSTR lpszError, UINT nMaxError)
{
	return m_xmlDocument.GetLastErrorMessage( lpszError, nMaxError );
}

//////////////////////////////////////////////////////////////////////////////////
//	class CDownloadServer

CDownloadServer::CDownloadServer( const CDownloadServer &src )
{
	*this	=	src;
}

CDownloadServer & CDownloadServer::operator = ( const CDownloadServer & src )
{
	m_strAddress	=	src.m_strAddress;
	m_nPort			=	src.m_nPort;
	m_strDescript	=	src.m_strDescript;
	return *this;
}

BOOL CDownloadServer::SetAttribute( CMarkup & markup )
{
	m_strAddress	=	CXMLNode::getAttrValue( markup, szServerAddress );
	m_nPort			=	CXMLNode::getAttrValue_I( markup, szServerPort );
	m_strDescript	=	CXMLNode::getAttrValue( markup, szServerDescript );
	
#ifdef CLKLAN_ENGLISH_US
	if( !AfxIsEnglish( m_strDescript, m_strDescript.GetLength() ) )
		m_strDescript.Empty();
#endif

	return TRUE;
}

BOOL CDownloadServer::FromString( CString	string )
{
	m_strAddress.Empty();
	m_nPort	=	INTERNET_DEFAULT_HTTP_PORT;
	m_strDescript.Empty();

	int	nIndex	=	string.Find( ':' );
	if( -1 == nIndex )
	{
		m_strAddress	=	string;
		return m_strAddress.GetLength()>0;
	}
	m_strAddress	=	string.Left( nIndex );
	string	=	string.Mid( nIndex+1 );
	
	nIndex	=	string.Find( ':' );
	if( -1 == nIndex )
	{
		m_nPort	=	(INTERNET_PORT)atol(string);
		return m_strAddress.GetLength()>0;
	}
	m_nPort	=	(INTERNET_PORT)atol(string.Left(nIndex));
	m_strDescript	=	string.Mid(nIndex+1);
	return m_strAddress.GetLength()>0;
}

CString	CDownloadServer::AsString( )
{
	CString	string;
	string.Format( "%s:%d:%s", m_strAddress, m_nPort, m_strDescript );
	return string;
}

BOOL AliasSetAttribute( CMarkup & markup, CAlias & alias )
{
	alias.m_strName		=	CXMLNode::getAttrValue( markup, szAliasName );
	alias.m_strValue		=	CXMLNode::getAttrValue( markup, szAliasValue );
	return TRUE;
}

BOOL	QuoteServerSetAttribute( CMarkup & markup, CQuoteServer & qs )
{
	qs.m_strName		=	CXMLNode::getAttrValue( markup, szQuoteServerName );
	qs.m_strAddress		=	CXMLNode::getAttrValue( markup, szQuoteServerAddress );
	qs.m_nPort			=	CXMLNode::getAttrValue_I( markup, szQuoteServerPort );
	qs.m_strUser		=	CXMLNode::getAttrValue( markup, szQuoteServerUser );
	qs.m_strPasswd		=	CXMLNode::getAttrValue( markup, szQuoteServerPasswd );
	qs.m_bSavePasswd	=	TRUE;
#ifdef CLKLAN_ENGLISH_US
	if( !AfxIsEnglish( qs.m_strName, qs.m_strName.GetLength() ) )
		qs.m_strName = qs.m_strAddress;
#endif
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////
//	class CServers

CServers::CServers()
{
	Clear( );
}

CServers::~CServers()
{
	Clear();
}

void CServers::Clear( )
{
	m_bIsOK	=	FALSE;
	m_xmlDocument.Release();

	m_strVersion.Empty();
	m_aDownloadServers.SetSize( 0 );
	m_aQuoteServers.SetSize( 0 );
	m_aAliases.SetSize( 0 );
}

BOOL CServers::IsOK( )
{
	return m_bIsOK;
}

BOOL CServers::SetRawXMLDocument( CString &strDoc )
{
	Clear( );
	m_bIsOK	=	FALSE;

	if( m_xmlDocument.Initialize() )
		m_bIsOK	=	m_xmlDocument.SetRawDocument( strDoc );

	if( m_bIsOK )
	{
		m_bIsOK	=	ExtractFromXMLDoc( );
	}

	return m_bIsOK;
}

BOOL CServers::SetAttribute( CMarkup & markup )
{
	CString	strType		=	CXMLNode::getAttrValue( markup, szSettingType );

	if( 0 == strType.CompareNoCase(szSettingVersion) )
		m_strVersion	=	CXMLNode::getAttrValue( markup, szSettingValue );
	
	return TRUE;
}

BOOL CServers::ExtractFromXMLDoc( )
{
	CMarkup	&	markup	=	m_xmlDocument.Markup();

	// extract setting
	markup.ResetPos();
	if( !markup.FindElem(szMarkupStockana) )
		return FALSE;
	ASSERT( 0 == markup.GetTagName().Compare( szMarkupStockana ) );
	if( markup.FindChildElem(szMarkupSetting) )
	{
		markup.IntoElem();
		ASSERT( 0 == markup.GetTagName().Compare( szMarkupSetting ) );
		if( markup.FindChildElem( szMarkupElement ) )
		{
			do	{
				markup.IntoElem();
				ASSERT( 0 == markup.GetTagName().Compare( szMarkupElement ) );
				if( !SetAttribute( markup ) )
				{
					m_xmlDocument.SetLastErrorMessage( szErrXMLServerSetting );
					return FALSE;
				}
			}while( markup.FindElem( szMarkupElement ) );
		}
	}

	// extract server
	markup.ResetPos();
	if( !markup.FindElem(szMarkupStockana) )
		return FALSE;
	ASSERT( 0 == markup.GetTagName().Compare( szMarkupStockana ) );
	if( markup.FindChildElem(szMarkupServer) )
	{
		markup.IntoElem();
		ASSERT( 0 == markup.GetTagName().Compare( szMarkupServer ) );
		if( markup.FindChildElem( szMarkupElement ) )
		{
			do	{
				markup.IntoElem();
				ASSERT( 0 == markup.GetTagName().Compare( szMarkupElement ) );
				CDownloadServer	server;
				if( server.SetAttribute( markup ) )
					m_aDownloadServers.Add( server );
			} while( markup.FindElem( szMarkupElement ) );
		}
	}
	
	// extract quote server
	markup.ResetPos();
	if( !markup.FindElem(szMarkupStockana) )
		return FALSE;
	ASSERT( 0 == markup.GetTagName().Compare( szMarkupStockana ) );
	if( markup.FindChildElem(szMarkupQuoteServer) )
	{
		markup.IntoElem();
		ASSERT( 0 == markup.GetTagName().Compare( szMarkupQuoteServer ) );
		if( markup.FindChildElem( szMarkupElement ) )
		{
			do	{
				markup.IntoElem();
				ASSERT( 0 == markup.GetTagName().Compare( szMarkupElement ) );
				CQuoteServer	server;
				if( QuoteServerSetAttribute( markup, server ) )
					m_aQuoteServers.Add( server );
			} while( markup.FindElem( szMarkupElement ) );
		}
	}
	
	// extract alias
	markup.ResetPos();
	if( !markup.FindElem(szMarkupStockana) )
		return FALSE;
	ASSERT( 0 == markup.GetTagName().Compare( szMarkupStockana ) );
	if( markup.FindChildElem(szMarkupAlias) )
	{
		markup.IntoElem();
		ASSERT( 0 == markup.GetTagName().Compare( szMarkupAlias ) );
		if( markup.FindChildElem( szMarkupElement ) )
		{
			do	{
				markup.IntoElem();
				ASSERT( 0 == markup.GetTagName().Compare( szMarkupElement ) );
				CAlias	alias;
				if( AliasSetAttribute( markup, alias ) )
					m_aAliases.Add( alias );
			} while( markup.FindElem( szMarkupElement ) );
		}
	}

	markup.ResetPos();
	return TRUE;

/*	OLD VERSION
	SetSize( 0 );
	m_strVersion.Empty();

	MSXML::IXMLDOMNodePtr		pNodeRoot = m_xmlDocument.XMLDoc();

	try
	{
		// extract setting
		MSXML::IXMLDOMNodeListPtr	pNodeList = pNodeRoot->selectNodes( szSettingElement );
		if( NULL != pNodeList )
		{
			for( int i=0; i<pNodeList->length; i++ )
			{
				MSXML::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);
				if( !SetAttribute( pNode ) )
				{
					m_xmlDocument.SetLastErrorMessage( szErrXMLServerSetting );
					return FALSE;
				}
			}
		}

		// extract server
		pNodeList = pNodeRoot->selectNodes( szServerElement );
		if( NULL == pNodeList || 0 == pNodeList->length )
		{
			return TRUE;
		}

		for( int i=0; i<pNodeList->length; i++ )
		{
			MSXML::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);
			CDownloadServer	server;
			if( server.SetAttribute( pNode ) )
				m_aDownloadServers.Add( server );
		}
	}
	catch( _com_error e )
	{
		m_xmlDocument.SetLastErrorMessage( e.ErrorMessage() );
		return FALSE;
	}
	return TRUE;
*/
}

BOOL CServers::GetLastErrorMessage(LPTSTR lpszError, UINT nMaxError)
{
	return m_xmlDocument.GetLastErrorMessage( lpszError, nMaxError );
}

BOOL CServers::SaveToProfile( )
{
	CSPStringArray	astrServers;
	for( int i=0; i<m_aDownloadServers.GetSize(); i++ )
		astrServers.Add( m_aDownloadServers.ElementAt(i).AsString() );
	AfxGetProfile().SetDownloadServers( astrServers );
	AfxGetProfile().StoreProfile();
	AfxGetQSProfile().AddQuoteServers( m_aQuoteServers );
	AfxGetQSProfile().StoreAliases( m_aAliases );
	AfxGetQSProfile().StoreProfile();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
//	class CNetDatabase

CNetDatabase::CNetDatabase( )
{
}

CNetDatabase::~CNetDatabase( )
{
}

BOOL CNetDatabase::SetServer( LPCTSTR lpszServer, INTERNET_PORT port )
{
	if( !m_httpClient.SetServer( lpszServer, port ) )
	{
		SetLastError( ERR_NETDB_HTTPCLIENT );
		return FALSE;
	}
	return TRUE;
}

BOOL CNetDatabase::SetAccessType( int nAccessType, int nProxyType, LPCTSTR lpszProxyAddress, UINT nProxyPort,
								LPCSTR lpszProxyUser, LPCTSTR lpszProxyPasswd )
{
	if( ! m_httpClient.SetAccessType( nAccessType, nProxyType, lpszProxyAddress, nProxyPort, lpszProxyUser, lpszProxyPasswd ) )
	{
		SetLastError( ERR_NETDB_HTTPCLIENT );
		return FALSE;
	}
	return TRUE;
}

BOOL CNetDatabase::Login( LPCTSTR lpszRegCode, LPCTSTR lpszUserName,
						  LPCTSTR lpszPasswd, LPCTSTR lpszVersion )
{
	if( ! m_httpClient.Login( lpszRegCode, lpszUserName, lpszPasswd, lpszVersion ) )
	{
		SetLastError( ERR_NETDB_HTTPCLIENT );
		return FALSE;
	}
	return TRUE;
}

BOOL CNetDatabase::Logout( )
{
	if( ! m_httpClient.Logout( ) )
	{
		SetLastError( ERR_NETDB_HTTPCLIENT );
		return FALSE;
	}
	return TRUE;
}

BOOL CNetDatabase::GetLastErrorMessage(LPTSTR lpszError, UINT nMaxError )
{
	switch( GetLastError() )
	{
	case ERR_NETDB_HTTPCLIENT:
		return m_httpClient.GetLastErrorMessage( lpszError, nMaxError );
	case ERR_NETDB_PACKAGES:
		return m_packages.GetLastErrorMessage( lpszError, nMaxError );
	case ERR_NETDB_SERVERS:
		return m_servers.GetLastErrorMessage( lpszError, nMaxError );
	case ERR_NETDB_ZIPDLL:
		strncpy( lpszError, szErrZipDll, nMaxError );
		return strlen(lpszError)>0;
	case ERR_NETDB_ZIP:
		strncpy( lpszError, szErrZip, nMaxError );
		return strlen(lpszError)>0;
	default:
		return CStDatabase::GetLastErrorMessage( lpszError, nMaxError );
	}
}

CString CNetDatabase::GetProgressMessage(UINT nProgressCode)
{
	CString	string;
	switch( nProgressCode )
	{
	case PROG_HTTPCONNECTTING:	string	=	szProgHttpConnectting;	break;
	case PROG_REQUESTSENT:		string	=	szProgRequestSent;	break;
	case PROG_REDIRECTING:		string	=	szProgRedirecting;	break;
	case PROG_TRANSFERRING:		string	=	szProgTransferring;	break;
	case PROG_INSTALLPACKAGE:	string	=	szProgInstallPackage;	break;
	case PROG_EXTRACTZIPFILES:	string	=	szProgExtractZipFiles;	break;
	case PROG_ERRORPAC:			string	=	szProgErrorPac;	break;
	default:;
	}
	return string;
}

BOOL CNetDatabase::NetloadPackageInfo( PROGRESS_CALLBACK fnCallback, void *cookie )
{
	CString	strPackageInfo;
	if( !m_httpClient.LoadPackageInfo( strPackageInfo, fnCallback, cookie ) )
	{
		SetLastError( ERR_NETDB_HTTPCLIENT );
		return FALSE;
	}

	if( ! m_packages.SetRawXMLDocument( strPackageInfo ) )
	{
		SetLastError( ERR_NETDB_PACKAGES );
		return FALSE;
	}

	AfxGetProfile().SetNetInfo( m_packages.m_strInfo, m_packages.m_strInfourl );
	AfxGetProfile().SetLatestver( m_packages.m_strLatestver );

	return TRUE;
}

BOOL CNetDatabase::NetloadServerInfo( PROGRESS_CALLBACK fnCallback, void *cookie )
{
	CString	strServerInfo;
	if( !m_httpClient.LoadServerInfo( strServerInfo, fnCallback, cookie ) )
	{
		SetLastError( ERR_NETDB_HTTPCLIENT );
		return FALSE;
	}

	if( ! m_servers.SetRawXMLDocument( strServerInfo ) )
	{
		SetLastError( ERR_NETDB_SERVERS );
		return FALSE;
	}

	return TRUE;
}

int CNetDatabase::NetloadCodetbl( PROGRESS_CALLBACK fnCallback, void *cookie )
{
	if( ! m_packages.IsOK() )
		return FALSE;
	
	CSPTime	tmFrom(NULL);
	CSPTime	tmTo	=	CSPTime::GetCurrentTime();
	return NetloadPackage( CPackage::packageCode, tmFrom, tmTo, fnCallback, cookie );
}


int CNetDatabase::NetloadDayKLine( LPCTSTR lpszStockCode, CSPTime *ptmFrom, CSPTime *ptmTo,
								   PROGRESS_CALLBACK fnCallback, void *cookie )
{
	if( ! m_packages.IsOK() )
		return FALSE;
	
	CSPTime	tmFrom(NULL), tmTo(NULL);
	if( NULL != ptmFrom )
		tmFrom	=	*ptmFrom;

	if( NULL != ptmTo )
		tmTo	=	*ptmTo;
	else
		GetLatestTimeNet( tmTo, CPackage::packageDay );

	return NetloadPackage( CPackage::packageDay, tmFrom, tmTo, fnCallback, cookie );
}

int CNetDatabase::Netload5MinKLine( LPCTSTR lpszStockCode, CSPTime *ptmFrom, CSPTime *ptmTo,
									PROGRESS_CALLBACK fnCallback, void *cookie )
{
	if( ! m_packages.IsOK() )
		return FALSE;

	CSPTime	tmFrom(NULL), tmTo(NULL);
	if( NULL != ptmFrom )
		tmFrom	=	*ptmFrom;

	if( NULL != ptmTo )
		tmTo	=	*ptmTo;
	else
		GetLatestTimeNet( tmTo, CPackage::packageMin5 );

	return NetloadPackage( CPackage::packageMin5, tmFrom, tmTo, fnCallback, cookie );
}

int CNetDatabase::NetloadDRData( LPCTSTR lpszStockCode, CSPTime *ptmFrom, CSPTime *ptmTo,
								 PROGRESS_CALLBACK fnCallback, void *cookie )
{
	if( ! m_packages.IsOK() )
		return FALSE;

	CSPTime	tmFrom(NULL), tmTo(NULL);
	if( NULL != ptmFrom )
		tmFrom	=	*ptmFrom;
	
	if( NULL != ptmTo )
		tmTo	=	*ptmTo;
	else
		GetLatestTimeNet( tmTo, CPackage::packageDR );

	return NetloadPackage( CPackage::packageDR, tmFrom, tmTo, fnCallback, cookie );
}

int CNetDatabase::NetloadBasetext( LPCTSTR lpszStockCode, CSPTime *ptmFrom, CSPTime *ptmTo,
								   PROGRESS_CALLBACK fnCallback, void *cookie )
{
	if( ! m_packages.IsOK() )
		return FALSE;

	CSPTime	tmFrom(NULL), tmTo(NULL);
	if( NULL != ptmFrom )
		tmFrom	=	*ptmFrom;
	else
		tmFrom	=	CSPTime(NULL);
	if( NULL != ptmTo )
		tmTo	=	*ptmTo;
	else
		GetLatestTimeNet( tmTo, CPackage::packageBasetext );

	return NetloadPackage( CPackage::packageBasetext, tmFrom, tmTo, fnCallback, cookie );
}

int CNetDatabase::NetloadBasetable( LPCTSTR lpszStockCode, CSPTime *ptmFrom, CSPTime *ptmTo,
								   PROGRESS_CALLBACK fnCallback, void *cookie )
{
	if( ! m_packages.IsOK() )
		return FALSE;

	CSPTime	tmFrom(NULL), tmTo(NULL);
	if( NULL != ptmFrom )
		tmFrom	=	*ptmFrom;
	else
		tmFrom	=	CSPTime(NULL);
	if( NULL != ptmTo )
		tmTo	=	*ptmTo;
	else
		GetLatestTimeNet( tmTo, CPackage::packageBasetable );

	return NetloadPackage( CPackage::packageBasetable, tmFrom, tmTo, fnCallback, cookie );
}

struct progress_cookie_t {
	DWORD nLenFinished;
	DWORD nLenCur;
	DWORD	nLenTotal;
	PROGRESS_CALLBACK	fnCallbackSuper;
	void	* cookieSuper;
};

BOOL CALLBACK LocalProgressCallback(DWORD dwCode, DWORD dwProgress,
							LPCTSTR lpszMsg, void *cookie)
{
	struct progress_cookie_t *p = (struct progress_cookie_t *)cookie;

	if( PROG_PROGRESS == dwCode )
	{
		DWORD dwLenCurFinished = DWORD( (double(dwProgress))*p->nLenCur / STKLIB_MAXF_PROGRESS );
		if( dwLenCurFinished > p->nLenCur )
			dwLenCurFinished	=	p->nLenCur;
		DWORD dwTotalProgress = dwProgress;
		if( p->nLenTotal > 0 )
			dwTotalProgress	= DWORD( STKLIB_MAXF_PROGRESS*(p->nLenFinished+dwLenCurFinished)/(p->nLenTotal) );
		if( p->fnCallbackSuper )
			return p->fnCallbackSuper( dwCode, dwTotalProgress, lpszMsg, p->cookieSuper );
		return TRUE;
	}
	else
	{
		if( p->fnCallbackSuper )
			return p->fnCallbackSuper( dwCode, dwProgress, lpszMsg, p->cookieSuper );
		return TRUE;
	}
}

int CNetDatabase::NetloadPackage( int nPackageType, CSPTime &tmFrom, CSPTime &tmTo,
								  PROGRESS_CALLBACK fnCallback, void *cookie )
{
	if( ! m_packages.IsOK() )
		return FALSE;

	m_packages.Sort( );
	
	int nTotalLen = 0;
	for( int i=0; i<m_packages.GetSize(); i++ )
	{
		CPackage	& pac = m_packages.ElementAt(i);
		if( pac.m_nType != nPackageType
			|| pac.m_tmTo < tmFrom
			|| pac.m_tmFrom > tmTo )
			continue;

		nTotalLen	+=	pac.m_nLength;
	}

	struct	progress_cookie_t	cookieLocal;
	cookieLocal.fnCallbackSuper	=	fnCallback;
	cookieLocal.cookieSuper		=	cookie;
	cookieLocal.nLenTotal		=	nTotalLen<<1;
	cookieLocal.nLenFinished	=	0;

	int i;
	for( i=0; i<m_packages.GetSize(); i++ )
	{
		CPackage	pac = m_packages.ElementAt(i);
		if( pac.m_nType != nPackageType
			|| pac.m_tmTo < tmFrom
			|| pac.m_tmFrom > tmTo )
			continue;

		cookieLocal.nLenCur	=	pac.m_nLength;

		CFile	file;
		BOOL	bOK		=	FALSE;
		BOOL	bExpand	=	FALSE;
		CString	sFileName;
		if( GetTempFile( sFileName ) 
			&& file.Open( sFileName, CFile::modeCreate | CFile::modeReadWrite ) )
		{
			bOK	=	m_httpClient.LoadPackage( pac.GetURL( m_packages.m_strBasePath ), &file, LocalProgressCallback, &cookieLocal );
			if( !bOK && pac.HasAlternateURL() )
			{
				file.Close();
				if( file.Open( sFileName, CFile::modeCreate | CFile::modeReadWrite ) )
					bOK	=	m_httpClient.LoadPackage( pac.GetAlternateURL(m_packages.m_strBasePath), &file, LocalProgressCallback, &cookieLocal );
			}

			if( !bOK && pac.HasExpandPackage( ) )
			{
				m_packages.AppendPackages( pac.GetExpandPackages() );
				m_packages.ElementAt(i).DeleteExpandPackages();
				bExpand	=	TRUE;
			}

			if( CFile::hFileNull != file.m_hFile )
				file.Close();

			m_packages.ElementAt(i).m_bDownloadOK =	bOK;
			m_packages.ElementAt(i).m_strLocalFilename =	sFileName;
		}
		
		if( !bExpand )
			cookieLocal.nLenFinished	+=	pac.m_nLength;
	}

	int	nCount	=	0;
	for( i=0; i<m_packages.GetSize(); i++ )
	{
		CPackage	& pac = m_packages.ElementAt(i);

		BOOL	bOK	=	pac.m_bDownloadOK;
		pac.m_bDownloadOK	=	FALSE;
		if( bOK )
		{
			cookieLocal.nLenCur	=	pac.m_nLength;
			bOK	=	InstallPackageEx( pac, pac.m_strLocalFilename, LocalProgressCallback, &cookieLocal );
			if( !bOK && fnCallback )
			{
				CString	str	=	pac.m_strDescript;
				fnCallback( PROG_ERRORPAC, 0, str.GetBuffer(str.GetLength()+1), cookie );
				str.ReleaseBuffer();
			}
			
			RemoveTempFile( pac.m_strLocalFilename );
			LocalProgressCallback( PROG_PROGRESS, STKLIB_MAX_PROGRESS, NULL, &cookieLocal );
			cookieLocal.nLenFinished	+=	pac.m_nLength;
			if( bOK )
				nCount	++;
		}
	}

	return nCount;
}

BOOL CNetDatabase::GetLatestTimeNet( CSPTime &tm, int packagetype )
{
	return m_packages.GetLatestTime( tm, packagetype );
}

CServers & CNetDatabase::GetServers( )
{
	return m_servers;
}

CPackages & CNetDatabase::GetPackages( )
{
	return m_packages;
}

int	CNetDatabase::GetMaxStockNumber( )
{
	return CStDatabase::GetMaxStockNumber();
}

int	CNetDatabase::LoadCodetable( CStockContainer & container )
{
	return CStDatabase::LoadCodetable( container );
}

int	CNetDatabase::LoadBaseText( CStock *pstock )
{
	return CStDatabase::LoadBaseText( pstock );
}

int	CNetDatabase::LoadKData( CStock *pstock, int period )
{
	return CStDatabase::LoadKData( pstock, period );
}

int	CNetDatabase::LoadDRData( CStock *pstock )
{
	return CStDatabase::LoadDRData( pstock );
}

BOOL CNetDatabase::EmptyTempDirectory( LPCTSTR lpszPath )
{
	CString	strFinder;
	if( NULL == lpszPath )
	{
		char	szTempPath[MAX_PATH+1];
		memset( szTempPath, 0, sizeof(szTempPath) );
		if( GetSelfTempPath( szTempPath, MAX_PATH ) <= 0 )
			return FALSE;
		strFinder	=	szTempPath;
	}
	else
		strFinder	=	lpszPath;
	
	strFinder	+=	"*";
	
	CFileFind	finder;
	BOOL	bWorking	=	finder.FindFile( strFinder );
	while( bWorking )
	{
		bWorking	=	finder.FindNextFile();
		DWORD	attr = GetFileAttributes( finder.GetFilePath() );
		if( 0xFFFFFFFF != attr && (attr & FILE_ATTRIBUTE_DIRECTORY) )
		{
			CString	strName	=	finder.GetFileName();
			if( 0 != strName.CompareNoCase(".") && 0 != strName.CompareNoCase("..") )
			{
				EmptyTempDirectory( finder.GetFilePath() + STRING_DIRSEP );
				RemoveDirectory( finder.GetFilePath() );
			}
		}
		else
		{
			DeleteFile( finder.GetFilePath() );
		}
	}
	finder.Close();
	return TRUE;
}


// protected method
BOOL CNetDatabase::GetTempFile( CString &rString )
{
	char	szTempPath[MAX_PATH+1];
	memset( szTempPath, 0, sizeof(szTempPath) );
	GetSelfTempPath( szTempPath, MAX_PATH );

	if( 0 == strlen(szTempPath) )
		GetTempPath( MAX_PATH, szTempPath );

	if( 0 == strlen(szTempPath) )
		return FALSE;

	char szFile[MAX_PATH+1];
	memset( szFile, 0, sizeof(szFile) );
	if( 0!=GetTempFileName( szTempPath, "net", 0, szFile ) )
	{
		rString	=	szFile;
		return TRUE;
	}
	return FALSE;
}

BOOL CNetDatabase::RemoveTempFile( CString sFileName )
{
	return DeleteFile( sFileName );
}

BOOL CNetDatabase::OpenTempFile( CFile &file )
{
	CString	strFileName;
	GetTempFile( strFileName );

	BOOL bOpen = file.Open( strFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive );
	if( !bOpen )
		DeleteFile(strFileName);
	return bOpen;
}

BOOL CNetDatabase::CloseAndRemoveTempFile( CFile &file )
{
	if( CFile::hFileNull != file.m_hFile )
	{
		CString	strFileName	=	file.GetFilePath( );
		try{
			file.Close();
		}catch( CException * e )	{	e->Delete();	}
		return DeleteFile( strFileName );
	}
	return FALSE;
}

BOOL CNetDatabase::GetTempNewDirectory( CString &sTempNewDir )
{
	char	szTempPath[MAX_PATH+1];
	memset( szTempPath, 0, sizeof(szTempPath) );

	GetSelfTempPath( szTempPath, MAX_PATH );

	if( 0 == strlen(szTempPath) )
		GetTempPath( MAX_PATH, szTempPath );

	if( 0 == strlen(szTempPath) )
		return FALSE;

	CString	sDir	=	szTempPath;
	sDir			+=	"NFO";

	srand( time(NULL) );
	DWORD	dw		=	rand() % 10000;
	sTempNewDir.Format( "%s%.4d\\", sDir, dw );

	int	nCount	=	0;
	while( 0 == access(sDir,0) )
	{
		nCount	++;
		if( nCount > 1000 )
			return FALSE;
		dw	++;
		sTempNewDir.Format( "%s%.4d\\", sDir, dw );
	}

	return CreateDirectory( sTempNewDir, NULL );
}

BOOL CNetDatabase::RemoveTempNewDirectory( CString sTempNewDir )
{
	CFileFind	finder;
	BOOL	bWorking	=	finder.FindFile( sTempNewDir + "*.*" );
	while( bWorking )
	{
		bWorking	=	finder.FindNextFile();
		DeleteFile( finder.GetFilePath() );
	}
	finder.Close();

	return RemoveDirectory(sTempNewDir);
}

int CNetDatabase::GetFileCount( const char * path, BOOL bRecurse )
{
	if( NULL == path || strlen(path) == 0 )
		return FALSE;

	int	nCount	=	0;

	CString	strFinder	=	path;
	if( strFinder.GetLength() > 0 &&
		strFinder[strFinder.GetLength()-1] != '\\' && strFinder[strFinder.GetLength()-1] != '/' )
		strFinder	+=	STRING_DIRSEP;
	strFinder	+=	"*";
	CFileFind	finder;
	BOOL	bWorking	=	finder.FindFile( strFinder );
	while( bWorking )
	{
		bWorking	=	finder.FindNextFile();
		DWORD	attr = GetFileAttributes( finder.GetFilePath() );
		if( 0xFFFFFFFF != attr && (attr & FILE_ATTRIBUTE_DIRECTORY) && bRecurse )
		{
			CString	strName	=	finder.GetFileName();
			if( 0 != strName.CompareNoCase(".") && 0 != strName.CompareNoCase("..") )
				nCount	+=	GetFileCount( finder.GetFilePath() + STRING_DIRSEP, bRecurse );
		}
		if( 0xFFFFFFFF != attr && !(attr & FILE_ATTRIBUTE_DIRECTORY) )
			nCount	++;
	}
	finder.Close();
	return nCount;
}

BOOL CNetDatabase::InstallPackageEx( CPackage &pac, const char *zipfilename,
									PROGRESS_CALLBACK fnCallback, void *cookie )
{
	if( !pac.m_bIsZipped )
	{
		if( fnCallback )
			fnCallback( PROG_INSTALLPACKAGE, 0, NULL, cookie );
		return InstallPackage( pac, zipfilename, fnCallback, cookie );
	}

	if( NULL == zipfilename || strlen(zipfilename) == 0 )
		return FALSE;

	CInfoZip InfoZip;
	if (!InfoZip.InitializeUnzip())
	{
		SetLastError( ERR_NETDB_ZIPDLL );
		return FALSE;
	}

	CString	sTempNewDir;
	if( !GetTempNewDirectory( sTempNewDir ) )
	{
		InfoZip.FinalizeUnzip();
		return FALSE;
	}

	if( fnCallback )
		fnCallback( PROG_EXTRACTZIPFILES, 0, NULL, cookie );

	if (!InfoZip.ExtractFiles(zipfilename, sTempNewDir))
	{
		SetLastError( ERR_NETDB_ZIP );
		InfoZip.FinalizeUnzip();
		return FALSE;
	}

	if (!InfoZip.FinalizeUnzip())
	{
		SetLastError( ERR_NETDB_ZIPDLL );
	}

	if( fnCallback )
		fnCallback( PROG_INSTALLPACKAGE, 0, NULL, cookie );

	int	nTotalCount	=	GetFileCount( sTempNewDir );
	BOOL	bRet	=	InstallPackagePath( pac, sTempNewDir, fnCallback, cookie, nTotalCount, 0 );

	RemoveTempNewDirectory( sTempNewDir );

	return bRet;
}

BOOL CNetDatabase::InstallPackagePath( CPackage &pac, const char *path,
									PROGRESS_CALLBACK fnCallback, void *cookie,
									int nTotalCount, int nFinishCount, BOOL bDeleteAfterInstall )
{
	if( NULL == path || strlen(path) == 0 )
		return FALSE;

	CString	strFinder	=	path;
	if( strFinder[strFinder.GetLength()-1] != '\\' && strFinder[strFinder.GetLength()-1] != '/' )
		strFinder += STRING_DIRSEP;
	strFinder	+=	"*";

	BOOL	bRet	=	TRUE;
	CFileFind	finder;
	BOOL	bWorking	=	finder.FindFile( strFinder );
	while( bWorking )
	{
		bWorking	=	finder.FindNextFile();
		DWORD	attr = GetFileAttributes( finder.GetFilePath() );
		if( 0xFFFFFFFF != attr && (attr & FILE_ATTRIBUTE_DIRECTORY) )
		{
			CString	strName	=	finder.GetFileName();
			if( 0 != strName.CompareNoCase(".") && 0 != strName.CompareNoCase("..") )
			{
				bRet	&=	InstallPackagePath( pac, finder.GetFilePath() + STRING_DIRSEP,
												fnCallback, cookie, nTotalCount, nFinishCount );
				if( bDeleteAfterInstall )
					RemoveDirectory( finder.GetFilePath() );
			}
		}
		if( 0xFFFFFFFF != attr && !(attr & FILE_ATTRIBUTE_DIRECTORY) )
		{
			if( 1 == nTotalCount )
				bRet	&=	InstallPackage( pac, finder.GetFilePath(), fnCallback, cookie );
			else
				bRet	&=	InstallPackage( pac, finder.GetFilePath(), fnCallback, cookie );
			if( bDeleteAfterInstall )
				DeleteFile( finder.GetFilePath() );
			nFinishCount	++;
			if( fnCallback && nTotalCount > 1 )
				fnCallback( PROG_PROGRESS, DWORD(STKLIB_MAXF_PROGRESS*nFinishCount/nTotalCount), NULL, cookie );
		}
	}
	finder.Close();
	if( fnCallback )
		fnCallback( PROG_PROGRESS, STKLIB_MAX_PROGRESS, NULL, cookie );
	return bRet;
}

BOOL CNetDatabase::InstallPackage( CPackage &pac, const char *pacfile,
  									PROGRESS_CALLBACK fnCallback, void *cookie )
{
	CString	strFilenameOrg;
	if( pac.m_bIsZipped )
		strFilenameOrg	=	pacfile;
	else if( !pac.m_strURL.IsEmpty() )
		strFilenameOrg	=	pac.m_strURL;
	else
		strFilenameOrg	=	pacfile;
	int nIndex	=	strFilenameOrg.ReverseFind( '/' );
	if( -1 != nIndex )
		strFilenameOrg	=	strFilenameOrg.Mid( nIndex + 1 );
	nIndex	=	strFilenameOrg.ReverseFind( '\\' );
	if( -1 != nIndex )
		strFilenameOrg	=	strFilenameOrg.Mid( nIndex + 1 );

	BOOL	bOK	=	strFilenameOrg.GetLength() > 4;
	if( bOK && CPackage::packageCode == pac.m_nType )
	{
		CString strDomainFile = AfxGetProfile().GetDomainFile();
		int nIndex	=	strDomainFile.ReverseFind( '/' );
		if( -1 != nIndex )
			strDomainFile	=	strDomainFile.Mid( nIndex + 1 );
		nIndex	=	strDomainFile.ReverseFind( '\\' );
		if( -1 != nIndex )
			strDomainFile	=	strDomainFile.Mid( nIndex + 1 );

		if( 0 == strFilenameOrg.CompareNoCase( strDomainFile ) )	// 板块数据
			bOK	=	(InstallCodetblBlock( pacfile, strFilenameOrg ) >= 0 );
		else if( 0 == strFilenameOrg.Right(4).CompareNoCase( ".blk" ) )	// 分析家板块
			bOK	=	(InstallCodetblFxjBlock( pacfile, strFilenameOrg ) >= 0 );
		else
			bOK	=	(InstallCodetbl( pacfile, strFilenameOrg ) > 0 );
	}
	else if( bOK && CPackage::packageDay == pac.m_nType )
	{
		if( 0 == strFilenameOrg.Right(4).CompareNoCase( ".stk" ) )	// 通用格式
			bOK	=	( InstallKDataTy( pacfile, CKData::ktypeDay, fnCallback, cookie ) > 0 );
		else if( 0 == strFilenameOrg.Right(4).CompareNoCase( ".dad" ) )	// 分析家格式
			bOK	=	( InstallKDataFxj( pacfile, CKData::ktypeDay, fnCallback, cookie ) > 0 );
		else if( 0 == strFilenameOrg.Right(4).CompareNoCase( ".txt" ) )	// 说明文件
			bOK	=	TRUE;
		else
			bOK	=	FALSE;
	}
	else if( bOK && CPackage::packageMin5 == pac.m_nType )
	{
		if( 0 == strFilenameOrg.Right(4).CompareNoCase( ".stk" ) )	// 通用格式
			bOK	=	( InstallKDataTy( pacfile, CKData::ktypeMin5, fnCallback, cookie ) > 0 );
		else if( 0 == strFilenameOrg.Right(4).CompareNoCase( ".dad" ) )	// 分析家格式
			bOK	=	( InstallKDataFxj( pacfile, CKData::ktypeMin5, fnCallback, cookie ) > 0 );
		else if( 0 == strFilenameOrg.Right(4).CompareNoCase( ".txt" ) )	// 说明文件
			bOK	=	TRUE;
		else
			bOK	=	FALSE;
	}
	else if( bOK && CPackage::packageDR == pac.m_nType )
	{
		if( 0 == strFilenameOrg.Right(4).CompareNoCase( ".dat" ) )		// 普通格式
			bOK	=	( InstallDRDataClk( pacfile, strFilenameOrg ) > 0 );
		else if( 0 == strFilenameOrg.Right(4).CompareNoCase( ".pwr" ) )	// 分析家除权格式
			bOK	=	( InstallDRDataFxj( pacfile ) > 0 );
		else
			bOK	=	FALSE;
	}
	else if( bOK && CPackage::packageBasetext == pac.m_nType )
	{
		if( 0 == strFilenameOrg.Right(4).CompareNoCase( ".txt" ) )	// 文本资料
			bOK	=	( InstallBaseText( pacfile, strFilenameOrg ) > 0 );
		else
			bOK	=	FALSE;
	}
	else if( bOK && CPackage::packageBasetable == pac.m_nType )
	{
		if( 0 == strFilenameOrg.Right(4).CompareNoCase( ".bst" ) )	// 财务格式
			bOK	=	( InstallBasetable( pacfile, strFilenameOrg ) > 0 );
		else if( 0 == strFilenameOrg.Right(4).CompareNoCase( ".dbf" ) )	// 通达信财务格式
			bOK	=	( InstallBasetableTdx( pacfile ) > 0 );
		else if( 0 == strFilenameOrg.Right(4).CompareNoCase( ".fin" ) )	// 分析家财务格式
			bOK	=	( InstallBasetableFxj( pacfile ) > 0 );
		else
			bOK	=	FALSE;
	}
	else
		bOK	=	FALSE;

	if( !bOK )
	{
		CFileStatus	rStatus;
		if( ! (CFile::GetStatus( pacfile, rStatus ) && rStatus.m_size > 0) )
			bOK	=	TRUE;
	}
	if( !bOK && fnCallback )
	{
		CString	string	=	pac.m_strDescript;
		fnCallback( PROG_ERRORPAC, 0, string.GetBuffer(string.GetLength()+1), cookie );
		string.ReleaseBuffer();
	}
	return bOK;
}

