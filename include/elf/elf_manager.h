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
	Elf64_Off	entrypoint;
	Elf64_Off	text_section;
	size_t		text_size;
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

typedef char * ptr_t;

/*
 * This macro is used to check whether a pointer of a given type can fit in a file's memory map.
 */
#define IS_VALID_PTR(__PTR, __F, __TYPE) 														\
		({																						\
			ptr_t	p = (ptr_t)(__PTR); 														\
			t_file	*f = (__F); 																\
			int		ret = 1; 																	\
  																								\
  			DEBUG_LOG(                                       									\
			  "%s (%ld bytes), file map: [%p, %p], ptr: [%p, %p]", 								\
			  #__TYPE, sizeof(__TYPE), 															\
			  f->map, (ptr_t)f->map + f->stat.st_size, 											\
			  p, (ptr_t)p + sizeof(__TYPE) 														\
			);																					\
  																								\
			if (p < (ptr_t)f->map || p + sizeof(__TYPE) > (ptr_t)f->map + f->stat.st_size)		\
				ret = 0;																		\
			ret; 																				\
		})

#endif //WW_PACKER_ELF_MANAGER_H
