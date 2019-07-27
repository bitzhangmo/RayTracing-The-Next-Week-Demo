#ifndef LANBERTIANH
#define LANBERTIANH

#include "material.h"
class lambertian :public material
{
public:
	lambertian(const vec3& a):albedo(a){}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const;
	vec3 albedo;
};

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		float random0 = rand() % (100) / (float)(100);
		float random1 = rand() % (100) / (float)(100);
		float random2 = rand() % (100) / (float)(100);
		p = 2.0*vec3(random0, random1, random2) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

bool lambertian::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
	vec3 target = rec.p + rec.normal + random_in_unit_sphere();
	scattered = ray(rec.p, target - rec.p);
	attenuation = albedo;
	return true;
}

#endif // !LANBERTIANH
