#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include "color.h"
#include "ray.h"
#include "vec3.h"

#include "useful_utils.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h" // using to save PPM to JPG

#include <iostream>

#include <vector>

using std::vector;

double hit_sphere(const point3 &center, double radius, const ray &r)
{
    // os is a vector from origin of the ray to center of the sphere
    vec3 oc = r.origin() - center;
    // rewrite sphere eq. in form of quadratic eq.
    //      (x - Center_x)^2 + (y - Center_y)^2 + (z - Center_z)^2 = r^2
    //      (P(t) - Center_xyz) * (P(t) - Center_xyz) = r^2;
    //          -- where `Center_xyz` is vector which points at 
    //             sphere center coordinates (x, y, z)
    //
    //      - note that:    P(t) := orig + t*dir
    //                      b := dir
    //      
    //      (orig + t*b - Center_xyz) * (orig + t*b - Center_xyz) = r^2
    //      ((orig - Center_xyz) + t*b) * ((orig - Center_xyz) + t*b) - r^2 = 0
    //      ((orig - Center_xyz)^2 - r^2) + 2*(orig - Center_xyz)*t*b + (t^2)*b^2  = 0
    //
    //      - a, b, c - coefficients of quadratic equation: a*t^2 + b^t + c,
    //          where
    //              a := b^2
    //              b := 2*(orig - Center_xyz)*b
    //              c := (orig - Center_xyz)^2 - r^2
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (-half_b - sqrt(discriminant) ) / a;
    }
}

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
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

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

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
            color pixel_color = ray_color(r, world);
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