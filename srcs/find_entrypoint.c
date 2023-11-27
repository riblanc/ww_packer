//
//
//
# include <stdio.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <stddef.h>

# include "elf/elf_manager.h"
# include <elf.h>
# include "debug.h"

void	find_entrypoint(t_elf64_info *elf){
	Elf64_Ehdr hdr;
	Elf64_Shdr shdr;
	printf("Premiere etape : elf_fd ? %d // stat.st_size ? %ld\n", elf->fd, elf->stat.st_size);
	void	*jsp_ptr = mmap(NULL,elf->stat.st_size, PROT_READ, MAP_PRIVATE, elf->fd, 0);


	(void) hdr;
	(void)	shdr;
	if (jsp_ptr == NULL){
		printf("mmap failed\n");
		exit (EXIT_FAILURE);
	}
//	addr =  (char *)(jsp_ptr);

// Using memcpy to read elf header

// https://stackoverflow.com/questions/10352768/reading-elf-header-in-c
	printf("/**********************************************/\n\t\tENTRYPOINT STUFFS\n");
	printf("entrypoint_offset = %#lx\n", ((Elf64_Ehdr *)jsp_ptr)->e_entry);
	printf("/**********************************************/\n\t\tSECTION HEADER STUFFS\n");
	printf("section header table_offset = %#lx\n", ((Elf64_Ehdr *)jsp_ptr)->e_shoff);
	printf("section header table entry sz = %d\n", ((Elf64_Ehdr *)jsp_ptr)->e_shentsize);
	printf("/**********************************************/\n\t\tPROGRAM HEADER STUFFS\n");
	printf("Program header table_offset = %#lx\n", ((Elf64_Ehdr *)jsp_ptr)->e_phoff);
	printf("program header table entry sz = %d\n", ((Elf64_Ehdr *)jsp_ptr)->e_phentsize);
	printf("/**********************************************/\n\t\tSECTION HEADER STRUCT STUFFS\n");
	printf("entrypoint section header name ?????? %d\n", (((Elf64_Shdr *)(((Elf64_Ehdr *)jsp_ptr)->e_entry))->sh_name));
	return ;
}
