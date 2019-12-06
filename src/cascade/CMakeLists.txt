cmake_minimum_required(VERSION 2.8)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# version
set(cascade_VERSION "0.9")
set(cascade_build_VERSION "0.9.1")

# compiler flags
set(CMAKE_CXX_FLAGS "-fPIC -std=c++1z -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -gdwarf-3 -D_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -ggdb -gdwarf-3 -D_DEBUG -D_PERFORMANCE_DEBUG")

# objectstore tester
add_executable(sample_cascade_store sample_cascade_store.cpp)
target_include_directories(sample_cascade_store PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_DIR}>
)
target_link_libraries(sample_cascade_store dpods)
add_dependencies(sample_cascade_store dpods)
