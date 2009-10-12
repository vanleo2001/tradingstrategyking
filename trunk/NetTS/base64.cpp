/*
	B A S E 6 4. C P P

	Copyright (C) 2001 Balang SoftWare

	Version: 0.0.1
*/

#include "stdafx.h"
#include "base64.h"

///////////////////////////////////////////////////////////////////////////////////
/// The usefull base64 encoder and decoder

#define MAXLINE  76

/*============================================================================
 * Everything below this line is private to this file (static)
 *============================================================================
 */

static char base64tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz0123456789+/";

static char base64idx[128] = {
    '\377','\377','\377','\377','\377','\377','\377','\377',
    '\377','\377','\377','\377','\377','\377','\377','\377',
    '\377','\377','\377','\377','\377','\377','\377','\377',
    '\377','\377','\377','\377','\377','\377','\377','\377',
    '\377','\377','\377','\377','\377','\377','\377','\377',
    '\377','\377','\377',    62,'\377','\377','\377',    63,
        52,    53,    54,    55,    56,    57,    58,    59,
        60,    61,'\377','\377','\377','\377','\377','\377',
    '\377',     0,     1,     2,     3,     4,     5,     6,
         7,     8,     9,    10,    11,    12,    13,    14,
        15,    16,    17,    18,    19,    20,    21,    22,
        23,    24,    25,'\377','\377','\377','\377','\377',
    '\377',    26,    27,    28,    29,    30,    31,    32,
        33,    34,    35,    36,    37,    38,    39,    40,
        41,    42,    43,    44,    45,    46,    47,    48,
        49,    50,    51,'\377','\377','\377','\377','\377'
};

static char hextab[] = "0123456789ABCDEF";
static char eol_string[] = "\r\n";

#ifdef __cplusplus
inline int isbase64(int a) {
    return ('A' <= a && a <= 'Z')
        || ('a' <= a && a <= 'z')
        || ('0' <= a && a <= '9')
        || a == '+' || a == '/';
}
#else
#define isbase64(a) (  ('A' <= (a) && (a) <= 'Z') \
                    || ('a' <= (a) && (a) <= 'z') \
                    || ('0' <= (a) && (a) <= '9') \
                    ||  (a) == '+' || (a) == '/'  )
#endif


int encode_base64(const char* aIn, size_t aInLen, char* aOut,
    size_t aOutSize, size_t* aOutLen)
{
    size_t inLen = 0;
    char* out = 0;
    size_t outSize = 0;
    size_t inPos = 0;
    size_t outPos = 0;
    int c1, c2, c3;
    int lineLen = 0;
    size_t i;
    const char *cp = 0;
    
    
    if (!aIn || !aOut || !aOutLen)
        return -1;
    inLen = aInLen;
    out = aOut;
    outSize = (inLen+2)/3*4;     /* 3:4 conversion ratio */
    outSize += strlen(eol_string)*outSize/MAXLINE + 2;  /* Space for newlines and NUL */
    if (aOutSize < outSize)
        return -1;
    
    /* Get three characters at a time and encode them. */
    for (i=0; i < inLen/3; ++i) {
        c1 = aIn[inPos++] & 0xFF;
        c2 = aIn[inPos++] & 0xFF;
        c3 = aIn[inPos++] & 0xFF;
        out[outPos++] = base64tab[(c1 & 0xFC) >> 2];
        out[outPos++] = base64tab[((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4)];
        out[outPos++] = base64tab[((c2 & 0x0F) << 2) | ((c3 & 0xC0) >> 6)];
        out[outPos++] = base64tab[c3 & 0x3F];
        lineLen += 4;
        if (lineLen >= MAXLINE-3) {
            cp = eol_string;
            out[outPos++] = *cp++;
            if (*cp) {
                out[outPos++] = *cp;
            }
            lineLen = 0;
        }
    }
    /* Encode the remaining one or two characters. */
    switch (inLen % 3) {
    case 0:
        cp = eol_string;
        out[outPos++] = *cp++;
        if (*cp) {
            out[outPos++] = *cp;
        }
        break;
    case 1:
        c1 = aIn[inPos] & 0xFF;
        out[outPos++] = base64tab[(c1 & 0xFC) >> 2];
        out[outPos++] = base64tab[((c1 & 0x03) << 4)];
        out[outPos++] = '=';
        out[outPos++] = '=';
        cp = eol_string;
        out[outPos++] = *cp++;
        if (*cp) {
            out[outPos++] = *cp;
        }
        break;
    case 2:
        c1 = aIn[inPos++] & 0xFF;
        c2 = aIn[inPos] & 0xFF;
        out[outPos++] = base64tab[(c1 & 0xFC) >> 2];
        out[outPos++] = base64tab[((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4)];
        out[outPos++] = base64tab[((c2 & 0x0F) << 2)];
        out[outPos++] = '=';
        cp = eol_string;
        out[outPos++] = *cp++;
        if (*cp) {
            out[outPos++] = *cp;
        }
        break;
    }
    out[outPos] = 0;
    *aOutLen = outPos;
    return 0;
}

int decode_base64(const char* aIn, size_t aInLen, char* aOut,
    size_t aOutSize, size_t* aOutLen)
{
    size_t inLen = 0;
    char * out = 0;
    size_t outSize = 0;
    
    int isErr = 0;
    int isEndSeen = 0;
    int b1, b2, b3;
    int a1, a2, a3, a4;
    size_t inPos = 0;
    size_t outPos = 0;
    
    if (!aIn || !aOut || !aOutLen)
        return -1;
    inLen = aInLen;
    out = aOut;
    outSize = (inLen+3)/4*3;
    if (aOutSize < outSize)
        return -1;
    /* Get four input chars at a time and decode them. Ignore white space
     * chars (CR, LF, SP, HT). If '=' is encountered, terminate input. If
     * a char other than white space, base64 char, or '=' is encountered,
     * flag an input error, but otherwise ignore the char.
     */
    while (inPos < inLen) {
        a1 = a2 = a3 = a4 = 0;
        while (inPos < inLen) {
            a1 = aIn[inPos++] & 0xFF;
            if (isbase64(a1)) {
                break;
            }
            else if (a1 == '=') {
                isEndSeen = 1;
                break;
            }
            else if (a1 != '\r' && a1 != '\n' && a1 != ' ' && a1 != '\t') {
                isErr = 1;
            }
        }
        while (inPos < inLen) {
            a2 = aIn[inPos++] & 0xFF;
            if (isbase64(a2)) {
                break;
            }
            else if (a2 == '=') {
                isEndSeen = 1;
                break;
            }
            else if (a2 != '\r' && a2 != '\n' && a2 != ' ' && a2 != '\t') {
                isErr = 1;
            }
        }
        while (inPos < inLen) {
            a3 = aIn[inPos++] & 0xFF;
            if (isbase64(a3)) {
                break;
            }
            else if (a3 == '=') {
                isEndSeen = 1;
                break;
            }
            else if (a3 != '\r' && a3 != '\n' && a3 != ' ' && a3 != '\t') {
                isErr = 1;
            }
        }
        while (inPos < inLen) {
            a4 = aIn[inPos++] & 0xFF;
            if (isbase64(a4)) {
                break;
            }
            else if (a4 == '=') {
                isEndSeen = 1;
                break;
            }
            else if (a4 != '\r' && a4 != '\n' && a4 != ' ' && a4 != '\t') {
                isErr = 1;
            }
        }
        if (isbase64(a1) && isbase64(a2) && isbase64(a3) && isbase64(a4)) {
            a1 = base64idx[a1] & 0xFF;
            a2 = base64idx[a2] & 0xFF;
            a3 = base64idx[a3] & 0xFF;
            a4 = base64idx[a4] & 0xFF;
            b1 = ((a1 << 2) & 0xFC) | ((a2 >> 4) & 0x03);
            b2 = ((a2 << 4) & 0xF0) | ((a3 >> 2) & 0x0F);
            b3 = ((a3 << 6) & 0xC0) | ( a4       & 0x3F);
            out[outPos++] = (char)(b1);
            out[outPos++] = (char)(b2);
            out[outPos++] = (char)(b3);
        }
        else if (isbase64(a1) && isbase64(a2) && isbase64(a3) && a4 == '=') {
            a1 = base64idx[a1] & 0xFF;
            a2 = base64idx[a2] & 0xFF;
            a3 = base64idx[a3] & 0xFF;
            b1 = ((a1 << 2) & 0xFC) | ((a2 >> 4) & 0x03);
            b2 = ((a2 << 4) & 0xF0) | ((a3 >> 2) & 0x0F);
            out[outPos++] = (char)(b1);
            out[outPos++] = (char)(b2);
            break;
        }
        else if (isbase64(a1) && isbase64(a2) && a3 == '=' && a4 == '=') {
            a1 = base64idx[a1] & 0xFF;
            a2 = base64idx[a2] & 0xFF;
            b1 = ((a1 << 2) & 0xFC) | ((a2 >> 4) & 0x03);
            out[outPos++] = (char)(b1);
            break;
        }
        else {
            break;
        }
        if (isEndSeen) {
            break;
        }
    } /* end while loop */
    *aOutLen = outPos;
    return (isErr) ? -1 : 0;
}

int EncodeBase64( CString &strSrc, CString &strDest )
{
	size_t destSize = 4*(strSrc.GetLength()+2)/3;
	destSize += strlen("\r\n")*destSize/72 + 2;
	destSize += 64; /*64;  a little extra room */

	/* Allocate destination buffer */
	char * destBuf = strDest.GetBuffer(destSize);
	if( 0 == destBuf )
	{
		strDest.ReleaseBuffer();
		return FALSE;
	}
	
	/* Encode source to destination */
	size_t destLen = 0;
	int result = encode_base64(strSrc, strSrc.GetLength(), destBuf, destSize, &destLen);
	if( destLen < destSize )
		destBuf[destLen]	=	'\0';

	strDest.ReleaseBuffer();
	return strDest.GetLength();
}

int DecodeBase64( CString &strSrc, CString &strDest )
{
	size_t destSize = 4*(strSrc.GetLength()+2)/3;
	destSize += strlen("\r\n")*destSize/72 + 2;
	destSize += 64; /*64;  a little extra room */

	/* Allocate destination buffer */
	char * destBuf = strDest.GetBuffer(destSize);
	if( 0 == destBuf )
	{
		strDest.ReleaseBuffer();
		return FALSE;
	}
	
	/* Encode source to destination */
	size_t destLen = 0;
	int result = decode_base64(strSrc, strSrc.GetLength(), destBuf, destSize, &destLen);
	if( destLen < destSize )
		destBuf[destLen]	=	'\0';

	strDest.ReleaseBuffer();
	return strDest.GetLength();
}

//////////////////////////////////////////////////////////////////////////
// CRC32 and CRC16

static unsigned int crc_32_tab[] = {
        0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
        0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
        0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
        0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
        0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
        0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
        0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
        0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
        0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
        0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
        0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
        0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
        0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
        0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
        0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
        0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
        0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
        0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
        0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
        0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
        0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
        0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
        0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
        0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
        0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
        0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
        0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
        0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
        0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
        0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
        0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
        0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
        0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
        0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
        0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
        0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
        0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
        0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
        0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
        0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
        0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
        0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
        0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
        0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
        0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
        0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
        0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
        0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
        0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
        0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
        0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
        0x2d02ef8dL
};

unsigned int CRC32(char *s, int len)
{
	register unsigned c;
	unsigned int crc = (unsigned int)0xffffffffL;

	if (s == 0)
	{
		c = 0xffffffffL;
	}
	else
	{
		c = crc;
		if(len)
		{
			do
			{
				c = crc_32_tab[((int)c ^ (*s++)) & 0xff] ^ (c >> 8);
			} while (--len);
		}
	}
	crc = c;
	return c ^ 0xffffffffL;
}

WORD CRC16(unsigned char *info, DWORD len)
{ 
	WORD acc;
	unsigned char i;
	acc=0;
	while( len-- )
	{
		acc=acc^(((unsigned int)(*info))<<8);
		info++;
		for( i=8; i>0; i-- )
		{
			if (acc&0x8000)
				acc=(acc<<1)^0x1021;
			else acc<<=1; 
		}
	} 
	return acc; 
} 

