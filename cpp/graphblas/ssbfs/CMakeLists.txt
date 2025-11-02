set(SSBFS_TARGET_NAME ssbfs-pagerank)

set(SSBFS_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/algorunner.cpp
)

add_executable(${SSBFS_TARGET_NAME} ${SSBFS_SRC_FILES})

target_include_directories(
        ${SSBFS_TARGET_NAME} 
        SYSTEM
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../common>"
        # "/opt/homebrew/Cellar/suite-sparse/7.11.0/include/suitesparse"
)

# target_link_libraries(${SSBFS_TARGET_NAME} PRIVATE /opt/homebrew/Cellar/suite-sparse/7.11.0/lib/libgraphblas.dylib)
