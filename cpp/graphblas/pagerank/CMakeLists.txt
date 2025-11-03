set(PR_TARGET_NAME gb-pagerank)

find_package(GraphBLAS REQUIRED)

set(PR_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/algorunner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/algo.cpp
)

add_executable(${PR_TARGET_NAME} ${PR_SRC_FILES})

target_include_directories(
        ${PR_TARGET_NAME} 
        SYSTEM
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../common>"
)

target_link_libraries(${PR_TARGET_NAME} PRIVATE SuiteSparse::GraphBLAS LAGraphX)