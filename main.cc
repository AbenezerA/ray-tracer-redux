#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

color ray_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

int main() {
    auto aspect_ratio = 16.0 / 9.0; 
    int image_width = 400;

    // Get image height from aspect ratio, should be at least 1
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera
    auto focal_length = 1.0;
    auto camera_center = point3(0, 0, 0);
    // Viewport: virtual rectangle in the 3D world that contains grid of image pixel locations
    // Initialize height as 2, calculate width from ratio (not aspect ratio since that is only
    // an "ideal" ratio - muddied by int rounding and clamping to 1)
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);

    // Vectors across the viewport width and height
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + pixel_delta_u/2.0 + pixel_delta_v/2; 

    // Infra code telling file system to render output as image
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
    // Output to the logging stream instead of standard output; for progress indicator
    std::clog << "\rScanlines remaining: " << (image_height - j) << std::flush;
        for (int i = 0; i < image_width; i++) {
            // auto clr = color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0);
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_clr = ray_color(r);
            write_color(std::cout, pixel_clr);
        }
    }

    std::clog << "\rDone.                 \n";
}