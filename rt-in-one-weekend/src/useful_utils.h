#ifndef USEFUL_UTILS_H
#define USEFUL_UTILS_H
#include <string>
#include <iostream> // used for "logging"
#include <fstream>  // used in `save_to_ppm(...)`
#include <ctime>    // used in `get_time_str()
// data structures used for storing pixels data
#include <vector>

// using these data structures:
using std::vector;

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
            auto r = double(i) / (width - 1);
            auto g = double(j) / (height - 1);
            auto b = 0.25;
            pixels[index++] = static_cast<unsigned int>(255.999 * r);
            pixels[index++] = static_cast<unsigned int>(255.999 * g);
            pixels[index++] = static_cast<unsigned int>(255.999 * b);
        }
    }

    return pixels;
}

// Get string with current year - month - time
std::string get_time_str()
{
    std::time_t t = std::time(0); // get time now
    std::tm *now = std::localtime(&t);

    auto str_timestamp =
        "_" + std::to_string(now->tm_year + 1900) +
        '_' + std::to_string(now->tm_mon + 1) +
        "_" + std::to_string(now->tm_mday) +
        "_" + std::to_string(now->tm_hour) +
        "_" + std::to_string(now->tm_min) +
        "_" + std::to_string(now->tm_sec);

    return str_timestamp;
}

void save_to_ppm(const std::string filename, const int width, const int height, vector<shared_ptr<unsigned char>> data, const int channels_num = 3)
{
    std::cout << "Trying to write PPM data ........." << std::endl;
    std::ofstream output(filename + ".ppm");
    output << "P3\n"
           << width << " " << height << "\n255\n";

    for (int index = 0; index < width * height * channels_num; index += 3)
    {
        output << (unsigned int) (*data[index]) << " "
               << (unsigned int) (*data[index + 1]) << " "
               << (unsigned int) (*data[index + 2]) << std::endl;
    }

    // close file
    output.close();
}
#endif