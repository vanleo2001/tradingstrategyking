//Object
#ifdef	_SPOBJ_INLINE	
_SPOBJ_INLINE	Object::Object()
{ SetLastError( errObject_None ); }

_SPOBJ_INLINE	Object::~Object()
{ }

#ifndef _DEBUG
_SPOBJ_INLINE	void Object::operator delete(void* p)
{ 
	::operator delete(p); 
}

_SPOBJ_INLINE	void* Object::operator new(size_t nSize)
{ 
	return ::operator new(nSize); 
}
#endif //_DEBUG

_SPOBJ_INLINE	int	Object::GetLastError( ) const
{
	return	m_nErrorStatus;
}

_SPOBJ_INLINE	void Object::SetLastError( int nErr )
{
	m_nErrorStatus	=	nErr;
	return;
}

#endif //_SPOBJ_INLINE