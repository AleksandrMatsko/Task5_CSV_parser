cmake_minimum_required(VERSION 3.21)
project(Task5_CSV_parser)

set(CMAKE_CXX_STANDARD 23)

add_executable(Task5_CSV_parser
        main.cpp CSVParser.h TuplePrinter.h Exceptions.h Exceptions.cpp)
