#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "useful_utils.h"

#include <iostream>
#include <vector>


RGB write_color_epta(std::ostream &out, color pixel_color)
{
    // @TODO: do not forget to refactor
    auto red    = static_cast<int>(255.999 * pixel_color.x());
    auto green  = static_cast<int>(255.999 * pixel_color.y());
    auto blue   = static_cast<int>(255.999 * pixel_color.z());

    out << red  << ' '
        << green << ' '
        << blue << '\n';

    return RGB(red, green, blue);
}


void write_color(std::ostream &out, color pixel_color)
{
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif