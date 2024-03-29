#ifndef _HITABLE_LIST_H_
#define _HITABLE_LIST_H_

#include "hitable.h"

class HitableList : public Hitable{
public:
	HitableList(){}
	HitableList(Hitable **l, int n) { 
		list = l; list_size = n; 
	}
	virtual bool hit(const Ray& r, double tmin, double tmax, hit_record& rec) const;
	Hitable **list;
	int list_size;
};

bool HitableList::hit(const Ray& r, double tmin, double tmax, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = tmax;
	for (int i = 0; i < list_size; i++) {
		if (list[i]->hit(r, tmin, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}


#endif // !HITABLE_LIST_H