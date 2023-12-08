//
// Created by riblanc on 11/11/23.
//

#include <stdio.h>
#include <string.h>

#include "utils/error.h"
#include "utils/file.h"

int error_custom_hook(t_error *err) {
	t_file *file = (t_file *)err->data;

	err->error = true;
	if (!file)
		return 1;
	if (file->filename)
		fprintf(stderr, "%s: '%s': Error: %s\n", __EXEC_NAME__, file->filename, err->msg);
	else
		fprintf(stderr, "%s: Error: %s\n", __EXEC_NAME__, err->msg);
	return 1;
}

int	error_hook(t_error *err, int _errno) {
	err->code = _errno;
	err->msg = strerror(_errno);
	error_custom_hook(err);
	return 1;
}
