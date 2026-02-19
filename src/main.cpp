#include <iostream>

#include <filesystem>
#include "ImageIO.hpp"
#include "Filter.hpp"

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: program input output" << std::endl;
        return 1;
    }

    try
    {
        std::filesystem::path input_path = argv[1];
        std::filesystem::path output_path = argv[2];

        Image image = load(input_path);
        Image output_image = grayscale(image);
        save(output_image, output_path);
    }
    
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
