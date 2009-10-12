/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CSPString;
				like CString
*/

#if	!defined( __SP_STRING_H__)
#define	__SP_STRING_H__

#include	<TCHAR.h>

/////////////////////////////////////////////////////////////////////////////
// CSPString

struct CSPStringData
{
	long nRefs;             // reference count
	int nDataLength;        // length of data (including terminator)
	int nAllocLength;       // length of allocation
	// TCHAR data[nAllocLength]

	TCHAR* data()           // TCHAR* to managed data
		{ return (TCHAR*)(this+1); }
};

class STKLIB_API CSPString
{
public:
// Constructors

	// constructs empty CSPString
	CSPString();
	// copy constructor
	CSPString(const CSPString& stringSrc);
	// from a single character
	CSPString(TCHAR ch, int nRepeat = 1);
	// from an ANSI string (converts to TCHAR)
	CSPString(LPCSTR lpsz);
	// from a UNICODE string (converts to TCHAR)
	CSPString(LPCWSTR lpsz);
	// subset of characters from an ANSI string (converts to TCHAR)
	CSPString(LPCSTR lpch, int nLength);
	// subset of characters from a UNICODE string (converts to TCHAR)
	CSPString(LPCWSTR lpch, int nLength);
	// from unsigned characters
	CSPString(const unsigned char* psz);

// Attributes & Operations

	// get data length
	int GetLength() const;
	// TRUE if zero length
	BOOL IsEmpty() const;
	// clear contents to empty
	void Empty();

	// return single character at zero-based index
	TCHAR GetAt(int nIndex) const;
	// return single character at zero-based index
	TCHAR operator[](int nIndex) const;
	// set a single character at zero-based index
	void SetAt(int nIndex, TCHAR ch);
	// return pointer to const string
	operator LPCTSTR() const;

	// overloaded assignment

	// ref-counted copy from another CSPString
	const CSPString& operator=(const CSPString& stringSrc);
	// set string content to single character
	const CSPString& operator=(TCHAR ch);
#ifdef _UNICODE
	const CSPString& operator=(char ch);
#endif
	// copy string content from ANSI string (converts to TCHAR)
	const CSPString& operator=(LPCSTR lpsz);
	// copy string content from UNICODE string (converts to TCHAR)
	const CSPString& operator=(LPCWSTR lpsz);
	// copy string content from unsigned chars
	const CSPString& operator=(const unsigned char* psz);

	// string concatenation

	// concatenate from another CSPString
	const CSPString& operator+=(const CSPString& string);

	// concatenate a single character
	const CSPString& operator+=(TCHAR ch);
#ifdef _UNICODE
	// concatenate an ANSI character after converting it to TCHAR
	const CSPString& operator+=(char ch);
#endif
	// concatenate a UNICODE character after converting it to TCHAR
	const CSPString& operator+=(LPCTSTR lpsz);

	STKLIB_API	friend CSPString __stdcall operator+(const CSPString& string1,
			const CSPString& string2);
	STKLIB_API	friend CSPString __stdcall operator+(const CSPString& string, TCHAR ch);
	STKLIB_API	friend CSPString __stdcall operator+(TCHAR ch, const CSPString& string);
#ifdef _UNICODE
	STKLIB_API	friend CSPString __stdcall operator+(const CSPString& string, char ch);
	STKLIB_API	friend CSPString __stdcall operator+(char ch, const CSPString& string);
#endif
	STKLIB_API	friend CSPString __stdcall operator+(const CSPString& string, LPCTSTR lpsz);
	STKLIB_API	friend CSPString __stdcall operator+(LPCTSTR lpsz, const CSPString& string);

	// string comparison

	// straight character comparison
	int Compare(LPCTSTR lpsz) const;
	// compare ignoring case
	int CompareNoCase(LPCTSTR lpsz) const;
	// NLS aware comparison, case sensitive
	int Collate(LPCTSTR lpsz) const;
	// NLS aware comparison, case insensitive
	int CollateNoCase(LPCTSTR lpsz) const;

	// simple sub-string extraction

	// return nCount characters starting at zero-based nFirst
	CSPString Mid(int nFirst, int nCount) const;
	// return all characters starting at zero-based nFirst
	CSPString Mid(int nFirst) const;
	// return first nCount characters in string
	CSPString Left(int nCount) const;
	// return nCount characters from end of string
	CSPString Right(int nCount) const;

	//  characters from beginning that are also in passed string
	CSPString SpanIncluding(LPCTSTR lpszCharSet) const;
	// characters from beginning that are not also in passed string
	CSPString SpanExcluding(LPCTSTR lpszCharSet) const;

	// upper/lower/reverse conversion

	// NLS aware conversion to uppercase
	void MakeUpper();
	// NLS aware conversion to lowercase
	void MakeLower();
	// reverse string right-to-left
	void MakeReverse();

	// trimming whitespace (either side)

	// remove whitespace starting from right edge
	void TrimRight();
	// remove whitespace starting from left side
	void TrimLeft();

	// trimming anything (either side)

	// remove continuous occurrences of chTarget starting from right
	void TrimRight(TCHAR chTarget);
	// remove continuous occcurrences of characters in passed string,
	// starting from right
	void TrimRight(LPCTSTR lpszTargets);
	// remove continuous occurrences of chTarget starting from left
	void TrimLeft(TCHAR chTarget);
	// remove continuous occcurrences of characters in
	// passed string, starting from left
	void TrimLeft(LPCTSTR lpszTargets);

	// advanced manipulation

	// replace occurrences of chOld with chNew
	int Replace(TCHAR chOld, TCHAR chNew);
	// replace occurrences of substring lpszOld with lpszNew;
	// empty lpszNew removes instances of lpszOld
	int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew);
	// remove occurrences of chRemove
	int Remove(TCHAR chRemove);
	// insert character at zero-based index; concatenates
	// if index is past end of string
	int Insert(int nIndex, TCHAR ch);
	// insert substring at zero-based index; concatenates
	// if index is past end of string
	int Insert(int nIndex, LPCTSTR pstr);
	// delete nCount characters starting at zero-based index
	int Delete(int nIndex, int nCount = 1);

	// searching

	// find character starting at left, -1 if not found
	int Find(TCHAR ch) const;
	// find character starting at right
	int ReverseFind(TCHAR ch) const;
	// find character starting at zero-based index and going right
	int Find(TCHAR ch, int nStart) const;
	// find first instance of any character in passed string
	int FindOneOf(LPCTSTR lpszCharSet) const;
	// find first instance of substring
	int Find(LPCTSTR lpszSub) const;
	// find first instance of substring starting at zero-based index
	int Find(LPCTSTR lpszSub, int nStart) const;

	// simple formatting

	// printf-like formatting using passed string
	void __cdecl Format(LPCTSTR lpszFormat, ...);
	// printf-like formatting using referenced string resource
	void __cdecl Format(UINT nFormatID, ...);
	// printf-like formatting using variable arguments parameter
	void FormatV(LPCTSTR lpszFormat, va_list argList);

	// load from string resource
	BOOL LoadString(UINT nID);

	// Access to string implementation buffer as "C" character array

	// get pointer to modifiable buffer at least as long as nMinBufLength
	LPTSTR GetBuffer(int nMinBufLength);
	// release buffer, setting length to nNewLength (or to first nul if -1)
	void ReleaseBuffer(int nNewLength = -1);
	// get pointer to modifiable buffer exactly as long as nNewLength
	LPTSTR GetBufferSetLength(int nNewLength);
	// release memory allocated to but unused by string
	void FreeExtra();

	// Use LockBuffer/UnlockBuffer to turn refcounting off

	// turn refcounting back on
	LPTSTR LockBuffer();
	// turn refcounting off
	void UnlockBuffer();

// Implementation
public:
	~CSPString();
	int GetAllocLength() const;

protected:
	LPTSTR m_pchData;   // pointer to ref counted string data

	// implementation helpers
	CSPStringData* GetData() const;
	void Init();
	void AllocCopy(CSPString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;
	void AllocBuffer(int nLen);
	void AssignCopy(int nSrcLen, LPCTSTR lpszSrcData);
	void ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data, int nSrc2Len, LPCTSTR lpszSrc2Data);
	void ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData);
	void CopyBeforeWrite();
	void AllocBeforeWrite(int nLen);
	void Release();
	static void PASCAL Release(CSPStringData* pData);
	static int PASCAL SafeStrlen(LPCTSTR lpsz);
	static void __fastcall FreeData(CSPStringData* pData);
};

// Compare helpers
bool __stdcall operator==(const CSPString& s1, const CSPString& s2);
bool __stdcall operator==(const CSPString& s1, LPCTSTR s2);
bool __stdcall operator==(LPCTSTR s1, const CSPString& s2);
bool __stdcall operator!=(const CSPString& s1, const CSPString& s2);
bool __stdcall operator!=(const CSPString& s1, LPCTSTR s2);
bool __stdcall operator!=(LPCTSTR s1, const CSPString& s2);
bool __stdcall operator<(const CSPString& s1, const CSPString& s2);
bool __stdcall operator<(const CSPString& s1, LPCTSTR s2);
bool __stdcall operator<(LPCTSTR s1, const CSPString& s2);
bool __stdcall operator>(const CSPString& s1, const CSPString& s2);
bool __stdcall operator>(const CSPString& s1, LPCTSTR s2);
bool __stdcall operator>(LPCTSTR s1, const CSPString& s2);
bool __stdcall operator<=(const CSPString& s1, const CSPString& s2);
bool __stdcall operator<=(const CSPString& s1, LPCTSTR s2);
bool __stdcall operator<=(LPCTSTR s1, const CSPString& s2);
bool __stdcall operator>=(const CSPString& s1, const CSPString& s2);
bool __stdcall operator>=(const CSPString& s1, LPCTSTR s2);
bool __stdcall operator>=(LPCTSTR s1, const CSPString& s2);

// Globals
extern STKLIB_API TCHAR afxSPChNil;
#ifdef _AFXDLL
const CSPString& __stdcall AfxGetEmptySPString();
#define afxEmptySPString AfxGetEmptySPString()
#else
extern LPCTSTR _afxSPPchNil;
#define afxEmptySPString ((CSPString&)*(CSPString*)&_afxSPPchNil)
#endif

#ifdef	_SP_ENABLE_INLINES
#define	_SPSTRING_INLINE	inline
#include "SpString.inl"
#undef	_SPSTRING_INLINE
#endif

#endif //__SP_STRING_H__
