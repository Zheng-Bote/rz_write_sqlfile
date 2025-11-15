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
#include <string>

static const std::string project_name = "rz_write_sqlfile";
static const std::string prog_longname = "write SQL-File";
static const std::string project_description = "Plugin to export Metadata to sql file";

static const std::string project_executable = "rz_write_sqlfile";

static const std::string project_version = "0.2.0";
static const std::int32_t project_version_major{0};
static const std::int32_t project_version_minor{2};
static const std::int32_t project_version_patch{0};

static const std::string project_homepage = "https://github.com/Zheng-Bote/rz_write_sqlfile";
static const std::string project_author = "ZHENG Bote";
static const std::string project_organization_name = "ZHENG Robert";
static const std::string project_organization_domain = "net.hase-zheng";

static const std::string cpp_standard_version = "c++23";
static const std::string cpp_compiler = "Clang 20.1.8";
