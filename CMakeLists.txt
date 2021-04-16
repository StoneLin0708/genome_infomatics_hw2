cmake_minimum_required(VERSION 3.14)

project(hw2)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -fsanitize=address")

find_package(fmt)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

enable_testing()
add_subdirectory(src)
add_subdirectory(test)