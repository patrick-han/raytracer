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
		vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

		scattered = ray(rec.p, refracted);
		return true;
	}
};

#endif
