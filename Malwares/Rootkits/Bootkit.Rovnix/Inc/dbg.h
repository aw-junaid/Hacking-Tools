//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK driver loader project. Version 2.9.2
//	
// module: dbg.h
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description: 
//	Debug-build support routines and constants.

#pragma once

#include <crtdbg.h>

#if _DEBUG
 #define _DBG			TRUE
 #define _TRACE_ALLOC	TRUE
#endif

#define BAD_PTR		(LONG_PTR)0xBAADF00D
#define	PAGE_SIZE	0x1000

// Usefull types
typedef INT	WINERROR;					// One of the Windows error codes defined within winerror.h
#define ERROR_UNSUCCESSFULL	0xffffffff	// Common unsuccessfull code

extern	HANDLE	g_AppHeap;

#ifdef _TRACE_ALLOC

typedef struct _DBG_ALLOC
{
	unsigned long Size;
	unsigned long Sing;
	char  Buffer[1];
}DBG_ALLOC,*PDBG_ALLOC;


__inline void* DbgAlloc(size_t Size)
{
	void* mem = malloc(Size+16);
	if (mem)
	{
		PDBG_ALLOC pd = (PDBG_ALLOC) mem;
		memset(mem, 0xcc, Size+16);
		pd->Size = (unsigned long) Size;
		return(&pd->Buffer);
	}
	return(mem);
}

__inline void DbgFree(void* mem)
{
	PDBG_ALLOC pd = CONTAINING_RECORD(mem, DBG_ALLOC, Buffer);
	if (*(unsigned long*)((PCHAR)pd->Buffer + pd->Size)!= 0xcccccccc)
		__debugbreak();
	if (pd->Sing != 0xcccccccc)
		__debugbreak();
	free(pd);

}


__inline void* DbgRealloc(void* Mem, size_t Size)
{
	void* mem = malloc(Size+16);
	if (mem)
	{
		PDBG_ALLOC pd = (PDBG_ALLOC) mem;
		PDBG_ALLOC pd1 = CONTAINING_RECORD(Mem, DBG_ALLOC, Buffer);
		if (pd1->Sing != 0xcccccccc)
			__debugbreak();

		memset(mem, 0xcc, Size+16);
		pd->Size = (unsigned long) Size;
		memcpy(&pd->Buffer, &pd1->Buffer, pd1->Size);
	
		DbgFree(Mem);
		return(&pd->Buffer);
	}

	return(mem);
}

__inline void* DbgHeapAlloc(size_t Size)
{
	void* mem;
	if (g_AppHeap == 0)
		__debugbreak();

	mem = HeapAlloc(g_AppHeap, 0, Size+16);
	if (mem)
	{
		PDBG_ALLOC pd = (PDBG_ALLOC) mem;
		memset(mem, 0xcc, Size+16);
		pd->Size = (unsigned long) Size;
		return(&pd->Buffer);
	}
	return(mem);
}

__inline void DbgHeapFree(void* mem)
{
	PDBG_ALLOC pd = CONTAINING_RECORD(mem, DBG_ALLOC, Buffer);
	if (g_AppHeap == 0)
		__debugbreak();
	if (*(unsigned long*)((PCHAR)pd->Buffer + pd->Size)!= 0xcccccccc)
		__debugbreak();
	if (pd->Sing != 0xcccccccc)
		__debugbreak();
	HeapFree(g_AppHeap, 0, pd);

}


__inline void* DbgHeapRealloc(void* Mem, size_t Size)
{
	void* mem = HeapAlloc(g_AppHeap, 0, Size+16);
	if (mem)
	{
		PDBG_ALLOC pd = (PDBG_ALLOC) mem;
		PDBG_ALLOC pd1 = CONTAINING_RECORD(Mem, DBG_ALLOC, Buffer);
		if (g_AppHeap == 0)
			__debugbreak();

		if (pd1->Sing != 0xcccccccc)
			__debugbreak();

		memset(mem, 0xcc, Size+16);
		pd->Size = (unsigned long) Size;
		memcpy(&pd->Buffer, &pd1->Buffer, pd1->Size);
	
		DbgHeapFree(Mem);
		return(&pd->Buffer);
	}

	return(mem);
}




#define Alloc(x)		DbgAlloc(x)
#define Free(x)			DbgFree(x)//;x = (PVOID)BAD_PTR
#define Realloc(x,y)	DbgRealloc(x,y)

#define hAlloc(x)		DbgHeapAlloc(x)
#define hFree(x)		DbgHeapFree(x)//;x = (PVOID)BAD_PTR
#define hRealloc(x,y)	DbgHeapRealloc(x,y)

#else

#define Alloc(x)		LocalAlloc(LPTR, x)
#define Free(x)			LocalFree(x)
#define Realloc(x,y)	LocalReAlloc(x, y, LPTR)

#define hAlloc(x)		HeapAlloc(g_AppHeap, 0, x)
#define hFree(x)		HeapFree(g_AppHeap, 0, x)


#endif


#define vAlloc(x)	VirtualAlloc(0, x, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)
#define vFree(x)	VirtualFree(x, 0, MEM_RELEASE)


#ifdef _DBG



#pragma warning(disable:4996) // 'sprintf': This function or variable may be unsafe.
#define  DbgPrint(args, ...) \
		{ char* buff = (char*)LocalAlloc(LPTR, 0x100); \
		  wsprintfA(buff, args, __VA_ARGS__); \
		  OutputDebugStringA((LPCSTR)buff); \
		  LocalFree(buff); } 

#define  DbgPrintW(args, ...) \
		{ wchar_t* buff = (wchar_t*)LocalAlloc(LPTR, 0x400); \
		  wsprintfW(buff, args, __VA_ARGS__); \
		  OutputDebugStringW((LPCWSTR)buff); \
		  LocalFree(buff); } 

#define ASSERT(x) _ASSERT(x)
  

#else
	#define DbgPrint(x, ...) 
	#define DbgPrintW(x, ...) 
	#define ASSERT(x)
/*
#define Alloc(x)		malloc(x)
#define Free(x)		free(x)
#define Realloc(x,y)	realloc(x,y)

*/	
#endif

