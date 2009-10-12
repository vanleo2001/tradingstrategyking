
#pragma once

class CTSKStatusBar;

//=============================================================================
// CTSKStatusBarPane

class CTSKStatusBarPane : public CCmdTarget
{
	DECLARE_DYNAMIC(CTSKStatusBarPane)
public:
	CTSKStatusBarPane();
	virtual ~CTSKStatusBarPane();

protected:
	UINT m_nID;
	int m_cxText;
	UINT m_nStyle;
	UINT m_nFlags;

	int m_nIndex;
	int m_nStatusIndex;
	CTSKStatusBar* m_pStatusBar;

	CRect m_rcMargins;

	friend class CTSKStatusBar;
};

//=============================================================================
// CTSKStatusBar

class CTSKStatusBar : public CControlBar
{
	DECLARE_DYNAMIC(CTSKStatusBar)
public:
	CTSKStatusBar();
	virtual ~CTSKStatusBar();

public:
	BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM, UINT nID = AFX_IDW_STATUS_BAR);
	BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle = 0, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM, UINT nID = AFX_IDW_STATUS_BAR);

public:
	void InitStatusBar();

public:
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

protected:
	afx_msg void OnPaint();
	afx_msg void OnDraw(CDC* pDC);
	DECLARE_MESSAGE_MAP()

	friend class CTSKStatusBarPane;
};
