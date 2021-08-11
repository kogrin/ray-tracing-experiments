#include <iostream>
#include <vector>

#include <fstream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

struct RGB
{
    int red;
    int green;
    int blue;
    RGB() = default;
};

vector<vector<RGB>> get_pixels(int height, int width)
{
    // define RGB data storage
    auto data = vector<vector<RGB>>(height, vector<RGB>(width));

    for (int j = height - 1; j >= 0; --j)
    {
        for (int i = 0; i < width; ++i)
        {
            auto r = double(i) / (width - 1);
            auto g = double(j) / (height - 1);
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            data[i][j].red = ir;
            data[i][j].green = ig;
            data[i][j].blue = ib;
        }
    }
    return data;
}

int main()
{
    // Image parameters
    const int image_width = 256;
    const int image_height = 256;

    // generate data
    auto pixels_data = get_pixels(image_height, image_width);
    
    // Render
    cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (auto row : pixels_data)
    {
        for (auto p : row)
        {
            cout << p.red << ' ' << p.green << ' ' << p.blue << '\n';
        }
    }
}