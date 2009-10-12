
#include "stdafx.h"
#include "TSKTabManager.h"

//=============================================================================
// CTSKTabManagerItem

CTSKTabManagerItem::CTSKTabManagerItem()
{
	m_nIndex = -1;
	m_rcItem.SetRectEmpty();
	m_pTabManager = NULL;
}

CTSKTabManagerItem::~CTSKTabManagerItem()
{
}

void CTSKTabManagerItem::Reposition()
{
	if (m_pTabManager)
		m_pTabManager->Reposition();
}

void CTSKTabManagerItem::SetCaption(LPCTSTR lpszCaption)
{
	if (m_strCaption != lpszCaption)
	{
		m_strCaption = lpszCaption;
		Reposition();
	}
}

void CTSKTabManagerItem::SetRect(CRect rcItem)
{
	m_rcItem = rcItem;
}

//=============================================================================
// CTSKTabManager

CTSKTabManager::CTSKTabManager()
{
	m_rcControl.SetRectEmpty();
	m_nMinWidth = 0;

	m_pSelected = 0;
}

CTSKTabManager::~CTSKTabManager()
{
	DeleteAllItems();
}

void CTSKTabManager::DeleteAllItems()
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CTSKTabManagerItem* pItem = m_arrItems[i];
		delete pItem;
	}

	m_arrItems.RemoveAll();
}

CTSKTabManagerItem* CTSKTabManager::AddItem(int nItem, CTSKTabManagerItem *pItem)
{
	if (!pItem)
		pItem = new CTSKTabManagerItem();

	pItem->m_pTabManager = this;

	if (nItem < 0 || nItem > GetItemCount())
		nItem = GetItemCount();

	m_arrItems.InsertAt(nItem, pItem);

	OnItemsChanged();

	return pItem;
}

CTSKTabManagerItem* CTSKTabManager::HitTest(CPoint point) const
{
	//if (!m_rcControl.PtInRect(point))
	//	return NULL;

	//if (!m_rcHeaderRect.IsRectEmpty() && !m_rcHeaderRect.PtInRect(point))
	//	return NULL;

	for (int i = 0; i < GetItemCount(); i++)
	{
		CTSKTabManagerItem* pItem = GetItem(i);

		if (pItem->GetRect().PtInRect(point)/* && pItem->IsEnabled() && pItem->IsVisible()*/)
		{
			return pItem;
		}
	}

	return NULL;
}

BOOL CTSKTabManager::PerformClick(HWND hWnd, CPoint pt, BOOL bNoItemClick)
{
	//CXTPTabManagerNavigateButton* pNavigateButton = HitTestNavigateButton(pt, FALSE);
	//if (pNavigateButton)
	//{
	//	pNavigateButton->PerformClick(hWnd, pt);
	//	return TRUE;
	//}

	if (bNoItemClick)
		return FALSE;

	CTSKTabManagerItem* pItem = HitTest(pt);

	if (pItem)
	{
		if (!OnBeforeItemClick(pItem))
			return FALSE;

		//if (IsAllowReorder())
		//{
		//	ReOrder(hWnd, pt, pItem);
		//}
		//else if (GetPaintManager()->m_bSelectOnButtonDown)
		{
			OnItemClick(pItem);
		}
		//else
		//{
		//	TrackClick(hWnd, pt, pItem);
		//}
		return TRUE;
	}

	return FALSE;
}

void CTSKTabManager::SetSelectedItem(CTSKTabManagerItem* pItem)
{
	if (m_pSelected != pItem)
	{
		m_pSelected = pItem;
		Reposition();
		//EnsureVisible(pItem);
	}
}

BOOL CTSKTabManager::OnBeforeItemClick(CTSKTabManagerItem* pItem)
{
	return TRUE;
}

void CTSKTabManager::OnItemClick(CTSKTabManagerItem* pItem)
{
	SetSelectedItem(pItem);
}

void CTSKTabManager::OnItemsChanged()
{
	for (int i = 0; i < GetItemCount(); i++)
	{
		GetItem(i)->m_nIndex = i;
	}

	Reposition();
}

void CTSKTabManager::Reposition()
{
}

void CTSKTabManager::RepositionTabControl(CTSKTabManager* pTabManager, CDC* pDC, CRect rcClient)
{
	CRect rc(rcClient);

	CRect rcItem(rc.left, rc.top, rc.left, rc.bottom);

	int nWidth = /*rcClient.Width() / pTabManager->GetItemCount()*/20;
	if (nWidth < m_nMinWidth)
		nWidth = m_nMinWidth;

	for (int i = 0; i < pTabManager->GetItemCount(); i++)
	{
		CTSKTabManagerItem* pItem = pTabManager->GetItem(i);

		rcItem.right = rcItem.left + nWidth;

		pItem->SetRect(rcItem);

		rcItem.left += nWidth;
	}
}
