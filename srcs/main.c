//
// Created by riblanc on 10/11/23.
//

#include <stdio.h>
#include <stdlib.h>
#include "elf/elf_manager.h"

#ifdef _DEBUG
# include "debug.h"

void	__attribute__((constructor)) debug_header(void) {
	fprintf(stderr, BOLD RED "0x2A" BOLD WHITE ": WOODY-WOODPACKER - DEBUG MODE" LIGHT_GRAY " ---" RESET "\n");
}
#endif

int	main(int ac, char **av) {
	t_elf64_info elf;
	int ret;

	if (ac != 2){
		fprintf(stderr, "Error: wrong number of arguments\n");
		exit(EXIT_FAILURE);
	}
	if ((ret = open_elf64_file(&elf, av[1])))
		exit(ret);
	find_entrypoint(&elf);
	return (0);
}
