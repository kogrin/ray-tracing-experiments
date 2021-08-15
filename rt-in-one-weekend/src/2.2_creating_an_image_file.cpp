#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>
#include <array>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
// using these data structures:
using std::array;
using std::vector;

array<unsigned char, 3> get_gradient_pixel(const int i, const int j, const int width, const int height)
{
    auto r = double(i) / (width - 1);
    auto g = double(j) / (height - 1);
    auto b = 0.25;

    array<unsigned char, 3> result = {
        static_cast<unsigned int>(255.999 * r),
        static_cast<unsigned int>(255.999 * g),
        static_cast<unsigned int>(255.999 * b)};

    return result;
}

// Generate pixel data and write it to array
vector<unsigned char> get_gradient(const int width, const int height, const int channels_num = 3)
{
    vector<unsigned char> pixels(width * height * channels_num);
    std::cout << "Generating data .................." << std::endl;
    int index = 0;
    for (int j = height - 1; j >= 0; --j)
    {
        for (int i = 0; i < width; ++i)
        {
            auto pixel = get_gradient_pixel(i, j, width, height);
            pixels[index++] = pixel[0];
            pixels[index++] = pixel[1];
            pixels[index++] = pixel[2];
        }
    }

    return pixels;
}

// Get string with current year - month - time
std::string get_time_str()
{
    std::time_t t = std::time(0); // get time now
    std::tm *now = std::localtime(&t);
    auto str_timestamp = "_" + std::to_string(now->tm_year + 1900) + '_' + std::to_string(now->tm_mon + 1) + "_" + std::to_string(now->tm_mday) + "_" + std::to_string(now->tm_hour) + "_" + std::to_string(now->tm_min) + "_" + std::to_string(now->tm_sec);
    return str_timestamp;
}

void save_to_ppm(const std::string filename, const int width, const int height, const vector<unsigned char> &data, const int channels_num = 3)
{
    std::cout << "Trying to write PPM data ........." << std::endl;
    std::ofstream output(filename + ".ppm");
    output << "P3\n"
           << width << " " << height << "\n255\n";

    for (int index = 0; index < width * height * channels_num; index += 3)
    {
        output << (unsigned int)data[index] << " "
               << (unsigned int)data[index + 1] << " "
               << (unsigned int)data[index + 2] << std::endl;
    }

    // close file
    output.close();
}

int main()
{
    // Image parameters
    const int channels_num = 3;
    const int image_width = 256;
    const int image_height = 256;

    // Generate data
    auto pixels_data = get_gradient(image_width, image_height);

    // save to .ppm file
    save_to_ppm((get_time_str() + "_image").c_str(), image_width, image_height, pixels_data);
    std::cout << "Trying to write JPG data ........." << std::endl;
    stbi_write_jpg((get_time_str() + "_image.jpg").c_str(), image_width, image_height, channels_num, pixels_data.data(), image_width * channels_num);
    return 0;
}