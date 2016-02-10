cmake_minimum_required(VERSION 3.4)
project(wplayer)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
add_definitions(-std=c++14 -Wall -pedantic -Wextra -DBOOST_ALL_DYN_LINK)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)

find_package( Boost 1.58 REQUIRED program_options filesystem system thread log log_setup)
find_package(OpenSSL REQUIRED)
find_library(HTTP_LIB httpp)
find_library(COMMONPP_LIB commonpp)
find_library(MAGIC_LIB magic)

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}:/usr/local/lib/pkgconfig/")
find_package(PkgConfig REQUIRED)
pkg_search_module(MPV REQUIRED /usr/local/lib/pkgconfig/mpv.pc)
#pkg_search_module(HTTP REQUIRED libnghttp2)

include_directories(${Boost_INCLUDE_DIRS}) 
link_directories(${Boost_LIBRARY_DIRS} ${MPV_LIBDIR})
include_directories(${MPV_INCLUDE_DIRS})
#include_directories(${HTTP_INCLUDE_DIRS})

set(SOURCE_FILES src/main.cpp 
              src/mpv_manager.cpp
              src/http_controller.cpp
              src/file_controller.cpp
              src/api_controller.cpp
              src/files_listing_controller.cpp
              src/movie_controller.cpp
              src/http_connection_handler.cpp
              src/http_config.cpp
              src/magic_handler.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

#message("${Boost_LIBRARIES}")

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${MPV_STATIC_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${HTTP_LIB})
target_link_libraries(${PROJECT_NAME} ${COMMONPP_LIB})
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${MAGIC_LIB})
target_link_libraries(${PROJECT_NAME} pthread)
target_compile_options(${PROJECT_NAME} PUBLIC ${MPV_CFLAGS_OTHER})


install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
