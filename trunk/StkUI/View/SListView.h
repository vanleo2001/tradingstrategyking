
#pragma once

#include "..\GridCtrl.h"
#include "..\TSKTabManager.h"

class CStaticDoc;
class CListExportDlg;

class CSListView : public CView, public CTSKTabManager
{
	DECLARE_DYNCREATE(CSListView)
protected:
	CSListView();
	virtual ~CSListView();

protected:
	CGridCtrl	m_Grid;

	int		m_nColSort;
	BOOL	m_bSortAscend;

	BOOL	m_bShouldUpdate;
	BOOL	m_bFirstUpdate;

public:
	inline virtual CStaticDoc* GetDocument() { return (CStaticDoc*)m_pDocument; }
	void			SetFont(LPLOGFONT pLogFont);

public:
	void			GetSelectedStocks(CSPStringArray & astr);
	void			ExportList(CListExportDlg * pDlg);
	
	void			RemoveSortSign();
	void			SetSortSign();
	void			SetSortVariant(UINT nVariant);
	virtual	void	SetCurrentStatus(int nType, LPCTSTR lpszDomain, DWORD dwDate);
	void			StoreColumnOrderArray();
	
	virtual	void	ResetColumns();
	BOOL			SetAverageItem(CGridCtrl &grid, CStockContainer &container, BOOL bRedraw);
	void			OnDblclkItem(int nStockIndex);

	void			StockInfoChanged(LONG id, CStockInfo & info);
	void			SendRequestQuote(BOOL bForced);

public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnDraw(CDC* pDC);

protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

public:
	virtual void SetSelectedItem(CTSKTabManagerItem* pItem);
	virtual BOOL OnBeforeItemClick(CTSKTabManagerItem* pItem);
	virtual void Reposition();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSlistExportlist();
	afx_msg LRESULT OnGetViewTitle(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetViewCmdid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStkReceiverData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
