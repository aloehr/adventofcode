cmake_minimum_required(VERSION 3.18)

project(aoc2023)

set(SRC_FILES
    main.cpp
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

add_executable(
    ${PROJECT_NAME}
    ${SRC_FILES}
)

target_link_libraries(${PROJECT_NAME} aoc_helper)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data $<TARGET_FILE_DIR:${PROJECT_NAME}>/data)
