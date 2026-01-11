cmake_minimum_required(VERSION 3.10)
project(MyProject CXX)

add_executable(film src/main.cpp)

# Link against your library
target_link_libraries(film PRIVATE edsdk)
