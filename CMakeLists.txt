cmake_minimum_required(VERSION 3.4)
project(wplayer)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_definitions(-std=c++14 -Wall -pedantic -Wextra -DBOOST_ALL_DYN_LINK)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)

find_package(Boost 1.58 REQUIRED program_options filesystem system thread log log_setup)
find_package(OpenSSL REQUIRED)
find_library(MAGIC_LIB magic)
find_library(UCHARDET_LIB uchardet)
find_library(LOG4CPLUS_LIBS log4cplus)

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}:/usr/local/lib/pkgconfig/")
find_package(PkgConfig REQUIRED)
pkg_search_module(MPV REQUIRED /usr/local/lib/pkgconfig/mpv.pc)

include(GNUInstallDirs)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DWPLAYER_DEBUG)
endif(CMAKE_BUILD_TYPE MATCHES Debug)

include_directories(${Boost_INCLUDE_DIRS}) 
link_directories(${Boost_LIBRARY_DIRS} ${MPV_LIBDIR})
include_directories(${MPV_INCLUDE_DIRS})

set(SOURCE_FILES src/main.cpp 
              src/mpv_manager.cpp
              src/file_controller.cpp
              src/files_listing_controller.cpp
              src/http_config.cpp
              src/magic_handler.cpp
              src/not_found_exception.cpp
              src/utils.cpp
              src/player_service.cpp
              src/options_parser.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

message("${Boost_LIBRARIES}")

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${LOG4CPLUS_LIBS})
target_link_libraries(${PROJECT_NAME} ${MPV_STATIC_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${MAGIC_LIB})
target_link_libraries(${PROJECT_NAME} ${UCHARDET_LIB})

target_link_libraries(${PROJECT_NAME} pthread)
target_compile_options(${PROJECT_NAME} PUBLIC ${MPV_CFLAGS_OTHER})


install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_RPM_RELOCATION_PATHS "/${CMAKE_INSTALL_SYSCONFDIR}" "${CMAKE_INSTALL_BINDIR}")


install(DIRECTORY "web/"
  DESTINATION "/var/www/html/wplayer")

install(FILES "config/wplayer.conf.sample"
  DESTINATION "/${CMAKE_INSTALL_SYSCONFDIR}/wplayer")

install(FILES "config/log.conf.sample"
  DESTINATION "/${CMAKE_INSTALL_SYSCONFDIR}/wplayer")
  
install(FILES "config/wplayer.conf.sample"
  DESTINATION "/${CMAKE_INSTALL_SYSCONFDIR}/wplayer"
  RENAME "wplayer.conf")

install(FILES "config/log.conf.sample"
  DESTINATION "/${CMAKE_INSTALL_SYSCONFDIR}/wplayer"
  RENAME "log.conf")

install(FILES "service/wplayer.service" 
    DESTINATION "/${CMAKE_INSTALL_SYSCONFDIR}/systemd/system"
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ
    RENAME "wplayer@.service"
    )
SET(CPACK_PACKAGE_NAME "wplayer")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Web MPV")
SET(CPACK_PACKAGE_VENDOR "Sergiu Giurgiu")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Web MPV")
SET(CPACK_PACKAGE_CONTACT "sgiurgiu11@gmail.com")
SET(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST "/var" "/var/www" "/var/www/html" "/etc" "/etc/systemd" "/etc/systemd/system" "/usr" "/usr/bin" "/etc/systemd/user")

SET(CPACK_PACKAGE_VERSION_MAJOR 0)
SET(CPACK_PACKAGE_VERSION_MINOR 1)
SET(CPACK_PACKAGE_VERSION_PATCH 4)



SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

INCLUDE(CPack)
