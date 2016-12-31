cmake_minimum_required (VERSION 2.8)

project (MyTinySTL CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

include_directories("$(PROJECT_DIR)/MyTinySTL/Test")
add_subdirectory(MyTinySTL/Test)
