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


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <search.h>

#include "libelfmaster.h"
#include "internal.h"
#include "dwarf.h"
#include "misc.h"

static char *
ldso_strdup(struct elf_shared_object_iterator *, const char *);

bool
check_static_pie(struct elfobj *elfobj)
{
	elf_segment_iterator_t p_iter;
	struct elf_segment segment;
	struct elf_section shdr;
	elf_iterator_res_t ires;
	bool has_dynamic_segment = false;
	bool has_interp_segment = false;

	/*
	 * A binary built with gcc -static-pie is not going to
	 * have a .plt section, even though it has many other
	 * characteristics of a dynamically linked object.
	 */
	if (elf_section_by_name(elfobj, ".plt", &shdr) == true)
		return false;
	/*
	 * A gcc -static-pie binary is of type ET_DYN since
	 * it is PIC code.
	 */
	if (elf_type(elfobj) != ET_DYN)
		return false;

	elf_segment_iterator_init(elfobj, &p_iter);
	for (;;) {
		ires = elf_segment_iterator_next(&p_iter, &segment);
		if (ires == ELF_ITER_DONE)
			break;
		if (ires == ELF_ITER_ERROR)
			return false;
		if (segment.type == PT_INTERP)
			has_interp_segment = true;
		if (segment.type == PT_DYNAMIC)
			has_dynamic_segment = true;
	}
	if (has_interp_segment == false && has_dynamic_segment == true) {
		/*
		 * NO interpreter, but yes there is a dynamic segment--
		 * yet there is no .plt section, and this is of type ET_DYN
		 * so yes its a -static-pie executable.
		 */
		return true;
	} else if (has_interp_segment == false & has_dynamic_segment == false) {
		/*
		 * Pay attention to other static PIE types, including
		 * those that ElfMaster (me) published in June of 2019 on
		 * making statically linked executables PIE, in which case
		 * there is no dynamic segment.
		 */
		return true;
	}
	return false;
}

/*
 * TODO Why is this defined in internal.c?
 */
bool
elf_error_set(elf_error_t *error, const char *fmt, ...)
{
	va_list va;

	if (error == NULL)
		return false;

	va_start(va, fmt);
	vsnprintf(error->string, sizeof(error->string), fmt, va);
	va_end(va);
	error->_errno = errno;
	return false;
}

int
section_name_cmp(const void *p0, const void *p1)
{
	const struct elf_section *s0 = *(void **)p0;
	const struct elf_section *s1 = *(void **)p1;

	return strcmp(s0->name, s1->name);
}

/*
 * Get the toplevel basenames from DT_NEEDED in the
 * dynamic segment. This is fast since it just uses
 * the linked list cache.
 */
bool
verify_so_is_toplevel(const char *basename, elfobj_t *obj)
{
	elf_shared_object_iterator_t so_iter;
	elf_error_t error;
	struct elf_shared_object entry;

	elf_shared_object_iterator_init(obj, &so_iter, NULL, 0, &error);
	while (elf_shared_object_iterator_next(&so_iter, &entry, &error)
	    == ELF_ITER_OK) {
		if (strcmp(basename, entry.basename) == 0) {
			return true;
		}
	}
	return false;
}

char *
ldd_parse_line(struct elf_shared_object_iterator *iter)
{
	char cmd[PATH_MAX], buf[PATH_MAX * 2];
	char *p, *ep;
	const char *env = "LD_TRACE_LOADED_OBJECTS=1";

	if (iter->index++ == 0) {
		iter->chunk = malloc(PATH_MAX * 2);
		if (iter->chunk == NULL)
			return NULL;
		snprintf(cmd, sizeof(cmd), "%s %s",
		    env, iter->obj->path);
		iter->pd = popen(cmd, "r");
		if (iter->pd == NULL)
			return NULL;
	}
	while (fgets(buf, sizeof(buf), iter->pd) != NULL) {
		p = strchr(buf, '/');
		if (p == NULL) {
			for (p = buf; *p == '\t'; p++)
				;
			ep = strchr(p, ' ');
			if (ep == NULL)
				return NULL;
			*ep = '\0';
			strncpy(iter->chunk, p, PATH_MAX * 2);
			iter->chunk[(PATH_MAX * 2) - 1] = '\0';
			return iter->chunk;
		}
		ep = strchr(p, ' ');
		if (ep == NULL)
			return NULL;
		*ep = '\0';
		memset(iter->chunk, 0, PATH_MAX * 2);
		strncpy(iter->chunk, p, PATH_MAX * 2);
		iter->chunk[(PATH_MAX * 2) - 1] = '\0';
		return iter->chunk;
	}
	return NULL;
}
/*
 * Same for x86 and i386
 */
#define ELF_RELOC_JUMP_SLOT 7

bool
build_plt_data(struct elfobj *obj)
{
	ENTRY e, *ep;
	struct elf_section plt;
	struct elf_relocation_iterator r_iter;
	struct elf_relocation r_entry;
	struct elf_plt_node *plt_node;
	uint64_t plt_addr;
	elf_iterator_res_t res;

	if (elf_section_by_name(obj, ".plt", &plt) == false) {
		return false;
	}
	/*
	 * We can use the relocation iterator at this point, since all of its
	 * necessary components have been set already within elfobj *
	 */
	if (elf_relocation_iterator_init(obj, &r_iter) == false) {
		printf("elf_relocation_iterator_init\n");
		return false;
	}

	plt_node = malloc(sizeof(*plt_node));
	if (plt_node == NULL)
		return false;

	/*
	 * First PLT entry is always PLT-0, even though objdump always
	 * names it with same symbol name as the next entry.
	 */
	plt_node->addr = plt.address;
	plt_node->symname = (char *)"PLT-0";
	LIST_INSERT_HEAD(&obj->list.plt, plt_node, _linkage);

	/*
	 * Also hash the PLT entries by symbol name.
	 */
	e.key = (char *)plt_node->symname;
	e.data = (void *)plt_node;
	hsearch_r(e, ENTER, &ep, &obj->cache.plt);
	plt_addr = plt.address + plt.entsize;

	for (;;) {
		res = elf_relocation_iterator_next(&r_iter, &r_entry);
		if (res == ELF_ITER_ERROR)
			return false;
		if (res == ELF_ITER_DONE)
			break;
		if (r_entry.type != ELF_RELOC_JUMP_SLOT)
			continue;
		plt_node = malloc(sizeof(*plt_node));
		if (plt_node == NULL)
			return false;
		plt_node->addr = plt_addr;
		plt_node->symname = r_entry.symname;
		LIST_INSERT_HEAD(&obj->list.plt, plt_node, _linkage);
		e.key = (char *)plt_node->symname;
		e.data = (void *)plt_node;
		hsearch_r(e, ENTER, &ep, &obj->cache.plt);
		plt_addr += plt.entsize;
	}
	return true;
}

bool
build_dynsym_data(struct elfobj *obj)
{
	ENTRY e, *ep;
	unsigned int i;
	Elf32_Sym *dsym32;
	Elf64_Sym *dsym64;
	struct elf_dynsym_list *list = &obj->list.dynsym;

	LIST_INIT(&obj->list.dynsym);

	for (i = 0; i < obj->dynsym_count; i++) {
		struct elf_symbol_node *symbol = malloc(sizeof(*symbol));

		if (symbol == NULL)
			return false;

		switch(obj->e_class) {
		case elfclass32:
			dsym32 = obj->dynsym32;
			symbol->name = &obj->dynstr[dsym32[i].st_name];
			symbol->value = dsym32[i].st_value;
			symbol->shndx = dsym32[i].st_shndx;
			symbol->size = dsym32[i].st_size;
			symbol->bind = ELF32_ST_BIND(dsym32[i].st_info);
			symbol->type = ELF32_ST_TYPE(dsym32[i].st_info);
			symbol->visibility = ELF32_ST_VISIBILITY(dsym32[i].st_other);
			break;
		case elfclass64:
			dsym64 = obj->dynsym64;
			symbol->name = &obj->dynstr[dsym64[i].st_name];
			symbol->value = dsym64[i].st_value;
			symbol->shndx = dsym64[i].st_shndx;
			symbol->size = dsym64[i].st_size;
			symbol->bind = ELF64_ST_BIND(dsym64[i].st_info);
			symbol->type = ELF64_ST_TYPE(dsym64[i].st_info);
			symbol->visibility = ELF64_ST_VISIBILITY(dsym64[i].st_other);
			break;
		}
		e.key = (char *)symbol->name;
		e.data = (void *)symbol;
		hsearch_r(e, ENTER, &ep, &obj->cache.dynsym);
		if (ep == NULL && errno == ENOMEM)
			return false;
		/*
		 * We also maintain a linked list for the iterator
		 */
		LIST_INSERT_HEAD(list, symbol, _linkage);
	}
	return true;
}

static bool
sanity_check_st_name(struct elfobj *obj, size_t offset)
{

	(void)obj;
	(void)offset;

	/*
	 * XXX TODO
	 */
	return true;
}

bool
build_symtab_data(struct elfobj *obj)
{
	ENTRY e, *ep;
	unsigned int i;
	Elf32_Sym *symtab32;
	Elf64_Sym *symtab64;
	struct elf_symtab_list *list = &obj->list.symtab;

	LIST_INIT(&obj->list.symtab);
	/*
	 * Since there is no .symtab in the binary we are only re-creating
	 * the symbol entries internally to libelfmaster (Within memory)
	 * and can reconstruct the address and size of the local functions
	 * that correspond to each FDE within .eh_frame.
	 */
	if ((obj->load_flags & ELF_LOAD_F_FORENSICS) &&
	    insane_section_headers(obj) == true) {
		struct elf_symbol_node *symbol;
		struct elf_eh_frame fde;
		elf_eh_frame_iterator_t  eh_iter;
		int ret;

		elf_eh_frame_iterator_init(obj, &eh_iter);
		while (elf_eh_frame_iterator_next(&eh_iter, &fde) == ELF_ITER_OK) {
			char *name;

			symbol = malloc(sizeof(*symbol));
			if (symbol == NULL)
				return false;
			ret = asprintf(&name, "sub_%llx", fde.pc_begin);
			if (ret < 0) {
				perror("asprintf");
				return false;
			}
			symbol->name = (const char *)name;
			symbol->value = fde.pc_begin;
			symbol->shndx = 0; // TODO relate this to one of the internally reconstructed section indexes
			symbol->size = fde.len;
			symbol->bind = STB_LOCAL;
			symbol->type = STT_FUNC;
			symbol->visibility = STV_DEFAULT;
			e.key = (char *)symbol->name;
			e.data = (char *)symbol;
			hsearch_r(e, ENTER, &ep, &obj->cache.symtab);
			if (ep == NULL && errno == ENOMEM) {
				perror("hsearch_r");
				return false;
			}
			LIST_INSERT_HEAD(list, symbol, _linkage);
		}
		return true;
	}
	/*
	 * If we made it here than the target object already has a symbol table
	 * that we are simply storing internally.
	 */
	for (i = 0; i < obj->symtab_count; i++) {
		struct elf_symbol_node *symbol = malloc(sizeof(*symbol));

		if (symbol == NULL)
			return false;

		switch(obj->e_class) {
		case elfclass32:
			/*
			 * NOTE:
			 * libelfmaster takes the position of assigning symbol names
			 * to symbols with out of bounds st_name, that will denote that
			 * there is an issue with the symbol->st_name string table index.
			 */
			symtab32 = obj->symtab32;
			if (elf_type(obj) == ET_REL) {
				if (symtab32[i].st_name < elf_size(obj)) {
					symbol->name = &obj->strtab[symtab32[i].st_name];
				} else {
					symbol->name = "invalid_name_index";
				}
			} else if (elf_type(obj) == ET_DYN || elf_type(obj) == ET_EXEC) {
				if (symtab32[i].st_name < elf_data_offset(obj) +
				    elf_data_filesz(obj)) {
					symbol->name = &obj->strtab[symtab32[i].st_name];
				} else {
					symbol->name = "invalid_name_index";
				}
			}
			symbol->value = symtab32[i].st_value;
			symbol->shndx = symtab32[i].st_shndx;
			symbol->size = symtab32[i].st_size;
			symbol->bind = ELF32_ST_BIND(symtab32[i].st_info);
			symbol->type = ELF32_ST_TYPE(symtab32[i].st_info);
			symbol->visibility = ELF32_ST_VISIBILITY(symtab32[i].st_other);
			break;
		case elfclass64:
			symtab64 = obj->symtab64;
			if (elf_type(obj) == ET_REL) {
				if (symtab64[i].st_name < elf_size(obj)) {
					symbol->name = &obj->strtab[symtab64[i].st_name];
				} else {
					symbol->name = "invalid_name_index";
				}
			} else if (elf_type(obj) == ET_DYN || elf_type(obj) == ET_EXEC) {
					if (sanity_check_st_name(obj, symtab64[i].st_name) == true)
						symbol->name = &obj->strtab[symtab64[i].st_name];
					else
						symbol->name = "invalid_name_index";
			}
			symbol->value = symtab64[i].st_value;
			symbol->shndx = symtab64[i].st_shndx;
			symbol->size = symtab64[i].st_size;
			symbol->bind = ELF64_ST_BIND(symtab64[i].st_info);
			symbol->type = ELF64_ST_TYPE(symtab64[i].st_info);
			symbol->visibility = ELF64_ST_VISIBILITY(symtab64[i].st_other);
			break;
		}
		e.key = (char *)symbol->name;
		e.data = (char *)symbol;
		hsearch_r(e, ENTER, &ep, &obj->cache.symtab);
		if (ep == NULL && errno == ENOMEM)
			return false;
		/*
		 * We also maintain a linked list for the iterator
		 */
		LIST_INSERT_HEAD(list, symbol, _linkage);
	}
	return true;
}

/*
 * Compares libraries by version numbers, and returns 0
 * on equal.
 */
static int
ldso_cache_cmp(const char *p1, const char *p2)
{
	while (*p1) {
		if (isdigit(*p1) && isdigit(*p2)) {
			int v1, v2;

			v1 = strtoul(p1, (char **)&p1, 10);
			v2 = strtoul(p2, (char **)&p2, 10);
			if (v1 != v2)
				return v1 - v2;
		}
		else if (isdigit(*p1) && !isdigit(*p2)) {
			return 1;
		} else if (!isdigit(*p1) && isdigit(*p2)) {
			return -1;
		} else if (*p1 != *p2) {
			return *p1 - *p2;
		} else {
			p1++, p2++;
		}
	}
	return *p1 - *p2;
}

#define ldso_cache_verify_offset(offset) (offset < iter->cache_size)

static inline bool
ldso_cache_check_flags(struct elf_shared_object_iterator *iter,
    uint32_t flags)
{
	if (iter->obj->arch == i386) {
		if (flags == 0x803)
			return true;
	} else if (iter->obj->arch == x64) {
		if (flags == 0x303)
			return true;
	}
	return false;
}

const char *
ldso_cache_bsearch(struct elf_shared_object_iterator *iter,
    const char *name)
{
	int ret;
	uint64_t value;
	uint32_t middle, flags;
	uint32_t left = 0;
	uint32_t right = (iter->cache_flags & ELF_LDSO_CACHE_NEW) ?
	    iter->cache_new->nlibs - 1 : iter->cache->nlibs - 1;
	const char *best = NULL;

	while (left <= right) {
		uint32_t key;

		middle = (left + right) / 2;
		if (iter->cache_flags & ELF_LDSO_CACHE_NEW) {
			key = iter->cache_new->libs[middle].key;
		} else {
			key = iter->cache->libs[middle].key;
		}
		ret = ldso_cache_cmp(name, iter->cache_data + key);
		if (ret == 0) {
			left = middle;
			while (middle > 0) {
				if (iter->cache_flags & ELF_LDSO_CACHE_NEW) {
					key = iter->cache_new->libs[middle - 1].key;
				} else {
					key = iter->cache->libs[middle - 1].key;
				}
				if (ldso_cache_cmp(name,
				    iter->cache_data + key) != 0) {
					break;
				}
				--middle;
			}
			do {
				uint32_t new_key;

				if (iter->cache_flags & ELF_LDSO_CACHE_NEW) {
					new_key = iter->cache_new->libs[middle].key;
					value = iter->cache_new->libs[middle].value;
					flags = iter->cache_new->libs[middle].flags;
				} else {
					new_key = iter->cache->libs[middle].key;
					value = iter->cache->libs[middle].value;
					flags = iter->cache->libs[middle].flags;
				}
				if (middle > left && (ldso_cache_cmp(name,
				    iter->cache_data + new_key) != 0))
					break;
				if (ldso_cache_check_flags(iter, flags) &&
				    ldso_cache_verify_offset(value)) {
					if (best == NULL) {
						best = iter->cache_data + value;
						break;
					}
				}
			} while (++middle <= right);
			break;
		}
		if (ret < 0) {
			left = middle + 1;
		} else {
			right = middle - 1;
		}
	}

	if (iter->obj->load_flags & ELF_LOAD_F_LXC_MODE) {
		char lxc_path[PATH_MAX];

		if (elf_lxc_get_rootfs(iter->obj, lxc_path, PATH_MAX / 2) == false) {
			if (strlen(lxc_path) + strlen(best) >= PATH_MAX)
				return false;
		}
		strcat(lxc_path, best);
		best = ldso_strdup(iter, lxc_path);
	}

	return best;
}

/*
e* ldso_malloc allows us to maintain a linked list of heap
 * allocations used for the shared object path names, this way
 * we can keep them available in the cache throughout the duration
 * of the entire iterator, rather than free them once we yield
 * them. This is necessary in order to make sure we don't yield
 * duplicates.
 */
static void *
ldso_malloc(struct elf_shared_object_iterator *iter, size_t size)
{
	void *p;
	struct elf_malloc_node *n;

	p = malloc(size);
	if (p == NULL)
		return NULL;
	n = malloc(sizeof(*n));
	if (n == NULL)
		return NULL;
	n->ptr = p;
	LIST_INSERT_HEAD(&iter->malloc_list, n, _linkage);
	return n->ptr;
}

static char *
ldso_strdup(struct elf_shared_object_iterator *iter, const char *s)
{

	char *string;

	string = ldso_malloc(iter, strlen(s) + 1);
	if (string == NULL)
		return NULL;
	strcpy(string, s);
	return string;
}

void
ldso_free_malloc_list(struct elf_shared_object_iterator *iter)
{
	struct elf_malloc_node *next, *current;

	LIST_FOREACH_SAFE(current, &iter->malloc_list, _linkage, next) {
		free(current->ptr);
		free(current);
	}
	return;
}

void
ldso_cleanup(struct elf_shared_object_iterator *iter)
{

	ldso_free_malloc_list(iter);
	if (iter->flags & ELF_SO_RESOLVE_ALL_F)
		hdestroy_r(&iter->yield_cache);
	(void) munmap(iter->mem, iter->st.st_size);
}

/*
 * We add shared objects to the yield list. Since there will be duplicates we
 * also maintain a hash table of entries so we know which ones we already have
 * in the list, without having to traverse the list.
 */
static bool
ldso_insert_yield_entry(struct elf_shared_object_iterator *iter,
    const char *path)
{
	struct elf_shared_object_node *so = malloc(sizeof(*so));
	ENTRY e = {(char *)path, (char *)path}, *ep;

	if (so == NULL)
		return false;
	/*
	 * If we find the item in the cache then don't add it
	 * to the list again.
	 */
	if (hsearch_r(e, FIND, &ep, &iter->yield_cache) != 0) {
		free(so);
		return true;
	}
	/*
	 * Add path to cache.
	 */
	if (hsearch_r(e, ENTER, &ep, &iter->yield_cache) == 0)
		return false;
	/*
	 * Add path to yield list.
	 */
	so->path = (char *)path;
	so->basename = strrchr(path, '/') + 1;
	LIST_INSERT_HEAD(&iter->yield_list, so, _linkage);
	iter->yield = LIST_FIRST(&iter->yield_list);
	return true;
}

/*
 * Inserts path into yield cache, but not into the yield list.
 * this is necessary when the iterator yields top-level paths
 * to make sure they don't end up in the yield list and therefore
 * get yielded to the user as a duplicate.
 */
bool
ldso_insert_yield_cache(struct elf_shared_object_iterator *iter,
    const char *path)
{
	ENTRY e = {(char *)path, (char *)path}, *ep;

	if (hsearch_r(e, FIND, &ep, &iter->yield_cache) != 0)
		return true;
	if (hsearch_r(e, ENTER, &ep, &iter->yield_cache) == 0)
		return false;
	return true;
}

bool
ldso_recursive_cache_resolve(struct elf_shared_object_iterator *iter,
    const char *bname)
{
	const char *path = ldso_cache_bsearch(iter, bname);
	struct elf_shared_object_node *current;
	elfobj_t obj;
	elf_error_t error;

	if (path == NULL) {
		return true;
	}
	if (elf_open_object(path, &obj, false, &error) == false) {
		return false;
	}
	if (LIST_EMPTY(&obj.list.shared_objects))
		goto done;

	LIST_FOREACH(current, &obj.list.shared_objects, _linkage) {
		if (current->basename == NULL) {
			goto err;
		}
		path = (char *)ldso_cache_bsearch(iter, current->basename);
		if (path == NULL) {
			DEBUG_LOG("cannot resolve %s\n", current->basename);
			goto err;
		}
		/*
		 * We update the existing object list to now contain the
		 * full path. That way any subsequent calls to the shared
		 * object iterator will use the linked list cache.
		 */
		current->path = ldso_strdup(iter, path);
		if (current->path == NULL)
			goto err;
		if (ldso_insert_yield_entry(iter, current->path) == false) {
			goto err;
		}
		if (ldso_recursive_cache_resolve(iter, current->basename) == false){
			goto err;
		}
	}
done:
	elf_close_object(&obj);
	return true;
err:
	elf_close_object(&obj);
	return false;
}

/*
 * Used internally to build information from the dynamic segment.
 */
bool
load_dynamic_segment_data(struct elfobj *obj)
{
	struct elf_dynamic_entry entry;
	elf_dynamic_iterator_t iter;
	elf_iterator_res_t res;
	struct elf_shared_object_node *so;
	uint32_t dt_pltgot = 0, dt_pltrelsz = 0, dt_symtab = 0,
	    dt_strtab = 0, dt_strsz = 0, dt_hash = 0, dt_pltrel = 0,
	    dt_jmprel = 0, dt_rela = 0, dt_relasz = 0, dt_rel = 0, dt_relsz = 0,
	    dt_fini = 0, dt_init = 0, dt_relent = 0, dt_relaent = 0,
	    dt_init_array = 0, dt_init_arraysz = 0,
	    dt_fini_array = 0, dt_fini_arraysz = 0, dt_debug = 0;
	uint32_t ptr_width = elf_class(obj) == elfclass32 ? 4 : 8;
	/*
	 * If the ELF object has no section headers, then .dynstr won't be set
	 * yet, and elf_dynamic_string() will fail. So before we use the
	 * dynamic iterator to set all dynamic segment values, we must first
	 * manually find the location of dynstr and set it. Its somewhat
	 * redundant, but its a quick and simple fix. After that we can
	 * call elf_dynamic_string() as necessary.
	 */
	elf_dynamic_iterator_init(obj, &iter);
	for (;;) {
		res = elf_dynamic_iterator_next(&iter, &entry);
		if (res == ELF_ITER_DONE)
			break;
		if (res == ELF_ITER_ERROR) {
			fprintf(stderr, "Initial iteration over dynamic segment failed\n");
			return false;
		}
		if (entry.tag != DT_STRTAB)
			continue;
		/*
		 * we must handle anomalies where .dynstr
		 * is not stored in the text segment. I've seen this before
		 * with strange linker script configs where .dynstr is writable
		 * and in the data segment. For now return false if .dynstr is
		 * not in the text segment and we are performing forensics
		 * reconstruction. We must also adjust elf_data_base and elf_text_base
		 * to account for SCOP binaries.
		 */
		if (entry.value >=
		    elf_text_base(obj) + elf_text_filesz(obj)) {
			if (entry.value >= elf_data_base(obj) &&
			    entry.value < elf_data_base(obj) + elf_data_filesz(obj)) {
				obj->dynstr = (char *)&obj->mem[entry.value -
				    elf_data_base(obj)];
				if (obj->dynstr == NULL)
					return false;
			} else {
				fprintf(stderr,
				    ".dynstr points outside of text and data segment\n");
				return false;
			}
		}
		obj->dynstr = (char *)&obj->mem[entry.value - elf_text_base(obj)];
		if (obj->dynstr == NULL)
			return false;
	}
	LIST_INIT(&obj->list.shared_objects);
	elf_dynamic_iterator_init(obj, &iter);
	for (;;) {
		res = elf_dynamic_iterator_next(&iter, &entry);
		if (res == ELF_ITER_DONE)
			return true;
		if (res == ELF_ITER_ERROR) {
			fprintf(stderr, "Second iteration over dynamic segment failed\n");
			return false;
		}
		obj->dynseg.exists = true;
		/*
		 * SECURITY: Some of these tags are expected more
		 * than once:
		 * like DT_NEEDED. But an attacker who wants to
		 * circumvent our reconstruction could put two
		 * DT_PLTGOT tags for instance and we would save
		 * the second one as the PLT/GOT address, and it
		 * could be bunk. So lets make sure there's only
		 * one of each unless it expected otherwise (such as NEEDED)
		 * Eventually lets make sure to do further validation
		 *
		 * SECURITY TODO: Currently we do boundary checks on each value
		 * and make sure that certain vital values are sane. This
		 * is imperative for forensics reconstruction. There is a
		 * fundamental problem with our approach that needs to be
		 * addressed later on. We are assuming that these tags point
		 * to values either in the traditional text segment or the
		 * traditional data segment. Consider a scenario where the
		 * DT_PLTGOT is in a loadable segment that is not determined
		 * to be the data segment, yet we are using elf_data_base(obj)
		 * to calculate sanity checks. This is important since we
		 * need to be able to reconstruct the data segment, however
		 * we may advance our technique in the future to support
		 * reconstruction, even if a given tag value points to a
		 * non-traditional memory range, as long as that segment
		 * permissions are in alignment with the dynamic tag value
		 * then we could respect it. This enhancement will ultimately
		 * be spec'd out and designed properly and documented based
		 * on more analysis and samples.
		 */
		obj->dtag_count++;
		switch(entry.tag) {
		case DT_PLTGOT:
			if (dt_pltgot++ > 0)
				break;
			if (obj->dynseg.pltgot.addr < elf_data_base(obj) ||
			    obj->dynseg.pltgot.addr > elf_data_base(obj) +
			    elf_data_filesz(obj) - ptr_width - 1) {
				obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			}
			obj->dynseg.pltgot.addr = entry.value;
			break;
		case DT_PLTRELSZ:
			if (dt_pltrelsz++ > 0)
				break;
			if (entry.value > obj->size - 1)
				obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			obj->dynseg.pltrel.size = entry.value;
			break;
		case DT_SYMTAB:
			if (dt_symtab++ > 0)
				break;
			if (entry.value < elf_text_base(obj) ||
			    entry.value > elf_text_base(obj) +
			    elf_text_filesz(obj) - ptr_width - 1) {
				obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			}
			obj->dynseg.dynsym.addr = entry.value;
			break;
		case DT_STRTAB:
			if (dt_strtab++ > 0)
				break;
			if (entry.value < elf_text_base(obj) ||
			    entry.value > elf_text_base(obj) +
			    elf_text_filesz(obj) - 1) {
				obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			}
			obj->dynseg.dynstr.addr = entry.value;
			break;
		case DT_STRSZ:
			if (dt_strsz++ > 0)
				break;
			if (entry.value >= obj->size)
				obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			obj->dynseg.dynstr.size = entry.value;
			break;
		case DT_HASH:
		case DT_GNU_HASH:
			if (dt_hash++ > 0)
				break;
			if (entry.value < elf_text_base(obj) ||
			    entry.value > elf_text_base(obj) +
			    elf_text_filesz(obj) - ptr_width - 1) {
				obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			}
			obj->dynseg.hash.addr = entry.value;
			break;
		case DT_PLTREL:
			if (dt_pltrel++ > 0)
				break;
			if (entry.value != ELF_DT_PLTREL_RELA &&
			    entry.value != ELF_DT_PLTREL_RELA) {
				obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			}
			obj->flags |= ELF_PLT_RELOCS_F;
			obj->dynseg.pltrel.type = entry.value;
			break;
		case DT_JMPREL:
			if (dt_jmprel++ > 0)
				break;
			if (entry.value < elf_text_base(obj) ||
			    entry.value > elf_text_base(obj) +
			    elf_text_filesz(obj) - ptr_width - 1) {
				obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			}
			obj->dynseg.pltrel.addr = entry.value;
			break;
		case DT_RELA:
			if (dt_rela++ > 0)
				break;
			if (entry.value < elf_text_base(obj) ||
			    entry.value > elf_text_base(obj) +
			    elf_text_filesz(obj) - ptr_width - 1) {
				obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			}
			obj->dynseg.rela.addr = entry.value;
			break;
		case DT_RELASZ:
			if (dt_relasz++ > 0)
				break;
			/*
			 * If we haven't hit DT_RELA yet, then we
			 * cannot properly calculate an invalid size
			 * since we won't have the exact location
			 * of the relocation section yet. In that
			 * case we do an approximation of whether the
			 * size is 'likely' or 'approximately' sane
			 */
			if (obj->dynseg.rela.addr == 0) {
				/*
				 * Approximate check
				 */
				if (entry.value > elf_text_offset(obj) +
				    elf_text_filesz(obj) - 1) {
					obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
				}
			} else {
				if (obj->dynseg.rela.addr + entry.value >
				    elf_text_base(obj) + elf_text_filesz(obj) - 1) {
					obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
				}
			}
			obj->dynseg.rela.size = entry.value;
			break;
		case DT_REL:
			if (dt_rel++ > 0)
				break;
			if (entry.value < elf_text_base(obj) ||
			    entry.value > elf_text_base(obj) +
			    elf_text_filesz(obj) - ptr_width - 1) {
				obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			}
			obj->dynseg.rel.addr = entry.value;
			break;
		case DT_RELSZ:
			if (dt_relsz++ > 0)
				break;
			/*
			 * Same logic as how we handle DT_RELASZ, read
			 * comments in case DT_RELASZ
			 */
			if (obj->dynseg.rela.addr == 0) {
				/*
				 * Approximate check
				 */
				if (entry.value > elf_text_offset(obj) +
				    elf_text_filesz(obj) - 1) {
					obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
				}
			} else {
				if (obj->dynseg.rel.addr + entry.value >
				    elf_text_base(obj) + elf_text_filesz(obj) - 1) {
					obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
				}
			}
			obj->dynseg.rel.size = entry.value;
			break;
		case DT_INIT:
			if (dt_init++ > 0)
				break;
			/*
			 * Approximate guess since we don't yet know the
			 * size of .init
			 */
			if (entry.value < elf_text_base(obj) ||
			    entry.value > elf_text_base(obj) +
			    elf_text_filesz(obj) - 1) {
				obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			}
			obj->dynseg.init.addr = entry.value;
			break;
		case DT_FINI:
			if (dt_fini++ > 0)
				break;
			/*
			 * Approximate guess since we don't yet know the
			 * size of .fini
			 */
			if (entry.value < elf_text_base(obj) ||
			    entry.value > elf_text_base(obj) +
			    elf_text_filesz(obj) - 1) {
				obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			}
			obj->dynseg.fini.addr = entry.value;
			break;
		case DT_NEEDED:
			/*
			 * We expect multiple NEEDED tags.
			 */
			so = malloc(sizeof(*so));
			if (so == NULL)
				return false;
			so->index++;
			so->basename = elf_dynamic_string(obj, entry.value);
			if (so->basename == NULL) {
				obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
				free(so);
				break;
			}
			LIST_INSERT_HEAD(&obj->list.shared_objects, so,
			    _linkage);
			break;
		case DT_RELAENT:
			if (dt_relaent++ > 0)
				break;
			if (elf_class(obj) == elfclass32) {
				if (entry.value != sizeof(Elf32_Rela))
					obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			} else if (elf_class(obj) == elfclass64) {
				if (entry.value != sizeof(Elf64_Rela))
					obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			}
			obj->dynseg.relaent.size = entry.value;
			break;
		case DT_RELENT:
			/*
			 * At this point DT_RELENT and DT_RELAENT are not
			 * necessarily vital for forensics reconstruction we may want
			 * to take the anomaly checks out for these.
			 */
			if (dt_relent++ > 0)
				break;
			if (elf_class(obj) == elfclass32) {
				if (entry.value != sizeof(Elf32_Rel))
					obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			} else if (elf_class(obj) == elfclass64) {
				if (entry.value != sizeof(Elf64_Rel))
					obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			}
			obj->dynseg.relent.size = entry.value;
			break;
		case DT_INIT_ARRAY:
			if (dt_init_array++ > 0)
				break;
			if (entry.value < elf_data_base(obj) ||
			    entry.value > elf_data_base(obj) +
			    elf_data_filesz(obj) - ptr_width - 1) {
				obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			}
			obj->dynseg.init_array.addr = entry.value;
			break;
		case DT_FINI_ARRAY:
			if (dt_fini_array++ > 0)
				break;
			/*
			 * note to self: approximate guess here as well
			 * since usually DT_FINI_ARRAY comes before DT_INIT_ARRAY
			 */
			if (entry.value < elf_data_base(obj) ||
			    entry.value > elf_data_base(obj) +
			    elf_data_filesz(obj) - ptr_width - 1) {
				obj->anomalies |= INVALID_F_VITAL_DTAG_VALUE;
			}
			obj->dynseg.fini_array.addr = entry.value;
			break;
			/*
			 * TODO put in anomaly checks for these to make sure
			 * they are valid for reconstruction.
			 */
		case DT_INIT_ARRAYSZ:
			if (dt_init_arraysz++ > 0)
				break;
			obj->dynseg.init_array.size = entry.value;
			break;
		case DT_FINI_ARRAYSZ:
			if (dt_fini_arraysz++ > 0)
				break;
			obj->dynseg.fini_array.size = entry.value;
			break;
		case DT_RPATH:
			/*
			 * TODO we must get the runpath for
			 * supporting these types of .so path
			 * lookups. Also $ORIGIN expansion support
			 * is a must as it is actually used in
			 * a number of important ELF applications
			 * that I've seen.
			 */
		case DT_RUNPATH:
			break;
		case DT_DEBUG:
			if (dt_debug++ > 0)
				break;
			obj->dynseg.debug.value = entry.value;
			obj->flags |= ELF_DT_DEBUG_F;
			break;
		default:
			break;
		}
	}
	return true;
}

void free_misc(elfobj_t *obj)
{

	if (elf_flags(obj, ELF_FORENSICS_F) == true) {
		free(obj->shstrtab);
		switch(obj->e_class) {
		case elfclass32:
			free(obj->shdr32);
			break;
		case elfclass64:
			free(obj->shdr64);
			break;
		}
	}
	return;
}

void
free_lists(elfobj_t *obj)
{
	if (LIST_EMPTY(&obj->list.eh_frame_entries) == 0) {
		struct elf_eh_frame_node *current, *next;

		LIST_FOREACH_SAFE(current, &obj->list.eh_frame_entries,
		    _linkage, next) {
			free(current);
		}
	}
	if (LIST_EMPTY(&obj->list.symtab) == 0) {
		struct elf_symbol_node *current, *next;

		LIST_FOREACH_SAFE(current, &obj->list.symtab,
		    _linkage, next) {
			/*
			 * If we forensically reconstructed .symtab then we are
			 * going to have heap allocated symbol->name's so we must
			 * free them.
			 */
			if (elf_flags(obj, ELF_SYMTAB_RECONSTRUCTION_F) == true)
				free((char *)current->name);
			free(current);
		}
	}
	if (LIST_EMPTY(&obj->list.dynsym) == 0) {
		struct elf_symbol_node *current, *next;

		LIST_FOREACH_SAFE(current, &obj->list.dynsym,
		    _linkage, next) {
			free(current);
		}
	}
	if (LIST_EMPTY(&obj->list.plt) == 0) {
		struct elf_plt_node *current, *next;

		LIST_FOREACH_SAFE(current, &obj->list.plt,
		    _linkage, next) {
			free(current);
		}
	}
	if (LIST_EMPTY(&obj->list.shared_objects) == 0) {
		struct elf_shared_object_node *current, *next;

		LIST_FOREACH_SAFE(current, &obj->list.shared_objects,
		    _linkage, next) {
			free(current);
		}
	}
	return;
}

void
free_caches(elfobj_t *obj)
{

	hdestroy_r(&obj->cache.symtab);
	hdestroy_r(&obj->cache.dynsym);
	hdestroy_r(&obj->cache.plt);
	return;
}

void
free_arrays(elfobj_t *obj)
{
	size_t i;

	for (i = 0; i < obj->section_count; i++) {
		free(obj->sections[i]->name);
		free(obj->sections[i]);
	}
	free(obj->sections);
	return;
}

bool
insane_section_headers(elfobj_t *obj)
{

	if (obj->anomalies & INVALID_F_SHOFF)
		return true;
	if (obj->anomalies & INVALID_F_SHSTRNDX)
		return true;
	if (obj->anomalies & INVALID_F_SHOFFSET)
		return true;
	if (obj->anomalies & INVALID_F_SHNUM)
		return true;
	if (obj->anomalies & INVALID_F_SHENTSIZE)
		return true;
	if (obj->anomalies & INVALID_F_SH_HEADERS)
		return true;
	if (obj->anomalies & INVALID_F_SHSTRTAB)
		return true;
	return false;
}

bool
insane_dynamic_segment(elfobj_t *obj)
{

	if (obj->anomalies & INVALID_F_VITAL_DTAG_VALUE)
		return true;
	return false;
}

/*
 * Returns string table offset, which can then be
 * set into sh_name for the given shdr entry.
 */
static bool
add_shstrtab_entry(elfobj_t *obj, const char *name, uint32_t *out)
{

	if (obj->strindex + strlen(name) + 1 >= obj->internal_shstrtab_size) {
		obj->shstrtab =
		    realloc(obj->shstrtab, obj->internal_shstrtab_size <<= 1);
		if (obj->shstrtab == NULL)
			return false;
	}
	strcpy(&obj->shstrtab[obj->strindex], name);
	*out = (uint32_t)obj->strindex;
	obj->strindex += strlen(name) + 1;
	return true;
}

static void
add_section_entry(elfobj_t *obj, void *ptr)
{

	if (obj->e_class == elfclass32) {
		memcpy(&obj->shdr32[obj->shdrindex], ptr, sizeof(Elf32_Shdr));
		obj->shdrindex++;
		obj->ehdr32->e_shnum++;
		obj->section_count++;
		return;
	} else if (obj->e_class == elfclass64) {
		memcpy(&obj->shdr64[obj->shdrindex], ptr, sizeof(Elf64_Shdr));
		obj->shdrindex++;
		obj->ehdr64->e_shnum++;
		obj->section_count++;
		return;
	}
	return;
}

/*
 * XXX This is temporary:
 * We may want to consider using a disassembler we write ourselves, however
 * that can be time consuming. We can use capstone as well, but haven't really
 * wanted to bring anything else open source into the project yet.
 * TODO: Lets make
 * sure this works with PIE executables as well.
 */
#define INIT_SIZE_THRESHOLD 60 /* This .init section is really about 25 bytes, but we need
				* enough space to go past .init into the .plt
				* until we find it.
				*/
uint64_t
resolve_plt_addr(elfobj_t *obj)
{

	uint64_t start = 25;
	uint64_t init_offset = obj->dynseg.init.addr - elf_text_base(obj);
	uint8_t *inst, *marker;
	uint32_t i = 0;

	if (obj->arch == i386)
		goto i386;
	/*
	 * this code won't work on 32bit binaries, because the opcodes
	 * for the indirect GOT jump won't be using IP relative addressing.
	 */
	for (marker = inst = &obj->mem[init_offset]; inst; inst++, i++) {
		if (inst - marker > INIT_SIZE_THRESHOLD)
			return 0;
		if (*inst != 0x48 && *(inst + 1) != 0x83)
			continue;
		for (;; inst++) {
			if (*inst == 0xc3) {
				for (;; inst++) {
					if (inst - marker > INIT_SIZE_THRESHOLD)
						return 0;
					if (*inst == 0xff && *(inst + 1) == 0x35) {
						return (uint64_t)((uint8_t *)inst -
						    (uint8_t *)marker) + obj->dynseg.init.addr;
					}
				}
			}
			if (inst - marker > INIT_SIZE_THRESHOLD)
				return 0;
		}
	}
	return 0;
i386:
	for (marker = inst = &obj->mem[init_offset + start]; inst; inst++, i++) {
		if (inst - marker > INIT_SIZE_THRESHOLD)
			return 0;
		if (*inst != 0x5b) /* pop %ebx */
			continue;
		if (*(inst + 1) == 0xc3) { /* ret */
			return (uint64_t)((uint8_t *)inst -
			    (uint8_t *)marker) + obj->dynseg.init.addr;
		}
	}
	return 0;
}

/* 
 * This is of course only necessary if there are no section headers
 * so lets locate the beginning of the text segment and search
 * from there since we are looking for the _start glibc init code
 * and if we can't find it we create a section called .text_segment
 * We return 0 on failure, since 0 will never be a valid entry point
 * address due to the fact that there must always be an ELF file header
 * first. Even in a heavily modified binary (i.e. a virus infected file)
 * the smallest entry address would be 0x41, assuming the program header
 * table was shifted forward, such as in a reverse text infection.
 */
#define GLIBC_START_CODE_64	"\x55\x48\x89\xe5\x48" /* enough to identify _start */
#define GLIBC_START_CODE_64_v2	"\x31\xed\x49\x89\xd1" /* enough to identify _start */
#define GLIBC_START_CODE_32	"\x31\xed\x5e\x89\xe1" /* enough to identify _start */

static uint64_t
original_ep(elfobj_t *obj)
{
	uint8_t *ptr = &obj->mem[0];
	uint8_t *inst, *marker;
	size_t i;

	for (i = 0, marker = inst = ptr; inst; inst++, i++) {
		if (i >= (elf_text_offset(obj) + elf_text_filesz(obj) - 6))
			return 0;
		if (obj->arch == x64) {
			if (memcmp(&inst[i], GLIBC_START_CODE_64,
			    sizeof(GLIBC_START_CODE_64) - 1) == 0)
				return elf_text_base(obj) + inst - marker;
			else if (memcmp(&inst[i], GLIBC_START_CODE_64_v2,
				    sizeof(GLIBC_START_CODE_64_v2) - 1) == 0)
					return elf_text_base(obj) + inst - marker;
		} else if (obj->arch == i386) {
			if (memcmp(&inst[i], GLIBC_START_CODE_32,
			    sizeof(GLIBC_START_CODE_32) - 1) == 0)
				return elf_text_base(obj) + inst - marker;
		}
	}
	return 0;
}

/*
 * Reconstruct static executables as best we can with
 * limited data; there is no PT_GNU_EH_FRAME segment,
 * .text_segment .text, .note, .data_segment, .data, .bss, .tls, .relro.
 */
/*
 * This reconstructs the section header tables internally if the
 * FORENSICS flag is passed to elf_object_open(), which is very
 * useful with malware that has corrupted or stripped section
 * headers. The result is that we use techniques such as the
 * dynamic symbol table, and the PT_GNU_EH_FRAME segment to
 * reconstruct symbol and function data which is indespensible
 * for forensics analysts.
 */
#define RELA_ENT_SIZE 24 /* REL_ENT_SIZE is 12 */
#define REL_ENT_SIZE 12

bool
reconstruct_elf_sections(elfobj_t *obj, elf_error_t *e)
{
	union {
		Elf32_Shdr shdr32;
		Elf64_Shdr shdr64;
	} elf;
	uint32_t soffset, dynstr_index; /* string table offset */
	size_t dynsym_count = 0, relaplt_count = 0, relaplt_size = 0;
	size_t word_size = obj->arch == i386 ? 4 : 8;
	int dynsym_index = 0;
	int gotplt_index = 0;
	const char *sname = NULL;
	size_t total_sh_offset_len = 0;

	obj->internal_section_count = INTERNAL_SECTION_COUNT;
	obj->internal_shstrtab_size = INTERNAL_SHSTRTAB_SIZE;
	/*
	 * We only reconstruct ELF sections if the section headers
	 * are damaged or don't exist so we must create our own
	 * string table. Keep in mind we don't update the binary
	 * with section headers, these are for internal representation
	 * only.
	 */
	obj->strindex = 0; /* Should be initialized already anyway */
	obj->shstrtab = malloc(obj->internal_shstrtab_size);
	if (obj->shstrtab == NULL) {
		return elf_error_set(e, "malloc failed");
	}
	obj->flags |= ELF_FORENSICS_F;
	/*
	 * Lets reconstruct the dynamic segment
	 * into forensics relevant data in this first
	 * big block of code. Next we will reconstruct
	 * some other areas that don't have to do with the
	 * data segment.
	 */
	switch(obj->e_class) {
	case elfclass32:
		/*
		 * Initialize incase it was corrupted in the actual target elf.
		 */
		obj->ehdr32->e_shnum = 0;
		if (obj->dynseg.exists == false) /* XXX fix this; for static binaries */
			break;
		obj->shdr32 =
		    malloc(sizeof(Elf32_Shdr) * obj->internal_section_count);
		if (obj->shdr32 == NULL)
			return elf_error_set(e, "malloc");

		if (add_shstrtab_entry(obj, ".gnu_hash", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry");
		}
		elf.shdr32.sh_addr = obj->dynseg.hash.addr;
		elf.shdr32.sh_size = (obj->dynseg.dynsym.addr - elf_text_base(obj)) -
		    (obj->dynseg.hash.addr - elf_text_base(obj));
		elf.shdr32.sh_offset = (obj->dynseg.hash.addr - elf_text_base(obj));
		elf.shdr32.sh_addralign = 4;
		elf.shdr32.sh_info = 0;
		elf.shdr32.sh_flags = SHF_ALLOC;
		elf.shdr32.sh_name = soffset;
		elf.shdr32.sh_entsize = sizeof(Elf32_Sym);
		elf.shdr32.sh_type = SHT_GNU_HASH;
		add_section_entry(obj, &elf.shdr32);
		/*
		 * Create internal representation of .dynsym section
		 */
		if (add_shstrtab_entry(obj, ".dynsym", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}
		elf.shdr32.sh_addr = obj->dynseg.dynsym.addr;
		elf.shdr32.sh_size =
		    obj->dynseg.dynstr.addr - obj->dynseg.dynsym.addr;
		elf.shdr32.sh_link = obj->section_count + 1; /* The next section will be .dynstr */
		elf.shdr32.sh_offset =
		    obj->dynseg.dynsym.addr - elf_text_base(obj);
		elf.shdr32.sh_addralign = sizeof(long);
		elf.shdr32.sh_info = 0;
		elf.shdr32.sh_flags = SHF_ALLOC;
		elf.shdr32.sh_name = soffset;
		elf.shdr32.sh_entsize = sizeof(Elf32_Sym);
		elf.shdr32.sh_type = SHT_DYNSYM;
		dynsym_index = obj->section_count;
		/*
		 * We must set dynsym_count and dynsym32 so that build_dynsym_data
		 * can be run properly. We also will need the dynstr section properly
		 * setup to retrieve the symbol names.
		 */
		if (elf.shdr32.sh_size != 0 && elf.shdr32.sh_entsize != 0)
			dynsym_count = obj->dynsym_count = (elf.shdr32.sh_size / elf.shdr32.sh_entsize);
		obj->dynsym32 = (Elf32_Sym *)((uint8_t *)&obj->mem[elf.shdr32.sh_offset]);
		add_section_entry(obj, &elf.shdr32);
		obj->flags |= ELF_DYNSYM_F;


		/*
		 * .dynstr
		 */
		if (add_shstrtab_entry(obj, ".dynstr", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}
		elf.shdr32.sh_addr = obj->dynseg.dynstr.addr;
		elf.shdr32.sh_size = obj->dynseg.dynstr.size;
		elf.shdr32.sh_link = 0;
		elf.shdr32.sh_offset =
		    obj->dynseg.dynstr.addr - elf_text_base(obj);
		elf.shdr32.sh_addralign = 1;
		elf.shdr32.sh_info = 0;
		elf.shdr32.sh_flags = SHF_ALLOC;
		elf.shdr32.sh_name = soffset;
		elf.shdr32.sh_entsize = 1;
		elf.shdr32.sh_type = SHT_STRTAB;
		obj->dynstr = (char *)&obj->mem[elf.shdr32.sh_offset];
		dynstr_index = obj->shdrindex;
		add_section_entry(obj, &elf.shdr32);
		/*
		 * NOTE: set the ELF_DYNSYM_F flag so the client code knows
		 * if its able to use the dynamic symbol table.
		 */
		obj->flags |= ELF_DYNSTR_F;

		/*
		 * .got.plt
		 */
		if (add_shstrtab_entry(obj, ".got.plt", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}
		elf.shdr32.sh_addr = obj->dynseg.pltgot.addr;
		/*
		 * GOT should be big enough for 3 reserved entries and
		 * then enough slots for each dynamic symbol.
		 */
		elf.shdr32.sh_size = (word_size * 3) +
		    (word_size * dynsym_count);
		elf.shdr32.sh_link = 0;
		elf.shdr32.sh_offset =
		    obj->dynseg.pltgot.addr - elf_text_base(obj);
		elf.shdr32.sh_addralign = word_size;
		elf.shdr32.sh_info = 0;
		elf.shdr32.sh_flags = SHF_ALLOC|SHF_WRITE;
		elf.shdr32.sh_name = soffset;
		elf.shdr32.sh_entsize = word_size;
		elf.shdr32.sh_type = SHT_PROGBITS;
		gotplt_index = obj->section_count;
		add_section_entry(obj, &elf.shdr32);
		obj->flags |= ELF_PLTGOT_F;

		/*
		 * .plt
		 */
		if (add_shstrtab_entry(obj, ".plt", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}
		elf.shdr32.sh_addr = resolve_plt_addr(obj);
		/*
		 * Prevent floating point exception if a malformed binary has
		 * d_tag values that are set to 0.
		 */
		if (obj->dynseg.pltrel.size != 0 && obj->dynseg.relent.size != 0)
			relaplt_count = obj->dynseg.pltrel.size / obj->dynseg.relent.size;
		relaplt_size = relaplt_count * 16;
		elf.shdr32.sh_size = relaplt_size;
		elf.shdr32.sh_size += 16;
		elf.shdr32.sh_link = 0;
		elf.shdr32.sh_offset =
		    elf.shdr32.sh_addr - elf_text_base(obj);
		elf.shdr32.sh_info = 0;
		elf.shdr32.sh_flags = SHF_ALLOC|SHF_EXECINSTR;
		elf.shdr32.sh_name = soffset;
		elf.shdr32.sh_entsize = 16;
		elf.shdr32.sh_type = SHT_PROGBITS;
		add_section_entry(obj, &elf.shdr32);
		obj->flags |= ELF_PLT_F;

		/*
		 * .rel[a].plt (Necessary for plt iterators)
		 */
		sname = obj->dynseg.relent.size == RELA_ENT_SIZE ? ".rela.plt" :
		    ".rel.plt";

		if (add_shstrtab_entry(obj, sname, &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}
		elf.shdr32.sh_addr = obj->dynseg.pltrel.addr;
		elf.shdr32.sh_size = obj->dynseg.pltrel.size;
		elf.shdr32.sh_link = dynsym_index;
		elf.shdr32.sh_offset = obj->dynseg.pltrel.addr - elf_text_base(obj);
		elf.shdr32.sh_info = gotplt_index;
		elf.shdr32.sh_entsize = obj->dynseg.relent.size;
		elf.shdr32.sh_name = soffset;
		elf.shdr32.sh_type = obj->dynseg.relent.size ==
		    RELA_ENT_SIZE ? SHT_RELA : SHT_REL;
		elf.shdr32.sh_flags = SHF_ALLOC;
		add_section_entry(obj, &elf.shdr32);
		obj->flags |= ELF_PLT_RELOCS_F;

		/*
		 * .init
		 */
		if (add_shstrtab_entry(obj, ".init", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}

		elf.shdr32.sh_addr = obj->dynseg.init.addr;
		elf.shdr32.sh_size = resolve_plt_addr(obj) - obj->dynseg.init.addr;
		elf.shdr32.sh_link = 0;
		elf.shdr32.sh_offset = obj->dynseg.init.addr - elf_text_base(obj);
		elf.shdr32.sh_info = 0;
		elf.shdr32.sh_entsize = 0;
		elf.shdr32.sh_name = soffset;
		elf.shdr32.sh_type = SHT_PROGBITS;
		elf.shdr32.sh_flags = SHF_ALLOC|SHF_EXECINSTR;
		add_section_entry(obj, &elf.shdr32);

		/*
		 * .fini
		 */
		if (add_shstrtab_entry(obj, ".fini", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}
		elf.shdr32.sh_addr = obj->dynseg.fini.addr;
		elf.shdr32.sh_size = resolve_plt_addr(obj) - obj->dynseg.fini.addr;
		elf.shdr32.sh_offset = obj->dynseg.init.addr - elf_text_base(obj);
		elf.shdr32.sh_info = 0;
		elf.shdr32.sh_entsize = 0;
		elf.shdr32.sh_name = soffset;
		elf.shdr32.sh_type = SHT_PROGBITS;
		elf.shdr32.sh_flags = SHF_ALLOC|SHF_EXECINSTR;
		add_section_entry(obj, &elf.shdr32);

		/*
		 * .text
		 */
		if (add_shstrtab_entry(obj, ".text", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}

		elf.shdr32.sh_addr = original_ep(obj);
		if (obj->dynseg.fini.addr < original_ep(obj)) {
			elf.shdr32.sh_size = 1024; /* If we can't rely on .fini being after .text
						    * then we will just assign 1024 bytes
						    */
		} else {
			elf.shdr32.sh_size = obj->dynseg.fini.addr - original_ep(obj);
		}
		elf.shdr32.sh_link = 0;
		elf.shdr32.sh_info = 0;
		elf.shdr32.sh_name = soffset;
		elf.shdr32.sh_type = SHT_PROGBITS;
		elf.shdr32.sh_entsize = 1;
		elf.shdr32.sh_flags = SHF_EXECINSTR|SHF_ALLOC;
		add_section_entry(obj, &elf.shdr32);

		/*
		 * INIT_ARRAY
		 */
		if (add_shstrtab_entry(obj, ".init_array", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}
		elf.shdr32.sh_addr = obj->dynseg.init_array.addr;
		elf.shdr32.sh_size = obj->dynseg.init_array.size;
		elf.shdr32.sh_link = 0;
		elf.shdr32.sh_name = soffset;
		elf.shdr32.sh_type = SHT_INIT_ARRAY;
		elf.shdr32.sh_entsize = sizeof(uint32_t);
		elf.shdr32.sh_info = 0;
		elf.shdr32.sh_flags = SHF_WRITE|SHF_ALLOC;
		add_section_entry(obj, &elf.shdr32);

		/*
		 * FINI_ARRAY
		 */
		if (add_shstrtab_entry(obj, ".fini_array", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}

		elf.shdr32.sh_addr = obj->dynseg.fini_array.addr;
		elf.shdr32.sh_size = obj->dynseg.fini_array.size;
		elf.shdr32.sh_link = 0;
		elf.shdr32.sh_name = soffset;
		elf.shdr32.sh_type = SHT_FINI_ARRAY;
		elf.shdr32.sh_entsize = sizeof(uint32_t);
		elf.shdr32.sh_info = 0;
		elf.shdr32.sh_flags = SHF_WRITE|SHF_ALLOC;
		add_section_entry(obj, &elf.shdr32);

		if (add_shstrtab_entry(obj, ".dynamic", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}
		elf.shdr32.sh_addr = obj->dynamic_addr;
		elf.shdr32.sh_size = obj->dynamic_size;
		elf.shdr32.sh_offset = obj->dynamic_offset;
		elf.shdr32.sh_link = 0;
		elf.shdr32.sh_name = soffset;
		elf.shdr32.sh_entsize = sizeof(Elf32_Dyn);
		elf.shdr32.sh_info = 0;
		elf.shdr32.sh_flags = SHF_ALLOC|SHF_WRITE;
		elf.shdr32.sh_addralign = sizeof(uint32_t);
		add_section_entry(obj, &elf.shdr32);

		if (obj->eh_frame_hdr_addr != 0) {
			/*
			 * .eh_frame (Necessary for stack unwinding)
			 */
			if (add_shstrtab_entry(obj, ".eh_frame_hdr",
			    &soffset) == false) {
				return elf_error_set(e, "add_shstrtab_entry failed");
			}
			elf.shdr32.sh_addr = obj->eh_frame_hdr_addr;
			elf.shdr32.sh_size = obj->eh_frame_hdr_size;
			elf.shdr32.sh_offset = obj->eh_frame_hdr_offset;
			elf.shdr32.sh_link = 0;
			elf.shdr32.sh_info = 0;
			elf.shdr32.sh_entsize = sizeof(uint32_t);
			elf.shdr32.sh_name = soffset;
			elf.shdr32.sh_type = SHT_PROGBITS;
			add_section_entry(obj, &elf.shdr32);

			if (add_shstrtab_entry(obj, ".eh_frame",
			    &soffset) == false) {
				return elf_error_set(e, "add_shstrtab_entry failed");
			}
			elf.shdr32.sh_addr =
			    obj->eh_frame_hdr_addr + obj->eh_frame_hdr_size +
			    ((sizeof(uint32_t)) & ~(sizeof(uint32_t) - 1));
			/*
			 * .eh_frame is right before .init_array, which is the first
			 * section in the data segment.
			 */
			elf.shdr32.sh_offset =
			    obj->eh_frame_hdr_offset + obj->eh_frame_hdr_size +
			    ((sizeof(uint32_t)) & ~(sizeof(uint32_t) - 1));
			elf.shdr32.sh_size = elf.shdr32.sh_offset - obj->text_segment_filesz;
			elf.shdr32.sh_name = soffset;
			elf.shdr32.sh_link = 0;
			elf.shdr32.sh_info = 0;
			elf.shdr32.sh_entsize = sizeof(uintptr_t);
			elf.shdr32.sh_type = SHT_PROGBITS;
			add_section_entry(obj, &elf.shdr32);

			if (add_shstrtab_entry(obj, ".symtab",
			    &soffset) == false) {
				return elf_error_set(e, "add_shstrtab_entry failed");
			}

			elf.shdr32.sh_addr = 0UL;
			elf.shdr32.sh_offset = total_sh_offset_len;
			elf.shdr32.sh_size = obj->fde_count * sizeof(Elf32_Sym);
			elf.shdr32.sh_info = 0;
			elf.shdr32.sh_entsize = sizeof(Elf32_Sym);
			elf.shdr32.sh_type = SHT_SYMTAB;
			elf.shdr32.sh_name = soffset;
			elf.shdr32.sh_link = 0; /* should link to .strtab */
			add_section_entry(obj, &elf.shdr32);
			total_sh_offset_len += elf.shdr32.sh_size;
			obj->flags |= ELF_SYMTAB_RECONSTRUCTION_F;

			if (add_shstrtab_entry(obj, ".strtab",
			    &soffset) == false) {
				return elf_error_set(e, sname, &soffset);
			}

			elf.shdr32.sh_addr = 0UL;
			elf.shdr32.sh_offset = total_sh_offset_len;
			elf.shdr32.sh_size = 0;
			total_sh_offset_len = elf.shdr32.sh_size;
			elf.shdr32.sh_info = 0;
			elf.shdr32.sh_entsize = 1;
			elf.shdr32.sh_type = SHT_STRTAB;
			elf.shdr32.sh_name = soffset;
			elf.shdr32.sh_link = obj->section_count;
			add_section_entry(obj, &elf.shdr32);

			if (add_shstrtab_entry(obj, ".shstrtab",
			    &soffset) == false) {
				return elf_error_set(e, sname, &soffset);
			}

		}
		break;
	case elfclass64:
		obj->ehdr64->e_shnum = 0;
		if (obj->dynseg.exists == false)
			break;
		/*
		 * Create internal representation of .dynsym section
		 */
		obj->shdr64 =
		    malloc(sizeof(Elf64_Shdr) * obj->internal_section_count);
		if (obj->shdr64 == NULL)
			return elf_error_set(e, "malloc");

		if (add_shstrtab_entry(obj, ".gnu.hash", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}

		elf.shdr64.sh_addr = obj->dynseg.hash.addr;
		elf.shdr64.sh_size = (obj->dynseg.dynsym.addr - elf_text_base(obj)) -
		    (obj->dynseg.hash.addr - elf_text_base(obj));
		elf.shdr64.sh_offset = (obj->dynseg.hash.addr - elf_text_base(obj));
		elf.shdr64.sh_addralign = 4;
		elf.shdr64.sh_info = 0;
		elf.shdr64.sh_flags = SHF_ALLOC;
		elf.shdr64.sh_name = soffset;
		elf.shdr64.sh_entsize = sizeof(Elf64_Sym);
		elf.shdr64.sh_type = SHT_GNU_HASH;
		add_section_entry(obj, &elf.shdr64);
		total_sh_offset_len += elf.shdr64.sh_size;

		if (add_shstrtab_entry(obj, ".dynsym", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}

		elf.shdr64.sh_addr = obj->dynseg.dynsym.addr;
		elf.shdr64.sh_size =
		    obj->dynseg.dynstr.addr - obj->dynseg.dynsym.addr;
		elf.shdr64.sh_link = 1;
		elf.shdr64.sh_offset =
		    obj->dynseg.dynsym.addr - elf_text_base(obj);
		elf.shdr64.sh_addralign = sizeof(long);
		elf.shdr64.sh_info = 0;
		elf.shdr64.sh_flags = SHF_ALLOC;
		elf.shdr64.sh_name = soffset;
		elf.shdr64.sh_entsize = sizeof(Elf64_Sym);
		if (elf.shdr64.sh_size != 0 && elf.shdr64.sh_entsize != 0)
			obj->dynsym_count = (elf.shdr64.sh_size / elf.shdr64.sh_entsize);
		obj->dynsym64 = (Elf64_Sym *)((uint8_t *)&obj->mem[elf.shdr64.sh_offset]);
		add_section_entry(obj, &elf.shdr64);
		total_sh_offset_len += elf.shdr64.sh_size;
		obj->flags |= ELF_DYNAMIC_F;
		/*
		 * .dynstr
		 */
		if (add_shstrtab_entry(obj, ".dynstr", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}
		elf.shdr64.sh_addr = obj->dynseg.dynstr.addr;
		elf.shdr64.sh_size = obj->dynseg.dynstr.size;
		elf.shdr64.sh_link = 0;
		elf.shdr64.sh_offset = obj->dynseg.dynstr.addr - elf_text_base(obj);
		elf.shdr64.sh_addralign = 1;
		elf.shdr64.sh_info = 0;
		elf.shdr64.sh_flags = SHF_ALLOC;
		elf.shdr64.sh_name = soffset;
		elf.shdr64.sh_entsize = 1;
		obj->dynstr = (char *)&obj->mem[elf.shdr64.sh_offset];
		dynstr_index = obj->shdrindex;
		add_section_entry(obj, &elf.shdr64);
		total_sh_offset_len += elf.shdr64.sh_size;

		/*
		 * NOTE: set the ELF_DYNSYM_F flag so the client code knows
		 * if its able to use the dynamic symbol table.
		 */
		obj->flags |= ELF_DYNSTR_F;

		/*
		 * .got.plt
		 */
		if (add_shstrtab_entry(obj, ".got.plt", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}
		elf.shdr64.sh_addr = obj->dynseg.pltgot.addr;
		/*
		 * GOT should be big enough for 3 reserved entries and
		 * then enough slots for each dynamic symbol.
		 */
		elf.shdr64.sh_size = (word_size * 3) +
		    (word_size * obj->dynsym_count);
		elf.shdr64.sh_link = 0;
		elf.shdr64.sh_offset =
		    (obj->dynseg.pltgot.addr - elf_data_base(obj)) + elf_data_offset(obj);
		elf.shdr64.sh_addralign = word_size;
		elf.shdr64.sh_info = 0;
		elf.shdr64.sh_flags = SHF_ALLOC|SHF_WRITE;
		elf.shdr64.sh_name = soffset;
		elf.shdr64.sh_entsize = word_size;
		add_section_entry(obj, &elf.shdr64);
		obj->flags |= ELF_PLTGOT_F;
		total_sh_offset_len += elf.shdr64.sh_size;

		/*
		 * .plt
		 */
		if (add_shstrtab_entry(obj, ".plt", &soffset) == false) {
			return elf_error_set(e, ".plt", &soffset);
		}
		elf.shdr64.sh_addr = resolve_plt_addr(obj);
		if (obj->dynseg.pltrel.size != 0 && obj->dynseg.relaent.size != 0)
			relaplt_count = obj->dynseg.pltrel.size / obj->dynseg.relaent.size;
		relaplt_size = relaplt_count * 16;
		elf.shdr64.sh_size = relaplt_size;
		elf.shdr64.sh_size += 16;
		elf.shdr64.sh_link = 0;
		elf.shdr64.sh_offset =
		    elf.shdr64.sh_addr - elf_text_base(obj);
		elf.shdr64.sh_info = 0;
		elf.shdr64.sh_flags = SHF_ALLOC|SHF_EXECINSTR;
		elf.shdr64.sh_name = soffset;
		elf.shdr64.sh_entsize = 16;
		add_section_entry(obj, &elf.shdr64);
		total_sh_offset_len += elf.shdr64.sh_size;
		obj->flags |= ELF_PLT_F;

		/*
		 * .rel[a].plt (Necessary for plt iterators)
		 */
		sname = obj->dynseg.relaent.size == RELA_ENT_SIZE ? ".rela.plt" :
		    "rel.plt";
		if (add_shstrtab_entry(obj, sname, &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}
		elf.shdr64.sh_addr = obj->dynseg.pltrel.addr;
		elf.shdr64.sh_size = obj->dynseg.pltrel.size;
		elf.shdr64.sh_link = dynsym_index;
		elf.shdr64.sh_offset = obj->dynseg.pltrel.addr - elf_text_base(obj);
		elf.shdr64.sh_info = gotplt_index;
		elf.shdr64.sh_entsize = obj->dynseg.relaent.size;
		elf.shdr64.sh_name = soffset;
		elf.shdr64.sh_type = obj->dynseg.relaent.size == RELA_ENT_SIZE
		    ? SHT_RELA : SHT_REL;
		total_sh_offset_len += elf.shdr64.sh_size;

		add_section_entry(obj, &elf.shdr64);
		obj->flags |= ELF_PLT_RELOCS_F;

		if (add_shstrtab_entry(obj, ".init", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}
		elf.shdr64.sh_addr = obj->dynseg.init.addr;
		elf.shdr64.sh_size = resolve_plt_addr(obj) - obj->dynseg.init.addr;
		elf.shdr64.sh_link = 0;
		elf.shdr64.sh_offset = obj->dynseg.init.addr - elf_text_base(obj);
		elf.shdr64.sh_info = 0;
		elf.shdr64.sh_entsize = 0;
		elf.shdr64.sh_name = soffset;
		elf.shdr64.sh_type = SHT_PROGBITS;
		elf.shdr64.sh_flags = SHF_ALLOC|SHF_EXECINSTR;
		add_section_entry(obj, &elf.shdr64);

		if (add_shstrtab_entry(obj, ".fini", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}
		elf.shdr64.sh_addr = obj->dynseg.fini.addr;
		elf.shdr64.sh_size = resolve_plt_addr(obj) - obj->dynseg.fini.addr;
		elf.shdr64.sh_link = 0;
		elf.shdr64.sh_offset = obj->dynseg.init.addr - elf_text_base(obj);
		elf.shdr64.sh_info = 0;
		elf.shdr64.sh_entsize = 0;
		elf.shdr64.sh_name = soffset;
		elf.shdr64.sh_type = SHT_PROGBITS;
		elf.shdr64.sh_flags = SHF_ALLOC|SHF_EXECINSTR;
		add_section_entry(obj, &elf.shdr64);

		/*
		 * .text
		 */
		if (add_shstrtab_entry(obj, ".text", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry");
		}
		elf.shdr64.sh_addr = original_ep(obj);
		if (obj->dynseg.fini.addr < original_ep(obj)) {
			elf.shdr64.sh_size = 1024;
		} else {
			elf.shdr64.sh_size = obj->dynseg.fini.addr - original_ep(obj);
		}
		elf.shdr64.sh_link = 0;
		elf.shdr64.sh_info = 0;
		elf.shdr64.sh_name = soffset;
		elf.shdr64.sh_type = SHT_PROGBITS;
		elf.shdr64.sh_entsize = 1;
		elf.shdr64.sh_flags = SHF_EXECINSTR|SHF_ALLOC;
		add_section_entry(obj, &elf.shdr64);

		if (add_shstrtab_entry(obj, ".init_array", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}
		elf.shdr64.sh_addr = obj->dynseg.init_array.addr;
		elf.shdr64.sh_size = obj->dynseg.init_array.size;
		elf.shdr64.sh_link = 0;
		elf.shdr64.sh_name = soffset;
		elf.shdr64.sh_type = SHT_INIT_ARRAY;
		elf.shdr64.sh_entsize = sizeof(uintptr_t);
		elf.shdr64.sh_info = 0;
		elf.shdr64.sh_flags = SHF_WRITE|SHF_ALLOC;
		add_section_entry(obj, &elf.shdr64);

		if (add_shstrtab_entry(obj, ".fini_array", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}

		elf.shdr64.sh_addr = obj->dynseg.fini_array.addr;
		elf.shdr64.sh_size = obj->dynseg.fini_array.size;
		elf.shdr64.sh_link = 0;
		elf.shdr64.sh_name = soffset;
		elf.shdr64.sh_type = SHT_FINI_ARRAY;
		elf.shdr64.sh_entsize = sizeof(uintptr_t);
		elf.shdr64.sh_info = 0;
		elf.shdr64.sh_flags = SHF_WRITE|SHF_ALLOC;
		add_section_entry(obj, &elf.shdr64);

		if (add_shstrtab_entry(obj, ".dynamic", &soffset) == false) {
			return elf_error_set(e, "add_shstrtab_entry failed");
		}
		elf.shdr64.sh_addr = obj->dynamic_addr;
		elf.shdr64.sh_size = obj->dynamic_size;
		elf.shdr64.sh_offset = obj->dynamic_offset;
		elf.shdr64.sh_link = dynstr_index;
		elf.shdr64.sh_name = soffset;
		elf.shdr64.sh_entsize = sizeof(Elf64_Dyn);
		elf.shdr64.sh_info = 0;
		elf.shdr64.sh_flags = SHF_ALLOC|SHF_WRITE;
		elf.shdr64.sh_addralign = sizeof(uint32_t);
		add_section_entry(obj, &elf.shdr64);

		/*
		 * eh_frame/.eh_frame_hdr (Necessary for stack unwinding)
		 */
		if (obj->eh_frame_hdr_addr != 0) {

			if (add_shstrtab_entry(obj, ".eh_frame_hdr",
			    &soffset) == false) {
				return elf_error_set(e, "add_shstrtab_entry failed");
			}
			elf.shdr64.sh_addr = obj->eh_frame_hdr_addr;
			elf.shdr64.sh_size = obj->eh_frame_hdr_size;
			elf.shdr64.sh_offset = obj->eh_frame_hdr_offset;
			elf.shdr64.sh_link = 0;
			elf.shdr64.sh_info = 0;
			elf.shdr64.sh_entsize = sizeof(uint32_t);
			elf.shdr64.sh_name = soffset;
			elf.shdr64.sh_type = SHT_PROGBITS;
			add_section_entry(obj, &elf.shdr64);
			total_sh_offset_len += elf.shdr64.sh_size;

			if (add_shstrtab_entry(obj, ".eh_frame",
			    &soffset) == false) {
				return elf_error_set(e, sname, &soffset);
			}
			elf.shdr64.sh_addr =
			    obj->eh_frame_hdr_addr + obj->eh_frame_hdr_size +
			    ((sizeof(uint32_t)) & ~(sizeof(uint32_t) - 1));
			/*
			 * .eh_frame is right before .init_array, which is the first
			 * section in the data segment.
			 */
			elf.shdr64.sh_offset =
			    obj->eh_frame_hdr_offset + obj->eh_frame_hdr_size +
			    ((sizeof(uint32_t)) & ~(sizeof(uint32_t) - 1));
			elf.shdr64.sh_size = elf.shdr64.sh_offset - obj->text_segment_filesz;
			elf.shdr64.sh_name = soffset;
			elf.shdr64.sh_link = 0;
			elf.shdr64.sh_info = 0;
			elf.shdr64.sh_entsize = sizeof(uintptr_t);
			elf.shdr64.sh_type = SHT_PROGBITS;
			add_section_entry(obj, &elf.shdr64);
			total_sh_offset_len += elf.shdr64.sh_size;

			obj->flags |= ELF_EH_FRAME_F;

			if (add_shstrtab_entry(obj, ".symtab",
			    &soffset) == false) {
				return elf_error_set(e, "add_shstrtab_entry failed");
			}
			elf.shdr64.sh_addr = 0UL;
			elf.shdr64.sh_offset = total_sh_offset_len;
			elf.shdr64.sh_size = obj->fde_count * sizeof(Elf64_Sym);
			elf.shdr64.sh_info = 0;
			elf.shdr64.sh_entsize = sizeof(Elf64_Sym);
			elf.shdr64.sh_type = SHT_SYMTAB;
			elf.shdr64.sh_name = soffset;
			elf.shdr64.sh_link = 0; /* should link to .strtab */
			add_section_entry(obj, &elf.shdr64);
			total_sh_offset_len += elf.shdr64.sh_size;
			obj->flags |= ELF_SYMTAB_RECONSTRUCTION_F;

			if (add_shstrtab_entry(obj, ".strtab",
			    &soffset) == false) {
				return elf_error_set(e, sname, &soffset);
			}
			elf.shdr64.sh_addr = 0UL;
			elf.shdr64.sh_offset = total_sh_offset_len;
			/*
			 * This .strtab section must account for the size of our string table
			 * which is fortunately a fixed length of sub_<14 bytes> * obj->fde_count
			 */
			elf.shdr64.sh_size = 0;
			total_sh_offset_len = elf.shdr64.sh_size;

			elf.shdr64.sh_info = 0;
			elf.shdr64.sh_entsize = 1;
			elf.shdr64.sh_type = SHT_NOBITS; /* This is not a real string table */
			elf.shdr64.sh_name = soffset;
			elf.shdr64.sh_link = obj->section_count;
			add_section_entry(obj, &elf.shdr64);

			if (add_shstrtab_entry(obj, ".shstrtab",
			    &soffset) == false) {
				return elf_error_set(e, "add_shstrtab_entry failed\n");
			}
		}
		break;
	default:
		break;
	}
	return sort_elf_sections(obj, e);
}

/*
 * Sections are sorted by name for binary search
 * lookup by name. Should be changed to a cache
 */
bool
sort_elf_sections(elfobj_t *obj, elf_error_t *error)
{
	size_t section_count = elf_section_count(obj);
	size_t i;

	obj->sections = (struct elf_section **)
	    malloc(sizeof(struct elf_section *) * (section_count + 1));

	if (obj->sections == NULL) {
		elf_error_set(error, "malloc: %s", strerror(errno));
		return false;
	}
	for (i = 0; i < section_count; i++) {
		obj->sections[i] = malloc(sizeof(struct elf_section));
		if (obj->sections[i] == NULL) {
			elf_error_set(error, "malloc: %s", strerror(errno));
			return false;
		}
		switch(obj->e_class) {
		case elfclass32:
			obj->sections[i]->name =
			    strdup(&obj->shstrtab[obj->shdr32[i].sh_name]);
			obj->sections[i]->type = obj->shdr32[i].sh_type;
			obj->sections[i]->link = obj->shdr32[i].sh_link;
			obj->sections[i]->info = obj->shdr32[i].sh_info;
			obj->sections[i]->flags = obj->shdr32[i].sh_flags;
			obj->sections[i]->align = obj->shdr32[i].sh_addralign;
			obj->sections[i]->entsize = obj->shdr32[i].sh_entsize;
			obj->sections[i]->offset = obj->shdr32[i].sh_offset;
			obj->sections[i]->address = obj->shdr32[i].sh_addr;
			obj->sections[i]->size = obj->shdr32[i].sh_size;
			break;
		case elfclass64:
			obj->sections[i]->name =
			    strdup(&obj->shstrtab[obj->shdr64[i].sh_name]);
			obj->sections[i]->type = obj->shdr64[i].sh_type;
			obj->sections[i]->link = obj->shdr64[i].sh_link;
			obj->sections[i]->info = obj->shdr64[i].sh_info;
			obj->sections[i]->flags = obj->shdr64[i].sh_flags;
			obj->sections[i]->align = obj->shdr64[i].sh_addralign;
			obj->sections[i]->entsize = obj->shdr64[i].sh_entsize;
			obj->sections[i]->offset = obj->shdr64[i].sh_offset;
			obj->sections[i]->address = obj->shdr64[i].sh_addr;
			obj->sections[i]->size = obj->shdr64[i].sh_size;
			break;
		}
	}

	/*
	 * Sorting an array of pointers to struct elf_section
	 */
	qsort(obj->sections, section_count,
	    sizeof(struct elf_section *), section_name_cmp);
	return true;
}

static uint64_t
dw_read_uleb128(uint8_t *ptr, size_t len)
{
	uint32_t shift = 0;
	uint64_t retval = 0;

	while (ptr != NULL && len > 0) {
		uint8_t b = *ptr;

		retval |= (uint64_t)(b & 0x7f) << shift;
		if ((b & 0x80) == 0)
			break;
		shift += 7;
		ptr++, len--;
	}
	return retval;
}

static int64_t
dw_read_sleb128(uint8_t *ptr, size_t len)
{
	uint32_t shift = 0;
	uint64_t retval = 0, srv = 0;
	uint8_t sign = 0;

	while(ptr != NULL && len > 0) {
		uint8_t b = *ptr;

		retval |= (uint64_t)(b & 0x7f) << shift;
		shift += 7;
		sign = (b & 0x40);
		if ((b & 0x80) == 0)
			break;
	}
	srv = (int64_t)retval;
	if ((shift < 64) && sign == 1) {
		uint64_t shift_bit = (1 << shift);
		srv |= ~(shift_bit - 1);
	}
	return srv;
}
/*
 * Any info we are reading will always be coming from the code segment
 * for our purposes, since that's where .eh_frame will always live
 * but we handle data segment locations just in-case future of future
 * uses. This function is a bit ugly, it either returns a single value
 * or it stores two uint32_t values into an eh_frame_vec, which is not
 * the best naming convention.
 */
static uint64_t
dw_read_location(elfobj_t *obj, uint64_t vaddr, size_t len,
    bool *res, struct eh_frame_vec *ev)
{
	uint8_t *ptr = NULL;

	ptr = elf_address_pointer(obj, vaddr);
	if (ptr == NULL) {
		*res = false;
		return 0;
	}
	*res = true;
	if (ev != NULL) {
		ev->initial_loc = *(uint32_t *)ptr;
		ev->fde_entry_offset = *(uint32_t *)&ptr[4];
		return 0;
	}
	switch(len) {
	case 1:
		return *ptr;
	case 2:
		return *(uint16_t *)ptr;
	case 4:
		return *(uint32_t *)ptr;
	case 8:
		return *(uint64_t *)ptr;
	default:
		fprintf(stderr, "dw_read_location: invalid read len: %u\n",
		    (unsigned)len);
		break;
	}
	*res = true;
	return 0;
}
/*
 * Takes the uint8_t encoded bytes, i.e. eh_frame_hdr->fde_count_enc;
 */
static inline void
dw_byte_encoding(uint8_t encoded_byte, uint8_t *encoding, uint8_t *value)
{

	*encoding = encoded_byte & 0xf0;
	*value = encoded_byte & 0x07;

	return;
}
/*
 * encoding_byte is the byte that contains the encoding value and encoding
 * application. The encoded_value is the value that is encoded that we want
 * to decode using whatever decoding scheme indicated by the encoding_byte.
 * i.e. encoding_byte: eh_frame_hdr->fde_count_enc (Contains which type of encoding)
 *	encoded_value: eh_frame_hdr->fde_count (Contains the value to be decoded)
 */
static bool
dw_decode_pointer(elfobj_t *obj, uint8_t encoding_byte,
    uint32_t encoded_value, uint64_t pc, uint64_t *outval, uint64_t *outval2 /* optional */)
{
	struct encoding {
		uint8_t encoding;
		uint8_t value;
	} encoding;

	int value_size;
	bool res = false;
	uint64_t text_section_vaddr;
	struct elf_section shdr;

	/*
	 * get exception header encoding encoding/value
	 */
	dw_byte_encoding(encoding_byte, &encoding.encoding, &encoding.value);
	if (encoding.value == DW_EH_PE_omit ||
	    encoding.encoding == DW_EH_PE_omit)
		return false;
	switch(encoding.value) {
	case DW_EH_PE_uleb128:
		*outval = dw_read_uleb128((uint8_t *)&encoded_value, 4);
		return true;
	case DW_EH_PE_sleb128:
		*outval = dw_read_sleb128((uint8_t *)&encoded_value, 4);
		return true;
	case DW_EH_PE_udata2:
	case DW_EH_PE_sdata2:
		value_size = 2;
		break;
	case DW_EH_PE_udata4:
	case DW_EH_PE_sdata4:
		value_size = 4;
		break;
	case DW_EH_PE_udata8:
	case DW_EH_PE_sdata8:
		value_size = 8;
		break;
	default:
		fprintf(stderr, "Unknown dwarf tag: %x\n", encoding.value);
		return false;
	}

	/*
	 * get value based on exception header encoding application
	 */
	switch(encoding.encoding) {
	case DW_EH_PE_pcrel:
		*outval = dw_read_location(obj, pc + encoded_value,
		    value_size, &res, NULL);
		if (res == false) {
			fprintf(stderr,
			    "dw_read_location: invalid len\n");
			return false;
		}
		return true;
	case DW_EH_PE_absptr:
		*outval = encoded_value;
		return true;
	case DW_EH_PE_textrel:
		/*
		 * even if there are no section headers, they will be
		 * reconstructed by now if the forensics mode is being
		 * used. Otherwise we will try to locate .text by using
		 * glibc fingerprinting found in the original_ep()
		 * function in this source file.
		 */
		if (elf_section_by_name(obj, ".text", &shdr) == false) {
			text_section_vaddr = original_ep(obj);
			if (text_section_vaddr == 0) {
				fprintf(stderr, "Unable to locate .text section\n");
				return false;
			}
		} else {
			text_section_vaddr = shdr.address;
		}
		*outval = dw_read_location(obj,
		    text_section_vaddr + encoded_value, value_size, &res, NULL);
		return res;
	case DW_EH_PE_datarel:
		encoded_value > 0x7fffffff ? encoded_value -= 0x100000000 : encoded_value;
		encoded_value += obj->eh_frame_hdr_addr;
		*outval = encoded_value;
		if (outval2 != NULL) {
			*outval2 = dw_read_location(obj,
			    obj->eh_frame_hdr_addr + 12 + pc, value_size, &res, NULL);
		}
		return res;
	case DW_EH_PE_funcrel:
	case DW_EH_PE_aligned:
	default:
		break;
	}
	return true;
}

ssize_t
dw_get_eh_frame_ranges(struct elfobj *obj)
{
	struct encoding {
		uint8_t encoding;
		uint8_t value;
	};
	size_t i;
	uint64_t pc = obj->eh_frame_hdr_addr + 8;
	uint64_t fde_table_vaddr = obj->eh_frame_hdr_addr + 12;
	bool res = false;
	struct eh_frame_hdr *eh_hdr = (struct eh_frame_hdr *)obj->eh_frame_hdr;
	struct eh_frame_vec fde_vec;
	uint64_t faddr, fsize, fde_count;

	res = dw_decode_pointer(obj, eh_hdr->fde_count_enc,
	    eh_hdr->fde_count, pc, &fde_count, NULL);
	if (res == false) {
		fprintf(stderr, "dw_decode_pointer failed\n");
		return -1;
	}
	LIST_INIT(&obj->list.eh_frame_entries);
	for (i = 0; i < fde_count; i++) {
		struct elf_eh_frame_node *eh_node;

		eh_node = malloc(sizeof(*eh_node));
		if (eh_node == NULL) {
			perror("malloc");
			return -1;
		}
		/*
		 * Read in the initial_loc, and the fde_entry_offset
		 */
		dw_read_location(obj, fde_table_vaddr + 8 * i, 8, &res, &fde_vec);
		if (res == false) {
			fprintf(stderr, "dw_read_location: %#lx failed\n",
			    fde_table_vaddr + 8 * i);
			return -1;
		}
		res = dw_decode_pointer(obj, eh_hdr->table_enc, fde_vec.initial_loc,
		    fde_vec.fde_entry_offset, &faddr, &fsize);
		if (res == false) {
			fprintf(stderr, "failed to reconstruct data from .eh_frame\n");
			return -1;
		}
		eh_node->pc_begin = faddr;
		eh_node->pc_end = faddr + fsize;
		eh_node->len = eh_node->pc_end - eh_node->pc_begin;
		LIST_INSERT_HEAD(&obj->list.eh_frame_entries, eh_node, _linkage);
	}
	obj->fde_count = fde_count;
	return true;
}

bool
sanity_check(uint64_t offset, uint64_t limit)
{
	if (offset > limit)
		return false;
	return true;
}

bool
phdr_sanity(elfobj_t *obj, void *phdr)
{
	Elf32_Phdr *phdr32;
	Elf64_Phdr *phdr64;

	switch(elf_class(obj)) {
	case elfclass32:
		phdr32 = (void *)phdr;
		if ((int32_t)phdr32->p_filesz < 0)
			return false;
		if (phdr32->p_filesz >= obj->size)
			return false;
		if (phdr32->p_offset + phdr32->p_filesz > obj->size - 1)
			return false;
		return true;
	case elfclass64:
		phdr64 = (void *)phdr;
		if ((int64_t)phdr64->p_filesz < 0)
			return false;
		if (phdr64->p_filesz >= obj->size)
			return false;
		if (phdr64->p_offset + phdr64->p_filesz > obj->size - 1)
			return false;
		return true;
	}
	return true;
}
