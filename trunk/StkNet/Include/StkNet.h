//
// Copyright 2001 Balang SoftWare
//
// You are free to use, modify and distribute this source, as long as
// there is no charge, and this HEADER stays intact. This source is
// supplied "AS-IS", without WARRANTY OF ANY KIND.
//
//////////////////////////////////////////////////////////////////////

#if !defined(STKNET_H_INCLUDED)
#define STKNET_H_INCLUDED

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef	STKNET_DLL
	#if defined(STKNET_STATIC) 
	  #if defined (_DEBUG) 
		#pragma comment(lib,"StkNet30dStatic.lib") 
		#pragma message("Automatically linking with StkNet30dStatic.lib") 
	  #else 
		#pragma comment(lib,"StkNet30Static.lib") 
		#pragma message("Automatically linking with StkNet30Static.lib") 
	  #endif 
	#elif defined(_DEBUG) 
	  #pragma comment(lib,"StkNet30D.lib") 
	  #pragma message("Automatically linking with StkNet30D.dll") 
	#else 
	  #pragma comment(lib,"StkNet30.lib") 
	  #pragma message("Automatically linking with StkNet30.dll") 
	#endif 
#endif

#if !defined(STKNET_STATIC)
	#ifdef STKNET_DLL
	#define STKNET_API __declspec(dllexport)
	#else
	#define STKNET_API __declspec(dllimport)
	#endif
#else 
	#define	STKNET_API
#endif

#include	"QSProfile.h"
#include	"XMLDoc.h"
#include	"HttpClt.h"
#include	"NetDB.h"
#include	"InfoZip.h"
#include	"Secret.h"
#include	"AfxCore.h"
#include	"StkReceiver.h"
#include	"StartupDlg.h"

#endif // STKNET_H_INCLUDED
