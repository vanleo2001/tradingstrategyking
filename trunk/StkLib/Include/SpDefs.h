/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		fix long data type:
			BYTE	--  1 byte
			WORD	--	2 bytes
			DWORD	--	4 bytes
		variable long data type:
			CHAR	--	char
			INT		--	int
			SHORT	--	short
			UINT	--	unsigned int
			LONG	--	long
*/

#ifndef	__SP_DEFS_H__
#define	__SP_DEFS_H__

#define	CHAR_EQ		char('=')
#define	CHAR_DOT	char('.')
#define	CHAR_ENTER	char('\n')
#define	CHAR_TAB	char('\t')
#define	CHAR_ESC	char(0x1B)
#define	CHAR_SPACE	char(0x20)
#define	CHAR_SZ		char(0x00)

typedef unsigned    char    BYTE;  /* 8-bits  0 to 255*/
typedef unsigned    short   WORD;  /* 16-bits 0 to 65,535*/
typedef unsigned    long    DWORD; /* 32-bits 0 to 4,294,967,295*/

#ifdef	FAR
	#define	_FAR	FAR
#else
	#define	_FAR
#endif

#define	CONST	const

/*
	Enable inline function while not use _DEBUG.
*/
#ifndef	_DEBUG
#define	_SP_ENABLE_INLINES
#else
//#define	_SP_ENABLE_INLINES
#endif

#define	SP_INLINE

#if	defined(SP_DOS)
#define	NEAR	near
#endif

#ifndef VOID
#define VOID			void
typedef	void _FAR*		LPVOID;
typedef	char			CHAR;
typedef	int				INT;
typedef	short			SHORT;
typedef	unsigned int	UINT;
typedef	long			LONG;
#endif

typedef	const void _FAR* LPCVOID;

#if	!defined(SP_WINDOWS)
	typedef	char*				PSTR;
	typedef char _FAR*			LPSTR;
	typedef CONST char _FAR*	LPCSTR;

	typedef	BYTE*				PBYTE;
	typedef	BYTE _FAR*			LPBYTE;

	typedef	LPCSTR				LPCTSTR;
	typedef	LPSTR				LPTSTR;

	#define	CHAR_DIRSEP			'/'
	#define	STRING_DIRSEP		"/"
	#define	STRING_CRLF			"\n"
#else
	#define	CHAR_DIRSEP			'\\'
	#define	STRING_DIRSEP		"\\"
	#define	STRING_CRLF			"\r\n"
#endif


#ifdef	_WINDOWS
	typedef	int		BOOL;
	#define FALSE   0
#ifndef	TRUE
	#define	TRUE	(!FALSE)
#endif
#endif // WIN32

#if defined(SP_DOS)
	typedef	int		BOOL;
	#define	FALSE	0
	#define	TRUE	1
#endif

#ifndef	SP_WIN32
	#define	false	0
	#define	true	1
#endif

#ifndef NOMINMAX
	#ifndef max
	#define max(a,b)            (((a) > (b)) ? (a) : (b))
	#endif

	#ifndef min
	#define min(a,b)            (((a) < (b)) ? (a) : (b))
	#endif
#endif  /* NOMINMAX */

#ifndef	MAKEWORD
	#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#endif
#ifndef	MAKELONG
	#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16
#endif

#ifndef	SP_WINDOWS
#define IDOK                1
#define IDCANCEL            2
#define IDABORT             3
#define IDRETRY             4
#define IDIGNORE            5
#define IDYES               6
#define IDNO                7

#define MB_OK                       0x00000000L
#define MB_OKCANCEL                 0x00000001L
#define MB_ABORTRETRYIGNORE         0x00000002L
#define MB_YESNOCANCEL              0x00000003L
#define MB_YESNO                    0x00000004L
#define MB_RETRYCANCEL              0x00000005L
#define MB_ICONHAND                 0x00000010L
#define MB_ICONQUESTION             0x00000020L
#define MB_APPLMODAL                0x00000000L
#define MB_SYSTEMMODAL              0x00001000L
#define MB_TASKMODAL                0x00002000L

#endif

#define	SP_GBK_HEADBIT		0x80

#endif //__SP_DEFS__
