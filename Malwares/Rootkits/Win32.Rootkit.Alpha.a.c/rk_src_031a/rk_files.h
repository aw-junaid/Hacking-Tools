
#ifndef __FILESH__
#define __FILESH__

/* HOOK STUPH for Drives */
typedef struct {
   PDEVICE_OBJECT   FileSystem;
   unsigned         LogicalDrive;
} HOOK_EXTENSION, *PHOOK_EXTENSION;  

void cmdHookAllDrives();
BOOLEAN cmdHookDrive( IN char Drive, IN PDRIVER_OBJECT DriverObject );

#endif
