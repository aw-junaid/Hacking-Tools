
#ifndef __KPATCH_H__
#define __KPATCH_H__

/* ________________________________________________________________________________
 . header file goes like this:
 . 1. local prototypes and structs
 . 2. pointer typedefs for original calls
 . 3. prototypes for our trojan calls
 . ________________________________________________________________________________ */


/**********************************************************************************
 * System Service Descriptor Table
 **********************************************************************************/

typedef struct _SRVTABLE {
	PVOID           *ServiceTable;
	ULONG           LowCall;        
	ULONG           HiCall;
	PVOID			*ArgTable;
} SRVTABLE, *PSRVTABLE;

#pragma pack(1)
typedef struct ServiceDescriptorEntry {
	unsigned int *ServiceTableBase;
	unsigned int *ServiceCounterTableBase; //Used only in checked build
	unsigned int NumberOfServices;
	unsigned char *ParamTableBase;
} ServiceDescriptorTableEntry_t, *PServiceDescriptorTableEntry_t;
#pragma pack()

/* build number, so we know which call numbers to use in our hooks */
__declspec(dllimport) ULONG NtBuildNumber;

extern ULONG BuildNumber;

/* ________________________________________________________________________________
 . Pointer to the image of the system service table
 . ------------------------------------------------
 . The SystemServiceTable is a playground.  It is implemented as an array of
 . function pointers.  The function address for every system call is placed
 . in this table.  When interrupt 2Eh fires, the interrupt service routine queries
 . this table to find out which system service function to call.  Every system
 . service has an associated call number, which is translated into the offset into
 . this table.  The same function will usually have a different call number on 
 . different builds of NT.  For example, windows 2000 has many new system services,
 . so the call numbers have changed.  You need to play close attention to this when
 . adding new hooks - else you are going to patch the wrong call and you will be 
 . seeing blue.
 .
 . To reverse engineer a new call number, just take a look at NTDLL.DLL and find 
 . the value that is loaded in EAX right before the call to interrupt 2Eh.  That
 . is the call number.  A pointer to the parameters for the call are placed in EDX.
 . ________________________________________________________________________________ */
__declspec(dllimport)  ServiceDescriptorTableEntry_t KeServiceDescriptorTable;

/* macro for easy system call table manipulation 
 . On x86 the DWORD following the first byte is the system call number, 
 . so we reach into the function and pull the number out. This is
 . dependent only on the Zw* function implementations not changing. */

#define SYSTEMSERVICE(_function)  KeServiceDescriptorTable.ServiceTableBase[ *(PULONG)((PUCHAR)_function+1)]


/* __________________________________________________
 . additional function prototypes local to kpatch 
 . __________________________________________________ */
void	SetupCallNumbers();
VOID	HookSyscalls( void );
VOID	UnhookSyscalls( void );
BOOL	GetProcessName( PCHAR theName );
void	GetProcessNameOffset();

#endif