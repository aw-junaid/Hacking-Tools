#include "lp_interp.h"
#include "lp_debug.h"
#include <sys/mman.h>

/*
 * The purpose of this code is to take a relocatable object,
 * and turn it into a runtime executable module. This means
 * that we must organize SHF_ALLOC and SHF_ALLOC|SHF_ sections into the text
 * segment, and SHF_WRITE sections into the data segment.
 */

/*
 * Our custom PLT stubs are a simple IP relative indirect
 * JMP into the global offset table.
 * i.e. jmp *0x0(%rip)
 * NOTE: Our linker uses strict linking.
 */
uint8_t plt_stub[6] = "\xff\x25\x00\x00\x00\x00";

static inline void
transfer_to_module(struct evil_linker *linker, uint64_t entry)
{
	__asm__ __volatile__ ("mov %0, %%rax\n"
			      "call %%rax" :: "r" (entry));
	return;
}
/*
 * Module entry point. Lookup symbol "main"
 */
static bool
module_entrypoint(struct evil_linker *linker, uint64_t *entry)
{
	struct elf_symbol symbol;

	if (elf_symbol_by_name(&linker->elfobj, "main", &symbol) == false) {
		lp_debug("elf_symbol_by_name failed\n");
		return false;
	}
	lp_debug("Module text: %#lx\n", linker->text_vaddr);
	*entry = linker->text_vaddr + symbol.value;
	return true;
}

/*
 * Our custom interpreter is built with musl-libc. Lets
 * locate the path to our interpreter (i.e. /home/elfmaster/lp_interp)
 * and resolve the modules external library calls to the corresponding
 * musl-libc functions in memory.
 */
static bool
resolve_pltgot_entries(struct evil_linker *linker)
{
	uint64_t gotaddr;
	uint64_t *GOT;
	char interp_path[PATH_MAX];
	elfobj_t self;
	elf_error_t error;
	elf_relocation_iterator_t rel_iter;
	struct elf_relocation rel;
	struct elf_symbol symbol;
	int i;

	if (elf_open_object(EVIL_LINKER_PATH, &self, ELF_LOAD_F_STRICT,
	    &error) == false) {
		lp_debug("elf_open_object(%s, ...) failed: %s\n",
		    "/proc/self/exe", elf_error_msg(&error));
		return false;
	}

	i = 0;
	gotaddr = linker->data_vaddr + linker->pltgot_off;
	/*
	 * Here we are using the relocation iterator to retrieve the PLT32
	 * relocations from the loaded module, and then resolve the symbols
	 * associated with them from our own binary "lp_interp" which has
	 * musl-libc code statically linked within it. NOTE: gcc may also
	 * create PLT relocations for function calls that are local to the
	 * module itself, so first try to resolve symbols in the module before we
	 * check within lp_interp itself. 
	 */
	elf_relocation_iterator_init(&linker->elfobj, &rel_iter);
	while (elf_relocation_iterator_next(&rel_iter, &rel) == ELF_ITER_OK) {
		if (rel.type != R_X86_64_PLT32)
			continue;
		if ((elf_symbol_by_name(&linker->elfobj, rel.symname, &symbol) == true) &&
		    symbol.type == STT_FUNC) {
			lp_debug("Setting [%#lx] GOT entry '%s' to %#lx\n", gotaddr,
			    rel.symname, symbol.value);
			GOT = (uint64_t *)gotaddr;
			*GOT = symbol.value + linker->text_vaddr;
			gotaddr += sizeof(uint64_t);
			continue;
		}
		if (elf_symbol_by_name(&self, rel.symname, &symbol) == false) {
	    	    lp_debug("Could not resolve symbol '%s'."
	            " runtime-linkage failure\n", rel.symname);
			return false;
		}
		lp_debug("Setting [%#lx] GOT entry '%s' to %#lx\n", gotaddr, rel.symname,
	    	    symbol.value);
		GOT = (uint64_t *)gotaddr;
		*GOT = symbol.value;
		gotaddr += sizeof(uint64_t);
	}
	return true;
}

static bool
patch_plt_stubs(struct evil_linker *linker)
{
	size_t i = 0;
	struct lp_plt_entry *current;
	uint8_t *stub;
	uint64_t gotaddr, pltaddr, gotoff;

	TAILQ_FOREACH(current, &linker->tailq.plt_list, _linkage) {
		stub = &linker->text_mem[linker->plt_off + i * sizeof(plt_stub)];
		gotaddr = linker->data_vaddr + linker->pltgot_off + i * sizeof(uint64_t);
		pltaddr = linker->text_vaddr + (linker->plt_off + i * sizeof(plt_stub));
		gotoff = gotaddr - pltaddr - sizeof(plt_stub);
		*(uint32_t *)&stub[2] = gotoff;
		i++;
		lp_debug("SYMNAME: %s PLTADDR: %#lx GOTADDR: %#lx GOTOFF: %#lx\n", current->symname, pltaddr, gotaddr, gotoff);
		lp_debug("Fixedup PLT stub with GOT offset: %#lx\n", gotoff);
	}
	return true;
}

static bool
get_section_mapping(struct evil_linker *linker, char *shdrname, struct lp_section_mapping *smap)
{
	struct lp_section_mapping *current;

	TAILQ_FOREACH(current, &linker->tailq.section_maplist, _linkage) {
		if (strcmp(shdrname, current->name) != 0)
			continue;
		memcpy(smap, current, sizeof(*smap));
		return true;
	}
	return false;
}

bool
apply_relocation(struct evil_linker *linker, struct elf_relocation rel)
{
	struct lp_plt_entry *current = NULL;
	struct lp_section_mapping *smap_current;
	struct lp_section_mapping smap;
	uint8_t *rel_unit;
	uint64_t symval;
	uint64_t rel_addr;
	uint32_t rel_val;
	struct elf_symbol symbol;

	char *shdrname = strrchr(rel.shdrname, '.');
	if (shdrname == NULL) {
		lp_debug("strrchr failed\n");
		return false;
	}
	if (get_section_mapping(linker, shdrname, &smap) == false) {
		lp_debug("Failed to retrieve section data for %s\n", rel.shdrname);
		return false;
	}
	lp_debug("Successfully retrieved section mapping for %s\n", shdrname);

	switch(rel.type) {
	case R_X86_64_PLT32: /* computation: L + A - P */
		TAILQ_FOREACH(current, &linker->tailq.plt_list, _linkage) {
			if (strcmp(rel.symname, current->symname) != 0)
				continue;
			lp_debug("Applying PLT32 relocation for %s\n", current->symname);
			rel_unit = &linker->text_mem[smap.offset + rel.offset];
			rel_addr = linker->text_vaddr + smap.offset + rel.offset;
			rel_val = current->vaddr + rel.addend - rel_addr;
			lp_debug("rel_addr: %#lx rel_val: %#lx\n", rel_addr, rel_val);
			*(uint32_t *)&rel_unit[0] = rel_val;
			return true;
		}
		break;
	case R_X86_64_PC32: /* computation: S + A - P */
		lp_debug("Applying PC32 relocation for %s\n", rel.symname);
		if (rel.symname[0] == '.') { /* symname is a section name in this case */
			/*
			 * i.e. if rel.symname is ".eh_frame" then we must find that section
			 * mapping to get it's address, as our symbol value. therefore S =
			 * address of ".eh_frame" mapping.
			 */
			TAILQ_FOREACH(smap_current, &linker->tailq.section_maplist, _linkage) {
				if (strcmp(smap_current->name, rel.symname) != 0)
					continue;
				symval = smap_current->vaddr;
			}
		}
		rel_unit = &linker->text_mem[smap.offset + rel.offset];
		rel_addr = linker->text_vaddr + smap.offset + rel.offset;
		rel_val = symval + rel.addend - rel_addr;
		lp_debug("rel_addr: %#lx rel_val: %#lx\n", rel_addr, rel_val);
		*(uint32_t *)&rel_unit[0] = rel_val;
		return true;
	}
	return false;
}
bool
relocate_module(struct evil_linker *linker)
{
	struct elf_relocation_iterator rel_iter;
	struct elf_relocation rel;
	bool res;
	char *shdrname;

	elf_relocation_iterator_init(&linker->elfobj, &rel_iter);
	while (elf_relocation_iterator_next(&rel_iter, &rel) == ELF_ITER_OK) {
		shdrname = strrchr(rel.shdrname, '.');
		if (shdrname == NULL) {
			lp_debug("strrchr parse error");
			return false;
		}
		if (strcmp(shdrname, ".eh_frame") == 0) {
			/*
			 * We don't need to process relocations for .eh_frame. Maybe
			 * in the future for module debugging purposes.
			 */
			continue;
		}
		res = apply_relocation(linker, rel);
		if (res == false) {
			lp_debug("Failed to apply %s relocation at offset %#lx\n",
			    rel.shdrname, rel.offset);
			return false;
		}
	}
	return true;
}
/*
 * This function copies the code/data from a given section into it's
 * respective memory mapped segment (i.e. the text segment).
 * Section data is copied from elfobj to the respective memory mapped segment
 * pointed to by dst. The section data to be copied is described by
 * section. Without the segment_offset this function would not be re-entrant.
 * The segment_offset tells us at which within the segment to copy the given
 * section data to.
 */
bool
elf_section_map(elfobj_t *elfobj, uint8_t *dst, struct elf_section section,
    uint64_t *segment_offset)
{
	size_t len = section.size / sizeof(uint64_t);
	size_t rem = section.size % sizeof(uint64_t);
	uint64_t qword;
	bool res;
	size_t i = 0;

	lp_debug("Reading from offset %#lx - %#lx\n", section.offset,
	    section.offset + section.size);
	for (i = 0; i < section.size; i += sizeof(uint64_t)) {
		if (i + sizeof(uint64_t) >= section.size) {
			size_t j;
			/*
			 * If there are 7 or less remaining bytes we cannot read
			 * by QWORD and will read the remainder byte by byte
			 */
			for (j = 0; j < rem; j++) {
				lp_debug("Reading remaining byte from offset: %zu\n",
				    section.offset + i + j);
				res = elf_read_offset(elfobj, section.offset + i + j,
				    &qword, ELF_BYTE);
				if (res == false) {
					lp_debug("elf_read_offset failed at %#lx\n",
					    section.offset + i + j);
					return false;
				}
				dst[*segment_offset + i + j] = (uint8_t)qword;
			}
			break;
		}
		lp_debug("Reading qword from offset: %zu\n", section.offset + i);
		res = elf_read_offset(elfobj, section.offset + i, &qword, ELF_QWORD);
		if (res == false) {
			lp_debug("elf_read_offset failed at %#lx\n", section.offset + i);
			return false;
		}
		*(uint64_t *)&dst[*segment_offset + i] = qword;
	}
	*segment_offset += section.size;
	return true;
}

bool
calculate_data_size(struct evil_linker *linker)
{
	struct elf_section section;
	elf_section_iterator_t iter;
	struct elf_relocation rel;
	elf_relocation_iterator_t rel_iter;
	struct elf_segment segment;

	elf_section_iterator_init(&linker->elfobj, &iter);
	while (elf_section_iterator_next(&iter, &section) == ELF_ITER_OK) {
		if (section.flags == (SHF_ALLOC|SHF_WRITE)) {
			linker->data_size += section.size;
		}
	}
	linker->pltgot_off = linker->data_size;
	if (elf_section_by_name(&linker->elfobj, ".bss", &section) == false) {
		lp_debug("elf_section_by_name() failed\n");
		return false;
	}
	/*
	 * Make room for the .bss
	 */
	linker->data_size += section.size;

	elf_relocation_iterator_init(&linker->elfobj, &rel_iter);
	while (elf_relocation_iterator_next(&rel_iter, &rel) == ELF_ITER_OK) {
		if (rel.type == R_X86_64_PLT32) {
			/*
			 * Create room for the modules pltgot
			 */
			linker->data_size += sizeof(uint64_t);
			linker->pltgot_size += sizeof(uint64_t);
		}
	}

	lp_debug("Parasite data segment size: %zu\n", linker->data_size);
	return true;
}

bool
calculate_text_size(struct evil_linker *linker)
{
	struct elf_section section;
	elf_section_iterator_t iter;
	struct elf_relocation rel;
	elf_relocation_iterator_t rel_iter;

	elf_section_iterator_init(&linker->elfobj, &iter);
	while (elf_section_iterator_next(&iter, &section) == ELF_ITER_OK) {
		if (section.flags & SHF_ALLOC) {
			if (section.flags & SHF_WRITE)
				continue;
			/*
			 * Looking only for section types of AX, and A
			 */
			linker->text_size += section.size;
		}
	}
	linker->plt_off = linker->text_size;
	elf_relocation_iterator_init(&linker->elfobj, &rel_iter);
	while (elf_relocation_iterator_next(&rel_iter, &rel) == ELF_ITER_OK) {
		if (rel.type == R_X86_64_PLT32) {
			/*
			 * Create room for each PLT stub
			 */
			linker->plt_size += sizeof(plt_stub);
			linker->text_size += sizeof(plt_stub);
			linker->plt_count++;
		}
	}
	lp_debug("Parasite text segment size: %zu\n", linker->text_size);
	lp_debug("PLT Count: %zu\n", linker->plt_count);
	return true;
}

bool
create_data_image(struct evil_linker *linker)
{
	elf_section_iterator_t shdr_iter;
	struct elf_section section;
	bool res;
	size_t data_size_aligned;
	size_t off = 0;
	size_t count = 0;
	uint64_t vaddr;
	int i;

	data_size_aligned = ELF_PAGEALIGN(linker->data_size, PAGE_SIZE);
	linker->data_mem = mmap(NULL, data_size_aligned, PROT_READ|PROT_WRITE,
	    MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if (linker->data_mem == MAP_FAILED) {
		lp_debug("mmap failed: %s\n", strerror(errno));
		return false;
	}
	linker->data_vaddr = (uint64_t)linker->data_mem;
	elf_section_iterator_init(&linker->elfobj, &shdr_iter);
	while (elf_section_iterator_next(&shdr_iter, &section) == ELF_ITER_OK) {
		if (section.flags & SHF_WRITE) {
			struct lp_section_mapping *n;

			if (section.size == 0)
				continue;

			lp_debug("Attempting to map section %s(offset: %zu) into data segment"
			    " at address %p\n", section.name, off, linker->data_mem + off);
			res = elf_section_map(&linker->elfobj, linker->data_mem,
			    section, &off);
			if (res == false) {
				lp_debug("elf_section_map failed\n");
				return false;
			}
			n = malloc(sizeof(*n));
			if (n == NULL) {
				lp_debug("malloc failed\n");
				return false;
			}
			n->map_attribute = LP_SECTION_TEXTSEGMENT;
			n->vaddr = (unsigned long)linker->data_mem + count;
			n->offset = count; // offset within data segment that section lives at
			n->size = section.size;
			n->name = section.name;
			lp_debug("Inserting section to segment mapping\n");
			lp_debug("Address: %#lx\n", n->vaddr);
			lp_debug("Offset: %#lx\n", n->offset);
			lp_debug("Size: %#lx\n", n->size);
			TAILQ_INSERT_TAIL(&linker->tailq.section_maplist, n, _linkage);
			count += section.size;
			lp_debug("COUNT: %zu\n", count);
		}
	}

	return true;
}

bool
create_text_image(struct evil_linker *linker)
{
	elf_section_iterator_t shdr_iter;
	struct elf_section section;
	elf_relocation_iterator_t rel_iter;
	struct elf_relocation rel;
	bool res;
	uint8_t *mem;
	size_t text_size_aligned;
	size_t off = 0;
	size_t count = 0;
	uint64_t vaddr;
	int i;

	text_size_aligned = ELF_PAGEALIGN(linker->text_size, PAGE_SIZE);
	linker->text_mem = mmap(NULL, text_size_aligned, PROT_READ|PROT_WRITE|PROT_EXEC,
	    MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if (linker->text_mem == MAP_FAILED) {
		lp_debug("mmap failed: %s\n", strerror(errno));
		return false;
	}
	linker->text_vaddr = (uint64_t)linker->text_mem;
	elf_section_iterator_init(&linker->elfobj, &shdr_iter);
	while (elf_section_iterator_next(&shdr_iter, &section) == ELF_ITER_OK) {
		if (section.flags & SHF_ALLOC) {
			if (section.flags & SHF_WRITE) // skip if its for the data segment
				continue;
			struct lp_section_mapping *n;

			/*
			 * If we made it here then the section should be
			 * placed into the text segment :)
			 */
			lp_debug("Attempting to map section %s(offset: %zu) into text segment"
			    " at address %p\n", section.name, off, linker->text_mem + off);
			res = elf_section_map(&linker->elfobj, linker->text_mem,
			    section, &off);
			if (res == false) {
				lp_debug("elf_section_map failed\n");
				return false;
			}
			n = malloc(sizeof(*n));
			if (n == NULL) {
				lp_debug("malloc failed\n");
				return false;
			}
			n->map_attribute = LP_SECTION_TEXTSEGMENT;
			n->vaddr = (unsigned long)linker->text_mem + count;
			n->offset = count; // offset within text segment that section lives at
			n->size = section.size;
			n->name = strdup(section.name);
			if (n->name == NULL) {
				lp_debug("strdup: %s\n", strerror(errno));
				return false;
			}
			lp_debug("Inserting section to segment mapping\n");
			lp_debug("Address: %#lx\n", n->vaddr);
			lp_debug("Offset: %#lx\n", n->offset);
			lp_debug("Size: %#lx\n", n->size);
			TAILQ_INSERT_TAIL(&linker->tailq.section_maplist, n, _linkage);
			count += section.size;
			lp_debug("COUNT: %zu\n", count);
		}
	}
	lp_debug("count: %zu off: %zu\n", count, off);
	linker->plt_off = off;
	elf_relocation_iterator_init(&linker->elfobj, &rel_iter);
	for (i = 0; elf_relocation_iterator_next(&rel_iter, &rel) == ELF_ITER_OK;) {
		if (i == 0) {
			struct lp_section_mapping *n;

			n = malloc(sizeof(*n));
			if (n == NULL) {
				lp_debug("malloc: %s\n", strerror(errno));
				return false;
			}
			n->map_attribute = LP_SECTION_TEXTSEGMENT;
			n->vaddr = linker->text_vaddr + linker->plt_off;
			n->size = section.size;
			n->name = strdup(".plt");
			if (n->name == NULL) {
				lp_debug("strdup: %s\n", strerror(errno));
				return false;
			}
			TAILQ_INSERT_TAIL(&linker->tailq.section_maplist, n, _linkage);
		}
		if (rel.type != R_X86_64_PLT32)
			continue;
		/*
		 * We have a tailq list for the address/offset of each PLT entry
		 * and it's corresponding symbol.
		 */
		struct lp_plt_entry *plt;

		plt = malloc(sizeof(*plt));
		if (plt == NULL) {
			lp_debug("malloc: %s\n", strerror(errno));
			return false;
		}
		plt->symname = strdup(rel.symname);
		if (plt->symname == NULL) {
			lp_debug("strdup: %s\n", strerror(errno));
			return false;
		}
		plt->offset = linker->plt_off + i * sizeof(plt_stub);
		plt->vaddr = linker->text_vaddr + linker->plt_off + i * sizeof(plt_stub);
		TAILQ_INSERT_TAIL(&linker->tailq.plt_list, plt, _linkage);

		lp_debug("Copying PLT stub to %#lx, offset %#lx\n",
		    linker->text_vaddr + linker->plt_off + i * sizeof(plt_stub),
		    linker->plt_off + i * sizeof(plt_stub));

		memcpy(&linker->text_mem[linker->plt_off + i * sizeof(plt_stub)],
		    plt_stub, sizeof(plt_stub));
		i++;
	}
	return true;
}

/*
 * Take the relocatable object (The module), and map the respective sections
 * into either the text or data segment of our runtime module.
 */
bool
build_module_image(struct evil_linker *linker)
{
	bool res;

	res = create_text_image(linker);
}

/*
 * NOTE: const char *path: path to the ELF module
 */
 bool
lp_evil_loader(const char *path, struct evil_linker **linkerptr)
{
	struct evil_linker *linker;
	elf_error_t error;
	bool res;
	uint64_t entry;

	linker = malloc(sizeof(struct evil_linker));
	if (linker == NULL) {
		lp_debug("Malloc failed\n");
		return false;
	}
	memset(linker, 0, sizeof(*linker));
	*linkerptr = linker;

	TAILQ_INIT(&linker->tailq.section_maplist);
	TAILQ_INIT(&linker->tailq.plt_list);

	lp_debug("elf_open_object(%s, ...)\n", path);

	res = elf_open_object(path, &linker->elfobj,
	    ELF_LOAD_F_STRICT, &error);
	if (res == false) {
		lp_debug("elf_open_object(%s, ...) failed\n", path);
		return false;
	}
	if (calculate_text_size(linker) == false) {
		lp_debug("Failed to calculate .text size for parasite module\n");
		return false;
	}
	if (calculate_data_size(linker) == false) {
		lp_debug("Failed to calculate .data size for parasite module\n");
		return false;
	}
	if (create_text_image(linker) == false) {
		lp_debug("Failed to create text segment\n");
		return false;
	}
	if (create_data_image(linker) == false) {
		lp_debug("Failed to create data segment\n");
		return false;
	}
	if (relocate_module(linker) == false) {
		lp_debug("Failed to relocate module\n");
		return false;
	}
	if (patch_plt_stubs(linker) == false) {
		lp_debug("Failed to patch PLT stubs\n");
		return false;
	}
	if (resolve_pltgot_entries(linker) == false) {
		lp_debug("Failed to resolve PLTGOT entries\n");
		return false;
	}
	if (module_entrypoint(linker, &entry) == false) {
		lp_debug("Failed to get module entry point\n");
		return false;
	}
	lp_debug("Entry point address: %#lx\n", entry);
	transfer_to_module(linker, entry);
	lp_debug("Successfully executed module\n");
	return true;
}


