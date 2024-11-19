/*
 * Copyright (c) 2018, Ryan O'Neill
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


#ifndef _LIBELFMASTER_H_
#define _LIBELFMASTER_H_

#include <link.h>
#include <limits.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <search.h>
#include <sys/stat.h>
#include "o_elf.h"
#include "queue.h"

#define MAX_ERROR_STR_LEN 128

/*
 * DT_PLTREL tag has two valid
 * values, 0x7 and 0x11
 */
#define ELF_DT_PLTREL_RELA	0x7
#define ELF_DT_PLTREL_REL	0x11

/*
 * In reality will never exceed 2,3, or 4 at the highest
 * i.e. 4 PT_LOAD with SCOP (secure code partitioning)
 */
#define MAX_LOADABLE_MAPPINGS 12

#define ELFNOTE_NAME(_n_) ((unsigned char*)(_n_) + sizeof(*(_n_)))
#define ELFNOTE_ALIGN(_n_) (((_n_)+3)&~3)
#define ELFNOTE_NAME(_n_) ((unsigned char*)(_n_) + sizeof(*(_n_)))
#define ELFNOTE_DESC(_n_) (ELFNOTE_NAME(_n_) + ELFNOTE_ALIGN((_n_)->n_namesz))
#define ELFNOTE32_NEXT(_n_) ((Elf32_Nhdr *)(ELFNOTE_DESC(_n_) + ELFNOTE_ALIGN((_n_)->n_descsz)))
#define ELFNOTE64_NEXT(_n_) ((Elf64_Nhdr *)(ELFNOTE_DESC(_n_) + ELFNOTE_ALIGN((_n_)->n_descsz)))
#define ELFNOTE_DESCSZ(_n_) ELFNOTE_ALIGN((_n_)->n_descsz)
#define ELFNOTE_NAMESZ(_n_) ELFNOTE_ALIGN((_n_)->n_namesz)
#ifndef PT_PAX_FLAGS
#define PT_PAX_FLAGS  0x65041580
#endif

#ifndef PT_GNU_EH_FRAME
#define PT_GNU_EH_FRAME 0x6474e550
#endif

#ifndef PT_GNU_STACK
#define PT_GNU_STACK 0x6474e551
#endif

#ifndef PT_GNU_RELRO
#define PT_GNU_RELRO 0x6474e552
#endif

#define MAX_VALID_SHNUM 65535 - 1

#define SYMTAB_RECONSTRUCT_COUNT 8192

#define ELF_ERRNO_INVALMAGIC	0
#define ELF_ERRNO_INVALSHDRS	1

#define ELF_LXC_ROOTFS_VAR "LXC_ROOTFS_VAR_LIBELFMASTER"

typedef struct elf_error {
	char string[MAX_ERROR_STR_LEN];
	int _errno;
} elf_error_t;

typedef enum elf_arch {
	i386,
	x64,
	unsupported
} elf_arch_t;

typedef enum elf_class {
	elfclass64,
	elfclass32
} elf_class_t;

typedef enum elf_obj_flags {
	ELF_SYMTAB_F =			(1 << 0),
	ELF_DYNSYM_F =			(1 << 1),
	ELF_PHDRS_F =			(1 << 2),
	ELF_SHDRS_F =			(1 << 3),
	ELF_NOTE_F =			(1 << 4),
	ELF_PLT_RELOCS_F =		(1 << 5),
	ELF_DYN_RELOCS_F =		(1 << 6),
	ELF_TEXT_RELOCS_F =		(1 << 7),
	ELF_PIE_F =			(1 << 8), /* ET_DYN not necessarily fully relocatable though */
	ELF_DYNAMIC_F =			(1 << 9),
	ELF_PLT_F =			(1 << 10),
	ELF_PLTGOT_F =			(1 << 11),
	ELF_DYNSTR_F =			(1 << 12),
	ELF_EH_FRAME_F =		(1 << 13),
	ELF_FULL_PIE_F =		(1 << 14), /* fully relocatable ET_DYN */
	ELF_SYMTAB_RECONSTRUCTION_F =	(1 << 15), /* .symtab is being reconstructed */
	ELF_FORENSICS_F =		(1 << 16),  /* elf sections at the least are reconstructed */
	ELF_DT_DEBUG_F =		(1 << 17),
	ELF_SCOP_F =			(1 << 18), /* secure code partitioning */
	ELF_MERGED_SEGMENTS_F =		(1 << 19)  /* Merged text+data segment, i.e. gcc -nostdlib -N -static test.c -o test */
} elf_obj_flags_t;

/*
 * Portable ELF section type. Contains pointer to
 * actual string. These sections are also stored in
 * a sorted array which can be searched with bsearch
 * by name, address, or offset.
 */
struct elf_section {
	char *name;
	uint32_t type;
	uint32_t link;
	uint32_t info;
	uint64_t flags;
	uint64_t align;
	uint64_t entsize;
	uint64_t offset;
	uint64_t address;
	size_t size;
};

struct elf_segment {
	uint32_t type;
	uint32_t flags;
	uint64_t offset;
	uint64_t paddr;
	uint64_t vaddr;
	uint64_t filesz;
	uint64_t memsz;
	uint64_t align;
	unsigned int index;
};

struct elf_symbol {
	const char *name;
	uint64_t value;
	uint64_t size;
	uint16_t shndx;
	uint8_t bind;
	uint8_t type;
	uint8_t visibility;
};

struct elf_relocation {
	uint64_t offset;
	uint64_t type;
	int64_t addend;
	char *symname;
	char *shdrname;
};

typedef struct elf_mapping {
	uint8_t *mem[MAX_LOADABLE_MAPPINGS];
	unsigned int index;
	unsigned int flags;
	size_t len;
} elf_mapping_t;

typedef struct elf_shared_object {
	const char *basename;
	char *path;
} elf_shared_object_t;

typedef struct elf_eh_frame {
	unsigned long long pc_begin;
	unsigned long long pc_end;
	size_t len;
} elf_eh_frame_t;

typedef struct elf_plt {
	char *symname;
	uint64_t addr;
} elf_plt_t;

/*
 * Used by elfobj
 */
struct pt_load {
	union {
		Elf64_Phdr phdr64;
		Elf32_Phdr phdr32;
	};
	uint32_t flag;
};

/*
 * TODO
 * Move the anomaly values to internal.h.
 */
/*
 * Flags for anomalies on section headers
 */
#define INVALID_F_SHOFF		(1ULL << 0)
#define INVALID_F_SHSTRNDX	(1ULL << 1)
#define INVALID_F_SHOFFSET	(1ULL << 2) /* e_shoff + e_shnum * e_shentsize are invalid */
#define INVALID_F_SHNUM		(1ULL << 3)
#define INVALID_F_SHENTSIZE	(1ULL << 4)
#define INVALID_F_SH_HEADERS	(1ULL << 5)
#define INVALID_F_SHSTRTAB	(1ULL << 6)

/*
 * Flags for anomalies on dynamic segment
 */
#define INVALID_F_VITAL_DTAG_VALUE	(1ULL << 7)

/*
 * This struct is not meant to access directly. It is an opaque
 * type. It is only accessed directly from within the API code
 * itself (obviously).
 */
typedef struct elfobj {
	int fd;
	elf_arch_t arch;
	elf_class_t e_class;
	elf_obj_flags_t flags;
	unsigned int type;
	unsigned long long int anomalies;
	uint64_t load_flags;
	const char *path;
	union {
		Elf32_Ehdr *ehdr32;
		Elf64_Ehdr *ehdr64;
	};
	union {
		Elf32_Phdr *phdr32;
		Elf64_Phdr *phdr64;
	};
	union {
		Elf32_Shdr *shdr32;
		Elf64_Shdr *shdr64;
	};
	union {
		Elf32_Sym *symtab32;
		Elf64_Sym *symtab64;
	};
	union {
		Elf32_Sym *dynsym32;
		Elf64_Sym *dynsym64;
	};
	union {
		Elf32_Dyn *dynamic32;
		Elf64_Dyn *dynamic64;
	};
	union {
		Elf32_Nhdr *note32;
		Elf64_Nhdr *note64;
	};
	void *eh_frame_hdr;
	void *eh_frame;
	/*
	 * Sorted sections
	 */
	struct elf_section **sections;
	/*
	 * Loadable segments (i.e. text, data)
	 */
#define MAX_PT_LOAD 65535
#define ELF_PT_LOAD_TEXT_F (1 << 0)
#define ELF_PT_LOAD_DATA_F (1 << 1)
#define ELF_PT_LOAD_MISC_F (1 << 2)
	/* Handle SCOP cases */
#define ELF_PT_LOAD_TEXT_RDONLY_F (1 << 3)
	/*
	 * Handle merged text and data
	 */
#define ELF_PT_LOAD_MERGED_F	(1 << 4)

	struct pt_load *pt_load;
	size_t load_count;
	/*
	 * caches
	 */
	struct {
		struct hsearch_data symtab;
		struct hsearch_data dynsym;
		struct hsearch_data plt;
	} cache;
	/*
	 * lists
	 */
	struct {
		LIST_HEAD(elf_symtab_list, elf_symbol_node) symtab, symtab_backup;
		LIST_HEAD(elf_dynsym_list, elf_symbol_node) dynsym, dynsym_backup;
		LIST_HEAD(elf_plt_list, elf_plt_node) plt;
		LIST_HEAD(elf_shared_object_list, elf_shared_object_node) shared_objects;
		LIST_HEAD(elf_section_list, elf_section_node) sections;
		LIST_HEAD(elf_eh_frame_list, elf_eh_frame_node) eh_frame_entries;
	} list;
	/*
	 * dynamic segment values
	 */
	struct {
		struct {
			uint64_t addr;
			uint64_t size;
		} init_array;
		struct {
			uint64_t addr;
			uint64_t size;
		} fini_array;
		struct {
			uint64_t addr;
		} pltgot;
		struct {
			uint64_t size;
		} relent;
		struct {
			uint64_t size;
		} relaent;
		struct {
			uint64_t addr;
			uint64_t size;
			uint32_t type;
		} pltrel;
		struct {
			uint64_t addr;
			uint64_t size;
		} rela;
		struct {
			uint64_t addr;
			uint64_t size;
		} rel;
		struct {
			uint64_t addr;
		} dynsym;
		struct {
			uint64_t addr;
			uint64_t size;
		} dynstr;
		struct {
			uint64_t addr;
		} init;
		struct {
			uint64_t addr;
		} fini;
		struct {
			uint64_t addr;
		} hash;
		struct {
			uint64_t offset;
		} runpath;
		struct {
			uint64_t value;
		} debug;
		bool exists;
	} dynseg;

	/*
	 * Pointers to .dynstr, .strtab, and .shstrtab
	 */
	char *dynstr;
	char *strtab;
	char *shstrtab;
	size_t internal_section_count;
	size_t internal_shstrtab_size;
	size_t strindex; // used only when creating custom strtabs
	size_t shdrindex; // used only when reconstructing sections
	uint8_t *mem;
	size_t size;
	size_t text_segment_filesz;
	size_t data_segment_filesz;
	size_t section_count;
	size_t segment_count;
	size_t symtab_count;
	size_t dynsym_count;
	size_t note_size;
	size_t dtag_count; /* dynamic tag count */
	size_t dynamic_size;
	size_t eh_frame_hdr_size;
	size_t init_array_size;
	size_t fini_array_size;
	size_t fde_count; /* holds the number of fde's after they've been parsed */
	uint64_t eh_frame_hdr_addr;
	uint64_t eh_frame_hdr_offset;
	uint64_t init_array_vaddr;
	uint64_t fini_array_vaddr;
	uint64_t dynamic_addr;
	uint64_t dynamic_offset;
	uint64_t entry_point;
	uint64_t text_address; /* text base address */
	uint64_t text_offset;
	uint64_t data_address; /* data segment address */
	uint64_t data_offset;
	uint64_t note_offset; /* Offset of first note section found */
} elfobj_t;

/*
 * Iterator types.
 */
typedef struct elf_section_iterator {
	unsigned int index;
	elfobj_t *obj;
} elf_section_iterator_t;

typedef struct elf_segment_iterator {
	unsigned int index;
	elfobj_t *obj;
} elf_segment_iterator_t;

typedef struct elf_note_iterator {
	unsigned int index;
	elfobj_t *obj;
	union {
		Elf32_Nhdr *note32;
		Elf64_Nhdr *note64;
	};
} elf_note_iterator_t;

typedef struct elf_note_entry {
	unsigned int type;
	size_t size;
	void *mem;
} elf_note_entry_t;

typedef struct elf_dynamic_iterator {
	unsigned int index;
	elfobj_t *obj;
} elf_dynamic_iterator_t;

typedef struct elf_dynamic_entry {
	Elf64_Sxword tag;
	uint64_t value;
} elf_dynamic_entry_t;

typedef struct elf_symtab_iterator {
	struct elf_symbol_node *current;
	unsigned int index;
} elf_symtab_iterator_t;

typedef struct elf_dynsym_iterator {
	struct elf_symbol_node *current;
	unsigned int index;
} elf_dynsym_iterator_t;

typedef struct elf_eh_frame_iterator {
	struct elf_eh_frame_node *current;
} elf_eh_frame_iterator_t;

typedef struct elf_plt_iterator {
	struct elf_plt_node *current;
} elf_plt_iterator_t;

typedef struct elf_pltgot_iterator {
	unsigned int index;
	elfobj_t *obj;
	void *pltgot;
	size_t wordsize;
	size_t gotsize;
} elf_pltgot_iterator_t;

#define ELF_PLTGOT_RESERVED_DYNAMIC_F		(1 << 0)
#define ELF_PLTGOT_RESERVED_LINKMAP_F		(1 << 1)
#define ELF_PLTGOT_RESERVED_DL_RESOLVE_F	(1 << 2)
#define ELF_PLTGOT_PLT_STUB_F			(1 << 3)
#define ELF_PLTGOT_RESOLVED_F			(1 << 4)

typedef struct elf_pltgot_entry {
	uint64_t offset;
	uint64_t value;
	uint32_t flags; /* can be set to ELF_PLTGOT_* flags */
} elf_pltgot_entry_t;

typedef enum elf_iterator_res {
	ELF_ITER_OK,
	ELF_ITER_DONE,
	ELF_ITER_ERROR,
	ELF_ITER_NOTFOUND
} elf_iterator_res_t;

typedef struct elf_relocation_iterator {
	unsigned int index;
	elfobj_t *obj;
	LIST_HEAD(elf_rel_helper_list, elf_rel_helper_node) list;
	struct elf_rel_helper_node *current, *head;
} elf_relocation_iterator_t;

/*
 * Resolve top-level basenames to full paths using ld.so.cache parsing
 */
#define ELF_SO_RESOLVE_F (1 << 0)
/*
 * Get all dependencies recursively
 */
#define ELF_SO_RESOLVE_ALL_F (1 << 1)
/*
 * Piggy back on top of the dynamic linkers own functionality
 * to print transitively resolved shared library paths
 * to stdout with environment variable LD_TRACE_LOADED_OBJECTS
 * This flag currently only supports complete resolution-- meaning
 * that it will resolve all shared library dependencies for a
 * binary, and not just the top-level shared libs who's basenames
 * exist in the dynamic segment as DT_NEEDED entries. This
 * is significantly faster performing than the ELF_SO_RESOLVE_F_ALL
 * flag, and cannot be used in conjunction with ELF_SO_RESOLVE_F.
 */
#define ELF_SO_LDSO_FAST_F (1 << 2)

/*
 * Use this flag to ignore VDSO since it will not actually
 * have a full path. Combine this flag with ELF_SO_LDSO_FAST_F
 * since that is the only flag that will yield the vdso
 * basename.
 */
#define ELF_SO_IGNORE_VDSO_F (1 << 3)

typedef struct elf_shared_object_iterator {
	unsigned int index;
	elfobj_t *obj;
	int fd;
	void *mem;
	char *chunk;
	FILE *pd;
	struct stat st;
	struct cache_file *cache;
	struct cache_file_new *cache_new;
	char *cache_data;
	size_t cache_size;
	uint32_t flags;
	uint32_t cache_flags;
	bool resolve;
	struct elf_shared_object_node *current;
	struct elf_shared_object_node *yield;
	struct hsearch_data yield_cache;
	LIST_HEAD(ldso_cache_yield_list, elf_shared_object_node) yield_list;
	LIST_HEAD(ldso_malloc_list, elf_malloc_node) malloc_list;
} elf_shared_object_iterator_t;

/*
 * API flags for loading.
 */
#define ELF_LOAD_F_STRICT	(1UL << 0) //only load binaries if ALL headers are sane
#define ELF_LOAD_F_SMART	(1UL << 1) //(implicit flag) load any binary that the kernel can load and reconstruct
					   //--although symbols and sections won't be available... see next flag
#define ELF_LOAD_F_FORENSICS	(1UL << 2) //this flag will fully reconstruct all forensics relevant data similarly
					   //if the section header tables and symbols are missing or are corrupted.
#define ELF_LOAD_F_MODIFY	(1UL << 3) //Used for modifying binaries
#define ELF_LOAD_F_ULEXEC	(1UL << 4) //Used for ulexec based debugging API
#define ELF_LOAD_F_MAP_WRITE	(1UL << 5)
#define ELF_LOAD_F_LXC_MODE	(1UL << 6) // Used when scanning binaries within an LXC container

/*
 * Loads an ELF object of any type, for reading or modifying.
 * arg1: file path
 * arg2: ELF object handle to be filled in
 * arg3: ELF Load flags (i.e. ELF_LOAD_FLAGS_STRICT|ELF_LOAD_FLAGS_MODIFY)
 * arg4: error object handle to be filled in upon failure.
 */
bool elf_open_object(const char *path, elfobj_t *, uint64_t, elf_error_t *);
void elf_close_object(elfobj_t *);

/*
 * Returns a string containing an error message for any failed libelfmaster
 * API functions.
 */
const char * elf_error_msg(elf_error_t *);

/*
 * Fills in 'struct elf_section *'  on success.
 * Performs a binary search on sorted section headers.
 */
bool elf_section_by_name(elfobj_t *, const char *, struct elf_section *);

/*
 * Outputs index of shdr searched by name
 */
bool elf_section_index_by_name(elfobj_t *, const char *, uint64_t *);

/*
 * Fills in 'struct elf_section *' on success.
 */
bool elf_section_by_index(elfobj_t *, unsigned int index, struct elf_section *);


/*
 * ELF Section iterator
 * Iterates over each section header, filling in a struct elf_section upon each
 * iteration.
 */
void elf_section_iterator_init(elfobj_t *, elf_section_iterator_t *);
elf_iterator_res_t elf_section_iterator_next(elf_section_iterator_t *, struct elf_section *);

/*
 * ELF Segment (Program header) iterator
 * Iterates over each program header (which describe segment), filling in a struct elf_segment
 * upon each successful iteration.
 */
void elf_segment_iterator_init(elfobj_t *, elf_segment_iterator_t *);
elf_iterator_res_t elf_segment_iterator_next(elf_segment_iterator_t *, struct elf_segment *);

bool elf_note_iterator_init(elfobj_t *, elf_note_iterator_t *);
elf_iterator_res_t elf_note_iterator_next(elf_note_iterator_t *, elf_note_entry_t *,
    elf_error_t *);

void elf_dynamic_iterator_init(elfobj_t *, elf_dynamic_iterator_t *);
elf_iterator_res_t elf_dynamic_iterator_next(elf_dynamic_iterator_t *, elf_dynamic_entry_t *);

void elf_symtab_iterator_init(elfobj_t *, elf_symtab_iterator_t *);
elf_iterator_res_t elf_symtab_iterator_next(elf_symtab_iterator_t *, struct elf_symbol *);

void elf_dynsym_iterator_init(elfobj_t *, elf_dynsym_iterator_t *);
elf_iterator_res_t elf_dynsym_iterator_next(elf_dynsym_iterator_t *, struct elf_symbol *);

void elf_plt_iterator_init(elfobj_t *, elf_plt_iterator_t *);
elf_iterator_res_t elf_plt_iterator_next(elf_plt_iterator_t *, struct elf_plt *);

/*
 * Lookup a PLT entry by name. Output is stored in 3rd arg
 * elf_plt *entry
 */
bool elf_plt_by_name(elfobj_t *, const char *, struct elf_plt *);

uint64_t elf_entry_point(elfobj_t *);
uint32_t elf_type(elfobj_t *);
size_t elf_size(elfobj_t *);
uint16_t elf_machine(elfobj_t *);

bool elf_map_loadable_segments(elfobj_t *, struct elf_mapping *, elf_error_t *);

/*
 * Fill in elf_symbol, through cache lookup by name.
 */
bool elf_symbol_by_name(elfobj_t *, const char *, struct elf_symbol *);

bool elf_symbol_by_index(elfobj_t *, unsigned int, struct elf_symbol *, const int);

bool elf_symbol_by_value(elfobj_t *, uint64_t, struct elf_symbol *);

/*
 * Return a pointer to an offset into the memory mapped ELF file.
 */
void * elf_offset_pointer(elfobj_t *, uint64_t);

/*
 * Return a pointer to the section passed in arg2.
 * This function takes either a pointer to an Elf32_Shdr
 * or an Elf64_Shdr as the second argument, and then uses
 * the sh_offset field to locate the section within the ELF
 * file, then returns a pointer to it.
 */
void * elf_section_pointer(elfobj_t *, void *);

/*
 * ELF section by address
 */
bool elf_section_by_address(elfobj_t *, uint64_t, struct elf_section *);
/*
 * Success: returns section name
 * failure: returns NULL
 */
const char * elf_section_name_by_index(elfobj_t *, uint32_t);
/*
 * initialize the relocation iterator.
 */
bool elf_relocation_iterator_init(elfobj_t *, struct elf_relocation_iterator *);

/*
 * iterate over every relocation entry in the entire ELF file. The elf_relocation struct
 * is filled in upon each iteration, and it gives lots of information including which
 * section the relocation applies to, which symbol, and what type of relocation it is.
 */
bool
elf_relocation_iterator_init(struct elfobj *,
    struct elf_relocation_iterator *);

elf_iterator_res_t
elf_relocation_iterator_next(elf_relocation_iterator_t *, struct elf_relocation *);

/*
 * return pointer to string table index for ELF sections (.shstrtab)
 * dynamic symbols (.dynstr) and local symbols (.strtab)
 */
const char * elf_section_string(elfobj_t *, uint64_t);
const char * elf_dynamic_string(elfobj_t *, uint64_t);
const char * elf_symtab_string(elfobj_t *, uint64_t);

/*
 * API for iterating over an ELF files shared object dependencies
 * arg0: elfobj_t *, ptr to elf descriptor
 * arg1: elf_shared_object_iterator_t *, ptr to iterator descriptor
 * arg2: optional path to ld.so.cache file (uses /etc/ld.so.cache by default)
 * arg3: iterator flags. optional: ELF_SO_RESOLVE_F, ELF_SO_RESOLVE_ALL_F
 * arg4: error descriptor
 */
bool elf_shared_object_iterator_init(elfobj_t *,
    elf_shared_object_iterator_t *, const char *, unsigned int,
    elf_error_t *);

elf_iterator_res_t elf_shared_object_iterator_next(elf_shared_object_iterator_t *,
    struct elf_shared_object *, elf_error_t *);

/*
 * Convert phdr->p_type to string describing segment type, i.e. "DYNAMIC"
 */
const char * elf_segment_type_string(uint32_t);

/*
 * Get pointer to ELF binary location by address
 */
void * elf_address_pointer(elfobj_t *, uint64_t);

void elf_pltgot_iterator_init(elfobj_t *, elf_pltgot_iterator_t *);
elf_iterator_res_t elf_pltgot_iterator_next(elf_pltgot_iterator_t *, elf_pltgot_entry_t *);

/*
 * Iterates all function addresses and sizes associated with each eh_frame entry
 */
void elf_eh_frame_iterator_init(elfobj_t *, elf_eh_frame_iterator_t *);
elf_iterator_res_t elf_eh_frame_iterator_next(elf_eh_frame_iterator_t *, elf_eh_frame_t *);

/*
 * Get string describing the GOT entry based on elf_pltgot_entry.flags
 */
const char *
elf_pltgot_flag_string(uint32_t);

/*
 * return base address of text or data segment
 */
uint64_t elf_text_base(elfobj_t *);
uint64_t elf_data_base(elfobj_t *);
size_t elf_text_filesz(elfobj_t *);
size_t elf_data_filesz(elfobj_t *);

/*
 * return base offset of text or data segment
 */
uint64_t elf_text_offset(elfobj_t *);
uint64_t elf_data_offset(elfobj_t *);
/*
 * Return data segment phdr in struct elf_segment arg
 */
bool elf_data_segment(elfobj_t *, struct elf_segment *);
/*
 * r_type converted to string representing the relocation
 * type; i.e. "R_X86_64_JUMP_SLOT"
 */
const char *
elf_reloc_type_string(elfobj_t *, uint32_t);

bool
elf_flags(elfobj_t *, elf_obj_flags_t);

#if 0
static inline uint64_t
elf_error_code(elf_error_t e)
{

	return e._errno;
}

static inline bool
elf_error_check(elf_error_t e, uint64_t code)
{

	if (elf_error_code(e) == code)
		return true;
	return false;
}
#endif
/*
 * Get string tables
 */
static inline char *
elf_dynstr(elfobj_t *obj)
{

	return obj->dynstr;
}

static inline char *
elf_shstrtab(elfobj_t *obj)
{

	return obj->shstrtab;
}

static inline char *
elf_strtab(elfobj_t *obj)
{

	return obj->strtab;
}

/*
 * Is the ELF obj dynamically, statically linked, or neither (Meaning object or core file)
 */
typedef enum elf_linking_type {
	ELF_LINKING_DYNAMIC,
	ELF_LINKING_STATIC,
	ELF_LINKING_STATIC_PIE,
	ELF_LINKING_UNDEF /* Means its neither, such as an ET_REL or ET_CORE */
} elf_linking_type_t;

/*
 * Returns ELF_LINKING_DYNAMIC, ELF_LINKING_STATIC, or ELF_LINKING_UNDEF
 * (For core files, relocatable objects, and ET_NONE objects its undefined)
 */
elf_linking_type_t
elf_linking_type(elfobj_t *);

/*
 * Returns elfclass32 or elfclass64
 */
elf_class_t
elf_class(elfobj_t *);

/*
 * Return basename and pathname of
 * current ELF object.
 */
const char * elf_basename(elfobj_t *);
const char * elf_pathname(elfobj_t *);

typedef enum typewidth {
	ELF_DWORD,
	ELF_QWORD,
	ELF_WORD,
	ELF_BYTE
} typewidth_t;

bool elf_read_address(elfobj_t *, uint64_t, uint64_t *, typewidth_t);
bool elf_read_offset(elfobj_t *, uint64_t, uint64_t *, typewidth_t);

ssize_t elf_scop_text_filesz(elfobj_t *);
uint64_t elf_executable_text_offset(elfobj_t *);
uint64_t elf_executable_text_base(elfobj_t *);

/*
 * 2nd arg is an output of the number of entries in .symtab
 * returns true on success. Same thing for elf_dynsym_count
 */
bool elf_symtab_count(elfobj_t *, uint64_t *);
bool elf_dynsym_count(elfobj_t *, uint64_t *);

static inline size_t
elf_dtag_count(elfobj_t *obj)
{

	return obj->dtag_count;
}

static inline size_t
elf_segment_count(elfobj_t *obj)
{

	switch(obj->e_class) {
	case elfclass32:
		return obj->ehdr32->e_phnum;
	case elfclass64:
		return obj->ehdr64->e_phnum;
	}
	return 0;
}

static inline uint64_t
elf_phoff(elfobj_t *obj)
{

	switch(obj->e_class) {
	case elfclass32:
		return obj->ehdr32->e_phoff;
	case elfclass64:
		return obj->ehdr64->e_phoff;
	default:
		break;
	}
	return 0;
}

static inline uint64_t
elf_shoff(elfobj_t *obj)
{

	switch(obj->e_class) {
	case elfclass32:
		return obj->ehdr32->e_shoff;
	case elfclass64:
		return obj->ehdr64->e_shoff;
	default:
		break;
	}
	return 0;
}

static inline uint64_t
elf_shnum(elfobj_t *obj)
{

	switch(obj->e_class) {
	case elfclass32:
		return obj->ehdr32->e_shnum;
	case elfclass64:
		return obj->ehdr64->e_shnum;
	default:
		break;
	}
	return 0;
}

static inline uint16_t
elf_ehsize(elfobj_t *obj)
{

	switch(obj->e_class) {
	case elfclass32:
		return obj->ehdr32->e_ehsize;
	case elfclass64:
		return obj->ehdr64->e_ehsize;
	default:
		break;
	}
	return 0;
}

static inline size_t
elf_section_count(elfobj_t *obj)
{

	return obj->section_count;
}

ssize_t elf_phdr_table_size(elfobj_t *);
size_t elf_ehdr_size(elfobj_t *);

/*
 * Modify an elf_segment entry
 */
bool elf_segment_by_index(elfobj_t *, uint64_t, struct elf_segment *);
/*
 * Will find the first ocurrence of a given p_type
 */
bool elf_segment_by_p_type(elfobj_t *, uint64_t, struct elf_segment *);
/*
 * Write accessor functions.
 */
bool elf_symtab_modify(elfobj_t *, uint64_t index, struct elf_symbol *, elf_error_t *);
bool elf_dynsym_modify(elfobj_t *, uint64_t index, struct elf_symbol *, elf_error_t *);
bool elf_segment_modify(elfobj_t *, uint64_t index, struct elf_segment *, elf_error_t *);
bool elf_section_modify(elfobj_t *, uint64_t index, struct elf_section *, elf_error_t *);
bool elf_dynamic_modify(elfobj_t *, uint64_t index, struct elf_dynamic_entry *,
    bool, elf_error_t *);
/*
 * Must be used after elf_symtab_modify/elf_dynsym_modify, and cannot be used within calls
 * elf_symtab_iterator_next/elf_dynsym_iterator_next since a commit would change the linked
 * list that the symbol table iterator functions use.
 */
bool elf_symtab_commit(elfobj_t *);
bool elf_dynsym_commit(elfobj_t *);

/*
 * We could have put this commit function directly into the code for elf_section_modify but it
 * (thus not needing an elf_section_commit function) but it is computationally expensive if you
 * are modifying more than one section, so its best to modify N sections, and then have a commit
 * function call that updates the internal section header table only once. Every commit requires
 * freeing the existing internal representation and then sorting a new array of strings.
 */
bool elf_section_commit(elfobj_t *);

/*
 * Set the environment variable that libelfmaster reads to get
 * the LXC containers rootfs in order to resolve shared libaries
 * from this location internally.
 */
bool elf_lxc_set_rootfs(elfobj_t *, const char *);
bool elf_lxc_get_rootfs(elfobj_t *, char *, const size_t);

#endif
