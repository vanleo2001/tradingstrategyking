#ifndef ZCALLBCK_H
#define ZCALLBCK_H

#include "StdAfx.h"

#define MAX_FILES 4096
/***
	Zip文件压缩解压缩定义
*/
/* flag values for status callback function */
#define UZ_ST_START_EXTRACT     1       /* no details */
#define UZ_ST_IN_PROGRESS       2       /* no details */
#define UZ_ST_FINISH_MEMBER     3       /* 'details': extracted size */

/* return values of status callback function */
#define UZ_ST_CONTINUE          0
#define UZ_ST_BREAK             1

#pragma pack (push)

struct CZipCallbackData
{
	DWORD m_hwndHandle;
	HWND m_pCaller;
	long int m_liVersion;
	BOOL m_bIsOperationZip;
	long int m_liActionCode;
	long int m_liErrorCode;
	long int m_liFileSize;
	char m_pszFileNameOrMsg[512];
};

#pragma pack (pop)

typedef BOOL (__stdcall *ZFunctionPtrType) (CZipCallbackData*);

#endif ZCALLBCK_H  //   ZCALLBCK_H
