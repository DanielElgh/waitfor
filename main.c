/**********************************************************************
 * main.c                                                             *
 *                                                                    *
 *                   Copyright (C) 2018, Daniel Elgh                  *
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

static int help (const int);

int main (int argc, char *argv[]) {
    char *matchstr, *filename;
    int status_code;
    time_t timeout = TIMEOUT_DISABLED;
    size_t i = 0;

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
    puts ("Usage: waitfor <search string> <file> [timeout]");
    return exit_code;
}
