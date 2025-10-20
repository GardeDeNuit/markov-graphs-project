cmake_minimum_required(VERSION 3.27)
project(2526_TI301_PJT C)

set(CMAKE_C_STANDARD 11)

add_executable(TI_301_PJT
        main.c
        #utils.c
        #hasse.c
        cell.c
        cell.h
        list.c
        list.h
        adjacency-list.c
        adjacency-list.h
        graph.c
        graph.h)


