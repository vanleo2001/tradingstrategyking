/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CSPArchive;
				like CArchive
*/

#if	!defined( __SP_ARCHIVE_H__)
#define	__SP_ARCHIVE_H__

class STKLIB_API CSPArchive
{
public:
// Flag values
	enum Mode { store = 0, load = 1, bNoFlushOnDelete = 2, bNoByteSwap = 4 };

	CSPArchive(CSPFile* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = NULL);
	~CSPArchive();

// Attributes
	BOOL IsLoading() const;
	BOOL IsStoring() const;
	BOOL IsByteSwapping() const;
	BOOL IsBufferEmpty() const;

	CSPFile* GetFile() const;

// Operations
	UINT Read(void* lpBuf, UINT nMax);
	void Write(const void* lpBuf, UINT nMax);
	void Flush();
	void Close();
	void Abort();   // close and shutdown without exceptions

	// reading and writing strings
	void WriteString(LPCTSTR lpsz);
	LPTSTR ReadString(LPTSTR lpsz, UINT nMax);
	BOOL ReadString(CSPString& rString);

public:
	// insertion operations
	CSPArchive& operator<<(BYTE by);
	CSPArchive& operator<<(WORD w);
	CSPArchive& operator<<(LONG l);
	CSPArchive& operator<<(DWORD dw);
	CSPArchive& operator<<(float f);
	CSPArchive& operator<<(double d);

	CSPArchive& operator<<(int i);
	CSPArchive& operator<<(short w);
	CSPArchive& operator<<(char ch);
	CSPArchive& operator<<(unsigned u);

	// extraction operations
	CSPArchive& operator>>(BYTE& by);
	CSPArchive& operator>>(WORD& w);
	CSPArchive& operator>>(DWORD& dw);
	CSPArchive& operator>>(LONG& l);
	CSPArchive& operator>>(float& f);
	CSPArchive& operator>>(double& d);

	CSPArchive& operator>>(int& i);
	CSPArchive& operator>>(short& w);
	CSPArchive& operator>>(char& ch);
	CSPArchive& operator>>(unsigned& u);

	CSPArchive& operator<<(const CSPTime& tm);
	CSPArchive& operator>>(CSPTime& tm);

	friend CSPArchive& operator<<(CSPArchive& ar, CSPString& string);
	friend CSPArchive& operator>>(CSPArchive& ar, CSPString& string);

// Implementation
public:
	BOOL m_bForceFlat;  // for COleClientItem implementation (default TRUE)
	BOOL m_bDirectBuffer;   // TRUE if m_pFile supports direct buffering
	void FillBuffer(UINT nBytesNeeded);
	void CheckCount();  // throw exception if m_nMapCount is too large

	// special functions for reading and writing (16-bit compatible) counts
	DWORD ReadCount();
	void WriteCount(DWORD dwCount);

protected:
	// archive objects cannot be copied or assigned
	CSPArchive(const CSPArchive& arSrc);
	void operator=(const CSPArchive& arSrc);

	BOOL m_nMode;
	BOOL m_bUserBuf;
	int m_nBufSize;
	CSPFile* m_pFile;
	BYTE* m_lpBufCur;
	BYTE* m_lpBufMax;
	BYTE* m_lpBufStart;
};


#ifdef	_SP_ENABLE_INLINES
#define	_SPARCHIVE_INLINE	inline
#include "SpArchive.inl"
#undef	_SPARCHIVE_INLINE
#endif

#endif //__SP_ARCHIVE_H__
