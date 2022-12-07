// AOC 2022 results in C
// @chadsy
// Copyright (C) 2022 Chad Royal
// MIT License http://opensource.org/licenses/MIT
//
// Common header for the AOC 2022 results driver

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define MAX_ANS_STR_LEN     128

#define countof(x)          (sizeof(x)/sizeof(x[0]))

#define min(a,b)            ((a)<(b)?(a):(b))
#define max(a,b)            ((a)>(b)?(a):(b))

int stricmp(char const *a, char const *b);
char *trim(char *str);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

typedef union {
    int   val;
    char *pval;
} answer_t;

typedef struct {
    FILE *input;
    answer_t actual[2];
} arguments;
