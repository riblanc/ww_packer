//
// Created by riblanc on 11/15/23.
//

#ifndef WW_PACKER_WOODY_H

# ifndef __EXEC_NAME__
#  define __EXEC_NAME__ "woody"
# endif //__EXEC_NAME__

# define WW_PACKER_WOODY_H

# include "elf/elf_manager.h"

extern unsigned const char	__bytecode[];
extern unsigned const int	__bytecode_len;

char	*generate_key(char *dst, size_t len);
void 	crypt_elf(t_elf_info *elf);

#endif //WW_PACKER_WOODY_H
