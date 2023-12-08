//
// Created by riblanc on 08/12/23.
//

#include "utils/random.h"

#include <sys/types.h>

static u_long next = 1;

int	ft_rand()
{
	return ((next = next * 1103515245 + 12345) % ((u_long)RAND_MAX + 1));
}

u_int	ft_srand(u_int seed)
{
	next = seed;
	return next;
}