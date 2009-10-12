/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CSPArray,
		class	CSPDWordArray,
		class	CSPPtrArray,
		class	CSPObArray,
		class	CSPStringArray,
		class	CSPMapStringToPtr;
*/
#if	!defined( __SP_COLLECT_H__ )
#define	__SP_COLLECT_H__

#define SIZE_T_MAX  UINT_MAX            /* max size for a size_t */

////////////////////////////////////////////////////////////////////////////
// Elements Operating Functions

////////////////////////////////////////////////////////////////////////////
// WARNING!!!!!!
// 1. 在其他dll或者可执行程序里，不能直接调用CSPArray<TYPE,TYPE&>的函数，因为在
//    _DEBUG方式下，内存的分配、释放方式不同。
////////////////////////////////////////////////////////////////////////////

#define	_SPARRAY_INLINE

////////////////////////////////////////////////////////////////////////////
// WARNING!!!!!!
// 1. 下面的 operator new 和 delete 仅供SPConstructElements调用，用以调用元素的
// 构造函数。
inline void *__cdecl operator new(size_t, void *_P, int, int )
        {return (_P); }
#if     _MSC_VER >= 1200
inline void __cdecl operator delete(void *, void *, int, int )
	{return; }
#endif

template<class TYPE>
_SPARRAY_INLINE void __stdcall SPConstructElements(TYPE* pElements, int nCount)
{
	SP_ASSERT(nCount == 0 ||
		SP_IsValidAddress((void*)pElements, nCount * sizeof(TYPE)));

	// first do bit-wise zero initialization
	memset((void*)pElements, 0, nCount * sizeof(TYPE));

	for (; nCount--; pElements++)
		::new(((void*)pElements), 0, 0) TYPE;
}

template<class TYPE>
_SPARRAY_INLINE void __stdcall SPDestructElements(TYPE* pElements, int nCount)
{
	SP_ASSERT(nCount == 0 ||
		SP_IsValidAddress((void*)pElements, nCount * sizeof(TYPE)));

	// call the destructor(s)
	for (; nCount--; pElements++)
		pElements->~TYPE();
}

template<class TYPE>
_SPARRAY_INLINE void __stdcall SPCopyElements(TYPE* pDest, const TYPE* pSrc, int nCount)
{
	SP_ASSERT(nCount == 0 ||
		SP_IsValidAddress((void*)pDest, nCount * sizeof(TYPE)));
	SP_ASSERT(nCount == 0 ||
		SP_IsValidAddress((void*)pSrc, nCount * sizeof(TYPE)));

	// default is element-copy using assignment
	while (nCount--)
		*pDest++ = *pSrc++;
}

template<class TYPE>
_SPARRAY_INLINE void __stdcall SPSerializeElements(CSPArchive& ar, TYPE* pElements, int nCount)
{
	SP_ASSERT(nCount == 0 ||
		SP_IsValidAddress((void*)pElements, nCount * sizeof(TYPE)));

	// default is bit-wise read/write
	if (ar.IsStoring())
		ar.Write((void*)pElements, nCount * sizeof(TYPE));
	else
		ar.Read((void*)pElements, nCount * sizeof(TYPE));
}

template<class TYPE, class ARG_TYPE>
_SPARRAY_INLINE BOOL __stdcall SPCompareElements(const TYPE* pElement1, const ARG_TYPE* pElement2)
{
	SP_ASSERT(SP_IsValidAddress((void*)pElement1, sizeof(TYPE), FALSE));
	SP_ASSERT(SP_IsValidAddress((void*)pElement2, sizeof(ARG_TYPE), FALSE));

	return *pElement1 == *pElement2;
}

/////////////////////////////////////////////////////////////////////////////
// CSPArray<TYPE, ARG_TYPE>

template<class TYPE, class ARG_TYPE>
class CSPArray : public Object
{
public:
// Construction
	CSPArray();

// Attributes
	int GetSize() const;
	int GetUpperBound() const;
protected:
	virtual	void SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	virtual	void FreeExtra();
	virtual	void RemoveAll();

public:
	// Accessing elements
	TYPE GetAt(int nIndex) const;
	void SetAt(int nIndex, ARG_TYPE newElement);
	TYPE& ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const TYPE* GetData() const;
	TYPE* GetData();

protected:
	// Potentially growing the array
	virtual	void SetAtGrow(int nIndex, ARG_TYPE newElement);
	virtual	int Add(ARG_TYPE newElement);
	virtual	int Append(const CSPArray& src);
	virtual	void Copy(const CSPArray& src);

public:
	// overloaded operator helpers
	TYPE operator[](int nIndex) const;
	TYPE& operator[](int nIndex);

protected:
	// Operations that move elements around
	virtual	void InsertAt(int nIndex, ARG_TYPE newElement, int nCount = 1);
	virtual	void RemoveAt(int nIndex, int nCount = 1);
	virtual	void InsertAt(int nStartIndex, CSPArray* pNewArray);

// Implementation
protected:
	TYPE* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

protected:
	virtual	~CSPArray();
public:
	void Serialize(CSPArchive&);
#ifdef _DEBUG
	void Dump() const;
	void AssertValid() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CSPArray<TYPE, ARG_TYPE> inline functions

template<class TYPE, class ARG_TYPE>
_SPARRAY_INLINE int CSPArray<TYPE, ARG_TYPE>::GetSize() const
	{ return m_nSize; }
template<class TYPE, class ARG_TYPE>
_SPARRAY_INLINE int CSPArray<TYPE, ARG_TYPE>::GetUpperBound() const
	{ return m_nSize-1; }
template<class TYPE, class ARG_TYPE>
_SPARRAY_INLINE void CSPArray<TYPE, ARG_TYPE>::RemoveAll()
	{ SetSize(0, -1); }
template<class TYPE, class ARG_TYPE>
_SPARRAY_INLINE TYPE CSPArray<TYPE, ARG_TYPE>::GetAt(int nIndex) const
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
template<class TYPE, class ARG_TYPE>
_SPARRAY_INLINE void CSPArray<TYPE, ARG_TYPE>::SetAt(int nIndex, ARG_TYPE newElement)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }
template<class TYPE, class ARG_TYPE>
_SPARRAY_INLINE TYPE& CSPArray<TYPE, ARG_TYPE>::ElementAt(int nIndex)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
template<class TYPE, class ARG_TYPE>
_SPARRAY_INLINE const TYPE* CSPArray<TYPE, ARG_TYPE>::GetData() const
	{ return (const TYPE*)m_pData; }
template<class TYPE, class ARG_TYPE>
_SPARRAY_INLINE TYPE* CSPArray<TYPE, ARG_TYPE>::GetData()
	{ return (TYPE*)m_pData; }
template<class TYPE, class ARG_TYPE>
_SPARRAY_INLINE int CSPArray<TYPE, ARG_TYPE>::Add(ARG_TYPE newElement)
	{ int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex; }
template<class TYPE, class ARG_TYPE>
_SPARRAY_INLINE TYPE CSPArray<TYPE, ARG_TYPE>::operator[](int nIndex) const
	{ return GetAt(nIndex); }
template<class TYPE, class ARG_TYPE>
_SPARRAY_INLINE TYPE& CSPArray<TYPE, ARG_TYPE>::operator[](int nIndex)
	{ return ElementAt(nIndex); }


/////////////////////////////////////////////////////////////////////////////
// CSPArray<TYPE, ARG_TYPE> out-of-line functions

template<class TYPE, class ARG_TYPE>
CSPArray<TYPE, ARG_TYPE>::CSPArray()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

template<class TYPE, class ARG_TYPE>
CSPArray<TYPE, ARG_TYPE>::~CSPArray()
{
	SP_ASSERT_VALID(this);

	if (m_pData != NULL)
	{
		SPDestructElements<TYPE>(m_pData, m_nSize);
		delete[] (BYTE*)m_pData;
		m_pData	=	NULL;
	}
	m_nSize = m_nMaxSize = 0;
}

template<class TYPE, class ARG_TYPE>
void CSPArray<TYPE, ARG_TYPE>::SetSize(int nNewSize, int nGrowBy)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(nNewSize >= 0);

	if (nGrowBy != -1)
		m_nGrowBy = nGrowBy;  // set new size

	if (nNewSize == 0)
	{
		// shrink to nothing
		if (m_pData != NULL)
		{
			SPDestructElements<TYPE>(m_pData, m_nSize);
			delete[] (BYTE*)m_pData;
			m_pData = NULL;
		}
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
		SP_ASSERT(nNewSize <= SIZE_T_MAX/sizeof(TYPE));    // no overflow
#endif
		m_pData = (TYPE*) new BYTE[nNewSize * sizeof(TYPE)];
		SPConstructElements<TYPE>(m_pData, nNewSize);
		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements
			SPConstructElements<TYPE>(&m_pData[m_nSize], nNewSize-m_nSize);
		}
		else if (m_nSize > nNewSize)
		{
			// destroy the old elements
			SPDestructElements<TYPE>(&m_pData[nNewSize], m_nSize-nNewSize);
		}
		m_nSize = nNewSize;
	}
	else
	{
		// otherwise, grow array
		int nGrowBy = m_nGrowBy;
		if (nGrowBy == 0)
		{
			// heuristically determine growth when nGrowBy == 0
			//  (this avoids heap fragmentation in many situations)
			nGrowBy = m_nSize / 8;
			nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);
		}
		int nNewMax;
		if (nNewSize < m_nMaxSize + nGrowBy)
			nNewMax = m_nMaxSize + nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

		SP_ASSERT(nNewMax >= m_nMaxSize);  // no wrap around
#ifdef SIZE_T_MAX
		SP_ASSERT(nNewMax <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
		TYPE* pNewData = (TYPE*) new BYTE[nNewMax * sizeof(TYPE)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));

		// construct remaining elements
		SP_ASSERT(nNewSize > m_nSize);
		SPConstructElements<TYPE>(&pNewData[m_nSize], nNewSize-m_nSize);

		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

template<class TYPE, class ARG_TYPE>
int CSPArray<TYPE, ARG_TYPE>::Append(const CSPArray& src)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(this != &src);   // cannot append to itself

	int nOldSize = m_nSize;
	SetSize(m_nSize + src.m_nSize);
	SPCopyElements<TYPE>(m_pData + nOldSize, src.m_pData, src.m_nSize);
	return nOldSize;
}

template<class TYPE, class ARG_TYPE>
void CSPArray<TYPE, ARG_TYPE>::Copy(const CSPArray& src)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(this != &src);   // cannot append to itself

	SetSize(src.m_nSize);
	SPCopyElements<TYPE>(m_pData, src.m_pData, src.m_nSize);
}

template<class TYPE, class ARG_TYPE>
void CSPArray<TYPE, ARG_TYPE>::FreeExtra()
{
	SP_ASSERT_VALID(this);

	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		SP_ASSERT(m_nSize <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
		TYPE* pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (TYPE*) new BYTE[m_nSize * sizeof(TYPE)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));
		}

		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

template<class TYPE, class ARG_TYPE>
void CSPArray<TYPE, ARG_TYPE>::SetAtGrow(int nIndex, ARG_TYPE newElement)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1, -1);
	m_pData[nIndex] = newElement;
}

template<class TYPE, class ARG_TYPE>
void CSPArray<TYPE, ARG_TYPE>::InsertAt(int nIndex, ARG_TYPE newElement, int nCount /*=1*/)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(nIndex >= 0);    // will expand to meet need
	SP_ASSERT(nCount > 0);     // zero or negative size not allowed

	if (nIndex >= m_nSize)
	{
		// adding after the end of the array
		SetSize(nIndex + nCount, -1);   // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		int nOldSize = m_nSize;
		SetSize(m_nSize + nCount, -1);  // grow it to new size
		// destroy intial data before copying over it
		SPDestructElements<TYPE>(&m_pData[nOldSize], nCount);
		// shift old data up to fill gap
		memmove(&m_pData[nIndex+nCount], &m_pData[nIndex],
			(nOldSize-nIndex) * sizeof(TYPE));

		// re-init slots we copied from
		SPConstructElements<TYPE>(&m_pData[nIndex], nCount);
	}

	// insert new value in the gap
	SP_ASSERT(nIndex + nCount <= m_nSize);
	while (nCount--)
		m_pData[nIndex++] = newElement;
}

template<class TYPE, class ARG_TYPE>
void CSPArray<TYPE, ARG_TYPE>::RemoveAt(int nIndex, int nCount)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(nIndex >= 0);
	SP_ASSERT(nCount >= 0);
	SP_ASSERT(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);
	SPDestructElements<TYPE>(&m_pData[nIndex], nCount);
	if (nMoveCount)
		memmove(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(TYPE));
	m_nSize -= nCount;
}

template<class TYPE, class ARG_TYPE>
void CSPArray<TYPE, ARG_TYPE>::InsertAt(int nStartIndex, CSPArray* pNewArray)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(pNewArray != NULL);
	SP_ASSERT_VALID(pNewArray);
	SP_ASSERT(nStartIndex >= 0);

	if (pNewArray->GetSize() > 0)
	{
		InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
		for (int i = 0; i < pNewArray->GetSize(); i++)
			SetAt(nStartIndex + i, pNewArray->GetAt(i));
	}
}

template<class TYPE, class ARG_TYPE>
void CSPArray<TYPE, ARG_TYPE>::Serialize(CSPArchive& ar)
{
	SP_ASSERT_VALID(this);

	// Object::Serialize(ar);
	if (ar.IsStoring())
	{
		ar.WriteCount(m_nSize);
	}
	else
	{
		DWORD nOldSize = ar.ReadCount();
		SetSize(nOldSize, -1);
	}
	SPSerializeElements<TYPE>(ar, m_pData, m_nSize);
}


#ifdef _DEBUG
template<class TYPE, class ARG_TYPE>
void CSPArray<TYPE, ARG_TYPE>::Dump() const
{
	Object::Dump();
}

template<class TYPE, class ARG_TYPE>
void CSPArray<TYPE, ARG_TYPE>::AssertValid() const
{
	Object::AssertValid();

	if (m_pData == NULL)
	{
		SP_ASSERT(m_nSize == 0);
		SP_ASSERT(m_nMaxSize == 0);
	}
	else
	{
		SP_ASSERT(m_nSize >= 0);
		SP_ASSERT(m_nMaxSize >= 0);
		SP_ASSERT(m_nSize <= m_nMaxSize);
		SP_ASSERT(SP_IsValidAddress((void*)m_pData, m_nMaxSize * sizeof(TYPE)));
	}
}
#endif //_DEBUG



////////////////////////////////////////////////////////////////////////////
// CSPDWordArray
class STKLIB_API CSPDWordArray : public Object
{
public:
// Construction
	CSPDWordArray();

// Attributes
	int		GetSize() const;
	int		GetUpperBound() const;
	void	SetSize(int nNewSize, int nGrowBy = -1);

	void	Copy( const CSPDWordArray & src );

// Operations
	// Clean up
	void	FreeExtra();
	void	RemoveAll();

	// Accessing elements
	DWORD	GetAt(int nIndex) const;
	void	SetAt(int nIndex, DWORD newElement);
	DWORD&	ElementAt(int nIndex);

	// Potentially growing the array
	void	SetAtGrow(int nIndex, DWORD newElement);
	int		Add(DWORD newElement);

	// overloaded operator helpers
	DWORD	operator[](int nIndex) const;
	DWORD&	operator[](int nIndex);

	// Operations that move elements around
	void	InsertAt(int nIndex, DWORD newElement, int nCount = 1);
	void	RemoveAt(int nIndex, int nCount = 1);
	void	InsertAt(int nStartIndex, CSPDWordArray* pNewArray);

	// 
	int		AddUnique( DWORD newElement );
	void	Sort( );
	BOOL	IsEqualTo( CSPDWordArray & adw );
	DWORD * GetData();

// Implementation
protected:
	DWORD* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~CSPDWordArray();

	void Serialize(CSPArchive&);
#ifdef	_DEBUG
public:
	virtual	void	AssertValid() const;
	virtual	void	Dump( ) const;
#endif
};


////////////////////////////////////////////////////////////////////////////
// CSPPrtArray
class STKLIB_API CSPPtrArray : public Object
{
public:
// Construction
	CSPPtrArray();

// Attributes
	int GetSize() const;
	int GetUpperBound() const;
	void SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void FreeExtra();
	void RemoveAll();

	// Accessing elements
	void* GetAt(int nIndex) const;
	void SetAt(int nIndex, void* newElement);
	void*& ElementAt(int nIndex);

	// Potentially growing the array
	void SetAtGrow(int nIndex, void* newElement);
	int Add(void* newElement);

	// overloaded operator helpers
	void* operator[](int nIndex) const;
	void*& operator[](int nIndex);

	// Operations that move elements around
	void InsertAt(int nIndex, void* newElement, int nCount = 1);
	void RemoveAt(int nIndex, int nCount = 1);
	void InsertAt(int nStartIndex, CSPPtrArray* pNewArray);

	
	int		Append(const CSPPtrArray& src);
	void	Copy(const CSPPtrArray& src);

// Implementation
protected:
	void** m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~CSPPtrArray();

#ifdef	_DEBUG
public:
	virtual	void	AssertValid() const;
	virtual	void	Dump( ) const;
#endif
};

////////////////////////////////////////////////////////////////////////////
// CSPObArray
class STKLIB_API CSPObArray : public Object
{
public:

// Construction
	CSPObArray();

// Attributes
	int GetSize() const;
	int GetUpperBound() const;
	void SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void FreeExtra();
	void RemoveAll();

	// Accessing elements
	Object* GetAt(int nIndex) const;
	void SetAt(int nIndex, Object* newElement);
	Object*& ElementAt(int nIndex);

	// Potentially growing the array
	void SetAtGrow(int nIndex, Object* newElement);
	int Add(Object* newElement);

	// overloaded operator helpers
	Object* operator[](int nIndex) const;
	Object*& operator[](int nIndex);

	// Operations that move elements around
	void InsertAt(int nIndex, Object* newElement, int nCount = 1);
	void RemoveAt(int nIndex, int nCount = 1);
	void InsertAt(int nStartIndex, CSPObArray* pNewArray);

// Implementation
protected:
	Object** m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~CSPObArray();

#ifdef _DEBUG
	void Dump( ) const;
	void AssertValid() const;
#endif
};

////////////////////////////////////////////////////////////////////////////
// CSPStringArray
class STKLIB_API CSPStringArray : public Object
{
public:
// Construction
	CSPStringArray();

	static CSPStringArray * m_pSortSPStringArray;

// Attributes
	int GetSize() const;
	int GetUpperBound() const;
	void SetSize(int nNewSize, int nGrowBy = -1);

	int		Append(const CSPStringArray& src);
	void	Copy(const CSPStringArray& src);

// Operations
	// Clean up
	void FreeExtra();
	void RemoveAll();

	// Accessing elements
	CSPString GetAt(int nIndex) const;
	void SetAt(int nIndex, const char* newElement);
	CSPString& ElementAt(int nIndex);

	// Potentially growing the array
	void SetAtGrow(int nIndex, const char* newElement);
	int Add(const char* newElement);

	// overloaded operator helpers
	CSPString operator[](int nIndex) const;
	CSPString& operator[](int nIndex);

	// Operations that move elements around
	void InsertAt(int nIndex, const char* newElement, int nCount = 1);
	void RemoveAt(int nIndex, int nCount = 1);
	void InsertAt(int nStartIndex, CSPStringArray* pNewArray);
	BOOL GetSortIDArray( CSPDWordArray & adwSortID );

// Implementation
protected:
	CSPString* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

	void	SPConstructElements(CSPString* pElements, int nCount);
	void	SPDestructElements(CSPString* pElements, int nCount);
public:
	~CSPStringArray();

	void Serialize(CSPArchive&);
#ifdef _DEBUG
	void Dump() const;
	void AssertValid() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CSPPlex
struct CSPPlex     // warning variable length structure
{
	CSPPlex* pNext;
#if (_AFX_PACKING >= 8)
	DWORD dwReserved[1];    // align on 8 byte boundary
#endif
	// BYTE data[maxNum*elementSize];

	void* data() { return this+1; }

	static CSPPlex* PASCAL Create(CSPPlex*& head, UINT nMax, UINT cbElement);
			// like 'calloc' but no zero fill
			// may throw memory exceptions

	void FreeDataChain();       // free this one and links
};


/////////////////////////////////////////////////////////////////////////////
// CSPMapStringToPtr
// abstract iteration position
struct __SPPOSITION { };
typedef __SPPOSITION* SPPOSITION;
#define BEFORE_START_SPPOSITION ((SPPOSITION)-1L)

class STKLIB_API CSPMapStringToPtr : public Object
{
protected:
	// Association
	struct CAssoc
	{
		CAssoc* pNext;
		UINT nHashValue;  // needed for efficient iteration
		CSPString key;
		void* value;
	};

public:

// Construction
	CSPMapStringToPtr(int nBlockSize = 10);

// Attributes
	// number of elements
	int GetCount() const;
	BOOL IsEmpty() const;

	// Lookup
	BOOL Lookup(LPCTSTR key, void*& rValue) const;
	BOOL LookupKey(LPCTSTR key, LPCTSTR& rKey) const;

// Operations
	// Lookup and add if not there
	void*& operator[](LPCTSTR key);

	// add a new (key, value) pair
	void SetAt(LPCTSTR key, void* newValue);

	// removing existing (key, ?) pair
	BOOL RemoveKey(LPCTSTR key);
	void RemoveAll();

	// iterating all (key, value) pairs
	SPPOSITION GetStartPosition() const;
	void GetNextAssoc(SPPOSITION& rNextPosition, CSPString& rKey, void*& rValue) const;

	// advanced features for derived classes
	UINT GetHashTableSize() const;
	void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);

// Overridables: special non-virtual (see map implementation for details)
	// Routine used to user-provided hash keys
	UINT HashKey(LPCTSTR key) const;

// Implementation
protected:
	CAssoc** m_pHashTable;
	UINT m_nHashTableSize;
	int m_nCount;
	CAssoc* m_pFreeList;
	struct CSPPlex* m_pBlocks;
	int m_nBlockSize;

	CAssoc* NewAssoc();
	void FreeAssoc(CAssoc*);
	CAssoc* GetAssocAt(LPCTSTR, UINT&) const;

public:
	~CSPMapStringToPtr();
#ifdef _DEBUG
	void Dump() const;
	void AssertValid() const;
#endif

protected:
	// local typedefs for CTypedPtrMap class template
	typedef CSPString BASE_KEY;
	typedef LPCTSTR BASE_ARG_KEY;
	typedef void* BASE_VALUE;
	typedef void* BASE_ARG_VALUE;
};


#ifdef	_SP_ENABLE_INLINES
#define	_SPCOLL_INLINE		inline
#include "SpColl.inl"
#undef	_SPCOLL_INLINE
#endif

#endif //__SP_COLLECT_H__