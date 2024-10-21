//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Vitual File System management tool
//	
// module: ntsup.c
// $Revision: 62 $
// $Date: 2012-05-20 18:48:20 +0400 (Вс, 20 май 2012) $
// description:
//  Sample Virtual File System management tool. Native NT API support.

#include <main.h>

//	Returns full path of the main module of the process specified by ProcessId.
BOOL GetProcessImagePath(
	HANDLE	ProcessId, 
	PWCHAR	pImagePath,
	ULONG	PathLength
	)
{
	ULONG		bSize;
	BOOL		Ret = FALSE;
	NTSTATUS	ntStatus;
	HANDLE		hProcess;
	CLIENT_ID	ClientId = {0};
	OBJECT_ATTRIBUTES	oa = {0};

	ClientId.UniqueProcess = ProcessId;
	InitializeObjectAttributes(&oa, NULL, OBJ_CASE_INSENSITIVE, 0, NULL);

	if (NT_SUCCESS(ZwOpenProcess(&hProcess, GENERIC_READ, &oa, &ClientId)))
	{		
		ntStatus = ZwQueryInformationProcess(hProcess, ProcessImageFileName, NULL, 0, &bSize);
		if (ntStatus == STATUS_INFO_LENGTH_MISMATCH)
		{
			PUNICODE_STRING	ProcessFullName = (PUNICODE_STRING)malloc(bSize);
			if (ProcessFullName)
			{
				ntStatus = ZwQueryInformationProcess(hProcess, ProcessImageFileName, ProcessFullName, bSize, &bSize);
				if (NT_SUCCESS(ntStatus))
				{
					if ((ProcessFullName->Length + sizeof(WCHAR)) <= (PathLength * sizeof(WCHAR)))
					{
						memcpy(pImagePath, ProcessFullName->Buffer, ProcessFullName->Length);
						pImagePath[ProcessFullName->Length / sizeof(WCHAR)] = 0;
						Ret = TRUE;
					}
				}	// if (NT_SUCCESS(ntStatus))
				free(ProcessFullName);
			}	// if (ProcessFullName)
		}	// if (ntStatus == STATUS_INFO_LENGTH_MISMATCH)
		ZwClose(hProcess);
	}	// if (NT_SUCCESS(ZwOpenProcess(&hProcess, GENERIC_READ, &oa, &ClientId)))

	return(Ret);
}
