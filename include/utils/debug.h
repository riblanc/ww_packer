//
// Created by riblanc on 11/11/23.
//

#ifndef WW_PACKER_DEBUG_H
# define WW_PACKER_DEBUG_H

# ifdef _DEBUG
#  include <stdio.h>

#  define BOLD			"\x1b[1m"
#  define ITALIC		"\x1b[3m"
#  define UNDERLINE		"\x1b[4m"
#  define BLINK			"\x1b[5m"
#  define RED			"\x1b[31m"
#  define GREEN			"\x1b[32m"
#  define YELLOW		"\x1b[33m"
#  define BLUE			"\x1b[34m"
#  define MAGENTA		"\x1b[35m"
#  define CYAN			"\x1b[36m"
#  define LIGHT_GRAY	"\x1b[37m"
#  define RESET			"\x1b[0m"
#  define WHITE			"\x1b[97m"
#  define DARK_GRAY		"\x1b[90m"

#  define __GENERIC_DEBUG_LOG(__COLOR, ...)													\
	do {																					\
		fprintf(stderr, "%s%s", BOLD, WHITE);												\
		fprintf(stderr, __COLOR "%s()"WHITE":%s:%d: "RESET, __func__, __FILE__, __LINE__);	\
		fprintf(stderr, __VA_ARGS__);														\
		fprintf(stderr, "\n");																\
	} while (0)

#  define DEBUG_LOG(...)	__GENERIC_DEBUG_LOG(BOLD GREEN,		__VA_ARGS__)
#  define DEBUG_WARN(...)	__GENERIC_DEBUG_LOG(BOLD YELLOW,	__VA_ARGS__)
#  define DEBUG_ERROR(...)	__GENERIC_DEBUG_LOG(BOLD RED,		__VA_ARGS__)
#  define DEBUG_FATAL(...)	__GENERIC_DEBUG_LOG(BOLD MAGENTA,	__VA_ARGS__)

# else //!_DEBUG

#  define DEBUG_LOG(...)	;
#  define DEBUG_WARN(...)	;
#  define DEBUG_ERROR(...)	;
#  define DEBUG_FATAL(...)	;

# endif //_DEBUG

#endif //WW_PACKER_DEBUG_H
