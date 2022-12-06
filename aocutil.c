// AOC 2022 results in C
// @chadsy
// Copyright (C) 2022 Chad Royal
// MIT License http://opensource.org/licenses/MIT
//
// Driver utilities for the Advent of Code 2022 contest

#include <ctype.h>
#include "aoc22.h"
#include "aocdriver.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int stricmp(char const *a, char const *b)
{
    for (;; a++, b++) {
        int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (d != 0 || !*a)
            return d;
    }
}

char *trim(char *str) {
    if (!str || !*str)
        return str;

    char *p = str + (strlen(str) - 1);
    while (isspace(*p)) {
        *p-- = '\0';
    }
    return str;
}
