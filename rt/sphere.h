#ifndef SPHERE_H
#define SPHERE_H

#include "rtweekend.h"

#include "hittable.h"


/*
 * Child class for a hittable sphere object
 */
class sphere : public hittable
{
public:
	point3 center;
	double radius;

	sphere()
	{
	}
	sphere(point3 cen, double r) : center(cen), radius(r)
	{
	};

	virtual bool hit(const ray&r, double t_min, double t_max, hit_record& rec) const override;

};

/*
 * Requires:
 *	Incoming ray, bounds for t, rec (hit_record) to be modified
 *
 * Effects:
 *	Returns true if an intersection occurs, writes the appropriate information in rec
 */
bool sphere::hit(const ray&r, double t_min, double t_max, hit_record& rec) const
{
	vec3 oc = r.origin() - center; // A - C
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;


	if (discriminant > 0)
	{
		auto root = sqrt(discriminant);
		auto temp = (-half_b - root) / a;
		if (temp < t_max && temp > t_min)
		{
			// Record the intersection's info + calculate normal
			rec.t = temp;
			rec.p = r.at(rec.t); // Point on the sphere
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			return true;
		}

		temp = (-half_b + root) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.at(rec.t);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			return true;
		}
	}

	return false;
}

#endif