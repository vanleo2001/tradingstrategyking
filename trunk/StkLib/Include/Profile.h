/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CTechParameters;
		class	CColorRefContainer;
		class	CColorClass;
		class	CColorClassContainer;
		class	CStProfile;
*/

#if	!defined( __STKLIB_PROFILE_H__ )
#define	__STKLIB_PROFILE_H__

/////////////////////////////////////////////////////////////////////////////
// CTechParameters
/***
	技术指标参数，包含所有技术指标的参数设定
*/
class STKLIB_API CTechParameters
{
public:
	CTechParameters( );

	void	Clear( );
	void	SetKData( CKData * pKData );
	void	SetDefaultParametersAll( );
	BOOL	FindParameters( UINT nTech, CTechnique * pTech );
	void	Serialize( CSPArchive &ar );

	CMA		ma;
	CBBI	bbi;
	CBOLL	boll;
	CPV		pv;
	CSAR	sar;
	CDJ		dj;
	CCW		cw;

	CMACD	macd;
	CMIKE	mike;
	CPSY	psy;

	CVOLUME	volume;
	CNVI	nvi;
	CPVI	pvi;
	CVR		vr;
	CVROC	vroc;
	COBV	obv;
	CMOBV	mobv;
	CMFI	mfi;
	CVMACD	vmacd;
	CWVAD	wvad;
	CEMV	emv;
	CVRSI	vrsi;
	CNVRSI	nvrsi;
	CAD		ad;
	CCI		ci;

	CKDJ	kdj;
	CR		r;
	CRSI	rsi;
	CBIAS	bias;
	CMTM	mtm;
	CDMI	dmi;
	CROC	roc;
	CCCI	cci;
	CCV		cv;
	CARBR	arbr;
	CCR		cr;
	COSC	osc;
	CUOS	uos;
	CMAOSC	maosc;
	C36BIAS	bias36;
	CDPO	dpo;
	CKST	kst;

	CREI	rei;
	CDMKI	dmki;
	CPCNT	pcnt;
	CHLC	hlc;
	CCDP	cdp;
	CASI	asi;
	CATR	atr;

	CCYO	cyo;
	CDCYO	dcyo;
	CHSL	hsl;
	CDPER	dper;
};

//template class __declspec(dllexport)	CSPArray<COLORREF, COLORREF>;
typedef	CSPArray<COLORREF, COLORREF>	CColorRefArray;

class STKLIB_API	CColorRefContainer : public CColorRefArray
{
public:
	CColorRefContainer( ) { };
	virtual ~CColorRefContainer( )	{	RemoveAll();	}

	virtual	void SetSize(int nNewSize, int nGrowBy = -1)	{	CColorRefArray::SetSize(nNewSize,nGrowBy);	}
	virtual	void Copy(const CColorRefArray& src)	{	CColorRefArray::Copy(src);	}
};

// 颜色定义
#define	NDCLR_BLACK			RGB(0,0,0)
#define	NDCLR_WHITE			RGB(255,255,255)
#define	NDCLR_GREYISH		RGB(215,212,206)
#define	NDCLR_WATER			RGB(206,212,215)
#define	NDCLR_DARKGREY		RGB(128,128,128)
#define	NDCLR_YELLOW		RGB(255,255,0)
#define	NDCLR_MINIYELLOW	RGB(247,243,233)
#define	NDCLR_RED			RGB(255,0,0)
#define	NDCLR_FLATRED		RGB(168,0,0)
#define	NDCLR_WATERRED		RGB(255,84,255)
#define	NDCLR_GREEN			RGB(0,255,0)
#define	NDCLR_GREENBLUE		RGB(0,127,127)
#define	NDCLR_BLUE			RGB(0,0,255)
#define	NDCLR_MINIBLUE		RGB(153,153,204)
#define	NDCLR_FLATBLUE		RGB(0,255,255)
#define	NDCLR_WATERBLUE		RGB(128,128,192)
#define	NDCLR_DEFAULT		RGB(255,255,255)
/***
	界面配色方案
*/
class STKLIB_API CColorClass
{
public:
	CColorClass( );
	CColorClass( const CColorClass &src );
	virtual	~CColorClass( );

	// operation
	CColorClass	& operator = ( const CColorClass &src );
	void	Serialize( CSPArchive & ar );

	// attributes
	enum ColorIndex {
		clrMin			=	0x00,
		clrDefault		=	0x00,
		clrReserved1	=	0x01,
		clrReserved2	=	0x02,
		clrReserved3	=	0x03,
		clrReserved4	=	0x04,
		clrReserved5	=	0x05,
		clrReserved6	=	0x06,
		clrWorkspBK		=	0x07,
		clrWorkspText	=	0x08,
		clrSimuBK		=	0x09,
		clrSimuText		=	0x0A,
		clrSListBK		=	0x0B,
		clrSListSelected=	0x0C,
		clrBaseBK		=	0x0D,
		clrBaseText		=	0x0E,
		clrGraphBK		=	0x0F,
		clrBorder		=	0x10,
		clrRise			=	0x11,
		clrFall			=	0x12,
		clrFallEntity	=	0x13,
		clrPlane		=	0x14,
		clrNewKLine		=	0x15,
		clrCW			=	0x16,
		clrDJ			=	0x17,
		clrTitle		=	0x18,
		clrText			=	0x19,
		clrLine1		=	0x1A,
		clrLine2		=	0x1B,
		clrLine3		=	0x1C,
		clrLine4		=	0x1D,
		clrLine5		=	0x1E,
		clrLine6		=	0x1F,
		clrReserved7	=	0x20,
		clrReserved8	=	0x21,
		clrReserved9	=	0x22,
		clrReserved10	=	0x23,
		clrReserved11	=	0x24,
		clrReserved12	=	0x25,
		clrMax			=	0x25,
	};

	void		SetName( LPCTSTR lpszName );
	CSPString	GetName( );
	void		SetColor( int nColorIndex, COLORREF clrRGB );
	COLORREF	GetColor( int nColorIndex );
	void		SetColorDefault( );
	void		SetColorStandard( );		// 传统式配色方案
	void		SetColorByzantinesque( );	// 拜占庭式配色方案
	void		SetColorGothic( );			// 哥特式配色方案
	void		SetColorBaroque( );			// 巴洛克式配色方案
	static	BOOL	IsSysColorClass( LPCTSTR lpszColorClassName );
	static	BOOL	IsColorStandard( LPCTSTR lpszColorClassName );
	static	BOOL	IsColorByzantinesque( LPCTSTR lpszColorClassName );
	static	BOOL	IsColorGothic( LPCTSTR lpszColorClassName );
	static	BOOL	IsColorBaroque( LPCTSTR lpszColorClassName );

protected:
	CSPString	m_strName;

	CColorRefContainer	m_aColorRef;
};

//template class __declspec(dllexport)	CSPArray<CColorClass, CColorClass &>;
typedef	CSPArray<CColorClass, CColorClass &>	CColorClassArray;
/***
	配色方案数组
*/
class STKLIB_API CColorClassContainer : public CColorClassArray
{
public:
	CColorClassContainer( );
	CColorClassContainer( CColorClassContainer & src );
	virtual ~CColorClassContainer( );

	// operation
	CColorClassContainer & operator = ( CColorClassContainer & src );
	void	Serialize( CSPArchive & ar );

	// attributes
	int			AddColorClass( CColorClass & newClass );
	BOOL		RemoveColorClass( LPCTSTR lpszColorClassName );
	BOOL		SetColorClass( CColorClass & clrcls );
	BOOL		GetColorClass( LPCTSTR lpszColorClassName, CColorClass & clrcls );
	COLORREF	GetColor( int nColorIndex );
	void		SetContainerDefault( );
	BOOL		SelectColorClass( LPCTSTR lpszColorClassName );
	int			GetSelected();

protected:
	int		m_nSelectedColorClass;
};

/***
	系统设置
*/
class STKLIB_API CStProfile
{
public:
	CStProfile();
	virtual ~CStProfile();

	void	StoreProfile( );
	void	LoadProfile( );

	BOOL	Store( LPCTSTR lpszFileName );
	BOOL	Load( LPCTSTR lpszFileName );

	virtual	void	FileSerialize( CSPArchive &ar );

	//Attributes

	//Operations
	// Server Infomation
	CSPString	GetMainServerDomain( );
	CSPString	GetHTTPServerDomain( );
	WORD		GetHTTPServerPort( );
	CSPString	GetMainServer( );
	CSPString	GetProductURL( );
	CSPString	GetDownloadURL( );
	CSPString	GetPurchaseURL( );
	CSPString	GetAgentRootURL( );
	CSPString	GetSupportMailto( );
	CSPString	GetUpgradeURL( );

	CSPString	GetEvaluateEmail( );
	CSPString	GetEvaluateSMTPServer( );
	CSPString	GetEvaluateSMTPUser( );
	CSPString	GetEvaluateSMTPPasswd( );
	
	CSPString	GetDefaultServer( );
	void		SetDefaultServer( LPCTSTR lpszDefaultServer );
	CSPStringArray	& GetDownloadServers( );		// Not include main server, include default server
	void			SetDownloadServers( CSPStringArray & astrAllServers );

	/* not use now, use CQSProfile's proxy settings
	int			GetAccessType( );
	void		SetAccessType( int nAccessType );
	int			GetProxyType( );
	void		SetProxyType( int nProxyType );
	CSPString	GetProxyAddress( );
	void		SetProxyAddress( LPCTSTR lpszProxyAddress );
	WORD		GetProxyPort( );
	void		SetProxyPort( WORD nPort );
	CSPString	GetProxyUser( );
	void		SetProxyUser( LPCTSTR lpszProxyUser );
	CSPString	GetProxyPasswd( );
	void		SetProxyPasswd( LPCTSTR lpszProxyUser );
	*/

	// Window Placement
	BOOL	GetWindowPlacement(LPWINDOWPLACEMENT pwp);
	void	SetWindowPlacement(LPWINDOWPLACEMENT pwp);

	// Work Directory
	CSPString		GetWorkDirectory( );

	// Version
	CSPString		GetVersion( );

	// Stock Data Format and Path
	CSPString		GetSViewFile( );
	CSPString		GetDomainFile( );
	CSPString		GetGroupFile( );
	CSPString		GetProjectPath( );
	CSPString		GetExportPath( );
	CSPString		GetYongxinPath( );
	CSPString		GetSelfDBPath( );
	CSPString		GetSelfDBNewsPath( DWORD dwMarket );
	CSPString		GetSelfDBBasePath( DWORD dwMarket );
	CSPString		GetQianlongPath( );
	void		SetQianlongPath( LPCTSTR lpszPath );

	// Color
	COLORREF	GetColor( int nColorIndex );
	CColorClassContainer & GetColorClassContainer( );

	// Last Opened Strategies
	CSPStringArray &	GetLastOpenedStrategy( );
	void				SetLastOpenedStrategy( CSPStringArray & astr );
	CSPString			GetLastActiveStrategy( );
	void				SetLastActiveStrategy( LPCTSTR lpszPath );

	// Net Info
	void	SetNetInfoShowLater( BOOL bShowLater );
	void	SetNetInfo( LPCTSTR lpszInfo, LPCTSTR lpszInfourl );
	void	GetNetInfo( CSPString & strInfo, CSPString & strInfourl, int * pbShow );
	void	SetLatestver( LPCTSTR lpszLatestver );
	BOOL	HasNewVersion( );

	// Auto Resume DR Limit
	int					GetAutoResumeDRLimit( );
	void				SetAutoResumeDRLimit( int nAutoResumeDRLimit );

	// SList Columns
	void				SetSListColumnsDefault( );
	CSPDWordArray	&	GetSListColumnsShow( );
	CSPDWordArray	&	GetSListColumnsOrder( );
	CIndexContainer	&	GetSListColumnsUser( );
	
	void				SetDiffDays( int nDays );
	int					GetDiffDays( );
	void				SetDiffPercentDays( int nDays );
	int					GetDiffPercentDays( );
	void				SetScopeDays( int nDays );
	int					GetScopeDays( );
	void				SetRatioChangeHandDays( int nDays );
	int					GetRatioChangeHandDays( );
	void				SetRatioVolumeDays( int nDays );
	int					GetRatioVolumeDays( );
	void				SetRSDays( int nDays );
	int					GetRSDays( );
	void				SetYieldAverageDays( int nDays );
	int					GetYieldAverageDays( );
	int					GetMaxDaysCalculate( );
	
	void				SetCurrentStock( int nStockIndex, BOOL bFromSList );
	void				SetCurrentStock( const char * szCode, BOOL bFromSList );
	BOOL				GetCurrentStock( CStockInfo * pInfo );
	BOOL				GetPrevStock( CStockInfo * pInfo );
	BOOL				GetNextStock( CStockInfo * pInfo );

	// Info View
	int					GetInfoCurrentServer( );
	void				SetInfoCurrentServer( int nServer );

	// Graph Techs
	void				GetGraphTechsDefault( CSPDWordArray & anShow );
	CSPDWordArray	&	GetGraphTechsShow( );
	CSPDWordArray	&	GetGraphTechsKLine( );
	CSPDWordArray	&	GetGraphTechsAll( );
	BOOL				IsGraphTechShow( UINT nTech );
	void				ToggleGraphViewTech( UINT nTech );
	void				ShowGraphViewTech( UINT nTech );
	int					GetGraphKFormat( );
	void				SetGraphKFormat( int kformat );
	int					GetGraphMaindataType( );
	void				SetGraphMaindataType( int maindatatype );
	DWORD				GetAutoResumeDRBegin( );	//	0 means all, -1 means none, others means time_t
	void				SetAutoResumeDRBegin( DWORD dateBegin );

	// SListView Font And BaseView Font
	BOOL				IsValidFont( LPLOGFONT pLogFont );
	void				GetFontSListView( LPLOGFONT pLogFont );
	void				SetFontSListView( LPLOGFONT pLogFont );
	void				GetFontBaseView( LPLOGFONT pLogFont );
	void				SetFontBaseView( LPLOGFONT pLogFont );

	CTechParameters	&	GetTechParameters( );

	void				AddHTMLURLToExplorer( LPCTSTR lpszURL );
	CSPStringArray &	GetHTMLURLToExplorer( );

	BOOL				GetRcvDriverDefault( CSPStringArray & astrProvider, CSPStringArray & astrDriver );
	void				SetRcvDriverDefault( CSPStringArray & astrProvider, CSPStringArray & astrDriver );

	CSPString			GetTrader( );
	void				SetTrader( LPCTSTR lpszFileTrader );

	BOOL				GetAutoSaveKDataMin( );
	void				SetAutoSaveKDataMin( BOOL bSave );

	BOOL				GetNotifyIcon( );
	void				SetNotifyIcon( BOOL bNotifyIcon );

	UINT				GetCacheDays( );
	void				SetCacheDays( UINT nCacheDays );

	CAlarmCondContainer & GetAlarmCondContainer( );

	int					GetLocalDataVersion( );
	void				SetLocalDataVersion( int nDataVersion );
protected:
	DWORD			m_dwFileVersion;

	CSPString		m_strDefaultServer;
	CSPStringArray	m_astrDownloadServers;
	// Proxy (first part), not use now, use CQSProfile's proxy settings
	int				m_nAccessType;
	CSPString		m_strProxyAddress;
	WORD			m_nProxyPort;

	CSPString		m_strWindowPlacement;
	CSPString		m_strWorkDirectory;			//	Not Save in profile
	CSPString		m_strQianlongPath;
	
	// Color
	CColorClassContainer	m_ColorClassContainer;

	// Last Opened Strategis
	CSPStringArray	m_astrLastOpenedStrategy;
	CSPString		m_strLastActiveStrategy;

	// Net Info
	BOOL			m_bNetInfoShow;
	CSPString		m_strNetInfo;
	CSPString		m_strNetInfourl;
	CSPString		m_strLatestver;

	// Auto Resume DR limit
	int				m_nAutoResumeDRLimit;
	
	// SListView
	CSPDWordArray		m_anSListColumnsShow;
	CSPDWordArray		m_anSListColumnsOrder;
	CIndexContainer		m_aSListColumnsUser;
	int					m_nDiffDays;
	int					m_nDiffPercentDays;
	int					m_nScopeDays;
	int					m_nRatioChangeHandDays;
	int					m_nRatioVolumeDays;
	int					m_nRSDays;
	int					m_nYieldAverageDays;
	
	int					m_bCurStockFromSList;	//	Not Save in profile

	// Info View
	int					m_nInfoCurrentServer;

	// GraphView
	CSPDWordArray		m_anGraphTechsShow;
	CSPDWordArray		m_anGraphTechsKLine;
	CSPDWordArray		m_anGraphTechsAll;
	int					m_nGraphKFormat;
	int					m_nGraphMaindataType;
	DWORD				m_dwAutoResumeDRBegin;

	// SListView Font And BaseView Font
	LOGFONT				m_lfSListView;
	LOGFONT				m_lfBaseView;

	CTechParameters		m_techparameters;

	// HTML URL to Explorer
	CSPStringArray		m_aHTMLURLToExplorer;

	// 缺省启动兼容通视引擎
	CSPStringArray		m_astrProvider;
	CSPStringArray		m_astrDriver;

	// 缺省交易软件
	CSPString			m_strTrader;

	// 是否自动保存KData Minute数据
	BOOL				m_bAutoSaveKDataMin;

	// Proxy (second part), not use now, use CQSProfile's proxy settings
	int				m_nProxyType;
	CSPString		m_strProxyUser;
	CSPString		m_strProxyPasswd;

	// 是否最小化时显示为通知图标
	BOOL				m_bNotifyIcon;

	// K线缓存天数,该设置数据仅在本次运行期间保存,不存盘
	UINT				m_nCacheDays;

	// 预警系统设置
	CAlarmCondContainer	m_aAlarmCond;

	// 本地代码表,板块,基本资料等数据的版本号
	int					m_nLocalDataVersion;
};

#endif // __STKLIB_PROFILE_H__

