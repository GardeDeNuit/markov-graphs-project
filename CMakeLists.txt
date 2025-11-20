cmake_minimum_required(VERSION 3.27)
project(2526_TI301_PJT C)

set(CMAKE_C_STANDARD 11)

add_executable(TI_301_PJT
        main.c
        cell.c
        list.c
        graph.c
        utils.c
        mermaidchart-file-generator.c
        tarjan_vertex.c
        class.c
        partition.c
        tarjan.c
        tests.c
        matrix.c
        matrix.h)
