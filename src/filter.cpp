#include "../include/filter.hpp"
#include <stdexcept>

Image grayscale(const Image& input){

    if (input.channels < 3 || input.channels > 4) {
        throw std::runtime_error("Requires 3 or 4 input channels.");
    }

    Image output(input.width, input.height, 1);

    for (std::size_t i = 0; i < input.height; ++i) {

        for (std::size_t j = 0; j < input.width; ++j) {
            
            std::size_t index = (i * input.width + j) * input.channels;

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