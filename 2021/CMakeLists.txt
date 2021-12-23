cmake_minimum_required(VERSION 3.18)

project(aoc2021)

add_executable(
    aoc
    main.cpp
    aoc.cpp
    solutions/day01.cpp
    solutions/day02.cpp
    solutions/day03.cpp
    solutions/day04.cpp
    solutions/day05.cpp
    solutions/day06.cpp
    solutions/day07.cpp
    solutions/day08.cpp
    solutions/day09.cpp
    solutions/day10.cpp
    solutions/day11.cpp
    solutions/day12.cpp
    solutions/day13.cpp
    solutions/day14.cpp
    solutions/day15.cpp
    solutions/day16.cpp
    solutions/day17.cpp
    solutions/day18.cpp
    solutions/day19.cpp
    solutions/day20.cpp
    solutions/day21.cpp
    solutions/day22.cpp
    solutions/day23.cpp
    #solutions/day24.cpp
    #solutions/day25.cpp
)

target_compile_options(aoc PRIVATE -Wall)
target_compile_features(aoc PRIVATE cxx_std_17)
