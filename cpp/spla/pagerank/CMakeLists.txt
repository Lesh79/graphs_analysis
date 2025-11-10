set(PR_TARGET_NAME spla-pagerank)

set(PR_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pr.cpp
)

add_executable(${PR_TARGET_NAME} ${PR_SRC_FILES})

target_include_directories(
        ${PR_TARGET_NAME} 
        SYSTEM
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../common>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../common>"
)

target_link_libraries(${PR_TARGET_NAME} PRIVATE spla)

# Pr squared

set(PR_SQ_TARGET_NAME spla-pagerank-sq)

set(PR_SQ_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pr-sq.cpp
)

add_executable(${PR_SQ_TARGET_NAME} ${PR_SQ_SRC_FILES})

target_include_directories(
        ${PR_SQ_TARGET_NAME} 
        SYSTEM
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../common>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../common>"
)

target_link_libraries(${PR_SQ_TARGET_NAME} PRIVATE spla)