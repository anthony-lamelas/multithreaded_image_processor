#include <iostream>
#include <filesystem>
#include <string>
#include "../include/imageIO.hpp"
#include "../include/filter.hpp"
#include "../include/thread_pool.hpp"
#include "../include/walk_directory.hpp"

int main(int argc, char* argv[])
{
    // Check for correct arguments
    if (argc < 4)
    {
        std::cout << "Usage: imgtool <input_dir> <output_dir> <filter_name>" << std::endl;
        std::cout << "Available filters: grayscale, sharpen, blur, invert" << std::endl;
        return 1;
    }

    std::filesystem::path input_dir = argv[1];
    std::filesystem::path output_dir = argv[2];
    std::string filter_name = argv[3];

    if (!std::filesystem::exists(input_dir)) {
        std::cerr << "Error: Input directory does not exist!" << std::endl;
        return 1;
    }

    // Create output directory if it doesn't exist
    std::filesystem::create_directories(output_dir);

    // Initialize ThreadPool with the number of CPU cores available
    unsigned int cores = std::thread::hardware_concurrency();
    if (cores == 0) cores = 4; // Fallback
    ThreadPool pool(cores);
    
    std::cout << "Started ThreadPool with " << cores << " workers." << std::endl;

    // Define callback function. Captures the 'pool', 'output_dir', and 'filter_name'
    auto process_image = [&](std::filesystem::path img_path) {
        
        // Enqueue the job
        pool.enqueue([img_path, output_dir, filter_name]() {
            try {
                std::cout << "Thread " << std::this_thread::get_id() << " processing: " << img_path.filename() << std::endl;
                
                Image img = load(img_path);
                std::filesystem::path out_path = output_dir / img_path.filename();

                // Apply filter and save immediately
                if (filter_name == "grayscale") {
                    save(grayscale(img), out_path);
                } else if (filter_name == "sharpen") {
                    save(sharpen(img), out_path);
                } else if (filter_name == "blur") {
                    save(blur(img), out_path);
                } else if (filter_name == "invert") {
                    save(invert(img), out_path);
                } else {
                    std::cerr << "Unknown filter: " << filter_name << std::endl;
                }

            } catch (const std::exception& e) {
                std::cerr << "Error processing " << img_path << ": " << e.what() << std::endl;
            }
        });
    };

    // Walk the directory
    walk_directory(input_dir, process_image);

    std::cout << "All jobs enqueued. Waiting for background threads to finish..." << std::endl;

    return 0;
}
