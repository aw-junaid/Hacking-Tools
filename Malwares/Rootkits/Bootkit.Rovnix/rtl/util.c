#include "project.h"
#include "debug.h"

//***********************************************************************************
// Name: UF_LockUserBuffer
//
// Routine Description: 
//
//		Locks user buffer in memory and maps it into system space.
//
// Return Value: 
//
//		Operation status.
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	UF_LockUserBuffer(
		IN PVOID UserBuffer,
		IN ULONG UserBufferSize,
		IN LOCK_OPERATION LockOperation,
		OUT PMDL *pMdl,
		OUT PVOID* SystemBuffer
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	PVOID LockedAddress = NULL;
	BOOLEAN bLocked;
	PMDL Mdl;
	SIZE_T MdlSize = 0;

	// sanity checks
	ASSERT(LockOperation==IoReadAccess || LockOperation == IoWriteAccess || LockOperation == IoModifyAccess );

	/*
	* Allocate and initialize MDL
	*/
	MdlSize = MmSizeOfMdl(UserBuffer,UserBufferSize);
	if ( !MdlSize )	{
		DBGLOG((LError,"MmSizeOfMdl failed\n"));
		return STATUS_UNSUCCESSFUL;
	}

	Mdl = ExAllocatePoolWithTag( NonPagedPool, MdlSize, 'UBPL' );
	ntStatus = Mdl ? STATUS_SUCCESS : STATUS_INSUFFICIENT_RESOURCES;
	if(!NT_SUCCESS(ntStatus)) {
		return ntStatus;
	}

	MmInitializeMdl(Mdl,UserBuffer,UserBufferSize);

	/*
	*	Lock user pages
	*/
	try {
		MmProbeAndLockPages(
			Mdl, 
			UserMode, 
			LockOperation
			);
		bLocked = TRUE;
	} except (EXCEPTION_EXECUTE_HANDLER) {
		ntStatus = GetExceptionCode();
	}

	if(!NT_SUCCESS(ntStatus)){
		DBGLOG((LError,"%08X=MmProbeAndLockPages failed\n",ntStatus));
		IoFreeMdl( Mdl );
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	*SystemBuffer = MmGetSystemAddressForMdlSafe( Mdl, NormalPagePriority );

	if ( *SystemBuffer == NULL ){
		DBGLOG((LError,"MmGetSystemAddressForMdlPrettySafe failed\n"));

		MmUnlockPages( Mdl );
		IoFreeMdl( Mdl );
	}else{
		*pMdl = Mdl;
	}
	return STATUS_SUCCESS;
}
//***********************************************************************************
// Name: UF_LockUserBufferLight
//
// Routine Description: 
//
//		Locks user buffer in memory and maps it into system space.
//
// Return Value: 
//
//		Operation status.
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	UF_LockUserBufferLight(
		IN PVOID UserBuffer,
		IN ULONG UserBufferSize,
		OUT PMDL *pMdl,
		OUT PVOID* SystemBuffer
		)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	PVOID LockedAddress = NULL;
	BOOLEAN bLocked;
	PMDL Mdl;
	SIZE_T MdlSize = 0;

	/*
	* Allocate and initialize MDL
	*/
	Mdl = IoAllocateMdl(UserBuffer,UserBufferSize,FALSE,FALSE,NULL);
	if ( Mdl == NULL )	{
		DBGLOG((LError,"MmSizeOfMdl failed\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	/*
	*	Lock user pages
	*/
	try {
		MmProbeAndLockPages(
			Mdl, 
			KernelMode, 
			IoModifyAccess
			);
		bLocked = TRUE;
	} except (EXCEPTION_EXECUTE_HANDLER) {
		ntStatus = GetExceptionCode();
	}

	if(!NT_SUCCESS(ntStatus)){
		DBGLOG((LError,"%08X=MmProbeAndLockPages failed\n",ntStatus));
		IoFreeMdl( Mdl );
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	*SystemBuffer = MmGetSystemAddressForMdlSafe( Mdl, NormalPagePriority );

	if ( *SystemBuffer == NULL ){
		DBGLOG((LError,"MmGetSystemAddressForMdlPrettySafe failed\n"));

		MmUnlockPages( Mdl );
		IoFreeMdl( Mdl );
	}else{
		*pMdl = Mdl;
	}
	return STATUS_SUCCESS;
}

//***********************************************************************************
// Name: UF_UnlockUserBuffer
//
// Routine Description: 
//
//		Unlocks user buffer locked with UF_LockUserBuffer function.
//
// Return Value: 
//
//		Operation status.
//
// Parameters:
//
//***********************************************************************************
NTSTATUS 
	UF_UnlockUserBuffer(
		IN PMDL Mdl,
		IN PVOID SystemBuffer
		)
{
	if ( Mdl ) {
		MmUnlockPages( Mdl );
		ExFreePoolWithTag( Mdl, 'UBPL' );
	}
	return STATUS_SUCCESS;
}


