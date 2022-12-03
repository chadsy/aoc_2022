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
    return (int) (*(answer_t *) b - *(answer_t *) a);
}

bool day01_solver(arguments *args) {
    char *buf = calloc(BUFSIZ, sizeof(char));
    answer_t *payloads = calloc(MAX_ELVES, sizeof(answer_t));

    answer_t cur = 0;
    int elf = 0;

    while (fgets(buf, sizeof(buf) - 1, args->input) && elf < MAX_ELVES) {
        if (!strlen(trim(buf))) {
            // end of elf
            payloads[elf] = cur;
            cur = 0;
            elf++;
        } else {
            cur += atoi(trim(buf));
        }
    }

    payloads[elf++] = cur;

    qsort(payloads, elf, sizeof(answer_t), comp_answer_desc);

    free(buf);
    free(payloads);

    args->answers[0] = payloads[0];
    args->answers[1] = payloads[0] + payloads[1] + payloads[2];

    return true;
}
