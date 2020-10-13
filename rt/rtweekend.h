#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>


// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

/*
 * Requires:
 *	Degrees
 *
 * Effects:
 *	Returns the radians converted from the degrees
 */
inline double degrees_to_radians(double degrees) 
{
	return degrees * pi / 180.0;
}

/*
 * Requires:
 *	Nothing
 *
 * Effects:
 *	Returns a random real number [0, 1)
 */
inline double random_double()
{
	return rand() / (RAND_MAX + 1.0);
}

/*
 * Requires:
 *	Nothing
 *
 * Effects:
 *	Returns a random real number [min,max)
 */
inline double random_double(double min, double max)
{
	return min + (max - min) * random_double();
}



/*
 * Requires:
 *	A number x to be clamped, and the desired boundaries
 *
 * Effects:
 *	Returns x clamped to [min, max]
 */
inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}




// Common Headers
#include "ray.h"
#include "vec3.h"

#endif