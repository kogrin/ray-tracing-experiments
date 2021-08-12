#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <numeric>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

typedef unsigned char channel_value_t;

struct RGB
{
    channel_value_t red;
    channel_value_t green;
    channel_value_t blue;
    RGB() = default;
};

typedef std::vector<std::vector<RGB>> matrix_t;

matrix_t get_gradient_rgb(int width, int height)
{
    // define RGB data storage
    auto data = matrix_t(height, std::vector<RGB>(width));

    for (int j = height - 1; j >= 0; --j)
    {
        for (int i = 0; i < width; ++i)
        {
            auto r = double(i) / (width - 1);
            auto g = double(j) / (height - 1);
            auto b = 0.25;

            data[i][j].red = static_cast<int>(255.999 * r);
            data[i][j].green = static_cast<int>(255.999 * g);
            data[i][j].blue = static_cast<int>(255.999 * b);
        }
    }

    return data;
}

bool save_to_ppm(std::string filename, int width, int height, const matrix_t &data)
{
    try
    {
        std::ofstream output(filename + ".ppm");
        output << "P3\n"
               << width << " " << height << "\n255\n";
        for (int j = height - 1; j >= 0; --j)
        {
            for (int i = 0; i < width; ++i)
            {
                output
                    << data[i][j].red << " "
                    << data[i][j].green << " "
                    << data[i][j].blue << std::endl;
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

int main()
{
    // Image parameters
    const int image_width = 256;
    const int image_height = 256;

    // generate data
    auto pixels_data = get_gradient_rgb(image_width, image_height);

    // save to .ppm file
    if (!save_to_ppm("image", image_width, image_height, pixels_data))
    {
        std::cout << "Error writing to .ppm file" << std::endl;
    }

    int data[image_width * image_height * 3];

    for (int j = image_height - 1; j >= 0; --j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            data[i][j] = pixels_data[i][j];
        }
    }

    // for (const auto i = 0; )
    // save to JPEG
    stbi_write_jpg("image.jpg", image_height, image_width, 3, pixels_data, 100);

    return 0;
}