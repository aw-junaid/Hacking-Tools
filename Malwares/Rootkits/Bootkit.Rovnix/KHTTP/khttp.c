//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KHTTP (NT-kernel HTTP implementation library)
//	
// $Date: 2012-05-04 17:37:22 +0400 (Пт, 04 май 2012) $
// $Rev: 88 $
// description: 
//	Kernel-mode HTTP implemntation over KIP.

#include <ntddk.h>
#define NTSTRSAFE_NO_DEPRECATE
#include <ntstrsafe.h>
#include "..\inc\ntddkex.h"
#include "..\inc\kdbg.h"

#include "inaddr.h"
#include "..\inc\kipapi.h"
#include "khttp.h"

#define	szGet					"GET"
#define	szCRLF					"\r\n"
#define	cCRLF					*(PUSHORT)szCRLF
#define	szCRLFCRLF				"\r\n\r\n"
#define	szTmpRequest			"%s %s HTTP/1.1\r\nHost: %s\r\n"
#define	szTmpUserAgent			"User-Agent: %s\r\n"
#define	szTmpReferer			"Referer: %s\r\n"
#define	szAccept				"Accept: "
#define	szTmpContentLength		"Content-Length: %s\r\n"
#define	cContentLength			"Content-Length"

#define	szTransferEncoding	"Transfer-Encoding"
#define	szChunked			"chunked"


#define	INT_MAX_LENGTH				10		// number of characters in INT_MAX
#define	MAX_HTTP_BUFFER_LENGTH		0x2000	// maximum size of a buffer required to read all HTTP header

#define	HTTP_STATUS_CODE_OFFSET		9	// chars
#define	HTTP_STATUS_CODE_LENGTH		3	// chars	

#define	CHUNK_SIZE_LENGTH			8	// chars 

#pragma warning(disable:4996)	// 'strcpy': This function or variable may be unsafe. 

//	Heap management routines
KHTTP_ALLOC_ROUTINE	KHttpAlloc;
KHTTP_FREE_ROUTINE	KHttpFree;


//
//	Returns a string without starting spaces.
//
PCHAR	strzero(PCHAR Str)
{
	while(Str[0] == ' ')
		Str += 1;

	return(Str);
}


LONG __strtol(PCHAR p, PCHAR* out_p, LONG base)
{
	long v = 0;
	int is_neg = 0;

	while (*p == ' ')
		p++;
	if (*p == '-')
		is_neg = 1, p++;
	else if (*p == '+')
		is_neg = 0;
	if (((base == 16) || (base == 0)) &&
	    ((*p == '0') && ((p[1] == 'x') || (p[1] == 'X'))))
	{
		p += 2;
		base = 16;
	}
	if (base == 0)
	{
		if (*p == '0')
			base = 8;
		else
			base = 10;
	}
	while (1)
	{
		char c = *p;
		if ((c >= '0') && (c <= '9') && (c - '0' < base))
			v = (v * base) + (c - '0');
		else if ((c >= 'a') && (c <= 'z') && (c - 'a' + 10 < base))
			v = (v * base) + (c - 'a' + 10);
		else if ((c >= 'A') && (c <= 'Z') && (c - 'A' + 10 < base))
			v = (v * base) + (c - 'A' + 10);
		else
			break;
		p++;
	}
	if (is_neg)
		v = -v;
	if (out_p) *out_p = (char*)p;
	return v;
}


//
//	Searches specified Headers for the header with specified Name.
//
PCHAR HttpFindHeader(
	LPSTR	Headers,	
	PCHAR	Name,
	PULONG	pSize
	)
{
	PCHAR	aStr = Headers, bStr, FoundStr = NULL;
	ULONG	cLen = (ULONG)strlen(Name);


	while(*(PUSHORT)aStr != cCRLF)
	{
		bStr = strchr(aStr, '\r') + 2;
		if (_strnicmp(aStr, Name, cLen) == 0)
		{
			FoundStr = strzero(aStr + (cLen + 1));
			if (pSize)
				*pSize = (ULONG)(bStr - FoundStr - 2);
			break;
		}
		aStr = bStr;
	}

	return(FoundStr);
}

BOOL	RequestParseHeaders(PKHTTP_REQUEST Request, LPSTR Headers, ULONG Length)
{
	BOOL Ret = TRUE;
	LPSTR	Content;
	ULONG	ContentLen;

	Request->Flags |= CF_READY;

	// Determine content length
	if (Content = HttpFindHeader(Headers, cContentLength, &ContentLen))
	{
		// There is Content-Length specified
		ASSERT(Content[ContentLen] == '\r');
		Content[ContentLen] = 0;
		if (!(Request->ContentLength = __strtol(Content, NULL, 0)))
			Request->Flags &= ~CF_READY;
		Content[ContentLen] = '\r';
		Request->Flags |= CF_LENGTH;
		
	}

	if ((Content = HttpFindHeader(Headers, szTransferEncoding, NULL)) && (!_strnicmp(Content, szChunked, cstrlenA(szChunked))))
	{
		//	There is transfer encoding chunked specified
		Request->Flags |= CF_CHUNKED;
	}

	return(Ret);
}


//
//	Returns pointer to a chunk data and it's size in bytes.
//
PCHAR HttpGetChunk(
	LPSTR	ChunkHeader,	// pointer to chunk header
	PULONG	pChunkSize		// receives size of the chunk
	)
{
	CHAR cStr[CHUNK_SIZE_LENGTH + 1 + 2] = {0};	// 1 for zero, 2 for "0x"
	PCHAR Chunk = NULL, aStr = strstr(ChunkHeader, szCRLF);

	aStr = strstr(ChunkHeader, szCRLF);

	if (aStr == ChunkHeader)
	{
		// Skipping end of the previouse chunk
		ChunkHeader += cstrlenA(szCRLF);
		aStr = strstr(ChunkHeader, szCRLF);
	}


	if (aStr)
	{
		ULONG vSize = (ULONG)(aStr - ChunkHeader);
		if (vSize <= CHUNK_SIZE_LENGTH)
		{
			*(PUSHORT)cStr = 'x0';
			memcpy(&cStr[2], ChunkHeader, vSize);
//			if (StrToIntEx(cStr, STIF_SUPPORT_HEX, &vSize))
			if ((vSize = __strtol(cStr, NULL, 0)) || cStr[2] == '0')
			{
				Chunk = aStr+2;		// skipping "\r\n"
				*pChunkSize = vSize;
			}
		}
	}
	return(Chunk);
}


//
//	Decodes response data.
//
BOOL RequestDecodeData(PKHTTP_REQUEST Request)
{
	BOOL	Ret = FALSE;
	LONG	bLen;
	LPSTR	ChunkData;
	LONG	ChunkSize;

	ASSERT(Request->ReadyLen);

	if (Request->Flags & CF_CHUNKED)
	{
		// Check if we alredy processing a chunk
		if (Request->ContentLength == 0)
		{
			// No chunk being processed, looking for a new one.
			// The buffer may start with a chunk header or a chunk end sequence
			if (ChunkData = HttpGetChunk(Request->ReadyBuffer + Request->ReadyOffset, &ChunkSize))
			{
				// There's a chunk header found within the buffer
				Request->ReadyLen -= (LONG)(ChunkData - (Request->ReadyBuffer + Request->ReadyOffset));
				memcpy(Request->ReadyBuffer + Request->ReadyOffset, ChunkData, Request->ReadyLen);

				if (ChunkSize == 0)
				{
					// NULL-chunk, end of the stream
					Request->ReadyLen = 0;
					Request->Flags &= ~CF_READY;
					Ret = TRUE;
				}
				else if (ChunkSize < Request->ReadyLen)
				{
					// It's a whole chunk within the buffer
					Request->ReadyOffset += ChunkSize;
					Request->ReadyLen -= ChunkSize;
					Ret = RequestDecodeData(Request);
				}
				else
				{
					// There's only a part of chunk data 
					Request->ContentLength = ChunkSize - Request->ReadyLen;
					Ret = TRUE;
				}
			}
			else	// if (ChunkData = HttpGetChunk(Request->ReadyBuffer + Request->ReadyOffset, &ChunkSize))
			{
				// No chunk header found within the buffer, maybe there's only a fragment of it
				if (Request->ChunkBuffer = KHttpAlloc(Request->ReadyLen))
				{
					memcpy(Request->ChunkBuffer, Request->ReadyBuffer + Request->ReadyOffset, Request->ReadyLen);
					Request->ChunkLen = Request->ReadyLen;
					Request->ReadyLen = 0;
					Ret = TRUE;
				}
			}
		}
		else	// if (Request->ContentLength == 0)
		{
			// The buffer contains a part of a chunk being processed
			bLen = min(Request->ContentLength, Request->ReadyLen);
			Request->ContentLength -= bLen;
			Request->ReadyOffset += bLen;
			if (Request->ReadyLen -= bLen)
				// The part of a chunk processed, looking for one more
				Ret = RequestDecodeData(Request);
			else
				Ret = TRUE;						
		}

		Request->ReadyLen += Request->ReadyOffset;
		Request->ReadyOffset = 0;
	}
	else if (Request->Flags & CF_LENGTH)
	{
		// Request has a length specified
		ASSERT(Request->ContentLength > 0);
		ASSERT(Request->ContentLength >= Request->ReadyLen);

		Request->ReadyLen = min(Request->ContentLength, Request->ReadyLen);
		// Cheking if whole request length was downloaded
		if ((Request->ContentLength -= Request->ReadyLen) == 0)
			Request->Flags &= ~CF_READY;
		Ret = TRUE;
	}
	else
		// Request not chunked and has no length specified: downloading until connection is closed.
		Ret = TRUE;

	return(Ret);
}


//
//	Reads response data from a socket and decodes it.
//
BOOL RequestReadDecodeData(
	PKHTTP_REQUEST Request,		// Request to read data from
	BOOL bWait					// TRUE if the function should wait until some data received
	)
{
	BOOL	Ret = FALSE;
	LONG	bRead = 0;
	ASSERT(Request->ReadyLen == 0);
	Request->ReadyBuffer = NULL;

	do	// not a loop
	{
		if (_tioctlsocket(Request->Connection->Socket, FIONREAD, &bRead) == SOCKET_ERROR)
			break;
		
		if (bRead || (bWait && (bRead = MAX_HTTP_BUFFER_LENGTH)))
		{
			if (!(Request->ReadyBuffer = KHttpAlloc(Request->ChunkLen + bRead)))
				break;
			
			if ((bRead = _trecv(Request->Connection->Socket, Request->ReadyBuffer + Request->ChunkLen, bRead, 0)) == SOCKET_ERROR)
				break;
		
			if (bRead == 0)
			{
				// Nothing was read, perfarps the end of stream reached
				Ret = TRUE;
				break;
			}

			Request->ReadyLen = bRead + Request->ChunkLen;
			Request->ReadyOffset = 0;

			if (Request->ChunkLen)
			{
				memcpy(Request->ReadyBuffer, Request->ChunkBuffer, Request->ChunkLen);
				KHttpFree(Request->ChunkBuffer);
				Request->ChunkLen = 0;
			}
			
			if (!(Ret = RequestDecodeData(Request)))
				Request->ReadyLen = 0;

		}	// if (bRead || (bWait && (bRead = MAX_HTTP_BUFFER_LENGTH)))
		else
			// Nothing to read and no waiting is possible
			Ret = TRUE;

	} while(FALSE);

	if (!Ret && Request->ReadyBuffer)
		KHttpFree(Request->ReadyBuffer);

	return(Ret);
}

// ---- KHTTP API ---------------------------------------------------------------------------------------------------------------


BOOL _stdcall KHttpInit(
	__in	KHTTP_ALLOC_ROUTINE	AllocRoutine,
	__in	KHTTP_FREE_ROUTINE	FreeRoutine
	)
{
	KHttpAlloc = AllocRoutine;
	KHttpFree = FreeRoutine;
	return(TRUE);
}


//
//	Initializes, for an application, the use of KHTTP functions and returns a KHTTP-session handle.
//
HANDLE _stdcall KHttpOpen(
  __in_opt  LPCSTR pwszUserAgent,
  __in      ULONG dwAccessType,
  __in      LPCSTR pwszProxyName,
  __in      LPCSTR pwszProxyBypass,
  __in      ULONG dwFlags
)
{
	HANDLE	hSession = NULL;
	PKHTTP_SESSION	pSession;
	SOCKET	s;

	// Checking if KIP driver present by trying to create a KIP-socket
	if ((s = _tsocket(AF_INET, SOCK_STREAM, 0)) != SOCKET_ERROR)
	{
		if (pSession = KHttpAlloc(sizeof(KHTTP_SESSION)))
		{
			pSession->Object.Type = KHTTP_SESSION_TYPE;
			pSession->Object.Flags = dwFlags;
			if (pSession->UserAgent = KHttpAlloc(strlen(pwszUserAgent) + sizeof(CHAR)))
			{
				strcpy(pSession->UserAgent, pwszUserAgent);
				hSession = (HANDLE)pSession;
			}
			else
				KHttpFree(pSession);
		}	// if (pSession = Alloc(sizeof(KHTTP_SESSION)))
		_tclosesocket(s);
	}	// if ((s = _tsocket(AF_INET, SOCK_STREAM, 0)) != SOCKET_ERROR)
	UNREFERENCED_PARAMETER(dwAccessType);
	UNREFERENCED_PARAMETER(pwszProxyName);
	UNREFERENCED_PARAMETER(pwszProxyBypass);

	return(hSession);
}


//
//	Closes a single KHTTP-specific handle.
//
BOOL _stdcall KHttpCloseHandle(
	__in	HANDLE Handle
)
{
	BOOL	Ret = FALSE;
	PKHTTP_OBJECT	pObject = (PKHTTP_OBJECT)Handle;

	switch(pObject->Type)
	{
	case KHTTP_SESSION_TYPE:
		{
			PKHTTP_SESSION	Session = CONTAINING_RECORD(pObject, KHTTP_SESSION, Object);
			if (Session->UserAgent)
				KHttpFree(Session->UserAgent);
			KHttpFree(Session);
			Ret = TRUE;
		}
		break;
	case KHTTP_CONNECTION_TYPE:
		{
			PKHTTP_CONNECTION	Connection = CONTAINING_RECORD(pObject, KHTTP_CONNECTION, Object);
			_tshutdown(Connection->Socket, 2);
			_tclosesocket(Connection->Socket);
			KHttpFree(Connection->HostName);
			KHttpFree(Connection);
			Ret = TRUE;
		}
		break;
	case KHTTP_REQUEST_TYPE:
		{
			PKHTTP_REQUEST	Request = CONTAINING_RECORD(pObject, KHTTP_REQUEST, Object);
			KHttpFree(Request->MainHeaders);
			if (Request->OtherHeaders)
				KHttpFree(Request->OtherHeaders);
			KHttpFree(Request);
			Ret = TRUE;
		}
	default:
		break;
	}	// switch(pObject->Type)

	return(Ret);
}


//
//	Specifies the initial target server of an HTTP request and returns a connection handle to 
//	 an HTTP session for that initial target.
//
HANDLE _stdcall KHttpConnect(
	__in        HANDLE hSession,
	__in        LPCSTR pswzServerName,
	__in        ULONG nServerPort,
	__reserved  ULONG dwReserved
)
{
	HANDLE	hConnect = NULL;
	PKHTTP_CONNECTION	Connection = NULL;
	SOCKADDR_IN	Sin = {0};
	struct	hostent*	pHostEnt;
	ULONG	Timeout = KHTTP_CONNECTION_TIMEOUT;

	do	// not a loop
	{
		if (!(pHostEnt = _tgethostbyname(pswzServerName)) || pHostEnt->h_addrtype != AF_INET || pHostEnt->h_length != sizeof(ULONG))
			break;

		if (!(Connection = KHttpAlloc(sizeof(KHTTP_CONNECTION))))
			break;

		memset(Connection, 0, sizeof(KHTTP_CONNECTION));

		Connection->Object.Type = KHTTP_CONNECTION_TYPE;
		Connection->Session = (PKHTTP_SESSION)hSession;

		if (!(Connection->HostName = KHttpAlloc(strlen(pswzServerName) + sizeof(CHAR))))
			break;
		
		strcpy(Connection->HostName, pswzServerName);

		// Creating HTTP connection socket
		if (!(Connection->Socket = _tsocket(AF_INET, SOCK_STREAM, 0)))
			break;

		Sin.sin_family = AF_INET;
		Sin.sin_port = htons((USHORT)nServerPort);
		Sin.sin_addr.S_un.S_addr = *(PULONG)(*pHostEnt->h_addr_list);

		// Setting socket send and receive timeout values
		if (_tsetsockopt(Connection->Socket, SOL_SOCKET, SO_RCVTIMEO, (PCHAR)&Timeout, sizeof(ULONG)))
			break;

		// Connecting to the host
		if (_tconnect(Connection->Socket, (SOCKADDR*)&Sin, sizeof(SOCKADDR)))
			break;

		hConnect = (HANDLE)Connection;

	} while(FALSE);

	if (!hConnect && Connection)
	{
		// An error occurred, performing cleanup
		if (Connection->Socket)
			_tclosesocket(Connection->Socket);

		if (Connection->HostName)
			KHttpFree(Connection->HostName);

		KHttpFree(Connection);
	}	// if ((!hConnect) && Connection)
		
	UNREFERENCED_PARAMETER(dwReserved);

	return(hConnect);
}


//
//	Creates an HTTP request handle.
//
HANDLE _stdcall KHttpOpenRequest(
	__in  HANDLE hConnect,
	__in  LPCSTR pwszVerb,
	__in  LPCSTR pwszObjectName,
	__in  LPCSTR pwszVersion,
	__in  LPCSTR pwszReferrer,
	__in  LPCSTR *ppwszAcceptTypes,
	__in  ULONG dwFlags
)
{
	HANDLE	hRequest = NULL;
	PKHTTP_REQUEST		Request = NULL;
	PKHTTP_CONNECTION	Connection = (PKHTTP_CONNECTION)hConnect;
	ULONG	TypeCount = 0, bLen = 0;

	do	// not a loop
	{
		if (!(Request = KHttpAlloc(sizeof(KHTTP_REQUEST))))
			break;

		memset(Request, 0, sizeof(KHTTP_REQUEST));
	
		Request->Object.Type = KHTTP_REQUEST_TYPE;
		Request->Connection = Connection;

		if (!pwszVerb)
			pwszVerb = szGet;

		// Allocating request main string
		Request->MainHeaders = KHttpAlloc(cstrlenA(szTmpRequest) + strlen(pwszVerb) + strlen(pwszObjectName) + strlen(Connection->HostName) + sizeof(CHAR));
		if (!Request->MainHeaders)
			break;
		// Fromating request main string
		sprintf(Request->MainHeaders, szTmpRequest, pwszVerb, pwszObjectName, Connection->HostName);

		// Calculating request headers length
		if (Connection->Session->UserAgent)
			bLen += (ULONG)(cstrlenA(szTmpUserAgent) + strlen(Connection->Session->UserAgent));

		if (pwszReferrer)
			bLen += (ULONG)(cstrlenA(szTmpReferer) + strlen(pwszReferrer));
		
		if (ppwszAcceptTypes)
		{
			while(ppwszAcceptTypes[TypeCount])
			{
				bLen += (ULONG)strlen(ppwszAcceptTypes[TypeCount]);
				TypeCount += 1;
			}

			if (TypeCount && bLen)
				bLen += (ULONG)(cstrlenA(szAccept) + TypeCount + cstrlenA(szCRLF));
		}	// if (ppwszAcceptTypes)

		if (bLen)
		{
			// Allocating request headers
			if (!(Request->OtherHeaders = KHttpAlloc(bLen + sizeof(CHAR))))
				break;
			
			// Formatting request headers
			bLen = 0;

			if (Connection->Session->UserAgent)
				// Formatting "User-Agent"
				bLen += sprintf(Request->OtherHeaders + bLen, szTmpUserAgent, Connection->Session->UserAgent);

			if (pwszReferrer)
				// Formatting "Referer"
				bLen += sprintf(Request->OtherHeaders + bLen, szTmpReferer, pwszReferrer);

			if (TypeCount)
			{
				// Fromatting "Accept"
				strcat(Request->OtherHeaders + bLen, szAccept);
				while(TypeCount)
				{
					strcat(Request->OtherHeaders, ppwszAcceptTypes[TypeCount - 1]);
					if (TypeCount -= 1)
						strcat(Request->OtherHeaders, ",");
				}
				strcat(Request->OtherHeaders, szCRLF);
			}	// if (TypeCount)
		}	// if (bLen)

		hRequest = (HANDLE)Request;

	} while(FALSE);

	if (!hRequest && Request)
	{
		// An error occurred, performing cleanup
		if (Request->MainHeaders)
			KHttpFree(Request->MainHeaders);

		if (Request->OtherHeaders)
			KHttpFree(Request->OtherHeaders);

		KHttpFree(Request);
	}	// if (!hRequest && Request)

	UNREFERENCED_PARAMETER(pwszVersion);
	UNREFERENCED_PARAMETER(dwFlags);

	return(hRequest);
}


//
//	Sends the specified request to the HTTP server.
//
BOOL _stdcall KHttpSendRequest(
	__in      HANDLE hRequest,
	__in_opt  LPCSTR pwszHeaders,
	__in      ULONG dwHeadersLength,
	__in_opt  PVOID lpOptional,
	__in      ULONG dwOptionalLength,
	__in      ULONG dwTotalLength,
	__in      DWORD_PTR dwContext
)
{
	BOOL	Ret = FALSE;
	ULONG	SendLen, bLen = 0;
	PKHTTP_REQUEST		Request = (PKHTTP_REQUEST)hRequest;
	LPSTR	SendBuffer = NULL;
	
	do	// not a loop
	{
		// Calculating send buffer length
		bLen = (ULONG)strlen(Request->MainHeaders);

		if (Request->OtherHeaders)
			bLen += (ULONG)strlen(Request->OtherHeaders);

		if (pwszHeaders && dwHeadersLength)
		{
			if (dwHeadersLength == -1)
				bLen += (ULONG)strlen(pwszHeaders);
			else
				bLen += dwHeadersLength;
		}

		if (dwOptionalLength || dwTotalLength)
			bLen += (cstrlenA(szTmpContentLength) + INT_MAX_LENGTH);

		bLen += cstrlenA(szCRLF) + cstrlenA(szCRLF);

		// Allocating send buffer
		if (!(SendBuffer = KHttpAlloc(bLen + sizeof(CHAR))))
			break;

		// Formatting send buffer
		strcpy(SendBuffer, Request->MainHeaders);

		if (Request->OtherHeaders)
			strcat(SendBuffer, Request->OtherHeaders);

		if (pwszHeaders && dwHeadersLength)
		{
			if (dwHeadersLength == -1)
				strcat(SendBuffer, pwszHeaders);
			else
				strncat(SendBuffer, pwszHeaders, dwHeadersLength);

			if (*(PUSHORT)szCRLF != *(PUSHORT)(SendBuffer + strlen(SendBuffer) - cstrlenA(szCRLF)))
				strcat(SendBuffer, szCRLF);
		}

		if (dwOptionalLength || dwTotalLength)
			sprintf(SendBuffer + strlen(SendBuffer), szTmpContentLength, max(dwOptionalLength, dwTotalLength));

		strcat(SendBuffer, szCRLF);

		SendLen = (ULONG)strlen(SendBuffer);
		ASSERT(SendLen <= bLen);

		// Sending request headers
		if ((bLen = _tsend(Request->Connection->Socket, SendBuffer, SendLen, 0)) == SOCKET_ERROR)
			break;

		if (lpOptional && dwOptionalLength)
			bLen = _tsend(Request->Connection->Socket, lpOptional, dwOptionalLength, 0);

		if (bLen != SOCKET_ERROR)
			Ret = TRUE;		

	} while(FALSE);

	if (SendBuffer)
		KHttpFree(SendBuffer);

	return(Ret);
}


//
// Waits to receive the response to an HTTP request initiated by KHttpSendRequest. 
// When KHttpReceiveResponse completes successfully, the status code and response headers have been received and 
//	are available for the application to inspect using KHttpQueryHeaders. 
// An application must call KHttpReceiveResponse before it can use KHttpQueryDataAvailable and KHttpReadData 
//  to access the response entity body (if any).
//
BOOL _stdcall KHttpReceiveResponse(
	__in        HANDLE hRequest,
	__reserved  PVOID lpReserved
)
{
	BOOL	Ret = FALSE;
	LPSTR	EndHeaders = NULL, ReceiveBuffer = NULL;
	LONG	bRead = 0, bLen;
	PKHTTP_REQUEST		Request = (PKHTTP_REQUEST)hRequest;

	do	// not a loop
	{
		if (!(ReceiveBuffer = KHttpAlloc(MAX_HTTP_BUFFER_LENGTH + sizeof(CHAR))))
			break;

		// Reading data from the socket until we receive complete response headers
		while(bRead < MAX_HTTP_BUFFER_LENGTH)
		{
			bRead = _trecv(Request->Connection->Socket, ReceiveBuffer + bRead, (MAX_HTTP_BUFFER_LENGTH - bRead), 0);
			if (bRead <= 0)
				break;

			ReceiveBuffer[bRead] = 0;
			if (EndHeaders = strstr(ReceiveBuffer, szCRLFCRLF))
				break;
		}

		if (!EndHeaders)
			break;

		// Calculating length of response headers
		bLen = (LONG)(EndHeaders - ReceiveBuffer + cstrlenA(szCRLFCRLF));

		KHttpFree(Request->MainHeaders);
		if (Request->OtherHeaders)
		{
				KHttpFree(Request->OtherHeaders);
				Request->OtherHeaders = NULL;
		}

		if (!(Request->MainHeaders = KHttpAlloc(bLen + sizeof(CHAR))))
			break;

		memcpy(Request->MainHeaders, ReceiveBuffer, bLen);
		Request->MainHeaders[bLen] = 0;
		Request->HeadersLen = bLen;

		if (!RequestParseHeaders(Request, Request->MainHeaders, bLen))
			break;

		if (bRead -= bLen)
		{
			// There is some more data read after the response headers
			if (!(Request->ReadyBuffer = KHttpAlloc(bRead)))
				break;

			memcpy(Request->ReadyBuffer, ReceiveBuffer + bLen, bRead);
			Request->ReadyLen = bRead;

			if (!RequestDecodeData(Request))
			{
				KHttpFree(Request->ReadyBuffer);
				Request->ReadyLen = 0;
				break;
			}
		}

		Ret = TRUE;
		
	} while(FALSE);

	if (ReceiveBuffer)
		KHttpFree(ReceiveBuffer);

	return(Ret);
}

//
//	Retrieves header information associated with the specified HTTP request.
//
BOOL _stdcall KHttpQueryHeaders(
	__in      HANDLE hRequest,
	__in      ULONG dwInfoLevel,
	__in_opt  LPCSTR pwszName,
	__out     PVOID lpBuffer,
	__inout   PULONG lpdwBufferLength,
	__inout   PULONG lpdwIndex
)
{
	BOOL	Ret = FALSE;
	ULONG	bLen;
	PKHTTP_REQUEST		Request = (PKHTTP_REQUEST)hRequest;

	switch(dwInfoLevel)
	{
	case KHTTP_QUERY_RAW_HEADERS_CRLF:
		if (*lpdwBufferLength >= Request->HeadersLen)
		{
			memcpy(lpBuffer, Request->MainHeaders, Request->HeadersLen);
			Ret = TRUE;
		}
		*lpdwBufferLength = Request->HeadersLen;
		break;
	case KHTTP_QUERY_STATUS_CODE:
		if (Request->HeadersLen >= (HTTP_STATUS_CODE_OFFSET + HTTP_STATUS_CODE_LENGTH))
		{
			if (*lpdwBufferLength >= (HTTP_STATUS_CODE_LENGTH + sizeof(CHAR)))
			{
				memcpy(lpBuffer, Request->MainHeaders + HTTP_STATUS_CODE_OFFSET, HTTP_STATUS_CODE_LENGTH);
				*((PCHAR)lpBuffer + HTTP_STATUS_CODE_LENGTH) = 0;
				Ret = TRUE;
			}
			*lpdwBufferLength = (HTTP_STATUS_CODE_LENGTH + sizeof(CHAR));
		}
		break;
	case KHTTP_QUERY_STATUS_TEXT:
		if (Request->HeadersLen >= (HTTP_STATUS_CODE_OFFSET + HTTP_STATUS_CODE_LENGTH + sizeof(CHAR) + sizeof(szCRLF)))
		{
			LPSTR TextEnd = strstr(Request->MainHeaders + HTTP_STATUS_CODE_OFFSET + HTTP_STATUS_CODE_LENGTH + sizeof(CHAR), szCRLF);
			if (TextEnd)
			{
				bLen = (ULONG)(TextEnd - Request->MainHeaders);
				if (*lpdwBufferLength >= bLen)
				{
					memcpy(lpBuffer, Request->MainHeaders + HTTP_STATUS_CODE_OFFSET + HTTP_STATUS_CODE_LENGTH + sizeof(CHAR), bLen);
					Ret = TRUE;
				}
				*lpdwBufferLength = bLen;
			}
		}
		break;
	default:
		break;
	}	// switch(dwInfoLevel)

	UNREFERENCED_PARAMETER(pwszName);
	UNREFERENCED_PARAMETER(lpdwIndex);
	return(Ret);
}


//
//	Returns the amount of data, in bytes, available to be read with KHttpReadData.
//
BOOL _stdcall KHttpQueryDataAvailable(
  __in   HANDLE hRequest,
  __out  PULONG lpdwNumberOfBytesAvailable
)
{
	BOOL	Ret = FALSE;
	PKHTTP_REQUEST		Request = (PKHTTP_REQUEST)hRequest;

	if (!Request->ReadyLen)
	{
		if (Request->Flags & CF_READY)
			Ret = RequestReadDecodeData(Request, FALSE);
		else
			// Whole page content already read
			Ret = TRUE;
	}

	if (Request->ReadyLen)
	{
		*lpdwNumberOfBytesAvailable = Request->ReadyLen;
		Ret = TRUE;
	}
	
	return(Ret);
}


//
//	Reads data from a handle opened by the KHttpOpenRequest function.
//
BOOL _stdcall KHttpReadData(
	__in   HANDLE hRequest,
	__out  PVOID lpBuffer,
	__in   ULONG dwNumberOfBytesToRead,
	__out  PULONG lpdwNumberOfBytesRead
)
{
	BOOL	Ret = FALSE;
	LONG	bRead = 0;
	PKHTTP_REQUEST		Request = (PKHTTP_REQUEST)hRequest;

	if (!Request->ReadyLen)
	{
		if (Request->Flags & CF_READY)
			Ret = RequestReadDecodeData(Request, TRUE);
		else
			// Whole page content already read
			Ret = TRUE;
	}

	if (Request->ReadyLen)
	{
		bRead = min((LONG)dwNumberOfBytesToRead, Request->ReadyLen);
		memcpy(lpBuffer, Request->ReadyBuffer + Request->ReadyOffset, bRead);

		Request->ReadyOffset += bRead;
		if ((Request->ReadyLen -= bRead) == 0)
			KHttpFree(Request->ReadyBuffer);

		Ret = TRUE;
	}

	*lpdwNumberOfBytesRead = bRead;

	return(Ret);
}


