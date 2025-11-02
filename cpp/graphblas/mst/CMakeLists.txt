set(MST_TARGET_NAME gb-mst)

set(MST_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${MST_TARGET_NAME} ${MST_SRC_FILES})

target_include_directories(
        ${MST_TARGET_NAME} 
        SYSTEM
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../common>"
)
