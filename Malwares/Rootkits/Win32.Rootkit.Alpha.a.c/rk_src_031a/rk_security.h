
#ifndef __RK_SECURITY_H__
#define __RK_SECURITY_H__

/* __________________________________________________________
 . NT Security Functions
 . patch these for back doors
 . __________________________________________________________ */
NTSYSAPI
NTSTATUS
NTAPI
NtAccessCheck(
	PSECURITY_DESCRIPTOR pSecurityDescriptor,
	HANDLE hTokenClient,
	ACCESS_MASK DesiredAccess,
	PGENERIC_MAPPING pGenericMapping,
	PPRIVILEGE_SET pPrivilegeSet,
	PULONG pPrivilegeSetLength,
	PACCESS_MASK pAccessGranted,
	PNTSTATUS AccessGrantedReturnStatus
);

NTSYSAPI
NTSTATUS
NTAPI
ZwAccessCheck(
	PSECURITY_DESCRIPTOR pSecurityDescriptor,
	HANDLE hTokenClient,
	ACCESS_MASK DesiredAccess,
	PGENERIC_MAPPING pGenericMapping,
	PPRIVILEGE_SET pPrivilegeSet,
	PULONG pPrivilegeSetLength,
	PACCESS_MASK pAccessGranted,
	PNTSTATUS AccessGrantedReturnStatus
);

/* ------------[ windows 2000 extensions ]-------------------*/
#ifdef NT50
typedef struct _OBJECT_TYPE_LIST {
	USHORT Level;
	USHORT Sbz;
	GUID *ObjectType;
} OBJECT_TYPE_LIST, *POBJECT_TYPE_LIST;

NTSYSAPI
NTSTATUS
NTAPI
NtAccessCheckByType(
	PSECURITY_DESCRIPTOR pSecurityDescriptor,
	PSID PrincipalSelfSid,
	HANDLE hClientToken,
	ACCESS_MASK DesiredAccess,
	POBJECT_TYPE_LIST ObjectTypeList,
	ULONG ObjectTypeListLength,
	PGENERIC_MAPPING pGenericMapping,
	PPRIVILEGE_SET pPrivilegeSet,
	PULONG pPrivilegeSetLength,
	PACCESS_MASK pAccessGranted,
	PNTSTATUS AccessGrantedReturnStatus
);

NTSYSAPI
NTSTATUS
NTAPI
ZwAccessCheckByType(
	PSECURITY_DESCRIPTOR pSecurityDescriptor,
	PSID PrincipalSelfSid,
	HANDLE hClientToken,
	ACCESS_MASK DesiredAccess,
	POBJECT_TYPE_LIST ObjectTypeList,
	ULONG ObjectTypeListLength,
	PGENERIC_MAPPING pGenericMapping,
	PPRIVILEGE_SET pPrivilegeSet,
	PULONG pPrivilegeSetLength,
	PACCESS_MASK pAccessGranted,
	PNTSTATUS AccessGrantedReturnStatus
);

typedef enum _AUDIT_EVENT_TYPE {
	AuditEventObjectAccess,
	AuditEventDirectoryServiceAccess
} AUDIT_EVENT_TYPE, *PAUDIT_EVENT_TYPE;

NTSYSAPI
NTSTATUS
NTAPI
NtAccessCheckByTypeAndAuditAlarm(
	PUNICODE_STRING SubSystemName,
	PVOID HandleId,
	PUNICODE_STRING ObjectTypeName,
	PUNICODE_STRING ObjectName,
	PSECURITY_DESCRIPTOR pSecurityDescriptor,
	PSID PrincipalSelfSid,
	ACCESS_MASK DesiredAccess,
	AUDIT_EVENT_TYPE AuditType,
	ULONG Flags,
	POBJECT_TYPE_LIST ObjectTypeList,
	ULONG ObjectTypeListLength,
	PGENERIC_MAPPING pGenericMapping,
	BOOLEAN bObjectCreation,
	PACCESS_MASK pAccessGranted,
	PNTSTATUS AccessGrantedReturnStatus,
	PBOOLEAN bGenerateOnClose
);

NTSYSAPI
NTSTATUS
NTAPI
ZwAccessCheckByTypeAndAuditAlarm(
	PUNICODE_STRING SubSystemName,
	PVOID HandleId,
	PUNICODE_STRING ObjectTypeName,
	PUNICODE_STRING ObjectName,
	PSECURITY_DESCRIPTOR pSecurityDescriptor,
	PSID PrincipalSelfSid,
	ACCESS_MASK DesiredAccess,
	AUDIT_EVENT_TYPE AuditType,
	ULONG Flags,
	POBJECT_TYPE_LIST ObjectTypeList,
	ULONG ObjectTypeListLength,
	PGENERIC_MAPPING pGenericMapping,
	BOOLEAN bObjectCreation,
	PACCESS_MASK pAccessGranted,
	PNTSTATUS AccessGrantedReturnStatus,
	PBOOLEAN bGenerateOnClose
);

NTSYSAPI
NTSTATUS
NTAPI
NtAccessCheckByTypeResultList(
	PSECURITY_DESCRIPTOR pSecurityDescriptor,
	PSID PrincipalSelfSid,
	HANDLE hClientToken,
	ACCESS_MASK DesiredAccess,
	POBJECT_TYPE_LIST ObjectTypeList,
	ULONG ObjectTypeListLength,
	PGENERIC_MAPPING pGenericMapping,
	PPRIVILEGE_SET pPrivilegeSet,
	PULONG pPrivilegeSetLength,
	PACCESS_MASK pAccessGranted,
	PNTSTATUS AccessGrantedReturnStatus
);

NTSYSAPI
NTSTATUS
NTAPI
ZwAccessCheckByTypeResultList(
	PSECURITY_DESCRIPTOR pSecurityDescriptor,
	PSID PrincipalSelfSid,
	HANDLE hClientToken,
	ACCESS_MASK DesiredAccess,
	POBJECT_TYPE_LIST ObjectTypeList,
	ULONG ObjectTypeListLength,
	PGENERIC_MAPPING pGenericMapping,
	PPRIVILEGE_SET pPrivilegeSet,
	PULONG pPrivilegeSetLength,
	PACCESS_MASK pAccessGranted,
	PNTSTATUS AccessGrantedReturnStatus
);

NTSYSAPI
NTSTATUS
NTAPI
NtAccessCheckByTypeResultListAndAuditAlarm(
	PUNICODE_STRING SubSystemName,
	PVOID HandleId,
	PUNICODE_STRING ObjectTypeName,
	PUNICODE_STRING ObjectName,
	PSECURITY_DESCRIPTOR pSecurityDescriptor,
	PSID PrincipalSelfSid,
	ACCESS_MASK DesiredAccess,
	AUDIT_EVENT_TYPE AuditType,
	ULONG Flags,
	POBJECT_TYPE_LIST ObjectTypeList,
	ULONG ObjectTypeListLength,
	PGENERIC_MAPPING pGenericMapping,
	BOOLEAN bObjectCreation,
	PACCESS_MASK pAccessGranted,
	PNTSTATUS AccessGrantedReturnStatus,
	PBOOLEAN bGenerateOnClose
);

/* holy shit */
NTSYSAPI
NTSTATUS
NTAPI
ZwAccessCheckByTypeResultListAndAuditAlarm(
	PUNICODE_STRING SubSystemName,
	PVOID HandleId,
	PUNICODE_STRING ObjectTypeName,
	PUNICODE_STRING ObjectName,
	PSECURITY_DESCRIPTOR pSecurityDescriptor,
	PSID PrincipalSelfSid,
	ACCESS_MASK DesiredAccess,
	AUDIT_EVENT_TYPE AuditType,
	ULONG Flags,
	POBJECT_TYPE_LIST ObjectTypeList,
	ULONG ObjectTypeListLength,
	PGENERIC_MAPPING pGenericMapping,
	BOOLEAN bObjectCreation,
	PACCESS_MASK pAccessGranted,
	PNTSTATUS AccessGrantedReturnStatus,
	PBOOLEAN bGenerateOnClose
);

NTSYSAPI
NTSTATUS
NTAPI
NtImpersonateAnonymousToken(
	IN HANDLE hThread
);

NTSYSAPI
NTSTATUS
NTAPI
ZwImpersonateAnonymousToken(
	IN HANDLE hThread
);

typedef enum {
    LT_DONT_CARE,
    LT_LOWEST_LATENCY
} LATENCY_TIME;

NTSYSAPI
NTSTATUS
NTAPI
NtRequestWakeupLatency(
	IN LATENCY_TIME Latency
);

NTSYSAPI
NTSTATUS
NTAPI
ZwRequestWakeupLatency(
	IN LATENCY_TIME Latency
);

NTSYSAPI
NTSTATUS
NTAPI
NtAreMappedFilesTheSame(
	IN PVOID VirtualAddress1,
	IN PVOID VirtualAddress2
);

NTSYSAPI
NTSTATUS
NTAPI
ZwAreMappedFilesTheSame(
	IN PVOID VirtualAddress1,
	IN PVOID VirtualAddress2
);
#endif
/* ----[ end win2k ] --------------------- */

NTSYSAPI
NTSTATUS
NTAPI
NtAccessCheckAndAuditAlarm(
	PUNICODE_STRING SubSystemName,
	PVOID HandleId,
	PUNICODE_STRING ObjectTypeName,
	PUNICODE_STRING ObjectName,
	PSECURITY_DESCRIPTOR pSecurityDescriptor,
	ACCESS_MASK DesiredAccess,
	PGENERIC_MAPPING pGenericMapping,
	BOOLEAN bObjectCreation,
	PACCESS_MASK pAccessGranted,
	PNTSTATUS AccessGrantedReturnStatus,
	PBOOLEAN bGenerateOnClose
);

NTSYSAPI
NTSTATUS
NTAPI
ZwAccessCheckAndAuditAlarm(
	PUNICODE_STRING SubSystemName,
	PVOID HandleId,
	PUNICODE_STRING ObjectTypeName,
	PUNICODE_STRING ObjectName,
	PSECURITY_DESCRIPTOR pSecurityDescriptor,
	ACCESS_MASK DesiredAccess,
	PGENERIC_MAPPING pGenericMapping,
	BOOLEAN bObjectCreation,
	PACCESS_MASK pAccessGranted,
	PNTSTATUS AccessGrantedReturnStatus,
	PBOOLEAN bGenerateOnClose
);

typedef struct _SID_AND_ATTRIBUTES {
	PSID Sid;
	ULONG Attributes;
}SID_AND_ATTRIBUTES, * PSID_AND_ATTRIBUTES;


typedef struct _TOKEN_GROUPS {
	ULONG GroupCount;
	SID_AND_ATTRIBUTES Groups[ANYSIZE_ARRAY];
}TOKEN_GROUPS, *PTOKEN_GROUPS;

NTSYSAPI
NTSTATUS
NTAPI
NtAdjustGroupsToken(
	IN HANDLE hToken,
	IN BOOLEAN ResetToDefault,
	IN PTOKEN_GROUPS pNewTokenGroups,
	OUT ULONG pOldTokenGroupsLength,
	OUT PTOKEN_GROUPS pOldTokenGroups,
	OUT PULONG pOldTokenGroupsActualLength OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwAdjustGroupsToken(
	IN HANDLE hToken,
	IN BOOLEAN ResetToDefault,
	IN PTOKEN_GROUPS pNewTokenGroups,
	OUT ULONG pOldTokenGroupsLength,
	OUT PTOKEN_GROUPS pOldTokenGroups,
	OUT PULONG pOldTokenGroupsActualLength OPTIONAL
);

typedef struct _TOKEN_PRIVILEGES {
    ULONG PrivilegeCount;
    LUID_AND_ATTRIBUTES Privileges[ANYSIZE_ARRAY];
} TOKEN_PRIVILEGES, *PTOKEN_PRIVILEGES;

#define TOKEN_ASSIGN_PRIMARY    (0x0001)
#define TOKEN_DUPLICATE         (0x0002)
#define TOKEN_IMPERSONATE       (0x0004)
#define TOKEN_QUERY             (0x0008)
#define TOKEN_QUERY_SOURCE      (0x0010)
#define TOKEN_ADJUST_PRIVILEGES (0x0020)
#define TOKEN_ADJUST_GROUPS     (0x0040)
#define TOKEN_ADJUST_DEFAULT    (0x0080)

#define TOKEN_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED  |\
                          TOKEN_ASSIGN_PRIMARY      |\
                          TOKEN_DUPLICATE           |\
                          TOKEN_IMPERSONATE         |\
                          TOKEN_QUERY               |\
                          TOKEN_QUERY_SOURCE        |\
                          TOKEN_ADJUST_PRIVILEGES   |\
                          TOKEN_ADJUST_GROUPS       |\
                          TOKEN_ADJUST_DEFAULT)


#define TOKEN_READ       (STANDARD_RIGHTS_READ      |\
                          TOKEN_QUERY)


#define TOKEN_WRITE      (STANDARD_RIGHTS_WRITE     |\
                          TOKEN_ADJUST_PRIVILEGES   |\
                          TOKEN_ADJUST_GROUPS       |\
                          TOKEN_ADJUST_DEFAULT)

#define TOKEN_EXECUTE    (STANDARD_RIGHTS_EXECUTE)

typedef enum _TOKEN_TYPE {
    TokenPrimary = 1,
    TokenImpersonation
    } TOKEN_TYPE;
typedef TOKEN_TYPE *PTOKEN_TYPE;



	
NTSYSAPI
NTSTATUS
NTAPI
NtAdjustPrivilegesToken(
	IN HANDLE hToken,
    IN BOOLEAN DisableAllPrivileges,
    IN PTOKEN_PRIVILEGES pNewPrivlegeSet,
    IN ULONG PreviousPrivilegeSetBufferLength OPTIONAL,
    PTOKEN_PRIVILEGES pPreviousPrivlegeSet OPTIONAL,
    PULONG PreviousPrivlegeSetReturnLength OPTIONAL
);

NTSTATUS
NTAPI
ZwAdjustPrivilegesToken(
	IN HANDLE hToken,
    IN BOOLEAN DisableAllPrivileges,
    IN PTOKEN_PRIVILEGES pNewPrivlegeSet,
    IN ULONG PreviousPrivilegeSetBufferLength OPTIONAL,
    PTOKEN_PRIVILEGES pPreviousPrivlegeSet OPTIONAL,
    PULONG PreviousPrivlegeSetReturnLength OPTIONAL
);


NTSYSAPI
NTSTATUS
NTAPI
NtCloseObjectAuditAlarm(
	IN PUNICODE_STRING SubSystemName,
	IN PVOID HandleId,
	IN BOOLEAN bGenerateOnClose
);


NTSYSAPI
NTSTATUS
NTAPI
ZwCloseObjectAuditAlarm(
	IN PUNICODE_STRING SubSystemName,
	IN PVOID HandleId,
	IN BOOLEAN bGenerateOnClose
);

NTSYSAPI
NTSTATUS
NTAPI
NtDeleteObjectAuditAlarm(
	IN PUNICODE_STRING SubSystemName,
	IN PVOID HandleId,
	IN BOOLEAN bGenerateOnClose
);

NTSYSAPI
NTSTATUS
NTAPI
ZwDeleteObjectAuditAlarm(
	IN PUNICODE_STRING SubSystemName,
	IN PVOID HandleId,
	IN BOOLEAN bGenerateOnClose
);

NTSYSAPI
NTSTATUS
NTAPI
NtDuplicateToken(
	IN HANDLE hToken,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes, //Describing quality of service structure and security descriptor and OBJ_INHERIT flag
	IN BOOLEAN bMakeTokenEffectiveOnly,
	IN TOKEN_TYPE TokenType,
	OUT PHANDLE phNewToken
);


NTSYSAPI
NTSTATUS
NTAPI
ZwDuplicateToken(
	IN HANDLE hToken,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes, //Describing quality of service structure and security descriptor and OBJ_INHERIT flag
	IN BOOLEAN bMakeTokenEffectiveOnly,
	IN TOKEN_TYPE TokenType,
	OUT PHANDLE phNewToken
);

NTSYSAPI
NTSTATUS
NTAPI
NtImpersonateThread(
	IN HANDLE hThread,
	IN HANDLE hThreadToImpersonate,
	IN PSECURITY_QUALITY_OF_SERVICE Qos
);

NTSYSAPI
NTSTATUS
NTAPI
ZwImpersonateThread(
	IN HANDLE hThread,
	IN HANDLE hThreadToImpersonate,
	IN PSECURITY_QUALITY_OF_SERVICE Qos
);

NTSYSAPI
NTSTATUS
NTAPI
NtOpenObjectAuditAlarm(
	IN PUNICODE_STRING SubsystemName,
	IN PVOID HandleId,
	IN PUNICODE_STRING ObjectTypeName,
	IN PUNICODE_STRING ObjectName,
	IN PSECURITY_DESCRIPTOR pSecurityDescriptor,
	IN HANDLE hTokenClient,
	IN ACCESS_MASK DesiredAccess,
	IN ACCESS_MASK GrantedAccess,
	IN PPRIVILEGE_SET pPrivilegeSet,
	IN BOOLEAN bObjectCreation,
	IN BOOLEAN bAccessGranted,
	OUT PBOOLEAN bGenerateOnClose
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenObjectAuditAlarm(
	IN PUNICODE_STRING SubsystemName,
	IN PVOID HandleId,
	IN PUNICODE_STRING ObjectTypeName,
	IN PUNICODE_STRING ObjectName,
	IN PSECURITY_DESCRIPTOR pSecurityDescriptor,
	IN HANDLE hTokenClient,
	IN ACCESS_MASK DesiredAccess,
	IN ACCESS_MASK GrantedAccess,
	IN PPRIVILEGE_SET pPrivilegeSet,
	IN BOOLEAN bObjectCreation,
	IN BOOLEAN bAccessGranted,
	OUT PBOOLEAN bGenerateOnClose
);

NTSYSAPI
NTSTATUS
NTAPI
NtOpenProcessToken(
	IN HANDLE hProcess,
	IN ACCESS_MASK DesiredAccess,
	OUT PHANDLE phToken
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenProcessToken(
	IN HANDLE hProcess,
	IN ACCESS_MASK DesiredAccess,
	OUT PHANDLE phToken
);

NTSYSAPI
NTSTATUS
NTAPI
NtOpenThreadToken(
	IN HANDLE hThread,
	IN ACCESS_MASK DesiredAccess,
	IN BOOLEAN bUseContextOfProcess,
	OUT PHANDLE phToken
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenThreadToken(
	IN HANDLE hThread,
	IN ACCESS_MASK DesiredAccess,
	IN BOOLEAN bUseContextOfProcess,
	OUT PHANDLE phToken
);

NTSYSAPI
NTSTATUS
NTAPI
NtPrivilegeCheck(
	IN HANDLE hToken,
	PPRIVILEGE_SET pPrivilegeSet,
	PBOOLEAN pbHasPrivileges
);

NTSYSAPI
NTSTATUS
NTAPI
ZwPrivilegeCheck(
	IN HANDLE hToken,
	IN PPRIVILEGE_SET pPrivilegeSet,
	OUT PBOOLEAN pbHasPrivileges
);

NTSYSAPI
NTSTATUS
NTAPI
NtPrivilegeObjectAuditAlarm(
	IN PUNICODE_STRING SubsystemName,
    IN PVOID HandleId,
    IN HANDLE hToken,
    IN ACCESS_MASK DesiredAccess,
    IN PPRIVILEGE_SET pPrivilegeSet,
    IN BOOLEAN AccessGranted
);

NTSYSAPI
NTSTATUS
NTAPI
ZwPrivilegeObjectAuditAlarm(
	IN PUNICODE_STRING SubsystemName,
    IN PVOID HandleId,
    IN HANDLE hToken,
    IN ACCESS_MASK DesiredAccess,
    IN PPRIVILEGE_SET pPrivilegeSet,
    IN BOOLEAN AccessGranted
);

NTSYSAPI
NTSTATUS
NTAPI
NtPrivilegedServiceAuditAlarm(
	IN PUNICODE_STRING SubsystemName,
    IN PUNICODE_STRING ServiceName,
    IN HANDLE hToken,
    IN PPRIVILEGE_SET pPrivilegeSet,
    IN BOOLEAN AccessGranted
);

NTSYSAPI
NTSTATUS
NTAPI
ZwPrivilegedServiceAuditAlarm(
	IN PUNICODE_STRING SubsystemName,
    IN PUNICODE_STRING ServiceName,
    IN HANDLE hToken,
    IN PPRIVILEGE_SET pPrivilegeSet,
    IN BOOLEAN AccessGranted
);

typedef enum _TOKEN_INFORMATION_CLASS {
    TokenUser = 1,
    TokenGroups,
    TokenPrivileges,
    TokenOwner,
    TokenPrimaryGroup,
    TokenDefaultDacl,
    TokenSource,
    TokenType,
    TokenImpersonationLevel,
    TokenStatistics
} TOKEN_INFORMATION_CLASS, *PTOKEN_INFORMATION_CLASS;


NTSYSAPI
NTSTATUS
NTAPI
NtQueryInformationToken(
	IN HANDLE hToken,
	IN TOKEN_INFORMATION_CLASS TokenInfoClass,
	OUT PVOID TokenInfoBuffer,
	IN ULONG TokenInfoBufferLength,
	OUT PULONG BytesReturned
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationToken(
	IN HANDLE hToken,
	IN TOKEN_INFORMATION_CLASS TokenInfoClass,
	OUT PVOID TokenInfoBuffer,
	IN ULONG TokenInfoBufferLength,
	OUT PULONG BytesReturned
);

NTSYSAPI
NTSTATUS
NTAPI
NtSetInformationToken(
	IN HANDLE hToken,
	IN TOKEN_INFORMATION_CLASS TokenInfoClass,
	IN PVOID TokenInfoBuffer,
	IN ULONG TokenInfoBufferLength
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetInformationToken(
	IN HANDLE hToken,
	IN TOKEN_INFORMATION_CLASS TokenInfoClass,
	IN PVOID TokenInfoBuffer,
	IN ULONG TokenInfoBufferLength
);

NTSYSAPI
NTSTATUS
NTAPI
NtQuerySecurityObject(
	IN HANDLE hObject,
	IN SECURITY_INFORMATION SecurityInfoRequested,
	IN PSECURITY_DESCRIPTOR pSecurityDescriptor,
	IN ULONG pSecurityDescriptorLength,
	OUT PULONG BytesRequired
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySecurityObject(
	IN HANDLE hObject,
	IN SECURITY_INFORMATION SecurityInfoRequested,
	IN PSECURITY_DESCRIPTOR pSecurityDescriptor,
	IN ULONG pSecurityDescriptorLength,
	OUT PULONG BytesRequired
);

NTSYSAPI
NTSTATUS
NTAPI
NtSetSecurityObject(
	IN HANDLE hObject,
	IN SECURITY_INFORMATION SecurityInfoRequested,
	IN PSECURITY_DESCRIPTOR pSecurityDescriptor
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetSecurityObject(
	IN HANDLE hObject,
	IN SECURITY_INFORMATION SecurityInfoRequested,
	IN PSECURITY_DESCRIPTOR pSecurityDescriptor
);


#endif