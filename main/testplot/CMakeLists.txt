cmake_minimum_required(VERSION 3.27)

project(RunDataPlot)

set(CMAKE_CXX_STANDARD 20)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../source)

# Download package manager if not installed
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
)
include(build/cmake/CPM.cmake)

# Download matplot++
CPMAddPackage(
    NAME matplotplusplus
    GITHUB_REPOSITORY alandefreitas/matplotplusplus
    GIT_TAG origin/master
    SOURCE_DIR matplot
)

# Compilation and link setup:
add_executable(plotting plotting.cpp)

target_sources(plotting
    PRIVATE
        plotting.cpp 
        ${SOURCE_DIR}/runclasses.cpp
        ${SOURCE_DIR}/timestamp.cpp
        ${SOURCE_DIR}/tcxparsing.cpp
        ${SOURCE_DIR}/runsummary.cpp
        )

target_include_directories(plotting
    PUBLIC
        ${INCLUDE_DIR}
        )

target_link_libraries(plotting PUBLIC matplot)



