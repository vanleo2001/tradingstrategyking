/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		operator new;
		operator delete;
		other memory method.
*/
#include	"StdAfx.h"
#include	"SPLock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// WARNING: Debug版对内存的处理，不支持线程同步，所以可能会出问题，但是不影响Release版

#if	defined(_DEBUG) && !defined(_AFX)

void* operator new(size_t nSize)
{
	void* p = SP_AllocMemoryDebug(nSize, FALSE, NULL, 0);

	if (p == NULL)
	{
		SP_TRACE1("::operator new(%u) failed - throwing exception\n", nSize);
		SP_ASSERT( FALSE );
	}
	return p;
}

void* operator new(size_t nSize, LPCSTR lpszFileName, int nLine)
{
	void* p = SP_AllocMemoryDebug(nSize, FALSE, lpszFileName, nLine);
	if (p == NULL)
	{
		SP_TRACE1("::operator new(%u) failed - throwing exception\n", nSize);
		SP_ASSERT( FALSE );
	}
	return p;
}

#if	_MSC_VER>=1200
void operator delete(void *pMem, LPCSTR lpszFileName, int nLine )
{
	SP_ASSERT( SP_CheckMemory() );

	SP_FreeMemoryDebug(pMem, FALSE);
}
#endif

void operator delete(void* pbData)
{
	SP_FreeMemoryDebug(pbData, FALSE);
}

#endif

#ifdef	_DEBUG

#define	SIZE_T_MAX	UINT_MAX
static LONG  lTotalAlloc;
static LONG  lCurAlloc;
static LONG  lMaxAlloc;

static BOOL bTrackingOn = TRUE;
static LONG  lRequestLast = 0;
#define lNotTracked 0

struct	CBlockHeader;

static struct CBlockHeader*  pFirstBlock = NULL;
static CSPMutex g_mutexBlockChain;

static char  szFree[] = "Free";
static char  szObject[] = "Object";
static char  szNonObject[] = "Non-Object";
static char  szDamage[] = "Damage";

#define nNoMansLandSize     4
#define bNoMansLandFill     0xFD
#define bDeadLandFill       0xDD
#define bCleanLandFill      0xCD

struct CBlockHeader
{
	struct CBlockHeader* pBlockHeaderNext;
	struct CBlockHeader* pBlockHeaderPrev;
	LPCSTR              lpszFileName;
	int                 nLine;
	size_t              nDataSize;
	enum CSPMemoryState::blockUsage use;
	LONG                lRequest;
	BYTE                gap[nNoMansLandSize];
	BYTE* pbData()
		{ return (BYTE*) (this + 1); }
};

static LPCSTR  blockUseName[CSPMemoryState::nBlockUseMax] =
	{ szFree, szObject, szNonObject };



static LONG _afxBreakAlloc = -1;

void*  SP_AllocMemoryDebug(size_t nSize, BOOL bIsObject, LPCSTR lpszFileName, int nLine)
{
	SP_ASSERT(nSize > 0);

	LONG    lRequest;
	lRequest = bTrackingOn ? ++lRequestLast : lNotTracked;

	if (nSize > (size_t)SIZE_T_MAX - nNoMansLandSize - sizeof(CBlockHeader))
	{
		SP_TRACE1("Error: memory allocation: tried to allocate %u bytes\n", nSize);
	}

	lTotalAlloc += nSize;
	lCurAlloc += nSize;

	if (lCurAlloc > lMaxAlloc)
		lMaxAlloc = lCurAlloc;

	struct CBlockHeader* p = (struct CBlockHeader*)
	   malloc(sizeof(CBlockHeader) + nSize + nNoMansLandSize);

	if (p == NULL)
		return NULL;

	CSPMutex::Scoped locker(g_mutexBlockChain);
	if (pFirstBlock)
		pFirstBlock->pBlockHeaderPrev = p;

	p->pBlockHeaderNext = pFirstBlock;
	p->pBlockHeaderPrev = NULL;
	p->lpszFileName = lpszFileName;
	p->nLine = nLine;
	p->nDataSize = nSize;
	p->use = bIsObject ? CSPMemoryState::objectBlock : CSPMemoryState::bitBlock;
	p->lRequest = lRequest;

	memset(p->gap, bNoMansLandFill, nNoMansLandSize);
	memset(p->pbData() + nSize, bNoMansLandFill, nNoMansLandSize);

	memset(p->pbData(), bCleanLandFill, nSize);

	pFirstBlock = p;
	return (void*)p->pbData();
}


void  SP_FreeMemoryDebug(void* pbData, BOOL bIsObject)
{
	if (pbData == NULL)
		return;

	struct CBlockHeader* p = ((struct CBlockHeader*) pbData)-1;

	SP_ASSERT(p->use == (bIsObject ? CSPMemoryState::objectBlock
		: CSPMemoryState::bitBlock));

	lCurAlloc -= p->nDataSize;

	p->use = CSPMemoryState::freeBlock;

	CSPMutex::Scoped locker(g_mutexBlockChain);
	if (p->pBlockHeaderNext)
		p->pBlockHeaderNext->pBlockHeaderPrev = p->pBlockHeaderPrev;

	if (p->pBlockHeaderPrev)
	{
		p->pBlockHeaderPrev->pBlockHeaderNext = p->pBlockHeaderNext;
	}
	else
	{
		SP_ASSERT(pFirstBlock == p);
		if( pFirstBlock == p )
			pFirstBlock = p->pBlockHeaderNext;
	}

	memset(p, bDeadLandFill,
		sizeof(CBlockHeader) + p->nDataSize + nNoMansLandSize);
	free(p);
}

void *SP_ReAllocMemoryDebug( void *pbData, size_t nSize, BOOL bObject, LPCSTR lpszFileName, int nLine )
{
	void	*p;
	struct CBlockHeader* p1 = ((struct CBlockHeader*) pbData)-1;

	p	=	SP_AllocMemoryDebug( nSize, bObject, lpszFileName, nLine );
	if( NULL == p )
	{
		SP_TRACE0("alloc Memory fail!.");
		return	pbData;
	}
	memcpy(p, pbData, min( nSize, p1->nDataSize) );
	SP_FreeMemoryDebug( pbData, FALSE );
	return	p;
}

static BOOL CheckBytes(BYTE* pb, WORD bCheck, size_t nSize)
{
	BOOL bOkay = TRUE;
	while (nSize--)
	{
		if (*pb++ != bCheck)
		{
			SP_TRACE3("memory check error at $%08lX = $%02X, should be $%02X\n",
				(BYTE *) (pb-1),*(pb-1), bCheck);
			bOkay = FALSE;
		}
	}
	return bOkay;
}


BOOL SP_CheckMemory()
{
	CSPMutex::Scoped locker(g_mutexBlockChain);

	BOOL    allOkay = TRUE;

	struct CBlockHeader* p;
	for (p = pFirstBlock; p != NULL; p = p->pBlockHeaderNext)
	{
		BOOL okay = TRUE;
		LPCSTR blockUse;

		if (p->use >= 0 && p->use < CSPMemoryState::nBlockUseMax)
			blockUse = blockUseName[p->use];
		else
			blockUse = szDamage;

		if (!CheckBytes(p->gap, bNoMansLandFill, nNoMansLandSize))
		{
			SP_TRACE2("DAMAGE: before %Fs block at $%08lX\n", blockUse,
				(BYTE *) p->pbData());
			okay = FALSE;
		}

		if (!CheckBytes(p->pbData() + p->nDataSize, bNoMansLandFill,
		  nNoMansLandSize))
		{
			SP_TRACE2("DAMAGE: after %Fs block at $%08lX\n", blockUse,
				(BYTE *) p->pbData());
			okay = FALSE;
		}

		if (p->use == CSPMemoryState::freeBlock &&
		  !CheckBytes(p->pbData(), bDeadLandFill, p->nDataSize))
		{
			SP_TRACE1("DAMAGE: on top of Free block at $%08lX\n",
				(BYTE *) p->pbData());
			okay = FALSE;
		}

		if (!okay)
		{
			if (p->lpszFileName != NULL)
				SP_TRACE3("%Fs allocated at file %Fs(%d)\n", blockUse,
					p->lpszFileName, p->nLine);

			SP_TRACE3("%Fs located at $%08lX is %u bytes long\n", blockUse,
				(BYTE *) p->pbData(), p->nDataSize);
			allOkay = FALSE;
		}
	}
	return allOkay;
}


void  SP_ExitCheckMemory()
{
	CSPMutex::Scoped locker(g_mutexBlockChain);

	struct CBlockHeader* p;
	if( pFirstBlock )
	{
		SP_TRACE0("----------------------------!!!Warning!!!-----------------------\n");
		SP_TRACE0("Detect memory leaked!\n");
	}

	for (p = pFirstBlock; p != NULL; p = p->pBlockHeaderNext)
	{
		LPCSTR blockUse;

		if (p->use >= 0 && p->use < CSPMemoryState::nBlockUseMax)
			blockUse = blockUseName[p->use];
		else
			blockUse = szDamage;

		if (p->lpszFileName != NULL)
		{
			SP_TRACE3("%Fs allocated at file %Fs(%d).", blockUse,
				p->lpszFileName, p->nLine);
		}
		SP_TRACE3("%Fs located at $%08lX is %u bytes long.", blockUse,
			(BYTE *) p->pbData(), p->nDataSize);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CSPMemoryState

CSPMemoryState::CSPMemoryState()
{
	m_pBlockHeader = NULL;
}

// fills 'this' with the difference, returns TRUE if significant
BOOL CSPMemoryState::Difference(const CSPMemoryState& oldState,
		const CSPMemoryState& newState)
{
	BOOL bSignificantDifference = FALSE;
	for (int use = 0; use < CSPMemoryState::nBlockUseMax; use++)
	{
		m_lSizes[use] = newState.m_lSizes[use] - oldState.m_lSizes[use];
		m_lCounts[use] = newState.m_lCounts[use] - oldState.m_lCounts[use];

		if ((m_lSizes[use] != 0 || m_lCounts[use] != 0) &&
		  use != CSPMemoryState::freeBlock)
			bSignificantDifference = TRUE;
	}
	m_lHighWaterCount = newState.m_lHighWaterCount - oldState.m_lHighWaterCount;
	m_lTotalCount = newState.m_lTotalCount - oldState.m_lTotalCount;

	return bSignificantDifference;
}


void CSPMemoryState::DumpStatistics() const
{
	for (int use = 0; use < CSPMemoryState::nBlockUseMax; use++)
	{
		SP_TRACE3("%ld bytes in %ld %Fs Blocks\n", m_lSizes[use],
			m_lCounts[use], blockUseName[use]);
	}

	SP_TRACE1("Largest number used: %ld bytes\n", m_lHighWaterCount);
	SP_TRACE1("Total allocations: %ld bytes\n", m_lTotalCount);
}

// -- fill with current memory state
void CSPMemoryState::Checkpoint()
{
	CSPMutex::Scoped locker(g_mutexBlockChain);

	m_pBlockHeader = pFirstBlock;
	for (int use = 0; use < CSPMemoryState::nBlockUseMax; use++)
		m_lCounts[use] = m_lSizes[use] = 0;

	struct CBlockHeader* p;
	for (p = pFirstBlock; p != NULL; p = p->pBlockHeaderNext)
	{
		if (p->lRequest == lNotTracked)
		{
			// ignore it for statistics
		}
		else if (p->use >= 0 && p->use < CSPMemoryState::nBlockUseMax)
		{
			m_lCounts[p->use]++;
			m_lSizes[p->use] += p->nDataSize;
		}
		else
		{
			SP_TRACE1("Bad memory block found at $%08lX\n", (BYTE*) p);
		}
	}

	m_lHighWaterCount = lMaxAlloc;
	m_lTotalCount = lTotalAlloc;
}

// Dump objects created after this memory state was checkpointed
// Will dump all objects if this memory state wasn't checkpointed
// Dump all objects, report about non-objects also
// List request number in {}
void CSPMemoryState::DumpAllObjectsSince() const
{
	CSPMutex::Scoped locker(g_mutexBlockChain);
	
	struct CBlockHeader* pBlockStop;

	SP_TRACE0("<Spring> Dumping objects ->\n");
	pBlockStop = m_pBlockHeader;

	struct CBlockHeader* p;
	for (p = pFirstBlock; p != NULL && p != pBlockStop;
		p = p->pBlockHeaderNext)
	{
		char sz[255];

		if (p->lRequest == lNotTracked)
		{
			// ignore it for dumping
		}
		else if (p->use == CSPMemoryState::objectBlock)
		{
			Object* pObject = (Object*) p->pbData();

			SP_TRACE1("{%ld} ", p->lRequest);
			if (p->lpszFileName != NULL)
			{
				if (!SP_IsValidAddress( (void*)(p->lpszFileName), 1, FALSE))
					sprintf(sz, "#File Error#(%d) : ", p->nLine);
				else
					sprintf(sz, "%Fs(%d) : ", p->lpszFileName, p->nLine);
				SP_OutputDebugString( (LPCSTR)sz );
			}

#if !defined(_NEARDATA) || defined(_M_I86MM)
			// with large vtable, verify that object and vtable are valid
			if (!SP_IsValidAddress(*(void FAR**)pObject, sizeof(void FAR*), FALSE) )
#else
			// with near vtable, verify that object and vtable are valid
			if (!SP_IsValidAddress(*(void**)pObject, sizeof(void*), FALSE) )
#endif
			{
				// short form for trashed objects
				sprintf(sz, "an invalid object at $%08lX, %u bytes long\n",
					(BYTE FAR*) p->pbData(), p->nDataSize);
				SP_OutputDebugString( (LPCSTR)sz );
			}
			else 
			{
				// short form
				sprintf(sz, "a object at $%08lX, %u bytes long\n",
					(BYTE FAR*) p->pbData(), p->nDataSize);
				SP_OutputDebugString( (LPCSTR)sz );
				pObject->Dump();
			}
		}
		else if (p->use == CSPMemoryState::bitBlock)
		{
			SP_TRACE1("{%ld} ", p->lRequest);
			if (p->lpszFileName != NULL)
			{
				if (NULL == p->lpszFileName)
					sprintf(sz, "#File Error#(%d) : ", p->nLine);
				else
					sprintf(sz, "%Fs(%d) : ", p->lpszFileName, p->nLine);
				SP_OutputDebugString( (LPCSTR)sz );
			}

			sprintf(sz, "non-object block at $%08lX, %u bytes long\n",
				(BYTE *) p->pbData(), p->nDataSize);
			SP_OutputDebugString( (LPCSTR)sz );
		}
	}
	SP_TRACE0("<Spring> Object dump complete.\n\n");
}


#ifdef SP_WINDOWS // SP_EXITDUMP comes too late for non-Windows app termination

class SP_EXITDUMP
{
public:
	~SP_EXITDUMP();
};

SP_EXITDUMP::~SP_EXITDUMP()
{

	// only dump leaks when there are in fact leaks
	CSPMemoryState msNow;
	msNow.Checkpoint();

	if (msNow.m_lCounts[CSPMemoryState::objectBlock] != 0 ||
		msNow.m_lCounts[CSPMemoryState::bitBlock] != 0)
	{
		// dump objects since empty state since difference detected.
		SP_TRACE0("----------------------------!!!Warning!!!-----------------------\n");
		SP_TRACE0("-<Spring>-  Detected memory leaks!\n");
		CSPMemoryState msEmpty;   // construct empty memory state object
		msEmpty.DumpAllObjectsSince();
	}

}

static SP_EXITDUMP SP_ExitDump;

#endif //_WINDOWS

#else

static void*  SP_AllocMemoryDebug(size_t nSize, BOOL bIsObject, LPCSTR lpszFileName, int nLine)
{
	return	malloc( nSize );
}

static void  SP_FreeMemoryDebug(void* pbData, BOOL bIsObject)
{
	free( pbData );
}

#endif//_DEBUG