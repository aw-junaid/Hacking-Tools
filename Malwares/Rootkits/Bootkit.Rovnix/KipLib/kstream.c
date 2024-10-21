#include "project.h"
#include "kstream.h"



// ---- Linked buffer ------------------------------------------------------------------------------------------------------------

PLBUFFER	LBufferAllocate(ULONG	Size)
{
	PLBUFFER	lBuffer = (PLBUFFER)ExAllocatePoolWithTag(NonPagedPool, (sizeof(LBUFFER) + Size), TAG_LBUFFER);
	if (lBuffer)
	{
#if	DBG
		lBuffer->Magic = LBUFFER_MAGIC;
#endif
		InitializeListHead(&lBuffer->Entry);
		lBuffer->Size = Size;
	}
	return(lBuffer);
}

VOID	LBufferFree(PLBUFFER lBuffer, PCHAR Data)
{
	if (!lBuffer)
		lBuffer = CONTAINING_RECORD(Data, LBUFFER, Data);

	ASSERT_LBUFFER(lBuffer);
	ExFreePool(lBuffer);
}


//---- Kernel stream --------------------------------------------------------------------------------------------------------------

static	VOID KStreamLock(PKSTREAM	pStream)
{
	ASSERT_KSTREAM(pStream);
	KeWaitForSingleObject(&pStream->Lock, Executive, KernelMode, FALSE, NULL);
}


static	VOID KStreamUnlock(PKSTREAM	pStream)
{
	ASSERT_KSTREAM(pStream);
	KeReleaseMutex(&pStream->Lock, FALSE);
}


PKSTREAM	KStreamAllocate(VOID)
{
	PKSTREAM	pStream = ExAllocatePoolWithTag(NonPagedPool, sizeof(KSTREAM), TAG_KSTREAM);
	if (pStream)
	{
		pStream->Position = 0;
		pStream->Length = 0;
		InitializeListHead(&pStream->BuffersListHead);
		KeInitializeEvent(&pStream->DataReadyEvent, NotificationEvent, FALSE);
		KeInitializeMutex(&pStream->Lock, 0);
		ExInitializeNPagedLookasideList(&pStream->Buffers, NULL, NULL, 0, (sizeof(LBUFFER) + KSTREAM_BUFFER_SIZE), TAG_KSTREAM, 0);
#if DBG
		pStream->Magic = KSTREAM_MAGIC;
#endif
	}
	return(pStream);

}

VOID	KStreamRelease(PKSTREAM	pStream)
{
	ASSERT_KSTREAM(pStream);
	ASSERT(pStream->Length == 0);
	ASSERT(IsListEmpty(&pStream->BuffersListHead));

	ExDeleteNPagedLookasideList(&pStream->Buffers);
	ExFreePool(pStream);
}

BOOLEAN	KStreamWrite(PKSTREAM pStream, PCHAR Data, ULONG Length)
{
	BOOLEAN	Ret = FALSE;
	ULONG	bRead = 0, bSize, lSize;
	PLBUFFER	lBuffer;
	ULONG	OldLength = 0;

	ASSERT_KSTREAM(pStream);
	ASSERT(Length);

	KStreamLock(pStream);

	ASSERT(pStream->Position == 0 || pStream->Length != 0);
	OldLength = pStream->Length;

	lSize = (pStream->Position + pStream->Length);
	if (lSize >= KSTREAM_BUFFER_SIZE)
		lSize %= KSTREAM_BUFFER_SIZE;

	while(Length)
	{
		bSize = KSTREAM_BUFFER_SIZE - lSize;
		if (bSize == KSTREAM_BUFFER_SIZE)
		{
			// Allocating new buffer
			if (!(lBuffer = ExAllocateFromNPagedLookasideList(&pStream->Buffers)))
				break;
#if DBG
			memset(lBuffer, 0xcc,  (sizeof(LBUFFER) + KSTREAM_BUFFER_SIZE));
#endif
			InitializeListHead(&lBuffer->Entry);
			InsertTailList(&pStream->BuffersListHead, &lBuffer->Entry);
		}
		else
		{
			ASSERT(!IsListEmpty(&pStream->BuffersListHead));
			// Appending existing buffer
			lBuffer = CONTAINING_RECORD(pStream->BuffersListHead.Blink, LBUFFER, Entry);
		}

		if (Length < bSize)
			bSize = Length;

		ASSERT((bSize + lSize) <= KSTREAM_BUFFER_SIZE);

		memcpy((PCHAR)&lBuffer->Data + lSize, Data + bRead, bSize);
		Length -= bSize;
		pStream->Length += bSize;
		bRead += bSize;
		lSize = 0;
	}	// while(Length)

	if (Length == 0)
		Ret = TRUE;

	if (OldLength == 0)
		KeSetEvent(&pStream->DataReadyEvent, 0, FALSE);	

	KStreamUnlock(pStream);
	return(Ret);
}

ULONG	KStreamRead(PKSTREAM pStream, PCHAR Data, ULONG Length)
{
	PLBUFFER	lBuffer;	// current buffer
	ULONG		bRead = 0;	// bytes already read
	ULONG		bSize;		// bytes we can read from current buffer
	ULONG		bOffset;	// offset within the buffer to start reading from

	ASSERT_KSTREAM(pStream);
	KStreamLock(pStream);

	bOffset = pStream->Position;
	ASSERT(bOffset == 0 || pStream->Length != 0);

	while((Length) && (pStream->Length))
	{
		ASSERT(!IsListEmpty(&pStream->BuffersListHead));

		// Determine how many bytes we can copy now
		bSize = KSTREAM_BUFFER_SIZE - bOffset;
		if (bSize > pStream->Length)
			bSize = pStream->Length;
		if (bSize > Length)
			bSize = Length;

		// Take the first buffer from the list
		lBuffer = CONTAINING_RECORD(pStream->BuffersListHead.Flink, LBUFFER, Entry);
		ASSERT(bSize <= pStream->Length);

		memcpy(Data + bRead, (PCHAR)&lBuffer->Data + bOffset, bSize);
		Length -= bSize;
		pStream->Length -= bSize;
		bRead += bSize;

		if ((bOffset + bSize) == KSTREAM_BUFFER_SIZE || pStream->Length == 0)
		{
			// Current buffer is empty
			RemoveEntryList(&lBuffer->Entry);
			ExFreeToNPagedLookasideList(&pStream->Buffers, lBuffer);
			pStream->Position = 0;
		}
		else
		{
			ASSERT(pStream->Length == 0 || Length == 0);
			pStream->Position = bOffset + bSize;
		}

		bOffset = 0;
	}

	if (pStream->Length == 0)
	{
		ASSERT(IsListEmpty(&pStream->BuffersListHead));
		ASSERT(pStream->Position == 0);
		KeResetEvent(&pStream->DataReadyEvent);
	}
	else
	{
		ASSERT(!IsListEmpty(&pStream->BuffersListHead));
	}

	KStreamUnlock(pStream);

	return(bRead);
}


ULONG	KStreamGetLength(PKSTREAM	pStream)
{
	ULONG register Length;

	ASSERT_KSTREAM(pStream);
	Length = pStream->Length;

	return(Length);
}


VOID	KStreamClear(PKSTREAM	pStream)
{
	ASSERT_KSTREAM(pStream);

	KStreamLock(pStream);

//	ASSERT(pStream->Length == 0);

	while (!IsListEmpty(&pStream->BuffersListHead))
	{
		PLBUFFER lBuffer = CONTAINING_RECORD(pStream->BuffersListHead.Flink, LBUFFER, Entry);

		RemoveEntryList(&lBuffer->Entry);
		ExFreeToNPagedLookasideList(&pStream->Buffers, lBuffer);
	}

	pStream->Length = 0;
	pStream->Position = 0;

	// Seting DataReadyEvent to satisfy all waiters
	KeSetEvent(&pStream->DataReadyEvent, 0, FALSE);	

	// Reseting the event to indicate that stream is clean
	KeResetEvent(&pStream->DataReadyEvent);

	KStreamUnlock(pStream);
}

#ifdef _TDI_SUPPORT

// ---- Datagram  collector --------------------------------------------------------------------------------------------------------



VOID DStreamLock(PDSTREAM	pStream)
{
	KIRQL	Irql;

	ASSERT_DSTREAM(pStream);

	KeAcquireSpinLock(&pStream->Lock, &Irql);
	pStream->OldIrql = Irql;
}


VOID DStreamUnlock(PDSTREAM	pStream)
{
	ASSERT_DSTREAM(pStream);
	ASSERT_LOCKED(&pStream->Lock);
	KeReleaseSpinLock(&pStream->Lock, pStream->OldIrql);
}


PDSTREAM	DStreamAllocate(VOID)
{
	PDSTREAM	pStream = ExAllocatePoolWithTag(NonPagedPool, sizeof(DSTREAM), TAG_DSTREAM);
	if (pStream)
	{
#if	DBG
		pStream->Magic = DSTREAM_MAGIC;
#endif
		InitializeListHead(&pStream->BufferListHead);
		pStream->Count = 0;
		pStream->Lock = 0;
	}
	return(pStream);
}

VOID	DStreamRelease(PDSTREAM	pStream)
{
	ASSERT_DSTREAM(pStream);
	ASSERT(IsListEmpty(&pStream->BufferListHead));

	ExFreePool(pStream);
}


BOOLEAN	DStreamPush(PDSTREAM pStream, PCHAR	Data, ULONG	Length)
{
	PLBUFFER	lBuffer;
	BOOLEAN		Ret = FALSE;

	ASSERT_DSTREAM(pStream);
	ASSERT(Length);

	if (lBuffer = LBufferAllocate(Length))
	{
		memcpy((PCHAR)&lBuffer->Data, Data, Length);
		DStreamLock(pStream);
		InsertTailList(&pStream->BufferListHead, &lBuffer->Entry);
		pStream->Count += 1;
		DStreamUnlock(pStream);
		Ret = TRUE;
	}	// if (lBuffer = LBufferAllocate(Length))
	return(Ret);
}

BOOLEAN	DStreamPop(PDSTREAM pStream, PCHAR	Data, PULONG pLength)
{
	ULONG	Size = 0;
	BOOLEAN	Ret = FALSE;
	PLIST_ENTRY	pEntry;
	PLBUFFER	lBuffer = NULL;

	ASSERT_DSTREAM(pStream);
	if (pStream->Count)
	{
		DStreamLock(pStream);
		pEntry = pStream->BufferListHead.Flink;
		if (pEntry != &pStream->BufferListHead)
		{
			lBuffer = CONTAINING_RECORD(pEntry, LBUFFER, Entry);
			ASSERT_LBUFFER(lBuffer);
			ASSERT(pStream->Count);

			Size = lBuffer->Size;
			if (Size <= *pLength)
			{
				RemoveEntryList(&lBuffer->Entry);
				pStream->Count -= 1;
			}
			else
				lBuffer = NULL;
		}	// if (pEntry != &pStream->BufferListHead)
		else
			ASSERT(pStream->Count == 0);

		DStreamUnlock(pStream);

		if (lBuffer)
		{
			ASSERT(Size > 0);
			ASSERT(Size <= *pLength);
			memcpy(Data, &lBuffer->Data, Size);
			ExFreePool(lBuffer);
			Ret = TRUE;
		}	// if (lBuffer)
	}	// if (pStreamCount)
	*pLength = Size;

	return(Ret);
}


PCHAR	DStreamNext(PDSTREAM pStream, PCHAR	Buffer)
{
	PLBUFFER	lBuffer;
	PCHAR		Next = NULL;

	ASSERT_DSTREAM(pStream);
	ASSERT_LOCKED(&pStream->Lock);

	if (Buffer)
	{
		lBuffer = CONTAINING_RECORD(Buffer, LBUFFER, Data);
		lBuffer = CONTAINING_RECORD(lBuffer->Entry.Flink, LBUFFER, Entry);
		Next = (PCHAR)&lBuffer->Data;
	}
	else
	{
		if (!IsListEmpty(&pStream->BufferListHead))
		{
			lBuffer = CONTAINING_RECORD(pStream->BufferListHead.Flink, LBUFFER, Entry);
			Next = (PCHAR)&lBuffer->Data;
		}
	}	// if (Buffer)

	return(Next);
}


VOID	DStreamPushBuffer(PDSTREAM	pStream, PLBUFFER lBuffer)
{
	ASSERT_DSTREAM(pStream);
	ASSERT_LBUFFER(lBuffer);

	DStreamLock(pStream);
	InsertTailList(&pStream->BufferListHead, &lBuffer->Entry);
	pStream->Count += 1;
	DStreamUnlock(pStream);
}


ULONG	DStreamGetLength(PDSTREAM	pStream)
{
	ULONG register Length;

	ASSERT_DSTREAM(pStream);
	Length = pStream->Count;

	return(Length);
}


VOID	DStreamClear(PDSTREAM	pStream)
{
	ASSERT_DSTREAM(pStream);

	DStreamLock(pStream);
	while (!IsListEmpty(&pStream->BufferListHead))
	{
		PLBUFFER lBuffer = CONTAINING_RECORD(pStream->BufferListHead.Flink, LBUFFER, Entry);

		RemoveEntryList(&lBuffer->Entry);
		ExFreePool(lBuffer);
		pStream->Count -= 1;
	}
	ASSERT(pStream->Count == 0);
	DStreamUnlock(pStream);
}

#endif //_TDI_SUPPORT