cmake_minimum_required(VERSION 2.6)
project(wplayer)

add_executable(wplayer main.cpp)

install(TARGETS wplayer RUNTIME DESTINATION bin)
