//
// Copyright ?1998 Kirk Stowell ( kstowel@sprynet.com )
// www.geocities.com/SiliconValley/Haven/8230/index.html
//
// You are free to use, modify and distribute this source, as long as
// there is no charge, and this HEADER stays intact. This source is
// supplied "AS-IS", without WARRANTY OF ANY KIND, and the user
// holds Kirk Stowell blameless for any or all problems that may arise
// from the use of this code.
//
//////////////////////////////////////////////////////////////////////

#if !defined(XPFACE_H_INCLUDED)
#define XPFACE_H_INCLUDED

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

#ifndef XPFACE_DLL
	#if defined(XPFACE_STATIC) 
	  #if defined (_DEBUG) 
		#pragma comment(lib,"XPFaceStaticLibd.lib") 
		#pragma message("Automatically linking with XPFaceStaticLibd.lib") 
	  #else 
		#pragma comment(lib,"XPFaceStaticLib.lib") 
		#pragma message("Automatically linking with XPFaceStaticLib.lib") 
	  #endif 
	#elif defined(_DEBUG) 
	  #pragma comment(lib,"XPFaced.lib") 
	  #pragma message("Automatically linking with XPFaced.dll") 
	#else 
	  #pragma comment(lib,"XPFace.lib") 
	  #pragma message("Automatically linking with XPFace.dll") 
	#endif 
#endif

#if defined(XPFACE_STATIC) 
  #define CLASS_EXPORT 
#elif defined(XPFACE_DLL) 
  #define CLASS_EXPORT AFX_CLASS_EXPORT
#else 
  #define CLASS_EXPORT AFX_CLASS_IMPORT
#endif 

#include "Dibapi.h"
#include "HyperLink.h"
#include "XFDialog.h"
#include "MagicEdit.h"
#include "Dialogs.h"
#include "SplashDlg.h"


#endif // XPFACE_H_INCLUDED
