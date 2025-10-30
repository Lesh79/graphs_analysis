set(MSBFS_TARGET_NAME msbfs)

set(MSBFS_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${MSBFS_TARGET_NAME} ${MSBFS_SRC_FILES})

target_include_directories(
        ${MSBFS_TARGET_NAME} 
        SYSTEM
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../common>"
)

target_link_libraries(${MSBFS_TARGET_NAME} PRIVATE spla)