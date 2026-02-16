#pragma once
#include <cstdint>
#include <vector>
#include <cstddef>
#include <stdexcept>

struct Image {
    std::size_t width;
    std::size_t height;
    std::size_t channels;
    std::vector<uint8_t> data;

    Image(std::size_t w, std::size_t h, std::size_t c) 
        : width(w), height(h), channels(c), data(w * h * c) {
            if (w == 0 || h == 0) {
                throw std::invalid_argument("Dimensions must be greater than 0.");
            }

            if (c != 1 && c != 3 && c != 4) {
                throw std::invalid_argument("Invalid number of channels.");
            }
        }

    Image(const Image&) = delete;  // Not allowed to be copied 
    Image& operator=(const Image&) = delete;

    Image(Image&&) noexcept = default;  // Use move constructor
    Image& operator=(Image&&) noexcept = default;

};