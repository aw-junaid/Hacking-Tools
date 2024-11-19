/*
 * lp_parse_exec.c - This is responsible for parsing the target ELF executable
 * within the process.
 */
#include "lp_interp.h"
#include "lp_debug.h"
#include "libelfmaster.h"

bool
lp_parse_elf(struct lp_ctx *ctx)
{
	bool res;
	elf_iterator_res_t ires;
	elf_error_t error;
	uint64_t elf_load_flag = ELF_LOAD_F_STRICT;

	/*
	 */
	if (ctx->flag & INTERPX_F_LOAD_STRIPPED_ELF)
		elf_load_flag = ELF_LOAD_F_FORENSICS;

	res = elf_open_object(ctx->proc.exec_proc_path, &ctx->elfobj, ELF_LOAD_F_STRICT,
	    &error);
	if (res == false ) {
		lp_debug("elf_open_object(%s, ...) failed: %s\n", ctx->proc.exec_proc_path,
		    strerror(errno));
		return false;
	}

	strncpy(ctx->ldso_path, LDSO_PATH, PATH_MAX);
	res = elf_open_object(LDSO_PATH, &ctx->ldsobj, ELF_LOAD_F_STRICT,
	    &error);
	if (res == false) {
		lp_debug("elf_open_object(%s, ...) failed: %s\n", ctx->ldso_path, strerror(errno));
		return false;
	}
	lp_debug("Loaded ELF file: %s\n", ctx->proc.exec_proc_path);
	lp_debug("Loaded ELF file: %s\n", LDSO_PATH);
	return true;
}


