/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CXMLNode;
		class	CXMLDocument;
	Warning:
		To use these classes, must call AfxOleInit() first.
*/

#include "stdafx.h"
#include <afxconv.h>
#include <errno.h>
#include "../Include/Markup.h"
#include "../Include/XMLDoc.h"
#include "strings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static CString FormatTime( CSPTime tm )
{
	return (LPCTSTR)tm.Format( "%Y/%m/%d %H:%M:%S" );
}

static BOOL ParseTime( CString strTime, CSPTime &tm )
{
	char	sepDate	=	'/';
	char	sepTime	=	':';
	char	sepDateTime	=	' ';

	strTime.TrimLeft();
	strTime.TrimRight();
	if( strTime.IsEmpty() )
		return FALSE;

	int	nYear = 1971, nMonth = 1, nDay = 1;
	int nHour = 0, nMinute= 0, nSecond = 0;
	CString	strTemp;
	int nIndex;
	
	// year
	nIndex = strTime.Find( sepDate );
	if( -1 != nIndex )
	{
		strTemp	=	strTime.Left(nIndex);
		strTime	=	strTime.Mid(nIndex+1);
		nYear	=	atol(strTemp);
	}
	else
	{
		nYear	=	atol(strTime);
		tm		=	CSPTime( nYear, nMonth, nDay, nHour, nMinute, nSecond );
		return (-1 != tm.GetTime());
	}

	// month
	nIndex = strTime.Find( sepDate );
	if( -1 != nIndex )
	{
		strTemp	=	strTime.Left(nIndex);
		strTime	=	strTime.Mid(nIndex+1);
		nMonth	=	atol(strTemp);
	}
	else
	{
		nMonth	=	atol(strTime);
		tm		=	CSPTime( nYear, nMonth, nDay, nHour, nMinute, nSecond );
		return (-1 != tm.GetTime());
	}

	// day
	nIndex = strTime.Find( sepDateTime );
	if( -1 != nIndex )
	{
		strTemp	=	strTime.Left(nIndex);
		strTime	=	strTime.Mid(nIndex+1);
		nDay	=	atol(strTemp);
	}
	else
	{
		nDay	=	atol(strTime);
		tm		=	CSPTime( nYear, nMonth, nDay, nHour, nMinute, nSecond );
		return (-1 != tm.GetTime());
	}

	// hour
	nIndex = strTime.Find( sepTime );
	if( -1 != nIndex )
	{
		strTemp	=	strTime.Left(nIndex);
		strTime	=	strTime.Mid(nIndex+1);
		nHour	=	atol(strTemp);
	}
	else
	{
		nHour	=	atol(strTime);
		tm		=	CSPTime( nYear, nMonth, nDay, nHour, nMinute, nSecond );
		return (-1 != tm.GetTime());
	}

	// minute
	nIndex = strTime.Find( sepTime );
	if( -1 != nIndex )
	{
		strTemp	=	strTime.Left(nIndex);
		strTime	=	strTime.Mid(nIndex+1);
		nMinute	=	atol(strTemp);
	}
	else
	{
		nMinute	=	atol(strTime);
		tm		=	CSPTime( nYear, nMonth, nDay, nHour, nMinute, nSecond );
		return (-1 != tm.GetTime());
	}

	// second
	nSecond	=	atol(strTime);
	tm		=	CSPTime( nYear, nMonth, nDay, nHour, nMinute, nSecond );
	return (-1 != tm.GetTime());
}

//////////////////////////////////////////////////////////////////////////////////
//	class CXMLNode
CXMLNode::CXMLNode( )
{
}

BOOL CXMLNode::SetAttribute( CMarkup & markup )
{
	return TRUE;
}

CString CXMLNode::getAttrValue( CMarkup & markup, LPCTSTR lpszName )
{
	ASSERT( NULL != lpszName );
	return markup.GetAttrib( lpszName );
}

long CXMLNode::getAttrValue_I( CMarkup & markup, LPCTSTR lpszName )
{
	ASSERT( NULL != lpszName );
	CString strValue	=	markup.GetAttrib( lpszName );

	if( strValue.GetLength() <= 0 )
		return 0;
	LPTSTR	lpstrRet = NULL;
	long	nRet;
	nRet	=	_tcstoul( (LPCTSTR)strValue, &lpstrRet, 10 );
	if( ERANGE == errno || ( lpstrRet && '\0' != *lpstrRet ) )
		return 0;
	return	nRet;
}

CSPTime CXMLNode::getAttrValue_T( CMarkup & markup, LPCTSTR lpszName )
{
	ASSERT( NULL != lpszName );
	CString strValue	=	markup.GetAttrib( lpszName );

	CSPTime	tmRet;
	if( ParseTime( strValue, tmRet ) )
		return tmRet;
	else
		return CSPTime::GetCurrentTime();
}

//////////////////////////////////////////////////////////////////////////////////
//	class CXMLDocument

CXMLDocument::CXMLDocument()
{
}

CXMLDocument::~CXMLDocument()
{
}

BOOL CXMLDocument::Initialize()
{
	return TRUE;
}

void CXMLDocument::Release()
{
	m_markup.SetDoc( _T("") );
}

BOOL CXMLDocument::SetRawDocument( CString &strDoc )
{
	return m_markup.SetDoc( strDoc );
}

void CXMLDocument::SetLastErrorMessage(LPCTSTR lpszError)
{
	if( NULL != lpszError )
		m_strLastErrorMessage	=	lpszError;
}

BOOL CXMLDocument::GetLastErrorMessage(LPTSTR lpszError, UINT nMaxError )
{
	if( m_strLastErrorMessage.IsEmpty() )
		m_strLastErrorMessage	=	szErrXMLDefault;

	strncpy( lpszError, m_strLastErrorMessage.GetBuffer(m_strLastErrorMessage.GetLength()+1), nMaxError );
	m_strLastErrorMessage.ReleaseBuffer();
	return strlen(lpszError)>0;
}

/////////////////////////////////////////////////////////////////////////////////////
// The Old XMLDoc using msxml.dll 2.5(gb2312) or above
/* 
//////////////////////////////////////////////////////////////////////////////////
//	class CXMLNode
CXMLNode::CXMLNode( )
{
}

BOOL CXMLNode::SetAttribute( MSXML::IXMLDOMNodePtr pNode )
{
	MSXML::IXMLDOMNamedNodeMapPtr pMap = pNode->Getattributes( );
	if( NULL == pMap )
		return FALSE;
	return TRUE;
}

BOOL CXMLNode::getAttrValue( MSXML::IXMLDOMNamedNodeMapPtr pMap, 
							LPCTSTR lpcszName, _bstr_t& bstrValue )
{
	ASSERT( NULL != pMap );
	ASSERT( NULL != lpcszName );

	_bstr_t	bstrName	=	lpcszName;
	MSXML::IXMLDOMNodePtr pNode = pMap->getNamedItem( bstrName );
	if( NULL == pNode )
	{
		return	FALSE;
	}
	bstrValue	=	pNode->Gettext( );
	return TRUE;
}

CString CXMLNode::getAttrValue( MSXML::IXMLDOMNamedNodeMapPtr pMap, 
							LPCTSTR lpcszName )
{
	ASSERT( NULL != pMap );
	ASSERT( NULL != lpcszName );

	_bstr_t	bstrName	=	lpcszName;
	MSXML::IXMLDOMNodePtr pNode = pMap->getNamedItem( bstrName );
	if( NULL == pNode )
	{
		return "";
	}
	return	(LPCTSTR)pNode->Gettext( );
}

long CXMLNode::getAttrValue_I( MSXML::IXMLDOMNamedNodeMapPtr pMap, LPCTSTR lpcszName )
{
	LPTSTR	lpstrRet = NULL;
	long	nRet;

	CString	strValue	=	getAttrValue( pMap, lpcszName );
	nRet	=	_tcstoul( (LPCTSTR)strValue, &lpstrRet, 10 );
	if( ERANGE == errno || ( lpstrRet && '\0' != *lpstrRet ) )
		return 0;
	return	nRet;
}

CSPTime CXMLNode::getAttrValue_T( MSXML::IXMLDOMNamedNodeMapPtr pMap, LPCTSTR lpcszName )
{
	CSPTime	tmRet;

	CString	strValue	=	getAttrValue( pMap, lpcszName );
	if( ParseTime( strValue, tmRet ) )
		return tmRet;
	else
		return CSPTime::GetCurrentTime();
}

//////////////////////////////////////////////////////////////////////////////////
//	class CXMLDocument

CXMLDocument::CXMLDocument()
{
	m_pDOMDoc	=	NULL;
}

CXMLDocument::~CXMLDocument()
{

}

BOOL CXMLDocument::Initialize()
{
	if( m_pDOMDoc )
		return TRUE;

	try
	{
		MSXML::IXMLDOMDocumentPtr pDOMDoc(__uuidof(MSXML::DOMDocument));
		m_pDOMDoc = pDOMDoc;
	}
	catch(_com_error e)
	{
		TRACE(_T("Caught Exception: OnOpenURL"));
	}
	catch(...)
	{
		TRACE(_T("Caught Exception: OnOpenURL"));
	}

	return ( NULL != m_pDOMDoc );
}

void CXMLDocument::Release()
{
}

//Synchronously create a stream on a URL.
//
//	hr = URLOpenBlockingStream(0, pszURL, &pStm, 0,0);    
//	CHECK_ERROR(SUCCEEDED(hr) && pStm, "Couldn't open stream on URL")
//
//Get the IPersistStreamInit interface to the XML doc.
//
//	hr = pDoc->QueryInterface(IID_IPersistStreamInit, (void **)&pPSI);
//	CHECK_ERROR(SUCCEEDED(hr), "QI for IPersistStreamInit failed");
//
//Init the XML doc from the stream.
//	hr = pPSI->Load(pStm);

BOOL CXMLDocument::Load(LPCTSTR lpcszURL)
{
USES_CONVERSION;

	if( NULL == m_pDOMDoc || NULL == lpcszURL )
		return FALSE;

	try
	{
		m_pDOMDoc->put_async(VARIANT_FALSE);
		_bstr_t bstrURL = lpcszURL;
		VARIANT_BOOL varResult = m_pDOMDoc->load(_variant_t(bstrURL));
		if (VARIANT_FALSE == varResult)
		{
			// Load failed
			MSXML::IXMLDOMParseErrorPtr pParseError = m_pDOMDoc->GetparseError();
			long dwError = pParseError->GeterrorCode();
			_bstr_t bstrReason = pParseError->Getreason();
			CString strError;
			strError.Format(_T("XML Parse Error 0x%x : %s"), dwError, W2T(bstrReason));
			m_strLastErrorMessage	=	strError;
			return FALSE;
		}
	}
	catch( ... )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CXMLDocument::SetRawDocument( CString &strDoc )
{
USES_CONVERSION;
	if( NULL == m_pDOMDoc )
		return FALSE;

	try
	{
		m_pDOMDoc->put_async(VARIANT_FALSE);
		VARIANT_BOOL varResult = m_pDOMDoc->loadXML( _bstr_t(strDoc) );
		if (VARIANT_FALSE == varResult)
		{
			// Load failed
			MSXML::IXMLDOMParseErrorPtr pParseError = m_pDOMDoc->GetparseError();
			long dwError = pParseError->GeterrorCode();
			_bstr_t bstrReason = pParseError->Getreason();
			CString strError;
			strError.Format(_T("XML Parse Error 0x%x : %s"), dwError, W2T(bstrReason));
			m_strLastErrorMessage	=	strError;
			return FALSE;
		}
	}
	catch( ... )
	{
		return FALSE;
	}
	
	return TRUE;
}

void CXMLDocument::SetLastErrorMessage(LPCTSTR lpszError)
{
	if( NULL != lpszError )
		m_strLastErrorMessage	=	lpszError;
}

BOOL CXMLDocument::GetLastErrorMessage(LPTSTR lpszError, UINT nMaxError )
{
	if( m_strLastErrorMessage.IsEmpty() )
		m_strLastErrorMessage	=	szErrXMLDefault;

	strncpy( lpszError, m_strLastErrorMessage.GetBuffer(m_strLastErrorMessage.GetLength()+1), nMaxError );
	m_strLastErrorMessage.ReleaseBuffer();
	return strlen(lpszError)>0;
}
*/
// The Old XMLDoc using msxml.dll 2.5(gb2312) or above
/////////////////////////////////////////////////////////////////////////////////////
