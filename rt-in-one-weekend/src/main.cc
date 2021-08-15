#include "color.h"
#include "ray.h"
#include "vec3.h"

#include "useful_utils.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h" // using to save PPM to JPG

#include <iostream>
#include <vector>

using std::vector;

color ray_color(const ray &r)
{
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
    // Image
    const int channels_num = 3;
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

    // Store data in std::vector
    int index = 0;
    vector<unsigned char> pixels_data(image_width * image_height * channels_num);

    // Render
    std::cout << "P3\n"
              << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);

            pixels_data[index++] = static_cast<int>(255.999 * pixel_color.x());
            pixels_data[index++] = static_cast<int>(255.999 * pixel_color.y());
            pixels_data[index++] = static_cast<int>(255.999 * pixel_color.z());
        }
    }

    // save to .ppm file
    save_to_ppm((get_time_str() + "_blue_grad").c_str(), image_width, image_height, pixels_data);
    // save to .jpg file
    std::cout << "Trying to write JPG data ........." << std::endl;
    stbi_write_jpg((get_time_str() + "_blue_grad.jpg").c_str(), image_width, image_height, channels_num, pixels_data.data(), image_width * channels_num);
    
    std::cerr << "\nDone.\n";
    return 0;
}