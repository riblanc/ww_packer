//
// Created by riblanc on 08/12/23.
//

#ifndef WW_PACKER_RANDOM_H
# define WW_PACKER_RANDOM_H

# include <sys/signal.h>
# include <unistd.h>
# include <sys/types.h>

# ifndef RAND_MAX
#  define RAND_MAX	2147483647
# endif

int		ft_rand();
u_int	ft_srand(u_int seed);

#endif //WW_PACKER_RANDOM_H
