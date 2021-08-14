// using stb_image
#include "stb_image_write.h"

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>


struct RGB
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    RGB() = default;
};


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
}