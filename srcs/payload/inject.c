#include "elf/elf_manager.h"
#include "utils/error.h"
#include "utils/string.h"
#include "woody.h"

int	inject(t_elf_info *elf) {
	t_error error = { .data = &elf->file };

	CUSTOM_PROTECT(
		!IS_VALID_PTR(elf->file.map + elf->padding, &elf->file, sizeof(unsigned char) * __bytecode_len),
		&error,
		"Unable to inject payload, file is too small"
	);

	// filling payload with good offsets
	size_t	seg_len = elf->exec_segment->p_filesz;
	size_t	seg_off = elf->padding - elf->exec_segment->p_offset;
	size_t	ep_off = elf->padding - elf->entrypoint;

	ft_memcpy((__bytecode + __bytecode_len - 40), (&seg_len), sizeof(seg_len));
	ft_memcpy((__bytecode + __bytecode_len - 48), (&seg_off), sizeof(seg_off));
	ft_memcpy((__bytecode + __bytecode_len - 56), (&ep_off), sizeof(ep_off));
	
	// injecting payload
	ft_memcpy((elf->file.map + elf->padding), __bytecode, __bytecode_len);

	// sign executable
	*(int *)(elf->file.map + 9) = 0x574f4f57;

	// change entrypoint in Elf header
	elf->header->e_entry = elf->padding;

	return EXIT_SUCCESS;
}
