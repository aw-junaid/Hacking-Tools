/*
 * Copyright (c) 2015, Ryan O'Neill
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _LIBELFMASTER_INTERNAL_H_
#define _LIBELFMASTER_INTERNAL_H_

#include "dwarf.h"

#ifdef DEBUG
#define DEBUG_LOG(...) do { fprintf(stderr, __VA_ARGS__); } while(0)
#else
#define DEBUG_LOG(...) do {} while(0)
#endif

#if 0
#ifdef __GNUC__
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#else
#define likely(x)       (x)
#define unlikely(x)     (x)
#endif
#endif

#define CACHEMAGIC "ld.so-1.7.0"
struct file_entry {
	int flags;
	uint32_t key;
	uint32_t value;
};

struct cache_file {
	char magic[sizeof CACHEMAGIC - 1];
	uint32_t nlibs;
	struct file_entry libs[0];
};

#define CACHEMAGIC_NEW "glibc-ld.so.cache"
#define CACHE_VERSION "1.1"

#define ELF_LDSO_CACHE_OLD (1 << 0)
#define ELF_LDSO_CACHE_NEW (1 << 1)

struct file_entry_new {
	int32_t flags;
	uint32_t key;
	uint32_t value;
	uint32_t osversion;
	uint64_t hwcap;
};

struct cache_file_new {
	char magic[sizeof CACHEMAGIC_NEW - 1];
	char version[sizeof CACHE_VERSION - 1];
	uint32_t nlibs;		/* number of entries */
	uint32_t len_strings;	/* size of string table */
	uint32_t unused[5];	/* space for future extension */
	struct file_entry_new libs[0]; /* Entries describing libraries */
	/* After this the string table of size len_strings is found */
};

/*
 * This struct is used internally only.
 */
struct elf_rel_helper_node {
	union {
		Elf32_Rel *rel32;
		Elf64_Rel *rel64;
	};
	union {
		Elf32_Rela *rela32;
		Elf64_Rela *rela64;
	};
	size_t size;
	bool addend;
	char *section_name;
	LIST_ENTRY(elf_rel_helper_node) _linkage;
};

/*
 * When we reconstruct sections we create a string table
 * that is a maximum of this size.
 */
#define INTERNAL_SHSTRTAB_SIZE 1024
#define INTERNAL_SECTION_COUNT 4096
/*
 * This should only be used internally.
 */
struct elf_eh_frame_node {
        unsigned long long pc_begin;
        unsigned long long pc_end;
        size_t len;
        LIST_ENTRY(elf_eh_frame_node) _linkage;
};

struct elf_symbol_node {
	const char *name;
	uint64_t value;
	uint64_t size;
	uint16_t shndx;
	uint8_t bind;
	uint8_t type;
	uint8_t visibility;
	LIST_ENTRY(elf_symbol_node) _linkage;
};

struct elf_section_node {
	char *name;
	uint32_t type;
	uint32_t link;
	uint32_t info;
	uint64_t align;
	uint64_t entsize;
	uint64_t offset;
	uint64_t address;
	size_t size;
	LIST_ENTRY(elf_section_node) _linkage;
};

typedef struct elf_shared_object_node {
	const char *basename;
	char *path;
	unsigned int index; // used by elf_shared_object iterator
	LIST_ENTRY(elf_shared_object_node) _linkage;
} elf_shared_object_node_t;

typedef struct elf_plt_node {
	char *symname;
	uint64_t addr;
	LIST_ENTRY(elf_plt_node) _linkage;
} elf_plt_node_t;


typedef struct elf_malloc_node {
	void *ptr;
	LIST_ENTRY(elf_malloc_node) _linkage;
} elf_malloc_node_t;

typedef struct elf_fde_node {
	unsigned long pc_start;
	unsigned long pc_end;
	size_t len;
	LIST_ENTRY(elf_fde_node) _linkage;
} elf_fde_node;

bool elf_error_set(elf_error_t *, const char *, ...);

int section_name_cmp(const void *, const void *);

bool build_plt_data(struct elfobj *);

bool build_dynsym_data(struct elfobj *);

bool build_symtab_data(struct elfobj *);

const char * ldso_cache_bsearch(struct elf_shared_object_iterator *,
    const char *);

bool ldso_recursive_cache_resolve(struct elf_shared_object_iterator *,
    const char *);

bool ldso_insert_yield_cache(struct elf_shared_object_iterator *,
    const char *);

void ldso_free_malloc_list(struct elf_shared_object_iterator *);

void ldso_cleanup(struct elf_shared_object_iterator *);

bool load_dynamic_segment_data(struct elfobj *);

void free_lists(elfobj_t *);

void free_caches(elfobj_t *);

void free_arrays(elfobj_t *);

void free_misc(elfobj_t *);

bool insane_section_headers(elfobj_t *);

bool insane_dynamic_segment(elfobj_t *);

bool reconstruct_elf_sections(elfobj_t *, elf_error_t *);

bool sort_elf_sections(elfobj_t *obj, elf_error_t *);

bool sanity_check(uint64_t, uint64_t);
/*
 * Resolve PLT address since it is not directly in the dynamic segment
 * and we need to locate it for stripped executables. We use a few
 * simple tricks.
 */
uint64_t resolve_plt_addr(elfobj_t *obj);

bool phdr_sanity(elfobj_t *, void *);
/*
 * Get the address range of every function found in .eh_frame
 */
ssize_t dw_get_eh_frame_ranges(elfobj_t *);

/*
 * Parse a single line of the /bin/ldd output
 */
char * ldd_parse_line(struct elf_shared_object_iterator *);

/*
 * Verify that a shared object basename is top-level only, meaning
 * it exists in the dynamic segment's DT_NEEDED list only within
 * the root executable.
 */
bool verify_so_is_toplevel(const char *, elfobj_t *);

/*
 * See if a binaries linking is static PIE
 */
bool check_static_pie(elfobj_t *);

#endif // _LIBELFMASTER_INTERNAL_H_
