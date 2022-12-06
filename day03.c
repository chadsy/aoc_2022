//
// Created by Chad Royal on 12/02/22.
//
// AOC 2022 results in C
// @chadsy
// Copyright (C) 2022 Chad Royal
// MIT License http://opensource.org/licenses/MIT

// Day 02
// The one about rucksack compartments
//
// This is classically a hashing problem to find the duplicate item in two sets.
// I think it'll be fun to store each compartment as a 64-bit int, set a corresponding
// bit in the int for the preseence of an item, then AND the two together to find the
// duplicate item. The challenge will then be, how to quickly identify which bit is set
// and translate that to a priority value.
//
// Screw that. Make an array for each of the compartments. Set the element whose index
// corresponds to the item character. Spin thru the arrays in parallel and capture the
// element that exists in both
//
// Part 2: same basic process, but instead of a single rucksack split in half, collect
// full rucksack content in groups of 3, and then find the common item.

#include "aoc22.h"
#include <ctype.h>

#define UNIQUE_ITEMS        52      // items represented by 'a'-'z' and 'A'-'Z'
#define ITEM_INDEX(c)       ((c) - 'A' - (islower(c) ? ('a'-'A'-26) : 0))
#define ITEM_PRIORITY(c)    (isupper(c) ? ((c) - 'A' + 26 + 1) : ((c) - 'a' + 1))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool day03_solver(arguments *args) {
    char *buf = calloc(BUFSIZ, sizeof(char));

    args->answers[0].val = 0;
    args->answers[1].val = 0;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Part 1
    int sum = 0;
    while (trim(fgets(buf, BUFSIZ - 1, args->input))) {
        int split = strlen(buf) / 2;
        uint8_t comp_1[UNIQUE_ITEMS], comp_2[UNIQUE_ITEMS];
        memset(comp_1, 0, sizeof(comp_1));
        memset(comp_2, 0, sizeof(comp_2));
        char *p = buf;
        while (*(p + split)) {
            comp_1[ITEM_INDEX(*p)] = *p;
            comp_2[ITEM_INDEX(*(p + split))] = *(p + split);
            p++;
        }

        for (int i = 0; i < UNIQUE_ITEMS; i++) {
            if (comp_1[i] && comp_1[i] == comp_2[i]) {
                sum += ITEM_PRIORITY(comp_1[i]);
                break;
            }
        }
    }

    args->answers[0].val = sum;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Part 2
    rewind(args->input);
    sum = 0;

    while (trim(fgets(buf, BUFSIZ - 1, args->input))) {
        uint8_t elf_1[UNIQUE_ITEMS], elf_2[UNIQUE_ITEMS], elf_3[UNIQUE_ITEMS];

        char *p = buf;
        memset(elf_1, 0, sizeof(elf_1));
        while (*p) {
            elf_1[ITEM_INDEX(*p)] = *p;
            p++;
        }

        trim(fgets(buf, BUFSIZ - 1, args->input));
        p = buf;
        memset(elf_2, 0, sizeof(elf_2));
        while (*p) {
            elf_2[ITEM_INDEX(*p)] = *p;
            p++;
        }

        trim(fgets(buf, BUFSIZ - 1, args->input));
        p = buf;
        memset(elf_3, 0, sizeof(elf_3));
        while (*p) {
            elf_3[ITEM_INDEX(*p)] = *p;
            p++;
        }

        for (int i = 0; i < UNIQUE_ITEMS; i++) {
            if (elf_1[i] && elf_1[i] == elf_2[i] && elf_1[i] == elf_3[i]) {
                sum += ITEM_PRIORITY(elf_1[i]);
                break;
            }
        }
    }

    args->answers[1].val = sum;

    free(buf);
    return true;
}
