#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>
#include "useful_utils.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h" // using to save PPM to JPG

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