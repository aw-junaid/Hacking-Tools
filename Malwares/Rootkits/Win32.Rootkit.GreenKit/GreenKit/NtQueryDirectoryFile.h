#ifndef NTQUERYDIRECTORYFILE_H
#define NTQUERYDIRECTORYFILE_H

typedef enum _FILE_INFORMATION_CLASS
/*{
	FileDirectoryInformation = 1,
	FileFullDirectoryInformation, // 2
	FileBothDirectoryInformation, // 3
	FileBasicInformation,         // 4
	FileStandardInformation,      // 5
	FileInternalInformation,      // 6
	FileEaInformation,            // 7
	FileAccessInformation,        // 8
	FileNameInformation,          // 9
	FileRenameInformation,        // 10
	FileLinkInformation,          // 11
	FileNamesInformation,         // 12
	FileDispositionInformation,   // 13
	FilePositionInformation,      // 14
	FileFullEaInformation,        // 15
	FileModeInformation,          // 16
	FileAlignmentInformation,     // 17
	FileAllInformation,           // 18
	FileAllocationInformation,    // 19
	FileEndOfFileInformation,     // 20
	FileAlternateNameInformation, // 21
	FileStreamInformation,        // 22
	FilePipeInformation,          // 23
	FilePipeLocalInformation,     // 24
	FilePipeRemoteInformation,    // 25
	FileMailslotQueryInformation, // 26
	FileMailslotSetInformation,   // 27
	FileCompressionInformation,   // 28
	FileObjectIdInformation,      // 29
	FileCompletionInformation,    // 30
	FileMoveClusterInformation,   // 31
	FileInformationReserved32,    // 32
	FileInformationReserved33,    // 33
	FileNetworkOpenInformation,   // 34
	FileAttributeTagInformation,    // 35
	FileTrackingInformation,        // 36
	FileIdBothDirectoryInformation,     // 37
	FileIdFullDirectoryInformation, // 38
	FileValidDataLengthInformation, // 39
	FileShortNameInformation,       // 40
	FileMaximumInformation

} FILE_INFORMATION_CLASS, */*PFILE_INFORMATION_CLASS;

typedef struct _FILE_DIRECTORY_INFORMATION
{
	ULONG			NextEntryOffset;
	ULONG			FileIndex;
	LARGE_INTEGER	CreationTime;
	LARGE_INTEGER	LastAccessTime;
	LARGE_INTEGER	LastWriteTime;
	LARGE_INTEGER	ChangeTime;
	LARGE_INTEGER	EndOfFile;
	LARGE_INTEGER	AllocationSize;
	ULONG			FileAttributes;
	ULONG			FileNameLength;
	WCHAR			FileName[1];
} FILE_DIRECTORY_INFORMATION, *PFILE_DIRECTORY_INFORMATION;

typedef struct _FILE_FULL_DIR_INFORMATION
{
	ULONG			NextEntryOffset;
	ULONG			FileIndex;
	LARGE_INTEGER	CreationTime;
	LARGE_INTEGER	LastAccessTime;
	LARGE_INTEGER	LastWriteTime;
	LARGE_INTEGER	ChangeTime;
	LARGE_INTEGER	EndOfFile;
	LARGE_INTEGER	AllocationSize;
	ULONG			FileAttributes;
	ULONG			FileNameLength;
	ULONG			EaSize;
	WCHAR			FileName[1];
} FILE_FULL_DIR_INFORMATION, *PFILE_FULL_DIR_INFORMATION;

typedef struct _FILE_BOTH_DIR_INFORMATION
{
	ULONG			NextEntryOffset;
	ULONG			FileIndex;
	LARGE_INTEGER	CreationTime;
	LARGE_INTEGER	LastAccessTime;
	LARGE_INTEGER	LastWriteTime;
	LARGE_INTEGER	ChangeTime;
	LARGE_INTEGER	EndOfFile;
	LARGE_INTEGER	AllocationSize;
	ULONG			FileAttributes;
	ULONG			FileNameLength;
	ULONG			EaSize;
	UCHAR			ShortNameLength;
	WCHAR			ShortName[12];
	WCHAR			FileName[1];
} FILE_BOTH_DIR_INFORMATION, *PFILE_BOTH_DIR_INFORMATION;

typedef struct _FILE_ID_BOTH_DIR_INFORMATION {
	ULONG NextEntryOffset;
	ULONG FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG FileAttributes;
	ULONG FileNameLength;
	ULONG EaSize;
	CCHAR ShortNameLength;
	WCHAR ShortName[12];
	LARGE_INTEGER FileId;
	WCHAR FileName[1];
} FILE_ID_BOTH_DIR_INFORMATION, *PFILE_ID_BOTH_DIR_INFORMATION;

typedef struct _FILE_ID_FULL_DIR_INFORMATION {
	ULONG NextEntryOffset;
	ULONG FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG FileAttributes;
	ULONG FileNameLength;
	ULONG EaSize;
	LARGE_INTEGER FileId;
	WCHAR FileName[1];
} FILE_ID_FULL_DIR_INFORMATION, *PFILE_ID_FULL_DIR_INFORMATION;
typedef struct _FILE_NAMES_INFORMATION
{
	ULONG	NextEntryOffset;
	ULONG	FileIndex;
	ULONG	FileNameLength;
	WCHAR	FileName[1];
} FILE_NAMES_INFORMATION, *PFILE_NAMES_INFORMATION;
#endif