INTERP_PATH="/lib64/evil.ld"
GCC_OPTS=-DDEBUG -D_GNU_SOURCE -fPIC -nostdlib -c

CC=musl-gcc
all:
	$(CC) $(GCC_OPTS) lp_interp.c -o lp_interp.o
	$(CC) $(GCC_OPTS) lp_iter.c -o lp_iter.o
	$(CC) $(GCC_OPTS) lp_util.c -o lp_util.o
	$(CC) $(GCC_OPTS) lp_proc.c -o lp_proc.o
	$(CC) $(GCC_OPTS) lp_parse_elf.c -o lp_parse_elf.o
	$(CC) $(GCC_OPTS) libelfmaster.c -o libelfmaster.o
	$(CC) $(GCC_OPTS) internal.c -o internal.o
	$(CC) $(GCC_OPTS) lp_load_ldso.c -o lp_load_ldso.o
	$(CC) $(GCC_OPTS) lp_loader.c -o lp_loader.o
	$(CC) -D_GNU_SOURCE -Wl,-undefined=printf -Wl,-undefined=readdir -Wl,-undefined=opendir -T ./ld.script -static -g lp_interp.o lp_util.o \
		lp_proc.o lp_iter.o lp_parse_elf.o lp_load_ldso.o lp_loader.o \
		libelfmaster.o internal.o -o lp_interp
	mkdir -p /opt/evil_linker/
	cp lp_interp /lib64/evil.ld
tests:
	gcc -ggdb -Wl,--dynamic-linker=$(INTERP_PATH) test.c -o test
virus:
	gcc -c virus_payload.c -fno-stack-protector
	cp virus_payload.o virus_test/
	cp test virus_test/
clean:
	rm -f lp_interp *.o
