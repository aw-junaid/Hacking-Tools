//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FsLib project
//	
// module: vfat.h
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description: 
//  Virtual FAT partition driver.


#if	DBG
	#define		TAG_VFAT	'taFv'
#else
	#define		TAG_VFAT	0
#endif

#define		VFAT_OEM_ID					"VFAT1.1 "


#define		VfatLockObject(pObject)			KeWaitForSingleObject(&(pObject)->Lock, Executive, KernelMode, FALSE, NULL)
#define		VfatUnlockObject(pObject)		KeReleaseMutex(&(pObject)->Lock, FALSE)

#define		VfatLockVolume(pVolume)			{ASSERT((pVolume)->Flags & VF_ACTIVE);VfatLockObject(pVolume);}
#define		VfatUnlockVolume(pVolume)		VfatUnlockObject(pVolume)

#define		VfatReferenceObject(pObject)	InterlockedIncrement(&(pObject)->ReferenceCount)
#define		VfatDereferenceObject(pObject)	InterlockedDecrement(&(pObject)->ReferenceCount)

#define		ASSERT_OBJECT_LOCKED(pObject)	ASSERT(KeReadStateEvent((PKEVENT)&(pObject)->Lock) < 0)


// File allocation table for FAT16
#define		FAT		USHORT
#define		PFAT	PUSHORT

#define		FAT_AVALIABLE	0
#define		FAT_RESERVED	0xfff0
#define		FAT_BAD			0xfff7
#define		FAT_LAST		0xffff
#define		FAT_EOC			0xfff8


#define		ROOT_DIR_SIZE	10	// % of disk size

// Minimum and maxumum supported number of root directory entries
#define		RD_ENTRIES_MIN	32
#define		RD_ENTRIES_MAX	512

#define		FAT_SIZE_MAX	255	// sectors (aproximately 32mb volume)



#define		VFAT_SECTOR_SIZE			0x200	// bytes

#define		VFAT_NAME_LENGTH			8
#define		VFAT_EXTENSION_LENGTH		3

#define		VFAT_MAX_NAME_LENGTH	(VFAT_NAME_LENGTH + VFAT_EXTENSION_LENGTH + 1)



typedef	struct	_VFAT_VOLUME VFAT_VOLUME, *PVFAT_VOLUME;
typedef struct  _VFAT_FCB VFAT_FCB, *PVFAT_FCB;

typedef struct	_FAT_NAME
{
	CHAR	Name[VFAT_NAME_LENGTH];
	CHAR	Extension[VFAT_EXTENSION_LENGTH];
} FAT_NAME, *PFAT_NAME;

typedef struct _FAT_DIR_ENTRY
{
	FAT_NAME	FileName;
	UCHAR		Attribute;
	UCHAR		Index;
	UCHAR		CreateTimeMs;
	USHORT		CreateTime;
	USHORT		CreateDate;
	USHORT		LastAccessDate;
	USHORT		ExtendedAttribute;
	USHORT		Time;
	USHORT		Date;
	USHORT		Cluster;
	ULONG		FileSize;
} FAT_DIR_ENTRY, *PFAT_DIR_ENTRY;

typedef	struct	_VFAT_FCB
{
#if DBG
	ULONG		Magic;
#endif
	LIST_ENTRY		FcbListEntry;
	LIST_ENTRY		FcbListHead;
	PVFAT_VOLUME	pVolume;			// Volume descriptor 
	PVFAT_FCB		ParentFcb;			// Parent FCB (represents parent directory for the file)
	FAT_DIR_ENTRY	DirEntry;			// Current directory entry for the file
	ULONG			ReferenceCount;
	ULONG			Flags;
	KMUTEX			Lock;
} VFAT_FCB, *PVFAT_FCB;

#define		FCB_FLAG_DIRTY		1
#define		FCB_FLAG_DIRECTORY	2
#define		FCB_FLAG_DELETED	4

#define		VFAT_FCB_MAGIC		'BCFv'
#define		ASSERT_FCB(x)		ASSERT(x->Magic == VFAT_FCB_MAGIC)


typedef struct _VFAT_FILE_CONTEXT
{
	LARGE_INTEGER	FilePosition;
	PVFAT_FCB		Fcb;			// Pointer to current FS-specific File Control Block

	PVOID			WorkBuffer;		// File work buffer
	ULONG			WorkBufLen;		// Work buffer size in bytes or number of records

	PLARGE_INTEGER	pPosition;		// Current file read/write position

	ACCESS_MASK		AccessMask;		// File access mask

	USHORT			Flags;
	USHORT			Index;			// Directory specific record index
} VFAT_FILE_CONTEXT, *PVFAT_FILE_CONTEXT;


typedef	struct	_VFAT_VOLUME
{
	PUNICODE_STRING	pDeviceName;
	PVBR			pVbr;
	PFAT			Fat0;
	PVFAT_FCB		RootFcb;
	ULONGLONG		StartSector;
	KMUTEX			Lock;
	ULONG			Flags;
} VFAT_VOLUME, *PVFAT_VOLUME;


typedef struct _FAT_DATE
{
	USHORT	Day:5;		// 1-31
	USHORT	Month:4;	// 1-12
	USHORT	Year:7;		// 0-127 (accual year - 1980)
} FAT_DATE, *PFAT_DATE;

typedef	struct _FAT_TIME
{
	USHORT	Seconds:5;	// 0-25 (accual seconds / 2)
	USHORT	Minutes:6;	// 0-59
	USHORT	Hour:5;		// 0-23
} FAT_TIME, *PFAT_TIME;


#define		VF_ACTIVE	1
#define		DISPOSITION_TO_OPTIONS(x)		(x << 24)

NTSTATUS	VfatInitialize(PBK_FS_AREA pFsArea);
VOID		VfatCleanup(VOID);

NTSTATUS VfatCreateFile(
				PVOID*			pContext, 
				PANSI_STRING	Path, 
				DWORD			DesiredAccess, 
				DWORD			SharedMode, 
				DWORD			CreateOptions, 
				DWORD			FlagsAndAttributes,
				PLARGE_INTEGER	pFilePosition
				);

NTSTATUS VfatCloseHandle(PVOID Context);
NTSTATUS VfatReadWriteFile(PVOID Context, PCHAR Buffer, ULONG Size, PLARGE_INTEGER Offset, PULONG_PTR pReturned, BOOL IsWrite);
NTSTATUS VfatQueryFileInfo(PVOID Context, PVOID Buffer, ULONG Size, ULONG FileInformationClass, PULONG_PTR pReturned);
NTSTATUS VfatSetFileInfo(PVOID Context, PVOID Buffer, ULONG Size, ULONG FileInformationClass);
NTSTATUS VfatQueryDirectory(PVOID Context, PCHAR Buffer, ULONG Size, ULONG FileInfoClass, ULONG Index, ULONG Flags, PULONG_PTR pReturned);
NTSTATUS VfatQueryFsInfo(PVOID* pFsInfo, PULONG	pInfoLength);


