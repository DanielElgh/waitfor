#!/bin/false
# License: The MIT License (MIT)
# Copyright (c) 2019 Daniel Elgh
# This work is licensed under the terms of the MIT license
# For details, see LICENSE.txt
#
# Far from my best work but it gets the job done....
#

# GLOBALS
EXECUTABLE=''
EXIT_STATUS=''
CHILD_PID=''
STDOUT=''
STDERR=''
FAILS=0
# Colors
normal=''
green=''
cyan=''
red=''

function caption {
    CAPTION=${1:0:49}
    LINE=''
    if [ "x$2" != "x" ]; then
        LINE="(line:$2)"
    fi
    printf " %12s | ${cyan}%-50s${normal}" "${LINE}" "${CAPTION}"
}

function run {
    TMPFILE="$(mktemp)"
    STDOUT=$($EXECUTABLE "$@" 2> $TMPFILE)
    EXIT_STATUS=$?
    STDERR=$(cat $TMPFILE)
    rm $TMPFILE
    return $EXIT_STATUS
}

function success {
    echo "[${green} Successful ${normal}]"
}

function fail {
    echo "[${red}   Failed   ${normal}]  "
    ((FAILS++))
}

function hr {
    echo " -------------------------------------------------------------------------------"
}

function done_testing {
    hr
    echo " Failed $FAILS test case(s)"
    [ $FAILS = 0 ] && exit 0 || exit 1
}

function init_colors {
    # Thanks to Mikel for the colorful terminal
    # (https://unix.stackexchange.com/a/10065)
    if test -t 1; then
        ncolors=$(tput colors)
        if test -n "$ncolors" && test $ncolors -ge 8; then
            normal="$(tput sgr0)"
            green="$(tput setaf 2)"
            red="$(tput setaf 1)"
            cyan="$(tput setaf 6)"
        fi
    fi
}

function init_test_framework {
    init_colors
    if [ "x$BASEDIR" = 'x' ] || [ "x$BINNAME" = 'x' ]; then
        echo "The variables BASEDIR and BINNAME must be set before"\
             "initializing the test framework."
        exit 1
    fi
    EXECUTABLE="$BASEDIR/$BINNAME"

    EXEC_ERROR=''
    [ ! -x $EXECUTABLE ] && EXEC_ERROR="File is not an executable"
    [ ! -e $EXECUTABLE ] && EXEC_ERROR="No such file"

    if [ "x$EXEC_ERROR" != "x" ]; then
        echo " ${red}Cannot execute '$EXECUTABLE':${normal} $EXEC_ERROR"
        exit 1
    fi

    hr
    echo "                            Running $PROJECT test cases";
    hr
}
