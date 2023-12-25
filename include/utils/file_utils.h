#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <filesystem>
#include <vector>
#include <string>

std::error_code renameFile(std::string& from, std::string& to);
std::error_code getFiles(std::string& directory, std::vector<std::filesystem::path>& vec);

#endif