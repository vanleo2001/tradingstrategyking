// RealTimeView.cpp : implementation of the CRealTimeView class
//

#include "stdafx.h"
#include "..\StaticDoc.h"
#include "RealTimeView.h"

#include "..\memDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRealTimeView

#define	RTV_TIMER_REFRESHBIGTRADE	1
#define	RTV_TIMER_REFRESH			2

#define	IDC_REALTIMEVIEW_REPORTTAB	200

#define	RT_REPORTTAB_QUOTE		0	// 笔
#define	RT_REPORTTAB_PRICE		1	// 价
#define	RT_REPORTTAB_MINUTE		2	// 分
#define	RT_REPORTTAB_BUYSELLEX	3	// 盘
#define	RT_REPORTTAB_VALUE		4	// 值
#define	RT_REPORTTAB_DISTRIBUTE	5	// 筹
#define	RT_REPORTTAB_BIGTRADE	6	// 单


IMPLEMENT_DYNCREATE(CRealTimeView, CView)

BEGIN_MESSAGE_MAP(CRealTimeView, CView)
	//{{AFX_MSG_MAP(CRealTimeView)
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_REALTIME_PREV, OnRealtimePrev)
	ON_COMMAND(ID_REALTIME_NEXT, OnRealtimeNext)
	ON_COMMAND(ID_REALTIME_LEFT, OnRealtimeLeft)
	ON_COMMAND(ID_REALTIME_RIGHT, OnRealtimeRight)
	ON_COMMAND(ID_REALTIME_HOME, OnRealtimeHome)
	ON_COMMAND(ID_REALTIME_END, OnRealtimeEnd)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_MESSAGE(WM_USER_GETVIEWTITLE, OnGetViewTitle)
	ON_MESSAGE(WM_USER_GETVIEWCMDID, OnGetViewCmdid)
	ON_MESSAGE(WM_USER_COLORCHANGE, OnColorChange)
	ON_MESSAGE(WM_APP_STKRECEIVER_DATA, OnStkReceiverData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealTimeView construction/destruction

CRealTimeView::CRealTimeView()
{
	m_nMultiStockCount	=	1;

	ASSERT(RTV_MAX_MULTICOUNT > 0);
}

CRealTimeView::~CRealTimeView()
{
}

void CRealTimeView::ShowMultiStock(int nMultiStockCount)
{
	ASSERT(1 == nMultiStockCount || 2 == nMultiStockCount
			|| 4 == nMultiStockCount || 6 == nMultiStockCount
			|| 9 == nMultiStockCount);
	if(1 != nMultiStockCount && 2 != nMultiStockCount
			&& 4 != nMultiStockCount && 6 != nMultiStockCount
			&& 9 != nMultiStockCount)
		return;
	if(nMultiStockCount == m_nMultiStockCount)
		return;

	m_nMultiStockCount	=	nMultiStockCount;

	int	nDrawMode	=	m_realtime[0].GetDrawMode();
	int	nTechLine	=	m_realtime[0].GetDrawTechLine();
	int	nReportWhat	=	m_realtime[0].GetReportWhat();
	if(1 == m_nMultiStockCount)
	{
		for(int i = 0; i < RTV_MAX_MULTICOUNT; i++)
			m_realtime[i].SetDrawMode(nDrawMode, nTechLine, CRealTime::reportQuote);
		//m_wndReportTab.SetCurtab(RT_REPORTTAB_QUOTE);
	}
	else
	{
		for(int i = 0; i < RTV_MAX_MULTICOUNT; i++)
			m_realtime[i].SetDrawMode(CRealTime::modePriceLine, CRealTime::techNone, CRealTime::reportNone);
	}

	OnUpdate(NULL, UPDATE_HINT_REALTIMEVIEW, NULL);
}

void CRealTimeView::ChangeDrawMode(int nDrawMode)
{
	int nTechLine	=	m_realtime[0].GetDrawTechLine();
	int	nReportWhat	=	m_realtime[0].GetReportWhat();
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].SetDrawMode(nDrawMode, nTechLine, nReportWhat);

	//if(::IsWindow(m_wndReportTab.GetSafeHwnd()))
	//{
	//	if(1 == m_nMultiStockCount && CRealTime::modePriceLine == nDrawMode)
	//		m_wndReportTab.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	//	else
	//		m_wndReportTab.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
	//}

	Invalidate();
}

void CRealTimeView::ToggleDrawTechLine(int nTechLine)
{
	int	nOld	=	m_realtime[0].GetDrawTechLine();
	int nNew	=	(nTechLine == nOld ? CRealTime::techNone : nTechLine);
	int	nDrawMode	=	m_realtime[0].GetDrawMode();
	int	nReportWhat	=	m_realtime[0].GetReportWhat();
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].SetDrawMode(nDrawMode, nNew, /*nReportWhat*/nNew);

	Invalidate();
}

void CRealTimeView::SendRequestQuote(BOOL bForced)
{
	BOOL bInTrade = CSPTime::InTradeTime(CSPTime::GetCurrentTime().GetTime(),900);
	CView * pView = AfxGetStaticDoc()->GetActiveView();
	if(bForced || (bInTrade && pView == this))
	{
		for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		{
			CStock & stock = m_realtime[i].GetCurStock();
			CStockInfo & info = stock.GetStockInfo();
			if(info.IsValidStock())
				AfxGetStkReceiver().RequestStockData(CStock::dataReport, &info, 1, 0, 0);
		}
	}
}

BOOL CRealTimeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CRealTimeView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	OnUpdate(NULL, UPDATE_HINT_REALTIMEVIEW, NULL);
}

void CRealTimeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(UPDATE_HINT_REALTIMEVIEW != lHint)
		return;
	if(!IsWindowVisible())
		return;
	SetFocus();

	ASSERT(RTV_MAX_MULTICOUNT > 0);

	CStockInfo	info;
	if(AfxGetProfile().GetCurrentStock(&info))
	{
		m_realtime[0].SetCurStock(info);
		m_realtime[0].PrepareStockData();
	}
	else
	{
		m_realtime[0].SetCurStock(STKLIB_CODE_MAIN);
		m_realtime[0].PrepareStockData();
		AfxGetProfile().SetCurrentStock(STKLIB_CODE_MAIN, FALSE);
	}
	if(CRealTime::modeReportDetail == m_realtime[0].GetDrawMode())
		AfxGetStkReceiver().RequestStockData(CStock::dataDetail, &info, 1, 0, 0);
	else
		AfxGetStkReceiver().RequestStockData(CStock::dataMinute, &info, 1, 0, 0);

	// 多股同列
	for(int i=1; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
	{
		if(AfxGetProfile().GetNextStock(&info))
		{
			m_realtime[i].SetCurStock(info);
			m_realtime[i].PrepareStockData();
			if(CRealTime::modeReportDetail == m_realtime[0].GetDrawMode())
				AfxGetStkReceiver().RequestStockData(CStock::dataDetail, &info, 1, 0, 0);
			else
				AfxGetStkReceiver().RequestStockData(CStock::dataMinute, &info, 1, 0, 0);
		}
	}
	for(int i=1; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		AfxGetProfile().GetPrevStock(&info);

	Invalidate();

	//if(1 == m_nMultiStockCount && CRealTime::modePriceLine == m_realtime[0].GetDrawMode())
	//	m_wndReportTab.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	//else
	//	m_wndReportTab.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);

	CMainFrame	* pFrame = AfxGetMainFrame();
	if(pFrame)
		pFrame->m_SearchBox.SetCurrentWindowText();

	SendRequestQuote(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CRealTimeView drawing

void CRealTimeView::OnDraw(CDC* pDC)
{
	CStaticDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect rectClient;
	GetClientRect(&rectClient);

	CMemDC memDC(pDC);
	RedrawAll(&memDC, rectClient);

	CRect rcClient(rectClient);
	rcClient.top = rcClient.bottom - 20;

	//memDC.FillSolidRect(rcClient, RGB(0, 0, 0));

	//CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	//memDC.SelectObject(pen);

	//memDC.MoveTo(rcClient.left, rcClient.top);
	//memDC.LineTo(rcClient.right, rcClient.top);

	//memDC.MoveTo(rcClient.left, rcClient.bottom - 1);
	//memDC.LineTo(rcClient.right, rcClient.bottom - 1);

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

void CRealTimeView::RedrawAll(CDC * pDC, CRect rectClient)
{		
	int	i	=	0;
	CRect	rect[RTV_MAX_MULTICOUNT];
	for(i = 0; i < RTV_MAX_MULTICOUNT; i++)
		rect[i]	=	rectClient;
	switch(m_nMultiStockCount)
	{
	case 2:
		rect[0].bottom	=	rect[1].top	=	rectClient.top + rectClient.Height()/2;
		for(i = 0; i < m_nMultiStockCount; i++)
			m_realtime[i].Redraw(pDC, rect[i]);
		break;
	case 4:
		rect[0].bottom	=	rect[2].bottom	=	rect[1].top		=	rect[3].top		=	rectClient.top + rectClient.Height()/2;
		rect[0].right	=	rect[1].right	=	rect[2].left	=	rect[3].left	=	rectClient.left + rectClient.Width()/2;
		for(i = 0; i < m_nMultiStockCount; i++)
			m_realtime[i].Redraw(pDC, rect[i]);
		break;
	case 6:
		rect[0].bottom	=	rect[3].bottom	=	rect[1].top		=	rect[4].top		=	rectClient.top + rectClient.Height()/3;
		rect[1].bottom	=	rect[4].bottom	=	rect[2].top		=	rect[5].top		=	rectClient.top + 2*rectClient.Height()/3;
		rect[0].right	=	rect[1].right	=	rect[2].right	=	rect[3].left	=	rect[4].left	=	rect[5].left	=	rectClient.left + rectClient.Width()/2;
		for(i = 0; i < m_nMultiStockCount; i++)
			m_realtime[i].Redraw(pDC, rect[i]);
		break;
	case 9:
		rect[0].bottom	=	rect[3].bottom	=	rect[6].bottom	=	rect[1].top		=	rect[4].top		=	rect[7].top		=	rectClient.top + rectClient.Height()/3;
		rect[1].bottom	=	rect[4].bottom	=	rect[7].bottom	=	rect[2].top		=	rect[5].top		=	rect[8].top		=	rectClient.top + 2*rectClient.Height()/3;
		rect[0].right	=	rect[1].right	=	rect[2].right	=	rect[3].left	=	rect[4].left	=	rect[5].left	=	rectClient.left + rectClient.Width()/3;
		rect[3].right	=	rect[4].right	=	rect[5].right	=	rect[6].left	=	rect[7].left	=	rect[8].left	=	rectClient.left + 2*rectClient.Width()/3;
		for(i = 0; i < m_nMultiStockCount; i++)
			m_realtime[i].Redraw(pDC, rect[i]);
		break;
	default:
		m_realtime[0].Redraw(pDC, rectClient);	// 单股
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRealTimeView printing

BOOL CRealTimeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRealTimeView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
	CView::OnBeginPrinting(pDC, pInfo);
}

void CRealTimeView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
	CView::OnEndPrinting(pDC, pInfo);
}

void CRealTimeView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add customized printing code here
	pDC->SetMapMode(MM_ISOTROPIC);
	CRect	rectClient(0, 0, 1020, 586);
	// GetClientRect(&rectClient);
	pDC->SetWindowOrg(rectClient.left, rectClient.top);
	pDC->SetWindowExt(rectClient.Width(), rectClient.Height());
	pDC->SetViewportOrg(pInfo->m_rectDraw.left, pInfo->m_rectDraw.top);
	pDC->SetViewportExt(pInfo->m_rectDraw.Width(), pInfo->m_rectDraw.Height());
	
	RedrawAll(pDC, rectClient);
	// CView::OnPrint(pDC,pInfo);
}

void CRealTimeView::SetSelectedItem(CTSKTabManagerItem* pItem)
{
	CTSKTabManager::SetSelectedItem(pItem);
}

BOOL CRealTimeView::OnBeforeItemClick(CTSKTabManagerItem* pItem)
{
	int nCur = pItem->GetIndex();
	int nTechLine = m_realtime[0].GetDrawTechLine();

	switch(nCur)
	{
	case RT_REPORTTAB_QUOTE:
		m_realtime[0].SetDrawMode(CRealTime::modePriceLine, nTechLine, CRealTime::reportQuote);
		break;
	case RT_REPORTTAB_PRICE:
		m_realtime[0].SetDrawMode(CRealTime::modePriceLine, nTechLine, CRealTime::reportPrice);
		break;
	case RT_REPORTTAB_MINUTE:
		m_realtime[0].SetDrawMode(CRealTime::modePriceLine, nTechLine, CRealTime::reportMinute);
		break;
	case RT_REPORTTAB_BUYSELLEX:
		m_realtime[0].SetDrawMode(CRealTime::modePriceLine, nTechLine, CRealTime::reportBuySellEx);
		break;
	case RT_REPORTTAB_VALUE:
		m_realtime[0].SetDrawMode(CRealTime::modePriceLine, nTechLine, CRealTime::reportValue);
		break;
	case RT_REPORTTAB_DISTRIBUTE:
		m_realtime[0].SetDrawMode(CRealTime::modePriceLine, nTechLine, CRealTime::reportDistribute);
		break;
	case RT_REPORTTAB_BIGTRADE:
		m_realtime[0].SetDrawMode(CRealTime::modePriceLine, nTechLine, CRealTime::reportBigTrade);
		break;
	default:;
	}
	m_realtime[0].DrawReportRegion(NULL);

	Invalidate();

	return TRUE;
}

void CRealTimeView::Reposition()
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
// CRealTimeView message handlers

int CRealTimeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CFont font;
	if (!font.CreateStockObject(DEFAULT_GUI_FONT))
		font.CreateStockObject(ANSI_VAR_FONT);
	SetFont(&font);

	CTSKTabManagerItem* pItem;
	CString	strName;

	strName.LoadString(IDS_REALTIMETAB_QUOTE);
	pItem = new CTSKTabManagerItem();
	pItem->SetCaption(strName);
	AddItem(RT_REPORTTAB_QUOTE, pItem);

	strName.LoadString(IDS_REALTIMETAB_PRICE);
	pItem = new CTSKTabManagerItem();
	pItem->SetCaption(strName);
	AddItem(RT_REPORTTAB_PRICE, pItem);

	strName.LoadString(IDS_REALTIMETAB_MINUTE);
	pItem = new CTSKTabManagerItem();
	pItem->SetCaption(strName);
	AddItem(RT_REPORTTAB_MINUTE, pItem);

	strName.LoadString(IDS_REALTIMETAB_BUYSELLEX);
	pItem = new CTSKTabManagerItem();
	pItem->SetCaption(strName);
	AddItem(RT_REPORTTAB_BUYSELLEX, pItem);

	strName.LoadString(IDS_REALTIMETAB_VALUE);
	pItem = new CTSKTabManagerItem();
	pItem->SetCaption(strName);
	AddItem(RT_REPORTTAB_VALUE, pItem);

	strName.LoadString(IDS_REALTIMETAB_DISTRIBUTE);
	pItem = new CTSKTabManagerItem();
	pItem->SetCaption(strName);
	AddItem(RT_REPORTTAB_DISTRIBUTE, pItem);

	strName.LoadString(IDS_REALTIMETAB_BIGTRADE);
	pItem = new CTSKTabManagerItem();
	pItem->SetCaption(strName);
	AddItem(RT_REPORTTAB_BIGTRADE, pItem);

	SetSelectedItem(GetItem(0));

	// m_realtime
	for(int i = 0; i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].SetParent(this);

	// 实时行情刷新
	AfxGetStkReceiver().AddRcvDataWnd(GetSafeHwnd());

	// 大单成交刷新
	SetTimer(RTV_TIMER_REFRESHBIGTRADE, 30000, NULL);
	SetTimer(RTV_TIMER_REFRESH, 30000, NULL);

	return 0;
}

void CRealTimeView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CView::OnWindowPosChanged(lpwndpos);

	if(!(lpwndpos->flags & SWP_NOSIZE) || !(lpwndpos->flags & SWP_NOMOVE)
		|| (lpwndpos->flags & SWP_SHOWWINDOW))
	{
		CRect rect;
		GetClientRect(&rect);
		//if(::IsWindow(m_wndReportTab.GetSafeHwnd()))
		//{
		//	if(1 == m_nMultiStockCount && CRealTime::modePriceLine == m_realtime[0].GetDrawMode())
		//		m_wndReportTab.SetWindowPos(NULL, rect.right-200, rect.bottom-39, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		//	else
		//		m_wndReportTab.SetWindowPos(NULL, rect.right-200, rect.bottom-39, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);
		//}

		m_rcControl.SetRect(rect.right - 200, rect.bottom - 19, rect.right, rect.bottom);
		RepositionTabControl(this, NULL, m_rcControl);
	}

	//Reposition();
}

void CRealTimeView::OnDestroy() 
{
	// 停止行情刷新通知消息
	AfxGetStkReceiver().RemoveRcvDataWnd(GetSafeHwnd());
	KillTimer(RTV_TIMER_REFRESHBIGTRADE);
	KillTimer(RTV_TIMER_REFRESH);

	CView::OnDestroy();
}

void CRealTimeView::OnTimer(UINT nIDEvent) 
{
	if(RTV_TIMER_REFRESHBIGTRADE == nIDEvent)
	{
		int	nDrawMode	=	m_realtime[0].GetDrawMode();
		int	nReportWhat	=	m_realtime[0].GetReportWhat();

		if(CRealTime::modeBigTradeDetail == nDrawMode)
			Invalidate();
		else if(CRealTime::modePriceLine == nDrawMode
			&& CRealTime::reportBigTrade == nReportWhat)
			m_realtime[0].DrawReportRegion(NULL);
	}
	if(RTV_TIMER_REFRESH == nIDEvent)
	{
		SendRequestQuote(FALSE);
	}

	CView::OnTimer(nIDEvent);
}

void CRealTimeView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(bActivate)
		OnUpdate(NULL, UPDATE_HINT_REALTIMEVIEW, NULL);

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL CRealTimeView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

LRESULT CRealTimeView::OnGetViewTitle(WPARAM wParam, LPARAM lParam)
{
	CString	strTitle;
	strTitle.LoadString(IDS_TITLE_REALTIMEVIEW);

	lstrcpyn((LPTSTR)lParam, (LPCTSTR)strTitle, wParam);
	if ((int)wParam > strTitle.GetLength())
		wParam = strTitle.GetLength();
	return wParam;
}

LRESULT CRealTimeView::OnGetViewCmdid(WPARAM wParam, LPARAM lParam)
{
	if(lParam)
		*((LPARAM *)lParam)	=	ID_VIEW_REALTIME;
	return 0L;
}

LRESULT CRealTimeView::OnColorChange(WPARAM wParam, LPARAM lParam)
{
	Invalidate();

	return 0L;
}

LRESULT CRealTimeView::OnStkReceiverData(WPARAM wParam, LPARAM lParam)
{
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].OnStkReceiverData(wParam,lParam);
	return 0;
}

void CRealTimeView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if ((GetStyle() & WS_TABSTOP) && m_pSelected && (m_pSelected == HitTest(point)))
	{
		SetFocus();
	}

	PerformClick(m_hWnd, point);

	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].OnLButtonDown(nFlags, point, IsSHIFTpressed(), IsCTRLpressed());

	CView::OnLButtonDown(nFlags, point);
}

void CRealTimeView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CView::OnLButtonUp(nFlags, point);
}

void CRealTimeView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].OnLButtonDblClk(nFlags, point);
	CView::OnLButtonDblClk(nFlags, point);
}

void CRealTimeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch(nChar)
	{
	case VK_LEFT:
		OnRealtimeLeft();
		break;
	case VK_RIGHT:
		OnRealtimeRight();
		break;
	case VK_UP:
		OnRealtimeUp();
		break;
	case VK_DOWN:
		OnRealtimeDown();
		break;
	case VK_HOME:
		OnRealtimeHome();
		break;
	case VK_END:
		OnRealtimeEnd();
		break;
	case VK_PRIOR:
		OnRealtimePrev();
		break;
	case VK_NEXT:
		OnRealtimeNext();
		break;
	case VK_ESCAPE:
		if (m_realtime[0].GetDrawMode() != CRealTime::modePriceLine)
		{
			ChangeDrawMode(CRealTime::modePriceLine);
		}
		else
		{
			AfxSwitchToStaticView(RUNTIME_CLASS(CSListView));
		}
		for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
			m_realtime[i].ResetIndexCurrent();
		Invalidate();
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CRealTimeView::OnRealtimePrev() 
{
	if(CRealTime::modePriceLine == m_realtime[0].GetDrawMode())
	{
		CStockInfo	info;
		for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
			AfxGetProfile().GetPrevStock(&info);
		OnUpdate(NULL, UPDATE_HINT_REALTIMEVIEW, NULL);
	}
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].PageUp();
}

void CRealTimeView::OnRealtimeNext() 
{
	if(CRealTime::modePriceLine == m_realtime[0].GetDrawMode())
	{
		CStockInfo	info;
		for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
			AfxGetProfile().GetNextStock(&info);
		OnUpdate(NULL, UPDATE_HINT_REALTIMEVIEW, NULL);
	}
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].PageDown();
}

void CRealTimeView::OnRealtimeLeft() 
{
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].MoveLeft();
}

void CRealTimeView::OnRealtimeRight() 
{
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].MoveRight();
}

void CRealTimeView::OnRealtimeUp() 
{
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].MoveUp();
}

void CRealTimeView::OnRealtimeDown() 
{
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].MoveDown();
}

void CRealTimeView::OnRealtimeHome() 
{
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].MoveHome();
}

void CRealTimeView::OnRealtimeEnd() 
{
	for(int i = 0; i < m_nMultiStockCount && i < RTV_MAX_MULTICOUNT; i++)
		m_realtime[i].MoveEnd();
}

