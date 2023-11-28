//
// Created by riblanc on 11/11/23.
//

#ifndef WW_PACKER_ERROR_H
#define WW_PACKER_ERROR_H

#define ERRNO_PROTECT(__CALL, __DATA)								\
	do	{															\
		errno = 0;													\
		(__CALL); 													\
		if (errno != 0)		return (error_hook(__DATA, errno));		\
	} while (0)

#define ECODE_PROTECT(__CONDITION, __ERROR_CODE, __DATA)				\
	do {																\
		if (__CONDITION)	return (error_hook(__DATA, __ERROR_CODE));	\
	} while (0)

#define CUSTOM_PROTECT(__CONDITION, __DATA, __MSG)								\
	do {																		\
		__DATA.msg = __MSG;														\
		if (__CONDITION)	return (error_custom_hook(&__DATA));				\
	} while (0)

#endif //WW_PACKER_ERROR_H
