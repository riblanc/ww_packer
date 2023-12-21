//
// Created by riblanc on 10/11/23.
//

#include <stdlib.h>

#ifdef _DEBUG
# include "utils/debug.h"

void	__attribute__((constructor)) debug_header(void) {
	fprintf(stderr, BOLD RED "0x2A" BOLD WHITE ": WOODY-WOODPACKER - DEBUG MODE" LIGHT_GRAY " ---" RESET "\n");
}
#endif

#include "elf/elf_manager.h"
#include "woody.h"

int	main(int ac, char **av) {
	t_elf_info elf __attribute__((cleanup(elf_cleaner))) = {0};
	int ret;

	if (ac != 2)
		return EXIT_FAILURE;
	if ((ret = open_elf_file(&elf, av[1])))
		return ret;
	if ((ret = parse_elf(&elf)))
		return ret;
	if ((ret = crypt_elf(&elf)))
		return ret;
	if ((ret = inject(&elf)))
		return ret;
	return (0);
}
