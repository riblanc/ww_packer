//
// Created by riblanc on 08/12/23.
//

#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>

#include "utils/debug.h"
#include "utils/error.h"
#include "utils/random.h"
#include "elf/elf_manager.h"
#include "woody.h"


static unsigned char	*generate_key(unsigned char *dst, size_t len) {
	char display_buf[len * 2];
	char ret = 0;

	DEBUG_LOG("Generating a new key (len: %ld)", len);

	for (size_t i = 0; i < len; i++) {
		ret = ft_rand() % 127;
		dst[i] = ret;
		sprintf(display_buf + (i * 2), "%02x", dst[i] & 0xff);
	}

	printf("Generated key: "BOLD BLUE"0x%.*s"RESET"\n", (int)len * 2 + 2, display_buf);
	return dst;
}

static long	get_timestamp(void) {
	struct timeval tp = {0};
	long timestamp = 0;

	if (syscall(SYS_gettimeofday, &tp, NULL) == -1)
		return 42;
	timestamp = tp.tv_sec * 1e3 + tp.tv_usec / 1e3;
	return timestamp;
}

int 	crypt_elf(t_elf_info *elf) {
	t_error error = { .data = &elf->file };

	unsigned char *key = __bytecode + __bytecode_len - 32;

	ft_srand(get_timestamp());
	generate_key(key, 32);

	ptr_t const start = (ptr_t)elf->file.map + elf->exec_segment->p_offset;
	ptr_t const end = start + elf->exec_segment->p_filesz;

	DEBUG_LOG("Elf encryption (start: %#lx, end: %#lx)",
		start - (ptr_t)elf->file.map,
		end - (ptr_t)elf->file.map
	);
	CUSTOM_PROTECT(
		!IS_VALID_PTR(start, &elf->file, unsigned char[elf->exec_segment->p_filesz]),
		&error,
		"Segment is not valid"
	);

	for (ptr_t ptr = start; ptr < end; ptr++) {
		*ptr ^= key[(ptr - start) % 32];
	}

	return EXIT_SUCCESS;
}
