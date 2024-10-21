
#ifndef __RK_MEMORY_H__
#define __RK_MEMORY_H__

/* structures */

/* __________________________________
 . Virtual Address Descriptor
 . __________________________________ */

typedef struct vad 
{
	void *StartingAddress;
	void *EndingAddress;
	struct vad *ParentLink;
	struct vad *LeftLink;
	struct vad *RightLink;
	ULONG Flags;
}VAD, *PVAD;

/* locals */
/* ____________________________________________
 . Global Descriptor Table
 . ____________________________________________ */

NTSYSAPI 
NTSTATUS 
NTAPI 
KeI386AllocateGdtSelectors(
	PUSHORT pSelectorArray, 
	ULONG NumberOfSelectors
);

NTSYSAPI 
NTSTATUS 
NTAPI 
KeI386ReleaseGdtSelectors(
	PUSHORT pSelectorArray, 
	ULONG NumberOfSelectors
);

NTSYSAPI 
NTSTATUS 
NTAPI 
KeI386SetGdtSelector(
	ULONG Selector, 
	PVOID pDescriptor
);

/* _____________________________________________________________________________
 . pointer defs for memory functions
 . _____________________________________________________________________________ */

typedef NTSTATUS (*ZWCREATESECTION) (
	OUT PHANDLE phSection,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PLARGE_INTEGER MaximumSize OPTIONAL,
	IN ULONG SectionPageProtection,
	IN ULONG AllocationAttributes,
	IN HANDLE hFile OPTIONAL
);
extern ZWCREATESECTION OldZwCreateSection;

/* _____________________________________________________________________________
 . prototypes for memory trojan calls
 . _____________________________________________________________________________ */
NTSYSAPI
NTSTATUS
NTAPI NewZwCreateSection (
	OUT PHANDLE phSection,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PLARGE_INTEGER MaximumSize OPTIONAL,
	IN ULONG SectionPageProtection,
	IN ULONG AllocationAttributes,
	IN HANDLE hFile OPTIONAL
	);

/* ___________________________________________________________________________
 . prototypes for memory real calls
 . ___________________________________________________________________________ */
/* __________________________________________________________
 . NT Memory manipulation
 . __________________________________________________________ */
#define SEC_FILE           0x800000     
#define SEC_IMAGE         0x1000000     
#define SEC_RESERVE       0x4000000     
#define SEC_COMMIT        0x8000000     
#define SEC_NOCACHE      0x10000000     


NTSYSAPI
NTSTATUS
NTAPI
NtCreateSection (
	OUT PHANDLE phSection,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PLARGE_INTEGER MaximumSize OPTIONAL,
	IN ULONG SectionPageProtection,
	IN ULONG AllocationAttributes,
	IN HANDLE hFile OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwCreateSection (
	OUT PHANDLE phSection,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PLARGE_INTEGER MaximumSize OPTIONAL,
	IN ULONG SectionPageProtection,
	IN ULONG AllocationAttributes,
	IN HANDLE hFile OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
NtExtendSection(
	IN HANDLE hSection,
	IN OUT PLARGE_INTEGER ExtendSize
);

NTSYSAPI
NTSTATUS
NTAPI
ZwExtendSection(
	IN HANDLE hSection,
	IN OUT PLARGE_INTEGER ExtendSize
);


typedef enum _SECTION_INFORMATION_CLASS {
	SectionBasicInfo,
	SectionDetailedInfo,
} SECTION_INFORMATION_CLASS, *PSECTION_INFORMATION_CLASS;

typedef struct SectionBasicInfo_t {
	ULONG Unknown;
	ULONG AllocationAttributes;
	LARGE_INTEGER MaximumSize;
} SECTION_BASIC_INFO, *PSECTION_BASIC_INFO;

//SectionDetailedInfo works only on image mapped sections
typedef struct SectionDetailedInfo_t {
	char UnknownData[0x30];
} SECTION_DETAILED_INFO, *PSECTION_DETAILED_INFO;


NTSYSAPI
NTSTATUS
NTAPI
NtQuerySection(
	IN HANDLE hSection,
	IN SECTION_INFORMATION_CLASS SectionInfoClass,
	OUT PVOID Buffer,
	IN ULONG BufferSize,
	OUT PULONG BytesReturned
);

NTSYSAPI
NTSTATUS
NTAPI
NtOpenSection(
	OUT PHANDLE phSection,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);


NTSYSAPI
NTSTATUS
NTAPI
ZwOpenSection(
	OUT PHANDLE phSection,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);


NTSYSAPI
NTSTATUS
NTAPI
NtMapViewOfSection(
	IN HANDLE hSection,
	IN HANDLE hProcess,
	IN OUT PVOID *BaseAddress,
	IN ULONG ZeroBits,
	IN ULONG CommitSize,
	IN OUT PLARGE_INTEGER SectionOffset OPTIONAL,
    IN OUT PULONG ViewSize,
	IN SECTION_INHERIT InheritDisposition,
	IN ULONG AllocationType,
	IN ULONG Protect
);

NTSYSAPI
NTSTATUS
NTAPI
ZwMapViewOfSection(
	IN HANDLE hSection,
	IN HANDLE hProcess,
	IN OUT PVOID *BaseAddress,
	IN ULONG ZeroBits,
	IN ULONG CommitSize,
	IN OUT PLARGE_INTEGER SectionOffset OPTIONAL,
    IN OUT PULONG ViewSize,
	IN SECTION_INHERIT InheritDisposition,
	IN ULONG AllocationType,
	IN ULONG Protect
);

NTSYSAPI
NTSTATUS
NTAPI
NtUnmapViewOfSection(
	IN HANDLE hProcess,
	IN PVOID BaseAddress
);

NTSYSAPI
NTSTATUS
NTAPI
ZwUnmapViewOfSection(
	IN HANDLE hProcess,
	IN PVOID BaseAddress
);

NTSYSAPI
NTSTATUS
NTAPI
NtAllocateVirtualMemory(
	IN HANDLE hProces,
	IN OUT PVOID *PreferredBaseAddress,
	IN ULONG nLowerZeroBits,
	IN OUT PULONG SizeRequestedAllocated,
	IN ULONG AllocationType,
	IN ULONG ProtectionAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwAllocateVirtualMemory(
	IN HANDLE hProces,
	IN OUT PVOID *PreferredBaseAddress,
	IN ULONG nLowerZeroBits,
	IN OUT PULONG SizeRequestedAllocated,
	IN ULONG AllocationType,
	IN ULONG ProtectionAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
NtFreeVirtualMemory(
	IN HANDLE hProcess,
	IN OUT PVOID StartingAddress,
	IN OUT PULONG SizeRequestedReleased,
	IN ULONG ReleaseType
);

NTSYSAPI
NTSTATUS
NTAPI
ZwFreeVirtualMemory(
	IN HANDLE hProcess,
	IN OUT PVOID StartingAddress,
	IN OUT PULONG SizeRequestedReleased,
	IN ULONG ReleaseType
);

NTSYSAPI
NTSTATUS
NTAPI
NtFlushVirtualMemory(
	IN HANDLE hProcess,
	IN OUT PVOID StartingAddress,
	IN OUT PULONG SizeToFlush,
	OUT PIO_STATUS_BLOCK pIoStatusBlock
);

NTSYSAPI
NTSTATUS
NTAPI
ZwFlushVirtualMemory(
	IN HANDLE hProcess,
	IN OUT PVOID StartingAddress,
	IN OUT PULONG SizeToFlush,
	OUT PIO_STATUS_BLOCK pIoStatusBlock
);

typedef struct _MEMORY_BASIC_INFORMATION {
	PVOID BaseAddress;
	PVOID AllocationBase;
	ULONG AllocationProtect;
	ULONG RegionSize;
	ULONG State;
	ULONG Protect;
	ULONG Type;
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

typedef struct _BACKEDUP_SECTION_FILENAME_INFO {
	UNICODE_STRING BackedupSectionFileName;
	WCHAR Filename[1];
} MEMORY_BACKEDUP_SECTION_FILENAME_INFO, *PMEMORY_BACKEDUP_SECTION_FILENAME_INFO;


typedef enum _MEMORY_INFO_CLASS {
	MemoryBasicInformation,
	WorkingSetInfo,
	BackedupSectionFileNameInfo
} MEMORY_INFO_CLASS;


NTSYSAPI
NTSTATUS
NTAPI
NtQueryVirtualMemory(
	IN HANDLE hProcess,
	IN PVOID BaseAddress,
	IN MEMORY_INFO_CLASS MemoryInfoClass,
	OUT PVOID MemoryBasicInfo,
	IN ULONG MemoryBasicInfoSize,
	OUT PULONG BytesReturned
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryVirtualMemory(
	IN HANDLE hProcess,
	IN PVOID BaseAddress,
	IN MEMORY_INFO_CLASS MemoryInfoClass,
	OUT PVOID MemoryBasicInfo,
	IN ULONG MemoryBasicInfoSize,
	OUT PULONG BytesReturned
);

NTSYSAPI
NTSTATUS
NTAPI
NtProtectVirtualMemory(
	IN HANDLE hProcess,
	IN OUT PVOID *BaseAddress,
	IN OUT PULONG RegionSize,
	IN ULONG Protect,
	OUT PULONG OldProtect
);

NTSYSAPI
NTSTATUS
NTAPI
ZwProtectVirtualMemory(
	IN HANDLE hProcess,
	IN OUT PVOID *BaseAddress,
	IN OUT PULONG RegionSize,
	IN ULONG Protect,
	OUT PULONG OldProtect
);


NTSYSAPI
NTSTATUS
NTAPI
NtLockVirtualMemory(
	IN HANDLE hProcess,
	IN OUT PVOID *BaseAddress,
	IN OUT PULONG RegionSize,
	IN ULONG Unknown //(valid values are 1,2,3, VirtualAlloc uses 1
);

NTSYSAPI
NTSTATUS
NTAPI
ZwLockVirtualMemory(
	IN HANDLE hProcess,
	IN OUT PVOID *BaseAddress,
	IN OUT PULONG RegionSize,
	IN ULONG Unknown //(valid values are 1,2,3, VirtualLock uses 1
);

NTSYSAPI
NTSTATUS
NTAPI
NtUnlockVirtualMemory(
	IN HANDLE hProcess,
	IN OUT PVOID *BaseAddress,
	IN OUT PULONG RegionSize,
	IN ULONG Unknown //(valid values are 1,2,3, VirtualUnlock uses 1
);

NTSYSAPI
NTSTATUS
NTAPI
ZwUnlockVirtualMemory(
	IN HANDLE hProcess,
	IN OUT PVOID *BaseAddress,
	IN OUT PULONG RegionSize,
	IN ULONG Unknown //(valid values are 1,2,3, VirtualUnlock uses 1
);

NTSYSAPI
NTSTATUS
NTAPI
NtReadVirtualMemory(
	IN HANDLE hProcess,
	IN PVOID BaseAddress,
	OUT PVOID Buffer,
	IN ULONG BytesToRead,
	OUT PULONG BytesRead
);

NTSYSAPI
NTSTATUS
NTAPI
ZwReadVirtualMemory(
	IN HANDLE hProcess,
	IN PVOID BaseAddress,
	OUT PVOID Buffer,
	IN ULONG BytesToRead,
	OUT PULONG BytesRead
);

NTSYSAPI
NTSTATUS
NTAPI
NtWriteVirtualMemory(
	IN HANDLE hProcess,
	IN PVOID BaseAddress,
	IN PVOID Buffer,
	IN ULONG BytesToWrite,
	OUT PULONG BytesWritten
);


NTSYSAPI
NTSTATUS
NTAPI
ZwWriteVirtualMemory(
	IN HANDLE hProcess,
	IN PVOID BaseAddress,
	IN PVOID Buffer,
	IN ULONG BytesToWrite,
	OUT PULONG BytesWritten
);

/* _____________________________________________________________________
 . memory paging - move into memory section
 . _____________________________________________________________________ */
NTSYSAPI
NTSTATUS
NTAPI
NtCreatePagingFile(
	IN PUNICODE_STRING PagingFileName,
	IN PLARGE_INTEGER InitialSize,
	IN PLARGE_INTEGER MaxSize,
	IN ULONG Unused OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwCreatePagingFile(
	IN PUNICODE_STRING PagingFileName,
	IN PLARGE_INTEGER InitialSize,
	IN PLARGE_INTEGER MaxSize,
	IN ULONG Unused OPTIONAL
);

#endif
