//
//
//
# include <stdio.h>
# include <sys/mman.h>
# include <stddef.h>

# include "elf/elf_manager.h"
# include <elf.h>

void	find_entrypoint(t_elf64_info *elf){
	printf("Premiere etape : elf_fd ? %d // stat.st_size ? %ld\n", elf->fd, elf->stat.st_size);
//	size_t	random_size_lol = 64;
	void	*jsp_ptr = mmap(NULL,elf->stat.st_size, PROT_READ, MAP_PRIVATE, elf->fd, 0);
//	char *addr;
//	addr =  (char *)(jsp_ptr);

	printf("/**********************************************/\n\t\tENTRYPOINT STUFFS\n");
	printf("entrypoint_offset = %#lx\n", ((Elf64_Ehdr *)jsp_ptr)->e_entry);
	printf("/**********************************************/\n\t\tSECTION HEADER STUFFS\n");
	printf("section header table_offset = %#lx\n", ((Elf64_Ehdr *)jsp_ptr)->e_shoff);
	printf("section header table entry sz = %d\n", ((Elf64_Ehdr *)jsp_ptr)->e_shentsize);
	printf("/**********************************************/\n\t\tPROGRAM HEADER STUFFS\n");
	printf("Program header table_offset = %#lx\n", ((Elf64_Ehdr *)jsp_ptr)->e_phoff);
	printf("program header table entry sz = %d\n", ((Elf64_Ehdr *)jsp_ptr)->e_phentsize);
/*	for (int i = 0;  i < 7; i++){
		printf("indice : %d // addr : %p // ???? : %d\n", i, &(jsp_ptr + i), i);
	}*/
	return ;
}
