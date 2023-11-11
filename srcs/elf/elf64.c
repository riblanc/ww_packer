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

void    close_file(int *fd) {
    DEBUG_LOG("fd = %d", *fd);
    if (*fd >= 0) close(*fd);
}

int    open_elf64_file(t_elf64_info *elf, const char *filename) {
    int fd __attribute__((cleanup(close_file))) = -1;

    DEBUG_LOG("filename = %s", filename);
    DEBUG_WARN("filename = %s", filename);
    DEBUG_ERROR("filename = %s", filename);
    DEBUG_FATAL("filename = %s", filename);

    ERRNO_PROTECT(
        (elf->fd = fd = open(filename, O_RDONLY | O_NONBLOCK)),
        filename
    );

    ERRNO_PROTECT(
        syscall(SYS_fstat, elf->fd, &elf->stat),
        filename
    );

    ECODE_PROTECT(
        S_ISDIR(elf->stat.st_mode),
        EISDIR,
        filename
    );

    CUSTOM_PROTECT(
        S_ISFIFO(elf->stat.st_mode),
        filename,
        "FIFOs are not supported"
    );

    return EXIT_SUCCESS;
}
