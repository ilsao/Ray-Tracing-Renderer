cmake_minimum_required(VERSION 3.10)

project(ray_tracing)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cc
)

add_executable(main ${SOURCES})