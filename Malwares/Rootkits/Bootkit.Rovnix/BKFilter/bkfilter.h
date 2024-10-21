//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BkFilter project
//	
// module: bkfilter.h
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description: 
//	Disk filter driver. 
//	Constants, structures and definitions.


typedef struct _FLT_COMPLETION_CONTEXT
{
	ULONGLONG	StartBlock;
	ULONG		NumberBlocks;
	PCHAR		DataBuffer;
	ULONG		Length;
	PVOID		CompletionRoutine;
	PVOID		CompletionContext;
	UCHAR		Control;
} FLT_COMPLETION_CONTEXT, *PFLT_COMPLETION_CONTEXT;


NTSTATUS FltStartup(IN PBK_FS_AREA FsArea);
VOID FltCleanup(VOID);
NTSTATUS FltReadWriteSectors(IN ULONG MajorFunction, IN PVOID Buffer, IN ULONG SectorOffset, IN ULONG SectorCount);
NTSTATUS FltCheckRestoreBoot(VOID);	

#define	FltReadSectors(Buffer, StartSector, NumberSectors)	FltReadWriteSectors(IRP_MJ_READ, Buffer, StartSector, NumberSectors)
#define	FltWriteSectors(Buffer, StartSector, NumberSectors)	FltReadWriteSectors(IRP_MJ_WRITE, Buffer, StartSector, NumberSectors)
