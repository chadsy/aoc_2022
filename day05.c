//
// Created by Chad Royal on 12/05/22.
//
// AOC 2022 results in C
// @chadsy
// Copyright (C) 2022 Chad Royal
// MIT License http://opensource.org/licenses/MIT

// Day 05
// The one about stacks of crates.
//
// Part 1 is to load some crates on stacks, then parse instructions of the form
// 'move A from B t C' where we move A number of crates from stack B to stack C.
// Implemented a simple stack class and iterated through the instructions to
// carry them out. Two interesting notes. First, loading the stacks has to be
// by inserting subsequent items at the bottom; this is because the input is
// presented from top down. Second, 'crane' can only move one at a time so to
// execute the instructions we have to iterate in a loop. Once done, scraped
// the stack tops to get the answer.
//
// Part 2 is to use the same inputs but this time the crane can move all at once
// meaning, no need to iterate if I were to 'break' the stack model and just move
// stuff. Decided instead to iterate and use a temp stack to push/pop so that the
// order is preserved when moving from source to dest.
//
// Extra challenge here that the outputs are a string, so had to hack in a bunch of
// unions to handle polymorphic answer types.

#include <ctype.h>
#include "aoc22.h"
#include "aocstack.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

typedef struct {
    int count, from, to;
} instruction;

#define MAX_STACKS  10

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

instruction parse_instruction(const char *str) {
    instruction i = {};

    sscanf(str, "move %d from %d to %d", &i.count, &i.from, &i.to);
    return i;
}

void execute_move_single(stackref *stacks, instruction i) {
    for (int count = 0; count < i.count; count++) {
        char val = stack_pop(stacks[i.from - 1]);
        stack_push(stacks[i.to - 1], val);
    }
}

void execute_move_multiple(stackref *stacks, instruction i) {
    stackref worker = stack_create(0);

    for (int count = 0; count < i.count; count++) {
        char val = stack_pop(stacks[i.from - 1]);
        stack_push(worker, val);
    }
    for (int count = 0; count < i.count; count++) {
        char val = stack_pop(worker);
        stack_push(stacks[i.to - 1], val);
    }

    stack_destroy(worker);
}

bool day05_solver(arguments *args) {
    char *buf = calloc(BUFSIZ, sizeof(char));

    // two sets of stacks, one for each part of the puzzle
    stackref single_stacks[MAX_STACKS];
    stackref mult_stacks[MAX_STACKS];

    for (int i = 0; i < countof(single_stacks); i++) {
        single_stacks[i] = stack_create(0);
        mult_stacks[i] = stack_create(0);
    }

    // Load the starting crates single_stacks and mult_stacks
    // We have to insert at base of stack to initialize properly, we're reading from the top
    while (trim(fgets(buf, BUFSIZ - 1, args->input))) {
        // until there's a row of numbers
        //  1   2   3   4   5   6   7   8   9
        if (isdigit(buf[1]))
            break;

        for (int box = 1; box < strlen(buf); box += 4) {
            if (!isalpha(buf[box]))
                continue;

            int si = (box - 1) / 4;
            stack_insert(single_stacks[si], buf[box]);
            stack_insert(mult_stacks[si], buf[box]);
        }
    }

//    for (int i = 0; i < countof(single_stacks); i++) {
//        if (stack_count(single_stacks[i])) {
//            printf("%d: ", i + 1);
//            stack_dump(single_stacks[i]);
//        }
//    }

    // eat lines until we get to "move..."
    while (fgets(buf, BUFSIZ - 1, args->input)) {
        if (strncmp("move", buf, strlen("move")) == 0)
            break;
    }

    // Load and execute the instructions for both single and multi
    do {
        instruction i = parse_instruction(buf);
        execute_move_single(single_stacks, i);
        execute_move_multiple(mult_stacks, i);
    } while (trim(fgets(buf, BUFSIZ - 1, args->input)));

    char result[MAX_STACKS + 1];
    memset(result, 0, sizeof(result));
    // Peek the single results off the top
    for (int i = 0; i < countof(single_stacks); i++) {
        if (stack_count(single_stacks[i]))
            result[i] = stack_peek(single_stacks[i]);
    }

    strcpy(args->actual[0].pval, result);

    memset(result, 0, sizeof(result));
    // Peek the multi results off the top
    for (int i = 0; i < countof(mult_stacks); i++) {
        if (stack_count(mult_stacks[i]))
            result[i] = stack_peek(mult_stacks[i]);
    }

    strcpy(args->actual[1].pval, result);

    for (int i = 0; i < countof(single_stacks); i++) {
        stack_destroy(single_stacks[i]);
        stack_destroy(mult_stacks[i]);
    }

    free(buf);
    return true;
}
