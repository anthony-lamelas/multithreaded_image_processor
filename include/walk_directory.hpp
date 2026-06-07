#pragma once
#include <filesystem>
#include <functional>

void walk_directory(const std::filesystem::path& directory, const std::function<void(std::filesystem::path)>& callback);