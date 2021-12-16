project(aoc2021)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")

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
    #solutions/day17.cpp
    #solutions/day18.cpp
    #solutions/day19.cpp
    #solutions/day20.cpp
    #solutions/day21.cpp
    #solutions/day22.cpp
    #solutions/day23.cpp
    #solutions/day24.cpp
    #solutions/day25.cpp
)
