// Secret.inl : implementation file
//

/////////////////////////////////////////////////////////////////////////////
// CSView

/*
1 系列号结构
	CHECK-RANDM-IDENT-IFYNO
	①不输入系列号，则为试用
	②加密文件保存试用日期和系列号文本，密钥为512位
	③CHECK为RANDM-IDENT-IFYNO以及注册用户名称的Hash函数校验码
	④RANDM为随机数
	⑤IDENT-IFYNO为唯一标识，应该包含年月日时分秒信息，转换成20进制表示，
	  IDENT为高位，IFYNO为低位
	⑥所有字母为A-Z大写字母和0-9数字，不能使用以下字母和数字：
		0、O、2、Z、8、B、6、U、V、C、G、1、I、D、Q、Y
*/

// static functions
_SECRET_INLINE CString sysstrtosysstr( CString strSrc )
{
	CString	strResult;
	for( int i=strSrc.GetLength()-1; i>=0; i-- )
	{
		char	chCur	=	strSrc[i];
		strResult	+=	chCur;
	}
	return strResult;
}

_SECRET_INLINE CString strtostr( CString strSrc )
{
	CString	strResult;
	for( int i=strSrc.GetLength()-1; i>=0; i-- )
	{
		char	chCur	=	strSrc[i];
		chCur	=	char(256-chCur);
		strResult	+=	chCur;
	}
	return strResult;
}

_SECRET_INLINE int int64tostr( unsigned __int64 src, char * s, int len )
{
	char * pbuf = new char[len+1];
	if( 0 == pbuf )
		return -1;

	memset( s, 0, len );

	int n = 0;
	while( src != 0 && n < len-1 )
	{
		*(pbuf+n)	=	(unsigned char)(src % 256);
		src	=	src/256;
		n ++;
	}
	
	for( int i=n-1; i>=0; i-- )
		*(s+n-1-i)	=	*(pbuf+i);
	
	delete [] pbuf;

	if( src != 0 )
		return -1;
	return n;
}

_SECRET_INLINE unsigned __int64 strtoint64( const char * s, int len )
{
	unsigned __int64	x	=	0;
	int n = 0;
	while (n<len)
	{
		x = x * 256 + (unsigned __int64)((unsigned char)*s);
		s += 1;
		n += 1;
	}
	return x;
}

_SECRET_INLINE CString int64tosysstr( unsigned __int64 src )
{
	char	szSystem20[SE_LEN_SYSTEMSTR+1]	=	SE_SYSTEM20_CHAR;

	CString	strResult;
	while( src > 0 )
	{
		strResult	=	szSystem20[ src % SE_LEN_SYSTEMSTR ] + strResult;
		src	=	src / SE_LEN_SYSTEMSTR;
	}
	return strResult;
}

_SECRET_INLINE unsigned __int64 sysstrtoint64( CString strSrc )
{
	char	szSystem20[SE_LEN_SYSTEMSTR+1]	=	SE_SYSTEM20_CHAR;

	unsigned __int64	result	=	0;
	unsigned __int64	w	=	1;
	for( int i=strSrc.GetLength()-1; i>=0; i-- )
	{
		int k;
		for( k=0; k<SE_LEN_SYSTEMSTR; k++ )
		{
			if( strSrc[i] == szSystem20[k] )
			{
				result	+=	k * w;
			}
		}
		if( k == SE_LEN_SYSTEMSTR )
		{
			TRACE( "sysstrtoint64, s is not sysstr.\n" );
		}
		w	*=	SE_LEN_SYSTEMSTR;
	}
	return result;
}

_SECRET_INLINE CString strtosysstr( const char * s, int len )
{
	vlong	vstr( s, len, FALSE );

	CString	strResult;
	char	* psysstr = new char [len*3+2];
	if( psysstr )
	{
		memset( psysstr, 0, len*3+2 );
		vstr.to_sysstr( psysstr, len*3+1 );
		strResult	=	psysstr;
		delete	[] psysstr;
	}
	return strResult;
}

_SECRET_INLINE CSPString CSView::GetS( )
{
	DWORD	id	=	SE_MASK_SOFTNO;
	// WILLDO
	// ::GetVolumeInformation( "C:\\", NULL, 0, &id, NULL, NULL, NULL, 0 );
	// id	=	(DWORD) getHardDriveComputerID ();
	// id	=	id ^ SE_MASK_SOFTNO;

	CString	strSerial	=	int64tosysstr( (unsigned __int64)id );
	return (LPCTSTR)strSerial;
}

_SECRET_INLINE BOOL CSView::Valid( LPCTSTR lpszSrc, LPCTSTR lpszRegCode, public_key & rsapub )
{
	CString	strSrc	=	lpszSrc;
	CString	strRegCode	=	lpszRegCode;

	/* Convert strRegCode to strDest */
	CString	strDest;
	for( int i=0; i<strRegCode.GetLength(); i++ )
	{
		if( strRegCode[i] != '-' )	strDest	+=	strRegCode[i];
	}

	/* Length Valid */
	if( strSrc.GetLength() < SE_MIN_RSASYSSTR || strSrc.GetLength() > SE_MAX_RSASYSSTR
		|| strDest.GetLength() < SE_MIN_RSASYSSTR || strDest.GetLength() > SE_MAX_RSASYSSTR )
		return FALSE;
	
	/* RSA Decrypt With Public Key*/
	vlong	vDest( strDest, strDest.GetLength(), TRUE );
	if( vDest > rsapub.m )
		return FALSE;
	vlong	vSrcCal = rsapub.encrypt( vDest );

	/* Convert vSrcCal to bufSrcCal */
	char	bufSrcCal[SE_MAX_RSASYSSTR+2];
	memset( bufSrcCal, 0, sizeof(bufSrcCal) );
	int	nBufSrcCalLen = vSrcCal.to_sysstr( bufSrcCal, sizeof(bufSrcCal)-1 );
	if( -1 == nBufSrcCalLen /*|| nBufSrcCalLen < SE_MIN_RSASYSSTR*/ || nBufSrcCalLen > SE_MAX_RSASYSSTR )
		return FALSE;

	/* Prepare strSrc */
	strSrc.MakeUpper();
	strSrc	=	sysstrtosysstr( strSrc );

	/* Modified Jan 5, 2003 */
	/* 3 is 0, and is not showed in bufSrcCal */
	char	szSystem20[SE_LEN_SYSTEMSTR+1]	=	SE_SYSTEM20_CHAR;
	while( strSrc.GetLength() > nBufSrcCalLen && strSrc.GetLength() > 0 && szSystem20[0] == strSrc[0] )
		strSrc	=	strSrc.Mid( 1 );

	/* Compare */
	return ( nBufSrcCalLen == strSrc.GetLength()
		&& 0 == strncmp( strSrc, bufSrcCal, nBufSrcCalLen ) );
}

_SECRET_INLINE BOOL CSView::ValidStd( LPCTSTR lpszUser, LPCTSTR lpszSoftNO, LPCTSTR lpszRegCode )
{
	// WILLDO For Standard Versions, No Standard Version Now
	return FALSE;

	CString	strUser	=	lpszUser;
	CString	strSoftNO	=	lpszSoftNO;
	CString	strRegCode	=	lpszRegCode;

	if( strUser.GetLength() < SE_MIN_USERNAME || strUser.GetLength() > SE_MAX_USERNAME
		|| strSoftNO.GetLength() < SE_MIN_SOFTNO || strSoftNO.GetLength() > SE_MAX_SOFTNO )
		return FALSE;

	if( -1 == m_mStdlen || -1 == m_eStdlen )
	{
		// ASSERT( FALSE );
		return FALSE;
	}

	public_key	rsapub;
	rsapub.m	=	vlong( m_mStd, m_mStdlen, FALSE );
	rsapub.e	=	vlong( m_eStd, m_eStdlen, FALSE );

	strUser.MakeUpper( );
	return Valid( strtosysstr( strUser, 2 ) + strSoftNO, strRegCode, rsapub );
}

_SECRET_INLINE BOOL CSView::ValidProf( LPCTSTR lpszUser, LPCTSTR lpszSoftNO, LPCTSTR lpszRegCode )
{
	CString	strUser	=	lpszUser;
	CString	strSoftNO	=	lpszSoftNO;
	CString	strRegCode	=	lpszRegCode;

	if( strUser.GetLength() < SE_MIN_USERNAME || strUser.GetLength() > SE_MAX_USERNAME
		|| strSoftNO.GetLength() < SE_MIN_SOFTNO || strSoftNO.GetLength() > SE_MAX_SOFTNO )
		return FALSE;

	if( -1 == m_mProflen || -1 == m_eProflen )
	{
		// ASSERT( FALSE );
		return FALSE;
	}

	public_key	rsapub;
	rsapub.m	=	vlong( m_mProf, m_mProflen, FALSE );
	rsapub.e	=	vlong( m_eProf, m_eProflen, FALSE );

	strUser.MakeUpper( );
	return Valid( strtosysstr( strUser, 2 ) + strSoftNO, strRegCode, rsapub );
}

////////////////////////////////////////////////////////////////////////////////////
// Function Cannot Dispatched Begin

#define	EVACODE_MASK	0x128932A2

_SECRET_INLINE BOOL CSView::DumpEva( long tm, CSPString &strRegCode )
{
	__int64	t		=	tm / 86400;
	__int64 rand	=	tm % 86400;
	t	^=	EVACODE_MASK;
	t	|=	(rand << 32);
	CString str = int64tosysstr( t );
	strRegCode = sysstrtosysstr( str );
	return (strRegCode.GetLength() > 0);
}

_SECRET_INLINE BOOL CSView::Dump( LPCTSTR lpszSrc, CSPString &strRegCode, private_key & rsapri )
{
	CString	strSrc	=	lpszSrc;

	if( strSrc.GetLength() < SE_MIN_RSASYSSTR || strSrc.GetLength() > SE_MAX_RSASYSSTR )
		return FALSE;

	// Prepare strSrc
	strSrc.MakeUpper();
	strSrc	=	sysstrtosysstr( strSrc );

	// RSA Encrypt With Private Key
	vlong	vSrc( strSrc, strSrc.GetLength(), TRUE );
	if( vSrc >= rsapri.m )
		return FALSE;
	vlong vDest = rsapri.decrypt( vSrc );

	// Convert vDest to bufDest
	char	bufDest[SE_MAX_RSASYSSTR+2];
	memset( bufDest, 0, sizeof(bufDest) );
	int	nBufDestLen = vDest.to_sysstr( bufDest, sizeof(bufDest)-1 );
	if( -1 == nBufDestLen || nBufDestLen < SE_MIN_RSASYSSTR || nBufDestLen > SE_MAX_RSASYSSTR )
		return FALSE;

	// Convert bufDest to strDest
	CSPString	strDest	=	bufDest;

	// Convert strDest to strDest
	strRegCode.Empty();
	while( strDest.GetLength() > 0 )
	{
		if( strDest.GetLength() <= SE_LEN_SEPREGCODE )
		{
			strRegCode	+=	strDest;
			strDest.Empty();
		}
		else
		{
			strRegCode	+=	strDest.Left( SE_LEN_SEPREGCODE );
			strRegCode	+=	'-';
			strDest	=	strDest.Mid( SE_LEN_SEPREGCODE );
		}
	}

	return TRUE;
}

_SECRET_INLINE BOOL CSView::Dump( LPCTSTR lpszUser, LPCTSTR lpszSoftNO, CSPString &strRegCode,
									const char * r1, const char *r2 )
{
	CString	strUser	=	lpszUser;
	CString	strSoftNO	=	lpszSoftNO;

	if( strUser.GetLength() < SE_MIN_USERNAME || strUser.GetLength() > SE_MAX_USERNAME
		|| strSoftNO.GetLength() < SE_MIN_SOFTNO || strSoftNO.GetLength() > SE_MAX_SOFTNO )
		return FALSE;

	private_key	rsapri;
	rsapri.create( r1, r2 );
	strUser.MakeUpper( );
	BOOL b = Dump( strtosysstr( strUser, 2 ) + strSoftNO, strRegCode, rsapri );

	public_key	rsapub;
	rsapub.m	=	rsapri.m;
	rsapub.e	=	rsapri.e;
	return ( b && Valid( strtosysstr( strUser, 2 ) + strSoftNO, strRegCode, rsapub ) );
}

_SECRET_INLINE BOOL CSView::DumpStd( LPCTSTR lpszUser, LPCTSTR lpszSoftNO, CSPString &strRegCode )
{
	return Dump( lpszUser, lpszSoftNO, strRegCode, SE_SEED_STD1, SE_SEED_STD2 );
}

_SECRET_INLINE BOOL CSView::DumpProf( LPCTSTR lpszUser, LPCTSTR lpszSoftNO, CSPString &strRegCode )
{
	return Dump( lpszUser, lpszSoftNO, strRegCode, SE_SEED_PROF1, SE_SEED_PROF2 );
}

_SECRET_INLINE BOOL CSView::SetME( )
{
	private_key	rsapriStd;
	rsapriStd.create( SE_SEED_STD1, SE_SEED_STD2 );

	m_mStdlen	=	rsapriStd.m.to_str( m_mStd, sizeof(m_mStd)-1 );
	m_eStdlen	=	rsapriStd.e.to_str( m_eStd, sizeof(m_eStd)-1 );

	private_key	rsapriProf;
	rsapriProf.create( SE_SEED_PROF1, SE_SEED_PROF2 );

	m_mProflen	=	rsapriProf.m.to_str( m_mProf, sizeof(m_mProf)-1 );
	m_eProflen	=	rsapriProf.e.to_str( m_eProf, sizeof(m_eProf)-1 );

	return TRUE;
}

_SECRET_INLINE BOOL CSView::SetURDir( LPCTSTR lpszUser, LPCTSTR lpszRegCode )
{
	m_strU	=	strtostr( lpszUser );
	m_strR	=	strtostr( lpszRegCode );
	return TRUE;
}

// Function Cannot Dispatched Ended
////////////////////////////////////////////////////////////////////////////////////

_SECRET_INLINE CSPString	CSView::GetU( )
{
	return (LPCTSTR)strtostr(m_strU);
}

_SECRET_INLINE CSPString	CSView::GetR( )
{
	return (LPCTSTR)strtostr(m_strR);
}

_SECRET_INLINE BOOL CSView::SetUR( LPCTSTR lpszUser, LPCTSTR lpszRegCode )
{
	if( !ValidStd( lpszUser, GetS(), lpszRegCode )
		&& !ValidProf( lpszUser, GetS(), lpszRegCode ) )
		return FALSE;

	m_strU	=	strtostr( lpszUser );
	m_strR	=	strtostr( lpszRegCode );
	return TRUE;
}

_SECRET_INLINE void CSView::SetInsTimeIfNull( )
{
	CSPTime	tmNow	=	CSPTime::GetCurrentTime();

	if( NULL == m_tmIns.GetTime() )
		m_tmIns	=	tmNow;
	if( NULL == m_tmExp.GetTime() )
		m_tmExp	=	m_tmIns+CSPTimeSpan(GetMaxEvaDays(),23,0,0);
}

_SECRET_INLINE void CSView::ResetInsTime( )
{
	CSPTime	tmNow	=	CSPTime::GetCurrentTime();

	m_tmIns	=	tmNow;
	m_tmExp	=	m_tmIns+CSPTimeSpan(GetMaxEvaDays(),23,0,0);
}

_SECRET_INLINE BOOL CSView::IsEva( )
{
	if( !ValidStd( strtostr(m_strU), GetS(), strtostr(m_strR) )
		&& !ValidProf( strtostr(m_strU), GetS(), strtostr(m_strR) ) )
		return TRUE;

	return FALSE;
}

_SECRET_INLINE BOOL CSView::IsStd( )
{
	return ValidStd( strtostr(m_strU), GetS(), strtostr(m_strR) );
}

_SECRET_INLINE BOOL CSView::IsProf( )
{
	return ValidProf( strtostr(m_strU), GetS(), strtostr(m_strR) );
}

_SECRET_INLINE BOOL CSView::AssertValid( )
{
	if( IsEva() && GetLeft() <= 0 )
		return FALSE;
	return TRUE;
}

_SECRET_INLINE int CSView::GetLeft( )
{
	// WILLDO
	// return	GetMaxEvaDays() + 10;	//	No Time Limit

	SetInsTimeIfNull( );

	CSPTime	tmNow	=	CSPTime::GetCurrentTime();

	CSPTimeSpan	used	=	tmNow - m_tmIns;
	if( used.GetDays() > GetMaxEvaDays() )
		return 0;

	CSPTimeSpan	left	=	m_tmExp - tmNow;
	if( left.GetDays() > GetMaxEvaDays() || left.GetDays() < 0 )
		return 0;
	return (int)left.GetDays();
}

_SECRET_INLINE BOOL CSView::SetEvaCode( LPCTSTR lpszRegCode )
{
	//如果不允许用户连续试用，取消下面两行的注释。
	//if( GetMaxEvaDays() > 0 )
	//	return FALSE;

	CString	strRegCode = lpszRegCode;

	strRegCode = sysstrtosysstr( strRegCode );
	__int64 t = sysstrtoint64( strRegCode );
	t	&=	0xFFFFFFFF;
	t	^=	EVACODE_MASK;
	__int64 now = CSPTime::GetCurrentTime().GetTime();
	__int64	days = t - (now/86400);

	if( days < 0 || days > 3650 )
		return FALSE;

	m_strR = strRegCode;
	SetMaxEvaDays( );
	ResetInsTime( );
	return TRUE;
}

_SECRET_INLINE void CSView::SetMaxEvaDays(int nDays)
{
	m_nMaxEvaDays = nDays;
}

_SECRET_INLINE int CSView::GetMaxEvaDays()
{
	return m_nMaxEvaDays;
}

_SECRET_INLINE CSPString	CSView::GetNSAText( )
{
	return (LPCTSTR)m_strNSAText;
}

_SECRET_INLINE void CSView::SetNSAText( LPCTSTR lpszNSAText )
{
	m_strNSAText	=	lpszNSAText;
}


