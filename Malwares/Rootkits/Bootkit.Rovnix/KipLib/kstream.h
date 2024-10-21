// ---- Linked buffer ------------------------------------------------------------------------------------------------------------

#define		LBUFFER_SIGN_LENGTH		16	// bytes
#define		LBUFFER_DATA_LENGTH		0	// undefined


// Stream buffer
typedef struct _LBUFFER
{
#ifdef DBG
	ULONG		Magic;
#endif
	LIST_ENTRY	Entry;
	ULONG		Size;
	CHAR		Sign[LBUFFER_SIGN_LENGTH];
	CHAR		Data[LBUFFER_DATA_LENGTH];
} LBUFFER, *PLBUFFER;

// Size of a stream buffer
#define		KSTREAM_BUFFER_SIZE		0x1000

#ifdef DBG
	#define		TAG_LBUFFER		'fuBL'
#else
	#define		TAG_LBUFFER		0
#endif

#define		LBUFFER_MAGIC	'FUBL'
#define		ASSERT_LBUFFER(x)	ASSERT(x->Magic == LBUFFER_MAGIC)


PLBUFFER	LBufferAllocate(ULONG Size);
VOID		LBufferFree(PLBUFFER lBuffer, PCHAR Data);

//---- Kernel stream ------------------------------------------------------------------------------------------------------------


// Stream
typedef struct _KSTREAM
{
#if DBG
	ULONG		Magic;
#endif
	LIST_ENTRY		BuffersListHead;
	ULONG			Position;
	ULONG volatile	Length;
	KMUTANT			Lock;
	KEVENT			DataReadyEvent;
	NPAGED_LOOKASIDE_LIST	Buffers;
} KSTREAM, *PKSTREAM;

#define		KSTREAM_MAGIC	'RTSK'
#define		ASSERT_KSTREAM(x)	ASSERT(x->Magic == KSTREAM_MAGIC)

#ifdef DBG
	#define		TAG_KSTREAM		'rtSK'
#else
	#define		TAG_KSTREAM		0
#endif

PKSTREAM	KStreamAllocate(VOID);
VOID		KStreamRelease(PKSTREAM	pStream);
BOOLEAN		KStreamWrite(PKSTREAM pStream, PCHAR Data, ULONG Length);
ULONG		KStreamRead(PKSTREAM pStream, PCHAR Data, ULONG Length);
ULONG		KStreamGetLength(PKSTREAM	pStream);
VOID		KStreamClear(PKSTREAM	pStream);


//---- Datagram collector --------------------------------------------------------------------------------------------------------

typedef struct _DSTREAM
{
#if DBG
	ULONG	Magic;
#endif
	LIST_ENTRY		BufferListHead;
	KSPIN_LOCK		Lock;
	KIRQL			OldIrql;
	ULONG volatile	Count;
} DSTREAM, *PDSTREAM;


#define		DSTREAM_MAGIC	'RTSD'
#define		ASSERT_DSTREAM(x)	ASSERT(x->Magic == DSTREAM_MAGIC)

#ifdef DBG
	#define		TAG_DSTREAM		'rtSD'
#else
	#define		TAG_DSTREAM		0
#endif


VOID		DStreamLock(PDSTREAM pStream);
VOID		DStreamUnlock(PDSTREAM pStream);

PDSTREAM	DStreamAllocate(VOID);
VOID		DStreamRelease(PDSTREAM	pStream);
VOID		DStreamPushBuffer(PDSTREAM	pStream, PLBUFFER lBuffer);
ULONG		DStreamGetLength(PDSTREAM	pStream);
VOID		DStreamClear(PDSTREAM	pStream);

BOOLEAN	DStreamPush(PDSTREAM pStream, PCHAR	Data, ULONG	Length);
BOOLEAN	DStreamPop(PDSTREAM pStream, PCHAR	Data, PULONG pLength);