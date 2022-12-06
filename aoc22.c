// AOC 2022 results in C
// @chadsy
// Copyright (C) 2022 Chad Royal
// MIT License http://opensource.org/licenses/MIT
//
// Driver/CLI for the Advent of Code 2022 contest

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#include <ctype.h>
#include <time.h>
#include "aoc22.h"
#include "aocdriver.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// To add a solution for a new day:
// Step 1: create a new dayXX.c and add it to the cmakelists.txt file
// Step 2: add a prototype for the extern solver function
// Step 3: add the appropriate solver entry to the solvers[] table
//         The solvers table takes a day number, a solver function pointer, and a pretty name.
//         You can have multiple solvers for a day. It helps if you use distinct pretty names.
// Step 4: datasets that contain the day number, a bool indicating if it's a sample (test)
//         dataset, the filename containing the data, and the part 1 & part 2 answer values.

// Step 2 here
extern bool day01_solver(arguments *);
extern bool day02_solver(arguments *);
extern bool day03_solver(arguments *);
extern bool day04_solver(arguments *);
extern bool day05_solver(arguments *);

// Step 3 here
solver_entry solvers[] = {
        {day01, day01_solver, ANS_INT, "Prime solver for Day 01"},
        {day02, day02_solver, ANS_INT, "Prime solver for Day 02"},
        {day03, day03_solver, ANS_INT, "Prime solver for Day 03"},
        {day04, day04_solver, ANS_INT, "Prime solver for Day 04"},
        {day05, day05_solver, ANS_STR, "Prime solver for Day 05"},
};

// Step 4 here
dataset datasets[] = {
        {day01, true,  "inputs/sample01.txt", 24000, 45000},
        {day01, false, "inputs/day01.txt",    75622, 213159},
        {day02, true,  "inputs/sample02.txt", 15,    12},
        {day02, false, "inputs/day02.txt",    13526, 14204},
        {day03, true,  "inputs/sample03.txt", 157,   70},
        {day03, false, "inputs/day03.txt",    7597,  2607},
        {day04, true,  "inputs/sample04.txt", 2,     4},
        {day04, false, "inputs/day04.txt",    466,   865},
        {day05, true,  "inputs/sample05.txt", {.pv = "a"},     {.pv = "a"}},
        {day05, false, "inputs/day05.txt",    {.pv = "a"},     {.pv = "a"}},
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int main(int argc, char **argv) {
    bool opt_run_all = false;
    bool opt_sample_data = false;
    bool opt_summarize = true;
    bool opt_profile = false;
    bool opt_unit_test = false;
    int fail_count = 0;

    int solver_count = countof(solvers);
    int day_num = solver_count;     // TODO: Spin the solvers array for the most recent day available, regardless of position

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == 'a'&& !stricmp(argv[i], "all")) {
            opt_run_all = true;
        }
        else if (argv[i][0] == '-') {
            switch (tolower(argv[i][1])) {
                case 'q':
                    opt_summarize = false;
                    break;
                case 's':
                    opt_sample_data = true;
                    break;
                case 'p':
                    opt_profile = true;
                    break;
                case 't':
                    opt_unit_test = true;
                    break;
                case 'h':
                case '?':
                    show_help();
                    return 0;
                case '\0':
                default:
                    fprintf(stderr, "error: unknown argument '%c'\n", tolower(argv[i][1]));
                    break;
            }
        }
        else if (isdigit(argv[i][0])) {
            day_num = atoi(argv[i]); // NOLINT(cert-err34-c)
            if (day_num == 0 || day_num > solver_count) {
                fprintf(stderr, "error: day number %d ('%s') is invalid, max is %d\n", day_num, argv[i], solver_count);
                return ENOENT;
            }
        }
        else {
            fprintf(stderr, "error: unknown argument '%s'\n\n", argv[i]);
            fflush(stderr);
            show_help();
            return EINVAL;
        }
    }

    for (int i = 0; i < solver_count; i++) {
        if (solvers[i].day != day_num && !opt_run_all)
            continue;

        solver_entry se = solvers[i];

        for (int j = 0; j < countof(datasets); j++) {
            if (datasets[j].day == se.day) {

                dataset data = datasets[j];

                if (!opt_sample_data && data.is_sample ||
                    opt_sample_data && !data.is_sample)
                    continue;

                arguments runinfo;
                runinfo.input = fopen(data.data_file, "r");

                bool result = true;

                if (opt_profile) {
                    clock_t start, finish;
                    start = clock();

                    for (int r = 0; r < PROFILE_RUNS; r++) {
                        runinfo.answers[0].val = 0;
                        runinfo.answers[1].val = 0;
                        se.solver(&runinfo);
                        rewind(runinfo.input);
                    }

                    finish = clock();

                    double diff = (double) (finish - start) / CLOCKS_PER_SEC;
                    printf("day %d: took %lf seconds for %d iterations (%.2lf IPS)\n",
                           i + 1, diff, PROFILE_RUNS, 1.0f / diff * PROFILE_RUNS);
                }
                else
                    result = se.solver(&runinfo);

                fclose(runinfo.input);

                if (!result) {
                    fprintf(stderr, "error: day %d solver failed\n", i + 1);
                    return 1;
                }

                if (opt_summarize) {
                    char *sample_slug = data.is_sample ? "sample: " : "";

                    printf("%s [%s%s]:\n", se.title, sample_slug, data.data_file);
                    printf("  part 1: %u\n", runinfo.answers[0].val);
                    printf("  part 2: %u\n", runinfo.answers[1].val);
                    fflush(stdout);
                }

                if (opt_unit_test) {
                    if (runinfo.answers[0].val == data.result_1.val && runinfo.answers[1].val == data.result_2.val) {
                        printf(" success!\n");
                    }
                    else {
                        fprintf(stderr, "failed!\n");
                        if (runinfo.answers[0].val != data.result_1.val) {
                            fprintf(stderr, "day %d part 1 failed: expected %u and received %u\n",
                                    i + 1, data.result_1.val, runinfo.answers[0].val);
                            fail_count++;
                        }
                        if (runinfo.answers[1].val != data.result_2.val) {
                            fprintf(stderr, "day %d part 2 failed: expected %u and received %u\n",
                                    i + 1, data.result_2.val, runinfo.answers[1].val);
                            fail_count++;
                        }
                    }
                }
            }
        }
        printf("\n");
    }

    return fail_count;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void show_help(void) {
    printf("aoc - CLI driver for AOC 2022 puzzles\n");
    printf("   most recent solver available: Day %lu\n\n", countof(solvers));
    printf("usage:\n");
    printf("   aoc (empty)     runs the puzzle dataset for the most recent day that's been implemented\n");
    printf("   aoc N           runs the puzzle dataset for day N\n");
    printf("   aoc all         runs the puzzle dataset for all the days implemented\n");
    printf("   -s              Sample:  solves the sample dataset instead of the actual puzzle dataset\n");
    printf("   -t              Test:    run both the puzzle and sample datasets and compares the results\n");
    printf("                            the expected saved values\n");
    printf("   -q              Quiet:   suppress the summary message at the end of a dataset solution\n");
    printf("   -p              Profile: iterates 10 times over the dataset and times the execution\n");
    printf("   -?, -h          Help:    you're looking at it\n");
}
