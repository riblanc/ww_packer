#include "elf/elf_manager.h"
#include "utils/string.h"
#include "woody.h"

void	inject(t_elf_info *elf) {

	// filling payload with good offsets
	size_t	seg_len_tmp = elf->exec_segment->p_filesz;
	size_t	seg_off_tmp = elf->padding - elf->exec_segment->p_offset;
	size_t	ep_off_tmp = elf->padding - elf->entrypoint;

	ft_memcpy((__bytecode + __bytecode_len - 40), (&seg_len_tmp), sizeof(seg_len_tmp));
	ft_memcpy((__bytecode + __bytecode_len - 48), (&seg_off_tmp), sizeof(seg_off_tmp));
	ft_memcpy((__bytecode + __bytecode_len - 56), (&ep_off_tmp), sizeof(ep_off_tmp));
	

	// injecting payload	

	ft_memcpy((elf->file.map + elf->padding), __bytecode, __bytecode_len);


	// sign executable
	*(int *)(elf->file.map + 9) = 0x574f4f57;

	// change entrypoint in Ehdr
	elf->header->e_entry = elf->padding;
	
}
