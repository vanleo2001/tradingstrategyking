// Markup.cpp: implementation of the CMarkup class.
//
// Markup Release 6.1 Lite
// Copyright (C) 1999-2001 First Objective Software, Inc. All rights reserved
// This entire notice must be retained in this source code
// Redistributing this source code requires written permission
// This software is provided "as is", with no warranty.
// Latest fixes enhancements and documentation at www.firstobject.com

#include "stdafx.h"
#include "afxconv.h"
#include "Markup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CMarkup::operator=( const CMarkup& markup )
{
	m_iPos = markup.m_iPos;
	m_iPosChild = markup.m_iPosChild;
	m_iPosFree = markup.m_iPosFree;
	m_aPos.RemoveAll();
	m_aPos.Append( markup.m_aPos );
	m_nLevel = markup.m_nLevel;
	m_csDoc = markup.m_csDoc;
}

void CMarkup::ResetPos()
{
	// Reset the main and child positions
	m_iPos = 0;
	m_iPosChild = 0;
	m_nLevel = 0;
};

bool CMarkup::SetDoc( LPCTSTR szDoc )
{
	// Reset indexes
	m_iPosFree = 1;
	ResetPos();

	// Set document text
	if ( szDoc )
		m_csDoc = szDoc;
	else
		m_csDoc.Empty();

	// Starting size of position array: 1 element per 64 bytes of document
	// Tight fit when parsing small doc, only 0 to 2 reallocs when parsing large doc
	// Start at 8 when creating new document
	int nStartSize = m_csDoc.GetLength() / 64 + 8;
	if ( m_aPos.GetSize() < nStartSize )
		m_aPos.SetSize( nStartSize );

	// Parse document
	bool bWellFormed = false;
	if ( m_csDoc.GetLength() )
	{
		m_aPos[0].Clear();
		int iPos = x_ParseElem( 0 );
		if ( iPos > 0 )
		{
			m_aPos[0].iElemChild = iPos;
			bWellFormed = true;
		}
	}

	// Clear indexes if parse failed or empty document
	if ( ! bWellFormed )
	{
		m_aPos[0].Clear();
		m_iPosFree = 1;
	}

	ResetPos();
	return bWellFormed;
};

bool CMarkup::IsWellFormed()
{
	if ( m_aPos.GetSize() && m_aPos[0].iElemChild )
		return TRUE;
	return FALSE;
}

bool CMarkup::FindElem( LPCTSTR szName )
{
	// If szName is NULL or empty, go to next sibling element
	// Otherwise go to next sibling element with matching tag name
	// If the current position is valid, start looking from next
	// Change current position only if found
	//
	int iPos = m_iPos;
	if ( ! iPos )
	{
		if ( m_aPos.GetSize() )
			iPos = m_aPos[0].iElemChild;
	}
	else
	{
		iPos = m_aPos[iPos].iElemNext;
	}

	while ( iPos )
	{
		// Compare tag name unless szName is not specified
		if ( szName == NULL || !szName[0] || x_GetTagName(iPos) == szName )
		{
			// Assign new position
			m_iPos = iPos;
			m_iPosChild = 0;
			return true;
		}
		iPos = m_aPos[iPos].iElemNext;
	}
	return false;
}

bool CMarkup::FindChildElem( LPCTSTR szName )
{
	// If szName is NULL or empty, go to next sibling child element
	// Otherwise go to next sibling child element with matching tag name
	// If the current child position is valid, start looking from next
	// Change current child position only if found
	//
	// Shorthand: call this with no current position means under root element
	if ( ! m_iPos )
		FindElem();

	// Is main position valid and not empty?
	if ( ! m_iPos || m_aPos[m_iPos].IsEmptyElement() )
		return false;

	// Is current child position valid?
	int iPosChild = m_iPosChild;
	if ( iPosChild )
		iPosChild = m_aPos[iPosChild].iElemNext;
	else
		iPosChild = m_aPos[m_iPos].iElemChild;

	// Search
	while ( iPosChild )
	{
		// Compare tag name unless szName is not specified
		if ( szName == NULL || !szName[0] || x_GetTagName(iPosChild) == szName )
		{
			// Assign new position
			m_iPosChild = iPosChild;
			return true;
		}
		iPosChild = m_aPos[iPosChild].iElemNext;
	}
	return false;
}

bool CMarkup::IntoElem()
{
	// Find child element unless there is already a child element position
	if ( ! m_iPosChild )
		FindChildElem();

	if ( m_iPosChild )
	{
		m_iPos = m_iPosChild;
		m_iPosChild = 0;
		++m_nLevel;
		return true;
	}

	return false;
}

bool CMarkup::OutOfElem()
{
	// Go to parent element
	if ( m_iPos && m_nLevel > 0 )
	{
		m_iPosChild = m_iPos;
		m_iPos = m_aPos[m_iPos].iElemParent;
		--m_nLevel;
		return true;
	}
	return false;
}

bool CMarkup::GetOffsets( int& nStart, int& nEnd ) const
{
	// Return document offsets of current main position element
	// This is not part of EDOM but is used by the Markup project
	if ( m_iPos )
	{
		nStart = m_aPos[m_iPos].nStartL;
		nEnd = m_aPos[m_iPos].nEndR;
		return true;
	}
	return false;
}

bool CMarkup::AddElem( LPCTSTR szName, LPCTSTR szValue )
{
	// Add an element after current main position
	int iPosParent = m_iPos? m_aPos[m_iPos].iElemParent : 0;
	m_iPosChild = 0;

	// Setting root element?
	if ( iPosParent == 0 )
	{
		if ( IsWellFormed() )
			return false;

		m_csDoc.Empty();
	}

	m_iPos = x_Add( iPosParent, m_iPos, szName, szValue );
	return true;
}

bool CMarkup::AddChildElem( LPCTSTR szName, LPCTSTR szValue )
{
	// Add a child element under main position, after current child position
	if ( ! m_iPos )
		return false;

	// If no child position, add after last sibling
	int iPosLast = m_aPos[m_iPos].iElemChild;
	if ( ! m_iPosChild && iPosLast )
	{
		m_iPosChild = iPosLast;
		while ( (iPosLast=m_aPos[iPosLast].iElemNext) != 0 )
			m_iPosChild = iPosLast;
	}

	m_iPosChild = x_Add( m_iPos, m_iPosChild, szName, szValue );
	return true;
}

bool CMarkup::AddAttrib( LPCTSTR szAttrib, LPCTSTR szValue )
{
	// Add attribute to current main position element
	if ( m_iPos )
	{
		x_AddAttrib( m_iPos, szAttrib, szValue );
		return true;
	}
	return false;
}

bool CMarkup::AddChildAttrib( LPCTSTR szAttrib, LPCTSTR szValue )
{
	// Add attribute to current child position element
	if ( m_iPosChild )
	{
		x_AddAttrib( m_iPosChild, szAttrib, szValue );
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// Private Methods
//////////////////////////////////////////////////////////////////////

int CMarkup::x_GetFreePos()
{
	//
	// This returns the index of the next unused ElemPos in the array
	//
	if ( m_iPosFree == m_aPos.GetSize() )
		m_aPos.SetSize( m_iPosFree + m_iPosFree / 2 );
	++m_iPosFree;
	return m_iPosFree - 1;
}

int CMarkup::x_ReleasePos()
{
	//
	// This decrements the index of the next unused ElemPos in the array
	// allowing the element index returned by GetFreePos() to be reused
	//
	--m_iPosFree;
	return 0;
}

int CMarkup::x_ParseError( LPCTSTR szError, LPCTSTR szTag )
{
	if ( szTag )
		m_csError.Format( szError, szTag );
	else
		m_csError = szError;
	x_ReleasePos();
	return -1;
}

int CMarkup::x_ParseElem( int iPosParent )
{
	// This is either called by SetDoc, x_AddSubDoc, or itself recursively
	// m_aPos[iPosParent].nEndL is where to start parsing for the child element
	// This returns the new position if a tag is found, otherwise zero
	// In all cases we need to get a new ElemPos, but release it if unused
	//
	int iPos = x_GetFreePos();
	m_aPos[iPos].nStartL = m_aPos[iPosParent].nEndL;
	m_aPos[iPos].nNext = m_aPos[iPosParent].nStartR + 1;
	m_aPos[iPos].iElemParent = iPosParent;
	m_aPos[iPos].iElemChild = 0;
	m_aPos[iPos].iElemNext = 0;

	// Start Tag
	// A loop is used to ignore all remarks tags and special tags
	// i.e. <?xml version="1.0"?>, and <!-- comment here -->
	// So any tag beginning with ? or ! is ignored
	// Loop past ignored tags
	TokenPos token;
	token.nNext = m_aPos[iPosParent].nEndL;
	CString csName;
	while ( csName.IsEmpty() )
	{
		// Look for left angle bracket of start tag
		m_aPos[iPos].nStartL = token.nNext;
		if ( ! x_FindChar( m_aPos[iPos].nStartL, _T('<') ) )
			return x_ParseError( _T("Element tag not found") );

		// Set parent's End tag to start looking from here (or later)
		m_aPos[iPosParent].nEndL = m_aPos[iPos].nStartL;

		// Determine whether this is an element, comment or version tag
		LPCTSTR szEndOfTag = NULL;
		token.nNext = m_aPos[iPos].nStartL + 1;
		if ( x_FindToken( token ) )
		{
			if ( token.bIsString )
				return x_ParseError( _T("Tag starts with quote") );
			TCHAR cFirstChar = m_csDoc[token.nL];
			if ( cFirstChar == _T('?') )
				szEndOfTag = _T("?>"); // version
			else if ( cFirstChar == _T('!') )
			{
				TCHAR cSecondChar = 0;
				if ( token.nL+1 < m_csDoc.GetLength() )
					cSecondChar = m_csDoc[token.nL+1];
				if ( cSecondChar == _T('[') )
					szEndOfTag = _T("]]>"); // CDATA section
				else if ( cSecondChar == _T('-') )
					szEndOfTag = _T("-->"); // comment
				else
					szEndOfTag = _T(">"); // DTD
			}
			else if ( cFirstChar != _T('/') )
			{
				csName = x_GetToken( token );
				szEndOfTag = _T(">");
			}
			else
				return x_ReleasePos(); // probably end tag of parent
		}
		else
			return x_ParseError( _T("Abrupt end within tag") );

		// Look for end of tag
		token.nNext = m_csDoc.Find( szEndOfTag, token.nNext );
		if ( token.nNext == -1 )
			return x_ParseError( _T("End of tag not found") );
	}
	m_aPos[iPos].nStartR = token.nNext;

	// Is ending mark within start tag, i.e. empty element?
	if ( m_csDoc[m_aPos[iPos].nStartR-1] == _T('/') )
	{
		// Empty element
		// Close tag left is set to ending mark, and right to open tag right
		m_aPos[iPos].nEndL = m_aPos[iPos].nStartR-1;
		m_aPos[iPos].nEndR = m_aPos[iPos].nStartR;
	}
	else // look for end tag
	{
		// Element probably has contents
		// Determine where to start looking for left angle bracket of end tag
		// This is done by recursively parsing the contents of this element
		int iInner, iInnerPrev = 0;
		m_aPos[iPos].nEndL = m_aPos[iPos].nStartR + 1;
		while ( (iInner = x_ParseElem( iPos )) > 0 )
		{
			// Set links to iInner
			if ( iInnerPrev )
				m_aPos[iInnerPrev].iElemNext = iInner;
			else
				m_aPos[iPos].iElemChild = iInner;
			iInnerPrev = iInner;

			// Set offset to reflect child
			m_aPos[iPos].nEndL = m_aPos[iInner].nEndR + 1;
		}
		if ( iInner == -1 )
			return -1;

		// Look for left angle bracket of end tag
		if ( ! x_FindChar( m_aPos[iPos].nEndL, _T('<') ) )
			return x_ParseError( _T("End tag of %s element not found"), csName );

		// Look through tokens of end tag
		token.nNext = m_aPos[iPos].nEndL + 1;
		int nTokenCount = 0;
		while ( x_FindToken( token ) )
		{
			++nTokenCount;
			if ( ! token.bIsString )
			{
				// Is first token not an end slash mark?
				if ( nTokenCount == 1 && m_csDoc[token.nL] != _T('/') )
					return x_ParseError( _T("Expecting end tag of element %s"), csName );

				else if ( nTokenCount == 2 && csName != x_GetToken( token ) )
					return x_ParseError( _T("End tag does not correspond to %s"), csName );

				// Else is it a right angle bracket?
				else if ( m_csDoc[token.nL] == _T('>') )
					break;
			}
		}

		// Was a right angle bracket not found?
		if ( ! token.IsValid() || nTokenCount < 2 )
			return x_ParseError( _T("End tag not completed for element %s"), csName );
		m_aPos[iPos].nEndR = token.nL;
	}

	// Successfully found positions of angle brackets
	m_aPos[iPos].nNext = m_aPos[iPos].nEndR;
	x_FindChar( m_aPos[iPos].nNext, _T('<') );
	return iPos;
}

bool CMarkup::x_FindChar( int&n, _TCHAR c ) const
{
	// Look for char c starting at n, and set n to point to it
	// c is always the first char of a multi-byte char
	// Return false if not found before end of document
	LPCTSTR szDoc = (LPCTSTR)m_csDoc;
	while ( szDoc[n] && szDoc[n] != c )
		n += _tclen( &szDoc[n] );
	if ( ! szDoc[n] )
		return false;
	return true;
}

bool CMarkup::x_FindToken( CMarkup::TokenPos& token ) const
{
	// Starting at token.nNext, find the next token
	// upon successful return, token.nNext points after the retrieved token
	LPCTSTR szDoc = (LPCTSTR)m_csDoc;
	int n = token.nNext;

	// Statically defined CStrings for whitespace and special chars
	static CString csWhitespace = _T(" \t\n\r");
	static CString csSpecial = _T("<>=\\/?!");

	// By-pass leading whitespace
	while ( szDoc[n] && csWhitespace.Find(szDoc[n]) > -1 )
		++n;

	// Are we still within the document?
	token.bIsString = false;
	if ( szDoc[n] )
	{
		// Is it an opening quote?
		if ( szDoc[n] == _T('\"') )
		{
			// Move past opening quote
			++n;
			token.nL = n;

			// Look for closing quote
			x_FindChar( n, _T('\"') );

			// Set right to before closing quote
			token.nR = n-1;

			// Set n past closing quote unless at end of document
			if ( szDoc[n] )
				++n;

			// Set flag
			token.bIsString = true;
		}
		else
		{
			// Go until special char or whitespace
			token.nL = n;
			while ( szDoc[n] &&
				csSpecial.Find(m_csDoc[n]) == -1 &&
				csWhitespace.Find(m_csDoc[n]) == -1
				)
				n += _tclen(&szDoc[n]);

			// Adjust end position if it is one special char
			if ( n == token.nL )
				++n; // it is a special char
			token.nR = n-1;
		}
	}

	token.nNext = n;
	if ( ! szDoc[n] )
		return false;

	// nNext points to one past last char of token
	return true;
}

CString CMarkup::x_GetToken( const CMarkup::TokenPos& token ) const
{
	// The token contains indexes into the document identifying a small substring
	// Build the substring from those indexes and return it
	if ( ! token.IsValid() )
		return _T("");
	return m_csDoc.Mid( token.nL,
		token.nR - token.nL + ((token.nR<m_csDoc.GetLength())? 1:0) );
}

CString CMarkup::x_GetTagName( int iPos ) const
{
	// Return the tag name at specified element
	TokenPos token;
	token.nNext = m_aPos[iPos].nStartL + 1;
	if ( ! iPos || ! x_FindToken( token ) )
		return _T("");

	// Return substring of document
	return x_GetToken( token );
}

bool CMarkup::x_FindAttrib( CMarkup::TokenPos& token, LPCTSTR szAttrib ) const
{
	// If szAttrib is NULL find next attrib, otherwise find named attrib
	// Return true if found
	int nAttrib = 0;
	for ( int nCount = 0; x_FindToken(token); ++nCount )
	{
		if ( ! token.bIsString )
		{
			// Is it the right angle bracket?
			if ( m_csDoc[token.nL] == _T('>') || m_csDoc[token.nL] == _T('/') )
				break; // attrib not found

			// Equal sign
			if ( m_csDoc[token.nL] == _T('=') )
				continue;

			// Potential attribute
			if ( ! nAttrib && nCount )
			{
				// Attribute name search?
				if ( ! szAttrib || ! szAttrib[0] )
					return true; // return with token at attrib name

				// Compare szAttrib
				if ( x_GetToken(token) == szAttrib )
					nAttrib = nCount;
			}
		}
		else if ( nAttrib && nCount == nAttrib + 2 )
		{
			return true;
		}
	}

	// Not found
	return false;
}

CString CMarkup::x_GetAttrib( int iPos, LPCTSTR szAttrib ) const
{
	// Return the value of the attrib at specified element
	TokenPos token;
	token.nNext = m_aPos[iPos].nStartL + 1;
	if ( szAttrib && x_FindAttrib( token, szAttrib ) )
		return x_TextFromDoc( token.nL, token.nR - ((token.nR<m_csDoc.GetLength())?0:1) );
	return _T("");
}

CString CMarkup::x_GetData( int iPos ) const
{
	// Return a string representing data between start and end tag
	// Return empty string if there are any children elements
	if ( ! m_aPos[iPos].iElemChild && ! m_aPos[iPos].IsEmptyElement() )
	{
		// See if it is a CDATA section
		TokenPos token;
		token.nNext = m_aPos[iPos].nStartR+1;
		if ( x_FindToken( token ) && m_csDoc[token.nL] == _T('<')
				&& token.nL + 11 < m_aPos[iPos].nEndL
				&& _tcsncmp( &((LPCTSTR)m_csDoc)[token.nL+1], _T("![CDATA["), 8 ) == 0 )
		{
			int nEndCDATA = m_csDoc.Find( _T("]]>"), token.nNext );
			if ( nEndCDATA != -1 && nEndCDATA < m_aPos[iPos].nEndL )
			{
				return m_csDoc.Mid( token.nL+9, nEndCDATA-token.nL-9 );
			}
		}
		return x_TextFromDoc( m_aPos[iPos].nStartR+1, m_aPos[iPos].nEndL-1 );
	}
	return "";
}

CString CMarkup::x_TextToDoc( LPCTSTR szText, bool bAttrib ) const
{
	// Convert text as seen outside XML document to XML friendly
	// replacing special characters with ampersand escape codes
	// E.g. convert "6>7" to "6&gt;7"
	//
	// &lt;   less than
	// &amp;  ampersand
	// &gt;   greater than
	//
	// and for attributes:
	//
	// &apos; apostrophe or single quote
	// &quot; double quote
	//
	static CHAR* szaReplace[] = { _T("&lt;"),_T("&amp;"),_T("&gt;"),_T("&apos;"),_T("&quot;") };
	const CHAR* pFind = bAttrib?_T("<&>\'\""):_T("<&>");
	CString csText;
	const CHAR* pSource = szText;
	int nDestSize = _tcslen(pSource);
	nDestSize += nDestSize / 10 + 7;
	CHAR* pDest = csText.GetBuffer(nDestSize);
	int nLen = 0;
	CHAR cSource = *pSource;
	CHAR* pFound;
	while ( cSource )
	{
		if ( nLen > nDestSize - 6 )
		{
			csText.ReleaseBuffer(nLen);
			nDestSize *= 2;
			pDest = csText.GetBuffer(nDestSize);
		}
		if ( (pFound=_tcschr((PSTR)pFind,cSource)) != NULL )
		{
			pFound = szaReplace[pFound-pFind];
			_tcscpy(&pDest[nLen],pFound);
			nLen += _tcslen(pFound);
		}
		else
		{
			_tccpy( &pDest[nLen], pSource );
			++nLen;
		}
		pSource += _tclen( pSource );
		cSource = *pSource;
	}
	csText.ReleaseBuffer(nLen);
	return csText;
}

CString CMarkup::x_TextFromDoc( int nLeft, int nRight ) const
{
	// Convert XML friendly text to text as seen outside XML document
	// replacing ampersand escape codes with special characters
	// E.g. convert "6&gt;7" to "6>7"
	//
	// Conveniently the result is always the same or shorter in length
	//
	static CHAR* szaCode[] = { _T("lt;"),_T("amp;"),_T("gt;"),_T("apos;"),_T("quot;") };
	static int anCodeLen[] = { 3,4,3,5,5 };
	static CHAR* szSymbol = _T("<&>\'\"");
	CString csText;
	const CHAR* pSource = m_csDoc;
	int nDestSize = nRight - nLeft + 1;
	CHAR* pDest = csText.GetBuffer(nDestSize);
	int nLen = 0;
	int nCharLen;
	int nChar = nLeft;
	while ( nChar <= nRight )
	{
		if ( pSource[nChar] == _T('&') )
		{
			// Look for matching &code;
			int nMatch;
			for ( nMatch = 0; nMatch < 5; ++nMatch )
			{
				if ( nChar <= nRight - anCodeLen[nMatch]
					&& _tcsncmp(szaCode[nMatch],&pSource[nChar+1],anCodeLen[nMatch]) == 0 )
				{
					pDest[nLen++] = szSymbol[nMatch];
					nChar += anCodeLen[nMatch] + 1;
					break;
				}
			}

			// If no match is found it means XML doc is invalid
			// no devastating harm done, ampersand code will just be left in result
			if ( nMatch == 5 )
			{
				pDest[nLen++] = _T('&');
				++nChar;
			}
		}
		else
		{
			nCharLen = _tclen(&pSource[nChar]);
			_tccpy( &pDest[nLen], &pSource[nChar] );
			nLen += nCharLen;
			nChar += nCharLen;
		}
	}
	csText.ReleaseBuffer(nLen);
	return csText;
}

void CMarkup::x_DocChange( int nLeft, int nReplace, const CString& csInsert )
{
	// Insert csInsert int m_csDoc at nLeft replacing nReplace chars
	// Do this with only one buffer reallocation if it grows
	//
	int nDocLength = m_csDoc.GetLength();
	int nInsLength = csInsert.GetLength();

	// Make sure nLeft and nReplace are within bounds
	nLeft = max( 0, min( nLeft, nDocLength ) );
	nReplace = max( 0, min( nReplace, nDocLength-nLeft ) );

	// Get pointer to buffer with enough room
	int nNewLength = nInsLength + nDocLength - nReplace;
	int nBufferLen = nNewLength;
	_TCHAR* pDoc = m_csDoc.GetBuffer( nBufferLen );

	// Move part of old doc that goes after insert
	if ( nLeft+nReplace < nDocLength )
		memmove( &pDoc[nLeft+nInsLength], &pDoc[nLeft+nReplace], (nDocLength-nLeft-nReplace)*sizeof(_TCHAR) );

	// Copy insert
	memcpy( &pDoc[nLeft], csInsert, nInsLength*sizeof(_TCHAR) );

	// Release
	m_csDoc.ReleaseBuffer( nNewLength );
}

void CMarkup::x_Adjust( int iPos, int nShift )
{
	// Loop through affected elements and adjust indexes
	// Does not affect iPos itself
	// Algorithm:
	// 1. update next siblings and all their children
	// 2. then go up a level update end points and to step 1
	int iPosTop = m_aPos[iPos].iElemParent;
	while ( iPos )
	{
		// Were we at containing parent of affected position?
		bool bPosTop = false;
		if ( iPos == iPosTop )
		{
			// Move iPosTop up one towards root
			iPosTop = m_aPos[iPos].iElemParent;
			bPosTop = true;
		}

		// Traverse to the next update position
		if ( ! bPosTop && m_aPos[iPos].iElemChild )
		{
			// Depth first
			iPos = m_aPos[iPos].iElemChild;
		}
		else if ( m_aPos[iPos].iElemNext )
		{
			iPos = m_aPos[iPos].iElemNext;
		}
		else
		{
			// Look for next sibling of a parent of iPos
			// When going back up, parents have already been done except iPosTop
			while ( (iPos=m_aPos[iPos].iElemParent) != 0 && iPos != iPosTop )
				if ( m_aPos[iPos].iElemNext )
				{
					iPos = m_aPos[iPos].iElemNext;
					break;
				}
		}

		// Shift indexes at iPos
		if ( iPos != iPosTop )
		{
			// Move the start tag indexes
			// Don't do this for containing parent tag
			m_aPos[iPos].nStartL += nShift;
			m_aPos[iPos].nStartR += nShift;
		}
		// Move end tag indexes
		m_aPos[iPos].nEndL += nShift;
		m_aPos[iPos].nEndR += nShift;
		m_aPos[iPos].nNext += nShift;
	}
}

int CMarkup::x_Add( int iPosParent, int iPosBefore, LPCTSTR szName, LPCTSTR szValue )
{
	// Create element and modify positions of affected elements
	// if iPosBefore is NULL, insert as first element under parent
	// If no szValue is specified, an empty element is created
	// i.e. either <NAME>value</NAME> or <NAME/>
	//
	int iPos = x_GetFreePos();
	bool bEmptyParent = false;
	if ( iPosBefore )
	{
		// Follow iPosBefore
		m_aPos[iPos].nStartL = m_aPos[iPosBefore].nNext;
	}
	else if ( m_aPos[iPosParent].iElemChild )
	{
		// Insert before first child of parent
		m_aPos[iPos].nStartL = m_aPos[m_aPos[iPosParent].iElemChild].nStartL;
	}
	else if ( m_aPos[iPosParent].IsEmptyElement() )
	{
		// Parent has no separate end tag
		m_aPos[iPos].nStartL = m_aPos[iPosParent].nStartR + 2;
		bEmptyParent = true;
	}
	else
	{
		// Parent has content, but no children
		m_aPos[iPos].nStartL = m_aPos[iPosParent].nEndL;
	}

	// Set links
	m_aPos[iPos].iElemParent = iPosParent;
	m_aPos[iPos].iElemChild = 0;
	if ( iPosBefore )
	{
		m_aPos[iPos].iElemNext = m_aPos[iPosBefore].iElemNext;
		m_aPos[iPosBefore].iElemNext = iPos;
	}
	else
	{
		m_aPos[iPos].iElemNext = m_aPos[iPosParent].iElemChild;
		m_aPos[iPosParent].iElemChild = iPos;
	}

	// Create string for insert
	CString csInsert;
	int nLenName = _tcslen(szName);
	int nLenValue = szValue? _tcslen(szValue) : 0;
	if ( ! nLenValue )
	{
		// <NAME/> empty element
		csInsert.Format( _T("<%s/>\r\n"), szName );
		m_aPos[iPos].nStartR = m_aPos[iPos].nStartL + nLenName + 2;
		m_aPos[iPos].nEndL = m_aPos[iPos].nStartR - 1;
		m_aPos[iPos].nEndR = m_aPos[iPos].nEndL + 1;
		m_aPos[iPos].nNext = m_aPos[iPos].nEndR + 3;
	}
	else
	{
		// <NAME>value</NAME>
		CString csValue = x_TextToDoc( szValue );
		nLenValue = csValue.GetLength();
		csInsert.Format( _T("<%s>%s</%s>\r\n"), szName, csValue, szName );
		m_aPos[iPos].nStartR = m_aPos[iPos].nStartL + nLenName + 1;
		m_aPos[iPos].nEndL = m_aPos[iPos].nStartR + nLenValue + 1;
		m_aPos[iPos].nEndR = m_aPos[iPos].nEndL + nLenName + 2;
		m_aPos[iPos].nNext = m_aPos[iPos].nEndR + 3;
	}

	// Insert
	int nReplace = 0, nLeft = m_aPos[iPos].nStartL;
	if ( bEmptyParent )
	{
		CString csParentTagName = x_GetTagName(iPosParent);
		csInsert = _T(">\r\n") + csInsert + _T("</") + csParentTagName;
		nLeft -= 3;
		nReplace = 1;
		// x_Adjust is going to update all affected indexes by one amount
		// This will satisfy all except the empty parent
		// Here we pre-adjust for the empty parent
		// The empty tag slash is removed
		m_aPos[iPosParent].nStartR -= 1;
		// For the newly created end tag, see the following example:
		// <A/> (len 4) becomes <A><B/></A> (len 11)
		// In x_Adjust everything will be adjusted 11 - 4 = 7
		// But the nEndL of element A should only be adjusted 5
		m_aPos[iPosParent].nEndL -= (csParentTagName.GetLength() + 1);
	}
	x_DocChange( nLeft, nReplace, csInsert );
	x_Adjust( iPos, csInsert.GetLength() - nReplace );

	// Return the index of the new element
	return iPos;
}

int CMarkup::x_AddAttrib( int iPos, LPCTSTR szAttrib, LPCTSTR szValue )
{
	// Add attribute to iPos element
	CString csInsert;
	if ( iPos )
	{
		// Insert string taking into account whether it is a single tag
		CString csValue = x_TextToDoc( szValue, true );
		csInsert.Format( _T(" %s=\"%s\""), szAttrib, csValue );
		int nL = m_aPos[iPos].nStartR - (m_aPos[iPos].IsEmptyElement()?1:0);
		x_DocChange( nL, 0, csInsert );

		int nLen = csInsert.GetLength();
		m_aPos[iPos].nStartR += nLen;
		m_aPos[iPos].nEndL += nLen;
		m_aPos[iPos].nEndR += nLen;
		m_aPos[iPos].nNext += nLen;
		x_Adjust( iPos, nLen );
	}
	return csInsert.GetLength();
}

