#ifndef SPHEREH
#define SPHEREH
#include "hitable.h"
#include "material.h"
#include "phongmaterial.h"

class sphere: public hitable {
	public:
		sphere(){}
		sphere(vec3 cen, float r, phongMaterial m) : center(cen), radius(r), material(m) {};
		virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
		vec3 center;
		float radius;
		phongMaterial material;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const{
	vec3 oc = r.origin() - center; //ponto do centro da esfera
	float a = dot(r.direction(), r.direction());
	float b = dot(oc,r.direction()); // 
	float c = dot(oc,oc) - radius*radius;
	float discriminant = b*b - a*c;//"DELTA" - - > talvez tenha um 4 aqui
	if (discriminant > 0.0) {
		float temp = (-b - sqrt (discriminant)) /a;
		if(temp<t_max && temp>t_min){
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.material = material;
			return true;
		}
		temp = (-b + sqrt(discriminant))/a;
		if(temp < t_max && temp > t_min){
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.material = material;
			return true;
		}
	}
	return false;
}
#endif
