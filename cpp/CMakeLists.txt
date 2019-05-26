cmake_minimum_required(VERSION 3.14)
project(crackme_keygen)

set(CMAKE_CXX_STANDARD 11)

add_executable(crackme_keygen main.cpp string_generator.h string_generator.cpp hexstring.h hexstring.cpp)