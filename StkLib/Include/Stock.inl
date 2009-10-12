#include <time.h>

///////////////////////////////////////////////////////////////////////
//  CDRData
_STOCK_INLINE int CDRData::GetSize() const
	{ return m_nSize; }
_STOCK_INLINE int CDRData::GetUpperBound() const
	{ return m_nSize-1; }
_STOCK_INLINE void CDRData::RemoveAll()
	{ SetSize(0); }
_STOCK_INLINE DRDATA CDRData::GetAt(int nIndex) const
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_STOCK_INLINE void CDRData::SetAt(int nIndex, DRDATA newElement)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }
_STOCK_INLINE DRDATA& CDRData::ElementAt(int nIndex)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_STOCK_INLINE const DRDATA* CDRData::GetData() const
	{ return (const DRDATA*)m_pData; }
_STOCK_INLINE DRDATA* CDRData::GetData()
	{ return (DRDATA*)m_pData; }
_STOCK_INLINE int CDRData::Add(DRDATA newElement)
	{ int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex; }
_STOCK_INLINE DRDATA CDRData::operator[](int nIndex) const
	{ return GetAt(nIndex); }
_STOCK_INLINE DRDATA& CDRData::operator[](int nIndex)
	{ return ElementAt(nIndex); }


///////////////////////////////////////////////////////////////////////
//  CKData
_STOCK_INLINE int CKData::GetUpperBound() const
	{ return m_nSize-1; }
_STOCK_INLINE void CKData::RemoveAll()
	{ SetSize(0); }
_STOCK_INLINE KDATA CKData::GetAt(int nIndex) const
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_STOCK_INLINE void CKData::SetAt(int nIndex, KDATA newElement)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }
_STOCK_INLINE KDATA& CKData::ElementAt(int nIndex)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_STOCK_INLINE const KDATA* CKData::GetData() const
	{ return (const KDATA*)m_pData; }
_STOCK_INLINE KDATA* CKData::GetData()
	{ return (KDATA*)m_pData; }
_STOCK_INLINE KDATA CKData::operator[](int nIndex) const
	{ return GetAt(nIndex); }
_STOCK_INLINE KDATA& CKData::operator[](int nIndex)
	{ return ElementAt(nIndex); }
_STOCK_INLINE BOOL CKData::DateAt(int nIndex,
					int &nYear, int &nMonth, int &nDay, int &nHour, int &nMinute ) const
{
	SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
	if( nIndex < 0 || nIndex >= m_nSize )
		return 0;
	DWORD	date	=	m_pData[nIndex].m_date;
	CSPTime	sptime;
	BOOL	bOK	=	FALSE;
	if( ktypeMonth == m_nKType 
		|| ktypeWeek == m_nKType 
		|| ktypeDay == m_nKType )
	{
		bOK	=	sptime.FromStockTimeDay( date );
	}
	else if( ktypeMin60 == m_nKType
		|| ktypeMin30 == m_nKType
		|| ktypeMin15 == m_nKType
		|| ktypeMin5 == m_nKType )
	{
		bOK	=	sptime.FromStockTimeMin( date );
	}
	else
	{
		nYear	=	nMonth	=	nDay	=	nHour	=	nMinute	=	0;
		return FALSE;
	}
	if( !bOK )
		return FALSE;

	nYear	=	sptime.GetYear();
	nMonth	=	sptime.GetMonth();
	nDay	=	sptime.GetDay();
	nHour	=	sptime.GetHour();
	nMinute	=	sptime.GetMinute();

	return TRUE;
}
_STOCK_INLINE float	CKData::MaindataAt(int nIndex) const
// 得到CKData的nIndex日的主数据，根据主数据类型不同，返回值可能是开盘价、收盘价或者平均价
{
	SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
	if( nIndex < 0 || nIndex >= m_nSize )
		return 0;
	if( mdtypeOpen == m_nCurMaindataType )
		return m_pData[nIndex].m_fOpen;
	else if( mdtypeAverage == m_nCurMaindataType
			&& m_pData[nIndex].m_fVolume > 1e-4 && m_pData[nIndex].m_fAmount > 1e-4 )
	{
		int		nCount	=	0;
		double	average	=	((double)(m_pData[nIndex].m_fAmount)) / m_pData[nIndex].m_fVolume;
		while( average < m_pData[nIndex].m_fLow && nCount < 10 )	{	average	*=	10;	nCount ++;	}
		while( average > m_pData[nIndex].m_fHigh && nCount < 20 )	{	average	/=	10;	nCount ++;	}
		if( average < m_pData[nIndex].m_fLow )		//	说明是指数
			average	=	(m_pData[nIndex].m_fOpen+m_pData[nIndex].m_fHigh+m_pData[nIndex].m_fLow+m_pData[nIndex].m_fClose)/4;
		return (float)average;
	}
	else
		return m_pData[nIndex].m_fClose;

	// WARNING CPV::Calculate( ... ) use the save code.
}

///////////////////////////////////////////////////////////////////////
//  CBaseData
_STOCK_INLINE int CBaseData::GetSize() const
	{ return m_nSize; }
_STOCK_INLINE int CBaseData::GetUpperBound() const
	{ return m_nSize-1; }
_STOCK_INLINE void CBaseData::RemoveAll()
	{ SetSize(0); }
_STOCK_INLINE BASEDATA CBaseData::GetAt(int nIndex) const
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_STOCK_INLINE void CBaseData::SetAt(int nIndex, BASEDATA newElement)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }
_STOCK_INLINE BASEDATA& CBaseData::ElementAt(int nIndex)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_STOCK_INLINE const BASEDATA* CBaseData::GetData() const
	{ return (const BASEDATA*)m_pData; }
_STOCK_INLINE BASEDATA* CBaseData::GetData()
	{ return (BASEDATA*)m_pData; }
_STOCK_INLINE int CBaseData::Add(BASEDATA newElement)
	{ int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex; }
_STOCK_INLINE BASEDATA CBaseData::operator[](int nIndex) const
	{ return GetAt(nIndex); }
_STOCK_INLINE BASEDATA& CBaseData::operator[](int nIndex)
	{ return ElementAt(nIndex); }

///////////////////////////////////////////////////////////////////////
//  CReport
_STOCK_INLINE int CReport::GetSize() const
	{ return m_nSize; }
_STOCK_INLINE int CReport::GetUpperBound() const
	{ return m_nSize-1; }
_STOCK_INLINE void CReport::RemoveAll()
	{ SetSize(0); }
_STOCK_INLINE REPORT CReport::GetAt(int nIndex) const
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_STOCK_INLINE void CReport::SetAt(int nIndex, REPORT newElement)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }
_STOCK_INLINE REPORT& CReport::ElementAt(int nIndex)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_STOCK_INLINE const REPORT* CReport::GetData() const
	{ return (const REPORT*)m_pData; }
_STOCK_INLINE REPORT* CReport::GetData()
	{ return (REPORT*)m_pData; }
_STOCK_INLINE int CReport::Add(REPORT newElement)
	{ int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex; }
_STOCK_INLINE REPORT CReport::operator[](int nIndex) const
	{ return GetAt(nIndex); }
_STOCK_INLINE REPORT& CReport::operator[](int nIndex)
	{ return ElementAt(nIndex); }

///////////////////////////////////////////////////////////////////////
//  CMinute
_STOCK_INLINE int CMinute::GetSize() const
	{ return m_nSize; }
_STOCK_INLINE int CMinute::GetUpperBound() const
	{ return m_nSize-1; }
_STOCK_INLINE void CMinute::RemoveAll()
	{ SetSize(0); }
_STOCK_INLINE MINUTE CMinute::GetAt(int nIndex) const
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_STOCK_INLINE void CMinute::SetAt(int nIndex, MINUTE newElement)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }
_STOCK_INLINE MINUTE& CMinute::ElementAt(int nIndex)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_STOCK_INLINE const MINUTE* CMinute::GetData() const
	{ return (const MINUTE*)m_pData; }
_STOCK_INLINE MINUTE* CMinute::GetData()
	{ return (MINUTE*)m_pData; }
_STOCK_INLINE int CMinute::Add(MINUTE newElement)
	{ int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex; }
_STOCK_INLINE MINUTE CMinute::operator[](int nIndex) const
	{ return GetAt(nIndex); }
_STOCK_INLINE MINUTE& CMinute::operator[](int nIndex)
	{ return ElementAt(nIndex); }

///////////////////////////////////////////////////////////////////////
//  COutline
_STOCK_INLINE int COutline::GetSize() const
	{ return m_nSize; }
_STOCK_INLINE int COutline::GetUpperBound() const
	{ return m_nSize-1; }
_STOCK_INLINE void COutline::RemoveAll()
	{ SetSize(0); }
_STOCK_INLINE OUTLINE COutline::GetAt(int nIndex) const
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_STOCK_INLINE void COutline::SetAt(int nIndex, OUTLINE newElement)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }
_STOCK_INLINE OUTLINE& COutline::ElementAt(int nIndex)
	{ SP_ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
_STOCK_INLINE const OUTLINE* COutline::GetData() const
	{ return (const OUTLINE*)m_pData; }
_STOCK_INLINE OUTLINE* COutline::GetData()
	{ return (OUTLINE*)m_pData; }
_STOCK_INLINE int COutline::Add(OUTLINE newElement)
	{ int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex; }
_STOCK_INLINE OUTLINE COutline::operator[](int nIndex) const
	{ return GetAt(nIndex); }
_STOCK_INLINE OUTLINE& COutline::operator[](int nIndex)
	{ return ElementAt(nIndex); }


