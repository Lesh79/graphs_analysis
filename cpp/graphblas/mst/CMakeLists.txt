set(MST_TARGET_NAME gb-mst)

set(MST_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/algorunner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/algo.cpp
)

add_executable(${MST_TARGET_NAME} ${MST_SRC_FILES})

target_include_directories(
        ${MST_TARGET_NAME} 
        SYSTEM
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../common>"
        "/opt/homebrew/Cellar/suite-sparse/7.11.0/include/suitesparse"
)

target_link_libraries(${MST_TARGET_NAME} PRIVATE /opt/homebrew/Cellar/suite-sparse/7.11.0/lib/libgraphblas.dylib)

target_link_libraries(${MST_TARGET_NAME} PRIVATE LAGraph)

target_link_libraries(${MST_TARGET_NAME} PRIVATE LAGraphX)
