#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

/*
 * Materials need to:
 * 1. Produce a scattered ray (or say it absorbed the incident ray
 * 2. If scattered, say how much attenuation should occur
 */

struct hit_record;

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};



/*
 * Lambertian material with specified attentuation
 */
class lambertian : public material {
public:
	color albedo;

	lambertian(const color& a) : albedo(a)
	{
	}

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override 
	{
		vec3 scatter_direction = rec.normal + random_unit_vector();
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}
};

/*
 * Metal material reflects rays based on reflect() in vec3.h with specified attenuation
 */
class metal : public material {
public:
	color albedo;
	double fuzz; // Fuzzy scattering parameter

	metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) 
	{
	}

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override 
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
};


/*
 * Dielectrics like water, glass, and diamonds split and reflect and refract the incident ray based on angle
 * of incidence and refractive indices of the materials
 */
class dielectric : public material {
public:
	double ir;

	dielectric(double index_of_refraction) : ir(index_of_refraction)
	{
	}

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		attenuation = color(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

		vec3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		vec3 direction;

		// Depending on the refraction ratio, the light might not be able to refract, and instead reflects
		// Uses Schlick's approximation as the reflection varies with the angle.
		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
		{
			direction = reflect(unit_direction, rec.normal);
		}
		else
		{
			direction = refract(unit_direction, rec.normal, refraction_ratio);
		}

		scattered = ray(rec.p, direction);
		return true;
	}
private:
	static double reflectance(double cosine, double ref_idx) 
	{
		// Use Schlick's approximation
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};

#endif
