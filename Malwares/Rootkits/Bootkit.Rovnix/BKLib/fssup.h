//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK driver loader project. Version 2.9.2.
//	
// module: fssup.h
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description: 
//	Disk and file systems support library.


// BIOS constants
#define		BIOS_DEFAULT_SECTOR_SIZE			0x200		// 512 bytes
#define		BIOS_MBR_MAGIC						0xaa55
#define		BIOS_PARTITION_TABLE_OFFSET			0x1be
#define		BIOS_MAX_PARTITION_COUNT			4
#define		BIOS_PARTITION_ACTIVE_FLAG			0x80
#define		BIOS_PARTITION_TYPE_INSTALLABLE		7

// BIOS structures
typedef struct _PARTITION_TABLE_ENTRY
{
	ULONG		ActiveFlag		: 8;
	ULONG		CHSStartSector	: 24;
	ULONG		Descriptor		: 8;
	ULONG		CHSEndSector	: 24;
	ULONG		LBAStartSector;
	ULONG		PartitionSize;	
}PARTITION_TABLE_ENTRY, *PPARTITION_TABLE_ENTRY;


typedef	struct	_PARTITION_TABLE	
{
	PARTITION_TABLE_ENTRY	Entry[BIOS_MAX_PARTITION_COUNT];
}PARTITION_TABLE, *PPARTITION_TABLE;


#pragma pack (push)
#pragma pack(1)

// BIOS Parameter block
typedef	struct _BPB
{
	USHORT		SectorSize;
	UCHAR		SectorsPerCluster;
	USHORT		RecervedSectors;
	UCHAR		NumberOfFats;			// for FAT only
	union {
		USHORT		MaxRDEntries;		// for FAT only
		USHORT		FirstRDSector;		// for VFAT
	};
	USHORT		SmallSectorsCount;		// for FAT12/16 only
	UCHAR		MediaDescriptorId;		
	USHORT		SectorsPerFat;			// for FAT only
	USHORT		SectorsPerTrack;
	USHORT		NumberOfHeads;
	ULONG		NumberOfHiddenSectors;
	ULONG		FatTotalSectors;		// for FAT only
	union {
		ULONG		DriveNumber;
		ULONG		RDSize;				// for VFAT
	};
	ULONGLONG	VolumeTotalSectors;		// NTFS only
	ULONGLONG	MftStartingCluster;		// NTFS only
	ULONGLONG	MftMirrowCluster;		// NTFS only
	LONG		ClustersPerRecord;		// NTFS only
	ULONG		ClustersPerIndex;		// NTFS only
	ULONGLONG	VolumeSerialNumber;		// NTFS only
	ULONG		Checksum;
} BPB, *PBPB;

// Volume Boot Record
typedef struct	_VBR
{
	CHAR	JumpInstruction[3];
	CHAR	VolumeOemId[8];
	BPB		Bpb;
} VBR, *PVBR;

#pragma pack(pop)

#define		NTFS_OEM_ID				"NTFS    "
#define		NTFS_LOADER_SIZE		16		// sectors
#define		NTFS_LDR_HEADER_SIZE	0x20	// bytes
