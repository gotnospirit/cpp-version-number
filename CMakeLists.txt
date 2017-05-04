cmake_minimum_required(VERSION 3.4)

project(t_version)

add_definitions(-D_WIN32_WINNT=_WIN32_WINNT_WIN8 -DWIN32_LEAN_AND_MEAN)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

if ("${UNITTESTCPP_DIR}" STREQUAL "")
  message(FATAL_ERROR "please provide unittest directory as UNITTESTCPP_DIR")
endif ()

INCLUDE_DIRECTORIES("${UNITTESTCPP_DIR}/UnitTest++")
LINK_DIRECTORIES("${UNITTESTCPP_DIR}/builds/Debug")
INCLUDE_DIRECTORIES(.)

ADD_EXECUTABLE(t_version EXCLUDE_FROM_ALL
  Version.cpp
  t_version.cpp
)
target_link_libraries(t_version "UnitTest++")
SET_TARGET_PROPERTIES(t_version PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
