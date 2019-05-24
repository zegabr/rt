#ifndef MATERIALH
#define MATERIALH
#include <random>

struct hit_record;

float random_digit() { // windows não possui drand48(), então criei essa função,  mas a preferência é drand48().
	return (float(rand()) / float((RAND_MAX + 1.0))); // retorna float entre 0.0 e 1.0 (menos precisão que drand48())
}

#include "ray.h"
#include "hitable.h"
vec3 reflect(const vec3& v, const vec3& n){
	return v - 2*dot(v,n)*n;
}

float schlick(float cosine, float refraction_index) { // equação de refração de vidro de Schlick
	float r0 = (1-refraction_index) / (1+refraction_index);
	r0 = r0*r0;
	return r0 + (1-r0)*pow((1-cosine),5);
}

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
	if (discriminant > 0 ) {
		refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
		return true;
	} else {
		return false;
	}
}
vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		// p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
		p = 2.0*vec3(random_digit(), random_digit(), random_digit()); // utilizar em windows
	} while (p.squared_size() >= 1.0);
	return p;
}
class material{
	public:
		virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
		vec3 color;


};
class lambertian : public material{
	public:
		lambertian(const vec3& a) : color(a){}
		virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const{
			vec3 target = rec.p + rec.normal + random_in_unit_sphere();
			scattered = ray(rec.p, target-rec.p);
			attenuation = color;
			return true;
		}
		//cor agora está no material
		vec3 color; // troquei de albedo para color, representa melhor
};
class metal : public material {
	public:
		metal(const vec3& a, float f): color(a){if (f < 1) fuzz = f; else fuzz = 1; }
		virtual bool scatter(const ray& r_in,const hit_record& rec, vec3& attenuation, ray& scattered) const{
			vec3 reflected = reflect(unit_vector(r_in.direction()),rec.normal);
			scattered = ray(rec.p,reflected + fuzz*random_in_unit_sphere());
			attenuation = color;
			return (dot(scattered.direction(),rec.normal) > 0);
		}
		//cor agora está no material
		vec3 color; // troquei de albedo para color, representar melhor
		float fuzz; // nivel de "rigidez do metal"; quanto menor, mais ele reflete
};
class dieletric : public material { // exemplos são agua, vidro, diamantes, etc
	public:
		dieletric(float ri) : refraction_index(ri) {}
		virtual bool scatter(const ray& r_in,const hit_record& rec, vec3& attenuation, ray& scattered) const{
			vec3 outward_normal;
			vec3 reflected = reflect (r_in.direction(), rec.normal);
			float ni_over_nt; // lei de snell
			attenuation = vec3(1.0,1.0,1.0);
			vec3 refracted;
			float reflect_prob;
			float cosine;
			if(dot(r_in.direction(), rec.normal) > 0) {
				outward_normal = -rec.normal;
				ni_over_nt = refraction_index;
				cosine = refraction_index * dot(r_in.direction(), rec.normal) / r_in.direction().size();
			} else {
				outward_normal  = rec.normal;
				ni_over_nt = 1.0/refraction_index;
				cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().size();
			}
			if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
				reflect_prob = schlick(cosine, refraction_index);
			} else {
				scattered = ray(rec.p, reflected);
				reflect_prob = 1.0;
			}
			if(random_digit() < reflect_prob) {
				scattered = ray(rec.p, reflected);
			} else {
				scattered = ray(rec.p, refracted);
			}
			return true;
		}
		float refraction_index; // indice de refração de snell, recomendo ler sobre, tem no rt in a weekend
};
#endif
