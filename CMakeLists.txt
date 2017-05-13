project (mnist CXX)
cmake_minimum_required (VERSION 2.6)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -std=c++1z")

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set (SOURCES
    main.cpp)

add_executable (mnist ${SOURCES})
target_link_libraries (mnist)
