
#pragma once

class CTSKTabManager;

//=============================================================================
// CTSKTabManagerItem

class CTSKTabManagerItem
{
public:
	CTSKTabManagerItem();
	virtual ~CTSKTabManagerItem();

protected:
	int m_nIndex;
	CRect m_rcItem;
	CString m_strCaption;

	CTSKTabManager* m_pTabManager;

public:
	void Reposition();

public:
	CString GetCaption() const;
	void SetCaption(LPCTSTR lpszCaption);
	CRect GetRect() const;
	void SetRect(CRect rcItem);
	int GetIndex() const;

public:
	BOOL IsSelected() const;

	friend class CTSKTabManager;
};

//=============================================================================
// CTSKTabManager

class CTSKTabManager
{
public:
	CTSKTabManager();
	virtual ~CTSKTabManager();

protected:
	CArray<CTSKTabManagerItem*, CTSKTabManagerItem*> m_arrItems;

	CRect m_rcControl;
	int m_nMinWidth;

	CTSKTabManagerItem* m_pSelected;

public:
	void DeleteAllItems();
	CTSKTabManagerItem* AddItem(int nItem, CTSKTabManagerItem* pItem = NULL);
	CTSKTabManagerItem* GetItem(int nIndex) const;
	int GetItemCount() const;
	CTSKTabManagerItem* HitTest(CPoint point) const;
	BOOL PerformClick(HWND hWnd, CPoint pt, BOOL bNoItemClick = FALSE);

protected:
	virtual void SetSelectedItem(CTSKTabManagerItem* pItem);
	virtual BOOL OnBeforeItemClick(CTSKTabManagerItem* pItem);
	virtual void OnItemClick(CTSKTabManagerItem* pItem);

public:
	virtual void OnItemsChanged();
	virtual void Reposition();

public:
	virtual void RepositionTabControl(CTSKTabManager* pTabManager, CDC* pDC, CRect rcClient);

	friend class CTSKTabManagerItem;
};

AFX_INLINE CString CTSKTabManagerItem::GetCaption() const {
	return m_strCaption;
}

AFX_INLINE CRect CTSKTabManagerItem::GetRect() const {
	return m_rcItem;
}

AFX_INLINE int CTSKTabManagerItem::GetIndex() const {
	return m_nIndex;
}

AFX_INLINE BOOL CTSKTabManagerItem::IsSelected() const {
	return m_pTabManager->m_pSelected == this;
}

AFX_INLINE int CTSKTabManager::GetItemCount() const {
	return (int)m_arrItems.GetSize();
}

AFX_INLINE CTSKTabManagerItem* CTSKTabManager::GetItem(int nIndex) const {
	return nIndex >= 0 && nIndex < GetItemCount() ? m_arrItems[nIndex] : NULL;
}
