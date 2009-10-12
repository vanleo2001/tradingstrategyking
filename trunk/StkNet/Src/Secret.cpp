/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CDView;
		class	CDLocalView;
*/

#include "StdAfx.h"
#include "../Include/Secret.h"
#include <winioctl.h>


/////////////////////////////////////////////////////////////////////////////
// CDView

CDView::CDView( DWORD key )
{
	for( int i=0; i<4; i++ )
	{
		m_SubKey[i] = LOWORD( key );
		key = key>>4;
	}
	InitTable( );
}

void CDView::Create( BYTE *pSrc, BYTE *pDest, DWORD len )
{
	WORD L,R,t;

	while( len >=4 )
	{
		L = *(WORD*)pSrc;
		pSrc += 2;
		R = *(WORD*)pSrc;
		pSrc += 2;

		L = ( R+m_SubKey[0] )^L;
		R = (L+m_SubKey[1])^R;
		t = L;
		L = MAKEWORD( HIBYTE(R), LOBYTE(R) );
		R = MAKEWORD( HIBYTE(t), LOBYTE(t) );

		L = ( R+m_SubKey[2] )^L;
		R = (L+m_SubKey[3])^R;
		t = L;
		L = MAKEWORD( HIBYTE(R), LOBYTE(R) );
		R = MAKEWORD( HIBYTE(t), LOBYTE(t) );

		L = ( R+m_SubKey[3] )^L;
		R = (L+m_SubKey[2])^R;
		t = L;
		L = MAKEWORD( HIBYTE(R), LOBYTE(R) );
		R = MAKEWORD( HIBYTE(t), LOBYTE(t) );

		L = ( R+m_SubKey[1] )^L;
		R = (L+m_SubKey[0])^R;
		t = L;
		L = R;
		R = t;

		*((DWORD*)pDest) = MAKELONG( L, R );
		pDest += 4;
		len -= 4;
	}//while
	for( DWORD i=0; i<len; i++ )
		*pDest++ = ~(*pSrc++);
}

void CDView::InitTable()
{
	for( int i=0;i<256;i++)  
		m_awTable[i] = CalcTable(i, DESLOCAL_PRODUCT_STOCKANA, 0 );
}

WORD CDView::CalcTable(WORD dat,WORD genpoly,WORD accum)
{
	int i;
	dat <<= 8;
	for(i=8;i>0;i--)
	{
		if((dat^accum) & 0x8000) 
			accum = (accum << 1) ^ genpoly;
        else 
        	accum <<= 1;
		dat <<= 1;
	}
	return(accum);
}

WORD CDView::Update( WORD crc,WORD byte )
{
 	return( (crc << 8) ^ m_awTable[ (crc >> 8) ^ byte ] );
}

/////////////////////////////////////////////////////////////////////////////
// CDLocalView

CDLocalView::CDLocalView( )
{
}

void CDLocalView::Create( BYTE *pSrc, BYTE *pDest, DWORD len )
{
	CDView	des1( DESLOCAL_KEY1 );
	CDView	des2( DESLOCAL_KEY2 );
	CDView	des3( DESLOCAL_KEY3 );
	CDView	des4( DESLOCAL_KEY4 );

	CDView	des5( DESLOCAL_KEY5 );
	CDView	des6( DESLOCAL_KEY6 );
	CDView	des7( DESLOCAL_KEY7 );
	CDView	des8( DESLOCAL_KEY8 );

	ASSERT( len < 0x00FFFFFF );

	BYTE * pTemp = new BYTE[len+1];
	memset(pTemp, 0, sizeof(pTemp) );

	des1.Create( pSrc, pDest, len );
	memcpy( pTemp, pDest, len );
	des2.Create( pTemp, pDest, len );
	memcpy( pTemp, pDest, len );
	des3.Create( pTemp, pDest, len );
	memcpy( pTemp, pDest, len );
	des4.Create( pTemp, pDest, len );

	memcpy( pTemp, pDest, len );
	des5.Create( pTemp, pDest, len );
	memcpy( pTemp, pDest, len );
	des6.Create( pTemp, pDest, len );
	memcpy( pTemp, pDest, len );
	des7.Create( pTemp, pDest, len );
	memcpy( pTemp, pDest, len );
	des8.Create( pTemp, pDest, len );
	delete pTemp;
}

void CDLocalView::Destroy(BYTE *pSrc, BYTE *pDest, DWORD len)
{
	CDView	des1( DESLOCAL_KEY8 );
	CDView	des2( DESLOCAL_KEY7 );
	CDView	des3( DESLOCAL_KEY6 );
	CDView	des4( DESLOCAL_KEY5 );

	CDView	des5( DESLOCAL_KEY4 );
	CDView	des6( DESLOCAL_KEY3 );
	CDView	des7( DESLOCAL_KEY2 );
	CDView	des8( DESLOCAL_KEY1 );

	ASSERT( len < 0x00FFFFFF );

	BYTE * pTemp = new BYTE[len+1];
	memset(pTemp, 0, sizeof(pTemp) );

	des1.Create( pSrc, pDest, len );
	memcpy( pTemp, pDest, len );
	des2.Create( pTemp, pDest, len );
	memcpy( pTemp, pDest, len );
	des3.Create( pTemp, pDest, len );
	memcpy( pTemp, pDest, len );
	des4.Create( pTemp, pDest, len );

	memcpy( pTemp, pDest, len );
	des5.Create( pTemp, pDest, len );
	memcpy( pTemp, pDest, len );
	des6.Create( pTemp, pDest, len );
	memcpy( pTemp, pDest, len );
	des7.Create( pTemp, pDest, len );
	memcpy( pTemp, pDest, len );
	des8.Create( pTemp, pDest, len );

	delete pTemp;
}

/////////////////////////////////////////////////////////////////////////////
// RSA
class flex_unit // Provides storage allocation and index checking
{
  unsigned * a; // array of units
  unsigned z; // units allocated
public:
  unsigned n; // used units (read-only)
  flex_unit();
  ~flex_unit();
  void clear(); // set n to zero
  unsigned get( unsigned i ) const;     // get ith unsigned
  void set( unsigned i, unsigned x );   // set ith unsigned
  void reserve( unsigned x );           // storage hint

  // Time critical routine
  void fast_mul( flex_unit &x, flex_unit &y, unsigned n );
};

class vlong_value : public flex_unit
{
  public:
  unsigned share; // share count, used by vlong to delay physical copying
  int is_zero() const;
  int test( unsigned i ) const;
  unsigned bits() const;
  int cf( vlong_value& x ) const;
  void shl();
  void shr();
  void shr( unsigned n );
  void add( vlong_value& x );
  void subtract( vlong_value& x );
  void init( unsigned x );
  void copy( vlong_value& x );
  operator unsigned(); // Unsafe conversion to unsigned
  vlong_value();
  void mul( vlong_value& x, vlong_value& y );
  void divide( vlong_value& x, vlong_value& y, vlong_value& rem );
};

unsigned flex_unit::get( unsigned i ) const
{
  if ( i >= n ) return 0;
  return a[i];
}

void flex_unit::clear()
{
   n = 0;
}

flex_unit::flex_unit()
{
  z = 0;
  a = 0;
  n = 0;
}

flex_unit::~flex_unit()
{
  unsigned i=z;
  while (i) { i-=1; a[i] = 0; } // burn
  delete [] a;
}

void flex_unit::reserve( unsigned x )
{
  if (x > z)
  {
    unsigned * na = new unsigned[x];
    for (unsigned i=0;i<n;i+=1) na[i] = a[i];
    delete [] a;
    a = na;
    z = x;
  }
}

void flex_unit::set( unsigned i, unsigned x )
{
  if ( i < n )
  {
    a[i] = x;
    if (x==0) while (n && a[n-1]==0) n-=1; // normalise
  }
  else if ( x )
  {
    reserve(i+1);
    for (unsigned j=n;j<i;j+=1) a[j] = 0;
    a[i] = x;
    n = i+1;
  }
}

// Macros for doing double precision multiply
#define BPU ( 8*sizeof(unsigned) )       // Number of bits in an unsigned
#define lo(x) ( (x) & ((1<<(BPU/2))-1) ) // lower half of unsigned
#define hi(x) ( (x) >> (BPU/2) )         // upper half
#define lh(x) ( (x) << (BPU/2) )         // make upper half

void flex_unit::fast_mul( flex_unit &x, flex_unit &y, unsigned keep )
{
  // *this = (x*y) % (2**keep)
  unsigned i,limit = (keep+BPU-1)/BPU; // size of result in words
  reserve(limit); for (i=0; i<limit; i+=1) a[i] = 0;
  unsigned min = x.n; if (min>limit) min = limit;
  for (i=0; i<min; i+=1)
  {
    unsigned m = x.a[i];
    unsigned c = 0; // carry
    unsigned min = i+y.n; if (min>limit) min = limit;
	unsigned j;
    for ( j=i; j<min; j+=1 )
    {
      // This is the critical loop
      // Machine dependent code could help here
      // c:a[j] = a[j] + c + m*y.a[j-i];
      unsigned w, v = a[j], p = y.a[j-i];
      v += c; c = ( v < c );
      w = lo(p)*lo(m); v += w; c += ( v < w );
      w = lo(p)*hi(m); c += hi(w); w = lh(w); v += w; c += ( v < w );
      w = hi(p)*lo(m); c += hi(w); w = lh(w); v += w; c += ( v < w );
      c += hi(p) * hi(m);
      a[j] = v;
    }
    while ( c && j<limit )
    {
      a[j] += c;
      c = a[j] < c;
      j += 1;
    }
  }

  // eliminate unwanted bits
  keep %= BPU; if (keep) a[limit-1] &= (1<<keep)-1;

   // calculate n
  while (limit && a[limit-1]==0) limit-=1;
  n = limit;
};

vlong_value::operator unsigned()
{
  return get(0);
}

int vlong_value::is_zero() const
{
  return n==0;
}

int vlong_value::test( unsigned i ) const
{ return ( get(i/BPU) & (1<<(i%BPU)) ) != 0; }

unsigned vlong_value::bits() const
{
  unsigned x = n*BPU;
  while (x && test(x-1)==0) x -= 1;
  return x;
}

int vlong_value::cf( vlong_value& x ) const
{
  if ( n > x.n ) return +1;
  if ( n < x.n ) return -1;
  unsigned i = n;
  while (i)
  {
    i -= 1;
    if ( get(i) > x.get(i) ) return +1;
    if ( get(i) < x.get(i) ) return -1;
  }
  return 0;
}

void vlong_value::shl()
{
  unsigned carry = 0;
  unsigned N = n; // necessary, since n can change
  for (unsigned i=0;i<=N;i+=1)
  {
    unsigned u = get(i);
    set(i,(u<<1)+carry);
    carry = u>>(BPU-1);
  }
}

void vlong_value::shr()
{
  unsigned carry = 0;
  unsigned i=n;
  while (i)
  {
    i -= 1;
    unsigned u = get(i);
    set(i,(u>>1)+carry);
    carry = u<<(BPU-1);
  }
}

void vlong_value::shr( unsigned x )
{
  unsigned delta = x/BPU; x %= BPU;
  for (unsigned i=0;i<n;i+=1)
  {
    unsigned u = get(i+delta);
    if (x)
    {
      u >>= x;
      u += get(i+delta+1) << (BPU-x);
    }
    set(i,u);
  }
}

void vlong_value::add( vlong_value & x )
{
  unsigned carry = 0;
  unsigned max = n; if (max<x.n) max = x.n;
  reserve(max);
  for (unsigned i=0;i<max+1;i+=1)
  {
    unsigned u = get(i);
    u = u + carry; carry = ( u < carry );
    unsigned ux = x.get(i);
    u = u + ux; carry += ( u < ux );
    set(i,u);
  }
}

void vlong_value::subtract( vlong_value & x )
{
  unsigned carry = 0;
  unsigned N = n;
  for (unsigned i=0;i<N;i+=1)
  {
    unsigned ux = x.get(i);
    ux += carry;
    if ( ux >= carry )
    {
      unsigned u = get(i);
      unsigned nu = u - ux;
      carry = nu > u;
      set(i,nu);
    }
  }
}

void vlong_value::init( unsigned x )
{
  clear();
  set(0,x);
}

void vlong_value::copy( vlong_value& x )
{
  clear();
  unsigned i=x.n;
  while (i) { i -= 1; set( i, x.get(i) ); }
}

vlong_value::vlong_value()
{
  share = 0;
}

void vlong_value::mul( vlong_value& x, vlong_value& y )
{
  fast_mul( x, y, x.bits()+y.bits() );
}

void vlong_value::divide( vlong_value& x, vlong_value& y, vlong_value& rem )
{
  init(0);
  rem.copy(x);
  vlong_value m,s;
  m.copy(y);
  s.init(1);
  while ( rem.cf(m) > 0 )
  {
    m.shl();
    s.shl();
  }
  while ( rem.cf(y) >= 0 )
  {
    while ( rem.cf(m) < 0 )
    {
      m.shr();
      s.shr();
    }
    rem.subtract( m );
    add( s );
  }
}

// Implementation of vlong

void vlong::docopy()
{
  if ( value->share )
  {
    value->share -= 1;
    vlong_value * nv = new vlong_value;
    nv->copy(*value);
    value = nv;
  }
}

int vlong::cf( const vlong x ) const
{
  int neg = negative && !value->is_zero();
  if ( neg == (x.negative && !x.value->is_zero()) )
    return value->cf( *x.value );
  else if ( neg ) return -1;
  else return +1;
}

vlong::vlong (unsigned x)
{
  value = new vlong_value;
  negative = 0;
  value->init(x);
}

vlong::vlong ( const vlong& x ) // copy constructor
{
  negative = x.negative;
  value = x.value;
  value->share += 1;
}

vlong::vlong ( const char * s, int len, int bSysStr )
{
  value = new vlong_value;
  negative = 0;
  value->init(0);
  if( bSysStr )
    *this = from_sysstr( s, len );
  else
    *this = from_str( s, len );
}

vlong& vlong::operator =(const vlong& x)
{
  if ( value->share ) value->share -=1; else delete value;
  value = x.value;
  value->share += 1;
  negative = x.negative;
  return *this;
}

vlong vlong::from_str( const char * s, int len )
{
	vlong x(0);
	int n = 0;
	while (n<len)
	{
		x = x * vlong(256) + vlong((unsigned char)*s);
		s += 1;
		n += 1;
	}
	return x;
}

vlong vlong::from_sysstr( const char * s, int len )
{
	char	szSystem20[SE_LEN_SYSTEMSTR+1]	=	SE_SYSTEM20_CHAR;
	
	vlong x(0);
	int n = 0;
	while (n<len)
	{
		int k;
		for( k=0; k<SE_LEN_SYSTEMSTR; k++ )
		{
			if( szSystem20[k] == *s )
				break;
		}
		if( k == SE_LEN_SYSTEMSTR )
		{
			TRACE( "vlong::from_sysstr( ... ), s is not sysstr.\n" );
			// ASSERT( FALSE );
			break;
		}
		x = x * vlong(SE_LEN_SYSTEMSTR) + vlong(k);
		s += 1;
		n += 1;
	}
	return x;
}

int vlong::to_str( char * s, int len )
{
	char * pbuf = new char[len+1];
	if( 0 == pbuf )
		return -1;

	memset( s, 0, len );

	vlong	vllocal( *this );

	int n = 0;
	while( vllocal != vlong(0) && n < len-1 )
	{
		*(pbuf+n)	=	vllocal % vlong(256);
		vllocal	=	vllocal/vlong(256);
		n ++;
	}
	
	for( int i=n-1; i>=0; i-- )
		*(s+n-1-i)	=	*(pbuf+i);
	
	delete [] pbuf;

	if( vllocal != vlong(0) )
		return -1;
	return n;
}

int vlong::to_sysstr( char * s, int len )
{
	char	szSystem20[SE_LEN_SYSTEMSTR+1]	=	SE_SYSTEM20_CHAR;

	char * pbuf = new char[len+1];
	if( 0 == pbuf )
		return -1;

	memset( s, 0, len );

	vlong	vllocal( *this );

	int n = 0;
	while( vllocal != vlong(0) && n < len-1 )
	{
		*(pbuf+n)	=	szSystem20[ unsigned(vllocal % vlong(SE_LEN_SYSTEMSTR)) ];
		vllocal	=	vllocal/vlong(SE_LEN_SYSTEMSTR);
		n ++;
	}

	for( int i=n-1; i>=0; i-- )
		*(s+n-1-i)	=	*(pbuf+i);
	
	delete [] pbuf;

	if( vllocal != vlong(0) )
		return -1;
	return n;
}

vlong::~vlong()
{
  if ( value->share ) value->share -=1; else delete value;
}

vlong::operator unsigned () // conversion to unsigned
{
  return *value;
}

vlong& vlong::operator +=(const vlong& x)
{
  if ( negative == x.negative )
  {
    docopy();
    value->add( *x.value );
  }
  else if ( value->cf( *x.value ) >= 0 )
  {
    docopy();
    value->subtract( *x.value );
  }
  else
  {
    vlong tmp = *this;
    *this = x;
    *this += tmp;
  }
  return *this;
}

vlong& vlong::operator -=(const vlong& x)
{
  if ( negative != x.negative )
  {
    docopy();
    value->add( *x.value );
  }
  else if ( value->cf( *x.value ) >= 0 )
  {
    docopy();
    value->subtract( *x.value );
  }
  else
  {
    vlong tmp = *this;
    *this = x;
    *this -= tmp;
    negative = 1 - negative;
  }
  return *this;
}

vlong operator +( const vlong& x, const vlong& y )
{
  vlong result = x;
  result += y;
  return result;
}

vlong operator -( const vlong& x, const vlong& y )
{
  vlong result = x;
  result -= y;
  return result;
}

vlong operator *( const vlong& x, const vlong& y )
{
  vlong result;
  result.value->mul( *x.value, *y.value );
  result.negative = x.negative ^ y.negative;
  return result;
}

vlong operator /( const vlong& x, const vlong& y )
{
  vlong result;
  vlong_value rem;
  result.value->divide( *x.value, *y.value, rem );
  result.negative = x.negative ^ y.negative;
  return result;
}

vlong operator %( const vlong& x, const vlong& y )
{
  vlong result;
  vlong_value divide;
  divide.divide( *x.value, *y.value, *result.value );
  result.negative = x.negative; // not sure about this?
  return result;
}

vlong gcd( const vlong &X, const vlong &Y )
{
  vlong x=X, y=Y;
  while (1)
  {
    if ( y == vlong(0) ) return x;
    x = x % y;
    if ( x == vlong(0) ) return y;
    y = y % x;
  }
}

vlong modinv( const vlong &a, const vlong &m ) // modular inverse
// returns i in range 1..m-1 such that i*a = 1 mod m
// a must be in range 1..m-1
{
  vlong j=1,i=0,b=m,c=a,x,y;
  while ( c != vlong(0) )
  {
    x = b / c;
    y = b - x*c;
    b = c;
    c = y;
    y = j;
    j = i - j*x;
    i = y;
  }
  if ( i < vlong(0) )
    i += m;
  return i;
}

class monty // class for montgomery modular exponentiation
{
  vlong R,R1,m,n1;
  vlong T,k;   // work registers
  unsigned N;  // bits for R
  void mul( vlong &x, const vlong &y );
public:
  vlong exp( const vlong &x, const vlong &e );
  monty( const vlong &M );
};

monty::monty( const vlong &M )
{
  m = M;
  N = 0; R = 1; while ( R < M ) { R += R; N += 1; }
  R1 = modinv( R-m, m );
  n1 = R - modinv( m, R );
}

void monty::mul( vlong &x, const vlong &y )
{
  // T = x*y;
  T.value->fast_mul( *x.value, *y.value, N*2 );

  // k = ( T * n1 ) % R;
  k.value->fast_mul( *T.value, *n1.value, N );

  // x = ( T + k*m ) / R;
  x.value->fast_mul( *k.value, *m.value, N*2 );
  x += T;
  x.value->shr( N );

  if (x>=m) x -= m;
}

vlong monty::exp( const vlong &x, const vlong &e )
{
  vlong result = R-m, t = ( x * R ) % m;
  unsigned bits = e.value->bits();
  unsigned i = 0;
  while (1)
  {
    if ( e.value->test(i) )
      mul( result, t);
    i += 1;
    if ( i == bits ) break;
    mul( t, t );
  }
  return ( result * R1 ) % m;
}

vlong modexp( const vlong & x, const vlong & e, const vlong & m )
{
  monty me(m);
  return me.exp( x,e );
}

///////////////////////////////////////////////////////////////////////
// class prime_factory

class prime_factory
{
  unsigned np;
  unsigned *pl;
  public:
  prime_factory();
  ~prime_factory();
  vlong find_prime( vlong & start );
};

// prime factory implementation

static int is_probable_prime( const vlong &p )
{
  // Test based on Fermats theorem a**(p-1) = 1 mod p for prime p
  // For 1000 bit numbers this can take quite a while
  const int rep = 4;
  const unsigned any[rep] = { 2,3,5,7 };
  for ( unsigned i=0; i<rep; i+=1 )
    if ( modexp( any[i], p-1, p ) != vlong(1) )
      return 0;
  return 1;
}

prime_factory::prime_factory()
{
  np = 0;
  unsigned NP = 200;
  pl = new unsigned[NP];

  // Initialise pl
  unsigned SS = 8*NP; // Rough estimate to fill pl
  char * b = new char[SS+1]; // one extra to stop search
  for (unsigned i=0;i<=SS;i+=1) b[i] = 1;
  unsigned p = 2;
  while (1)
  {
    // skip composites
    while ( b[p] == 0 ) p += 1;
    if ( p == SS ) break;
    pl[np] = p;
    np += 1;
    if ( np == NP ) break;
    // cross off multiples
    unsigned c = p*2;
    while ( c < SS )
    {
      b[c] = 0;
      c += p;
    }
    p += 1;
  }
  delete [] b;
}

prime_factory::~prime_factory()
{
  delete [] pl;
}

vlong prime_factory::find_prime( vlong & start )
{
  unsigned SS = 1000; // should be enough unless we are unlucky
  char * b = new char[SS]; // bitset of candidate primes
  unsigned tested = 0;
  while (1)
  {
    unsigned i;
    for (i=0;i<SS;i+=1)
      b[i] = 1;
    for (i=0;i<np;i+=1)
    {
      unsigned p = pl[i];
      unsigned r = start % vlong(p); // not as fast as it should be - could do with special routine
      if (r) r = p - r;
      // cross off multiples of p
      while ( r < SS )
      {
        b[r] = 0;
        r += p;
      }
    }
    // now test candidates
    for (i=0;i<SS;i+=1)
    {
      if ( b[i] )
      {
        tested += 1;
        if ( is_probable_prime(start) )
          return start;
      }
      start += 1;
    }
  }
  delete [] b;
}

///////////////////////////////////////////////////////////////////////
// class private_key and public key

void private_key::create( const char * r1, const char * r2 )
{
  // Choose primes
  {
    prime_factory pf;
    p = pf.find_prime( vlong::from_str(r1,strlen(r1)) );
    q = pf.find_prime( vlong::from_str(r2,strlen(r2)) );
    if ( p > q ) { vlong tmp = p; p = q; q = tmp; }
  }
  // Calculate public key
  {
    m = p*q;
    e = 50001; // must be odd since p-1 and q-1 are even
    while ( gcd(p-vlong(1),e) != vlong(1) || gcd(q-vlong(1),e) != vlong(1) ) e += 2;
  }
}

vlong public_key::encrypt( const vlong& plain )
{
  return modexp( plain, e, m );
}

vlong private_key::decrypt( const vlong& cipher )
{
  // Calculate values for performing decryption
  // These could be cached, but the calculation is quite fast
  vlong d = modinv( e, (p-vlong(1))*(q-vlong(1)) );
  vlong u = modinv( p, q );
  vlong dp = d % (p-vlong(1));
  vlong dq = d % (q-vlong(1));

  // Apply chinese remainder theorem
  vlong a = modexp( cipher % p, dp, p );
  vlong b = modexp( cipher % q, dq, q );
  if ( b < a ) b += q;
  return a + p * ( ((b-a)*u) % q );
}

/////////////////////////////////////////////////////////////////////////////
// Disk Serial

#define PRINTING_TO_CONSOLE_ALLOWED 


#include <stdlib.h>
#include <stdio.h>
#include <windows.h>


// Required to ensure correct PhysicalDrive IOCTL structure setup 
#pragma pack(1) 


// Max number of drives assuming primary/secondary, master/slave topology 
#define MAX_IDE_DRIVES 4 
#define IDENTIFY_BUFFER_SIZE 512 


// IOCTL commands 
#define DFP_GET_VERSION 0x00074080 
#define DFP_SEND_DRIVE_COMMAND 0x0007c084 
#define DFP_RECEIVE_DRIVE_DATA 0x0007c088 

#define FILE_DEVICE_SCSI 0x0000001b 
#define IOCTL_SCSI_MINIPORT_IDENTIFY ((FILE_DEVICE_SCSI << 16) + 0x0501) 
#define IOCTL_SCSI_MINIPORT 0x0004D008 // see NTDDSCSI.H for definition 



// GETVERSIONOUTPARAMS contains the data returned from the 
// Get Driver Version function. 
typedef struct _GETVERSIONOUTPARAMS 
{ 
BYTE bVersion; // Binary driver version. 
BYTE bRevision; // Binary driver revision. 
BYTE bReserved; // Not used. 
BYTE bIDEDeviceMap; // Bit map of IDE devices. 
DWORD fCapabilities; // Bit mask of driver capabilities. 
DWORD dwReserved[4]; // For future use. 
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS; 


// Bits returned in the fCapabilities member of GETVERSIONOUTPARAMS 
#define CAP_IDE_ID_FUNCTION 1 // ATA ID command supported 
#define CAP_IDE_ATAPI_ID 2 // ATAPI ID command supported 
#define CAP_IDE_EXECUTE_SMART_FUNCTION 4 // SMART commannds supported 


// IDE registers 
#if(_WIN32_WINNT < 0x0400)
typedef struct _IDEREGS 
{ 
BYTE bFeaturesReg; // Used for specifying SMART "commands". 
BYTE bSectorCountReg; // IDE sector count register 
BYTE bSectorNumberReg; // IDE sector number register 
BYTE bCylLowReg; // IDE low order cylinder value 
BYTE bCylHighReg; // IDE high order cylinder value 
BYTE bDriveHeadReg; // IDE drive/head register 
BYTE bCommandReg; // Actual IDE command. 
BYTE bReserved; // reserved for future use. Must be zero. 
} IDEREGS, *PIDEREGS, *LPIDEREGS; 
#endif	// (_WIN32_WINNT < 0x0400)

// SENDCMDINPARAMS contains the input parameters for the 
// Send Command to Drive function.
#if(_WIN32_WINNT < 0x0400)
typedef struct _SENDCMDINPARAMS 
{ 
DWORD cBufferSize; // Buffer size in bytes 
IDEREGS irDriveRegs; // Structure with drive register values. 
BYTE bDriveNumber; // Physical drive number to send 
// command to (0,1,2,3). 
BYTE bReserved[3]; // Reserved for future expansion. 
DWORD dwReserved[4]; // For future use. 
BYTE bBuffer[1]; // Input buffer. 
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS; 
#endif	// (_WIN32_WINNT < 0x0400)

#define IDE_ATAPI_IDENTIFY 0xA1 // Returns ID sector for ATAPI. 
#define IDE_ATA_IDENTIFY 0xEC // Returns ID sector for ATA. 



// Status returned from driver 
#if(_WIN32_WINNT < 0x0400)
typedef struct _DRIVERSTATUS 
{ 
BYTE bDriverError; // Error code from driver, or 0 if no error. 
BYTE bIDEStatus; // Contents of IDE Error register. 
// Only valid when bDriverError is SMART_IDE_ERROR. 
BYTE bReserved[2]; // Reserved for future expansion. 
DWORD dwReserved[2]; // Reserved for future expansion. 
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS; 
#endif	// (_WIN32_WINNT < 0x0400)


// Structure returned by PhysicalDrive IOCTL for several commands 
#if(_WIN32_WINNT < 0x0400)
typedef struct _SENDCMDOUTPARAMS 
{ 
DWORD cBufferSize; // Size of bBuffer in bytes 
DRIVERSTATUS DriverStatus; // Driver status structure. 
BYTE bBuffer[1]; // Buffer of arbitrary length in which to store the data read from the
				 // drive. 
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS; 
#endif	// (_WIN32_WINNT < 0x0400)


// The following struct defines the interesting part of the IDENTIFY 
// buffer: 
typedef struct _IDSECTOR 
{ 
USHORT wGenConfig; 
USHORT wNumCyls; 
USHORT wReserved; 
USHORT wNumHeads; 
USHORT wBytesPerTrack; 
USHORT wBytesPerSector; 
USHORT wSectorsPerTrack; 
USHORT wVendorUnique[3]; 
CHAR sSerialNumber[20]; 
USHORT wBufferType; 
USHORT wBufferSize; 
USHORT wECCSize; 
CHAR sFirmwareRev[8]; 
CHAR sModelNumber[40]; 
USHORT wMoreVendorUnique; 
USHORT wDoubleWordIO; 
USHORT wCapabilities; 
USHORT wReserved1; 
USHORT wPIOTiming; 
USHORT wDMATiming; 
USHORT wBS; 
USHORT wNumCurrentCyls; 
USHORT wNumCurrentHeads; 
USHORT wNumCurrentSectorsPerTrack; 
ULONG ulCurrentSectorCapacity; 
USHORT wMultSectorStuff; 
ULONG ulTotalAddressableSectors; 
USHORT wSingleWordDMA; 
USHORT wMultiWordDMA; 
BYTE bReserved[128]; 
} IDSECTOR, *PIDSECTOR; 


typedef struct _SRB_IO_CONTROL 
{ 
ULONG HeaderLength; 
UCHAR Signature[8]; 
ULONG Timeout; 
ULONG ControlCode; 
ULONG ReturnCode; 
ULONG Length; 
} SRB_IO_CONTROL, *PSRB_IO_CONTROL; 


// Define global buffers. 
BYTE IdOutCmd [sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1]; 


char *ConvertToString (DWORD diskdata [256], int firstIndex, int lastIndex); 
void PrintIdeInfo (int drive, DWORD diskdata [256]); 
BOOL DoIDENTIFY (HANDLE, PSENDCMDINPARAMS, PSENDCMDOUTPARAMS, BYTE, BYTE, 
PDWORD); 
int ReadPhysicalDriveInNT (void) 
{ 
int done = FALSE; 
int drive = 0; 

for (drive = 0; drive < MAX_IDE_DRIVES; drive++) 
{ 
HANDLE hPhysicalDriveIOCTL = 0; 


// Try to get a handle to PhysicalDrive IOCTL, report failure 
// and exit if can't. 
char driveName [256]; 

sprintf (driveName, "\\\\.\\PhysicalDrive%d", drive); 

// Windows NT, Windows 2000, must have admin rights 
hPhysicalDriveIOCTL = CreateFile (driveName, 
GENERIC_READ | GENERIC_WRITE, 
FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, 
OPEN_EXISTING, 0, NULL); 
// if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE) 
// printf ("Unable to open physical drive %d, error code: 0x%lX\n", 
// drive, GetLastError ()); 

if (hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE) 
{ 
GETVERSIONOUTPARAMS VersionParams; 
DWORD cbBytesReturned = 0; 

// Get the version, etc of PhysicalDrive IOCTL 
memset ((void*) &VersionParams, 0, sizeof(VersionParams)); 

if ( ! DeviceIoControl (hPhysicalDriveIOCTL, DFP_GET_VERSION, 
NULL, 
0, 
&VersionParams, 
sizeof(VersionParams), 
&cbBytesReturned, NULL) ) 
{ 
// printf ("DFP_GET_VERSION failed for drive %d\n", i); 
// continue; 
} 

// If there is a IDE device at number "i" issue commands 
// to the device 
if (VersionParams.bIDEDeviceMap > 0) 
{ 
BYTE bIDCmd = 0; // IDE or ATAPI IDENTIFY cmd 
SENDCMDINPARAMS scip; 
//SENDCMDOUTPARAMS OutCmd; 

// Now, get the ID sector for all IDE devices in the sysstem. 
// If the device is ATAPI use the IDE_ATAPI_IDENTIFY command, 
// If the device is ATAPI use the IDE_ATAPI_IDENTIFY command, 
// otherwise use the IDE_ATA_IDENTIFY command 
bIDCmd = (VersionParams.bIDEDeviceMap >> drive & 0x10) ? \
IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY; 

memset (&scip, 0, sizeof(scip)); 
memset (IdOutCmd, 0, sizeof(IdOutCmd)); 

if ( DoIDENTIFY (hPhysicalDriveIOCTL, 
&scip, 
(PSENDCMDOUTPARAMS)&IdOutCmd, 
(BYTE) bIDCmd, 
(BYTE) drive, 
&cbBytesReturned)) 
{ 
DWORD diskdata [256]; 
int ijk = 0; 
USHORT *pIdSector = (USHORT *) 
((PSENDCMDOUTPARAMS) IdOutCmd) -> bBuffer; 

for (ijk = 0; ijk < 256; ijk++) 
diskdata [ijk] = pIdSector [ijk]; 


PrintIdeInfo (drive, diskdata); 

done = TRUE; 
} 
} 

CloseHandle (hPhysicalDriveIOCTL); 
} 
} 

return done; 
} 


// DoIDENTIFY 
// FUNCTION: Send an IDENTIFY command to the drive 
// bDriveNum = 0-3 
// bIDCmd = IDE_ATA_IDENTIFY or IDE_ATAPI_IDENTIFY 
BOOL DoIDENTIFY (HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP, 
PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum, 
PDWORD lpcbBytesReturned) 
{ 
// Set up data structures for IDENTIFY command. 
pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE; 
pSCIP -> irDriveRegs.bFeaturesReg = 0; 
pSCIP -> irDriveRegs.bSectorCountReg = 1; 
pSCIP -> irDriveRegs.bSectorNumberReg = 1; 
pSCIP -> irDriveRegs.bCylLowReg = 0; 
pSCIP -> irDriveRegs.bCylHighReg = 0; 

// Compute the drive number. 
pSCIP -> irDriveRegs.bDriveHeadReg = 0xA0 | ((bDriveNum & 1) << 4); 

// The command can either be IDE identify or ATAPI identify. 
pSCIP -> irDriveRegs.bCommandReg = bIDCmd; 
pSCIP -> bDriveNumber = bDriveNum; 
pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE; 

return ( DeviceIoControl (hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA, 
(LPVOID) pSCIP, 
sizeof(SENDCMDINPARAMS) - 1, 
(LPVOID) pSCOP, 
sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1, 
lpcbBytesReturned, NULL) ); 
} 
// ------------------------------------------------- // 
// WinIo v1.2 // 
// Direct Hardware Access Under Windows 9x/NT/2000 // 
// Copyright 1998-2000 Yariv Kaplan // 
// http://www.internals.com // 
// ------------------------------------------------- // 

//#include <windows.h>
//#include "instdrv.h" 

BOOL LoadDeviceDriver( const TCHAR * Name, const TCHAR * Path, HANDLE * lphDevice );
BOOL UnloadDeviceDriver( const TCHAR * Name );

HANDLE hDriver;
bool IsNT;
bool IsWinIoInitialized = false;

bool IsWinNT()
{
OSVERSIONINFO OSVersionInfo;

OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

GetVersionEx(&OSVersionInfo);

return OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT; 
} 


bool InitializeWinIo() 
{ 
char szExePath[MAX_PATH]; 
PSTR pszSlash; 

IsNT = IsWinNT(); 
if (IsNT) 
{ 
if (!GetModuleFileName(GetModuleHandle(NULL), szExePath, 
sizeof(szExePath))) 
return false; 


pszSlash = strrchr(szExePath, '\\'); 

if (pszSlash) 
pszSlash[1] = 0; 
else 
return false; 

strcat(szExePath, "winio.sys"); 

// UnloadDeviceDriver("WINIO"); 

// if (!LoadDeviceDriver("WINIO", szExePath, &hDriver)) 
// return false; 
} 

IsWinIoInitialized = true; 

return true; 
} 


void ShutdownWinIo() 
{ 
// if (IsNT) 
// UnloadDeviceDriver("WINIO"); 
} 


// ------------------------------------------------ // 
// Port32 v3.0 // 
// Direct Port Access Under Windows 9x/NT/2000 // 
// Copyright 1998-2000 Yariv Kaplan // 
// http://www.internals.com // 
// ------------------------------------------------ // 

//#include <windows.h>
#include <winioctl.h>
//#include "diskid32.h" 
//#include "general.h" 

// These are our ring 0 functions responsible for tinkering with the hardware ports. 
// They have a similar privilege to a Windows VxD and are therefore free to re therefore free to access 
// protected system resources (such as the page tables) and even place calls to 
// exported VxD services. 

__declspec(naked) void Ring0GetPortVal() 
{ 
_asm 
{ 
Cmp CL, 1 
Je ByteVal 
Cmp CL, 2 
Je WordVal 
Cmp CL, 4 
Je DWordVal 

ByteVal: 

In AL, DX 
Mov [EBX], AL 
Retf 


WordVal: 

In AX, DX 
Mov [EBX], AX 
Retf 

DWordVal: 

In EAX, DX 
Mov [EBX], EAX 
Retf 
} 
} 
__declspec(naked) void Ring0SetPortVal() 
{ 
_asm 
{ 
Cmp CL, 1 
Je ByteVal 
Cmp CL, 2 
Je WordVal 
Cmp CL, 4 
Cmp CL, 4 
Je DWordVal 

ByteVal: 

Mov AL, [EBX] 
Out DX, AL 
Retf 

WordVal: 

Mov AX, [EBX] 
Out DX, AX 
Retf 

DWordVal: 

Mov EAX, [EBX] 
Out DX, EAX 
Retf 
} 
} 



// This function makes it possible to call ring 0 code from a ring 3 
// application. 

bool CallRing0(PVOID pvRing0FuncAddr, WORD wPortAddr, PDWORD pdwPortVal, BYTE 
bSize) 
{ 

struct GDT_DESCRIPTOR *pGDTDescriptor; 
struct GDTR gdtr; 
WORD CallgateAddr[3]; 
WORD wGDTIndex = 1; 

_asm Sgdt [gdtr] 

// Skip the null descriptor 

pGDTDescriptor = (struct GDT_DESCRIPTOR *)(gdtr.dwGDTBase + 8); 

// Search for a free GDT descriptor 

for (wGDTIndex = 1; wGDTIndex < (gdtr.wGDTLimit / 8); wGDTIndex++) 
{ 
if (pGDTDescriptor->Type == 0 && 
pGDTDescriptor->System == 0 && 
pGDTDescriptor->DPL == 0 && 
pGDTDescriptor->Present == 0) 
{ 
// Found one ! 
// Now we need to transform this descriptor into a callgate. 
// Note that we're using selector 0x28 since it corresponds 
// to a ring 0 segment which spans the entire linear address 
// space of the processor (0-4GB). 

struct CALLGATE_DESCRIPTOR *pCallgate; 

pCallgate = (struct CALLGATE_DESCRIPTOR *) pGDTDescriptor; 
pCallgate->Offset_0_15 = LOWORD(pvRing0FuncAddr); 
pCallgate->Selector = 0x28; 
pCallgate->ParamCount = 0; 
pCallgate->Unused = 0; 
pCallgate->Type = 0xc; 
pCallgate->System = 0; 
pCallgate->DPL = 3; 
pCallgate->DPL = 3; 
pCallgate->Present = 1; 
pCallgate->Offset_16_31 = HIWORD(pvRing0FuncAddr); 

// Prepare the far call parameters 
CallgateAddr[0] = 0x0; 
CallgateAddr[1] = 0x0; 
CallgateAddr[2] = (wGDTIndex << 3) | 3; 

// Please fasten your seat belts! 
// We're about to make a hyperspace jump into RING 0. 

_asm Mov DX, [wPortAddr] 
_asm Mov EBX, [pdwPortVal] 
_asm Mov CL, [bSize] 
_asm Call FWORD PTR [CallgateAddr] 

// We have made it ! 
// Now free the GDT descriptor 

memset(pGDTDescriptor, 0, 8); 

// Our journey was successful. Seeya. 

return true; 
} 

// Advance to the next GDT descriptor 

pGDTDescriptor++; 
} 

// Whoops, the GDT is full 

return false; 
} 

bool GetPortVal(WORD wPortAddr, PDWORD pdwPortVal, BYTE bSize) 
{ 
bool Result; 
DWORD dwBytesReturned; 
struct tagPort32Struct Port32Struct; 

if (IsNT) 
{ 
if (!IsWinIoInitialized) 
return false; 

Port32Struct.wPortAddr = wPortAddr; 
Port32Struct.bSize = bSize; 

if (!DeviceIoControl(hDriver, IOCTL_WINIO_READPORT, &Port32Struct, 
sizeof(struct tagPort32Struct), &Port32Struct, 
sizeof(struct tagPort32Struct), 
&dwBytesReturned, NULL)) 
return false; 
else 
*pdwPortVal = Port32Struct.dwPortVal; 
} 
else 
{ 
Result = CallRing0((PVOID)Ring0GetPortVal, wPortAddr, pdwPortVal, bSize); 

if (Result == false) 
return false; 
} 


return true; 
} 


bool SetPortVal(WORD wPortAddr, DWORD dwPortVal, BYTE bSize) 
{ 
DWORD dwBytesReturned; 
struct tagPort32Struct Port32Struct; 

if (IsNT) 
{ 
if (!IsWinIoInitialized) 
return false; 

Port32Struct.wPortAddr = wPortAddr; 
Port32Struct.dwPortVal = dwPortVal; 
Port32Struct.bSize = bSize; 
if (!DeviceIoControl(hDriver, IOCTL_WINIO_WRITEPORT, &Port32Struct, 
sizeof(struct tagPort32Struct), NULL, 0, 
&dwBytesReturned, NULL)) 
return false; 
} 
else 
return CallRing0((PVOID)Ring0SetPortVal, wPortAddr, &dwPortVal, bSize); 

return true; 
} 


int ReadDrivePortsInWin9X (void) 
{ 
int done = FALSE; 
int drive = 0; 

InitializeWinIo (); 

// Get IDE Drive info from the hardware ports 
// loop thru all possible drives 
for (drive = 0; drive < 8; drive++) 
{ 
DWORD diskdata [256]; 
WORD baseAddress = 0; // Base address of drive controller 
DWORD portValue = 0; 
int waitLoop = 0; 
int index = 0; 

switch (drive / 2) 
{ 
case 0: baseAddress = 0x1f0; break; 
case 1: baseAddress = 0x170; break; 
case 2: baseAddress = 0x1e8; break; 
case 3: baseAddress = 0x168; break; 
} 

// Wait for controller not busy 
waitLoop = 100000; 
while (--waitLoop > 0) 
{ 
GetPortVal ((WORD) (baseAddress + 7), &portValue, (BYTE) 1); 
// drive is ready 
if ((portValue & 0x40) == 0x40) break; 
// previous drive command ended in error 
if ((portValue & 0x01) == 0x01) break; 
} 

if (waitLoop < 1) continue; 
if (waitLoop < 1) continue; 

// Set Master or Slave drive 
if ((drive % 2) == 0) 
SetPortVal ((WORD) (baseAddress + 6), 0xA0, 1); 
else 
SetPortVal ((WORD) (baseAddress + 6), 0xB0, 1); 

// Get drive info data 
SetPortVal ((WORD) (baseAddress + 7), 0xEC, 1); 

// Wait for data ready 
waitLoop = 100000; 
while (--waitLoop > 0) 
{ 
GetPortVal ((WORD) (baseAddress + 7), &portValue, 1); 
// see if the drive is ready and has it's info ready for us 
if ((portValue & 0x48) == 0x48) break; 
// see if there is a drive error 
if ((portValue & 0x01) == 0x01) break; 
} 

// check for time out or other error 

if (waitLoop < 1 || portValue & 0x01) continue; 

// read drive id information 
for (index = 0; index < 256; index++) 
{ 
diskdata [index] = 0; // init the space 
GetPortVal (baseAddress, &(diskdata [index]), 2); 
} 
PrintIdeInfo (drive, diskdata); 
done = TRUE; 
} 

ShutdownWinIo (); 

return done; 
} 


#define SENDIDLENGTH sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE 



int ReadIdeDriveAsScsiDriveInNT (void) 
{ 
int done = FALSE; 
int controller = 0; 

for (controller = 0; controller < 2; controller++) 
{ 
HANDLE hScsiDriveIOCTL = 0; 
char driveName [256]; 

// Try to get a handle to PhysicalDrive IOCTL, report failure 
// and exit if can't. 
sprintf (driveName, "\\\\.\\Scsi%d:", controller); 

// Windows NT, Windows 2000, any rights should do 
hScsiDriveIOCTL = CreateFile (driveName, 
GENERIC_READ | GENERIC_WRITE, 
FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, 
OPEN_EXISTING, 0, NULL); 
// if (hScsiDriveIOCTL == INVALID_HANDLE_VALUE) 
// printf ("Unable to open SCSI controller %d, error code: 0x%lX\n", 
// controller, GetLastError ()); 

if (hScsiDriveIOCTL != INVALID_HANDLE_VALUE) 
{ 
int drive = 0; 
for (drive = 0; drive < 2; drive++) 
{ 
char buffer [sizeof (SRB_IO_CONTROL) + SENDIDLENGTH]; 
SRB_IO_CONTROL *p = (SRB_IO_CONTROL *) buffer; 
SENDCMDINPARAMS *pin = 
(SENDCMDINPARAMS *) (buffer + sizeof (SRB_IO_CONTROL)); 
DWORD dummy; 

memset (buffer, 0, sizeof (buffer)); 
p -> HeaderLength = sizeof (SRB_IO_CONTROL); 
p -> Timeout = 10000; 
p -> Length = SENDIDLENGTH; 
p -> ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY; 
strncpy ((char *) p -> Signature, "SCSIDISK", 8); 

pin -> irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY; 
pin -> bDriveNumber = drive; 


if (DeviceIoControl (hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT, 
buffer, 
sizeof (SRB_IO_CONTROL) + 
sizeof (SENDCMDINPARAMS) - 1, 
buffer, 
sizeof (SRB_IO_CONTROL) + SENDIDLENGTH, 
&dummy, NULL)) 
{ 
SENDCMDOUTPARAMS *pOut = 
(SENDCMDOUTPARAMS *) (buffer + sizeof (SRB_IO_CONTROL)); 
IDSECTOR *pId = (IDSECTOR *) (pOut -> bBuffer); 
if (pId -> sModelNumber [0]) 
{ 
DWORD diskdata [256]; 
int ijk = 0; 
USHORT *pIdSector = (USHORT *) pId; 

for (ijk = 0; ijk < 256; ijk++) 
diskdata [ijk] = pIdSector [ijk]; 

PrintIdeInfo (controller * 2 + drive, diskdata); 
done = TRUE; 
} 
} 
} 
CloseHandle (hScsiDriveIOCTL); 
} 
} 

return done; 
} 


char HardDriveSerialNumber [1024]; 


void PrintIdeInfo (int drive, DWORD diskdata [256]) 
{ 
// copy the hard driver serial number to the buffer 
strcpy (HardDriveSerialNumber, ConvertToString (diskdata, 10, 19)); 

#ifdef PRINTING_TO_CONSOLE_ALLOWED 

switch (drive / 2) 
{ 
case 0: printf ("\nPrimary Controller - "); 
break; 
case 1: printf ("\nSecondary Controller - "); 
break; 
case 2: printf ("\nTertiary Controller - "); 
break; 
case 3: printf ("\nQuaternary Controller - "); 
break; 
} 

switch (drive % 2) 
{ 
case 0: printf ("Master drive\n\n"); 
break; 
case 1: printf ("Slave drive\n\n"); 
break; 
} 
printf ("Drive Model Number________________: %s\n", 
ConvertToString (diskdata, 27, 46)); 
printf ("Drive Serial Number_______________: %s\n", 
ConvertToString (diskdata, 10, 19)); 
printf ("Drive Controller Revision Number__: %s\n", 
ConvertToString (diskdata, 23, 26)); 

printf ("Controller Buffer Size on Drive___: %u bytes\n", 
diskdata [21] * 512); 

printf ("Drive Type________________________: "); 
if (diskdata [0] & 0x0080) 
printf ("Removable\n"); 
else if (diskdata [0] & 0x0040) 
printf ("Fixed\n"); 
else printf ("Unknown\n"); 

printf ("Physical Geometry: " 
"%u Cylinders %u Heads %u Sectors per track\n", 
diskdata [1], diskdata [3], diskdata [6]); 

#else // PRINTING_TO_CONSOLE_ALLOWED 

// nothing to do 

#endif // PRINTING_TO_CONSOLE_ALLOWED 

} 

char *ConvertToString (DWORD diskdata [256], int firstIndex, int lastIndex) 
{ 
static char string [1024]; 
int index = 0; 
int position = 0; 

// each integer has two characters stored in it backwards 
for (index = firstIndex; index <= lastIndex; index++) 
{ 
// get high byte for 1st character 
string [position] = (char) (diskdata [index] / 256); 
position++; 

// get low byte for 2nd character 
string [position] = (char) (diskdata [index] % 256); 
position++; 
} 

// end the string 
string [position] = '\0'; 

// cut off the trailing blanks 
for (index = position - 1; index > 0 && ' ' == string [index]; index--) 
string [index] = '\0'; 

return string; 
} 


long getHardDriveComputerID () 
{ 
int done = FALSE; 
//char string [1024]; 
__int64 id = 0; 

strcpy (HardDriveSerialNumber, ""); 

// this works under WinNT4 or Win2K if you have admin rights 
done = ReadPhysicalDriveInNT (); 
if ( ! done) done = ReadIdeDriveAsScsiDriveInNT (); 


// this works under Win9X and calls WINIO.DLL 
if ( ! done) done = ReadDrivePortsInWin9X (); 

if (done) 
{ 
char *p = HardDriveSerialNumber; 

//WriteConstantString ("HardDriveSerialNumber", HardDriveSerialNumber); 

// ignore first 5 characters from western digital hard drives if 
// the first four characters are WD-W 
if ( ! strncmp (HardDriveSerialNumber, "WD-W", 4)) p += 5; 
for ( ; p && *p; p++) 
{ 
if ('-' == *p) continue; 
id *= 10; 
switch (*p) 
{ 
case '0': id += 0; break; 
case '1': id += 1; break; 
case '2': id += 2; break; 
case '3': id += 3; break; 
case '4': id += 4; break; 
case '5': id += 5; break; 
case '6': id += 6; break; 
case '7': id += 7; break; 
case '8': id += 8; break; 
case '9': id += 9; break; 
case 'a': case 'A': id += 10; break; 
case 'b': case 'B': id += 11; break; 
case 'c': case 'C': id += 12; break; 
case 'd': case 'D': id += 13; break; 
case 'e': case 'E': id += 14; break; 
case 'f': case 'F': id += 15; break; 
case 'g': case 'G': id += 16; break; 
case 'h': case 'H': id += 17; break; 
case 'i': case 'I': id += 18; break; 
case 'j': case 'J': id += 19; break; 
case 'k': case 'K': id += 20; break; 
case 'l': case 'L': id += 21; break; 
case 'm': case 'M': id += 22; break; 
case 'n': case 'N': id += 23; break; 
case 'o': case 'O': id += 24; break; 
case 'p': case 'P': id += 25; break; 
case 'q': case 'Q': id += 26; break; 
case 'r': case 'R': id += 27; break; 
case 's': case 'S': id += 28; break; 
case 't': case 'T': id += 29; break; 
case 'u': case 'U': id += 30; break; 
case 'v': case 'V': id += 31; break; 
case 'w': case 'W': id += 32; break; 
case 'x': case 'X': id += 33; break; 
case 'y': case 'Y': id += 34; break; 
case 'z': case 'Z': id += 35; break; 
} 
} 
} 

// make sure no bigger than 16^7 
if (id > 268435455) id %= 268435456; 

#ifdef PRINTING_TO_CONSOLE_ALLOWED 

printf ("\nComputer ID_______________________: %d\n", id); 

#endif 

return (long) id; 
} 


/////////////////////////////////////////////////////////////////////////////
// CSView

//#ifndef	_AFX_ENABLE_INLINES
#define _SECRET_INLINE
#include "Secret.inl"
#undef	_SECRET_INLINE
//#endif


CSView::CSView( )
{
	m_tmExp	=	0;
	m_tmIns	=	0;
	m_nMaxEvaDays	=	0;

	memset( m_mStd, 0, sizeof(m_mStd) );
	memset( m_eStd, 0, sizeof(m_eStd) );
	m_mStdlen	=	-1;
	m_eStdlen	=	-1;

	memset( m_mProf, 0, sizeof(m_mProf) );
	memset( m_eProf, 0, sizeof(m_eProf) );
	m_mProflen	=	-1;
	m_eProflen	=	-1;
}

CSView::~CSView( )
{
}

#define	SF_MAX_SECRETFILE_LENGTH	0x00080000 
#define	SF_SECRETMAGIC_LENGTH				6

char szSViewFileMagic[]	=	"MGCSCF";
DWORD	dwSViewFileVersion		=	0x20000000;

BOOL CSView::Load( LPCTSTR lpszFileName )
{
	if( NULL == lpszFileName || strlen(lpszFileName) <= 0 )
		return FALSE;

	CFile file;
	try
	{
		if( file.Open( lpszFileName, CFile::modeRead | CFile::shareDenyWrite) )
		{
			DWORD dwLen = (DWORD)file.GetLength();
			if( 0 == dwLen || dwLen > SF_MAX_SECRETFILE_LENGTH )
			{
				file.Close();
				return FALSE;
			}
			
			BYTE *lpSrc = new BYTE[dwLen];
			BYTE *lpDest = new BYTE[dwLen];
			if( !(lpSrc && lpDest && dwLen == file.Read( lpSrc, dwLen )) )
			{
				if( lpSrc )	delete [] lpSrc;
				if( lpDest ) delete [] lpDest;
				file.Close();
				return FALSE;
			}
			file.Close();

			CDLocalView	des;
			des.Destroy( lpSrc, lpDest, dwLen );

			CMemFile	memfile( lpDest, dwLen );
			CArchive ar( &memfile, CArchive::load );
			TCHAR buffer[SF_SECRETMAGIC_LENGTH+1];
			memset( buffer, 0, sizeof(buffer) );
			ar.ReadString( buffer, SF_SECRETMAGIC_LENGTH );
			if( 0 != strncmp( szSViewFileMagic, buffer, SF_SECRETMAGIC_LENGTH ) )
			{
				ar.Close();
				memfile.Close();
				delete	[] lpSrc;
				delete	[] lpDest;
				return FALSE;
			}
			
			Serialize( ar );
			ar.Close();
			memfile.Close();
			delete	[] lpSrc;
			delete	[]	lpDest;
			return TRUE;
		}
	}
	catch ( CException* e )
	{
		e->Delete();
	}
	return FALSE;
}

BOOL CSView::Store( LPCTSTR lpszFileName )
{
	if( NULL == lpszFileName || strlen(lpszFileName) <= 0 )
		return FALSE;

	CFile file;
	try
	{
		CMemFile memfile( 4098 );
		CArchive ar( &memfile, CArchive::store );
		ar.WriteString( szSViewFileMagic );
		Serialize( ar );
		ar.Close();

		DWORD dwLen = (DWORD)memfile.GetLength();
		if( 0 == dwLen || dwLen > SF_MAX_SECRETFILE_LENGTH )
		{
			memfile.Close();
			return FALSE;
		}
			
		BYTE *lpSrc = new BYTE[dwLen];
		BYTE *lpDest = new BYTE[dwLen];
		memfile.SeekToBegin();
		if( !(lpSrc && lpDest && dwLen == memfile.Read( lpSrc, dwLen )) )
		{
			if( lpSrc )	delete [] lpSrc;
			if( lpDest ) delete [] lpDest;
			memfile.Close();
			return FALSE;
		}
		memfile.Close();

		CDLocalView	des;
		des.Create( lpSrc, lpDest, dwLen );

		if( file.Open( lpszFileName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite) )
		{
			file.Write( lpDest, dwLen );
			file.Close();
			delete	[] lpSrc;
			delete	[] lpDest;
			return TRUE;
		}
		delete	[] lpSrc;
		delete	[] lpDest;
	}
	catch ( CException* e )
	{
		e->Delete();
	}
	return FALSE;
}

void CSView::Serialize( CArchive &ar )
{
	if( ar.IsStoring() )
	{
		ar << dwSViewFileVersion;

		ar << m_strU;
		ar << m_strR;

		ar << m_mStdlen;
		ar << m_eStdlen;
		ar.Write( m_mStd, sizeof(m_mStd) );
		ar.Write( m_eStd, sizeof(m_eStd) );

		ar << m_mProflen;
		ar << m_eProflen;
		ar.Write( m_mProf, sizeof(m_mProf) );
		ar.Write( m_eProf, sizeof(m_eProf) );

		ar << m_tmExp.GetTime();
		ar << m_tmIns.GetTime();

		ar << m_nMaxEvaDays;

		ar << m_strAR;
		ar << m_strNSAText;
	}
	else
	{
		ar >> m_dwFileVersion;
		if( m_dwFileVersion > dwSViewFileVersion )
			return;

		ar >> m_strU;
		ar >> m_strR;

		ar >> m_mStdlen;
		ar >> m_eStdlen;
		ar.Read( m_mStd, sizeof(m_mStd) );
		ar.Read( m_eStd, sizeof(m_eStd) );

		ar >> m_mProflen;
		ar >> m_eProflen;
		ar.Read( m_mProf, sizeof(m_mProf) );
		ar.Read( m_eProf, sizeof(m_eProf) );

		time_t temp;
		ar >> temp;		m_tmExp = temp;
		ar >> temp;		m_tmIns	= temp;

		ar >> m_nMaxEvaDays;

		ar >> m_strAR;
		ar >> m_strNSAText;

		SetInsTimeIfNull();	// Write m_tmExp and m_tmIns if it is NULL.
	}
}

CSView &AfxGetSView( )
{
	static	CSView	g_sview;
	return g_sview;
}
