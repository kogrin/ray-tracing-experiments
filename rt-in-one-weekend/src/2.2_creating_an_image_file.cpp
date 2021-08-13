#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

struct RGB
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    RGB() = default;
};


// Using template is a trick to allow functions access to
// 2d array withot usage of pointers
// or other overcomplicated data structures
template <size_t rows, size_t cols>
// Generate pixel data and write it to array
void get_gradient_rgb(const int width, const int height, RGB (&data)[rows][cols])
{
    std::cout << "Generating data .................." << std::endl;
    int index = 0;
    for (int j = height - 1; j >= 0; --j)
    {
        for (int i = 0; i < width; ++i)
        {
            auto r = double(i) / (width - 1);
            auto g = double(j) / (height - 1);
            auto b = 0.25;

            data[i][j].red = static_cast<unsigned char>(255.999 * r);
            data[i][j].green = static_cast<unsigned char>(255.999 * g);
            data[i][j].blue = static_cast<unsigned char>(255.999 * b);
        }
    }
}


template <size_t rows, size_t cols>
// Write data from array to .ppm file
bool save_to_ppm(std::string filename, const int width, const int height, const RGB (&data)[rows][cols])
{
    try
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
                    << (int)data[i][j].red << " "
                    << (int)data[i][j].green << " "
                    << (int)data[i][j].blue << std::endl;
            }
        }
        output.close();
    }
    catch (const std::exception &err)
    {
        return false;
    }

    return true;
}


// Get string with current year - month - time
std::string get_time_str()
{
    std::time_t t = std::time(0); // get time now
    std::tm *now = std::localtime(&t);
    auto str_timestamp = "_"
        + std::to_string(now->tm_year + 1900)
        + '_' + std::to_string(now->tm_mon + 1)
        + "_" + std::to_string(now->tm_mday)
        + "_" + std::to_string(now->tm_hour)
        + "_" + std::to_string(now->tm_min)
        + "_" + std::to_string(now->tm_sec);
    return str_timestamp;
}


template <size_t rows, size_t cols>
void save_to_jpg(const int width, const int height, const RGB (&data)[rows][cols], int channels_num = 3)
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
    //stbi_write_jpg("image.jpg", width, height, channels_num, tmp, width * channels_num);
}


int main()
{
    // Image parameters
    const unsigned int channels_num = 3;
    const unsigned int image_width = 256;
    const unsigned int image_height = 256;

    RGB pixels_data[image_width][image_height];

    // generate data
    get_gradient_rgb(image_width, image_height, pixels_data);

    // save to .ppm file
    if (!save_to_ppm((get_time_str() + "_image").c_str(), image_width, image_height, pixels_data))
    {
        std::cout << "Error writing to .ppm file" << std::endl;
    }

    save_to_jpg(image_width, image_height, pixels_data);
    return 0;
}