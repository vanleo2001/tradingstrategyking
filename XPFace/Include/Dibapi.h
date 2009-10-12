// dibapi.h
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1997 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#ifndef _STKUI_DIBAPI__
#define _STKUI_DIBAPI__

/* Handle to a DIB */
DECLARE_HANDLE(HDIB);

/* DIB constants */
#define PALVERSION   0x300

/* DIB Macros*/

#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)

// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits"
// parameter is the bit count for the scanline (biWidth * biBitCount),
// and this macro returns the number of DWORD-aligned bytes needed
// to hold those bits.

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

/* Function prototypes */
BOOL      CLASS_EXPORT  PaintDIB (HDC, LPRECT, HDIB, LPRECT, CPalette* pPal);
BOOL      CLASS_EXPORT  CreateDIBPalette(HDIB hDIB, CPalette* cPal);
LPSTR     CLASS_EXPORT  FindDIBBits (LPSTR lpbi);
DWORD     CLASS_EXPORT  DIBWidth (LPSTR lpDIB);
DWORD     CLASS_EXPORT  DIBHeight (LPSTR lpDIB);
WORD      CLASS_EXPORT  PaletteSize (LPSTR lpbi);
WORD      CLASS_EXPORT  DIBNumColors (LPSTR lpbi);
HGLOBAL   CLASS_EXPORT  CopyHandle (HGLOBAL h);
BOOL      CLASS_EXPORT  SaveDIB (HDIB hDib, CFile& file);
HDIB      CLASS_EXPORT  ReadDIBFile(CFile& file);

HGLOBAL	CLASS_EXPORT LoadDIBResource( HMODULE hModule, UINT nResource );
HGLOBAL	CLASS_EXPORT LoadDIBResource( HMODULE hModule, LPCTSTR );

void CLASS_EXPORT DrawTransparentBitmap(HDC hdc,	HBITMAP hBitmap, int xStart, int yStart, 
							int xBitmap,	int yBitmap, int xWidth, int yHeight,
							COLORREF cTransparentColor);

#endif //!_STKUI_DIBAPI__
