#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>
#include <vector>

using std::shared_ptr;
using std::make_shared;

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

void write_and_save_color(std::ostream &out, color pixel_color, int samples_per_pixel, std::vector<shared_ptr<unsigned char>> pixels_vector, int i)
{
    auto scaled_px_color = (1.0 / samples_per_pixel) * pixel_color;
    pixels_vector[i]    = make_shared<unsigned char>(static_cast<int>(255.999 * clamp(scaled_px_color.x(), 0.0, 0.999)));
    pixels_vector[i+1]  = make_shared<unsigned char>(static_cast<int>(255.999 * clamp(scaled_px_color.y(), 0.0, 0.999)));
    pixels_vector[i+2]  = make_shared<unsigned char>(static_cast<int>(255.999 * clamp(scaled_px_color.z(), 0.0, 0.999)));
}
#endif