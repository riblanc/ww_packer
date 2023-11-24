//
// Created by riblanc on 11/24/23.
//

#include <stddef.h>

void *ft_memcpy(void *dest, const void *src, size_t n) {
    size_t i = 0;

    // Copy byte by byte until we're aligned on 8 bytes
    while (i < n && ((size_t)dest & 7)) {
        ((char *)dest)[i] = ((char *)src)[i];
        i++;
    }
    if (i == n) return dest;

#if __GNUC__ && (__x86_64__ || __ppc64__)
    // Copy 8 bytes at a time if we're on a 64-bit architecture
    while (i + 8 <= n) {
        ((long *)dest)[i / 8] = ((long *)src)[i / 8];
        i += 8;
    }
    if (i == n) return dest;
#endif

    // Copy 4 bytes at a time as much as possible
    if (i + 4 <= n) {
        ((int *)dest)[i / 4] = ((int *)src)[i / 4];
        i += 4;
        if (i == n) return dest;
    }

    // Copy 2 bytes at a time as much as possible
    if (i + 2 <= n) {
        ((short *)dest)[i / 2] = ((short *)src)[i / 2];
        i += 2;
        if (i == n) return dest;
    }

    // Copy the last byte if we have to
    if (i < n) ((char *)dest)[i] = ((char *)src)[i];

    return dest;
}