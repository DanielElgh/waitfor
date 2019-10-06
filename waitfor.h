/**********************************************************************
 * waitfor.h                                                          *
 *                                                                    *
 *               Copyright (C) 2018 - 2019, Daniel Elgh               *
 *      This work is licensed under the terms of the MIT license      *
 *                    For details, see LICENSE.txt                    *
 *                                                                    *
 *********************************************************************/
#ifndef WAITFOR_H_
#define WAITFOR_H_

#include <time.h>
#include <stdio.h>

static const char WAITFOR_VERSION[] = "1.1.2";
static const time_t TIMEOUT_DISABLED = -1;

enum exit_codes {
    MATCH_FOUND           = 0,
    TIMEOUT_REACHED       = 1,
    MISSING_ARG           = 10,
    TIMEOUT_ARG_MALFORMED = 11,
    MEMORY_ALLOC_ERROR    = 20,
    STREAM_OPEN_ERROR     = 21,
    STREAM_READ_ERROR     = 22,
    UNKNOWN_ERROR         = 255
};

int waitfor (char *, const char *, const time_t);

#endif
