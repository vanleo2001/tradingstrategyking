
#include "stdafx.h"
#include "StkUI.h"
#include "TSKStatusBar.h"

//=============================================================================
// CTSKStatusBarPane

IMPLEMENT_DYNAMIC(CTSKStatusBarPane, CCmdTarget)

CTSKStatusBarPane::CTSKStatusBarPane()
{
	m_nID = 0;
	m_cxText = 0;
	m_nStyle = 0;
	m_nFlags = 0;

	m_nIndex = -1;
	m_nStatusIndex = -1;
	m_pStatusBar = NULL;

	m_rcMargins.SetRect(2, 1, 2, 1);
}

CTSKStatusBarPane::~CTSKStatusBarPane()
{
}

//=============================================================================
// CTSKStatusBar

IMPLEMENT_DYNAMIC(CTSKStatusBar, CControlBar)

CTSKStatusBar::CTSKStatusBar()
{
}

CTSKStatusBar::~CTSKStatusBar()
{
}

BEGIN_MESSAGE_MAP(CTSKStatusBar, CControlBar)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CTSKStatusBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{
	return CreateEx(pParentWnd, 0, dwStyle, nID);
}

BOOL CTSKStatusBar::CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle, DWORD dwStyle, UINT nID)
{
	ASSERT_VALID(pParentWnd);   // must have a parent

	// save the style (some of these style bits are MFC specific)
	m_dwStyle = (dwStyle & CBRS_ALL);

	// translate MFC style bits to windows style bits
	dwStyle &= ~CBRS_ALL;
	dwStyle |= CCS_NOPARENTALIGN|CCS_NOMOVEY|CCS_NODIVIDER|CCS_NORESIZE;
	if (pParentWnd->GetStyle() & WS_THICKFRAME)
		dwStyle |= SBARS_SIZEGRIP;
	dwStyle |= dwCtrlStyle;

	// initialize common controls
	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_BAR_REG));

	// create the HWND
	CRect rect;
	rect.SetRectEmpty();

	return CWnd::Create(STATUSCLASSNAME, NULL, dwStyle, rect, pParentWnd, nID);
}

void CTSKStatusBar::InitStatusBar()
{
}

CSize CTSKStatusBar::CalcFixedLayout(BOOL, BOOL bHorz)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	// determinme size of font being used by the status bar
	TEXTMETRIC tm;
	{
		CClientDC dc(NULL);
		HFONT hFont = (HFONT)SendMessage(WM_GETFONT);
		HGDIOBJ hOldFont = NULL;
		if (hFont != NULL)
			hOldFont = dc.SelectObject(hFont);
		VERIFY(dc.GetTextMetrics(&tm));
		if (hOldFont != NULL)
			dc.SelectObject(hOldFont);
	}

	// get border information
	CRect rect; rect.SetRectEmpty();
	CalcInsideRect(rect, bHorz);
	int rgBorders[3];
	DefWindowProc(SB_GETBORDERS, 0, (LPARAM)rgBorders);

	// determine size, including borders
	CSize size;
	size.cx = 32767;
	size.cy = tm.tmHeight - tm.tmInternalLeading - 1
		+ rgBorders[1] * 2 + ::GetSystemMetrics(SM_CYBORDER) * 2
		- rect.Height();
	//if (size.cy < m_nMinHeight)
	//	size.cy = m_nMinHeight;

	return size;
}

void CTSKStatusBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
}

void CTSKStatusBar::OnPaint()
{
	CPaintDC dc(this);

	OnDraw(&dc);
}

void CTSKStatusBar::OnDraw(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	pDC->FillSolidRect(rcClient, RGB(0, 0, 0));
}
