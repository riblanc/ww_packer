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

typedef struct s_elf_info {			\
	Elf64_Ehdr	*header;			\
	Elf64_Phdr	*program_header;	\
	Elf64_Shdr	*section_header;	\
	struct stat	stat;				\
	int			fd;					\
} t_elf_info;

typedef struct s_elf_error  {		\
		t_elf_info	*elf;			\
		const char	*filename;		\
		char		*msg;			\
		int			code;			\
		bool		error;			\
} t_elf_error;

int error_custom_hook(t_elf_error *error);
int	error_hook(t_elf_error *err, int _errno);
int	open_elf_file(t_elf_info *elf, const char *filename);

#endif //WW_PACKER_ELF_MANAGER_H
