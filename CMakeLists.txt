cmake_minimum_required(VERSION 3.20)
project(akinator)

set(CMAKE_CXX_STANDARD 14)

add_executable(akinator main.cpp include/config.h src/tree.cpp include/tree.h src/akinator.cpp include/akinator.h src/dump.cpp include/dump.h src/database.cpp include/database.h)
