
cmake_minimum_required(VERSION 3.23)
project(crap)

add_executable(
    crap
    src/main.cpp
    src/args.cpp
    src/status.cpp
    )

target_compile_features(
    crap
    PRIVATE
    cxx_std_20
    )

find_package(Threads)

include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 9.1.0 # Replace with the desired version or commit
)

FetchContent_MakeAvailable(fmt)

target_link_libraries(
    crap
    PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    fmt::fmt
    )

add_subdirectory(
    test
)
