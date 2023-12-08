//
// Created by riblanc on 08/12/23.
//

#include <stdio.h>

#include "utils/debug.h"
#include "utils/random.h"
#include "utils/string.h"

#include <stdlib.h>

char	*generate_key(char *dst, size_t len) {
	char display_buf[len * 2];
	char ret = 0;

	DEBUG_LOG("Generating a new key (len: %ld)", len);

	for (size_t i = 0; i < len; i++) {
		ret = ft_rand() % 127;
		dst[i] = ret;
		sprintf(display_buf + (i * 2), "%02x", dst[i] & 0xff);
	}

	printf("Generated key: 0x%.*s\n", (int)len * 2 + 2, display_buf);
	return dst;
}
