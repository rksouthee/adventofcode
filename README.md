# Advent of Code Solutions

This repository contains my solutions to the [Advent of
Code](https://adventofcode.com/) programming puzzles created by Eric Wastl.

## Building

I have mostly been developing on Windows using
[Ninja](https://ninja-build.org/) and [LLVM](https://llvm.org/). The main
driver program, `aoc`, uses Win32 libraries to load the dynamic libraries.

### Third party libraries

* [CMake](https://cmake.org/)
* [Catch2](https://github.com/catchorg/Catch2)
* [curl](https://github.com/curl/curl)
* [cxxopts](https://github.com/jarro2783/cxxopts)

```sh
cmake -B build -S source
cmake --build build --config debug
```

## Running

Each of the solutions are a dynamic library that is loaded by the driver `aoc`.
For example, to run the solution for day 8 for 2023 we can run

```sh
aoc --year 2023 --day 8
```

this will usually wait for input on `stdin`. Can also pass a file

```sh
aoc --year 2023 --day 8 < ../data/2023/day/8/input.txt
```

or point at the `data` directory, and the program will try and find the correct
input file.

```sh
aoc --year 2023 --day 8 --data-dir ../data
```

The program can also download the input file if it doesn't exist, it requires
passing your session cookie

```sh
aoc --year 2023 --day 8 --data-dir ../data --session <your session token>
```

To get your session cookie you can use the developer tools in your browser and
go to one of the input file links. In the request headers there will be the
Cookie field, grab the session value.

Some of the solutions have tests that can be run. The tests will be run automatically unless the command line
option `--skip-tests` is used.

```sh
aoc --year 2023 --day 8 --skip-tests
```

## New year

Setting up a new year can be done by modifying the root `CMakeLists.txt` file
and updating the `foreach` for the `year` and updating the `<stop>` to the
current year.

When you run configure again it will generate the directory structure and stub
out all the days.
