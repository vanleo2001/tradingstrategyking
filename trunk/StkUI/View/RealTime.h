// RealTime.h : interface of the CRealTime class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REALTIME_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_)
#define AFX_REALTIME_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/***
	画图类，负责画出分时行情图中的所有部分
*/
class CRealTime
{
public:
	CRealTime( );
	virtual	~CRealTime( );

	enum	DrawMode {
		modePriceLine		=	0x01,	// 分时价格线
		modeReportDetail	=	0x02,	// 每笔成交明细
		modeMinuteDetail	=	0x03,	// 分钟成交明细
		modeBigTradeDetail	=	0x04,	// 大单成交明细
	};
	enum	TechLine {
		techNone	=	0x00,
		techLBDK	=	0x01,
		techMMLD	=	0x02
	};
	enum	ReportWhat {
		reportNone		=	0x00,
		reportQuote		=	0x01,
		reportPrice		=	0x02,
		reportMinute	=	0x03,
		reportBuySellEx	=	0x04,
		reportValue		=	0x05,
		reportDistribute=	0x06,
		reportBigTrade	=	0x07,
	};
	
	// 设定父窗口，以便在父窗口区域内画图线
	void		SetParent( CWnd * pParent );

	// 设定边框大小
	void		ResetMargins(	int nMarginTop = 19, int nMarginLeft = 60,
								int nMarginCenter = 45, int nMarginBottom = 19,
								int nGapYAxis = 18, int nWidthReport = 200 );
	
	// 清除当前股票及数据
	void		ClearCurStock( );
	
	// 设定当前要画的股票
	BOOL		SetCurStock( const char * szCode );
	BOOL		SetCurStock( CStockInfo & info );
	CStock &	GetCurStock( )	{	return m_CurStock;	}

	BOOL		PrepareStockData( );			// 准备股票数据

	// 画图属性
	void		SetDrawMode( int nDrawMode = modePriceLine, int nTechLine = techNone, int nReportWhat = CRealTime::reportQuote );
	int			GetDrawMode( );
	int			GetDrawTechLine( );
	int			GetReportWhat( );

	// 刷新实时行情数据
	LRESULT		OnStkReceiverData( WPARAM wParam, LPARAM lParam );
	
	// 重画
	void		Redraw( CDC * pDC, CRect rectAll );
	void		DrawReportRegion( CDC * pDC );

public:
	//////////////////////////////////////////////////////////
	// modePriceLine 焦点移动，按左、右键移动
	void		MoveLeft( BOOL bShiftPressed = FALSE );
	void		MoveRight( BOOL bShiftPressed = FALSE );
	void		MoveUp( BOOL bShiftPressed = FALSE );
	void		MoveDown( BOOL bShiftPressed = FALSE );
	void		MoveHome( BOOL bShiftPressed = FALSE );
	void		MoveEnd( BOOL bShiftPressed = FALSE );
	void		MoveTo( int nIndex, BOOL bShiftPressed, BOOL bCtrlPressed );
	void		Move( int nMove, BOOL bShiftPressed, BOOL bCtrlPressed );
	void		OnLButtonDown( UINT nFlags, CPoint point, BOOL bShiftPressed, BOOL bCtrlPressed );
	void		OnLButtonDblClk( UINT nFlags, CPoint point );
	void		ResetIndexCurrent( int nIndexCurrent = -1 );
	//////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////
	// modeReportDetail, modeMinuteDetail, modeBigTradeDetail 明细使用
	void		PageUp( );
	void		PageDown( );
	//////////////////////////////////////////////////////////

protected:
	//////////////////////////////////////////////////////////
	// modePriceLine 分时价格线使用
	void		ResetClient( CRect rectAll );		// 重新设定整图区域
	BOOL		AddIndexCurrent( int nIndex );
	void		OnIndexCurrentChanged( );
	void		DrawSelectionLine( CDC * pDC, BOOL bAddOrRemove );

	void		DrawDateAxis( CDC * pDC );			// 画时间轴
	BOOL		GetMinMaxInfo( double* pdLastClose, double *pdMin, double *pdMax, double *pdMaxVolume, BOOL bUptoAxis );	// 得到最高最低成交价格、最高成交量
	void		DrawPriceVolume( CDC * pDC );		// 画价格线和成交量线
	void		DrawMMLD( CDC * pDC );				// 画买卖力道指标
	void		DrawLBDK( CDC * pDC );				// 画量比多空指标

	int			DrawBuySell( CDC * pDC, int xStart, int yStart, int nWidth );	// 画报价信息
	int			DrawBuySellEx( CDC * pDC, int xStart, int yStart, int nWidth );	// 画扩充盘报价信息
	void		DrawReportQuote( CDC * pDC );		// 画报价信息、成交明细
	void		DrawReportPrice( CDC * pDC );		// 画报价信息、分价表
	void		DrawReportMinute( CDC * pDC );		// 画报价信息、分钟成交明细
	void		DrawReportBuySellEx( CDC * pDC );	// 画报价信息、扩充买卖盘
	void		DrawReportValue( CDC * pDC );		// 画报价信息、价格信息
	void		DrawReportDistribute( CDC * pDC );	// 画成交分布
	void		DrawReportBigTrade( CDC * pDC );	// 画大单成交信息

	BOOL		LoadReportBigTrade( );				// 读取保存的大单成交信息
	//////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////
	// modeReportDetail, modeMinuteDetail, modeBigTradeDetail 明细使用
	int			GetColumnPerPage( CRect rectAll );
	int			GetCountPerColumn( CRect rectAll );
	int			CheckValidStartPos( int nStartPosOld, int nTotalCount, int nPageCount, BOOL bAlignBegin );

	void		DrawReportDetail( CDC * pDC );
	void		DrawReportColumn( CDC * pDC, CStockInfo & info, CReport & aReport, int nStartPos, int nMaxCount, CRect rect );
	
	void		DrawMinuteDetail( CDC * pDC );
	void		DrawMinuteColumn( CDC * pDC, CStockInfo & info, CMinute & aMinute, int nStartPos, int nMaxCount, CRect rect );
	
	void		DrawBigTradeDetail( CDC * pDC );
	void		DrawBigTradeColumn( CDC * pDC, CReport & aMinute, int nStartPos, int nMaxCount, CRect rect );
	//////////////////////////////////////////////////////////

protected:
	// Attributes
	int			m_nMarginTop;		// 上边空白
	int			m_nMarginLeft;		// 左边空白
	int			m_nMarginCenter;	// 中间空白
	int			m_nMarginBottom;	// 下边空白
	int			m_nGapYAxis;		// 纵坐标格线高度
	int			m_nWidthReport;		// 右边信息栏宽度

	CWnd	*	m_pParent;			// 父窗口
	CStock		m_CurStock;			// 当前股票
	CRect		m_rectAll;			// 整个Rect
	int			m_nDrawMode;		// See CRealTime::DrawMode，标记当前画分时价格线、成交明细等

	// modePriceLine use
	CRect		m_rectPrice;		// 价格Rect
	CRect		m_rectVolume;		// 成交量Rect
	CRect		m_rectLBDK;			// 量比(多空)指标
	CRect		m_rectMMLD;			// 买卖力道，总买盘减总卖盘
	CRect		m_rectReport;		// 买卖信息及成交明细Rect
	int			m_nIndexCurrent;	// 当前选中的序号
	CUIntArray	m_anIndexCurrent;	// 当前多个选中的序号
	int			m_nTechLine;		// see enum CRealTime::TechLine
	int			m_nReportWhat;		// 是否画买卖信息及成交明细，多股同列时不画

	// others
	CReport		m_aReportBigTrade;
	int			m_nCurrentStartPos;	// 当前第一个明细数据在其数组中的位置指针
									// modeReportDetail => m_CurStock.GetReport()
									// modeMinuteDetail => m_CurStock.GetMinute()
									// modeBigTradeDetail => m_aReportBigTrade
									// modePriceLine's reportQuote => m_CurStock.GetReport()
									// modePriceLine's reportPrice => m_CurStock.GetMinute().StateDealInfo(...)
									// modePriceLine's reportMinute => m_CurStock.GetMinute()
									// modePriceLine's reportBuySellEx => m_CurStock.GetReport()
									// modePriceLine's reportBigTrade => m_aReportBigTrade
	int			m_nCurrentPageCount;// 当前数据每页显示个数，应用场合
									// modePriceLine's reportQuote
									// modePriceLine's reportPrice
									// modePriceLine's reportMinute
									// modePriceLine's reportBuySellEx
									// modePriceLine's reportBigTrade
	int			m_nCurrentSelectPos;// 当前选中的某一个大单明细数据在m_aReportBigTrade中的位置指针
									// 仅供大单成交明细(modeBigTradeDetail或reportBigTrade)使用

	double		m_dMaxPrice;		// modePriceLine 当前价格线的最高值
	double		m_dMinPrice;		// modePriceLine 当前价格线的最低值
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALTIME_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_)
