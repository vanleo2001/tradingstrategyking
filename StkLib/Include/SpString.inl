
// CSPString
_SPSTRING_INLINE CSPStringData* CSPString::GetData() const
	{ SP_ASSERT(m_pchData != NULL); return ((CSPStringData*)m_pchData)-1; }
_SPSTRING_INLINE void CSPString::Init()
	{ m_pchData = afxEmptySPString.m_pchData; }
#ifndef _AFXDLL
_SPSTRING_INLINE CSPString::CSPString()
	{ m_pchData = afxEmptySPString.m_pchData; }
#endif
_SPSTRING_INLINE CSPString::CSPString(const unsigned char* lpsz)
	{ Init(); *this = (LPCSTR)lpsz; }
_SPSTRING_INLINE const CSPString& CSPString::operator=(const unsigned char* lpsz)
	{ *this = (LPCSTR)lpsz; return *this; }
#ifdef _UNICODE
_SPSTRING_INLINE const CSPString& CSPString::operator+=(char ch)
	{ *this += (TCHAR)ch; return *this; }
_SPSTRING_INLINE const CSPString& CSPString::operator=(char ch)
	{ *this = (TCHAR)ch; return *this; }
_SPSTRING_INLINE CSPString __stdcall operator+(const CSPString& string, char ch)
	{ return string + (TCHAR)ch; }
_SPSTRING_INLINE CSPString __stdcall operator+(char ch, const CSPString& string)
	{ return (TCHAR)ch + string; }
#endif

_SPSTRING_INLINE int CSPString::GetLength() const
	{ return GetData()->nDataLength; }
_SPSTRING_INLINE int CSPString::GetAllocLength() const
	{ return GetData()->nAllocLength; }
_SPSTRING_INLINE BOOL CSPString::IsEmpty() const
	{ return GetData()->nDataLength == 0; }
_SPSTRING_INLINE CSPString::operator LPCTSTR() const
	{ return m_pchData; }
_SPSTRING_INLINE int PASCAL CSPString::SafeStrlen(LPCTSTR lpsz)
	{ return (lpsz == NULL) ? 0 : lstrlen(lpsz); }

_SPSTRING_INLINE int CSPString::Compare(const char* psz) const
	{ return _tcscmp(m_pchData, psz); }
_SPSTRING_INLINE int CSPString::CompareNoCase(const char* psz) const
	{ return _tcsicmp(m_pchData, psz); }
_SPSTRING_INLINE int CSPString::Collate(const char* psz) const
	{ return _tcscoll(m_pchData, psz); }

_SPSTRING_INLINE TCHAR CSPString::GetAt(int nIndex) const
{
	SP_ASSERT(nIndex >= 0);
	SP_ASSERT(nIndex < GetData()->nDataLength);
	return m_pchData[nIndex];
}
_SPSTRING_INLINE TCHAR CSPString::operator[](int nIndex) const
{
	// same as GetAt
	SP_ASSERT(nIndex >= 0);
	SP_ASSERT(nIndex < GetData()->nDataLength);
	return m_pchData[nIndex];
}
_SPSTRING_INLINE bool __stdcall operator==(const CSPString& s1, const CSPString& s2)
	{ return s1.Compare(s2) == 0; }
_SPSTRING_INLINE bool __stdcall operator==(const CSPString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) == 0; }
_SPSTRING_INLINE bool __stdcall operator==(LPCTSTR s1, const CSPString& s2)
	{ return s2.Compare(s1) == 0; }
_SPSTRING_INLINE bool __stdcall operator!=(const CSPString& s1, const CSPString& s2)
	{ return s1.Compare(s2) != 0; }
_SPSTRING_INLINE bool __stdcall operator!=(const CSPString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) != 0; }
_SPSTRING_INLINE bool __stdcall operator!=(LPCTSTR s1, const CSPString& s2)
	{ return s2.Compare(s1) != 0; }
_SPSTRING_INLINE bool __stdcall operator<(const CSPString& s1, const CSPString& s2)
	{ return s1.Compare(s2) < 0; }
_SPSTRING_INLINE bool __stdcall operator<(const CSPString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) < 0; }
_SPSTRING_INLINE bool __stdcall operator<(LPCTSTR s1, const CSPString& s2)
	{ return s2.Compare(s1) > 0; }
_SPSTRING_INLINE bool __stdcall operator>(const CSPString& s1, const CSPString& s2)
	{ return s1.Compare(s2) > 0; }
_SPSTRING_INLINE bool __stdcall operator>(const CSPString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) > 0; }
_SPSTRING_INLINE bool __stdcall operator>(LPCTSTR s1, const CSPString& s2)
	{ return s2.Compare(s1) < 0; }
_SPSTRING_INLINE bool __stdcall operator<=(const CSPString& s1, const CSPString& s2)
	{ return s1.Compare(s2) <= 0; }
_SPSTRING_INLINE bool __stdcall operator<=(const CSPString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) <= 0; }
_SPSTRING_INLINE bool __stdcall operator<=(LPCTSTR s1, const CSPString& s2)
	{ return s2.Compare(s1) >= 0; }
_SPSTRING_INLINE bool __stdcall operator>=(const CSPString& s1, const CSPString& s2)
	{ return s1.Compare(s2) >= 0; }
_SPSTRING_INLINE bool __stdcall operator>=(const CSPString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) >= 0; }
_SPSTRING_INLINE bool __stdcall operator>=(LPCTSTR s1, const CSPString& s2)
	{ return s2.Compare(s1) <= 0; }
