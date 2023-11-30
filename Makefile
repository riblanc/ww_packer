# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/28 20:35:15 by riblanc           #+#    #+#              #
#    Updated: 2022/05/20 18:20:06 by riblanc          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = woody

#--- DEFINES
#   Will define a compilation rule for each element in the defines variable,
#   compiling a binary and defining a preprocessing macro with the element name.
#   i.e:
#   If DEFINES = hello world,
#   when 'make hello', it will compile a binary named hello with -D _HELLO
#   and when 'make world', it will compile a binary named world with -D _WORLD.
#
DEFINES = debug

#--- COMPILE_ALL
#   When setting COMPILE_ALL to 'true', 'make' will compile $(NAME) and each
#   elements defined in $(DEFINES). Otherwise, 'make' will only compile $(NAME).
#
COMPILE_ALL = false

CC = gcc
SRC_LIST = 	main.c            \
			elf/elf_manager.c \
			elf/error.c       \
			payload/payload.c \
			utils/memcpy.c

LD = -lc
CFLAGS = -Wall -Wextra -Werror

-include ./make_utils/Generic.mk

PAYLOAD = $(SRC_DIR)/payload/payload.asm

$(PAYLOAD:.asm=.c): $(PAYLOAD)
	nasm -f elf64 -o $(SRC_DIR)/payload/bytecode $<
	bash $(SRC_DIR)/payload/extract_bytecode.sh