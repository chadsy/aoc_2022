//
// Created by Chad Royal on 12/05/22.
//
// AOC 2022 results in C
// @chadsy
// Copyright (C) 2022 Chad Royal
// MIT License http://opensource.org/licenses/MIT

// Day 04
// The one about ...

#include "aoc22.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

typedef struct {
    int l, r;
} range;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// sample works as 2
// same code fails as 404 is too low

bool day04_solver(arguments *args) {
    char *buf = calloc(BUFSIZ, sizeof(char));
    int contains = 0;
    int overlaps = 0;

    args->answers[0] = 0;
    args->answers[1] = 0;

    while (trim(fgets(buf, BUFSIZ - 1, args->input))) {
        range a = {}, b = {};

        if (sscanf(buf, "%d-%d,%d-%d", &a.l, &a.r, &b.l, &b.r) != 4) {
            fprintf(stderr, "error: cannot parse input line '%s'\n", buf);
        }

        if (a.l == b.l) {
            overlaps++;         // there's an overlap no matter what
            contains++;         // there's also containment for sure
        }
        else if (a.r == b.r) {
            overlaps++;
            contains++;
        }
        else if (a.l < b.l) {
            if (a.r > b.r)
                contains++;     // a contains b
            if (a.r >= b.l)
                overlaps++;     // a overlaps b
        }
        else {                  // a.l > b.l
            if (a.r <= b.r)
                contains++;     // b contains a
            if (a.l <= b.r)
                overlaps++;
        }
    }

    free(buf);

    args->answers[0] = contains;
    args->answers[1] = overlaps;

    return true;
}
