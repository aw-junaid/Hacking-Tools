#ifndef __DEBUG_H_
#define __DEBUG_H_

#define DRIVER_NAME NTTDIDR_DRIVER_NAMEA ".sys"

/*
 *	Debug levels, DL() macro defined below
 */
typedef enum {
	DbgLevelNone = 0,
	DbgLevelTrace,
	DbgLevelDebug,
	DbgLevelInfo,
	DbgLevelError,
	DbgLevelCriticalTrace,
	DbgLevelCriticalDebug,
	DbgLevelCriticalInfo,
	DbgLevelCriticalError,

	DbgLevelBeforeBsod = 255,
	DbgLevelMax = 0x0FFF
} DBG_LEVEL;

#define DbgNoDbgOut 0x2000 //if you want prevent write to debug output
#define DbgNoEventLog 0x4000 // if you want prevent write to event log (event log is written only if irql < DISPATCH_LEVEL)
#define DbgNoDiskLog 0x8000 // if you want prevent write to disk log (MUST if very offten)

#define DbgNoLevel 0xFFFFFFFF

#define LNone		DL(DbgLevelNone)
#define LTrace		DL(DbgLevelTrace)
#define LDebug		DL(DbgLevelDebug)
#define LInfo		DL(DbgLevelInfo)
#define LError		DL(DbgLevelError)
#define LCriticalTrace	DL(DbgLevelCriticalTrace)
#define LCriticalDebug	DL(DbgLevelCriticalDebug)
#define LCriticalInfo	DL(DbgLevelCriticalInfo)
#define LCriticalError	DL(DbgLevelCriticalError)

/*
 *	Main routine, macros and usage
 */
VOID __cdecl 
	DebugLog(
		IN DBG_LEVEL level,
		IN PCHAR file,
		IN ULONG line,
		IN PCHAR func,
		IN PCHAR fmt,
		...
		);
NTSTATUS 
	DbgQueryLevel(
		IN PUNICODE_STRING RegistryPath
		);

#if DBG
	#define DBGLOG(x)	DebugLog##x
	#define SAVE_IRQL KIRQL _Irql_SAVE=KeGetCurrentIrql();
	#define CHECK_IRQL if (_Irql_SAVE!=KeGetCurrentIrql()) __debugbreak();
	#define ASSERT_LOCKED(x) ASSERT((KeGetCurrentIrql() == DISPATCH_LEVEL) && (*x != 0 || KeNumberProcessors == 1))
#else
	#define DBGLOG(x)
	#define SAVE_IRQL KIRQL _Irql_SAVE=KeGetCurrentIrql();
	#define CHECK_IRQL if (_Irql_SAVE!=KeGetCurrentIrql()) __debugbreak();
	#define ASSERT_LOCKED(x)
#endif

#define __POS__ __FILE__,__LINE__,__FUNCTION__
#define DL(x)	x,__POS__

#if 0
		DBGLOG(LError,"This will shown in both DBG and RELEASE \n");
		DBGLOG((LError,"This will shown in DBG only \n"));
		DBGLOG((DbgNoEventLog|LTrace,"This is my DPC routine \n"));
#endif

#define SHOULD_NEVER() __debugbreak()

#endif