#include <filesystem>

void walk_directory(std::filesystem::path directory, void (*callback)(std::filesystem::path));