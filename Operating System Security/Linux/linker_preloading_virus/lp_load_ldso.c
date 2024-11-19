#include "lp_interp.h"
#include "lp_debug.h"
#include <sys/mman.h>

#define LINKER_BASE 0x600000
/*
 * Copy the data from the segment specified my 'segment' into
 * the destination buffer 'dst', using the elf_read_address function
 * which requires 'elfobj'
 */
static bool
elf_segment_copy(elfobj_t *elfobj, uint8_t *dst, struct elf_segment segment)
{
	size_t len = segment.filesz / sizeof(uint64_t);
	size_t rem = len % sizeof(uint64_t);
	uint64_t qword;
	bool res;
	uint8_t byte;
	size_t i = 0;

	lp_debug("Reading from address %#lx - %#lx\n", segment.vaddr,
	    segment.vaddr + segment.filesz);
	for (i = 0; i < segment.filesz; i += sizeof(uint64_t)) {
		res = elf_read_address(elfobj, segment.vaddr + i, &qword, ELF_QWORD);
		if (res == false) {
			lp_debug("elf_read_address failed at %#lx\n", segment.vaddr + i);
			return false;
		}
		*(uint64_t *)&dst[i] = qword;
	}
	lp_debug("i = %#lx\n", i);
	/*
	 * For the remainder of bytes (7 or less) we will read a byte
	 * at a time.
	 */
	if (rem != 0) {
		size_t i;
		lp_debug("Copying remaining bytes: %zu\n", rem);
		for (i = 0; i < rem; i++) {
			res = elf_read_address(elfobj, segment.vaddr + i, (void *)&byte, ELF_BYTE);
			if (res == false) {
				lp_debug("elf_read_address (ELF_BYTE) at: %#lx\n",
				    segment.vaddr + i);
				return false;
			}
			*(uint8_t *)&dst[i] = byte;
		}
	}
	return true;
}

static inline int
make_prot(uint32_t p_flags)
{
	int prot = 0;

	if (p_flags & PF_R)
		prot |= PROT_READ;
	if (p_flags & PF_W)
		prot |= PROT_WRITE;
	if (p_flags & PF_X)
		prot |= PROT_EXEC;
	return prot;
}

static size_t
total_mapping_size(elfobj_t *obj)
{
	elf_iterator_res_t ires;
	elf_segment_iterator_t phdr_iter;
	struct elf_segment phdr;
	int count = 0;
	uint64_t first_vaddr, last_vaddr, last_memsz;

	elf_segment_iterator_init(obj, &phdr_iter);
	while(elf_segment_iterator_next(&phdr_iter, &phdr) == ELF_ITER_OK) {
		if (phdr.type != PT_LOAD)
			continue;
		if (count++ == 0) {
			lp_debug("first_vaddr: %#lx\n", phdr.vaddr);
			lp_debug("first_memsz: %#lx\n", phdr.memsz);
			first_vaddr = phdr.vaddr;
		} else {
			last_vaddr = phdr.vaddr;
			last_memsz = phdr.memsz;
		}
	}
	return last_vaddr + last_memsz - ELF_PAGESTART(first_vaddr);
}

/*
 * Adapted from /usr/src/linux/fs/binfmt_elf.c
 */
bool
lp_load_elf_interp(struct lp_ctx *ctx, uint64_t *entry_point)
{
	elfobj_t *elfobj = &ctx->elfobj;
	elfobj_t *ldsobj = &ctx->ldsobj;
	uint64_t vaddr;
	bool res;
	elf_iterator_res_t ires;
	elf_segment_iterator_t phdr_iter;
	struct elf_segment phdr;
	size_t total_size;
	bool load_addr_set = false;
	uint64_t elf_bss = 0, last_bss = 0;
	uint64_t load_addr, map_addr, mapped;
	uint64_t last_vaddr, last_memsz, last_offset, base_vaddr;
	uint8_t *mem;
	struct elf_symbol sym;

	/*
	 * Currently we only expect for /lib64/ld-linux.so to be loaded
	 * as the dynamic linker, which is always a position independent
	 * shared library.
	 */
	assert(elf_type(ldsobj) == ET_DYN);

	total_size = total_mapping_size(ldsobj);
	elf_segment_iterator_init(ldsobj, &phdr_iter);
	for (;;) {
		uint32_t elfprot, bss_prot;

		ires = elf_segment_iterator_next(&phdr_iter, &phdr);
		if (ires == ELF_ITER_DONE)
			break;
		if (ires == ELF_ITER_ERROR)
			return false;
		if (phdr.type != PT_LOAD)
			continue;
		elfprot = make_prot(phdr.flags);
		if (elfprot & PROT_READ)
			lp_debug("PROT_READ\n");
		if (elfprot & PROT_WRITE)
			lp_debug("PROT_WRITE\n");
		if (elfprot & PROT_EXEC)
			lp_debug("PROT_EXEC\n");
		if (phdr.offset == 0) {
			/*
			 * Create an anonymous memory region large enough to house the entire dynamic
			 * linker, and then unmap all of it except for the portion needed for the
			 * first segment. This will ensure that subsequent mmap's of the other LOAD
			 * segments are contiguous in address space.
			 */
			base_vaddr = LINKER_BASE;
			lp_debug("Attempting to map %#lx\n", base_vaddr);
			mem = mmap((void *)base_vaddr, phdr.memsz, PROT_READ|PROT_WRITE, MAP_PRIVATE|
			    MAP_ANONYMOUS|MAP_FIXED, -1, 0);
			lp_debug("Mapped at %p\n", mem);

			if (mem == MAP_FAILED) {
				perror("mmap");
				exit(EXIT_FAILURE);
			}
			mem = (uint8_t *)base_vaddr;
			lp_debug("Mapped segment at %p\n", mem);
			res = elf_segment_copy(ldsobj, mem, phdr);
			if (res == false) {
				lp_debug("elf_segment_copy(%p, %p, %p) failed\n",
				    ldsobj, mem, &phdr);
				return false;
			}
			if (mprotect(mem,
			    phdr.memsz + 4095 & ~4095, elfprot) < 0) {
				lp_debug("mprotect: %s\n", strerror(errno));
				return false;
			}
			lp_debug("mprotect succeeded at %p %zu bytes\n", mem, phdr.memsz);
			last_vaddr = base_vaddr + phdr.vaddr;
			last_memsz = phdr.memsz;
			last_offset = phdr.offset;
			continue;
		}
		load_addr = base_vaddr + phdr.vaddr;
		load_addr = ELF_PAGESTART(load_addr);
		lp_debug("Mapping segment: %#lx of size %zu\n", load_addr, phdr.memsz);
		unsigned long memsz = phdr.memsz;
		if (phdr.flags == (PF_R|PF_W)) {
			/*
			 * If this is the data segment map enough room for
			 * the .bss segment.
			 */
			memsz = ELF_PAGEALIGN(phdr.memsz, 0x1000) +
			    ELF_PAGEALIGN(phdr.memsz - phdr.filesz, 0x1000);
		}
		mem = mmap((void *)load_addr, memsz, PROT_READ|PROT_WRITE,
		    MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED, -1, 0);
		if (mem == MAP_FAILED) {
			lp_debug("mmap failed: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		elf_segment_copy(ldsobj, &mem[phdr.vaddr & (PAGE_SIZE - 1)], phdr);
		if (mprotect(mem, phdr.memsz + 4095 & ~4095,
		    elfprot) < 0) {
			lp_debug("mprotect: %s\n", strerror(errno));
			return false;
		}
		last_vaddr = load_addr;
		last_memsz = phdr.memsz;
		last_offset = phdr.offset;
	}
	if (elf_symbol_by_name(ldsobj, "_dl_start", &sym) == false) {
		lp_debug("Could not find __dl_start symbol, trying e_entry\n");
		*entry_point = base_vaddr + elf_entry_point(ldsobj);
	} else {
		*entry_point = base_vaddr + sym.value;
	}
	/*
	 * Update auxilary vector with base address information
	 * for the ldso.
	 */
	lp_iterator_res_t lp_res;
	struct lp_auxv_iterator iter;
	struct lp_auxv_entry entry;
	if (lp_auxv_iterator_init(ctx, &iter) == false) {
		lp_debug("lp_auxv_iterator_init() failed\n");
		return false;
	}
	for (;;) {
		lp_res = lp_auxv_iterator_next(&iter, &entry);
		if (lp_res == LP_ITER_ERROR) {
			lp_debug("lp_auxv_iterator_next() failed\n");
			return false;
		}
		if (lp_res == LP_ITER_DONE)
			break;
		if (entry.type == AT_BASE) {
			lp_debug("AT_BASE is set to: %#lx\n", entry.value);
			lp_auxv_set_value(&iter, base_vaddr);
			lp_debug("Set AT_BASE auxiliary value to %#lx\n", base_vaddr);
		}
	}
	lp_debug("Successfully loaded dynamic linker\n");
	lp_debug("Dynamic linker entry point: %#lx\n", *entry_point);
	return true;
}
