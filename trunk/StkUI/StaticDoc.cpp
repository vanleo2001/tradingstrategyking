// StaticDoc.cpp : CStaticDoc 类的实现
//

#include "stdafx.h"
#include "StkUI.h"
#include "StaticDoc.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CStaticDoc

IMPLEMENT_DYNCREATE(CStaticDoc, CDocument)

BEGIN_MESSAGE_MAP(CStaticDoc, CDocument)
END_MESSAGE_MAP()

// CStaticDoc 构造/析构

CStaticDoc::CStaticDoc()
{
	//m_bAutoDelete = FALSE;
}

CStaticDoc::~CStaticDoc()
{
}

CStaticDoc* CStaticDoc::CreateNewDocument()
{
	CRuntimeClass* pDocClass = RUNTIME_CLASS(CStaticDoc);
	CStaticDoc* pDocument = (CStaticDoc*)pDocClass->CreateObject();
	if (pDocument == NULL)
	{
		TRACE1("Warning: Dynamic create of document type %hs failed.\n", pDocClass->m_lpszClassName);
		return NULL;
	}
	ASSERT_KINDOF(CStaticDoc, pDocument);

	return pDocument;
}

CView* CStaticDoc::GetViewIfExist(CRuntimeClass* pViewClass)
{
	POSITION pos = GetFirstViewPosition();
	CView* pView = NULL;
	while (pView = GetNextView(pos))
	{
		if (pView && pView->IsKindOf(pViewClass))
		{
			return pView;
		}
	}

	return NULL;
}

CView* CStaticDoc::GetActiveView()
{
	CMainFrame* pMainFrame = AfxGetMainFrame();
	if (pMainFrame)
	{
#ifdef _TSK_MAINFRAME
		CMainFrame* pFrame = pMainFrame;
#else
		CChildFrame* pFrame = DYNAMIC_DOWNCAST(CChildFrame, pMainFrame->MDIGetActive());
#endif
		if (pFrame)
		{
			CView* pView = pFrame->GetActiveView();
			if (pView && pView->GetDocument() == this)
				return pView;
		}
	}

	return NULL;
}

BOOL CStaticDoc::ShowStaticView(CRuntimeClass* pViewClass, BOOL bMaximized)
{
	if (pViewClass == NULL)
		return FALSE;

	static UINT nID = AFX_IDW_PANE_FIRST + 1;

	CFrameWnd* pFrame = NULL;

	POSITION pos = GetFirstViewPosition();
	CView* pView = NULL;
	while (pView = GetNextView(pos))
	{
		if (pView->IsKindOf(pViewClass))
		{
#ifdef _TSK_MAINFRAME
			pFrame = DYNAMIC_DOWNCAST(CFrameWnd, AfxGetMainWnd());
#else
			CChildFrame* pChildFrame = DYNAMIC_DOWNCAST(CChildFrame, pView->GetParentFrame());
			if (pChildFrame)
				pFrame = pChildFrame;
#endif
			ASSERT(pFrame);
			if (pFrame)
				break;
		}
	}

#ifdef _TSK_MAINFRAME
	CCreateContext context;
	context.m_pCurrentDoc = NULL;
	context.m_pCurrentFrame = AfxGetMainFrame();
	context.m_pNewViewClass = pViewClass;

	if (pView == NULL)
	{
		pView = (CView*)AfxGetMainFrame()->CreateView(&context, nID);
		AddView(pView);
		nID++;
	}

	if (pView->IsKindOf(pViewClass))
	{
		CFrameWnd* pFrameWnd = DYNAMIC_DOWNCAST(CFrameWnd, AfxGetMainWnd());

		CView* pOldView = pFrameWnd->GetActiveView();
		if (pOldView == NULL)
		{
			pOldView = DYNAMIC_DOWNCAST(CView, pFrameWnd->GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE));
		}

		if (pOldView != pView)
		{
			UINT temp = ::GetWindowLong(pOldView->m_hWnd, GWL_ID);
			::SetWindowLong(pOldView->m_hWnd, GWL_ID, ::GetWindowLong(pView->m_hWnd, GWL_ID));
			::SetWindowLong(pView->m_hWnd, GWL_ID, temp);

			pOldView->ShowWindow(SW_HIDE);
			pView->ShowWindow(SW_SHOW);

			pFrameWnd->SetActiveView(pView);
			pView->OnInitialUpdate();

			pFrameWnd->RecalcLayout();

			pView->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);
			pFrameWnd->SendMessage(WM_SETMESSAGESTRING, (WPARAM)AFX_IDS_IDLEMESSAGE, 0L);
			pFrameWnd->UpdateWindow();
		}
	}
#else
	if (pFrame == NULL)
	{
		CChildFrame* pChildFrame = CChildFrame::CreateNewFrame(this);
		ASSERT(pChildFrame);

		if (pChildFrame)
		{
			// create view
			CCreateContext context;
			context.m_pCurrentDoc = this;
			context.m_pCurrentFrame = pChildFrame;
			context.m_pLastView = NULL;
			context.m_pNewDocTemplate = NULL;
			context.m_pNewViewClass = pViewClass;

			CView* pNewView = DYNAMIC_DOWNCAST(CView, pChildFrame->CreateView(&context, AFX_IDW_PANE_FIRST));
			if (pNewView)
			{
				pChildFrame->SetActiveView(pNewView);
				pNewView->OnInitialUpdate();
			}

			// if no active child frame, maximize this frame
			CMainFrame* pMainFrame = AfxGetMainFrame();
			if (pMainFrame)
			{
				CFrameWnd* pActiveFrame = AfxGetMainFrame()->GetActiveFrame();
				if (!pActiveFrame || !pActiveFrame->IsKindOf(RUNTIME_CLASS(CChildFrame)))
					pChildFrame->MDIMaximize();
			}

			pFrame = pChildFrame;
		}
	}

	if (pFrame)
	{
		((CChildFrame*)pFrame)->MDIActivate();
		if (bMaximized)
		{
			((CChildFrame*)pFrame)->MDIMaximize();
		}
		else
		{
			pFrame->ShowWindow(SW_SHOW);
		}
	}
#endif

	return TRUE;
}

UINT CStaticDoc::GetIDResource()
{
	return IDR_SIMTYPE;
}

void CStaticDoc::SetTitle(LPCTSTR lpszTitle) 
{
	m_strTitle = lpszTitle;
}

BOOL CStaticDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

// CStaticDoc 序列

void CStaticDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

// CStaticDoc 诊断

#ifdef _DEBUG
void CStaticDoc::AssertValid() const
{
	CDocument::AssertValid();
}
void CStaticDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CStaticDoc 命令
