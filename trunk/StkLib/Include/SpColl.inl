
////////////////////////////////////////////////////////////////////////////
// CSPDWordArray inline functions

_SPCOLL_INLINE int CSPDWordArray::GetSize() const
	{ return m_nSize; }
_SPCOLL_INLINE int CSPDWordArray::GetUpperBound() const
	{ return m_nSize-1; }
_SPCOLL_INLINE void CSPDWordArray::RemoveAll()
	{ SetSize(0); }
_SPCOLL_INLINE DWORD CSPDWordArray::GetAt(int nIndex) const
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_SPCOLL_INLINE void CSPDWordArray::SetAt(int nIndex, DWORD newElement)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }
_SPCOLL_INLINE DWORD& CSPDWordArray::ElementAt(int nIndex)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_SPCOLL_INLINE int CSPDWordArray::Add(DWORD newElement)
	{ int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex; }
_SPCOLL_INLINE DWORD CSPDWordArray::operator[](int nIndex) const
	{ return GetAt(nIndex); }
_SPCOLL_INLINE DWORD& CSPDWordArray::operator[](int nIndex)
	{ return ElementAt(nIndex); }

////////////////////////////////////////////////////////////////////////////
// CSPPtrArray inline functions

_SPCOLL_INLINE int CSPPtrArray::GetSize() const
	{ return m_nSize; }
_SPCOLL_INLINE int CSPPtrArray::GetUpperBound() const
	{ return m_nSize-1; }
_SPCOLL_INLINE void CSPPtrArray::RemoveAll()
	{ SetSize(0); }
_SPCOLL_INLINE void* CSPPtrArray::GetAt(int nIndex) const
{ 
	SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex]; 
}
_SPCOLL_INLINE void CSPPtrArray::SetAt(int nIndex, void* newElement)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }
_SPCOLL_INLINE void*& CSPPtrArray::ElementAt(int nIndex)
{ 
	SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex]; 
}

_SPCOLL_INLINE int CSPPtrArray::Add(void* newElement)
{ 
	int nIndex = m_nSize;
	SetAtGrow(nIndex, newElement);
	return nIndex; 
}

_SPCOLL_INLINE void* CSPPtrArray::operator[](int nIndex) const
{ 
	return GetAt(nIndex); 
}

_SPCOLL_INLINE void*& CSPPtrArray::operator[](int nIndex)
{ 
	return ElementAt(nIndex); 
}


////////////////////////////////////////////////////////////////////////////
// CSPObArray inline functions
_SPCOLL_INLINE int CSPObArray::GetSize() const
	{ return m_nSize; }
_SPCOLL_INLINE int CSPObArray::GetUpperBound() const
	{ return m_nSize-1; }
_SPCOLL_INLINE void CSPObArray::RemoveAll()
	{ SetSize(0); }
_SPCOLL_INLINE Object* CSPObArray::GetAt(int nIndex) const
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_SPCOLL_INLINE void CSPObArray::SetAt(int nIndex, Object* newElement)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }
_SPCOLL_INLINE Object*& CSPObArray::ElementAt(int nIndex)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_SPCOLL_INLINE int CSPObArray::Add(Object* newElement)
	{ int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex; }
_SPCOLL_INLINE Object* CSPObArray::operator[](int nIndex) const
	{ return GetAt(nIndex); }
_SPCOLL_INLINE Object*& CSPObArray::operator[](int nIndex)
	{ return ElementAt(nIndex); }


////////////////////////////////////////////////////////////////////////////
// CSPStringArray inline functions

_SPCOLL_INLINE int CSPStringArray::GetSize() const
	{ return m_nSize; }
_SPCOLL_INLINE int CSPStringArray::GetUpperBound() const
	{ return m_nSize-1; }
_SPCOLL_INLINE void CSPStringArray::RemoveAll()
	{ SetSize(0); }
_SPCOLL_INLINE CSPString CSPStringArray::GetAt(int nIndex) const
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_SPCOLL_INLINE void CSPStringArray::SetAt(int nIndex, const char* newElement)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }
_SPCOLL_INLINE CSPString& CSPStringArray::ElementAt(int nIndex)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_SPCOLL_INLINE int CSPStringArray::Add(const char* newElement)
	{ int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex; }
_SPCOLL_INLINE CSPString CSPStringArray::operator[](int nIndex) const
	{ return GetAt(nIndex); }
_SPCOLL_INLINE CSPString& CSPStringArray::operator[](int nIndex)
	{ return ElementAt(nIndex); }


////////////////////////////////////////////////////////////////////////////
// CSPMapStringToPtr inline functions

_SPCOLL_INLINE int CSPMapStringToPtr::GetCount() const
	{ return m_nCount; }
_SPCOLL_INLINE BOOL CSPMapStringToPtr::IsEmpty() const
	{ return m_nCount == 0; }
_SPCOLL_INLINE void CSPMapStringToPtr::SetAt(LPCTSTR key, void* newValue)
	{ (*this)[key] = newValue; }
_SPCOLL_INLINE SPPOSITION CSPMapStringToPtr::GetStartPosition() const
	{ return (m_nCount == 0) ? NULL : BEFORE_START_SPPOSITION; }
_SPCOLL_INLINE UINT CSPMapStringToPtr::GetHashTableSize() const
	{ return m_nHashTableSize; }
