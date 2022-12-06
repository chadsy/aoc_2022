//
// Created by Chad Royal on 12/1/22.
//
// AOC 2022 results in C
// @chadsy
// Copyright (C) 2022 Chad Royal
// MIT License http://opensource.org/licenses/MIT

// Day 2
// The one about rock/paper/scissors.
//
// Part 1: Just evaluate each round according to the play rules, and sum the scores.
// Prolly easiest, given the super simple input, is to just sscanf("%c %c\n") the inputs.
// I could write some complicated logic to evaluate the scores too, but since the input
// values are sequences (A/B/C, and X/Y/Z) I think I'll just use them as array indices.
//
// Part 2: Oops, the elf says those aren't moves, they're the results we want to target
// by choosing the move to make to achieve the result. Same basic process, different conversion table;

#include "aoc22.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int basic_score(char you, char me) {
    // this is a table of result scores for R/P/S in you vs me
    // you == row, me == column; value there is the result score
    static char rules[3][3] = {
            {3, 6, 0},
            {0, 3, 6},
            {6, 0, 3}
    };

    int score = me - 'X' + 1;
    score += rules[you - 'A'][me - 'X'];

    return score;
}

int alternate_score(char you, char me) {
    // this is a table of moves to achieve the desired result
    // you == row, me == 0:lose, 1:draw, 2:win; value is
    // the R/P/S move value.
    static char rules[3][3] = {
            {3, 1, 2},
            {1, 2, 3},
            {2, 3, 1}
    };

    int score = (me - 'X') * 3;
    score += rules[you - 'A'][me - 'X'];

    return score;
}

bool day02_solver(arguments *args) {
    char you, me;

    args->answers[0].val = 0;
    args->answers[1].val = 0;

    while (fscanf(args->input, "%c %c\n",&you, &me) == 2) {
        args->answers[0].val += basic_score(you, me);
        args->answers[1].val += alternate_score(you, me);
    }

    return true;
}
