#include <iostream>
#include "ray.h"
#include "vec3.h"
#include "color.h"

/*
 * Requires:
 *	Reference to ray being shot from camera
 *
 * Effects:
 *	Calculates and returns the color associated with the ray/pixel pair
 */
color ray_color(const ray& r) {
	vec3 unit_direction = unit_vector(r.direction()); // Normalize from -1 to 1
	auto t = 0.5 * (unit_direction.y() + 1.0); // Blends between white and blue vertically since we use the y-component, scale to [0,1] [white, blue]
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); // blendedValue = (1-5)*startValue + t*endValue
}

int main() {

	// Image aspect ratio and size
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);

	// Viewport setup
	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;
	auto focal_length = 1.0; // Distance from eye/camera to the viewport/projection plane

	// Camera setup, we traverse from the lower left hand corner
	auto origin = point3(0, 0, 0);
	auto horizontal = vec3(viewport_width, 0, 0);
	auto vertical = vec3(0, viewport_height, 0);
	auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);



	// Render

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// Pixels are written left to right, rows from top to bottom
	for (int j = image_height - 1; j >= 0; --j) {

		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush; // Progress bar

		for (int i = 0; i < image_width; ++i) {

			// Normalized pixel coordinates used as offsets below
			auto u = double(i) / (image_width - 1); 
			auto v = double(j) / (image_height - 1);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			color pixel_color = ray_color(r);
			write_color(std::cout, pixel_color);
		}
	}

	std::cerr << "\nDone.\n";
}