# Advent of Code 2022 

I'm taking another at an [AOC](https://adventofcode.com/2022) series, once again in C. I wanted to use it to learn CLion, work hard at not giving up at yet-another-graph traversal thing, and build a fun CLI.

Always appreciative for these annual puzzles that [Eric Wastl](http://was.tl/) puts together.

New this time is a single-entry-point CLI with a relatively simple model for extensibility, i.e. adding solution for a new day. Also is some support for profiling (like if I have different implementations of the same solution), and a form of unit-testing to capture the expected output value and validate it. Again, setting myself up to be able to iterate or optimize or play with solutions after they're solved.

Here's the simple help for the CLI:

```
aoc - CLI driver for AOC 2022 puzzles

usage:
aoc (empty)     runs the puzzle dataset for the most recent day that's been implemented
aoc *           runs the puzzle dataset for all the days implemented
aoc N           runs the puzzle dataset for day N
-s              Sample:  solves the sample dataset instead of the actual puzzle dataset
-t              Test:    run both the puzzle and sample datasets and compares the results
the expected saved values
-q              Quiet:   suppress the summary message at the end of a dataset solution
-p              Profile: iterates 10 times over the dataset and times the execution
-?, -h          Help:    you're looking at it
```

# To Do
There's a loop in `main()` to run all the available solution days. All the code inside it is duplicated outside the loop for a single day run. The code in both cases is largely identical and needs to be refactored. There are too many parameters that flow into the block to make function extraction reasonable, it would get ugly. But I want to fix that, one way or the other.

# Build it

Welp, there's a CMakeLists.txt (possibly in the top 5 of dumb hardcoded naming patterns from tools), so maybe it'll just work for you.

# License

This code is copyrighted 2022 by Chad Royal but is free to use under the [MIT License](http://opensource.org/licenses/MIT). Share and enjoy.
