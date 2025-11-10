set(MSBFS_TARGET_NAME spla-msbfs)

set(MSBFS_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/msbfs.cpp
)

add_executable(${MSBFS_TARGET_NAME} ${MSBFS_SRC_FILES})

target_include_directories(
        ${MSBFS_TARGET_NAME} 
        SYSTEM
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../common>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../common>"
)

target_link_libraries(${MSBFS_TARGET_NAME} PRIVATE spla)