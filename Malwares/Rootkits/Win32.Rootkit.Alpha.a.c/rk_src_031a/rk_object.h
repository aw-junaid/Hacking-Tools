
#ifndef __RK_OBJECT_H__
#define __RK_OBJECT_H__


/* ________________________________________________
 . Object Manager
 . ________________________________________________ */
typedef struct QueryDirectoryObjectBuffer_t 
{
	UNICODE_STRING DirectoryEntry;
	UNICODE_STRING DirectoryEntryType;
	char Buffer[1000];
} QUERYDIRECTORYOBJECTBUFFER, *PQUERYDIRECTORYOBJECTBUFFER;

#define DUPLICATE_SAME_ACCESS	0x00000002

typedef struct ObjectBasicInfo_t {
	char Unknown1[8];
	ULONG HandleCount;
	ULONG ReferenceCount;
	ULONG PagedQuota;
	ULONG NonPagedQuota;
	char Unknown2[32];
} OBJECT_BASIC_INFO, *POBJECT_BASIC_INFO;

typedef struct ObjectNameInfo_t {
	UNICODE_STRING ObjectName;
	WCHAR ObjectNameBuffer[1];
} OBJECT_NAME_INFO, *POBJECT_NAME_INFO;

typedef struct ObjectTypeInfo_t {
	UNICODE_STRING ObjectTypeName;
	char Unknown[0x58];
	WCHAR ObjectTypeNameBuffer[1];
} OBJECT_TYPE_INFO, *POBJECT_TYPE_INFO;

typedef struct ObjectAllTypeInfo_t {
	ULONG NumberOfObjectTypes;
	OBJECT_TYPE_INFO ObjectsTypeInfo[1];
} OBJECT_ALL_TYPES_INFO, *POBJECT_ALL_TYPES_INFO;

typedef struct ObjectProtectionInfo_t {
	BOOLEAN bInherit;
	BOOLEAN bProtectHandle;
} OBJECT_PROTECTION_INFO, *POBJECT_PROTECTION_INFO;

typedef enum _OBJECT_INFO_CLASS {
	ObjectBasicInfo,
	ObjectNameInfo,
	ObjectTypeInfo,
	ObjectAllTypesInfo,
	ObjectProtectionInfo
} OBJECT_INFO_CLASS;

/* _______________________________________________
 . Atoms
 . _______________________________________________ */
typedef USHORT ATOM;
typedef PUSHORT PATOM;

typedef enum _ATOM_INFO_CLASS {
	SingleAtom,
	AllAtoms,
	MaxAtomInfoClass,
} ATOM_INFO_CLASS;

typedef struct AtomInfoSingle {
	USHORT ReferenceCount;
	USHORT Unknown;
	USHORT AtomStringLength;
	WCHAR AtomString[1];
} ATOMINFOSINGLE, *PATOMINFOSINGLE;

typedef struct AtomInfoAll {
	ULONG TotalNumberOfEntriesInGlobalAtomTable;
	ATOM AtomValues[1];
} ATOMINFOALL, *PATOMINFOALL;



/* _______________________________________________
 . function typedefs
 . _______________________________________________ */
typedef NTSTATUS (*ZWCLOSE)( IN HANDLE );
extern ZWCLOSE OldZwClose;

/* function pointers */
typedef NTSTATUS (*ZWQUERYDIRECTORYOBJECT)(
	IN HANDLE hDirectory,
	OUT PQUERYDIRECTORYOBJECTBUFFER DirectoryEntryBuffer,
	IN ULONG DirectoryEntryBufferSize,
	IN BOOLEAN  bOnlyFirstEntry,
	IN BOOLEAN bFirstEntry,
	IN PULONG  BytesReturned,
	IN PULONG  EntryIndex
);
extern ZWQUERYDIRECTORYOBJECT OldZwQueryDirectoryObject;


/* _______________________________________________
 . Atoms
 . _______________________________________________ */

#ifdef NT50
NTSYSAPI
NTSTATUS
NTAPI
NtAddAtom(
	IN PWCHAR pString,
	IN ULONG StringLength,
	OUT PATOM pAtom
);


NTSTATUS
NTAPI
ZwAddAtom(
	IN PWCHAR pString,
	IN ULONG StringLength,
	OUT PATOM pAtom
);

#else
NTSYSAPI
NTSTATUS
NTAPI
NtAddAtom(
	IN PWCHAR pString,
	OUT PATOM pAtom
);

NTSTATUS
NTAPI
ZwAddAtom(
	IN PWCHAR pString,
	OUT PATOM pAtom
);

#endif

NTSYSAPI
NTSTATUS
NTAPI
NtQueryInformationAtom(
	IN ATOM Atom,
	IN ATOM_INFO_CLASS AtomInfoClass,
	OUT PVOID AtomInfoBuffer,
	IN ULONG AtomInfoBufferLength,
	OUT PULONG BytesCopied
);


NTSTATUS
NTAPI
ZwQueryInformationAtom(
	IN ATOM Atom,
	IN ATOM_INFO_CLASS AtomInfoClass,
	OUT PVOID AtomInfoBuffer,
	IN ULONG AtomInfoBufferLength,
	OUT PULONG BytesCopied
);


#ifdef NT50
NTSYSAPI
NTSTATUS
NTAPI
NtFindAtom(
	IN PWCHAR pString,
	IN ULONG StringLength,
	OUT PATOM pAtom
);


NTSTATUS
NTAPI
ZwFindAtom(
	IN PWCHAR pString,
	IN ULONG StringLength,
	OUT PATOM pAtom
);

#else
NTSYSAPI
NTSTATUS
NTAPI
NtFindAtom(
	IN PWCHAR pString,
	OUT PATOM pAtom
);


NTSTATUS
NTAPI
ZwFindAtom(
	IN PWCHAR pString,
	OUT PATOM pAtom
);

#endif

NTSYSAPI
NTSTATUS
NTAPI
NtDeleteAtom(
	IN ATOM Atom
);


NTSTATUS
NTAPI
ZwDeleteAtom(
	IN ATOM Atom
);

/* _______________________________________________
 . Object Manager routines
 . _______________________________________________ */
NTSYSAPI
NTSTATUS
NTAPI
NtClose(
    IN HANDLE Handle
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwClose(
    IN HANDLE Handle
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDuplicateObject(
	IN HANDLE hSourceProcessHandle,
	IN HANDLE hSourceHandle,
	IN HANDLE hTargetProcessHandle,
	IN OUT PHANDLE hTargetHandle,
	IN ACCESS_MASK AccessMask,
	IN BOOLEAN bInheritHandle,
	IN ULONG dwOptions
);

NTSYSAPI
NTSTATUS
NTAPI
ZwDuplicateObject(
	IN HANDLE hSourceProcessHandle,
	IN HANDLE hSourceHandle,
	IN HANDLE hTargetProcessHandle,
	IN OUT PHANDLE hTargetHandle,
	IN ACCESS_MASK AccessMask,
	IN BOOLEAN bInheritHandle,
	IN ULONG dwOptions
);

NTSYSAPI
NTSTATUS
NTAPI
NtCreateDirectoryObject(
    OUT PHANDLE DirectoryHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwCreateDirectoryObject(
    OUT PHANDLE DirectoryHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
NtCreateSymbolicLinkObject(
	OUT PHANDLE hSymbolicLink,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PUNICODE_STRING SymbolicLinkValue
);

NTSYSAPI
NTSTATUS
NTAPI
ZwCreateSymbolicLinkObject(
	OUT PHANDLE hSymbolicLink,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PUNICODE_STRING SymbolicLinkValue
);

NTSYSAPI
NTSTATUS
NTAPI
NtMakeTemporaryObject(
	IN HANDLE hObject
);

NTSYSAPI
NTSTATUS
NTAPI
ZwMakeTemporaryObject(
	IN HANDLE hObject
);

NTSYSAPI
NTSTATUS
NTAPI
NtOpenDirectoryObject(
	OUT PHANDLE hDirectory,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenDirectoryObject(
	OUT PHANDLE hDirectory,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
NtQueryDirectoryObject(
	IN HANDLE hDirectory,
	OUT PQUERYDIRECTORYOBJECTBUFFER DirectoryEntryBuffer,
	IN ULONG DirectoryEntryBufferSize,
	IN BOOLEAN  bOnlyFirstEntry,
	IN BOOLEAN bFirstEntry,
	IN PULONG  BytesReturned,
	IN PULONG  EntryIndex
);

NTSTATUS
NTAPI
ZwQueryDirectoryObject(
	IN HANDLE hDirectory,
	OUT PQUERYDIRECTORYOBJECTBUFFER DirectoryEntryBuffer,
	IN ULONG DirectoryEntryBufferSize,
	IN BOOLEAN  bOnlyFirstEntry,
	IN BOOLEAN bFirstEntry,
	IN PULONG  BytesReturned,
	IN PULONG  EntryIndex
);

NTSYSAPI
NTSTATUS
NTAPI
NtOpenSymbolicLinkObject(
	OUT PHANDLE hSymbolicLink,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenSymbolicLinkObject(
	OUT PHANDLE hSymbolicLink,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
NtQuerySymbolicLinkObject(
	IN HANDLE hSymbolicLink,
	IN OUT PUNICODE_STRING ObjectName,
	OUT PULONG BytesReturned
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySymbolicLinkObject(
	IN HANDLE hSymbolicLink,
	IN OUT PUNICODE_STRING ObjectName,
	OUT PULONG BytesReturned
);


NTSYSAPI
NTSTATUS
NTAPI
NtQueryObject(
	IN HANDLE hObject,
	IN OBJECT_INFO_CLASS ObjectInfoClass,
	OUT PVOID Buffer,
	IN ULONG BufferSize,
	OUT PULONG BytesReturned
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryObject(
	IN HANDLE hObject,
	IN OBJECT_INFO_CLASS ObjectInfoClass,
	OUT PVOID Buffer,
	IN ULONG BufferSize,
	OUT PULONG BytesReturned
);

NTSYSAPI
NTSTATUS
NTAPI
NtSetInformationObject(
	IN HANDLE hObject,
	IN OBJECT_INFO_CLASS ObjectInfoClass,
	IN PVOID Buffer,
	IN ULONG BufferSize
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetInformationObject(
	IN HANDLE hObject,
	IN OBJECT_INFO_CLASS ObjectInfoClass,
	IN PVOID Buffer,
	IN ULONG BufferSize
);


/* hooked functions */
NTSTATUS
NTAPI NewZwQueryDirectoryObject(
	IN HANDLE hDirectory,
	OUT PQUERYDIRECTORYOBJECTBUFFER DirectoryEntryBuffer,
	IN ULONG DirectoryEntryBufferSize,
	IN BOOLEAN  bOnlyFirstEntry,
	IN BOOLEAN bFirstEntry,
	IN PULONG  BytesReturned,
	IN PULONG  EntryIndex
);

NTSYSAPI
NTSTATUS
NTAPI NewZwClose(
	HANDLE Handle
	);



#endif
