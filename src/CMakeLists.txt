cmake_minimum_required(VERSION 3.23)

project(
    rz_write_sqlfile
    VERSION 0.2.0
    DESCRIPTION "Plugin to export Metadata to sql file"
    HOMEPAGE_URL "https://github.com/Zheng-Bote/rz_write_sqlfile"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE_NAME "${PROJECT_NAME}")
set(PROG_LOGNAME "write SQL-File")
set(PROG_AUTHOR "ZHENG Bote")
set(PROG_ORGANIZATION_NAME "ZHENG Robert")
set(PROG_ORGANIZATION_DOMAIN "net.hase-zheng")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_CXX_COMPILER clang++)
endif()

add_subdirectory(configure)

option(USE_CONAN "Whether to use Conan." OFF)
option(USE_VCPKG "Whether to use VCPKG." OFF)
option(USE_CPM "Whether to use CPM." OFF)

option(ENABLE_WARNINGS "Enable to add warnings to a target." ON)
option(ENABLE_WARNINGS_AS_ERRORS "Enable to treat warnings as errors." OFF)

option(ENABLE_TESTING "Enable a Unit Testing build." OFF)
option(ENABLE_COVERAGE "Enable a Code Coverage build." OFF)

option(ENABLE_CLANG_TIDY "Enable to add clang tidy." ON)

option(ENABLE_SANITIZE_ADDR "Enable address sanitize." OFF)
option(ENABLE_SANITIZE_UNDEF "Enable undefined sanitize." OFF)
option(ENABLE_SANITIZE_LEAK "Enable leak sanitize (Gcc/Clang only)." OFF)
option(ENABLE_SANITIZE_THREAD "Enable thread sanitize (Gcc/Clang only)." OFF)

option(ENABLE_CLANG_FORMAT "Enable to add clang-format." OFF)
option(ENABLE_CMAKE_FORMAT "Enable to add cmake-format." OFF)

option(ENABLE_LTO "Enable to add Link Time Optimization." ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
message("source dir: ${CMAKE_SOURCE_DIR}")
include(ConfigSafeGuards)
include(AddGitSubmodule)
include(Docs)
include(Tools)

if(ENABLE_WARNINGS)
    include(Warnings)
endif()

add_cmake_format_target()
add_clang_format_target()

if(ENABLE_CLANG_TIDY MATCHES ON)
    message("DEBUG mode")
    set(CMAKE_CXX_CLANG_TIDY clang-tidy; -header-filter=.;
                             -checks=cppcoreguidelines-*)
    #include(ClangTidy)
    #addclangtidy(test)
endif()

if(ENABLE_SANITIZE_ADDR
   OR ENABLE_SANITIZE_UNDEF
   OR ENABLE_SANITIZE_LEAK
   OR ENABLE_SANITIZE_THREAD)
    include(Sanitizer)
    add_sanitizer_flags()
endif()

if(ENABLE_TESTING AND ENABLE_COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
endif()

if(ENABLE_LTO)
    include(LTO)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Sql)

qt_standard_project_setup()

add_library(
    ${PROJECT_NAME} SHARED
    rz_write_sqlfile.cpp
    includes/rz_write_sqlfile.hpp
    includes/rz_photo-gallery_plugins.hpp
    includes/database.cpp
    includes/database.hpp
    includes/sqlite3.cpp
    includes/sqlite3.hpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Sql)

target_compile_definitions(${PROJECT_NAME} PRIVATE RZ_WRITE_SQLFILE_LIBRARY)
