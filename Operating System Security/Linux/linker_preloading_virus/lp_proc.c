#include "lp_interp.h"
#include "lp_debug.h"

static void
lp_proc_filepath(struct lp_ctx *ctx)
{
	int pid = getpid();
	char tmp[PATH_MAX + 1];
	
	snprintf(tmp, PATH_MAX, "/proc/%d/exe", pid);
	ctx->proc.exec_proc_path = lp_strdup(tmp);
	lp_debug("Set exec_proc_path: %s\n", ctx->proc.exec_proc_path);
	return;
}

bool
lp_set_procinfo(struct lp_ctx *ctx)
{
	struct lp_auxv_entry entry;
	lp_auxv_iterator_t auxv_iter;
	struct lp_proc *proc = &ctx->proc;
	int argc, i;
	
	/*
	 * Retrive values from the auxiliary vector
	 */
	if (lp_auxv_iterator_init(ctx, &auxv_iter) == false) {
		lp_debug("No auxiliary vector found\n");
		return false;
	}
	while(lp_auxv_iterator_next(&auxv_iter, &entry) == LP_ITER_OK) {
		switch(entry.type) {
		case AT_EXECFN:
			lp_debug("AT_EXECFN: %s\n", entry.string);
			proc->exec_file = lp_strdup((const char *)entry.string);
			break;
		case AT_PAGESZ:
			lp_debug("AT_PAGESZ: %#lx\n", entry.value);
			proc->pagesz = entry.value;
			break;
		case AT_PHDR:
			lp_debug("AT_PHDR: %#lx\n", entry.value);
			proc->phdr_vaddr = entry.value;
			break;
		case AT_PHENT:
			lp_debug("AT_PHENT: %lu\n", entry.value);
			proc->phdr_count = entry.value;
			break;
		case AT_BASE:
			lp_debug("AT_BASE: %#lx\n", entry.value);
			proc->interp_base = entry.value;
			break;
		case AT_FLAGS:
			lp_debug("AT_FLAGS: %#lx\n", entry.value);
			proc->flags = entry.value;
			break;
		case AT_ENTRY:
			proc->entrypoint = entry.value;
			lp_debug("AT_ENTRY: %#lx\n", entry.value);
			break;
		case AT_UID:
			proc->uid = entry.value;
			break;
		case AT_EUID:
			proc->euid = entry.value;
			break;
		case AT_GID:
			proc->gid = entry.value;
			break;
		case AT_EGID:
			proc->egid = entry.value;
			break;
		case AT_SECURE:
			proc->secure = entry.value;
			break;
		case AT_RANDOM:
			proc->random = entry.value;
			break;
		}
	}
	lp_proc_filepath(ctx);
}
