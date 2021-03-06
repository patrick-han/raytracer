#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

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
		ray scattered; // The scattered ray from the intersection
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) // If hit occurs (and subsequently, a scatter)
		{
			return attenuation * ray_color(scattered, world, depth - 1);
		}
		return color(0, 0, 0);
	}

	// Background gradient
	vec3 unit_direction = unit_vector(r.direction()); // Normalize ray direction to -1 to 1 unit vector
	auto t = 0.5 * (unit_direction.y() + 1.0); // Blends between white and blue vertically since we use the y-component, scale to [0,1] [white, blue]
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); // blendedValue = (1-t)*startValue + t*endValue
}


hittable_list random_scene() 
{
	hittable_list world;

	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) 
	{
		for (int b = -11; b < 11; b++) 
		{
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9) 
			{
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) 
				{
					// Diffuse
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) 
				{
					// Metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else 
				{
					// Glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	return world;
}


int main() 
{

	// Image setup
	const auto aspect_ratio = 3.0 / 2.0;
	const int image_width = 1200;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 500;
	const int max_depth = 50;

	// Viewport setup
	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;;
	auto focal_length = 1.0; // Distance from eye/camera to the viewport/projection plane

	// World setup
	auto world = random_scene();

	// Camera Setup
	point3 lookfrom(13, 2, 3);
	point3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.1;

	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);


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