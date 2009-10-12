/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CSPFile;
*/

#include	"StdAfx.h"
#include	"SpFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
//	CSPFile
CSPFile::CSPFile()
{
	m_hFile = (UINT) hFileNull;
	m_bCloseOnDelete = FALSE;
}

CSPFile::~CSPFile()
{
	if (m_hFile != (UINT)hFileNull && m_bCloseOnDelete)
		Close();
}

BOOL CSPFile::Open(LPCTSTR lpszFileName, UINT nOpenFlags)
{
	// if already opened, return FALSE.
	if( (UINT)hFileNull != m_hFile )
	{
		SP_TRACE0( "hFileNull != m_hFile" );
		SetLastError( errCSPFile_OpenFile );
		return	FALSE;
	}

	SP_ASSERT_VALID(this);
	SP_ASSERT(SP_IsValidString(lpszFileName));
	SP_ASSERT((nOpenFlags & typeText) == 0);   // text mode not supported

	// CFile objects are always binary and CreateFile does not need flag
	nOpenFlags &= ~(UINT)typeBinary;

	m_bCloseOnDelete = FALSE;
	m_hFile = (UINT)hFileNull;

	SP_ASSERT((modeRead|modeWrite|modeReadWrite) == 3);
	int	nAccess = 0;
	switch (nOpenFlags & 3)
	{
	case modeRead:
		nAccess = O_RDONLY;
		break;
	case modeWrite:
		nAccess = O_WRONLY;
		break;
	case modeReadWrite:
		nAccess = O_RDWR;
		break;
	default:
		SP_ASSERT(FALSE);  // invalid access mode
	}

	// no share mode
	SP_ASSERT( (nOpenFlags & 0x30) == 0 );
	// no inherit.
	SP_ASSERT( (nOpenFlags & 0x80) == 0 );
	
	// map create mode.
	if (nOpenFlags & modeCreate)
	{
		nAccess	|=	O_CREAT;
		if (nOpenFlags & modeNoTruncate)
			;
		else
			nAccess	|=	O_TRUNC;
	}
	
	nAccess	|=	O_BINARY;

	UINT	hFile	=	hFileNull;
	hFile	=	open( lpszFileName, nAccess, S_IREAD | S_IWRITE );

	if( hFileNull == hFile )
		return	FALSE;
	
	m_hFile = (HFILE)hFile;
	m_bCloseOnDelete	=	TRUE;
	m_strFileName		=	lpszFileName;

	return TRUE;
}

BOOL CSPFile::CopyFile( LPCTSTR lpExistingFileName,
					   LPCTSTR lpNewFileName, BOOL bFailIfExists )
{
	return ::CopyFile( lpExistingFileName, lpNewFileName, bFailIfExists );
}

UINT CSPFile::Read(void* lpBuf, UINT nCount)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(m_hFile != (UINT)hFileNull);

	if (nCount == 0)
		return 0;   // avoid Win32 "null-read"

	SP_ASSERT(lpBuf != NULL);
	SP_ASSERT(SP_IsValidAddress(lpBuf, nCount));

	int	nRet	=	read( m_hFile, lpBuf, nCount );
	if( nCount != (UINT)nRet )
	{
		SetLastError( errCFileSystem_ReadFile );
		//SP_ASSERT( FALSE );
	}

	return	nRet;
}

void CSPFile::Write(const void* lpBuf, UINT nCount)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(m_hFile != (UINT)hFileNull);

	if (nCount == 0)
		return;     // avoid Win32 "null-write" option

	SP_ASSERT(lpBuf != NULL);
	SP_ASSERT(SP_IsValidAddress((void *)lpBuf, nCount, FALSE));

	int	nRet	=	write( m_hFile, lpBuf, nCount );
	if( nCount != (UINT)nRet )
	{
		SetLastError( errCFileSystem_WriteFile );
		SP_ASSERT( FALSE );
	}
}

void CSPFile::WriteString(LPCTSTR lpsz)
{
	SP_ASSERT(SP_IsValidString(lpsz));
	Write(lpsz, lstrlen(lpsz) * sizeof(TCHAR));
}

LPTSTR CSPFile::ReadString(LPTSTR lpsz, UINT nMax)
{
	// if nMax is negative (such a large number doesn't make sense given today's
	// 2gb address space), then assume it to mean "keep the newline".
	int nStop = (int)nMax < 0 ? -(int)nMax : (int)nMax;
	SP_ASSERT(SP_IsValidAddress(lpsz, (nStop+1) * sizeof(TCHAR)));

	TCHAR ch;
	int nRead = 0;

	lpsz[nRead] = '\0';
	while (nRead < nStop)
	{
		if( sizeof(TCHAR) != Read( &ch, sizeof(TCHAR) ) )
		{
			if( 0 == nRead )
				return NULL;
		}

		// stop and end-of-line (trailing '\n' is ignored)
		if (ch == '\r')
			continue;

		if (ch == '\n')
		{
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

BOOL CSPFile::ReadString( CSPString & rString )
{
	rString.Empty();
	const int nMaxSize = 256;
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

LONG CSPFile::Seek(LONG lOff, UINT nFrom)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(m_hFile != (UINT)hFileNull);
	
	SP_ASSERT(nFrom == begin || nFrom == end || nFrom == current);
#ifdef	SP_WIN32	
	SP_ASSERT(begin == FILE_BEGIN && end == FILE_END && current == FILE_CURRENT);
#endif

	UINT	nOrgin	=	-1;
	if( nFrom == begin )
		nOrgin	=	SEEK_SET;
	if( nFrom == end )
		nOrgin	=	SEEK_END;
	if( nFrom == current )
		nOrgin	=	SEEK_CUR;
	
	SP_ASSERT(	SEEK_SET == nOrgin || SEEK_CUR == nOrgin ||	SEEK_END == nOrgin );
	if( nOrgin == -1 )
	{
		SetLastError( errCSPFile_SeekFile );
		return -1;
	}

	LONG lnRet	=	lseek( m_hFile, lOff, nOrgin );
	if( -1 == lnRet )
		SetLastError( errCSPFile_SeekFile );

	return	lnRet;
}

void CSPFile::Flush()
{
	SP_ASSERT_VALID(this);

	if (m_hFile == (UINT)hFileNull)
		return;
	
	_commit( m_hFile );
}

void CSPFile::Close()
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(m_hFile != (UINT)hFileNull);

	if( hFileNull != m_hFile )
	{
		if( -1 == close( m_hFile ) )
		{
			SetLastError( errCSPFile_CloseFile );
			SP_ASSERT( FALSE );
		}
		m_hFile	=	hFileNull;
	}

	m_hFile = (UINT) hFileNull;
	m_bCloseOnDelete = FALSE;
	m_strFileName.Empty();
}

void CSPFile::Abort()
{
	SP_ASSERT_VALID(this);
	if (m_hFile != (UINT)hFileNull)
	{
		// close but ignore errors
		close( m_hFile );
		m_hFile = (UINT)hFileNull;
	}
	m_strFileName.Empty();
}

void CSPFile::SeekToBegin()
{
	Seek( 0, begin );
}

DWORD CSPFile::SeekToEnd()
{
	return Seek( 0, end );
}

DWORD CSPFile::GetLength() const
{
	SP_ASSERT_VALID(this);

	DWORD dwLen, dwCur;

	// Seek is a non const operation
	CSPFile	*pFile	=	(CSPFile*)this;
	dwCur = pFile->Seek(0L, current);
	dwLen = pFile->SeekToEnd();
	SP_VERIFY(dwCur == (DWORD)pFile->Seek(dwCur, begin));

	return dwLen;
}

DWORD CSPFile::GetPosition() const
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(m_hFile != (UINT)hFileNull);

	DWORD dwPos = lseek( m_hFile, 0, SEEK_CUR );
	return dwPos;
}

#ifdef	_DEBUG
void CSPFile::Dump() const
{
	Object::Dump( );
}

void CSPFile::AssertValid() const
{
	Object::AssertValid();
	if( m_hFile == UINT(hFileNull) )
	{
		SP_ASSERT(m_strFileName.IsEmpty());
	}
}
#endif	//	_DEBUG

// CSPFile does not support direct buffering (CMemFile does)
UINT CSPFile::GetBufferPtr(UINT nCommand, UINT /*nCount*/,
	void** /*ppBufStart*/, void** /*ppBufMax*/)
{
	SP_ASSERT(nCommand == bufferCheck);
	UNUSED(nCommand);    // not used in retail build

	return 0;   // no support
}
