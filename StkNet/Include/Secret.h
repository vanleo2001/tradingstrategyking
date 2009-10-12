/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CDView;
		class	CDLocalView;
*/

#ifndef __ND_SECRET_H__
#define __ND_SECRET_H__

/***
	有关加密的部分，包括DES数据加密，RSA对称密钥加密，硬盘系列号读取等

	如果希望脱离本软件单独使用secret.h, secret.inl, secret.cpp，将三个
	文件中用到的所有CSPString替换成CString即可。
*/
///////////////////////////////////////////////////////////////
//	32bit DES
///////////////////////////////////////////////////////////////

#define		DESLOCAL_PRODUCT_STOCKANA	0x1021
#define		DESLOCAL_DEFAULTKEY			0xE496A1B9

class CDView : public CWnd
{
private:
	WORD	m_SubKey[4];

public:
	CDView( DWORD key = DESLOCAL_DEFAULTKEY );
	void Create( BYTE *pSrc, BYTE *pDest, DWORD len );
	
protected:
	WORD m_awTable[ 256 ];
	void InitTable();
	WORD CalcTable(WORD dat,WORD genpoly,WORD accum);
public:
	WORD Update( WORD crc,WORD byte );
};

#define	DESLOCAL_KEY1	0xDEC3F075
#define	DESLOCAL_KEY2	0xABCD0123
#define	DESLOCAL_KEY3	0xDC03782A
#define	DESLOCAL_KEY4	0xEF00257D

#define	DESLOCAL_KEY5	0xACE02574
#define	DESLOCAL_KEY6	0xDEF03547
#define	DESLOCAL_KEY7	0xBCDEFACD
#define	DESLOCAL_KEY8	0x53796ACD

///////////////////////////////////////////////////////////////
//	256bit DES
///////////////////////////////////////////////////////////////

class CDLocalView : public CWnd
{
public:
	CDLocalView( );

	void Create( BYTE *pSrc, BYTE *pDest, DWORD len );
	void Destroy( BYTE *pSrc, BYTE *pDest, DWORD len );
};


///////////////////////////////////////////////////////////////////////////////
//	RSA
// RSA public key encryption

class vlong // very long integer - can be used like long
{
public:
	// Standard arithmetic operators
	friend vlong operator +( const vlong& x, const vlong& y );
	friend vlong operator -( const vlong& x, const vlong& y );
	friend vlong operator *( const vlong& x, const vlong& y );
	friend vlong operator /( const vlong& x, const vlong& y );
	friend vlong operator %( const vlong& x, const vlong& y );
	vlong& operator +=( const vlong& x );
	vlong& operator -=( const vlong& x );

	// Standard comparison operators
	friend inline int operator !=( const vlong& x, const vlong& y ){ return x.cf( y ) != 0; }
	friend inline int operator ==( const vlong& x, const vlong& y ){ return x.cf( y ) == 0; }
	friend inline int operator >=( const vlong& x, const vlong& y ){ return x.cf( y ) >= 0; }
	friend inline int operator <=( const vlong& x, const vlong& y ){ return x.cf( y ) <= 0; }
	friend inline int operator > ( const vlong& x, const vlong& y ){ return x.cf( y ) > 0; }
	friend inline int operator < ( const vlong& x, const vlong& y ){ return x.cf( y ) < 0; }

	// Construction and conversion operations
	vlong ( unsigned x=0 );
	vlong ( const vlong& x );
	vlong ( const char * s, int len, int bSysStr );
	~vlong();
	operator unsigned ();
	vlong& operator =(const vlong& x);

	static	vlong from_str( const char * s, int len );
	static	vlong from_sysstr( const char * s, int len );
	int to_str( char * s, int len );
	int to_sysstr( char * s, int len );

private:
	class vlong_value * value;
	int negative;
	int cf( const vlong x ) const;
	void docopy();
	friend class monty;
};

vlong modexp( const vlong & x, const vlong & e, const vlong & m ); // m must be odd
vlong gcd( const vlong &X, const vlong &Y ); // greatest common denominator
vlong modinv( const vlong &a, const vlong &m ); // modular inverse


class public_key
{
public:
	vlong m,e;
	vlong encrypt( const vlong& plain ); // Requires 0 <= plain < m
};

class private_key : public public_key
{
public:
	vlong p,q;
	vlong decrypt( const vlong& cipher );

	void create( const char * r1, const char * r2 );
	// r1 and r2 should be null terminated random strings
	// each of length around 35 characters (for a 500 bit modulus)
};

///////////////////////////////////////////////////////////////////////////////
// Disk Serial 

#pragma pack(1)

struct GDT_DESCRIPTOR 
{ 
	WORD Limit_0_15; 
	WORD Base_0_15; 
	BYTE Base_16_23; 

	BYTE Type : 4; 
	BYTE System : 1; 
	BYTE DPL : 2; 
	BYTE Present : 1; 
	BYTE Limit_16_19 : 4; 
	BYTE Available : 1; 
	BYTE Reserved : 1; 
	BYTE D_B : 1; 
	BYTE Granularity : 1; 
	BYTE Base_24_31; 
}; 

struct CALLGATE_DESCRIPTOR 
{ 
	WORD Offset_0_15; 
	WORD Selector; 
	WORD ParamCount : 5; 
	WORD Unused : 3; 
	WORD Type : 4; 
	WORD System : 1; 
	WORD DPL : 2; 
	WORD Present : 1; 
	WORD Offset_16_31; 
}; 

struct GDTR 
{ 
	WORD wGDTLimit; 
	DWORD dwGDTBase; 
}; 

#pragma pack() 


//extern "C" 
//{ 
// PORT32API bool _stdcall GetPortVal(WORD wPortAddr, PDWORD pdwPortVal, BYTE bSize); 
// PORT32API bool _stdcall SetPortVal(WORD wPortAddr, DWORD dwPortVal, BYTE bSize); 
//} 

//////////////////////////////////////////////////////////////////////
//winio.h 

// Define the various device type values. Note that values used by Microsoft 
// Corporation are in the range 0-32767, and 32768-65535 are reserved for use 
// by customers. 

#define FILE_DEVICE_WINIO 0x00008010

// Macro definition for defining IOCTL and FSCTL function control codes. 
// Note that function codes 0-2047 are reserved for Microsoft Corporation, 
// and 2048-4095 are reserved for customers. 

#define WINIO_IOCTL_INDEX 0x810 

// Define our own private IOCTL 

#define IOCTL_WINIO_MAPPHYSTOLIN CTL_CODE(FILE_DEVICE_WINIO, \
WINIO_IOCTL_INDEX, \
METHOD_BUFFERED, \
FILE_ANY_ACCESS)

#define IOCTL_WINIO_UNMAPPHYSADDR CTL_CODE(FILE_DEVICE_WINIO, \
WINIO_IOCTL_INDEX + 1, \
METHOD_BUFFERED, \
FILE_ANY_ACCESS)

#define IOCTL_WINIO_WRITEPORT CTL_CODE(FILE_DEVICE_WINIO, \
WINIO_IOCTL_INDEX + 2, \
METHOD_BUFFERED, \
FILE_ANY_ACCESS)

#define IOCTL_WINIO_READPORT CTL_CODE(FILE_DEVICE_WINIO, \
WINIO_IOCTL_INDEX + 3, \
METHOD_BUFFERED, \
FILE_ANY_ACCESS)

struct tagPhys32Struct 
{ 
	HANDLE PhysicalMemoryHandle; 
	ULONG dwPhysMemSizeInBytes; 
	PVOID pvPhysAddress; 
	PVOID pvPhysMemLin; 
}; 

extern struct tagPhys32Struct Phys32Struct; 

struct tagPort32Struct 
{ 
	USHORT wPortAddr; 
	ULONG dwPortVal; 
	UCHAR bSize; 
}; 

extern struct tagPort32Struct Port32Struct; 

///////////////////////////////////////////////////////////////
//	硬盘系列号
///////////////////////////////////////////////////////////////
long getHardDriveComputerID ();

///////////////////////////////////////////////////////////////////////////////
//	CSView

#define	SE_MIN_USERNAME			3
#define	SE_MAX_USERNAME			128
#define	SE_MIN_SOFTNO			4
#define	SE_MAX_SOFTNO			14

#define	SE_MIN_RSASTR			4
#define	SE_MAX_RSASTR			10
#define	SE_MIN_RSASYSSTR		8
#define	SE_MAX_RSASYSSTR		20

#define	SE_LEN_SEPREGCODE		5
#define	SE_MAX_RSAKEYLEN		64

#define	SE_LEN_SYSTEMSTR		20
#define	SE_SYSTEM20_CHAR		"34579AEFHJKLMNPRSTWX"
#define	SE_MASK_SOFTNO			0x3D45C8A5

#ifdef	CLKVER_OEM
	#define	SE_SEED_STD1			OEM_SEED_STD1
	#define	SE_SEED_STD2			OEM_SEED_STD2
	#define	SE_SEED_PROF1			OEM_SEED_PROF1
	#define	SE_SEED_PROF2			OEM_SEED_PROF2
#else
	#define	SE_SEED_STD1			"x2&^0"
	#define	SE_SEED_STD2			"c0-+5"
	#define	SE_SEED_PROF1			"i0^&1"
	#define	SE_SEED_PROF2			"h5(*8"
#endif

/***
	软件注册类
*/
class	CSView : public CWnd
{
public:
	CSView( );
	virtual	~CSView( );

	static	CSPString	GetS( );		//	Serial NO, from HardDisk Serial

	// operations
	BOOL	Load( LPCTSTR lpszFileName );
	BOOL	Store( LPCTSTR lpszFileName );

	static	BOOL	Valid( LPCTSTR lpszSrc, LPCTSTR lpszRegCode, public_key & rsapub );

	////////////////////////////////////////////////////////////////////////////////////
	// Function Cannot Dispatched Begin
	static	BOOL	DumpEva( long tm, CSPString &strRegCode );
	static	BOOL	Dump( LPCTSTR lpszUser, CSPString &strRegCode, private_key & rsapri );
	static	BOOL	Dump( LPCTSTR lpszUser, LPCTSTR lpszSoftNO, CSPString &strRegCode, const char * r1, const char * r2 );
	static	BOOL	DumpStd( LPCTSTR lpszUser, LPCTSTR lpszSoftNO, CSPString &strRegCode );
	static	BOOL	DumpProf( LPCTSTR lpszUser, LPCTSTR lpszSoftNO, CSPString &strRegCode );
	BOOL	SetME( );
	BOOL	SetURDir( LPCTSTR lpszUserr, LPCTSTR lpszRegCode );
	// Function Cannot Dispatched Ended
	////////////////////////////////////////////////////////////////////////////////////

	// Authorized User and RegCode
	CSPString	GetU( );
	CSPString	GetR( );
	BOOL	SetUR( LPCTSTR lpszUser, LPCTSTR lpszRegCode );

	void	SetInsTimeIfNull( );
	void	ResetInsTime( );

	// evaluate version
	BOOL	IsEva( );
	BOOL	IsStd( );
	BOOL	IsProf( );
	BOOL	AssertValid( );

	// expired time
	int		GetLeft( );
	BOOL	SetEvaCode( LPCTSTR lpszRegCode );
	void	SetMaxEvaDays( int nDays = 365 );
	int		GetMaxEvaDays( );

	// agent enable
	CSPString	GetNSAText( );
	void	SetNSAText( LPCTSTR lpszNSAText );
	
protected:
	// operations
	virtual void Serialize(CArchive& ar);
	BOOL	ValidStd( LPCTSTR lpszUser, LPCTSTR lpszSoftNO, LPCTSTR lpszRegCode );
	BOOL	ValidProf( LPCTSTR lpszUser, LPCTSTR lpszSoftNO, LPCTSTR lpszRegCode );

protected:
	DWORD			m_dwFileVersion;

	CString			m_strU;		//	Authorized User
	CString			m_strR;		//	Register Code

	char			m_mStd[SE_MAX_RSAKEYLEN];		//	RSA Public Key
	char			m_eStd[SE_MAX_RSAKEYLEN];		//	RSA Public Key
	int				m_mStdlen;
	int				m_eStdlen;

	char			m_mProf[SE_MAX_RSAKEYLEN];		//	RSA Public Key
	char			m_eProf[SE_MAX_RSAKEYLEN];		//	RSA Public Key
	int				m_mProflen;
	int				m_eProflen;

	CSPTime			m_tmExp;	//	time Expired
	CSPTime			m_tmIns;	//	time Install

	int				m_nMaxEvaDays;

	CString			m_strAR;		//	m_strAgentRegCode
	CString			m_strNSAText;
};

// Get The Static CSView Object
CSView		&AfxGetSView( );

#endif //__ND_SECRET_H__
