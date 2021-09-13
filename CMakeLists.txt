#cmake generated vscode

cmake_minimum_required(VERSION 3.0.0)
project(yAPI VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(yAPI yAPI.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
