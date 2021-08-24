#include "rtweekend.h"
#include "color.h"
#include "camera.h"
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
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

color ray_color(const ray& r, const hittable& world, int depth)
{
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    if (world.hit(r, 0, infinity, rec))
    {
        point3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.95 * ray_color(ray(rec.p, target - rec.p), world, depth-1);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
    // Image
    const int channels_num = 3; // made by stupid

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // Camera
    camera cam;

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

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
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
            auto scaled_px_color = (1.0 / samples_per_pixel) * pixel_color;
            pixels_data[index++] = static_cast<int>(255.999 * clamp(scaled_px_color.x(), 0.0, 0.999));
            pixels_data[index++] = static_cast<int>(255.999 * clamp(scaled_px_color.y(), 0.0, 0.999));
            pixels_data[index++] = static_cast<int>(255.999 * clamp(scaled_px_color.z(), 0.0, 0.999));
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