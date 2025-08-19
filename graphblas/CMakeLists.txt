cmake_minimum_required(VERSION 3.10)
project(GraphBlasAlgorithms)

find_package(GraphBLAS REQUIRED)
find_package(LAGraph REQUIRED)

set(SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/algos/boruvka.cpp
    )

add_executable(mst ${SRC_FILES})
target_include_directories(
        mst 
        SYSTEM
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(mst PRIVATE GraphBLAS::GraphBLAS SuiteSparse::LAGraphX)