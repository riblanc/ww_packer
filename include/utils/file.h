//
// Created by riblanc on 12/6/23.
//

#ifndef WW_PACKER_FILE_H
# define WW_PACKER_FILE_H

# include <sys/mman.h>
# include <sys/stat.h>
# include <sys/syscall.h>
# include <sys/types.h>
# include <fcntl.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>

# include "utils/debug.h"

typedef struct s_file {
	void 		*map;
	const char	*filename;
	struct stat	stat;
	int			fd;
} t_file;

int 	open_regular_file(const char *filename, t_file *file, int flags, mode_t mode);
int 	map_file(t_file *file, int prot, int flags);
int 	clone_file(const char *new_filename, t_file *dst, t_file *src);
int 	cmp_file(t_file *file1, t_file *file2);

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


#endif //WW_PACKER_FILE_H
