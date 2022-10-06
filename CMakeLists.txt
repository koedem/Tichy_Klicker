cmake_minimum_required(VERSION 3.23)
project(Tichy_Klicker)

add_compile_options(-mcmodel=medium)

set(CMAKE_CXX_STANDARD 14)

add_executable(Tichy_Klicker main.cpp Calculator.h Dijkstra.h)
