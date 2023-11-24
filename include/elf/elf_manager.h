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

#define DECLARE_ELF_INFO(__ARCH)				\
	typedef struct s_elf##__ARCH##_info {		\
		Elf##__ARCH##_Ehdr	*header;			\
		Elf##__ARCH##_Phdr	*program_header;	\
		Elf##__ARCH##_Shdr	*section_header;	\
		struct stat			stat;				\
		int					fd;					\
	} t_elf##__ARCH##_info;

# define DECLARE_ELF_ERROR(__ARCH)				\
	typedef struct s_elf##__ARCH##_error  {		\
		t_elf##__ARCH##_info	*elf;			\
		const char				*filename;		\
		char					*msg;			\
		int						code;			\
		bool					error;			\
	} t_elf##__ARCH##_error;

DECLARE_ELF_INFO(32);
DECLARE_ELF_INFO(64);
DECLARE_ELF_ERROR(32);
DECLARE_ELF_ERROR(64);

int error_custom_hook(t_elf64_error *error);
int	error_hook(t_elf64_error *err, int _errno);
int	open_elf64_file(t_elf64_info *elf, const char *filename);

void	find_entrypoint(t_elf64_info *elf);

#endif //WW_PACKER_ELF_MANAGER_H
