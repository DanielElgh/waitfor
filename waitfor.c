/**********************************************************************
 * waitfor.c                                                          *
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
#include <stdbool.h>

#ifdef _WIN32
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#    pragma warning (disable : 4996)
#else
#    include <sys/time.h>
#    include <sys/types.h>
#    include <unistd.h>
#endif

#include "waitfor.h"

static void mssleep (const unsigned int);
static void interpretEscapes (char *);
static void cleanup (char *, FILE *);

int waitfor (char *matchstr, const char *filename, const time_t timeout) {
    bool isTimeoutDisabled = false;
    bool isMatchFound = false;
    size_t strlength = 0;
    char *buffer = NULL;
    FILE *fh = NULL;
    time_t startTime = time (NULL);

    if (timeout == TIMEOUT_DISABLED)
        isTimeoutDisabled = true;

    interpretEscapes (matchstr);
    strlength = strlen (matchstr);

    buffer = (char *) calloc (strlength + 1, sizeof (char));
    if (buffer == NULL) {
        fputs ("Memory allocation error.\n", stderr);
        return MEMORY_ALLOC_ERROR;
    }

    if (!strcmp (filename, "--")) {
        fh = stdin;
    } else {
        fh = fopen (filename, "r");
    }

    if (fh == NULL) {
        fprintf (stderr, "Unable to open file '%s' for reading: ", filename);
        perror (NULL);
        cleanup (buffer, fh);
        return STREAM_OPEN_ERROR;
    }

    fread (buffer, 1, strlength, fh);   /* Try to fill the buffer at once */

    while (isMatchFound != true) {
        if (!strcmp (buffer, matchstr)) {
            isMatchFound = true;
            cleanup (buffer, fh);
        } else {
            char ch;
            if ((ch = (char) fgetc (fh)) != EOF) {
                if (strlen (buffer) == strlength)
                    strcpy (buffer, &buffer[1]);
                buffer[strlen (buffer)] = ch;
            } else if (feof (fh)) {
                if (!isTimeoutDisabled &&
                      time (NULL) - startTime >= timeout) {
                    fputs ("Timeout reached - Giving up\n", stderr);
                    cleanup (buffer, fh);
                    return TIMEOUT_REACHED;
                }
                mssleep (500);
            } else if (ferror (fh) != 0) {
                perror ("Error occured during read");
                cleanup (buffer, fh);
                return STREAM_READ_ERROR;
            }
        }
    }

    return MATCH_FOUND;
}

static void mssleep (const unsigned int t) {
#ifdef _WIN32
    Sleep (t);
#else
    struct timeval tv;
    tv.tv_sec = t / 1000;
    tv.tv_usec = t % 1000 * 1000;
    select (0, NULL, NULL, NULL, &tv);
#endif
    return;
}

static void interpretEscapes (char *str) {
    while (*str++) {
        if (*str == '\\') {
            switch (str[1]) {
            case 'n':
                strcpy (str, &str[1]);
                *str = '\n';
                break;
            case 'r':
                strcpy (str, &str[1]);
                *str = '\r';
                break;
            case 't':
                strcpy (str, &str[1]);
                *str = '\t';
                break;
            case '0':
                strcpy (str, &str[1]);
                *str = '\0';
                break;
            case '\'':
                strcpy (str, &str[1]);
                *str = '\'';
                break;
            case '"':
                strcpy (str, &str[1]);
                *str = '"';
                break;
            case '\\':
                strcpy (str, &str[1]);
                *str = '\\';
                break;
            }
        }
    }
    return;
}

static void cleanup (char *buf, FILE *hndl) {
    free (buf);
    if (hndl != NULL && hndl != stdin)
        fclose (hndl);
    return;
}
