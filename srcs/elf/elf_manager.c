//
// Created by riblanc on 11/11/23.
//

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "elf/elf_manager.h"
#include "utils/error.h"
#include "utils/file.h"
#include "debug.h"

int	open_elf_file(t_elf_info *elf, const char *filename) {
	t_file original_file = {0};

	if (open_regular_file(filename, &original_file, O_RDWR, 0))
		return EXIT_FAILURE;
	if (clone_file("woody", &elf->file, &original_file))
		return EXIT_FAILURE;
	if (cmp_file(&elf->file, &original_file))
		close(original_file.fd);
	if (map_file(&elf->file, PROT_READ | PROT_WRITE, MAP_SHARED))
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int	get_exec_segment(t_elf_info *elf) {
	for (size_t i = 0; i < elf->pht_size; i++) {
		if (elf->ph_table[i].p_type == PT_LOAD
				&& elf->ph_table[i].p_flags == (PF_R | PF_X)){
			DEBUG_LOG(
				"EXECUTABLE SEGMENT : %#lx and its size is %#lx",
				elf->ph_table[i].p_offset, elf->ph_table[i].p_memsz
			);
			elf->exec_segment = elf->ph_table + i;
			return (0);
		}
	}
	return (1);
}

int	get_padding(t_elf_info *elf) {
	Elf64_Off current = elf->file.stat.st_size;

	for (size_t i = 0; i < elf->pht_size; i++) {
		Elf64_Off offset = elf->ph_table[i].p_offset;
		if (offset > elf->exec_segment->p_offset && offset < current)
			current = offset;
	}

	DEBUG_LOG("current is : %#lx", current);

	elf->padding = (ptr_t)elf->exec_segment->p_offset + elf->exec_segment->p_memsz;
	elf->padding_size = (ptr_t)current - elf->padding;

	DEBUG_LOG("PADDING: %p padding sz: %#lx", elf->padding, elf->padding_size);

	return (0);
}

int parse_elf(t_elf_info *elf) {
	t_error error;

	elf->header = elf->file.map;
	error.data = &elf->file;

	// Check if the file contains enough data to parse an Elf header
	CUSTOM_PROTECT(
		!IS_VALID_PTR(elf->file.map, &elf->file, Elf64_Ehdr),
		&error,
		"Corrupted File: Failed to parse Elf header"
	);

	// Check if the file is an Elf file (magic number: 0x7f 'E' 'L' 'F')
	CUSTOM_PROTECT(
		(*(int*)elf->header != 0x464c457f),
		&error,
		"Not an Elf file"
	);

	elf->ph_table = elf->file.map + elf->header->e_phoff;
	elf->pht_size = elf->header->e_phnum;

	// Check if the file contains a valid Program header table
	CUSTOM_PROTECT(
		!IS_VALID_PTR(elf->ph_table, &elf->file, Elf64_Phdr[elf->pht_size]),
		&error,
		"Corrupted File: Failed to parse Program headers"
	);

	elf->sh_table = elf->file.map + elf->header->e_shoff;
	elf->sht_size = elf->header->e_shnum;

	// Check if the file contains a valid Section header table
	CUSTOM_PROTECT(
		!IS_VALID_PTR(elf->sh_table, &elf->file, Elf64_Shdr[elf->sht_size]),
		&error,
		"Corrupted File: Failed to parse Section headers"
	);

	elf->entrypoint = elf->header->e_entry;

	// Check if the file contains a valid entrypoint
	CUSTOM_PROTECT(
		!IS_VALID_PTR((ptr_t)elf->file.map + elf->entrypoint, &elf->file, char),
		&error,
		"Corrupted File: Bad entrypoint"
	);


	// Check if the file contains a valid executable segment
	CUSTOM_PROTECT(
		get_exec_segment(elf),
		&error,
		"Failed to get executable segment"
	);

	// Check if the file contains a valid padding
	CUSTOM_PROTECT(
		get_padding(elf),
		&error,
		"Unable to pack this binary: not enough padding"
	);

	return 0;
}
