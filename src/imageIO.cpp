#include <iostream>
#include "../include/imageIO.hpp"
#include <stdexcept>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"


Image load(const std::filesystem::path& path)
{
    int width, height, channels;

    // Load raw pixel data from file
    unsigned char* raw = stbi_load(
        path.string().c_str(),
        &width,
        &height,
        &channels,
        0
    );

    if (!raw)
    {
        throw std::runtime_error(
            std::string("Failed to load image: ") + stbi_failure_reason()
        );
    }

    // Construct Image 
    Image img(width, height, channels);

    // Copy data into our managed buffer
    std::size_t size = static_cast<std::size_t>(width) *
                       static_cast<std::size_t>(height) *
                       static_cast<std::size_t>(channels);

    std::memcpy(img.data.data(), raw, size);

    // Free stb memory
    stbi_image_free(raw);

    return img;  
}

void save(const Image& img, const std::filesystem::path& path)
{
    std::string ext = path.extension().string();

    int success = 0;

    if (ext == ".png")
    {
        success = stbi_write_png(
            path.string().c_str(),
            static_cast<int>(img.width),
            static_cast<int>(img.height),
            static_cast<int>(img.channels),
            img.data.data(),
            static_cast<int>(img.width * img.channels)
        );
    }
    else if (ext == ".jpg" || ext == ".jpeg")
    {
        success = stbi_write_jpg(
            path.string().c_str(),
            static_cast<int>(img.width),
            static_cast<int>(img.height),
            static_cast<int>(img.channels),
            img.data.data(),
            100  // quality
        );
    }
    else
    {
        throw std::runtime_error("Unsupported file format");
    }

    if (!success)
    {
        throw std::runtime_error("Failed to save image");
    }
}