// using stb_image
#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>

struct RGB
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    RGB(unsigned char r,
        unsigned char g,
        unsigned char b)
    {
        red = r;
        green = g;
        blue = b;
    }

    RGB() = default;
};

// Get string with current year - month - time
std::string get_time_str()
{
    std::time_t t = std::time(0); // get time now
    std::tm *now = std::localtime(&t);
    auto str_timestamp = "_" + std::to_string(now->tm_year + 1900) + '_' + std::to_string(now->tm_mon + 1) + "_" + std::to_string(now->tm_mday) + "_" + std::to_string(now->tm_hour) + "_" + std::to_string(now->tm_min) + "_" + std::to_string(now->tm_sec);
    return str_timestamp;
}

// Write data from array to .ppm file
void save_to_ppm(std::string filename, const int width, const int height, std::vector<std::vector<RGB>> data)
{

    std::cout << "Trying to write PPM data ........." << std::endl;
    std::ofstream output(filename + ".ppm");
    output << "P3\n"
           << width << " " << height << "\n255\n";
    for (int j = height - 1; j >= 0; --j)
    {
        for (int i = 0; i < width; ++i)
        {
            output
                << static_cast<int>(data[i][j].red) << " "
                << static_cast<int>(data[i][j].green) << " "
                << static_cast<int>(data[i][j].blue) << std::endl;
        }
    }
    output.close();
}

void save_to_jpg(const int width, const int height, const std::vector<std::vector<RGB>> data, int channels_num = 3)
{
    std::cout << "Trying to write JPG data ........." << std::endl;

    unsigned char tmp[width * height * channels_num];

    int index = 0;
    for (int j = height - 1; j >= 0; --j)
    {
        for (int i = 0; i < width; ++i)
        {
            tmp[index++] = data[i][j].red;
            tmp[index++] = data[i][j].green;
            tmp[index++] = data[i][j].blue;
        }
    }

    stbi_write_jpg((get_time_str() + "_image.jpg").c_str(), width, height, channels_num, tmp, width * channels_num);
}
#endif