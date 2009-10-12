/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CSPMutex
		class	CSPMutex::Scoped
*/

#ifndef	__SP_LOCK_H__
#define	__SP_LOCK_H__

#ifdef SP_WINDOWS

class CSPMutex
{
	HANDLE	m_hObject;
public:
	CSPMutex( )
	{
		m_hObject = ::CreateMutex( NULL, FALSE, NULL);
	}
	~CSPMutex ()
	{
		if (m_hObject != NULL)
		{
			::CloseHandle(m_hObject);
			m_hObject = NULL;
		}
	}
	BOOL Lock()
	{
		if (::WaitForSingleObject(m_hObject, INFINITE) == WAIT_OBJECT_0)
			return TRUE;
		return FALSE;
	}

	BOOL UnLock()
	{
		return ::ReleaseMutex(m_hObject);
	}

	class Scoped
	{
		CSPMutex *mx;
	public:
		Scoped(CSPMutex& m) : mx(&m) { mx->Lock(); }
		~Scoped () { mx->UnLock(); }
	};
};

#else

#include <pthread.h>

class CSPMutex
{
	pthread_mutex_t mutex;
public:
	CSPMutex( )
	{
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&mutex, &attr);
		pthread_mutexattr_destroy(&attr);
	}
	~CSPMutex ()
	{
		while (pthread_mutex_destroy(&mutex) == EBUSY) { Lock(); UnLock(); }
	}
	BOOL Lock()   { pthread_mutex_lock(&mutex); return true; }
	BOOL UnLock() { pthread_mutex_unlock(&mutex); return true; }

	class Scoped
	{
		CSPMutex *mx;
	public:
		Scoped(CSPMutex& m) : mx(&m) { mx->Lock(); }
		~Scoped () { mx->UnLock(); }
	};
};

#endif // SP_WINDOWS

#endif //__SP_LOCK_H__
