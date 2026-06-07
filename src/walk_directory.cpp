#include "../include/walk_directory.hpp"
#include <iostream>
#include <string>
#include <algorithm>

void walk_directory(const std::filesystem::path& directory, const std::function<void(std::filesystem::path)>& callback)
{
    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.is_regular_file())
        {
            std::string ext = entry.path().extension().string();
            
            // Convert extension to lowercase so we catch .JPG, .Png, etc.
            std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c){ return std::tolower(c); });

            if (ext == ".png" || ext == ".jpg" || ext == ".jpeg")
            {
                callback(entry.path());
            }
        }
    }
}
