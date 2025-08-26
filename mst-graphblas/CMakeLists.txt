cmake_minimum_required(VERSION 3.17)
project(GraphBlasMST)

find_package(GraphBLAS REQUIRED)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/LAGraph)

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
target_link_libraries(mst PRIVATE SuiteSparse::GraphBLAS LAGraphX)