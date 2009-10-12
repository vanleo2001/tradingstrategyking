
#include "stdafx.h"
#include "../StaticDoc.h"
#include "GraphView.h"

#include "..\MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphView

#define	IDC_GRAPHVIEW_REPORTTAB	200

#define	GV_REPORTTAB_PARAM			0	// 参数
#define	GV_REPORTTAB_COST			1	// 成本
#define	GV_REPORTTAB_FLAME			2	// 火焰山
#define	GV_REPORTTAB_ACTIVITY		3	// 活跃度

IMPLEMENT_DYNCREATE(CGraphView, CView)

BEGIN_MESSAGE_MAP(CGraphView, CView)
	//{{AFX_MSG_MAP(CGraphView)
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_VIEW_PDAY, OnViewPday)
	ON_COMMAND(ID_VIEW_PWEEK, OnViewPweek)
	ON_COMMAND(ID_VIEW_PMONTH, OnViewPmonth)
	ON_COMMAND(ID_VIEW_PMIN5, OnViewPmin5)
	ON_COMMAND(ID_VIEW_PMIN15, OnViewPmin15)
	ON_COMMAND(ID_VIEW_PMIN30, OnViewPmin30)
	ON_COMMAND(ID_VIEW_PMIN60, OnViewPmin60)
	ON_COMMAND(ID_VIEW_LEFT, OnViewLeft)
	ON_COMMAND(ID_VIEW_RIGHT, OnViewRight)
	ON_COMMAND(ID_VIEW_PAGELEFT, OnViewPageleft)
	ON_COMMAND(ID_VIEW_PAGERIGHT, OnViewPageright)
	ON_COMMAND(ID_VIEW_HOME, OnViewHome)
	ON_COMMAND(ID_VIEW_END, OnViewEnd)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_COMMAND(ID_VIEW_PREV, OnViewPrev)
	ON_COMMAND(ID_VIEW_NEXT, OnViewNext)
	ON_COMMAND(ID_VIEW_INSERT, OnViewInsert)
	ON_COMMAND(ID_VIEW_NORESUMEDR, OnViewNoresumedr)
	ON_COMMAND(ID_VIEW_RESUMEDRUP, OnViewResumedrup)
	ON_COMMAND(ID_VIEW_RESUMEDRDOWN, OnViewResumedrdown)
	ON_COMMAND(ID_VIEW_MAINDATACLOSE, OnViewMaindataclose)
	ON_COMMAND(ID_VIEW_MAINDATAOPEN, OnViewMaindataopen)
	ON_COMMAND(ID_VIEW_MAINDATAAVERAGE, OnViewMaindataaverage)
	ON_COMMAND(ID_VIEW_KLINECANDLE, OnViewKlinecandle)
	ON_COMMAND(ID_VIEW_KLINEAMERICA, OnViewKlineamerica)
	ON_COMMAND(ID_VIEW_KLINETOWER, OnViewKlinetower)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PDAY, OnUpdateViewPday)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PWEEK, OnUpdateViewPweek)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PMONTH, OnUpdateViewPmonth)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PMIN5, OnUpdateViewPmin5)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PMIN15, OnUpdateViewPmin15)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PMIN30, OnUpdateViewPmin30)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PMIN60, OnUpdateViewPmin60)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NORESUMEDR, OnUpdateViewNoresumedr)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESUMEDRUP, OnUpdateViewResumedrup)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESUMEDRDOWN, OnUpdateViewResumedrdown)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAINDATACLOSE, OnUpdateViewMaindataclose)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAINDATAOPEN, OnUpdateViewMaindataopen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAINDATAAVERAGE, OnUpdateViewMaindataaverage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_KLINECANDLE, OnUpdateViewKlinecandle)
	ON_UPDATE_COMMAND_UI(ID_VIEW_KLINEAMERICA, OnUpdateViewKlineamerica)
	ON_UPDATE_COMMAND_UI(ID_VIEW_KLINETOWER, OnUpdateViewKlinetower)
	ON_COMMAND(ID_VIEW_HIDETHISTECH, OnViewHidethistech)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HIDETHISTECH, OnUpdateViewHidethistech)
	ON_COMMAND(ID_VIEW_THISTECHPARAM, OnViewThistechparam)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THISTECHPARAM, OnUpdateViewThistechparam)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND_RANGE(ID_VIEW_TECH_START, ID_VIEW_TECH_END, OnViewTechRange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_TECH_START, ID_VIEW_TECH_END, OnUpdateViewTechRange)
	ON_MESSAGE(WM_USER_GETVIEWTITLE, OnGetViewTitle)
	ON_MESSAGE(WM_USER_GETVIEWCMDID, OnGetViewCmdid)
	ON_MESSAGE(WM_USER_COLORCHANGE, OnColorChange)
	ON_MESSAGE(WM_USER_MYRBUTTONDOWN, OnMyRButtonDown)
	ON_MESSAGE(WM_APP_STKRECEIVER_DATA, OnStkReceiverData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphView construction/destruction

CGraphView::CGraphView()
{
}

CGraphView::~CGraphView()
{
}

void CGraphView::ResetClientRect()
{
	CRect rectClient;
	GetClientRect(&rectClient);
	m_graph.ResetClient(rectClient);
}

void CGraphView::ChangeKType(int ktype)
{
	m_graph.SetKType(ktype);
	m_graph.ResetIndex();
	Invalidate();
}

void CGraphView::ChangeKFormat(int kformat)
{
	m_graph.SetKFormat(kformat);
	m_graph.ResetIndex();
	Invalidate();
}

void CGraphView::OnToggleKType()
{
	int ktype = m_graph.GetKType();
	ktype	++;
	if (ktype > CKData::ktypeMax)
		ktype	=	CKData::ktypeMin;
	ChangeKType(ktype);
}

void CGraphView::OnToggleKFormat(int kformat)
{
	int nOld = m_graph.GetKFormat();
	if (nOld == kformat)
		ChangeKFormat(CKData::formatOriginal);
	else
		ChangeKFormat(kformat);
}

void CGraphView::ChangeMaindataType(int maindatatype)
{
	m_graph.SetMaindataType(maindatatype);
	m_graph.ResetIndex();
	Invalidate();
}

void CGraphView::ChangeKLineMode(int klineMode)
{
	m_graph.SetKLineMode(klineMode);
	m_graph.ResetIndex();
	Invalidate();
}

void CGraphView::OnTechParametersChange()
{
	m_graph.ClearTechniqueCache();
}

BOOL CGraphView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CGraphView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	OnUpdate(NULL, UPDATE_HINT_GRAPHVIEW, NULL);
}

void CGraphView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (UPDATE_HINT_GRAPHVIEW != lHint && UPDATE_HINT_GRAPHVIEW_NOTRELOADDATA != lHint)
		return;
	if (!IsWindowVisible())
		return;
	SetFocus();

	CStockInfo	info;
	if (AfxGetProfile().GetCurrentStock(&info))
	{
		m_graph.SetCurStock(info);
		m_graph.PrepareStockData(UPDATE_HINT_GRAPHVIEW_NOTRELOADDATA != lHint);
	}
	else
	{
		m_graph.SetCurStock(STKLIB_CODE_MAIN);
		m_graph.PrepareStockData(UPDATE_HINT_GRAPHVIEW_NOTRELOADDATA != lHint);
		AfxGetProfile().SetCurrentStock(STKLIB_CODE_MAIN, FALSE);
	}

	Invalidate();

	CMainFrame	* pFrame = AfxGetMainFrame();
	if (pFrame)
		pFrame->m_SearchBox.SetCurrentWindowText();
}

BOOL CGraphView::PreTranslateMessage(MSG* pMsg) 
{
	if (WM_KEYDOWN == pMsg->message)
	{
		switch(pMsg->wParam)
		{
		case VK_F5:
			{
				AfxSwitchToStaticView(RUNTIME_CLASS(CRealTimeView));
				CRealTimeView * pView = AfxGetRealTimeView();
				if (pView)
					pView->ChangeDrawMode(CRealTime::modePriceLine);
			}
			return TRUE;
		}
	}
	
	return CView::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CGraphView drawing

void CGraphView::OnDraw(CDC* pDC)
{
	CStaticDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect rectClient;
	GetClientRect(&rectClient);

	CMemDC memDC(pDC);
	m_graph.Redraw(&memDC, rectClient);

	CRect rcClient(rectClient);
	rcClient.top = rcClient.bottom - 20;

	memDC.SetBkMode(TRANSPARENT);

	//for (int i = /*pTabManager->*/GetItemCount() - 1; i >= 0; i--)
	for (int i = 0; i < GetItemCount(); i++)
	{
		CTSKTabManagerItem* pItem = /*pTabManager->*/GetItem(i);

		CRect rcItem = pItem->GetRect();

		if (CRect().IntersectRect(rcClient, rcItem) && !rcItem.IsRectEmpty())
		{
			//DrawSingleButton(pDC, pItem);
			memDC.Draw3dRect(rcItem, RGB(255, 0, 0), RGB(255, 0, 0));

			CRect rc(rcItem);
			rc.DeflateRect(1, 1, 1, 1);

			if (pItem->IsSelected())
				memDC.FillSolidRect(rc, RGB(50, 50, 50));

			memDC.SetTextColor(RGB(255, 0, 0));
			CString strItem = pItem->GetCaption();
			memDC.DrawText(strItem, rcItem, DT_CENTER);
		}
	}
}

void CGraphView::SetSelectedItem(CTSKTabManagerItem* pItem)
{
	CTSKTabManager::SetSelectedItem(pItem);
}

BOOL CGraphView::OnBeforeItemClick(CTSKTabManagerItem* pItem)
{
	int nCur = pItem->GetIndex();

	switch(nCur)
	{
	case GV_REPORTTAB_PARAM:
		m_graph.SetReportWhat(CStockGraph::reportParam);
		break;
	case GV_REPORTTAB_COST:
		m_graph.SetReportWhat(CStockGraph::reportCost);
		break;
	case GV_REPORTTAB_FLAME:
		m_graph.SetReportWhat(CStockGraph::reportFlame);
		break;
	case GV_REPORTTAB_ACTIVITY:
		m_graph.SetReportWhat(CStockGraph::reportActivity);
		break;
	default:;
	}

	m_graph.DrawReportRegion(NULL);

	Invalidate();

	return TRUE;
}

void CGraphView::Reposition()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC* pDC;
	pDC = GetDC();

	rcClient.top = rcClient.bottom - 20;
	rcClient.SetRect(0, 0, 200, 20);
	RepositionTabControl(this, pDC, m_rcControl);
}

/////////////////////////////////////////////////////////////////////////////
// CGraphView message handlers

int CGraphView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CFont font;
	if (!font.CreateStockObject(DEFAULT_GUI_FONT))
		font.CreateStockObject(ANSI_VAR_FONT);
	SetFont(&font);

	m_graph.SetParent(this);

	// tab
	//if (!m_wndReportTab.Create(WS_VISIBLE|WS_CHILD,CRect(0,0,180,19),this,IDC_GRAPHVIEW_REPORTTAB))
	//	return -1;

	CTSKTabManagerItem* pItem;
	CString	strName;

	strName.LoadString(IDS_GRAPHTAB_PARAM);
	pItem = new CTSKTabManagerItem();
	pItem->SetCaption(strName);
	AddItem(GV_REPORTTAB_PARAM, pItem);

	strName.LoadString(IDS_GRAPHTAB_COST);
	pItem = new CTSKTabManagerItem();
	pItem->SetCaption(strName);
	AddItem(GV_REPORTTAB_COST, pItem);

	strName.LoadString(IDS_GRAPHTAB_FLAME);
	pItem = new CTSKTabManagerItem();
	pItem->SetCaption(strName);
	AddItem(GV_REPORTTAB_FLAME, pItem);

	strName.LoadString(IDS_GRAPHTAB_ACTIVITY);
	pItem = new CTSKTabManagerItem();
	pItem->SetCaption(strName);
	AddItem(GV_REPORTTAB_ACTIVITY, pItem);

	SetSelectedItem(GetItem(0));

	m_nMinWidth = 40;

	//m_wndReportTab.SetColorTabs(AfxGetProfile().GetColor(CColorClass::clrGraphBK));
	//m_wndReportTab.SetSelectTabColor(AfxGetProfile().GetColor(CColorClass::clrGraphBK),
	//								AfxGetProfile().GetColor(CColorClass::clrTitle));
	//m_wndReportTab.SetAutoAjust(FALSE);
	//m_wndReportTab.ShowButtonClose(FALSE);
	//m_wndReportTab.SetALingTabs(CGuiTabWnd::ALN_BOTTOM);

	// 实时行情刷新
	AfxGetStkReceiver().AddRcvDataWnd(GetSafeHwnd());

	return 0;
}

void CGraphView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CView::OnWindowPosChanged(lpwndpos);

	if (!(lpwndpos->flags & SWP_NOSIZE) || !(lpwndpos->flags & SWP_NOMOVE)
		|| (lpwndpos->flags & SWP_SHOWWINDOW))
	{
		CRect rect;
		GetClientRect(&rect);
		//if (::IsWindow(m_wndReportTab.GetSafeHwnd()))
		//	m_wndReportTab.SetWindowPos(NULL, rect.right-172, rect.bottom-19, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

		m_rcControl.SetRect(rect.right - 172, rect.bottom - 19, rect.right, rect.bottom);
		RepositionTabControl(this, NULL, m_rcControl);
	}
}

void CGraphView::OnDestroy() 
{
	AfxGetStkReceiver().RemoveRcvDataWnd(GetSafeHwnd());

	CView::OnDestroy();
}

void CGraphView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	TRACE(_T("CGraphView::OnActivateView %d %d %d\n"), bActivate, pActivateView, pDeactiveView);

	if (bActivate)
		OnUpdate(NULL, UPDATE_HINT_GRAPHVIEW_NOTRELOADDATA, NULL);

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL CGraphView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	if (pDC && pDC->IsKindOf(RUNTIME_CLASS(CDC)))
	{
		CRect rect;
		GetClientRect(&rect);
		CBrush brush;
		brush.CreateSolidBrush(AfxGetProfile().GetColor(CColorClass::clrGraphBK));
		pDC->FillRect(&rect, &brush);

		return TRUE;
	}

	return CView::OnEraseBkgnd(pDC);
}

LRESULT CGraphView::OnGetViewTitle(WPARAM wParam, LPARAM lParam)
{
	CString	strTitle;
	strTitle.LoadString(IDS_TITLE_GRAPHVIEW);

	lstrcpyn((LPTSTR)lParam, (LPCTSTR)strTitle, wParam);
	if ((int)wParam > strTitle.GetLength())
		wParam = strTitle.GetLength();

	return wParam;
}

LRESULT CGraphView::OnGetViewCmdid(WPARAM wParam, LPARAM lParam)
{
	if (lParam)
		*((LPARAM *)lParam)	=	ID_VIEW_GRAPH;
	return 0L;
}

LRESULT CGraphView::OnColorChange(WPARAM wParam, LPARAM lParam)
{
	//m_wndReportTab.SetColorTabs(AfxGetProfile().GetColor(CColorClass::clrGraphBK));
	//m_wndReportTab.SetSelectTabColor(AfxGetProfile().GetColor(CColorClass::clrGraphBK),
	//								AfxGetProfile().GetColor(CColorClass::clrTitle));

	Invalidate();
	return 0L;
}

LRESULT CGraphView::OnStkReceiverData(WPARAM wParam, LPARAM lParam)
{
	return m_graph.OnStkReceiverData(wParam,lParam);
}

void CGraphView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if ((GetStyle() & WS_TABSTOP) && m_pSelected && (m_pSelected == HitTest(point)))
	{
		SetFocus();
	}

	PerformClick(m_hWnd, point);

	m_graph.OnLButtonDown(nFlags, point, IsSHIFTpressed(), IsCTRLpressed());

	CView::OnLButtonDown(nFlags, point);
}

void CGraphView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_graph.OnLButtonUp(nFlags, point, IsSHIFTpressed(), IsCTRLpressed());
	CView::OnLButtonUp(nFlags, point);
}

void CGraphView::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_graph.OnMouseMove(nFlags, point);
	CView::OnMouseMove(nFlags, point);
}

void CGraphView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	m_graph.OnLButtonDblClk(nFlags, point, IsSHIFTpressed(), IsCTRLpressed());
	CView::OnLButtonDblClk(nFlags, point);
}

LRESULT CGraphView::OnMyRButtonDown(WPARAM wParam, LPARAM lParam) 
{
	UINT nFlags = wParam;
	CPoint point((lParam & 0x0000ffff), ((lParam & 0xffff0000) >> 16));
	m_graph.OnRButtonDown(nFlags, point, IsSHIFTpressed(), IsCTRLpressed());
	return 0L;
}

void CGraphView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch(nChar)
	{
	case VK_RETURN:
		m_graph.OnReturn(IsSHIFTpressed(), IsCTRLpressed());
		break;
	case VK_UP:
		m_graph.ExpandUp(IsSHIFTpressed());
		break;
	case VK_DOWN:
		m_graph.ExpandDown(IsSHIFTpressed());
		break;
	case VK_LEFT:
		if (IsCTRLpressed())
			m_graph.PageLeft(IsSHIFTpressed());
		else
			m_graph.MoveLeft(IsSHIFTpressed());
		break;
	case VK_RIGHT:
		if (IsCTRLpressed())
			m_graph.PageRight(IsSHIFTpressed());
		else
			m_graph.MoveRight(IsSHIFTpressed());
		break;
	case VK_HOME:
		m_graph.MoveHome(IsSHIFTpressed());
		break;
	case VK_END:
		m_graph.MoveEnd(IsSHIFTpressed());
		break;
	case VK_PRIOR:
		OnViewPrev();
		break;
	case VK_NEXT:
		OnViewNext();
		break;
	case VK_F8:
		OnToggleKType();
		break;
	case VK_ESCAPE:
		if (m_graph.GetIndexCurrent() < 0)
			AfxSwitchToStaticView(RUNTIME_CLASS(CSListView));
		m_graph.ResetIndexCurrent();
		Invalidate();
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGraphView::OnViewPday()	{	ChangeKType(CKData::ktypeDay);	}
void CGraphView::OnViewPweek()	{	ChangeKType(CKData::ktypeWeek);	}
void CGraphView::OnViewPmonth()	{	ChangeKType(CKData::ktypeMonth);	}
void CGraphView::OnViewPmin5()	{	ChangeKType(CKData::ktypeMin5);	}
void CGraphView::OnViewPmin15()	{	ChangeKType(CKData::ktypeMin15);	}
void CGraphView::OnViewPmin30()	{	ChangeKType(CKData::ktypeMin30);	}
void CGraphView::OnViewPmin60()	{	ChangeKType(CKData::ktypeMin60);	}

void CGraphView::OnUpdateViewPday(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::ktypeDay == m_graph.GetKType());		}
void CGraphView::OnUpdateViewPweek(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::ktypeWeek == m_graph.GetKType());	}
void CGraphView::OnUpdateViewPmonth(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::ktypeMonth == m_graph.GetKType());	}
void CGraphView::OnUpdateViewPmin5(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::ktypeMin5 == m_graph.GetKType());	}
void CGraphView::OnUpdateViewPmin15(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::ktypeMin15 == m_graph.GetKType());	}
void CGraphView::OnUpdateViewPmin30(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::ktypeMin30 == m_graph.GetKType());	}
void CGraphView::OnUpdateViewPmin60(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::ktypeMin60 == m_graph.GetKType());	}

void CGraphView::OnViewLeft()		{	m_graph.MoveLeft();	}
void CGraphView::OnViewRight()		{	m_graph.MoveRight();	}
void CGraphView::OnViewPageleft()	{	m_graph.PageLeft();	}
void CGraphView::OnViewPageright()	{	m_graph.PageRight();	}
void CGraphView::OnViewHome()		{	m_graph.MoveHome();	}
void CGraphView::OnViewEnd()		{	m_graph.MoveEnd();		}

void CGraphView::OnViewZoomin()		{	m_graph.ExpandUp();	}
void CGraphView::OnViewZoomout()	{	m_graph.ExpandDown();	}

void CGraphView::OnViewPrev() 
{
	CStockInfo	info;
	if (AfxGetProfile().GetPrevStock(&info))
		OnUpdate(NULL, UPDATE_HINT_GRAPHVIEW, NULL);
}
void CGraphView::OnViewNext() 
{
	CStockInfo	info;
	if (AfxGetProfile().GetNextStock(&info))
		OnUpdate(NULL, UPDATE_HINT_GRAPHVIEW, NULL);
}
void CGraphView::OnViewInsert()
{
	m_graph.OnInsert();
}

void CGraphView::OnViewTechRange(UINT nCmdID)
{
	if (nCmdID < ID_VIEW_TECH_START || nCmdID > ID_VIEW_TECH_END)
	{
		ASSERT(FALSE);
		return;
	}

	UINT	nID	=	nCmdID - ID_VIEW_TECH_START;
	if (nID > STT_MAX)
		nID = nID-STT_MAX-1+STT_USER_MIN;
	AfxGetProfile().ToggleGraphViewTech(nID);

	CRect	rectClient;
	GetClientRect(&rectClient);
	m_graph.ResetClient(rectClient);
	Invalidate();
}

void CGraphView::OnUpdateViewTechRange(CCmdUI* pCmdUI)
{
	UINT	nCmdID	=	pCmdUI->m_nID;
	if (nCmdID < ID_VIEW_TECH_START || nCmdID > ID_VIEW_TECH_END)
	{
		ASSERT(FALSE);
		return;
	}

	UINT	nID	=	nCmdID - ID_VIEW_TECH_START;
	if (nID > STT_MAX)
		nID = nID-STT_MAX-1+STT_USER_MIN;
	pCmdUI->SetCheck(AfxGetProfile().IsGraphTechShow(nID));
}

// K Format
void CGraphView::OnViewNoresumedr()	{	OnToggleKFormat(CKData::formatOriginal);	}
void CGraphView::OnViewResumedrup()	{	OnToggleKFormat(CKData::formatXDRup);		}
void CGraphView::OnViewResumedrdown(){	OnToggleKFormat(CKData::formatXDRdown);		}

void CGraphView::OnUpdateViewNoresumedr(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck(CKData::formatOriginal == m_graph.GetKFormat());	}
void CGraphView::OnUpdateViewResumedrup(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck(CKData::formatXDRup == m_graph.GetKFormat());	}
void CGraphView::OnUpdateViewResumedrdown(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::formatXDRdown == m_graph.GetKFormat());	}

// Main data Type
void CGraphView::OnViewMaindataclose(){	ChangeMaindataType(CKData::mdtypeClose);	}
void CGraphView::OnViewMaindataopen(){	ChangeMaindataType(CKData::mdtypeOpen);		}
void CGraphView::OnViewMaindataaverage(){ChangeMaindataType(CKData::mdtypeAverage);	}

void CGraphView::OnUpdateViewMaindataclose(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::mdtypeClose == m_graph.GetMaindataType());	}
void CGraphView::OnUpdateViewMaindataopen(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CKData::mdtypeOpen == m_graph.GetMaindataType());	}
void CGraphView::OnUpdateViewMaindataaverage(CCmdUI* pCmdUI){	pCmdUI->SetCheck(CKData::mdtypeAverage == m_graph.GetMaindataType());	}

// kline Mode
void CGraphView::OnViewKlinecandle()	{	ChangeKLineMode(CStockGraph::klineCandle);	}
void CGraphView::OnViewKlineamerica()	{	ChangeKLineMode(CStockGraph::klineAmerica);	}
void CGraphView::OnViewKlinetower()		{	ChangeKLineMode(CStockGraph::klineTower);	}

void CGraphView::OnUpdateViewKlinecandle(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CStockGraph::klineCandle == m_graph.GetKLineMode());		}
void CGraphView::OnUpdateViewKlineamerica(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck(CStockGraph::klineAmerica == m_graph.GetKLineMode());		}
void CGraphView::OnUpdateViewKlinetower(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck(CStockGraph::klineTower == m_graph.GetKLineMode());		}


void CGraphView::OnViewHidethistech() 
{
	m_graph.HideSelectedTech();
}

void CGraphView::OnUpdateViewHidethistech(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(STT_INVALID != m_graph.GetSelectedTech());
}

void CGraphView::OnViewThistechparam() 
{
	m_graph.AdjustSelectedTech();
}

void CGraphView::OnUpdateViewThistechparam(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(STT_INVALID != m_graph.GetSelectedTech());
}
