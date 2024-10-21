//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BkDrv project
//	
// module: handle.h
// $Revision: 16 $
// $Date: 2012-02-21 19:01:51 +0400 (Вт, 21 фев 2012) $
// description:
//	Lightweight handle management engine.

#include <ntddk.h>
#include "ntddkex.h"
#include "kdbg.h"
#include "handle.h"


static VOID HandleTableLock(
				PHANDLE_TABLE	HTable
				)
{
	KIRQL	OldIrql;

	ASSERT_HANDLE_TABLE(HTable);

	KeAcquireSpinLock(&HTable->TableLock, &OldIrql);
	HTable->OldIrql = OldIrql;
}

static VOID HandleTableUnlock(
				PHANDLE_TABLE	HTable
				)
{
	ASSERT_HANDLE_TABLE(HTable);
	KeReleaseSpinLock(&HTable->TableLock, HTable->OldIrql);
}


NTSTATUS	HandleAllocateTable(
				PHANDLE_TABLE*			pHTable,
				ULONG					ContextSize,
				HANDLE_INIT_ROUTINE		InitCallback,
				HANDLE_CLEANUP_ROUTINE	CleanupCallback
				)
{
	NTSTATUS	ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	PHANDLE_TABLE HTable = (PHANDLE_TABLE)MyAllocatePool(NonPagedPool, sizeof(HANDLE_TABLE));
	if (HTable)
	{
		ULONG i;
#ifdef _DEBUG
		HTable->Magic = HANDLE_TABLE_MAGIC;
#endif
		KeInitializeSpinLock(&HTable->TableLock);
		InitializeListHead(&HTable->GlobalListHead);

		for (i=0; i<HANDLE_ROOT_COUNT; i++)
			InitializeListHead(&HTable->KeyRoot[i]);

		HTable->Records = 0;
		HTable->ContextSize = ContextSize;
		HTable->InitCallback = InitCallback;
		HTable->CleanupCallBack = CleanupCallback;
		*pHTable = HTable;
		ntStatus = STATUS_SUCCESS;
	}	// if (HTable)

	return(ntStatus);
}


static PHANDLE_RECORD	HandleAllocate(ContextSize)
{
	PHANDLE_RECORD	pHRec = (PHANDLE_RECORD)MyAllocatePool(NonPagedPool, sizeof(HANDLE_RECORD) + ContextSize);
	if (pHRec)
	{
		RtlFillMemory(pHRec, sizeof(HANDLE_RECORD) + ContextSize, 0);
#ifdef _DEBUG
		pHRec->Magic = HANDLE_RECORD_MAGIC;
#endif
		InitializeListHead(&pHRec->Entry);
		InitializeListHead(&pHRec->GlobalEntry);
	}
	return(pHRec);
}

static VOID HandleFree(
			   PHANDLE_RECORD	pHRec
			   )
{
	ASSERT_HANDLE_RECORD(pHRec);
	MyFreePool(pHRec);
}

static VOID	HandleDeleteLocked(PHANDLE_TABLE HTable, PHANDLE_RECORD pHRec)
{
	ASSERT_HANDLE_TABLE(HTable);
	ASSERT(pHRec->RefCount == 0);

	RemoveEntryList(&pHRec->Entry);
	RemoveEntryList(&pHRec->GlobalEntry);

	if (HTable->CleanupCallBack)
		(HTable->CleanupCallBack)(pHRec->Key, (PVOID)&pHRec->Context);

	HandleFree(pHRec);
	HTable->Records -= 1;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Creates new hanlde and returns it's context. Returns FALSE if the handle already exists.
//
BOOL	HandleCreate(
			IN	PHANDLE_TABLE	HTable,		// Handle table 
			IN	HANDLE			Key,		// Handle key (value or index)
			OUT	PVOID*			pContext	// Variable to receive handle context 
			)
{
	BOOL	Ret = FALSE;
	PHANDLE_RECORD pHRec = NULL;
	PLIST_ENTRY pHead, pEntry;

	ASSERT_HANDLE_TABLE(HTable);

	HandleTableLock(HTable);
	pHead = &HTable->KeyRoot[(((ULONG_PTR)Key) >> HANDLE_SHIFT) & (HANDLE_ROOT_COUNT-1)];
	pEntry = pHead->Flink;

	while (pEntry != pHead)
	{
		pHRec = CONTAINING_RECORD(pEntry, HANDLE_RECORD, Entry);
		if (pHRec->Key >= Key)
			break;
		pEntry = pEntry->Flink;
	}


	if (!pHRec || pHRec->Key != Key)
	{
		// No such handle found, creating a new one
		if (pHRec = HandleAllocate(HTable->ContextSize))
		{
			pHRec->Key = Key;
			pHRec->HTable = HTable;
		
			if (HTable->InitCallback)
				Ret = (HTable->InitCallback)(Key, (PVOID)&pHRec->Context);
			else
				Ret = TRUE;

			if (Ret)
			{
				if (pContext)
					*pContext = &pHRec->Context;
				pHRec->RefCount = 1;
				InsertTailList(pEntry, &pHRec->Entry);
				InsertTailList(&HTable->GlobalListHead, &pHRec->GlobalEntry);
				HTable->Records += 1;
			}
			else
				HandleFree(pHRec);
		}	// if (pHRec = HandleAllocate())
	}	// if (!pHRec || pHRec->Key != Key)
	else
	{
		// Handle already exists, check if we can reuse it
		if (HTable->Flags & TF_REUSE_HANDLE)
		{
			ASSERT(pHRec->RefCount == 1);
			if (pContext)
				*pContext = &pHRec->Context;
			Ret = TRUE;
		}
	}
	

	HandleTableUnlock(HTable);

	return(Ret);
}


BOOL	HandleOpen(
					PHANDLE_TABLE	HTable, 
					HANDLE			Key,
					PVOID*			pContext
					)
{
	BOOL	Ret = FALSE;
	PHANDLE_RECORD pHRec = NULL;
	PLIST_ENTRY pHead, pEntry;

	ASSERT_HANDLE_TABLE(HTable);

	HandleTableLock(HTable);
	pHead = &HTable->KeyRoot[(((ULONG_PTR)Key) >> HANDLE_SHIFT) & (HANDLE_ROOT_COUNT-1)];
	pEntry = pHead->Flink;

	while (pEntry != pHead)
	{
		pHRec = CONTAINING_RECORD(pEntry, HANDLE_RECORD, Entry);
		if (pHRec->Key >= Key)
			break;
		pEntry = pEntry->Flink;
	}

	if (pHRec && pHRec->Key == Key)
	{
		if (pContext)
			*pContext = pHRec->Context;
		pHRec->RefCount += 1;
		Ret = TRUE;
	}

	HandleTableUnlock(HTable);

	return(Ret);
}


BOOL	HandleClose(
			PHANDLE_TABLE	HTable,
			HANDLE			Key,
			PHANDLE_RECORD	pHRec
			)
{
	BOOL Ret = FALSE;
	PLIST_ENTRY	pEntry, pHead;

	if (pHRec)
	{
		Key = pHRec->Key;
		HTable = pHRec->HTable;
	}

	ASSERT_HANDLE_TABLE(HTable);

	HandleTableLock(HTable);

	if (!pHRec)
	{
		ASSERT(Key);

		pHead = &HTable->KeyRoot[(((ULONG_PTR)Key) >> HANDLE_SHIFT) & (HANDLE_ROOT_COUNT-1)];
		pEntry = pHead->Flink;

		while (pEntry != pHead)
		{
			pHRec = CONTAINING_RECORD(pEntry, HANDLE_RECORD, Entry);
			if (pHRec->Key >= Key)
				break;
			pEntry = pEntry->Flink;
		}
	}

	if (pHRec && pHRec->Key == Key)
	{	
		ASSERT(pHRec->RefCount > 0);

		pHRec->RefCount -= 1;
		if (pHRec->RefCount == 0)
			HandleDeleteLocked(HTable, pHRec);
		Ret = TRUE;
	}
	HandleTableUnlock(HTable);

	return(Ret);
}


VOID	HandleReleaseTable(
				PHANDLE_TABLE	HTable
				)
{
	ULONG		i;
	PLIST_ENTRY	pHead, pEntry;
	PHANDLE_RECORD	pHRec;
	ASSERT_HANDLE_TABLE(HTable);

	HandleTableLock(HTable);

	for (i=0; i<HANDLE_ROOT_COUNT; i++)
	{
		pHead = &HTable->KeyRoot[i];
		pEntry = pHead->Flink;
		while (pEntry != pHead)
		{
			pHRec = CONTAINING_RECORD(pEntry, HANDLE_RECORD, Entry);
			pEntry = pEntry->Flink;
			ASSERT(HTable->Records > 0);
			ASSERT(pHRec->RefCount == 1);
			pHRec->RefCount -= 1;
			HandleDeleteLocked(HTable, pHRec);
		}
	}

	ASSERT(HTable->Records == 0);	
	HandleTableUnlock(HTable);

#if _DEBUG
	HTable->Magic = 0;
#endif
	MyFreePool(HTable);
}



BOOL	HandleGet(
				  PHANDLE_TABLE	HTable, 
				  ULONG			Index,
	    		  PVOID*		pContext
				  )
{
	BOOL	Ret = FALSE;
	ULONG	i = 0;
	PLIST_ENTRY	pHead, pEntry;
	PHANDLE_RECORD	pHRec;
	ASSERT_HANDLE_TABLE(HTable);

	HandleTableLock(HTable);
	pEntry = HTable->GlobalListHead.Flink;
	while(Index && pEntry != &HTable->GlobalListHead)
	{
		Index -= 1;
		pEntry = pEntry->Flink;		
	}

	if (pEntry != &HTable->GlobalListHead)
	{
		pHRec = CONTAINING_RECORD(pEntry, HANDLE_RECORD, GlobalEntry);
		if (pContext)
			*pContext = pHRec->Context;
		pHRec->RefCount += 1;
		Ret = TRUE;
	}
	HandleTableUnlock(HTable);
	return(Ret);
}

