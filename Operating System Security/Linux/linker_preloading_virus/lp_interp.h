#pragma once
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <search.h>
#include <sys/time.h>
#include <stdbool.h>
#include <elf.h>
#include <errno.h>

#include "libelfmaster.h"
#include "queue.h"

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif
#define INTERPX_F_LOAD_STRIPPED_ELF	(1 << 0)
#define INTERPX_F_DEBUG_MODE		(1 << 1)

#define ELF_MIN_ALIGN 4096
#define ELF_PAGESTART(_v) ((_v) & ~(unsigned long)(ELF_MIN_ALIGN-1))
#define ELF_PAGEOFFSET(_v) ((_v) & (ELF_MIN_ALIGN-1))
#define ELF_PAGEALIGN(_v, _a) (((_v) + _a - 1) & ~(_a - 1))

#define EVIL_LINKER_PATH "/lib64/evil.ld"
#define LDSO_PATH "/lib64/ld-linux-x86-64.so.2"

typedef struct lp_auxv_iterator {
	unsigned int index;
	struct lp_ctx *ctx;
	Elf64_auxv_t *auxv;
} lp_auxv_iterator_t;

typedef struct lp_auxv_entry {
	uint64_t value;
	int type;
	char *string;
} lp_auxv_entry_t;

typedef struct lp_elf {
	ElfW(Ehdr) *ehdr;
	ElfW(Phdr) *phdr;
	ElfW(Dyn) *dyn;
	ElfW(Sym) *dynsym;
	uint64_t entry_point;
	char interp_path[PATH_MAX + 1];
} lp_elf_t;

typedef struct lp_proc {
	uint64_t pid;
	uint64_t ppid;
	uint64_t gid;
	uint64_t egid;
	uint64_t uid;
	uint64_t euid;
	uint64_t phdr_vaddr;
	uint64_t entry_vaddr;
	uint64_t base_vaddr;
	uint64_t canary_value;
	uint64_t phdr_count;
	char *exec_proc_path; // /proc/pid/exe
	char *exec_path;      // filepath of ELF exe
	char *exec_file;      // filename of ELF exe
	uint32_t platform;
	uint64_t secure;
	uint64_t pagesz;
	uint64_t random;
	uint64_t interp_base;
	uint64_t flags;
	uint64_t entrypoint;
} lp_proc_t;

typedef struct lp_ctx {
	char **envp;
	char *lp_script;
	uint64_t flag;
	uint64_t mode;
	struct lp_proc proc;
	elfobj_t elfobj; // ELF parsing for /proc/pid/exe
	elfobj_t ldsobj; // ELF parsing for /lib64/linux-ld.so
	char ldso_path[PATH_MAX + 1];
} lp_ctx_t;

typedef enum lp_iterator_res {
	LP_ITER_OK = 0,
	LP_ITER_DONE,
	LP_ITER_ERROR
} lp_iterator_res_t;

typedef enum lp_section_map_attr {
	LP_SECTION_TEXTSEGMENT = 0,
	LP_SECTION_DATASEGMENT,
	LP_SECTION_UNKNOWN
} lp_section_map_attr_t;

struct lp_section_mapping {
	struct elf_section section;
	lp_section_map_attr_t map_attribute;
	uint64_t vaddr; /* Which memory address the section contents is placed in */
	uint64_t offset;
	uint64_t size;
	char *name;
	TAILQ_ENTRY(lp_section_mapping) _linkage;
};

#define MAX_PLT_COUNT 4096

struct lp_plt_entry {
	char *symname;
	uint64_t vaddr;
	size_t offset;
	TAILQ_ENTRY(lp_plt_entry) _linkage;
};

struct evil_linker {
        int fd;
        uint8_t *text_mem;
        uint8_t *data_mem; /* Includes .bss */
        uintptr_t *pltgot;
        uintptr_t *plt;
	size_t pltgot_size;
	size_t plt_size;
	size_t plt_off;
	size_t plt_count;
	size_t pltgot_off;
        size_t text_size;
        size_t data_size;
        uint64_t text_vaddr;
        uint64_t data_vaddr;
        elfobj_t elfobj;
	struct {
		TAILQ_HEAD(, lp_section_mapping) section_maplist;
		TAILQ_HEAD(, lp_plt_entry) plt_list;
	} tailq;
	struct {
		struct hsearch_data plt;
	} cache;
};

/*
 * lp_iter.c
 */
bool lp_auxv_iterator_init(struct lp_ctx *, struct lp_auxv_iterator *);
lp_iterator_res_t lp_auxv_iterator_next(struct lp_auxv_iterator *, struct lp_auxv_entry *);
bool lp_auxv_set_value(struct lp_auxv_iterator *, long);
/*
 * lp_proc.c
 */
bool lp_set_procinfo(struct lp_ctx *);

/*
 * lp_util.c
 */
char * lp_strdup(const char *);

/*
 * lp_parse_elf.c
 */
bool lp_parse_elf(struct lp_ctx *);

/*
 * lp_load_ldso.c
 */
bool lp_load_elf_interp(struct lp_ctx *, uint64_t *);

/*
 * lp_loader.c
 */
bool
lp_evil_loader(const char *, struct evil_linker **);
