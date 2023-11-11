//
// Created by riblanc on 11/11/23.
//

#include <elf/error.h>
#include <stdio.h>
#include <string.h>

int exit_custom_hook(const char *prefix, const char *str) {
    if (prefix)
        fprintf(stderr, "%s: '%s': Error: %s\n", __EXEC_NAME__, prefix, str);
    else
        fprintf(stderr, "%s: Error: %s\n", __EXEC_NAME__, str);
    return 1;
}

int    exit_hook(int _errno, const char *prefix) {
    exit_custom_hook(prefix, strerror(_errno));
    return 1;
}