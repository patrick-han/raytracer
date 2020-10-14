#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "ray.h"

class material;

/* 
 * Struct that stores, for a given ray intersection, normal and direction information
 */
struct hit_record
{
	point3 p;
	vec3 normal;
	shared_ptr<material> mat_ptr;
	double t;
	bool front_face; // If true, ray is incoming from the outside

	// dot(a,b) = |a||b|cos(theta), so we can use dot() to determine if the ray and normal are "facing" the same direction
	// since dot(a,b) = 0 if theta = 90 degrees

	// Modify outward_normal so that it always points against the incoming ray, outside or inside
	inline void set_face_normal(const ray& r, const vec3& outward_normal) { 
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};


class hittable // Abstract class (contains a pure virutal function)
{
public:
	// Pure virtual function (set to 0), forces derived classes to provide an implementation
	virtual bool hit(const ray&r, double t_min, double t_max, hit_record& rec) const = 0;
};




#endif