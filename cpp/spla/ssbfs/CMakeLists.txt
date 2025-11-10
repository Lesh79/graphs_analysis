set(SSBFS_TARGET_NAME spla-ssbfs)

set(SSBFS_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ssbfs.cpp
)

add_executable(${SSBFS_TARGET_NAME} ${SSBFS_SRC_FILES})

target_include_directories(
        ${SSBFS_TARGET_NAME} 
        SYSTEM
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../common>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../common>"
)

target_link_libraries(${SSBFS_TARGET_NAME} PRIVATE spla)