cmake_minimum_required(VERSION 3.15)
project(Dithering VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include (utils.cmake)
InstallAndSetupConanPackages()

add_subdirectory(src)
add_subdirectory(perf)
add_subdirectory(test)

enable_testing()
