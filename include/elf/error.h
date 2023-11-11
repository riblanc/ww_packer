//
// Created by riblanc on 11/11/23.
//

#ifndef WW_PACKER_ERROR_H
#define WW_PACKER_ERROR_H

int	exit_custom_hook(const char *prefix, const char *str);
int	exit_hook(int _errno, const char *prefix);

#define ERRNO_PROTECT(__CALL, __PREFIX)								\
	do	{															\
		errno = 0;													\
		if (__CALL < 0)		return (exit_hook(errno, __PREFIX));	\
	} while (0)

#define ECODE_PROTECT(__CONDITION, __ERROR_CODE, __PREFIX)				\
	do {																\
		if (__CONDITION)	return (exit_hook(__ERROR_CODE, __PREFIX));	\
	} while (0)

#define CUSTOM_PROTECT(__CONDITION, __PREFIX, __CUSTOM_STR)						\
	do {																		\
		if (__CONDITION)	return (exit_custom_hook(__PREFIX, __CUSTOM_STR));	\
	} while (0)

#endif //WW_PACKER_ERROR_H
