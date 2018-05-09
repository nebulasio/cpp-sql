project(NebulasCppSQL)
cmake_minimum_required(VERSION 3.0)
message(STATUS "This is BINARY dir" ${PROJECT_BINARY_DIR})
message(STATUS "This is SOURCE dir" ${PROJECT_SOURCE_DIR})

add_definitions(-std=c++11)

if(Release)
  message(STATUS "This is a Release version!")
  add_definitions(-O2)
  add_definitions(-DNDEBUG)
else(Release)
  message(STATUS "This is a Debug version!")
  add_definitions(-g)
  add_definitions(-O0)
endif(Release)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})


include_directories(${PROJECT_SOURCE_DIR}/)
include_directories(/usr/include/cppconn)
include_directories(/usr/local/include)

include_directories($ENV{CPPCONN_DIR}/include)
link_directories($ENV{CPPCONN_DIR}/lib)
link_directories(/usr/local/lib)



set(common_libs mysqlcppconn )
add_subdirectory(examples)

