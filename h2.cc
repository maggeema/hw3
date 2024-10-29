#include <iostream>
#include <string>
#include "image.h"

using namespace ComputerVisionProjects;

void h2(Image* image, int threshold) {
    for (size_t i = 0; i < image->num_rows(); i++) {
        for (size_t j = 0; j < image->num_columns(); j++) {
            int pixel = image->GetPixel(i, j);

            if (pixel >= threshold) {
                image->SetPixel(i, j, 255);  // Set to white
            } else {
                image->SetPixel(i, j, 0);    // Set to black
            }
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Usage: %s {input gray-level image} {input gray-level threshold} {output binary image}\n", argv[0]);
        return 1;
    }

    const std::string input_file(argv[1]);
    const std::string output_file(argv[3]);
    int threshold = std::stoi(argv[2]);  // Convert threshold to an integer.

    Image an_image;

    // Read input image
    if (!ReadImage(input_file, &an_image)) {
        std::cout << "Can't open file " << input_file << std::endl;
        return 1;
    }

    // Print dimensions of the read image
    std::cout << "Read image successfully, dimensions: " 
              << an_image.num_rows() << " x " 
              << an_image.num_columns() << std::endl;

    // Apply thresholding to convert the grayscale image to binary
    h2(&an_image, threshold);

    // Write the binary image to the output file
    if (!WriteImage(output_file, an_image)) {
        std::cout << "Can't write to file " << output_file << std::endl;
        return 1;
    }

    return 0;
}
