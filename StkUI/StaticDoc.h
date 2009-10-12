// StaticDoc.h : CStaticDoc 类的接口
//

#pragma once

#define	UPDATE_HINT_WIZARDVIEW			1
#define	UPDATE_HINT_SIMUVIEW			2
#define	UPDATE_HINT_SIMUVIEW_REREALRUN	3
#define	UPDATE_HINT_SLISTVIEW			4
#define	UPDATE_HINT_REALTIMEVIEW		5
#define	UPDATE_HINT_MULTISORTVIEW		6
#define	UPDATE_HINT_GRAPHVIEW			7
#define	UPDATE_HINT_BASEVIEW			8
#define	UPDATE_HINT_INFOVIEW			9
#define	UPDATE_HINT_SELECTORVIEW		10

#define	UPDATE_HINT_GRAPHVIEW_NOTRELOADDATA	10

#define	WM_USER_GETVIEWTITLE		(WM_USER + 1)
#define	WM_USER_GETVIEWCMDID		(WM_USER + 2)
#define	WM_USER_CANCLOSEVIEW		(WM_USER + 3)
#define	WM_USER_COLORCHANGE			(WM_USER + 4)

class CWizardView;
class CChildFrame;

/* ////////////////////////////////////////////////////////////////////////////

	CDocument 类，程序启动时，创建一个对象，主管几个视图的显示

*/

class CStaticDoc : public CDocument
{
	DECLARE_DYNCREATE(CStaticDoc)
protected:
	CStaticDoc();
	virtual ~CStaticDoc();

public:
	static CStaticDoc* CreateNewDocument();

public:
	CView* GetViewIfExist(CRuntimeClass* pViewClass);
	CView* GetActiveView();
	BOOL ShowStaticView(CRuntimeClass* pViewClass, BOOL bMaximized = FALSE);
	UINT GetIDResource();
	void SetTitle(LPCTSTR lpszTitle);

public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};
