/**********************************************************************
 * main.c                                                             *
 *                                                                    *
 *               Copyright (C) 2018 - 2019, Daniel Elgh               *
 *      This work is licensed under the terms of the MIT license      *
 *                    For details, see LICENSE.txt                    *
 *                                                                    *
 *********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include <ctype.h>
#include "waitfor.h"

#define optparser(...) op((const char *[]){__VA_ARGS__, NULL})

static int op (const char *[]);
static int help (const int);
static int version (void);

int main (int argc, char *argv[]) {
    char *matchstr, *filename;
    int status_code;
    time_t timeout = TIMEOUT_DISABLED;
    size_t i = 0;

    if (argc == 2) {
        if (optparser (argv[1], "--help", "-h")) {
            return help (EXIT_SUCCESS);
        }
        if (optparser (argv[1], "--version")) {
            return version ();
        }
    }

    if (argc < 3) {
        return help (MISSING_ARG);
    }

    matchstr = argv[1];
    filename = argv[2];

    if (argc > 3) {
        /* Verify third argument only contain digits before we atol() it */
        for (i = 0; i < strlen (argv[3]); i++) {
            if (!isdigit (argv[3][i])) {
                fputs ("Invalid timeout value.\n", stderr);
                return TIMEOUT_ARG_MALFORMED;
            }
        }
        timeout = atol (argv[3]);
    }

    status_code = waitfor (matchstr, filename, timeout);

    switch (status_code) {
    case MATCH_FOUND:
        puts ("Matched!");
        break;
    }

    return status_code;
}

static int help (const int exit_code) {
    puts ("waitfor - Unpretentious utility to detect if a string occurs in a text file.");
    puts ("Usage:");
    puts ("  waitfor <search string> <file> [timeout]");
    puts ("  waitfor --version");
    puts ("  waitfor --help");
    return exit_code;
}

static int version(void) {
    printf ("waitfor %s\n", WAITFOR_VERSION);
    return 0;
}

static int op (const char *str[]) {
    const char *arg = *str++;
    while (*str) {
        if (!strcmp (arg, *str++))
            return 1;
    }
    return 0;
}
