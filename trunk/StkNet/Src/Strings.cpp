/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		strings;
*/

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef	CLKLAN_ENGLISH_US

char szErrCancel[]			=	"User canceled";
char szErrUrlFormat[]		=	"Server address error";
char szErrRedirect[]		=	"Error occurs when redirecting to net data source";
char szErrInterrupted[]		=	"Transfer interrupted, please contact software provider";
char szErrDataTransfer[]	=	"Transfer error, please try again.";
char szErrZipDll[]			=	"Decompressing dynamic link library error";
char szErrZip[]				=	"Decompression error, please download it again";
char szErrXMLDefault[]		=	"Error occurs when parsing XML";
char szErrXMLPackageSetting[]	=	"Error occurs when getting package settings";
char szErrXMLPackageData[]		=	"Error occurs when getting package data";
char szErrXMLServerSetting[]	=	"Error occurs when getting server settings";
char szErrXMLServerData[]		=	"Error occurs when getting server data";


char szProgHttpConnectting[]=	"Connecting to server...";
char szProgRequestSent[]	=	"Receiving data...";
char szProgRedirecting[]	=	"Redirecting server address...";
char szProgTransferring[]	=	"Transfering data...";
char szProgInstallPackage[]	=	"Installing package (may take a few minutes)...";
char szProgExtractZipFiles[]=	"Decompressing Files (may take a few minutes)...";
char szProgErrorPac[]		=	"Packages which error: ";

char szFmtYearDescript[]	=	"Year %.4d, Day";
char szFmtMonthDescript[]	=	"Year %.4d, Month %.2dth, Day";
char szFmtDayDescript[]		=	"Year %.4d, Month %.2dth, Day %.2dth, Day";

#else

char szErrCancel[]			=	"用户取消操作";
char szErrUrlFormat[]		=	"服务器地址有错误";
char szErrRedirect[]		=	"重定向网络数据源时发生错误";
char szErrInterrupted[]		=	"网络传输意外终止，请与软件供应商联系";
char szErrDataTransfer[]	=	"数据传输错误，请重新下载";
char szErrZipDll[]			=	"解压缩文件DLL错误";
char szErrZip[]				=	"解压缩文件时出现错误，请重新下载";
char szErrXMLDefault[]		=	"解析XML文件时出现错误";
char szErrXMLPackageSetting[]	=	"获取数据包属性时出现错误";
char szErrXMLPackageData[]		=	"获取数据包数据时出现错误";
char szErrXMLServerSetting[]	=	"获取服务器属性时出现错误";;
char szErrXMLServerData[]		=	"获取服务器数据时出现错误";;


char szProgHttpConnectting[]=	"正在连接服务器...";
char szProgRequestSent[]	=	"正在准备接收数据...";
char szProgRedirecting[]	=	"重定向服务器地址...";
char szProgTransferring[]	=	"正在传输数据...";
char szProgInstallPackage[]	=	"正在安装数据包（可能需要几分钟）...";
char szProgExtractZipFiles[]=	"正在解压缩文件（可能需要几分钟）...";
char szProgErrorPac[]		=	"未完成数据包：";

char szFmtYearDescript[]	=	"%.4d年日线数据";
char szFmtMonthDescript[]	=	"%.4d年%.2d月日线数据";
char szFmtDayDescript[]		=	"%.4d年%.2d月%.2d日日线数据";

#endif	// CLKLAN_ENGLISH_US
