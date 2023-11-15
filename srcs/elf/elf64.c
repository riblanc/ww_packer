//
// Created by riblanc on 11/11/23.
//

#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "elf/elf_manager.h"
#include "debug.h"

void	open_elf64_cleaner(t_elf64_error *eerror) {
	if (eerror->error == false)
		return ;

	DEBUG_LOG("cleaning elf, eerror ("BOLD"code: "RED"%d"RESET")", eerror->code);

	if (eerror->elf->fd != -1) {
		DEBUG_LOG("File opened, closing it ("BOLD"fd: "RED"%d"RESET")", eerror->elf->fd);
		close(eerror->elf->fd);
	}
	return ;
}

int	open_elf64_file(t_elf64_info *elf, const char *filename) {
	t_elf64_error eerror __attribute__((cleanup(open_elf64_cleaner))) = {
		.error = false,
		.code = 0,
		.filename = filename,
		.elf = elf
	};

	ERRNO_PROTECT(
		(elf->fd = open(filename, O_RDONLY | O_NONBLOCK)),
		&eerror
	);

	ERRNO_PROTECT(
		syscall(SYS_fstat, elf->fd, &elf->stat),
		&eerror
	);

	ECODE_PROTECT(
		S_ISDIR(elf->stat.st_mode),
		EISDIR,
		&eerror
	);

	CUSTOM_PROTECT(
		S_ISFIFO(elf->stat.st_mode),
		eerror,
		"Cannot read from a pipe"
	);

	return eerror.error ? EXIT_FAILURE : EXIT_SUCCESS;
}
