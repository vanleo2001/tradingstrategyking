// CSPArchive
_SPARCHIVE_INLINE BOOL CSPArchive::IsLoading() const
	{ return (m_nMode & CSPArchive::load) != 0; }
_SPARCHIVE_INLINE BOOL CSPArchive::IsStoring() const
	{ return (m_nMode & CSPArchive::load) == 0; }
_SPARCHIVE_INLINE BOOL CSPArchive::IsByteSwapping() const
	{ return (m_nMode & CSPArchive::bNoByteSwap) == 0; }
_SPARCHIVE_INLINE BOOL CSPArchive::IsBufferEmpty() const
	{ return m_lpBufCur == m_lpBufMax; }
_SPARCHIVE_INLINE CSPFile* CSPArchive::GetFile() const
	{ return m_pFile; }

_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator<<(int i)
	{ return CSPArchive::operator<<((LONG)i); }
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator<<(unsigned u)
	{ return CSPArchive::operator<<((LONG)u); }
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator<<(short w)
	{ return CSPArchive::operator<<((WORD)w); }
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator<<(char ch)
	{ return CSPArchive::operator<<((BYTE)ch); }
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator<<(BYTE by)
	{ if (m_lpBufCur + sizeof(BYTE) > m_lpBufMax) Flush();
		*(UNALIGNED BYTE*)m_lpBufCur = by; m_lpBufCur += sizeof(BYTE); return *this; }
#ifndef _AFX_BYTESWAP
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator<<(WORD w)
	{ if (m_lpBufCur + sizeof(WORD) > m_lpBufMax) Flush();
		*(UNALIGNED WORD*)m_lpBufCur = w; m_lpBufCur += sizeof(WORD); return *this; }
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator<<(LONG l)
	{ if (m_lpBufCur + sizeof(LONG) > m_lpBufMax) Flush();
		*(UNALIGNED LONG*)m_lpBufCur = l; m_lpBufCur += sizeof(LONG); return *this; }
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator<<(DWORD dw)
	{ if (m_lpBufCur + sizeof(DWORD) > m_lpBufMax) Flush();
		*(UNALIGNED DWORD*)m_lpBufCur = dw; m_lpBufCur += sizeof(DWORD); return *this; }
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator<<(float f)
	{ if (m_lpBufCur + sizeof(float) > m_lpBufMax) Flush();
		*(UNALIGNED float*)m_lpBufCur = *(float*)&f; m_lpBufCur += sizeof(float); return *this;
	}
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator<<(double d)
	{ if (m_lpBufCur + sizeof(double) > m_lpBufMax) Flush();
		*(UNALIGNED double*)m_lpBufCur = *(double*)&d; m_lpBufCur += sizeof(double); return *this; }
#endif
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator>>(int& i)
	{ return CSPArchive::operator>>((LONG&)i); }
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator>>(unsigned& u)
	{ return CSPArchive::operator>>((LONG&)u); }
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator>>(short& w)
	{ return CSPArchive::operator>>((WORD&)w); }
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator>>(char& ch)
	{ return CSPArchive::operator>>((BYTE&)ch); }
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator>>(BYTE& by)
	{ if (m_lpBufCur + sizeof(BYTE) > m_lpBufMax)
			FillBuffer(sizeof(BYTE) - (UINT)(m_lpBufMax - m_lpBufCur));
		by = *(UNALIGNED BYTE*)m_lpBufCur; m_lpBufCur += sizeof(BYTE); return *this; }
#ifndef _AFX_BYTESWAP
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator>>(WORD& w)
	{ if (m_lpBufCur + sizeof(WORD) > m_lpBufMax)
			FillBuffer(sizeof(WORD) - (UINT)(m_lpBufMax - m_lpBufCur));
		w = *(UNALIGNED WORD*)m_lpBufCur; m_lpBufCur += sizeof(WORD); return *this; }
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator>>(DWORD& dw)
	{ if (m_lpBufCur + sizeof(DWORD) > m_lpBufMax)
			FillBuffer(sizeof(DWORD) - (UINT)(m_lpBufMax - m_lpBufCur));
		dw = *(UNALIGNED DWORD*)m_lpBufCur; m_lpBufCur += sizeof(DWORD); return *this; }
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator>>(float& f)
	{ if (m_lpBufCur + sizeof(float) > m_lpBufMax)
			FillBuffer(sizeof(float) - (UINT)(m_lpBufMax - m_lpBufCur));
		*(float*)&f = *(UNALIGNED float*)m_lpBufCur; m_lpBufCur += sizeof(float); return *this; }
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator>>(double& d)
	{ if (m_lpBufCur + sizeof(double) > m_lpBufMax)
			FillBuffer(sizeof(double) - (UINT)(m_lpBufMax - m_lpBufCur));
		*(double*)&d = *(UNALIGNED double*)m_lpBufCur; m_lpBufCur += sizeof(double); return *this; }
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator>>(LONG& l)
	{ if (m_lpBufCur + sizeof(LONG) > m_lpBufMax)
			FillBuffer(sizeof(LONG) - (UINT)(m_lpBufMax - m_lpBufCur));
		l = *(UNALIGNED LONG*)m_lpBufCur; m_lpBufCur += sizeof(LONG); return *this; }
#endif

_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator<<(const CSPTime& tm)
{
	*this << (long)tm.GetTime();
	return *this;
}
_SPARCHIVE_INLINE CSPArchive& CSPArchive::operator>>(CSPTime& tm)
{
	time_t tmt;
	//*this >> tmt;
	//tm = CSPTime(tmt);
	return *this;
}

_SPARCHIVE_INLINE CSPArchive::CSPArchive(const CSPArchive& /* arSrc */)
	{ }
_SPARCHIVE_INLINE void CSPArchive::operator=(const CSPArchive& /* arSrc */)
	{ }
