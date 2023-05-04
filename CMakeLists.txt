
cmake_minimum_required(VERSION 3.23)
project(crap)

add_executable(
    crap
    src/main.cpp
    )

target_compile_features(
    crap
    PRIVATE
    cxx_std_17
    )

find_package(Threads)
target_link_libraries(
    crap
    PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    )
