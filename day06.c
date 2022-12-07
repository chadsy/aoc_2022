//
// Created by Chad Royal on 12/06/22.
//
// AOC 2022 results in C
// @chadsy
// Copyright (C) 2022 Chad Royal
// MIT License http://opensource.org/licenses/MIT

// Day 06
// The one about message encoding headers.
//
// Part 1: Take a random string of incoming characters, find the first run of 4 unique characters,
// then return the index of the next (non-header) character. This is a basic leetcode thing
// I think. Prolly just use an array to store counts, this will be an O(n*26) solution.
//
// Part 2: Same as part 1, except look for 14 uniques instead of 4. So just double the businses
// for a set of 'message' characters.

#include "aoc22.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define ALPHABET        26
#define HEADER_LEN      4
#define MESSAGE_LEN     14

bool header_char_count(int *counts) {
    int count = 0;

    // TODO: we should be able to just determine whether there are any dups in the tails set
    for (int i = 0; i < ALPHABET; i++) {
        if (counts[i] > 1)
            return false;
        else
            count += counts[i];
    }

    return count == HEADER_LEN;
}

bool message_char_count(int *counts) {
    int count = 0;

    // TODO: we should be able to just determine whether there are any dups in the tails set
    for (int i = 0; i < ALPHABET; i++) {
        if (counts[i] > 1)
            return false;
        else
            count += counts[i];
    }

    return count == MESSAGE_LEN;
}

bool day06_solver(arguments *args) {
    char input;
    int header_counts[ALPHABET] = {};
    int message_counts[ALPHABET] = {};
    char header_tails[HEADER_LEN] = {};
    char message_tails[MESSAGE_LEN] = {};
    int headeridx = 0, messageidx = 0;

    while ((input = fgetc(args->input)) != EOF) {
        if (input == '\n')
            continue;

        if (header_tails[headeridx % HEADER_LEN])
            header_counts[header_tails[headeridx % HEADER_LEN] - 'a']--;
        header_tails[headeridx % HEADER_LEN] = input;

        if (message_tails[messageidx % MESSAGE_LEN])
            message_counts[message_tails[messageidx % MESSAGE_LEN] - 'a']--;
        message_tails[messageidx % MESSAGE_LEN] = input;

        // TODO: we don't really have to count, if the existing slot is non-zero we're done with this iteration
        header_counts[input - 'a']++;
        message_counts[input - 'a']++;

        headeridx++;
        messageidx++;

        if (headeridx > HEADER_LEN) {
            if (!args->actual[0].val && header_char_count(header_counts))
                args->actual[0].val = headeridx;
        }
        if (messageidx > MESSAGE_LEN) {
            if (!args->actual[1].val && message_char_count(message_counts))
                args->actual[1].val = messageidx;
        }
    }

    return true;
}
