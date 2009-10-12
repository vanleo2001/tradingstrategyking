/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
*/

#ifndef	__SP_PLAT_H__
#define	__SP_PLAT_H__

#if defined(macintosh) || defined(__MWERKS__) || defined(applec)
#ifndef macintosh
#define macintosh 1
#endif
#endif

#if defined(__unix) || defined(unix) || defined(UNIX) || defined(SP_UNIX)
#ifndef unix
#define unix 1
#endif
#endif

#if !defined(macintosh) && !defined(_WINDOWS) && !defined(unix) && !defined(__MSDOS__)
#error spring library can't determine system type
#endif

#ifdef macintosh
	# ifndef SP_MAC
	# define SP_MAC 1
	# endif
	# define SP_IS_MAC 1
	# define SP_MAC_ARG(x) x
#else
	# define SP_IS_MAC 0
	# define SP_MAC_ARG(x)
#endif

#ifdef _WINDOWS
	# ifndef SP_WINDOWS
	# define SP_WINDOWS
	# endif
	# define SP_IS_WIN 1
	# define SP_WIN_ARG(x) x
	#if defined(_WIN32) || defined(WIN32)
		# ifndef SP_WIN32
		# define SP_WIN32
		# endif
	#else
		# ifndef SP_WIN16
		# define SP_WIN16
		# endif
	#endif
#else
	# define SP_IS_WIN 0
	# define SP_WIN_ARG(x)
#endif


#if defined(__MSDOS__) && !defined(SP_WINDOWS)
	#ifndef	SP_DOS
	#define	SP_DOS
	#endif
	#define	SP_IS_DOS	1
	#define	SP_DOS_ARG(x)	x
#else
	#define	SP_IS_DOS	0
	#define	SP_DOS_ARG(x)
#endif


#ifdef unix
	# ifndef SP_UNIX
	# define SP_UNIX
	# endif
	# define SP_IS_UNIX 1
	# define SP_UNIX_ARG(x) x
#else
	# define SP_IS_UNIX 0
	# define SP_UNIX_ARG(x)
#endif

/*  IBM-LTB  Setup system macros for OS/2   */
#if defined (__OS2__)
	# ifndef SP_OS2
	# define SP_OS2
	# endif
	# define SP_IS_OS2 1
	# define SP_OS2_ARG(x) x
#else
	# define SP_IS_OS2 0
	# define SP_OS2_ARG(x)
#endif

/* what language do we have? */

#if defined(__cplusplus)
	# define SP_CPLUSPLUS
	# define SP_IS_CPLUSPLUS 1
#else
	# define SP_IS_CPLUSPLUS 0
#endif

#endif //__SP_PLAT_H__