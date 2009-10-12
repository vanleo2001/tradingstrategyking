/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CStock;
*/

#include	"StdAfx.h"
#include	"../Include/Stock.h"
#include	"../Include/Database.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif

#ifndef	_SP_ENABLE_INLINES
#define	_STOCK_INLINE
#include "../Include/Stock.inl"
#undef	_STOCK_INLINE
#endif //_SP_ENABLE_INLINES


//////////////////////////////////////////////////////////////////////
//	class	CStockInfo

CStockInfo::CStockInfo( )
{
	Clear( );
}

CStockInfo::CStockInfo( const CStockInfo &src )
{
	Clear( );
	*this	=	src;
}

void CStockInfo::Clear( )
{
	m_dwMarket	=	CStock::marketUnknown;
	memset( m_szCode, 0, sizeof(m_szCode) );

	m_type	=	CStock::typeNone;
	memset( m_szName, 0, sizeof(m_szName) );
	memset( m_szNameEnu, 0, sizeof(m_szNameEnu) );
	memset( m_szShortname, 0, sizeof(m_szShortname) );

	memset( m_szDomain, 0, sizeof(m_szDomain) );
	memset( m_szProvince, 0, sizeof(m_szProvince) );

	m_kdata.RemoveAll();
	m_kdata.SetKType(CKData::ktypeDay);
	m_basedata.RemoveAll();
	m_minute.RemoveAll();
	memset( &m_reportLatest, 0, sizeof(m_reportLatest) );

	m_datetech		=	0;

	m_fLast			=	0;
	m_fOpen			=	0;
	m_fHigh			=	0;
	m_fLow			=	0;
	m_fClose		=	0;
	m_fVolume		=	0;
	m_fAmount		=	0;
	m_dwAdvance		=	0;
	m_dwDecline		=	0;


	memset( m_fBuyPrice, 0, sizeof(m_fBuyPrice) );
	memset( m_fBuyVolume, 0, sizeof(m_fBuyVolume) );
	memset( m_fSellPrice, 0, sizeof(m_fSellPrice) );
	memset( m_fSellVolume, 0, sizeof(m_fSellVolume) );

	// base data
	m_datebase				=	0;
	m_reporttype			=	0;
	m_fErate_dollar			=	(float)8.27;
	m_fErate_hkdollar		=	(float)1.06;
	
	m_fRatio_liquidity		=	0;
	m_fRatio_quick			=	0;
	m_fVelocity_receivables	=	0;
	
	m_fVelocity_merchandise	=	0;
	m_fMain_income			=	0;
	m_fCash_ps				=	0;
	
	m_fProfit_margin		=	0;
	m_fNetasset_yield		=	0;
	
	m_datebegin				=	0;
	m_fShare_count_total	=	0;
	m_fShare_count_a		=	0;
	m_fShare_count_b		=	0;
	m_fShare_count_h		=	0;
	m_fShare_count_national	=	0;
	m_fShare_count_corp		=	0;
	m_fProfit_psud			=	0;
	m_fAsset				=	0;
	m_fRatio_holderright	=	0;
	m_fRatio_longdebt		=	0;
	m_fRatio_debt			=	0;
	
	m_fNetasset_ps			=	0;
	m_fNetasset_ps_regulate	=	0;
	m_fEps					=	0;
	m_fEps_deduct			=	0;
	m_fNet_profit			=	0;
	m_fMain_profit			=	0;
	m_fTotal_profit			=	0;

	m_fProfit_inc			=	0;
	m_fIncome_inc			=	0;
	m_fAsset_inc			=	0;
	m_fYield_average		=	(float) STKLIB_DATA_INVALID;
	m_fYield_stddev			=	(float) STKLIB_DATA_INVALID;
	m_fBeite				=	(float) STKLIB_DATA_INVALID;
}

CStockInfo &CStockInfo::operator = ( const CStockInfo & si )
{
	m_dwMarket			=	si.m_dwMarket;
	memcpy( m_szCode, si.m_szCode, sizeof(m_szCode) );

	m_type	=	si.m_type;

	memcpy( m_szName, si.m_szName, sizeof(m_szName) );
	memcpy( m_szNameEnu, si.m_szNameEnu, sizeof(m_szNameEnu) );
	memcpy( m_szShortname, si.m_szShortname, sizeof(m_szShortname) );

	memcpy( m_szDomain, si.m_szDomain, sizeof(m_szDomain) );
	memcpy( m_szProvince, si.m_szProvince, sizeof(m_szProvince) );

	m_kdata			=	si.m_kdata;
	m_basedata		=	si.m_basedata;
	m_minute		=	si.m_minute;
	m_reportLatest	=	si.m_reportLatest;

	m_datetech		=	si.m_datetech;

	m_fLast			=	si.m_fLast;
	m_fOpen			=	si.m_fOpen;
	m_fHigh			=	si.m_fHigh;
	m_fLow			=	si.m_fLow;
	m_fClose		=	si.m_fClose;
	m_fVolume		=	si.m_fVolume;
	m_fAmount		=	si.m_fAmount;
	m_dwAdvance		=	si.m_dwAdvance;
	m_dwDecline		=	si.m_dwDecline;


	memcpy( m_fBuyPrice, si.m_fBuyPrice, sizeof(m_fBuyPrice) );
	memcpy( m_fBuyVolume, si.m_fBuyVolume, sizeof(m_fBuyVolume) );
	memcpy( m_fSellPrice, si.m_fSellPrice, sizeof(m_fSellPrice) );
	memcpy( m_fSellVolume, si.m_fSellVolume, sizeof(m_fSellVolume) );

	// base data
	m_datebase				=	si.m_datebase;
	m_reporttype			=	si.m_reporttype;
	m_fErate_dollar			=	si.m_fErate_dollar;
	m_fErate_hkdollar		=	si.m_fErate_hkdollar;

	m_fRatio_liquidity		=	si.m_fRatio_liquidity;
	m_fRatio_quick			=	si.m_fRatio_quick;
	m_fVelocity_receivables	=	si.m_fVelocity_receivables;
	
	m_fVelocity_merchandise	=	si.m_fVelocity_merchandise;
	m_fMain_income			=	si.m_fMain_income;
	m_fCash_ps				=	si.m_fCash_ps;
	
	m_fProfit_margin		=	si.m_fProfit_margin;
	m_fNetasset_yield		=	si.m_fNetasset_yield;
	
	m_datebegin				=	si.m_datebegin;
	m_fShare_count_total	=	si.m_fShare_count_total;
	m_fShare_count_a		=	si.m_fShare_count_a;
	m_fShare_count_b		=	si.m_fShare_count_b;
	m_fShare_count_h		=	si.m_fShare_count_h;
	m_fShare_count_national	=	si.m_fShare_count_national;
	m_fShare_count_corp		=	si.m_fShare_count_corp;
	m_fProfit_psud			=	si.m_fProfit_psud;
	m_fAsset				=	si.m_fAsset;
	m_fRatio_holderright	=	si.m_fRatio_holderright;
	m_fRatio_longdebt		=	si.m_fRatio_longdebt;
	m_fRatio_debt			=	si.m_fRatio_debt;
	
	m_fNetasset_ps			=	si.m_fNetasset_ps;
	m_fNetasset_ps_regulate	=	si.m_fNetasset_ps_regulate;
	m_fEps					=	si.m_fEps;
	m_fEps_deduct			=	si.m_fEps_deduct;
	m_fNet_profit			=	si.m_fNet_profit;
	m_fMain_profit			=	si.m_fMain_profit;
	m_fTotal_profit			=	si.m_fTotal_profit;

	m_fProfit_inc			=	si.m_fProfit_inc;
	m_fIncome_inc			=	si.m_fIncome_inc;
	m_fAsset_inc			=	si.m_fAsset_inc;
	m_fYield_average		=	si.m_fYield_average;
	m_fYield_stddev			=	si.m_fYield_stddev;
	m_fBeite				=	si.m_fBeite;

	return *this;
}

BOOL CStockInfo::IsEqualTo( DWORD dwMarket, const char * szCode )
{
	if( m_dwMarket == dwMarket
		|| CStock::marketUnknown == dwMarket || CStock::marketUnknown == m_dwMarket
		|| 0 == dwMarket || 0 == m_dwMarket )
	{
		if( szCode && 0 == strcmp(szCode,m_szCode) )
			return TRUE;
	}
	if( CStock::marketCHNA == m_dwMarket
		&& ( CStock::marketCHNA == dwMarket || CStock::marketSHSE == dwMarket || CStock::marketSZSE == dwMarket || CStock::marketCYSE == dwMarket ) )
	{
		if( szCode && 0 == strcmp(szCode,m_szCode) )
			return TRUE;
	}
	if( CStock::marketCHNA == dwMarket
		&& ( CStock::marketCHNA == m_dwMarket || CStock::marketSHSE == m_dwMarket || CStock::marketSZSE == m_dwMarket || CStock::marketCYSE == m_dwMarket ) )
	{
		if( szCode && 0 == strcmp(szCode,m_szCode) )
			return TRUE;
	}

	return FALSE;
}

DWORD CStockInfo::GetMarket( )
{
	return m_dwMarket;
}

void CStockInfo::SetMarket( DWORD dwMarket )
{
	m_dwMarket	=	dwMarket;
	if( CStock::marketSHSE == dwMarket )
		m_type	|=	CStock::markShanghai;
	if( CStock::marketSZSE == dwMarket )
		m_type	|=	CStock::markShenzhen;
}

void CStockInfo::ResolveTypeAndMarket( )
{
	CStockInfo	info;
	if( AfxGetStockContainer().GetStockInfo( m_szCode, &info )
		&& CStock::typeNone != info.GetType()
		&& CStock::marketUnknown != info.GetMarket() )
	{
		if( CStock::typeNone == m_type )
			m_type = info.GetType();
		if( CStock::marketUnknown == m_dwMarket )
			m_dwMarket = info.GetMarket();
		return;
	}

	long	type = CStock::typeNone;
	DWORD	dwMarket = CStock::marketUnknown;
	if( strlen(m_szCode) > 0 )
	{
		if( strcmp(m_szCode,"000001") >= 0 && strcmp(m_szCode,"000999") <= 0 )		// 深圳A股
		{	type = CStock::typeszA;			dwMarket = CStock::marketSZSE;	}
		else if( strcmp(m_szCode,"002001") >= 0 && strcmp(m_szCode,"002999") <= 0 )	// 深圳中小企业
		{	type = CStock::typeszMsmall;			dwMarket = CStock::marketSZSE;	}
		else if( strcmp(m_szCode,"200001") >= 0 && strcmp(m_szCode,"200999") <= 0 )	// 深圳B股
		{	type = CStock::typeszB;			dwMarket = CStock::marketSZSE;	}
		else if( strncmp(m_szCode,"600",3) == 0 )	// 上海A股
		{	type = CStock::typeshA;			dwMarket = CStock::marketSHSE;	}
		else if( strncmp(m_szCode,"500",3) == 0 )	// 上海基金
		{	type = CStock::typeshA;			dwMarket = CStock::marketSHSE;	}
		else if( strncmp(m_szCode,"900",3) == 0 )	// 上海B股
		{	type = CStock::typeshB;			dwMarket = CStock::marketSHSE;	}

		else if( strcmp(m_szCode,"001001") >= 0 && strcmp(m_szCode,"001199") <= 0 )	// 深圳企业债券
		{	type = CStock::typeszBond;		dwMarket = CStock::marketSZSE;	}
		else if( strcmp(m_szCode,"001200") >= 0 && strcmp(m_szCode,"001899") <= 0 )	// 深圳国债回购
		{	type = CStock::typeszBond;		dwMarket = CStock::marketSZSE;	}
		else if( strcmp(m_szCode,"001900") >= 0 && strcmp(m_szCode,"001999") <= 0 )	// 深圳国债现货
		{	type = CStock::typeszBond;		dwMarket = CStock::marketSZSE;	}
		else if( strcmp(m_szCode,"030001") >= 0 && strcmp(m_szCode,"038999") <= 0 )	// 深圳A股权证
		{	type = CStock::typeszRight;		dwMarket = CStock::marketSZSE;	}
		else if( strcmp(m_szCode,"003001") >= 0 && strcmp(m_szCode,"003999") <= 0 )	// 深圳转配认购
		{	type = CStock::typeszAdmeasure;	dwMarket = CStock::marketSZSE;	}
		else if( strcmp(m_szCode,"009001") >= 0 && strcmp(m_szCode,"009999") <= 0 )	// 深圳新股配售
		{	type = CStock::typeszAdmeasure;	dwMarket = CStock::marketSZSE;	}
		else if( strcmp(m_szCode,"005001") >= 0 && strcmp(m_szCode,"005999") <= 0 )	// 深圳可转换债券
		{	type = CStock::typeszBond;		dwMarket = CStock::marketSZSE;	}
		else if( strcmp(m_szCode,"006001") >= 0 && strcmp(m_szCode,"006999") <= 0 )	// 深圳国债期货
		{	type = CStock::typeszBond;		dwMarket = CStock::marketSZSE;	}
		else if( strcmp(m_szCode,"004601") >= 0 && strcmp(m_szCode,"004699") <= 0 )	// 深圳证券投资基金
		{	type = CStock::typeszA;			dwMarket = CStock::marketSZSE;	}
		else if( strcmp(m_szCode,"008001") >= 0 && strcmp(m_szCode,"008999") <= 0 )	// 深圳配股认购
		{	type = CStock::typeszAdmeasure;	dwMarket = CStock::marketSZSE;	}

		else if( strncmp(m_szCode,"001",3) == 0 )	// 上海国债现货
		{	type = CStock::typeshBond;		dwMarket = CStock::marketSHSE;	}
		else if( strncmp(m_szCode,"110",3) == 0 || strncmp(m_szCode,"120",3) == 0 )	// 上海企业债券
		{	type = CStock::typeshBond;		dwMarket = CStock::marketSHSE;	}
		else if( strncmp(m_szCode,"310",3) == 0 )	// 上海国债期货
		{	type = CStock::typeshBond;		dwMarket = CStock::marketSHSE;	}
		else if( strncmp(m_szCode,"580",3) == 0 )	// 上海A股权证
		{	type = CStock::typeshRight;		dwMarket = CStock::marketSHSE;	}
		else if( strncmp(m_szCode,"710",3) == 0 )	// 上海转配股
		{	type = CStock::typeshAdmeasure;	dwMarket = CStock::marketSHSE;	}
		else if( strncmp(m_szCode,"711",3) == 0 )	// 上海转配股再转配股
		{	type = CStock::typeshAdmeasure;	dwMarket = CStock::marketSHSE;	}
		else if( strncmp(m_szCode,"730",3) == 0 )	// 上海新股申购
		{	type = CStock::typeshAdmeasure;	dwMarket = CStock::marketSHSE;	}
		else if( strncmp(m_szCode,"737",3) == 0 )	// 上海新股配售
		{	type = CStock::typeshAdmeasure;	dwMarket = CStock::marketSHSE;	}
		else if( strncmp(m_szCode,"201",3) == 0 )	// 上海国债回购	
		{	type = CStock::typeshBond;		dwMarket = CStock::marketSHSE;	}
		else if( strncmp(m_szCode,"129",3) == 0 || strncmp(m_szCode,"100",3) == 0 )	// 上海可转换债券
		{	type = CStock::typeshBond;		dwMarket = CStock::marketSHSE;	}
		else if( strncmp(m_szCode,"700",3) == 0 )	// 上海配股
		{	type = CStock::typeshAdmeasure;	dwMarket = CStock::marketSHSE;	}
		else if( strncmp(m_szCode,"701",3) == 0 )	// 上海转配股再配股
		{	type = CStock::typeshAdmeasure;	dwMarket = CStock::marketSHSE;	}
		else if( strncmp(m_szCode,"720",3) == 0 )	// 上海红利
		{	type = CStock::typeshBonus;		dwMarket = CStock::marketSHSE;	}
		else if( strncmp(m_szCode,"735",3) == 0 )	// 上海新基金申购
		{	type = CStock::typeshAdmeasure;	dwMarket = CStock::marketSHSE;	}
	}

	if( CStock::typeNone == m_type )
		m_type		= type;
	if( CStock::marketUnknown == m_dwMarket )
		m_dwMarket	= dwMarket;
}

const char * CStockInfo::GetStockCode( )
{
	return m_szCode;
}

BOOL CStockInfo::SetStockCode( DWORD dwMarket, const char * szCode )
{
	if( NULL == szCode || strlen(szCode) <= 0 )
	{
		Clear();
		return FALSE;
	}

	if( IsEqualTo( dwMarket, szCode ) )
	{
		if( dwMarket != CStock::marketUnknown )
			m_dwMarket	=	dwMarket;
		return IsValidStock();
	}

	Clear( );

	m_dwMarket	=	dwMarket;
	memset(m_szCode, 0, sizeof(m_szCode) );
	strncpy( m_szCode, szCode, min(sizeof(m_szCode)-1,strlen(szCode)) );
	ResolveTypeAndMarket();
	return IsValidStock();
}

LONG CStockInfo::GetType( )
{
	return m_type;
}

void CStockInfo::SetType( LONG t )
{
	m_type	=	t;

	if( CStock::marketUnknown == m_dwMarket )
	{
		if( IsShenZhen() )
			m_dwMarket	=	CStock::marketSZSE;
		if( IsShangHai() )
			m_dwMarket	=	CStock::marketSHSE;
	}
}

const char * CStockInfo::GetStockName( )
{
#ifdef	CLKLAN_ENGLISH_US
	if( strlen(m_szNameEnu) > 0 )
		return m_szNameEnu;
	if( strlen(m_szName) > 0 )
	{
		if( AfxIsEnglish(m_szName,sizeof(m_szName)) )
			return m_szName;
		else
		{
			CSPString sTemp = AfxFaintlyChsToEnu( m_szName );
			sTemp = AfxMakeSpellCode( sTemp );
			strncpy( m_szNameEnu, sTemp, min(sizeof(m_szNameEnu)-1,sTemp.GetLength()) );
			return m_szNameEnu;
		}
	}
	if( strlen(m_szShortname) > 0 && AfxIsEnglish(m_szShortname,sizeof(m_szShortname))
		&& sizeof(m_szNameEnu) > strlen(m_szCode)+strlen(m_szShortname)+1 )
	{
		sprintf( m_szNameEnu, "%s-%s", m_szCode, m_szShortname );
		return m_szNameEnu;
	}
	return m_szCode;
#else
	return m_szName;
#endif
}

void CStockInfo::SetStockName( const char * szName )
{
	memset(m_szName, 0, sizeof(m_szName) );
	if( NULL == szName )
		return;
	strncpy( m_szName, szName, min(sizeof(m_szName)-1,strlen(szName)) );
}

const char * CStockInfo::GetStockNameEnu( )
{
	return m_szNameEnu;
}

void CStockInfo::SetStockNameEnu( const char *szNameEnu )
{
	memset(m_szNameEnu, 0, sizeof(m_szNameEnu) );
	if( NULL == szNameEnu )
		return;
	strncpy( m_szNameEnu, szNameEnu, min(sizeof(m_szNameEnu)-1,strlen(szNameEnu)) );
}

const char * CStockInfo::GetStockNameChs( )
{
	return m_szName;
}

void CStockInfo::SetStockNameChs( const char *szName )
{
	memset(m_szName, 0, sizeof(m_szName) );
	if( NULL == szName )
		return;
	strncpy( m_szName, szName, min(sizeof(m_szName)-1,strlen(szName)) );
}

const char * CStockInfo::GetStockShortName( )
{
	if( strlen(m_szShortname) <= 0 )
	{
		CSPString sName = m_szName;
		CSPString sTemp = AfxMakeSpellCode( sName, 0x0 );
		strncpy( m_szShortname, sTemp, min(sizeof(m_szShortname)-1,sTemp.GetLength()) );
	}
	return m_szShortname;
}

void CStockInfo::SetStockShortName( const char *szShortName )
{
	memset(m_szShortname, 0, sizeof(m_szShortname) );
	if( NULL == szShortName )
		return;
	strncpy( m_szShortname, szShortName, min(sizeof(m_szShortname)-1,strlen(szShortName)) );
}

const char * CStockInfo::GetStockDomain( )
{
#ifdef	CLKLAN_ENGLISH_US
	if( !AfxIsEnglish(m_szDomain,sizeof(m_szDomain)) )
	{
		CSPString sTemp = AfxFaintlyChsToEnu( m_szDomain );
		memset( m_szDomain, 0, sizeof(m_szDomain) );
		if( AfxIsEnglish(sTemp,sTemp.GetLength()) )
			strncpy( m_szDomain, sTemp, min(sizeof(m_szDomain)-1,sTemp.GetLength()) );
	}
	return m_szDomain;
#else
	return m_szDomain;
#endif
}

void CStockInfo::SetStockDomain( const char * szDomain )
{
	memset(m_szDomain, 0, sizeof(m_szDomain) );
	if( NULL == szDomain )
		return;
	strncpy( m_szDomain, szDomain, min(sizeof(m_szDomain)-1,strlen(szDomain)) );
}

const char * CStockInfo::GetStockProvince( )
{
#ifdef	CLKLAN_ENGLISH_US
	if( !AfxIsEnglish(m_szProvince,sizeof(m_szProvince)) )
	{
		CSPString sTemp = AfxFaintlyChsToEnu( m_szProvince );
		sTemp = AfxMakeSpellCode( sTemp );
		memset( m_szProvince, 0, sizeof(m_szProvince) );
		strncpy( m_szProvince, sTemp, min(sizeof(m_szProvince)-1,sTemp.GetLength()) );
	}
	return m_szProvince;
#else
	return m_szProvince;
#endif
}

void CStockInfo::SetStockProvince( const char * szProvince )
{
	memset(m_szProvince, 0, sizeof(m_szProvince) );
	if( NULL == szProvince )
		return;
	strncpy( m_szProvince, szProvince, min(sizeof(m_szProvince)-1,strlen(szProvince)) );
}

BOOL CStockInfo::StatTechIndex( DWORD dwDate )
{
	SP_ASSERT( 0 != dwDate );

	if( m_kdata.GetSize() == 0 )
		return FALSE;

	int	nIndex	=	-1;
	if( -1 == dwDate )
		nIndex	=	m_kdata.GetSize() - 1;
	else
		nIndex	=	m_kdata.GetIndexByDate( dwDate );

	if( -1 == nIndex )
	{
		m_datetech	=	0;
		m_fLast		=	0;
		m_fOpen		=	0;
		m_fHigh		=	0;
		m_fLow		=	0;
		m_fClose	=	0;
		m_fAmount	=	0;
		m_fVolume	=	0;
		m_dwAdvance	=	0;
		m_dwDecline	=	0;
		
		return FALSE;
	}
	else
	{
		KDATA	& kd	=	m_kdata.ElementAt( nIndex );
		m_datetech	=	kd.m_date;
		if( nIndex > 0 )
			m_fLast	=	m_kdata.ElementAt(nIndex-1).m_fClose;
		else
			m_fLast	=	kd.m_fOpen;
		m_fOpen		=	kd.m_fOpen;
		m_fHigh		=	kd.m_fHigh;
		m_fLow		=	kd.m_fLow;
		m_fClose	=	kd.m_fClose;
		m_fAmount	=	kd.m_fAmount;
		m_fVolume	=	kd.m_fVolume;
		m_dwAdvance	=	kd.m_dwAdvance;
		m_dwDecline	=	kd.m_dwDecline;
		
		return TRUE;
	}
}

BOOL CStockInfo::StatBaseIndex( DWORD dwDate )
{
	SP_ASSERT( 0 != dwDate );

	if( m_basedata.GetSize() == 0 )
		return FALSE;

	int	nIndex	=	m_basedata.GetSize()-1;
	for( int i=m_basedata.GetSize()-1; i>=0; i-- )
	{
		if( m_basedata.ElementAt(i).m_date <= dwDate )
		{
			nIndex	=	i;
			break;
		}
	}

	BASEDATA	& block		=	m_basedata.ElementAt(nIndex);

	strncpy( m_szDomain, block.m_szDomain, min(sizeof(m_szDomain),sizeof(block.m_szDomain)) );
	strncpy( m_szProvince, block.m_szProvince, min(sizeof(m_szProvince),sizeof(block.m_szProvince)) );
	m_datebase				=	block.m_date;
	m_reporttype			=	block.m_reporttype;
	if( block.m_fErate_dollar > 1e-6 )
		m_fErate_dollar			=	block.m_fErate_dollar;
	if( block.m_fErate_hkdollar > 1e-6 )
		m_fErate_hkdollar		=	block.m_fErate_hkdollar;

	m_fRatio_liquidity		=	block.m_fRatio_liquidity;
	m_fRatio_quick			=	block.m_fRatio_quick;
	m_fVelocity_receivables	=	block.m_fVelocity_receivables;

	m_fVelocity_merchandise	=	block.m_fVelocity_merchandise;
	m_fMain_income			=	block.m_fMain_income;
	m_fCash_ps				=	block.m_fCash_ps;
	
	m_fProfit_margin		=	block.m_fProfit_margin;
	m_fNetasset_yield		=	block.m_fNetasset_yield;

	m_datebegin				=	block.m_datebegin;
	m_fShare_count_total	=	block.m_fShare_count_total;
	m_fShare_count_a		=	block.m_fShare_count_a;
	m_fShare_count_b		=	block.m_fShare_count_b;
	m_fShare_count_h		=	block.m_fShare_count_h;
	m_fShare_count_national	=	block.m_fShare_count_national;
	m_fShare_count_corp		=	block.m_fShare_count_corp;
	m_fProfit_psud			=	block.m_fProfit_psud;
	m_fAsset				=	block.m_fAsset;
	m_fRatio_holderright	=	block.m_fRatio_holderright;
	m_fRatio_longdebt		=	block.m_fRatio_longdebt;
	m_fRatio_debt			=	block.m_fRatio_debt;

	m_fNetasset_ps			=	block.m_fNetasset_ps;
	m_fNetasset_ps_regulate	=	block.m_fNetasset_ps_regulate;
	m_fEps					=	block.m_fEps;
	m_fEps_deduct			=	block.m_fEps_deduct;
	m_fNet_profit			=	block.m_fNet_profit;
	m_fMain_profit			=	block.m_fMain_profit;
	m_fTotal_profit			=	block.m_fTotal_profit;

	m_fProfit_inc			=	block.m_fProfit_inc;
	m_fIncome_inc			=	block.m_fIncome_inc;
	m_fAsset_inc			=	block.m_fAsset_inc;

	/* 以下统计值永远只取最近一期数值
	//	★技术数据统计值
	LONG		m_fYield_average;		// 平均收益率% 0.001
	LONG		m_fYield_stddev;		// 收益标准差% 0.001
	LONG		m_fBeite;				// β值 0.001
	*/
	BASEDATA	& blkLatest	=	m_basedata.ElementAt(m_basedata.GetSize()-1);
	m_fYield_average		=	blkLatest.m_fYield_average;
	m_fYield_stddev			=	blkLatest.m_fYield_stddev;
	m_fBeite				=	blkLatest.m_fBeite;

/*** stat
	m_fProfit_inc			=	0;
	m_fIncome_inc			=	0;
	LONG	nProfitCount = 0;
	LONG	nIncomeCount = 0;
	float	fProfitLast = 0, fProfitSum = 0;
	float	fIncomeLast = 0, fIncomeSum = 0;
	for( int i=0; i<m_basedata.GetSize(); i++ )
	{
		BASEDATA	& base	=	m_basedata.ElementAt(i);
		if( fabs(fProfitLast) > 1e-4 )
		{
			nProfitCount	++;
			fProfitSum	+=	(float) ( (base.m_fNet_profit)/nProfitLast - 1 );
		}
		if( fabs(fIncomeLast) > 1e-4 )
		{
			nIncomeCount	++;
			fIncomeSum	+=	(float)( (base.m_fMain_income)/nIncomeLast - 1 );
		}

		fProfitLast	=	base.m_fNet_profit;
		fIncomeLast	=	base.m_fMain_income;
	}
	if( nProfitCount > 0 )
		m_fProfit_inc	=	(float)(((double)fProfitSum)/nProfitCount);
	if( nIncomeCount > 0 )
		m_fIncome_inc	=	(float)(((double)fIncomeSum)/nIncomeCount);
*/
	return TRUE;
}

BOOL CStockInfo::IsValidStock( )
{
	return (strlen(m_szCode) > 0);
}

BOOL CStockInfo::IsShangHai( )
{
	if( CStock::marketSHSE == m_dwMarket )
		return TRUE;
	return (m_type & CStock::markShanghai);
}

BOOL CStockInfo::IsShenZhen( )
{
	if( CStock::marketSZSE == m_dwMarket )
		return TRUE;
	return (m_type & CStock::markShenzhen);
}

BOOL CStockInfo::IsIndex( )
{
	return ( CStock::typeshIndex == m_type || CStock::typeszIndex == m_type );
}

BOOL CStockInfo::IsBond( )
{
	if( CStock::typeshBond == m_type || CStock::typeszBond == m_type )
		return TRUE;
	return ( 0 == strncmp( m_szName, sz_stockname_bond, min(sizeof(m_szName),strlen(sz_stockname_bond)) ) );
}

BOOL CStockInfo::IsBondIndex( )
{
	return ( 0 == strncmp( m_szName, sz_stockname_bondindex, min(sizeof(m_szName),strlen(sz_stockname_bondindex)) )
		|| 0 == strncmp( m_szName, sz_stockname_bondindexcorp, min(sizeof(m_szName),strlen(sz_stockname_bondindexcorp)) ) );
}

BOOL CStockInfo::IsFund( )
{
	return ( 0 == strncmp( m_szName, sz_stockname_fund, min(sizeof(m_szName),strlen(sz_stockname_fund)) ) );
}

BOOL CStockInfo::IsRight( )
{
	return ( CStock::typeshRight == m_type || CStock::typeszRight == m_type );
}

int CStockInfo::DigitBit( )
{
	if( IsFund() || IsRight() || CStock::typeshB == m_type)
		return 3;
	return 2;
}

BOOL CStockInfo::GetAverage( double *pValue )
{
	if( CStock::typeshIndex == m_type || CStock::typeszIndex == m_type )
		return FALSE;
	if( m_fVolume > 1e-4 )
	{
		int		nCount	=	0;
		double	average	=	((double)(m_fAmount)) / m_fVolume;
		while( average < m_fLow && nCount < 10 )	{	average	*=	10;	nCount ++;	}
		while( average > m_fHigh && nCount < 20 )	{	average	/=	10;	nCount ++; }
		if( average < m_fLow )		//	说明是指数
			average	=	(m_fOpen+m_fHigh+m_fLow+m_fClose)/4;
		if( pValue )
			*pValue	=	average;
		return TRUE;
	}
	return FALSE;
}

BOOL CStockInfo::GetDiff( double *pValue, DWORD dateCur, int nDays )
{
	if( 1 == nDays && m_datetech == dateCur )
	{
		if( m_fLast <= 1e-4 || m_fClose < 1e-4 )
			return FALSE;
		if( pValue )
			*pValue	=	m_fClose - m_fLast;
		return TRUE;
	}
	return m_kdata.GetDiff( pValue, dateCur, nDays );
}

BOOL CStockInfo::GetDiffPercent( double *pValue, DWORD dateCur, int nDays )
{
	if( 1 == nDays && m_datetech == dateCur )
	{
		if( m_fLast <= 1e-4 || m_fClose < 1e-4 )
			return FALSE;
		if( pValue )
			*pValue	=	100. * m_fClose / m_fLast - 100;
		return TRUE;
	}
	return m_kdata.GetDiffPercent( pValue, dateCur, nDays );
}

BOOL CStockInfo::GetScope( double *pValue, DWORD dateCur, int nDays )
{
	if( 1 == nDays && m_datetech == dateCur )
	{
		if( m_fLast <= 1e-4 || m_fHigh <= 1e-4 || m_fLow <= 1e-4 )
			return FALSE;
		if( pValue )
			*pValue	=	100. * (m_fHigh-m_fLow)/m_fLast;
		return TRUE;
	}
	return m_kdata.GetScope( pValue, dateCur, nDays );
}

BOOL CStockInfo::GetDiffPercentMin5( double *pValue )
{
	return m_minute.GetDiffPercentMin5( pValue );
}

BOOL CStockInfo::GetPE( double * pValue )
{
	double	factor	=	CStock::GetReportFactor( m_reporttype );
	if( fabs(m_fEps) > 1e-4 && m_fClose > 1e-4 )
	{
		double	close_price_rmb	=	m_fClose;
		if( CStock::typeshB == m_type )
			close_price_rmb	=	/*m_fErate_dollar * */m_fClose;	// 备注,从通达信得到的基本资料数据是经过汇率换算的.
		else if( CStock::typeszB == m_type )
			close_price_rmb	=	/*m_fErate_hkdollar * */m_fClose;
		if( pValue )
			*pValue	=	close_price_rmb / (m_fEps * factor);
		return TRUE;
	}
	return FALSE;
}

BOOL CStockInfo::GetPNetAsset( double *pValue )
{
	if( fabs(m_fNetasset_ps) > 1e-4 && m_fClose > 1e-4 )
	{
		double	close_price_rmb	=	m_fClose;
		if( CStock::typeshB == m_type )
			close_price_rmb	=	/*m_fErate_dollar * */m_fClose;	// 备注,从通达信得到的基本资料数据是经过汇率换算的.
		else if( CStock::typeszB == m_type )
			close_price_rmb	=	/*m_fErate_hkdollar * */m_fClose;

		if( pValue )
			*pValue	=	close_price_rmb / m_fNetasset_ps;
		return TRUE;
	}
	return FALSE;
}

BOOL CStockInfo::GetPMainIncome( double * pValue )
{
	double	factor	=	CStock::GetReportFactor( m_reporttype );
	
	if( fabs(m_fMain_income) > 1e-4 && m_fClose > 1e-4 )
	{
		double	close_price_rmb	=	m_fClose;
		if( CStock::typeshB == m_type )
			close_price_rmb	=	/*m_fErate_dollar * */m_fClose;	// 备注,从通达信得到的基本资料数据是经过汇率换算的.
		else if( CStock::typeszB == m_type )
			close_price_rmb	=	/*m_fErate_hkdollar * */m_fClose;

		if( pValue )
			*pValue	=	close_price_rmb * m_fShare_count_total / (m_fMain_income*factor);
		return TRUE;
	}
	return FALSE;
}

BOOL CStockInfo::GetRatioPCash( double * pValue )
{
	if( CStock::typeshA != m_type && CStock::typeszA != m_type
		&& CStock::typeshB != m_type && CStock::typeszB != m_type )
		return FALSE;

	double	factor	=	CStock::GetReportFactor( m_reporttype );
	
	if( fabs(m_fCash_ps) > 1e-4 && m_fClose > 1e-4 )
	{
		double	close_price_rmb	=	m_fClose;
		if( CStock::typeshB == m_type )
			close_price_rmb	=	/*m_fErate_dollar * */m_fClose;	// 备注,从通达信得到的基本资料数据是经过汇率换算的.
		else if( CStock::typeszB == m_type )
			close_price_rmb	=	/*m_fErate_hkdollar * */m_fClose;

		if( pValue )
			*pValue	=	close_price_rmb / (m_fCash_ps*factor);
		return TRUE;
	}
	return FALSE;
}

BOOL CStockInfo::GetShareCurrency( double * pValue )
{
	if( CStock::typeshA != m_type && CStock::typeszA != m_type
		&& CStock::typeshB != m_type && CStock::typeszB != m_type )
		return FALSE;

	if( CStock::typeshA == m_type || CStock::typeszA == m_type )
	{
		*pValue	=	(double)m_fShare_count_a;
		return TRUE;
	}
	if( CStock::typeshB == m_type || CStock::typeszB == m_type )
	{
		*pValue	=	(double)m_fShare_count_b;
		return TRUE;
	}
	return FALSE;
}

BOOL CStockInfo::GetRatioCurrency( double * pValue )
{
	if( CStock::typeshA != m_type && CStock::typeszA != m_type
		&& CStock::typeshB != m_type && CStock::typeszB != m_type )
		return FALSE;

	if( m_fShare_count_total > 1e-4 )
	{
		if( CStock::typeshA == m_type || CStock::typeszA == m_type )
		{
			*pValue	=	(100. * (double)m_fShare_count_a) / m_fShare_count_total;
			return TRUE;
		}
		if( CStock::typeshB == m_type || CStock::typeszB == m_type )
		{
			*pValue	=	(100. * (double)m_fShare_count_b) / m_fShare_count_total;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CStockInfo::GetRatioChangeHand( double * pValue, double dVolume )
{
	if( CStock::typeshA != m_type && CStock::typeszA != m_type
		&& CStock::typeshB != m_type && CStock::typeszB != m_type )
		return FALSE;

	// get share currency
	double	dShareCurrency	=	0.;
	if( !GetShareCurrency( &dShareCurrency ) )
		return FALSE;
	if( fabs(dShareCurrency) < 1e-4 )
		return FALSE;

	if( pValue )
		*pValue	=	100. * dVolume / dShareCurrency;
	return TRUE;
}

BOOL CStockInfo::GetRatioChangeHand( CKData &kdata, double * pValue, DWORD dateCur, int nDays )
{
	double	dVolume	=	0;
	if( !kdata.GetVolumeSum( &dVolume, dateCur, nDays ) )
		return FALSE;

	return GetRatioChangeHand( pValue, dVolume );
}

BOOL CStockInfo::GetRatioChangeHand( double * pValue, DWORD dateCur, int nDays )
{
	return GetRatioChangeHand( m_kdata, pValue, dateCur, nDays );
}

BOOL CStockInfo::GetRatioVolume( double * pValue, DWORD dateCur, int nDays )
{
	return m_kdata.GetRatioVolume( pValue, dateCur, nDays );
}

BOOL CStockInfo::GetRS( double * pValue, DWORD dateCur, int nDays )
{
	return m_kdata.GetRS( pValue, dateCur, nDays );
}

BOOL CStockInfo::GetSellBuyRatio( double *pdRatio, double *pdDiff )
{
	double	dBuyVolume	= m_fBuyVolume[0];
	dBuyVolume	+=	m_fBuyVolume[1];
	dBuyVolume	+=	m_fBuyVolume[2];
	dBuyVolume	+=	m_fBuyVolume[3];
	double	dSellVolume	= m_fSellVolume[0];
	dSellVolume	+=	m_fSellVolume[1];
	dSellVolume	+=	m_fSellVolume[2];
	dSellVolume	+=	m_fSellVolume[3];
	
	double	dRatio	=	0;
	if( dBuyVolume + dSellVolume > 1e-4 )
		dRatio	=	200*dBuyVolume/(dBuyVolume+dSellVolume)-100;
	double	dDiff		=	dBuyVolume - dSellVolume;
	if( pdRatio )
		*pdRatio	=	dRatio;
	if( pdDiff )
		*pdDiff		=	dDiff;
	return TRUE;
}

BOOL CStockInfo::GetMarketValue( double * pValue )
{
	if( CStock::typeshA != m_type && CStock::typeszA != m_type
		&& CStock::typeshB != m_type && CStock::typeszB != m_type )
		return FALSE;

	if( m_fClose > 1e-4 && m_fShare_count_total > 1e-4 )
	{
		if( pValue )
			*pValue	=	m_fClose * m_fShare_count_total;
		return TRUE;
	}
	return FALSE;
}

BOOL CStockInfo::GetMarketValueA( double * pValue )
{
	if( m_fClose > 1e-4 && m_fShare_count_a > 1e-4
		&& (CStock::typeshA == m_type || CStock::typeszA == m_type) )
	{
		if( pValue )
			*pValue	=	m_fClose * m_fShare_count_a;
		return TRUE;
	}
	return FALSE;
}

BOOL CStockInfo::GetMarketValueB( double * pValue )
{
	if( m_fClose > 1e-4 && m_fShare_count_b > 1e-4
		&& (CStock::typeshB == m_type || CStock::typeszB == m_type) )
	{
		if( pValue )
			*pValue	=	m_fClose * m_fShare_count_b;
		return TRUE;
	}
	return FALSE;
}

BOOL CStockInfo::GetXiapu( double * pValue )
{
	if( fabs(STKLIB_DATA_INVALID - m_fYield_average) > 1e-4
		&& fabs(STKLIB_DATA_INVALID - m_fYield_stddev) > 1e-4 && fabs(m_fYield_stddev) > 1e-4 )
	{
		if( pValue )
			*pValue	=	100. * (m_fYield_average-3) / m_fYield_stddev;
		//                                       ^^^^ 市场无风险利率3%
		return TRUE;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	class	CStock

double CStock::GetReportFactor( int nReportType )
{
	double	factor	=	1;
	if( CStock::reportQuarter == nReportType )
		factor	=	4;
	else if( CStock::reportMid == nReportType )
		factor	=	2;
	else if( CStock::reportQuarter3 == nReportType )
		factor	=	1.333333;
	return factor;
}

CStock::CStock( )
{
	m_nBaseTextLen	=	0;
	m_pBaseText		=	NULL;

	m_pDatabase	=	NULL;

	SetKDataTypes( );
}

CStock::CStock( CStDatabase * pDatabase )
{
	m_nBaseTextLen	=	0;
	m_pBaseText		=	NULL;

	SP_ASSERT( pDatabase );
	m_pDatabase	=	pDatabase;

	SetKDataTypes( );
}

CStock::CStock( CStDatabase * pDatabase, DWORD dwMarket, const char * code )
{
	m_nBaseTextLen	=	0;
	m_pBaseText		=	NULL;

	SP_ASSERT( pDatabase );
	m_pDatabase	=	pDatabase;
	m_stockinfo.SetStockCode( dwMarket, code );

	SetKDataTypes( );
}

CStock::~CStock()
{
	Clear( );
}

void CStock::SetKDataTypes( )
{
	m_kdMonth.SetKType( CKData::ktypeMonth );
	m_kdWeek.SetKType( CKData::ktypeWeek );
	m_kdDay.SetKType( CKData::ktypeDay );
	m_kdMin60.SetKType( CKData::ktypeMin60 );
	m_kdMin30.SetKType( CKData::ktypeMin30 );
	m_kdMin15.SetKType( CKData::ktypeMin15 );
	m_kdMin5.SetKType( CKData::ktypeMin5 );
}

void CStock::SetDatabase( CStDatabase * pDatabase )
{
	m_pDatabase	=	pDatabase;
}

CStDatabase * CStock::GetDatabase( )
{
	return m_pDatabase;
}

void CStock::SetStockCode( DWORD dwMarket, const char *szCode )
{
	if( m_stockinfo.IsEqualTo(dwMarket, szCode) )
		return;

	Clear();
	m_stockinfo.SetStockCode(dwMarket,szCode);
}

const char * CStock::GetStockCode( )
{
	return m_stockinfo.GetStockCode();
}

BOOL CStock::SetStockInfo( CStockInfo * pInfo )
{
	if( NULL == pInfo )
	{
		Clear();
		return FALSE;
	}
	if( m_stockinfo.IsEqualTo( pInfo->GetMarket(), pInfo->GetStockCode() ) )
	{
		m_stockinfo	=	*pInfo;
		return m_stockinfo.IsValidStock( );
	}
	Clear();
	m_stockinfo	=	*pInfo;
	return m_stockinfo.IsValidStock( );
}

CStockInfo & CStock::GetStockInfo( )	{	return m_stockinfo;	}
CKData	& CStock::GetKData( int ktype )
{
	switch( ktype )
	{
	case CKData::ktypeMonth:
		return GetKDataMonth();
	case CKData::ktypeWeek:
		return GetKDataWeek();
	case CKData::ktypeDay:
		return GetKDataDay();
	case CKData::ktypeMin60:
		return GetKDataMin60();
	case CKData::ktypeMin30:
		return GetKDataMin30();
	case CKData::ktypeMin15:
		return GetKDataMin15();
	case CKData::ktypeMin5:
		return GetKDataMin5();
	default:
		SP_ASSERT( FALSE );
		return GetKDataDay();
	}
}
CKData	& CStock::GetKDataMonth( )		{	return m_kdMonth;	}
CKData	& CStock::GetKDataWeek( )		{	return m_kdWeek;	}
CKData	& CStock::GetKDataDay( )		{	return m_kdDay;		}
CKData	& CStock::GetKDataMin60( )		{	return m_kdMin60;	}
CKData	& CStock::GetKDataMin30( )		{	return m_kdMin30;	}
CKData	& CStock::GetKDataMin15( )		{	return m_kdMin15;	}
CKData	& CStock::GetKDataMin5( )		{	return m_kdMin5;	}
CDRData	& CStock::GetDRData( )			{	return m_drData;	}
CReport & CStock::GetReport( )			{	return m_report;	}
CMinute & CStock::GetMinute( )			{	return m_minute;	}
COutline & CStock::GetOutline( )		{	return m_outline;	}

char	* CStock::GetBaseTextPtr( )		{	return m_pBaseText;	}
int		CStock::GetBaseTextLength( )	{	return m_nBaseTextLen;	}

BOOL	CStock::AllocBaseTextMem( int nNewLength )
{
	SP_ASSERT( nNewLength <= 0x200000 );
	if( nNewLength > 0x200000 )	return FALSE;

	if( m_pBaseText )
	{
		delete	m_pBaseText;
		m_nBaseTextLen	=	0;
	}

	m_pBaseText	=	new CHAR[nNewLength+1];
	if( m_pBaseText )
	{
		memset( m_pBaseText, 0, nNewLength+1 );
		m_nBaseTextLen	=	nNewLength;
		return TRUE;
	}
	return FALSE;
}

int CStock::PrepareData( int type, int period, BOOL bReload )
{
	SP_ASSERT( m_pDatabase );
	if( ! m_pDatabase )	return 0;

	switch( type )
	{
	case dataInfo:
		return AfxGetStockContainer().GetStockInfo( m_stockinfo.GetStockCode(), &m_stockinfo );
	case dataK:
		{
			switch( period )
			{
			case CKData::ktypeMonth:
				if( bReload || GetKDataMonth().GetSize() == 0 )
					m_pDatabase->LoadKData( this, CKData::ktypeMonth );
				break;
			case CKData::ktypeWeek:
				if( bReload || GetKDataWeek().GetSize() == 0 )
					m_pDatabase->LoadKData( this, CKData::ktypeWeek );
				break;
			case CKData::ktypeDay:
				if( bReload || GetKDataDay().GetSize() == 0 )
					m_pDatabase->LoadKData( this, CKData::ktypeDay );
				break;		// month, week, and day willnot extract from min60,min30...

			case CKData::ktypeMin60:
				if( bReload || GetKDataMin60().GetSize() == 0 )
					m_pDatabase->LoadKData( this, CKData::ktypeMin60 );
				break;
			case CKData::ktypeMin30:
				if( bReload || GetKDataMin30().GetSize() == 0 )
					m_pDatabase->LoadKData( this, CKData::ktypeMin30 );
				break;
			case CKData::ktypeMin15:
				if( bReload || GetKDataMin15().GetSize() == 0 )
					m_pDatabase->LoadKData( this, CKData::ktypeMin15 );
				break;
			case CKData::ktypeMin5:
				if( bReload || GetKDataMin5().GetSize() == 0 )
					m_pDatabase->LoadKData( this, CKData::ktypeMin5 );
				break;
			default:
				SP_ASSERT( FALSE );
				return 0;
			}
			return ExtractKData( period, FALSE );
		}
	case dataDR:
		if( bReload || GetDRData().GetSize() == 0 )
			return m_pDatabase->LoadDRData( this );
		else
			return GetDRData().GetSize();
	case dataBasetable:
		return AfxGetStockContainer().GetStockInfo( m_stockinfo.GetStockCode(), &m_stockinfo );
	case dataBasetext:
		if( bReload || !GetBaseTextPtr() )
			return m_pDatabase->LoadBaseText( this );
		else
			return GetBaseTextLength();
	case dataReport:
		if( bReload || GetReport().GetSize() == 0 )
			return m_pDatabase->LoadReport( this );
		else
			return GetReport().GetSize();
	case dataMinute:
		if( bReload || GetMinute().GetSize() == 0 )
			return m_pDatabase->LoadMinute( this );
		else
			return GetMinute().GetSize();
	case dataOutline:
		if( bReload || GetOutline().GetSize() == 0 )
			return m_pDatabase->LoadOutline( this );
		else
			return GetOutline().GetSize();
	default:
		SP_ASSERT( FALSE );
		return 0;
	}
}

int CStock::PrepareBaseInfo( BOOL bReload )
{
	// Load From File
	PrepareData( CStock::dataBasetext, 0, bReload );
	
	// Save To pText
	int nTextLen = m_nBaseTextLen;
	char * pText	=	NULL;
	if( m_pBaseText && nTextLen > 0 )
	{
		pText = new CHAR[nTextLen+1];
		if( pText )	memcpy( pText, m_pBaseText, nTextLen );
		if( pText )	pText[nTextLen] = 0;
	}
	
	int nIntroLen = SLH_MAX * 80;
	if( !AllocBaseTextMem( nIntroLen + m_nBaseTextLen + 10 ) )
		return 0;

	int nCount = 0;
	
#ifdef	CLKLAN_ENGLISH_US
	{ // Chinese pronunciation
		CSPString strTemp = m_stockinfo.GetStockNameChs();
		strTemp = AfxMakeSpellCode( strTemp );
		strTemp = "Chinese Name(pronunciation): \t" + strTemp + STRING_CRLF;
		if( nCount + strTemp.GetLength() < nIntroLen )
		{
			nCount += strTemp.GetLength();
			strcat( m_pBaseText, (LPCTSTR)strTemp );
		}
	}
#endif

	for( int nVariantID = SLH_MIN; nVariantID <= SLH_MAX; nVariantID ++ )
	{
		CSPString	strTemp	=	AfxGetVariantName( nVariantID, TRUE );
		strTemp	+=	": \t";
		strTemp	+=	(LPCTSTR)AfxGetVariantDispString( nVariantID, m_stockinfo, NULL );
		strTemp	+=	STRING_CRLF;

		SP_ASSERT( nCount + strTemp.GetLength() < nIntroLen );
		if( nCount + strTemp.GetLength() < nIntroLen )
		{
			nCount += strTemp.GetLength();
			strcat( m_pBaseText, (LPCTSTR)strTemp );
		}
	}
	
	nCount += strlen(STRING_CRLF);
	strcat( m_pBaseText, STRING_CRLF );
	nCount += strlen(STRING_CRLF);
	strcat( m_pBaseText, STRING_CRLF );
	
#ifdef	CLKLAN_ENGLISH_US
	if( pText && nCount + nTextLen < m_nBaseTextLen && AfxIsEnglish( pText, nTextLen, 0.01f ) )
#else
	if( pText && nCount + nTextLen < m_nBaseTextLen )
#endif
	{
		nCount += nTextLen;
		strcat( m_pBaseText, pText );
	}
	
	if( pText )
		delete [] pText;

	return nCount;
}

int CStock::ExtractKData( int period, BOOL bForced )
{
	switch( period )
	{
	case CKData::ktypeMonth:
		{
			if( bForced )
				return CKData::DayToMonth( GetKDataDay(), GetKDataMonth() );
			int compare = GetKDataMonth().CompareLatestDate( GetKDataDay() );
			if( -1 == compare )
				return CKData::DayToMonth( GetKDataDay(), GetKDataMonth() );
			else if( 1 == compare )
				return GetKDataMonth().GetSize();
			else if( GetKDataMonth().GetSize() * 18 < GetKDataDay().GetSize() )
				return CKData::DayToMonth( GetKDataDay(), GetKDataMonth() );
			else
				return GetKDataMonth().GetSize();
		}
		break;
	case CKData::ktypeWeek:
		{
			if( bForced )
				return CKData::DayToWeek( GetKDataDay(), GetKDataWeek() );
			int compare = GetKDataWeek().CompareLatestDate( GetKDataDay() );
			if( -1 == compare )
				return CKData::DayToWeek( GetKDataDay(), GetKDataWeek() );
			else if( 1 == compare )
				return GetKDataWeek().GetSize();
			else if( GetKDataWeek().GetSize() * 5 < GetKDataDay().GetSize() )
				return CKData::DayToWeek( GetKDataDay(), GetKDataWeek() );
			else
				return GetKDataWeek().GetSize();
		}
		break;
	case CKData::ktypeDay:
		return GetKDataDay().GetSize();
	case CKData::ktypeMin60:
		{
			if( bForced )
				return CKData::Min5ToMin60( GetKDataMin5(), GetKDataMin60() );
			int compare = GetKDataMin60().CompareLatestDate( GetKDataMin5() );
			if( -1 == compare )
				return CKData::Min5ToMin60( GetKDataMin5(), GetKDataMin60() );
			else if( 1 == compare )
				return GetKDataMin60().GetSize();
			else if( GetKDataMin60().GetSize() * 12 < GetKDataMin5().GetSize() )
				return CKData::Min5ToMin60( GetKDataMin5(), GetKDataMin60() );
			else
				return GetKDataMin60().GetSize();
		}
		break;
	case CKData::ktypeMin30:
		{
			if( bForced )
				return CKData::Min5ToMin30( GetKDataMin5(), GetKDataMin30() );
			int compare = GetKDataMin30().CompareLatestDate( GetKDataMin5() );
			if( -1 == compare )
				return CKData::Min5ToMin30( GetKDataMin5(), GetKDataMin30() );
			else if( 1 == compare )
				return GetKDataMin30().GetSize();
			else if( GetKDataMin30().GetSize() * 6 < GetKDataMin5().GetSize() )
				return CKData::Min5ToMin30( GetKDataMin5(), GetKDataMin30() );
			else
				return GetKDataMin30().GetSize();
		}
		break;
	case CKData::ktypeMin15:
		{
			if( bForced )
				return CKData::Min5ToMin15( GetKDataMin5(), GetKDataMin15() );
			int compare = GetKDataMin15().CompareLatestDate( GetKDataMin5() );
			if( -1 == compare )
				return CKData::Min5ToMin15( GetKDataMin5(), GetKDataMin15() );
			else if( 1 == compare )
				return GetKDataMin15().GetSize();
			else if( GetKDataMin15().GetSize() * 3 < GetKDataMin5().GetSize() )
				return CKData::Min5ToMin15( GetKDataMin5(), GetKDataMin15() );
			else
				return GetKDataMin15().GetSize();
		}
		break;
	case CKData::ktypeMin5:
		return GetKDataMin5().GetSize();
	default:
		SP_ASSERT( FALSE );
		return 0;
	}
}

int CStock::MergeBaseText( CStock &stock )
{
	if( GetBaseTextLength() > 0 )
		return GetBaseTextLength();
	if( stock.GetBaseTextLength() > 0 )
	{
		if( AllocBaseTextMem( stock.GetBaseTextLength() ) )
			strncpy( GetBaseTextPtr(), stock.GetBaseTextPtr(), stock.GetBaseTextLength() );
		return stock.GetBaseTextLength();
	}
	return 0;
}

int CStock::MergeKData( CStock &stock, int period )
{
	CKData	* pkdata	=	NULL;
	CKData	* pkdata2	=	NULL;
	switch( period )
	{
	case CKData::ktypeMonth:
		pkdata	=	&(GetKDataMonth());
		pkdata2	=	&(stock.GetKDataMonth());
		break;
	case CKData::ktypeWeek:
		pkdata	=	&(GetKDataWeek());
		pkdata2	=	&(stock.GetKDataWeek());
		break;
	case CKData::ktypeDay:
		pkdata	=	&(GetKDataDay());
		pkdata2	=	&(stock.GetKDataDay());
		break;
	case CKData::ktypeMin60:
		pkdata	=	&(GetKDataMin60());
		pkdata2	=	&(stock.GetKDataMin60());
		break;
	case CKData::ktypeMin30:
		pkdata	=	&(GetKDataMin30());
		pkdata2	=	&(stock.GetKDataMin30());
		break;
	case CKData::ktypeMin15:
		pkdata	=	&(GetKDataMin15());
		pkdata2	=	&(stock.GetKDataMin15());
		break;
	case CKData::ktypeMin5:
		pkdata	=	&(GetKDataMin5());
		pkdata2	=	&(stock.GetKDataMin5());
		break;
	default:
		SP_ASSERT( FALSE );
		return 0;
	}

	return pkdata->MergeKData( pkdata2 );
}

int CStock::MergeDRData( CStock &stock )
{
	if( GetDRData().GetSize() < stock.GetDRData().GetSize() )
		GetDRData().Copy( stock.GetDRData() );
	return GetDRData().GetSize();
}

int CStock::StoreDRData( )
{
	if( m_pDatabase )
		return m_pDatabase->StoreDRData( this );
	return 0;
}

void CStock::Clear( )
{
	m_stockinfo.Clear( );
	if( m_pBaseText )
		delete [] (CHAR*)m_pBaseText;
	m_nBaseTextLen	=	0;
	m_pBaseText		=	NULL;

	m_kdMonth.Clear();
	m_kdWeek.Clear();
	m_kdDay.Clear();
	m_kdMin60.Clear();
	m_kdMin30.Clear();
	m_kdMin15.Clear();
	m_kdMin5.Clear();
	m_drData.SetSize( 0 );
	m_report.SetSize( 0 );
	m_minute.SetSize( 0 );
	m_outline.SetSize( 0 );

	SetKDataTypes( );
}

// 投资家万能引擎证券市场
/*
#define  SH_MARKET_EX   'HS'  // 上海证券
#define  SZ_MARKET_EX   'ZS'  // 深圳证券
#define  CZ_MARKET_EX   'ZC'  // 郑州商品
#define  DL_MARKET_EX   'LD'  // 大连商品
#define  SF_MARKET_EX   'FS'  // 上海商品
#define  SM_MARKET_EX   'MS'  // 上海金属
#define  CE_MARKET_EX   'EC'  // 亚洲指数
#define  CB_MARKET_EX   'BC'  // ＣＢＯＴ
#define  CC_MARKET_EX   'CC'  // 软商品
#define  MC_MARKET_EX   'CM'  // Ｃ Ｍ Ｘ
#define  IM_MARKET_EX   'MI'  // 外汇期货
#define  LM_MARKET_EX   'ML'  // Ｌ Ｍ Ｅ
#define  NY_MARKET_EX   'YN'  // ＮＹＳＥ
#define  AI_MARKET_EX   'IA'  // 亚洲期指
#define  WH_MARKET_EX   'HW'  // 外汇交易
#define  XH_MARKET_EX   'HX'  // 欧美指数
#define  CM_MARKET_EX   'MC'  // Ｃ Ｍ Ｅ
#define  NF_MARKET_EX   'FN'  // 欧美期指
#define  NM_MARKET_EX   'MN'  // 石油产品
#define  LC_MARKET_EX   'CL'  // 巴黎油籽
#define  LI_MARKET_EX   'IL'  // 马来西亚
#define  SI_MARKET_EX   'IS'  // 温尼伯格
#define  HS_MARKET_EX   'SH'  // Ｈ Ｓ Ｉ
#define  HK_MARKET_EX   'HK'  // 香港股票
#define  ZJ_MARKET_EX   'JZ'  // ＺＪＦＥ
*/
