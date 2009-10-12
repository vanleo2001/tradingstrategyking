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

#if !defined(AFX_XMLDOC_H__3533AF4F_42FA_4117_86FB_57AD0B2283A3__INCLUDED_)
#define AFX_XMLDOC_H__3533AF4F_42FA_4117_86FB_57AD0B2283A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Markup.h"

/***
	XML节点属性获取
*/
class STKNET_API CXMLNode
{
public:
	CXMLNode( );

	virtual	BOOL	SetAttribute( CMarkup & markup );

	static	CString	getAttrValue( CMarkup & markup, LPCTSTR lpcszName );
	static	long	getAttrValue_I( CMarkup & markup, LPCTSTR lpcszName );
	static	CSPTime	getAttrValue_T( CMarkup & markup, LPCTSTR lpcszName );
};

/***
	XML文档
	使用方法
	CXMLDocument	xmlDoc;
	xmlDoc.Initialize( );
	xmlDoc.SetRawDocument( "<xml> ... </xml>" );
	...
*/
class STKNET_API CXMLDocument : public CObject
{
public:
	CXMLDocument();
	virtual ~CXMLDocument();

	// Attributes
	CMarkup & Markup() { return m_markup; }

	// Operations
	BOOL Initialize();
	void Release();
	
	BOOL			SetRawDocument( CString &strDoc );
	void			SetLastErrorMessage(LPCTSTR lpszError);
	virtual BOOL	GetLastErrorMessage(LPTSTR lpszError, UINT nMaxError );

protected:
	CMarkup			m_markup;
	CString	m_strLastErrorMessage;
};


/////////////////////////////////////////////////////////////////////////////////////
// The Old XMLDoc using msxml.dll 2.5(gb2312) or above
/*
#import	<msxml.dll>

class CXMLNode
{
public:
	CXMLNode( );

	virtual	BOOL	SetAttribute( MSXML::IXMLDOMNodePtr pNode );

	static	BOOL	getAttrValue( MSXML::IXMLDOMNamedNodeMapPtr pMap, LPCTSTR lpcszName, _bstr_t& bstrValue );
	static	CString	getAttrValue( MSXML::IXMLDOMNamedNodeMapPtr pMap, LPCTSTR lpcszName );
	static	long	getAttrValue_I( MSXML::IXMLDOMNamedNodeMapPtr pMap, LPCTSTR lpcszName );
	static	CSPTime	getAttrValue_T( MSXML::IXMLDOMNamedNodeMapPtr pMap, LPCTSTR lpcszName );
};

class CXMLDocument : public CObject
{
public:
	CXMLDocument();
	virtual ~CXMLDocument();

	// Attributes
	MSXML::IXMLDOMDocumentPtr XMLDoc() { return m_pDOMDoc; };

	// Operations
	BOOL Initialize();
	void Release();
	BOOL Load( LPCTSTR lpcszURL );
	
	BOOL			SetRawDocument( CString &strDoc );
	void			SetLastErrorMessage(LPCTSTR lpszError);
	virtual BOOL	GetLastErrorMessage(LPTSTR lpszError, UINT nMaxError );

protected:
	MSXML::IXMLDOMDocumentPtr m_pDOMDoc;
	CString	m_strLastErrorMessage;
};
*/
// The Old XMLDoc using msxml.dll 2.5(gb2312) or above
/////////////////////////////////////////////////////////////////////////////////////


#endif // !defined(AFX_XMLDOC_H__3533AF4F_42FA_4117_86FB_57AD0B2283A3__INCLUDED_)
