cmake_minimum_required(VERSION 3.4)
project(wplayer)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
add_definitions(-std=c++14 -Wall -pedantic -Wextra)
find_package( Boost 1.58 REQUIRED program_options filesystem system)
find_package(SDL2 REQUIRED)

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}:/usr/local/lib/pkgconfig/")
find_package(PkgConfig REQUIRED)
pkg_search_module(MPV REQUIRED /usr/local/lib/pkgconfig/mpv.pc)
pkg_search_module(HTTP REQUIRED libmicrohttpd)

include_directories(${Boost_INCLUDE_DIRS}) 
link_directories(${Boost_LIBRARY_DIRS} ${MPV_LIBDIR})
include_directories(${MPV_INCLUDE_DIRS})
include_directories(${HTTP_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIR}) 

set(SOURCE_FILES src/main.cpp src/mpv_manager.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})


target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${MPV_STATIC_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${HTTP_LIBRARIES})
target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} dl)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
target_compile_options(${PROJECT_NAME} PUBLIC ${MPV_CFLAGS_OTHER})
target_compile_options(${PROJECT_NAME} PUBLIC ${HTTP_CFLAGS_OTHER})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
