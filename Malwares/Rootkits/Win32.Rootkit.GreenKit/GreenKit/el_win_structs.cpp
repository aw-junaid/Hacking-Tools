// el_win_structs.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "./el_win_structs.h"

typedef LONG(WINAPI *NTQIP)(HANDLE, PMY_SYSTEM_PROCESS_INFORMATION, PVOID, ULONG, PULONG);

PELPEB EL_GetPeb()
{
	ELPEB *peb = (ELPEB *) 0x7ffdf000;
	
	ELPROCESS_BASIC_INFORMATION tPbi;

	NTQIP NtQueryInfoProcess = NULL;

	DWORD tSize;

	HMODULE tNtDll = LoadLibrary( "ntdll.dll" );

	if( tNtDll == NULL )
		return NULL;

	NtQueryInfoProcess = (NTQIP) GetProcAddress( tNtDll, "NtQueryInformationProcess" );

	if( NtQueryInfoProcess == NULL )
		return NULL;

	HANDLE tProc = GetCurrentProcess();
	
	NtQueryInfoProcess( tProc, 0, &tPbi, sizeof( tPbi ), &tSize );
	
	peb = (ELPEB *) tPbi.PebBaseAddress;

	return peb;
}

DWORD EL_PrintModules( PELPEB peb )
{
	if( peb == NULL )
	{
		return 0;
	}
	
	//printf( "Image Base: 0x%x\n", peb->ImageBaseAddress );
	//printf( "Current Dir: %s\n", peb->ProcessParameters->CurrentDirectoryPath.Buffer ); 

	//walk module list
	//load order
	//printf( "Load Order\n" );
	PELLIST_ENTRY tLinkEntry = (PELLIST_ENTRY)&peb->LoaderData->InLoadOrderModuleList;
	PELLIST_ENTRY tHead = tLinkEntry;
	do
	{
		tLinkEntry = tLinkEntry->Flink;

		ELLDR_MODULE *tTmp = (ELLDR_MODULE *)tLinkEntry;

		//wprintf( L"\tMODULE: %s\n", tTmp->BaseDllName.Buffer );

	}while( tLinkEntry->Flink != tHead );
	
	//printf( "Init Order\n" );
	tLinkEntry = (PELLIST_ENTRY)&peb->LoaderData->InInitializationOrderModuleList;
	tHead = tLinkEntry;
	
	do
	{
		tLinkEntry = tLinkEntry->Flink;

		ELLDR_MODULE *tTmp = (ELLDR_MODULE *)( (DWORD)tLinkEntry - sizeof( ELLIST_ENTRY ) * 2 );

		//wprintf( L"\tMODULE: %s\n", tTmp->BaseDllName.Buffer );

	}while( tLinkEntry->Flink != tHead );
	
	//printf( "Memory Order\n" );
	tLinkEntry = (PELLIST_ENTRY)&peb->LoaderData->InMemoryOrderModuleList;
	tHead = tLinkEntry;
	
	do
	{
		tLinkEntry = tLinkEntry->Flink;

		ELLDR_MODULE *tTmp = (ELLDR_MODULE *)( (DWORD)tLinkEntry - sizeof( ELLIST_ENTRY ) );

		//wprintf( L"\tMODULE: %s\n", tTmp->BaseDllName.Buffer );

	}while( tLinkEntry->Flink != tHead );

	return 1;
}

DWORD EL_HideModule( PELPEB peb, const wchar_t *dll )
{
	if( peb == NULL || dll == NULL )
	{
		return 0;
	}

	//walk module list
	//load order
	DWORD tOffSet = 0x00;
	PELLIST_ENTRY tLinkEntry = (PELLIST_ENTRY)&peb->LoaderData->InLoadOrderModuleList;
	PELLIST_ENTRY tHead = tLinkEntry;
	do
	{
		tLinkEntry = tLinkEntry->Flink;

		ELLDR_MODULE *tTmp = (ELLDR_MODULE *)tLinkEntry;

		if( wcsstr( tTmp->BaseDllName.Buffer, dll ) != 0 )
		{
			tLinkEntry->Blink->Flink = tLinkEntry->Flink;
			tLinkEntry->Flink->Blink = tLinkEntry->Blink;

			tTmp->HashTableEntry.Blink->Flink = tTmp->HashTableEntry.Flink;
			tTmp->HashTableEntry.Flink->Blink = tTmp->HashTableEntry.Blink;
			break;
		}

	}while( tLinkEntry->Flink != tHead );

	tLinkEntry = (PELLIST_ENTRY)&peb->LoaderData->InInitializationOrderModuleList;
	tHead = tLinkEntry;
	
	do
	{
		tLinkEntry = tLinkEntry->Flink;

		ELLDR_MODULE *tTmp = (ELLDR_MODULE *)( (DWORD)tLinkEntry - sizeof( ELLIST_ENTRY ) * 2 );

		if( wcsstr( tTmp->BaseDllName.Buffer, dll ) != 0 )
		{
			tLinkEntry->Blink->Flink = tLinkEntry->Flink;
			tLinkEntry->Flink->Blink = tLinkEntry->Blink;
			break;
		}

	}while( tLinkEntry->Flink != tHead );

	tLinkEntry = (PELLIST_ENTRY)&peb->LoaderData->InMemoryOrderModuleList;
	tHead = tLinkEntry;
	
	do
	{
		tLinkEntry = tLinkEntry->Flink;

		ELLDR_MODULE *tTmp = (ELLDR_MODULE *)( (DWORD)tLinkEntry - sizeof( ELLIST_ENTRY ) );

		if( wcsstr( tTmp->BaseDllName.Buffer, dll ) != 0 )
		{
			tLinkEntry->Blink->Flink = tLinkEntry->Flink;
			tLinkEntry->Flink->Blink = tLinkEntry->Blink;

			memset( tTmp->FullDllName.Buffer, 0, tTmp->FullDllName.Length );
			memset( tTmp, 0, sizeof( ELLDR_MODULE ) );

			break;
		}

	}while( tLinkEntry->Flink != tHead );

	return 1;
}