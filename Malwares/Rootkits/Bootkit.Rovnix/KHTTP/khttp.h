//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KHTTP (NT-kernel HTTP implementation library)
//	
// $Date: 2012-05-04 17:37:22 +0400 (Пт, 04 май 2012) $
// $Rev: 88 $
// description: 
//	Kernel-mode HTTP implemntation over KIP.


// By default, all KHTTP functions are performed synchronously. 
// When this flag is set, the caller needs to specify a callback function through KHttpSetStatusCallback.
#define KHTTP_FLAG_ASYNC	1

// Uses the default port for HTTP servers (port 80). 
#define	KHTTP_DEFAULT_HTTP_PORT			80
#define	KHTTP_CONNECTION_TIMEOUT		15000	// milliseconds

	
#define KHTTP_QUERY_STATUS_CODE			19  // special: part of status line
#define KHTTP_QUERY_STATUS_TEXT         20  // special: part of status line
#define KHTTP_QUERY_RAW_HEADERS_CRLF    22  // special: all headers

#define	KHTTP_STATUS_OK					0x00303032	// "200\0"


typedef	PVOID	(_stdcall* KHTTP_ALLOC_ROUTINE)	(ULONG	Bytes);
typedef	VOID	(_stdcall* KHTTP_FREE_ROUTINE)	(PVOID	Mem);


#ifdef __cplusplus
extern "C" {
#endif


BOOL _stdcall KHttpInit(
	__in	KHTTP_ALLOC_ROUTINE	AllocRoutine,
	__in	KHTTP_FREE_ROUTINE	FreeRoutine
	);



//
//	Initializes, for an application, the use of KHTTP functions and returns a KHTTP-session handle.
//
HANDLE _stdcall KHttpOpen(
	__in_opt	LPCSTR pwszUserAgent,
	__in		ULONG dwAccessType,
	__in		LPCSTR pwszProxyName,
	__in		LPCSTR pwszProxyBypass,
	__in		ULONG dwFlags
);


//
//	Closes a single KHTTP-specific handle.
//
BOOL _stdcall KHttpCloseHandle(
	__in	HANDLE Handle
);


//
//	Specifies the initial target server of an HTTP request and returns a connection handle to 
//	 an HTTP session for that initial target.
//
HANDLE _stdcall KHttpConnect(
	__in        HANDLE hSession,
	__in        LPCSTR pswzServerName,
	__in        ULONG nServerPort,
	__reserved  ULONG dwReserved
);


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
);


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
	__in      ULONG_PTR dwContext
);



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
);


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
);


//
//	Returns the amount of data, in bytes, available to be read with KHttpReadData.
//
BOOL _stdcall KHttpQueryDataAvailable(
	__in   HANDLE hRequest,
	__out  PULONG lpdwNumberOfBytesAvailable
);


//
//	Reads data from a handle opened by the KHttpOpenRequest function.
//
BOOL _stdcall KHttpReadData(
	__in   HANDLE hRequest,
	__out  PVOID lpBuffer,
	__in   ULONG dwNumberOfBytesToRead,
	__out  PULONG lpdwNumberOfBytesRead
);


#ifdef __cplusplus
}
#endif



// ---- KHTTP internal --------------------------------------------------------------------------------------------------------

#if (defined(_DEBUG) || defined(DBG))
	#define	_KHTTP_DEBUG		TRUE
#endif


#define	KHTTP_SESSION_TYPE		1
#define	KHTTP_CONNECTION_TYPE	2
#define	KHTTP_REQUEST_TYPE		3


typedef	struct	_KHTTP_OBJECT
{
	ULONG	Type;
	ULONG	Flags;
} KHTTP_OBJECT, *PKHTTP_OBJECT;

typedef	struct	_KHTTP_SESSION
{
	KHTTP_OBJECT	Object;
	LPSTR			UserAgent;
	ULONG			LastErrorValue;
} KHTTP_SESSION, *PKHTTP_SESSION;


typedef	struct	_KHTTP_CONNECTION
{
	KHTTP_OBJECT	Object;
	PKHTTP_SESSION	Session;
	SOCKET			Socket;
	LPSTR			HostName;
} KHTTP_CONNECTION, *PKHTTP_CONNECTION;


typedef struct  _KHTTP_REQUEST
{
	KHTTP_OBJECT		Object;
	PKHTTP_CONNECTION	Connection;
	LPSTR				MainHeaders;
	LPSTR				OtherHeaders;
	LPSTR				ReadyBuffer;	// Buffer containing decoded data ready to return to a user
	LONG volatile		ReadyLen;		// Length of the ready buffer in bytes
	ULONG				ReadyOffset;	// Offset of the data to read within the ready buffer
	LPSTR				ChunkBuffer;	// Buffer containing a part of a chunk header
	ULONG				ChunkLen;		// Length of the chunk buffer in bytes
	ULONG				HeadersLen;
	LONG				ContentLength;
	ULONG				Flags;
} KHTTP_REQUEST, *PKHTTP_REQUEST;

#define	KHTTP_SESSION_MAGIC		'eSHK'
#define	ASSERT_KHTTP_SESSION(x)	ASSERT(x->Magic == KHTTP_SESSION_MAGIC)


// Content type flags
#define		CF_READY	1		// there is more data to read
#define		CF_LENGTH	2		// request has content length specified
#define		CF_CHUNKED	4		// request has transfer encoding chunked specified
