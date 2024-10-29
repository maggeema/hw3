#include <iostream>
#include <string>
#include <cmath>  // Include cmath for sqrt
#include "image.h"

using namespace ComputerVisionProjects;

void SobelEdgeDetection(Image* image, int threshold) {
    // Sobel kernels
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

    // Temporary image for storing gradient magnitudes
    Image edge_image;
    edge_image.AllocateSpaceAndSetSize(image->num_rows(), image->num_columns());
    edge_image.SetNumberGrayLevels(image->num_gray_levels());

    for (int y = 1; y < image->num_rows() - 1; ++y) {
        for (int x = 1; x < image->num_columns() - 1; ++x) {
            int gx_sum = 0, gy_sum = 0;
            
            // Apply Sobel operator
            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    int pixel_value = image->GetPixel(y + j, x + i);
                    gx_sum += gx[j + 1][i + 1] * pixel_value;
                    gy_sum += gy[j + 1][i + 1] * pixel_value;
                }
            }

            // Calculate gradient magnitude
            int magnitude = static_cast<int>(std::sqrt(gx_sum * gx_sum + gy_sum * gy_sum));
            int max_value = image->num_gray_levels() - 1; // Assuming 0 to max_value

            // Apply threshold to emphasize strong edges
            edge_image.SetPixel(y, x, (magnitude > threshold) ? std::min(magnitude, max_value) : 0);
        }
    }

    // Copy results back to the original image
    for (int y = 0; y < image->num_rows(); ++y) {
        for (int x = 0; x < image->num_columns(); ++x) {
            image->SetPixel(y, x, edge_image.GetPixel(y, x));
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " {input gray-level image} {output gray-level edge image} {threshold}\n";
        return 1;
    }

    const std::string input_file(argv[1]);
    const std::string output_file(argv[2]);
    int threshold = std::stoi(argv[3]);

    // Check threshold is non-negative
    if (threshold < 0) {
        std::cerr << "Error: Threshold must be non-negative.\n";
        return 1;
    }

    Image an_image;
    if (!ReadImage(input_file, &an_image)) {
        std::cerr << "Error: Cannot read image " << input_file << std::endl;
        return 1;
    }

    SobelEdgeDetection(&an_image, threshold);

    if (!WriteImage(output_file, an_image)) {
        std::cerr << "Error: Cannot write image " << output_file << std::endl;
        return 1;
    }

    std::cout << "Edge detection completed. Output saved to " << output_file << std::endl;
    return 0;
}
