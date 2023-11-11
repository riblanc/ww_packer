//
// Created by riblanc on 10/11/23.
//

#ifndef WW_PACKER_ELF_MANAGER_H
#define WW_PACKER_ELF_MANAGER_H

#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "error.h"

#define DECLARE_ELF_INFO(__ARCH)	       	    \
    typedef struct s_elf##__ARCH##_info {  	    \
        Elf##__ARCH##_Ehdr	*header;        	\
        Elf##__ARCH##_Phdr	*program_header;	\
        Elf##__ARCH##_Shdr	*section_header;	\
        struct stat         stat;               \
        int                 fd;                 \
    } t_elf##__ARCH##_info;

DECLARE_ELF_INFO(32);
DECLARE_ELF_INFO(64);

int    open_elf64_file(t_elf64_info *elf, const char *filename);

#endif //WW_PACKER_ELF_MANAGER_H