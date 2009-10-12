
#pragma once

#include "RealTime.h"
#include "..\TSKTabManager.h"

#define	RTV_MAX_MULTICOUNT 9

class CRealTimeView : public CView, public CTSKTabManager
{
	DECLARE_DYNCREATE(CRealTimeView)
protected:
	CRealTimeView();
	virtual ~CRealTimeView();

protected:
	int m_nMultiStockCount;
	CRealTime m_realtime[RTV_MAX_MULTICOUNT];

public:
	inline virtual CStaticDoc* GetDocument()   { return (CStaticDoc *)m_pDocument; }

public:
	void	ShowMultiStock( int nMultiStockCount );
	int		GetShowMultiStock( )	{	return m_nMultiStockCount;	}

	void	ChangeDrawMode( int nDrawMode );
	void	ToggleDrawTechLine( int nTechLine );
	int		GetDrawMode( )	{	return m_realtime[0].GetDrawMode();	}
	int		GetDrawTechLine()	{	return m_realtime[0].GetDrawTechLine();	}

	void	SendRequestQuote( BOOL bForced );

public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

protected:
	void	RedrawAll( CDC * pDC, CRect rectClient );

public:
	virtual void SetSelectedItem(CTSKTabManagerItem* pItem);
	virtual BOOL OnBeforeItemClick(CTSKTabManagerItem* pItem);
	virtual void Reposition();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRealtimePrev();
	afx_msg void OnRealtimeNext();
	afx_msg void OnRealtimeLeft();
	afx_msg void OnRealtimeRight();
	afx_msg void OnRealtimeUp();
	afx_msg void OnRealtimeDown();
	afx_msg void OnRealtimeHome();
	afx_msg void OnRealtimeEnd();
	afx_msg LRESULT OnGetViewTitle( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetViewCmdid( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnColorChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnStkReceiverData( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};
