/*++ BUILD Version: 0122    // Increment this if a change has global effects

Copyright (c) 1990-1994  Microsoft Corporation

Module Name:

    ndis.h

Abstract:

    This module defines the structures, macros, and functions available
    to NDIS drivers.

Revision History:

--*/

#if !defined(_NDIS_)
#define _NDIS_


//
// If we're building a miniport on x86, set BINARY_COMPATIBLE so that
// we don't use functions that aren't available on Chicago.
//

#if !defined(BINARY_COMPATIBLE)
#if defined(NDIS_MINIPORT_DRIVER) && defined(_M_IX86)
#define BINARY_COMPATIBLE 1
#else
#define BINARY_COMPATIBLE 0
#endif
#endif

#if !defined(_M_IX86)
#define BINARY_COMPATIBLE 0
#endif

#ifdef _PNP_POWER
#define NDIS40  1
#endif

//
// BEGIN INTERNAL DEFINITIONS
//

#if BINARY_COMPATIBLE

//
// The following internal definitions are included here in order to allow
// the exported NDIS structures, macros, and functions to compile.  They
// must not be used directly by miniport drivers.
//

#define _NTDDK_

#include <ctype.h>  

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

#ifndef NOTHING
#define NOTHING
#endif

#ifndef CRITICAL
#define CRITICAL
#endif

#ifndef ANYSIZE_ARRAY
#define ANYSIZE_ARRAY 1       // winnt
#endif

// begin_winnt

#if defined(_M_MRX000) && !(defined(MIDL_PASS) || defined(RC_INVOKED)) && defined(ENABLE_RESTRICTED)
#define RESTRICTED_POINTER __restrict
#else
#define RESTRICTED_POINTER
#endif

#if defined(_M_MRX000) || defined(_M_ALPHA) || defined(_M_PPC)
#define UNALIGNED __unaligned
#else
#define UNALIGNED
#endif

// end_winnt

#ifndef CONST
#define CONST               const
#endif

// begin_winnt

#if (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS)
#define DECLSPEC_IMPORT __declspec(dllimport)
#else
#define DECLSPEC_IMPORT
#endif

// end_winnt

//
// Void
//

typedef void *PVOID;    // winnt


#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#define NTAPI __stdcall
#else
#define _cdecl
#define NTAPI
#endif

//
// Define API decoration for direct importing system DLL references.
//

#if !defined(_NTSYSTEM_)
#define NTSYSAPI DECLSPEC_IMPORT
#else
#define NTSYSAPI
#endif


//
// Basics
//

#ifndef VOID
#define VOID void
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
#endif

//
// UNICODE (Wide Character) types
//

typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character

typedef WCHAR *PWCHAR;
typedef WCHAR *LPWCH, *PWCH;
typedef CONST WCHAR *LPCWCH, *PCWCH;
typedef WCHAR *NWPSTR;
typedef WCHAR *LPWSTR, *PWSTR;

typedef CONST WCHAR *LPCWSTR, *PCWSTR;

//
// ANSI (Multi-byte Character) types
//
typedef CHAR *PCHAR;
typedef CHAR *LPCH, *PCH;

typedef CONST CHAR *LPCCH, *PCCH;
typedef CHAR *NPSTR;
typedef CHAR *LPSTR, *PSTR;
typedef CONST CHAR *LPCSTR, *PCSTR;

//
// Neutral ANSI/UNICODE types and macros
//
#ifdef  UNICODE                     // r_winnt

#ifndef _TCHAR_DEFINED
typedef WCHAR TCHAR, *PTCHAR;
typedef WCHAR TUCHAR, *PTUCHAR;
#define _TCHAR_DEFINED
#endif /* !_TCHAR_DEFINED */

typedef LPWSTR LPTCH, PTCH;
typedef LPWSTR PTSTR, LPTSTR;
typedef LPCWSTR LPCTSTR;
typedef LPWSTR LP;
#define __TEXT(quote) L##quote      // r_winnt

#else   /* UNICODE */               // r_winnt

#ifndef _TCHAR_DEFINED
typedef char TCHAR, *PTCHAR;
typedef unsigned char TUCHAR, *PTUCHAR;
#define _TCHAR_DEFINED
#endif /* !_TCHAR_DEFINED */

typedef LPSTR LPTCH, PTCH;
typedef LPSTR PTSTR, LPTSTR;
typedef LPCSTR LPCTSTR;
#define __TEXT(quote) quote         // r_winnt

#endif /* UNICODE */                // r_winnt
#define TEXT(quote) __TEXT(quote)   // r_winnt


// end_winnt

typedef double DOUBLE;

typedef struct _QUAD {              // QUAD is for those times we want
    double  DoNotUseThisField;      // an 8 byte aligned 8 byte long structure
} QUAD;                             // which is NOT really a floating point
                                    // number.  Use DOUBLE if you want an FP
                                    // number.

//
// Pointer to Basics
//

typedef SHORT *PSHORT;  // winnt
typedef LONG *PLONG;    // winnt
typedef QUAD *PQUAD;

//
// Unsigned Basics
//

// Tell windef.h that some types are already defined.
#define BASETYPES

typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
typedef QUAD UQUAD;

//
// Pointer to Unsigned Basics
//

typedef UCHAR *PUCHAR;
typedef USHORT *PUSHORT;
typedef ULONG *PULONG;
typedef UQUAD *PUQUAD;

//
// Signed characters
//

typedef signed char SCHAR;
typedef SCHAR *PSCHAR;

#ifndef NO_STRICT
#ifndef STRICT
#define STRICT 1
#endif
#endif

//
// Handle to an Object
//

// begin_winnt

#ifdef STRICT
typedef void *HANDLE;
#define DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
#else
typedef PVOID HANDLE;
#define DECLARE_HANDLE(name) typedef HANDLE name
#endif
typedef HANDLE *PHANDLE;

//
// Flag (bit) fields
//

typedef UCHAR  FCHAR;
typedef USHORT FSHORT;
typedef ULONG  FLONG;

// end_winnt

//
// Low order two bits of a handle are ignored by the system and available
// for use by application code as tag bits.  The remaining bits are opaque
// and used to store a serial number and table index.
//

#define OBJ_HANDLE_TAGBITS  0x00000003L

//
// Cardinal Data Types [0 - 2**N-2)
//

typedef char CCHAR;          // winnt
typedef short CSHORT;
typedef ULONG CLONG;

typedef CCHAR *PCCHAR;
typedef CSHORT *PCSHORT;
typedef CLONG *PCLONG;

//
// NTSTATUS
//

typedef LONG NTSTATUS;
/*lint -e624 */  // Don't complain about different typedefs.   // winnt
typedef NTSTATUS *PNTSTATUS;
/*lint +e624 */  // Resume checking for different typedefs.    // winnt

//
//  Status values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-------------------------+-------------------------------+
//  |Sev|C|       Facility          |               Code            |
//  +---+-+-------------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//

//
// Generic test for success on any status value (non-negative numbers
// indicate success).
//

#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)

//
// Generic test for information on any status value.
//

#define NT_INFORMATION(Status) ((ULONG)(Status) >> 30 == 1)

//
// Generic test for warning on any status value.
//

#define NT_WARNING(Status) ((ULONG)(Status) >> 30 == 2)

//
// Generic test for error on any status value.
//

#define NT_ERROR(Status) ((ULONG)(Status) >> 30 == 3)

// begin_winnt
#define APPLICATION_ERROR_MASK       0x20000000
#define ERROR_SEVERITY_SUCCESS       0x00000000
#define ERROR_SEVERITY_INFORMATIONAL 0x40000000
#define ERROR_SEVERITY_WARNING       0x80000000
#define ERROR_SEVERITY_ERROR         0xC0000000
// end_winnt


//
// __int64 is only supported by 2.0 and later midl.
// __midl is set by the 2.0 midl and not by 1.0 midl.
//

#define _ULONGLONG_
#if (!defined(MIDL_PASS) || defined(__midl)) && (!defined(_M_IX86) || (defined(_INTEGRAL_MAX_BITS) && _INTEGRAL_MAX_BITS >= 64))
typedef __int64 LONGLONG;
typedef unsigned __int64 ULONGLONG;

#define MAXLONGLONG                      (0x7fffffffffffffff)
#else
typedef double LONGLONG;
typedef double ULONGLONG;
#endif

typedef LONGLONG *PLONGLONG;
typedef ULONGLONG *PULONGLONG;

// Update Sequence Number

typedef LONGLONG USN;

#if defined(MIDL_PASS)
typedef struct _LARGE_INTEGER {
#else // MIDL_PASS
typedef union _LARGE_INTEGER {
    struct {
        ULONG LowPart;
        LONG HighPart;
    };
    struct {
        ULONG LowPart;
        LONG HighPart;
    } u;
#endif //MIDL_PASS
    LONGLONG QuadPart;
} LARGE_INTEGER;

typedef LARGE_INTEGER *PLARGE_INTEGER;


#if defined(MIDL_PASS)
typedef struct _ULARGE_INTEGER {
#else // MIDL_PASS
typedef union _ULARGE_INTEGER {
    struct {
        ULONG LowPart;
        ULONG HighPart;
    };
    struct {
        ULONG LowPart;
        ULONG HighPart;
    } u;
#endif //MIDL_PASS
    ULONGLONG QuadPart;
} ULARGE_INTEGER;

typedef ULARGE_INTEGER *PULARGE_INTEGER;


//
// Physical address.
//

typedef LARGE_INTEGER PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS; // windbgkd

//
// Counted String
//

typedef struct _STRING {
    USHORT Length;
    USHORT MaximumLength;
#ifdef MIDL_PASS
    [size_is(MaximumLength), length_is(Length) ]
#endif // MIDL_PASS
    PCHAR Buffer;
} STRING;
typedef STRING *PSTRING;

typedef STRING ANSI_STRING;
typedef PSTRING PANSI_STRING;

typedef STRING OEM_STRING;
typedef PSTRING POEM_STRING;

//
// CONSTCounted String
//

typedef struct _CSTRING {
    USHORT Length;
    USHORT MaximumLength;
    CONST char *Buffer;
} CSTRING;
typedef CSTRING *PCSTRING;

typedef STRING CANSI_STRING;
typedef PSTRING PCANSI_STRING;

//
// Unicode strings are counted 16-bit character strings. If they are
// NULL terminated, Length does not include trailing NULL.
//

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
#ifdef MIDL_PASS
    [size_is(MaximumLength / 2), length_is((Length) / 2) ] USHORT * Buffer;
#else // MIDL_PASS
    PWSTR  Buffer;
#endif // MIDL_PASS
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
#define UNICODE_NULL ((WCHAR)0) // winnt

// begin_ntminiport begin_ntminitape

//
// Boolean
//

typedef UCHAR BOOLEAN;           // winnt
typedef BOOLEAN *PBOOLEAN;       // winnt

// end_ntminiport end_ntminitape

// begin_winnt
//
//  Doubly linked list structure.  Can be used as either a list head, or
//  as link words.
//

typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY * volatile Flink;
   struct _LIST_ENTRY * volatile Blink;
} LIST_ENTRY, *PLIST_ENTRY, *RESTRICTED_POINTER PRLIST_ENTRY;

//
//  Singly linked list structure. Can be used as either a list head, or
//  as link words.
//

typedef struct _SINGLE_LIST_ENTRY {
    struct _SINGLE_LIST_ENTRY *Next;
} SINGLE_LIST_ENTRY, *PSINGLE_LIST_ENTRY;

//
// Constants
//

#define FALSE   0
#define TRUE    1

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif // NULL


//
// Base data structures for OLE support
//

#ifndef GUID_DEFINED
#define GUID_DEFINED

typedef struct _GUID {          // size is 16
    ULONG Data1;
    USHORT Data2;
    USHORT Data3;
    UCHAR Data4[8];
} GUID;

#endif // !GUID_DEFINED

#ifndef __OBJECTID_DEFINED
#define __OBJECTID_DEFINED

typedef struct  _OBJECTID {     // size is 20
    GUID Lineage;
    ULONG Uniquifier;
} OBJECTID;
#endif // !_OBJECTID_DEFINED

//
// Determine if an argument is present by testing the value of the pointer
// to the argument value.
//

#define ARGUMENT_PRESENT(ArgumentPointer)    (\
    (CHAR *)(ArgumentPointer) != (CHAR *)(NULL) )

// begin_winnt begin_ntminiport
//
// Calculate the byte offset of a field in a structure of type type.
//

#define FIELD_OFFSET(type, field)    ((LONG)&(((type *)0)->field))


//
// Calculate the address of the base of the structure given its type, and an
// address of a field within the structure.
//

#define CONTAINING_RECORD(address, type, field) ((type *)( \
                                                  (PCHAR)(address) - \
                                                  (PCHAR)(&((type *)0)->field)))

//
// Interrupt Request Level (IRQL)
//

typedef UCHAR KIRQL;

typedef KIRQL *PKIRQL;


//
// Macros used to eliminate compiler warning generated when formal
// parameters or local variables are not declared.
//
// Use DBG_UNREFERENCED_PARAMETER() when a parameter is not yet
// referenced but will be once the module is completely developed.
//
// Use DBG_UNREFERENCED_LOCAL_VARIABLE() when a local variable is not yet
// referenced but will be once the module is completely developed.
//
// Use UNREFERENCED_PARAMETER() if a parameter will never be referenced.
//
// DBG_UNREFERENCED_PARAMETER and DBG_UNREFERENCED_LOCAL_VARIABLE will
// eventually be made into a null macro to help determine whether there
// is unfinished work.
//

#if ! (defined(lint) || defined(_lint))
#define UNREFERENCED_PARAMETER(P)          (P)
#define DBG_UNREFERENCED_PARAMETER(P)      (P)
#define DBG_UNREFERENCED_LOCAL_VARIABLE(V) (V)

#else // lint or _lint

// Note: lint -e530 says don't complain about uninitialized variables for
// this.  line +e530 turns that checking back on.  Error 527 has to do with
// unreachable code.

#define UNREFERENCED_PARAMETER(P)          \
    /*lint -e527 -e530 */ \
    { \
        (P) = (P); \
    } \
    /*lint +e527 +e530 */
#define DBG_UNREFERENCED_PARAMETER(P)      \
    /*lint -e527 -e530 */ \
    { \
        (P) = (P); \
    } \
    /*lint +e527 +e530 */
#define DBG_UNREFERENCED_LOCAL_VARIABLE(V) \
    /*lint -e527 -e530 */ \
    { \
        (V) = (V); \
    } \
    /*lint +e527 +e530 */

#endif // lint or _lint



typedef union _SLIST_HEADER {
    ULONGLONG Alignment;
    struct {
        SINGLE_LIST_ENTRY Next;
        USHORT Depth;
        USHORT Sequence;
    };
} SLIST_HEADER, *PSLIST_HEADER;

//
// Define fastcall decoration for functions.
//

#if defined(_M_IX86)
#define FASTCALL _fastcall
#else
#define FASTCALL
#endif

//
// Processor modes.
//

typedef CCHAR KPROCESSOR_MODE;

typedef enum _MODE {
    KernelMode,
    UserMode,
    MaximumMode
} MODE;

//
// DPC routine
//

struct _KDPC;

typedef
VOID
(*PKDEFERRED_ROUTINE) (
    IN struct _KDPC *Dpc,
    IN PVOID DeferredContext,
    IN PVOID SystemArgument1,
    IN PVOID SystemArgument2
    );

//
// Define DPC importance.
//
// LowImportance - Queue DPC at end of target DPC queue.
// MediumImportance - Queue DPC at front of target DPC queue.
// HighImportance - Queue DPC at front of target DPC DPC queue and interrupt
//     the target processor if the DPC is targeted and the system is an MP
//     system.
//
// N.B. If the target processor is the same as the processor on which the DPC
//      is queued on, then the processor is always interrupted if the DPC queue
//      was previously empty.
//

typedef enum _KDPC_IMPORTANCE {
    LowImportance,
    MediumImportance,
    HighImportance
} KDPC_IMPORTANCE;

//
// Deferred Procedure Call (DPC) object
//

typedef struct _KDPC {
    CSHORT Type;
    UCHAR Number;
    UCHAR Importance;
    LIST_ENTRY DpcListEntry;
    PKDEFERRED_ROUTINE DeferredRoutine;
    PVOID DeferredContext;
    PVOID SystemArgument1;
    PVOID SystemArgument2;
    PULONG Lock;
} KDPC, *PKDPC, *RESTRICTED_POINTER PRKDPC;

//
// Interprocessor interrupt worker routine function prototype.
//

typedef PULONG PKIPI_CONTEXT;

typedef
VOID
(*PKIPI_WORKER)(
    IN PKIPI_CONTEXT PacketContext,
    IN PVOID Parameter1,
    IN PVOID Parameter2,
    IN PVOID Parameter3
    );

//
// Define interprocessor interrupt performance counters.
//

typedef struct _KIPI_COUNTS {
    ULONG Freeze;
    ULONG Packet;
    ULONG DPC;
    ULONG APC;
    ULONG FlushSingleTb;
    ULONG FlushMultipleTb;
    ULONG FlushEntireTb;
    ULONG GenericCall;
    ULONG ChangeColor;
    ULONG SweepDcache;
    ULONG SweepIcache;
    ULONG SweepIcacheRange;
    ULONG FlushIoBuffers;
    ULONG GratuitousDPC;
} KIPI_COUNTS, *PKIPI_COUNTS;

#if defined(NT_UP)

#define HOT_STATISTIC(a) a

#else

#define HOT_STATISTIC(a) (KeGetCurrentPrcb()->a)

#endif

//
// I/O system definitions.
//
// Define a Memory Descriptor List (MDL)
//
// An MDL describes pages in a virtual buffer in terms of physical pages.  The
// pages associated with the buffer are described in an array that is allocated
// just after the MDL header structure itself.  In a future compiler this will
// be placed at:
//
//      ULONG Pages[];
//
// Until this declaration is permitted, however, one simply calculates the
// base of the array by adding one to the base MDL pointer:
//
//      Pages = (PULONG) (Mdl + 1);
//
// Notice that while in the context of the subject thread, the base virtual
// address of a buffer mapped by an MDL may be referenced using the following:
//
//      Mdl->StartVa | Mdl->ByteOffset
//

typedef struct _MDL {
    struct _MDL *Next;
    CSHORT Size;
    CSHORT MdlFlags;
    struct _EPROCESS *Process;
    PVOID MappedSystemVa;
    PVOID StartVa;
    ULONG ByteCount;
    ULONG ByteOffset;
} MDL, *PMDL;

#define MDL_MAPPED_TO_SYSTEM_VA     0x0001
#define MDL_PAGES_LOCKED            0x0002
#define MDL_SOURCE_IS_NONPAGED_POOL 0x0004
#define MDL_ALLOCATED_FIXED_SIZE    0x0008
#define MDL_PARTIAL                 0x0010
#define MDL_PARTIAL_HAS_BEEN_MAPPED 0x0020
#define MDL_IO_PAGE_READ            0x0040
#define MDL_WRITE_OPERATION         0x0080
#define MDL_PARENT_MAPPED_SYSTEM_VA 0x0100
#define MDL_LOCK_HELD               0x0200
#define MDL_SYSTEM_VA               0x0400
#define MDL_IO_SPACE                0x0800
#define MDL_NETWORK_HEADER          0x1000
#define MDL_MAPPING_CAN_FAIL        0x2000
#define MDL_ALLOCATED_MUST_SUCCEED  0x4000


#define MDL_MAPPING_FLAGS (MDL_MAPPED_TO_SYSTEM_VA     | \
                           MDL_PAGES_LOCKED            | \
                           MDL_SOURCE_IS_NONPAGED_POOL | \
                           MDL_PARTIAL_HAS_BEEN_MAPPED | \
                           MDL_PARENT_MAPPED_SYSTEM_VA | \
                           MDL_LOCK_HELD               | \
                           MDL_SYSTEM_VA               | \
                           MDL_IO_SPACE )

typedef ULONG KSPIN_LOCK;  
//
// Define the I/O bus interface types.
//

typedef enum _INTERFACE_TYPE {
    InterfaceTypeUndefined = -1,
    Internal,
    Isa,
    Eisa,
    MicroChannel,
    TurboChannel,
    PCIBus,
    VMEBus,
    NuBus,
    PCMCIABus,
    CBus,
    MPIBus,
    MPSABus,
    ProcessorInternal,
    InternalPowerBus,
    PNPISABus,
    MaximumInterfaceType
}INTERFACE_TYPE, *PINTERFACE_TYPE;

//
// Define types of bus information.
//

typedef enum _BUS_DATA_TYPE {
    ConfigurationSpaceUndefined = -1,
    Cmos,
    EisaConfiguration,
    Pos,
    CbusConfiguration,
    PCIConfiguration,
    VMEConfiguration,
    NuBusConfiguration,
    PCMCIAConfiguration,
    MPIConfiguration,
    MPSAConfiguration,
    PNPISAConfiguration,
    MaximumBusDataType
} BUS_DATA_TYPE, *PBUS_DATA_TYPE;

//
// Define the DMA transfer widths.
//

typedef enum _DMA_WIDTH {
    Width8Bits,
    Width16Bits,
    Width32Bits,
    MaximumDmaWidth
}DMA_WIDTH, *PDMA_WIDTH;

//
// Define DMA transfer speeds.
//

typedef enum _DMA_SPEED {
    Compatible,
    TypeA,
    TypeB,
    TypeC,
    MaximumDmaSpeed
}DMA_SPEED, *PDMA_SPEED;

//
// If debugging support enabled, define an ASSERT macro that works.  Otherwise
// define the ASSERT macro to expand to an empty expression.
//

#if DBG
NTSYSAPI
VOID
NTAPI
RtlAssert(
    PVOID FailedAssertion,
    PVOID FileName,
    ULONG LineNumber,
    PCHAR Message
    );

#define ASSERT( exp ) \
    if (!(exp)) \
        RtlAssert( #exp, __FILE__, __LINE__, NULL )

#define ASSERTMSG( msg, exp ) \
    if (!(exp)) \
        RtlAssert( #exp, __FILE__, __LINE__, msg )

#else
#define ASSERT( exp )
#define ASSERTMSG( msg, exp )
#endif // DBG

//
//  Doubly-linked list manipulation routines.  Implemented as macros
//  but logically these are procedures.
//

//
//  VOID
//  InitializeListHead(
//      PLIST_ENTRY ListHead
//      );
//

#define InitializeListHead(ListHead) (\
    (ListHead)->Flink = (ListHead)->Blink = (ListHead))

//
//  BOOLEAN
//  IsListEmpty(
//      PLIST_ENTRY ListHead
//      );
//

#define IsListEmpty(ListHead) \
    ((ListHead)->Flink == (ListHead))

//
//  PLIST_ENTRY
//  RemoveHeadList(
//      PLIST_ENTRY ListHead
//      );
//

#define RemoveHeadList(ListHead) \
    (ListHead)->Flink;\
    {RemoveEntryList((ListHead)->Flink)}

//
//  PLIST_ENTRY
//  RemoveTailList(
//      PLIST_ENTRY ListHead
//      );
//

#define RemoveTailList(ListHead) \
    (ListHead)->Blink;\
    {RemoveEntryList((ListHead)->Blink)}

//
//  VOID
//  RemoveEntryList(
//      PLIST_ENTRY Entry
//      );
//

#define RemoveEntryList(Entry) {\
    PLIST_ENTRY _EX_Blink;\
    PLIST_ENTRY _EX_Flink;\
    _EX_Flink = (Entry)->Flink;\
    _EX_Blink = (Entry)->Blink;\
    _EX_Blink->Flink = _EX_Flink;\
    _EX_Flink->Blink = _EX_Blink;\
    }

//
//  VOID
//  InsertTailList(
//      PLIST_ENTRY ListHead,
//      PLIST_ENTRY Entry
//      );
//

#define InsertTailList(ListHead,Entry) {\
    PLIST_ENTRY _EX_Blink;\
    PLIST_ENTRY _EX_ListHead;\
    _EX_ListHead = (ListHead);\
    _EX_Blink = _EX_ListHead->Blink;\
    (Entry)->Flink = _EX_ListHead;\
    (Entry)->Blink = _EX_Blink;\
    _EX_Blink->Flink = (Entry);\
    _EX_ListHead->Blink = (Entry);\
    }

//
//  VOID
//  InsertHeadList(
//      PLIST_ENTRY ListHead,
//      PLIST_ENTRY Entry
//      );
//

#define InsertHeadList(ListHead,Entry) {\
    PLIST_ENTRY _EX_Flink;\
    PLIST_ENTRY _EX_ListHead;\
    _EX_ListHead = (ListHead);\
    _EX_Flink = _EX_ListHead->Flink;\
    (Entry)->Flink = _EX_Flink;\
    (Entry)->Blink = _EX_ListHead;\
    _EX_Flink->Blink = (Entry);\
    _EX_ListHead->Flink = (Entry);\
    }

//
//
//  PSINGLE_LIST_ENTRY
//  PopEntryList(
//      PSINGLE_LIST_ENTRY ListHead
//      );
//

#define PopEntryList(ListHead) \
    (ListHead)->Next;\
    {\
        PSINGLE_LIST_ENTRY FirstEntry;\
        FirstEntry = (ListHead)->Next;\
        if (FirstEntry != NULL) {     \
            (ListHead)->Next = FirstEntry->Next;\
        }                             \
    }


//
//  VOID
//  PushEntryList(
//      PSINGLE_LIST_ENTRY ListHead,
//      PSINGLE_LIST_ENTRY Entry
//      );
//

#define PushEntryList(ListHead,Entry) \
    (Entry)->Next = (ListHead)->Next; \
    (ListHead)->Next = (Entry)


#if defined(_M_MRX000) || defined(_M_ALPHA)
PVOID
_ReturnAddress (
    VOID
    );

#pragma intrinsic(_ReturnAddress)

#define RtlGetCallersAddress(CallersAddress, CallersCaller) \
    *CallersAddress = (PVOID)_ReturnAddress(); \
    *CallersCaller = NULL;
#else
NTSYSAPI
VOID
NTAPI
RtlGetCallersAddress(
    OUT PVOID *CallersAddress,
    OUT PVOID *CallersCaller
    );
#endif


NTSYSAPI
NTSTATUS
NTAPI
RtlUnicodeStringToAnsiString(
    PANSI_STRING DestinationString,
    PUNICODE_STRING SourceString,
    BOOLEAN AllocateDestinationString
    );

#if defined(_M_IX86) || defined(_M_MRX000) || defined(_M_ALPHA)

#if defined(_M_MRX000)
NTSYSAPI
ULONG
NTAPI
RtlEqualMemory (
    CONST VOID *Source1,
    CONST VOID *Source2,
    ULONG Length
    );

#else
#define RtlEqualMemory(Destination,Source,Length) (!memcmp((Destination),(Source),(Length)))
#endif

#define RtlMoveMemory(Destination,Source,Length) memmove((Destination),(Source),(Length))
#define RtlCopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#define RtlFillMemory(Destination,Length,Fill) memset((Destination),(Fill),(Length))
#define RtlZeroMemory(Destination,Length) memset((Destination),0,(Length))

#else // _M_PPC

NTSYSAPI
ULONG
NTAPI
RtlEqualMemory (
    CONST VOID *Source1,
    CONST VOID *Source2,
    ULONG Length
    );

NTSYSAPI
VOID
NTAPI
RtlCopyMemory (
   VOID UNALIGNED *Destination,
   CONST VOID UNALIGNED *Source,
   ULONG Length
   );

NTSYSAPI
VOID
NTAPI
RtlCopyMemory32 (
   VOID UNALIGNED *Destination,
   CONST VOID UNALIGNED *Source,
   ULONG Length
   );

NTSYSAPI
VOID
NTAPI
RtlMoveMemory (
   VOID UNALIGNED *Destination,
   CONST VOID UNALIGNED *Source,
   ULONG Length
   );

NTSYSAPI
VOID
NTAPI
RtlFillMemory (
   VOID UNALIGNED *Destination,
   ULONG Length,
   UCHAR Fill
   );

NTSYSAPI
VOID
NTAPI
RtlZeroMemory (
   VOID UNALIGNED *Destination,
   ULONG Length
   );
#endif
//
// Define kernel debugger print prototypes and macros.
//

VOID
NTAPI
DbgBreakPoint(
    VOID
    );

VOID
NTAPI
DbgBreakPointWithStatus(
    IN ULONG Status
    );

#define DBG_STATUS_CONTROL_C        1
#define DBG_STATUS_SYSRQ            2
#define DBG_STATUS_BUGCHECK_FIRST   3
#define DBG_STATUS_BUGCHECK_SECOND  4
#define DBG_STATUS_FATAL            5

#if DBG

#define KdPrint(_x_) DbgPrint _x_
#define KdBreakPoint() DbgBreakPoint()
#define KdBreakPointWithStatus(s) DbgBreakPointWithStatus(s)

#else

#define KdPrint(_x_)
#define KdBreakPoint()
#define KdBreakPointWithStatus(s)

#endif

#ifndef _DBGNT_
ULONG
_cdecl
DbgPrint(
    PCH Format,
    ...
    );
#endif // _DBGNT_

#if defined(_X86_)

//
// Define maximum size of flush multple TB request.
//

#define FLUSH_MULTIPLE_MAXIMUM 16

//
// Indicate that the i386 compiler supports the pragma textout construct.
//

#define ALLOC_PRAGMA 1
//
// Indicate that the i386 compiler supports the DATA_SEG("INIT") and
// DATA_SEG("PAGE") pragmas
//

#define ALLOC_DATA_PRAGMA 1

//
// Define function decoration depending on whether a driver, a file system,
// or a kernel component is being built.
//

#if (defined(_NTDRIVER_) || defined(_NTDDK_) || defined(_NTIFS_) || defined(_NTHAL_)) && !defined (_BLDR_)

#define NTKERNELAPI DECLSPEC_IMPORT

#else

#define NTKERNELAPI

#endif

//
// Define function decoration depending on whether the HAL or other kernel
// component is being build.
//

#if !defined(_NTHAL_) && !defined(_BLDR_)

#define NTHALAPI DECLSPEC_IMPORT

#else

#define NTHALAPI

#endif


//
// I/O space read and write macros.
//
//  These have to be actual functions on the 386, because we need
//  to use assembler, but cannot return a value if we inline it.
//
//  The READ/WRITE_REGISTER_* calls manipulate I/O registers in MEMORY space.
//  (Use x86 move instructions, with LOCK prefix to force correct behavior
//   w.r.t. caches and write buffers.)
//
//  The READ/WRITE_PORT_* calls manipulate I/O registers in PORT space.
//  (Use x86 in/out instructions.)
//

NTKERNELAPI
UCHAR
READ_REGISTER_UCHAR(
    PUCHAR  Register
    );

NTKERNELAPI
USHORT
READ_REGISTER_USHORT(
    PUSHORT Register
    );

NTKERNELAPI
ULONG
READ_REGISTER_ULONG(
    PULONG  Register
    );

NTKERNELAPI
VOID
READ_REGISTER_BUFFER_UCHAR(
    PUCHAR  Register,
    PUCHAR  Buffer,
    ULONG   Count
    );

NTKERNELAPI
VOID
READ_REGISTER_BUFFER_USHORT(
    PUSHORT Register,
    PUSHORT Buffer,
    ULONG   Count
    );

NTKERNELAPI
VOID
READ_REGISTER_BUFFER_ULONG(
    PULONG  Register,
    PULONG  Buffer,
    ULONG   Count
    );


NTKERNELAPI
VOID
WRITE_REGISTER_UCHAR(
    PUCHAR  Register,
    UCHAR   Value
    );

NTKERNELAPI
VOID
WRITE_REGISTER_USHORT(
    PUSHORT Register,
    USHORT  Value
    );

NTKERNELAPI
VOID
WRITE_REGISTER_ULONG(
    PULONG  Register,
    ULONG   Value
    );

NTKERNELAPI
VOID
WRITE_REGISTER_BUFFER_UCHAR(
    PUCHAR  Register,
    PUCHAR  Buffer,
    ULONG   Count
    );

NTKERNELAPI
VOID
WRITE_REGISTER_BUFFER_USHORT(
    PUSHORT Register,
    PUSHORT Buffer,
    ULONG   Count
    );

NTKERNELAPI
VOID
WRITE_REGISTER_BUFFER_ULONG(
    PULONG  Register,
    PULONG  Buffer,
    ULONG   Count
    );

NTKERNELAPI
UCHAR
READ_PORT_UCHAR(
    PUCHAR  Port
    );

NTKERNELAPI
USHORT
READ_PORT_USHORT(
    PUSHORT Port
    );

NTKERNELAPI
ULONG
READ_PORT_ULONG(
    PULONG  Port
    );

NTKERNELAPI
VOID
READ_PORT_BUFFER_UCHAR(
    PUCHAR  Port,
    PUCHAR  Buffer,
    ULONG   Count
    );

NTKERNELAPI
VOID
READ_PORT_BUFFER_USHORT(
    PUSHORT Port,
    PUSHORT Buffer,
    ULONG   Count
    );

NTKERNELAPI
VOID
READ_PORT_BUFFER_ULONG(
    PULONG  Port,
    PULONG  Buffer,
    ULONG   Count
    );

NTKERNELAPI
VOID
WRITE_PORT_UCHAR(
    PUCHAR  Port,
    UCHAR   Value
    );

NTKERNELAPI
VOID
WRITE_PORT_USHORT(
    PUSHORT Port,
    USHORT  Value
    );

NTKERNELAPI
VOID
WRITE_PORT_ULONG(
    PULONG  Port,
    ULONG   Value
    );

NTKERNELAPI
VOID
WRITE_PORT_BUFFER_UCHAR(
    PUCHAR  Port,
    PUCHAR  Buffer,
    ULONG   Count
    );

NTKERNELAPI
VOID
WRITE_PORT_BUFFER_USHORT(
    PUSHORT Port,
    PUSHORT Buffer,
    ULONG   Count
    );

NTKERNELAPI
VOID
WRITE_PORT_BUFFER_ULONG(
    PULONG  Port,
    PULONG  Buffer,
    ULONG   Count
    );


#define KeFlushIoBuffers(Mdl, ReadOperation, DmaOperation)

//
// i386 Specific portions of mm component
//

//
// Define the page size for the Intel 386 as 4096 (0x1000).
//

#define PAGE_SIZE (ULONG)0x1000

//
// Define the number of trailing zeroes in a page aligned virtual address.
// This is used as the shift count when shifting virtual addresses to
// virtual page numbers.
//

#define PAGE_SHIFT 12L


#endif // defined(_X86_)


#if defined(_MIPS_)

//
// Define maximum size of flush multple TB request.
//

#define FLUSH_MULTIPLE_MAXIMUM 16

//
// Indicate that the MIPS compiler supports the pragma textout construct.
//

#define ALLOC_PRAGMA 1

//
// Define function decoration depending on whether a driver, a file system,
// or a kernel component is being built.
//

#if (defined(_NTDRIVER_) || defined(_NTDDK_) || defined(_NTIFS_) || defined(_NTHAL_)) && !defined (_BLDR_)

#define NTKERNELAPI DECLSPEC_IMPORT

#else

#define NTKERNELAPI

#endif

//
// Define function decoration depending on whether the HAL or other kernel
// component is being build.
//

#if !defined(_NTHAL_) && !defined(_BLDR_)

#define NTHALAPI DECLSPEC_IMPORT

#else

#define NTHALAPI

#endif

//
// Cache and write buffer flush functions.
//

NTKERNELAPI
VOID
KeFlushIoBuffers (
    IN PMDL Mdl,
    IN BOOLEAN ReadOperation,
    IN BOOLEAN DmaOperation
    );

//
// I/O space read and write macros.
//

#define READ_REGISTER_UCHAR(x) \
    *(volatile UCHAR * const)(x)

#define READ_REGISTER_USHORT(x) \
    *(volatile USHORT * const)(x)

#define READ_REGISTER_ULONG(x) \
    *(volatile ULONG * const)(x)

#define READ_REGISTER_BUFFER_UCHAR(x, y, z) {                           \
    PUCHAR registerBuffer = x;                                          \
    PUCHAR readBuffer = y;                                              \
    ULONG readCount;                                                    \
    for (readCount = z; readCount--; readBuffer++, registerBuffer++) {  \
        *readBuffer = *(volatile UCHAR * const)(registerBuffer);        \
    }                                                                   \
}

#define READ_REGISTER_BUFFER_USHORT(x, y, z) {                          \
    PUSHORT registerBuffer = x;                                         \
    PUSHORT readBuffer = y;                                             \
    ULONG readCount;                                                    \
    for (readCount = z; readCount--; readBuffer++, registerBuffer++) {  \
        *readBuffer = *(volatile USHORT * const)(registerBuffer);       \
    }                                                                   \
}

#define READ_REGISTER_BUFFER_ULONG(x, y, z) {                           \
    PULONG registerBuffer = x;                                          \
    PULONG readBuffer = y;                                              \
    ULONG readCount;                                                    \
    for (readCount = z; readCount--; readBuffer++, registerBuffer++) {  \
        *readBuffer = *(volatile ULONG * const)(registerBuffer);        \
    }                                                                   \
}

#define WRITE_REGISTER_UCHAR(x, y) {    \
    *(volatile UCHAR * const)(x) = y;   \
    KeFlushWriteBuffer();               \
}

#define WRITE_REGISTER_USHORT(x, y) {   \
    *(volatile USHORT * const)(x) = y;  \
    KeFlushWriteBuffer();               \
}

#define WRITE_REGISTER_ULONG(x, y) {    \
    *(volatile ULONG * const)(x) = y;   \
    KeFlushWriteBuffer();               \
}

#define WRITE_REGISTER_BUFFER_UCHAR(x, y, z) {                            \
    PUCHAR registerBuffer = x;                                            \
    PUCHAR writeBuffer = y;                                               \
    ULONG writeCount;                                                     \
    for (writeCount = z; writeCount--; writeBuffer++, registerBuffer++) { \
        *(volatile UCHAR * const)(registerBuffer) = *writeBuffer;         \
    }                                                                     \
    KeFlushWriteBuffer();                                                 \
}

#define WRITE_REGISTER_BUFFER_USHORT(x, y, z) {                           \
    PUSHORT registerBuffer = x;                                           \
    PUSHORT writeBuffer = y;                                              \
    ULONG writeCount;                                                     \
    for (writeCount = z; writeCount--; writeBuffer++, registerBuffer++) { \
        *(volatile USHORT * const)(registerBuffer) = *writeBuffer;        \
    }                                                                     \
    KeFlushWriteBuffer();                                                 \
}

#define WRITE_REGISTER_BUFFER_ULONG(x, y, z) {                            \
    PULONG registerBuffer = x;                                            \
    PULONG writeBuffer = y;                                               \
    ULONG writeCount;                                                     \
    for (writeCount = z; writeCount--; writeBuffer++, registerBuffer++) { \
        *(volatile ULONG * const)(registerBuffer) = *writeBuffer;         \
    }                                                                     \
    KeFlushWriteBuffer();                                                 \
}


#define READ_PORT_UCHAR(x) \
    *(volatile UCHAR * const)(x)

#define READ_PORT_USHORT(x) \
    *(volatile USHORT * const)(x)

#define READ_PORT_ULONG(x) \
    *(volatile ULONG * const)(x)

#define READ_PORT_BUFFER_UCHAR(x, y, z) {                             \
    PUCHAR readBuffer = y;                                            \
    ULONG readCount;                                                  \
    for (readCount = 0; readCount < z; readCount++, readBuffer++) {   \
        *readBuffer = *(volatile UCHAR * const)(x);                   \
    }                                                                 \
}

#define READ_PORT_BUFFER_USHORT(x, y, z) {                            \
    PUSHORT readBuffer = y;                                            \
    ULONG readCount;                                                  \
    for (readCount = 0; readCount < z; readCount++, readBuffer++) {   \
        *readBuffer = *(volatile USHORT * const)(x);                  \
    }                                                                 \
}

#define READ_PORT_BUFFER_ULONG(x, y, z) {                             \
    PULONG readBuffer = y;                                            \
    ULONG readCount;                                                  \
    for (readCount = 0; readCount < z; readCount++, readBuffer++) {   \
        *readBuffer = *(volatile ULONG * const)(x);                   \
    }                                                                 \
}

#define WRITE_PORT_UCHAR(x, y) {        \
    *(volatile UCHAR * const)(x) = y;   \
    KeFlushWriteBuffer();               \
}

#define WRITE_PORT_USHORT(x, y) {       \
    *(volatile USHORT * const)(x) = y;  \
    KeFlushWriteBuffer();               \
}

#define WRITE_PORT_ULONG(x, y) {        \
    *(volatile ULONG * const)(x) = y;   \
    KeFlushWriteBuffer();               \
}

#define WRITE_PORT_BUFFER_UCHAR(x, y, z) {                                \
    PUCHAR writeBuffer = y;                                               \
    ULONG writeCount;                                                     \
    for (writeCount = 0; writeCount < z; writeCount++, writeBuffer++) {   \
        *(volatile UCHAR * const)(x) = *writeBuffer;                      \
        KeFlushWriteBuffer();                                             \
    }                                                                     \
}

#define WRITE_PORT_BUFFER_USHORT(x, y, z) {                               \
    PUSHORT writeBuffer = y;                                              \
    ULONG writeCount;                                                     \
    for (writeCount = 0; writeCount < z; writeCount++, writeBuffer++) {   \
        *(volatile USHORT * const)(x) = *writeBuffer;                     \
        KeFlushWriteBuffer();                                             \
    }                                                                     \
}

#define WRITE_PORT_BUFFER_ULONG(x, y, z) {                                \
    PULONG writeBuffer = y;                                               \
    ULONG writeCount;                                                     \
    for (writeCount = 0; writeCount < z; writeCount++, writeBuffer++) {   \
        *(volatile ULONG * const)(x) = *writeBuffer;                      \
        KeFlushWriteBuffer();                                             \
    }                                                                     \
}

//
// Define the page size for the MIPS R4000 as 4096 (0x1000).
//

#define PAGE_SIZE (ULONG)0x1000

//
// Define the number of trailing zeroes in a page aligned virtual address.
// This is used as the shift count when shifting virtual addresses to
// virtual page numbers.
//

#define PAGE_SHIFT 12L

#endif // defined(_MIPS_)

#if defined(_ALPHA_)

//
// Define maximum size of flush multple TB request.
//

#define FLUSH_MULTIPLE_MAXIMUM 16

//
// Indicate that the MIPS compiler supports the pragma textout construct.
//

#define ALLOC_PRAGMA 1


//
// Define function decoration depending on whether a driver, a file system,
// or a kernel component is being built.
//

#if (defined(_NTDRIVER_) || defined(_NTDDK_) || defined(_NTIFS_) || defined(_NTHAL_)) && !defined (_BLDR_)

#define NTKERNELAPI DECLSPEC_IMPORT

#else

#define NTKERNELAPI

#endif

//
// Define function decoration depending on whether the HAL or other kernel
// component is being build.
//

#if !defined(_NTHAL_) && !defined(_BLDR_)

#define NTHALAPI DECLSPEC_IMPORT

#else

#define NTHALAPI

#endif

//
// I/O space read and write macros.
//
//  These have to be actual functions on Alpha, because we need
//  to shift the VA and OR in the BYTE ENABLES.
//
//  These can become INLINEs if we require that ALL Alpha systems shift
//  the same number of bits and have the SAME byte enables.
//
//  The READ/WRITE_REGISTER_* calls manipulate I/O registers in MEMORY space?
//
//  The READ/WRITE_PORT_* calls manipulate I/O registers in PORT space?
//

NTHALAPI
UCHAR
READ_REGISTER_UCHAR(
    PUCHAR Register
    );

NTHALAPI
USHORT
READ_REGISTER_USHORT(
    PUSHORT Register
    );

NTHALAPI
ULONG
READ_REGISTER_ULONG(
    PULONG Register
    );

NTHALAPI
VOID
READ_REGISTER_BUFFER_UCHAR(
    PUCHAR  Register,
    PUCHAR  Buffer,
    ULONG   Count
    );

NTHALAPI
VOID
READ_REGISTER_BUFFER_USHORT(
    PUSHORT Register,
    PUSHORT Buffer,
    ULONG   Count
    );

NTHALAPI
VOID
READ_REGISTER_BUFFER_ULONG(
    PULONG  Register,
    PULONG  Buffer,
    ULONG   Count
    );


NTHALAPI
VOID
WRITE_REGISTER_UCHAR(
    PUCHAR Register,
    UCHAR   Value
    );

NTHALAPI
VOID
WRITE_REGISTER_USHORT(
    PUSHORT Register,
    USHORT  Value
    );

NTHALAPI
VOID
WRITE_REGISTER_ULONG(
    PULONG Register,
    ULONG   Value
    );

NTHALAPI
VOID
WRITE_REGISTER_BUFFER_UCHAR(
    PUCHAR  Register,
    PUCHAR  Buffer,
    ULONG   Count
    );

NTHALAPI
VOID
WRITE_REGISTER_BUFFER_USHORT(
    PUSHORT Register,
    PUSHORT Buffer,
    ULONG   Count
    );

NTHALAPI
VOID
WRITE_REGISTER_BUFFER_ULONG(
    PULONG  Register,
    PULONG  Buffer,
    ULONG   Count
    );

NTHALAPI
UCHAR
READ_PORT_UCHAR(
    PUCHAR Port
    );

NTHALAPI
USHORT
READ_PORT_USHORT(
    PUSHORT Port
    );

NTHALAPI
ULONG
READ_PORT_ULONG(
    PULONG  Port
    );

NTHALAPI
VOID
READ_PORT_BUFFER_UCHAR(
    PUCHAR  Port,
    PUCHAR  Buffer,
    ULONG   Count
    );

NTHALAPI
VOID
READ_PORT_BUFFER_USHORT(
    PUSHORT Port,
    PUSHORT Buffer,
    ULONG   Count
    );

NTHALAPI
VOID
READ_PORT_BUFFER_ULONG(
    PULONG  Port,
    PULONG  Buffer,
    ULONG   Count
    );

NTHALAPI
VOID
WRITE_PORT_UCHAR(
    PUCHAR  Port,
    UCHAR   Value
    );

NTHALAPI
VOID
WRITE_PORT_USHORT(
    PUSHORT Port,
    USHORT  Value
    );

NTHALAPI
VOID
WRITE_PORT_ULONG(
    PULONG  Port,
    ULONG   Value
    );

NTHALAPI
VOID
WRITE_PORT_BUFFER_UCHAR(
    PUCHAR  Port,
    PUCHAR  Buffer,
    ULONG   Count
    );

NTHALAPI
VOID
WRITE_PORT_BUFFER_USHORT(
    PUSHORT Port,
    PUSHORT Buffer,
    ULONG   Count
    );

NTHALAPI
VOID
WRITE_PORT_BUFFER_ULONG(
    PULONG  Port,
    PULONG  Buffer,
    ULONG   Count
    );


//
// Define the page size for the Alpha ev4 and lca as 8k.
//

#define PAGE_SIZE (ULONG)0x2000

//
// Define the number of trailing zeroes in a page aligned virtual address.
// This is used as the shift count when shifting virtual addresses to
// virtual page numbers.
//

#define PAGE_SHIFT 13L

//
// Cache and write buffer flush functions.
//

VOID
KeFlushIoBuffers (
    IN PMDL Mdl,
    IN BOOLEAN ReadOperation,
    IN BOOLEAN DmaOperation
    );

#endif // _ALPHA_

#if defined(_PPC_)

//
// Define maximum size of flush multple TB request.
//

#define FLUSH_MULTIPLE_MAXIMUM 48

//
// Indicate that the compiler (with MIPS front-end) supports
// the pragma textout construct.
//

#define ALLOC_PRAGMA 1

//
// Define function decoration depending on whether a driver, a file system,
// or a kernel component is being built.
//

#if defined(_NTDRIVER_) || defined(_NTDDK_) || defined(_NTIFS_) || defined(_NTHAL_)

#define NTKERNELAPI DECLSPEC_IMPORT

#else

#define NTKERNELAPI

#endif

//
// Define function decoration depending on whether the HAL or other kernel
// component is being build.
//

#if !defined(_NTHAL_)

#define NTHALAPI DECLSPEC_IMPORT

#else

#define NTHALAPI

#endif

//
// Cache and write buffer flush functions.
//

NTKERNELAPI
VOID
KeFlushIoBuffers (
    IN PMDL Mdl,
    IN BOOLEAN ReadOperation,
    IN BOOLEAN DmaOperation
    );


//
// I/O space read and write macros.
//
// **FINISH** Ensure that these are appropriate for PowerPC

#define READ_REGISTER_UCHAR(x) \
    *(volatile UCHAR * const)(x)

#define READ_REGISTER_USHORT(x) \
    *(volatile USHORT * const)(x)

#define READ_REGISTER_ULONG(x) \
    *(volatile ULONG * const)(x)

#define READ_REGISTER_BUFFER_UCHAR(x, y, z) {                           \
    PUCHAR registerBuffer = x;                                          \
    PUCHAR readBuffer = y;                                              \
    ULONG readCount;                                                    \
    for (readCount = z; readCount--; readBuffer++, registerBuffer++) {  \
        *readBuffer = *(volatile UCHAR * const)(registerBuffer);        \
    }                                                                   \
}

#define READ_REGISTER_BUFFER_USHORT(x, y, z) {                          \
    PUSHORT registerBuffer = x;                                         \
    PUSHORT readBuffer = y;                                             \
    ULONG readCount;                                                    \
    for (readCount = z; readCount--; readBuffer++, registerBuffer++) {  \
        *readBuffer = *(volatile USHORT * const)(registerBuffer);       \
    }                                                                   \
}

#define READ_REGISTER_BUFFER_ULONG(x, y, z) {                           \
    PULONG registerBuffer = x;                                          \
    PULONG readBuffer = y;                                              \
    ULONG readCount;                                                    \
    for (readCount = z; readCount--; readBuffer++, registerBuffer++) {  \
        *readBuffer = *(volatile ULONG * const)(registerBuffer);        \
    }                                                                   \
}

#define WRITE_REGISTER_UCHAR(x, y) {    \
    *(volatile UCHAR * const)(x) = y;   \
    KeFlushWriteBuffer();               \
}

#define WRITE_REGISTER_USHORT(x, y) {   \
    *(volatile USHORT * const)(x) = y;  \
    KeFlushWriteBuffer();               \
}

#define WRITE_REGISTER_ULONG(x, y) {    \
    *(volatile ULONG * const)(x) = y;   \
    KeFlushWriteBuffer();               \
}

#define WRITE_REGISTER_BUFFER_UCHAR(x, y, z) {                            \
    PUCHAR registerBuffer = x;                                            \
    PUCHAR writeBuffer = y;                                               \
    ULONG writeCount;                                                     \
    for (writeCount = z; writeCount--; writeBuffer++, registerBuffer++) { \
        *(volatile UCHAR * const)(registerBuffer) = *writeBuffer;         \
    }                                                                     \
    KeFlushWriteBuffer();                                                 \
}

#define WRITE_REGISTER_BUFFER_USHORT(x, y, z) {                           \
    PUSHORT registerBuffer = x;                                           \
    PUSHORT writeBuffer = y;                                              \
    ULONG writeCount;                                                     \
    for (writeCount = z; writeCount--; writeBuffer++, registerBuffer++) { \
        *(volatile USHORT * const)(registerBuffer) = *writeBuffer;        \
    }                                                                     \
    KeFlushWriteBuffer();                                                 \
}

#define WRITE_REGISTER_BUFFER_ULONG(x, y, z) {                            \
    PULONG registerBuffer = x;                                            \
    PULONG writeBuffer = y;                                               \
    ULONG writeCount;                                                     \
    for (writeCount = z; writeCount--; writeBuffer++, registerBuffer++) { \
        *(volatile ULONG * const)(registerBuffer) = *writeBuffer;         \
    }                                                                     \
    KeFlushWriteBuffer();                                                 \
}


#define READ_PORT_UCHAR(x) \
    *(volatile UCHAR * const)(x)

#define READ_PORT_USHORT(x) \
    *(volatile USHORT * const)(x)

#define READ_PORT_ULONG(x) \
    *(volatile ULONG * const)(x)

#define READ_PORT_BUFFER_UCHAR(x, y, z) {                             \
    PUCHAR readBuffer = y;                                            \
    ULONG readCount;                                                  \
    for (readCount = 0; readCount < z; readCount++, readBuffer++) {   \
        *readBuffer = *(volatile UCHAR * const)(x);                   \
    }                                                                 \
}

#define READ_PORT_BUFFER_USHORT(x, y, z) {                            \
    PUSHORT readBuffer = y;                                            \
    ULONG readCount;                                                  \
    for (readCount = 0; readCount < z; readCount++, readBuffer++) {   \
        *readBuffer = *(volatile USHORT * const)(x);                  \
    }                                                                 \
}

#define READ_PORT_BUFFER_ULONG(x, y, z) {                             \
    PULONG readBuffer = y;                                            \
    ULONG readCount;                                                  \
    for (readCount = 0; readCount < z; readCount++, readBuffer++) {   \
        *readBuffer = *(volatile ULONG * const)(x);                   \
    }                                                                 \
}

#define WRITE_PORT_UCHAR(x, y) {        \
    *(volatile UCHAR * const)(x) = y;   \
    KeFlushWriteBuffer();               \
}

#define WRITE_PORT_USHORT(x, y) {       \
    *(volatile USHORT * const)(x) = y;  \
    KeFlushWriteBuffer();               \
}

#define WRITE_PORT_ULONG(x, y) {        \
    *(volatile ULONG * const)(x) = y;   \
    KeFlushWriteBuffer();               \
}

#define WRITE_PORT_BUFFER_UCHAR(x, y, z) {                                \
    PUCHAR writeBuffer = y;                                               \
    ULONG writeCount;                                                     \
    for (writeCount = 0; writeCount < z; writeCount++, writeBuffer++) {   \
        *(volatile UCHAR * const)(x) = *writeBuffer;                      \
        KeFlushWriteBuffer();                                             \
    }                                                                     \
}

#define WRITE_PORT_BUFFER_USHORT(x, y, z) {                               \
    PUSHORT writeBuffer = y;                                              \
    ULONG writeCount;                                                     \
    for (writeCount = 0; writeCount < z; writeCount++, writeBuffer++) {   \
        *(volatile USHORT * const)(x) = *writeBuffer;                     \
        KeFlushWriteBuffer();                                             \
    }                                                                     \
}

#define WRITE_PORT_BUFFER_ULONG(x, y, z) {                                \
    PULONG writeBuffer = y;                                               \
    ULONG writeCount;                                                     \
    for (writeCount = 0; writeCount < z; writeCount++, writeBuffer++) {   \
        *(volatile ULONG * const)(x) = *writeBuffer;                      \
        KeFlushWriteBuffer();                                             \
    }                                                                     \
}

//
// PowerPC page size = 4 KB
//

#define PAGE_SIZE (ULONG)0x1000

//
// Define the number of trailing zeroes in a page aligned virtual address.
// This is used as the shift count when shifting virtual addresses to
// virtual page numbers.
//

#define PAGE_SHIFT 12L

#endif // defined(_PPC_)

//
// Defines the Type in the RESOURCE_DESCRIPTOR
//

typedef enum _CM_RESOURCE_TYPE {
    CmResourceTypeNull = 0,    // Reserved
    CmResourceTypePort,
    CmResourceTypeInterrupt,
    CmResourceTypeMemory,
    CmResourceTypeDma,
    CmResourceTypeDeviceSpecific,
    CmResourceTypeMaximum
} CM_RESOURCE_TYPE;

//
// Defines the ShareDisposition in the RESOURCE_DESCRIPTOR
//

typedef enum _CM_SHARE_DISPOSITION {
    CmResourceShareUndetermined = 0,    // Reserved
    CmResourceShareDeviceExclusive,
    CmResourceShareDriverExclusive,
    CmResourceShareShared
} CM_SHARE_DISPOSITION;

//
// Define the bit masks for Flags when type is CmResourceTypeInterrupt
//

#define CM_RESOURCE_INTERRUPT_LEVEL_SENSITIVE 0
#define CM_RESOURCE_INTERRUPT_LATCHED         1

//
// Define the bit masks for Flags when type is CmResourceTypeMemory
//

#define CM_RESOURCE_MEMORY_READ_WRITE       0x0000
#define CM_RESOURCE_MEMORY_READ_ONLY        0x0001
#define CM_RESOURCE_MEMORY_WRITE_ONLY       0x0002
#define CM_RESOURCE_MEMORY_PREFETCHABLE     0x0004

#define CM_RESOURCE_MEMORY_COMBINEDWRITE    0x0008
#define CM_RESOURCE_MEMORY_24               0x0010

//
// Define the bit masks for Flags when type is CmResourceTypePort
//

#define CM_RESOURCE_PORT_MEMORY 0
#define CM_RESOURCE_PORT_IO 1

//
// Define the bit masks for Flags when type is CmResourceTypeDma
//

#define CM_RESOURCE_DMA_8                   0x0000
#define CM_RESOURCE_DMA_16                  0x0001
#define CM_RESOURCE_DMA_32                  0x0002


#include "pshpack4.h"
typedef struct _CM_PARTIAL_RESOURCE_DESCRIPTOR {
    UCHAR Type;
    UCHAR ShareDisposition;
    USHORT Flags;
    union {

        //
        // Range of port numbers, inclusive. These are physical, bus
        // relative. The value should be the same as the one passed to
        // HalTranslateBusAddress().
        //

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length;
        } Port;

        //
        // IRQL and vector. Should be same values as were passed to
        // HalGetInterruptVector().
        //

        struct {
            ULONG Level;
            ULONG Vector;
            ULONG Affinity;
        } Interrupt;

        //
        // Range of memory addresses, inclusive. These are physical, bus
        // relative. The value should be the same as the one passed to
        // HalTranslateBusAddress().
        //

        struct {
            PHYSICAL_ADDRESS Start;    // 64 bit physical addresses.
            ULONG Length;
        } Memory;

        //
        // Physical DMA channel.
        //

        struct {
            ULONG Channel;
            ULONG Port;
            ULONG Reserved1;
        } Dma;

        //
        // Device Specific information defined by the driver.
        // The DataSize field indicates the size of the data in bytes. The
        // data is located immediately after the DeviceSpecificData field in
        // the structure.
        //

        struct {
            ULONG DataSize;
            ULONG Reserved1;
            ULONG Reserved2;
        } DeviceSpecificData;
    } u;
} CM_PARTIAL_RESOURCE_DESCRIPTOR, *PCM_PARTIAL_RESOURCE_DESCRIPTOR;
#include "poppack.h"

//
// A Partial Resource List is what can be found in the ARC firmware
// or will be generated by ntdetect.com.
// The configuration manager will transform this structure into a Full
// resource descriptor when it is about to store it in the regsitry.
//
// Note: There must a be a convention to the order of fields of same type,
// (defined on a device by device basis) so that the fields can make sense
// to a driver (i.e. when multiple memory ranges are necessary).
//

typedef struct _CM_PARTIAL_RESOURCE_LIST {
    USHORT Version;
    USHORT Revision;
    ULONG Count;
    CM_PARTIAL_RESOURCE_DESCRIPTOR PartialDescriptors[1];
} CM_PARTIAL_RESOURCE_LIST, *PCM_PARTIAL_RESOURCE_LIST;

//
// A Full Resource Descriptor is what can be found in the registry.
// This is what will be returned to a driver when it queries the registry
// to get device information; it will be stored under a key in the hardware
// description tree.
//
// Note: The BusNumber and Type are redundant information, but we will keep
// it since it allows the driver _not_ to append it when it is creating
// a resource list which could possibly span multiple buses.
//
// Note2: There must a be a convention to the order of fields of same type,
// (defined on a device by device basis) so that the fields can make sense
// to a driver (i.e. when multiple memory ranges are necessary).
//

typedef struct _CM_FULL_RESOURCE_DESCRIPTOR {
    INTERFACE_TYPE InterfaceType;
    ULONG BusNumber;
    CM_PARTIAL_RESOURCE_LIST PartialResourceList;
} CM_FULL_RESOURCE_DESCRIPTOR, *PCM_FULL_RESOURCE_DESCRIPTOR;

//
// The Resource list is what will be stored by the drivers into the
// resource map via the IO API.
//

typedef struct _CM_RESOURCE_LIST {
    ULONG Count;
    CM_FULL_RESOURCE_DESCRIPTOR List[1];
} CM_RESOURCE_LIST, *PCM_RESOURCE_LIST;


#include "pshpack1.h"


//
// Define Mca POS data block for slot
//

typedef struct _CM_MCA_POS_DATA {
    USHORT AdapterId;
    UCHAR PosData1;
    UCHAR PosData2;
    UCHAR PosData3;
    UCHAR PosData4;
} CM_MCA_POS_DATA, *PCM_MCA_POS_DATA;

//
// Memory configuration of eisa data block structure
//

typedef struct _EISA_MEMORY_TYPE {
    UCHAR ReadWrite: 1;
    UCHAR Cached : 1;
    UCHAR Reserved0 :1;
    UCHAR Type:2;
    UCHAR Shared:1;
    UCHAR Reserved1 :1;
    UCHAR MoreEntries : 1;
} EISA_MEMORY_TYPE, *PEISA_MEMORY_TYPE;

typedef struct _EISA_MEMORY_CONFIGURATION {
    EISA_MEMORY_TYPE ConfigurationByte;
    UCHAR DataSize;
    USHORT AddressLowWord;
    UCHAR AddressHighByte;
    USHORT MemorySize;
} EISA_MEMORY_CONFIGURATION, *PEISA_MEMORY_CONFIGURATION;


//
// Interrupt configurationn of eisa data block structure
//

typedef struct _EISA_IRQ_DESCRIPTOR {
    UCHAR Interrupt : 4;
    UCHAR Reserved :1;
    UCHAR LevelTriggered :1;
    UCHAR Shared : 1;
    UCHAR MoreEntries : 1;
} EISA_IRQ_DESCRIPTOR, *PEISA_IRQ_DESCRIPTOR;

typedef struct _EISA_IRQ_CONFIGURATION {
    EISA_IRQ_DESCRIPTOR ConfigurationByte;
    UCHAR Reserved;
} EISA_IRQ_CONFIGURATION, *PEISA_IRQ_CONFIGURATION;


//
// DMA description of eisa data block structure
//

typedef struct _DMA_CONFIGURATION_BYTE0 {
    UCHAR Channel : 3;
    UCHAR Reserved : 3;
    UCHAR Shared :1;
    UCHAR MoreEntries :1;
} DMA_CONFIGURATION_BYTE0;

typedef struct _DMA_CONFIGURATION_BYTE1 {
    UCHAR Reserved0 : 2;
    UCHAR TransferSize : 2;
    UCHAR Timing : 2;
    UCHAR Reserved1 : 2;
} DMA_CONFIGURATION_BYTE1;

typedef struct _EISA_DMA_CONFIGURATION {
    DMA_CONFIGURATION_BYTE0 ConfigurationByte0;
    DMA_CONFIGURATION_BYTE1 ConfigurationByte1;
} EISA_DMA_CONFIGURATION, *PEISA_DMA_CONFIGURATION;


//
// Port description of eisa data block structure
//

typedef struct _EISA_PORT_DESCRIPTOR {
    UCHAR NumberPorts : 5;
    UCHAR Reserved :1;
    UCHAR Shared :1;
    UCHAR MoreEntries : 1;
} EISA_PORT_DESCRIPTOR, *PEISA_PORT_DESCRIPTOR;

typedef struct _EISA_PORT_CONFIGURATION {
    EISA_PORT_DESCRIPTOR Configuration;
    USHORT PortAddress;
} EISA_PORT_CONFIGURATION, *PEISA_PORT_CONFIGURATION;


//
// Eisa slot information definition
// N.B. This structure is different from the one defined
//      in ARC eisa addendum.
//

typedef struct _CM_EISA_SLOT_INFORMATION {
    UCHAR ReturnCode;
    UCHAR ReturnFlags;
    UCHAR MajorRevision;
    UCHAR MinorRevision;
    USHORT Checksum;
    UCHAR NumberFunctions;
    UCHAR FunctionInformation;
    ULONG CompressedId;
} CM_EISA_SLOT_INFORMATION, *PCM_EISA_SLOT_INFORMATION;


//
// Eisa function information definition
//

typedef struct _CM_EISA_FUNCTION_INFORMATION {
    ULONG CompressedId;
    UCHAR IdSlotFlags1;
    UCHAR IdSlotFlags2;
    UCHAR MinorRevision;
    UCHAR MajorRevision;
    UCHAR Selections[26];
    UCHAR FunctionFlags;
    UCHAR TypeString[80];
    EISA_MEMORY_CONFIGURATION EisaMemory[9];
    EISA_IRQ_CONFIGURATION EisaIrq[7];
    EISA_DMA_CONFIGURATION EisaDma[4];
    EISA_PORT_CONFIGURATION EisaPort[20];
    UCHAR InitializationData[60];
} CM_EISA_FUNCTION_INFORMATION, *PCM_EISA_FUNCTION_INFORMATION;

//
// The followings define the way pnp bios information is stored in
// the registry \\HKEY_LOCAL_MACHINE\HARDWARE\Description\System\
// MultifunctionAdapter\x key, where x is an integer number indicating
// adapter instance. The "Identifier" of the key must equal to "PNP BIOS"
// and the "ConfigurationData" is organized as follow:
//
//      CM_PNP_BIOS_INSTALLATION_CHECK        +
//      CM_PNP_BIOS_DEVICE_NODE for device 1  +
//      CM_PNP_BIOS_DEVICE_NODE for device 2  +
//                ...
//      CM_PNP_BIOS_DEVICE_NODE for device n
//

//
// Pnp BIOS device node structure
//

typedef struct _CM_PNP_BIOS_DEVICE_NODE {
    USHORT Size;
    UCHAR Node;
    ULONG ProductId;
    UCHAR DeviceType[3];
    USHORT DeviceAttributes;
    // followed by AllocatedResourceBlock, PossibleResourceBlock
    // and CompatibleDeviceId
} CM_PNP_BIOS_DEVICE_NODE,*PCM_PNP_BIOS_DEVICE_NODE;

//
// Pnp BIOS Installation check
//

typedef struct _CM_PNP_BIOS_INSTALLATION_CHECK {
    UCHAR Signature[4];             // $PnP (ascii)
    UCHAR Revision;
    UCHAR Length;
    USHORT ControlField;
    UCHAR Checksum;
    ULONG EventFlagAddress;         // Physical address
    USHORT RealModeEntryOffset;
    USHORT RealModeEntrySegment;
    USHORT ProtectedModeEntryOffset;
    ULONG ProtectedModeCodeBaseAddress;
    ULONG OemDeviceId;
    USHORT RealModeDataBaseAddress;
    ULONG ProtectedModeDataBaseAddress;
} CM_PNP_BIOS_INSTALLATION_CHECK, *PCM_PNP_BIOS_INSTALLATION_CHECK;

#include "poppack.h"

//
// Masks for EISA function information
//

#define EISA_FUNCTION_ENABLED                   0x80
#define EISA_FREE_FORM_DATA                     0x40
#define EISA_HAS_PORT_INIT_ENTRY                0x20
#define EISA_HAS_PORT_RANGE                     0x10
#define EISA_HAS_DMA_ENTRY                      0x08
#define EISA_HAS_IRQ_ENTRY                      0x04
#define EISA_HAS_MEMORY_ENTRY                   0x02
#define EISA_HAS_TYPE_ENTRY                     0x01
#define EISA_HAS_INFORMATION                    EISA_HAS_PORT_RANGE + \
                                                EISA_HAS_DMA_ENTRY + \
                                                EISA_HAS_IRQ_ENTRY + \
                                                EISA_HAS_MEMORY_ENTRY + \
                                                EISA_HAS_TYPE_ENTRY

//
// Masks for EISA memory configuration
//

#define EISA_MORE_ENTRIES                       0x80
#define EISA_SYSTEM_MEMORY                      0x00
#define EISA_MEMORY_TYPE_RAM                    0x01

//
// Returned error code for EISA bios call
//

#define EISA_INVALID_SLOT                       0x80
#define EISA_INVALID_FUNCTION                   0x81
#define EISA_INVALID_CONFIGURATION              0x82
#define EISA_EMPTY_SLOT                         0x83
#define EISA_INVALID_BIOS_CALL                  0x86


//
// Interrupt modes.
//

typedef enum _KINTERRUPT_MODE {
    LevelSensitive,
    Latched
    } KINTERRUPT_MODE;

//
// Common dispatcher object header
//
// N.B. The size field contains the number of dwords in the structure.
//

typedef struct _DISPATCHER_HEADER {
    UCHAR Type;
    UCHAR Absolute;
    UCHAR Size;
    UCHAR Inserted;
    LONG SignalState;
    LIST_ENTRY WaitListHead;
} DISPATCHER_HEADER;

//
// Event object
//

typedef struct _KEVENT {
    DISPATCHER_HEADER Header;
} KEVENT, *PKEVENT, *RESTRICTED_POINTER PRKEVENT;

typedef struct _KINTERRUPT *PKINTERRUPT, *RESTRICTED_POINTER PRKINTERRUPT; 
//
// Timer object
//

typedef struct _KTIMER {
    DISPATCHER_HEADER Header;
    ULARGE_INTEGER DueTime;
    LIST_ENTRY TimerListEntry;
    struct _KDPC *Dpc;
    LONG Period;
} KTIMER, *PKTIMER, *RESTRICTED_POINTER PRKTIMER;

typedef struct _ADAPTER_OBJECT *PADAPTER_OBJECT; 
typedef struct _DEVICE_OBJECT *PDEVICE_OBJECT; 
typedef struct _DRIVER_OBJECT *PDRIVER_OBJECT; 
typedef struct _FILE_OBJECT *PFILE_OBJECT; 
#if defined(_MIPS_) || defined(_ALPHA_) || defined(_PPC_) 
                                                
NTHALAPI                                        
ULONG                                           
HalGetDmaAlignmentRequirement (                 
    VOID                                        
    );                                          
                                                
#endif                                          
                                                
#if defined(_M_IX86)                            
                                                
#define HalGetDmaAlignmentRequirement() 1L      
                                                
#endif                                          
                                                
NTHALAPI                                        
VOID                                            
KeFlushWriteBuffer (                            
    VOID                                        
    );                                          
                                                
//
// Stall processor execution function.
//

NTHALAPI
VOID
KeStallExecutionProcessor (
    IN ULONG MicroSeconds
    );


#else // BINARY_COMPATIBLE

//
// The definitions available in ntddk.h are intended for use only by full
// MAC drivers.  They must not be used directly by miniport drivers.
//

#include <ntddk.h>

#endif // else BINARY_COMPATIBLE

//
// END INTERNAL DEFINITIONS
//
// The following definitions may be used by NDIS drivers, except as noted.
//

//
// Indicate that we're building for NT. NDIS_NT is always used for
// miniport builds.
//

#define NDIS_NT 1

#if defined(NDIS_DOS)
#undef NDIS_DOS
#endif


//
// Define status codes and event log codes.
//

#include <ntstatus.h>
#include <netevent.h>

//
// Define a couple of extra types.
//

#if !defined(_WINDEF_)		// these are defined in windows.h too
typedef signed int INT, *PINT;
typedef unsigned int UINT, *PUINT;
#endif

typedef UNICODE_STRING NDIS_STRING, *PNDIS_STRING;


//
// Portability extentions
//

#define NDIS_INIT_FUNCTION(_F)		alloc_text(INIT,_F)
#define NDIS_PAGABLE_FUNCTION(_F)	alloc_text(PAGE,_F)
#define NDIS_PAGEABLE_FUNCTION(_F)	alloc_text(PAGE,_F)


//
// This file contains the definition of an NDIS_OID as
// well as #defines for all the current OID values.
//

#include <ntddndis.h>


//
// Ndis defines for configuration manager data structures
//

typedef CM_MCA_POS_DATA NDIS_MCA_POS_DATA, *PNDIS_MCA_POS_DATA;
typedef CM_EISA_SLOT_INFORMATION NDIS_EISA_SLOT_INFORMATION, *PNDIS_EISA_SLOT_INFORMATION;
typedef CM_EISA_FUNCTION_INFORMATION NDIS_EISA_FUNCTION_INFORMATION, *PNDIS_EISA_FUNCTION_INFORMATION;

//
// Define an exported function.
//
#if defined(NDIS_WRAPPER)
#define EXPORT
#else
#define EXPORT DECLSPEC_IMPORT
#endif

//
// Memory manipulation functions.
//

#define NdisMoveMemory(Destination,Source,Length) RtlCopyMemory(Destination,Source,Length)
#define NdisZeroMemory(Destination,Length) RtlZeroMemory(Destination,Length)
#define NdisRetrieveUlong(Destination,Source) RtlRetrieveUlong(Destination,Source)
#define NdisStoreUlong(Destination,Value) RtlStoreUlong(Destination,Value)

#define NDIS_STRING_CONST(x)	{sizeof(L##x)-2, sizeof(L##x), L##x}

//
// On a MIPS machine, I/O mapped memory can't be accessed with
// the Rtl routines.
//

#if defined(_M_IX86)

#define NdisMoveMappedMemory(Destination,Source,Length) RtlCopyMemory(Destination,Source,Length)
#define NdisZeroMappedMemory(Destination,Length) RtlZeroMemory(Destination,Length)

#elif defined(_M_MRX000)

#define NdisMoveMappedMemory(Destination,Source,Length)						\
{																			\
	PUCHAR _Src = (Source);													\
	PUCHAR _Dest = (Destination);											\
	PUCHAR _End = _Dest + (Length);											\
	while (_Dest < _End)													\
	{																		\
		*_Dest++ = *_Src++;													\
	}																		\
}

#define NdisZeroMappedMemory(Destination,Length)							\
{																			\
	PUCHAR _Dest = (Destination);											\
	PUCHAR _End = _Dest + (Length);											\
	while (_Dest < _End)													\
	{																		\
		*_Dest++ = 0;														\
	}																		\
}

#elif defined(_PPC_)

#define NdisMoveMappedMemory(Destination,Source,Length)						\
								RtlCopyMemory32( Destination, Source, Length);
#define NdisZeroMappedMemory(Destination,Length)							\
{																			\
	PUCHAR _Dest = (Destination);											\
	PUCHAR _End = _Dest + (Length);											\
	while (_Dest < _End)													\
	{																		\
		*_Dest++ = 0;														\
	}																		\
}

#elif defined(_ALPHA_)

#define NdisMoveMappedMemory(Destination,Source,Length) 					\
{																			\
	PUCHAR _Src = (Source);													\
	PUCHAR _Dest = (Destination);											\
	PUCHAR _End = _Dest + (Length);											\
	while (_Dest < _End)													\
	{																		\
		NdisReadRegisterUchar(_Src, _Dest);									\
		_Src++;																\
		_Dest++;															\
	}																		\
}

#define NdisZeroMappedMemory(Destination,Length)							\
{																			\
	PUCHAR _Dest = (Destination);											\
	PUCHAR _End = _Dest + (Length);											\
	while (_Dest < _End)													\
	{																		\
		NdisWriteRegisterUchar(_Dest,0);									\
		_Dest++;															\
	}																		\
}

#endif


//
// On Mips and Intel systems, these are the same. On Alpha, they are different.
//

#if defined(_ALPHA_)

#define NdisMoveToMappedMemory(Destination,Source,Length)					\
							WRITE_REGISTER_BUFFER_UCHAR(Destination,Source,Length)
#define NdisMoveFromMappedMemory(Destination,Source,Length)					\
							READ_REGISTER_BUFFER_UCHAR(Source,Destination,Length)
#else

#define NdisMoveToMappedMemory(Destination,Source,Length)					\
							NdisMoveMappedMemory(Destination,Source,Length)
#define NdisMoveFromMappedMemory(Destination,Source,Length)					\
							NdisMoveMappedMemory(Destination,Source,Length)
#endif


//
// definition of the basic spin lock structure
//

typedef struct _NDIS_SPIN_LOCK
{
	KSPIN_LOCK	SpinLock;
	KIRQL		OldIrql;
} NDIS_SPIN_LOCK, * PNDIS_SPIN_LOCK;


//
// definition of the ndis event structure
//

typedef struct _NDIS_EVENT
{
	KEVENT		Event;	
} NDIS_EVENT, *PNDIS_EVENT;

typedef PVOID NDIS_HANDLE, *PNDIS_HANDLE;

typedef int NDIS_STATUS, *PNDIS_STATUS; // note default size

#define NdisInterruptLatched Latched
#define NdisInterruptLevelSensitive LevelSensitive
typedef KINTERRUPT_MODE NDIS_INTERRUPT_MODE, *PNDIS_INTERRUPT_MODE;

//
// Configuration definitions
//

//
// Possible data types
//

typedef enum _NDIS_PARAMETER_TYPE
{
	NdisParameterInteger,
	NdisParameterHexInteger,
	NdisParameterString,
	NdisParameterMultiString
} NDIS_PARAMETER_TYPE, *PNDIS_PARAMETER_TYPE;

//
// To store configuration information
//
typedef struct _NDIS_CONFIGURATION_PARAMETER
{
	NDIS_PARAMETER_TYPE ParameterType;
	union
	{
		ULONG IntegerData;
		NDIS_STRING StringData;
	} ParameterData;
} NDIS_CONFIGURATION_PARAMETER, *PNDIS_CONFIGURATION_PARAMETER;


//
// Definitions for the "ProcessorType" keyword
//
typedef enum _NDIS_PROCESSOR_TYPE
{
	NdisProcessorX86,
	NdisProcessorMips,
	NdisProcessorAlpha,
	NdisProcessorPpc
} NDIS_PROCESSOR_TYPE, *PNDIS_PROCESSOR_TYPE;

//
// Definitions for the "Environment" keyword
//
typedef enum _NDIS_ENVIRONMENT_TYPE
{
	NdisEnvironmentWindows,
	NdisEnvironmentWindowsNt
} NDIS_ENVIRONMENT_TYPE, *PNDIS_ENVIRONMENT_TYPE;


//
// Possible Hardware Architecture. Define these to
// match the HAL INTERFACE_TYPE enum.
//
typedef enum _NDIS_INTERFACE_TYPE
{
	NdisInterfaceInternal = Internal,
	NdisInterfaceIsa = Isa,
	NdisInterfaceEisa = Eisa,
	NdisInterfaceMca = MicroChannel,
	NdisInterfaceTurboChannel = TurboChannel,
	NdisInterfacePci = PCIBus,
	NdisInterfacePcMcia = PCMCIABus
} NDIS_INTERFACE_TYPE, *PNDIS_INTERFACE_TYPE;

//
// Definition for shutdown handler
//

typedef
VOID
(*ADAPTER_SHUTDOWN_HANDLER) (
	IN	PVOID ShutdownContext
	);

//
// Stuff for PCI configuring
//

typedef CM_PARTIAL_RESOURCE_LIST NDIS_RESOURCE_LIST, *PNDIS_RESOURCE_LIST;


//
// The structure passed up on a WAN_LINE_UP indication
//

typedef struct _NDIS_WAN_LINE_UP
{
	IN ULONG				LinkSpeed;			// 100 bps units
	IN ULONG				MaximumTotalSize;	// suggested max for send packets
	IN NDIS_WAN_QUALITY		Quality;
	IN USHORT				SendWindow;			// suggested by the MAC
	IN UCHAR				RemoteAddress[6];
	IN OUT UCHAR			LocalAddress[6];
	IN ULONG				ProtocolBufferLength;	// Length of protocol info buffer
	IN PUCHAR				ProtocolBuffer;		// Information used by protocol
	IN USHORT				ProtocolType;		// Protocol ID
	IN OUT NDIS_STRING		DeviceName;
} NDIS_WAN_LINE_UP, *PNDIS_WAN_LINE_UP;

//
// The structure passed up on a WAN_LINE_DOWN indication
//

typedef struct _NDIS_WAN_LINE_DOWN
{
	IN UCHAR	RemoteAddress[6];
	IN UCHAR	LocalAddress[6];
} NDIS_WAN_LINE_DOWN, *PNDIS_WAN_LINE_DOWN;

//
// The structure passed up on a WAN_FRAGMENT indication
//

typedef struct _NDIS_WAN_FRAGMENT
{
	IN UCHAR	RemoteAddress[6];
	IN UCHAR	LocalAddress[6];
} NDIS_WAN_FRAGMENT, *PNDIS_WAN_FRAGMENT;


//
// DMA Channel information
//
typedef struct _NDIS_DMA_DESCRIPTION
{
	BOOLEAN		DemandMode;
	BOOLEAN		AutoInitialize;
	BOOLEAN		DmaChannelSpecified;
	DMA_WIDTH	DmaWidth;
	DMA_SPEED	DmaSpeed;
	ULONG		DmaPort;
	ULONG		DmaChannel;
} NDIS_DMA_DESCRIPTION, *PNDIS_DMA_DESCRIPTION;

//
// Internal structure representing an NDIS DMA channel
//
typedef struct _NDIS_DMA_BLOCK
{
	PVOID				MapRegisterBase;
	KEVENT				AllocationEvent;
	PADAPTER_OBJECT		SystemAdapterObject;
	BOOLEAN				InProgress;
} NDIS_DMA_BLOCK, *PNDIS_DMA_BLOCK;


//
// Ndis Buffer is actually an Mdl
//
typedef MDL NDIS_BUFFER, *PNDIS_BUFFER;

//
// Include an incomplete type for NDIS_PACKET structure so that
// function types can refer to a type to be defined later.
//
struct _NDIS_PACKET;
typedef	struct _NDIS_WAN_PACKET	NDIS_WAN_PACKET, *PNDIS_WAN_PACKET;

//
// packet pool definition
//
typedef struct _NDIS_PACKET_POOL
{
	NDIS_SPIN_LOCK		SpinLock;
	struct _NDIS_PACKET *FreeList;		// linked list of free slots in pool
	UINT				PacketLength;	// amount needed in each packet
	UCHAR				Buffer[1];		// actual pool memory
} NDIS_PACKET_POOL, * PNDIS_PACKET_POOL;


//
// wrapper-specific part of a packet
//

typedef struct _NDIS_PACKET_PRIVATE
{
	UINT				PhysicalCount;	// number of physical pages in packet.
	UINT				TotalLength;	// Total amount of data in the packet.
	PNDIS_BUFFER		Head;			// first buffer in the chain
	PNDIS_BUFFER		Tail;			// last buffer in the chain

	// if Head is NULL the chain is empty; Tail doesn't have to be NULL also

	PNDIS_PACKET_POOL	Pool;			// so we know where to free it back to
	UINT				Count;
	ULONG				Flags;			// See fPACKET_xxx bits below
	BOOLEAN				ValidCounts;
	UCHAR				NdisPacketFlags;
	USHORT				NdisPacketOobOffset;
} NDIS_PACKET_PRIVATE, * PNDIS_PACKET_PRIVATE;

#define fPACKET_CONTAINS_MEDIA_SPECIFIC_INFO	0x40
#define fPACKET_ALLOCATED_BY_NDIS				0x80

//
// Definition for layout of the media-specific data. More than one class of media-specific
// information can be tagged onto a packet.
//
typedef enum _NDIS_CLASS_ID
{
	NdisClass802_3Priority,
	NdisClassWirelessWanMbxMailbox,
	NdisClassIrdaPacketInfo

} NDIS_CLASS_ID;

typedef struct _MediaSpecificInformation
{
	UINT			NextEntryOffset;
	NDIS_CLASS_ID	ClassId;
	UINT			Size;
	UCHAR			ClassInformation[1];
} MEDIA_SPECIFIC_INFORMATION;

typedef struct _NDIS_PACKET_OOB_DATA
{
	union
	{
		ULONGLONG	TimeToSend;
		ULONGLONG	TimeSent;
	};
	ULONGLONG		TimeReceived;
	UINT			HeaderSize;
	UINT			SizeMediaSpecificInfo;
	PVOID			MediaSpecificInformation;

	NDIS_STATUS		Status;
} NDIS_PACKET_OOB_DATA, *PNDIS_PACKET_OOB_DATA;

#define NDIS_OOB_DATA_FROM_PACKET(_p)									\
						(PNDIS_PACKET_OOB_DATA)((PUCHAR)(_p) +			\
						(_p)->Private.NdisPacketOobOffset)
#define NDIS_GET_PACKET_HEADER_SIZE(_Packet)							\
						((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +	\
						(_Packet)->Private.NdisPacketOobOffset))->HeaderSize
#define NDIS_GET_PACKET_STATUS(_Packet)									\
						((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +	\
						(_Packet)->Private.NdisPacketOobOffset))->Status
#define	NDIS_GET_PACKET_TIME_TO_SEND(_Packet)							\
						((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +	\
						(_Packet)->Private.NdisPacketOobOffset))->TimeToSend
#define	NDIS_GET_PACKET_TIME_SENT(_Packet)								\
						((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +	\
						(_Packet)->Private.NdisPacketOobOffset))->TimeSent
#define NDIS_GET_PACKET_TIME_RECEIVED(_Packet)							\
						((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +	\
						(_Packet)->Private.NdisPacketOobOffset))->TimeReceived
#define NDIS_GET_PACKET_MEDIA_SPECIFIC_INFO(_Packet,					\
											_pMediaSpecificInfo,		\
											_pSizeMediaSpecificInfo)	\
{																		\
	if (!((_Packet)->Private.NdisPacketFlags & fPACKET_ALLOCATED_BY_NDIS) ||\
		!((_Packet)->Private.NdisPacketFlags & fPACKET_CONTAINS_MEDIA_SPECIFIC_INFO))\
	{																	\
		*(_pMediaSpecificInfo) = NULL;									\
		*(_pSizeMediaSpecificInfo) = 0;									\
	}																	\
	else																\
	{																	\
		*(_pMediaSpecificInfo) =((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +\
					(_Packet)->Private.NdisPacketOobOffset))->MediaSpecificInformation;\
		*(_pSizeMediaSpecificInfo) = ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +\
					(_Packet)->Private.NdisPacketOobOffset))->SizeMediaSpecificInfo;\
	}																	\
}

#define NDIS_SET_PACKET_HEADER_SIZE(_Packet, _HdrSize)					\
						((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +	\
						(_Packet)->Private.NdisPacketOobOffset))->HeaderSize = (_HdrSize)
#define NDIS_SET_PACKET_STATUS(_Packet, _Status)						\
						((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +	\
						(_Packet)->Private.NdisPacketOobOffset))->Status = (_Status)
#define	NDIS_SET_PACKET_TIME_TO_SEND(_Packet, _TimeToSend)				\
						((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +	\
						(_Packet)->Private.NdisPacketOobOffset))->TimeToSend = (_TimeToSend)
#define	NDIS_SET_PACKET_TIME_SENT(_Packet, _TimeSent)					\
						((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +	\
						(_Packet)->Private.NdisPacketOobOffset))->TimeSent = (_TimeSent)
#define NDIS_SET_PACKET_TIME_RECEIVED(_Packet, _TimeReceived)			\
						((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +	\
						(_Packet)->Private.NdisPacketOobOffset))->TimeReceived = (_TimeReceived)
#define NDIS_SET_PACKET_MEDIA_SPECIFIC_INFO(_Packet,					\
											_MediaSpecificInfo,			\
											_SizeMediaSpecificInfo)		\
{																		\
	if ((_Packet)->Private.NdisPacketFlags & fPACKET_ALLOCATED_BY_NDIS)	\
	{																	\
		(_Packet)->Private.NdisPacketFlags |= fPACKET_CONTAINS_MEDIA_SPECIFIC_INFO;\
		((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +					\
										  (_Packet)->Private.NdisPacketOobOffset))->MediaSpecificInformation = (_MediaSpecificInfo);\
		((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +					\
										  (_Packet)->Private.NdisPacketOobOffset))->SizeMediaSpecificInfo = (_SizeMediaSpecificInfo);\
	}																	\
}

//
// packet definition
//

typedef struct _NDIS_PACKET
{
	NDIS_PACKET_PRIVATE	Private;

	union
	{
		struct
		{
			UCHAR	MiniportReserved[8];
			UCHAR	WrapperReserved[8];
		};

		struct
		{
			UCHAR	MacReserved[16];
		};
	};

	UCHAR			ProtocolReserved[1];

} NDIS_PACKET, *PNDIS_PACKET, **PPNDIS_PACKET;

//
// For use by the wrapper for packet management
//
typedef struct _NDIS_PACKET_PRIVATE_EXTENSION
{
	LIST_ENTRY		Link;
	PNDIS_PACKET	Packet;
	ULONG			Reserved;	// to make the structure quad aligned
} NDIS_PACKET_PRIVATE_EXTENSION, *PNDIS_PACKET_PRIVATE_EXTENSION;

//
// Routines to get/set packet flags
//

/*++

UINT
NdisGetPacketFlags(
	IN	PNDIS_PACKET	Packet
	);

--*/

#define NdisGetPacketFlags(_Packet) 		(_Packet)->Private.Flags

/*++

VOID
NdisSetPacketFlags(
	IN	PNDIS_PACKET Packet,
	IN	UINT Flags
	);

--*/

#define NdisSetPacketFlags(_Packet, _Flags)	(_Packet)->Private.Flags = (_Flags)

//
// Request types used by NdisRequest; constants are added for
// all entry points in the MAC, for those that want to create
// their own internal requests.
//

typedef enum _NDIS_REQUEST_TYPE
{
	NdisRequestQueryInformation,
	NdisRequestSetInformation,
	NdisRequestQueryStatistics,
	NdisRequestOpen,
	NdisRequestClose,
	NdisRequestSend,
	NdisRequestTransferData,
	NdisRequestReset,
	NdisRequestGeneric1,
	NdisRequestGeneric2,
	NdisRequestGeneric3,
	NdisRequestGeneric4
} NDIS_REQUEST_TYPE, *PNDIS_REQUEST_TYPE;


//
// Structure of requests sent via NdisRequest
//

typedef struct _NDIS_REQUEST
{
	union
	{
		UCHAR			MacReserved[16];
		UCHAR			CallMgrReserved[16];
		UCHAR			ProtocolReserved[16];
	};
	NDIS_REQUEST_TYPE	RequestType;
	union _DATA
	{
		struct _QUERY_INFORMATION
		{
			NDIS_OID	Oid;
			PVOID		InformationBuffer;
			UINT		InformationBufferLength;
			UINT		BytesWritten;
			UINT		BytesNeeded;
		} QUERY_INFORMATION;

		struct _SET_INFORMATION
		{
			NDIS_OID	Oid;
			PVOID		InformationBuffer;
			UINT		InformationBufferLength;
			UINT		BytesRead;
			UINT		BytesNeeded;
		} SET_INFORMATION;

	} DATA;
#if NDIS41
	UCHAR				NdisReserved[24];
#endif
} NDIS_REQUEST, *PNDIS_REQUEST;


//
// Definitions for different address families supported by ATM
//
typedef enum _NDIS_AF
{
	CO_ADDRESS_FAMILY_Q2931 = 1,
	CO_ADDRESS_FAMILY_SPANS,
} NDIS_AF, *PNDIS_AF;

//
//	Address family structure registered/opened via
//		NdisCmRegisterAddressFamily
//		NdisClOpenAddressFamily
//
typedef struct
{
	NDIS_AF						AddressFamily;	// one of the CO_ADDRESS_FAMILY_xxx values above
	ULONG						MajorVersion;	// the major version of call manager
	ULONG						MinorVersion;	// the minor version of call manager
} CO_ADDRESS_FAMILY, *PCO_ADDRESS_FAMILY;

//
// Definition for a SAP
//
typedef struct
{
	ULONG						SapType;
	ULONG						SapLength;
	UCHAR						Sap[1];
} CO_SAP, *PCO_SAP;

//
// Definitions for physical address.
//

typedef PHYSICAL_ADDRESS NDIS_PHYSICAL_ADDRESS, *PNDIS_PHYSICAL_ADDRESS;
typedef struct _NDIS_PHYSICAL_ADDRESS_UNIT
{
	NDIS_PHYSICAL_ADDRESS		PhysicalAddress;
	UINT						Length;
} NDIS_PHYSICAL_ADDRESS_UNIT, *PNDIS_PHYSICAL_ADDRESS_UNIT;


/*++

ULONG
NdisGetPhysicalAddressHigh(
	IN	NDIS_PHYSICAL_ADDRESS	PhysicalAddress
	);

--*/

#define NdisGetPhysicalAddressHigh(_PhysicalAddress)						\
		((_PhysicalAddress).HighPart)

/*++

VOID
NdisSetPhysicalAddressHigh(
	IN	NDIS_PHYSICAL_ADDRESS	PhysicalAddress,
	IN	ULONG					Value
	);

--*/

#define NdisSetPhysicalAddressHigh(_PhysicalAddress, _Value)				\
	 ((_PhysicalAddress).HighPart) = (_Value)


/*++

ULONG
NdisGetPhysicalAddressLow(
	IN	NDIS_PHYSICAL_ADDRESS PhysicalAddress
	);

--*/

#define NdisGetPhysicalAddressLow(_PhysicalAddress)							\
	((_PhysicalAddress).LowPart)


/*++

VOID
NdisSetPhysicalAddressLow(
	IN	NDIS_PHYSICAL_ADDRESS	PhysicalAddress,
	IN	ULONG					Value
	);

--*/

#define NdisSetPhysicalAddressLow(_PhysicalAddress, _Value)					\
	((_PhysicalAddress).LowPart) = (_Value)

//
// Macro to initialize an NDIS_PHYSICAL_ADDRESS constant
//

#define NDIS_PHYSICAL_ADDRESS_CONST(_Low, _High)							\
	{ (ULONG)(_Low), (LONG)(_High) }


//
// block used for references...
//

typedef struct _REFERENCE
{
	NDIS_SPIN_LOCK				SpinLock;
	USHORT						ReferenceCount;
	BOOLEAN						Closing;
} REFERENCE, * PREFERENCE;


//
// This holds a map register entry.
//

typedef struct _MAP_REGISTER_ENTRY
{
	PVOID						MapRegister;
	BOOLEAN						WriteToDevice;
} MAP_REGISTER_ENTRY, * PMAP_REGISTER_ENTRY;

//
// Types of Memory (not mutually exclusive)
//

#define NDIS_MEMORY_CONTIGUOUS		0x00000001
#define NDIS_MEMORY_NONCACHED		0x00000002

//
// Open options
//
#define NDIS_OPEN_RECEIVE_NOT_REENTRANT	0x00000001

//
// NDIS_STATUS values
//

#define NDIS_STATUS_SUCCESS						((NDIS_STATUS)STATUS_SUCCESS)
#define NDIS_STATUS_PENDING						((NDIS_STATUS) STATUS_PENDING)
#define NDIS_STATUS_NOT_RECOGNIZED				((NDIS_STATUS)0x00010001L)
#define NDIS_STATUS_NOT_COPIED					((NDIS_STATUS)0x00010002L)
#define NDIS_STATUS_NOT_ACCEPTED				((NDIS_STATUS)0x00010003L)
#define NDIS_STATUS_CALL_ACTIVE					((NDIS_STATUS)0x00010007L)

#define NDIS_STATUS_ONLINE						((NDIS_STATUS)0x40010003L)
#define NDIS_STATUS_RESET_START					((NDIS_STATUS)0x40010004L)
#define NDIS_STATUS_RESET_END					((NDIS_STATUS)0x40010005L)
#define NDIS_STATUS_RING_STATUS					((NDIS_STATUS)0x40010006L)
#define NDIS_STATUS_CLOSED						((NDIS_STATUS)0x40010007L)
#define NDIS_STATUS_WAN_LINE_UP					((NDIS_STATUS)0x40010008L)
#define NDIS_STATUS_WAN_LINE_DOWN				((NDIS_STATUS)0x40010009L)
#define NDIS_STATUS_WAN_FRAGMENT				((NDIS_STATUS)0x4001000AL)
#define	NDIS_STATUS_MEDIA_CONNECT				((NDIS_STATUS)0x4001000BL)
#define	NDIS_STATUS_MEDIA_DISCONNECT			((NDIS_STATUS)0x4001000CL)
#define NDIS_STATUS_HARDWARE_LINE_UP			((NDIS_STATUS)0x4001000DL)
#define NDIS_STATUS_HARDWARE_LINE_DOWN			((NDIS_STATUS)0x4001000EL)
#define NDIS_STATUS_INTERFACE_UP				((NDIS_STATUS)0x4001000FL)
#define NDIS_STATUS_INTERFACE_DOWN				((NDIS_STATUS)0x40010010L)
#define NDIS_STATUS_MEDIA_BUSY					((NDIS_STATUS)0x40010011L)
#define	NDIS_STATUS_WW_INDICATION				((NDIS_STATUS)0x40010012L)

#define NDIS_STATUS_NOT_RESETTABLE				((NDIS_STATUS)0x80010001L)
#define NDIS_STATUS_SOFT_ERRORS					((NDIS_STATUS)0x80010003L)
#define NDIS_STATUS_HARD_ERRORS					((NDIS_STATUS)0x80010004L)
#define NDIS_STATUS_BUFFER_OVERFLOW				((NDIS_STATUS)STATUS_BUFFER_OVERFLOW)

#define NDIS_STATUS_FAILURE						((NDIS_STATUS) STATUS_UNSUCCESSFUL)
#define NDIS_STATUS_RESOURCES					((NDIS_STATUS)STATUS_INSUFFICIENT_RESOURCES)
#define NDIS_STATUS_CLOSING						((NDIS_STATUS)0xC0010002L)
#define NDIS_STATUS_BAD_VERSION					((NDIS_STATUS)0xC0010004L)
#define NDIS_STATUS_BAD_CHARACTERISTICS			((NDIS_STATUS)0xC0010005L)
#define NDIS_STATUS_ADAPTER_NOT_FOUND			((NDIS_STATUS)0xC0010006L)
#define NDIS_STATUS_OPEN_FAILED					((NDIS_STATUS)0xC0010007L)
#define NDIS_STATUS_DEVICE_FAILED				((NDIS_STATUS)0xC0010008L)
#define NDIS_STATUS_MULTICAST_FULL				((NDIS_STATUS)0xC0010009L)
#define NDIS_STATUS_MULTICAST_EXISTS			((NDIS_STATUS)0xC001000AL)
#define NDIS_STATUS_MULTICAST_NOT_FOUND			((NDIS_STATUS)0xC001000BL)
#define NDIS_STATUS_REQUEST_ABORTED				((NDIS_STATUS)0xC001000CL)
#define NDIS_STATUS_RESET_IN_PROGRESS			((NDIS_STATUS)0xC001000DL)
#define NDIS_STATUS_CLOSING_INDICATING			((NDIS_STATUS)0xC001000EL)
#define NDIS_STATUS_NOT_SUPPORTED				((NDIS_STATUS)STATUS_NOT_SUPPORTED)
#define NDIS_STATUS_INVALID_PACKET				((NDIS_STATUS)0xC001000FL)
#define NDIS_STATUS_OPEN_LIST_FULL				((NDIS_STATUS)0xC0010010L)
#define NDIS_STATUS_ADAPTER_NOT_READY			((NDIS_STATUS)0xC0010011L)
#define NDIS_STATUS_ADAPTER_NOT_OPEN			((NDIS_STATUS)0xC0010012L)
#define NDIS_STATUS_NOT_INDICATING				((NDIS_STATUS)0xC0010013L)
#define NDIS_STATUS_INVALID_LENGTH				((NDIS_STATUS)0xC0010014L)
#define NDIS_STATUS_INVALID_DATA				((NDIS_STATUS)0xC0010015L)
#define NDIS_STATUS_BUFFER_TOO_SHORT			((NDIS_STATUS)0xC0010016L)
#define NDIS_STATUS_INVALID_OID					((NDIS_STATUS)0xC0010017L)
#define NDIS_STATUS_ADAPTER_REMOVED				((NDIS_STATUS)0xC0010018L)
#define NDIS_STATUS_UNSUPPORTED_MEDIA			((NDIS_STATUS)0xC0010019L)
#define NDIS_STATUS_GROUP_ADDRESS_IN_USE		((NDIS_STATUS)0xC001001AL)
#define NDIS_STATUS_FILE_NOT_FOUND				((NDIS_STATUS)0xC001001BL)
#define NDIS_STATUS_ERROR_READING_FILE			((NDIS_STATUS)0xC001001CL)
#define NDIS_STATUS_ALREADY_MAPPED				((NDIS_STATUS)0xC001001DL)
#define NDIS_STATUS_RESOURCE_CONFLICT			((NDIS_STATUS)0xC001001EL)
#define NDIS_STATUS_NO_CABLE					((NDIS_STATUS)0xC001001FL)

#define NDIS_STATUS_INVALID_SAP					((NDIS_STATUS)0xC0010020L)
#define NDIS_STATUS_SAP_IN_USE					((NDIS_STATUS)0xC0010021L)
#define NDIS_STATUS_INVALID_ADDRESS				((NDIS_STATUS)0xC0010022L)
#define NDIS_STATUS_VC_NOT_ACTIVATED			((NDIS_STATUS)0xC0010023L)
#define NDIS_STATUS_DEST_OUT_OF_ORDER			((NDIS_STATUS)0xC0010024L)	// cause 27
#define NDIS_STATUS_VC_NOT_AVAILABLE			((NDIS_STATUS)0xC0010025L)	// cause 35,45
#define NDIS_STATUS_CELLRATE_NOT_AVAILABLE		((NDIS_STATUS)0xC0010026L)	// cause 37
#define NDIS_STATUS_INCOMPATABLE_QOS			((NDIS_STATUS)0xC0010027L)	// cause 49
#define NDIS_STATUS_AAL_PARAMS_UNSUPPORTED		((NDIS_STATUS)0xC0010028L)	// cause 93
#define NDIS_STATUS_NO_ROUTE_TO_DESTINATION		((NDIS_STATUS)0xC0010029L)	// cause 3

#define NDIS_STATUS_TOKEN_RING_OPEN_ERROR		((NDIS_STATUS)0xC0011000L)


//
// used in error logging
//

#define NDIS_ERROR_CODE ULONG

#define NDIS_ERROR_CODE_RESOURCE_CONFLICT			EVENT_NDIS_RESOURCE_CONFLICT
#define NDIS_ERROR_CODE_OUT_OF_RESOURCES			EVENT_NDIS_OUT_OF_RESOURCE
#define NDIS_ERROR_CODE_HARDWARE_FAILURE			EVENT_NDIS_HARDWARE_FAILURE
#define NDIS_ERROR_CODE_ADAPTER_NOT_FOUND			EVENT_NDIS_ADAPTER_NOT_FOUND
#define NDIS_ERROR_CODE_INTERRUPT_CONNECT			EVENT_NDIS_INTERRUPT_CONNECT
#define NDIS_ERROR_CODE_DRIVER_FAILURE				EVENT_NDIS_DRIVER_FAILURE
#define NDIS_ERROR_CODE_BAD_VERSION					EVENT_NDIS_BAD_VERSION
#define NDIS_ERROR_CODE_TIMEOUT						EVENT_NDIS_TIMEOUT
#define NDIS_ERROR_CODE_NETWORK_ADDRESS				EVENT_NDIS_NETWORK_ADDRESS
#define NDIS_ERROR_CODE_UNSUPPORTED_CONFIGURATION	EVENT_NDIS_UNSUPPORTED_CONFIGURATION
#define NDIS_ERROR_CODE_INVALID_VALUE_FROM_ADAPTER	EVENT_NDIS_INVALID_VALUE_FROM_ADAPTER
#define NDIS_ERROR_CODE_MISSING_CONFIGURATION_PARAMETER	EVENT_NDIS_MISSING_CONFIGURATION_PARAMETER
#define NDIS_ERROR_CODE_BAD_IO_BASE_ADDRESS			EVENT_NDIS_BAD_IO_BASE_ADDRESS
#define NDIS_ERROR_CODE_RECEIVE_SPACE_SMALL			EVENT_NDIS_RECEIVE_SPACE_SMALL
#define NDIS_ERROR_CODE_ADAPTER_DISABLED			EVENT_NDIS_ADAPTER_DISABLED

//
// Ndis Spin Locks
//

#if BINARY_COMPATIBLE

EXPORT
VOID
NdisAllocateSpinLock(
	IN	PNDIS_SPIN_LOCK			SpinLock
	);

EXPORT
VOID
NdisFreeSpinLock(
	IN	PNDIS_SPIN_LOCK			SpinLock
	);

EXPORT
VOID
NdisAcquireSpinLock(
	IN	PNDIS_SPIN_LOCK			SpinLock
	);

EXPORT
VOID
NdisReleaseSpinLock(
	IN	PNDIS_SPIN_LOCK			SpinLock
	);

EXPORT
VOID
NdisDprAcquireSpinLock(
	IN	PNDIS_SPIN_LOCK			SpinLock
	);

EXPORT
VOID
NdisDprReleaseSpinLock(
	IN	PNDIS_SPIN_LOCK			SpinLock
	);

#else

#define NdisAllocateSpinLock(_SpinLock)	KeInitializeSpinLock(&(_SpinLock)->SpinLock)

#define NdisFreeSpinLock(_SpinLock)

#define NdisAcquireSpinLock(_SpinLock)	KeAcquireSpinLock(&(_SpinLock)->SpinLock, &(_SpinLock)->OldIrql)

#define NdisReleaseSpinLock(_SpinLock)	KeReleaseSpinLock(&(_SpinLock)->SpinLock,(_SpinLock)->OldIrql)

#define NdisDprAcquireSpinLock(_SpinLock)									\
{																			\
	KeAcquireSpinLockAtDpcLevel(&(_SpinLock)->SpinLock);					\
	(_SpinLock)->OldIrql = DISPATCH_LEVEL;									\
}

#define NdisDprReleaseSpinLock(_SpinLock) KeReleaseSpinLockFromDpcLevel(&(_SpinLock)->SpinLock)

#endif

//
// List manipulation
//

/*++

VOID
NdisInitializeListHead(
	IN	PLIST_ENTRY ListHead
	);

--*/
#define NdisInitializeListHead(_ListHead) InitializeListHead(_ListHead)



//
// Configuration Requests
//

EXPORT
VOID
NdisOpenConfiguration(
	OUT PNDIS_STATUS			Status,
	OUT PNDIS_HANDLE			ConfigurationHandle,
	IN	NDIS_HANDLE				WrapperConfigurationContext
	);

EXPORT
VOID
NdisReadConfiguration(
	OUT PNDIS_STATUS			Status,
	OUT PNDIS_CONFIGURATION_PARAMETER *ParameterValue,
	IN	NDIS_HANDLE				ConfigurationHandle,
	IN	PNDIS_STRING			Keyword,
	IN	NDIS_PARAMETER_TYPE		ParameterType
	);

EXPORT
VOID
NdisWriteConfiguration(
	OUT PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	IN	PNDIS_STRING			Keyword,
	IN	PNDIS_CONFIGURATION_PARAMETER ParameterValue
	);

EXPORT
VOID
NdisCloseConfiguration(
	IN	NDIS_HANDLE				ConfigurationHandle
	);

EXPORT
VOID
NdisReadNetworkAddress(
	OUT PNDIS_STATUS			Status,
	OUT PVOID *					NetworkAddress,
	OUT PUINT					NetworkAddressLength,
	IN	NDIS_HANDLE				ConfigurationHandle
	);

EXPORT
VOID
NdisReadBindingInformation(
	OUT PNDIS_STATUS			Status,
	OUT PNDIS_STRING *			Binding,
	IN	NDIS_HANDLE				ConfigurationHandle
	);


EXPORT
VOID
NdisReadEisaSlotInformation(
	OUT PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	OUT PUINT					SlotNumber,
	OUT PNDIS_EISA_FUNCTION_INFORMATION EisaData
		);

EXPORT
VOID
NdisReadEisaSlotInformationEx(
	OUT PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	OUT PUINT					SlotNumber,
	OUT PNDIS_EISA_FUNCTION_INFORMATION *EisaData,
	OUT PUINT					NumberOfFunctions
		);

EXPORT
VOID
NdisReadMcaPosInformation(
	OUT PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	IN	PUINT					ChannelNumber,
	OUT PNDIS_MCA_POS_DATA		McaData
		);

EXPORT
ULONG
NdisReadPciSlotInformation(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	ULONG					SlotNumber,
	IN	ULONG					Offset,
	IN	PVOID					Buffer,
	IN	ULONG					Length
	);

EXPORT
ULONG
NdisWritePciSlotInformation(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	ULONG					SlotNumber,
	IN	ULONG					Offset,
	IN	PVOID					Buffer,
	IN	ULONG					Length
	);

EXPORT
NDIS_STATUS
NdisPciAssignResources(
	IN	NDIS_HANDLE				NdisMacHandle,
	IN	NDIS_HANDLE				NdisWrapperHandle,
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	IN	ULONG					SlotNumber,
	OUT PNDIS_RESOURCE_LIST *	AssignedResources
	);

//
// Buffer Pool
//

EXPORT
VOID
NdisAllocateBufferPool(
	OUT PNDIS_STATUS			Status,
	OUT PNDIS_HANDLE			PoolHandle,
	IN	UINT					NumberOfDescriptors
	);

EXPORT
VOID
NdisFreeBufferPool(
	IN	NDIS_HANDLE				PoolHandle
	);

EXPORT
VOID
NdisAllocateBuffer(
	OUT PNDIS_STATUS			Status,
	OUT PNDIS_BUFFER *			Buffer,
	IN	NDIS_HANDLE				PoolHandle,
	IN	PVOID					VirtualAddress,
	IN	UINT					Length
	);

EXPORT
VOID
NdisCopyBuffer(
	OUT PNDIS_STATUS			Status,
	OUT PNDIS_BUFFER *			Buffer,
	IN	NDIS_HANDLE				PoolHandle,
	IN	PVOID					MemoryDescriptor,
	IN	UINT					Offset,
	IN	UINT					Length
	);


//
// Packet Pool
//

EXPORT
VOID
NdisAllocatePacketPool(
	OUT PNDIS_STATUS			Status,
	OUT PNDIS_HANDLE			PoolHandle,
	IN	UINT					NumberOfDescriptors,
	IN	UINT					ProtocolReservedLength
	);

#if BINARY_COMPATIBLE

VOID
NdisFreePacketPool(
	IN	NDIS_HANDLE				PoolHandle
	);

VOID
NdisFreePacket(
	IN	PNDIS_PACKET			Packet
	);

VOID
NdisDprFreePacket(
	IN	PNDIS_PACKET			Packet
	);

VOID
NdisDprFreePacketNonInterlocked(
	IN	PNDIS_PACKET			Packet
	);

#else


#define NdisFreePacketPool(PoolHandle)										\
{																			\
	NdisFreeSpinLock(&((PNDIS_PACKET_POOL)PoolHandle)->SpinLock);			\
	ExFreePool(PoolHandle);													\
}

// VOID
// NdisFreePacket(
//	IN	PNDIS_PACKET			Packet
//	);
#define NdisFreePacket(Packet)												\
{																			\
	NdisAcquireSpinLock(&(Packet)->Private.Pool->SpinLock); 				\
	(Packet)->Private.Head = (PNDIS_BUFFER)(Packet)->Private.Pool->FreeList;\
	(Packet)->Private.Pool->FreeList = (Packet);							\
	NdisReleaseSpinLock(&(Packet)->Private.Pool->SpinLock);					\
}


// VOID
// NdisDprFreePacket(
//	IN	PNDIS_PACKET			Packet
//	);
#define NdisDprFreePacket(Packet)											\
{																			\
	NdisDprAcquireSpinLock(&(Packet)->Private.Pool->SpinLock); 				\
	(Packet)->Private.Head = (PNDIS_BUFFER)(Packet)->Private.Pool->FreeList;\
	(Packet)->Private.Pool->FreeList = (Packet);							\
	NdisDprReleaseSpinLock(&(Packet)->Private.Pool->SpinLock);				\
}


// VOID
// NdisDprFreePacketNonInterlocked(
//	IN	PNDIS_PACKET			Packet
//	);
#define NdisDprFreePacketNonInterlocked(Packet)								\
{																			\
	(Packet)->Private.Head = (PNDIS_BUFFER)(Packet)->Private.Pool->FreeList;\
	(Packet)->Private.Pool->FreeList = (Packet);							\
}

#endif

EXPORT
VOID
NdisAllocatePacket(
	OUT PNDIS_STATUS			Status,
	OUT PNDIS_PACKET *			Packet,
	IN	NDIS_HANDLE				PoolHandle
	);

EXPORT
VOID
NdisDprAllocatePacket(
	OUT PNDIS_STATUS			Status,
	OUT PNDIS_PACKET *			Packet,
	IN	NDIS_HANDLE				PoolHandle
	);

EXPORT
VOID
NdisDprAllocatePacketNonInterlocked(
	OUT PNDIS_STATUS			Status,
	OUT PNDIS_PACKET *			Packet,
	IN	NDIS_HANDLE				PoolHandle
	);

// VOID
// NdisReinitializePacket(
//	IN OUT PNDIS_PACKET			Packet
//	);
#define NdisReinitializePacket(Packet)										\
{																			\
	(Packet)->Private.Head = (PNDIS_BUFFER)NULL;							\
	(Packet)->Private.ValidCounts = FALSE;									\
}



#if BINARY_COMPATIBLE

EXPORT
VOID
NdisFreeBuffer(
	IN	PNDIS_BUFFER			Buffer
	);

EXPORT
VOID
NdisQueryBuffer(
	IN	PNDIS_BUFFER			Buffer,
	OUT PVOID *					VirtualAddress OPTIONAL,
	OUT PUINT					Length
	);

EXPORT
VOID
NdisQueryBufferOffset(
	IN	PNDIS_BUFFER			Buffer,
	OUT PUINT					Offset,
	OUT PUINT					Length
	);

#else

#define NdisFreeBuffer(Buffer)	IoFreeMdl(Buffer)

#define NdisQueryBuffer(_Buffer, _VirtualAddress, _Length)					\
{																			\
	if (ARGUMENT_PRESENT(_VirtualAddress))									\
	{																		\
		*(PVOID *)(_VirtualAddress) = MmGetSystemAddressForMdl(_Buffer);	\
	}																		\
	*(_Length) = MmGetMdlByteCount(_Buffer);								\
}

#define NdisQueryBufferOffset(_Buffer, _Offset, _Length)					\
{																			\
	*(_Offset) = MmGetMdlByteOffset(_Buffer);								\
	*(_Length) = MmGetMdlByteCount(_Buffer);								\
}

#endif


//
// This macro is a combination of NdisQueryPacket and NdisQueryBuffer
// and optimized for protocols to get the first Buffer, its VA and its
// size.
//

/*++

VOID
NdisGetFirstBufferFromPacket(
	IN	PNDIS_PACKET			_Packet,
	OUT PNDIS_BUFFER *			_FirstBuffer,
	OUT PVOID *					_FirstBufferVA,
	OUT PUINT					_FirstBufferLength,
	OUT	PUINT					_TotalBufferLength
	);

--*/

#define	NdisGetFirstBufferFromPacket(_Packet,								\
									 _FirstBuffer,							\
									 _FirstBufferVA,						\
									 _FirstBufferLength,					\
									 _TotalBufferLength)					\
	{																		\
		PNDIS_BUFFER	_pBuf;												\
																			\
		_pBuf = (_Packet)->Private.Head;									\
		*(_FirstBuffer) = _pBuf;											\
		*(_FirstBufferVA) =	MmGetMdlVirtualAddress(_pBuf);					\
		*(_FirstBufferLength) =												\
		*(_TotalBufferLength) = MmGetMdlByteCount(_pBuf);					\
		for (_pBuf = _pBuf->Next;											\
			 _pBuf != NULL;													\
			 _pBuf = _pBuf->Next)											\
		{                                                               	\
			*(_TotalBufferLength) += MmGetMdlByteCount(_pBuf);				\
		}																	\
	}

//
// This macro is used to determine how many physical pieces
// an NDIS_BUFFER will take up when mapped.
//

#if BINARY_COMPATIBLE

EXPORT
ULONG
NDIS_BUFFER_TO_SPAN_PAGES(
	IN	PNDIS_BUFFER				Buffer
	);

EXPORT
VOID
NdisGetBufferPhysicalArraySize(
	IN	PNDIS_BUFFER				Buffer,
	OUT PUINT						ArraySize
	);

#else

#define NDIS_BUFFER_TO_SPAN_PAGES(_Buffer)									\
	(MmGetMdlByteCount(_Buffer)==0 ?										\
			    1 :															\
			    (COMPUTE_PAGES_SPANNED(										\
					    MmGetMdlVirtualAddress(_Buffer),					\
					    MmGetMdlByteCount(_Buffer))))

#define NdisGetBufferPhysicalArraySize(Buffer, ArraySize)					\
	(*(ArraySize) = NDIS_BUFFER_TO_SPAN_PAGES(Buffer))

#endif


/*++

NDIS_BUFFER_LINKAGE(
	IN	PNDIS_BUFFER			Buffer
	);

--*/

#define NDIS_BUFFER_LINKAGE(Buffer)	((Buffer)->Next)


/*++

VOID
NdisRecalculatePacketCounts(
	IN OUT PNDIS_PACKET			Packet
	);

--*/

#define NdisRecalculatePacketCounts(Packet)									\
{																			\
	{																		\
		PNDIS_BUFFER TmpBuffer = (Packet)->Private.Head; 					\
		if (TmpBuffer)														\
		{																	\
			while (TmpBuffer->Next)											\
			{																\
				TmpBuffer = TmpBuffer->Next;								\
			}																\
			(Packet)->Private.Tail = TmpBuffer; 							\
		}																	\
		(Packet)->Private.ValidCounts = FALSE;								\
	}																		\
}


/*++

VOID
NdisChainBufferAtFront(
	IN OUT PNDIS_PACKET			Packet,
	IN OUT PNDIS_BUFFER			Buffer
	);

--*/

#define NdisChainBufferAtFront(Packet, Buffer)								\
{																			\
	PNDIS_BUFFER TmpBuffer = (Buffer);										\
																			\
	for (;;)																\
	{																		\
		if (TmpBuffer->Next == (PNDIS_BUFFER)NULL)							\
		    break;															\
		TmpBuffer = TmpBuffer->Next;										\
	}																		\
	if ((Packet)->Private.Head == NULL)										\
	{																		\
		(Packet)->Private.Tail = TmpBuffer;									\
	}																		\
	TmpBuffer->Next = (Packet)->Private.Head;								\
	(Packet)->Private.Head = (Buffer);										\
	(Packet)->Private.ValidCounts = FALSE;									\
}

/*++

VOID
NdisChainBufferAtBack(
	IN OUT PNDIS_PACKET			Packet,
	IN OUT PNDIS_BUFFER			Buffer
	);

--*/

#define NdisChainBufferAtBack(Packet, Buffer)								\
{																			\
	PNDIS_BUFFER TmpBuffer = (Buffer);										\
																			\
	for (;;)																\
	{																		\
		if (TmpBuffer->Next == NULL)										\
			break;															\
		TmpBuffer = TmpBuffer->Next;										\
	}																		\
	if ((Packet)->Private.Head != NULL)										\
	{																		\
		(Packet)->Private.Tail->Next = (Buffer);							\
	}																		\
	else																	\
	{																		\
		(Packet)->Private.Head = (Buffer);									\
	}																		\
	(Packet)->Private.Tail = TmpBuffer;										\
	TmpBuffer->Next = NULL;													\
	(Packet)->Private.ValidCounts = FALSE;									\
}

EXPORT
VOID
NdisUnchainBufferAtFront(
	IN OUT PNDIS_PACKET			Packet,
	OUT PNDIS_BUFFER *			Buffer
	);

EXPORT
VOID
NdisUnchainBufferAtBack(
	IN OUT PNDIS_PACKET			Packet,
	OUT PNDIS_BUFFER *			Buffer
	);


/*++

VOID
NdisQueryPacket(
	IN	PNDIS_PACKET			_Packet,
	OUT PUINT					_PhysicalBufferCount OPTIONAL,
	OUT PUINT					_BufferCount OPTIONAL,
	OUT PNDIS_BUFFER *			_FirstBuffer OPTIONAL,
	OUT PUINT					_TotalPacketLength OPTIONAL
	);

--*/

#define NdisQueryPacket(_Packet,											\
						_PhysicalBufferCount,								\
						_BufferCount,										\
						_FirstBuffer,										\
						_TotalPacketLength)									\
{																			\
	if ((_FirstBuffer) != NULL)												\
	{																		\
		PNDIS_BUFFER * __FirstBuffer = (_FirstBuffer);						\
		*(__FirstBuffer) = (_Packet)->Private.Head;							\
	}																		\
	if ((_TotalPacketLength) || (_BufferCount) || (_PhysicalBufferCount))	\
	{																		\
		if (!(_Packet)->Private.ValidCounts)								\
		{																	\
			PNDIS_BUFFER TmpBuffer = (_Packet)->Private.Head;				\
			UINT PTotalLength = 0, PPhysicalCount = 0, PAddedCount = 0;		\
			UINT PacketLength, Offset;										\
																			\
			while (TmpBuffer != (PNDIS_BUFFER)NULL)							\
			{																\
				NdisQueryBufferOffset(TmpBuffer, &Offset, &PacketLength);	\
				PTotalLength += PacketLength;								\
				PPhysicalCount += NDIS_BUFFER_TO_SPAN_PAGES(TmpBuffer);		\
				++PAddedCount;												\
				TmpBuffer = TmpBuffer->Next;								\
			}																\
			(_Packet)->Private.Count = PAddedCount;							\
			(_Packet)->Private.TotalLength = PTotalLength;					\
			(_Packet)->Private.PhysicalCount = PPhysicalCount;				\
			(_Packet)->Private.ValidCounts = TRUE;							\
		}			    			    			    			    	\
																			\
		if (_PhysicalBufferCount)											\
		{																	\
			PUINT __PhysicalBufferCount = (_PhysicalBufferCount);			\
			*(__PhysicalBufferCount) = (_Packet)->Private.PhysicalCount;	\
		}																	\
		if (_BufferCount)													\
		{			    			    			    			    	\
			PUINT __BufferCount = (_BufferCount);							\
			*(__BufferCount) = (_Packet)->Private.Count;					\
		}																	\
		if (_TotalPacketLength)												\
		{																	\
			PUINT __TotalPacketLength = (_TotalPacketLength);				\
			*(__TotalPacketLength) = (_Packet)->Private.TotalLength;		\
		}																	\
	}																		\
}


/*++

VOID
NdisGetNextBuffer(
	IN	PNDIS_BUFFER			CurrentBuffer,
	OUT PNDIS_BUFFER *			NextBuffer
	);

--*/

#define NdisGetNextBuffer(CurrentBuffer, NextBuffer)						\
{																			\
	*(NextBuffer) = (CurrentBuffer)->Next;									\
}

#if BINARY_COMPATIBLE

VOID
NdisAdjustBufferLength(
	IN	PNDIS_BUFFER			Buffer,
	IN	UINT					Length
	);

#else

#define NdisAdjustBufferLength(Buffer, Length)	(((Buffer)->ByteCount) = (Length))

#endif

EXPORT
VOID
NdisCopyFromPacketToPacket(
	IN	PNDIS_PACKET			Destination,
	IN	UINT					DestinationOffset,
	IN	UINT					BytesToCopy,
	IN	PNDIS_PACKET			Source,
	IN	UINT					SourceOffset,
	OUT PUINT					BytesCopied
	);


EXPORT
NDIS_STATUS
NdisAllocateMemory(
	OUT PVOID *					VirtualAddress,
	IN	UINT					Length,
	IN	UINT					MemoryFlags,
	IN	NDIS_PHYSICAL_ADDRESS	HighestAcceptableAddress
	);

EXPORT
VOID
NdisFreeMemory(
	IN	PVOID					VirtualAddress,
	IN	UINT					Length,
	IN	UINT					MemoryFlags
	);


/*++
VOID
NdisStallExecution(
	IN	UINT					MicrosecondsToStall
	)
--*/

#define NdisStallExecution(MicroSecondsToStall)								\
		KeStallExecutionProcessor(MicroSecondsToStall)


EXPORT
VOID
NdisInitializeEvent(
	IN	PNDIS_EVENT				Event
);

EXPORT
VOID
NdisSetEvent(
	IN	PNDIS_EVENT				Event
);

EXPORT
VOID
NdisResetEvent(
	IN	PNDIS_EVENT				Event
);

EXPORT
BOOLEAN
NdisWaitEvent(
	IN	PNDIS_EVENT				Event,
	IN	UINT					msToWait
);

EXPORT
NDIS_STATUS
NdisQueryMapRegisterCount(
	IN	NDIS_INTERFACE_TYPE		BusType,
	OUT	PUINT					MapRegisterCount
);

//
// Simple I/O support
//

EXPORT
VOID
NdisOpenFile(
	OUT PNDIS_STATUS			Status,
	OUT PNDIS_HANDLE			FileHandle,
	OUT PUINT					FileLength,
	IN	PNDIS_STRING			FileName,
	IN	NDIS_PHYSICAL_ADDRESS	HighestAcceptableAddress
	);

EXPORT
VOID
NdisCloseFile(
	IN	NDIS_HANDLE				FileHandle
	);

EXPORT
VOID
NdisMapFile(
	OUT PNDIS_STATUS			Status,
	OUT PVOID *					MappedBuffer,
	IN	NDIS_HANDLE				FileHandle
	);

EXPORT
VOID
NdisUnmapFile(
	IN	NDIS_HANDLE				FileHandle
	);


//
// Portability extensions
//

/*++
VOID
NdisFlushBuffer(
	IN	PNDIS_BUFFER			Buffer,
	IN	BOOLEAN					WriteToDevice
		)
--*/

#define NdisFlushBuffer(Buffer,WriteToDevice)								\
		KeFlushIoBuffers((Buffer),!(WriteToDevice), TRUE)

/*++
ULONG
NdisGetCacheFillSize(
	)
--*/
#define NdisGetCacheFillSize()	HalGetDmaAlignmentRequirement()

//
// This macro is used to convert a port number as the caller
// thinks of it, to a port number as it should be passed to
// READ/WRITE_PORT.
//

#define NDIS_PORT_TO_PORT(Handle,Port)	   (((PNDIS_ADAPTER_BLOCK)(Handle))->PortOffset + (Port))


//
// Write Port
//

/*++
VOID
NdisWritePortUchar(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	ULONG					Port,
	IN	UCHAR					Data
		)
--*/
#define NdisWritePortUchar(Handle,Port,Data)								\
		WRITE_PORT_UCHAR((PUCHAR)(NDIS_PORT_TO_PORT(Handle,Port)),(UCHAR)(Data))

/*++
VOID
NdisWritePortUshort(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	ULONG					Port,
	IN	USHORT					Data
		)
--*/
#define NdisWritePortUshort(Handle,Port,Data)								\
		WRITE_PORT_USHORT((PUSHORT)(NDIS_PORT_TO_PORT(Handle,Port)),(USHORT)(Data))


/*++
VOID
NdisWritePortUlong(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	ULONG					Port,
	IN	ULONG					Data
		)
--*/
#define NdisWritePortUlong(Handle,Port,Data)								\
		WRITE_PORT_ULONG((PULONG)(NDIS_PORT_TO_PORT(Handle,Port)),(ULONG)(Data))


//
// Write Port Buffers
//

/*++
VOID
NdisWritePortBufferUchar(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	ULONG					Port,
	IN	PUCHAR					Buffer,
	IN	ULONG					Length
		)
--*/
#define NdisWritePortBufferUchar(Handle,Port,Buffer,Length)					\
		NdisRawWritePortBufferUchar(NDIS_PORT_TO_PORT((Handle),(Port)),(Buffer),(Length))

/*++
VOID
NdisWritePortBufferUshort(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	ULONG					Port,
	IN	PUSHORT					Buffer,
	IN	ULONG					Length
		)
--*/
#define NdisWritePortBufferUshort(Handle,Port,Buffer,Length)				\
		NdisRawWritePortBufferUshort(NDIS_PORT_TO_PORT((Handle),(Port)),(Buffer),(Length))


/*++
VOID
NdisWritePortBufferUlong(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	ULONG					Port,
	IN	PULONG					Buffer,
	IN	ULONG					Length
		)
--*/
#define NdisWritePortBufferUlong(Handle,Port,Buffer,Length)					\
		NdisRawWritePortBufferUlong(NDIS_PORT_TO_PORT((Handle),(Port)),(Buffer),(Length))


//
// Read Ports
//

/*++
VOID
NdisReadPortUchar(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	ULONG					Port,
	OUT PUCHAR					Data
		)
--*/
#define NdisReadPortUchar(Handle,Port, Data)								\
		NdisRawReadPortUchar(NDIS_PORT_TO_PORT((Handle),(Port)),(Data))

/*++
VOID
NdisReadPortUshort(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	ULONG					Port,
	OUT PUSHORT					Data
		)
--*/
#define NdisReadPortUshort(Handle,Port,Data)								\
		NdisRawReadPortUshort(NDIS_PORT_TO_PORT((Handle),(Port)),(Data))


/*++
VOID
NdisReadPortUlong(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	ULONG					Port,
	OUT PULONG					Data
		)
--*/
#define NdisReadPortUlong(Handle,Port,Data)									\
		NdisRawReadPortUlong(NDIS_PORT_TO_PORT((Handle),(Port)),(Data))

//
// Read Buffer Ports
//

/*++
VOID
NdisReadPortBufferUchar(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	ULONG					Port,
	OUT PUCHAR					Buffer,
	IN	ULONG					Length
		)
--*/
#define NdisReadPortBufferUchar(Handle,Port,Buffer,Length)					\
		NdisRawReadPortBufferUchar(NDIS_PORT_TO_PORT((Handle),(Port)),(Buffer),(Length))

/*++
VOID
NdisReadPortBufferUshort(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	ULONG					Port,
	OUT PUSHORT					Buffer,
	IN	ULONG					Length
		)
--*/
#define NdisReadPortBufferUshort(Handle,Port,Buffer,Length) 				\
		NdisRawReadPortBufferUshort(NDIS_PORT_TO_PORT((Handle),(Port)),(Buffer),(Length))

/*++
VOID
NdisReadPortBufferUlong(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	ULONG					Port,
	OUT PULONG					Buffer,
	IN	ULONG					Length
		)
--*/
#define NdisReadPortBufferUlong(Handle,Port,Buffer) 						\
		NdisRawReadPortBufferUlong(NDIS_PORT_TO_PORT((Handle),(Port)),(Buffer),(Length))

//
// Raw Routines
//

//
// Write Port Raw
//

/*++
VOID
NdisRawWritePortUchar(
	IN	ULONG					Port,
	IN	UCHAR					Data
		)
--*/
#define NdisRawWritePortUchar(Port,Data) 									\
		WRITE_PORT_UCHAR((PUCHAR)(Port),(UCHAR)(Data))

/*++
VOID
NdisRawWritePortUshort(
	IN	ULONG					Port,
	IN	USHORT					Data
		)
--*/
#define NdisRawWritePortUshort(Port,Data)									\
		WRITE_PORT_USHORT((PUSHORT)(Port),(USHORT)(Data))

/*++
VOID
NdisRawWritePortUlong(
	IN	ULONG					Port,
	IN	ULONG					Data
		)
--*/
#define NdisRawWritePortUlong(Port,Data) 									\
		WRITE_PORT_ULONG((PULONG)(Port),(ULONG)(Data))


//
// Raw Write Port Buffers
//

/*++
VOID
NdisRawWritePortBufferUchar(
	IN	ULONG					Port,
	IN	PUCHAR					Buffer,
	IN	ULONG					Length
		)
--*/
#define NdisRawWritePortBufferUchar(Port,Buffer,Length) \
		WRITE_PORT_BUFFER_UCHAR((PUCHAR)(Port),(PUCHAR)(Buffer),(Length))

/*++
VOID
NdisRawWritePortBufferUshort(
	IN	ULONG					Port,
	IN	PUSHORT					Buffer,
	IN	ULONG					Length
		)
--*/
#if defined(_M_IX86)
#define NdisRawWritePortBufferUshort(Port,Buffer,Length)					\
		WRITE_PORT_BUFFER_USHORT((PUSHORT)(Port),(PUSHORT)(Buffer),(Length))
#else
#define NdisRawWritePortBufferUshort(Port,Buffer,Length)					\
{																			\
		ULONG _Port = (ULONG)(Port);										\
		PUSHORT _Current = (Buffer);										\
		PUSHORT _End = _Current + (Length);									\
		for ( ; _Current < _End; ++_Current)								\
		{																	\
		    WRITE_PORT_USHORT((PUSHORT)_Port,*(UNALIGNED USHORT *)_Current);\
		}																	\
}
#endif


/*++
VOID
NdisRawWritePortBufferUlong(
	IN	ULONG					Port,
	IN	PULONG					Buffer,
	IN	ULONG					Length
		)
--*/
#if defined(_M_IX86)
#define NdisRawWritePortBufferUlong(Port,Buffer,Length) 					\
		WRITE_PORT_BUFFER_ULONG((PULONG)(Port),(PULONG)(Buffer),(Length))
#else
#define NdisRawWritePortBufferUlong(Port,Buffer,Length)						\
{																			\
		ULONG _Port = (ULONG)(Port);										\
		PULONG _Current = (Buffer);											\
		PULONG _End = _Current + (Length);									\
		for ( ; _Current < _End; ++_Current)								\
		{																	\
		    WRITE_PORT_ULONG((PULONG)_Port,*(UNALIGNED ULONG *)_Current);	\
		}																	\
}
#endif


//
// Raw Read Ports
//

/*++
VOID
NdisRawReadPortUchar(
	IN	ULONG					Port,
	OUT PUCHAR					Data
		)
--*/
#define NdisRawReadPortUchar(Port, Data) \
		*(Data) = READ_PORT_UCHAR((PUCHAR)(Port))

/*++
VOID
NdisRawReadPortUshort(
	IN	ULONG					Port,
	OUT PUSHORT					Data
		)
--*/
#define NdisRawReadPortUshort(Port,Data) \
		*(Data) = READ_PORT_USHORT((PUSHORT)(Port))

/*++
VOID
NdisRawReadPortUlong(
	IN	ULONG					Port,
	OUT PULONG					Data
		)
--*/
#define NdisRawReadPortUlong(Port,Data) \
		*(Data) = READ_PORT_ULONG((PULONG)(Port))


//
// Raw Read Buffer Ports
//

/*++
VOID
NdisRawReadPortBufferUchar(
	IN	ULONG					Port,
	OUT PUCHAR					Buffer,
	IN	ULONG					Length
		)
--*/
#define NdisRawReadPortBufferUchar(Port,Buffer,Length)						\
		READ_PORT_BUFFER_UCHAR((PUCHAR)(Port),(PUCHAR)(Buffer),(Length))


/*++
VOID
NdisRawReadPortBufferUshort(
	IN	ULONG					Port,
	OUT PUSHORT					Buffer,
	IN	ULONG					Length
		)
--*/
#if defined(_M_IX86)
#define NdisRawReadPortBufferUshort(Port,Buffer,Length) 					\
		READ_PORT_BUFFER_USHORT((PUSHORT)(Port),(PUSHORT)(Buffer),(Length))
#else
#define NdisRawReadPortBufferUshort(Port,Buffer,Length)						\
{																			\
		ULONG _Port = (ULONG)(Port);										\
		PUSHORT _Current = (Buffer);										\
		PUSHORT _End = _Current + (Length);									\
		for ( ; _Current < _End; ++_Current)								\
		{ 																	\
		    *(UNALIGNED USHORT *)_Current = READ_PORT_USHORT((PUSHORT)_Port); \
		}																	\
}
#endif


/*++
VOID
NdisRawReadPortBufferUlong(
	IN	ULONG Port,
	OUT PULONG Buffer,
	IN	ULONG Length
		)
--*/
#if defined(_M_IX86)
#define NdisRawReadPortBufferUlong(Port,Buffer,Length)						\
		READ_PORT_BUFFER_ULONG((PULONG)(Port),(PULONG)(Buffer),(Length))
#else
#define NdisRawReadPortBufferUlong(Port,Buffer,Length)						\
{																			\
		ULONG _Port = (ULONG)(Port);										\
		PULONG _Current = (Buffer);											\
		PULONG _End = _Current + (Length);									\
		for ( ; _Current < _End; ++_Current)								\
		{																	\
		    *(UNALIGNED ULONG *)_Current = READ_PORT_ULONG((PULONG)_Port);	\
		}																	\
}
#endif


//
// Write Registers
//

/*++
VOID
NdisWriteRegisterUchar(
	IN	PUCHAR Register,
	IN	UCHAR Data
		)
--*/

#if defined(_M_IX86)
#define NdisWriteRegisterUchar(Register,Data)								\
		WRITE_REGISTER_UCHAR((Register),(Data))
#else
#define NdisWriteRegisterUchar(Register,Data)								\
	{																		\
		WRITE_REGISTER_UCHAR((Register),(Data));							\
		READ_REGISTER_UCHAR(Register);										\
	}
#endif

/*++
VOID
NdisWriteRegisterUshort(
	IN	PUSHORT Register,
	IN	USHORT Data
		)
--*/

#if defined(_M_IX86)
#define NdisWriteRegisterUshort(Register,Data)								\
		WRITE_REGISTER_USHORT((Register),(Data))
#else
#define NdisWriteRegisterUshort(Register,Data)								\
	{																		\
		WRITE_REGISTER_USHORT((Register),(Data));							\
		READ_REGISTER_USHORT(Register);										\
	}
#endif

/*++
VOID
NdisWriteRegisterUlong(
	IN	PULONG Register,
	IN	ULONG Data
		)
--*/

#if defined(_M_IX86)
#define NdisWriteRegisterUlong(Register,Data)	WRITE_REGISTER_ULONG((Register),(Data))
#else
#define NdisWriteRegisterUlong(Register,Data)								\
	{																		\
		WRITE_REGISTER_ULONG((Register),(Data));							\
		READ_REGISTER_ULONG(Register);										\
	}
#endif

/*++
VOID
NdisReadRegisterUchar(
	IN	PUCHAR					Register,
	OUT PUCHAR					Data
		)
--*/
#if defined(_M_IX86)
#define NdisReadRegisterUchar(Register,Data)	*((PUCHAR)(Data)) = *(Register)
#else
#define NdisReadRegisterUchar(Register,Data)	*(Data) = READ_REGISTER_UCHAR((PUCHAR)(Register))
#endif

/*++
VOID
NdisReadRegisterUshort(
	IN	PUSHORT					Register,
	OUT PUSHORT					Data
		)
--*/
#if defined(_M_IX86)
#define NdisReadRegisterUshort(Register,Data)	*((PUSHORT)(Data)) = *(Register)
#else
#define NdisReadRegisterUshort(Register,Data)	*(Data) = READ_REGISTER_USHORT((PUSHORT)(Register))
#endif

/*++
VOID
NdisReadRegisterUlong(
	IN	PULONG					Register,
	OUT PULONG					Data
		)
--*/
#if defined(_M_IX86)
#define NdisReadRegisterUlong(Register,Data)	*((PULONG)(Data)) = *(Register)
#else
#define NdisReadRegisterUlong(Register,Data)	*(Data) = READ_REGISTER_ULONG((PULONG)(Register))
#endif

#if BINARY_COMPATIBLE

EXPORT
BOOLEAN
NdisEqualString(
	IN	PNDIS_STRING			String1,
	IN	PNDIS_STRING			String2,
	IN	BOOLEAN					CaseInsensitive
	);

#else

#define NdisEqualString(_String1,_String2,CaseInsensitive)					\
			RtlEqualUnicodeString((_String1), (_String2), CaseInsensitive)
#endif

EXPORT
VOID
NdisWriteErrorLogEntry(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	NDIS_ERROR_CODE			ErrorCode,
	IN	ULONG					NumberOfErrorValues,
	...
	);

EXPORT
VOID
NdisInitializeString(
	OUT	PNDIS_STRING	Destination,
	IN	PUCHAR			Source
	);

#define NdisFreeString(String) NdisFreeMemory((String).Buffer, (String).MaximumLength, 0)

#define NdisPrintString(String) DbgPrint("%ls",(String).Buffer)


#if !defined(_ALPHA_)
/*++

VOID
NdisCreateLookaheadBufferFromSharedMemory(
	IN	PVOID					pSharedMemory,
	IN	UINT					LookaheadLength,
	OUT PVOID *					pLookaheadBuffer
	);

--*/

#define NdisCreateLookaheadBufferFromSharedMemory(_S, _L, _B)	((*(_B)) = (_S))

/*++

VOID
NdisDestroyLookaheadBufferFromSharedMemory(
	IN	PVOID					pLookaheadBuffer
	);

--*/

#define NdisDestroyLookaheadBufferFromSharedMemory(_B)

#else // Alpha

EXPORT
VOID
NdisCreateLookaheadBufferFromSharedMemory(
	IN	PVOID					pSharedMemory,
	IN	UINT					LookaheadLength,
	OUT PVOID *					pLookaheadBuffer
	);

EXPORT
VOID
NdisDestroyLookaheadBufferFromSharedMemory(
	IN	PVOID 					pLookaheadBuffer
	);

#endif


//
// The following declarations are shared between ndismac.h and ndismini.h. They
// are meant to be for internal use only. They should not be used directly by
// miniport drivers.
//

//
// declare these first since they point to each other
//

typedef struct _NDIS_WRAPPER_HANDLE	NDIS_WRAPPER_HANDLE, *PNDIS_WRAPPER_HANDLE;
typedef struct _NDIS_MAC_BLOCK		NDIS_MAC_BLOCK, *PNDIS_MAC_BLOCK;
typedef struct _NDIS_ADAPTER_BLOCK	NDIS_ADAPTER_BLOCK, *PNDIS_ADAPTER_BLOCK;
typedef struct _NDIS_PROTOCOL_BLOCK NDIS_PROTOCOL_BLOCK, *PNDIS_PROTOCOL_BLOCK;
typedef struct _NDIS_OPEN_BLOCK		NDIS_OPEN_BLOCK, *PNDIS_OPEN_BLOCK;

//
// Timers.
//

typedef
VOID
(*PNDIS_TIMER_FUNCTION) (
	IN	PVOID					SystemSpecific1,
	IN	PVOID					FunctionContext,
	IN	PVOID					SystemSpecific2,
	IN	PVOID					SystemSpecific3
	);

typedef struct _NDIS_TIMER {
	KTIMER		Timer;
	KDPC		Dpc;
} NDIS_TIMER, *PNDIS_TIMER;

EXPORT
VOID
NdisSetTimer(
	IN	PNDIS_TIMER				Timer,
	IN	UINT					MillisecondsToDelay
	);

//
// DMA operations.
//

EXPORT
VOID
NdisAllocateDmaChannel(
	OUT PNDIS_STATUS			Status,
	OUT PNDIS_HANDLE			NdisDmaHandle,
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	PNDIS_DMA_DESCRIPTION	DmaDescription,
	IN	ULONG					MaximumLength
	);

EXPORT
VOID
NdisFreeDmaChannel(
	IN	NDIS_HANDLE				NdisDmaHandle
	);

EXPORT
VOID
NdisSetupDmaTransfer(
	OUT PNDIS_STATUS			Status,
	IN	PNDIS_HANDLE			NdisDmaHandle,
	IN	PNDIS_BUFFER			Buffer,
	IN	ULONG					Offset,
	IN	ULONG					Length,
	IN	BOOLEAN					WriteToDevice
	);

EXPORT
VOID
NdisCompleteDmaTransfer(
	OUT PNDIS_STATUS			Status,
	IN	PNDIS_HANDLE			NdisDmaHandle,
	IN	PNDIS_BUFFER			Buffer,
	IN	ULONG					Offset,
	IN	ULONG					Length,
	IN	BOOLEAN					WriteToDevice
	);

/*++
ULONG
NdisReadDmaCounter(
	IN	NDIS_HANDLE				NdisDmaHandle
	)
--*/

#define NdisReadDmaCounter(_NdisDmaHandle) \
	HalReadDmaCounter(((PNDIS_DMA_BLOCK)(_NdisDmaHandle))->SystemAdapterObject)

//
// Wrapper initialization and termination.
//

EXPORT
VOID
NdisInitializeWrapper(
	OUT PNDIS_HANDLE			NdisWrapperHandle,
	IN	PVOID					SystemSpecific1,
	IN	PVOID					SystemSpecific2,
	IN	PVOID					SystemSpecific3
	);

EXPORT
VOID
NdisTerminateWrapper(
	IN	NDIS_HANDLE				NdisWrapperHandle,
	IN	PVOID					SystemSpecific
	);

//
// Shared memory
//

#define	NdisUpdateSharedMemory(_H, _L, _V, _P)

//
// System processor count
//

EXPORT
CCHAR
NdisSystemProcessorCount(
	VOID
	);

//
// Override bus number
//

EXPORT
VOID
NdisOverrideBusNumber(
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	IN	NDIS_HANDLE				MiniportAdapterHandle OPTIONAL,
	IN	ULONG					BusNumber
	);


EXPORT
VOID
NdisImmediateReadPortUchar(
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	IN	ULONG					Port,
	OUT PUCHAR					Data
	);

EXPORT
VOID
NdisImmediateReadPortUshort(
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	IN	ULONG					Port,
	OUT PUSHORT Data
	);

EXPORT
VOID
NdisImmediateReadPortUlong(
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	IN	ULONG					Port,
	OUT PULONG Data
	);

EXPORT
VOID
NdisImmediateWritePortUchar(
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	IN	ULONG					Port,
	IN	UCHAR					Data
	);

EXPORT
VOID
NdisImmediateWritePortUshort(
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	IN	ULONG					Port,
	IN	USHORT					Data
	);

EXPORT
VOID
NdisImmediateWritePortUlong(
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	IN	ULONG					Port,
	IN	ULONG					Data
	);

EXPORT
VOID
NdisImmediateReadSharedMemory(
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	IN	ULONG					SharedMemoryAddress,
	IN	PUCHAR					Buffer,
	IN	ULONG					Length
	);

EXPORT
VOID
NdisImmediateWriteSharedMemory(
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	IN	ULONG					SharedMemoryAddress,
	IN	PUCHAR					Buffer,
	IN	ULONG					Length
	);

EXPORT
ULONG
NdisImmediateReadPciSlotInformation(
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	IN	ULONG					SlotNumber,
	IN	ULONG					Offset,
	IN	PVOID					Buffer,
	IN	ULONG					Length
	);

EXPORT
ULONG
NdisImmediateWritePciSlotInformation(
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	IN	ULONG					SlotNumber,
	IN	ULONG					Offset,
	IN	PVOID					Buffer,
	IN	ULONG					Length
	);

//
// Ansi/Unicode support routines
//

#if BINARY_COMPATIBLE

EXPORT
VOID
NdisInitAnsiString(
    IN OUT	PANSI_STRING		DestinationString,
    IN		PCSTR				SourceString
    );

EXPORT
VOID
NdisInitUnicodeString(
    IN OUT	PUNICODE_STRING		DestinationString,
    IN		PCWSTR				SourceString
    );

EXPORT
NDIS_STATUS
NdisAnsiStringToUnicodeString(
    IN OUT	PUNICODE_STRING		DestinationString,
    IN		PANSI_STRING		SourceString
    );

EXPORT
NDIS_STATUS
NdisUnicodeStringToAnsiString(
    IN OUT	PANSI_STRING		DestinationString,
    IN		PUNICODE_STRING		SourceString
    );

#else

#define	NdisInitAnsiString(_as, s)				RtlInitString(_as, s)
#define	NdisInitUnicodeString(_us, s)			RtlInitUnicodeString(_us, s)
#define	NdisAnsiStringToUnicodeString(_us, _as)	RtlAnsiStringToUnicodeString(_us, _as, FALSE)
#define	NdisUnicodeStringToAnsiString(_as, _us)	RtlUnicodeStringToAnsiString(_as, _us, FALSE)

#endif



//
// Function types for NDIS_PROTOCOL_CHARACTERISTICS
//

typedef
VOID
(*OPEN_ADAPTER_COMPLETE_HANDLER)(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	NDIS_STATUS				Status,
	IN	NDIS_STATUS				OpenErrorStatus
	);

typedef
VOID
(*CLOSE_ADAPTER_COMPLETE_HANDLER)(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	NDIS_STATUS				Status
	);

typedef
VOID
(*RESET_COMPLETE_HANDLER)(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	NDIS_STATUS				Status
	);

typedef
VOID
(*REQUEST_COMPLETE_HANDLER)(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	PNDIS_REQUEST			NdisRequest,
	IN	NDIS_STATUS				Status
	);

typedef
VOID
(*STATUS_HANDLER)(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	NDIS_STATUS				GeneralStatus,
	IN	PVOID					StatusBuffer,
	IN	UINT					StatusBufferSize
	);

typedef
VOID
(*STATUS_COMPLETE_HANDLER)(
	IN	NDIS_HANDLE				ProtocolBindingContext
	);

typedef
VOID
(*SEND_COMPLETE_HANDLER)(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	PNDIS_PACKET			Packet,
	IN	NDIS_STATUS				Status
	);

typedef
VOID
(*WAN_SEND_COMPLETE_HANDLER) (
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	PNDIS_WAN_PACKET		Packet,
	IN	NDIS_STATUS				Status
	);

typedef
VOID
(*TRANSFER_DATA_COMPLETE_HANDLER)(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	PNDIS_PACKET			Packet,
	IN	NDIS_STATUS				Status,
	IN	UINT					BytesTransferred
	);

typedef
VOID
(*WAN_TRANSFER_DATA_COMPLETE_HANDLER)(
	VOID
    );

typedef
NDIS_STATUS
(*RECEIVE_HANDLER)(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	NDIS_HANDLE				MacReceiveContext,
	IN	PVOID					HeaderBuffer,
	IN	UINT					HeaderBufferSize,
	IN	PVOID					LookAheadBuffer,
	IN	UINT					LookaheadBufferSize,
	IN	UINT					PacketSize
	);

typedef
NDIS_STATUS
(*WAN_RECEIVE_HANDLER)(
	IN	NDIS_HANDLE				NdisLinkHandle,
	IN	PUCHAR					Packet,
	IN	ULONG					PacketSize
    );

typedef
VOID
(*RECEIVE_COMPLETE_HANDLER)(
	IN	NDIS_HANDLE				ProtocolBindingContext
	);

//
// Protocol characteristics for down-level NDIS 3.0 protocols
//
typedef struct _NDIS30_PROTOCOL_CHARACTERISTICS
{
	UCHAR							MajorNdisVersion;
	UCHAR							MinorNdisVersion;
	union
	{
		UINT						Reserved;
		UINT						Flags;
	};
	OPEN_ADAPTER_COMPLETE_HANDLER	OpenAdapterCompleteHandler;
	CLOSE_ADAPTER_COMPLETE_HANDLER	CloseAdapterCompleteHandler;
	union
	{
		SEND_COMPLETE_HANDLER		SendCompleteHandler;
		WAN_SEND_COMPLETE_HANDLER	WanSendCompleteHandler;
	};
	union
	{
	 TRANSFER_DATA_COMPLETE_HANDLER	TransferDataCompleteHandler;
	 WAN_TRANSFER_DATA_COMPLETE_HANDLER WanTransferDataCompleteHandler;
	};

	RESET_COMPLETE_HANDLER			ResetCompleteHandler;
	REQUEST_COMPLETE_HANDLER		RequestCompleteHandler;
	union
	{
		RECEIVE_HANDLER				ReceiveHandler;
		WAN_RECEIVE_HANDLER			WanReceiveHandler;
	};
	RECEIVE_COMPLETE_HANDLER		ReceiveCompleteHandler;
	STATUS_HANDLER					StatusHandler;
	STATUS_COMPLETE_HANDLER			StatusCompleteHandler;
	NDIS_STRING						Name;
} NDIS30_PROTOCOL_CHARACTERISTICS;

//
// Function types extensions for NDIS 4.0 Protocols
//
typedef
INT
(*RECEIVE_PACKET_HANDLER)(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	PNDIS_PACKET			Packet
	);

typedef
VOID
(*BIND_HANDLER)(
	OUT	PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				BindContext,
	IN	PNDIS_STRING			DeviceName,
	IN	PVOID					SystemSpecific1,
	IN	PVOID					SystemSpecific2
	);

typedef
VOID
(*UNBIND_HANDLER)(
	OUT	PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	NDIS_HANDLE				UnbindContext
	);

typedef
VOID
(*TRANSLATE_HANDLER)(
	OUT	PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				ProtocolBindingContext,
	OUT	PNET_PNP_ID				IdList,
	IN	ULONG					IdListLength,
	OUT	PULONG					BytesReturned
	);

typedef
VOID
(*UNLOAD_PROTOCOL_HANDLER)(
	VOID
	);

//
// Protocol characteristics for NDIS 4.0 protocols
//
typedef struct _NDIS40_PROTOCOL_CHARACTERISTICS
{
#ifdef __cplusplus
	NDIS30_PROTOCOL_CHARACTERISTICS	Ndis30Chars;
#else
	NDIS30_PROTOCOL_CHARACTERISTICS;
#endif

	//
	// Start of NDIS 4.0 extensions.
	//
	RECEIVE_PACKET_HANDLER			ReceivePacketHandler;

	//
	// PnP protocol entry-points
	//
	BIND_HANDLER					BindAdapterHandler;
	UNBIND_HANDLER					UnbindAdapterHandler;
	TRANSLATE_HANDLER				TranslateHandler;
	UNLOAD_PROTOCOL_HANDLER			UnloadHandler;

} NDIS40_PROTOCOL_CHARACTERISTICS;


//
// WARNING: NDIS v4.1 is under construction. Do not use.
//

//
// CoNdis Protocol (4.1) handler proto-types - used by clients as well as call manager modules
//
typedef
VOID
(*CO_SEND_COMPLETE_HANDLER)(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				ProtocolVcContext,
	IN	PNDIS_PACKET			Packet
	);

typedef
VOID
(*CO_STATUS_HANDLER)(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	NDIS_HANDLE				ProtocolVcContext	OPTIONAL,
	IN	NDIS_STATUS				GeneralStatus,
	IN	PVOID					StatusBuffer,
	IN	UINT					StatusBufferSize
	);

typedef
UINT
(*CO_RECEIVE_PACKET_HANDLER)(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	NDIS_HANDLE				ProtocolVcContext,
	IN	PNDIS_PACKET			Packet
	);

typedef
NDIS_STATUS
(*CO_REQUEST_HANDLER)(
	IN	NDIS_HANDLE				ProtocolAfContext,
	IN	NDIS_HANDLE				ProtocolVcContext		OPTIONAL,
	IN	NDIS_HANDLE				ProtocolPartyContext	OPTIONAL,
	IN OUT PNDIS_REQUEST		NdisRequest
	);

typedef
VOID
(*CO_REQUEST_COMPLETE_HANDLER)(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				ProtocolAfContext,
	IN	NDIS_HANDLE				ProtocolVcContext		OPTIONAL,
	IN	NDIS_HANDLE				ProtocolPartyContext	OPTIONAL,
	IN	PNDIS_REQUEST			NdisRequest
	);

//
// CO_CREATE_VC_HANDLER and CO_DELETE_VC_HANDLER are synchronous calls
//
typedef
NDIS_STATUS
(*CO_CREATE_VC_HANDLER)(
	IN	NDIS_HANDLE				ProtocolAfContext,
	IN	NDIS_HANDLE				NdisVcHandle,
	OUT	PNDIS_HANDLE			ProtocolVcContext
	);

typedef
NDIS_STATUS
(*CO_DELETE_VC_HANDLER)(
	IN	NDIS_HANDLE				ProtocolVcContext
	);

typedef struct _NDIS41_PROTOCOL_CHARACTERISTICS
{
#ifdef __cplusplus
	NDIS40_PROTOCOL_CHARACTERISTICS	Ndis40Chars;
#else
	NDIS40_PROTOCOL_CHARACTERISTICS;
#endif
	
	//
	// Start of NDIS 4.1 extensions.
	//

	CO_SEND_COMPLETE_HANDLER		CoSendCompleteHandler;
	CO_STATUS_HANDLER				CoStatusHandler;
	CO_RECEIVE_PACKET_HANDLER		CoReceivePacketHandler;
	CO_REQUEST_HANDLER				CoRequestHandler;
	CO_REQUEST_COMPLETE_HANDLER		CoRequestCompleteHandler;

} NDIS41_PROTOCOL_CHARACTERISTICS;

#if NDIS41
typedef NDIS41_PROTOCOL_CHARACTERISTICS  NDIS_PROTOCOL_CHARACTERISTICS;
#define	NDIS_PROTOCOL_CALL_MANAGER	0x00000001
#else
#if NDIS40
typedef NDIS40_PROTOCOL_CHARACTERISTICS  NDIS_PROTOCOL_CHARACTERISTICS;
#else
typedef NDIS30_PROTOCOL_CHARACTERISTICS  NDIS_PROTOCOL_CHARACTERISTICS;
#endif
#endif
typedef NDIS_PROTOCOL_CHARACTERISTICS *PNDIS_PROTOCOL_CHARACTERISTICS;

//
// Requests used by Protocol Modules
//

EXPORT
VOID
NdisRegisterProtocol(
	OUT	PNDIS_STATUS			Status,
	OUT	PNDIS_HANDLE			NdisProtocolHandle,
	IN	PNDIS_PROTOCOL_CHARACTERISTICS ProtocolCharacteristics,
	IN	UINT					CharacteristicsLength
	);

EXPORT
VOID
NdisDeregisterProtocol(
	OUT	PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				NdisProtocolHandle
	);


EXPORT
VOID
NdisOpenAdapter(
	OUT	PNDIS_STATUS			Status,
	OUT	PNDIS_STATUS			OpenErrorStatus,
	OUT	PNDIS_HANDLE			NdisBindingHandle,
	OUT	PUINT					SelectedMediumIndex,
	IN	PNDIS_MEDIUM			MediumArray,
	IN	UINT					MediumArraySize,
	IN	NDIS_HANDLE				NdisProtocolHandle,
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	PNDIS_STRING			AdapterName,
	IN	UINT					OpenOptions,
	IN	PSTRING					AddressingInformation OPTIONAL
	);


EXPORT
VOID
NdisCloseAdapter(
	OUT	PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				NdisBindingHandle
	);


EXPORT
VOID
NdisCompleteBindAdapter(
	IN	 NDIS_HANDLE			BindAdapterContext,
	IN	 NDIS_STATUS			Status,
	IN	 NDIS_STATUS			OpenStatus
	);

EXPORT
VOID
NdisCompleteUnbindAdapter(
	IN	 NDIS_HANDLE			UnbindAdapterContext,
	IN	 NDIS_STATUS			Status
	);

EXPORT
VOID
NdisSetProtocolFilter(
	OUT	PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				NdisBindingHandle,
	IN	RECEIVE_HANDLER			ReceiveHandler,
	IN	RECEIVE_PACKET_HANDLER	ReceivePacketHandler,
	IN	NDIS_MEDIUM				Medium,
	IN	UINT					Offset,
	IN	UINT					Size,
	IN	PUCHAR					Pattern
	);

EXPORT
VOID
NdisOpenProtocolConfiguration(
	OUT	PNDIS_STATUS			Status,
	OUT	PNDIS_HANDLE			ConfigurationHandle,
	IN	PNDIS_STRING			ProtocolSection
);

EXPORT
VOID
NdisGetDriverHandle(
	IN	NDIS_HANDLE				NdisBindingHandle,
	OUT	PNDIS_HANDLE			NdisDriverHandle
);

//
// The following is used by TDI/NDIS interface as part of Network PnP.
// For use by TDI alone.
//
typedef
NTSTATUS
(*TDI_REGISTER_CALLBACK)(
	IN	PUNICODE_STRING			DeviceName,
	OUT	HANDLE	*				TdiHandle
);

EXPORT
VOID
NdisRegisterTdiCallBack(
	IN	TDI_REGISTER_CALLBACK	RegsterCallback
);

#if BINARY_COMPATIBILE

VOID
NdisSend(
	OUT	PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				NdisBindingHandle,
	IN	PNDIS_PACKET			Packet
	);

VOID
NdisSendPackets(
	IN	NDIS_HANDLE				NdisBindingHandle,
	IN	PPNDIS_PACKET			PacketArray,
	IN	UINT					NumberOfPackets
	);

VOID
NdisTransferData(
	OUT	PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				NdisBindingHandle,
	IN	NDIS_HANDLE				MacReceiveContext,
	IN	UINT					ByteOffset,
	IN	UINT					BytesToTransfer,
	IN OUT	PNDIS_PACKET		Packet,
	OUT	PUINT					BytesTransferred
	);

VOID
NdisReset(
	OUT	PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				NdisBindingHandle
	);

VOID
NdisRequest(
	OUT	PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				NdisBindingHandle,
	IN	PNDIS_REQUEST			NdisRequest
	);

#else

#define NdisSend(Status, NdisBindingHandle, Packet)							\
{																			\
	*(Status) =																\
		(((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->SendHandler)(				\
			((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->MacBindingHandle,		\
		(Packet));															\
}

#define NdisSendPackets(NdisBindingHandle, PacketArray, NumberOfPackets)	\
{																			\
	(((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->SendPacketsHandler)( 			\
		(PNDIS_OPEN_BLOCK)(NdisBindingHandle),								\
		(PacketArray), 														\
		(NumberOfPackets)); 												\
}

#define NdisTransferData(Status,											\
						 NdisBindingHandle, 								\
						 MacReceiveContext, 								\
						 ByteOffset,										\
						 BytesToTransfer,									\
						 Packet,											\
						 BytesTransferred)									\
{																			\
	*(Status) =																\
		(((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->TransferDataHandler)( 	\
			((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->MacBindingHandle,		\
			(MacReceiveContext),											\
			(ByteOffset),													\
			(BytesToTransfer),												\
			(Packet),														\
			(BytesTransferred));											\
}


#define NdisReset(Status, NdisBindingHandle)								\
{																			\
	*(Status) =																\
		(((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->MacHandle->MacCharacteristics.ResetHandler)(\
			((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->MacBindingHandle);		\
}

#define NdisRequest(Status, NdisBindingHandle, NdisRequest)					\
{																			\
	*(Status) =																\
		(((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->MacHandle->MacCharacteristics.RequestHandler)( \
			((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->MacBindingHandle,		\
			(NdisRequest));													\
}

#endif
//
// Routines to access packet flags
//

/*++

VOID
NdisSetSendFlags(
	IN	PNDIS_PACKET			Packet,
	IN	UINT					Flags
	);

--*/

#define NdisSetSendFlags(_Packet,_Flags)	(_Packet)->Private.Flags = (_Flags)

/*++

VOID
NdisQuerySendFlags(
	IN	PNDIS_PACKET			Packet,
	OUT	PUINT					Flags
	);

--*/

#define NdisQuerySendFlags(_Packet,_Flags)	*(_Flags) = (_Packet)->Private.Flags

//
// The following is the minimum size of packets a miniport must allocate
// when it indicates packets via NdisMIndicatePacket or NdisMCoIndicatePacket
//
#define	PROTOCOL_RESERVED_SIZE_IN_PACKET	16

EXPORT
VOID
NdisReturnPackets(
	IN	PNDIS_PACKET	*		PacketsToReturn,
	IN	UINT					NumberOfPackets
	);

EXPORT
NDIS_STATUS
NdisQueryReceiveInformation(
	IN	NDIS_HANDLE				NdisBindingHandle,
	IN	NDIS_HANDLE				MacContext,
	OUT	PLONGLONG				TimeSent		OPTIONAL,
	OUT	PLONGLONG				TimeReceived	OPTIONAL,
	IN	PUCHAR					Buffer,
	IN	UINT					BufferSize,
	OUT	PUINT					SizeNeeded
	);

//
// Definition for protocol filters.
//

typedef struct _NDIS_PROTOCOL_FILTER
{
	struct _NDIS_PROTOCOL_FILTER *	Next;
	RECEIVE_HANDLER					ReceiveHandler;
	RECEIVE_PACKET_HANDLER			ReceivePacketHandler;
	USHORT							Offset;
	USHORT							Size;
	NDIS_MEDIUM						Medium;	// Mainly for ethernet to differentiate 802.3 and Dix
											// Followed by 'Size' bytes. Should be one of NdisMediumxxxx
} NDIS_PROTOCOL_FILTER, *PNDIS_PROTOCOL_FILTER;


//
// one of these per protocol registered
//

struct _NDIS_PROTOCOL_BLOCK
{
	PNDIS_OPEN_BLOCK				OpenQueue;				// queue of opens for this protocol
	REFERENCE						Ref;					// contains spinlock for OpenQueue
	UINT							Length;					// of this NDIS_PROTOCOL_BLOCK struct
	NDIS41_PROTOCOL_CHARACTERISTICS	ProtocolCharacteristics;// handler addresses

	struct _NDIS_PROTOCOL_BLOCK *	NextProtocol;			// Link to next
	ULONG							MaxPatternSize;
#if defined(NDIS_WRAPPER)
	//
	// Protocol filters
	//
	struct _NDIS_PROTOCOL_FILTER *	ProtocolFilter[NdisMediumMax+1];
	WORK_QUEUE_ITEM					WorkItem;				// Used during NdisRegisterProtocol to
															// notify protocols of existing drivers.
	KMUTEX							Mutex;					// For serialization of Bind/Unbind requests
	PKEVENT							DeregEvent;				// Used by NdisDeregisterProtocol
#endif
};


//
// The following definitions are available only to full MAC drivers.  They
// must not be used by miniport drivers.
//

#if !defined(NDIS_MINIPORT_DRIVER) || defined(NDIS_WRAPPER)

typedef
BOOLEAN
(*PNDIS_INTERRUPT_SERVICE)(
	IN	PVOID					InterruptContext
	);

typedef
VOID
(*PNDIS_DEFERRED_PROCESSING)(
	IN	PVOID					SystemSpecific1,
	IN	PVOID					InterruptContext,
	IN	PVOID					SystemSpecific2,
	IN	PVOID					SystemSpecific3
	);


typedef struct _NDIS_INTERRUPT
{
	PKINTERRUPT					InterruptObject;
	KSPIN_LOCK					DpcCountLock;
	PNDIS_INTERRUPT_SERVICE		MacIsr;     		// Pointer to Mac ISR routine
	PNDIS_DEFERRED_PROCESSING	MacDpc;				// Pointer to Mac DPC routine
	KDPC						InterruptDpc;
	PVOID						InterruptContext;	// Pointer to context for calling
													// adapters ISR and DPC.
	UCHAR						DpcCount;
	BOOLEAN						Removing;			// TRUE if removing interrupt

	//
	// This is used to tell when all the Dpcs for the adapter are completed.
	//
	KEVENT						DpcsCompletedEvent;

} NDIS_INTERRUPT, *PNDIS_INTERRUPT;

//
// Ndis Adapter Information
//

typedef
NDIS_STATUS
(*PNDIS_ACTIVATE_CALLBACK)(
	IN	NDIS_HANDLE				NdisAdatperHandle,
	IN	NDIS_HANDLE				MacAdapterContext,
	IN	ULONG					DmaChannel
	);

typedef struct _NDIS_PORT_DESCRIPTOR
{
	ULONG						InitialPort;
	ULONG						NumberOfPorts;
	PVOID *						PortOffset;
} NDIS_PORT_DESCRIPTOR, *PNDIS_PORT_DESCRIPTOR;

typedef struct _NDIS_ADAPTER_INFORMATION
{
	ULONG						DmaChannel;
	BOOLEAN						Master;
	BOOLEAN						Dma32BitAddresses;
	PNDIS_ACTIVATE_CALLBACK		ActivateCallback;
	NDIS_INTERFACE_TYPE			AdapterType;
	ULONG						PhysicalMapRegistersNeeded;
	ULONG						MaximumPhysicalMapping;
	ULONG						NumberOfPortDescriptors;
	NDIS_PORT_DESCRIPTOR		PortDescriptors[1];	// as many as needed
} NDIS_ADAPTER_INFORMATION, *PNDIS_ADAPTER_INFORMATION;

//
// Function types for NDIS_MAC_CHARACTERISTICS
//

typedef
NDIS_STATUS
(*OPEN_ADAPTER_HANDLER)(
	OUT PNDIS_STATUS			OpenErrorStatus,
	OUT NDIS_HANDLE *			MacBindingHandle,
	OUT PUINT					SelectedMediumIndex,
	IN	PNDIS_MEDIUM			MediumArray,
	IN	UINT					MediumArraySize,
	IN	NDIS_HANDLE				NdisBindingContext,
	IN	NDIS_HANDLE				MacAdapterContext,
	IN	UINT					OpenOptions,
	IN	PSTRING					AddressingInformation OPTIONAL
	);

typedef
NDIS_STATUS
(*CLOSE_ADAPTER_HANDLER)(
	IN	NDIS_HANDLE				MacBindingHandle
	);

typedef
NDIS_STATUS
(*SEND_HANDLER)(
	IN	NDIS_HANDLE				MacBindingHandle,
	IN	PNDIS_PACKET			Packet
	);

typedef
NDIS_STATUS
(*WAN_SEND_HANDLER)(
	IN	NDIS_HANDLE				MacBindingHandle,
	IN	PNDIS_WAN_PACKET		Packet
	);

typedef
NDIS_STATUS
(*TRANSFER_DATA_HANDLER)(
	IN	NDIS_HANDLE				MacBindingHandle,
	IN	NDIS_HANDLE				MacReceiveContext,
	IN	UINT					ByteOffset,
	IN	UINT					BytesToTransfer,
	OUT PNDIS_PACKET			Packet,
	OUT PUINT					BytesTransferred
	);

typedef
NDIS_STATUS
(*WAN_TRANSFER_DATA_HANDLER)(
	VOID
    );

typedef
NDIS_STATUS
(*RESET_HANDLER)(
	IN	NDIS_HANDLE				MacBindingHandle
	);

typedef
NDIS_STATUS
(*REQUEST_HANDLER)(
	IN	NDIS_HANDLE				MacBindingHandle,
	IN	PNDIS_REQUEST			NdisRequest
	);

typedef
NDIS_STATUS
(*QUERY_GLOBAL_STATISTICS_HANDLER)(
	IN	NDIS_HANDLE				MacAdapterContext,
	IN	PNDIS_REQUEST			NdisRequest
	);

typedef
VOID
(*UNLOAD_MAC_HANDLER)(
	IN	NDIS_HANDLE				MacMacContext
	);

typedef
NDIS_STATUS
(*ADD_ADAPTER_HANDLER)(
	IN	NDIS_HANDLE				MacMacContext,
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	IN	PNDIS_STRING			AdapterName
	);

typedef
VOID
(*REMOVE_ADAPTER_HANDLER)(
	IN	NDIS_HANDLE				MacAdapterContext
	);


//
// NDIS 4.0 extension - however available for miniports only
//
typedef
VOID
(*SEND_PACKETS_HANDLER)(
	IN NDIS_HANDLE				MiniportAdapterContext,
	IN PPNDIS_PACKET			PacketArray,
	IN UINT						NumberOfPackets
	);

typedef struct _NDIS_MAC_CHARACTERISTICS
{
	UCHAR						MajorNdisVersion;
	UCHAR						MinorNdisVersion;
	UINT						Reserved;
	OPEN_ADAPTER_HANDLER		OpenAdapterHandler;
	CLOSE_ADAPTER_HANDLER		CloseAdapterHandler;
	SEND_HANDLER				SendHandler;
	TRANSFER_DATA_HANDLER		TransferDataHandler;
	RESET_HANDLER				ResetHandler;
	REQUEST_HANDLER				RequestHandler;
	QUERY_GLOBAL_STATISTICS_HANDLER QueryGlobalStatisticsHandler;
	UNLOAD_MAC_HANDLER			UnloadMacHandler;
	ADD_ADAPTER_HANDLER			AddAdapterHandler;
	REMOVE_ADAPTER_HANDLER		RemoveAdapterHandler;
	NDIS_STRING					Name;

} NDIS_MAC_CHARACTERISTICS, *PNDIS_MAC_CHARACTERISTICS;

typedef	NDIS_MAC_CHARACTERISTICS		NDIS_WAN_MAC_CHARACTERISTICS;
typedef	NDIS_WAN_MAC_CHARACTERISTICS *	PNDIS_WAN_MAC_CHARACTERISTICS;

//
// MAC specific considerations.
//

struct _NDIS_WRAPPER_HANDLE
{
	//
	// These store the PDRIVER_OBJECT that
	// the MAC passes to NdisInitializeWrapper until it can be
	// used by NdisRegisterMac and NdisTerminateWrapper.
	//

	PDRIVER_OBJECT				NdisWrapperDriver;

	HANDLE						NdisWrapperConfigurationHandle;
};


//
// one of these per MAC
//

struct _NDIS_MAC_BLOCK
{
	PNDIS_ADAPTER_BLOCK			AdapterQueue;	// queue of adapters for this MAC
	NDIS_HANDLE					MacMacContext;	// Context for calling MACUnload and
												//	MACAddAdapter.

	REFERENCE					Ref;			// contains spinlock for AdapterQueue
	UINT						Length;			// of this NDIS_MAC_BLOCK structure
	NDIS_MAC_CHARACTERISTICS	MacCharacteristics;// handler addresses
	PNDIS_WRAPPER_HANDLE		NdisMacInfo;	// Mac information.
	PNDIS_MAC_BLOCK				NextMac;
	KEVENT						AdaptersRemovedEvent;// used to find when all adapters are gone.
	BOOLEAN						Unloading;		// TRUE if unloading

	//
	// Extensions added for NT 3.5 support
	//
	PCM_RESOURCE_LIST			PciAssignedResources;

	// Needed for PnP
	UNICODE_STRING				BaseName;
};

//
// one of these per adapter registered on a MAC
//

struct _NDIS_ADAPTER_BLOCK
{
	PDEVICE_OBJECT				DeviceObject;		// created by NdisRegisterAdapter
	PNDIS_MAC_BLOCK				MacHandle;			// pointer to our MAC block
	NDIS_HANDLE					MacAdapterContext;	// context when calling MacOpenAdapter
	NDIS_STRING					AdapterName;		// how NdisOpenAdapter refers to us
	PNDIS_OPEN_BLOCK			OpenQueue;			// queue of opens for this adapter
	PNDIS_ADAPTER_BLOCK			NextAdapter;		// used by MAC's AdapterQueue
	REFERENCE					Ref;				// contains spinlock for OpenQueue
	BOOLEAN						BeingRemoved;		// TRUE if adapter is being removed

	//
	// Resource information
	//
	PCM_RESOURCE_LIST			Resources;

	//
	// Obsolete field.
	//
	ULONG						NotUsed;

	//
	// Wrapper context.
	//
	PVOID						WrapperContext;

	//
	// contains adapter information
	//
	ULONG						BusNumber;
	NDIS_INTERFACE_TYPE			BusType;
	ULONG						ChannelNumber;
	NDIS_INTERFACE_TYPE			AdapterType;
	BOOLEAN						Master;
	ULONG						PhysicalMapRegistersNeeded;
	ULONG						MaximumPhysicalMapping;
	ULONG						InitialPort;
	ULONG						NumberOfPorts;

	//
	// Holds the mapping for ports, if needed.
	//
	PUCHAR						InitialPortMapping;

	//
	// TRUE if InitialPortMapping was mapped with NdisMapIoSpace.
	//
	BOOLEAN						InitialPortMapped;

	//
	// This is the offset added to the port passed to NdisXXXPort to
	// get to the real value to be passed to the NDIS_XXX_PORT macros.
	// It equals InitialPortMapping - InitialPort; that is, the
	// mapped "address" of port 0, even if we didn't actually
	// map port 0.
	//
	PUCHAR						PortOffset;

	//
	// Holds the map registers for this adapter.
	//
	PMAP_REGISTER_ENTRY			MapRegisters;

	//
	// These two are used temporarily while allocating
	// the map registers.
	//
	KEVENT						AllocationEvent;
	UINT						CurrentMapRegister;
	PADAPTER_OBJECT				SystemAdapterObject;

#if defined(NDIS_WRAPPER)
    //
    // Store information here to track adapters
    //
    ULONG                       BusId;
    ULONG                       SlotNumber;

	//
	// Needed for PnP. Upcased version. The buffer is allocated as part of the
	// NDIS_ADAPTER_BLOCK itself.
	//
	UNICODE_STRING				BaseName;
#endif
};

//
// one of these per open on an adapter/protocol
//

struct _NDIS_OPEN_BLOCK
{
	PNDIS_MAC_BLOCK				MacHandle;			// pointer to our MAC
	NDIS_HANDLE					MacBindingHandle;	// context when calling MacXX funcs
	PNDIS_ADAPTER_BLOCK			AdapterHandle;		// pointer to our adapter
	PNDIS_PROTOCOL_BLOCK		ProtocolHandle;		// pointer to our protocol
	NDIS_HANDLE					ProtocolBindingContext;// context when calling ProtXX funcs
	PNDIS_OPEN_BLOCK			AdapterNextOpen;	// used by adapter's OpenQueue
	PNDIS_OPEN_BLOCK			ProtocolNextOpen;	// used by protocol's OpenQueue
	PFILE_OBJECT				FileObject;			// created by operating system
	BOOLEAN						Closing;			// TRUE when removing this struct
	BOOLEAN						Unloading;			// TRUE when processing unload
	NDIS_HANDLE					CloseRequestHandle;	// 0 indicates an internal close
	KSPIN_LOCK					SpinLock;			// guards Closing
	PNDIS_OPEN_BLOCK			NextGlobalOpen;

	//
	// These are optimizations for getting to MAC routines.	They are not
	// necessary, but are here to save a dereference through the MAC block.
	//

	SEND_HANDLER				SendHandler;
	TRANSFER_DATA_HANDLER		TransferDataHandler;

	//
	// These are optimizations for getting to PROTOCOL routines.  They are not
	// necessary, but are here to save a dereference through the PROTOCOL block.
	//

	SEND_COMPLETE_HANDLER		SendCompleteHandler;
	TRANSFER_DATA_COMPLETE_HANDLER TransferDataCompleteHandler;
	RECEIVE_HANDLER				ReceiveHandler;
	RECEIVE_COMPLETE_HANDLER	ReceiveCompleteHandler;

	//
	// Extentions to the OPEN_BLOCK since Product 1.
	//
	RECEIVE_HANDLER				PostNt31ReceiveHandler;
	RECEIVE_COMPLETE_HANDLER	PostNt31ReceiveCompleteHandler;

	//
	// NDIS 4.0 extensions
	//
	RECEIVE_PACKET_HANDLER		ReceivePacketHandler;
	SEND_PACKETS_HANDLER		SendPacketsHandler;

	//
	// Needed for PnP
	//
	UNICODE_STRING				AdapterName;		// Upcased name of the adapter we are bound to
};

EXPORT
VOID
NdisInitializeTimer(
	IN OUT PNDIS_TIMER			Timer,
	IN	PNDIS_TIMER_FUNCTION	TimerFunction,
	IN	PVOID					FunctionContext
	);

/*++
VOID
NdisCancelTimer(
	IN	PNDIS_TIMER				Timer,
	OUT PBOOLEAN				TimerCancelled
	)
--*/
#define NdisCancelTimer(NdisTimer,TimerCancelled) \
			(*(TimerCancelled) = KeCancelTimer(&((NdisTimer)->Timer)))

//
// Shared memory
//

EXPORT
VOID
NdisAllocateSharedMemory(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	ULONG					Length,
	IN	BOOLEAN 				Cached,
	OUT PVOID *					VirtualAddress,
	OUT PNDIS_PHYSICAL_ADDRESS	PhysicalAddress
	);

EXPORT
VOID
NdisFreeSharedMemory(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	ULONG					Length,
	IN	BOOLEAN					Cached,
	IN	PVOID					VirtualAddress,
	IN	NDIS_PHYSICAL_ADDRESS	PhysicalAddress
	);

//
// Requests Used by MAC Drivers
//

EXPORT
VOID
NdisRegisterMac(
	OUT PNDIS_STATUS			Status,
	OUT PNDIS_HANDLE			NdisMacHandle,
	IN	NDIS_HANDLE				NdisWrapperHandle,
	IN	NDIS_HANDLE				MacMacContext,
	IN	PNDIS_MAC_CHARACTERISTICS MacCharacteristics,
	IN	UINT					CharacteristicsLength
	);

EXPORT
VOID
NdisDeregisterMac(
	OUT PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				NdisMacHandle
	);


EXPORT
NDIS_STATUS
NdisRegisterAdapter(
	OUT PNDIS_HANDLE			NdisAdapterHandle,
	IN	NDIS_HANDLE				NdisMacHandle,
	IN	NDIS_HANDLE				MacAdapterContext,
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	IN	PNDIS_STRING			AdapterName,
	IN	PVOID					AdapterInformation
	);

EXPORT
NDIS_STATUS
NdisDeregisterAdapter(
	IN	NDIS_HANDLE				NdisAdapterHandle
	);

EXPORT
VOID
NdisRegisterAdapterShutdownHandler(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	PVOID					ShutdownContext,
	IN	ADAPTER_SHUTDOWN_HANDLER ShutdownHandler
	);

EXPORT
VOID
NdisDeregisterAdapterShutdownHandler(
	IN	NDIS_HANDLE				NdisAdapterHandle
	);

EXPORT
VOID
NdisReleaseAdapterResources(
	IN	NDIS_HANDLE				NdisAdapterHandle
	);

EXPORT
VOID
NdisCompleteOpenAdapter(
	IN	NDIS_HANDLE				NdisBindingContext,
	IN	NDIS_STATUS				Status,
	IN	NDIS_STATUS				OpenErrorStatus
	);


EXPORT
VOID
NdisCompleteCloseAdapter(
	IN	NDIS_HANDLE NdisBindingContext,
	IN	NDIS_STATUS Status
	);


// VOID
// NdisCompleteSend(
//	IN NDIS_HANDLE NdisBindingContext,
//	IN PNDIS_PACKET Packet,
//	IN NDIS_STATUS Status
//	);
#define NdisCompleteSend(NdisBindingContext, Packet, Status)			\
{																		\
	(((PNDIS_OPEN_BLOCK)(NdisBindingContext))->SendCompleteHandler)(	\
		((PNDIS_OPEN_BLOCK)(NdisBindingContext))->ProtocolBindingContext,\
		(Packet),														\
		(Status));														\
}


// VOID
// NdisCompleteTransferData(
//	IN NDIS_HANDLE NdisBindingContext,
//	IN PNDIS_PACKET Packet,
//	IN NDIS_STATUS Status,
//	IN UINT BytesTransferred
//	);
#define NdisCompleteTransferData(NdisBindingContext,						\
								 Packet,									\
								 Status,									\
								 BytesTransferred)							\
{																			\
	(((PNDIS_OPEN_BLOCK)(NdisBindingContext))->TransferDataCompleteHandler)(\
		((PNDIS_OPEN_BLOCK)(NdisBindingContext))->ProtocolBindingContext,	\
		(Packet),															\
		(Status),															\
		(BytesTransferred));												\
}

// VOID
// NdisCompleteReset(
//	IN NDIS_HANDLE				NdisBindingContext,
//	IN NDIS_STATUS 				Status
//	);
#define NdisCompleteReset(NdisBindingContext, Status)	\
{ \
	(((PNDIS_OPEN_BLOCK)(NdisBindingContext))->ProtocolHandle->ProtocolCharacteristics.ResetCompleteHandler)( \
		((PNDIS_OPEN_BLOCK)(NdisBindingContext))->ProtocolBindingContext, \
		Status); \
}


// VOID
// NdisCompleteRequest(
//	IN NDIS_HANDLE				NdisBindingContext,
//	IN PNDIS_REQUEST			NdisRequest,
//	IN NDIS_STATUS				Status
//	);
#define NdisCompleteRequest(NdisBindingContext, NdisRequest, Status)\
{																	\
	(((PNDIS_OPEN_BLOCK)(NdisBindingContext))->ProtocolHandle->ProtocolCharacteristics.RequestCompleteHandler)( \
		((PNDIS_OPEN_BLOCK)(NdisBindingContext))->ProtocolBindingContext, \
		NdisRequest, \
		Status); \
}

// VOID
// NdisIndicateReceive(
//	OUT PNDIS_STATUS			Status,
//	IN NDIS_HANDLE				NdisBindingContext,
//	IN NDIS_HANDLE				MacReceiveContext,
//	IN PVOID					HeaderBuffer,
//	IN UINT						HeaderBufferSize,
//	IN PVOID					LookaheadBuffer,
//	IN UINT						LookaheadBufferSize,
//	IN UINT						PacketSize
//	);
#define NdisIndicateReceive(Status, \
							NdisBindingContext, \
							MacReceiveContext, \
							HeaderBuffer, \
							HeaderBufferSize, \
							LookaheadBuffer, \
							LookaheadBufferSize, \
							PacketSize) \
{\
	KIRQL oldIrql;\
	KeRaiseIrql( DISPATCH_LEVEL, &oldIrql );\
	*(Status) = \
		(((PNDIS_OPEN_BLOCK)(NdisBindingContext))->PostNt31ReceiveHandler)( \
			((PNDIS_OPEN_BLOCK)(NdisBindingContext))->ProtocolBindingContext, \
			(MacReceiveContext), \
			(HeaderBuffer), \
			(HeaderBufferSize), \
			(LookaheadBuffer), \
			(LookaheadBufferSize), \
			(PacketSize)); \
	KeLowerIrql( oldIrql );\
}

//
// Used by the filter packages for indicating receives
//

#define FilterIndicateReceive( \
	Status, \
	NdisBindingContext, \
	MacReceiveContext, \
	HeaderBuffer, \
	HeaderBufferSize, \
	LookaheadBuffer, \
	LookaheadBufferSize, \
	PacketSize \
	) \
{\
	*(Status) = \
		(((PNDIS_OPEN_BLOCK)(NdisBindingContext))->PostNt31ReceiveHandler)( \
			((PNDIS_OPEN_BLOCK)(NdisBindingContext))->ProtocolBindingContext, \
			(MacReceiveContext), \
			(HeaderBuffer), \
			(HeaderBufferSize), \
			(LookaheadBuffer), \
			(LookaheadBufferSize), \
			(PacketSize)); \
}


// VOID
// NdisIndicateReceiveComplete(
//	IN NDIS_HANDLE				NdisBindingContext
//	);
#define NdisIndicateReceiveComplete(NdisBindingContext) \
{\
	KIRQL oldIrql;\
	KeRaiseIrql( DISPATCH_LEVEL, &oldIrql );\
	(((PNDIS_OPEN_BLOCK)(NdisBindingContext))->PostNt31ReceiveCompleteHandler)( \
		((PNDIS_OPEN_BLOCK)(NdisBindingContext))->ProtocolBindingContext);\
	KeLowerIrql( oldIrql );\
}

//
// Used by the filter packages for indicating receive completion
//

#define FilterIndicateReceiveComplete(NdisBindingContext) \
{\
	(((PNDIS_OPEN_BLOCK)(NdisBindingContext))->PostNt31ReceiveCompleteHandler)( \
		((PNDIS_OPEN_BLOCK)(NdisBindingContext))->ProtocolBindingContext);\
}

// VOID
// NdisIndicateStatus(
//	IN NDIS_HANDLE				NdisBindingContext,
//	IN NDIS_STATUS				GeneralStatus,
//	IN PVOID					StatusBuffer,
//	IN UINT						StatusBufferSize
//	);
#define NdisIndicateStatus( \
	NdisBindingContext, \
	GeneralStatus, \
	StatusBuffer, \
	StatusBufferSize \
	) \
{\
	(((PNDIS_OPEN_BLOCK)(NdisBindingContext))->ProtocolHandle->ProtocolCharacteristics.StatusHandler)( \
		((PNDIS_OPEN_BLOCK)(NdisBindingContext))->ProtocolBindingContext, \
		(GeneralStatus), \
		(StatusBuffer), \
		(StatusBufferSize)); \
}


// VOID
// NdisIndicateStatusComplete(
//	IN NDIS_HANDLE				NdisBindingContext
//	);
#define NdisIndicateStatusComplete(NdisBindingContext) \
{ \
	(((PNDIS_OPEN_BLOCK)(NdisBindingContext))->ProtocolHandle->ProtocolCharacteristics.StatusCompleteHandler)( \
		((PNDIS_OPEN_BLOCK)(NdisBindingContext))->ProtocolBindingContext); \
}

EXPORT
VOID
NdisCompleteQueryStatistics(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	PNDIS_REQUEST			NdisRequest,
	IN	NDIS_STATUS				Status
	);

//
// Interlocked support functions
//

#if BINARY_COMPATIBLE

EXPORT
ULONG
NdisInterlockedIncrement(
    IN PULONG Addend
    );

EXPORT
ULONG
NdisInterlockedIncrement(
    IN PULONG Addend
    );

EXPORT
VOID
NdisInterlockedAddUlong(
	IN	PULONG					Addend,
	IN	ULONG					Increment,
	IN	PNDIS_SPIN_LOCK			SpinLock
	);


EXPORT
PLIST_ENTRY
NdisInterlockedInsertHeadList(
	IN	PLIST_ENTRY				ListHead,
	IN	PLIST_ENTRY				ListEntry,
	IN	PNDIS_SPIN_LOCK			SpinLock
	);


EXPORT
PLIST_ENTRY
NdisInterlockedInsertTailList(
	IN	PLIST_ENTRY				ListHead,
	IN	PLIST_ENTRY				ListEntry,
	IN	PNDIS_SPIN_LOCK			SpinLock
	);


EXPORT
PLIST_ENTRY
NdisInterlockedRemoveHeadList(
	IN	PLIST_ENTRY				ListHead,
	IN	PNDIS_SPIN_LOCK			SpinLock
	);

#else

#define	NdisInterlockedIncrement(Addend)	InterlockedIncrement(Addend)

#define	NdisInterlockedDecrement(Addend)	InterlockedDecrement(Addend)

#define NdisInterlockedAddUlong(_Addend, _Increment, _SpinLock) \
	ExInterlockedAddUlong(_Addend, _Increment, &(_SpinLock)->SpinLock)

#define NdisInterlockedInsertHeadList(_ListHead, _ListEntry, _SpinLock) \
	ExInterlockedInsertHeadList(_ListHead, _ListEntry, &(_SpinLock)->SpinLock)

#define NdisInterlockedInsertTailList(_ListHead, _ListEntry, _SpinLock) \
	ExInterlockedInsertTailList(_ListHead, _ListEntry, &(_SpinLock)->SpinLock)

#define NdisInterlockedRemoveHeadList(_ListHead, _SpinLock) \
	ExInterlockedRemoveHeadList(_ListHead, &(_SpinLock)->SpinLock)

#endif

//
// Operating System Requests
//

EXPORT
VOID
NdisMapIoSpace(
	OUT PNDIS_STATUS			Status,
	OUT PVOID *					VirtualAddress,
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	NDIS_PHYSICAL_ADDRESS 	PhysicalAddress,
	IN	UINT					Length
	);

#if defined(_ALPHA_)

/*++
VOID
NdisUnmapIoSpace(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	PVOID					VirtualAddress,
	IN	UINT					Length
	)
--*/
#define NdisUnmapIoSpace(Handle,VirtualAddress,Length)

#else

/*++
VOID
NdisUnmapIoSpace(
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	PVOID					VirtualAddress,
	IN	UINT					Length
	)
--*/
#define NdisUnmapIoSpace(Handle,VirtualAddress,Length)	MmUnmapIoSpace((VirtualAddress), (Length));

#endif

EXPORT
VOID
NdisInitializeInterrupt(
	OUT PNDIS_STATUS			Status,
	IN OUT PNDIS_INTERRUPT		Interrupt,
	IN	NDIS_HANDLE				NdisAdapterHandle,
	IN	PNDIS_INTERRUPT_SERVICE	InterruptServiceRoutine,
	IN	PVOID					InterruptContext,
	IN	PNDIS_DEFERRED_PROCESSING DeferredProcessingRoutine,
	IN	UINT					InterruptVector,
	IN	UINT					InterruptLevel,
	IN	BOOLEAN					SharedInterrupt,
	IN	NDIS_INTERRUPT_MODE		InterruptMode
	);

EXPORT
VOID
NdisRemoveInterrupt(
	IN	PNDIS_INTERRUPT			Interrupt
	);

/*++
BOOLEAN
NdisSynchronizeWithInterrupt(
	IN	PNDIS_INTERRUPT			Interrupt,
	IN	PVOID					SynchronizeFunction,
	IN	PVOID					SynchronizeContext
	)
--*/

#define NdisSynchronizeWithInterrupt(Interrupt,Function,Context)	\
			KeSynchronizeExecution((Interrupt)->InterruptObject,	\
								   (PKSYNCHRONIZE_ROUTINE)Function,	\
								   Context)

//
// Physical Mapping
//

//
// VOID
// NdisStartBufferPhysicalMapping(
//	IN NDIS_HANDLE NdisAdapterHandle,
//	IN PNDIS_BUFFER Buffer,
//	IN ULONG PhysicalMapRegister,
//	IN BOOLEAN WriteToDevice,
//	OUT PNDIS_PHYSICAL_ADDRESS_UNIT PhysicalAddressArray,
//	OUT PUINT ArraySize
//	);
//

#define NdisStartBufferPhysicalMapping(_NdisAdapterHandle,						\
									   _Buffer,									\
									   _PhysicalMapRegister,					\
									   _Write,									\
									   _PhysicalAddressArray,					\
									   _ArraySize)								\
{																				\
	PNDIS_ADAPTER_BLOCK _AdaptP = (PNDIS_ADAPTER_BLOCK)(_NdisAdapterHandle);	\
	PHYSICAL_ADDRESS _LogicalAddress;											\
	PUCHAR _VirtualAddress;														\
	ULONG _LengthRemaining;														\
	ULONG _LengthMapped;														\
	UINT _CurrentArrayLocation;													\
	_VirtualAddress = MmGetMdlVirtualAddress(_Buffer);							\
	_LengthRemaining = MmGetMdlByteCount(_Buffer);								\
	_CurrentArrayLocation = 0;													\
	while (_LengthRemaining > 0)												\
	{																			\
		_LengthMapped = _LengthRemaining;										\
		_LogicalAddress = IoMapTransfer(NULL,									\
										_Buffer,								\
										_AdaptP->MapRegisters[_PhysicalMapRegister].MapRegister,\
										_VirtualAddress,						\
										&_LengthMapped,							\
										_Write);								\
		(_PhysicalAddressArray)[_CurrentArrayLocation].PhysicalAddress = _LogicalAddress;\
		(_PhysicalAddressArray)[_CurrentArrayLocation].Length = _LengthMapped;	\
		_LengthRemaining -= _LengthMapped;										\
		_VirtualAddress += _LengthMapped;										\
		++_CurrentArrayLocation;												\
	}																			\
	_AdaptP->MapRegisters[_PhysicalMapRegister].WriteToDevice = (_Write);		\
	*(_ArraySize) = _CurrentArrayLocation;										\
}


//
// VOID
// NdisCompleteBufferPhysicalMapping(
//	IN	NDIS_HANDLE				NdisAdapterHandle,
//	IN	PNDIS_BUFFER			Buffer,
//	IN	ULONG					PhysicalMapRegister
//	);
//

#define NdisCompleteBufferPhysicalMapping(_NdisAdapterHandle,					\
										  _Buffer,								\
										  _PhysicalMapRegister					\
	)																			\
{																				\
	PNDIS_ADAPTER_BLOCK _AdaptP = (PNDIS_ADAPTER_BLOCK)(_NdisAdapterHandle);	\
	IoFlushAdapterBuffers(NULL,													\
						  _Buffer,												\
						  _AdaptP->MapRegisters[_PhysicalMapRegister].MapRegister,\
						  MmGetMdlVirtualAddress(_Buffer),						\
						  MmGetMdlByteCount(_Buffer),							\
						  _AdaptP->MapRegisters[_PhysicalMapRegister].WriteToDevice);\
}

#endif // !defined(NDIS_MINIPORT_DRIVER) || defined(NDIS_WRAPPER)

//
// The following definitions are available only to miniport drivers.  They
// must not be used by full MAC drivers.
//

#if defined(NDIS_MINIPORT_DRIVER) || defined(NDIS_WRAPPER)

#include <afilter.h>
#include <efilter.h>
#include <tfilter.h>
#include <ffilter.h>

#define NDIS_M_MAX_MULTI_LIST 32
#define NDIS_M_MAX_LOOKAHEAD 526

//
// declare these first since they point to each other
//

typedef struct _NDIS_M_DRIVER_BLOCK		NDIS_M_DRIVER_BLOCK,*PNDIS_M_DRIVER_BLOCK;
typedef struct _NDIS_MINIPORT_BLOCK		NDIS_MINIPORT_BLOCK,*PNDIS_MINIPORT_BLOCK;
typedef struct _NDIS_M_PROTOCOL_BLOCK	NDIS_M_PROTOCOL_BLOCK,*PNDIS_M_PROTOCOL_BLOCK;
typedef struct _NDIS_M_OPEN_BLOCK		NDIS_M_OPEN_BLOCK,*PNDIS_M_OPEN_BLOCK;
typedef struct _CO_CALL_PARAMETERS		CO_CALL_PARAMETERS, *PCO_CALL_PARAMETERS;
typedef struct _CO_MEDIA_PARAMETERS		CO_MEDIA_PARAMETERS, *PCO_MEDIA_PARAMETERS;
typedef	struct _NDIS_CALL_MANAGER_CHARACTERISTICS *PNDIS_CALL_MANAGER_CHARACTERISTICS;


//
// Function types for NDIS_MINIPORT_CHARACTERISTICS
//


typedef
BOOLEAN
(*W_CHECK_FOR_HANG_HANDLER)(
	IN	NDIS_HANDLE				MiniportAdapterContext
	);

typedef
VOID
(*W_DISABLE_INTERRUPT_HANDLER)(
	IN	NDIS_HANDLE				MiniportAdapterContext
	);

typedef
VOID
(*W_ENABLE_INTERRUPT_HANDLER)(
	IN	NDIS_HANDLE				MiniportAdapterContext
	);

typedef
VOID
(*W_HALT_HANDLER)(
	IN	NDIS_HANDLE				MiniportAdapterContext
	);

typedef
VOID
(*W_HANDLE_INTERRUPT_HANDLER)(
	IN	NDIS_HANDLE				MiniportAdapterContext
	);

typedef
NDIS_STATUS
(*W_INITIALIZE_HANDLER)(
	OUT PNDIS_STATUS			OpenErrorStatus,
	OUT PUINT					SelectedMediumIndex,
	IN	PNDIS_MEDIUM			MediumArray,
	IN	UINT					MediumArraySize,
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	NDIS_HANDLE				WrapperConfigurationContext
	);

typedef
VOID
(*W_ISR_HANDLER)(
	OUT PBOOLEAN				InterruptRecognized,
	OUT PBOOLEAN				QueueMiniportHandleInterrupt,
	IN	NDIS_HANDLE				MiniportAdapterContext
	);

typedef
NDIS_STATUS
(*W_QUERY_INFORMATION_HANDLER)(
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	NDIS_OID				Oid,
	IN	PVOID					InformationBuffer,
	IN	ULONG					InformationBufferLength,
	OUT PULONG					BytesWritten,
	OUT PULONG					BytesNeeded
	);

typedef
NDIS_STATUS
(*W_RECONFIGURE_HANDLER)(
	OUT PNDIS_STATUS			OpenErrorStatus,
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	NDIS_HANDLE				WrapperConfigurationContext
	);

typedef
NDIS_STATUS
(*W_RESET_HANDLER)(
	OUT PBOOLEAN				AddressingReset,
	IN	NDIS_HANDLE				MiniportAdapterContext
	);

typedef
NDIS_STATUS
(*W_SEND_HANDLER)(
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	PNDIS_PACKET			Packet,
	IN	UINT					Flags
	);

typedef
NDIS_STATUS
(*WM_SEND_HANDLER)(
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	NDIS_HANDLE				NdisLinkHandle,
	IN	PNDIS_WAN_PACKET		Packet
	);

typedef
NDIS_STATUS
(*W_SET_INFORMATION_HANDLER)(
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	NDIS_OID				Oid,
	IN	PVOID					InformationBuffer,
	IN	ULONG					InformationBufferLength,
	OUT PULONG					BytesRead,
	OUT PULONG					BytesNeeded
	);

typedef
NDIS_STATUS
(*W_TRANSFER_DATA_HANDLER)(
	OUT PNDIS_PACKET			Packet,
	OUT PUINT					BytesTransferred,
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	NDIS_HANDLE				MiniportReceiveContext,
	IN	UINT					ByteOffset,
	IN	UINT					BytesToTransfer
	);

typedef
NDIS_STATUS
(*WM_TRANSFER_DATA_HANDLER)(
	VOID
	);

typedef struct _NDIS30_MINIPORT_CHARACTERISTICS
{
	UCHAR						MajorNdisVersion;
	UCHAR						MinorNdisVersion;
	UINT						Reserved;
	W_CHECK_FOR_HANG_HANDLER	CheckForHangHandler;
	W_DISABLE_INTERRUPT_HANDLER	DisableInterruptHandler;
	W_ENABLE_INTERRUPT_HANDLER	EnableInterruptHandler;
	W_HALT_HANDLER				HaltHandler;
	W_HANDLE_INTERRUPT_HANDLER	HandleInterruptHandler;
	W_INITIALIZE_HANDLER		InitializeHandler;
	W_ISR_HANDLER				ISRHandler;
	W_QUERY_INFORMATION_HANDLER QueryInformationHandler;
	W_RECONFIGURE_HANDLER		ReconfigureHandler;
	W_RESET_HANDLER				ResetHandler;
	union
	{
		W_SEND_HANDLER			SendHandler;
		WM_SEND_HANDLER			WanSendHandler;
	};
	W_SET_INFORMATION_HANDLER	SetInformationHandler;
	union
	{
		W_TRANSFER_DATA_HANDLER	TransferDataHandler;
		WM_TRANSFER_DATA_HANDLER WanTransferDataHandler;
	};
} NDIS30_MINIPORT_CHARACTERISTICS;

//
// Miniport extensions for NDIS 4.0
//
typedef
VOID
(*W_RETURN_PACKET_HANDLER)(
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	PNDIS_PACKET			Packet
	);

//
// NDIS 4.0 extension
//
typedef
VOID
(*W_SEND_PACKETS_HANDLER)(
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	PPNDIS_PACKET			PacketArray,
	IN	UINT					NumberOfPackets
	);

typedef
VOID
(*W_ALLOCATE_COMPLETE_HANDLER)(
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	PVOID					VirtualAddress,
	IN	PNDIS_PHYSICAL_ADDRESS	PhysicalAddress,
	IN	ULONG					Length,
	IN	PVOID					Context
);

typedef struct _NDIS40_MINIPORT_CHARACTERISTICS
{
#ifdef __cplusplus
	NDIS30_MINIPORT_CHARACTERISTICS	Ndis30Chars;
#else
	NDIS30_MINIPORT_CHARACTERISTICS;
#endif
	//
	// Extensions for NDIS 4.0
	//
	W_RETURN_PACKET_HANDLER		ReturnPacketHandler;
	W_SEND_PACKETS_HANDLER		SendPacketsHandler;
	W_ALLOCATE_COMPLETE_HANDLER	AllocateCompleteHandler;

} NDIS40_MINIPORT_CHARACTERISTICS;


//
// WARNING: NDIS v4.1 is under construction. Do not use.
//

//
// Miniport extensions for NDIS 4.1
//
//
// NDIS 4.1 extension - however available for miniports only
//

//
// W_CO_CREATE_VC_HANDLER is a synchronous call
//
typedef
NDIS_STATUS
(*W_CO_CREATE_VC_HANDLER)(
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	NDIS_HANDLE				NdisVcHandle,
	OUT	PNDIS_HANDLE			MiniportVcContext
	);

typedef
NDIS_STATUS
(*W_CO_DELETE_VC_HANDLER)(
	IN	NDIS_HANDLE				MiniportVcContext
	);

typedef
NDIS_STATUS
(*W_CO_ACTIVATE_VC_HANDLER)(
	IN	NDIS_HANDLE				MiniportVcContext,
	IN OUT PCO_CALL_PARAMETERS	CallParameters
	);

typedef
NDIS_STATUS
(*W_CO_DEACTIVATE_VC_HANDLER)(
	IN	NDIS_HANDLE				MiniportVcContext
	);

typedef
VOID
(*W_CO_SEND_PACKETS_HANDLER)(
	IN	NDIS_HANDLE				MiniportVcContext,
	IN	PPNDIS_PACKET			PacketArray,
	IN	UINT					NumberOfPackets
	);

typedef
NDIS_STATUS
(*W_CO_REQUEST_HANDLER)(
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	NDIS_HANDLE				MiniportVcContext	OPTIONAL,
	IN OUT PNDIS_REQUEST		NdisRequest
	);

typedef struct _NDIS41_MINIPORT_CHARACTERISTICS
{
#ifdef __cplusplus
	NDIS40_MINIPORT_CHARACTERISTICS	Ndis40Chars;
#else
	NDIS40_MINIPORT_CHARACTERISTICS;
#endif
	//
	// Extensions for NDIS 4.1
	//
	W_CO_CREATE_VC_HANDLER		CoCreateVcHandler;
	W_CO_DELETE_VC_HANDLER		CoDeleteVcHandler;
	W_CO_ACTIVATE_VC_HANDLER	CoActivateVcHandler;
	W_CO_DEACTIVATE_VC_HANDLER	CoDeactivateVcHandler;
	W_CO_SEND_PACKETS_HANDLER	CoSendPacketsHandler;
	W_CO_REQUEST_HANDLER		CoRequestHandler;
} NDIS41_MINIPORT_CHARACTERISTICS;

#ifdef NDIS41_MINIPORT
typedef struct _NDIS41_MINIPORT_CHARACTERISTICS	NDIS_MINIPORT_CHARACTERISTICS;
#else
#ifdef NDIS40_MINIPORT
typedef struct _NDIS40_MINIPORT_CHARACTERISTICS	NDIS_MINIPORT_CHARACTERISTICS;
#else
typedef struct _NDIS30_MINIPORT_CHARACTERISTICS	NDIS_MINIPORT_CHARACTERISTICS;
#endif
#endif
typedef	NDIS_MINIPORT_CHARACTERISTICS *PNDIS_MINIPORT_CHARACTERISTICS;
typedef	NDIS_MINIPORT_CHARACTERISTICS	NDIS_WAN_MINIPORT_CHARACTERISTICS;
typedef	NDIS_WAN_MINIPORT_CHARACTERISTICS *	PNDIS_MINIPORT_CHARACTERISTICS;

//
// one of these per Driver
//

struct _NDIS_M_DRIVER_BLOCK
{
	PNDIS_MINIPORT_BLOCK		MiniportQueue;		// queue of mini-ports for this driver
	NDIS_HANDLE					MiniportIdField;

	REFERENCE					Ref;				// contains spinlock for MiniportQueue
	UINT						Length;				// of this NDIS_DRIVER_BLOCK structure
	NDIS41_MINIPORT_CHARACTERISTICS MiniportCharacteristics; // handler addresses
	PNDIS_WRAPPER_HANDLE		NdisDriverInfo;		// Driver information.
	PNDIS_M_DRIVER_BLOCK		NextDriver;
	PNDIS_MAC_BLOCK				FakeMac;
	KEVENT						MiniportsRemovedEvent;// used to find when all mini-ports are gone.
	BOOLEAN						Unloading;			// TRUE if unloading

	// Needed for PnP
	UNICODE_STRING				BaseName;
};

typedef struct _NDIS_MINIPORT_INTERRUPT
{
	PKINTERRUPT					InterruptObject;
	KSPIN_LOCK					DpcCountLock;
	PVOID						MiniportIdField;
	W_ISR_HANDLER				MiniportIsr;
	W_HANDLE_INTERRUPT_HANDLER	MiniportDpc;
	KDPC						InterruptDpc;
	PNDIS_MINIPORT_BLOCK		Miniport;

	UCHAR						DpcCount;
	BOOLEAN						Filler1;

	//
	// This is used to tell when all the Dpcs for the adapter are completed.
	//

	KEVENT						DpcsCompletedEvent;

	BOOLEAN						SharedInterrupt;
	BOOLEAN						IsrRequested;

} NDIS_MINIPORT_INTERRUPT, *PNDIS_MINIPORT_INTERRUPT;


typedef struct _NDIS_MINIPORT_TIMER
{
	KTIMER						Timer;
	KDPC						Dpc;
	PNDIS_TIMER_FUNCTION		MiniportTimerFunction;
	PVOID						MiniportTimerContext;
	PNDIS_MINIPORT_BLOCK		Miniport;
	struct _NDIS_MINIPORT_TIMER *NextDeferredTimer;
} NDIS_MINIPORT_TIMER, *PNDIS_MINIPORT_TIMER;

//
// Pending NdisOpenAdapter() structure (for miniports only).
//

typedef struct _MINIPORT_PENDING_OPEN *PMINIPORT_PENDING_OPEN;

typedef struct _MINIPORT_PENDING_OPEN
{

	PMINIPORT_PENDING_OPEN		NextPendingOpen;
	PNDIS_HANDLE				NdisBindingHandle;
	PNDIS_MINIPORT_BLOCK		Miniport;
	PVOID						NewOpenP;
	PVOID						FileObject;
	NDIS_HANDLE					NdisProtocolHandle;
	NDIS_HANDLE					ProtocolBindingContext;
	PNDIS_STRING				AdapterName;
	UINT						OpenOptions;
	PSTRING						AddressingInformation;
	ULONG						Flags;
	NDIS_STATUS					Status;
	NDIS_STATUS					OpenErrorStatus;
#if defined(NDIS_WRAPPER)
	WORK_QUEUE_ITEM				WorkItem;
#endif
} MINIPORT_PENDING_OPEN, *PMINIPORT_PENDING_OPEN;

//
// Flag definitions for MINIPORT_PENDING_OPEN
//
#define fPENDING_OPEN_USING_ENCAPSULATION	0x00000001

//
//  Defines the type of work item.
//
typedef enum _NDIS_WORK_ITEM_TYPE
{
	NdisWorkItemDpc,
	NdisWorkItemResetRequested,
	NdisWorkItemRequest,
	NdisWorkItemSend,
	NdisWorkItemHalt,
	NdisWorkItemSendLoopback,
	NdisWorkItemResetInProgress,
	NdisWorkItemTimer,
	NdisWorkItemPendingOpen,
	NdisWorkItemMiniportCallback,
	NdisMaxWorkItems
} NDIS_WORK_ITEM_TYPE, *PNDIS_WORK_ITEM_TYPE;


#define	NUMBER_OF_WORK_ITEM_TYPES	NdisMaxWorkItems
#define	NUMBER_OF_SINGLE_WORK_ITEMS	6

typedef
VOID
(*FILTER_PACKET_INDICATION_HANDLER)(
	IN	NDIS_HANDLE				Miniport,
	IN	PPNDIS_PACKET			PacketArray,
	IN	UINT					NumberOfPackets
	);

typedef
VOID
(*NDIS_M_SEND_COMPLETE_HANDLER)(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	PNDIS_PACKET			Packet,
	IN	NDIS_STATUS				Status
	);

typedef
VOID
(*NDIS_M_SEND_RESOURCES_HANDLER)(
	IN	NDIS_HANDLE				MiniportAdapterHandle
	);

typedef
VOID
(*NDIS_M_RESET_COMPLETE_HANDLER)(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	NDIS_STATUS				Status,
	IN	BOOLEAN					AddressingReset
	);

typedef
VOID
(FASTCALL *NDIS_M_PROCESS_DEFERRED)(
	IN	PNDIS_MINIPORT_BLOCK	Miniport
	);

typedef
BOOLEAN
(FASTCALL *NDIS_M_START_SENDS)(
	IN	PNDIS_MINIPORT_BLOCK	Miniport
	);

typedef
NDIS_STATUS
(FASTCALL *NDIS_M_QUEUE_WORK_ITEM)(
	IN	PNDIS_MINIPORT_BLOCK	Miniport,
	IN	NDIS_WORK_ITEM_TYPE		WorkItemType,
	IN	 PVOID					WorkItemContext1,
	IN	PVOID					WorkItemContext2
	);

typedef
NDIS_STATUS
(FASTCALL *NDIS_M_QUEUE_NEW_WORK_ITEM)(
	IN	PNDIS_MINIPORT_BLOCK	Miniport,
	IN	NDIS_WORK_ITEM_TYPE 	WorkItemType,
	IN	 PVOID					WorkItemContext1,
	IN	PVOID					WorkItemContext2
	);

typedef
VOID
(FASTCALL *NDIS_M_DEQUEUE_WORK_ITEM)(
	IN	PNDIS_MINIPORT_BLOCK	Miniport,
	IN	NDIS_WORK_ITEM_TYPE		WorkItemType,
	OUT PVOID	*				WorkItemContext1,
	OUT	PVOID	*				WorkItemContext2
	);

//
// Structure used by the logging apis
//
typedef struct _NDIS_LOG
{
	PNDIS_MINIPORT_BLOCK	Miniport;	// The owning miniport block
	KSPIN_LOCK				LogLock;	// For serialization
#if defined(NDIS_WRAPPER)
	PIRP					Irp;		// Pending Irp to consume this log
#endif
	UINT					TotalSize;	// Size of the log buffer
	UINT					CurrentSize;// Size of the log buffer
	UINT					InPtr;		// IN part of the circular buffer
	UINT					OutPtr;		// OUT part of the circular buffer
	UCHAR					LogBuf[1];	// The circular buffer
} NDIS_LOG, *PNDIS_LOG;

typedef	struct	_NDIS_AF_LIST	NDIS_AF_LIST, *PNDIS_AF_LIST;

//
// one of these per mini-port registered on a Driver
//
struct _NDIS_MINIPORT_BLOCK
{
	ULONG						NullValue;		// used to distinquish between MACs and mini-ports
	PDEVICE_OBJECT				DeviceObject;	// created by the wrapper
	PNDIS_M_DRIVER_BLOCK		DriverHandle;	// pointer to our Driver block
	NDIS_HANDLE					MiniportAdapterContext; // context when calling mini-port functions
	NDIS_STRING					MiniportName;	// how mini-port refers to us
	PNDIS_M_OPEN_BLOCK			OpenQueue;		// queue of opens for this mini-port
	PNDIS_MINIPORT_BLOCK		NextMiniport;	// used by driver's MiniportQueue
	REFERENCE					Ref;			// contains spinlock for OpenQueue

	BOOLEAN						padding1;		// normal ints:	DO NOT REMOVE OR NDIS WILL BREAK!!!
	BOOLEAN						padding2;		// processing def: DO NOT REMOVE OR NDIS WILL BREAK!!!


	//
	// Synchronization stuff.
	//
	// The boolean is used to lock out several DPCs from running at the
	// same time. The difficultly is if DPC A releases the spin lock
	// and DPC B tries to run, we want to defer B until after A has
	// exited.
	//
	BOOLEAN						LockAcquired;	// EXPOSED via macros. Do not move

	UCHAR						PmodeOpens;		// Count of opens which turned on pmode/all_local

	NDIS_SPIN_LOCK				Lock;

	PNDIS_MINIPORT_INTERRUPT	Interrupt;

	ULONG						Flags;			// Flags to keep track of the
												// miniport's state.
	//
	//Work that the miniport needs to do.
	//
	KSPIN_LOCK					WorkLock;
	SINGLE_LIST_ENTRY			WorkQueue[NUMBER_OF_WORK_ITEM_TYPES];
	SINGLE_LIST_ENTRY			WorkItemFreeQueue;

	//
	// Stuff that got deferred.
	//
	KDPC						Dpc;
	NDIS_TIMER					WakeUpDpcTimer;

	//
	// Holds media specific information.
	//
	PETH_FILTER					EthDB;		// EXPOSED via macros. Do not move
	PTR_FILTER					TrDB;		// EXPOSED via macros. Do not move
	PFDDI_FILTER				FddiDB;		// EXPOSED via macros. Do not move
	PARC_FILTER					ArcDB;		// EXPOSED via macros. Do not move

	FILTER_PACKET_INDICATION_HANDLER PacketIndicateHandler;
	NDIS_M_SEND_COMPLETE_HANDLER	SendCompleteHandler;
	NDIS_M_SEND_RESOURCES_HANDLER	SendResourcesHandler;
	NDIS_M_RESET_COMPLETE_HANDLER	ResetCompleteHandler;

	PVOID						WrapperContext;
	NDIS_MEDIUM					MediaType;

	//
	// contains mini-port information
	//
	ULONG						BusNumber;
	NDIS_INTERFACE_TYPE			BusType;
	NDIS_INTERFACE_TYPE			AdapterType;

	//
	// Holds the map registers for this mini-port.
	//
	ULONG						PhysicalMapRegistersNeeded;
	ULONG						MaximumPhysicalMapping;
	PMAP_REGISTER_ENTRY			MapRegisters;	// EXPOSED via macros. Do not move

	//
	//	WorkItem routines that can change depending on whether we
	//	are fullduplex or not.
	//
	NDIS_M_PROCESS_DEFERRED 	ProcessDeferredHandler;
	NDIS_M_QUEUE_WORK_ITEM		QueueWorkItemHandler;
	NDIS_M_QUEUE_NEW_WORK_ITEM	QueueNewWorkItemHandler;
	NDIS_M_DEQUEUE_WORK_ITEM	DeQueueWorkItemHandler;

	PNDIS_TIMER					DeferredTimer;

	//
	// Resource information
	//
	PCM_RESOURCE_LIST			Resources;
			
	//
	//	This pointer is reserved. Used for debugging
	//
	PVOID						Reserved;

	PADAPTER_OBJECT				SystemAdapterObject;

	SINGLE_LIST_ENTRY			SingleWorkItems[NUMBER_OF_SINGLE_WORK_ITEMS];

	//
	//	For efficiency
	//
	W_HANDLE_INTERRUPT_HANDLER	HandleInterruptHandler;
	W_DISABLE_INTERRUPT_HANDLER	DisableInterruptHandler;
	W_ENABLE_INTERRUPT_HANDLER	EnableInterruptHandler;
	W_SEND_PACKETS_HANDLER		SendPacketsHandler;
	NDIS_M_START_SENDS			DeferredSendHandler;

	/**************** Stuff above is potentially accessed by macros. Add stuff below **********/

	ULONG						InterruptLevel;
	ULONG						InterruptVector;
	NDIS_INTERRUPT_MODE			InterruptMode;

	UCHAR						TrResetRing;
	UCHAR						ArcnetAddress;

	//
	//	This is the processor number that the miniport's
	//	interrupt DPC and timers are running on.
	//
	UCHAR						AssignedProcessor;

	NDIS_HANDLE					ArcnetBufferPool;
	PARC_BUFFER_LIST			ArcnetFreeBufferList;
	PARC_BUFFER_LIST			ArcnetUsedBufferList;
	PUCHAR						ArcnetLookaheadBuffer;
	UINT						CheckForHangTimeout;

	//
	// These two are used temporarily while allocating the map registers.
	//
	KEVENT						AllocationEvent;
	UINT						CurrentMapRegister;

	//
	// Send information
	//
	NDIS_SPIN_LOCK				SendLock;
	ULONG						SendFlags;			// Flags for send path.
	PNDIS_PACKET				FirstPacket;		// This pointer serves two purposes;
													// it is the head of the queue of ALL
													// packets that have been sent to
													// the miniport, it is also the head
													// of the packets that have been sent
													// down to the miniport by the wrapper.
	PNDIS_PACKET				LastPacket;			// This is tail pointer for the global
													// packet queue and this is the tail
													// pointer to the queue of packets
													// waiting to be sent to the miniport.
	PNDIS_PACKET				FirstPendingPacket; // This is head of the queue of packets
													// waiting to be sent to miniport.
	PNDIS_PACKET				LastMiniportPacket; // This is the tail pointer of the
													// queue of packets that have been
													// sent to the miniport by the wrapper.

	PNDIS_PACKET				LoopbackHead;		// Head of loopback queue.
	PNDIS_PACKET				LoopbackTail;		// Tail of loopback queue.

	ULONG						SendResourcesAvailable;
	PPNDIS_PACKET				PacketArray;
	UINT						MaximumSendPackets;

	//
	// Transfer data information
	//
	PNDIS_PACKET				FirstTDPacket;
	PNDIS_PACKET				LastTDPacket;
	PNDIS_PACKET				LoopbackPacket;

	//
	// Reset information
	//
	NDIS_STATUS					ResetStatus;

	//
	// RequestInformation
	//
	PNDIS_REQUEST				PendingRequest;
	PNDIS_REQUEST				MiniportRequest;
	NDIS_STATUS					RequestStatus;
	UINT						MaximumLongAddresses;
	UINT						MaximumShortAddresses;
	UINT						CurrentLookahead;
	UINT						MaximumLookahead;
	UINT						MacOptions;

	KEVENT						RequestEvent;

	UCHAR						MulticastBuffer[NDIS_M_MAX_MULTI_LIST][6];

	//
	// Temp stuff for using the old NDIS functions
	//
	ULONG						ChannelNumber;

	UINT						NumberOfAllocatedWorkItems;

	PNDIS_LOG					Log;

#if defined(NDIS_WRAPPER)
	//
	// List of registered address families. Valid for the call-manager, Null for the client
	//
	PNDIS_AF_LIST				CallMgrAfList;

    //
    // Store information here to track adapters
    //
    ULONG                       BusId;
    ULONG                       SlotNumber;

	//
	//	Pointer to the current thread. Used in layered miniport code.
	//
	LONG						MiniportThread;
	LONG						SendThread;

	//
	// Needed for PnP. Upcased version. The buffer is allocated as part of the
	// NDIS_MINIPORT_BLOCK itself.
	//
	UNICODE_STRING				BaseName;
#endif
};

//
// Definitions for NDIS_MINIPORT_BLOCK GeneralFlags.
//
#define fMINIPORT_NORMAL_INTERRUPTS			0x00000001
#define fMINIPORT_IN_INITIALIZE				0x00000002
#define fMINIPORT_ARCNET_BROADCAST_SET		0x00000004
#define fMINIPORT_BUS_MASTER				0x00000008
#define fMINIPORT_DMA_32_BIT_ADDRESSES		0x00000010
#define fMINIPORT_BEING_REMOVED				0x00000020
#define fMINIPORT_HALTING					0x00000040
#define fMINIPORT_CLOSING					0x00000080
#define fMINIPORT_UNLOADING					0x00000100
#define fMINIPORT_REQUEST_TIMEOUT			0x00000200
#define fMINIPORT_INDICATES_PACKETS			0x00000400
#define fMINIPORT_IS_NDIS_4_0				0x00000800
#define fMINIPORT_PACKET_ARRAY_VALID		0x00001000
#define fMINIPORT_FULL_DUPLEX				0x00002000
#define fMINIPORT_IGNORE_PACKET_QUEUE		0x00004000
#define fMINIPORT_IGNORE_REQUEST_QUEUE		0x00008000
#define fMINIPORT_IGNORE_TOKEN_RING_ERRORS	0x00010000
#define fMINIPORT_IS_IN_ALL_LOCAL			0x00020000
#define fMINIPORT_INTERMEDIATE_DRIVER		0x00040000
#define fMINIPORT_IS_NDIS_4_1				0x00080000

#define MINIPORT_LOCK_ACQUIRED(_Miniport)	((_Miniport)->LockAcquired)

//
//	Send flags
//
#define fMINIPORT_SEND_COMPLETE_CALLED		0x00000001
#define fMINIPORT_SEND_PACKET_ARRAY			0x00000002
#define fMINIPORT_SEND_HALTING				0x00000004
#define fMINIPORT_SEND_LOOPBACK_DIRECTED	0x00000008


//
//	Flags used in both.
//
#define fMINIPORT_RESET_IN_PROGRESS			0x80000000
#define fMINIPORT_RESET_REQUESTED			0x40000000
#define fMINIPORT_PROCESS_DEFERRED			0x20000000


//
// one of these per open on an mini-port/protocol
//

struct _NDIS_M_OPEN_BLOCK
{
	PNDIS_M_DRIVER_BLOCK		DriverHandle;			// pointer to our driver
	PNDIS_MINIPORT_BLOCK		MiniportHandle;			// pointer to our mini-port
	PNDIS_PROTOCOL_BLOCK		ProtocolHandle;			// pointer to our protocol
	PNDIS_OPEN_BLOCK			FakeOpen;				// Pointer to fake open block
	NDIS_HANDLE					ProtocolBindingContext;	// context when calling ProtXX funcs
	NDIS_HANDLE					MiniportAdapterContext;	// context when calling MiniportXX funcs
	PNDIS_M_OPEN_BLOCK			MiniportNextOpen;		// used by mini-port's OpenQueue
	PFILE_OBJECT				FileObject;				// created by operating system
	ULONG						Flags;
	NDIS_HANDLE					CloseRequestHandle;		// 0 indicates an internal close
	NDIS_HANDLE					FilterHandle;
	NDIS_SPIN_LOCK				SpinLock;				// guards Closing
	ULONG						References;
	UINT						CurrentLookahead;
	ULONG						ProtocolOptions;

	//
	// These are optimizations for getting to driver routines. They are not
	// necessary, but are here to save a dereference through the Driver block.
	//
	W_SEND_HANDLER				SendHandler;
	W_TRANSFER_DATA_HANDLER		TransferDataHandler;

	//
	//	NDIS 4.0 miniport entry-points
	//
	W_SEND_PACKETS_HANDLER		SendPacketsHandler;

	//
	//	NDIS 4.1 miniport entry-points
	//
	W_CO_CREATE_VC_HANDLER		MiniportCoCreateVcHandler;
	W_CO_REQUEST_HANDLER		MiniportCoRequestHandler;

	//
	// These are optimizations for getting to PROTOCOL routines. They are not
	// necessary, but are here to save a dereference through the PROTOCOL block.
	//

	SEND_COMPLETE_HANDLER		SendCompleteHandler;
	TRANSFER_DATA_COMPLETE_HANDLER	TransferDataCompleteHandler;
	RECEIVE_HANDLER				ReceiveHandler;
	RECEIVE_COMPLETE_HANDLER	ReceiveCompleteHandler;
	//
	// NDIS 4.0 protocol completion routines
	//
	RECEIVE_PACKET_HANDLER	ReceivePacketHandler;

	//
	// NDIS 4.1 protocol completion routines
	//
	CO_REQUEST_COMPLETE_HANDLER	CoRequestCompleteHandler;
	CO_CREATE_VC_HANDLER		CoCreateVcHandler;
	CO_DELETE_VC_HANDLER		CoDeleteVcHandler;
	PVOID						CmActivateVcCompleteHandler;
	PVOID						CmDeactivateVcCompleteHandler;

	BOOLEAN						ReceivedAPacket;
	BOOLEAN						IndicatingNow;

	//
	// this is the list of the call manager opens done on this adapter
	//
	struct _NDIS_CO_AF_BLOCK *	NextAf;

	//
	// This is a bit mask of what the call manager modules this open supports.
	// Usually the field is null meaning no call manager
	//
	ULONG						AddressFamilyMask;

	//
	// lists for queuing connections. There is both a queue for currently
	// active connections and a queue for connections that are not active.
	//
	LIST_ENTRY					ActiveVcHead;
	LIST_ENTRY					InactiveVcHead;
};

//
// Flags definition for NDIS_M_OPEN_BLOCK.
//
#define fMINIPORT_OPEN_CLOSING					0x00000001
#define fMINIPORT_OPEN_USING_ETH_ENCAPSULATION	0x00000002
#define fMINIPORT_OPEN_CALL_MGR					0x00000004
#define fMINIPORT_OPEN_PMODE					0x00000008

//
// NOTE: THIS STRUCTURE MUST, MUST, MUST ALIGN WITH THE
// NDIS_USER_OPEN_CONTEXT STRUCTURE defined in the wrapper.
//
typedef struct _NDIS_M_USER_OPEN_CONTEXT
{
	PDEVICE_OBJECT				DeviceObject;
	PNDIS_MINIPORT_BLOCK		MiniportBlock;
	ULONG						OidCount;
	PNDIS_OID					OidArray;
} NDIS_M_USER_OPEN_CONTEXT, *PNDIS_M_USER_OPEN_CONTEXT;


typedef struct _NDIS_REQUEST_RESERVED
{
	PNDIS_REQUEST				Next;
	struct _NDIS_M_OPEN_BLOCK *	Open;
} NDIS_REQUEST_RESERVED, *PNDIS_REQUEST_RESERVED;

#define PNDIS_RESERVED_FROM_PNDIS_REQUEST(_request)	((PNDIS_REQUEST_RESERVED)((_request)->MacReserved))


//
//	Routines for intermediate miniport drivers.
//
typedef
VOID
(*W_MINIPORT_CALLBACK)(
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	PVOID					CallbackContext
	);

EXPORT
NDIS_STATUS
NdisIMQueueMiniportCallback(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	W_MINIPORT_CALLBACK		CallbackRoutine,
	IN	PVOID					CallbackContext
	);

EXPORT
BOOLEAN
NdisIMSwitchToMiniport(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	OUT	PNDIS_HANDLE			SwitchHandle
	);

EXPORT
VOID
NdisIMRevertBack(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	NDIS_HANDLE				SwitchHandle
	);

EXPORT
VOID
NdisMSetResetTimeout(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	UINT					TimeInSeconds
	);

//
// Operating System Requests
//

EXPORT
NDIS_STATUS
NdisMAllocateMapRegisters(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	UINT					DmaChannel,
	IN	BOOLEAN					Dma32BitAddresses,
	IN	ULONG					PhysicalMapRegistersNeeded,
	IN	ULONG					MaximumPhysicalMapping
	);

EXPORT
VOID
NdisMFreeMapRegisters(
	IN	NDIS_HANDLE				MiniportAdapterHandle
	);

EXPORT
NDIS_STATUS
NdisMRegisterIoPortRange(
	OUT PVOID *					PortOffset,
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	UINT					InitialPort,
	IN	UINT					NumberOfPorts
	);

EXPORT
VOID
NdisMDeregisterIoPortRange(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	UINT					InitialPort,
	IN	UINT					NumberOfPorts,
	IN	PVOID					PortOffset
	);

EXPORT
NDIS_STATUS
NdisMMapIoSpace(
	OUT PVOID *					VirtualAddress,
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	NDIS_PHYSICAL_ADDRESS	PhysicalAddress,
	IN	UINT					Length
	);

EXPORT
VOID
NdisMUnmapIoSpace(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	PVOID					VirtualAddress,
	IN	UINT					Length
	);

EXPORT
NDIS_STATUS
NdisMRegisterInterrupt(
	OUT	PNDIS_MINIPORT_INTERRUPT Interrupt,
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	UINT					InterruptVector,
	IN	UINT					InterruptLevel,
	IN	BOOLEAN					RequestIsr,
	IN	BOOLEAN					SharedInterrupt,
	IN	NDIS_INTERRUPT_MODE		InterruptMode
	);

EXPORT
VOID
NdisMDeregisterInterrupt(
	IN	PNDIS_MINIPORT_INTERRUPT Interrupt
	);

EXPORT
BOOLEAN
NdisMSynchronizeWithInterrupt(
	IN	PNDIS_MINIPORT_INTERRUPT Interrupt,
	IN	PVOID					SynchronizeFunction,
	IN	PVOID					SynchronizeContext
	);


EXPORT
VOID
NdisMQueryAdapterResources(
	OUT PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				WrapperConfigurationContext,
	OUT PNDIS_RESOURCE_LIST		ResourceList,
	IN	OUT PUINT				BufferSize
	);

//
// Timers
//

/*++
VOID
NdisMSetTimer(
	IN	PNDIS_MINIPORT_TIMER Timer,
	IN	UINT MillisecondsToDelay
	);
--*/
#define NdisMSetTimer(_Timer, _Delay) NdisSetTimer((PNDIS_TIMER)(_Timer), _Delay)

VOID
NdisMSetPeriodicTimer(
	IN	PNDIS_MINIPORT_TIMER	 Timer,
	IN	UINT					 MillisecondPeriod
	);

EXPORT
VOID
NdisMInitializeTimer(
	IN	OUT PNDIS_MINIPORT_TIMER Timer,
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	PNDIS_TIMER_FUNCTION	TimerFunction,
	IN	PVOID					FunctionContext
	);

EXPORT
VOID
NdisMCancelTimer(
	IN	PNDIS_MINIPORT_TIMER	Timer,
	OUT PBOOLEAN				TimerCancelled
	);

EXPORT
VOID
NdisMSleep(
	IN	ULONG					MicrosecondsToSleep
	);

//
// Physical Mapping
//
#if defined(_X86_)

#define NdisMStartBufferPhysicalMappingMacro(_MiniportAdapterHandle,			\
											 _Buffer,							\
											 _PhysicalMapRegister,				\
											 _Write,							\
											 _PhysicalAddressArray,				\
											 _ArraySize)						\
{																				\
	PNDIS_MINIPORT_BLOCK	Miniport;											\
	PHYSICAL_ADDRESS		LogicalAddress;										\
	PUCHAR					VirtualAddress;										\
	ULONG					LengthRemaining;									\
	ULONG					LengthMapped;										\
	PULONG					pageframe;											\
	PMDL					Mdl;												\
	UINT					CurrentArrayLocation;								\
	ULONG					TransferLength;										\
	ULONG					Pageoffset;											\
																				\
	Mdl=(PMDL)_Buffer;															\
	Miniport = (PNDIS_MINIPORT_BLOCK)(_MiniportAdapterHandle);					\
	CurrentArrayLocation = 0;													\
																				\
	if ((Miniport->MapRegisters[_PhysicalMapRegister].MapRegister == NULL) &&	\
		(Mdl->MdlFlags & MDL_SOURCE_IS_NONPAGED_POOL)	&&						\
		(Mdl->MdlFlags & MDL_NETWORK_HEADER)			&&						\
		((Mdl->ByteOffset+Mdl->ByteCount) < PAGE_SIZE))							\
	{																			\
		LogicalAddress.QuadPart = (ULONGLONG)((*(PULONG)(Mdl+1) << PAGE_SHIFT)+	\
												Mdl->ByteOffset);				\
		(_PhysicalAddressArray)[CurrentArrayLocation].PhysicalAddress = LogicalAddress;\
		(_PhysicalAddressArray)[CurrentArrayLocation].Length = Mdl->ByteCount;	\
		++CurrentArrayLocation;													\
	}																			\
	else																		\
	{																			\
		VirtualAddress = MmGetMdlVirtualAddress(_Buffer);						\
		LengthRemaining = MmGetMdlByteCount(_Buffer);							\
		while (LengthRemaining > 0)												\
		{																		\
			LengthMapped = LengthRemaining;										\
			if (Miniport->MapRegisters[_PhysicalMapRegister].MapRegister == NULL)\
			{																	\
			 	Pageoffset = BYTE_OFFSET(VirtualAddress);						\
			 	TransferLength = PAGE_SIZE - Pageoffset;						\
			 	pageframe = (PULONG)(Mdl+1);									\
			 	pageframe += ((ULONG)VirtualAddress -							\
							  (ULONG)Mdl->StartVa) >> PAGE_SHIFT;				\
			 	LogicalAddress.QuadPart =										\
							(ULONGLONG)((*pageframe << PAGE_SHIFT)+Pageoffset);	\
			 	while (TransferLength < LengthMapped)							\
				{																\
					if (*pageframe+1 != *(pageframe+1))							\
						break;													\
					TransferLength += PAGE_SIZE;								\
					pageframe++;												\
				}																\
				if (TransferLength < LengthMapped)								\
				{																\
					LengthMapped = TransferLength;								\
				}																\
			}																	\
			else																\
			{																	\
				LogicalAddress = IoMapTransfer(NULL,							\
											   (_Buffer),						\
											   Miniport->MapRegisters[_PhysicalMapRegister].MapRegister,\
											   VirtualAddress,&LengthMapped,	\
											   (_Write));						\
			}																	\
			(_PhysicalAddressArray)[CurrentArrayLocation].PhysicalAddress =		\
							 LogicalAddress;									\
			(_PhysicalAddressArray)[CurrentArrayLocation].Length = LengthMapped;\
			LengthRemaining -= LengthMapped;									\
			VirtualAddress += LengthMapped;										\
			++CurrentArrayLocation;												\
		}																		\
	 }																			\
	Miniport->MapRegisters[_PhysicalMapRegister].WriteToDevice = (_Write);		\
	*(_ArraySize) = CurrentArrayLocation;										\
}

#else

#define NdisMStartBufferPhysicalMappingMacro(									\
				_MiniportAdapterHandle,											\
				_Buffer,														\
				_PhysicalMapRegister,											\
				_Write,															\
				_PhysicalAddressArray,											\
				_ArraySize														\
				)																\
{																				\
	PNDIS_MINIPORT_BLOCK _Miniport = (PNDIS_MINIPORT_BLOCK)(_MiniportAdapterHandle);\
	PHYSICAL_ADDRESS _LogicalAddress;											\
	PUCHAR _VirtualAddress;														\
	ULONG _LengthRemaining;														\
	ULONG _LengthMapped;														\
	UINT _CurrentArrayLocation;													\
																				\
	_VirtualAddress = MmGetMdlVirtualAddress(_Buffer);							\
	_LengthRemaining = MmGetMdlByteCount(_Buffer);								\
	_CurrentArrayLocation = 0;													\
																				\
	while (_LengthRemaining > 0)												\
	{																			\
		_LengthMapped = _LengthRemaining;										\
		_LogicalAddress =														\
			IoMapTransfer(														\
				NULL,															\
				(_Buffer),														\
				_Miniport->MapRegisters[_PhysicalMapRegister].MapRegister,		\
				_VirtualAddress,												\
				&_LengthMapped,													\
				(_Write));														\
		(_PhysicalAddressArray)[_CurrentArrayLocation].PhysicalAddress = _LogicalAddress;\
		(_PhysicalAddressArray)[_CurrentArrayLocation].Length = _LengthMapped;	\
		_LengthRemaining -= _LengthMapped;										\
		_VirtualAddress += _LengthMapped;										\
		++_CurrentArrayLocation;												\
	}																			\
	_Miniport->MapRegisters[_PhysicalMapRegister].WriteToDevice = (_Write);		\
	*(_ArraySize) = _CurrentArrayLocation;										\
}

#endif

#if BINARY_COMPATIBLE

EXPORT
VOID
NdisMStartBufferPhysicalMapping(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	PNDIS_BUFFER			Buffer,
	IN	ULONG					PhysicalMapRegister,
	IN	BOOLEAN					WriteToDevice,
	OUT PNDIS_PHYSICAL_ADDRESS_UNIT PhysicalAddressArray,
	OUT PUINT					ArraySize
	);

#else

#define NdisMStartBufferPhysicalMapping(										\
				_MiniportAdapterHandle,											\
				_Buffer,														\
				_PhysicalMapRegister,											\
				_Write,															\
				_PhysicalAddressArray,											\
				_ArraySize														\
				)																\
		NdisMStartBufferPhysicalMappingMacro(									\
				(_MiniportAdapterHandle),										\
				(_Buffer),														\
				(_PhysicalMapRegister),											\
				(_Write),														\
				(_PhysicalAddressArray),										\
				(_ArraySize)													\
				)

#endif

#if defined(_X86_)

#define NdisMCompleteBufferPhysicalMappingMacro(_MiniportAdapterHandle,			\
												_Buffer,						\
												_PhysicalMapRegister)			\
{																				\
	PNDIS_MINIPORT_BLOCK _Miniport = (PNDIS_MINIPORT_BLOCK)(_MiniportAdapterHandle);\
																				\
	if (_Miniport->MapRegisters[_PhysicalMapRegister].MapRegister != NULL)		\
		IoFlushAdapterBuffers(NULL,												\
							  _Buffer,											\
							  _Miniport->MapRegisters[_PhysicalMapRegister].MapRegister,\
							  MmGetMdlVirtualAddress(_Buffer),					\
							  MmGetMdlByteCount(_Buffer),						\
							  _Miniport->MapRegisters[_PhysicalMapRegister].WriteToDevice);\
}

#else

#define NdisMCompleteBufferPhysicalMappingMacro(								\
			_MiniportAdapterHandle,												\
			_Buffer,															\
			_PhysicalMapRegister												\
			)																	\
{																				\
	PNDIS_MINIPORT_BLOCK _Miniport = (PNDIS_MINIPORT_BLOCK)(_MiniportAdapterHandle);\
																				\
	IoFlushAdapterBuffers(NULL,													\
						  _Buffer,												\
						  _Miniport->MapRegisters[_PhysicalMapRegister].MapRegister,\
						  MmGetMdlVirtualAddress(_Buffer),						\
						  MmGetMdlByteCount(_Buffer),							\
						  _Miniport->MapRegisters[_PhysicalMapRegister].WriteToDevice);\
}

#endif

#if BINARY_COMPATIBLE

EXPORT
VOID
NdisMCompleteBufferPhysicalMapping(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	PNDIS_BUFFER			Buffer,
	IN	ULONG					PhysicalMapRegister
	);

#else

#define NdisMCompleteBufferPhysicalMapping(										\
			_MiniportAdapterHandle,												\
			_Buffer,															\
			_PhysicalMapRegister												\
			)																	\
		NdisMCompleteBufferPhysicalMappingMacro(								\
			(_MiniportAdapterHandle),											\
			(_Buffer),															\
			(_PhysicalMapRegister)												\
			)

#endif

#if BINARY_COMPATIBLE

EXPORT
VOID
NdisGetCurrentProcessorCpuUsage(
	PULONG						pCpuUsage
	);

EXPORT
VOID
NdisGetCurrentSystemTime(
	PLARGE_INTEGER				pSystemTime
	);

#else

#define	NdisGetCurrentProcessorCpuUsage(_pCpuUsage)								\
	{																			\
		PKPRCB Prcb;															\
																				\
		Prcb = KeGetCurrentPrcb();												\
		*(_pCpuUsage) = 100 - (ULONG)((ULONGLONG)Prcb->IdleThread->KernelTime)/	\
								UInt32x32To64(Prcb->KernelTime + Prcb->UserTime,100);\
	}

#define	NdisGetCurrentSystemTime(_pSystemTime)									\
	{																			\
		KeQuerySystemTime(_pSystemTime);										\
	}

#endif

//
// Shared memory
//

EXPORT
VOID
NdisMAllocateSharedMemory(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	ULONG					Length,
	IN	BOOLEAN					Cached,
	OUT PVOID *					VirtualAddress,
	OUT PNDIS_PHYSICAL_ADDRESS	PhysicalAddress
	);

EXPORT
NDIS_STATUS
NdisMAllocateSharedMemoryAsync(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	ULONG					Length,
	IN	BOOLEAN					Cached,
	IN	PVOID					Context
	);

/*++
VOID
NdisMUpdateSharedMemory(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	ULONG					Length,
	IN	PVOID					VirtualAddress,
	IN	NDIS_PHYSICAL_ADDRESS	PhysicalAddress
	)
--*/
#define NdisMUpdateSharedMemory(_H, _L, _V, _P) NdisUpdateSharedMemory(_H, _L, _V, _P)


EXPORT
VOID
NdisMFreeSharedMemory(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	ULONG					Length,
	IN	BOOLEAN					Cached,
	IN	PVOID					VirtualAddress,
	IN	NDIS_PHYSICAL_ADDRESS	PhysicalAddress
	);


//
// DMA operations.
//

EXPORT
NDIS_STATUS
NdisMRegisterDmaChannel(
	OUT PNDIS_HANDLE			MiniportDmaHandle,
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	UINT					DmaChannel,
	IN	BOOLEAN					Dma32BitAddresses,
	IN	PNDIS_DMA_DESCRIPTION	DmaDescription,
	IN	ULONG					MaximumLength
	);


EXPORT
VOID
NdisMDeregisterDmaChannel(
	IN	NDIS_HANDLE				MiniportDmaHandle
	);

/*++
VOID
NdisMSetupDmaTransfer(
	OUT PNDIS_STATUS			Status,
	IN	PNDIS_HANDLE			MiniportDmaHandle,
	IN	PNDIS_BUFFER			Buffer,
	IN	ULONG					Offset,
	IN	ULONG					Length,
	IN	BOOLEAN					WriteToDevice
	)
--*/
#define NdisMSetupDmaTransfer(_S, _H, _B, _O, _L, _M_) \
		NdisSetupDmaTransfer(_S, _H, _B, _O, _L, _M_)

/*++
VOID
NdisMCompleteDmaTransfer(
	OUT PNDIS_STATUS			Status,
	IN	PNDIS_HANDLE			MiniportDmaHandle,
	IN	PNDIS_BUFFER			Buffer,
	IN	ULONG					Offset,
	IN	ULONG					Length,
	IN	BOOLEAN					WriteToDevice
	)
--*/
#define NdisMCompleteDmaTransfer(_S, _H, _B, _O, _L, _M_) \
		NdisCompleteDmaTransfer(_S, _H, _B, _O, _L, _M_)

EXPORT
ULONG
NdisMReadDmaCounter(
	IN	NDIS_HANDLE				MiniportDmaHandle
	);


//
// Requests Used by Miniport Drivers
//


#define NdisMInitializeWrapper(_a,_b,_c,_d) NdisInitializeWrapper((_a),(_b),(_c),(_d))

EXPORT
NDIS_STATUS
NdisMRegisterMiniport(
	IN	NDIS_HANDLE				NdisWrapperHandle,
	IN	PNDIS_MINIPORT_CHARACTERISTICS MiniportCharacteristics,
	IN	UINT					CharacteristicsLength
	);

//
// For use by intermediate miniports only
//
EXPORT
NDIS_STATUS
NdisIMRegisterLayeredMiniport(
	IN	NDIS_HANDLE				NdisWrapperHandle,
	IN	PNDIS_MINIPORT_CHARACTERISTICS MiniportCharacteristics,
	IN	UINT					CharacteristicsLength,
	OUT PNDIS_HANDLE			DriverHandle
	);

//
// For use by intermediate miniports only
//
EXPORT
NDIS_STATUS
NdisIMInitializeDeviceInstance(
	IN	NDIS_HANDLE				DriverHandle,
	IN	PNDIS_STRING			DriverInstance
	);

NDIS_STATUS
NdisIMDeInitializeDeviceInstance(
	IN	NDIS_HANDLE				NdisMiniportHandle
	);

EXPORT
VOID
NdisMSetAttributes(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	BOOLEAN					BusMaster,
	IN	NDIS_INTERFACE_TYPE		AdapterType
	);

EXPORT
VOID
NdisMSetAttributesEx(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	UINT					CheckForHangTimeInSeconds OPTIONAL,
	IN	ULONG					AttributeFlags,
	IN	NDIS_INTERFACE_TYPE		AdapterType	OPTIONAL
	);

#define	NDIS_ATTRIBUTE_IGNORE_PACKET_TIMEOUT		0x00000001
#define NDIS_ATTRIBUTE_IGNORE_REQUEST_TIMEOUT		0x00000002
#define NDIS_ATTRIBUTE_IGNORE_TOKEN_RING_ERRORS		0x00000004
#define NDIS_ATTRIBUTE_BUS_MASTER					0x00000008
#define NDIS_ATTRIBUTE_INTERMEDIATE_DRIVER			0x00000010


/*++
EXPORT
VOID
NdisMSendComplete(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	PNDIS_PACKET			Packet,
	IN	NDIS_STATUS				Status
	)
--*/
#ifdef	NDIS40_MINIPORT

#define	NdisMSendComplete(_M, _P, _S)	(*((PNDIS_MINIPORT_BLOCK)(_M))->SendCompleteHandler)(_M, _P, _S)

#else

EXPORT
VOID
NdisMSendComplete(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	PNDIS_PACKET			Packet,
	IN	NDIS_STATUS				Status
	);

#endif

/*++
EXPORT
VOID
NdisMSendResourcesAvailable(
	IN	NDIS_HANDLE				MiniportAdapterHandle
	)
--*/
#ifdef	NDIS40_MINIPORT

#define	NdisMSendResourcesAvailable(_M)	(*((PNDIS_MINIPORT_BLOCK)(_M))->SendResourcesHandler)(_M)
#else

EXPORT
VOID
NdisMSendResourcesAvailable(
	IN	NDIS_HANDLE				MiniportAdapterHandle
	);

#endif


/*++
EXPORT
VOID
NdisMResetComplete(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	NDIS_STATUS				Status,
	IN	BOOLEAN					AddressingReset
	)
--*/
#ifdef	NDIS40_MINIPORT

#define	NdisMResetComplete(_M, _S, _A)	(*((PNDIS_MINIPORT_BLOCK)(_M))->ResetCompleteHandler)(_M, _S, _A)
#else

EXPORT
VOID
NdisMResetComplete(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	NDIS_STATUS				Status,
	IN	BOOLEAN					AddressingReset
	);

#endif
EXPORT
VOID
NdisMTransferDataComplete(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	PNDIS_PACKET			Packet,
	IN	NDIS_STATUS				Status,
	IN	UINT					BytesTransferred
	);

EXPORT
VOID
NdisMSetInformationComplete(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	NDIS_STATUS				Status
	);

EXPORT
VOID
NdisMQueryInformationComplete(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	NDIS_STATUS				Status
	);

/*++

VOID
NdisMIndicateReceivePacket(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	PPNDIS_PACKET			ReceivedPackets,
	IN	UINT					NumberOfPackets
	)

--*/
#define NdisMIndicateReceivePacket(_H, _P, _N)									\
{																				\
	ASSERT(MINIPORT_LOCK_ACQUIRED((PNDIS_MINIPORT_BLOCK)(_H)));					\
	(*((PNDIS_MINIPORT_BLOCK)(_H))->PacketIndicateHandler)(						\
						_H,														\
						_P,														\
						_N);													\
}

/*++

VOID
NdisMEthIndicateReceive(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	NDIS_HANDLE				MiniportReceiveContext,
	IN	PVOID					HeaderBuffer,
	IN	UINT					HeaderBufferSize,
	IN	PVOID					LookaheadBuffer,
	IN	UINT					LookaheadBufferSize,
	IN	UINT					PacketSize
	)

--*/
#define NdisMEthIndicateReceive( _H, _C, _B, _SZ, _L, _LSZ, _PSZ)				\
{																				\
	ASSERT(MINIPORT_LOCK_ACQUIRED((PNDIS_MINIPORT_BLOCK)(_H)));					\
	EthFilterDprIndicateReceive(												\
		((PNDIS_MINIPORT_BLOCK)(_H))->EthDB,									\
		_C,																		\
		_B,																		\
		_B,																		\
		_SZ,																	\
		_L,																		\
		_LSZ,																	\
		_PSZ																	\
		);																		\
}

/*++

VOID
NdisMTrIndicateReceive(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	NDIS_HANDLE				MiniportReceiveContext,
	IN	PVOID					HeaderBuffer,
	IN	UINT					HeaderBufferSize,
	IN	PVOID					LookaheadBuffer,
	IN	UINT					LookaheadBufferSize,
	IN	UINT					PacketSize
	)

--*/
#define NdisMTrIndicateReceive( _H, _C, _B, _SZ, _L, _LSZ, _PSZ)				\
{																				\
	ASSERT(MINIPORT_LOCK_ACQUIRED((PNDIS_MINIPORT_BLOCK)(_H)));					\
	TrFilterDprIndicateReceive(													\
		((PNDIS_MINIPORT_BLOCK)(_H))->TrDB,										\
		_C,																		\
		_B,																		\
		_SZ,																	\
		_L,																		\
		_LSZ,																	\
		_PSZ																	\
		);																		\
}

/*++

VOID
NdisMFddiIndicateReceive(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	NDIS_HANDLE				MiniportReceiveContext,
	IN	PVOID					HeaderBuffer,
	IN	UINT					HeaderBufferSize,
	IN	PVOID					LookaheadBuffer,
	IN	UINT					LookaheadBufferSize,
	IN	UINT					PacketSize
	)

--*/

#define NdisMFddiIndicateReceive( _H, _C, _B, _SZ, _L, _LSZ, _PSZ)				\
{																				\
	ASSERT(MINIPORT_LOCK_ACQUIRED((PNDIS_MINIPORT_BLOCK)(_H)));					\
																				\
	FddiFilterDprIndicateReceive(												\
			((PNDIS_MINIPORT_BLOCK)(_H))->FddiDB,								\
			_C,																	\
			(PUCHAR)_B + 1,														\
			((((PUCHAR)_B)[0] & 0x40) ? FDDI_LENGTH_OF_LONG_ADDRESS 			\
							: FDDI_LENGTH_OF_SHORT_ADDRESS),					\
			_B,																	\
			_SZ,																\
			_L,																	\
			_LSZ,																\
			_PSZ																\
	);																			\
}

/*++

VOID
NdisMArcIndicateReceive(
	IN	NDIS_HANDLE				MiniportHandle,
	IN	PUCHAR					pRawHeader,		// Pointer to Arcnet frame header
	IN	PUCHAR					pData,			// Pointer to data portion of Arcnet frame
	IN	UINT					Length			// Data Length
	)

--*/
#define NdisMArcIndicateReceive( _H, _HD, _D, _SZ)								\
{																				\
	ASSERT(MINIPORT_LOCK_ACQUIRED((PNDIS_MINIPORT_BLOCK)(_H)));					\
	ArcFilterDprIndicateReceive(												\
		((PNDIS_MINIPORT_BLOCK)(_H))->ArcDB,									\
		_HD,																	\
		_D,																		\
		_SZ																		\
		);																		\
}


/*++

VOID
NdisMEthIndicateReceiveComplete(
	IN	NDIS_HANDLE				MiniportHandle
	);

--*/

#define NdisMEthIndicateReceiveComplete( _H )									\
{																				\
	ASSERT(MINIPORT_LOCK_ACQUIRED(((PNDIS_MINIPORT_BLOCK)_H)));					\
	EthFilterDprIndicateReceiveComplete(((PNDIS_MINIPORT_BLOCK)_H)->EthDB);		\
}

/*++

VOID
NdisMTrIndicateReceiveComplete(
	IN	NDIS_HANDLE				MiniportHandle
	);

--*/

#define NdisMTrIndicateReceiveComplete( _H )									\
{																				\
	ASSERT(MINIPORT_LOCK_ACQUIRED(((PNDIS_MINIPORT_BLOCK)_H)));					\
	TrFilterDprIndicateReceiveComplete(((PNDIS_MINIPORT_BLOCK)_H)->TrDB);		\
}

/*++

VOID
NdisMFddiIndicateReceiveComplete(
	IN	NDIS_HANDLE				MiniportHandle
	);

--*/

#define NdisMFddiIndicateReceiveComplete( _H )									\
{																				\
	ASSERT(MINIPORT_LOCK_ACQUIRED(((PNDIS_MINIPORT_BLOCK)_H)));					\
	FddiFilterDprIndicateReceiveComplete(((PNDIS_MINIPORT_BLOCK)_H)->FddiDB);	\
}

/*++

VOID
NdisMArcIndicateReceiveComplete(
	IN	NDIS_HANDLE				MiniportHandle
	);

--*/

#define NdisMArcIndicateReceiveComplete( _H )									\
{																				\
	ASSERT(MINIPORT_LOCK_ACQUIRED(((PNDIS_MINIPORT_BLOCK)_H)));					\
																				\
	if (((PNDIS_MINIPORT_BLOCK)_H)->EthDB)										\
	{																			\
		EthFilterDprIndicateReceiveComplete(((PNDIS_MINIPORT_BLOCK)_H)->EthDB);	\
	}																			\
																				\
	ArcFilterDprIndicateReceiveComplete(((PNDIS_MINIPORT_BLOCK)_H)->ArcDB);		\
}

EXPORT
VOID
NdisMIndicateStatus(
	IN	NDIS_HANDLE				MiniportHandle,
	IN	NDIS_STATUS				GeneralStatus,
	IN	PVOID					StatusBuffer,
	IN	UINT					StatusBufferSize
	);

EXPORT
VOID
NdisMIndicateStatusComplete(
	IN	NDIS_HANDLE				MiniportHandle
	);

EXPORT
VOID
NdisMRegisterAdapterShutdownHandler(
	IN	NDIS_HANDLE				MiniportHandle,
	IN	PVOID					ShutdownContext,
	IN	ADAPTER_SHUTDOWN_HANDLER ShutdownHandler
	);

EXPORT
VOID
NdisMDeregisterAdapterShutdownHandler(
	IN	NDIS_HANDLE				MiniportHandle
	);

EXPORT
NDIS_STATUS
NdisMPciAssignResources(
	IN	NDIS_HANDLE				MiniportHandle,
	IN	ULONG					SlotNumber,
	IN	PNDIS_RESOURCE_LIST *	AssignedResources
	);

//
// Logging support for miniports
//

EXPORT
NDIS_STATUS
NdisMCreateLog(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	UINT					Size,
	OUT	PNDIS_HANDLE			LogHandle
	);

EXPORT
VOID
NdisMCloseLog(
	IN	NDIS_HANDLE				LogHandle
	);

EXPORT
NDIS_STATUS
NdisMWriteLogData(
	IN	NDIS_HANDLE				LogHandle,
	IN	PVOID					LogBuffer,
	IN	UINT					LogBufferSize
	);

EXPORT
VOID
NdisMFlushLog(
	IN	NDIS_HANDLE				LogHandle
	);

//
// NDIS 4.1 extensions for miniports
//

EXPORT
VOID
NdisMCoIndicatePacket(
	IN	NDIS_HANDLE				NdisVcHandle,
	IN	PPNDIS_PACKET			PacketArray,
	IN	UINT					NumberOfPackets
	);

EXPORT
VOID
NdisMCoIndicateStatus(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	NDIS_HANDLE				NdisVcHandle,
	IN	NDIS_STATUS				GeneralStatus,
	IN	PVOID					StatusBuffer,
	IN	ULONG					StatusBufferSize
	);

EXPORT
VOID
NdisMCoIndicatePacketComplete(
	IN	NDIS_HANDLE				MiniportAdapterHandle
	);

EXPORT
VOID
NdisMCoSendComplete(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				NdisVcHandle,
	IN	PNDIS_PACKET			Packet
	);

EXPORT
VOID
NdisMCoActivateVcComplete(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				NdisVcHandle,
	IN	PCO_MEDIA_PARAMETERS	MediaParameters
	);

EXPORT
VOID
NdisMCoDeactivateVcComplete(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				NdisVcHandle
	);

EXPORT
VOID
NdisMCoRequestComplete(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	PNDIS_REQUEST			Request
	);

EXPORT
NDIS_STATUS
NdisMCoRegisterAddressFamily(
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	PCO_ADDRESS_FAMILY		AddressFamily,
	IN	PNDIS_CALL_MANAGER_CHARACTERISTICS CmCharacteristics,
	IN	UINT					SizeOfCmCharacteristics
	);




#endif // defined(NDIS_MINIPORT_DRIVER) || defined(NDIS_WRAPPER)

#if defined(NDIS41) || defined(NDIS41_MINIPORT)
//
// WARNING: NDIS v4.1 is under construction. Do not use.
//

typedef struct _CO_CALL_PARAMETERS		CO_CALL_PARAMETERS, *PCO_CALL_PARAMETERS;
typedef struct _CO_MEDIA_PARAMETERS		CO_MEDIA_PARAMETERS, *PCO_MEDIA_PARAMETERS;

//
// CoNdis client only handler proto-types - used by clients of call managers
//
typedef
VOID
(*CL_OPEN_AF_COMPLETE_HANDLER)(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				ProtocolAfContext,
	IN	NDIS_HANDLE				NdisAfHandle
	);

typedef
VOID
(*CL_CLOSE_AF_COMPLETE_HANDLER)(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				ProtocolAfContext
	);

typedef
VOID
(*CL_REG_SAP_COMPLETE_HANDLER)(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				ProtocolSapContext,
	IN	PCO_SAP					Sap,
	IN	NDIS_HANDLE				NdisSapHandle
	);

typedef
VOID
(*CL_DEREG_SAP_COMPLETE_HANDLER)(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				ProtocolSapContext
	);

typedef
VOID
(*CL_MAKE_CALL_COMPLETE_HANDLER)(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				ProtocolVcContext,
	IN	NDIS_HANDLE				NdisPartyHandle		OPTIONAL,
	IN	PCO_CALL_PARAMETERS		CallParameters
	);

typedef
VOID
(*CL_CLOSE_CALL_COMPLETE_HANDLER)(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				ProtocolVcContext,
	IN	NDIS_HANDLE				ProtocolPartyContext OPTIONAL
	);

typedef
VOID
(*CL_ADD_PARTY_COMPLETE_HANDLER)(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				ProtocolPartyContext,
	IN	NDIS_HANDLE				NdisPartyHandle,
	IN	PCO_CALL_PARAMETERS		CallParameters
	);

typedef
VOID
(*CL_DROP_PARTY_COMPLETE_HANDLER)(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				ProtocolPartyContext
	);

typedef
NDIS_STATUS
(*CL_INCOMING_CALL_HANDLER)(
	IN	NDIS_HANDLE				ProtocolSapContext,
	IN	NDIS_HANDLE				ProtocolVcContext,
    IN OUT PCO_CALL_PARAMETERS	CallParameters
	);

typedef
VOID
(*CL_CALL_CONNECTED_HANDLER)(
	IN	NDIS_HANDLE				ProtocolVcContext
	);

typedef
VOID
(*CL_INCOMING_CLOSE_CALL_HANDLER)(
	IN	NDIS_STATUS				CloseStatus,
	IN	NDIS_HANDLE				ProtocolVcContext,
	IN	PVOID					CloseData	OPTIONAL,
	IN	UINT					Size		OPTIONAL
	);

typedef
VOID
(*CL_INCOMING_DROP_PARTY_HANDLER)(
	IN	NDIS_STATUS				DropStatus,
	IN	NDIS_HANDLE				ProtocolPartyContext,
	IN	PVOID					CloseData	OPTIONAL,
	IN	UINT					Size		OPTIONAL
	);

typedef
VOID
(*CL_MODIFY_CALL_QOS_COMPLETE_HANDLER)(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				ProtocolVcContext,
	IN	PCO_CALL_PARAMETERS		CallParameters
	);

typedef
VOID
(*CL_INCOMING_CALL_QOS_CHANGE_HANDLER)(
	IN	NDIS_HANDLE				ProtocolVcContext,
	IN	PCO_CALL_PARAMETERS		CallParameters
	);

typedef struct _NDIS_CLIENT_CHARACTERISTICS
{
	UCHAR							MajorVersion;
	UCHAR							MinorVersion;
	UINT							Reserved;

	CO_CREATE_VC_HANDLER			ClCreateVcHandler;
	CO_DELETE_VC_HANDLER			ClDeleteVcHandler;
	CL_OPEN_AF_COMPLETE_HANDLER		ClOpenAfCompleteHandler;
	CL_CLOSE_AF_COMPLETE_HANDLER	ClCloseAfCompleteHandler;
	CL_REG_SAP_COMPLETE_HANDLER		ClRegisterSapCompleteHandler;
	CL_DEREG_SAP_COMPLETE_HANDLER	ClDeregisterSapCompleteHandler;
	CL_MAKE_CALL_COMPLETE_HANDLER	ClMakeCallCompleteHandler;
	CL_MODIFY_CALL_QOS_COMPLETE_HANDLER	ClModifyCallQoSCompleteHandler;
	CL_CLOSE_CALL_COMPLETE_HANDLER	ClCloseCallCompleteHandler;
	CL_ADD_PARTY_COMPLETE_HANDLER	ClAddPartyCompleteHandler;
	CL_DROP_PARTY_COMPLETE_HANDLER	ClDropPartyCompleteHandler;
	CL_INCOMING_CALL_HANDLER		ClIncomingCallHandler;
	CL_INCOMING_CALL_QOS_CHANGE_HANDLER	ClIncomingCallQoSChangeHandler;
	CL_INCOMING_CLOSE_CALL_HANDLER	ClIncomingCloseCallHandler;
	CL_INCOMING_DROP_PARTY_HANDLER	ClIncomingDropPartyHandler;
    CL_CALL_CONNECTED_HANDLER		ClCallConnectedHandler;

} NDIS_CLIENT_CHARACTERISTICS, *PNDIS_CLIENT_CHARACTERISTICS;

//
// CoNdis call-manager only handler proto-types - used by call managers only
//
typedef
NDIS_STATUS
(*CM_OPEN_AF_HANDLER)(
	IN	NDIS_HANDLE				CallMgrBindingContext,
	IN	PCO_ADDRESS_FAMILY		AddressFamily,
	IN	NDIS_HANDLE				NdisAfHandle,
	OUT	PNDIS_HANDLE			CallMgrAfContext
	);

typedef
NDIS_STATUS
(*CM_CLOSE_AF_HANDLER)(
	IN	NDIS_HANDLE				CallMgrAfContext
	);

typedef
NDIS_STATUS
(*CM_REG_SAP_HANDLER)(
	IN	NDIS_HANDLE				CallMgrAfContext,
	IN	PCO_SAP					Sap,
	IN	NDIS_HANDLE				NdisSapHandle,
	OUT	PNDIS_HANDLE			CallMgrSapContext
	);

typedef
NDIS_STATUS
(*CM_DEREG_SAP_HANDLER)(
	IN	NDIS_HANDLE				CallMgrSapContext
	);

typedef
NDIS_STATUS
(*CM_MAKE_CALL_HANDLER)(
	IN	NDIS_HANDLE				CallMgrVcContext,
    IN OUT PCO_CALL_PARAMETERS	CallParameters,
	IN	NDIS_HANDLE				NdisPartyHandle		OPTIONAL,
	OUT	PNDIS_HANDLE			CallMgrPartyContext OPTIONAL
	);

typedef
NDIS_STATUS
(*CM_CLOSE_CALL_HANDLER)(
	IN	NDIS_HANDLE				CallMgrVcContext,
	IN	NDIS_HANDLE				CallMgrPartyContext	OPTIONAL,
	IN	PVOID					CloseData			OPTIONAL,
	IN	UINT					Size				OPTIONAL
	);

typedef
NDIS_STATUS
(*CM_MODIFY_CALL_QOS_HANDLER)(
	IN	NDIS_HANDLE				CallMgrVcContext,
    IN	PCO_CALL_PARAMETERS		CallParameters
	);

typedef
VOID
(*CM_INCOMING_CALL_COMPLETE_HANDLER)(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				CallMgrVcContext,
	IN	PCO_CALL_PARAMETERS		CallParameters
	);

typedef
VOID
(*CM_ACTIVATE_VC_COMPLETE_HANDLER)(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				CallMgrVcContext,
	IN	PCO_MEDIA_PARAMETERS	MediaParameters
	);

typedef
VOID
(*CM_DEACTIVATE_VC_COMPLETE_HANDLER)(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				CallMgrVcContext
	);

typedef
NDIS_STATUS
(*CM_ADD_PARTY_HANDLER)(
	IN	NDIS_HANDLE				CallMgrVcContext,
    IN OUT PCO_CALL_PARAMETERS	CallParameters,
	IN	NDIS_HANDLE				NdisPartyHandle,
	OUT	PNDIS_HANDLE			CallMgrPartyContext
	);

typedef
NDIS_STATUS
(*CM_DROP_PARTY_HANDLER)(
	IN	NDIS_HANDLE				CallMgrPartyContext,
	IN	PVOID					CloseData	OPTIONAL,
	IN	UINT					Size		OPTIONAL
	);

typedef struct _NDIS_CALL_MANAGER_CHARACTERISTICS
{
	UCHAR							MajorVersion;
	UCHAR							MinorVersion;
	UINT							Reserved;

	CO_CREATE_VC_HANDLER			CmCreateVcHandler;
	CO_DELETE_VC_HANDLER			CmDeleteVcHandler;
	CM_OPEN_AF_HANDLER				CmOpenAfHandler;
	CM_CLOSE_AF_HANDLER				CmCloseAfHandler;
	CM_REG_SAP_HANDLER				CmRegisterSapHandler;
	CM_DEREG_SAP_HANDLER			CmDeregisterSapHandler;
	CM_MAKE_CALL_HANDLER			CmMakeCallHandler;
	CM_CLOSE_CALL_HANDLER			CmCloseCallHandler;
    CM_INCOMING_CALL_COMPLETE_HANDLER CmIncomingCallCompleteHandler;
	CM_ADD_PARTY_HANDLER			CmAddPartyHandler;
	CM_DROP_PARTY_HANDLER			CmDropPartyHandler;
	CM_ACTIVATE_VC_COMPLETE_HANDLER	CmActivateVcCompleteHandler;
	CM_DEACTIVATE_VC_COMPLETE_HANDLER CmDeactivateVcCompleteHandler;
	CM_MODIFY_CALL_QOS_HANDLER		CmModifyCallQoSHandler;
	
} NDIS_CALL_MANAGER_CHARACTERISTICS, *PNDIS_CALL_MANAGER_CHARACTERISTICS;

#if defined(NDIS_WRAPPER)

typedef struct _NDIS_M_OPEN_BLOCK NDIS_M_OPEN_BLOCK,* PNDIS_M_OPEN_BLOCK;

//
// this structure represents an open address family
//

typedef struct _NDIS_CO_AF_BLOCK
{
	struct _NDIS_CO_AF_BLOCK *		NextAf;				// the next open of the call manager per adapter open
	ULONG							Flags;
	LONG							References;
	PNDIS_MINIPORT_BLOCK			Miniport;			// pointer to the miniport in question

	//
	// Cached call manager entry points
	//
	PNDIS_CALL_MANAGER_CHARACTERISTICS	CallMgrEntries;
	PNDIS_M_OPEN_BLOCK				CallMgrOpen;		// pointer to the call manager's open adapter
	NDIS_HANDLE						CallMgrContext;		// context when calling SigProtXX funcs

	//
	// Cached client entry points
	//
	NDIS_CLIENT_CHARACTERISTICS		ClientEntries;
	PNDIS_M_OPEN_BLOCK				ClientOpen;			// pointer to the client's open adapter
	NDIS_HANDLE						ClientContext;		// context when calling ProtXX funcs

	KSPIN_LOCK						Lock;
} NDIS_CO_AF_BLOCK, *PNDIS_CO_AF_BLOCK;

#define AF_CLOSING				0x80000000

//
// a Service Access Point (Sap) datastructure. The NdisSapHandle points to one of these.
//
typedef struct _NDIS_CO_SAP_BLOCK
{
	NDIS_HANDLE					CallMgrContext;
	NDIS_HANDLE					ClientContext;
	PNDIS_CO_AF_BLOCK			AfBlock;
	PCO_SAP						Sap;
	ULONG						Flags;
	LONG						References;
	KSPIN_LOCK					Lock;
} NDIS_CO_SAP_BLOCK, *PNDIS_CO_SAP_BLOCK;

//
// Definitions for Sap->Flags
//
#define SAP_CLOSING				0x80000000

//
// a connecton (Vc) datastructure. The NdisVcHandle points to one of these.
//
typedef struct _NDIS_CO_VC_BLOCK
{
	ULONG						References;
	ULONG						Flags;				// to track closes
	KSPIN_LOCK					Lock;

	PNDIS_CO_AF_BLOCK			AfBlock;			// OPTIONAL - NULL for call-mgr owned VCs

	//
	// References for client and call-manager
	//
	PNDIS_M_OPEN_BLOCK			ClientOpen;			// identifies the client
													// This could be the call-manager open
													// if the Vc is client-manager owned
	NDIS_HANDLE					ClientContext;		// pased up to the client on indications
	LIST_ENTRY					ClientLinkage;

	//
	// The non-creator's handler and context
	//
	CO_DELETE_VC_HANDLER		CoDeleteVcHandler;
	NDIS_HANDLE					DeleteVcContext;

	//
	// Clients cached entry points
	//
	CO_SEND_COMPLETE_HANDLER	CoSendCompleteHandler;
	CO_RECEIVE_PACKET_HANDLER	CoReceivePacketHandler;
	CL_MODIFY_CALL_QOS_COMPLETE_HANDLER	ClModifyCallQoSCompleteHandler;
	CL_INCOMING_CALL_QOS_CHANGE_HANDLER	ClIncomingCallQoSChangeHandler;
	CL_CALL_CONNECTED_HANDLER	ClCallConnectedHandler;

	PNDIS_M_OPEN_BLOCK			CallMgrOpen;		// identifies the call-manager
	NDIS_HANDLE					CallMgrContext;		// passed up to the call manager on indications
	LIST_ENTRY					CallMgrLinkage;

	//
	// Call-manager cached entry points
	//
	CM_ACTIVATE_VC_COMPLETE_HANDLER		CmActivateVcCompleteHandler;
	CM_DEACTIVATE_VC_COMPLETE_HANDLER	CmDeactivateVcCompleteHandler;
	CM_MODIFY_CALL_QOS_HANDLER	CmModifyCallQoSHandler;

	//
	// Miniport's context and some cached entry-points
	//
	PNDIS_MINIPORT_BLOCK		Miniport;			// pointer to the miniport in question
	NDIS_HANDLE					MiniportContext;	// passed down to the miniport
	W_CO_SEND_PACKETS_HANDLER	WCoSendPacketsHandler;
	W_CO_DELETE_VC_HANDLER		WCoDeleteVcHandler;
	W_CO_ACTIVATE_VC_HANDLER	WCoActivateVcHandler;
	W_CO_DEACTIVATE_VC_HANDLER	WCoDeactivateVcHandler;

	PVOID						pVcId;

} NDIS_CO_VC_BLOCK, *PNDIS_CO_VC_BLOCK;

#define VC_ACTIVE				0x00000001
#define VC_ACTIVATE_PENDING		0x00000002
#define VC_DEACTIVATE_PENDING	0x00000004
#define VC_CLOSING				0x80000000

//
// Structure to represent a handle generated when a multi-party call is generated.
// This handle can ONLY be used for NdisCoDropParty call.
//
typedef struct _NDIS_CO_PARTY_BLOCK
{
	PNDIS_CO_VC_BLOCK			Vc;
	NDIS_HANDLE					CallMgrContext;
	NDIS_HANDLE					ClientContext;

	//
	// Cached client Handlers
	//
	CL_INCOMING_DROP_PARTY_HANDLER	ClIncomingDropPartyHandler;
	CL_DROP_PARTY_COMPLETE_HANDLER	ClDropPartyCompleteHandler;
} NDIS_CO_PARTY_BLOCK, *PNDIS_CO_PARTY_BLOCK;

#endif

//
// this send flag is used on ATM net cards to set ( turn on ) the CLP bit
// (Cell Loss Priority) bit
//
#define CO_SEND_FLAG_SET_DISCARD_ELIBILITY	0x00000001

//
// the Address structure used on NDIS_CO_ADD_ADDRESS or NDIS_CO_DELETE_ADDRESS
//
typedef struct _CO_ADDRESS
{
	IN OUT ULONG				AddressSize;
	IN OUT UCHAR				Address[1];
} CO_ADDRESS, *PCO_ADDRESS;

//
// the list of addresses returned from the CallMgr on a NDIS_CO_GET_ADDRESSES
//
typedef struct _CO_ADDRESS_LIST
{
	OUT	ULONG					NumberOfAddressesAvailable;
	OUT	ULONG					NumberOfAddresses;
	OUT	CO_ADDRESS				AddressList;
} CO_ADDRESS_LIST, *PCO_ADDRESS_LIST;

//
// BUGBUG - these are temporary versions of these structures till we work out
// the correct ones.
//
typedef struct _SIG_VPVC
{
	UINT						Vp;
	UINT						Vc;
} CO_VPVC, *PCO_VPVC;

//
// Quality of Service definitions
//
typedef enum
{
    BestEffortService,
    PredictiveService,
    GuaranteedService
} GUARANTEE;

typedef struct _CO_FLOW_PARAMETERS
{
	ULONG						TokenRate;				// In Bytes/sec
	ULONG						TokenBucketSize;		// In Bytes
	ULONG						PeakBandwidth;			// In Bytes/sec
	ULONG						Latency;				// In microseconds
	ULONG						DelayVariation;			// In microseconds
	GUARANTEE					LevelOfGuarantee;		// Guaranteed, Predictive or Best Effort
	ULONG						CostOfCall;				// Reserved for future use,
														// must be set to 0 now
	ULONG						NetworkAvailability;	// read-only: 1 if accessible, 0 if not
	ULONG						MaxSduSize;				// In Bytes
} CO_FLOW_PARAMETERS, *PCO_FLOW_PARAMETERS;

typedef struct _CO_SPECIFIC_PARAMETERS
{
	ULONG						ParamType;
	ULONG						Length;
	UCHAR						Parameters[1];
} CO_SPECIFIC_PARAMETERS, *PCO_SPECIFIC_PARAMETERS;

typedef struct _CO_CALL_MANAGER_PARAMETERS
{
	CO_FLOW_PARAMETERS			Transmit;
	CO_FLOW_PARAMETERS			Receive;
	CO_SPECIFIC_PARAMETERS		CallMgrSpecific;
} CO_CALL_MANAGER_PARAMETERS, *PCO_CALL_MANAGER_PARAMETERS;


//
// this is the generic portion of the media parameters, including the media
// specific component too.
//
typedef struct _CO_MEDIA_PARAMETERS
{
	ULONG						Flags;
	ULONG						ReceivePriority;
	ULONG						ReceiveSizeHint;
	CO_SPECIFIC_PARAMETERS		MediaSpecific;
} CO_MEDIA_PARAMETERS, *PCO_MEDIA_PARAMETERS;

//
// definitions for the flags in CO_MEDIA_PARAMETERS
//
#define RECEIVE_TIME_INDICATION			0x00000001
#define USE_TIME_STAMPS					0x00000002
#define TRANSMIT_VC						0x00000004
#define RECEIVE_VC						0x00000008
#define INDICATE_ERRED_PACKETS			0x00000010
#define INDICATE_END_OF_TX				0x00000020
#define RESERVE_RESOURCES_VC			0x00000040
#define	ROUND_DOWN_FLOW					0x00000080
#define	ROUND_UP_FLOW					0x00000100
//
// define a flag to set in the flags of an Ndis packet when the miniport
// indicates a receive with an error in it
//
#define ERRED_PACKET_INDICATION	0x00000001

//
// this is the structure passed during call-setup
//
typedef struct _CO_CALL_PARAMETERS
{
	ULONG						Flags;
	PCO_CALL_MANAGER_PARAMETERS CallMgrParameters;
	PCO_MEDIA_PARAMETERS		MediaParameters;
} CO_CALL_PARAMETERS, *PCO_CALL_PARAMETERS;

//
// Definitions for the Flags in CO_CALL_PARAMETERS
//
#define PERMANENT_VC			0x00000001
#define CALL_PARAMETERS_CHANGED 0x00000002
#define QUERY_CALL_PARAMETERS	0x00000004
#define BROADCAST_VC			0x00000008

//
// The format of the Request for adding/deleting a PVC
//
typedef struct _CO_PVC
{
	IN	 NDIS_HANDLE			NdisAfHandle;
	IN	 CO_SPECIFIC_PARAMETERS	PvcParameters;
} CO_PVC,*PCO_PVC;

//
// NDIS 4.1 Extensions for protocols
//

EXPORT
VOID
NdisCoSendPackets(
	IN	NDIS_HANDLE				NdisVcHandle,
	IN	PPNDIS_PACKET			PacketArray,
	IN	UINT					NumberOfPackets
	);


EXPORT
NDIS_STATUS
NdisCoCreateVc(
	IN	NDIS_HANDLE				NdisBindingHandle,
	IN	NDIS_HANDLE				NdisAfHandle		OPTIONAL,	// For CM signalling VCs
	IN	NDIS_HANDLE				ProtocolVcContext,
	OUT	PNDIS_HANDLE			NdisVcHandle
	);


EXPORT
NDIS_STATUS
NdisCoDeleteVc(
    IN  NDIS_HANDLE             NdisVcHandle
    );


EXPORT
NDIS_STATUS
NdisCoRequest(
	IN	NDIS_HANDLE				NdisBindingHandle,
	IN	NDIS_HANDLE				NdisAfHandle,
	IN	NDIS_HANDLE				NdisVcHandle	OPTIONAL,
	IN	NDIS_HANDLE				NdisPartyHandle OPTIONAL,
	IN OUT PNDIS_REQUEST		NdisRequest
	);


EXPORT
VOID
NdisCoRequestComplete(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				NdisAfHandle,
	IN	NDIS_HANDLE				NdisVcHandle	OPTIONAL,
	IN	NDIS_HANDLE				NdisPartyHandle	OPTIONAL,
	IN	PNDIS_REQUEST			NdisRequest
	);


//
// Client Apis
//
EXPORT
NDIS_STATUS
NdisClOpenAddressFamily(
	IN	NDIS_HANDLE				NdisBindingHandle,
	IN	PCO_ADDRESS_FAMILY		AddressFamily,
	IN	NDIS_HANDLE				ProtocolAfContext,
	IN	PNDIS_CLIENT_CHARACTERISTICS ClCharacteristics,
	IN	UINT					SizeOfClCharacteristics,
	OUT	PNDIS_HANDLE			NdisAfHandle
	);


EXPORT
NDIS_STATUS
NdisClCloseAddressFamily(
	IN	NDIS_HANDLE				NdisAfHandle
	);


EXPORT
NDIS_STATUS
NdisClRegisterSap(
	IN	NDIS_HANDLE				NdisAfHandle,
	IN	NDIS_HANDLE				ProtocolSapContext,
	IN	PCO_SAP					Sap,
	OUT	PNDIS_HANDLE			NdisSapHandle
	);


EXPORT
NDIS_STATUS
NdisClDeregisterSap(
	IN	NDIS_HANDLE				NdisSapHandle
	);


EXPORT
NDIS_STATUS
NdisClMakeCall(
	IN	NDIS_HANDLE				NdisVcHandle,
	IN OUT PCO_CALL_PARAMETERS	CallParameters,
	IN	NDIS_HANDLE				ProtocolPartyContext	OPTIONAL,
	OUT	PNDIS_HANDLE			NdisPartyHandle			OPTIONAL
	);


EXPORT
NDIS_STATUS
NdisClCloseCall(
	IN	NDIS_HANDLE				NdisVcHandle,
	IN	NDIS_HANDLE				NdisPartyHandle			OPTIONAL,
	IN	PVOID					Buffer					OPTIONAL,
	IN	UINT					Size					OPTIONAL
	);


EXPORT
NDIS_STATUS
NdisClModifyCallQoS(
	IN	NDIS_HANDLE				NdisVcHandle,
	IN	PCO_CALL_PARAMETERS		CallParameters
	);


EXPORT
VOID
NdisClIncomingCallComplete(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				NdisVcHandle,
	IN	PCO_CALL_PARAMETERS		CallParameters
	);


EXPORT
NDIS_STATUS
NdisClAddParty(
	IN	NDIS_HANDLE				NdisVcHandle,
	IN	NDIS_HANDLE				ProtocolPartyContext,
	IN OUT PCO_CALL_PARAMETERS	CallParameters,
	OUT	PNDIS_HANDLE			NdisPartyHandle
	);


EXPORT
NDIS_STATUS
NdisClDropParty(
	IN	NDIS_HANDLE				NdisPartyHandle,
	IN	PVOID					Buffer	OPTIONAL,
	IN	UINT					Size	OPTIONAL
	);


//
// Call Manager Apis
//
EXPORT
NDIS_STATUS
NdisCmRegisterAddressFamily(
	IN	NDIS_HANDLE				NdisBindingHandle,
	IN	PCO_ADDRESS_FAMILY		AddressFamily,
	IN	PNDIS_CALL_MANAGER_CHARACTERISTICS CmCharacteristics,
	IN	UINT					SizeOfCmCharacteristics
	);


EXPORT
VOID
NdisCmOpenAddressFamilyComplete(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				NdisAfHandle,
	IN	NDIS_HANDLE				ClientAfContext
	);


EXPORT
VOID
NdisCmCloseAddressFamilyComplete(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				NdisSapHandle
	);


EXPORT
VOID
NdisCmRegisterSapComplete(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				NdisSapHandle,
	IN	NDIS_HANDLE				CalllMgrSapContext
	);


EXPORT
VOID
NdisCmDeregisterSapComplete(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				NdisSapHandle
	);


EXPORT
NDIS_STATUS
NdisCmActivateVc(
	IN	NDIS_HANDLE				NdisVcHandle,
	IN OUT PCO_CALL_PARAMETERS	MediaParameters
	);


EXPORT
NDIS_STATUS
NdisCmDeactivateVc(
	IN	NDIS_HANDLE				NdisVcHandle
	);


EXPORT
VOID
NdisCmMakeCallComplete(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				NdisVcHandle,
	IN  NDIS_HANDLE             NdisPartyHandle		OPTIONAL,
	IN	NDIS_HANDLE				CallMgrPartyContext	OPTIONAL,
	IN	PCO_CALL_PARAMETERS		CallParameters
	);


EXPORT
VOID
NdisCmCloseCallComplete(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				NdisVcHandle,
	IN	NDIS_HANDLE				NdisPartyHandle	OPTIONAL
	);


EXPORT
VOID
NdisCmAddPartyComplete(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				NdisPartyHandle,
	IN	NDIS_HANDLE				CallMgrPartyContext	OPTIONAL,
	IN	PCO_CALL_PARAMETERS		CallParameters
	);


EXPORT
VOID
NdisCmDropPartyComplete(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				NdisPartyHandle
	);


EXPORT
NDIS_STATUS
NdisCmDispatchIncomingCall(
	IN	NDIS_HANDLE				NdisSapHandle,
	IN	NDIS_HANDLE				NdisVcHandle,
	IN	PCO_CALL_PARAMETERS		CallParameters
	);


EXPORT
VOID
NdisCmDispatchCallConnected(
	IN	NDIS_HANDLE				NdisVcHandle
	);


EXPORT
VOID
NdisCmModifyCallQoSComplete(
	IN	NDIS_STATUS				Status,
	IN	NDIS_HANDLE				NdisVcHandle,
	IN	PCO_CALL_PARAMETERS		CallParameters
	);


EXPORT
VOID
NdisCmDispatchIncomingCallQoSChange(
	IN	NDIS_HANDLE				NdisVcHandle,
	IN	PCO_CALL_PARAMETERS		CallParameters
	);


EXPORT
VOID
NdisCmDispatchIncomingCloseCall(
	IN	NDIS_STATUS				CloseStatus,
	IN	NDIS_HANDLE				NdisVcHandle,
	IN	PVOID					Buffer,
	IN	UINT					Size
	);


EXPORT
VOID
NdisCmDispatchIncomingDropParty(
	IN	NDIS_STATUS				DropStatus,
	IN	NDIS_HANDLE				NdisPartyHandle,
	IN	PVOID					Buffer,
	IN	UINT					Size
	);




#endif // defined(NDIS41) || defined(NDIS41_MINIPORT)

#endif // _NDIS_
