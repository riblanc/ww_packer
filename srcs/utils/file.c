//
// Created by riblanc on 12/6/23.
//

#include "utils/file.h"
#include "utils/error.h"
#include "debug.h"

static void map_file_cleaner(t_error *err) {
	t_file *file = (t_file *) err->data;

	if (!err->error)
		return;

	if (file->map) {
		munmap(file->map, file->stat.st_size);
		file->map = NULL;
	}

	if (file->fd != -1) {
		close(file->fd);
		file->fd = -1;
	}
}

/*
 * The function map_file() is used to map a file into memory.
 * It takes the file pointer, already opened with regular_file_open(),
 * It takes the prot and flags arguments, which are the same as the ones
 * used by mmap(2).
 * It fills the map field of the given file structure with the mapped file.
 * It returns EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
int 	map_file(t_file *file, int prot, int flags) {
	t_error error __attribute__((cleanup(map_file_cleaner))) = {
		.error = false,
		.code = 0,
		.data = file
	};

	// TODO: check if st_size > 0, else throw an error
	DEBUG_LOG("Mapping file (length: %ld, prot: %d, flags: %d)", file->stat.st_size, prot, flags);
	ERRNO_PROTECT(
		(file->map = mmap(NULL, file->stat.st_size, prot, flags, file->fd, 0)),
		&error
	);
	return error.error ? EXIT_FAILURE : EXIT_SUCCESS;
}

static void regular_file_cleaner(t_error *err) {
	t_file *file = (t_file *) err->data;

	if (!(err->error && file->fd != -1))
		return ;

	close(file->fd);
	file->fd = -1;
}

/*
 * The function open_regular_file() is used to open a regular file.
 * It takes a filename, a file structure pointer, the flags and the mode to use.
 * The flags and mode arguments are the same as the ones used by open(2).
 * It fills the file structure with the file descriptor, the filename and the stat structure.
 * It returns EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
int 	open_regular_file(const char *filename, t_file *file, int flags, mode_t mode) {
	t_file _file = { .map = NULL, .filename = filename, .fd = -1, .stat = {0} };

	t_error error __attribute__((cleanup(regular_file_cleaner))) = {
		.error = false,
		.code = 0,
		.data = &_file
	};

	DEBUG_LOG("Opening file %s", _file.filename);
	ERRNO_PROTECT(
		(_file.fd = open(_file.filename, flags, mode)),
		&error
	);

	DEBUG_LOG("Getting file stat");
	ERRNO_PROTECT(
		syscall(SYS_fstat, _file.fd, &_file.stat),
		&error
	);

	DEBUG_LOG("Checking file type");
	ECODE_PROTECT(
		S_ISDIR(_file.stat.st_mode),
		EISDIR,
		&error
	);

	CUSTOM_PROTECT(
		S_ISFIFO(_file.stat.st_mode),
		&error,
		"Cannot read from a pipe"
	);

	CUSTOM_PROTECT(
		S_ISSOCK(_file.stat.st_mode),
		&error,
		"Cannot read from a socket"
	);

	*file = _file;
	DEBUG_LOG("fd: %d, size: %ld", file->fd, file->stat.st_size);
	return error.error ? EXIT_FAILURE : EXIT_SUCCESS;
}

/*
 * The function cmp_file() is used to compare two files.
 * It takes two file structure pointers.
 * It returns true if the files are different, false if they are the same.
 *
 * c.f. https://stackoverflow.com/questions/49943012/how-to-identify-if-two-file-diescriptors-point-to-the-same-file
 */
int 	cmp_file(t_file *file1, t_file *file2) {
	return !(file1->stat.st_dev == file2->stat.st_dev && file1->stat.st_ino == file2->stat.st_ino);
}

/*
 * The function clone_file() is used to clone a file.
 * It takes a filename, a destination file structure pointer and a source file structure pointer.
 * It creates a new file with the given filename, and copies the content of the source file into it.
 * It returns EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
int 	clone_file(const char *new_filename, t_file *dst, t_file *src) {
	t_file _dst = { .map = NULL, .filename = new_filename, .fd = -1, .stat = {0} };

	t_error error __attribute__((cleanup(regular_file_cleaner))) = {
		.error = false,
		.code = 0,
		.data = &_dst
	};

	syscall(SYS_stat, _dst.filename, &_dst.stat);

	// If the files are the same, we don't need to copy anything
	if (!cmp_file(src, &_dst)) {
		*dst = *src;
		return EXIT_SUCCESS;
	}

	if (open_regular_file(new_filename, &_dst, O_RDWR | O_CREAT | O_TRUNC, src->stat.st_mode))
		return EXIT_FAILURE;

	DEBUG_LOG("Copying file, (fd: %d, size: %ld) -> (%d)", src->fd, src->stat.st_size, _dst.fd);
	ERRNO_PROTECT(
		syscall(SYS_copy_file_range, src->fd, NULL, _dst.fd, NULL, src->stat.st_size, 0),
		&error
	);

	ERRNO_PROTECT(
		syscall(SYS_fstat, _dst.fd, &_dst.stat),
		&error
	);

	*dst = _dst;
	return error.error ? EXIT_FAILURE : EXIT_SUCCESS;
}