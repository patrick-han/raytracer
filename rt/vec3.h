#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3
{
public:
	// The good stuff
	double e[3];

	// Constructors
	vec3() : e{ 0, 0 ,0 } 
	{
	};
	vec3(double e0, double e1, double e2) : e{ e0, e1, e2 }
	{
	}

	// Retrieve components
	double x() const
	{ 
		return e[0]; 
	}
	double y() const 
	{ 
		return e[1]; 
	}
	double z() const 
	{ 
		return e[2]; 
	}

	vec3 operator-() const // Negate
	{ 
		return vec3(-e[0], -e[1], -e[2]); 
	} 

	// Index access (should I use size_t for indices?)
	double operator[](int i) const // For const vec3, const coming after the function name means we can't change object members within this function
	{ 
		return e[i]; 
	} 
	double& operator[](int i) // For assignment, returns a reference, which is an lvalue, allowing the vec3 to be modified by index
	{ 
		return e[i]; 
	} 

	vec3& operator+=(const vec3 &v) 
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	vec3& operator*=(const double t) 
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	vec3& operator/=(const double t) 
	{
		return *this *= 1 / t;
	}

	double length() const // 2-norm
	{ 
		return sqrt(length_squared());
	}

	double length_squared() const 
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	// For generating random vectors [0, 1)
	inline static vec3 random() 
	{
		return vec3(random_double(), random_double(), random_double());
	}

	// Or with [min, max)
	inline static vec3 random(double min, double max) 
	{
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

};


// Type aliases for vec3
using point3 = vec3; // 3D point
using color = vec3; // RGB color


// vec3 Utility Functions

// << behavior for outputting vec3 to something like cout
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) 
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) 
{
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) 
{
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) 
{
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) 
{
	return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) 
{
	return t * v;
}

inline vec3 operator/(vec3 v, double t) 
{
	return (1 / t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) 
{
	return u.e[0] * v.e[0]
		 + u.e[1] * v.e[1]
		 + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) 
{
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		        u.e[2] * v.e[0] - u.e[0] * v.e[2],
		        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) 
{
	return v / v.length();
}


/*
* Requires:
*	Nothing
*
* Effects:
*	Generates and returns a point in the unit sphere via the rejection method
*/
inline vec3 random_in_unit_sphere() 
{
	while (true) 
	{
		auto p = vec3::random(-1, 1);
		if (p.length_squared() >= 1)
		{
			continue;
		}
		return p;
	}
}

/*
* Requires:
*	Nothing
*
* Effects:
*	Generates and returns a random unit vector
*/
inline vec3 random_unit_vector() 
{
	auto angle = random_double(0, 2 * pi);
	auto z = random_double(-1, 1);
	auto r = sqrt(1 - z * z);
	return vec3(r * cos(angle), r * sin(angle), z);
}

/*
* Requires:
*	Incident vector v and unit normal vector n
*
* Effects:
*	Calculates and returns the vector that gets reflected on the other side of the normal.
*   Since the normal vector is unit, this just uses scalar projection with a negation since the incident is coming in.
*/
inline vec3 reflect(const vec3& v, const vec3& n) 
{
	return v - 2 * dot(v, n) * n;
}

/*
* Requires:
*	Incident uv, normal vector n, and refractive index ratios given by the divided etas
*
* Effects:
*	Calculates and returns the refracted ray as vec3
*/
inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}

#endif