cmake_minimum_required(VERSION 3.17)
add_subdirectory(cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/LAGraph)

set(SPLA_BUILD_TESTS OFF CACHE BOOL "Disable building of Spla unit tests" FORCE)
set(SPLA_BUILD_EXAMPLES OFF CACHE BOOL "Disable building of Spla examples" FORCE)
set(SPLA_BUILD_OPENCL ON CACHE BOOL "Build library with opencl backend" FORCE)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/spla)