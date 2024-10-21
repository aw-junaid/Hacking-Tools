//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BkDrv project
//	
// module: handle.h
// $Revision: 16 $
// $Date: 2012-02-21 19:01:51 +0400 (Вт, 21 фев 2012) $
// description:
//	Lightweight handle management engine. 


#ifndef BOOL
 #define BOOL ULONG
#endif

#define HANDLE_ROOT_COUNT		0x100
#ifdef _WIN64
	#define HANDLE_SHIFT		3
#else
	#define HANDLE_SHIFT		2
#endif

#define HANDLE_TABLE_MAGIC		'baTH'
#define HANDLE_RECORD_MAGIC		'ceRH'

typedef struct _HANDLE_RECORD HANDLE_RECORD, *PHANDLE_RECORD;

typedef BOOL	(_stdcall* HANDLE_INIT_ROUTINE)		(HANDLE Key, PVOID* pContext);
typedef BOOL	(_stdcall* HANDLE_CLEANUP_ROUTINE)	(HANDLE Key, PVOID* pContext);	

typedef struct _HANDLE_TABLE
{
#ifdef _DBG
	ULONG					Magic;
#endif
	KSPIN_LOCK				TableLock;
	KIRQL					OldIrql;
	ULONG					Records;
	ULONG					ContextSize;
	ULONG					Flags;
	HANDLE_INIT_ROUTINE		InitCallback;
	HANDLE_CLEANUP_ROUTINE	CleanupCallBack;
	LIST_ENTRY				GlobalListHead;
	LIST_ENTRY				KeyRoot[HANDLE_ROOT_COUNT];
} HANDLE_TABLE, *PHANDLE_TABLE;

#define		TF_REUSE_HANDLE		1

#pragma pack(push)
#pragma pack(1)
typedef struct _HANDLE_RECORD
{
#ifdef _DEBUG
	ULONG		Magic;
#endif
	LIST_ENTRY		Entry;
	LIST_ENTRY		GlobalEntry;
	PHANDLE_TABLE	HTable;
	HANDLE			Key;
	ULONG			RefCount;
	CHAR			Context[0];
} HANDLE_RECORD, *PHANDLE_RECORD;
#pragma pack(pop)

#if _DBG
	#define ASSERT_HANDLE_TABLE(x)	ASSERT(x->Magic == HANDLE_TABLE_MAGIC)
	#define ASSERT_HANDLE_RECORD(x) ASSERT(x->Magic == HANDLE_RECORD_MAGIC)
	#define	TAG_HANDLE_TABLE	HANDLE_TABLE_MAGIC
	#define	TAG_HANDLE_RECORD	HANDLE_RECORD_MAGIC
#else
	#define ASSERT_HANDLE_TABLE(x)
	#define ASSERT_HANDLE_RECORD(x)
	#define	TAG_HANDLE_TABLE	0
	#define	TAG_HANDLE_RECORD	0
#endif


typedef ULONG CRC32;
CRC32 Crc32(PCHAR pMem, ULONG uLen);


#define	CTX_TO_HREC(x)	CONTAINING_RECORD(x, HANDLE_RECORD, Context)
#define	CTX_TO_KEY(x)	(CONTAINING_RECORD(x, HANDLE_RECORD, Context))->Key


NTSTATUS	HandleAllocateTable(PHANDLE_TABLE* pHTable, ULONG ContextSize, HANDLE_INIT_ROUTINE	InitCallback, HANDLE_CLEANUP_ROUTINE CleanupCallback);
VOID	HandleReleaseTable(PHANDLE_TABLE HTable);
BOOL	HandleCreate(PHANDLE_TABLE	HTable, HANDLE Key, PVOID* pContext);
BOOL	HandleOpen(PHANDLE_TABLE HTable, HANDLE Key, PVOID* pContext);
BOOL	HandleClose(PHANDLE_TABLE HTable, HANDLE Key, PHANDLE_RECORD pHRec);
BOOL	HandleGet(PHANDLE_TABLE	HTable, ULONG Index, PVOID*	pContext);
