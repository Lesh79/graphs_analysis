set(MSBFS_TARGET_NAME gb-msbfs)

set(MSBFS_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/algorunner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/algo.cpp
)

add_executable(${MSBFS_TARGET_NAME} ${MSBFS_SRC_FILES})

target_include_directories(
        ${MSBFS_TARGET_NAME} 
        SYSTEM
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../common>"
        "/opt/homebrew/Cellar/suite-sparse/7.11.0/include/suitesparse"
)

target_link_libraries(${MSBFS_TARGET_NAME} PRIVATE /opt/homebrew/Cellar/suite-sparse/7.11.0/lib/libgraphblas.dylib)
