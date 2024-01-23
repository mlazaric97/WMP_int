cmake_minimum_required(VERSION 3.16.2)

if (DEFINED PROJECT_NAME)
	set(SUBPROJECT ON)
else()
	set(SUBPROJECT OFF)
endif()

project(WMP LANGUAGES CXX Fortran)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(libWMP/c++)
add_subdirectory(libWMP/interface)
add_subdirectory(libWMP/fortran)

if (NOT SUBPROJECT)
   add_subdirectory(utils)
endif()

