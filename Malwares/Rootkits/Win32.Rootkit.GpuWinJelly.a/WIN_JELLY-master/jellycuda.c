///////////////////////////////////////////////////////////////////////////////
//
// Gone but not forgotten, 
// dead or alive, 
// in the name of the unresolved, 
// here it is...
//
// The Cuda JellyFish

#include <windows.h>

#define CUDACALL __stdcall

//typedef int size_t ;
typedef struct CUctx_st *CUcontext; 

      /**< Host   -> Host */
#define MemcpyKindHostToHost         0
      /**< Host   -> Device */
#define MemcpyKindHostToDevice       1
      /**< Device -> Host */
#define MemcpyKindDeviceToHost       2
      /**< Device -> Device */
#define MemcpyKindDeviceToDevice     3
       /**< Default based unified virtual address space */
#define MemcpyKindDefault            4

typedef int (CUDACALL * LPFNcuMemAlloc) ( void** devPtr, size_t size );
typedef int (CUDACALL * LPFNcuInit)(int);
typedef int (CUDACALL * LPFNcuDeviceGetCount)(int *count);
typedef int (CUDACALL * LPFNcuDeviceGet)(int *device, int ordinal);
typedef int (CUDACALL * LPFNcuCtxCreate)(CUcontext *pctx, unsigned int flags, int dev);
typedef int (CUDACALL * LPFNcuMemcpy)(void *dst, const void *src, size_t count, int kind);

typedef struct _JellyCuda 
{
	LPFNcuMemAlloc fncuMemAlloc;
	LPFNcuInit fncuInit;
	LPFNcuDeviceGetCount fncuDeviceGetCount;
	LPFNcuDeviceGet fncuDeviceGet;
	LPFNcuCtxCreate fncuCtxCreate;
	LPFNcuMemcpy fncuMemcpy;
} JellyCuda, *PJellyCuda;



int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HMODULE hCuda;
	int nDevices;
	int device;
	int result;
	CUcontext context;
	void * memory;
	JellyCuda jc;

	hCuda = LoadLibraryA("nvcuda.dll");

	if(hCuda == NULL) return -1;

	jc.fncuInit = (LPFNcuInit)GetProcAddress(hCuda, "cuInit");
	jc.fncuMemAlloc = (LPFNcuMemAlloc)GetProcAddress(hCuda, "cuMemAlloc");
	jc.fncuDeviceGetCount = (LPFNcuDeviceGetCount)GetProcAddress(hCuda, "cuDeviceGetCount");
	jc.fncuDeviceGet = (LPFNcuDeviceGet)GetProcAddress(hCuda, "cuDeviceGet");
	jc.fncuCtxCreate = (LPFNcuCtxCreate)GetProcAddress(hCuda, "cuCtxCreate");
	jc.fncuMemcpy = (LPFNcuMemcpy)GetProcAddress(hCuda, "cuMemcpy");

	result = jc.fncuInit(0);
	if(result != 0) goto hell;

	result = jc.fncuDeviceGetCount(&nDevices);
	if(result != 0 || nDevices == 0) goto hell;

	result = jc.fncuDeviceGet(&device, 0);
	if(result != 0) goto hell;

	result = jc.fncuCtxCreate(&context, 0, device);
	if(result != 0) goto hell;

	result = jc.fncuMemAlloc(&memory, 1024 * 1024);
	if(result != 0) goto hell;

	
hell:
	FreeLibrary(hCuda);
	return 0;
}