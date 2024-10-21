#ifndef __UTIL_H__
#define __UTIL_H__

NTSTATUS 
	UF_LockUserBuffer(
		IN PVOID UserBuffer,
		IN ULONG UserBufferSize,
		IN LOCK_OPERATION LockOperation,
		OUT PMDL *pMdl,
		OUT PVOID* SystemBuffer
		);

NTSTATUS 
	UF_LockUserBufferLight(
		IN PVOID UserBuffer,
		IN ULONG UserBufferSize,
		OUT PMDL *pMdl,
		OUT PVOID* SystemBuffer
		);

NTSTATUS 
	UF_UnlockUserBuffer(
		IN PMDL Mdl,
		IN PVOID SystemBuffer
		);

#endif //__UTIL_H__