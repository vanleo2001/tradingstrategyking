// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__BD1B8846_C61A_405F_A998_52C5656A6730__INCLUDED_)
#define AFX_MAINFRM_H__BD1B8846_C61A_405F_A998_52C5656A6730__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	TIMER_TIME				1
#define	TIMER_STOCKINDEXREFRESH	2
#define	TIMER_CHECKVALID		3
#define	TIMER_SHOWNETINFO		4
#define TIMER_AUTOUPDATE		5

#define	WM_USER_CHECKSN				(WM_USER+1000)
#define	WM_USER_UPDATEBARMENU		(WM_USER+1001)
#define WM_USER_NIFMESSAGE			(WM_USER+1002)
#define	WM_USER_INITDATES			(WM_USER+1003)
#define	WM_USER_UPDATESLISTVIEW		(WM_USER+1004)

#include	"SListCombos.h"
#include	"View/StrategyView.h"
#include	"View/GroupView.h"
#include	"View/TechsView.h"

#include "TSKStatusBar.h"

#ifdef _TSK_MAINFRAME
#define CTskMainFrame CFrameWnd
#else
#define CTskMainFrame CMDIFrameWnd
#endif

class CMainFrame : public CTskMainFrame
{
	DECLARE_DYNCREATE(CMainFrame)
protected:
	CMainFrame();

public:
	static CMainFrame* CreateNewFrame( );

// Attributes
public:
	struct	tagFullScreen
	{
		BOOL		bMaximized;
		BOOL		bFullScreen;
		BOOL		bMainBar;
		BOOL		bSListBar;
		BOOL		bSimuBar;
		BOOL		bStatusBar;
		BOOL		bWorkspBar;
	}m_fullScreen;

	
// Operations
public:
	//CStrategyView *	GetStrategyView( ) { return DYNAMIC_DOWNCAST(CStrategyView,m_wndWorkspBar.GetView( RUNTIME_CLASS(CStrategyView) ) ); }
	//CGroupView *	GetGroupView( ) { return DYNAMIC_DOWNCAST(CGroupView,m_wndWorkspBar.GetView( RUNTIME_CLASS(CGroupView) ) ); }
	//CTechsView *	GetTechsView( ) { return DYNAMIC_DOWNCAST(CTechsView,m_wndWorkspBar.GetView( RUNTIME_CLASS(CTechsView) ) ); }

	BOOL	CreateMainBar( );
	BOOL	CreateSListBar( );
	BOOL	CreateSimuBar( );
	BOOL	CreateStatusBar( );
	BOOL	CreateWorkspBar( );

	void	ShowProgressBar( );
	void	HideProgressBar( );
	void	ShowWorkspBar( BOOL bShow = TRUE );
	void	SetStatusMsg( LPCTSTR lpszText );
	void	SetProgress( int nPercent );
	//void	DockControlBarLeftOf(CGuiToolBarWnd* Bar,CGuiToolBarWnd* LeftOf);
	CPoint	CalcPopupMenuPoint( CToolBar * pBar, UINT nID, CMenu *pMenu );
	void	InitMenuPopup(CMenu* pPopupMenu);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CSearchBox		m_SearchBox;
	//CDateComboBox	m_DateBox;

protected:
	// window placement persistence
	WINDOWPLACEMENT m_dataFrameWP;

	//CTextToolBar		m_wndMainBar;
	//CGuiToolBarWnd		m_wndViewBar;
	//CGuiToolBarWnd		m_wndPeriodBar;
	//CTextToolBar		m_wndSListBar;
	//CGuiToolBarWnd		m_wndSimuBar;
	//CDialogBar			m_wndProgressBar;

	CTSKStatusBar m_wndStatusBar;

	//CWorkspBar		m_wndWorkspBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy); 
	afx_msg void OnClose();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnMainbarSetting();
	afx_msg void OnMainbarView();
	afx_msg void OnMainbarTech();
	afx_msg void OnMainbarPeriod();
	afx_msg void OnSlistbarGroup();
	afx_msg void OnSlistbarDomain();
	afx_msg void OnSlistbarClass();

	afx_msg void OnSysConnectserver();
	afx_msg void OnUpdateSysConnectserver(CCmdUI* pCmdUI);
	afx_msg void OnSysDisconnectserver();
	afx_msg void OnUpdateSysDisconnectserver(CCmdUI* pCmdUI);
	afx_msg void OnSysStarttongshi();
	afx_msg void OnUpdateSysStarttongshi(CCmdUI* pCmdUI);
	afx_msg void OnSysSetuptongshi();
	afx_msg void OnUpdateSysSetuptongshi(CCmdUI* pCmdUI);
	afx_msg void OnSysStoptongshi();
	afx_msg void OnUpdateSysStoptongshi(CCmdUI* pCmdUI);
	afx_msg void OnSysAutoupgrade();
	afx_msg void OnSysAutosave();
	afx_msg void OnUpdateSysAutosave(CCmdUI* pCmdUI);
	afx_msg void OnSysNotifyicon();
	afx_msg void OnUpdateSysNotifyicon(CCmdUI* pCmdUI);

	afx_msg void OnViewMainbartext();
	afx_msg void OnUpdateViewMainbartext(CCmdUI* pCmdUI);
	afx_msg void OnViewFullscreen();
	afx_msg void OnUpdateViewFullscreen(CCmdUI* pCmdUI);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnViewWizard();
	afx_msg void OnUpdateViewWizard(CCmdUI* pCmdUI);
	afx_msg void OnViewSimu();
	afx_msg void OnUpdateViewSimu(CCmdUI* pCmdUI);
	afx_msg void OnViewSlist();
	afx_msg void OnUpdateViewSlist(CCmdUI* pCmdUI);
	afx_msg void OnViewRealtime();
	afx_msg void OnUpdateViewRealtime(CCmdUI* pCmdUI);
	afx_msg void OnViewMultisort();
	afx_msg void OnUpdateViewMultisort(CCmdUI* pCmdUI);
	afx_msg void OnViewTechgraph();
	afx_msg void OnUpdateViewTechgraph(CCmdUI* pCmdUI);
	afx_msg void OnViewBase();
	afx_msg void OnUpdateViewBase(CCmdUI* pCmdUI);
	afx_msg void OnViewInfo();
	afx_msg void OnUpdateViewInfo(CCmdUI* pCmdUI);
	afx_msg void OnViewSelector();
	afx_msg void OnUpdateViewSelector(CCmdUI* pCmdUI);
	afx_msg void OnViewF10();
	afx_msg void OnUpdateViewF10(CCmdUI* pCmdUI);

	afx_msg void OnSlistIndex();
	afx_msg void OnSlistStrategy();
	afx_msg void OnSlistGroup();
	afx_msg void OnSlistGroupself();
	afx_msg void OnSlistDomain();
	afx_msg void OnSlistAll();
	afx_msg void OnSlistShaa();
	afx_msg void OnSlistShab();
	afx_msg void OnSlistSzna();
	afx_msg void OnSlistSznb();
	afx_msg void OnSlistShabond();
	afx_msg void OnSlistSznbond();
	afx_msg void OnSlistShaasortdp();
	afx_msg void OnSlistShabsortdp();
	afx_msg void OnSlistSznasortdp();
	afx_msg void OnSlistSznbsortdp();
	afx_msg void OnSlistShabondsortdp();
	afx_msg void OnSlistSznbondsortdp();
	afx_msg void OnSlistSetcolumn();
	afx_msg void OnSlistBaseindex();
	afx_msg void OnSlistbarDate();
	afx_msg void OnSlistbarName();

	afx_msg void OnViewViewszzs();
	afx_msg void OnViewViewszybl();
	afx_msg void OnViewViewszag();
	afx_msg void OnViewViewszbg();
	afx_msg void OnViewViewszncz();
	afx_msg void OnViewViewsznzz();
	afx_msg void OnViewViewszncfaz();
	afx_msg void OnViewViewszncfbz();
	afx_msg void OnViewTechoption();
	afx_msg void OnViewPday();
	afx_msg void OnUpdateViewPday(CCmdUI* pCmdUI);
	afx_msg void OnViewPweek();
	afx_msg void OnUpdateViewPweek(CCmdUI* pCmdUI);
	afx_msg void OnViewPmonth();
	afx_msg void OnUpdateViewPmonth(CCmdUI* pCmdUI);
	afx_msg void OnViewPmin5();
	afx_msg void OnUpdateViewPmin5(CCmdUI* pCmdUI);
	afx_msg void OnViewPmin15();
	afx_msg void OnUpdateViewPmin15(CCmdUI* pCmdUI);
	afx_msg void OnViewPmin30();
	afx_msg void OnUpdateViewPmin30(CCmdUI* pCmdUI);
	afx_msg void OnViewPmin60();
	afx_msg void OnUpdateViewPmin60(CCmdUI* pCmdUI);
	afx_msg void OnViewLeft();
	afx_msg void OnViewRight();
	afx_msg void OnViewPageleft();
	afx_msg void OnViewPageright();
	afx_msg void OnViewHome();
	afx_msg void OnViewEnd();
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnViewPrev();
	afx_msg void OnViewNext();
	afx_msg void OnViewInsert();
	afx_msg void OnViewNoresumedr();
	afx_msg void OnUpdateViewNoresumedr(CCmdUI* pCmdUI);
	afx_msg void OnViewResumedrup();
	afx_msg void OnUpdateViewResumedrup(CCmdUI* pCmdUI);
	afx_msg void OnViewResumedrdown();
	afx_msg void OnUpdateViewResumedrdown(CCmdUI* pCmdUI);
	afx_msg void OnViewMaindataclose();
	afx_msg void OnUpdateViewMaindataclose(CCmdUI* pCmdUI);
	afx_msg void OnViewMaindataopen();
	afx_msg void OnUpdateViewMaindataopen(CCmdUI* pCmdUI);
	afx_msg void OnViewMaindataaverage();
	afx_msg void OnUpdateViewMaindataaverage(CCmdUI* pCmdUI);
	afx_msg void OnViewKlinecandle();
	afx_msg void OnUpdateViewKlinecandle(CCmdUI* pCmdUI);
	afx_msg void OnViewKlineamerica();
	afx_msg void OnUpdateViewKlineamerica(CCmdUI* pCmdUI);
	afx_msg void OnViewKlinetower();
	afx_msg void OnUpdateViewKlinetower(CCmdUI* pCmdUI);
	
	afx_msg void OnRealtimeViewszzs();
	afx_msg void OnRealtimeViewszybl();
	afx_msg void OnRealtimeViewszag();
	afx_msg void OnRealtimeViewszbg();
	afx_msg void OnRealtimeViewszncz();
	afx_msg void OnRealtimeViewsznzz();
	afx_msg void OnRealtimeViewszncfaz();
	afx_msg void OnRealtimeViewszncfbz();
	afx_msg void OnRealtimeLeft();
	afx_msg void OnRealtimeRight();
	afx_msg void OnRealtimeHome();
	afx_msg void OnRealtimeEnd();
	afx_msg void OnRealtimePrev();
	afx_msg void OnRealtimeNext();
	afx_msg void OnRealtimeOnestock();
	afx_msg void OnRealtimeTwostock();
	afx_msg void OnRealtimeFourstock();
	afx_msg void OnRealtimeSixstock();
	afx_msg void OnRealtimeNinestock();
	afx_msg void OnUpdateRealtimeOnestock(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRealtimeTwostock(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRealtimeFourstock(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRealtimeSixstock(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRealtimeNinestock(CCmdUI* pCmdUI);
	afx_msg void OnRealtimePriceline();
	afx_msg void OnRealtimeReportdetail();
	afx_msg void OnRealtimeMinutedetail();
	afx_msg void OnRealtimeBigtradedetail();
	afx_msg void OnRealtimeLbdk();
	afx_msg void OnRealtimeMmld();
	afx_msg void OnUpdateRealtimePriceline(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRealtimeReportdetail(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRealtimeMinutedetail(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRealtimeBigtradedetail(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRealtimeLbdk(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRealtimeMmld(CCmdUI* pCmdUI);
	afx_msg void OnRealtimeMsszag();
	afx_msg void OnRealtimeMsszbg();
	afx_msg void OnRealtimeMsszzq();
	afx_msg void OnRealtimeMssznag();
	afx_msg void OnRealtimeMssznbg();
	afx_msg void OnRealtimeMssznzq();
	afx_msg void OnUpdateRealtimeMsszag(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRealtimeMsszbg(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRealtimeMsszzq(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRealtimeMssznag(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRealtimeMssznbg(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRealtimeMssznzq(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRealtimeMsslbdk(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRealtimeMssmmld(CCmdUI* pCmdUI);

	afx_msg void OnInfoSelf();
	afx_msg void OnInfoF10();
	afx_msg void OnInfoStockstar();
	afx_msg void OnInfoSohu();
	afx_msg void OnInfoSina();
	afx_msg void OnInfo163();
	afx_msg void OnInfoShse();
	afx_msg void OnInfoSzse();
	afx_msg void OnInfoHkex();
	afx_msg void OnInfoLse();
	afx_msg void OnInfoNyse();
	afx_msg void OnInfoNasdaq();
	afx_msg void OnInfoP5w();
	afx_msg void OnInfoSsnews();
	afx_msg void OnInfoCs();
	afx_msg void OnUpdateInfoSelf(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInfoF10(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInfoStockstar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInfoSohu(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInfoSina(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInfo163(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInfoShse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInfoSzse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInfoHkex(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInfoLse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInfoNyse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInfoNasdaq(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInfoP5w(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInfoSsnews(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInfoCs(CCmdUI* pCmdUI);

	afx_msg void OnStrategyProperty();
	afx_msg void OnUpdateStrategyProperty(CCmdUI* pCmdUI);
	afx_msg void OnStrategyStocks();
	afx_msg void OnUpdateStrategyStocks(CCmdUI* pCmdUI);
	afx_msg void OnStrategySetRule();
	afx_msg void OnUpdateStrategySetRule(CCmdUI* pCmdUI);
	afx_msg void OnStrategySetRate();
	afx_msg void OnUpdateStrategySetRate(CCmdUI* pCmdUI);
	afx_msg void OnStrategySimu();
	afx_msg void OnUpdateStrategySimu(CCmdUI* pCmdUI);
	afx_msg void OnStrategyReport();
	afx_msg void OnUpdateStrategyReport(CCmdUI* pCmdUI);
	afx_msg void OnStrategyRealOp();
	afx_msg void OnUpdateStrategyRealOp(CCmdUI* pCmdUI);
	
	afx_msg void OnDataDownload();
	afx_msg void OnDataDatasource();
	afx_msg void OnDataManage();
	afx_msg void OnDataExport();
	afx_msg void OnDataRecalculateyield();
	
	afx_msg void OnOptionProxyset();
	afx_msg void OnOptionSelecttrader();
	afx_msg void OnOptionColor();
	afx_msg void OnOptionFontslist();
	afx_msg void OnOptionFontbase();
	afx_msg void OnOptionSetgroup();
	afx_msg void OnOptionAddtostrategy();
	afx_msg void OnUpdateOptionAddtostrategy(CCmdUI* pCmdUI);

	afx_msg void OnToolTrader();
	afx_msg void OnToolNote();
	afx_msg void OnToolCalculator();
	afx_msg void OnToolFilterinfo();
	afx_msg void OnToolAlarm();

	afx_msg void OnHelpShowhelp();
	afx_msg void OnHelpDeclare();

	//}}AFX_MSG
	afx_msg void OnMainBarDropDown(NMHDR* /*pNMHDR*/, LRESULT* pResult);
	afx_msg void OnSlistBarDropDown(NMHDR* /*pNMHDR*/, LRESULT* pResult);
	afx_msg void OnSlistDomainRange(UINT nCmdID);
	afx_msg void OnSlistGroupRange(UINT nCmdID);
	afx_msg void OnViewTechRange(UINT nCmdID);
	afx_msg void OnUpdateViewTechRange(CCmdUI* pCmdUI);
	afx_msg void OnOptionAddtogroupRange(UINT nCmdID);
	afx_msg void OnUpdateOptionAddtogroupRange(CCmdUI* pCmdUI);
	afx_msg void OnToggleWorkspBar( );
public:
	afx_msg LRESULT OnUpdateBarMenu( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnNifMessage( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnInitDates( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnUpdateSlistView( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnStkReceiverAlarm( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__BD1B8846_C61A_405F_A998_52C5656A6730__INCLUDED_)
