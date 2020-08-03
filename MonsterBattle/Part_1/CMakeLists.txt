# cmake_minimum_required(VERSION <specify CMake version here>)
project(main)

set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp MonsterBase.h MonsterBase.cpp BattleController.cpp BattleController.h)