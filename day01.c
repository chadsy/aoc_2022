// AOC 2022 results in C
// @chadsy
// Copyright (C) 2022 Chad Royal
// MIT License http://opensource.org/licenses/MIT

// Day 1
// The one about elves carrying food (payloads).
//
// Part 1: Pretty simple O(n) results, just scan the inputs looking for the max.
// Note there was a simple bug, falling out of the loop at EOF failed to compare
// and store the final entry. Fixed that.
//
// Part 2: Need the sum of the 3 largest payloads, so refactored to store the
// payloads in an array, then sort it descending.

#include "aoc22.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define MAX_ELVES       1000

int comp_answer_desc(const void *a, const void *b) {
      return (((answer_t *)b)->val - ((answer_t *)a)->val);
}

bool day01_solver(arguments *args) {
    char *buf = calloc(BUFSIZ, sizeof(char));
    int *payloads = calloc(MAX_ELVES, sizeof(int));

    int cur = 0;
    int elf = 0;

    while (fgets(buf, sizeof(buf) - 1, args->input) && elf < MAX_ELVES) {
        if (!strlen(trim(buf))) {
            // empty line means end of current elf, so dump it off
            payloads[elf] = cur;
            cur = 0;
            elf++;
        } else {
            cur += atoi(trim(buf));
        }
    }

    payloads[elf++] = cur;

    qsort(payloads, elf, sizeof(payloads[0]), comp_answer_desc);

    args->actual[0].val = payloads[0];
    args->actual[1].val = payloads[0] + payloads[1] + payloads[2];

    free(buf);
    free(payloads);

    return true;
}
