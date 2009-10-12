//
// Copyright 2001 Balang SoftWare
//
// You are free to use, modify and distribute this source, as long as
// there is no charge, and this HEADER stays intact. This source is
// supplied "AS-IS", without WARRANTY OF ANY KIND.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__STKLIB_STKLIB_H__)
#define __STKLIB_STKLIB_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef	STKLIB_DLL
	#if defined(STKLIB_STATIC) 
	  #if defined (_DEBUG) 
		#pragma comment(lib,"StkLib30dStatic.lib") 
		#pragma message("Automatically linking with StkLib30dStatic.lib") 
	  #else 
		#pragma comment(lib,"StkLib30Static.lib") 
		#pragma message("Automatically linking with StkLib30Static.lib") 
	  #endif 
	#elif defined(_DEBUG) 
	  #pragma comment(lib,"StkLib30d.lib") 
	  #pragma message("Automatically linking with StkLib30d.dll") 
	#else 
	  #pragma comment(lib,"StkLib30.lib") 
	  #pragma message("Automatically linking with StkLib30.dll") 
	#endif 
#endif

#if !defined(STKLIB_STATIC)
	#ifdef STKLIB_DLL
	#define STKLIB_API __declspec(dllexport)
	#else
	#define STKLIB_API __declspec(dllimport)
	#endif
#else 
	#define	STKLIB_API
#endif

// #define	CLKLAN_ENGLISH_US		// 英文版（美国）
// #define	CLKLAN_CHINESE_SIM		// 简体中文版
// must define one
#if !defined(CLKLAN_ENGLISH_US) && !defined(CLKLAN_CHINESE_SIM)
	#define	CLKLAN_CHINESE_SIM
#endif

/**********************************************************************
 * OEM 版请去掉#define CLKVER_OEM 前的注释，并修改以下常量，重新编译
 */

// #define	CLKVER_OEM		// OEM版去掉该行注释

#define	OEM_REGKEY_COMPANY	"Ninebulls-Stock Software"// 注册表中的公司代号
#define	OEM_REGKEY_APP		"CLKingOEM"				// 注册表中的软件代号

#define	OEM_SUPPORT_MAILTO	"support@ninebulls.com"	// 技术支持邮箱地址
#define	OEM_SERVER_DOMAIN	"www.ninebulls.com"		// 公司网站地址，
													// 注：软件中历史数据下载要从该网址下载

#define	OEM_SEED_STD1		"dc./x"					// 标准版注册机种子1
#define	OEM_SEED_STD2		"sfk%c"					// 标准版注册机种子2
#define	OEM_SEED_PROF1		"78cd2"					// 专业版注册机种子1
#define	OEM_SEED_PROF2		"d6$3#"					// 专业版注册机种子2

/*
 * OEM 版结束
**********************************************************************/

#include	"SpPlat.h"
#include	"SpDefs.h"
#include	"SpAssert.h"
#include	"SpObject.h"
#include	"SpString.h"
#include	"SpTime.h"
#include	"SpFile.h"
#include	"SpArchive.h"
#include	"SpColl.h"
#include	"MathParser.h"
#include	"Stock.h"
#include	"Technique.h"
#include	"Database.h"
#include	"Boyermor.h"
#include	"CommFuns.h"
#include	"Container.h"
#include	"Express.h"
#include	"Alarm.h"
#include	"Profile.h"
#include	"Strategy.h"
#include	"DayTrader.h"
#include	"AfxCore.h"
#include	"Packets.h"


#endif // __STKLIB_STKLIB_H__
