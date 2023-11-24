# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/28 20:35:15 by riblanc           #+#    #+#              #
#    Updated: 2023/11/24 16:31:18 by mcouppe          ###   ########.fr        #
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
COMPILE_ALL = true

CC = gcc
SRC_LIST = 	main.c		\
			find_entrypoint.c \
			elf/elf64.c \
			elf/error.c

LD = -lc
CFLAGS = -Wall -Wextra -Werror

-include ./make_utils/Generic.mk
