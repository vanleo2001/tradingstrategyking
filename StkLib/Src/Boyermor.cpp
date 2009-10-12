//=============================================================================
// File:       boyermor.cpp
// Contents:   Definitions for BoyerMoore
// Maintainer: Doug Sauder <dwsauder@hunnysoft.com>
// WWW:        http://www.hunnysoft.com/mimepp/
//
// Copyright (c) 1996-1998 Douglas W. Sauder
// All rights reserved.
// 
// IN NO EVENT SHALL DOUGLAS W. SAUDER BE LIABLE TO ANY PARTY FOR DIRECT,
// INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF
// THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF DOUGLAS W. SAUDER
// HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// DOUGLAS W. SAUDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT
// NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
// BASIS, AND DOUGLAS W. SAUDER HAS NO OBLIGATION TO PROVIDE MAINTENANCE,
// SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
//
//=============================================================================

#include "StdAfx.h"
#include <string.h>
#include "../Include/boyermor.h"


BoyerMoore::BoyerMoore(const char* aCstr)
{
	mPat	=	NULL;

    size_t len = strlen(aCstr);
    _Assign(aCstr, len);
}

BoyerMoore::~BoyerMoore()
{
	if( mPat )
	{
		delete	mPat;
		mPat	=	NULL;
	}
}

void BoyerMoore::Assign(const char* aCstr)
{
    size_t len = strlen(aCstr);
    _Assign(aCstr, len);
}

void BoyerMoore::_Assign(const char* aPat, size_t aPatLen)
{
    mPatLen = 0;
    mPat = new char[aPatLen+1];
    if (mPat != 0) {
        mPatLen = aPatLen;
        strncpy(mPat, aPat, mPatLen);
        mPat[mPatLen] = 0;
        // Initialize the jump table for Boyer-Moore-Horspool algorithm
        size_t i;
        for (i=0; i < 256; ++i) {
            mSkipAmt[i] = (unsigned char) mPatLen;
        }
        for (i=0; i < mPatLen-1; ++i) {
            mSkipAmt[(unsigned char)mPat[i]] = (unsigned char) (mPatLen - i - 1);
        }
    }
}

size_t BoyerMoore::FindIn(const char * string, size_t length, size_t startpos)
{
	if ( NULL == string || length <= startpos ) {
		return (size_t) -1;
	}
	if (mPat == 0 || mPatLen == 0) {
		return 0;
	}
	size_t bufLen = length - startpos;
	const char* buf = string + startpos;
	size_t i;
	for (i=mPatLen-1; i < bufLen; i += mSkipAmt[(unsigned char)buf[i]]) {
		int iBuf = i;
		int iPat = mPatLen - 1;
		while (iPat >= 0 && buf[iBuf] == mPat[iPat]) {
			--iBuf;
			--iPat;
		}
		if (iPat == -1)
			return startpos + iBuf + 1;
	}
	return (size_t)-1;
}

int BoyerMoore::GetNumStringAfter( const char *string, size_t length, size_t startpos,
								  char * rstring, size_t rmaxlen )
{
	size_t	nPos	=	FindIn( string, length, startpos );
	if( nPos == ((size_t)-1) || nPos >= length || nPos < 0 )
		return 0;

	nPos	+=	mPatLen;
	
	while( isspace( string[nPos] ) && nPos < length )
		nPos	++;

	int	nCount	=	0;
	while( !isdigit(BYTE(string[nPos])) && string[nPos] != '.' && string[nPos] != '-' && string[nPos] != '\n'
				&& nPos < length && nCount < 256 )
	{	nPos	++;	nCount ++;	}

	if( nPos >= length )
		return 0;

	size_t	nStart	=	nPos;
	while( ( isdigit(BYTE(string[nPos])) || string[nPos] == '.' || string[nPos] == '-' )
			&& nPos < length )
		nPos	++;
	size_t	nEnd	=	nPos;
	
	memset( rstring, 0, rmaxlen );
	if( nEnd - nStart > rmaxlen-1 )
		return 0;

	strncpy( rstring, &string[nStart], nEnd-nStart );
	return nEnd - nStart;
}

double BoyerMoore::GetNumberAfter( const char *string, size_t length, size_t startpos )
{
	double	result	=	0.;
	char	rstring[64];
	int rLen = GetNumStringAfter( string, length, startpos, rstring, 64 );
	if( rLen > 0 )
	{
		result	=	atof( rstring );
	}
	return result;
}

DWORD BoyerMoore::GetDateAfter( const char *string, size_t length, size_t startpos )
{
	// Convert to Date as DWORD
	char	rstring[64];
	int rLen = GetNumStringAfter( string, length, startpos, rstring, 64 );
	if( rLen > 0 )
	{
		int	nYear	=	atol( rstring );
		int k;
		for( k=0; k<rLen; k++ )
			if( rstring[k] == '-' )
				break;
		k++;
		int nMonth	=	atol( &(rstring[k]) );
		for( ; k<rLen; k++ )
			if( rstring[k] == '-' )
				break;
		k++;
		int nDay	=	atol( &(rstring[k]) );

		DWORD	result	=	nYear * 10000 + nMonth * 100 + nDay;
		return result;
	}
	return 0;
}

int BoyerMoore::GetPhraseAfter( const char *string, size_t length, size_t startpos,
								  char * rstring, size_t rmaxlen )
{
	size_t	nPos	=	FindIn( string, length, startpos );
	if( nPos == ((size_t)-1) || nPos >= length || nPos < 0 )
		return 0;

	nPos	+=	mPatLen;
	
	if( nPos >= length )
		return 0;

	size_t	nStart	=	nPos;
	while( string[nPos] != '\r' && string[nPos] != '\n'
			&& nPos < length )
		nPos	++;
	size_t	nEnd	=	nPos;
	
	memset( rstring, 0, rmaxlen );
	if( nEnd - nStart > rmaxlen-1 )
		return 0;

	strncpy( rstring, &string[nStart], nEnd-nStart );
	return nEnd - nStart;
}
