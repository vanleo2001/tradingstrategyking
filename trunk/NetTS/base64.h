/*
	B A S E 6 4. H

	Copyright (C) 2001 Balang Software

	Version: 0.0.1
*/

#ifndef __STKNET_BASE64_H__
#define __STKNET_BASE64_H__
/***
	Base64±àÂë
*/
int encode_base64(const char* aIn, size_t aInLen, char* aOut,
    size_t aOutSize, size_t* aOutLen);
int decode_base64(const char* aIn, size_t aInLen, char* aOut,
    size_t aOutSize, size_t* aOutLen);

int EncodeBase64( CString &strSrc, CString &strDest );
int DecodeBase64( CString &strSrc, CString &strDest );

unsigned int CRC32(char *s, int len);
WORD CRC16(unsigned char *info, DWORD len);

#endif

