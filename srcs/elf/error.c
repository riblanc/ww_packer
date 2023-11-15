//
// Created by riblanc on 11/11/23.
//

#include <stdio.h>
#include <string.h>

#include "elf/error.h"
#include "elf/elf_manager.h"
#include "woody.h"

int error_custom_hook(t_elf64_error *err) {
	err->error = true;
	if (err->filename)
		fprintf(stderr, "%s: '%s': Error: %s\n", __EXEC_NAME__, err->filename, err->msg);
	else
		fprintf(stderr, "%s: Error: %s\n", __EXEC_NAME__, err->msg);
	return 1;
}

int	error_hook(t_elf64_error *err, int _errno) {
	err->code = _errno;
	err->msg = strerror(_errno);
	error_custom_hook(err);
	return 1;
}
