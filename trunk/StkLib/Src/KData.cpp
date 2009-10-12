/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CKData;
*/

#include	"StdAfx.h"
#include	"../Include/Stock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
CKData::CKData()
{
	m_nKType	=	ktypeDay;
	m_nCurFormat=	formatOriginal;
	m_nCurMaindataType	=	mdtypeClose;
	m_pDataOriginal	=	NULL;
	m_nSizeOriginal = m_nMaxSizeOriginal = 0;
	m_pData	=	NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

CKData::CKData( int ktype, int maindatatype )
{
	m_nKType	=	ktype;
	m_nCurFormat=	formatOriginal;
	m_nCurMaindataType	=	maindatatype;
	m_pDataOriginal	=	NULL;
	m_nSizeOriginal = m_nMaxSizeOriginal = 0;
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

CKData::CKData( const CKData &src )
{
	m_nKType	=	ktypeDay;
	m_nCurFormat=	formatOriginal;
	m_nCurMaindataType	=	mdtypeClose;
	m_pDataOriginal	=	NULL;
	m_nSizeOriginal = m_nMaxSizeOriginal = 0;
	m_pData	=	NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
	*this	=	src;
}

CKData	& CKData::operator = ( const CKData &src )
{
	Clear();
	
	m_nKType			=	src.m_nKType;
	m_nCurFormat		=	src.m_nCurFormat;
	m_nCurMaindataType	=	src.m_nCurMaindataType;
	m_drdata			=	src.m_drdata;

	CopyData( src );
	
	return	*this;
}

BOOL CKData::IsDayOrMin( int nKType )
{
	return (nKType == CKData::ktypeDay || nKType == CKData::ktypeWeek || nKType == CKData::ktypeMonth);
}

CKData::~CKData()
{
	Clear();
}

int CKData::GetSize() const
	{ return m_nSize; }
int CKData::Add(KDATA newElement)
	{ int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex; }

void CKData::SetSize(int nNewSize, int nGrowBy /* = -1 */)
{
	SP_ASSERT(nNewSize >= 0);

	if (nGrowBy != -1)
		m_nGrowBy = nGrowBy;  // set new size

	if (nNewSize == 0)
	{
		// shrink to nothing
		delete [] (BYTE*)m_pData;
		m_pData = NULL;
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
		SP_ASSERT((long)nNewSize * sizeof(KDATA) <= SIZE_T_MAX);  // no overflow
#endif
		m_pData = (KDATA*) new BYTE[nNewSize * sizeof(KDATA)];

		memset(m_pData, 0, nNewSize * sizeof(KDATA));  // zero fill

		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements

			memset(&m_pData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(KDATA));

		}

		m_nSize = nNewSize;
	}
	else
	{
		// Otherwise grow array
		int nNewMax;
		if (nNewSize < m_nMaxSize + m_nGrowBy)
			nNewMax = m_nMaxSize + m_nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

#ifdef SIZE_T_MAX
		SP_ASSERT((long)nNewMax * sizeof(KDATA) <= SIZE_T_MAX);  // no overflow
#endif
		KDATA* pNewData = (KDATA*) new BYTE[nNewMax * sizeof(KDATA)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(KDATA));

		// construct remaining elements
		SP_ASSERT(nNewSize > m_nSize);

		memset(&pNewData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(KDATA));


		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

void CKData::FreeExtra()
{
	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		SP_ASSERT((long)m_nSize * sizeof(KDATA) <= SIZE_T_MAX);  // no overflow
#endif
		KDATA* pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (KDATA*) new BYTE[m_nSize * sizeof(KDATA)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(KDATA));
		}

		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

BOOL CKData::LatestDate( int &nYear, int &nMonth, int &nDay, int &nHour, int &nMinute ) const
{
	nYear	=	nMonth	=	nDay	=	nHour	=	nMinute	=	0;
	if( GetSize() > 0 )
		return DateAt( GetSize()-1, nYear, nMonth, nDay, nHour, nMinute );
	return FALSE;
}

DWORD CKData::GetDate( int nIndex )
{
	if( nIndex >=0 && nIndex < GetSize() )
		return ElementAt(nIndex).m_date;
	if( GetSize() > 0 )
		return ElementAt(GetSize()-1).m_date;
	return 0;
}

DWORD CKData::GetDateDay( int nIndex )
{
	DWORD	date	=	GetDate(nIndex);
	if( CKData::ktypeMin5 == m_nKType || CKData::ktypeMin15 == m_nKType
		|| CKData::ktypeMin30 == m_nKType || CKData::ktypeMin60 == m_nKType )
	{
		CSPTime	sptime;
		if( sptime.FromStockTimeMin( date ) )
			return sptime.ToStockTimeDay();
		else
			return 0;
	}
	return date;
}

int CKData::GetIndexByDate( DWORD date )
{
	if( GetSize() > 0 && ElementAt(GetSize()-1).m_date < date )
		return -1;
	if( GetSize() > 0 && ElementAt(0).m_date > date )
		return -1;
	for( int k=GetSize()-1; k >= 0 ; k-- )
	{
		if( ElementAt(k).m_date == date )
			return k;
		else if( ElementAt(k).m_date < date )
			break;
	}
	return -1;
}

int CKData::GetAboutIndexByDate( DWORD date )
{
	for( int k=GetSize()-1; k >= 0 ; k-- )
	{
		if( ElementAt(k).m_date <= date )
			return k;
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

void CKData::SetAtGrow(int nIndex, KDATA newElement)
{
	SP_ASSERT(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1);
	m_pData[nIndex] = newElement;
}

void CKData::InsertAt(int nIndex, KDATA newElement, int nCount /*=1*/)
{
	SP_ASSERT(nIndex >= 0);    // will expand to meet need
	SP_ASSERT(nCount > 0);     // zero or negative size not allowed

	if (nIndex >= m_nSize)
	{
		// adding after the end of the array
		SetSize(nIndex + nCount);  // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		int nOldSize = m_nSize;
		SetSize(m_nSize + nCount);  // grow it to new size
		// shift old data up to fill gap
		memmove(&m_pData[nIndex+nCount], &m_pData[nIndex],
			(nOldSize-nIndex) * sizeof(KDATA));

		// re-init slots we copied from

		memset(&m_pData[nIndex], 0, nCount * sizeof(KDATA));

	}

	// insert new value in the gap
	SP_ASSERT(nIndex + nCount <= m_nSize);
	while (nCount--)
		m_pData[nIndex++] = newElement;
}

int CKData::InsertKDataSort( KDATA newElement )
{
	for( int i=0; i<GetSize(); i++ )
	{
		KDATA	& temp = ElementAt(i);
		if( temp.m_date == newElement.m_date )
		{
			SetAt(i,newElement);
			return i;
		}
		if( temp.m_date > newElement.m_date )
		{
			InsertAt(i,newElement);
			return i;
		}
	}
	return Add( newElement );
}

BOOL CKData::IsNewValue( int nIndex, BOOL bTopOrBottom, int nDays )
{
	if( nIndex < nDays-1 || nIndex < 0 || nIndex >= GetSize() || nDays < 3 )
		return FALSE;

	BOOL	bFirst	=	TRUE;
	float	fLast	=	0;
	int		nLast	=	0;
	float	fNow	=	MaindataAt( nIndex );

	for( int k=nIndex-1; k>=0 && k>nIndex-nDays; k-- )
	{
		float	fCur	=	MaindataAt(k);
		if( bFirst )
		{
			fLast	=	fCur;
			nLast	=	k;
			bFirst	=	FALSE;
		}
		
		if( bTopOrBottom )
		{
			if( fCur > fNow )
				return FALSE;

			if( fCur > fLast )
			{
				fLast	=	fCur;
				nLast	=	k;
			}
		}
		else
		{
			if( fCur < fNow )
				return FALSE;

			if( fCur < fLast )
			{
				fLast	=	fCur;
				nLast	=	k;
			}
		}
	}
	if( bFirst )
		return FALSE;

	if( nLast >= nIndex-30 && nLast <= nIndex-5 )
		return TRUE;

	return FALSE;
}

BOOL CKData::GetMinMaxInfo( int nStart, int nEnd, float * pfMin, float *pfMax )
{
	if( nStart < 0 || nEnd < 0 || nStart > nEnd || nEnd >= GetSize() )
		return FALSE;

	float	fMin = 0, fMax = 0;
	BOOL	bFirst	=	TRUE;
	for( int k=nStart; k<=nEnd; k++ )
	{
		KDATA	& kd	=	ElementAt(k);
		if( bFirst )
		{
			fMin	=	kd.m_fLow;
			fMax	=	kd.m_fHigh;
			bFirst	=	FALSE;
		}
		if( kd.m_fLow < fMin )
			fMin	=	kd.m_fLow;
		if( kd.m_fHigh > fMax )
			fMax	=	kd.m_fHigh;
	}
	
	if( pfMin )		*pfMin	=	fMin;
	if( pfMax )		*pfMax	=	fMax;
	return !bFirst;
}

void CKData::RemoveAt(int nIndex, int nCount /* = 1 */)
{
	SP_ASSERT(nIndex >= 0);
	SP_ASSERT(nCount >= 0);
	SP_ASSERT(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);

	if (nMoveCount)
		memcpy(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(KDATA));
	m_nSize -= nCount;
}

void CKData::InsertAt(int nStartIndex, CKData* pNewArray)
{
	SP_ASSERT(pNewArray != NULL);
	SP_ASSERT(nStartIndex >= 0);

	if (pNewArray->GetSize() > 0)
	{
		InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
		for (int i = 0; i < pNewArray->GetSize(); i++)
			SetAt(nStartIndex + i, pNewArray->GetAt(i));
	}
}


/////////////////////////////////////////////////////////////////////////////
// Extra Operations

void CKData::SetKType( int ktype ) {	m_nKType = ktype;	}
void CKData::AutoSetKType( )
{
	if( GetSize() >= 4 )
	{
		long elapse1 = ElementAt(1).m_time - ElementAt(0).m_time;
		long elapse2 = ElementAt(2).m_time - ElementAt(1).m_time;
		long elapse3 = ElementAt(3).m_time - ElementAt(2).m_time;
		long elapse = min(elapse1,elapse2);
		elapse = min(elapse,elapse3);
		if( elapse < 600 )
			m_nKType = ktypeMin5;
		else if( elapse < 1200 )
			m_nKType = ktypeMin15;
		else if( elapse < 2400 )
			m_nKType = ktypeMin30;
		else if( elapse < 7200 )
			m_nKType = ktypeMin60;
		else if( elapse < 172800 )
			m_nKType = ktypeDay;
		else if( elapse < 864000 )
			m_nKType = ktypeWeek;
		else if( elapse < 4320000 )
			m_nKType = ktypeWeek;
	}
}
int CKData::GetKType( ) { return m_nKType; }
int CKData::GetCurFormat( ) { return m_nCurFormat; }
void CKData::SetMaindataType( int type ) { m_nCurMaindataType = type; }
int CKData::GetMaindataType( ) { return m_nCurMaindataType; }

void CKData::SetDRData( CDRData &drdata )	{	m_drdata	=	drdata;	}
CDRData &CKData::GetDRData( )	{	return m_drdata;	}

void CKData::ChangeCurFormat( int format, DWORD dateAutoDRBegin, double dAutoDRLimit )
{
	SP_ASSERT( GetKType() != ktypeMonth && GetKType() != ktypeWeek );

	if( m_nCurFormat == format )
		return;

	switch( format )
	{
	case formatOriginal:
		LoadDataOriginal( );
		m_nCurFormat = format;
		break;
	case formatXDRup:
		StoreDataOriginal( );
		ConvertXDR( TRUE, dateAutoDRBegin, dAutoDRLimit );
		m_nCurFormat = format;
		break;
	case formatXDRdown:
		StoreDataOriginal( );
		ConvertXDR( FALSE, dateAutoDRBegin, dAutoDRLimit );
		m_nCurFormat = format;
		break;
	default:
		SP_ASSERT( FALSE );
	}
}

void CKData::Clear( )
{
	if( m_pDataOriginal )
	{
		delete [] (BYTE*)m_pDataOriginal;
		m_pDataOriginal	=	NULL;
	}
	if( m_pData )
	{
		delete [] (BYTE*)m_pData;
		m_pData	=	NULL;
	}
	m_nKType	=	ktypeDay;
	m_nCurFormat=	formatOriginal;
	m_nCurMaindataType	=	mdtypeClose;
	m_pDataOriginal	=	NULL;
	m_nSizeOriginal = m_nMaxSizeOriginal = 0;
	m_pData	=	NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

int CKData::CompareLatestDate( CKData &kd )
{
	// compare this kdata with kd's latest time
	int	nYearThis, nMonthThis, nDayThis, nHourThis, nMinuteThis;
	int nYear, nMonth, nDay, nHour, nMinute;
	LatestDate( nYearThis, nMonthThis, nDayThis, nHourThis, nMinuteThis );
	kd.LatestDate( nYear, nMonth, nDay, nHour, nMinute );

	if( nYearThis > nYear || nMonthThis > nMonth || nDayThis > nDay
		|| nHourThis > nHour || nMinuteThis > nMinute )
		return 1;
	if( nYearThis < nYear || nMonthThis < nMonth || nDayThis < nDay
		|| nHourThis < nHour || nMinuteThis < nMinute )
		return -1;
	return 0;
}

int CKData::Min5ToMin15( CKData &kdm5, CKData &kdm15 )
{
	SP_ASSERT( ktypeMin5 == kdm5.GetKType() );
	SP_ASSERT( ktypeMin15 == kdm15.GetKType() );
	return ConvertKData( kdm5, kdm15, 3 );
}

int CKData::Min5ToMin30( CKData &kdm5, CKData &kdm30 )
{
	SP_ASSERT( ktypeMin5 == kdm5.GetKType() );
	SP_ASSERT( ktypeMin30 == kdm30.GetKType() );
	return ConvertKData( kdm5, kdm30, 6 );
}

int CKData::Min5ToMin60( CKData &kdm5, CKData &kdm60 )
{
	SP_ASSERT( ktypeMin5 == kdm5.GetKType() );
	SP_ASSERT( ktypeMin60 == kdm60.GetKType() );
	return ConvertKData( kdm5, kdm60, 12 );
}

int CKData::DayToMonth( CKData &kdday, CKData &kdmonth )
{
	// convert day k line to month k line
	SP_ASSERT( ktypeDay == kdday.GetKType() );
	SP_ASSERT( ktypeMonth == kdmonth.GetKType() );

	kdmonth.SetSize( 0, kdday.GetSize() / 20 + 5 );
	
	int nStart		=	kdday.GetSize() % 3;
	int	nCount		=	0;
	KDATA	dataDest;
	memset( &dataDest, 0, sizeof(dataDest) );
	int	nYearCur = 0, nMonthCur = 0 ;
	for( int pos=nStart; pos<kdday.GetSize(); pos++ )
	{
		KDATA & dataSrc = kdday.ElementAt( pos );

		CSPTime	tm;
		if( !tm.FromStockTimeDay(dataSrc.m_date) )
			continue;
		int nYear	=	tm.GetYear();
		int nMonth	=	tm.GetMonth();

		if( nYear != nYearCur || nMonth != nMonthCur )	// a new month
		{
			if( 0 != pos )
				kdmonth.Add( dataDest );				// add a month

			memcpy( &dataDest, &dataSrc, sizeof(dataDest) );	// begin a new month
			nYearCur	=	nYear;
			nMonthCur	=	nMonth;
		}
		else
		{
			dataDest.m_fAmount		+=	dataSrc.m_fAmount;
			if( dataDest.m_fHigh < dataSrc.m_fHigh )	dataDest.m_fHigh	=	dataSrc.m_fHigh;
			if( dataDest.m_fLow > dataSrc.m_fLow )		dataDest.m_fLow		=	dataSrc.m_fLow;
			dataDest.m_fVolume		+=	dataSrc.m_fVolume;
			dataDest.m_fClose		=	dataSrc.m_fClose;
		}

		if( pos == kdday.GetSize()-1 )	// the latest one
			kdmonth.Add( dataDest );
	}

	return kdmonth.GetSize();
}

int CKData::DayToWeek( CKData &kdday, CKData &kdweek )
{
	// convert day k line to week k line
	SP_ASSERT( ktypeDay == kdday.GetKType() );
	SP_ASSERT( ktypeWeek == kdweek.GetKType() );

	kdweek.SetSize( 0, kdday.GetSize() / 5 + 5 );
	
	int nStart		=	kdday.GetSize() % 3;
	int	nCount		=	0;
	KDATA	dataDest;
	memset( &dataDest, 0, sizeof(dataDest) );
	for( int pos=nStart; pos<kdday.GetSize(); pos++ )
	{
		KDATA & dataSrc = kdday.ElementAt( pos );

		CSPTime	tm;
		if( !tm.FromStockTimeDay(dataSrc.m_date) )
			continue;

		if( tm.GetDayOfWeek() == 2 )	// a new week
		{
			if( 0 != pos )
				kdweek.Add( dataDest );				// add a week

			memcpy( &dataDest, &dataSrc, sizeof(dataDest) );	// begin a new week
		}
		else
		{
			dataDest.m_fAmount		+=	dataSrc.m_fAmount;
			if( dataDest.m_fHigh < dataSrc.m_fHigh )	dataDest.m_fHigh	=	dataSrc.m_fHigh;
			if( dataDest.m_fLow > dataSrc.m_fLow )		dataDest.m_fLow		=	dataSrc.m_fLow;
			dataDest.m_fVolume		+=	dataSrc.m_fVolume;
			dataDest.m_fClose		=	dataSrc.m_fClose;
		}

		if( pos == kdday.GetSize()-1 )	// the latest one
			kdweek.Add( dataDest );
	}

	return kdweek.GetSize();
}

DWORD CKData::ToDayDate( DWORD date )
{
	switch( GetKType() )
	{
	case ktypeMin60:
	case ktypeMin30:
	case ktypeMin15:
	case ktypeMin5:
		return (date / 10000 + 1990 * 10000);
	default:
		return date;
	}
}

int CKData::MergeKData( CKData * pother )
{
	if( !pother || pother->GetSize() == 0 )
		return 0;
	if( GetKType() != pother->GetKType() )
		return 0;

	if( GetSize() == 0 )
	{
		CopyData( *pother );
		return GetSize();
	}

	int	nCount		= 0;
	SetSize( GetSize(), pother->GetSize()+1 );
	for( int i=0; i<pother->GetSize(); i++ )
	{
		KDATA	kdnew	=	pother->ElementAt(i);
		if( kdnew.m_fClose < 1e-4 || kdnew.m_fOpen < 1e-4 || kdnew.m_fHigh < 1e-4 || kdnew.m_fLow < 1e-4 )
			continue;

		int j;
		for( j=0; j<GetSize(); j++ )
		{
			if( kdnew.m_date == ElementAt(j).m_date )
			{
				SetAt(j,kdnew);
				break;
			}
			if( kdnew.m_date < ElementAt(j).m_date )
			{
				InsertAt(j,kdnew);
				break;
			}
		}
		if( GetSize() == j )
			Add( kdnew );
		nCount	++;
	}

	return nCount;

/*	The Old Version
	if( !pother || pother->GetSize() == 0 )
		return GetSize();
	if( GetKType() != pother->GetKType() )
		return GetSize();

	if( GetSize() == 0 )
	{
		CopyData( *pother );
		return GetSize();
	}

	int nLen = GetSize();
	int nLenOther = pother->GetSize();

	SP_ASSERT( 0 != nLen && 0 != nLenOther );

	if( ElementAt(nLen-1).m_date < pother->ElementAt(0).m_date )
		CopyData( *pother );
//	else if( ElementAt(0).m_date > pother->ElementAt(nLenOther-1).m_date )
//		;
//	else if( ElementAt(0).m_date <= pother->ElementAt(0).m_date
//		&& ElementAt(nLen-1).m_date >= pother->ElementAt(nLenOther-1).m_date )
//		;
	else if( ElementAt(0).m_date > pother->ElementAt(0).m_date
		&& ElementAt(nLen-1).m_date < pother->ElementAt(nLenOther-1).m_date )
		CopyData( *pother );
	else if( ElementAt(0).m_date <= pother->ElementAt(0).m_date
		&& ElementAt(nLen-1).m_date < pother->ElementAt(nLenOther-1).m_date )
	{
		// append from pother
		DWORD	date	=	ElementAt(nLen-1).m_date;
		SetSize( GetSize(), pother->GetSize() );
		for( int i=0; i<pother->GetSize(); i++ )
		{
			KDATA	& kd	=	pother->ElementAt(i);
			if( kd.m_date > m_data )
				Add( kd );
		}
	}
	else if( ElementAt(0).m_date >= pother->ElementAt(0).m_date
		&& ElementAt(nLen-1).m_date > pother->ElementAt(nLenOther-1).m_date )
	{
		// insert from pother
		CKData	temp	=	(*pother);
		DWORD	date	=	pother->ElementAt(nLenOther-1).m_date;
		temp.SetSize( temp.GetSize(), GetSize()+5 );
		for( int i=0; i<GetSize(); i++ )
		{
			KDATA	& kd	=	ElementAt(i);
			if( kd.m_date > date )
				temp.Add( kd );
		}
		CopyData( temp );
	}
	
	return GetSize();
*/
}

int CKData::FullFillKData( CKData & kdataMain, BOOL bFillToEnd )
{
	SP_ASSERT( GetKType() == kdataMain.GetKType() );
	if( GetKType() != kdataMain.GetKType() )
		return 0;
	if( GetSize() == 0 || kdataMain.GetSize() == 0 )
		return 0;

	DWORD	dateBegin	=	ElementAt(0).m_date;
	DWORD	dateMainEnd5	=	(kdataMain.GetSize() >= 5 ? kdataMain.ElementAt(kdataMain.GetSize()-5).m_date : 0);
	DWORD	dateEnd5		=	(GetSize() >= 5 ? ElementAt(GetSize()-5).m_date : 0);
	int		iMain = 0, iSelf = 0;
	for( iMain=0; iMain<kdataMain.GetSize(); iMain ++ )
	{
		if( dateBegin == kdataMain.ElementAt(iMain).m_date )
			break;
	}

	SetSize( GetSize(), kdataMain.GetSize()-iMain-GetSize() > 0 ? kdataMain.GetSize()-iMain-GetSize() : -1 );
	int	nCount	=	0;
	for( ; iMain <= kdataMain.GetSize() && iSelf <= GetSize(); iMain++, iSelf++ )
	{
		if( !bFillToEnd && iSelf == GetSize() && ElementAt(iSelf-1).m_date < dateMainEnd5 )
			break;
		if( !bFillToEnd && iMain == kdataMain.GetSize() && kdataMain.ElementAt(iMain-1).m_date < dateEnd5 )
			break;
		while( iMain > 0 && iMain <= kdataMain.GetSize() && iSelf < GetSize()
				&& ( iMain == kdataMain.GetSize() || kdataMain.ElementAt(iMain).m_date > ElementAt(iSelf).m_date ) )
		{
//			KDATA	kd;
//			memset( &kd, 0, sizeof(kd) );
//			kd.m_date	=	ElementAt(iSelf).m_date;
//			kd.open	=	kd.high	=	kd.low	=	kd.close	=	kdataMain.ElementAt(iMain-1).close;
//			kdataMain.InsertAt( iMain, kd );
//			iMain	++;
			iSelf	++;
//			nCount	++;
		}

		while( iMain < kdataMain.GetSize() && iSelf <= GetSize() && iSelf > 0
				&& ( iSelf == GetSize() || kdataMain.ElementAt(iMain).m_date < ElementAt(iSelf).m_date ) )
		{
			KDATA	kd;
			memset( &kd, 0, sizeof(kd) );
			kd.m_date	=	kdataMain.ElementAt(iMain).m_date;
			kd.m_fOpen	=	kd.m_fHigh	=	kd.m_fLow	=	kd.m_fClose	=	ElementAt(iSelf-1).m_fClose;
			InsertAt( iSelf, kd );
			iMain	++;
			iSelf	++;
			nCount	++;
		}
	}

	return nCount;
}

BOOL CKData::IsAdjacentDays( int nIndex, int nDays )
{
	// check parameters
	SP_ASSERT( nIndex >= 0 && nIndex < GetSize() && nDays >= 1 );
	if( nIndex < 0 || nIndex >= GetSize() || nDays < 1 )
		return FALSE;

	// data not enougy
	if( nDays > nIndex )
		return FALSE;

	CSPTime	sptime1, sptime2;
	sptime1.FromStockTime( ElementAt(nIndex-nDays).m_date, CKData::IsDayOrMin(m_nKType) );
	sptime1.FromStockTime( ElementAt(nIndex).m_date, CKData::IsDayOrMin(m_nKType) );

	if( CKData::ktypeMonth == m_nKType )
	{
		if( sptime2 - sptime1 >= CSPTimeSpan(nDays+63,0,0,0) )
			return FALSE;
	}
	else if( CKData::ktypeWeek == m_nKType )
	{
		if( sptime2 - sptime1 >= CSPTimeSpan(nDays+15,0,0,0) )
			return FALSE;
	}
	else
	{
		if( sptime2 - sptime1 >= CSPTimeSpan(nDays+8,0,0,0) )
			return FALSE;
	}
	return TRUE;
}

BOOL CKData::GetDiff( double * pValue, DWORD dateCur, int nDays )
{
	SP_ASSERT( pValue && nDays > 0 );

	// Find date Current to calculate from
	int		nIndex	=	GetIndexByDate( dateCur );
	if( -1 == nIndex )
		return FALSE;

	// data not enough
	if( nDays > nIndex )
		return FALSE;

	// 检查是否是相邻成交日
	if( !IsAdjacentDays( nIndex, nDays ) )
		return FALSE;

	if( ElementAt(nIndex-nDays).m_fClose < 1e-4 )
		return FALSE;
	if( pValue )
		*pValue	=	ElementAt(nIndex).m_fClose - ElementAt(nIndex-nDays).m_fClose;
	return TRUE;
}

BOOL CKData::GetDiffPercent( double * pValue, DWORD dateCur, int nDays )
{
	SP_ASSERT( pValue && nDays > 0 );

	// Find date Current to calculate from
	int		nIndex	=	GetIndexByDate( dateCur );
	if( -1 == nIndex )
		return FALSE;

	// data not enough
	if( nDays > nIndex )
		return FALSE;

	// 检查是否是相邻成交日
	if( !IsAdjacentDays( nIndex, nDays ) )
		return FALSE;

	if( ElementAt(nIndex-nDays).m_fAmount < 1e-4 )
		return FALSE;
	if( pValue )
		*pValue	=	(100. * ElementAt(nIndex).m_fClose) / ElementAt(nIndex-nDays).m_fClose - 100;
	return TRUE;
}

BOOL CKData::GetScope( double * pValue, DWORD dateCur, int nDays )
{
	SP_ASSERT( pValue && nDays > 0 );

	// Find date Current to calculate from
	int		nIndex	=	GetIndexByDate( dateCur );
	if( -1 == nIndex )
		return FALSE;

	// data not enough
	if( nDays > nIndex )
		return FALSE;

	// 检查是否是相邻成交日
	if( !IsAdjacentDays( nIndex, nDays ) )
		return FALSE;

	double	dMax = 0, dMin = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=0; k-- )
	{
		if( nIndex == k )
		{
			dMin	=	ElementAt(k).m_fLow;
			dMax	=	ElementAt(k).m_fHigh;
		}
		if( dMin > ElementAt(k).m_fLow )	dMin	=	ElementAt(k).m_fLow;
		if( dMax < ElementAt(k).m_fHigh )	dMax	=	ElementAt(k).m_fHigh;

		nCount	++;
		if( nCount == nDays )
			break;
	}
	// data not enough
	if( nCount != nDays || nCount <= 0 )
		return FALSE;

	if( ElementAt(nIndex-nDays).m_fClose <= 0 )
		return FALSE;
	if( pValue )
		*pValue	=	( 100. * (dMax-dMin) ) / ElementAt(nIndex-nDays).m_fClose ;
	return TRUE;
}

BOOL CKData::GetVolumeSum( double * pValue, DWORD dateCur, int nDays )
{
	SP_ASSERT( pValue && nDays > 0 );

	// Find date Current to calculate from
	int		nIndex	=	GetIndexByDate( dateCur );
	if( -1 == nIndex )
		return FALSE;

	// data not enough
	if( nDays > nIndex )
		return FALSE;

	// 检查是否是相邻成交日
	if( !IsAdjacentDays( nIndex, nDays ) )
		return FALSE;

	// begin calculate
	double	dAll	=	0;
	int		nCount	=	0;
	for( int i=nIndex; i>=0; i-- )
	{
		dAll	+=	ElementAt(i).m_fVolume;
		nCount	++;
		if( nCount >= nDays )
			break;
	}
	// data not enough
	if( nCount != nDays || nCount <= 0 )
		return FALSE;

	if( pValue )
		*pValue	=	dAll;
	return TRUE;
}

BOOL CKData::GetRatioVolume( double * pValue, DWORD dateCur, int nDays )
{
	SP_ASSERT( pValue && nDays > 0 );

	// Find date Current to calculate from
	int		nIndex	=	GetIndexByDate( dateCur );
	if( -1 == nIndex )
		return FALSE;

	// data not enough
	if( nDays > nIndex )
		return FALSE;

	// 检查是否是相邻成交日
	if( !IsAdjacentDays( nIndex, nDays ) )
		return FALSE;

	// begin calculate
	double	dAll	=	0;
	int		nCount	=	0;
	for( int i=nIndex-1; i>=0; i-- )
	{
		dAll	+=	ElementAt(i).m_fVolume;
		nCount	++;
		if( nCount >= nDays )
			break;
	}
	// data not enough
	if( nCount != nDays || nCount <= 0 )
		return FALSE;

	if( fabs(dAll) < 1 )
		return FALSE;

	if( pValue )
		*pValue	=	(ElementAt(nIndex).m_fVolume / dAll ) * nCount;
	return TRUE;
}

BOOL CKData::GetRS( double * pValue, DWORD dateCur, int nDays )
{
	SP_ASSERT( pValue && nDays > 0 );

	// Find date Current to calculate from
	int		nIndex	=	GetIndexByDate( dateCur );
	if( -1 == nIndex )
		return FALSE;

	// data not enough
	if( nDays > nIndex )
		return FALSE;

	// 检查是否是相邻成交日
	if( !IsAdjacentDays( nIndex, nDays ) )
		return FALSE;

	int	nCount	=	0;
	double	dA = 0, dB = 0;
	for( int k=nIndex; k>=1; k-- )
	{
		if( ElementAt(k).m_fClose > ElementAt(k-1).m_fClose )
			dA	+=	(ElementAt(k).m_fClose - ElementAt(k-1).m_fClose);
		else
			dB	+=	(ElementAt(k-1).m_fClose - ElementAt(k).m_fClose);
		
		nCount	++;
		if( nCount == nDays )
		{
			double	dResult;
			if( fabs(dB) < 1e-4 )
				dResult	=	100;
			else
				dResult	=	dA / dB;
			if( pValue )
				*pValue	=	dResult;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CKData::GetMA( double * pValue, int nIndex, int nDays )
{
	SP_ASSERT( nIndex >= 0 && nIndex < GetSize() && nDays > 0 );
	if( nIndex < 0 || nIndex >=GetSize() || nDays <= 0 )
		return FALSE;

	int	nCount	=	0;
	if( nDays > nIndex+1 )
		return FALSE;
	double	dResult	=	0;
	for( int k=nIndex; k>=0; k-- )
	{
		dResult	+=	MaindataAt(k);
		nCount	++;
		if( nCount == nDays )
		{
			if( pValue )
				*pValue	=	dResult / nDays;
			return TRUE;
		}
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Private Operations
int CKData::ConvertKData( CKData &kdSrc, CKData &kdDest, int multiple )
{
	// convert kdSrc k line to kdDest k line accordding to their multiple
	SP_ASSERT( multiple > 1 );
	if( multiple < 2 )	return 0;

	kdDest.SetSize( 0, kdSrc.GetSize() / multiple + 5 );
	
	int nStart		=	kdSrc.GetSize() % multiple;
	int	nCount		=	0;
	KDATA	dataDest;
	for( int pos=nStart; pos<kdSrc.GetSize(); pos++ )
	{
		KDATA & dataSrc = kdSrc.ElementAt( pos );

		nCount	++;
		if( 1 == nCount )
		{
			memcpy( &dataDest, &dataSrc, sizeof(dataDest) );
		}
		else
		{
			dataDest.m_fAmount		+=	dataSrc.m_fAmount;
			if( dataDest.m_fHigh < dataSrc.m_fHigh )	dataDest.m_fHigh	=	dataSrc.m_fHigh;
			if( dataDest.m_fLow > dataSrc.m_fLow )		dataDest.m_fLow		=	dataSrc.m_fLow;
			dataDest.m_fVolume		+=	dataSrc.m_fVolume;
			dataDest.m_fClose		=	dataSrc.m_fClose;
		}
		
		if( multiple == nCount )	// new dataDest is over
		{
			nCount	=	0;
			kdDest.Add( dataDest );
		}
	}
	
	return kdDest.GetSize();
}

void CKData::CopyData( const CKData &src )
{
	if( m_pDataOriginal )
	{
		delete [] (BYTE*)m_pDataOriginal;
		m_pDataOriginal	=	NULL;
		m_nSizeOriginal = m_nMaxSizeOriginal = 0;
	}
	if( m_pData )
	{
		delete [] (BYTE*)m_pData;
		m_pData	=	NULL;
		m_nSize = m_nMaxSize = m_nGrowBy = 0;
	}

	if( src.m_nSizeOriginal > 0 )
	{
		SP_ASSERT( src.m_pDataOriginal );
		SP_ASSERT( src.m_nMaxSizeOriginal >= src.m_nSizeOriginal );
		m_pDataOriginal = (KDATA*) new BYTE[src.m_nMaxSizeOriginal * sizeof(KDATA)];
		if( m_pDataOriginal )
		{
			memset( m_pDataOriginal, 0, src.m_nMaxSizeOriginal * sizeof(KDATA) );  // zero fill
			memcpy( m_pDataOriginal, src.m_pDataOriginal, src.m_nSizeOriginal * sizeof(KDATA) );
			m_nSizeOriginal		=	src.m_nSizeOriginal;
			m_nMaxSizeOriginal	=	src.m_nMaxSizeOriginal;
		}
	}
	if( src.m_nSize > 0 )
	{
		SP_ASSERT( src.m_pData );
		SP_ASSERT( src.m_nMaxSize >= src.m_nSize );
		m_pData = (KDATA*) new BYTE[src.m_nMaxSize * sizeof(KDATA)];
		if( m_pData )
		{
			memset( m_pData, 0, src.m_nMaxSize * sizeof(KDATA) );  // zero fill
			memcpy( m_pData, src.m_pData, src.m_nSize * sizeof(KDATA) );
			m_nSize		=	src.m_nSize;
			m_nMaxSize	=	src.m_nMaxSize;
		}
	}
}

void CKData::LoadDataOriginal( )
{
	if( m_pDataOriginal )
	{
		if( m_pData )
			delete [] (BYTE*)m_pData;

		m_pData	=	m_pDataOriginal;
		m_nSize	=	m_nSizeOriginal;
		m_nMaxSize	=	m_nMaxSizeOriginal;

		m_pDataOriginal	=	NULL;
		m_nSizeOriginal = m_nMaxSizeOriginal = 0;
	}
}

void CKData::StoreDataOriginal( )
{
	if( NULL == m_pDataOriginal )
	{
		m_pDataOriginal		=	m_pData;
		m_nSizeOriginal		=	m_nSize;
		m_nMaxSizeOriginal	=	m_nMaxSize;

		m_pData	=	NULL;
		m_nSize = m_nMaxSize = 0;
	}
}

float CKData::GetRatio( float fLastClose, DRDATA & dr )
{
	if( fLastClose < 1e-4 )
		return	1.0000;

	float	fRatio	=	1.0000;
	if( dr.m_fProfit > 1e-4 )
	{
		fRatio	=	fRatio * ( 1 - dr.m_fProfit/fLastClose );
	}
	if( dr.m_fGive > 1e-4 )
	{
		fRatio	=	(float)( fRatio * 1./(1.+dr.m_fGive) );
	}
	if( dr.m_fPei > 1e-4 )
	{
		float	priceNow = (float)( (dr.m_fPeiPrice * dr.m_fPei + fLastClose)/(1.+dr.m_fPei) );
		fRatio	=	fRatio * priceNow / fLastClose;
	}
	return fRatio;
}

void CKData::ConvertXDR( BOOL bUP, DWORD dateAutoDRBegin, double dAutoDRLimit )
{
	CDRData	drtemp;
	drtemp	=	m_drdata;
	drtemp.Sort( );

	if( m_pData )
	{
		delete [] (BYTE*)m_pData;
		m_nSize	=	m_nMaxSize	=	0;
		m_pData	=	NULL;
	}

	if( NULL == m_pDataOriginal || 0 == m_nSizeOriginal )
		return;
	
/*	if( drtemp.GetSize() == 0 )
	{
		SetSize( m_nSizeOriginal );
		if( m_pData )
			memcpy( m_pData, m_pDataOriginal, sizeof(KDATA)*m_nSize );
		return;
	}
*/
	dAutoDRLimit	=	dAutoDRLimit / 100;

	if( bUP )
	{
		SetSize( 0, m_nSizeOriginal );
		int	drPos	=	0;
		float	fRatio	=	1.000000;
		for( int i=0; i<m_nSizeOriginal; i++ )
		{
			KDATA	& kd = m_pDataOriginal[i];
			KDATA	newkd	=	kd;
			
			if( drPos < drtemp.GetSize() && ToDayDate(kd.m_date) >= drtemp.ElementAt(drPos).m_date )
			{
				if( i > 0 )
				{
					KDATA	kdLast	=	m_pDataOriginal[i-1];
					fRatio	=	fRatio * GetRatio( kdLast.m_fClose, drtemp.ElementAt(drPos) );
				}
				
				drPos	++;
			}
			else if( ToDayDate(kd.m_date) >= dateAutoDRBegin && i > 0 )	//	Auto XDR
			{
				KDATA	kdLast	=	m_pDataOriginal[i-1];
				if( kdLast.m_fClose > 1e-4 && kd.m_fOpen < kdLast.m_fClose
					&& fabs(kd.m_fOpen/kdLast.m_fClose-1) > dAutoDRLimit )
					fRatio	=	fRatio * kd.m_fOpen / kdLast.m_fClose;
			}

			newkd.m_fOpen	=	(kd.m_fOpen / fRatio);
			newkd.m_fHigh	=	(kd.m_fHigh / fRatio);
			newkd.m_fLow	=	(kd.m_fLow / fRatio);
			newkd.m_fClose	=	(kd.m_fClose / fRatio);
			newkd.m_fVolume	=	(kd.m_fVolume * fRatio);
			Add( newkd );
		}
	}
	else
	{
		SetSize( m_nSizeOriginal );
		int	drPos	=	drtemp.GetSize()-1;
		float	fRatio	=	1.000000;
		for( int i=m_nSizeOriginal-1; i>=0; i-- )
		{
			KDATA	& kd = m_pDataOriginal[i];
			KDATA	newkd	=	kd;
			
			if( drPos >= 0 && ToDayDate(kd.m_date) < drtemp.ElementAt(drPos).m_date )
			{
				if( i < m_nSizeOriginal-1 )
					fRatio	=	fRatio * GetRatio( kd.m_fClose, drtemp.ElementAt(drPos) );
				
				drPos	--;
			}
			else if( ToDayDate(kd.m_date) >= dateAutoDRBegin && i+1 < m_nSizeOriginal )	//	Auto XDR
			{
				KDATA	kdNext	=	m_pDataOriginal[i+1];
				if( kdNext.m_fOpen > 1e-4 && kdNext.m_fOpen < kd.m_fClose
					&& fabs(kdNext.m_fOpen/kd.m_fClose-1) > dAutoDRLimit )
					fRatio	=	fRatio * kdNext.m_fOpen / kd.m_fClose;
			}

			newkd.m_fOpen	=	(kd.m_fOpen * fRatio);
			newkd.m_fHigh	=	(kd.m_fHigh * fRatio);
			newkd.m_fLow	=	(kd.m_fLow * fRatio);
			newkd.m_fClose	=	(kd.m_fClose * fRatio);
			newkd.m_fVolume	=	(kd.m_fVolume / fRatio);
			SetAt( i, newkd );
		}
	}
}

