/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CSPArchive;
*/
#include	"StdAfx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif

#ifndef	_SP_ENABLE_INLINES
#define	_SPARCHIVE_INLINE
#include "../Include/SpArchive.inl"
#undef	_SPARCHIVE_INLINE
#endif

////////////////////////////////////////////////////////////////////////////
// Archive object input/output

// minimum buffer size
enum { nBufSizeMin = 128 };

// default amount to grow m_pLoadArray upon insert
enum { nGrowSize = 64 };
// default size of hash table in m_pStoreMap when storing
enum { nHashSize = 137 };
// default size to grow collision blocks when storing
enum { nBlockSize = 16 };

////////////////////////////////////////////////////////////////////////////

CSPArchive::CSPArchive(CSPFile* pFile, UINT nMode, int nBufSize, void* lpBuf)
{
	SP_ASSERT_VALID(pFile);

	// initialize members not dependent on allocated buffer
	m_nMode = nMode;
	m_pFile = pFile;
	m_bForceFlat = TRUE;

	// initialize the buffer.  minimum size is 128
	m_lpBufStart = (BYTE*)lpBuf;
	m_bUserBuf = TRUE;
	m_bDirectBuffer = FALSE;

	if (nBufSize < nBufSizeMin)
	{
		// force use of private buffer of minimum size
		m_nBufSize = nBufSizeMin;
		m_lpBufStart = NULL;
	}
	else
		m_nBufSize = nBufSize;

	nBufSize = m_nBufSize;
	if (m_lpBufStart == NULL)
	{
		// check for CSPFile providing buffering support
		m_bDirectBuffer = m_pFile->GetBufferPtr(CSPFile::bufferCheck);
		if (!m_bDirectBuffer)
		{
			// no support for direct buffering, allocate new buffer
			m_lpBufStart = new BYTE[m_nBufSize];
			m_bUserBuf = FALSE;
		}
		else
		{
			// CSPFile* supports direct buffering!
			nBufSize = 0;   // will trigger initial FillBuffer
		}
	}

	if (!m_bDirectBuffer)
	{
		SP_ASSERT(m_lpBufStart != NULL);
		SP_ASSERT(SP_IsValidAddress(m_lpBufStart, nBufSize, IsStoring()));
	}
	m_lpBufMax = m_lpBufStart + nBufSize;
	m_lpBufCur = (IsLoading()) ? m_lpBufMax : m_lpBufStart;
}

CSPArchive::~CSPArchive()
{
	// Close makes m_pFile NULL. If it is not NULL, we must Close the CSPArchive
	if (m_pFile != NULL && !(m_nMode & bNoFlushOnDelete))
		Close();

	Abort();    // abort completely shuts down the archive
}

void CSPArchive::Abort()
{
	SP_ASSERT(m_bDirectBuffer || m_lpBufStart == NULL ||
		SP_IsValidAddress(m_lpBufStart, m_lpBufMax - m_lpBufStart, IsStoring()));
	SP_ASSERT(m_bDirectBuffer || m_lpBufCur == NULL ||
		SP_IsValidAddress(m_lpBufCur, m_lpBufMax - m_lpBufCur, IsStoring()));

	// disconnect from the file
	m_pFile = NULL;

	if (!m_bUserBuf)
	{
		SP_ASSERT(!m_bDirectBuffer);
		delete[] m_lpBufStart;
		m_lpBufStart = NULL;
		m_lpBufCur = NULL;
	}
}

void CSPArchive::Close()
{
	SP_ASSERT_VALID(m_pFile);

	Flush();
	m_pFile = NULL;
}

UINT CSPArchive::Read(void* lpBuf, UINT nMax)
{
	SP_ASSERT_VALID(m_pFile);

	if (nMax == 0)
		return 0;

	SP_ASSERT(lpBuf != NULL);
	SP_ASSERT(SP_IsValidAddress(lpBuf, nMax));
	SP_ASSERT(m_bDirectBuffer || m_lpBufStart != NULL);
	SP_ASSERT(m_bDirectBuffer || m_lpBufCur != NULL);
	SP_ASSERT(m_lpBufStart == NULL ||
		SP_IsValidAddress(m_lpBufStart, m_lpBufMax - m_lpBufStart, FALSE));
	SP_ASSERT(m_lpBufCur == NULL ||
		SP_IsValidAddress(m_lpBufCur, m_lpBufMax - m_lpBufCur, FALSE));
	SP_ASSERT(IsLoading());

	// try to fill from buffer first
	UINT nMaxTemp = nMax;
	UINT nTemp = min(nMaxTemp, (UINT)(m_lpBufMax - m_lpBufCur));
	memcpy(lpBuf, m_lpBufCur, nTemp);
	m_lpBufCur += nTemp;
	lpBuf = (BYTE*)lpBuf + nTemp;
	nMaxTemp -= nTemp;

	if (nMaxTemp != 0)
	{
		SP_ASSERT(m_lpBufCur == m_lpBufMax);

		// read rest in buffer size chunks
		nTemp = nMaxTemp - (nMaxTemp % m_nBufSize);
		UINT nRead = 0;

		UINT nLeft = nTemp;
		UINT nBytes;
		do
		{
			nBytes = m_pFile->Read(lpBuf, nLeft);
			lpBuf = (BYTE*)lpBuf + nBytes;
			nRead += nBytes;
			nLeft -= nBytes;
		}
		while ((nBytes > 0) && (nLeft > 0));

		nMaxTemp -= nRead;

		// read last chunk into buffer then copy
		if (nRead == nTemp)
		{
			SP_ASSERT(m_lpBufCur == m_lpBufMax);
			SP_ASSERT(nMaxTemp < (UINT)m_nBufSize);

			// fill buffer (similar to CSPArchive::FillBuffer, but no exception)
			if (!m_bDirectBuffer)
			{
				UINT nLeft = max(nMaxTemp, (UINT)m_nBufSize);
				UINT nBytes;
				BYTE* lpTemp = m_lpBufStart;
				nRead = 0;
				do
				{
					nBytes = m_pFile->Read(lpTemp, nLeft);
					lpTemp = lpTemp + nBytes;
					nRead += nBytes;
					nLeft -= nBytes;
				}
				while ((nBytes > 0) && (nLeft > 0) && nRead < nMaxTemp);

				m_lpBufCur = m_lpBufStart;
				m_lpBufMax = m_lpBufStart + nRead;
			}
			else
			{
				nRead = m_pFile->GetBufferPtr(CSPFile::bufferRead, m_nBufSize,
					(void**)&m_lpBufStart, (void**)&m_lpBufMax);
				SP_ASSERT(nRead == (UINT)(m_lpBufMax - m_lpBufStart));
				m_lpBufCur = m_lpBufStart;
			}

			// use first part for rest of read
			nTemp = min(nMaxTemp, (UINT)(m_lpBufMax - m_lpBufCur));
			memcpy(lpBuf, m_lpBufCur, nTemp);
			m_lpBufCur += nTemp;
			nMaxTemp -= nTemp;
		}
	}
	return nMax - nMaxTemp;
}

void CSPArchive::Write(const void* lpBuf, UINT nMax)
{
	SP_ASSERT_VALID(m_pFile);

	if (nMax == 0)
		return;

	SP_ASSERT(lpBuf != NULL);
	SP_ASSERT(SP_IsValidAddress((void*)lpBuf, nMax, FALSE));  // read-only access needed
	SP_ASSERT(m_bDirectBuffer || m_lpBufStart != NULL);
	SP_ASSERT(m_bDirectBuffer || m_lpBufCur != NULL);
	SP_ASSERT(m_lpBufStart == NULL ||
		SP_IsValidAddress(m_lpBufStart, m_lpBufMax - m_lpBufStart));
	SP_ASSERT(m_lpBufCur == NULL ||
		SP_IsValidAddress(m_lpBufCur, m_lpBufMax - m_lpBufCur));
	SP_ASSERT(IsStoring());

	// copy to buffer if possible
	UINT nTemp = min(nMax, (UINT)(m_lpBufMax - m_lpBufCur));
	memcpy(m_lpBufCur, lpBuf, nTemp);
	m_lpBufCur += nTemp;
	lpBuf = (BYTE*)lpBuf + nTemp;
	nMax -= nTemp;

	if (nMax > 0)
	{
		Flush();    // flush the full buffer

		// write rest of buffer size chunks
		nTemp = nMax - (nMax % m_nBufSize);
		m_pFile->Write(lpBuf, nTemp);
		lpBuf = (BYTE*)lpBuf + nTemp;
		nMax -= nTemp;

		if (m_bDirectBuffer)
		{
			// sync up direct mode buffer to new file position
			SP_VERIFY(m_pFile->GetBufferPtr(CSPFile::bufferWrite, m_nBufSize,
				(void**)&m_lpBufStart, (void**)&m_lpBufMax) == (UINT)m_nBufSize);
			SP_ASSERT((UINT)m_nBufSize == (UINT)(m_lpBufMax - m_lpBufStart));
			m_lpBufCur = m_lpBufStart;
		}

		// copy remaining to active buffer
		SP_ASSERT(nMax < (UINT)m_nBufSize);
		SP_ASSERT(m_lpBufCur == m_lpBufStart);
		memcpy(m_lpBufCur, lpBuf, nMax);
		m_lpBufCur += nMax;
	}
}

void CSPArchive::Flush()
{
	SP_ASSERT_VALID(m_pFile);
	SP_ASSERT(m_bDirectBuffer || m_lpBufStart != NULL);
	SP_ASSERT(m_bDirectBuffer || m_lpBufCur != NULL);
	SP_ASSERT(m_lpBufStart == NULL ||
		SP_IsValidAddress(m_lpBufStart, m_lpBufMax - m_lpBufStart, IsStoring()));
	SP_ASSERT(m_lpBufCur == NULL ||
		SP_IsValidAddress(m_lpBufCur, m_lpBufMax - m_lpBufCur, IsStoring()));

	if (IsLoading())
	{
		// unget the characters in the buffer, seek back unused amount
		if (m_lpBufMax != m_lpBufCur)
			m_pFile->Seek(-(m_lpBufMax - m_lpBufCur), CSPFile::current);
		m_lpBufCur = m_lpBufMax;    // empty
	}
	else
	{
		if (!m_bDirectBuffer)
		{
			// write out the current buffer to file
			if (m_lpBufCur != m_lpBufStart)
				m_pFile->Write(m_lpBufStart, m_lpBufCur - m_lpBufStart);
		}
		else
		{
			// commit current buffer
			if (m_lpBufCur != m_lpBufStart)
				m_pFile->GetBufferPtr(CSPFile::bufferCommit, m_lpBufCur - m_lpBufStart);
			// get next buffer
			SP_VERIFY(m_pFile->GetBufferPtr(CSPFile::bufferWrite, m_nBufSize,
				(void**)&m_lpBufStart, (void**)&m_lpBufMax) == (UINT)m_nBufSize);
			SP_ASSERT((UINT)m_nBufSize == (UINT)(m_lpBufMax - m_lpBufStart));
		}
		m_lpBufCur = m_lpBufStart;
	}
}

void CSPArchive::FillBuffer(UINT nBytesNeeded)
{
	SP_ASSERT_VALID(m_pFile);
	SP_ASSERT(IsLoading());
	SP_ASSERT(m_bDirectBuffer || m_lpBufStart != NULL);
	SP_ASSERT(m_bDirectBuffer || m_lpBufCur != NULL);
	SP_ASSERT(nBytesNeeded > 0);
	SP_ASSERT(nBytesNeeded <= (UINT)m_nBufSize);
	SP_ASSERT(m_lpBufStart == NULL ||
		SP_IsValidAddress(m_lpBufStart, m_lpBufMax - m_lpBufStart, FALSE));
	SP_ASSERT(m_lpBufCur == NULL ||
		SP_IsValidAddress(m_lpBufCur, m_lpBufMax - m_lpBufCur, FALSE));

	UINT nUnused = m_lpBufMax - m_lpBufCur;
	ULONG nTotalNeeded = ((ULONG)nBytesNeeded) + nUnused;

	// fill up the current buffer from file
	if (!m_bDirectBuffer)
	{
		SP_ASSERT(m_lpBufCur != NULL);
		SP_ASSERT(m_lpBufStart != NULL);
		SP_ASSERT(m_lpBufMax != NULL);

		if (m_lpBufCur > m_lpBufStart)
		{
			// copy unused
			if ((int)nUnused > 0)
			{
				memmove(m_lpBufStart, m_lpBufCur, nUnused);
				m_lpBufCur = m_lpBufStart;
				m_lpBufMax = m_lpBufStart + nUnused;
			}

			// read to satisfy nBytesNeeded or nLeft if possible
			UINT nRead = nUnused;
			UINT nLeft = m_nBufSize-nUnused;
			UINT nBytes;
			BYTE* lpTemp = m_lpBufStart + nUnused;
			do
			{
				nBytes = m_pFile->Read(lpTemp, nLeft);
				lpTemp = lpTemp + nBytes;
				nRead += nBytes;
				nLeft -= nBytes;
			}
			while (nBytes > 0 && nLeft > 0 && nRead < nBytesNeeded);

			m_lpBufCur = m_lpBufStart;
			m_lpBufMax = m_lpBufStart + nRead;
		}
	}
	else
	{
		// seek to unused portion and get the buffer starting there
		if (nUnused != 0)
			m_pFile->Seek(-(LONG)nUnused, CSPFile::current);
		UINT nActual = m_pFile->GetBufferPtr(CSPFile::bufferRead, m_nBufSize,
			(void**)&m_lpBufStart, (void**)&m_lpBufMax);
		SP_ASSERT(nActual == (UINT)(m_lpBufMax - m_lpBufStart));
		m_lpBufCur = m_lpBufStart;
	}
}

void CSPArchive::WriteCount(DWORD dwCount)
{
	if (dwCount < 0xFFFF)
		*this << (WORD)dwCount;
	else
	{
		*this << (WORD)0xFFFF;
		*this << dwCount;
	}
}

DWORD CSPArchive::ReadCount()
{
	WORD wCount;
	*this >> wCount;
	if (wCount != 0xFFFF)
		return wCount;

	DWORD dwCount;
	*this >> dwCount;
	return dwCount;
}

// special functions for text file input and output

void CSPArchive::WriteString(LPCTSTR lpsz)
{
	SP_ASSERT(SP_IsValidString(lpsz));
	Write(lpsz, lstrlen(lpsz) * sizeof(TCHAR));
}

LPTSTR CSPArchive::ReadString(LPTSTR lpsz, UINT nMax)
{
	// if nMax is negative (such a large number doesn't make sense given today's
	// 2gb address space), then assume it to mean "keep the newline".
	int nStop = (int)nMax < 0 ? -(int)nMax : (int)nMax;
	SP_ASSERT(SP_IsValidAddress(lpsz, (nStop+1) * sizeof(TCHAR)));

	TCHAR ch;
	int nRead = 0;

	while (nRead < nStop)
	{
		*this >> ch;

		// stop and end-of-line (trailing '\n' is ignored)
		if (ch == '\n' || ch == '\r')
		{
			if (ch == '\r')
				*this >> ch;
			// store the newline when called with negative nMax
			if ((int)nMax != nStop)
				lpsz[nRead++] = ch;
			break;
		}
		lpsz[nRead++] = ch;
	}

	lpsz[nRead] = '\0';
	return lpsz;
}

BOOL CSPArchive::ReadString(CSPString& rString)
{
	rString.Empty();
	const int nMaxSize = 128;
	LPTSTR lpsz = rString.GetBuffer(nMaxSize);
	LPTSTR lpszResult;
	int nLen;
	for (;;)
	{
		lpszResult = ReadString(lpsz, (UINT)-nMaxSize); // store the newline
		rString.ReleaseBuffer();

		// if string is read completely or EOF
		if (lpszResult == NULL ||
			(nLen = lstrlen(lpsz)) < nMaxSize ||
			lpsz[nLen-1] == '\n')
		{
			break;
		}

		nLen = rString.GetLength();
		lpsz = rString.GetBuffer(nMaxSize + nLen) + nLen;
	}

	// remove '\n' from end of string if present
	lpsz = rString.GetBuffer(0);
	nLen = rString.GetLength();
	if (nLen != 0 && lpsz[nLen-1] == '\n')
		rString.GetBufferSetLength(nLen-1);

	return lpszResult != NULL;
}

///////////////////////////////////////////////////////////////////////////////
// CString serialization code
// String format:
//      UNICODE strings are always prefixed by 0xff, 0xfffe
//      if < 0xff chars: len:BYTE, TCHAR chars
//      if >= 0xff characters: 0xff, len:WORD, TCHAR chars
//      if >= 0xfffe characters: 0xff, 0xffff, len:DWORD, TCHARs

CSPArchive& operator<<(CSPArchive& ar, CSPString& string)
{
	if (string.GetLength() < 255)
	{
		ar << (BYTE)string.GetLength();
	}
	else if (string.GetLength() < 0xfffe)
	{
		ar << (BYTE)0xff;
		ar << (WORD)string.GetLength();
	}
	else
	{
		ar << (BYTE)0xff;
		ar << (WORD)0xffff;
		ar << (DWORD)string.GetLength();
	}
	ar.Write(string.GetBuffer(string.GetLength()), string.GetLength()*sizeof(TCHAR));
	return ar;
}

// return string length or -1 if UNICODE string is found in the archive
UINT _AfxReadStringLength(CSPArchive& ar)
{
	DWORD nNewLen;

	// attempt BYTE length first
	BYTE bLen;
	ar >> bLen;

	if (bLen < 0xff)
		return bLen;

	// attempt WORD length
	WORD wLen;
	ar >> wLen;
	if (wLen == 0xfffe)
	{
		// UNICODE string prefix (length will follow)
		return (UINT)-1;
	}
	else if (wLen == 0xffff)
	{
		// read DWORD of length
		ar >> nNewLen;
		return (UINT)nNewLen;
	}
	else
		return wLen;
}

CSPArchive& operator>>(CSPArchive& ar, CSPString& string)
{
	UINT nNewLen = _AfxReadStringLength(ar);
	if (nNewLen == (UINT)-1)
		SP_ASSERT(FALSE);

	// set length of string to new length
	string.Empty();
	char * lpBuf = string.GetBufferSetLength((int)nNewLen);

	// read in the characters
	if (nNewLen != 0)
	{
		// read new data
		if (ar.Read(lpBuf, nNewLen) != nNewLen)
		{
			SP_ASSERT( FALSE );
			//AfxThrowArchiveException(CArchiveException::endOfFile);
			return ar;
		}
	}
	return ar;
}
