//
// Created by riblanc on 10/11/23.
//

#ifndef WW_PACKER_ELF_MANAGER_H
#define WW_PACKER_ELF_MANAGER_H

#include <elf.h>
#include <unistd.h>

#include "utils/file.h"

typedef char * ptr_t;

typedef struct s_elf_info {
	Elf64_Ehdr	*header;
	Elf64_Phdr	*ph_table;
	size_t  	pht_size;
	Elf64_Shdr	*sh_table;
	size_t		sht_size;
	Elf64_Off	entrypoint;
	Elf64_Phdr	*exec_segment;
	size_t		padding;
	size_t		padding_size;
	t_file		file;
} t_elf_info;

int		open_elf_file(t_elf_info *elf, const char *filename);
int		parse_elf(t_elf_info *elf);
void	elf_cleaner(t_elf_info *elf);

#endif //WW_PACKER_ELF_MANAGER_H
