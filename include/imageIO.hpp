#pragma once
#include "image.hpp"
#include <filesystem>

Image load(const std::filesystem::path& path);
void save(const Image& img, const std::filesystem::path& path);