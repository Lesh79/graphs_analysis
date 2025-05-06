cmake_minimum_required(VERSION 3.10)
project(GraphBlasAlgorithms)

find_package(GraphBLAS REQUIRED)
find_package(LAGraph REQUIRED)

add_executable(prim-mst prim/main.cpp)
target_link_libraries(prim-mst PRIVATE GraphBLAS::GraphBLAS SuiteSparse::LAGraph)

add_executable(boruvka-mst boruvka/main.cpp)
target_link_libraries(boruvka-mst PRIVATE GraphBLAS::GraphBLAS SuiteSparse::LAGraph)
