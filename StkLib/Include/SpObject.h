/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	Object
*/
#if	!defined( __SP_OBJECT_H__ )
#define	__SP_OBJECT_H__

	class STKLIB_API Object
	{
	public:
		virtual	~Object();

		void*	operator new( size_t nSize );
		void*	operator new( size_t nSize, void *p);
		void	operator delete( void *p );

		#if	_MSC_VER>=1200
			void	operator delete(void* p, void* pPlace);
		#endif

#ifdef _DEBUG
		void*	operator new(size_t nSize, LPCSTR lpszFileName, int nLine);
	#if _MSC_VER >= 1200
        void PASCAL operator delete(void *p, LPCSTR lpszFileName, int nLine);
	#endif
#endif

// can't make Object instance directly.
	protected:
		Object();

	public:
		virtual	void	AssertValid( )	const;
		virtual	void	Dump( ) const;

////////////////////////////////////////////////////////////////
// Error status, provide record error status
	public:
		enum
		{
			#define	_SPRING(errCause)	errCause,
			#include	"SpObjErr.inl"
			#undef	_SPRING
			errorEnd
		};

		int			GetLastError( ) const;
		LPCSTR		GetLastErrorString( ) const;
	protected:
		void		SetLastError( int nErr );
	private:
		int			m_nErrorStatus;
	};

#ifdef	_SP_ENABLE_INLINES
#define	_SPOBJ_INLINE	inline
#include	"SpObject.inl"
#undef	_SPOBJ_INLINE
#endif

#endif // __SP_OBJECT_H__

