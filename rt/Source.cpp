#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"


#include <iostream>

/*
 * Requires:
 *	Ray being shot from the camera, sphere center coordinates and radius
 *
 * Effects:
 *	Returns the appropriate t-value that gives us the ray-sphere intersection
 */
double hit_sphere(const point3& center, double radius, const ray& r)
{
	vec3 oc = r.origin() - center; // A - C --> Ray origin - Center coords of the sphere
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


/*
 * Requires:
 *	Reference to ray being shot from camera
 *
 * Effects:
 *	Calculates and returns the color associated with the ray/pixel pair
 */
color ray_color(const ray& r, const hittable& world, int depth) 
{
	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
	{
		return color(0, 0, 0);
	}

	// Actual scene objects
	hit_record rec;

	if (world.hit(r, 0.001, infinity, rec))
	{
		// Diffuse, after intersecting object, generate a random ray from within a unit sphere 
		// tangent to the intersection point on the object surface.
		point3 target = rec.p + rec.normal + random_unit_vector();
		return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
	}

	// Background gradient
	vec3 unit_direction = unit_vector(r.direction()); // Normalize ray direction to -1 to 1 unit vector
	auto t = 0.5 * (unit_direction.y() + 1.0); // Blends between white and blue vertically since we use the y-component, scale to [0,1] [white, blue]
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); // blendedValue = (1-t)*startValue + t*endValue
}

int main() 
{

	// Image setup
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100;
	const int max_depth = 50;

	// Viewport setup
	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;;
	auto focal_length = 1.0; // Distance from eye/camera to the viewport/projection plane

	// World setup
	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	// Camera setup
	camera cam;


	// Render

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// Pixels are written left to right, rows from top to bottom
	for (int j = image_height - 1; j >= 0; --j) 
	{

		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush; // Progress bar

		for (int i = 0; i < image_width; ++i) 
		{
			color pixel_color(0, 0, 0);

			// Accumulate multiple rays per pixel (samples), which get averaged in write_color()
			for (int s = 0; s < samples_per_pixel; ++s) {
				// Approximately the middle of the pixel, and then some added randomness to sample
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}

	std::cerr << "\nDone.\n";
}