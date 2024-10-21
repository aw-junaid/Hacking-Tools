//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP (NT-kernel TCP/IP implementation library)
//	
// module: kiplib.c
// $Revision: 79 $
// $Date: 2012-01-18 12:59:39 +0400 (Ср, 18 янв 2012) $
// description: 
//  KIP library startup, initialization and cleanup routines.

#include "project.h"
#include "kiplib.h"
#include "ndishook.h"
#include "iodev.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, KipInitialize)
#pragma alloc_text (INIT, KipStartup)
#endif

// ---- Globals -----------------------------------------------------------------------------------------------------------

LONG volatile	g_WorkerEntryCount = 0;

// ---- Internal functions ------------------------------------------------------------------------------------------------



// ---- Startup/Cleanup ---------------------------------------------------------------------------------------------------


//
//	KIP Library initialization routine. Sets all required hooks.
//	Can be called early at the system startup when there are no debugger present and no kernel API avaliable.
//
NTSTATUS	KipInitialize(VOID)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;

	// Initializing hooking engine
	HookInit();

	// set ndis library hooks
	ntStatus = NdisSetHooks();

	return(ntStatus);
}


//
//	KIP Library cleanup routine. Removes hooks, frees resources.
//
VOID	KipCleanup(VOID)
{
	// Removing hooks
	RemoveAllHooks(NULL);
	// Waiting until all hook routines and workers are finished
	WaitForWorkers();
}



NTSTATUS
	KipStartup(
		IN PDRIVER_OBJECT  DriverObject,
		IN PUNICODE_STRING RegistryPath
		)
{
	NTSTATUS	ntStatus = STATUS_SUCCESS;

#if DBG
	HookDump();
#endif
	// initialize ndis library interface
	ntStatus = NDIS_InitializeInterface();
	if (NT_SUCCESS(ntStatus)){
		ntStatus = KipDriverEntry(DriverObject,RegistryPath);

		if (!NT_SUCCESS(ntStatus)){
			NDIS_ReleaseInterface();
		}
	}

	return(ntStatus);
}
