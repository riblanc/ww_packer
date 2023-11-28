//
// Created by riblanc on 10/11/23.
//

#ifndef WW_PACKER_ELF_MANAGER_H
#define WW_PACKER_ELF_MANAGER_H

#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

#include "error.h"

typedef struct s_file {
	void 		*map;
	const char	*filename;
	struct stat	stat;
	int			fd;
} t_file;

typedef struct s_elf_info {
	Elf64_Ehdr	*header;
	Elf64_Phdr	*ph_table;
	size_t  	pht_size;
	Elf64_Shdr	*sh_table;
	size_t		sht_size;
	t_file		file;
} t_elf_info;

typedef struct s_elf_error  {
	t_elf_info	*elf;
	char		*msg;
	int			code;
	bool		error;
} t_elf_error;

int error_custom_hook(t_elf_error *error);
int	error_hook(t_elf_error *err, int _errno);
int	open_elf_file(t_elf_info *elf, const char *filename);
int parse_elf(t_elf_info *elf);

#endif //WW_PACKER_ELF_MANAGER_H
