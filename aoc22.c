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
// Step 1: add a new dayxx.c file to the project (make sure it's in cmakelists.txt)
// Step 2: add a prototype for the extern solver function
// Step 3: add the appropriate solver entry to the solvers[] table
//         The solvers table takes a solver function pointer, then two struct instances
//         of dataset type, one for puzzle and one for sample. Each dataset has a filename
//         and the answers for parts 1 & 2.
//         We want to capture these values as unit test results for the -t flag.

// Step 2 here
extern bool day01_solver(arguments *);
extern bool day02_solver(arguments *);
extern bool day03_solver(arguments *);

// Step 3 here
solver_entry solvers[] = {
        {day01, day01_solver, "Prime solver for Day 01"},
        {day02, day02_solver, "Prime solver for Day 02"},
        {day03, day03_solver, "Prime solver for Day 03"},
};

dataset datasets[] = {
        {day01, false, "inputs/day01.txt",    75622, 213159},
        {day01, true,  "inputs/sample01.txt", 24000, 45000},
        {day02, false, "inputs/day02.txt",    13526, 14204},
        {day02, true,  "inputs/sample02.txt", 15,    12},
        {day03, false, "inputs/day03.txt",    7597,  2607},
        {day03, true,  "inputs/sample03.txt", 157,   70},
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
            if (datasets[j].day == day_num) {

                dataset data = datasets[j];

                if (data.is_sample && !opt_sample_data && !opt_unit_test)
                    continue;

                arguments runinfo;
                runinfo.input = fopen(data.data_file, "r");
                runinfo.answers[0] = 0;
                runinfo.answers[1] = 0;

                bool result = true;

                if (opt_profile) {
                    clock_t start, finish;
                    start = clock();
                    for (int i = 0; i < PROFILE_RUNS; i++) {
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
                    printf("  part 1: %u\n", runinfo.answers[0]);
                    printf("  part 2: %u\n", runinfo.answers[1]);
                    fflush(stdout);
                }

                if (opt_unit_test) {
                    if (runinfo.answers[0] == data.result_1 && runinfo.answers[1] == data.result_2) {
                        printf(" success!\n");
                    }
                    else {
                        fprintf(stderr, "failed!\n");
                        if (runinfo.answers[0] != data.result_1) {
                            fprintf(stderr, "day %d part 1 failed: expected %u and received %u\n",
                                    i + 1, data.result_1, runinfo.answers[0]);
                            fail_count++;
                        }
                        if (runinfo.answers[1] != data.result_2) {
                            fprintf(stderr, "day %d part 2 failed: expected %u and received %u\n",
                                    i + 1, data.result_2, runinfo.answers[1]);
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

