/**
 * @file config.hpp.in
 * @author ZHENG Robert (robert.hase-zheng.net)
 * @brief configuration
 * @version 2.0.0
 * @date 2025-11-15
 * 
 * @copyright Copyright (c) 2025 ZHENG Robert
 * 
 */
#pragma once

#include <cstdint>
#include <string_view>

static constexpr std::string_view project_name = "rz_write_sqlfile";
static constexpr std::string_view prog_longname = "write SQL-File";
static constexpr std::string_view project_description = "Plugin to export Metadata to sql file";

static constexpr std::string_view project_executable = "rz_write_sqlfile";

static constexpr std::string_view project_version = "0.4.0";
static constexpr std::int32_t project_version_major{0};
static constexpr std::int32_t project_version_minor{4};
static constexpr std::int32_t project_version_patch{0};

static constexpr std::string_view project_homepage = "https://github.com/Zheng-Bote/rz_write_sqlfile";
static constexpr std::string_view project_author = "ZHENG Bote";
static constexpr std::string_view project_organization_name = "ZHENG Robert";
static constexpr std::string_view project_organization_domain = "net.hase-zheng";

static constexpr std::string_view cpp_standard_version = "c++23";
static constexpr std::string_view cpp_compiler = "Clang 20.1.8";
