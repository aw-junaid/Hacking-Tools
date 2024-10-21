#ifndef __CC_H_
#define __CC_H_

#include <wdm.h>
#include <inaddr.h>
#include <limits.h>

/* ROS-specific mem defs */
#define _CRT_ALLOCATION_DEFINED

void *
malloc(size_t size);

void *
calloc(size_t count, size_t size);

void
free(void *mem);

void *
realloc(void *mem, size_t size);

#define mem_trim(_m_, _s_) realloc(_m_, _s_)

/* Unsigned int types */
typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned long u32_t;
typedef ULONG64 u64_t;

/* Signed int types */
typedef signed char s8_t;
typedef signed short s16_t;
typedef signed long s32_t;
typedef LONG64 s64_t;

/* Memory pointer */
#if _X86_
typedef u32_t mem_ptr_t;
#else
typedef u64_t mem_ptr_t;
#endif

/* Printf/DPRINT formatters */
#define U16_F "hu"
#define S16_F "hd"
#define X16_F "hx"
#define U32_F "lu"
#define S32_F "ld"
#define X32_F "lx"

/* Endianness */
#define BYTE_ORDER LITTLE_ENDIAN

/* Checksum calculation algorithm choice */
#define LWIP_CHKSUM_ALGORITHM 3

/* Diagnostics */
#define LWIP_PLATFORM_DIAG(x) (DbgPrint x)
#define LWIP_PLATFORM_ASSERT(x) ASSERTMSG(x, FALSE)

/* Synchronization */
#define SYS_ARCH_DECL_PROTECT(lev) sys_prot_t (lev)
#define SYS_ARCH_PROTECT(lev) sys_arch_protect(&(lev))
#define SYS_ARCH_UNPROTECT(lev) sys_arch_unprotect(lev)

/* Compiler hints for packing structures */
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_USE_INCLUDES

#endif //__CC_H_