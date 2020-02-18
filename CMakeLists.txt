cmake_minimum_required(VERSION 3.14)
project(TeXhelper)

set(CMAKE_CXX_STANDARD 17)

add_executable(TeXhelper main.cpp main/tex.h main/solver.h
        connectors/python.h connectors/vim.h connectors/terminal.h
        parsers/jupyter.h parsers/terminal.h parsers/files.h
        main/exception.h main/utils.h main/fraction.h main/matrix.h)

add_executable(Gaussian gaussian.cpp main/utils.h main/matrix.h main/fraction.h)