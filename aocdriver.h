// AOC 2022 results in C
// @chadsy
// Copyright (C) 2022 Chad Royal
// MIT License http://opensource.org/licenses/MIT
//
// Driver/CLI header for the Advent of Code 2022 contest

#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define PROFILE_RUNS    1000

typedef enum {
    day01 = 1, day02, day03, day04,
    day05, day06, day07, day08,
    day09, day10, day11, day12,
    day13, day14, day15, day16,
    day17, day18, day19, day20,
    day21, day22, day23, day24,
    day25,
} daynum;

typedef enum  {
    ANS_INT,
    ANS_STR,
} answer_flag;

typedef union {
    char *filename;
    char *data;
} input_t;

typedef struct {
    daynum day;
    bool is_sample;
    input_t input;
    answer_t result_1;
    answer_t result_2;
} dataset;

typedef struct {
    daynum day;
    bool (*solver)(arguments *);
    answer_flag typ;
    const char *title;
} solver_entry;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void show_help(void);
