#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

/*
 * Class that stores a list of hittable objects
 */
class hittable_list : public hittable {
public:
	std::vector<shared_ptr<hittable>> objects;

	hittable_list()
	{
	}

	hittable_list(shared_ptr<hittable> object) 
	{ 
		add(object); 
	}

	void clear() 
	{ 
		objects.clear(); 
	}

	void add(shared_ptr<hittable> object) 
	{ 
		objects.push_back(object); 
	}

	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;
	
};


/*
 * Requires:
 *	Incoming ray, bounds for t, rec (hit_record) to be modified
 *
 * Effects:
 *	Returns true if an intersection occurs on any of the objects, writes the appropriate information in rec
 */
bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto& object : objects) {
		if (object->hit(r, t_min, closest_so_far, temp_rec)) { // Executes when an object gets hit
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}

#endif