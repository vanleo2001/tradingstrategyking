/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CStockInfoArray;
		class	CStockContainer;
		class	CDomain;
		class	CDomainContainer;
*/

#if	!defined( __STKLIB_CONTAINER_H__ )
#define	__STKLIB_CONTAINER_H__

#define	ID_STOCKCNTN_AVERAGE		(-1)
#define	ID_STOCKCNTN_WEIGHTAVERAGE	(-2)

typedef	struct	variant_savevalue_t	{
	UINT	nVariantID;
	double	Value;
	BOOL	bNoValue;
}VARIANT_SAVEVALUE;

// 使用下句，将视包含Container.h的exe应用程序输出函数，其他相似语句类似
// template class __declspec(dllexport)	CSPArray<CStockInfo, CStockInfo &>;
typedef	CSPArray<CStockInfo, CStockInfo &>	CStockInfoArray;

class	CStrategy;
#include "../Src/SPLock.h"

/***
	股票信息数组类。
	有一个股票信息数组的全局对象AfxGetStockContainer()保存所有股票的信息
*/

class STKLIB_API CStockContainer : public CStockInfoArray
{
public:
	CStockContainer();
	virtual	~CStockContainer();

	BOOL Lock();
	BOOL UnLock();

	static int SortFunction(const void *s1,const void *s2);
	static int StockInfoCompareFunc( const void *s1,const void *s2, CStockContainer * pContainer, int nSortVariantID );

	// for access CStockInfoArray
	virtual	int	Add( CStockInfo &newElement );
	virtual	void SetSize(int nNewSize, int nGrowBy = -1)	{	CStockInfoArray::SetSize(nNewSize,nGrowBy);	}

	// attributes
	enum StockTypes {
		typeNone		=	0x00,
		typeMin			=	0x01,
		typeIndex		=	0x01,
		typeStrategy	=	0x02,
		typeGroup		=	0x03,
		typeDomain		=	0x04,
		typeAll			=	0x05,
		typeA			=	0x06,
		typeBond		=	0x07,
		typeFund		=	0x08,
		typeClassShaa	=	0x09,
		typeClassShab	=	0x0A,
		typeClassSzna	=	0x0B,
		typeClassSznb	=	0x0C,
		typeClassShabond=	0x0D,
		typeClassSznbond=	0x0E,
		typeClassMsmall	=	0x0F,
		typeRight		=	0x10,
		typeMax			=	0x10
	};

	// operations
	BOOL	GetCurrentType( int *pType, CSPString * pDomain, DWORD *pdwDate );
	BOOL	GetPioneerTechDate( DWORD * pdwDate );
	BOOL	GetLatestTechDate( DWORD * pdwDate );
	BOOL	GetTechDateArray( CSPDWordArray & dwArray );
	BOOL	RetrieveFromStatic( int nType, LPCTSTR lpszDomain, CStrategy * pStrategy, DWORD dwDate );
	BOOL	ReRetrieveFromStatic( CStrategy * pStrategy, BOOL bUpToDate = FALSE );
	BOOL	RetrieveSpecify( CSPStringArray & astr );

	BOOL	GetStockInfo( const char * szCode, CStockInfo * pInfo, int * pid = NULL );
	BOOL	SetCurrentStock( int nStockIndex );
	BOOL	SetCurrentStock( const char * szCode );
	BOOL	GetCurrentStock( CStockInfo * pInfo );
	BOOL	GetPrevStock( CStockInfo * pInfo );
	BOOL	GetNextStock( CStockInfo * pInfo );

	CStockInfo & GetStockInfoByID( int nID );
	CStockInfo & GetStockInfoByIDSort( int nID );
	CStockInfo & GetAverage( );
	CStockInfo & GetWeightAverage( );
	
	BOOL	Load( CStDatabase * pDB, PROGRESS_CALLBACK fnCallback = NULL, void *cookie = NULL, int nProgStart = 0, int nProgEnd = STKLIB_MAX_PROGRESS );
	void	OnDataChanged( );
	BOOL	ReloadBase( CStDatabase * pDB );
	BOOL	SetMap( );
	BOOL	SetAverage( PROGRESS_CALLBACK fnCallback = NULL, void *cookie = NULL, int nProgStart = 0, int nProgEnd = STKLIB_MAX_PROGRESS );
	BOOL	Clear( );
	void	CopyData( CStockContainer & src );

	static	CStockContainer * m_pSortContainer;
	UINT	m_nSortVariantID;
	BOOL	m_bSortAscend;
	
	BOOL	Sort( int nSortVariantID, BOOL bAscend );
	void	SetSortID( CSPDWordArray & auidsort );
	BOOL	GetMultiSortIDArray( CSPDWordArray & adwSortID, LONG lStockType, UINT nSLH, BOOL bAsc, int nCount );

	BOOL	GetVariantSaveValue( double *pValue, UINT nVariantID, CStockInfo &info, BOOL *pNoValue );
	void	SetVariantSaveValue( double Value, UINT nVariantID, CStockInfo &info, BOOL bNoValue );
	void	ClearVariantSaveValue( );

protected:
	int			m_nType;
	DWORD		m_dwDate;
	CSPString	m_strDomain;

	int			m_nCurrentStock;
	CSPString	m_strCurrentStockCode;	// 当m_nCurrentStock==-1时，即当前股票不在Container数组之内，则记录其StockCode

	CStockInfo	m_infoAverage;
	CStockInfo	m_infoWeightAverage;
	CStockInfo	m_infoNull;

	CSPMapStringToPtr	m_map;
	CSPPtrArray			m_aptrSaveValueArray;
	CSPDWordArray		m_auidSort;

	CSPMutex	m_mutex;
};

/***
	板块类，包含板块名称、板块中的股票代码
*/
class STKLIB_API CDomain : public CSPStringArray
{
public:
	CDomain( );
	CDomain( const CDomain &src );
	virtual	~CDomain( );

	CSPString	m_strName;

	BOOL	AddStock( LPCTSTR lpszStockCode );
	BOOL	AddStockSort( LPCTSTR lpszStockCode );
	BOOL	RemoveStock( LPCTSTR lpszStockCode );
	CDomain	& operator = ( const CDomain &src );

	int		AddFxjDomain( LPCTSTR lpszFile );
};

//template class __declspec(dllexport)	CSPArray<CDomain, CDomain &>;
typedef	CSPArray<CDomain, CDomain &>	CDomainArray;
/***
	板块数组类
*/
class STKLIB_API CDomainContainer : public CDomainArray
{
public:
	CDomainContainer( );
	CDomainContainer( CDomainContainer & src );
	virtual ~CDomainContainer( );

	CDomainContainer & operator = ( CDomainContainer & src );

	int		AddDomain( LPCTSTR lpszDomainName );
	int		AddDomain( CDomain & newdomain );
	int		AddDomainReplace( CDomain & newdomain );
	int		AddDomainReplace( CDomainContainer & newdomains );
	int		RemoveDomain( LPCTSTR lpszDomainName );
	BOOL	AddDomainStock( LPCTSTR lpszDomainName, LPCTSTR lpszStockCode );
	BOOL	AddDomainStock( LPCTSTR lpszDomainName, CSPStringArray & astr );
	BOOL	RemoveDomainStock( LPCTSTR lpszDomainName, LPCTSTR lpszStockCode );
	BOOL	RemoveDomainStockAll( LPCTSTR lpszDomainName );
	BOOL	GetDomainStocks( LPCTSTR lpszDomain, CSPStringArray &astr );
	BOOL	GetDomains( CSPStringArray & astr );
	BOOL	GetAllDomainInfo( CStockContainer * pContainer, DWORD dwDate );

	BOOL	Load( LPCTSTR lpszFileName );
	BOOL	Store( LPCTSTR lpszFileName );

protected:

};

#endif // __STKLIB_CONTAINER_H__
