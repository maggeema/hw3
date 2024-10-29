#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include "image.h"

using namespace ComputerVisionProjects;

struct HoughVote {
    int theta;
    int rho;
    int votes;
};

// Function to read the Hough voting array from a text file
bool ReadHoughVotes(const std::string &filename, std::vector<HoughVote> &votes_array) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error: Cannot open Hough voting array file " << filename << std::endl;
        return false;
    }

    int theta, rho, votes;
    while (infile >> theta >> rho >> votes) {
        votes_array.push_back({theta, rho, votes});
    }
    return true;
}

// Function to draw lines on the original image based on Hough votes
void DrawDetectedLines(Image *image, const std::vector<HoughVote> &votes_array, int threshold) {
    int max_rho = static_cast<int>(sqrt(image->num_rows() * image->num_rows() + image->num_columns() * image->num_columns()));

    for (const auto &vote : votes_array) {
        if (vote.votes >= threshold) {
            double theta = vote.theta * M_PI / 180.0;
            double rho = vote.rho - max_rho;  // Adjust rho to allow negative values

            for (int x = 0; x < image->num_columns(); ++x) {
                int y = static_cast<int>((rho - x * cos(theta)) / sin(theta));
                if (y >= 0 && y < image->num_rows()) {
                    image->SetPixel(y, x, 255);  // Draw line in white
                }
            }
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " {input original image} {input Hough array text file} {Hough threshold value} {output line image}" << std::endl;
        return 1;
    }

    const std::string input_image_file(argv[1]);
    const std::string hough_array_file(argv[2]);
    int threshold = std::stoi(argv[3]);
    const std::string output_image_file(argv[4]);

    // Read the original image
    Image original_image;
    if (!ReadImage(input_image_file, &original_image)) {
        std::cerr << "Error: Cannot read image " << input_image_file << std::endl;
        return 1;
    }

    // Read the Hough voting array from the text file
    std::vector<HoughVote> votes_array;
    if (!ReadHoughVotes(hough_array_file, votes_array)) {
        std::cerr << "Error: Cannot read Hough voting array file " << hough_array_file << std::endl;
        return 1;
    }

    // Draw the detected lines on the image
    DrawDetectedLines(&original_image, votes_array, threshold);

    // Save the output image with lines drawn
    if (!WriteImage(output_image_file, original_image)) {
        std::cerr << "Error: Cannot write output image " << output_image_file << std::endl;
        return 1;
    }

    std::cout << "Detected lines have been drawn and saved to " << output_image_file << std::endl;
    return 0;
}
