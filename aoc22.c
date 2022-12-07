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
extern bool day06_solver(arguments *);

// Step 3 here
solver_entry solvers[] = {
        {day01, day01_solver, ANS_INT, "Prime solver for Day 01"},
        {day02, day02_solver, ANS_INT, "Prime solver for Day 02"},
        {day03, day03_solver, ANS_INT, "Prime solver for Day 03"},
        {day04, day04_solver, ANS_INT, "Prime solver for Day 04"},
        {day05, day05_solver, ANS_STR, "Prime solver for Day 05"},
        {day06, day06_solver, ANS_INT, "Prime solver for Day 06"},
};

// Step 4 here
dataset datasets[] = {
        {day01, true, "inputs/sample01.txt", 24000, 45000},
        {day01, false, "inputs/day01.txt", 75622, 213159},
        {day02, true, "inputs/sample02.txt", 15, 12},
        {day02, false, "inputs/day02.txt", 13526, 14204},
        {day03, true, "inputs/sample03.txt", 157, 70},
        {day03, false, "inputs/day03.txt", 7597, 2607},
        {day04, true, "inputs/sample04.txt", 2, 4},
        {day04, false, "inputs/day04.txt", 466, 865},
        {day05, true, "inputs/sample05.txt", {.pval = "CMZ"}, {.pval = "MCD"}},
        {day05, false, "inputs/day05.txt", {.pval = "BWNCQRMDB"}, {.pval = "NHWZCBNBF"}},
        {day06, true, "inputs/sample06a.txt", 7, 19},
        {day06, true, "inputs/sample06b.txt", 5, 23},
        {day06, true, "inputs/sample06c.txt", 6, 23},
        {day06, true, "inputs/sample06d.txt", 10, 29},
        {day06, true, "inputs/sample06e.txt", 11, 26},
        {day06, false, "inputs/day06.txt", 1142, 2803},
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *render_typed_answer(solver_entry *s, answer_t a);
bool compare_typed_answer(solver_entry *s, answer_t a, answer_t b);

int main(int argc, char **argv) {
    bool opt_run_all = false;
    bool opt_sample_data = false;
    bool opt_summarize = true;
    bool opt_profile = false;
    bool opt_unit_test = false;

    int solver_count = countof(solvers);
    int day_num = 0;

    // Find the most recent solver (the highest day number) in the table
    for (int i = 0; i < solver_count; i++) {
        if (solvers[i].day > day_num)
            day_num = solvers[i].day;
    }

    // Parse the arguments & set options
    for (int i = 1; i < argc; i++) {
        // run all
        if (argv[i][0] == 'a' && !stricmp(argv[i], "all")) {
            opt_run_all = true;
        }
        // option switches
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
        // arg that's a specific day
        else if (isdigit(argv[i][0])) {
            day_num = atoi(argv[i]); // NOLINT(cert-err34-c)
            if (day_num <= 0 || day_num > solver_count) {
                fprintf(stderr, "error: day number %d ('%s') is invalid, max is %d\n", day_num, argv[i],
                        solver_count);
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

    int fail_count = 0;

    // spin through all the solvers to figure out what to run
    for (int i = 0; i < solver_count; i++) {

        // if we're not running all and this one isn't the target, then skip
        if (solvers[i].day != day_num && !opt_run_all)
            continue;

        solver_entry se = solvers[i];

        // spin through all the datasets to figure out what to evaluate
        for (int j = 0; j < countof(datasets); j++) {

            if (datasets[j].day == se.day) {

                dataset data = datasets[j];

                // If we're not running unit tests or samples and it's a sample set, skip it
                if (!opt_unit_test) {
                    if (!data.is_sample && opt_sample_data)
                        continue;
                }

                arguments runinfo;

                // TODO: refactor the input into a polymorphic union for string literal
                runinfo.input = fopen(data.input.filename, "r");

                // Initialize the actual results, alloc buffer for strings
                if (se.typ == ANS_INT) {
                    runinfo.actual[0].val = 0;
                    runinfo.actual[1].val = 0;
                }
                else if (se.typ == ANS_STR) {
                    runinfo.actual[0].pval = calloc(MAX_ANS_STR_LEN, sizeof(char));
                    runinfo.actual[1].pval = calloc(MAX_ANS_STR_LEN, sizeof(char));
                }

                bool run_success = true;
                double diff = 0.0;

                // Do a timed profile of PROFILE_RUNS iterations
                if (opt_profile) {
                    clock_t start, finish;
                    start = clock();

                    for (int r = 0; r < PROFILE_RUNS && run_success; r++) {
                        // Reset the result values and rewind the input file
                        if (se.typ == ANS_INT) {
                            runinfo.actual[0].val = 0;
                            runinfo.actual[1].val = 0;
                        }
                        else if (se.typ == ANS_STR) {
                            strcpy(runinfo.actual[0].pval,"");
                            strcpy(runinfo.actual[1].pval,"");
                        }
                        rewind(runinfo.input);

                        run_success = se.solver(&runinfo);
                    }

                    finish = clock();

                    diff = (double) (finish - start) / CLOCKS_PER_SEC;
                }
                else
                    run_success = se.solver(&runinfo);

                fclose(runinfo.input);

                // If there was no catestrophic failure (e.g. bad input, etc).
                if (run_success) {
                    // Show summary result text
                    if (opt_summarize) {
                        char *sample_slug = data.is_sample ? "sample: " : "";

                        printf("%s [%s%s]:\n", se.title, sample_slug, data.input.filename);
                        printf("  part 1: %s\n", render_typed_answer(&se, runinfo.actual[0]));
                        printf("  part 2: %s\n", render_typed_answer(&se, runinfo.actual[1]));
                        fflush(stdout);
                    }

                    // If we're running unit tests, evaluate the results
                    if (opt_unit_test) {
                        if (!compare_typed_answer(&se, runinfo.actual[0], data.result_1)) {
                            fail_count++;
                            fprintf(stderr, "day %d part 1 failed: received %s", i + 1,
                                    render_typed_answer(&se, runinfo.actual[0]));
                            fprintf(stderr, ", expected %s\n", render_typed_answer(&se, data.result_1));
                        }
                        if (!compare_typed_answer(&se, runinfo.actual[1], data.result_2)) {
                            fail_count++;
                            fprintf(stderr, "day %d part 2 failed: received %s", i + 1,
                                    render_typed_answer(&se, runinfo.actual[1]));
                            fprintf(stderr, ", expected %s\n", render_typed_answer(&se, data.result_2));
                        }
                        if (!fail_count)
                            printf("Success!\n");
                    }

                    if (opt_profile) {
                        printf("%s: took %lf seconds for %d iterations (%.2lf IPS)\n",
                               se.title, diff, PROFILE_RUNS, 1.0f / diff * PROFILE_RUNS);
                    }
                }
                else {
                    fprintf(stderr, "error: %s catastrophic failure, using input '%s'\n", se.title, data.input.filename);
                    fail_count++;
                }

                if (se.typ == ANS_STR) {
                    free(runinfo.actual[0].pval);
                    free(runinfo.actual[1].pval);
                }
            }
        }
        printf("\n");
    }

    // return a non-zero result if any of the unit test evaluations failed or a
    // catestrophic error occurred.
    return fail_count;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *render_typed_answer(solver_entry *s, answer_t a) {
    static char buf[BUFSIZ];
    char *fmt = "(bad solver type)";
    if (s->typ == ANS_INT)
        fmt = "%d";
    else if (s->typ == ANS_STR)
        fmt = "%s";
    sprintf(buf, fmt, a);
    return buf;
}

bool compare_typed_answer(solver_entry *s, answer_t a, answer_t b) {
    if (s->typ == ANS_INT)
        return a.val == b.val;
    else if (s->typ == ANS_STR)
        return !strcmp(a.pval, b.pval);
    return false;
}

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
