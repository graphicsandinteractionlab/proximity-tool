cmake_minimum_required(VERSION 3.10)

project(ProximityTool)

# only C++11
set(CMAKE_CXX_STANDARD 11)

# internal cmake modules
#set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/etc/cmake ${CMAKE_MODULE_PATH})

# boilerplate locations for build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE PATH "Executables go here")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE PATH "Libraries go here")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE PATH "Standalone DLLs go here")

# boilerplate settings for shared libs
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(src)
