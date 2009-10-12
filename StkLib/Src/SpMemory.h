/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		for C++:
			#include	"StdAfx.h"
			#define		new	DEBUG_NEW

		for	C:
			#include	"StdAfx.h"
			replace	<malloc>  with <SP_Alloc>
					<free>    with <SP_Free>
					<realloc> with <SP_ReAlloc>
*/

#ifndef	__SP_MEMORY_H__
#define	__SP_MEMORY_H__

#ifdef	_DEBUG

	struct CSPMemoryState
	{
		enum blockUsage
		{
			freeBlock,
			objectBlock,
			bitBlock,
			nBlockUseMax
		};
		struct CBlockHeader* m_pBlockHeader;
		LONG m_lCounts[nBlockUseMax];
		LONG m_lSizes[nBlockUseMax];
		LONG m_lHighWaterCount;
		LONG m_lTotalCount;

		CSPMemoryState();

	// Operations
		void Checkpoint();  // fill with current state
		BOOL Difference(const CSPMemoryState& oldState,
						const CSPMemoryState& newState);  // fill with difference

		// Output to afxDump
		void DumpStatistics() const;
		void DumpAllObjectsSince() const;
	};



void*	_cdecl operator new(size_t nSize, LPCSTR lpszFileName, int nLine);
#if	_MSC_VER >= 1200
	void	_cdecl operator delete(void *pMem, LPCSTR lpszFileName, int nLine );
#endif	//	_MSC_VER

#define DEBUG_NEW	new(THIS_FILE, __LINE__)

void*	SP_AllocMemoryDebug(size_t nSize, BOOL bIsObject, LPCSTR lpszFileName, int nLine);
void	SP_FreeMemoryDebug(void* pbData, BOOL bIsObject);
void*	SP_ReAllocMemoryDebug( void *pbData, size_t nSize, BOOL bObject, LPCSTR lpszFileName, int nLine );
BOOL	SP_CheckMemory();

#define	SP_Alloc(nSize)  SP_AllocMemoryDebug(nSize, FALSE, __FILE__, __LINE__ )
#define	SP_Free(pbData)  SP_FreeMemoryDebug(pbData, FALSE );
#define	SP_ReAlloc(pbData, nSize) \
		SP_ReAllocMemoryDebug(pbData, nSize, FALSE, __FILE__, __LINE__ )

#else

#define	SP_Alloc	malloc
#define	SP_Free		free
#define	SP_ReAlloc	realloc

#define DEBUG_NEW new
#define SP_CheckMemory() TRUE
#define SP_IsMemoryBlock(p, nBytes) TRUE

#endif // _DEBUG

#endif //__SP_MEMORY_H__