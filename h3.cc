#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include "image.h"

using namespace ComputerVisionProjects;

void hough3(Image* image, const std::string& output_image, const std::string& output_array) {
    int width = image->num_columns();
    int height = image->num_rows();
    int MAX_RHO = static_cast<int>(sqrt(width * width + height * height));
    int MAX_THETA = 180;
    double pi = 3.14159265358979323846;

    // Create a new image for the Hough Transform space
    Image hough_image;
    hough_image.AllocateSpaceAndSetSize(MAX_THETA, 2 * MAX_RHO); // 2 * MAX_RHO for positive/negative rho
    hough_image.SetNumberGrayLevels(255);

    // Use a 1D vector to store accumulator array as it’s more memory-efficient
    std::vector<std::vector<int>> acc_array(MAX_THETA, std::vector<int>(2 * MAX_RHO, 0));

    // Accumulate votes
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (image->GetPixel(y, x) > 0) { // Edge pixel
                for (int theta = 0; theta < MAX_THETA; ++theta) {
                    double radians = theta * pi / 180.0;
                    int rho = static_cast<int>(x * cos(radians) + y * sin(radians)) + MAX_RHO; // Shift rho
                    if (rho >= 0 && rho < 2 * MAX_RHO) {
                        acc_array[theta][rho]++;
                    }
                }
            }
        }
    }

    // Find max vote count for normalization
    int max_votes = 0;
    for (int theta = 0; theta < MAX_THETA; ++theta) {
        for (int rho = 0; rho < 2 * MAX_RHO; ++rho) {
            if (acc_array[theta][rho] > max_votes) {
                max_votes = acc_array[theta][rho];
            }
        }
    }

    // Populate hough_image with normalized votes
    for (int theta = 0; theta < MAX_THETA; ++theta) {
        for (int rho = 0; rho < 2 * MAX_RHO; ++rho) {
            int brightness = static_cast<int>(1000 * acc_array[theta][rho] / max_votes);
            hough_image.SetPixel(theta, rho, brightness);
        }
    }

    // Write the Hough Transform image
    if (!WriteImage(output_image, hough_image)) {
        std::cerr << "Error: Cannot write image " << output_image << std::endl;
    }

    // Save the accumulator array to a file
    std::ofstream array_file(output_array);
    if (array_file.is_open()) {
        for (int theta = 0; theta < MAX_THETA; ++theta) {
            for (int rho = 0; rho < 2 * MAX_RHO; ++rho) {
                array_file << acc_array[theta][rho] << " ";
            }
            array_file << "\n";
        }
        array_file.close();
    } else {
        std::cerr << "Error: Cannot write Hough array to " << output_array << std::endl;
    }
}

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " {input binary edge image} {output gray-level Hough image} {output Hough-voting array}\n";
        return 1;
    }

    const std::string input_file(argv[1]);
    const std::string output_image(argv[2]);
    const std::string output_array(argv[3]);

    Image edge_image;
    if (!ReadImage(input_file, &edge_image)) {
        std::cerr << "Error: Cannot read image " << input_file << std::endl;
        return 1;
    }

    hough3(&edge_image, output_image, output_array);

    return 0;
}
