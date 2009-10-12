#if !defined(AFX_AFXCORE_H__2F96EC6F_0775_45AA_99EE_0FC048AD3208__INCLUDED_)
#define AFX_AFXCORE_H__2F96EC6F_0775_45AA_99EE_0FC048AD3208__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AfxCore.h : header file
//

class	CStkUIApp;
class	CMainFrame;
class	CStaticDoc;
class	CStrategyView;
class	CGroupView;
class	CTechsView;
class	CWizardView;
class	CSimuView;
class	CSListView;
class	CRealTimeView;
class	CMultiSortView;
class	CGraphView;
class	CBaseView;
class	CInfoView;
class	CSelectorView;

/////////////////////////////////////////////////////////////////////////////
// The following is inline functions

// Secret Assert Valid Functions
void AfxAssertNull( );
BOOL AfxAssertValid( );
BOOL AfxAssertValidStd( );
BOOL AfxAssertValidProf( );
BOOL AfxAssertFunclimitStd( UINT nIDString = IDS_SECRET_FUNCLIMIT );
BOOL AfxAssertFunclimitProf( UINT nIDString = IDS_SECRET_FUNCLIMIT );
BOOL AfxAssertRegisterQuit( );
BOOL AfxAssertMessageQuit( );
BOOL AfxAssertQuit( );

// app and mainframe and doc functions
CStkUIApp *		AfxGetStkUIApp( );
CMainFrame *	AfxGetMainFrame( );
CStaticDoc *	AfxGetStaticDoc( );
CStrategy *		AfxGetActiveStrategy( );
CStrategyView *	AfxGetStrategyView( );
CGroupView *	AfxGetGroupView( );
CTechsView *	AfxGetTechsView( );

// view functions
CWizardView * AfxGetWizardView( );
CSListView * AfxGetSListView( );
CGraphView * AfxGetGraphView( );
CRealTimeView * AfxGetRealTimeView( );
CMultiSortView * AfxGetMultiSortView( );
CBaseView * AfxGetBaseView( );
CInfoView * AfxGetInfoView( );
CSelectorView * AfxGetSelectorView( );

BOOL AfxSwitchToStaticView( CRuntimeClass * pViewClass );
void AfxShowSlist( int nType, LPCTSTR lpszDomain = NULL, DWORD dwDate = -1 );		// 报价
void AfxShowSlistSortDP( int nType, LPCTSTR lpszDomain = NULL, DWORD dwDate = -1 );	// 涨跌排名
void AfxShowInfo( int nServer );	// 在线资讯
void AfxShowMultiSort( LONG type );	// 综合排名

// show graph and base or other command
BOOL AfxShowStockRealTime( int nStockIndex, BOOL bFromSList );
BOOL AfxShowStockRealTime( const char * szCode );
BOOL AfxShowStockGraph( int nStockIndex, BOOL bFromSList );
BOOL AfxShowStockGraph( const char * szCode );
BOOL AfxShowStockBase( int nStockIndex, BOOL bFromSList );
BOOL AfxShowStockBase( const char * szCode );
BOOL AfxShowStockInfo( int nStockIndex, BOOL bFromSList );
BOOL AfxShowStockInfo( const char * szCode );
BOOL AfxShowStockTech( UINT nTech );

void DDX_DateTimeCtrl(CDataExchange* pDX, int nIDC, CSPTime& value);

// 根据当前语言和高度选择pDC适应的Font
CFont * AfxSelectDCFont( CDC * pDC, int nHeight );

// 重新计算每只股票的投资年收益率和β值等
UINT AfxRecalculateYield( int nDays, BOOL bReport );

// 快捷键
void AfxExecuteAccelerator( UINT nAcce );

// 策略改动之后，刷新视图
void AfxOnStrategyUpdated( CStrategy * pStrategy, BOOL bReRealRun );

// Afx Menu Functions
BOOL AfxMenuInsertDomain( CMenu *pMenu, int nMenuPos, UINT nIDStart, UINT nIDEnd );
BOOL AfxMenuInsertGroup( CMenu *pMenu, int nMenuPos, UINT nIDStart, UINT nIDEnd, BOOL bEnable );

/////////////////////////////////////////////////////////////////////
// The Following is command functions
// Rigister and file functions
void AfxRegisterShellFileTypes(BOOL bCompat);
void AfxUnregisterShellFileTypes();
BOOL AfxDoPromptFileName(CString& fileName,
						 UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog );
BOOL AfxDoSelectDirectory( CString & strSrc, HWND hWnd = NULL );

CString	AfxGetStrategyFileExt( );
CString	AfxGetApplicationName( );
CString	AfxGetStockIndexReportTip( );

#include "StkUI.h"
#include "MainFrm.h"
#include "StaticDoc.h"
#include "View/WizardView.h"
#include "View/SListView.h"
#include "View/RealTimeView.h"
#include "View/MultiSortView.h"
#include "View/GraphView.h"
#include "View/BaseView.h"
#include "View/SimuView.h"
#include "View/InfoView.h"
#include "View/SelectorView.h"
#include "View/StrategyView.h"
#include "View/GroupView.h"
#include "View/TechsView.h"

#define _AFXCORE_INLINE AFX_INLINE
#include "AfxCore.inl"
#undef	_AFXCORE_INLINE

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AFXCORE_H__2F96EC6F_0775_45AA_99EE_0FC048AD3208__INCLUDED_)
