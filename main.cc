#include "color.h"
#include "vec3.h"

#include <iostream>

int main() {
    auto aspect_ratio = 16.0 / 9.0; 
    int image_width = 400;

    // Get image height from aspect ratio, should be at least 1
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Viewport: virtual rectangle in the 3D world that contains grid of image pixel locations
    // Initialize height as 2, calculate width from ratio (not aspect ratio since that is only
    // an "ideal" ratio - muddied by int rounding and clamping to 1)
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);

    // Infra code telling file system to render output as image
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
    // Output to the logging stream instead of standard output; for progress indicator
    std::clog << "\rScanlines remaining: " << (image_height - j) << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto clr = color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0);
            write_color(std::cout, clr);
        }
    }

    std::clog << "\rDone.                 \n";
}