//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP (NT-kernel TCP/IP implementation library)
//	
// module: kiplib.h
// $Revision: 79 $
// $Date: 2012-01-18 12:59:39 +0400 (Ср, 18 янв 2012) $
// description: 
//  KIP library startup, initialization and cleanup routines.

#pragma once

NTSTATUS 
	KipInitialize(
		VOID
		);

NTSTATUS
	KipStartup(
		IN PDRIVER_OBJECT  DriverObject,
		IN PUNICODE_STRING RegistryPath
		);

VOID 
	KipCleanup(
		VOID
		);