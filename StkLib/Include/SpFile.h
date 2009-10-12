/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CSPFile
*/

#if	!defined( __SP_FILE_H__ )
#define	__SP_FILE_H__

#include	<sys\stat.h>
#include	<fcntl.h>


class CSPFile : public Object
{
public:
// Flag values
	enum OpenFlags {
		modeRead =          0x0000,
		modeWrite =         0x0001,
		modeReadWrite =     0x0002,
//		shareCompat =       0x0000,
//		shareExclusive =    0x0010,
//		shareDenyWrite =    0x0020,
//		shareDenyRead =     0x0030,
		shareDenyNone =     0x0040,
//		modeNoInherit =     0x0080,
		modeCreate =        0x1000,
		modeNoTruncate =    0x2000,
		typeText =          0x4000, // typeText and typeBinary are used in
		typeBinary =   (int)0x8000 // derived classes only
		};

	enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };

	enum { hFileNull = -1 };

// Constructors
	CSPFile();

// Attributes
	UINT m_hFile;

// Operations
	virtual BOOL Open(LPCSTR lpszFileName, UINT nOpenFlags);

	static	BOOL CopyFile( LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists );

	DWORD	SeekToEnd();
	void	SeekToBegin();
	DWORD	GetLength()	const;
	DWORD	GetPosition()	const;

// Overridables
	virtual LONG Seek(LONG lOff, UINT nFrom);

	virtual UINT Read(void* lpBuf, UINT nCount);
	virtual void Write(const void* lpBuf, UINT nCount);

	// reading and writing strings
	void WriteString(LPCTSTR lpsz);
	LPTSTR ReadString(LPTSTR lpsz, UINT nMax);
	BOOL ReadString( CSPString & rString );

	virtual void Abort();
	virtual void Flush();
	virtual void Close();

// Implementation
public:
	virtual ~CSPFile();
#ifdef _DEBUG
	void AssertValid() const;
	void Dump() const;
#endif
	enum BufferCommand { bufferRead, bufferWrite, bufferCommit, bufferCheck };
	virtual UINT GetBufferPtr(UINT nCommand, UINT nCount = 0,
		void** ppBufStart = NULL, void** ppBufMax = NULL);

protected:
	BOOL		m_bCloseOnDelete;
	CSPString	m_strFileName;
};

#endif	//__SP_FILE_H__
