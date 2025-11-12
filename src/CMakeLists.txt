cmake_minimum_required(VERSION 3.23)

project(rz_write_sqlfile
        VERSION 0.1.0
        DESCRIPTION "Plugin to export Metadata to sql file"
        HOMEPAGE_URL "https://github.com/Zheng-Bote/rz_write_sqlfile"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROG_EXEC_NAME "${PROJECT_NAME}")
set(PROG_NAME "write SQL-File")
set(PROG_VERSION "${PROJECT_VERSION}")
set(PROG_HOMEPAGE "${PROJECT_HOMEPAGE_URL}")
set(PROG_DESCRIPTION "${PROJECT_DESCRIPTION}")
set(PROG_ORGANIZATION_NAME "ZHENG Robert")
set(PROG_ORGANIZATION_DOMAIN "net.hase-zheng")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(CMAKE_CXX_COMPILER clang++)
endif()

configure_file("${CMAKE_SOURCE_DIR}/configure/rz_config.h.in"
               "${CMAKE_SOURCE_DIR}/includes/rz_config.h")

find_package(Qt6 REQUIRED COMPONENTS Core Sql)

qt_standard_project_setup()

add_library(
  ${PROJECT_NAME} SHARED
  rz_write_sqlfile.cpp includes/rz_write_sqlfile.hpp
  includes/rz_photo-gallery_plugins.hpp
  includes/database.cpp includes/database.hpp includes/sqlite3.cpp includes/sqlite3.hpp


)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Sql
)

target_compile_definitions(${PROJECT_NAME} PRIVATE RZ_WRITE_SQLFILE_LIBRARY)
