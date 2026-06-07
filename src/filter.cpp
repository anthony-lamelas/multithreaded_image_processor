#include "../include/filter.hpp"
#include <stdexcept>
#include <algorithm>

Image grayscale(const Image& input){

    if (input.channels < 3 || input.channels > 4) {
        throw std::runtime_error("Requires 3 or 4 input channels.");
    }

    Image output(input.width, input.height, 1);

    for (std::size_t y = 0; y < input.height; ++y) {
        for (std::size_t x = 0; x < input.width; ++x) {
            std::size_t index = (y * input.width + x) * input.channels;

            uint8_t r = input.data[index];
            uint8_t g = input.data[index + 1];
            uint8_t b = input.data[index + 2];

            uint8_t gray =
                static_cast<uint8_t>(
                    0.299 * r +
                    0.587 * g +
                    0.114 * b
                );

            output.data[i * output.width + j] = gray;
        }
    }

    return output;
}

Image sharpen(const Image& input) {
    Image output(input.width, input.height, input.channels);
    int kernel[3][3] = { {0, -1, 0}, {-1, 5, -1}, {0, -1, 0} };

    for (std::size_t y = 0; y < input.height; ++y) {
        for (std::size_t x = 0; x < input.width; ++x) {
            for (std::size_t c = 0; c < input.channels; ++c) {
                std::size_t index = (y * input.width + x) * input.channels + c;
                
                // Don't apply convolution to the alpha channel
                if (input.channels == 4 && c == 3) {
                    output.data[index] = input.data[index];
                    continue;
                }

                int sum = 0;
                for (int ky = -1; ky <= 1; ++ky) {
                    for (int kx = -1; kx <= 1; ++kx) {
                        int ny = static_cast<int>(y) + ky;
                        int nx = static_cast<int>(x) + kx;

                        // Clamp to image boundaries
                        ny = std::max(0, std::min(static_cast<int>(input.height) - 1, ny));
                        nx = std::max(0, std::min(static_cast<int>(input.width) - 1, nx));

                        std::size_t n_index = (ny * input.width + nx) * input.channels + c;
                        sum += input.data[n_index] * kernel[ky + 1][kx + 1];
                    }
                }
                
                // Clamp the final value to [0, 255]
                if (sum > 255) sum = 255;
                if (sum < 0) sum = 0;

                output.data[index] = static_cast<uint8_t>(sum);
            }
        }
    }
    return output;
}

Image blur(const Image& input) {
    Image output(input.width, input.height, input.channels);

    for (std::size_t y = 0; y < input.height; ++y) {
        for (std::size_t x = 0; x < input.width; ++x) {
            for (std::size_t c = 0; c < input.channels; ++c) {
                std::size_t index = (y * input.width + x) * input.channels + c;
                
                // Don't blur the alpha channel
                if (input.channels == 4 && c == 3) {
                    output.data[index] = input.data[index];
                    continue;
                }

                int sum = 0;
                for (int ky = -1; ky <= 1; ++ky) {
                    for (int kx = -1; kx <= 1; ++kx) {
                        int ny = static_cast<int>(y) + ky;
                        int nx = static_cast<int>(x) + kx;

                        // Clamp to image boundaries
                        ny = std::max(0, std::min(static_cast<int>(input.height) - 1, ny));
                        nx = std::max(0, std::min(static_cast<int>(input.width) - 1, nx));

                        std::size_t n_index = (ny * input.width + nx) * input.channels + c;
                        sum += input.data[n_index];
                    }
                }
                
                // Average the 9 pixels
                sum /= 9;
                output.data[index] = static_cast<uint8_t>(sum);
            }
        }
    }
    return output;
}

Image invert(const Image& input) {
    Image output(input.width, input.height, input.channels);

    for (std::size_t i = 0; i < input.data.size(); i += input.channels) {
        for (std::size_t c = 0; c < input.channels; ++c) {
            if (input.channels == 4 && c == 3) {
                // Preserve the alpha channel as-is
                output.data[i + c] = input.data[i + c];
            } else {
                // Invert the color channels
                output.data[i + c] = 255 - input.data[i + c];
            }
        }
    }
    return output;
}
