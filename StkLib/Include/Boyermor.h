//=============================================================================
// File:       boyermor.h
// Contents:   Declarations for BoyerMoore
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

#ifndef __STKLIB_BOYERMOR_H__
#define __STKLIB_BOYERMOR_H__


//=============================================================================
//+ Name BoyerMoore -- Class for executing Boyer-Moore string search algorithm
//+ Description
//. <var>BoyerMoore</var> implements the Boyer-Moore algorithm for searching
//. for a string.  The Boyer-Moore algorithm is fast, but requires a bit
//. of start-up overhead compared to a brute force algorithm.
//=============================================================================
// Last modified 1998-04-28
//+ Noentry ~BoyerMoore

/***
	快速查找算法类定义，用法如下：
	CString	strToFind	=	"要查找的字符串";
	CString	strSrc		=	"源字符串";
	BoyerMoore moore( strToFind );
	LONG value = moore.GetNumberAfter( strSrc, strSrc.GetLength(), 0 );
	LONG date  = moore.GetDateAfter( strSrc, strSrc.GetLength(), 0 );
*/
class STKLIB_API BoyerMoore {

public:
    
    BoyerMoore(const char* aCstr);
    //. Constructs a <var>BoyerMoore</var> object for searching for
    //. a particular string.

    virtual ~BoyerMoore();

    void Assign(const char* aCstr);
    //. Sets the string to search for.

    size_t FindIn(const char *string, size_t length, size_t startpos);
    //. Searches for the search string in <var>string</var> starting at position
    //. <var>startpos</var>.  If found, the function returns the first position in
    //. <var>string</var> where the search string was found.  If not found, the
    //. function returns <var>CSPString::npos</var>.


	double	GetNumberAfter( const char *string, size_t length, size_t startpos );
	DWORD	GetDateAfter( const char *string, size_t length, size_t startpos );
	int		GetPhraseAfter( const char *string, size_t length, size_t startpos, char * rstring, size_t rmaxlen );

private:
	int		GetNumStringAfter( const char *string, size_t length, size_t startpos,
								  char * rstring, size_t rmaxlen );

    size_t mPatLen;
    char* mPat;
    unsigned char mSkipAmt[256];

    void _Assign(const char* aPat, size_t aPatLen);
};

#endif
