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

static const char WAITFOR_VERSION[] = "1.1.1";
static const time_t TIMEOUT_DISABLED = -1;

enum exit_codes {
    MATCH_FOUND = 0,
    UNKNOWN_ERROR,
    MISSING_ARG,
    TIMEOUT_ARG_MALFORMED,
    MEMORY_ALLOC_ERROR,
    STREAM_OPEN_ERROR,
    STREAM_READ_ERROR,
    TIMEOUT_REACHED
};

int waitfor (char *, const char *, const time_t);

#endif
