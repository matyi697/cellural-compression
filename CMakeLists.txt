cmake_minimum_required(VERSION 3.30)
project(cellural_compression C)

set(CMAKE_C_STANDARD 11)

add_executable(cellural_compression
    Examples/Demo1/main.c
    Examples/Demo2/main.c
    Examples/Demo3/compressor.c
    Examples/Demo3/main.c
    Examples/Demo4/main.c
    Examples/Demo5/main.c
    Examples/Demo6/main.c
    Examples/Demo6/test.c
    Examples/dev/test.c
    main.c)
