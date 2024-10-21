//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KLoader project
//	
// module: krep.h
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description:
//	Kernel-mode system file replacer.



typedef struct _FILE_DIRECTORY_ENTRY {
#if DBG
	ULONG			Magic;
#endif
	LIST_ENTRY		Entry;
	FILE_DIRECTORY_INFORMATION	FileInfo;
} FILE_DIRECTORY_ENTRY, *PFILE_DIRECTORY_ENTRY;

#define			FILE_DIRECTORY_ENTRY_MAGIC	'nEDF'
#define			ASSERT_FILE_DIRECTORY_ENTRY(x)		ASSERT(x->Magic == FILE_DIRECTORY_ENTRY_MAGIC)

#if DBG
	#define		TAG_FILE_DIRECTORY_ENTRY	FILE_DIRECTORY_ENTRY_MAGIC
#else
	#define		TAG_FILE_DIRECTORY_ENTRY	0
#endif


// Flags
#define		FILES_SCAN_SUBDIRECTORIES	1
#define		FILES_FIND_DIRECTORIES		2


// Constant string length
#define		cstrlenA(x)		(sizeof(x)-1)
#define		cstrlenW(x)		((sizeof(x) / sizeof(WCHAR)) - 1)


ULONG		CheckReplaceFiles(VOID);
