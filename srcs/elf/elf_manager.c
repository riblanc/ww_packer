//
// Created by riblanc on 11/11/23.
//

#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

#include "elf/elf_manager.h"
#include "debug.h"

void	open_elf_cleaner(t_elf_error *eerror) {
	if (eerror->error == false)
		return ;

	DEBUG_LOG("cleaning elf, eerror ("BOLD"code: "RED"%d"RESET")", eerror->code);

	if (eerror->elf->file.fd != -1) {
		DEBUG_LOG(
			"File opened, closing it ("BOLD"fd: "RED"%d"RESET")",
			eerror->elf->file.fd
		);
		close(eerror->elf->file.fd);
		eerror->elf->file.fd = -1;
	}
	return ;
}

int	open_elf_file(t_elf_info *elf, const char *filename) {
	t_elf_error eerror __attribute__((cleanup(open_elf_cleaner))) = {
		.error = false,
		.code = 0,
		.elf = elf,
	};

	elf->file.filename = filename;

	ERRNO_PROTECT(
		(elf->file.fd = open(filename, O_RDONLY | O_NONBLOCK)),
		&eerror
	);

	ERRNO_PROTECT(
		syscall(SYS_fstat, elf->file.fd, &elf->file.stat),
		&eerror
	);

	ECODE_PROTECT(
		S_ISDIR(elf->file.stat.st_mode),
		EISDIR,
		&eerror
	);

	CUSTOM_PROTECT(
		S_ISFIFO(elf->file.stat.st_mode),
		eerror,
		"Cannot read from a pipe"
	);

	CUSTOM_PROTECT(
		S_ISSOCK(elf->file.stat.st_mode),
		eerror,
		"Cannot read from a socket"
	);

	ERRNO_PROTECT(
		(elf->file.map = mmap(NULL, elf->file.stat.st_size, PROT_READ, MAP_PRIVATE, elf->file.fd, 0)),
		&eerror
	);

	/* cf. man mmap:
	 *   After the mmap() call has returned, the file descriptor, fd,
	 *   can be closed immediately without invalidating the mapping.
	*/
	close(elf->file.fd);
	elf->file.fd = -1;

	return eerror.error ? EXIT_FAILURE : EXIT_SUCCESS;
}
