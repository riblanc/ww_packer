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

#endif //WW_PACKER_FILE_H
