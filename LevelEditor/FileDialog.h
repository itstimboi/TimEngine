#pragma once

#include <string>
#include <filesystem>

std::string OpenMapDialog();

std::string SaveMapDialog();

std::filesystem::path GetProjectRoot();
std::string GetParentFolderName();